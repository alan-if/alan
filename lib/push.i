-- push.i
-- Library version 0.3

-- 0.3 
--      - use the named attribute to format 'CHECK' and 'DOES' text
--      - objects & actors now 'pushable' by default

SYNTAX
	push = push (obj)
		WHERE obj ISA THING
			ELSE "You can't push that."

VERB push
	CHECK obj IS pushable
		ELSE "You can't push that."
	DOES
		"You push"
		IF obj IS named THEN
			SAY obj.
		ELSE
			"the $1"
		END IF.
		"$$."
END VERB.


SYNTAX
	push_with = push (obj1) 'with' (obj2)
		WHERE obj1 ISA THING
			ELSE "You can't push that."
		AND obj2 ISA OBJECT
			ELSE "You can use only objects to push things with."

VERB push_with
	CHECK obj1 IS pushable
		ELSE "You can't push that."
	DOES
		"Using"
		IF obj2 IS named THEN
			SAY obj2.
			"you push"
		ELSE
			"the $2 you push"
		END IF.
		IF obj1 IS named THEN
			SAY obj1.
		ELSE
			"the $1"
		END IF.
		"$$."
END VERB.

