SYNONYMS
    l = 'look'.
    
SYNTAX
    'look' = 'look'.

OBJECT ATTRIBUTES
    lightsource 0.	-- This is a boolean, but we need to sum the number of
			-- light sources present so it is set to 1 for objecs
			-- providing light.


LOCATION ATTRIBUTES
    lit.		-- Set this to "NOT lit." for dark places.


VERB 'look'
    CHECK
	SUM OF lightsource HERE > 0 OR LOCATION IS lit
    ELSE
	"You cannot see anything without any light."
    DOES
        LOOK.
END VERB.
