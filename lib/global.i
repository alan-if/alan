-- global.i

-- Library 0.3.2
--      added 'plural' attribute (but its not used for anything)
--      converted to Alan v3 syntax


THE HERO ISA ACTOR
CONTAINER 
	LIMITS
		COUNT 10 THEN
			"You can't carry anything more. You have to drop something 
			first."
--                weight 50 THEN
--                        "You can't carry anything more. You have to drop something 
--                        first."
	HEADER
		"You are carrying"
	ELSE
		"You are empty-handed."
END THE HERO.


-- synonyms for player's character so can "examine myself" "look at me" etc
-- and synonyms for "it" so can say "look at guard" and then "salute him"

SYNONYMS
	me, myself, self = hero.
	her, him = it.


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
	touchable. --from turn
	NOT can_talk. --from talk
	InAnimate. -- from take
	pushable. -- from push
	examinable.  -- from examine
	searchable.  -- from examine
	NOT Shootable. -- from attack
END ADD TO THING. 


ADD TO EVERY ACTOR
IS
	named.
	can_talk. --from talk
	NOT InAnimate. --from talk
	weight 50. -- from invent
	NOT searchable. -- from examine
END ADD TO ACTOR.


ADD TO EVERY object
IS
	NOT wearable. -- from wear
	NOT 'on'.  --from turn
	NOT switchable. --from turn
	takeable. -- from take
	NOT readable. -- from read
	NOT closeable. --from open
	closed. --from open
	NOT lockable.  -- from lock
	locked.  -- from lock
	weight 5. --from invent
	NOT edible. -- from eat
	NOT drinkable.  -- from eat
	NOT Weapon. -- from attack
END ADD TO object. 


