-- kiss.i
-- Library version 0.3

-- 0.3 - use named attribute to format DOES text

SYNTAX
	kiss = kiss (obj)
		WHERE obj ISA THING
			ELSE "You can't kiss that!"

VERB kiss
	DOES
		IF obj=hero THEN
			"Well, if you must!"
		ELSE
			IF obj IS InAnimate THEN
				"You kiss"
				IF obj IS named THEN
					SAY obj.
				ELSE
					"the $1"
				END IF.
				"$$."
			ELSE 
				IF obj IS named THEN
					SAY obj.
					"avoids your advances."
				ELSE
					"The $o avoids your advances."
				END IF.
			END IF.
		END IF.
END VERB.


