-- invent.i
-- Library version 0.3

-- 0.3 
--      added worn container to implement clothing
--      added weight default attribute
--      weight LIMIT added to the inventory and worn containers


SYNONYMS
	i, inventory = invent.

SYNTAX
	invent = invent.

VERB invent
	DOES
		LIST hero.
		LIST worn.
END VERB invent.


THE worn ISA OBJECT
CONTAINER
	LIMITS
		COUNT 10 THEN
			"You can't wear anything more. You have to remove something 
			first."
--                weight 50 THEN
--                        "You can't wear anything more. You have to remove something 
--                        first."
	HEADER
		"You are wearing"
	ELSE
		""
END THE worn.

