../../alan -verbose ok.alan | grep -v "Adventure Language Compiler" | sed 's/[0-9 ][\x08 0-9]\+$/ <number>/g'
