-- global.i
-- Library version 0.4.1

-- 0.4.1 -- due to ALANv3 changes,
--          added definition of 'hero' (the player character) as a container
--          for holding the player's "inventory", removed inventory object
--          from invent.i

-- synonyms for player's character so can "examine myself" "look at me" etc
-- and synonyms for "it" so can say "look at guard" and then "salute him"

SYNONYMS
	me, myself, self = HERO.
	her, him = it.

THE HERO ISA ACTOR
CONTAINER 
	LIMITS
		COUNT 10 THEN
			"You can't carry anything more. You have to drop something 
			first."
		weight 50 THEN
				"You can't carry anything more. You have to drop something 
				first."
	HEADER
		"You are carrying"
	ELSE
		"You are empty-handed."
END THE HERO.


-- 'named' used in std.i messages to distinguish between actors with real 
-- names like Fred (ie: is 'named') and The Bus-driver. 
-- This only automatically works for actors as "SAY object" doesn't 
-- capitalise the first letter even though you might have an object called 
-- Fred. To use this technique with objects, give the object a 
-- capitalised name in single quotes plus another name without the 
-- quotes - arun will use the object's leftmost name in the SAY statement
-- and the player will be able to use the uncapitalised name in player 
-- commands
--      eg: -- 
--         OBJECT robot AT spaceship NAME 'Floyd' NAME floyd 

ADD TO EVERY THING
IS
	NOT plural.
	NOT named.
END ADD TO THING. 

ADD TO EVERY ACTOR
IS
	named.
END ADD TO ACTOR.
