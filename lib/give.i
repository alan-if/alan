-- give.i
-- Library version 0.1, 0.2


SYNTAX
    give = 'give' (obj) 'to' (recip)
	WHERE obj ISA OBJECT
	    ELSE "You can only give away objects."
	AND recip ISA CONTAINER ACTOR
	    ELSE "You can't give things to that!"

VERB give
    CHECK obj IN Inventory
	ELSE "You don't have the $1."
    DOES
	IF recip=hero THEN
	    "You already have the $o!"
	ELSE
	    "You give the"
	    SAY obj.
	    "to"
	    SAY recip.
	    LOCATE obj IN recip.
	END IF.
END VERB.

