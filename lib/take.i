OBJECT ATTRIBUTES
	takeable.

SYNONYMS
	grab, get = take.

SYNTAX
	take = take (obj) *
		WHERE obj ISA OBJECT
			ELSE "You can't take people with you!"
	drop = drop (obj) *.


VERB take
	CHECK obj IS takeable
		ELSE "You can't take that!"
	AND obj NOT IN inventory
		ELSE "You've already got that." 
	DOES
		LOCATE obj IN inventory.
		"Taken."
END VERB.


VERB drop
	CHECK obj IN inventory
		ELSE "You haven't got that."
	DOES
		LOCATE obj HERE.
		"Dropped."
END VERB.
