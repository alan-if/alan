-- take.i
-- Library version 0.2

DEFAULT ATTRIBUTES
    InAnimate.

OBJECT ATTRIBUTES
    takeable.

ACTOR ATTRIBUTES
    NOT InAnimate.


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


VERB take, pick_up1, pick_up2
    CHECK obj IS takeable
	ELSE "You can't take that!"
    AND obj NOT IN inventory
	ELSE "You've already got that." 
    DOES
	LOCATE obj IN inventory.
	"Taken."
END VERB.



SYNONYMS
    discard = drop.

SYNTAX
    drop = drop (obj)*.

SYNTAX
    put_down1 = put (obj) * down.

SYNTAX
    put_down2 = put down (obj)*.


VERB drop, put_down1, put_down2
    CHECK obj IN inventory
	ELSE "You haven't got that."
    DOES
	LOCATE obj HERE.
	"Dropped."
END VERB.



SYNTAX
  take_from = 'take' (obj) 'from' (holder)
        WHERE obj ISA OBJECT
           ELSE "You can only take objects."
        AND holder ISA CONTAINER ACTOR OR CONTAINER OBJECT
           ELSE "You can't take things from that!"

VERB take_from
    CHECK obj NOT IN Inventory
        ELSE "You already have the $1."
    DOES
        IF holder=hero THEN
            "You can't take things from yourself!"
        ELSIF holder IS InAnimate THEN
            "You take the"
            SAY obj.
	    "$$."
            LOCATE obj IN Inventory.
        ELSE    
            SAY holder.
            "won't let you take the"
            SAY obj.
	    "$$."
        END IF.
END VERB.

