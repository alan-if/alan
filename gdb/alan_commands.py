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
    "Print an Alan Compiler Symbol Node"

    def __init__(self, val):
        self.val = val

    def to_string(self):
        return None

    def children(self):
        kids = []
        for field in self.val.type.fields():
            if field.name != 'fields':
                kids.append((field.name, str(self.val[field.name])))
            else:
                if str(self.val['kind']) == 'CLASS_SYMBOL':
                    kids.append(('fields.entity', str(self.val[field.name]['entity'])))
                if str(self.val['kind']) == 'INSTANCE_SYMBOL':
                    kids.append(('fields.entity', str(self.val[field.name]['entity'])))
                if str(self.val['kind']) == 'VERB_SYMBOL':
                    kids.append(('fields.verb', str(self.val[field.name]['verb'])))
                if str(self.val['kind']) == 'PARAMETER_SYMBOL':
                    kids.append(('fields.parameter', str(self.val[field.name]['parameter'])))
                if str(self.val['kind']) == 'LOCAL_SYMBOL':
                    kids.append(('fields.local', str(self.val[field.name]['local'])))
        return kids

class ExpressionPrinter(object):
    "Print an Alan Compiler Expression Node"

    def __init__(self, val):
        self.val = val

    def to_string(self):
        return None

    def children(self):
        kids = []
        for field in self.val.type.fields():
            if field.name != 'fields':
                kids.append((field.name, str(self.val[field.name])))
            else:
                if str(self.val['kind']) == 'WHERE_EXPRESSION':
                    kids.append(('fields.whr', str(self.val[field.name]['whr'])))
                if str(self.val['kind']) == 'ATTRIBUTE_EXPRESSION':
                    kids.append(('fields.atr', str(self.val[field.name]['atr'])))
                if str(self.val['kind']) == 'BINARY_EXPRESSION':
                    kids.append(('fields.bin', str(self.val[field.name]['bin'])))
                if str(self.val['kind']) == 'INTEGER_EXRESSION':
                    kids.append(('fields.val', str(self.val[field.name]['val'])))
                if str(self.val['kind']) == 'STRING_EXRESSION':
                    kids.append(('fields.str', str(self.val[field.name]['str'])))
                if str(self.val['kind']) == 'SET_EXRESSION':
                    kids.append(('fields.set', str(self.val[field.name]['set'])))
                if str(self.val['kind']) == 'AGGREGATE_EXRESSION':
                    kids.append(('fields.agr', str(self.val[field.name]['agr'])))
                if str(self.val['kind']) == 'RANDOM_EXRESSION':
                    kids.append(('fields.rnd', str(self.val[field.name]['rnd'])))
                if str(self.val['kind']) == 'RANDOM_IN_EXRESSION':
                    kids.append(('fields.rin', str(self.val[field.name]['rin'])))
                if str(self.val['kind']) == 'WHAT_EXPRESSION':
                    kids.append(('fields.wht', str(self.val[field.name]['wht'])))
                if str(self.val['kind']) == 'BETWEEN_EXRESSION':
                    kids.append(('fields.btw', str(self.val[field.name]['btw'])))
                if str(self.val['kind']) == 'ISA_EXRESSION':
                    kids.append(('fields.isa', str(self.val[field.name]['isa'])))
        return kids

class StatementPrinter(object):
    "Print an Alan Compiler Statement Node"

    def __init__(self, val):
        self.val = val

    def to_string(self):
        return None

    def children(self):
        kids = []
        for field in self.val.type.fields():
            if field.name != 'fields':
                kids.append((field.name, str(self.val[field.name])))
            else:
                if str(self.val['kind']) == 'PRINT_STATEMENT':
                    kids.append(('fields.print', str(self.val[field.name]['print'])))
                if str(self.val['kind']) == 'STYLE_STATEMENT':
                    kids.append(('fields.style', str(self.val[field.name]['style'])))
                if str(self.val['kind']) == 'SCORE_STATEMENT':
                    kids.append(('fields.score', str(self.val[field.name]['score'])))
                if str(self.val['kind']) == 'VISITS_STATEMENT':
                    kids.append(('fields.visits', str(self.val[field.name]['visits'])))
                if str(self.val['kind']) == 'DESCRIBE_STATEMENT':
                    kids.append(('fields.describe', str(self.val[field.name]['describe'])))
                if str(self.val['kind']) == 'SAY_STATEMENT':
                    kids.append(('fields.say', str(self.val[field.name]['say'])))
                if str(self.val['kind']) == 'LIST_STATEMENT':
                    kids.append(('fields.list', str(self.val[field.name]['list'])))
                if str(self.val['kind']) == 'SHOW_STATEMENT':
                    kids.append(('fields.show', str(self.val[field.name]['show'])))
                if str(self.val['kind']) == 'PLAY_STATEMENT':
                    kids.append(('fields.play', str(self.val[field.name]['play'])))
                if str(self.val['kind']) == 'EMPTY_STATEMENT':
                    kids.append(('fields.empty', str(self.val[field.name]['empty'])))
                if str(self.val['kind']) == 'LOCATE_STATEMENT':
                    kids.append(('fields.locate', str(self.val[field.name]['locate'])))
                if str(self.val['kind']) == 'INCLUDE_STATEMENT':
                    kids.append(('fields.include', str(self.val[field.name]['include'])))
                if str(self.val['kind']) == 'EXCLUDE_STATEMENT':
                    kids.append(('fields.include', str(self.val[field.name]['include'])))
                if str(self.val['kind']) == 'MAKE_STATEMENT':
                    kids.append(('fields.make', str(self.val[field.name]['make'])))
                if str(self.val['kind']) == 'SET_STATEMENT':
                    kids.append(('fields.set', str(self.val[field.name]['set'])))
                if str(self.val['kind']) == 'INCREASE_STATEMENT':
                    kids.append(('fields.incr', str(self.val[field.name]['incr'])))
                if str(self.val['kind']) == 'DECREASE_STATEMENT':
                    kids.append(('fields.incr', str(self.val[field.name]['incr'])))
                if str(self.val['kind']) == 'SCHEDULE_STATEMENT':
                    kids.append(('fields.schedule', str(self.val[field.name]['schedule'])))
                if str(self.val['kind']) == 'CANCEL_STATEMENT':
                    kids.append(('fields.cancel', str(self.val[field.name]['cancel'])))
                if str(self.val['kind']) == 'IF_STATEMENT':
                    kids.append(('fields.iff', str(self.val[field.name]['iff'])))
                if str(self.val['kind']) == 'USE_STATEMENT':
                    kids.append(('fields.use', str(self.val[field.name]['use'])))
                if str(self.val['kind']) == 'STOP_STATEMENT':
                    kids.append(('fields.stop', str(self.val[field.name]['stop'])))
                if str(self.val['kind']) == 'SYSTEM_STATEMENT':
                    kids.append(('fields.system', str(self.val[field.name]['system'])))
                if str(self.val['kind']) == 'DEPEND_STATEMENT':
                    kids.append(('fields.depend', str(self.val[field.name]['depend'])))
                if str(self.val['kind']) == 'DEPENDCASE_STATEMENT':
                    kids.append(('fields.depcase', str(self.val[field.name]['depcase'])))
                if str(self.val['kind']) == 'EACH_STATEMENT':
                    kids.append(('fields.each', str(self.val[field.name]['each'])))
                if str(self.val['kind']) == 'STRIP_STATEMENT':
                    kids.append(('fields.strip', str(self.val[field.name]['strip'])))
                if str(self.val['kind']) == 'TRANSCRIPT_STATEMENT':
                    kids.append(('fields.transcript', str(self.val[field.name]['transcript'])))
        return kids

class SrcpPrinter(object):
    "Print an Alan Compiler Srcp struct"

    def __init__(self, val):
        self.val = val

    def to_string(self):
        return "{%d:%d:%d %d-%d}" % (self.val['file'], self.val['line'], self.val['col'], self.val['startpos'], self.val['endpos'])

    
import gdb.printing

def build_pretty_printers():
    pp = gdb.printing.RegexpCollectionPrettyPrinter("alanCommands")
    pp.add_printer('Symbol', '^Symbol$', SymbolPrinter)
    pp.add_printer('Expression', '^Expression$', ExpressionPrinter)
    pp.add_printer('Statement', '^Statement$', StatementPrinter)
    pp.add_printer('Srcp', '^Srcp$', SrcpPrinter)
    return pp

gdb.printing.register_pretty_printer(gdb.current_objfile(), build_pretty_printers(), replace=True)
