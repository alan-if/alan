-- kiss.i
-- Library version 0.5.0

-- 0.4.1 - converted to ALANv3



SYNTAX
	kiss = kiss (obj)
		WHERE obj ISA THING
			ELSE "You can't kiss that!"

Add To Every thing
  VERB kiss
	DOES
		IF obj=HERO THEN
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
End Add To.



