-- talk.i
-- Library version 0.4.1

-- 0.4.1 - converted to ALANv3


-- for verbs like 'ask' and 'tell' you need to individually program responses
-- to each topic (ie 'obj') for any actor who you want to respond to that
-- topic. 
-- Remember to use the 'when' clause so that actors only respond when they're
-- being talked to (and not when they're the *subject* of the conversation!)
-- see Section 3.7.1 - 'verb alternatives' - in the Alan manual.
-- eg:
-- ACTOR Simon
--    ....
--    VERB ask
--       WHEN act
--           DOES ONLY
--              IF topic = ball then
--                  "Simon replies ""I love playing ball sports. Football is
--                   my favourite."""
--              ELSIF obj = fred then
--                  .....
--              ELSE
--                  "Simon shrugs. ""Sorry, I don't know anything about 
--                  that."""
--              END IF.
--     END VERB ask.
--     ....
-- END ACTOR Simon.


ADD TO EVERY THING 
IS
	NOT can_talk.
END ADD TO THING.

ADD TO EVERY ACTOR 
IS
	can_talk.
END ADD TO ACTOR.


SYNONYMS
	yell = shout.
	scream = shout.

SYNTAX
	shout = shout.

VERB shout
	DOES
		"You make a lot of noise."
	END VERB.



SYNTAX
	say_word = 'say' (topic)!
		WHERE topic ISA THING 
			ELSE "You can't say that."

Add To Every thing
  VERB say_word
	DOES 
		"$o? That's a nice word!"
  	END VERB.
End Add To.



SYNTAX
	say_to = 'say' (topic)! 'to' (act)
		WHERE topic ISA THING 
			ELSE "You can't say that."
		AND act ISA THING
			ELSE "You can't talk to that."

Add To Every thing
  VERB say_to
	CHECK act HAS can_talk
		ELSE "You can't talk to that."
	DOES 
		IF act IS named THEN
			SAY act.
			"doesn't seem interested."
		ELSE
			"The $2 doesn't seem interested."
		END IF.
  END VERB.
End Add To.



SYNTAX ask = ask (act) about (topic)!
	WHERE topic ISA THING 
		ELSE "You can't ask about that."
	AND act ISA THING
		ELSE "You can't talk to that."

Add To Every thing
  VERB ask
	CHECK act HAS can_talk
		ELSE "You can't talk to that."
	DOES
		IF act IS named THEN
			SAY act.
			"says ""I don't know anything about"
		ELSE
			"The $1 says ""I don't know anything about"
		END IF.
		IF topic IS named THEN
			"them!"""
		ELSE
			"that!"""
		END IF.
  END VERB.
End Add To.




SYNTAX talk_to = talk 'to' (act) about (topic)!
	WHERE topic ISA THING
		ELSE "You can't ask about that."
	AND act ISA THING
		ELSE "You can't talk to that."

SYNTAX tell = tell (act) about (topic)!
	WHERE topic ISA THING 
		ELSE "You can't ask about that."
	AND act ISA THING
		ELSE "You can't talk to that."

Add To Every thing
  VERB tell, talk_to
	CHECK act HAS can_talk
		ELSE "You can't talk to that."
	DOES 
		"""I don't think I need to know about"
		IF topic IS named THEN
			SAY topic.
		ELSE
			"the $2"
		END IF.
		","" says"
		IF act IS named THEN
			SAY act. 
		ELSE
			"the $1"
		END IF. 
		"$$."
  END VERB.
End Add To.




SYNTAX talk_to_a = talk 'to' (act)
	WHERE act ISA THING
		ELSE "You can't talk to that."

Add To Every thing
  VERB talk_to_a
	CHECK act HAS can_talk
		ELSE "You can't talk to that."
	DOES 
		IF act IS named THEN
			SAY act.
			"looks at you, seemingly wondering if you have anything specific 
			to talk about."
		ELSE
			"The $1 looks at you, seemingly wondering if you have anything 
			specific to talk about."
		END IF. 
  END VERB.
End Add To.

