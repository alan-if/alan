-- throw.i
-- Library version 0.4.1

-- 0.4.1 - converted to ALANv3

SYNONYMS
	dump, cast = throw.


SYNTAX
	throw = throw (obj) *
		WHERE obj ISA OBJECT
			ELSE "You can only throw objects."


VERB throw
	CHECK obj IN HERO
		ELSE "You haven't got that!"
	DOES 
		"You can't throw very far,"
		IF obj IS named THEN
			SAY obj.
			"ends up on the ground."
		ELSE
			"the $1 ends up on the ground." 
		END IF.
		LOCATE obj HERE.
END VERB.


SYNTAX
	throw_at = throw (obj1) 'at' (obj2)
		WHERE obj1 ISA OBJECT
			ELSE "You can only throw objects."
		AND obj2 ISA THING
			ELSE "You can't throw anything at that."

	throw_to = throw (obj1) 'to' (obj2)
		WHERE obj1 ISA OBJECT
			ELSE "You can't be serious."
		AND obj2 ISA THING
			ELSE "You can't throw anything to that."

VERB throw_at, throw_to 
	CHECK obj1 IN HERO
		ELSE "You haven't got that!"
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
		IF obj1 IS named THEN
			SAY obj1.
			"bounces harmlessly off"
		ELSE
			"The $1 bounces harmlessly off"
		END IF.
		IF obj2 IS named THEN
			SAY obj2.
			"and ends up on the ground."
		ELSE 
			"the $2 and ends up on the ground."
		END IF.
		LOCATE obj1 HERE.
END VERB.


SYNTAX
	throw_in = throw (obj1) 'in' (obj2)
		WHERE obj1 ISA OBJECT
			ELSE "Don't be silly."
		AND obj2 ISA CONTAINER
			ELSE "You can't throw anything in that."

VERB throw_in
	CHECK obj1 IN HERO
		ELSE "You haven't got that!"
	AND obj1 <> obj2
		ELSE "Now, that would be a good trick!"
	DOES 
		LOCATE obj1 IN obj2.
		"Done."
END VERB.
