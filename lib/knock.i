-- knock.i
-- Library version 0.3

-- 0.3 - new 

SYNTAX
	knock_on = knock 'on' (obj)
		WHERE obj ISA THING
			ELSE "You can't knock on that!"

VERB knock_on
	DOES
		"You knock on"
		IF obj IS named THEN
			SAY obj.
		ELSE
			"the $1"
		END IF.
		"$$."
END VERB.



SYNTAX
	knock = knock.

VERB knock
	DOES
		"You need to specify what you want to knock on."
END VERB.