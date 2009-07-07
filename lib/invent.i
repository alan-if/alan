-- invent.i
-- 0.6.2 : changed 'the Worn' from a 'thing' to an 'entity'

-- Library version 0.5.0
-- 0.4.1 - converted to ALANv3

ADD TO EVERY THING
   IS weight 0.
END ADD TO THING.


ADD TO EVERY ACTOR IS
	weight 50. 
END ADD TO ACTOR. 

ADD TO EVERY object
IS
	weight 5.
END ADD TO object. 



SYNONYMS
	i, inventory = invent.

SYNTAX
	invent = invent.

VERB invent
	DOES
		LIST hero.
		LIST worn.
END VERB invent.


THE worn ISA thing IN hero
CONTAINER
	LIMITS
		COUNT 10 THEN
			"You can't wear anything more. You have to remove something 
			first."
		weight 50 THEN
				"You can't wear anything more. You have to remove something 
				first."
	HEADER
		"You are wearing"
	ELSE
		""
END THE worn.

