-- invent.i
-- Library version 0.1, 0.2


SYNONYMS
    i, inventory = invent.

SYNTAX
    invent = invent.

VERB invent
    DOES
	LIST inventory.
END VERB invent.


CONTAINER inventory
    LIMITS
    	COUNT 10 THEN
    	    "You can't carry anything more. You have to drop something first."
    HEADER
    	"You are carrying"
    ELSE
	"You are empty-handed."
END CONTAINER.

