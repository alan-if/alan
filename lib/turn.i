-- turn.i
-- Library version 0.1

OBJECT ATTRIBUTES
    NOT on.
    NOT switchable.


SYNTAX
    turn_on1 = turn on (obj)
	WHERE obj ISA OBJECT
            ELSE "You can't turn that on."

    turn_on2 = turn (obj) on
	WHERE obj ISA OBJECT
            ELSE "You can't turn that on."

    switch_on1 = switch on (obj)
	WHERE obj ISA OBJECT
            ELSE "You can't switch that on."

    switch_on2 = switch (obj) on
	WHERE obj ISA OBJECT
            ELSE "You can't switch that on."


VERB turn_on1, turn_on2, switch_on1, switch_on2
    CHECK obj IS switchable
	ELSE "You can't turn that on."
    AND obj IS NOT on
	ELSE "It's already on."
    DOES
	MAKE obj on.
        "You turn on the $o."
END VERB.


SYNTAX
    turn_off1 = turn off (obj)
	WHERE obj ISA OBJECT
            ELSE "You can't turn that off."

    turn_off2 = turn (obj) off
	WHERE obj ISA OBJECT
            ELSE "You can't turn that off."

    switch_off1 = switch off (obj)
	WHERE obj ISA OBJECT
            ELSE "You can't switch that off."

    switch_off2 = switch (obj) off
	WHERE obj ISA OBJECT
            ELSE "You can't switch that off."



VERB turn_off1, turn_off2, switch_off1, switch_off2
    CHECK obj IS switchable
	ELSE "You can't turn that off."
    AND obj IS on
	ELSE "It's already off."
    DOES
	MAKE obj NOT on.
        "You turn the $o off."
END VERB.
