-- open.i
-- Library version 0.4.1

-- 0.4.1 - converted to ALANv3


ADD TO EVERY OBJECT 
IS
	NOT closeable.
	closed.
END ADD TO OBJECT. 

SYNTAX
	open = open (obj)
	WHERE obj ISA OBJECT
		ELSE "You can't open that."

VERB open
	CHECK obj IS closeable
		ELSE "You can't open that!"
	AND obj IS closed
		ELSE "It's already open."
	DOES
		MAKE obj NOT closed.
		IF obj IS named THEN
			SAY obj.
			"is now open."
		ELSE
			"The $o is now open."
		END IF.
END VERB.


SYNTAX
	open_with = open (obj1) 'with' (obj2)
		WHERE obj1 ISA OBJECT
			ELSE "You can't open that."
		AND obj2 ISA OBJECT
			ELSE "You can't open anything with that."

VERB open_with
	CHECK obj2 IN HERO
		ELSE "You don't have"
		IF obj2 IS named THEN
			SAY obj2.
		ELSE
			"the $2"
		END IF.
		"$$."
	DOES
		"You can't open"
		IF obj1 IS named THEN
			SAY obj1.
		ELSE
			"the $1"
		END IF.
		"with"
		IF obj2 IS named THEN
			SAY obj2.
		ELSE
			"the $2"
		END IF.
		"$$."
END VERB.


SYNONYMS
	shut = close.

SYNTAX
	close = close (obj)
		WHERE obj ISA OBJECT
			ELSE "You can only close objects."

VERB close
	CHECK obj IS closeable
		ELSE "You can't close that."
	AND obj IS NOT closed
		ELSE "It is not open."
	DOES
		MAKE obj closed.
		IF obj IS named THEN
			SAY obj.
			"is now closed."
		ELSE
			"The $o is now closed."
		END IF.
END VERB.


SYNTAX
	close_with = close (obj1) 'with' (obj2)
		WHERE obj1 ISA OBJECT
			ELSE "You can't close that."
		AND obj2 ISA OBJECT
			ELSE "You can't close anything with that."

VERB close_with
	CHECK obj2 IN HERO
		ELSE 
			"You don't have"
			IF obj2 IS named THEN
				SAY obj2.
			ELSE
				"the $2"
			END IF.
			"$$."
	DOES
		"You can't close"
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

