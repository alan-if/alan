-- std.i
-- Library version 0.1

-- All verb definitions have a small, simple default body. To make
-- them do other things use DOES ONLY in your specialised verb body.


-- Standard verbs
$include 'look.i'
$include 'take.i'    -- + pick up, drop, put down
$include 'open.i'    -- + close
$include 'lock.i'    -- + unlock
$include 'eat.i'     -- + drink
$include 'throw.i'
$include 'push.i'
$include 'touch.i'
$include 'examine.i' -- + look at, search
$include 'read.i'
$include 'put.i'     -- + put near,behind,on,under 
$include 'give.i'
$include 'talk.i'    -- + ask, tell, say, shout, 
$include 'attack.i'  -- + shoot
$include 'kiss.i'
$include 'turn.i'    -- + switch

$include 'help.i'    -- + notes, hint


-- Inventory verb and inventory limits
$include 'invent.i'


-- The limbo location and directions
-- Defines directions as full words, and short directions as synonyms to these.
-- So remember to use the full words in your exits or you will have E 333's
-- (e.g. 'e' defined both as a synonym and another word class)
$include 'nowhere.i'


-- Verbose and brief mode
$include 'brief.i'


-- Score, save, restore etc.
$include 'meta.i'
