OBJECT ATTRIBUTES
	takeable.

SYNONYMS
	get, carry, obtain, grab, steal, confiscate, hold = take.

SYNTAX
	take = take (obj) *
		WHERE obj ISA OBJECT
			ELSE "You can't take people with you!"

VERB take
	CHECK obj IS takeable
		ELSE "You can't take that!"
	AND obj NOT IN inventory
		ELSE "You've already got that." 
	DOES
		LOCATE obj IN inventory.
		"Taken."
END VERB.


SYNTAX
	pick_up1 = pick u (obj)*
		WHERE obj ISA OBJECT
		ELSE "You can't take people with you!"

	pick_up2 = pick (obj)* u
		WHERE obj ISA OBJECT
		ELSE "You can't take people with you!"


VERB pick_up1, pick_up2
	CHECK obj IS takeable
		ELSE "You can't take that!"
	AND obj NOT IN inventory
		ELSE "You've already got that."
	DOES
		LOCATE obj IN inventory.
		"Taken."
END VERB.



SYNONYMS
	discard = drop.

SYNTAX
	drop = drop (obj) *.

VERB drop
	CHECK obj IN inventory
		ELSE "You haven't got that."
	DOES
		LOCATE obj HERE.
		"Dropped."
END VERB.
