-- wear.i
-- library version 0.4.1

-- 0.4.1 - converted to ALANv3


ADD TO EVERY OBJECT 
IS
	NOT wearable.
END ADD TO OBJECT.


SYNTAX 
	wear = wear (obj)
		WHERE obj ISA OBJECT
			ELSE "You can't wear that." 
	put_o_on = put (obj) 'on'
		WHERE obj ISA OBJECT
			ELSE "You can't wear that." 
	put_on_o = put 'on' (obj)
		WHERE obj ISA OBJECT
			ELSE "You can't wear that." 


Add To Every object
  verb wear, put_o_on, put_on_o
	CHECK obj IS wearable
		ELSE 
			"You can't wear"
			IF obj IS named THEN
				SAY obj.
			ELSE
				"the $o"
			END IF.
			"$$." 
	AND obj NOT IN worn 
		ELSE
			"You are already wearing"
			IF obj IS named THEN
				SAY obj.
			ELSE
				"the $o"
			END IF.
			"$$." 
	AND obj IS TAKEABLE
		ELSE
			"You can't pick"
			IF obj IS named THEN
				SAY obj.
				"up." 
			ELSE
				"the $o up."
			END IF.
	DOES
		IF obj NOT IN HERO THEN
			LOCATE obj IN HERO.
			"(You pick"
			IF obj IS named THEN
				SAY obj.
				"up.)$n"
			ELSE
				"the $o up.)$n"
			END IF.
		END IF. 
		LOCATE obj IN worn.
		"You put on"
		IF obj IS named THEN
			SAY obj.
		ELSE
			"the $o"
		END IF.
		"$$." 
  END VERB.
End Add To.




SYNTAX 
	remove = remove (obj)
		WHERE obj ISA OBJECT
			ELSE "You can't remove that." 
	take_o_off = take (obj) off
		WHERE obj ISA OBJECT
			ELSE "You can't remove that." 
	take_off_o = take off (obj) 
		WHERE obj ISA OBJECT
			ELSE "You can't remove that." 

Add To Every object
  VERB remove, take_o_off, take_off_o
	CHECK obj IN worn
		ELSE 
			"You are not wearing"
			IF obj IS named THEN
				SAY obj.
			ELSE
				"the $o"
			END IF.
			"$$." 
	DOES
		LOCATE obj IN HERO.
		"You take off"
		IF obj IS named THEN
			SAY obj.
		ELSE
			"the $o"
		END IF.
		"$$." 
  END VERB.
End Add To.



SYNTAX undress = undress.

Add To Every object
  VERB undress
	DOES
		IF COUNT IN worn > 0 THEN
			EMPTY worn IN HERO.
			"You remove all the items you were wearing."
		 ELSE
			"You're not wearing anything you can remove."
		END IF.
  END VERB.
End Add To.

