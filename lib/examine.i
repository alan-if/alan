DEFAULT ATTRIBUTES
	examinable.

SYNONYMS
	x, search, inspect, 'check' = examine.

SYNTAX
	examine = examine (obj) *
		WHERE obj ISA OBJECT OR ACTOR
			ELSE "You can't examine that!"

VERB examine
	CHECK obj IS examinable
		ELSE "You can't examine that!"
	DOES
		"There is nothing special about $o."
END VERB.

