-- give.i
-- Library version 0.4.1

-- 0.4.1 - converted to ALANv3



SYNTAX
	give = 'give' (obj) 'to' (recip)
		WHERE obj ISA OBJECT
			ELSE "You can only give away objects."
		AND recip ISA THING 
			ELSE "You can't give things to that!"
		AND recip ISA CONTAINER
			ELSE "You can't give things to that!"

VERB give
	CHECK obj IN HERO
		ELSE 
			"You don't have"
			IF obj IS named THEN
				SAY obj.
			ELSE
				"the $1"
			END IF.
			"$$."
	DOES
		IF recip=hero THEN
			"You already have"
			IF obj IS named THEN
				SAY obj.
			ELSE
				"the $o"
			END IF.
			"$$!"
		ELSE
			"You give"
			IF obj IS named THEN
				SAY obj.
				"to"
			ELSE
				"the $1 to"
			END IF.
			IF recip IS named THEN
				SAY recip.
			ELSE 
				"the $2"
			END IF.
			"$$."
			LOCATE obj IN recip.
		END IF.
END VERB.

