-- examine.i
-- Library version 0.3.1


-- 0.3.1
--  - added verb "look in <object>"
--
-- 0.3 
--  - use 'named' attribute to format DOES text
--  - added searchable attribute, separated default search definition
--          from look_at & examine definition

SYNONYMS
	x, inspect, 'check' = examine.

SYNTAX
	examine = examine (obj) *
		WHERE obj ISA THING
			ELSE "You can't examine that!"

SYNTAX
	look_at = 'look' 'at' (obj) *
		WHERE obj ISA THING
			ELSE "You can't examine that!"

VERB examine, look_at
	CHECK obj IS examinable
		ELSE 
			"You can't examine"
			IF obj IS named THEN
				SAY obj.
			ELSE
				"the $o"
			END IF.
			"$$." 
	DOES
		"There is nothing special about"
		IF obj IS named THEN
			SAY obj.
		ELSE
			"the $o"
		END IF.
		"$$." 
END VERB.

----

SYNONYMS
	inside, into = 'in'.

SYNTAX
	look_in = 'look' 'in' (obj) 
		WHERE obj ISA THING
			ELSE "You can't look inside that."
		AND obj ISA CONTAINER
			ELSE "You can't look inside that."

VERB look_in
	CHECK obj IS examinable
		ELSE 
			"You can't look inside"
			IF obj IS named THEN
				SAY obj.
			ELSE
				"the $o"
			END IF.
			"$$." 
	DOES
		LIST obj.
END VERB.

----

SYNTAX
	search = search (obj) 
		WHERE obj ISA THING
			ELSE "You can't search that!"

VERB search
	CHECK obj IS searchable
		ELSE 
			"You can't search" 
			IF obj IS named THEN
				SAY obj.
			ELSE
				"the $o"
			END IF.
			"$$!" 
	DOES
		"You find nothing of interest."
END VERB.
