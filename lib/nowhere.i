-- nowhere.i
-- Library version 0.1, 0.2


-- Useful for placing disapearing things
-- Also defines the default directions

SYNONYMS
    n = north.
    s = south.
    e = east.
    w = west.
    ne = northeast.
    se = southeast.
    nw = northwest.
    sw = southwest.
    u = up.
    d = down.


LOCATION nowhere
    EXIT north TO nowhere.
    EXIT south TO nowhere.
    EXIT west TO nowhere.
    EXIT east TO nowhere.
    EXIT northeast TO nowhere.
    EXIT southeast TO nowhere.
    EXIT northwest TO nowhere.
    EXIT southwest TO nowhere.
    EXIT up TO nowhere.
    EXIT down TO nowhere.
END LOCATION nowhere.
