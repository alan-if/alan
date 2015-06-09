import gdb

class AlanPrefixCommand(gdb.Command):
    "Prefix to do clever alan gdb commands"

    def __init__(self):
        super(AlanPrefixCommand), self).__init__("alan",
                                                 gdb.COMMAND_SUPPORT,
                                                 gdb.COMPLETE_NONE,
                                                 True)

AlanPrefixCommand()
