-- open.i
-- Library version 0.1, 0.2


OBJECT ATTRIBUTES
    NOT closable.
    closed.

SYNTAX
    open = open (obj)
	WHERE obj ISA OBJECT
	    ELSE "You can't open that."

VERB open
    CHECK obj IS closable
	ELSE "You can't open that!"
    AND obj IS closed
	ELSE "It's already open."
    DOES
	MAKE obj NOT closed.
	"The $o is now open."
END VERB.


SYNTAX
    open_with = open (obj1) 'with' (obj2)
	WHERE obj1 ISA OBJECT
	    ELSE "You can't open that."
	AND obj2 ISA OBJECT
            ELSE "You can't open anything with that."

VERB open_with
    CHECK obj2 IN inventory
	ELSE "You don't have the $2."
    DOES
	"You can't open the $1 with the $2."
END VERB.


SYNONYMS
    shut = close.

SYNTAX
    close = close (obj)
	WHERE obj ISA OBJECT
	    ELSE "You can't close people."

VERB close
    CHECK obj IS closable
	ELSE "You can't close that."
    AND obj IS NOT closed
	ELSE "It is not open."
    DOES
	MAKE obj closed.
	"The $o is now closed."
END VERB.


SYNTAX
    close_with = close (obj1) 'with' (obj2)
	WHERE obj1 ISA OBJECT
	    ELSE "You can't close that."
	AND obj2 ISA OBJECT
	    ELSE "You can't close anything with that."

VERB close_with
    CHECK obj2 IN inventory
	ELSE "You don't have the $2."
    DOES
	"You can't close the $1 with the $2."
END VERB.

