-- put.i
-- Library version 0.4.1

-- 0.4.1 - converted to ALANv3


SYNONYMS
	place = put.

SYNTAX
	put = put (obj) *
		WHERE obj ISA OBJECT
			ELSE "You can't put that anywhere."

Add To Every object
  VERB put
	CHECK obj IN HERO
		ELSE "You haven't got that."
	DOES
		LOCATE obj HERE.
		"Dropped."
  END VERB.
End Add To.




SYNTAX
	put_in = put (obj1) 'in' (obj2)
		WHERE obj1 ISA OBJECT
			ELSE "You can't put that anywhere."
		AND obj2 ISA CONTAINER
			ELSE "You can't put anything in that." 

Add To Every object
  VERB put_in
	CHECK obj1 IN HERO
		ELSE 
			"You haven't got"
			IF obj1 IS named THEN
				SAY obj1.
			ELSE
				"the $1"
			END IF.
			"$$!"
	AND obj1 <> obj2
		ELSE "You can't put something into itself!"
	DOES
		LOCATE obj1 IN obj2.
		"Done."
  END VERB.
End Add To.



SYNTAX
	put_near = put (obj1) near (obj2)
		WHERE obj1 ISA OBJECT
			ELSE "You can't put that anywhere."
		AND obj2 ISA THING
			ELSE "You can't put anything near that."

	put_behind = put (obj1) behind (obj2)
		WHERE obj1 ISA OBJECT
			ELSE "You can't put that anywhere."
		AND obj2 ISA THING
			ELSE "You can't put anything behind that."

	put_on = put (obj1) 'on' (obj2)
		WHERE obj1 ISA OBJECT
			ELSE "You can't put that anywhere."
		AND obj2 ISA THING
			ELSE "You can't put anything on that."

	put_under = put (obj1) under (obj2)
		WHERE obj1 ISA OBJECT
			ELSE "You can't put that anywhere."
		AND obj2 ISA THING
			ELSE "You can't put anything under that."

Add To Every object
  VERB put_near, put_behind, put_on, put_under 
	CHECK obj1 IN HERO
		ELSE 
			"You haven't got"
			IF obj1 IS named THEN
				SAY obj1.
			ELSE
				"the $1"
			END IF.
			"$$!"
	AND obj2 NOT IN HERO
		ELSE 
			"You are carrying"
			IF obj2 IS named THEN
				SAY obj2.
			ELSE
				"the $2"
			END IF.
			"$$!"
	DOES
		"Naaah. I'd rather just put"
		IF obj1 IS named THEN
			"them"
		ELSE
			"it"
		END IF.
		"down here."
		LOCATE obj1 AT obj2.
  END VERB.
End Add To.


