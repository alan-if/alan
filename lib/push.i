OBJECT ATTRIBUTES
  NOT pushable.

SYNTAX
        push = push (obj)
		WHERE obj ISA OBJECT
			ELSE "You can't push that."

        push_with = push (obj1) 'with' (obj2)
		WHERE obj1 ISA OBJECT
			ELSE "You can't push that."
		AND obj2 ISA OBJECT
			ELSE "You can't use a $2 to push anything."

VERB push
	CHECK obj IS pushable
		ELSE "You can't push that."
END VERB.


VERB push_with
	CHECK obj1 IS pushable
		ELSE "You can't push that."
END VERB.

