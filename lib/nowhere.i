-- nowhere.i
-- Library version 0.3.2

-- 0.3.2
--      converted to Alan v3 syntax


-- Useful for placing disappearing things
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


THE nowhere ISA LOCATION.
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
END THE nowhere.
