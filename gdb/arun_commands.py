from __future__ import with_statement
import gdb

class ArunInstanceCommand(gdb.Command):
    "Command to print an instance in Arun"

    def __init__(self):
        super(ArunInstanceCommand, self).__init__("instance",
                                                  gdb.COMMAND_USER,
                                                  gdb.COMPLETE_SYMBOL,
                                                  True
        )

    def invoke(self, arg, from_tty):
        gdb.execute("p (char *)&memory[instances[%d].id]" % gdb.parse_and_eval(arg))

ArunInstanceCommand()

class ArunStringCommand(gdb.Command):
    "Command to print an instance in Arun"

    def __init__(self):
        super(ArunStringCommand, self).__init__("string",
                                                gdb.COMMAND_USER,
                                                gdb.COMPLETE_SYMBOL,
                                                True
        )

    def invoke(self, arg, from_tty):
        gdb.execute("p (char *)&memory[%d]" % gdb.parse_and_eval(arg))

ArunStringCommand()
