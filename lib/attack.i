-- attack.i
-- Library version 0.1

DEFAULT ATTRIBUTES
    NOT Shootable.

OBJECT ATTRIBUTES
    NOT Weapon.

SYNONYMS
	kill, fight, hit = attack.
	fire = shoot.

SYNTAX
	attack = attack (act)
		WHERE act ISA ACTOR
			ELSE "You should not attack defenseless objects."

VERB attack
	DOES
        "Violence is not the answer."
END VERB.

SYNTAX
	attack_with = attack (act) 'with' (obj)
        WHERE act ISA ACTOR OR OBJECT
            ELSE "You can't attack that."
        AND obj ISA OBJECT
            ELSE "You can't attack anything with that!"

VERB attack_with
    CHECK obj IN inventory
		ELSE "You don't have that object to attack with."
    AND obj IS Weapon
        ELSE "No point attacking anything with that!"
	DOES
        "Violence is not the answer."
END VERB.


SYNTAX
	shoot = shoot (obj)
		WHERE obj ISA OBJECT OR ACTOR
            ELSE "You can't shoot at that."

VERB shoot
	DOES
        IF obj IS Shootable THEN
            "You need to specify what to shoot at."
        ELSE
            "You need to say what you want to shoot"
            SAY obj.
            "with."
        END IF.
END VERB.


SYNTAX
	shoot_at = shoot (obj) 'at' (act)
		WHERE obj ISA OBJECT
            ELSE "You can't shoot that."
		AND act ISA OBJECT OR ACTOR
            ELSE "You can't shoot at that."

	shoot_with = shoot (act) 'with' (obj)
		WHERE obj ISA OBJECT
            ELSE "You can't shoot that."
		AND act ISA OBJECT OR ACTOR
            ELSE "You can't shoot at that."

VERB shoot_at
    CHECK obj IN Inventory
        ELSE "You don't have that"
    AND obj IS Shootable
        ELSE "You can't shoot anything with that."
	DOES
        "Violence is not the answer."
END VERB.

VERB shoot_with
    CHECK obj IN Inventory
        ELSE "You don't have that"
    AND obj IS Shootable
        ELSE "You can't shoot anything with that."
	DOES
        "Violence is not the answer."
END VERB.

