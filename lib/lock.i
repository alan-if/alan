-- lock.i
-- Library version 0.3

-- 0.3 - use named attribute to format CHECK and DOES text

SYNTAX
	lock = lock (obj)
		WHERE obj ISA OBJECT
			ELSE "You can't lock that."


VERB lock
	CHECK obj IS lockable
		ELSE "You can't lock that!"
	AND obj IS NOT locked
		ELSE "It's already locked."
	DOES
		MAKE obj locked.
		IF obj IS named THEN
			SAY obj.
			"is now locked."
		ELSE
			"The $o is now locked."
		END IF.
END VERB.


SYNTAX
	lock_with = lock (obj) 'with' (key)
		WHERE obj ISA OBJECT
			ELSE "You can't lock that."
		AND key ISA OBJECT
			ELSE "You can't lock anything with that."

VERB lock_with
	CHECK obj IS lockable
		ELSE "You can't lock that!"
	AND obj IS NOT locked
		ELSE "It's already locked."
	AND key IN HERO
		ELSE 
			"You don't have"
			IF key IS named THEN
				SAY key.
			ELSE
				"the $2"
			END IF.
			"$$."
	DOES
		MAKE obj locked.
		IF obj IS named THEN
			SAY obj.
			"is now locked."
		ELSE
			"The $o is now locked."
		END IF.
END VERB.


SYNTAX
	unlock = unlock (obj)
		WHERE obj ISA OBJECT
			ELSE "You can't lock that."

VERB unlock
	CHECK obj IS lockable
		ELSE "You can't unlock that!"
	AND obj IS locked
		ELSE "It's already unlocked."
	DOES
		MAKE obj NOT locked.
		IF obj IS named THEN
			SAY obj.
			"is now unlocked."
		ELSE
			"The $o is now unlocked."
		END IF.
END VERB.


SYNTAX
	unlock_with = unlock (obj) 'with' (key)
		WHERE obj ISA OBJECT
			ELSE "You can't lock that."
		AND key ISA OBJECT
			ELSE "You can't lock anything with that."

VERB unlock_with
	CHECK obj IS lockable
		ELSE "You can't unlock that!"
	AND obj IS locked
		ELSE "It's already unlocked."
	AND key IN HERO
		ELSE 
			"You don't have"
			IF key IS named THEN
				SAY key.
			ELSE
				"the $2"
			END IF.
			"$$."
	DOES
		MAKE obj NOT locked.
		IF obj IS named THEN
			SAY obj.
			"is now unlocked."
		ELSE
			"The $o is now unlocked."
		END IF.
END VERB.

