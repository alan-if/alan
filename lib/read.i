OBJECT ATTRIBUTES
    NOT readable.

SYNONYMS
	browse = read.

SYNTAX
    read = read (obj)
	WHERE obj ISA OBJECT
	    ELSE "You can't read that."


VERB read
    CHECK obj IS readable
        ELSE "There is nothing written on $1."
END VERB.


