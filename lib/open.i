OBJECT ATTRIBUTES 
	NOT openable.
	NOT closable.
	closed.
	NOT lockable.
	locked.

SYNTAX
	open = open (obj)
		WHERE obj ISA OBJECT
			ELSE "You can't open people."

	close = close (obj)
		WHERE obj ISA OBJECT
			ELSE "You can't close people."

        open_with = open (obj1) 'with' (obj2)
		WHERE obj1 ISA OBJECT
			ELSE "You can't open that."
		AND obj2 ISA OBJECT
			ELSE "You can't close anything with that."

        close_with = close (obj1) 'with' (obj2)
		WHERE obj1 ISA OBJECT
			ELSE "You can't close that."
		AND obj2 ISA OBJECT
			ELSE "You can't close anything with that."

        lock = lock (obj)
		WHERE obj ISA OBJECT
			ELSE "You can't lock that."

        unlock = unlock (obj)
		WHERE obj ISA OBJECT
			ELSE "You can't lock that."

        lock_with = lock (obj) with (key)
		WHERE obj ISA OBJECT
			ELSE "You can't lock that."
		AND key ISA OBJECT
			ELSE "You can't lock anything with that."

        unlock_with = unlock (obj) with (key)
		WHERE obj ISA OBJECT
			ELSE "You can't lock that."
		AND key ISA OBJECT
			ELSE "You can't lock anything with that."

VERB open
	CHECK obj IS openable
		ELSE "You can't open that!"
	AND obj IS closed
		ELSE "It's already open."
	DOES
		MAKE obj NOT closed.
		"The $o is now open."
END VERB.

--

VERB close
	CHECK obj IS closable
		ELSE "You can't close that."
	AND obj IS NOT closed
		ELSE "It is not open."
	DOES
		MAKE obj closed.
		"The $o is now closed."
END VERB.

--

VERB open_with
	CHECK obj1 HERE
		ELSE "I don't see any $1 to open."
	AND obj2 IN inventory
		ELSE "You don't have the $2."
	DOES
		"You can't open the $1 with the $2."
END VERB.

--

VERB close_with
	CHECK obj1 HERE
		ELSE "I don't see any $1 to close."
	AND obj2 IN inventory
		ELSE "You don't have the $2."
	DOES
		"You can't close the $1 with the $2."
END VERB.

--

VERB lock
	CHECK obj IS lockable
		ELSE "You can't lock that!"
	AND obj HERE
		ELSE "I don't see what you want me to lock."
	AND obj IS NOT locked
		ELSE "It's already locked."
	DOES
		MAKE obj locked.
		"The $o is now locked."
END VERB.

--

VERB unlock
	CHECK obj IS lockable
		ELSE "You can't unlock that!"
	AND obj HERE
		ELSE "I don't see what you want me to unlock."
	AND obj IS locked
		ELSE "It's already unlocked."
	DOES
		MAKE obj NOT locked.
		"The $o is now unlocked."
END VERB.

VERB lock_with
	CHECK obj IS lockable
		ELSE "You can't lock that!"
	AND obj HERE
		ELSE "I don't see what you want me to lock."
	AND obj IS NOT locked
		ELSE "It's already locked."
END VERB.

--

VERB unlock_with
	CHECK obj IS lockable
		ELSE "You can't unlock that!"
	AND obj HERE
		ELSE "I don't see what you want me to unlock."
	AND obj IS locked
		ELSE "It's already unlocked."
END VERB.

