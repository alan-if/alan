-- read.i
-- Library version 0.1, 0.2


OBJECT ATTRIBUTES
    NOT readable.

SYNTAX
    read = read (obj)
	WHERE obj ISA OBJECT
	    ELSE "You can't read that."


VERB read
    CHECK obj IS readable
        ELSE "There is nothing written on $1."
    DOES
        "You read the $1."
END VERB.


