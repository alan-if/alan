-- std.i
-- Library version 0.5.2

-- All verb definitions have a small, simple default body. To make
-- them do other things use DOES ONLY in your specialised verb body.

-- player character, etc
import 'global.i'.

-- Standard verbs
import 'look.i'.
import 'take.i'.    -- + pick up, drop, put down.
import 'open.i'.    -- + close.
import 'lock.i'.    -- + unlock.
import 'eat.i'.    -- + drink.
import 'throw.i'.
import 'push.i'.
import 'touch.i'.
import 'examine.i'. -- + look at, search
import 'read.i'.
import 'put.i'.     -- + put near,behind,on,under 
import 'give.i'.
import 'talk.i'.    -- + ask, tell, say, shout, 
import 'attack.i'.  -- + shoot
import 'kiss.i'.
import 'turn.i'.    -- + switch
import 'listen.i'.
import 'smell.i'.
import 'knock.i'.
import 'jump.i'.
import 'wear.i'.   -- + remove, undress, put on

import 'help.i'.    -- + notes, hint


-- Scenery
import 'scenery.i'.

-- Inventory verb and inventory limits (including clothing items)
import 'invent.i'.


-- The limbo location and directions
-- Defines directions as full words, and short directions as synonyms to these.
-- So remember to use the full words in your exits or you will have E 333's
-- (e.g. 'e' defined both as a synonym and another word class)
import 'nowhere.i'.


-- Verbose and brief mode
import 'brief.i'.


-- Score, save, restore etc.
import 'meta.i'.
