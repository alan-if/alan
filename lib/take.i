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



Synonyms
  discard = drop.

Syntax
  drop = drop (obj)*.

Syntax
  put_down1 = put (obj) * down.

Syntax
  put_down2 = put down (obj)*.

Add To Every object
  Verb drop, put_down1, put_down2
    Check obj In hero
      Else "You aren't carrying that."
    Does
      Locate obj Here.
      "Dropped."
  End Verb.
End Add To.



Syntax
  take_from = 'take' (obj) 'from' (holder)
	Where obj Isa object
	   Else "You can only take objects."
	And holder Isa thing
	   Else "You can't take things from that!"
	And holder Isa Container
	   Else "You can't take things from that!"

Add To Every object
  Verb take_from
    When obj
      Check obj Not In hero
	Else
	  "You already have"
	  If obj Is named Then
	    Say obj.
	  Else
	    "the $1"
	  End If.
	  "$$."
      Does
	If holder=hero Then
	  "You can't take things from yourself!"
	Elsif holder Is inanimate Then
	  "You take"
	  If obj Is named Then
	    Say obj.
	  Else
	    "the $1"
	  End If.
	  "$$."
	  Locate obj In hero.
	Else
	  If holder Is named Then
	    Say holder.
	  Else
	    "The $2"
	  End If.
	  "won't let you take"
	  If obj Is named Then
	    Say obj.
	  Else
	    "the $1"
	  End If.
	  "$$."
	End If.
  End Verb.
End Add.

