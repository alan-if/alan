-- smell.i
-- Library version 0.5.0

-- 0.4.1 - converted to ALANv3


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

Add To Every thing
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
End Add To.




