#! /usr/bin/bash -i
../../alan -summary ok.alan |  sed 's/ *-\?[0-9]\+/ <number>/g'
