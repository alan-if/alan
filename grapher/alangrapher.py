#!/usr/bin/env python
from sys import argv, exit
from os.path import basename, splitext, split
from os import chdir
import argparse

import gtk
import gtk.gdk
import pygtk

import xdot

from alangrapher_utils import compile_game_to_xml, get_locations, get_exits, dot_for_location_header, dot_for_exit

IGNORE_LOCATIONS_TOOLTIP = "Locations listed here won't be include in the map. " \
                           "Separate location names with a space. " \
                           "This is handy for 'nowhere' and the like."

VERSION = "v0.3"


def handle_args():
    global parser
    parser = argparse.ArgumentParser(description='Create a graph over locations from an Alan source file.')
    parser.add_argument('filename', nargs="?", help='the name of the Alan source file to create the graph from.')
    parser.add_argument('--ignore-location', dest='ignore_list', action="append", metavar="LOCATION",
                        help="ignore the location with this name (handy for 'nowhere' etc.")
    parser.add_argument('--ignore-inherited', dest='inherited', action="store_false",
                        help="ignore any inherited exits")
    parser.add_argument('--version', action='version', version='%(prog)s {}'.format(VERSION))
    return parser.parse_args()


def init_output(gamename):
    return """
        digraph {} {{
                concentrate=true
                node [shape=octagon;style=filled;]
        """.format("gamename")


def terminate_output():
    return '}'



def main():
    ignore = []
    args = handle_args()
    if (args.filename is None):
        chooser = gtk.FileChooserDialog(title="AlanGrapher {} - Select Alan source file to create a map from".format(VERSION),
                                        action=gtk.FILE_CHOOSER_ACTION_OPEN,
                                        buttons=(gtk.STOCK_CANCEL,gtk.RESPONSE_CANCEL,gtk.STOCK_OPEN,gtk.RESPONSE_OK))
        filter = gtk.FileFilter()
        filter.set_name("Alan source files")
        filter.add_pattern("*.alan")

        chooser.add_filter(filter)

        ignore_box = gtk.HBox()

        ignore_label = gtk.Label(" Ignore locations named: ")
        ignore_label.set_tooltip_text(IGNORE_LOCATIONS_TOOLTIP)
        ignore_box.pack_start(ignore_label, expand=False, fill=False)
        ignore_label.show()

        ignore_text = gtk.Entry()
        ignore_text.set_tooltip_text(IGNORE_LOCATIONS_TOOLTIP)
        ignore_box.add(ignore_text)
        ignore_text.show()

        chooser.set_extra_widget(ignore_box)

        response = chooser.run()
        if response == gtk.RESPONSE_CANCEL:
            chooser.destroy()
            exit()

        filename = chooser.get_filename()

        filename = splitext(filename)[0]
        ignore_list = ignore_text.get_text().split()

        chooser.destroy()
    else:
        filename = args.filename
        ignore_list = args.ignore_list

    path, filename = split(filename)
    chdir(path)
    xmltree = compile_game_to_xml(filename)

    location_list = get_locations(xmltree, ignore_list)

    dotcode = init_output(basename(filename))

    for l in location_list:
        name = l.attributes['NAME'].value
        dotcode += "\n  {}".format(dot_for_location_header(l))
        xs = get_exits(l, ignore_list)
        for x in xs:
            dotcode += "    {}".format(dot_for_exit(name, x))

    dotcode += terminate_output()

    window = xdot.DotWindow()
    try:
        window.set_dotcode(dotcode)
        window.connect('destroy', gtk.main_quit)
        gtk.main()
    except Exception as e:
        dialog = gtk.MessageDialog(type=gtk.MESSAGE_ERROR, buttons=gtk.BUTTONS_OK,
            message_format="Could not draw graph!")
        dialog.set_title("Error!")
        dialog.format_secondary_text('Do you have Graphviz (http://graphviz.org/)\ninstalled and in the system PATH?')
        dialog.run()


if __name__ == '__main__':
    main()
