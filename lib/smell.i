-- smell.i
-- Library version 0.3

-- 0.3 - new 


SYNTAX
	smell0 = smell.

VERB smell0
	DOES
		"You smell nothing unusual."
	END VERB.


SYNTAX
	smell = smell (obj)
		WHERE obj ISA THING
			ELSE "You can't smell that!"

VERB smell
	DOES
		"You smell"
		IF obj IS named THEN
			SAY obj.
		ELSE
			"the $1"
		END IF.
		"$$."
END VERB.



