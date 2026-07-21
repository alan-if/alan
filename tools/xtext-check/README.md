# xtext-check

A headless Maven harness that runs the Xtext generator over
`src/se/alanif/alan/Alan.xtext` (the grammar produced by `../pmk2xtext.py`).

Its purpose is **diagnostic**. Running the Xtext generator *is* the
grammar-validity check — there is no separate "validate" step. On an invalid
grammar you get two tiers of errors from one run:

1. **Xtext's own checks** — grammar syntax, unresolved rule/cross-reference
   targets, left recursion (reported per rule, e.g. *"The rule 'Factor' is left
   recursive."*), and feature/type problems. These carry `.xtext` line numbers.
2. **ANTLR3** (only if tier 1 passes) — genuine LL(\*) failures, e.g. *"rule X
   has non-LL(\*) decision … resolve by left-factoring or syntactic
   predicates."* Cryptic, and it names the generated rule, not the `.xtext` one.

## Requirements

- **Java 21** (Xtext 2.43.0 requires exactly this; `java -version` should say 21).
- **Maven** (`mvn`). Not currently installed on this machine — `apt install
  maven`, or via SDKMAN: `sdk install maven`.

## Run

```sh
cd tools/xtext-check
mvn -q generate-sources
```

First run downloads the Xtext/EMF/ANTLR artifacts from Maven Central (a few
minutes, once).

## Expectation on the first grammar run

`Alan.xtext` is a *mechanical transliteration*. It is expected to FAIL Xtext
validation until the remaining hand-work is done. The generator is the worklist:
fix the top error, re-run, repeat. The tiers, in the order Xtext surfaces them:

1. **Syntax + terminals** — DONE (converter maps `Identifier`→`ID`,
   `Integer`→`INT`, and renders empty alternatives as `(...)?`).
2. **Left recursion** — DONE (see below).
3. **Model** — CURRENT wall: the grammar has no assignments/actions, so no EMF
   metamodel is inferred (`Generated package 'alan' may not be empty`).
4. Not yet reached: left-factoring, then ANTLR3 LL(*) decisions (the `%+`/`%-`
   predicate sites), then cross-references and the expression precedence design.

## Isolating toolchain problems from grammar problems

If the first run fails in a way that looks like Maven/MWE2 misconfiguration
rather than a grammar error, confirm the toolchain independently by temporarily
replacing the body of `Alan.xtext` with a trivial valid grammar:

```xtext
grammar se.alanif.alan.Alan with org.eclipse.xtext.common.Terminals
generate alan "http://www.alanif.se/Alan"

Model:  greetings+=Greeting* ;
Greeting: 'Hello' name=ID '!' ;
```

That should generate cleanly. Once it does, the toolchain is proven; restore the
real grammar with `git checkout src/se/alanif/alan/Alan.xtext` and the remaining
errors are all grammar.

## Plain-java fallback (bypasses exec-maven-plugin)

If `mvn generate-sources` fails inside exec-maven-plugin's classloader rather
than in the grammar, run the MWE2 launcher directly:

```sh
mvn dependency:build-classpath -Dmdep.outputFile=cp.txt
java -cp "$(cat cp.txt)" \
     org.eclipse.emf.mwe2.launch.runtime.Mwe2Launcher \
     src/se/alanif/alan/GenerateAlan.mwe2 < /dev/null
```

Identical result, no exec plugin in the loop. `cp.txt` is git-ignored.

## Dependency notes (learned the hard way)

- exec-maven-plugin is pinned to **1.x**; 3.x's isolated classloader breaks
  MWE2's Guice reflection.
- MWE2 versions come from the imported `xtext-dev-bom` (2.26.0 for Xtext 2.43);
  do **not** hand-pin them — 2.15.0's POM is incomplete.
- `org.eclipse.xtext.common.types` and `org.eclipse.xtext.xbase` are declared
  explicitly because the `mwe2.language` POM under-declares them (it lists only
  `mwe2.runtime`), yet the `.mwe2` language needs them to parse the workflow.

## Status

Executed end-to-end (via `mvn generate-sources` and the plain-java fallback).
The toolchain initialises, parses `Alan.xtext`, and runs Xtext's grammar
validation.

**Left recursion is fully eliminated by the converter.** `pmk2xtext.py` applies
the classic transform `A = A a | b  ->  A = b (a)*` to all 25 directly
left-recursive rules, producing clean EBNF:

    Options: Option | Options Option          ->  Option+
    Declarations: | Declarations Declaration  ->  Declaration*
    IdList: Id | IdList ',' Id                ->  Id (',' Id)*
    Attributes: AttrDef '.' | ...             ->  (AttrDef '.')+
    Expression: Term | Expression 'or' Term   ->  Term ('or' Term)*
    Factor / Arithmetic (the cascade)         ->  b (a1 | a2 | ...)*

The transform is purely syntactic — it flattens the parse tree. For lists that
is exactly right; for the operator rules and especially `Factor`/`Arithmetic` it
discards associativity/precedence, which is restored later at the model tier via
rewrite actions (`{Binary.left=current}`), against the compiler's expression
test corpus.

The one INDIRECT (mutual) recursion the transform cannot reach —
`What <-> AttributeReference` — is handled by the `STRUCTURAL_OVERRIDES` table in
`pmk2xtext.py`: hand-authored-from-`alan.pmk` rule bodies, audit-flagged in the
generated grammar. `What` becomes `(SimpleWhat | Id 'of' What) (':' Id)*`,
faithful to compiler associativity (`':'` left-associative suffix, `'of'`
right-nested) rather than to `alanModelBuilder.atg`.

With all left recursion cleared, Xtext now advances to the **model tier**:
`Generated package 'alan' may not be empty` — the grammar recognises the
language but builds no EMF model, because the converter emits unassigned rule
calls. The next phase is adding assignments/actions (guided by the preserved
`-- RETURNS:` annotations), then the scoping/validation code.
