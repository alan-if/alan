-- eat.i
-- Library version 0.4.1

-- 0.4.1 - converted to ALANv3


ADD TO EVERY OBJECT 
IS
	NOT edible.
	NOT drinkable.
END ADD TO OBJECT. 

SYNTAX
	eat = eat (obj)
	WHERE obj ISA OBJECT
		ELSE "You can't eat that!"

	drink = drink (obj)
	WHERE obj ISA OBJECT
		ELSE "You can't drink that!"

Add To Every object
  VERB eat
	CHECK obj IS edible
		ELSE "You can't eat that!"
	DOES
		LOCATE obj AT Nowhere.
		"You eat"
		IF obj IS named THEN
			SAY obj.
		ELSE
			"the $o"
		END IF.
		"$$." 
  END VERB.

  VERB drink
	CHECK obj IS drinkable
		ELSE "That is not drinkable."
	DOES
		LOCATE obj AT Nowhere.
		"You drink"
		IF obj IS named THEN
			SAY obj.
		ELSE
			"the $o"
		END IF.
		"$$." 
  END VERB.
End Add.

