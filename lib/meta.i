-- meta.i
-- Library version 0.1, 0.2


SYNONYMS
    q = quit.

SYNTAX
    quit = quit.

VERB quit
    DOES
    	QUIT.
END VERB.


SYNTAX
    save = save.

VERB save
    DOES
	SAVE.
	"Done."
END VERB.


SYNTAX
    restore = restore.

VERB restore
    DOES
    	RESTORE.
    	"Done.$n"
	LOOK.
END VERB.


SYNTAX
    restart = restart.

VERB restart
    DOES
        RESTART.
END VERB.



SYNTAX
    'score' = 'score'.

VERB 'score'
    DOES
    	SCORE.
END VERB 'score'.


SYNONYMS
    z = wait.

SYNTAX
    wait = wait.

VERB wait
    DOES
    	"Time passes..."
END VERB.


SYNONYMS
    g = again.

SYNTAX
    again = again.

VERB again
    DOES
        "Press the F3 key to repeat your last command."
END VERB.


SYNTAX
    undo = undo.

VERB undo
    DOES
        "Unfortunately you cannot 'undo' commands in this game."
END VERB.

