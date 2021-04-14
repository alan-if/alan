# Some extra commands for debugging Alan and Arun with gdb
#
# Load in gdb using: source <this file>
#
# help alan
# help arun
#

from __future__ import with_statement
import gdb

#
# Alan commands
#

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

def select_field(self, field_name, field, fields):
    if field.name != field_name:
        return ((field.name, str(self.val[field.name])))
    else:
        kind = str(self.val['kind'])
        if kind in fields:
            return ((field_name+'.'+fields[kind], str(self.val[field.name][fields[kind]])))
        else:
            return None

def select_fields(self, field_name, fields):
    kids = []
    for field in self.val.type.fields():
        kids.append(select_field(self, field_name, field, fields))
    return kids


class SymbolPrinter(object):
    "Print an Alan Compiler Symbol Node"

    def __init__(self, val):
        self.val = val

    def to_string(self):
        return None

    def children(self):
        symbol_fields = {
            'CLASS_SYMBOL':'entity',
            'INSTANCE_SYMBOL':'entity',
            'VERB_SYMBOL':'verb',
            'PARAMETER_SYMBOL':'parameter',
            'LOCAL_SYMBOL':'local'
        }

        return select_fields(self, 'fields', symbol_fields)


class ExpressionPrinter(object):
    "Print an Alan Compiler Expression Node"

    def __init__(self, val):
        self.val = val

    def to_string(self):
        return None

    def children(self):
        expression_fields = {
            'WHERE_EXPRESSION':'whr',
            'ATTRIBUTE_EXPRESSION':'atr',
            'BINARY_EXPRESSION':'bin',
            'INTEGER_EXRESSION':'val',
            'STRING_EXRESSION':'str',
            'SET_EXRESSION':'set',
            'AGGREGATE_EXRESSION':'agr',
            'RANDOM_EXRESSION':'rnd',
            'RANDOM_IN_EXRESSION':'rin',
            'WHAT_EXPRESSION':'wht',
            'BETWEEN_EXRESSION':'btw',
            'ISA_EXRESSION':'isa'
        }

        return select_fields(self, 'fields', expression_fields)


class StatementPrinter(object):
    "Print an Alan Compiler Statement Node"

    def __init__(self, val):
        self.val = val

    def to_string(self):
        return None

    def children(self):
        statement_fields = {
            'PRINT_STATEMENT':'print',
            'STYLE_STATEMENT':'style',
            'SCORE_STATEMENT':'score',
            'VISITS_STATEMENT':'visits',
            'DESCRIBE_STATEMENT':'describe',
            'SAY_STATEMENT':'say',
            'LIST_STATEMENT':'list',
            'SHOW_STATEMENT':'show',
            'PLAY_STATEMENT':'play',
            'EMPTY_STATEMENT':'empty',
            'LOCATE_STATEMENT':'locate',
            'INCLUDE_STATEMENT':'include',
            'EXCLUDE_STATEMENT':'include',
            'MAKE_STATEMENT':'make',
            'SET_STATEMENT':'set',
            'INCREASE_STATEMENT':'incr',
            'DECREASE_STATEMENT':'incr',
            'SCHEDULE_STATEMENT':'schedule',
            'CANCEL_STATEMENT':'cancel',
            'IF_STATEMENT':'iff',
            'USE_STATEMENT':'use',
            'STOP_STATEMENT':'stop',
            'SYSTEM_STATEMENT':'system',
            'DEPEND_STATEMENT':'depend',
            'DEPENDCASE_STATEMENT':'depcase',
            'EACH_STATEMENT':'each',
            'STRIP_STATEMENT':'strip',
            'TRANSCRIPT_STATEMENT':'transcript'
        }

        return select_fields(self, 'fields', statement_fields)


class ListPrinter(object):
    "Print an Alan Compiler List Node"

    def __init__(self, val):
        self.val = val

    def to_string(self):
        return None

    def children(self):
        list_fields = {
            'ADD_LIST':'add',
            'ALTERNATIVE_LIST':'alt',
            'ATTRIBUTE_LIST':'atr',
            'CASE_LIST':'stm',
            'CHECK_LIST':'chk',
            'CLASS_LIST':'cla',
            'CONTAINER_LIST':'cnt',
            'ELEMENT_LIST':'elm',
            'EVENT_LIST':'evt',
            'EXIT_LIST':'ext',
            'EXPRESSION_LIST':'exp',
            'ID_LIST':'id',
            'INSTANCE_LIST':'ins',
            'LIMIT_LIST':'lim',
            'ELEMENT_ENTRIES_LIST':'eent',
            'LIST_LIST':'lst',
            'MESSAGE_LIST':'msg',
            'NAME_LIST':'',
            'REFERENCE_LIST':'word',
            'RESTRICTION_LIST':'res',
            'RESOURCE_LIST':'resource',
            'RULE_LIST':'rul',
            'SCRIPT_LIST':'script',
            'STATEMENT_LIST':'stm',
            'STEP_LIST':'stp',
            'STRING_LIST':'str',
            'SYMBOL_LIST':'sym',
            'SYNONYM_LIST':'syn',
            'SYNTAX_LIST':'stx',
            'SRCP_LIST':'srcp',
            'VERB_LIST':'vrb',
            'WORD_LIST':'word',
            'IFID_LIST':'ifid'
        }

        return select_fields(self, 'member', list_fields)

class SrcpPrinter(object):
    "Print an Alan Compiler Srcp struct"

    def __init__(self, val):
        self.val = val

    def to_string(self):
        return "{%d:%d:%d %d-%d}" % (self.val['file'], self.val['line'], self.val['col'], self.val['startpos'], self.val['endpos'])

WORD_CLASS_NAME = [
    "Synonym",
    "Adjective",
    "All",
    "Except",
    "Conjunction",
    "Preposition",
    "Direction",
    "It",
    "Noise",
    "Noun",
    "Them",
    "Verb",
    "Pronoun"
    ]

# Bit 0 = 1
SYNONYM_WORD = 0
SYNONYM_BIT = (1<<SYNONYM_WORD)

# Bit 1 = 2
ADJECTIVE_WORD = (SYNONYM_WORD+1)
ADJECTIVE_BIT = (1<<ADJECTIVE_WORD)

# Bit 2 = 4
ALL_WORD = (ADJECTIVE_WORD+1)
ALL_BIT = (1<<ALL_WORD)

# Bit 3 = 8
EXCEPT_WORD = (ALL_WORD+1)
EXCEPT_BIT = (1<<EXCEPT_WORD)

# Bit 4 = 16
CONJUNCTION_WORD = (EXCEPT_WORD+1)
CONJUNCTION_BIT = (1<<CONJUNCTION_WORD)

# Bit 5 = 32
PREPOSITION_WORD = (CONJUNCTION_WORD+1)
PREPOSITION_BIT = (1<<PREPOSITION_WORD)

# Bit 6 = 64
DIRECTION_WORD = (PREPOSITION_WORD+1)
DIRECTION_BIT = (1<<DIRECTION_WORD)

# Bit 7 = 128
IT_WORD = (DIRECTION_WORD+1)
IT_BIT = (1<<IT_WORD)

# Bit 8 = 256
NOISE_WORD = (IT_WORD+1)
NOISE_BIT = (1<<NOISE_WORD)

# Bit 9 = 512
NOUN_WORD = (NOISE_WORD+1)
NOUN_BIT = (1<<NOUN_WORD)

# Bit 10 = 1024
THEM_WORD = (NOUN_WORD+1)
THEM_BIT = (1<<THEM_WORD)

# Bit 11 = 2048
VERB_WORD = (THEM_WORD+1)
VERB_BIT = (1<<VERB_WORD)

# Bit 12 = 4096
PRONOUN_WORD = (VERB_WORD+1)
PRONOUN_BIT = ((1)<<PRONOUN_WORD)

WORD_CLASS_COUNT = (PRONOUN_WORD+1)



def decodeWordBits(bits):
    # Need to be synchronized with interpreter/acode.h
    classes = "{"
    if bits&SYNONYM_BIT: classes += "Synonym "
    if bits&ADJECTIVE_BIT: classes += "Adjective "
    if bits&ALL_BIT: classes += "All "
    if bits&EXCEPT_BIT: classes += "Except "
    if bits&CONJUNCTION_BIT: classes += "Conjunction "
    if bits&PREPOSITION_BIT: classes += "Preposition "
    if bits&DIRECTION_BIT: classes += "Direction "
    if bits&IT_BIT: classes += "It "
    if bits&NOISE_BIT: classes += "Noise "
    if bits&NOUN_BIT: classes += "Noun "
    if bits&THEM_BIT: classes += "Them "
    if bits&VERB_BIT: classes += "Verb "
    if bits&PRONOUN_BIT: classes += "Pronoun "
    return classes+"}"

class WordPrinter(object):
    "Print an Alan Compiler Word struct"

    def __init__(self, val):
        self.val = val

    def to_string(self):
        r = "classbits = %s, code = %d, string = %s\n" % (decodeWordBits(self.val['classbits']), self.val['code'], self.val['string'])
        for i in range(WORD_CLASS_COUNT):
            r += "[%s] = %s\n" % (WORD_CLASS_NAME[i], self.val['ref'][i])
        return r


import gdb.printing

def build_pretty_printers():
    pp = gdb.printing.RegexpCollectionPrettyPrinter("alanCommands")
    pp.add_printer('Symbol', '^Symbol$', SymbolPrinter)
    pp.add_printer('Expression', '^Expression$', ExpressionPrinter)
    pp.add_printer('Statement', '^Statement$', StatementPrinter)
    pp.add_printer('List', '^List$', ListPrinter)
    pp.add_printer('Srcp', '^Srcp$', SrcpPrinter)
    pp.add_printer('Word', '^Word$', WordPrinter)
    return pp

gdb.printing.register_pretty_printer(gdb.current_objfile(), build_pretty_printers(), replace=True)

#
# Arun commands
#

class ArunPrefixCommand(gdb.Command):
    "Prefix to do clever arun gdb commands"

    def __init__(self):
        super(ArunPrefixCommand, self).__init__("arun",
                                                gdb.COMMAND_SUPPORT,
                                                gdb.COMPLETE_NONE,
                                                True)

ArunPrefixCommand()

class ArunStringCommand(gdb.Command):
    "Print a string at Arun memory address"

    def __init__(self):
        super(ArunStringCommand, self).__init__("arun string",
                                                 gdb.COMMAND_SUPPORT,
                                                 gdb.COMPLETE_NONE)

    def invoke(self, arg, from_tty):
        gdb.execute("x/s &memory["+arg+"]")

ArunStringCommand()

class ArunWordsCommand(gdb.Command):
    "Print player input word or all"

    def __init__(self):
        super(ArunWordsCommand, self).__init__("arun words",
                                                gdb.COMMAND_SUPPORT,
                                                gdb.COMPLETE_NONE)

    def invoke(self, arg, from_tty):
        if arg != '':
            gdb.execute(f"p playerWords[{arg}]")
        else:
            i = 0
            e = gdb.execute(f"print playerWords[0].code", to_string=True)
            while ("-1" not in e):
                gdb.execute(f"print playerWords[{i}]")
                i = i+1
                e = gdb.execute(f"print playerWords[{i}].code", to_string=True)


ArunWordsCommand()

class ArunDictCommand(gdb.Command):
    "Print dictionary entry or all"

    def __init__(self):
        super(ArunDictCommand, self).__init__("arun dictionary",
                                                gdb.COMMAND_SUPPORT,
                                                gdb.COMPLETE_NONE)

    def invoke(self, arg, from_tty):
        if arg != '':
            gdb.execute(f"p dictionary[{arg}]")
        else:
            i = 0
            d = gdb.execute("p dictionarySize", to_string=True)
            d = int(d.strip().split()[-1])
            while (i < d):
                gdb.execute(f"print dictionary[{i}]")
                i = i+1


ArunDictCommand()
