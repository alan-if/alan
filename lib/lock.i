-- lock.i
-- Library version 0.4.1

-- 0.4.1 - converted to ALANv3


ADD TO EVERY OBJECT 
    IS
	NOT lockable.
	locked.
END ADD TO OBJECT. 


SYNTAX
	lock = lock (obj)
		WHERE obj ISA OBJECT
			ELSE "You can't lock that."

Add To Every object
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
End Add To object. 



SYNTAX
	lock_with = lock (obj) 'with' (key)
		WHERE obj ISA OBJECT
			ELSE "You can't lock that."
		AND key ISA OBJECT
			ELSE "You can't lock anything with that."

Add To Every Object
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
End Add.


SYNTAX
	unlock = unlock (obj)
		WHERE obj ISA OBJECT
			ELSE "You can't lock that."
Add To Every object
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
End Add To.


SYNTAX
	unlock_with = unlock (obj) 'with' (key)
		WHERE obj ISA OBJECT
			ELSE "You can't lock that."
		AND key ISA OBJECT
			ELSE "You can't lock anything with that."

Add To Every object
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
End Add.

