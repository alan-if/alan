-- examine.i
-- Library version 0.2


DEFAULT ATTRIBUTES
    examinable.

SYNONYMS
    x, inspect, 'check' = examine.

SYNTAX
    examine = examine (obj) *
	WHERE obj ISA OBJECT OR ACTOR
	    ELSE "You can't examine that!"

SYNTAX
    look_at = 'look' 'at' (obj) *
	WHERE obj ISA OBJECT OR ACTOR
	    ELSE "You can't examine that!"

SYNTAX
    search = search (obj) *
	WHERE obj ISA OBJECT OR ACTOR
	    ELSE "You can't search that!"

VERB examine, look_at, search
    CHECK obj IS examinable
	ELSE "You can't examine that!"
    DOES
	"There is nothing special about the $o."
END VERB.

