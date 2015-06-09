from __future__ import with_statement
import gdb

class AlanPrefixCommand(gdb.Command):
    "Prefix to do clever alan gdb commands"

    def __init__(self):
        super(AlanPrefixCommand, self).__init__("alan",
                                                gdb.COMMAND_SUPPORT,
                                                gdb.COMPLETE_NONE,
                                                True)

AlanPrefixCommand()

class AlanSymbolsCommand(gdb.Command):
    "Print the symbols in the Alan compiler symbol table"

    def __init__(self):
        super(AlanSymbolsCommand, self).__init__("alan symbols",
                                                 gdb.COMMAND_SUPPORT,
                                                 gdb.COMPLETE_NONE)

    def invoke(self, arg, from_tty):
        (symbol, found) = gdb.lookup_symbol("symbolTree")
        if symbol is not None:
            print("symbolTree = {} ({})".format(symbol.value(), symbol.type))
        else:
            print("Could not find symbolTree")

    def printSymbols(self, symbol):
        if symbol:
            print("{}: ")

AlanSymbolsCommand()

class SymbolPrinter(object):
    "Print an Alan Symbol"

    def __init__(self, val):
        self.val = val

    def to_string(self):
        return self.val

import gdb.printing

def build_pretty_printer():
    pp = gdb.printing.RegexpCollectionPrettyPrinter("alanCommands")
    pp.add_printer('Symbol', '^Symbol$', SymbolPrinter)
    return pp

gdb.printing.register_pretty_printer(gdb.current_objfile(), build_pretty_printer())
