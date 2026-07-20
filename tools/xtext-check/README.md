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
validation until the known hand-work is done — left recursion, left-factoring,
and (separately, not needed just to parse) assignments and cross-references.
That failure is the point: the generator becomes the worklist. Fix the top
error, re-run, repeat, until it emits a parser.

The classes of error to expect map onto the `pmk2xtext.py` report:
`factor`, `arithmetic` and the infix-operator rules will report left recursion
first.

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

Executed end-to-end. The toolchain initialises, parses `Alan.xtext`, and reaches
Xtext's grammar validation. As of the transliterator's terminal/empty-alternative
fixes, the first real diagnostic is left recursion (`The rule 'Declarations' is
left recursive.`) — i.e. the mechanical setup is done and what remains is the
grammar port itself (left-recursion elimination, then the expression cascade).
