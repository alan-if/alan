-- eat.i
-- Library version 0.3

-- 0.3 - use 'named' attribute to format DOES text


SYNTAX
	eat = eat (obj)
	WHERE obj ISA OBJECT
		ELSE "You can't eat that!"

	drink = drink (obj)
	WHERE obj ISA OBJECT
		ELSE "You can't drink that!"

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




