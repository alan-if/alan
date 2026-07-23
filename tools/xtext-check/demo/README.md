# demo — drive the generated parser over real .alan files

Proves the generated Xtext parser parses real Alan adventures and builds the
outline model. Needs a JDK 21 (the system `javac` may be 17; Xtext 2.43 is
Java-21 bytecode). One is bundled with the VS Code Java extension, e.g.
`~/.vscode-server/extensions/redhat.java-*/jre/21.*/bin/javac`.

    cd ..                              # tools/xtext-check
    CP="$(cat cp.txt)"                 # regenerate cp.txt first if missing:
                                       #   mvn dependency:build-classpath -Dmdep.outputFile=cp.txt
    # 1. generate the language
    java -cp "$CP" org.eclipse.emf.mwe2.launch.runtime.Mwe2Launcher \
         src/se/alanif/alan/GenerateAlan.mwe2 < /dev/null
    # 2. compile generated code + a driver (JDK 21 javac)
    "$JDK21/bin/javac" -nowarn -d bin -cp "$CP" $(find src-gen src -name '*.java') demo/AlanRunner.java
    # 3. stage the serialized grammar + tokens as classpath resources
    (cd src-gen && find . \( -name '*.xtextbin' -o -name '*.tokens' \) -exec cp --parents {} ../bin/ \;)
    # 4. run
    java -cp "bin:$CP" AlanRunner <file>.alan          # one file, prints outline
    java -cp "bin:$CP" AlanBatch  <dir>                # many files, pass/fail rate

Case-insensitivity: the grammar is generated with `ignoreCase = true` (see the
parserGenerator block in GenerateAlan.mwe2), matching Alan's case-folding, so
mixed-case adventures (`Every`, `The`, `Start`) parse verbatim.
