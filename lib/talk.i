-- talk.i
-- Library version 0.1

-- for verbs like 'ask' and 'tell' you need to individually program responses
-- to each topic (ie 'obj') for any actor who you want to respond to that
-- topic
-- eg:
-- ACTOR Simon
--    ....
--    VERB ask
--       ....
--       DOES ONLY
--          IF obj = ball then
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
    sayword = 'say' (obj)!
        WHERE obj ISA OBJECT OR ACTOR  
            ELSE "You can't say that."

VERB sayword
    DOES 
        "$o? That's a nice word!"
    END VERB.


SYNTAX
    sayto = 'say' (obj)! 'to' (act)
        WHERE obj ISA OBJECT OR ACTOR  
            ELSE "You can't say that."
        AND act ISA ACTOR
            ELSE "You can't talk to that."

VERB sayto
    DOES 
        SAY act.
        "doesn't seem interested."
    END VERB.


SYNTAX ask = ask (act) about (obj)!
    WHERE obj ISA OBJECT OR ACTOR  
        ELSE "You can't ask about that."
    AND act ISA ACTOR
        ELSE "You can't talk to that."

VERB ask
    DOES 
        "The $1 says '$2? I don't know about that!'"
END VERB.


SYNTAX talkabout = talk about (obj)! with (act) 
    WHERE obj ISA OBJECT OR ACTOR  
        ELSE "You can't ask about that."
    AND act ISA ACTOR
        ELSE "You can't talk to that."

VERB talkabout
    DOES 
        """I don't think I need to know about $2."" Says a puzzled" SAY act. "$$."
END VERB.

SYNTAX talkto = talk 'to' (act) about (obj)!
    WHERE obj ISA OBJECT OR ACTOR  
        ELSE "You can't ask about that."
    AND act ISA ACTOR
        ELSE "You can't talk to that."

SYNTAX tell = tell (act) about (obj)!
    WHERE obj ISA OBJECT OR ACTOR  
        ELSE "You can't ask about that."
    AND act ISA ACTOR
        ELSE "You can't talk to that."

VERB tell, talkto
    DOES 
        """I don't think I need to know about $2."" Says a puzzled" SAY act. "$$."
END VERB.
