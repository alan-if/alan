SYNONYMS
	inquire, query = ask.


SYNTAX
	ask = ask (act)*
		WHERE act ISA ACTOR
			ELSE "You know, things don't talk. people do."

-- ASK
VERB ask
	DOES
		IF act IS NOT propername THEN
			"The $o ignores your question."
		ELSE
			"$o ignores your question."
		END IF.
END VERB.


-- ASK ABOUT
SYNTAX
	ask_about = ask (act) about (obj)*
		WHERE act ISA ACTOR
			ELSE "You know, things don't talk. people do."

VERB ask_about
	DOES
		IF act IS NOT propername THEN
			"The $1 ignores your question."
		ELSE
			"$1 ignores your question."
		END IF.
END VERB.


-- TELL
SYNONYMS
	inform = tell.

SYNTAX
	tell = tell (act)*
		WHERE act ISA ACTOR
			ELSE "You know, things don't talk. people do."

VERB tell
	DOES
		IF act IS NOT propername THEN
			"The $o is not impressed."
		ELSE
			"$o is not impressed."
		END IF.
END VERB.


-- TELL ABOUT
SYNTAX
	tell_about = tell (act) about (obj)*
		WHERE act ISA ACTOR
			ELSE "You know, things don't talk. people do."

VERB tell_about
	DOES
		IF act IS NOT propername THEN
			"The $1 is not impressed."
		ELSE
			"$1 is not impressed."
		END IF.
END VERB.


-- TALK TO
SYNONYMS
	speak = talk.

SYNTAX
	talk = 'talk' 'to' (act)*
		WHERE act ISA ACTOR
			ELSE "You know, things don't talk. people do."

VERB talk
	DOES
		IF act IS NOT propername THEN
			"The $o does not seem to want to talk."
		ELSE
			"$o does not seem to want to talk."
		END IF.
END VERB.


-- TALK ABOUT
SYNTAX
	talk_about = 'talk to' (act) about (obj)*
		WHERE act ISA ACTOR
			ELSE "You know, things don't talk. people do."

VERB talk_about
	DOES
		IF act IS NOT propername THEN
			"The $1 does not seem to want to talk."
		ELSE
			"$1 does not seem to want to talk."
		END IF.
END VERB.
