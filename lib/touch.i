OBJECT ATTRIBUTES
  NOT touchable.

SYNTAX
        touch = touch (obj)
		WHERE obj ISA OBJECT
			ELSE "You can't touch that."

        touch_with = touch (obj1) 'with' (obj2)
		WHERE obj1 ISA OBJECT
			ELSE "You can't touch that."
		AND obj2 ISA OBJECT
			ELSE "You can't use a $2 to touch anything."

VERB touch
	CHECK obj IS touchable
		ELSE "You can't touch that."
END VERB.


VERB touch_with
	CHECK obj1 IS touchable
		ELSE "You can't touch that."
END VERB.

