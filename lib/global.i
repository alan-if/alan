-- global.i
-- Library version 0.5.0

-- 0.4.1 -- due to ALANv3 changes:
--          added definition of 'hero' (the player character) as a
--          container for holding the player's "inventory", removed
--          inventory object from invent.i

--	    synonyms for player's character so can "examine myself" "look at
--          me" etc and synonyms for "it" so can say "look at guard"
--          and then "salute him"

Synonyms
  me, myself, self = hero.
  her, him = it.

The hero Isa actor
Container
  Limits
    Count 10 Then
      "You can't carry anything more. You have to drop something 
	first."
    weight 50 Then
      "You can't carry anything more. You have to drop something 
	first."
    Header
      "You are carrying"
    Else
      "You are empty-handed."
End The hero.


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

Add To Every thing
  Is
    Not plural.
    Not named.
End Add To thing. 

Add To Every actor
Is
  named.
End Add To actor.
