-- read.i
-- Library version 0.1, 0.2
-- Changed read check else to have "the $1" 0.3


OBJECT ATTRIBUTES
    NOT readable.

SYNTAX
    read = read (obj)
	WHERE obj ISA OBJECT
	    ELSE "You can't read that."


VERB read
    CHECK obj IS readable
        ELSE "There is nothing written on the $1."
    DOES
        "You read the $1."
END VERB.


