-- ALAN Standard Library v2.0
-- Classes (file name: 'lib_classes.i')


-- This library file defines various object and actor classes. 
-- Many of these classes are frequently  used in verb definitions in 'lib_verbs.i' 
-- so they should be edited or removed with caution. However, to ease things up, 
-- it is mentioned at the beginning of every class below if and where the class
-- is cross-referenced in the other library files.



-- Contents:

	



-- 1. OBJECT CLASSES
-- ================= 



-- CLOTHING    
	-- Is a piece of clothing that behaves according to Alan Bampton's 'xwear.i' extension.
     -- The said extension has been fully assimilated to this library. 
     -- This extension prevents clothes from being worn in an illogical order, for example you 
	-- cannot put on a shirt if you are already wearing a jacket, and so forth.
	-- This only applies to the hero; NPCs cannot be made to wear clothing in layers.
	-- Also the verbs 'wear', 'remove' and 'undress' are defined here.


-- DEVICE  
	-- Is a  machine or an electronic device, for example a TV. Can be turned 
	-- (=switched) on and off if it is not broken.
	-- Attributes: 'on' and NOT 'on', NOT broken.
      -- Is described by default as being either on or off when examined. 


-- DOOR 
	-- Can be opened, closed, and optionally locked and unlocked. 
	-- Is by default not open, not lockable.
	-- all default attributes: openable, NOT open, NOT lockable, NOT locked; not takeable.
	-- Is described by default as being either open or closed when examined.


-- LIQUID 
	-- Can only be taken if it is in a container. You can fill something with it, 
	-- and you can pour it somewhere.
	-- A liquid is by default NOT drinkable.


-- LIGHTSOURCE 
	-- IS natural or NOT natural 
	-- (a natural lightsource is for example a match or a torch).
	-- Can be turned on and off, lighted and extinguished (= put out) if it 
      -- is not broken. A natural lightsource 
	-- cannot be turned on or off, it can only be lighted and extinguished (= put out).
	-- When examined, a lightsource is automatically supplied with a description of
	-- whether it is providing light or not.


-- LISTED_CONTAINER
	-- Is a container object. The contents of a listed_container will be listed both after 
	-- 'look' (= in the room description), 'look in' and 'examine' (if the container is open). 
	-- (The contents of a normal container object are not listed after 'examine' by default, but only 
	-- after 'look' (=room description) and 'look in').


-- SOUND 
	-- Can be listened to but not examined, searched, smelled or manipulated.
     -- (Can be turned on and off if desirable.)


-- SUPPORTER 
	-- You can put things on this and you can stand on this. It is declared a container, 
	-- so you can take things from it, as well. When there's something on a supporter, 
      -- a default listing of it will appear in the room description and after 'examine'.


-- WEAPON  
	-- IS fireable (for example a cannon) or NOT fireable (for example a baseball bat).


-- WINDOW 
	-- Can be opened, closed, looked through and out of.
	-- Will be by default described as being either open or closed when examined.



-- 2. ACTOR CLASSES
-- ================

-- the ACTORS are defined to be NOT inanimate CONTAINERS (so that they can for example
-- receive and carry things. 
--
-- Actors are usually preceded by an article in-game:
-- for example "You see a man here."
--	  "There is nothing special about the dog."
-- unless they are declared as 'named'.
--
-- The following classes for actors are defined in this library:


-- PERSON 
	-- is able to talk (= 'CAN talk'). 


-- FEMALE
	-- a subclass of person (= is able to talk)
      -- can be referred to with the pronoun 'her'


-- MALE
	-- a subclass of person (= is able to talk)
      -- can be referred to with the pronoun 'him'





-- The contents end here. 



-- =============================================================

-- =============================================================
--
-- 1. Object classes
--
-- =============================================================

-- =============================================================






-- ==============================================================


----- CLOTHING     


-- ==============================================================


-- (See the file 'lib_verbs.i', verbs 'inventory' and 'take' where the 
-- container 'worn', defined below, is used in the verb definitions.)


-- To use this class, see the documentation text right after the
-- code below.

-- This class makes use of Alan Bampton's 'xwear.i' extension
-- written originally for ALAN V2, converted here to V3 and 
-- assimilated fully to the present library. Thanks to Alan Bampton
-- for the permission to use the code here.


-----------------------------------------------------------------
-- First, we declare the container for clothing.
-----------------------------------------------------------------


-- an entity is present everywhere and thus the hero's clothing is always accessible:

THE worn ISA ENTITY							
	CONTAINER TAKING CLOTHING.			
		HEADER SAY hero_worn_header OF my_game.
		ELSE SAY hero_worn_else OF my_game.
END THE.





-------------------------------------------------------------------
-- Now, we define some common attributes for clothing as well as 
-- how the verbs 'remove', 'undress' and 'wear' (and their synonyms) behave with this class.
-------------------------------------------------------------------


EVERY clothing ISA OBJECT

	IS wearable.
	IS NOT donned.     -- = not worn by an NPC
	IS sex 0.
	IS headcover 0.
	IS handscover 0.
	IS feetcover 0.
	IS topcover 0.
	IS botcover 0.


	INITIALIZE  
		-- all objects found in a piece of clothing, for example a wallet in a jacket,
		-- will be allowed back in the piece of clothing once taken from there:

		
		FOR EACH o ISA OBJECT, DIRECTLY IN THIS
			DO 
				INCLUDE o IN allowed OF THIS.
		END FOR.
		


		-- all clothing acquired and worn by an NPC mid-game is checked to show
           -- show correctly when the possessions of the NPC are listed:


		SCHEDULE npc_worn_check AFTER 0.
		


	CONTAINER			
	-- to allow for example a wallet to be put into a jacket

	-- If the clothing contains something, for example if a jacket contains a wallet,
    	-- the wallet will be mentioned by default when the jacket is examined: 

	VERB examine
		DOES AFTER
			IF THIS IS NOT OPAQUE
				THEN 
					IF COUNT ISA OBJECT, DIRECTLY IN THIS > 0		
						THEN LIST THIS.					
					END IF.							
			END IF.									
	END VERB.



  	VERB wear

		CHECK sex OF THIS = sex OF hero OR sex OF THIS = 0
			ELSE SAY check_clothing_sex OF my_game.
		
		DOES ONLY


--------------------------------------------------------------------
-- 'wear_flag' is a multi-purpose flag used for several purposes in 
-- this library, here it is reset to 0 before proceeding as a matter
-- of 'housekeeping' for the code.
--------------------------------------------------------------------


		SET wear_flag OF hero TO 0.


--------------------------------------------------------------------
-- First check to see if the player is carrying the item already, if
-- not, set the 'wear_flag' to 1 to indicate the item was picked up
-- in this turn.
--------------------------------------------------------------------


		IF THIS NOT IN hero 
			THEN
				SET wear_flag OF hero TO 1.
		END IF.

	
--------------------------------------------------------------------
--  Now see if the player can put this item on by testing 
--  all of its coverage attributes against the player's state.
--------------------------------------------------------------------


--------------------------------------------------------------------
-- First check the 'topcover' attributes, if 'obj' fails this test
-- then it means the hero is already wearing clothes that cover the
-- topcover area and those clothes are of the same layer or a layer 
-- that belongs on top of the 'obj' item. In either case it would 
-- NOT be possible to put on the 'obj'. To 'flag' this condition add
-- 5 to the 'wear_flag' attribute as an indicator this test failed.
--------------------------------------------------------------------


		IF topcover OF THIS <> 0 AND topcover OF THIS <= SUM OF topcover DIRECTLY IN worn 
			THEN
				INCREASE wear_flag OF hero BY 5.
		END IF.

	
--------------------------------------------------------------------
-- Perform a similar test for other attributes.
--------------------------------------------------------------------


		--IF THIS IN tempworn 
			--THEN
			
		IF handscover OF THIS <> 0 AND handscover OF THIS <= SUM OF handscover DIRECTLY IN worn 
			THEN
				INCREASE wear_flag OF hero BY 5.
		END IF.
	

		IF feetcover OF THIS <> 0 AND feetcover OF THIS <= SUM OF feetcover DIRECTLY IN worn 
			THEN
				INCREASE wear_flag OF hero BY 5.	
		END IF.

	
		IF headcover OF THIS <> 0 AND headcover OF THIS <= SUM OF headcover DIRECTLY IN worn 
			THEN
				INCREASE wear_flag OF hero BY 5.	
		END IF.


--------------------------------------------------------------------
--  botcover is a special case, adjust the 'tempcovered OF hero' 
--  attribute so that the code rejects non sensible options.
--  First of all, discount any coatlike clothes as these never 
--  affect ability to put on other lower body only garments.
--------------------------------------------------------------------


		SET tempcovered OF hero TO SUM OF botcover DIRECTLY IN worn.
		
		IF tempcovered OF hero >63 and botcover OF THIS < 33
			THEN 
				SET tempcovered OF hero TO tempcovered OF hero -64.
		END IF.


--------------------------------------------------------------------
-- Now discount any dress/ skirt coverall like clothes as these do 
-- not technically affect ability to put on lower body only clothes.
-- Special clause here excludes the full body coverage 'teddy' type
-- garment - as a skirt WOULD prevent that from being removed. 
-- ( dress/coat garments automatically prevent this by virtue of 
-- having higher 'topcover' settings than the teddy )
--------------------------------------------------------------------


		IF tempcovered OF hero >31 AND botcover OF THIS < 16 and botcover OF THIS <> 4 
			THEN
				SET tempcovered OF hero TO tempcovered OF hero -32.
		END IF.


--------------------------------------------------------------------
-- IF tempcovered OF hero is still > 15 then must have trousers 
-- type clothing on - therefore disallow wearing dress type clothing
-- because, although technically possible, it is not very sensible. 
--------------------------------------------------------------------


		IF tempcovered OF hero >15 AND botcover OF THIS > 16 
			THEN
				SET tempcovered OF hero TO tempcovered OF hero +16.
		END IF.


--------------------------------------------------------------------
--  From here down, clothes DO work as they do for other areas.
--------------------------------------------------------------------


		IF botcover OF THIS <> 0  AND botcover OF THIS <= tempcovered OF hero 
			THEN
				INCREASE wear_flag OF hero BY 5.		
		END IF.

	
--------------------------------------------------------------------
-- At this point, 'wear_flag' will be 0 if the obj was held by the
-- player and can be put on, or l if he picked it up this turn and 
-- it can be put on. Any higher value means one or more of the 
-- tests failed and the player cannot put on these clothes. 
--------------------------------------------------------------------


		IF wear_flag OF hero >1 
			THEN
				IF THIS NOT IN hero 
					THEN "You pick up the" SAY THE THIS. "."
				END IF.
				
				LOCATE THIS IN hero.
				EMPTY worn IN tempworn.	
				LIST tempworn.

				"Trying to put" SAY THE THIS. "on isn't very sensible."

				EMPTY tempworn IN worn.

		ELSIF wear_flag OF hero = 1 
			THEN
				LOCATE THIS IN worn.

				"You pick up the" SAY THE THIS.

				IF THIS IS NOT plural 
					THEN "and put it on."
					ELSE "and put them on."
				END IF.

		ELSE
			LOCATE THIS IN worn.
			"You put on" SAY THE THIS. "."
		END IF.

END VERB.



VERB remove
	CHECK THIS IN worn
		ELSE SAY check_obj_in_worn OF my_game.
	AND CURRENT LOCATION IS lit
		ELSE SAY check_current_loc_lit OF my_game.

	DOES ONLY
	
	SET wear_flag OF hero TO 0.

	
--------------------------------------------------------------------
-- Check the total 'topcover' of items worn. Because of the number
-- sequence used, by dividing the sum of the worn attributes by two 
-- and then comparing the result to the individual 'topcover' of the
-- obj in question, ( the former can only ever be greater than the 
-- latter if an article of clothing is worn that goes over 'obj' ) 
-- it's easy to tell if the obj ought to be removable. A temporary
-- attribute is used here because it needs to be manipulated. Once
-- again 'wear_flag' is used to indicate the results.
--------------------------------------------------------------------


	SET tempcovered OF hero TO SUM OF topcover DIRECTLY IN worn /2.
	IF topcover OF THIS <> 0 AND topcover OF THIS < tempcovered OF hero
		 THEN
			INCREASE wear_flag OF hero BY 1.
	END IF.

		
--------------------------------------------------------------------
-- Perform a similar test for other attributes.
--------------------------------------------------------------------


	SET tempcovered OF hero TO SUM OF handscover DIRECTLY IN worn /2.
	IF handscover OF THIS <> 0 AND handscover OF THIS < tempcovered OF hero 
		THEN
			INCREASE wear_flag OF hero BY 1.
	END IF.		


	SET tempcovered OF hero TO SUM OF feetcover DIRECTLY IN worn /2.
	IF feetcover OF THIS <> 0 AND feetcover OF THIS < tempcovered OF hero 
		THEN
			INCREASE wear_flag OF hero BY 1.		
	END IF.	


	SET tempcovered OF hero TO SUM OF headcover DIRECTLY IN worn /2.
	IF headcover OF THIS <> 0 AND headcover OF THIS < tempcovered OF hero 
		THEN
			INCREASE wear_flag OF hero BY 1.
	END IF.		


--------------------------------------------------------------------
-- botcover is a special case - first discount any coatlike clothes
-- as these do not affect ability to take off other lower garments.
--------------------------------------------------------------------


	SET tempcovered OF hero TO SUM OF botcover DIRECTLY IN worn.
	IF tempcovered OF hero >63 
		THEN 
			SET tempcovered OF hero TO tempcovered OF hero -64.
	END IF.


--------------------------------------------------------------------
-- Now discount any dress/ skirt coverall like clothes as these do 
-- not affect ability to take off other lower garments.	The 'teddy'
-- type garment is expressly NOT included in the exclusion here.
--------------------------------------------------------------------


	IF tempcovered OF hero >31 and botcover OF THIS <>4 
		THEN
			SET tempcovered OF hero TO tempcovered OF hero -32.
	END IF.


--------------------------------------------------------------------
-- Now process the manipulated value just as was done for the others
--------------------------------------------------------------------


	SET tempcovered OF hero TO tempcovered OF hero /2.
	IF botcover OF THIS <> 0 AND botcover OF THIS < tempcovered OF hero 
		THEN
			INCREASE wear_flag OF hero BY 1.
	END IF.


--------------------------------------------------------------------
-- Depending on the value of 'wear_flag' print and process the obj
-- as needed. If 'wear_flag' is NOT 0 then the clothes cannot be 
-- removed.
--------------------------------------------------------------------


	IF wear_flag OF hero > 0 
		THEN
			LIST worn.
			"Trying to take" SAY THE THIS. "off isn't very sensible."
		ELSE
			LOCATE THIS IN hero.
			"You take off" SAY THE THIS. "." 
	END IF.
END VERB.


END EVERY.



--------------------------------------------------------------------
-- These attributes are used internally in the library - ignore! 
--------------------------------------------------------------------

ADD TO EVERY ACTOR
	IS tempcovered 0.
	IS wear_flag 0.
	IS sex 0.
END ADD TO.
	

--------------------------------------------------------------------
-- A container used to provide a temporary storage space - ignore! 
--------------------------------------------------------------------

THE tempworn ISA OBJECT
	CONTAINER TAKING CLOTHING.
	HEADER "You're already wearing"
END THE tempworn.


--------------------------------------------------------------------
-- An event checking that clothing acquired and worn by an NPC 
-- mid-game is recognised to be worn by the NPC:
--------------------------------------------------------------------


EVENT npc_worn_check
   FOR EACH ac ISA ACTOR 
	DO
		FOR EACH cl ISA CLOTHING, IN wearing OF ac
			DO MAKE cl donned.
		END FOR.
   END FOR.
   SCHEDULE npc_worn_check AFTER 1.
END EVENT.


--------------------------------------------------------------------

-----------------------------------------------------------------------
-- INSTRUCTIONS FOR USING THE CLOTHING CLASS 
-----------------------------------------------------------------------


-- Here is a quick overview for using the class 'clothing'. 

-- A piece of clothing in your game code should look 
-- something similar to the following four examples:


-- THE jacket ISA CLOTHING AT lobby
-- 	IS topcover 32.
-- END THE.


-- use IN to refer to containers:

-- THE jeans ISA CLOTHING IN wardrobe		
--	IS botcover 16.					
-- END THE.


-- IN worn = worn by the player character (hero):

-- THE hat ISA CLOTHING IN worn  		 
--	IS headcover 2.
-- END THE.


-- worn by an NPC called Joe:

-- THE sweater ISA CLOTHING IN joe 
	-- Don't declare clothing attributes for NPCs (unless the hero is meant to take 
	-- and wear the NPC's clothing).
	-- NPCs cannot wear clothing in layers!   			
-- END THE. 			
		
-- THE joe ISA ACTOR AT room1
	-- IS wearing {sweater}.
-- END THE joe.

-- Note that if the piece of clothing worn
-- by an NPC is not meant to be takeable by the
-- player character, you should declare the
-- the piece of clothing to be NOT takeable.


-- In defining a piece of clothing, you should
--
--  1) define it ISA CLOTHING (and not: ISA OBJECT)
--
--  2) give it one of five attributes 'headcover', 'topcover', botcover', 'footcover'
--  or 'handcover'; sometimes two of these are needed.
--  Which attribute(s) to use depends on the type of clothing; see the clothing table below.

--  3) A number 2, 4, 8, 16, 32 or 64 needs to be added after the above attribute.
--  You cannot decide the number yourself; look it up from the clothing table below.
--  If the value of an attribute for a piece of clothing is 0 in the table, don't mention 
--  this attribute in connection with your clothing object.


-- The above is enough; the rest is then handled automatically by the library.


-- The quick guide ends here.  


-- The clothing table
-----------------------


-- Here is the chart showing a selection of fairly typical clothing items and the values to 
-- set to obtain appropriate behaviour. Should you wish to create an article of clothing not
-- listed, usually a bit of lateral thought as to what it is most like and where it fits into 
-- the scheme of things will suggest a workable set of values, but be aware that you MUST use 
-- values in this chart, simply adding things with intermediate values is probably going to 
-- create nasty bugs:


-- Clothing 			Headcover	Topcover 	Botcover 	Footcover 	Handcover

-- hat				2		0		0		0		0
-- vest/bra             	0        	2         	0		0		0
-- undies/panties		0		0		2		0		0
-- teddy			0		4		4		0		0
-- blouse/shirt/T-shirt	0	8		0		0		0
-- dress/coveralls		0		8		32		0		0
-- skirt			0		0		32		0		0
-- trousers/shorts		0		0		16		0		0
-- sweater/pullover		0		16		0		0		0
-- jacket			0		32		0		0		0
-- coat				0		64		64		0		0
-- socks/stockings		0		0		0		2		0
-- tights/pantiehose		0		0		8		2		0
-- shoes/boots			0		0		0		4		0			
-- gloves			0		0		0		0		2




-- =============================================================


----- DEVICE


-- =============================================================


-- (This class is not cross-referenced elsewhere in this or any other library file.)


EVERY device ISA OBJECT
	IS NOT 'on'.
	

	VERB examine
		DOES AFTER
			IF THIS IS NOT plural
				THEN "It is" 
				ELSE "They are"
			END IF.
			
			IF THIS IS 'on'
				THEN "currently on."
				ELSE "currently off."
			END IF.
	END VERB.


	VERB turn_on
		CHECK THIS IS NOT 'on'
			ELSE 
				IF THIS IS NOT plural
					THEN SAY check_device_not_on_sg OF my_game. 
					ELSE SAY check_device_not_on_pl OF my_game.
				END IF.
		AND CURRENT LOCATION IS lit
			ELSE SAY check_current_loc_lit OF my_game.
		AND THIS IS reachable AND THIS IS NOT distant
			ELSE 
				IF THIS IS NOT reachable
					THEN 
						IF THIS IS NOT plural
							THEN SAY check_obj_reachable_sg OF my_game.
							ELSE SAY check_obj_reachable_pl OF my_game.
						END IF.
				ELSIF THIS IS distant
					THEN
						IF THIS IS NOT plural
							THEN SAY check_obj_not_distant_sg OF my_game. 
							ELSE SAY check_obj_not_distant_pl OF my_game. 
						END IF.
				END IF.
		AND THIS IS NOT broken
			ELSE SAY check_obj_not_broken OF my_game.
		DOES ONLY
			"You turn on" SAY THE THIS. "."
			MAKE THIS 'on'.
	END VERB.


	VERB turn_off
		CHECK THIS IS 'on'
			ELSE 
				 IF THIS IS NOT plural
					THEN SAY check_device_on_sg OF my_game.
					ELSE SAY check_device_on_pl OF my_game.
				 END IF.
		AND CURRENT LOCATION IS lit
			ELSE SAY check_current_loc_lit OF my_game.
		AND THIS IS reachable AND THIS IS NOT distant
			ELSE 
				IF THIS IS NOT reachable
					THEN
						IF THIS IS NOT plural
							THEN SAY check_obj_reachable_sg OF my_game.
							ELSE SAY check_obj_reachable_pl OF my_game.
						END IF.
				ELSIF THIS IS distant
					THEN
						IF THIS IS NOT plural
							THEN SAY check_obj_not_distant_sg OF my_game. 
							ELSE SAY check_obj_not_distant_pl OF my_game. 
						END IF.
				END IF.
		DOES ONLY 
			"You turn off" SAY THE THIS. "."
			MAKE THIS NOT 'on'.
	END VERB.


-- The following verb switches a device off if the device is on, and vice versa.

	
	VERB switch
		CHECK CURRENT LOCATION IS lit
			ELSE SAY check_current_loc_lit OF my_game.
		AND THIS IS reachable AND THIS IS NOT distant
			ELSE
				IF THIS IS NOT reachable
					THEN  
						IF THIS IS NOT plural
							THEN SAY check_obj_reachable_sg OF my_game.
							ELSE SAY check_obj_reachable_pl OF my_game.
						END IF.
				ELSIF THIS IS distant
					THEN
						IF THIS IS NOT plural
							THEN SAY check_obj_not_distant_sg OF my_game. 
							ELSE SAY check_obj_not_distant_pl OF my_game. 
						END IF.
				END IF.
		AND THIS IS NOT broken
			ELSE SAY check_obj_not_broken OF my_game.
		DOES ONLY
			IF THIS IS 'on'
				THEN "You switch off" SAY THE THIS. "."
					MAKE THIS NOT 'on'.
				ELSE "You switch on" SAY THE THIS. "."
					MAKE THIS 'on'.
			END IF.
	END VERB.

END EVERY.



-- =============================================================


----- DOOR


-- =============================================================


-- (This class is not cross-referenced elsewhere in this or any other library file.)


EVERY door ISA OBJECT
	IS openable.
	IS NOT open.
	IS NOT lockable.
	IS NOT locked.
	IS NOT takeable.


	HAS otherside null_door.
	-- The other side of the door in the next room will be automatically taken care of
	-- so that it shows correctly in any room or object descriptions. 
	-- 'null_door' is a dummy default that can be ignored.
			


	INITIALIZE

		-- ensuring that the author didn't forget to declare a locked door closed (= NOT open), as well. This is
		-- just double-checking, as any door is by default closed (= "NOT open") at the start of the game:

		IF THIS IS locked
			THEN
				IF THIS IS open
					THEN MAKE THIS NOT open.
				END IF.
		END IF.

		-- ensuring that if a door has an otherside attribute declared, this otherside will have the original
		-- door as its otherside in turn:

		IF otherside OF THIS <> null_door
			THEN 
				SET otherside OF otherside OF THIS TO THIS.

				
			-- next, ensuring that some attributes are correctly assigned to the otherside of the door, as well.
			-- Only some non-default cases need to be addressed here:

				IF THIS IS NOT openable
					THEN MAKE otherside OF THIS NOT openable.
				END IF.

				IF THIS IS open
					THEN MAKE otherside OF THIS open.
				END IF.

				IF THIS IS lockable
					THEN MAKE otherside OF THIS lockable.
				END IF.

				IF THIS IS locked
					THEN MAKE otherside OF THIS locked.
				END IF.

		END IF.


		-- making the same matching_key open both sides of a door: 

		IF otherside OF THIS <> null_door AND matching_key OF THIS <> null_key
			THEN SET matching_key OF otherside OF THIS TO matching_key OF THIS.
		END IF. 
		

	-- If a door is lockable/locked, you should state at the door instance
	-- which object will unlock it, with the matching_key attribute. 
		-- for example

		-- THE attic_door ISA DOOR
			-- HAS matching_key brass_key.
			-- ...
		--   END THE.

		-- THE brass_key ISA OBJECT AT basement
		-- END THE.

	-- (null_key is a default dummy object that can be ignored.)

	

	VERB examine
		DOES AFTER
			IF THIS IS NOT plural
				THEN "It is" 
				ELSE "They are"
			END IF.
		
			IF THIS IS NOT open
				THEN "currently closed."
				ELSE "currently open."
			END IF.
	END VERB.

		

	VERB knock
		DOES ONLY
			IF THIS IS NOT open
				THEN "You knock on" SAY THE THIS. "$$. There is no reply."
				ELSE "You don't find it purposeful to knock on the open door"
					IF THIS IS NOT plural
						THEN "."
						ELSE "$$s."
					END IF.
					
			END IF.
	END VERB.



	VERB look_behind
		DOES ONLY 
			IF THIS IS NOT open
				THEN "You cannot look behind"
					IF THIS IS NOT plural
						THEN "the door - it is closed."
						ELSE "the doors - they are closed."
					END IF.
				ELSE "You notice nothing special behind the door"
					IF THIS IS NOT plural
						THEN "."
						ELSE "$$s."
					END IF.
			END IF.
	END VERB.



	VERB look_under
		DOES ONLY
			IF THIS IS NOT open
				THEN "The gap under the closed door"
					IF THIS IS plural 
						THEN "$$s"
					END IF.
					"is so narrow that you can't
					see anything of what lies on the other side."
				ELSE "You notice nothing special under the door"
					IF THIS IS plural
						THEN "$$s."
						ELSE "."
					END IF.
			END IF.
	END VERB.

	
	
	VERB close
		DOES 
			IF otherside OF THIS <> null_door
				THEN MAKE otherside OF THIS NOT open.
			END IF.
	END VERB.


	VERB lock 
		DOES 
			IF otherside OF THIS <> null_door
				THEN MAKE otherside OF THIS NOT open.
					MAKE otherside OF THIS locked.
			END IF.
	END VERB.


	VERB open 
		DOES 
			IF otherside OF THIS <> null_door
				THEN MAKE otherside OF THIS open.
					MAKE otherside OF THIS NOT locked.
			END IF.
	END VERB.


	VERB unlock 
		DOES 
			IF otherside OF THIS <> null_door
				THEN MAKE otherside OF THIS NOT locked.
			END IF.
	END VERB.


END EVERY.


-- a default dummy, ignore:

THE null_door ISA DOOR
END THE.



-- =============================================================


----- LIGHTSOURCE


-- =============================================================


-- (In the file 'lib_verbs.i', ISA LIGHTSOURCE is used in the syntax definition of the verb 'light'.
-- Also, in 'lib_locations.i', LIGHTSOURCE is used in defining the behavior of the class DARK_LOCATION.)



EVERY lightsource ISA OBJECT
	IS NOT lit.
	IS natural. 	-- A natural lightsource is for example a candle, a match or a torch. 
				-- A NOT natural lightsource is for example a flashlight or a lamp.
				-- You cannot switch on or off a natural lightsource.


	VERB examine
		DOES AFTER
			IF THIS IS lit
				THEN 
					IF THIS IS natural
						THEN 
							IF THIS IS NOT plural
								THEN "It is"
								ELSE "They are"
							END IF.
							"currently lit."
						ELSE 
							IF THIS IS NOT plural
								THEN "It is"
								ELSE "They are"
							END IF.
							"currently on."
					END IF.
				ELSE
					IF THIS IS natural
						THEN 
							IF THIS IS NOT plural
								THEN "It is"
								ELSE "They are"
							END IF.
							"currently not lit."
						ELSE 
							IF THIS IS NOT plural
								THEN "It is"
								ELSE "They are"
							END IF.
							"currently off."
					END IF.
			END IF.
	END VERB.

	
	VERB light
		CHECK THIS IS NOT lit
			ELSE 
				IF THIS IS NOT plural
					THEN SAY check_lightsource_not_lit_sg OF my_game.
					ELSE SAY check_lightsource_not_lit_pl OF my_game.
				END IF.
		AND THIS IS NOT broken
			ELSE SAY check_obj_not_broken OF my_game.
		DOES ONLY
			IF THIS IS natural
				THEN "You light" SAY THE THIS. "." 
					MAKE THIS lit.
				ELSE "You turn on" SAY THE THIS. "."
					MAKE THIS lit.
			END IF.
	END VERB.


	VERB extinguish
		CHECK THIS IS lit
			ELSE 
				IF THIS IS NOT plural
					THEN SAY check_lightsource_lit_sg OF my_game.
					ELSE SAY check_lightsource_lit_pl OF my_game.
				END IF.
		DOES ONLY "You extinguish" SAY THE THIS. "."
			MAKE THIS NOT lit.
	END VERB.


	VERB turn_on
		CHECK THIS IS NOT natural
			ELSE 
				IF THIS IS NOT plural
					THEN SAY check_obj_suitable_on_sg OF my_game.
					ELSE SAY check_obj_suitable_on_pl OF my_game.
				END IF.
		AND THIS IS NOT lit
			ELSE 
				IF THIS IS NOT plural
					THEN SAY check_lightsource_not_lit_sg OF my_game.
					ELSE SAY check_lightsource_not_lit_pl OF my_game.
				END IF.
		AND THIS IS NOT broken
			ELSE SAY check_obj_not_broken OF my_game.
		DOES ONLY
			"You turn on" SAY THE THIS. "."
			MAKE THIS lit.
					
	END VERB.


	VERB turn_off
		CHECK THIS IS NOT natural
			ELSE 
				IF THIS IS NOT plural
					THEN SAY check_obj_suitable_off_sg OF my_game.
					ELSE SAY check_obj_suitable_off_pl OF my_game.
				END IF.
		AND THIS IS lit
			ELSE 
				IF THIS IS NOT plural
					THEN SAY check_lightsource_lit_sg OF my_game.
					ELSE SAY check_lightsource_lit_sg OF my_game.
				END IF.
				
		DOES ONLY 
			"You turn off" SAY THE THIS. "."
			MAKE THIS NOT lit.	
			  
	END VERB.


-- The following verb switches a NOT natural lightsource on if it is off, and vice versa
-- (when the player forgets, or doesn't bother, to type 'on' or 'off' after 'switch').


	VERB switch
		CHECK THIS IS NOT natural
			ELSE 
				IF THIS IS NOT plural
					THEN SAY check_lightsource_switchable_sg OF my_game.
					ELSE SAY check_lightsource_switchable_pl OF my_game.
				END IF.
		AND THIS IS reachable
			ELSE 
				IF THIS IS NOT plural
					THEN SAY check_obj_reachable_sg OF my_game.
					ELSE SAY check_obj_reachable_pl OF my_game.
				END IF.
		AND THIS IS NOT broken
			ELSE SAY check_obj_not_broken OF my_game.	
		DOES ONLY
			IF THIS IS lit
				THEN "You switch off" SAY THE THIS. "."
					MAKE THIS NOT lit.
				ELSE "You switch on" SAY THE THIS. "."
					MAKE THIS lit.
			END IF.
	END VERB.
	

END EVERY.



-- ==============================================================


----- LIQUID


-- ==============================================================


-- (In the file 'lib_verbs.i', ISA LIQUID is used in the syntax definitions of the verbs 'drink' and 'sip'.)


EVERY liquid ISA OBJECT		
	
	CONTAINER
		HEADER "In" SAY THE THIS. "you see"
		ELSE "There is nothing in" SAY THE THIS. "."	

		-- We declare this class a container to enable player commands such as
		-- 'throw sack into water', 'look into water' and 'take pearl from water'.
		-- Also cases such as 'pour red potion into blue potion' require that this 
		-- class behaves like a container. 
	

	HAS vessel null_vessel.	

		-- The 'vessel' attribute takes care that if a liquid is
		-- in a container, the verb 'take' will automatically take the 
		-- container instead (if the container is takeable). Trying 				
		-- take a liquid that is in a fixed-in-place container, as well
		-- as trying to take a liquid outside any container, will yield 			
		-- "You can't carry [the liquid] around in your bare hands." 
		-- The default value 'null_vessel' tells the compiler that the liquid
		-- is not in any container. 'null_vessel' is a dummy default that can be
		-- ignored.
							

	INITIALIZE

	-- Every object found in a liquid, for example a fish in a pond of water, 
	-- will be allowed back in that liquid once taken out of there:

		FOR EACH liq ISA LIQUID
			DO
				FOR EACH o ISA OBJECT, DIRECTLY IN liq
					DO 
						INCLUDE o IN allowed OF liq.
				END FOR.
		END FOR.


	-- Every liquid in a container at the start of the game 
	-- will have that container as its vessel:

		FOR EACH lc ISA LISTED_CONTAINER
			DO						
				FOR EACH lq ISA LIQUID, DIRECTLY IN lc
					DO
						SET vessel OF lq TO lc.
				END FOR.	
		END FOR.


	-- If you have some liquid in a container in your game, you should declare the 
	-- liquid instance thus:
	
 	-- THE juice ISA LIQUID
	--      IN bottle
	-- END THE juice.

	-- The verb 'pour', as defined in this library, also works for the container of a liquid;
	-- i.e. if there is some juice in a bottle, 'pour bottle' and 'pour juice' will work equally well.
	-- Note, however, that the verb 'empty' is not a synonym for 'pour';
	-- 'empty' only works for container objects.



		SCHEDULE check_vessel AT THIS AFTER 0.		-- this event is defined further below


	VERB examine
		DOES ONLY
			IF vessel OF THIS <> null_vessel
				THEN 
					IF vessel OF THIS IS open
						THEN "You notice nothing unusual about" SAY THE THIS.
						ELSE "You can't, since" SAY THE vessel OF THIS. 
								IF THIS IS NOT plural
									THEN "is"
									ELSE "are"
								END IF.
								"closed."
								-- Here we prohibit the player from examining
								-- a liquid when the liquid is in a closed container.
					END IF.
				ELSE "You notice nothing unusual about" SAY THE THIS. "."
			END IF.
	END VERB.
		

	VERB look_in
		DOES ONLY
			IF vessel OF THIS <> null_vessel
				THEN 
					IF vessel OF THIS IS open
						THEN "You see nothing special in" SAY THE THIS. "."
						ELSE "You can't, since" SAY THE vessel OF THIS.
								IF THIS IS NOT plural
									THEN "is"
									ELSE "are"
								END IF.
								"closed."
								-- Here we prohibit the player from looking into
								-- a liquid when the liquid is in a closed container.
					END IF.
				ELSE "You see nothing special in" SAY THE THIS. "."
			END IF.
	END VERB.
		

	VERB take
		CHECK vessel OF THIS NOT IN hero
			ELSE SAY check_obj_not_in_hero2 OF my_game.
		DOES ONLY
			IF vessel OF THIS = null_vessel OR vessel OF THIS IS NOT takeable
				THEN "You can't carry" SAY THE THIS. "around in your bare hands."
			ELSE LOCATE vessel OF THIS IN hero.
				"($$" SAY THE vessel OF THIS. "of" SAY THIS. "$$)$nTaken." 
			END IF.
	END VERB.


	VERB take_from
	   WHEN obj
		CHECK holder <> vessel OF THIS
			ELSE SAY check_liquid_vessel_not_cont OF my_game.
			-- the above is triggered when the player types for example 
			-- >take juice from bottle   -- (when the juice is in the bottle)
		DOES ONLY
			IF vessel OF THIS = null_vessel OR vessel OF THIS IS NOT takeable
				THEN "You can't carry" SAY THE THIS. "around in your bare hands."
			ELSE LOCATE vessel OF THIS IN hero.
				"($$" SAY THE vessel OF THIS. "of" SAY THIS. "$$)$nTaken." 
			END IF.
	END VERB.	


	VERB drop
		DOES ONLY
			LOCATE vessel OF THIS AT hero.
			"($$" SAY THE vessel OF THIS. "of" SAY THIS. "$$)$nDropped."
			
	END VERB.


	VERB ask_for
		DOES ONLY
			LOCATE vessel OF THIS IN hero.
			SAY THE act. "gives" SAY THE vessel OF THIS. "of" SAY THIS. "to you."		
	END VERB.					
			

	VERB give
		WHEN obj
		DOES ONLY
			-- implicit taking:
			IF THIS NOT IN hero
				THEN 
					IF vessel OF THIS = null_vessel OR vessel OF THIS IS NOT takeable
						THEN "You can't carry" SAY THE THIS. "around in your bare hands."
					ELSE LOCATE vessel OF THIS IN hero.
						"(taking" SAY THE vessel OF THIS. "of" SAY THIS. "first)$n"
					END IF.
			END IF.
			-- end of implicit taking.

			IF THIS IN hero   
				-- i.e. if the implicit taking was successful
				THEN
					"You give" SAY THE vessel OF THIS. "of" SAY THIS. "to" SAY THE recip. "."
					LOCATE vessel OF THIS IN recip.
			END IF.
		
			-- there is no 'ELSE' statement in this last IF -clause, as the 'IF THIS NOT 
			-- IN hero' clause above it takes care of the 'ELSE' alternative.

	END VERB.


	VERB pour
		DOES ONLY
			-- implicit taking:
			IF THIS NOT IN hero
				THEN 
					IF vessel OF THIS = null_vessel OR vessel OF THIS IS NOT takeable
						THEN "You can't pour" SAY THE THIS. "anywhere since you are not
							carrying" 
								IF THIS IS NOT plural
									THEN "it."
									ELSE "them."
								END IF.
					ELSE LOCATE vessel OF THIS IN hero.
						"(taking" SAY THE vessel OF THIS. "of" SAY THIS. "first)$n"
					END IF.
			END IF.
			-- end of implicit taking.
			
			IF THIS IN hero
				THEN LOCATE THIS AT hero.
					SET vessel OF THIS TO null_vessel.
					"You pour" SAY THE THIS.
						IF floor HERE
							THEN "on the floor."
							ELSE "on the ground."
						END IF.			
			END IF.

	END VERB.


	VERB pour_in
		WHEN obj
			DOES ONLY
				-- implicit taking:
				IF THIS NOT IN hero
					THEN 
						IF vessel OF THIS = null_vessel
							THEN "You can't carry" SAY THE THIS. "around in your bare hands."
						ELSIF vessel OF THIS IS NOT takeable
							THEN "You don't have" SAY THE vessel OF THIS. "of" SAY THIS. "."
						ELSE LOCATE vessel OF THIS IN hero.
							"(taking" SAY THE vessel OF THIS. "of" SAY THIS. "first)$n"
						END IF.
				END IF.
				-- end of implicit taking.

				IF THIS IN hero		--i.e. if the implicit taking was successful
					THEN LOCATE THIS IN cont.
						SET vessel OF THIS TO cont.
						"You pour" SAY THE THIS. "into" SAY THE cont. "."						
				END IF.
		WHEN cont
			DOES ONLY
				IF vessel OF THIS = null_vessel
					THEN 
						"There's not much sense pouring" SAY THE obj. "into" SAY THE THIS. "."
					ELSE 
						IF vessel OF THIS IS open
							THEN "It wouldn't accomplish anything trying to pour" SAY THE obj. 
								"into" SAY THE THIS. "."
							ELSE "You can't, since" SAY THE vessel OF THIS. 
								IF THIS IS NOT plural
									THEN "is"
									ELSE "are"
								END IF.
								"closed."
						END IF.
				END IF.
	END VERB.


	VERB pour_on
		WHEN obj
			DOES ONLY
				-- implicit taking:
				IF THIS NOT IN hero
					THEN 
						IF vessel OF THIS = null_vessel 
							THEN "You can't carry" SAY THE THIS. "around in your bare hands."
						ELSIF vessel OF THIS IS NOT takeable
							THEN "You don't have" SAY THE vessel OF THIS. "of" SAY THIS. "." 
						ELSE LOCATE vessel OF THIS IN hero.
							"(taking" SAY THE vessel OF THIS. "of" SAY THIS. "first)$n"
						END IF.
				END IF.
				-- end of implicit taking.
				
				IF THIS IN hero		
					-- i.e. if the implicit taking was successful
					THEN
						IF surface = floor OR surface = ground
							THEN LOCATE THIS AT hero.
						 		"You pour" SAY THE THIS. "on" SAY THE surface. "."
								SET vessel OF THIS TO null_vessel.
						ELSIF surface ISA SUPPORTER
							THEN LOCATE THIS IN surface.
								"You pour" SAY THE THIS. "on" SAY THE surface. "."
				  				SET vessel OF THIS TO null_vessel.
						ELSE "It wouldn't be sensible to pour anything on" SAY THE surface.
						END IF.
				END IF.
	END VERB.		


	VERB fill_with
		-- when something is filled with a liquid, this something becomes the
		-- vessel of the liquid:
		WHEN substance					
			 DOES SET vessel OF THIS TO cont.   	  
	END VERB.


	VERB put_in
		WHEN obj
			DOES ONLY 
				IF vessel OF THIS = null_vessel
					THEN "You can't carry" SAY THE THIS. "around in your bare hands."
					ELSE 
						IF vessel OF THIS IS takeable
							THEN
								-- implicit taking:
								IF THIS NOT IN hero
									THEN 
										IF vessel OF THIS = null_vessel 
											THEN "You can't carry" SAY THE THIS. "around in your bare hands."
										ELSE LOCATE vessel OF THIS IN hero.
											"(taking" SAY THE vessel OF THIS. "of" SAY THIS. "first)$n"
										END IF.
								END IF.
								-- end of implicit taking.

								LOCATE vessel OF THIS IN cont.
						      	"You put" SAY THE vessel OF THIS. "of" SAY THIS. "into" SAY THE cont. "."

							ELSE "You don't have" SAY THE vessel OF THIS. "of" SAY THIS. "."
						END IF.
				END IF.
	      WHEN cont
			DOES ONLY
			IF vessel OF THIS = null_vessel
				THEN 
					"There's not much sense putting" SAY THE obj. "into" SAY THE THIS. "."
				ELSE 
					IF vessel OF THIS IS open
						THEN 
							IF obj = vessel OF THIS
								THEN "That doesn't make sense."
								ELSE "It wouldn't accomplish anything trying to put" SAY THE obj. 
									"into" SAY THE vessel OF THIS. "of" SAY THIS. "."
							END IF.
						ELSE "You can't, since" SAY THE vessel OF THIS. "of" SAY THIS.
								IF THIS IS NOT plural
									THEN "is"
									ELSE "are"
								END IF.
							"closed."
					END IF.
			END IF.
	END VERB.


	VERB put_on
		WHEN obj
			DOES ONLY
				-- implicit taking:
				IF THIS NOT IN hero
					THEN 
						IF vessel OF THIS = null_vessel 
							THEN "You can't carry" SAY THE THIS. "around in your bare hands."
						ELSIF vessel OF THIS IS NOT takeable
							THEN "You don't have" SAY THE vessel OF THIS. "of" SAY THIS. "." 
						ELSE LOCATE vessel OF THIS IN hero.
							"(taking" SAY THE vessel OF THIS. "of" SAY THIS. "first)$n"
						END IF.
				END IF.
				-- end of implicit taking.

				IF THIS IN hero 				
					-- i.e. if the implicit taking was successful
					THEN "You put" SAY THE vessel OF THIS. "of" SAY THIS. "onto" SAY THE surface. "."
				END IF.
		WHEN surface
			DOES ONLY "It is not possible to $v" SAY obj. "onto" SAY THE THIS. "."
	END VERB.





	-- The verbs 'empty', 'empty_in' and 'empty_on' will be disabled as ungrammatical with liquids:	

	VERB 'empty'
		WHEN obj
		DOES ONLY "You can only empty containers."
	END VERB.

	VERB empty_in
		WHEN obj
		DOES ONLY "You can only empty containers."
	END VERB.

	VERB empty_on
		WHEN obj
		DOES ONLY "You can only empty containers."
	END VERB.
	

END EVERY.



-- Here is the default vessel for liquids; if the vessel of a liquid is
-- 'null_vessel', it means that the liquid is not in any container; ignore.


THE null_vessel ISA OBJECT
	CONTAINER 
END THE.



-- This event checks that if a liquid is outside a container, its container will
-- be 'null_vessel'; ignore:


EVENT check_vessel
	FOR EACH liq ISA LIQUID, DIRECTLY AT CURRENT LOCATION DO	
			SET vessel OF liq TO null_vessel.
	END FOR.
	SCHEDULE check_vessel AFTER 1.
END EVENT.




-- =============================================================


----- LISTED_CONTAINER


-- =============================================================


-- (This class is not cross-referenced elsewhere in this or any other library file.)


EVERY LISTED_CONTAINER ISA OBJECT			
	CONTAINER						 	

		--  (ACTORS are separately defined to be containers further below.)

	INITIALIZE

	-- Every object in a container will be allowed back in that container by default if it's taken out:

		FOR EACH lc ISA LISTED_CONTAINER
			DO
				FOR EACH o ISA OBJECT, DIRECTLY IN lc
					DO 
						INCLUDE o IN allowed OF lc.
				END FOR.
		END FOR.





	VERB examine 
		DOES 
			IF THIS IS NOT OPAQUE
				THEN LIST THIS.
				ELSE "You can't see inside" SAY THE THIS. "."
			END IF.
	END VERB.


	VERB look_in
		DOES ONLY
			IF THIS IS NOT OPAQUE
				THEN LIST THIS.
				ELSE "You can't see inside" SAY THE THIS. "."
			END IF.
	END VERB.


	VERB search
		DOES ONLY
			IF THIS IS NOT OPAQUE
				THEN LIST THIS.
				ELSE "You can't see inside" SAY THE THIS. "."
			END IF.
	END VERB.



-- Note that closed listed_containers are by default opaque and they become "not opaque" when
-- they are opened: 


	VERB open
 		DOES
			MAKE THIS NOT OPAQUE.
 			LIST THIS.
 	END VERB.


 	VERB close
 		DOES
 			MAKE THIS OPAQUE.
 	END VERB.


END EVERY.





-- ===============================================================


----- SOUND


-- ===============================================================


-- (This class is not cross-referenced in this or any other library file.)


EVERY sound ISA OBJECT
	IS NOT examinable.
	IS NOT takeable.
	IS NOT reachable.
	IS NOT movable.

	VERB smell
		DOES ONLY
			 IF THIS IS NOT plural
				THEN "That's not"
				ELSE "Those are not"
			 END IF.
			"something you can smell."
	END VERB.


END EVERY.



-- ==============================================================


----- SUPPORTER


-- ==============================================================


-- (See the file 'verbs.i', verbs 'climb_on', 'empty_on', 'get_off', 'jump_on',
-- 'lie_on', 'pour_on', 'put_in', 'put_on', 'sit_on', 'stand_on', and 'take_from'
-- where SUPPORTER is used in either syntax definitions, verb checks
-- or verb definitions.)
 

EVERY supporter ISA OBJECT


	CONTAINER
		HEADER "On" SAY THE THIS. "you see"
		ELSE "There's nothing on" SAY THE THIS. "."	


	VERB examine
		DOES 
			LIST THIS.
	END VERB.


	-- in the following, we disable some verbs that are defined to work with normal containers:


	VERB look_in							
		DOES ONLY 
			IF THIS IS NOT plural
				THEN "That's not"
				ELSE "Those are not"
			END IF.
			"something you can look into."
	END VERB.


	VERB empty_in, pour_in
	   WHEN cont
		DOES ONLY
			 IF THIS IS NOT plural
				THEN "That's not"
				ELSE "Those are not"
			END IF.
			"something you can pour things into."
	END VERB.


	VERB put_in
   	  WHEN cont
		DOES ONLY "You can't put anything inside" SAY THE THIS. "."
	END VERB.


	VERB throw_in
   	  WHEN cont
		DOES ONLY "You can't put anything inside" SAY THE THIS. "."
	END VERB.


END EVERY.







-- ==============================================================


----- WEAPON


-- ==============================================================


-- (See the file 'lib_verbs.i', verbs 'attack_with', 'fire',
-- 'fire_with', 'kill_with', 'shoot' and 'shoot_with' where WEAPON is used
-- either in the syntax definitions or verb checks.)


EVERY weapon ISA OBJECT
	IS NOT fireable.
END EVERY.




-- ==============================================================


----- WINDOW


-- ==============================================================


-- (This class is not cross-referenced elsewhere in this or any other library file.)


-- You can look out of and through a window. 
-- When examined, a window is by default described as being either open or closed.


EVERY window ISA OBJECT
	IS openable.
	IS NOT open.
	IS NOT takeable.


	VERB examine
		DOES 
			IF THIS IS NOT open
				THEN 
					IF THIS IS NOT plural
						THEN "It is"
						ELSE "They are"
					END IF.
					"currently closed."
				ELSE 
					IF THIS IS NOT plural
						THEN "It is"
						ELSE "They are"
					END IF.
					"currently open."
			END IF.
	END VERB.


	VERB look_behind
		DOES ONLY 
			"That's not possible."
	END VERB.


	VERB look_out_of
		DOES ONLY "You see nothing special looking out of the"
				IF THIS IS NOT plural
					THEN "window."
					ELSE "windows."
				END IF.
	END VERB.


	VERB look_through
		DOES ONLY "You see nothing special looking through the"
				IF THIS IS NOT plural
					THEN "window."
					ELSE "windows."
				END IF.
	END VERB.


END EVERY.



-- ===============================================================

-- ===============================================================
--
-- 2. Actors
--
-- ===============================================================

-- ===============================================================


-- First, we declare some common characteristics for all actors:


ADD TO EVERY ACTOR
   	IS NOT inanimate.
   	IS NOT following. 
   	IS NOT sitting.
   	IS NOT lying_down.	
   	IS NOT named.	
	-- = the actor's name is not known to the player
	IS wearing {null_clothing}.  
	-- = the actor's clothing is not specified.
	-- "null_clothing" is a dummy default that can be ignored.
	IS NOT compliant.
	-- an actor only gives something to the hero if it is in a compliant mood.
	-- In practice, this happens by default when the hero asks the actor for anything.
	-- For example, implicit taking of objects is not successful if the object happens
	-- to be held by an NPC who is not compliant.
	IS NOT takeable.


	
   	DEFINITE ARTICLE 
		IF THIS IS NOT named
			THEN "the"		
			ELSE ""
		END IF.


   	INDEFINITE ARTICLE 
		IF THIS IS NOT named
			THEN 								
				IF THIS IS NOT plural
					THEN "a"				
					ELSE ""
				END IF.		
			ELSE ""
		END IF.
	
	-- if you need "an", you must declare it separately at the actor instance

	
   	CONTAINER							
		-- so that actors can receive and carry objects
		HEADER
			IF THIS = hero
				THEN "You are carrying"
				ELSE	
					IF THIS IS NOT named
						THEN SAY THE THIS. 
						ELSE SAY THIS.
					END IF.
										
					IF THIS IS NOT plural
						THEN "is carrying"
						ELSE "are carrying"
					END IF.
			END IF.
		ELSE
			IF THIS = hero
				THEN "You are empty-handed."
				ELSE 
					IF THIS IS NOT named
						THEN SAY THE THIS. 
						ELSE SAY THIS.
					END IF.
					
					IF THIS IS NOT plural					
						THEN "is not carrying anything."
						ELSE "are not carrying anything."
					END IF.
					
			END IF.

		EXTRACT
			CHECK THIS IS compliant
				ELSE "That seems to belong to" 
					IF THIS IS NOT named
						THEN SAY THE THIS. 
						ELSE SAY THIS.
					END IF.
					"."
	
			

  	INITIALIZE

		MAKE hero compliant.		
		-- so that the hero can give, drop, etc. carried objects.		


		-- excluding the default dummy clothing object from all actors; ignore.  
		
		EXCLUDE null_clothing FROM wearing OF THIS.


		-- the 'donned' attribute is used internally in the library; ignore.

		FOR EACH c ISA CLOTHING, IN wearing OF THIS
			DO 
				MAKE c donned.
		END FOR.


		-- all actors will obey this script from the start of the game:
	
		IF THIS <> hero
			THEN USE SCRIPT following_hero FOR THIS.
		END IF.


				
	SCRIPT following_hero						
		-- this code will make any actor follow the hero
		-- if the actor is given the attribute 'following'.

		STEP WAIT UNTIL hero NOT HERE			
				 
			IF THIS IS following
				THEN
					LOCATE THIS AT hero.
					"$p" SAY THE THIS. 
						IF THIS IS NOT plural
							THEN "follows you."
							ELSE "follow you."
						END IF.				
			END IF.
				
			USE SCRIPT following_hero FOR THIS.



	DESCRIPTION	
		IF THIS IS NOT named
			THEN 
				IF THIS IS NOT plural
					THEN "There is" SAY AN THIS. "here."		
					ELSE "There are" SAY THIS. "here."	
				END IF.	
			ELSE SAY THIS. 
				IF THIS IS NOT plural
					THEN "is here."
					ELSE "are here."
				END IF.
		END IF.	
							
	

END ADD TO.


THE null_clothing ISA CLOTHING
END THE.


	


-- ================================================================


----- PERSON			-- CAN talk


-- ================================================================


EVERY person ISA ACTOR
    	CAN talk.

	CONTAINER
		HEADER
			SAY THE THIS.
			IF THIS IS NOT plural
				THEN "is carrying"
				ELSE "are carrying"
			END IF.
		ELSE
			
			IF THIS IS NOT named
				THEN SAY THE THIS. 
				ELSE SAY THIS.
			END IF.
					
			IF THIS IS NOT plural					
				THEN "is empty-handed."
				ELSE "are empty-handed."
			END IF.

		EXTRACT
			CHECK THIS IS compliant
				ELSE "That seems to belong to" 
					IF THIS IS NOT named
						THEN SAY THE THIS. 
						ELSE SAY THIS.
					END IF.
					"."
					
END EVERY.





-- ================================================================


----- FEMALE and MALE		


-- ================================================================


-- MALE and FEMALE are actually subclasses of PERSON, so they both
-- have the ability to talk.


EVERY female ISA PERSON
	PRONOUN her
END EVERY.


EVERY male ISA PERSON
	PRONOUN him
END EVERY.




-- end of file.


