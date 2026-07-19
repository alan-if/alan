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

## Note

This harness was assembled from Xtext's documented headless (Maven Central)
build plus a known-good reference `.mwe2`, but it has **not** been executed here
(no Maven on the build machine at authoring time). The first real run may need a
small config adjustment; keep that separate from the grammar errors above.
