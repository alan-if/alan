OBJECT ATTRIBUTES
	NOT edible.
	NOT drinkable.

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
		LOCATE obj AT Limbo.
		"You eat the $o."
END VERB.

--

VERB drink
	CHECK obj IS drinkable
		ELSE "That is not drinkable."
	DOES
		LOCATE obj AT Limbo.
		"You drink the $o."
END VERB.

--


