#!/usr/bin/env python
from sys import argv, exit
from os.path import basename
from subprocess import check_call, call, Popen, PIPE
import argparse

from dumpreader import DumpReader

def usage():
    print argv[0] + " - a program to extract location graph data from an Alan game"
    print
    print "Usage: " + argv[0] + " <alan source file>"

def compile_game_to_dump_format():
    global process, output, lines, reader
    process = Popen(["alan", "-dump", "ci", filename], stdout=PIPE)
    output = process.communicate(None)[0]
    lines = ''.join(output).split("\n")
    reader = DumpReader(lines)

def init_output():
    print 'digraph ' + basename(filename) + ' {'
    print '  rankdir=LR;'
    print '  node [shape=octagon;style=filled;]'


# Mapping from possible exit directions to DOT port directions
portname = {"n":":n", "north":":n",
            "s":":s", "south":":s",
            "e":":e", "east":":e",
            "w":":w", "west":":w",
            "se":":se", "southeast":":se",
            "sw":":sw", "southwest":":sw",
            "ne":":ne", "northeast":":ne",
            "nw":":nw", "northwest":":nw"
}

def read_next_location():
    global line
    while 1 :
        while line.find("INS:") == -1 :
            line = reader.next()
        while line.find("id:") == -1 :
            line = reader.next()
        id = line.split()[7].split(":")[0].strip('"')
        line = reader.next()

        # Found an instance. Is it a direct descendant of "location"?
        # We can't handle indirect descendants yet
        while line.find("parentId:") == -1 :
            line = reader.next()
        if line.find("location") == -1 :
            continue;
        return id

def read_directions() :
    global line
    directions = []
    line = reader.next()
    while line.find("DIRECTION") == -1 and line.find("INS:") == -1 :
        line = reader.next()
    if line.find("INS:") != -1 :
        # No, found next instances, so start over
        return None
    # 9th field and then remove the part after colon
    while line.find("DIRECTION") != -1 :
        direction = line.split()[11].split(":")[0].strip('"')
        directions.append(direction)
        line = reader.next()
    return directions

def read_target() :
    global line
    target = line.strip("\n\r")
    line = reader.next()
    return target.split()[11].strip().split(":")[0].strip('"')

parser = argparse.ArgumentParser(description='Create a DOT graph from an Alan source file.')
parser.add_argument('filename', help='the name of the Alan source file to create the graph from.')
parser.add_argument('--ports', dest='use_ports', action="store_true", help='use ports (tries to determine where to attach the outgoing arrows if directions are compass points)')

args = parser.parse_args()

if len(argv) == 1 :
    usage()
    exit()

filename = args.filename
compile_game_to_dump_format()
line = reader.next()
message = line

init_output()

try :
    while 1 :
        id = read_next_location()

        # Print the node
        print '  ' + id + '[label="' + id + '"];'

        while 1 :
            # Read all exits, start with directions
            directions = read_directions()
            if directions is None :
                break

            target = read_target()

            # Print all outgoing connections
            for direction in directions :
                if args.use_ports :
                    try :
                        port = portname[direction]
                    except KeyError :
                        port = ""
                else :
                    port = ""
                print "  " + id + port + " -> " + target + " [label=" + direction + "];"

except StopIteration as x :
    print "}"
    pass

