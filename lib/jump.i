-- jump.i
-- Library version 0.4.1

-- 0.4.1 - converted to ALANv3


SYNTAX
	jump_on = jump 'on' (obj)
		WHERE obj ISA THING
			ELSE "You can't jump on that!"

VERB jump_on
	DOES
		"You jump on"
		IF obj IS named THEN
			SAY obj.
		ELSE
			"the $1"
		END IF.
		"$$."
END VERB.



SYNTAX
	jump = jump.

VERB jump
	DOES
		"You jump up and down."
END VERB.