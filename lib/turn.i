-- turn.i
-- Library version 0.4.1

-- 0.4.1 - converted to ALANv3


ADD TO EVERY OBJECT 
IS
	NOT 'on'.
	NOT switchable.
END ADD TO OBJECT.


SYNTAX
	turn_on1 = turn 'on' (obj)
		WHERE obj ISA OBJECT
			ELSE "You can't turn that on."

	turn_on2 = turn (obj) 'on'
		WHERE obj ISA OBJECT
			ELSE "You can't turn that on."

	switch_on1 = switch 'on' (obj)
		WHERE obj ISA OBJECT
			ELSE "You can't switch that on."

	switch_on2 = switch (obj) 'on'
		WHERE obj ISA OBJECT
			ELSE "You can't switch that on."


Add To Every object
  VERB turn_on1, turn_on2, switch_on1, switch_on2
	CHECK obj IS switchable
		ELSE "You can't turn that on."
	AND obj IS NOT 'on'
		ELSE "It's already on."
	DOES
		MAKE obj 'on'.
		"You turn on"
		IF obj IS named THEN
			SAY obj.
			"$$."
		ELSE 
			"the $o."
		END IF.
  END VERB.
End Add To.



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



Add To Every object
  VERB turn_off1, turn_off2, switch_off1, switch_off2
	CHECK obj IS switchable
		ELSE "You can't turn that off."
	AND obj IS 'on'
		ELSE "It's already off."
	DOES
		MAKE obj NOT 'on'.
		"You turn off"
		IF obj IS named THEN
			SAY obj.
		ELSE 
			"the $o"
		END IF.
		"$$."
  END VERB.
End Add To.

