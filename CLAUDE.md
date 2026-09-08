# Working in this repository

## Where design reasoning lives

Design documentation is **not** in this repository. It was moved to
[alan-docs](https://github.com/alan-if/alan-docs) and is maintained there as
AsciiDoc — see `doc/design/MOVED.md`. The design notes are in
`alan-design/`, normally cloned alongside this repository as
`../alan-docs/alan-design/`.

Read the relevant note before changing a subsystem it covers. These notes
exist to record *why* the code is the way it is, and several of them
document invariants that look like sloppiness until you know the reason:

- `encoding.asciidoc` — character encodings. Read this before touching
  anything to do with UTF-8, ISO-8859-1, `iconv`, the `-encoding` and `-u`
  switches, column arithmetic or the `.a3c` header layout.
- `design.asciidoc` — general system design, compiler and interpreter.
- `rules.asciidoc`, `actors-in-containers.asciidoc` — specific topics.

When work produces a durable conclusion — an invariant, a constraint, a
"we checked and it does not work that way" — put it in the matching note in
`alan-docs` and commit it there. An issue is the wrong place for it: issues
close, and the reasoning goes with them.

## Issues

Work is tracked as issues on
[alan-if/alan](https://github.com/alan-if/alan/issues), organised by topic
labels such as `:star: encoding` and `:star: ALAN Lang`. The design notes
link to a label rather than to issue numbers, so the link stays valid as
issues open and close. When filing an issue in a topic that has a design
note, reference the note instead of restating its reasoning.
