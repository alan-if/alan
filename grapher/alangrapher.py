#!/usr/bin/env python
from sys import argv, exit
from os.path import basename
import argparse

import gtk
import gtk.gdk

import xdot

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


def init_output(filename):
    return """
        digraph {} {{
                concentrate=true
                node [shape=octagon;style=filled;]
        """.format(basename(filename))


def terminate_output():
    return '}'



def main():
    args = handle_args()

    filename = args.filename

    xmltree = compile_game_to_xml(filename)
    location_list = get_locations(xmltree)

    dotcode = init_output(filename)

    for l in location_list:
        name = l.attributes['NAME'].value
        dotcode += "\n  {}".format(dot_for_location_header(l))
        xs = get_exits(l)
        for x in xs:
            dotcode += "    {}".format(dot_for_exit(name, x))

    dotcode += terminate_output()

    window = xdot.DotWindow()
    window.set_dotcode(dotcode)
    window.connect('destroy', gtk.main_quit)
    gtk.main()


if __name__ == '__main__':
    main()