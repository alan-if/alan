-- take.i
-- Library version 0.4.1

-- 0.4.1 - converted to ALANv3



ADD TO EVERY THING 
IS
	InAnimate.
END ADD TO THING.

ADD TO EVERY OBJECT 
IS
	takeable.
END ADD TO OBJECT.

ADD TO EVERY ACTOR 
IS
	NOT InAnimate.
END ADD TO ACTOR.


SYNONYMS
	get, carry, obtain, grab, steal, confiscate, hold = take.


SYNTAX
	take = take (obj) *
		WHERE obj ISA OBJECT
			ELSE "You can't take that with you!"


SYNTAX
	pick_up1 = pick up (obj)*
		WHERE obj ISA OBJECT
			ELSE "You can't take that with you!"

	pick_up2 = pick (obj)* up
		WHERE obj ISA OBJECT
			ELSE "You can't take that with you!"

Add To Every object
  VERB take, pick_up1, pick_up2
	CHECK obj IS takeable
		ELSE "You can't take that!"
	AND obj NOT IN worn
		ELSE "You've already got that - you're wearing that." 
	AND obj NOT IN HERO
		ELSE "You've already got that." 
	AND weight of obj <=50 
		ELSE "That is too heavy to lift."
	DOES
		LOCATE obj IN HERO.
		"Taken."
  END VERB.
End Add To.



SYNONYMS
	discard = drop.

SYNTAX
	drop = drop (obj)*.

SYNTAX
	put_down1 = put (obj) * down.

SYNTAX
	put_down2 = put down (obj)*.

Add To Every object
  VERB drop, put_down1, put_down2
	CHECK obj IN HERO
		ELSE "You aren't carrying that."
	DOES
		LOCATE obj HERE.
		"Dropped."
  END VERB.
End Add To.



SYNTAX
  take_from = 'take' (obj) 'from' (holder)
	WHERE obj ISA OBJECT
	   ELSE "You can only take objects."
	AND holder ISA THING
	   ELSE "You can't take things from that!"
	AND holder ISA CONTAINER
	   ELSE "You can't take things from that!"

Add To Every object
  VERB take_from
	CHECK obj NOT IN HERO
		ELSE 
			"You already have"
			IF obj IS named THEN
				SAY obj.
			ELSE
				"the $1"
			END IF.
			"$$."
	DOES
		IF holder=hero THEN
			"You can't take things from yourself!"
		ELSIF holder IS InAnimate THEN
			"You take"
			IF obj IS named THEN
				SAY obj.
			ELSE
				"the $1"
			END IF.
			"$$."
			LOCATE obj IN HERO.
		ELSE 
			IF holder IS named THEN
				SAY holder.
			ELSE
				"The $2"
			END IF.
			"won't let you take"
			IF obj IS named THEN
				SAY obj.
			ELSE
				"the $1"
			END IF.
			"$$."
		END IF.
  END VERB.
End Add.

