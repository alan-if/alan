#!/usr/bin/env python
from sys import argv, exit
from os.path import basename
import argparse

from alangrapher_utils import compile_game_to_xml, get_locations, get_exits, dot_for_location_header, dot_for_exit


def usage():
    print(argv[0] + " - a program to extract location graph data in DOT format from an Alan game")
    print
    print("Usage: " + argv[0] + " <alan source file>")


def handle_args():
    global parser
    parser = argparse.ArgumentParser(description='Create a DOT graph from an Alan source file.')
    parser.add_argument('filename', help='the name of the Alan source file to create the graph from.')
    parser.add_argument('--ports', dest='use_ports', action="store_true",
                        help='use ports (tries to determine where to attach the outgoing arrows if directions are compass points)')
    if len(argv) == 1:
        usage()
        exit()
    return parser.parse_args()


def init_output():
    print('digraph {} {{'.format(basename(filename)))
    print('  rankdir=LR;')
    print('  node [shape=octagon;style=filled;]')


def terminate_output():
    print('}')


args = handle_args()

filename = args.filename

xmltree = compile_game_to_xml(filename)
location_list = get_locations(xmltree)

init_output()

for l in location_list:
    name = l.attributes['NAME'].value
    print("\n  {}".format(dot_for_location_header(l)))
    xs = get_exits(l)
    for x in xs:
        print("    {}".format(dot_for_exit(name, x)))

terminate_output()
