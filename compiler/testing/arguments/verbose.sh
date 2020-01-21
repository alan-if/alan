../../alan -verbose ok.alan | grep -v "Adventure Language Compiler" | tr -d '\b' | sed 's/: *[0-9][0-9 ]*$/: <number>/g' | sed 's/: *$/: <number>/g'
