-- attack.i
-- Library version 0.4.1

-- 0.4.1 - converted to ALANv3


ADD TO EVERY THING
IS 
   NOT Shootable.
END ADD TO THING. 

ADD TO EVERY OBJECT
IS
	NOT Weapon.
	not shootable.
END ADD TO OBJECT. 


SYNONYMS
	kill, fight, hit = attack.
	fire = shoot.


SYNTAX
	attack = attack (act)
		WHERE act ISA THING
			ELSE "You can't attack that."

VERB attack
	DOES
		"Violence is not the answer."
END VERB.


SYNTAX
	attack_with = attack (act) 'with' (obj)
		WHERE act ISA THING
			ELSE "You can't attack that."
		AND obj ISA OBJECT
			ELSE "You can't attack anything with that!"

VERB attack_with
	CHECK obj IN HERO
		ELSE "You don't have that object to attack with."
	AND obj IS Weapon
		ELSE "No point attacking anything with that!"
	DOES
		"Violence is not the answer."
END VERB.



SYNTAX
	shoot = shoot (obj)
		WHERE obj ISA THING
			ELSE "You can't shoot at that."

VERB shoot
	DOES
		IF obj IS Shootable THEN
			"You need to specify what to shoot at."
		ELSE
			"You need to specify what you want to shoot"
			IF obj IS named THEN
				SAY obj.
			"with."
				ELSE
			"the $o with."
				END IF.
		END IF.
END VERB.


SYNTAX
	shoot_at0 = shoot 'at' (obj)
		WHERE obj ISA THING
			ELSE "You can't shoot at that."

VERB shoot_at0
	DOES
		"You need to specify what you want to shoot"
		IF obj IS named THEN
			SAY obj.
			"with."
		ELSE
			"the $o with."
		END IF.
END VERB.


SYNTAX
	shoot_at = shoot (obj) 'at' (act)
		WHERE obj ISA OBJECT
			ELSE "You can't shoot that."
		AND act ISA THING
			ELSE "You can't shoot at that."

	shoot_with = shoot (act) 'with' (obj)
		WHERE obj ISA OBJECT
			ELSE "You can't shoot that."
		AND act ISA THING
			ELSE "You can't shoot at that."

VERB shoot_at
	CHECK obj IN HERO
		ELSE "You don't have that."
	AND obj IS Shootable
		ELSE "You can't shoot anything with that."
	DOES
		"Violence is not the answer."
END VERB.

VERB shoot_with
	CHECK obj IN HERO
		ELSE "You don't have that."
	AND obj IS Shootable
		ELSE "You can't shoot anything with that."
	DOES
		"Violence is not the answer."
END VERB.

