-- talk.i
-- Library version 0.2

-- for verbs like 'ask' and 'tell' you need to individually program responses
-- to each topic (ie 'obj') for any actor who you want to respond to that
-- topic
-- eg:
-- ACTOR Simon
--    ....
--    VERB ask
--       ....
--       DOES ONLY
--          IF topic = ball then
--              "Simon replies ""I love playing ball sports. Football is
--              my favourite."""
--          ELSIF obj = .....
--          ELSE
--              "Simon shrugs. ""Sorry, I don't know anything about that."""
--          END IF.
--     END VERB ask.
--     ....
-- END ACTOR Simon.


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
    sayword = 'say' (topic)!
        WHERE topic ISA OBJECT OR ACTOR  
            ELSE "You can't say that."

VERB sayword
    DOES 
        "$o? That's a nice word!"
    END VERB.


SYNTAX
    sayto = 'say' (topic)! 'to' (act)
        WHERE topic ISA OBJECT OR ACTOR  
            ELSE "You can't say that."
        AND act ISA ACTOR OR OBJECT
            ELSE "You can't talk to that."

VERB sayto
    DOES 
        SAY act.
        "doesn't seem interested."
    END VERB.


SYNTAX ask = ask (act) about (topic)!
    WHERE topic ISA OBJECT OR ACTOR  
        ELSE "You can't ask about that."
    AND act ISA ACTOR OR OBJECT
        ELSE "You can't talk to that."

VERB ask
    DOES
        "The $1 says '$2? I don't know anything about that!'"
END VERB.


SYNTAX talkabout = talk about (topic)! with (act) 
    WHERE topic ISA OBJECT OR ACTOR  
        ELSE "You can't ask about that."
    AND act ISA ACTOR OR OBJECT
        ELSE "You can't talk to that."

VERB talkabout
    DOES 
        """I don't think I need to know about $1."" Says a puzzled" SAY act. "$$."
END VERB.

SYNTAX talkto = talk 'to' (act) about (topic)!
    WHERE topic ISA OBJECT OR ACTOR
        ELSE "You can't ask about that."
    AND act ISA ACTOR OR OBJECT
        ELSE "You can't talk to that."

SYNTAX tell = tell (act) about (topic)!
    WHERE topic ISA OBJECT OR ACTOR  
        ELSE "You can't ask about that."
    AND act ISA ACTOR OR OBJECT
        ELSE "You can't talk to that."

VERB tell, talkto
    DOES 
        """I don't think I need to know about $2.""
	Says a puzzled" SAY act. "$$."
END VERB.
