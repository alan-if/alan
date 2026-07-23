#!/usr/bin/env python3
"""
pmk2xtext -- transliterate a ToolMaker/ParserMaker (.pmk) grammar into an
Xtext-shaped draft grammar, and report everything that needs hand work.

This is a SCAFFOLD GENERATOR, not a compiler. The emitted .xtext will NOT
parse as-is: LR left recursion, left-factoring and cross-references are left
for a human. What it does give you is a faithful, complete, mechanically
derived starting point plus a diagnostic report of every construct that
resists mechanical translation.

Usage:
    pmk2xtext.py alan.pmk [-o Alan.xtext] [--report report.md] [--lang se.alanif.alan.Alan]
"""

import argparse
import re
import sys
from collections import Counter, OrderedDict

# ---------------------------------------------------------------- lexing

TOKEN_RE = re.compile(r"""
      (?P<nonterm>   < [A-Za-z_][A-Za-z0-9_ ]* > )
    | (?P<literal>   ' (?: [^'] | '' )* ' )
    | (?P<directive> % [+-] \s* \( )
    | (?P<define>    = )
    | (?P<alt>       ! )
    | (?P<bar>       \| )
    | (?P<end>       ; )
    | (?P<lparen>    \( )
    | (?P<rparen>    \) )
    | (?P<lbrack>    \[ )
    | (?P<rbrack>    \] )
    | (?P<lbrace>    \{ )
    | (?P<rbrace>    \} )
    | (?P<comma>     , )
    | (?P<ident>     [A-Za-z_][A-Za-z0-9_]* )
""", re.VERBOSE)


class Tok:
    __slots__ = ("kind", "text", "line")

    def __init__(self, kind, text, line):
        self.kind, self.text, self.line = kind, text, line

    def __repr__(self):
        return f"{self.kind}({self.text!r})@{self.line}"


class Lexer:
    """Scans the %%RULES section, skipping anonymous %%...%% semantic actions
    and -- comments, while preserving %+/%- modification directives."""

    def __init__(self, src):
        self.src = src
        self.diag = {
            "actions": 0,
            "comments": 0,
            "returns": OrderedDict(),   # rule name -> RETURNS text
            "directives": [],           # (line, kind, symbols)
            "unknown": [],              # (line, char)
            "banner_returns": [],       # RETURNS on section-banner comments
            "section_markers": [],
        }

    def rules_section(self):
        """Slice out %%RULES .. next named section / %%END at top level."""
        m = re.search(r"^%%RULES\b", self.src, re.M)
        if not m:
            raise SystemExit("error: no %%RULES section found")
        start = m.end()
        nxt = re.search(r"^%%(?:[A-Z]+)\b", self.src[start:], re.M)
        end = start + nxt.start() if nxt else len(self.src)
        if nxt:
            self.diag["section_markers"].append(nxt.group(0))
        return self.src[start:end], self.src[:start].count("\n") + 1

    def tokens(self):
        body, line0 = self.rules_section()
        i, n, line = 0, len(body), line0
        toks = []
        # RETURNS annotations are TRAILING comments on the rule-head line, so
        # they must bind to the head already seen, not the next one.
        last_head = None            # (name, line) of most recent rule head

        while i < n:
            c = body[i]

            if c == "\n":
                line += 1
                i += 1
                continue
            if c in " \t\r\f":
                i += 1
                continue

            # ---- anonymous semantic action: %% ... %%
            if body.startswith("%%", i):
                # a named section marker would have been sliced off already
                j = body.find("%%", i + 2)
                if j == -1:
                    self.diag["unknown"].append((line, "unterminated %% action"))
                    break
                line += body.count("\n", i, j + 2)
                i = j + 2
                self.diag["actions"] += 1
                continue

            # ---- comment: -- to end of line (also carries RETURNS annotations)
            if body.startswith("--", i):
                j = body.find("\n", i)
                if j == -1:
                    j = n
                text = body[i:j]
                self.diag["comments"] += 1
                r = re.search(r"RETURNS:\s*(.+?)\s*$", text)
                if r:
                    if last_head and last_head[1] == line:
                        self.diag["returns"].setdefault(last_head[0], r.group(1))
                    else:
                        # e.g. "---> <SYNTAX>  -- RETURNS: stxs" section banners
                        self.diag["banner_returns"].append((line, text.strip()))
                i = j
                continue

            m = TOKEN_RE.match(body, i)
            if not m:
                self.diag["unknown"].append((line, c))
                i += 1
                continue

            kind = m.lastgroup
            text = m.group()

            # ---- modification directive %+( ... ) / %-( ... )
            if kind == "directive":
                j = body.find(")", i)
                if j == -1:
                    self.diag["unknown"].append((line, "unterminated directive"))
                    break
                inner = body[m.end():j]
                sign = "+" if "+" in text else "-"
                syms = [s.strip() for s in inner.split(",") if s.strip()]
                self.diag["directives"].append((line, sign, syms))
                toks.append(Tok("directive", sign + "(" + ",".join(syms) + ")", line))
                line += body.count("\n", i, j)
                i = j + 1
                continue

            if kind == "nonterm":
                text = text[1:-1].strip()

            # A rule head is a nonterminal OR a bare identifier (e.g. `ID = ...`)
            # sitting at the start of a rule, i.e. directly followed by '='.
            if kind in ("nonterm", "ident"):
                rest = body[m.end():]
                if re.match(r"(?:[ \t]|--[^\n]*\n|\n)*=(?!=)", rest):
                    prev = toks[-1].kind if toks else "end"
                    if prev in ("end", "define") or not toks:
                        kind = "nonterm"
                        last_head = (text, line)

            toks.append(Tok(kind, text, line))
            line += text.count("\n")
            i = m.end()

        return toks


# ---------------------------------------------------------------- parsing

class Rule:
    def __init__(self, name, line):
        self.name = name
        self.line = line
        self.alts = []          # list of list-of-items
        self.returns = None
        self.directives = []    # (sign, syms) collected in this rule


def parse(toks, diag):
    """rules := (NONTERM '=' alt ('!' alt)* ';')*"""
    rules, i, n = [], 0, len(toks)
    while i < n:
        if toks[i].kind != "nonterm":
            diag["unknown"].append((toks[i].line, f"expected rule head, got {toks[i]!r}"))
            i += 1
            continue
        rule = Rule(toks[i].text, toks[i].line)
        i += 1
        if i >= n or toks[i].kind != "define":
            diag["unknown"].append((rule.line, f"rule {rule.name}: expected '='"))
            continue
        i += 1
        cur = []
        while i < n and toks[i].kind != "end":
            t = toks[i]
            if t.kind == "alt":
                rule.alts.append(cur)
                cur = []
            elif t.kind == "directive":
                sign, syms = t.text[0], t.text[2:-1].split(",")
                rule.directives.append((sign, syms, t.line))
            else:
                cur.append(t)
            i += 1
        rule.alts.append(cur)
        if i < n:
            i += 1                      # consume ';'
        rules.append(rule)
    return rules


# ------------------------------------------------- emission: .prod (verify)

def emit_prod(rules):
    """Re-emit the parsed grammar in ParserMaker's `.prod` listing format.

    VERIFICATION ONLY. This desugars inline groups and optionals into
    __genSym#N helper rules, exactly as ParserMaker does, so the output can be
    diffed against a real alan.prod. The Rule model is NOT mutated -- the
    genSym expansion is local to this function and thrown away, so the Xtext
    emitter still sees, and keeps, the original sugar.

    Layout, reverse-engineered from alan.prod:
      * production number right-aligned in 3 columns then ". " (width 5)
      * '=', '!' and the closing ';' align one space past the rule head
      * each production line carries a single trailing space
      * genSym definitions follow the *complete* rule that uses them,
        numbered in order of first use
    """
    out = []
    n = [0]                     # production counter
    gs = [0]                    # next genSym id

    def render(items):
        """Render one alternative, hoisting sugar out into genSym rules.
        Returns (text, [(gsname, alts), ...])."""
        parts, hoisted = [], []
        i = 0
        while i < len(items):
            t = items[i]
            if t.kind in ("lbrack", "lparen"):
                close = "rbrack" if t.kind == "lbrack" else "rparen"
                depth, j = 1, i + 1
                while j < len(items) and depth:
                    if items[j].kind == t.kind:
                        depth += 1
                    elif items[j].kind == close:
                        depth -= 1
                    j += 1
                inner = items[i + 1:j - 1]
                name = f"__genSym#{gs[0]}"
                gs[0] += 1
                if t.kind == "lbrack":
                    alts = [[], inner]                 # [x]   -> empty ! x
                else:
                    alts, cur = [], []                 # (a|b) -> a ! b
                    for it in inner:
                        if it.kind == "bar":
                            alts.append(cur)
                            cur = []
                        else:
                            cur.append(it)
                    alts.append(cur)
                parts.append(name)
                hoisted.append((name, alts))
                i = j
                continue
            if t.kind == "literal":
                parts.append(t.text)
            elif t.kind == "nonterm":
                parts.append(f"<{t.text}>")
            elif t.kind == "ident":
                parts.append(t.text)
            elif t.kind == "comma":
                parts.append("','")
            i += 1
        return " ".join(parts), hoisted

    def emit_one(head, alts):
        col = 5 + len(head) + 1        # column of '=' / '!' / ';'
        hoisted_all = []
        for k, alt in enumerate(alts):
            n[0] += 1
            text, hoisted = render(alt)
            hoisted_all += hoisted
            num = f"{n[0]:3d}. "
            if k == 0:
                line = num + head + " = " + text
            else:
                line = num + " " * (col - len(num)) + "! " + text
            out.append(line.rstrip() + " ")
        out.append(" " * col + ";")
        return hoisted_all

    for r in rules:
        head = r.name if r.name in TERMINAL_LIKE_HEADS else f"<{r.name}>"
        hoisted = emit_one(head, r.alts)
        while hoisted:                  # genSym defs follow the whole rule
            name, alts = hoisted.pop(0)
            hoisted += emit_one(name, alts)

    return "\n".join(out)


# ---------------------------------------------------------------- emission

# Alan's raw lexer terminals -> the rules provided by org.eclipse.xtext.common.Terminals.
TERMINAL_MAP = {"Identifier": "ID", "Integer": "INT", "STRING": "STRING"}
TERMINAL_LIKE_HEADS = {"ID"}        # rule heads written without <angle brackets>

# Xtext rule names must be unique even case-insensitively, so Alan's *nonterminal*
# ID (the soft-keyword rule `ID = Identifier | 'location' | ...`) cannot be emitted
# as `Id` -- it would collide with the built-in ID terminal from common.Terminals.
# Rename it to AlanId; references resolve to our rule, not the terminal.
RULE_RENAMES = {"ID": "AlanId"}


def camel(name):
    return "".join(p.capitalize() for p in re.split(r"[_\s]+", name) if p)


def xtext_name(pmk_name):
    """Xtext rule name for a .pmk nonterminal, applying case-collision renames."""
    return RULE_RENAMES.get(pmk_name, camel(pmk_name))


def emit_items(items, i=0, stop=None):
    """Render a token run as Xtext, converting [x] -> (x)? and (a|b) -> (a|b)."""
    out = []
    while i < len(items):
        t = items[i]
        k = t.kind
        if stop and k == stop:
            return out, i
        if k == "literal":
            out.append("'" + t.text[1:-1].replace("''", "'") + "'")
        elif k == "nonterm":
            out.append(xtext_name(t.text))
        elif k == "ident":
            out.append(TERMINAL_MAP.get(t.text, xtext_name(t.text)))
        elif k == "lbrack":
            inner, i = emit_items(items, i + 1, "rbrack")
            out.append("(" + " ".join(inner) + ")?")
        elif k == "lparen":
            inner, i = emit_items(items, i + 1, "rparen")
            out.append("(" + " ".join(inner) + ")")
        elif k == "lbrace":
            inner, i = emit_items(items, i + 1, "rbrace")
            out.append("(" + " ".join(inner) + ")*")
        elif k == "bar":
            out.append("|")
        elif k in ("rbrack", "rparen", "rbrace"):
            return out, i
        elif k == "comma":
            out.append("','")
        i += 1
    return out, i


def left_recursive(rule):
    """direct left recursion: some alternative starts with the rule itself"""
    for a in rule.alts:
        if a and a[0].kind == "nonterm" and a[0].text == rule.name:
            return True
    return False


# Structural overrides: rules whose Xtext body cannot be mechanically derived from
# alan.pmk (indirect left recursion now; the expression precedence cascade later).
# Each is HAND-AUTHORED from alan.pmk (the compiler grammar is the source of truth)
# and must be audited against compiler behaviour -- NOT copied from
# alanModelBuilder.atg, whose expression/attribute handling diverges on associativity.
# Keyed by the .pmk rule name; the value is the final Xtext body.
STRUCTURAL_OVERRIDES = {
    # What <-> AttributeReference is a mutual (indirect) left recursion:
    #   What               = SimpleWhat | AttributeReference
    #   AttributeReference = Id 'of' What | What ':' Id
    # Inline AttributeReference into What, then eliminate the now-direct recursion:
    #   What = SimpleWhat | Id 'of' What | What ':' Id
    #        -> (SimpleWhat | Id 'of' What) (':' Id)*
    # Faithful to alan.pmk: ':' is a left-associative suffix, 'of' stays right-nested.
    # AttributeReference keeps its own definition (used standalone in assignments);
    # breaking the cycle here leaves it non-recursive.
    "what": "(SimpleWhat | AlanId 'of' What) (':' AlanId)*",
}

# ---------------------------------------------------------------------------
# SHALLOW OUTLINE MODEL (Era-1 seed). See the docstrings below for the WHY.
#
# The full outline model is Era-2 hand-authoring because Xtext typing cascades:
# a rule's alternatives must be all-EClass or all-datatype, and `Verb` appears in
# BOTH `Declaration` (top level) and `Property` (class bodies). Making Verb an
# outline node therefore drags the whole property/statement/expression subtree
# into the model. So this seed deliberately does NOT model verbs; it keeps Verb
# (and everything it reuses) as a datatype rule, and captures only the
# non-cascading top-level declarations as typed containment on Adventure.
#
# Result: a green generation whose model is a named, nested outline tree --
# essentially the current model/ package, minus top-level verbs. Those, plus
# nested attributes and cross-references, are the first Era-2 tasks.

# Rules bypassed entirely: the Declaration/Declarations spine is replaced by
# typed containment directly on Adventure (see MODEL_OVERRIDES['adventure']).
# Emitting them would reintroduce the mixed EClass/datatype union error.
MODEL_SKIP = {"declaration", "declarations"}

# Hand-authored shallow-model bodies, keyed by .pmk rule name. `name=` where the
# declaration has an identifier; a bare `{Type}` action where it does not (still
# an EClass, so it shows in the outline). Everything else is consumed unassigned.
MODEL_OVERRIDES = {
    # Adventure owns typed lists of the non-cascading declarations. Verb and
    # 'import' are consumed (Verb stays datatype to avoid the cascade); options
    # and start are consumed too.
    "adventure":
        "OptionalOptions\n"
        "      ( classes+=Class | instances+=Instance | additions+=Addition\n"
        "      | synonyms+=Synonyms | messages+=Messages | syntaxes+=Syntax\n"
        "      | events+=Event | rules+=Rule | prompts+=Prompt\n"
        "      | Verb | 'import' )*\n"
        "      Start",
    # Named declarations: capture the name, consume the rest.
    "class":    "'every' name=AlanId OptionalHeritage Properties ClassTail",
    "instance": "'the' name=AlanId OptionalHeritage Properties InstanceTail",
    "addition": "'add' 'to' ('every')? name=AlanId OptionalHeritage Properties AddTail",
    # Unnamed declarations: a bare action makes the EClass; body consumed.
    "synonyms": "{Synonyms} 'synonyms' SynonymList",
    "messages": "{Messages} 'message' MessageList",
    "syntax":   "{Syntax} 'syntax' SyntaxList",
    "event":    "{Event} EventHeader Statements EventTail",
    "rule":     "{Rule} 'when' Expression Then Statements OptionalEndWhen",
    "prompt":   "{Prompt} 'prompt' Statements",
}

# LL(*) fixes: Xtext runs ANTLR3 (LL(*)), which the LALR compiler's %+/%- directives
# do not translate to. These hand-authored bodies left-factor common prefixes or add
# Xtext syntactic predicates (=>) so ANTLR3 can decide. Faithful to alan.pmk's language.
LLSTAR_OVERRIDES = {
    # 'for' shadowed 'for' 'each' (dead alt). Factor the optional 'each'.
    "for_each": "'for' 'each'? | 'each'",
    # Three base alternatives shared the prefix Primary; left-factor it.
    "arithmetic": "(Primary ('isa' AlanId | Is Something)? | Aggregate Filters) "
                  "(Binop Primary)*",
    # Both check-forms start 'check'; factor it, and predicate the else-bearing CheckList.
    "optional_checks": "('check' (=>CheckList | Statements))?",
    # AlanId 'of' What | What ':' AlanId both start with AlanId. What already absorbs
    # both the 'of' and ':' forms, so delegate (over-accepts a bare SimpleWhat --
    # harmless for a recogniser; the semantic validator is Era-2).
    "attribute_reference": "What",
    # `say it ...`: 'it' is both an article here and a soft-keyword AlanId that can
    # start the Expression. alan.pmk's %-('it') on the empty alt resolves it greedily
    # (bind 'it' to the article); a syntactic predicate makes ANTLR3 do the same.
    "say_form": "(=> ('the' | 'an' | 'it' | 'no'))?",
}


def render_alt(alt):
    """Render a whole alternative (list of tokens) to an Xtext fragment."""
    return " ".join(emit_items(alt)[0])


def derecurse_body(rule):
    """Eliminate DIRECT left recursion by the classic transform.

    A rule  A = A a1 | A a2 | ... | b1 | b2 | ...  (ai = tails after the leading A,
    bj = non-left-recursive alternatives) is rewritten to  A = (b...) (a...)* .

    This is purely SYNTACTIC: it removes left recursion and accepts the same token
    language, but flattens the parse tree. For the list rules that is exactly right;
    for the operator rules (or/and) and especially Factor/Arithmetic it discards
    associativity/precedence, which is deferred to the model tier (hand-authored
    assignments/actions against the compiler's expression test corpus).

    Returns the body string, or None if the rule is not directly left recursive.
    """
    rec_tails, bases = [], []
    for a in rule.alts:
        if a and a[0].kind == "nonterm" and a[0].text == rule.name:
            rec_tails.append(a[1:])          # drop the leading self-reference
        else:
            bases.append(a)
    if not rec_tails:
        return None

    nonempty_bases = [render_alt(b) for b in bases if b]
    has_empty_base = any(not b for b in bases)
    tails = [render_alt(t) for t in rec_tails]

    def loop(items, op):
        if len(items) == 1:
            t = items[0]
            return f"{t}{op}" if " " not in t else f"({t}){op}"
        return "(" + " | ".join(items) + ")" + op

    # Pure list: a single tail equal to the (single/absent) base  ->  X+  or  X*
    if len(tails) == 1 and len(nonempty_bases) <= 1 and \
       (not nonempty_bases or (not has_empty_base and nonempty_bases[0] == tails[0])):
        return loop(tails, "+" if nonempty_bases else "*")

    # General:  base (tails)*
    if nonempty_bases:
        if len(nonempty_bases) == 1 and not has_empty_base:
            base = nonempty_bases[0]
        else:
            base = "(" + " | ".join(nonempty_bases) + ")" + ("?" if has_empty_base else "")
    else:
        base = ""                            # only an empty base; the trailing * covers it
    return (base + " " + loop(tails, "*")).strip()


def first_symbol(alt):
    if not alt:
        return None
    t = alt[0]
    return (t.kind, t.text)


def emit(rules, diag, langname):
    L = []
    ap = L.append
    ap(f"grammar {langname} with org.eclipse.xtext.common.Terminals")
    ap("")
    ap(f"generate {langname.split('.')[-1].lower()} "
       f"\"http://www.alanif.se/{langname.split('.')[-1]}\"")
    ap("")
    ap("// " + "=" * 74)
    ap("// DRAFT -- mechanically transliterated from ParserMaker .pmk")
    ap("// This grammar does NOT compile as-is. See the accompanying report.")
    ap("//   * left-recursive rules are marked  TODO(left-recursion)")
    ap("//   * conflict sites are marked        TODO(predicate)")
    ap("//   * no cross-references ([Rule|ID]) have been inferred")
    ap("// " + "=" * 74)
    ap("")

    by_name = {r.name: r for r in rules}
    for r in rules:
        if r.name in MODEL_SKIP:
            continue
        notes = []
        if r.name in diag["returns"]:
            notes.append(f"RETURNS: {diag['returns'][r.name]}")
        model = MODEL_OVERRIDES.get(r.name)
        llstar = LLSTAR_OVERRIDES.get(r.name)
        override = STRUCTURAL_OVERRIDES.get(r.name) or model or llstar
        lr_body = derecurse_body(r)
        if model is not None:
            notes.append("SHALLOW MODEL (Era-1 seed; outline node) -- audit me")
        elif llstar is not None:
            notes.append("LL(*) FIX (left-factor / syntactic predicate for ANTLR3) "
                         "-- audit me")
        elif override is not None:
            notes.append("STRUCTURAL OVERRIDE (hand-authored from alan.pmk; "
                         "breaks indirect left recursion) -- audit me")
        elif lr_body is not None:
            notes.append("de-left-recursed: A -> b (a)*  "
                         "(flattens tree; associativity/precedence deferred)")
        if r.directives:
            for sign, syms, ln in r.directives:
                notes.append(f"TODO(predicate) %{sign}({','.join(syms)}) @pmk:{ln}")
        # common-prefix detection (still relevant after de-recursion)
        firsts = [first_symbol(a) for a in r.alts if a]
        dupes = [s for s, c in Counter(firsts).items() if c > 1 and s]
        if dupes:
            notes.append("TODO(left-factor): " +
                         ", ".join(t for _, t in dupes))
        if lr_body is None and any(not a for a in r.alts):
            notes.append("has empty alternative -> optional at use site")

        for nt in notes:
            ap(f"// {nt}")

        head = xtext_name(r.name)
        ap(f"{head}:")
        if override is not None:
            ap("      " + override)
        elif lr_body is not None:
            ap("      " + lr_body)
        else:
            nonempty = [" ".join(emit_items(a)[0]) for a in r.alts if a]
            if len(nonempty) < len(r.alts):
                # Nullable rule: Xtext has no bare empty alternative (a `/* */` there
                # is lexed as whitespace, leaving a body that starts/ends with '|'),
                # so make the whole thing optional.
                if nonempty:
                    ap("      (" + " | ".join(nonempty) + ")?")
                else:
                    ap("      /* nullable: matches empty */ ")
            else:
                ap("      " + "\n    | ".join(nonempty))
        ap(";")
        ap("")
    return "\n".join(L)


# ---------------------------------------------------------------- report

def report(rules, diag, toks):
    L = []
    ap = L.append
    ap("# pmk2xtext diagnostic report\n")

    lrec = [r for r in rules if left_recursive(r)]
    lfact = []
    for r in rules:
        firsts = [first_symbol(a) for a in r.alts if a]
        if any(c > 1 for s, c in Counter(firsts).items() if s):
            lfact.append(r)
    empties = [r for r in rules if any(not a for a in r.alts)]

    ap("## Totals\n")
    ap(f"- rules parsed: **{len(rules)}**")
    ap(f"- alternatives: **{sum(len(r.alts) for r in rules)}**")
    ap(f"- semantic action blocks skipped: **{diag['actions']}**")
    ap(f"- comments seen: **{diag['comments']}**")
    ap(f"- `-- RETURNS:` annotations: **{len(diag['returns'])}**")
    ap(f"- modification directives: **{len(diag['directives'])}**")
    ap(f"- directly left-recursive rules: **{len(lrec)}**")
    ap(f"- rules needing left-factoring: **{len(lfact)}**")
    ap(f"- rules with an empty alternative: **{len(empties)}**")
    ap(f"- unrecognised constructs: **{len(diag['unknown'])}**")
    ap("")

    ap("## Modification directives (-> Xtext syntactic predicates)\n")
    ap("| pmk line | kind | lookahead symbols |")
    ap("|---|---|---|")
    for ln, sign, syms in diag["directives"]:
        meaning = "reduce-FOR" if sign == "+" else "reduce-NOT-for"
        ap(f"| {ln} | `%{sign}` ({meaning}) | `{', '.join(syms)}` |")
    ap("")

    ap("## Left-recursive rules\n")
    listish, binop, hard = [], [], []
    for r in lrec:
        rec = [a for a in r.alts
               if a and a[0].kind == "nonterm" and a[0].text == r.name]
        if len(r.alts) == 2 and len(rec) == 1 and len(rec[0]) == 2:
            # X = Y ! X Y            -- pure repetition
            listish.append(r)
        elif (len(r.alts) == 2 and len(rec) == 1 and len(rec[0]) == 3
              and rec[0][1].kind == "literal"):
            # X = Y ! X 'op' Y  where 'op' is a LITERAL.
            # Structurally identical to a separator list; the difference is
            # semantic, so this bucket still needs a human eye:
            #   <expression> 'or' <term>   -> true left-assoc operator
            #   <id_list> ',' <id>         -> separator list, (...)* is fine
            # A trailing-delimiter list (X = Y '.' ! X Y '.') has a NONTERMINAL
            # in slot 1 and is classified as list-idiom instead.
            binop.append((r, rec[0][1].text))
        else:
            hard.append(r)

    ap(f"**{len(listish)} list-idiom** (mechanical -> `(...)* `):\n")
    ap(", ".join(f"`{r.name}`" for r in listish) or "_none_")
    ap("")
    ap(f"**{len(binop)} infix-literal recursion -- NEEDS HUMAN REVIEW.** "
       "Structurally `X = Y | X 'op' Y`. If `'op'` is a real operator this is "
       "left-associative and needs `{Binary.left=current}` in Xtext; if it is "
       "just a separator, a plain `(...)* ` is correct. The tool cannot tell "
       "these apart:\n")
    for r, op in binop:
        hint = "separator?" if op in ("','", "';'") else "operator?"
        ap(f"- `{r.name}` (pmk:{r.line}) -- infix `{op}`  <-- {hint}")
    ap("")
    ap(f"**{len(hard)} structural** (manual rewrite -- precedence / mutual recursion):\n")
    for r in hard:
        ap(f"- `{r.name}` (pmk:{r.line}) -- {len(r.alts)} alternatives")
    ap("")

    ap("## Rules needing left-factoring\n")
    for r in lfact:
        firsts = Counter(first_symbol(a) for a in r.alts if a)
        d = [f"`{t}`" for (k, t), c in firsts.items() if c > 1]
        ap(f"- `{r.name}` (pmk:{r.line}) -- {len(r.alts)} alts, shared first: {', '.join(d)}")
    ap("")

    ap("## Rules with `-- RETURNS: <nothing>` (candidates to inline / not emit an EClass)\n")
    nothing = [k for k, v in diag["returns"].items() if "nothing" in v.lower()]
    ap(f"**{len(nothing)}** of {len(diag['returns'])} annotated rules return nothing:\n")
    ap(", ".join(f"`{k}`" for k in nothing) or "_none_")
    ap("")
    ap("Rules with no RETURNS annotation at all "
       f"(**{len(rules) - len(diag['returns'])}**) need a manual decision:\n")
    unannot = [r.name for r in rules if r.name not in diag["returns"]]
    ap(", ".join(f"`{n}`" for n in unannot[:40]) + (" ..." if len(unannot) > 40 else ""))
    ap("")

    if diag["unknown"]:
        ap("## Unrecognised constructs (converter gaps)\n")
        for ln, what in diag["unknown"][:60]:
            ap(f"- line {ln}: `{what}`")
        ap("")
    else:
        ap("## Unrecognised constructs\n\nNone. Every construct in `%%RULES` was recognised.\n")

    kinds = Counter(t.kind for t in toks)
    ap("## Token kinds seen in %%RULES\n")
    for k, c in kinds.most_common():
        ap(f"- `{k}`: {c}")
    return "\n".join(L)


# ---------------------------------------------------------------- main

def main():
    p = argparse.ArgumentParser(description=__doc__,
                                formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument("pmk")
    p.add_argument("-o", "--out", default=None, help="write Xtext draft here")
    p.add_argument("--report", default=None, help="write markdown report here")
    p.add_argument("--lang", default="se.alanif.alan.Alan", help="Xtext grammar name")
    p.add_argument("--emit", choices=("xtext", "prod"), default="xtext",
                   help="xtext (default) keeps inline groups; "
                        "prod desugars them to __genSym#N for round-trip "
                        "verification against a real alan.prod")
    a = p.parse_args()

    src = open(a.pmk, encoding="utf-8", errors="replace").read()
    lx = Lexer(src)
    toks = lx.tokens()
    rules = parse(toks, lx.diag)

    xt = emit_prod(rules) if a.emit == "prod" else emit(rules, lx.diag, a.lang)
    rp = report(rules, lx.diag, toks)

    if a.out:
        open(a.out, "w").write(xt)
        print(f"wrote {a.out} ({len(xt.splitlines())} lines)", file=sys.stderr)
    if a.report:
        open(a.report, "w").write(rp)
        print(f"wrote {a.report}", file=sys.stderr)
    if not a.out and not a.report:
        print(xt)


if __name__ == "__main__":
    main()
