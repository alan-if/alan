
-- ATTACK
SYNONYMS
	break, smash, hit, fight, wreck, crack, destroy, murder, kill,
	torture, punch, thump, strike = attack.

SYNTAX
	attack = attack (act)*
	WHERE act ISA ACTOR
		ELSE "You should not attack defenseless objects."

VERB attack
	DOES
		"Enhance your calm, please. Chill out."
END VERB.


-- ATTACK WITH
SYNTAX
	attack_with = attack (act) 'with' (obj)*
		WHERE act ISA ACTOR
			ELSE "You should not attack defenseless objects."

VERB attack_with
	CHECK obj in inventory
		ELSE "You don't have that object to attack with."
	DOES
		"Enhance your calm, please. Chill out."
END VERB.

