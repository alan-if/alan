-- kiss.i
-- Library version 0.1

SYNTAX
    kiss = kiss (obj)
        WHERE obj ISA OBJECT OR ACTOR
            ELSE "You can't kiss that!"

VERB kiss
DOES
    IF obj=hero THEN
        "Well, if you must!"
    ELSE
        IF obj IS InAnimate THEN
            "You kiss the"
            SAY obj.
        ELSE    
            SAY obj.
            "avoids your advances."
        END IF.
    END IF.
END VERB.


