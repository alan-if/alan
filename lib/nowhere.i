-- Useful for placing disapearing things
-- Also defines the default directions

SYNONYMS
    north = n.
    south = s.
    east = e.
    west = w.
    northeast = ne.
    southeast = se.
    northwest = nw.
    southwest = sw.
    up = u.
    down = d.


LOCATION nowhere
    EXIT n TO nowhere.
    EXIT s TO nowhere.
    EXIT w TO nowhere.
    EXIT e TO nowhere.
    EXIT ne TO nowhere.
    EXIT se TO nowhere.
    EXIT nw TO nowhere.
    EXIT sw TO nowhere.
    EXIT u TO nowhere.
    EXIT d TO nowhere.
END LOCATION nowhere.
