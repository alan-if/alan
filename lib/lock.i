-- lock.i
-- Library version 0.1


OBJECT ATTRIBUTES
    NOT lockable.
    locked.

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
	"The $o is now locked."
END VERB.


SYNTAX
    lock_with = lock (obj) with (key)
	WHERE obj ISA OBJECT
	    ELSE "You can't lock that."
	AND key ISA OBJECT
	    ELSE "You can't lock anything with that."

VERB lock_with
    CHECK obj IS lockable
	ELSE "You can't lock that!"
    AND obj IS NOT locked
	ELSE "It's already locked."
    AND key IN Inventory
        ELSE "You don't have the $2"
    DOES
	MAKE obj locked.
	"The $o is now locked."
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
	"The $o is now unlocked."
END VERB.


SYNTAX
    unlock_with = unlock (obj) with (key)
	WHERE obj ISA OBJECT
	    ELSE "You can't lock that."
	AND key ISA OBJECT
	    ELSE "You can't lock anything with that."

VERB unlock_with
    CHECK obj IS lockable
	ELSE "You can't unlock that!"
    AND obj IS locked
	ELSE "It's already unlocked."
    AND key IN Inventory
        ELSE "You don't have the $2"
    DOES
	MAKE obj NOT locked.
	"The $o is now unlocked."
END VERB.

