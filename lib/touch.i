-- touch.i
-- Library version 0.4.1

-- 0.4.1 - converted to ALANv3


ADD TO EVERY THING 
IS
	touchable.
END ADD TO THING.


SYNTAX
	touch = touch (obj)
		WHERE obj ISA THING
			ELSE "You can't touch that."

	touch_with = touch (obj1) 'with' (obj2)
		WHERE obj1 ISA THING
			ELSE "You can't touch that."
		AND obj2 ISA OBJECT
			ELSE "You can use only objects to touch with."

Add To Every object
  VERB touch
	CHECK obj IS touchable
		ELSE "You can't touch that."
	DOES
		"You touch"
		IF obj IS named THEN
			SAY obj.
		ELSE
			"the $1"
		END IF.
		"$$."
  END VERB.

  VERB touch_with
	when obj1
	  CHECK obj1 IS touchable
		  ELSE "You can't touch that."
	  AND obj1 <> obj2
		  ELSE "It doesn't make sense to touch something with itself."
	  DOES
		  "You touch"
		  IF obj1 IS named THEN
			  SAY obj1.
			  "with"
		  ELSE
			  "the $1 with"
		  END IF.
		  IF obj2 IS named THEN
			  SAY obj2.
		  ELSE
			  "the $2"
		  END IF.
		  "$$."
  END VERB.
End Add To.
