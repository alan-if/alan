-- knock.i
-- Library version 0.5.0

-- 0.4.1 - converted to ALANv3



SYNTAX
	knock_on = knock 'on' (obj)
		WHERE obj ISA THING
			ELSE "You can't knock on that!"

Add To Every thing
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
End Add To.




SYNTAX
	knock = knock.

VERB knock
	DOES
		"You need to specify what you want to knock on."
END VERB.
