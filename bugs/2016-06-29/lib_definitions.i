-- ALAN Standard Library v2.0
-- Definitions (file name: 'lib_definitions.i')

-- Included in this file:
	-- general attributes
	-- some article declarations
	-- common synonyms
	-- the definition_block class 
		-- attributes for the start section
		-- messages for the hero
		-- messages for dark locations		
		-- illegal parameter messages, used in SYNTAX definitions of verbs
		-- verb check messages 
		-- implicit taking message
	-- the banner instance (for the start section)




-- General attributes
-- ==================

-- We define general attributes for every thing ( = object or actor): 


ADD TO EVERY THING

	IS examinable.
		-- the library declares SOUNDs as not examinable. 
	   inanimate.
		-- actors are NOT inanimate.
	   movable.  
		-- to allow pushing, pulling, lifting, etc.
	   open.
		-- = not closed. 
	   reachable. 		
		-- See also 'distant' below
	   takeable.
		-- you'll have to separately define which objects are NOT takeable.
		-- By default, the floor, walls, ceiling, ground and sky objects
		-- are not takeable. The same goes for all doors, windows, sounds, liquids
		-- that are not in containers, and actors.

	HAS allowed {null_object}.
		-- container objects only take what is allowed for them to take;
		-- this applies to verbs empty_in, pour_in, put_in and throw_in.
		-- "null_object" is a default dummy that can be ignored.


	HAS matching_key null_key.
		-- All lockable doors need a matching key to lock/unlock them.
		-- "null_key" is a default dummy that can be ignored. This attribute
		-- is here added to every thing instead of just doors, to enable
		-- matching keys to be programmed for other locked objects, too, like for
		-- example treasure chests etc.

	HAS text "".	

	NOT broken.
	NOT distant.		
		-- Usage: you can for example talk to a "not reachable" actor but not to a "distant" one.
		-- You can also throw things in, to or at a not reachable target but not to a distant one.
		-- Default response for not reachable things: "The [thing] is out of your reach."
		-- Default response for distant things: "The [thing] is too far away."
	NOT drinkable. 
	NOT edible.
	NOT fireable.		
		-- can (not) be used as a firearm
	NOT lockable.
	NOT locked.
	NOT openable.
	NOT readable.   
	NOT scenery.		
		-- scenery has special responses for 'examine' and 'take', behaves like a normal object otherwise.
	NOT wearable.
	NOT writeable.

	CAN NOT talk.		
	



-- We still define that plural nouns are preceded by "some" (and not by "a" or "an"):

INDEFINITE ARTICLE
	IF THIS IS plural
		THEN "some"
		ELSE "a"
	END IF.

END ADD TO.

	

-- If you need "an", you should declare it separately at the instance, for example:

-- THE owl ISA ACTOR
--  	AT woods
--  	INDEFINITE ARTICLE "an"
-- END THE.


-- (We add the 'plural' attribute to the 'entity' class, because the plural 
-- applies not only to things but also to for example parameters in syntax statements; ignore.)


ADD TO EVERY ENTITY
	IS NOT plural.
END ADD TO.



-- Some null defaults defined that have been mentioned above:


THE null_object ISA OBJECT
END THE.


THE null_key ISA OBJECT
END THE.



-- Some weight attributes for things:


ADD TO EVERY THING
	HAS weight 0.
END ADD TO THING.


ADD TO EVERY ACTOR 
	HAS weight 50. 
END ADD TO ACTOR. 


ADD TO EVERY OBJECT
	HAS weight 5.
END ADD TO OBJECT. 


-- These attributes are mostly used to check if something is movable.



-- An attribute for keeping track of nested locations; used internally in the library (ignore).

ADD TO EVERY LOCATION
	HAS nested {nowhere}.
END ADD TO.




-- Common synonyms
-- ===============


-- Next, we declare synonyms for some common words so that it will be possible
-- for the player to type commands such as both "put ball in box" and 
-- "put ball into box", etc:


SYNONYMS 

into, inside = 'in'.
onto = on.
thru = through.
using = 'with'.


-- Here are some synonyms for the player character:

me, myself, yourself, self = hero.




-- Attributes for the my_game definition block
-- ===========================================

-- Here, we create a class, "definition_block", to group various definitions under.
-- In the game source file, the author should declare an instance 'my_game' which belongs
-- to this class.



EVERY definition_block ISA LOCATION


	-- attributes for the start section (banner):
	-- ==========================================

	HAS title "My New Game".		
    	HAS subtitle "".			     	
    	HAS author "An ALAN Author".   	
    	HAS year 2015.				
    	HAS version "1".					
    	
	-- These will be shown at the start of the game if you add
		-- DESCRIBE banner.
	-- after START AT [location].


	-- messages for the hero:
	-- ======================

	-- The hero is not defined as an instance in the library; the game author
	-- has all the freedom to define the hero as (s)he sees fit. However,
	-- there are some messages for the hero defined in the library. These can be
	-- easily overridden. Two of these messages are right here, the rest are
	-- for example in verb checks.

	HAS hero_worn_header "You are wearing".
	HAS hero_worn_else "You are not wearing anything.".	


	-- description messages for dark locations:
	-- ========================================

	HAS dark_loc_desc "It is pitch black. You can't see anything at all.".
		-- This message is shown when the player tries to LOOK or do actions requiring light
		-- in a dark location.

	HAS light_goes_off "It is now pitch black.".
		-- This message is shown when a light goes off and the location becomes dark.


	-- response for restricted actions:
	-- ================================

	HAS restricted_response "You can't do that.".
		-- This message is shown whenever the player used a verb that has been disabled
		 -- by the "CAN NOT [verb]." attributes further down this file.


	-- all illegal parameter messages, typically found in the ELSE parts of SYNTAX structures and
	-- the first two below being by far the most common.
	-- ==========================================================================================


	-- the general message for when a parameter is not suitable with the verb:
	--------------------------------------------------------------------------

 	HAS illegal_parameter_sg "That's not something you can $v.".				-- (numerous)
	HAS illegal_parameter_pl "Those are not something you can $v.".


	-- variations of the above message when a preposition is required after the verb:
	---------------------------------------------------------------------------------

	HAS illegal_parameter_about_sg "That's not something you can $v about.".		-- ask_about, tell_about, think_about
	HAS illegal_parameter_about_pl "Those are not something you can $v about.".
	HAS illegal_parameter_at "You can't $v anything at $+2.".					-- fire_at, throw_at
	HAS illegal_parameter_for_sg "That's not something you can $v for.".			-- ask_for
	HAS illegal_parameter_for_pl "Those are not something you can $v for.".
	HAS illegal_parameter2_from_sg "That's not something you can take things from.".	-- take_from
	HAS illegal_parameter2_from_pl "Those are not something you can take things from.".
	HAS illegal_parameter_in_sg "That's not something you can $v in.".			-- dive_in, jump_in, lie_in, swim_in
	HAS illegal_parameter_in_pl "Those are not something you can $v in.".
	HAS illegal_parameter_on_sg "That's not something you can $v on.".			-- climb_on, jump_on, knock, lie_on, sit_on,
																  -- stand_on, switch_on, turn_on
	HAS illegal_parameter_on_pl "Those are not something you can $v on.".
	HAS illegal_parameter_off_sg "That's not something you can $v off.".			-- get_off, switch_off, turn_off
	HAS illegal_parameter_off_pl "Those are not something you can $v off.".
	HAS illegal_parameter_to_sg "That's not something you can $v to.".			-- listen_to, talk_to
	HAS illegal_parameter_to_pl "Those are not something you can $v to.".
	HAS illegal_parameter2_to_sg "That's not something you can $v things to.".		-- give, show, tell, tie_to, throw_to
	HAS illegal_parameter2_to_pl "Those are not something you can $v things to.".	
	HAS illegal_parameter_with_sg "That's not something you can $v with.".		-- kill_with, shoot_with, play_with
	HAS illegal_parameter_with_pl "Those are not something you can $v with.".	
	HAS illegal_parameter2_with_sg "That's not something you can $v things with.".	-- attack_with, break_with, burn_with, close_with, 
																 -- + cut_with, fill_with, lock_with, open_with, pry_with,
																 -- + push_with, unlock_with
	HAS illegal_parameter2_with_pl "Those are not something you can $v things with.".
		

	-- other illegal parameter messages:
	------------------------------------ 


	HAS illegal_parameter_act "That doesn't make sense.".							-- empty_in, pour_in, put_in, throw_in
	
	HAS illegal_parameter_consult_sg "That's not something you can find information		-- consult_about
								about.".
	HAS illegal_parameter_consult_pl "Those are not something you can find 			
								information about.".

	HAS illegal_parameter_examine_sg "That's not something you can examine.".			-- examine
	HAS illegal_parameter_examine_pl "Those are not something you can examine.".

	HAS illegal_parameter_go "You can't go there.".								-- go_to

	HAS illegal_parameter_look_out_sg "That's not something you can look out of.".		-- look_out_of  
	HAS illegal_parameter_look_out_pl "Those are not something you can look out of.".
	HAS illegal_parameter_look_through "You can't look through $+1.".				-- look_through  

	HAS illegal_parameter_obj "You can only $v objects.".							-- give, put, put_in, put_on, put_against,
																	   -- + put_behind, put_near, put_under, 
																	   -- + throw_at, throw_in, throw_to, tie_to,
																	   -- + use, use_with

	HAS illegal_parameter_string "Please state inside double quotes ("""") what you want to $v.".	-- answer, say, say_to, write
	
	HAS illegal_parameter_talk_sg "That's not something you can talk to.".			-- ask, ask_for, say_to, tell
	HAS illegal_parameter_talk_pl "Those are not something you can talk to.".

	HAS illegal_parameter_there "It's not possible to $v there.".					-- look_behind, look_in, look_under 
	
	HAS illegal_parameter2_there "It's not possible to $v anything there.".    		-- empty_in, empty_on, pour_in, pour_on, put_in,  
															    		 -- + put_on, put_against, put_behind, put_near, 
																	 -- + put_under, throw_in, throw_to, tie_to, write

	HAS illegal_parameter_what_sg "That's not something I know about.".				-- what_is, where_is
	HAS illegal_parameter_what_pl "Those are not something I know about.".			-- what_is, where_is
	HAS illegal_parameter_who_sg "That's not somebody I know about.".				-- who_is
	HAS illegal_parameter_who_pl "Those are not somebody I know about.".				-- who_is
		
	
	-- verb check messages, found before DOES sections of verbs and used mainly in 'lib_verbs.i':	
	-- ==========================================================================================


	-- a) attribute checks
	----------------------

		
	-- the general check message for when an instance cannot be used with the verb :
	--------------------------------------------------------------------------------	
		
	HAS check_obj_suitable_sg "That's not something you can $v.".				-- (numerous)				
	HAS check_obj_suitable_pl "Those are not something you can $v.".


	-- variations of the above message, needed for example when a preposition is required after the verb:
	-----------------------------------------------------------------------------------------------------

	HAS check_obj_suitable_at "You can't $v anything at $+2.".					-- fire_at, throw_at, throw_to
	HAS check_obj2_suitable_for_sg "That's not something you can $v for.".		-- ask_for
	HAS check_obj2_suitable_for_pl "Those are not something you can $v for.".
	HAS check_obj_suitable_off_sg "That's not something you can $v off.".			-- turn_off, switch_off
	HAS check_obj_suitable_off_pl "Those are not something you can $v off.".
	HAS check_obj_suitable_on_sg "That's not something you can $v on.".			-- knock, switch_on, turn_on
	HAS check_obj_suitable_on_pl "Those are not something you can $v on."	.	
	HAS check_obj_suitable_with_sg "That's not something you can $v with.".		-- play_with
	HAS check_obj_suitable_with_pl "Those are not something you can $v with.".		
	HAS check_obj2_suitable_with_sg "That's not something you can $v things with.".	  -- break_with, burn_with, close_with, cut_with, fill_with, 
	HAS check_obj2_suitable_with_pl "Those are not something you can $v things with.".	 -- + lock_with, open_with, pry_with, push_with,
																	 -- + touch_with, unlock_with

	HAS check_obj_suitable_examine_sg "That's not something you can examine.".			-- examine
	HAS check_obj_suitable_examine_pl "Those are not something you can examine.".		-- examine

	HAS check_obj_suitable_look_out_sg "That's not something you can look out of.".		-- look_out_of
	HAS check_obj_suitable_look_out_pl "Those are not something you can look out of.".			
	HAS check_obj_suitable_look_through "You can't look through $+1.".				-- look_through


	-- checks for open, closed and locked objects:
	----------------------------------------------

	HAS check_obj_not_open_sg "$+1 is already open.".					-- open, open_with
	HAS check_obj_not_open_pl "$+1 are already open.".
	HAS check_obj_open1_sg "$+1 is already closed.".					-- close, close_with
	HAS check_obj_open1_pl "$+1 are already closed.".
	HAS check_obj_open2_sg "You can't, since $+1 is closed.".				-- empty (in/on), look_in, pour (in/on)
	HAS check_obj_open2_pl "You can't, since $+1 are closed.".
	HAS check_obj2_open_sg "You can't, since $+2 is closed.".				-- empty_in, pour_in, put_in, throw_in
	HAS check_obj2_open_pl "You can't, since $+2 are closed.".
	HAS check_obj_locked_sg "$+1 is already unlocked.".					-- unlock, unlock_with
	HAS check_obj_locked_pl "$+1 are already unlocked.".
	HAS check_obj_not_locked_sg "$+1 is already locked.". 				-- lock, lock_with
	HAS check_obj_not_locked_pl "$+1 are already locked.".
	


	-- checks for "not reachable" and "distant" objects:
	----------------------------------------------------

	HAS check_obj_reachable_sg "$+1 is out of your reach.".				-- (numerous)
	HAS check_obj_reachable_pl "$+1 are out of your reach.".
	HAS check_obj2_reachable_sg "$+2 is out of your reach.".				-- empty_in, fill_with, pour_in, put_in, take_from, tie_to		
	HAS check_obj2_reachable_pl "$+2 are out of your reach.".
	HAS check_obj_reachable_ask "$+1 can't reach $+2.".					-- ask_for
	HAS check_obj_not_distant_sg "$+1 is too far away.".					-- (numerous)
	HAS check_obj_not_distant_pl "$+1 are too far away.".
	HAS check_obj2_not_distant_sg "$+2 is too far away.".					-- empty_in, fill_with, pour_in, put_in, show, take_from,  																 -- + throw_at, throw_in, throw_to
	HAS check_obj2_not_distant_pl "$+2 are too far away.".
	

	-- checks for the hero sitting or lying_down:
	---------------------------------------------

	HAS check_hero_not_sitting1 "It is difficult to $v while sitting down.".   		-- (with many intransitive verbs)
	HAS check_hero_not_sitting2 "It is difficult to $v anything while sitting down.".	-- (with many transitive verbs)
	HAS check_hero_not_sitting3 "It is difficult to $v anywhere while sitting down.".	-- (with many verbs of motion)
	HAS check_hero_not_sitting4 "You're sitting down already.".					-- sit, sit_on
	HAS check_hero_not_lying_down1 "It is difficult to $v while lying down.".			-- (with many intranstive verbs)
	HAS check_hero_not_lying_down2 "It is difficult to $v anything while lying down.".	-- (with many transitive verbs)
	HAS check_hero_not_lying_down3 "It is difficult to $v anywhere while lying down.".	-- (with many verbs of motion)
	HAS check_hero_not_lying_down4 "You're lying down already.".					-- lie_down, lie_in


	-- other attribute checks:
	--------------------------

	HAS check_act_can_talk_sg "That's not something you can talk to.".				-- ask, ask_for, say_to, tell
	HAS check_act_can_talk_pl "Those are not something you can talk to.". 

	HAS check_obj_allowed_in_sg "$+1 doesn't belong in $+2.". 						-- empty_in, pour_in, put_in, throw_in
	HAS check_obj_allowed_in_pl "$+1 don't belong in $+2.".	
	
	HAS check_obj_broken_sg "That doesn't need fixing.".  						-- fix					
	HAS check_obj_broken_pl "Those don't need fixing.".

	HAS check_obj_inanimate1 "$+1 wouldn't probably appreciate that.".				-- push, push_with, scratch, search
	HAS check_obj_inanimate2 "You are not sure whether $+1 would appreciate that.".		-- rub, touch, touch_with
	
	HAS check_obj_movable "It's not possible to $v $+1.".							-- lift, pull, push, push_with, shake, take, take_from
			
	HAS check_obj_not_scenery_sg "$+1 is not important.".							-- examine, take, take_from
	HAS check_obj_not_scenery_pl "$+1 are not important.".

	HAS check_obj2_not_scenery_sg "$+2 is not important.".							-- ask_for, take_from
	HAS check_obj2_not_scenery_pl "$+2 are not important.".

	HAS check_obj_suitable_there "It's not possible to $v there.".					-- look_behind, look_in, look_under
	HAS check_obj2_suitable_there "It's not possible to $v anything there.".			-- throw_in, tie_to

	HAS check_obj_takeable "You don't have $+1.".							-- (numerous; this check is in verbs using implicit taking)
	HAS check_obj2_takeable1 "You don't have $+2.".								-- fill_with
	HAS check_obj2_takeable2 "You can't have $+2.".								-- ask_for
	
	HAS check_obj_weight_sg "$+1 is too heavy to $v.".							-- lift, take, take_from
	HAS check_obj_weight_pl "$+1 are too heavy to $v.".

	HAS check_obj_writeable "Nothing can be written there.".						-- write


	-- b) location and containment checks for actors and objects
	------------------------------------------------------------


	-- containment checks for actors other than the hero (checks for the hero are listed separately below):
	-------------------------------------------------------------------------------------------------------
	
	HAS check_act_near_hero "You don't quite know where $+1 went.  					-- follow
		You should state a direction where you want to go.".

	HAS check_obj_in_act_sg "$+2 doesn't have $+1.".							-- take_from
	HAS check_obj_in_act_pl "$+2 don't have $+1.".
	HAS check_obj_not_in_act_sg "$+2 already has $+1.".							-- give
	HAS check_obj_not_in_act_pl "$+2 already have $+1.". 


	-- location and containment checks for the hero:
	------------------------------------------------

	HAS check_count_weapon_in_hero "You are not carrying any firearms.".				-- shoot

	HAS check_hero_in_cont "But you aren't in $+1 at present.".					-- exit
	HAS check_hero_not_in_cont "But you are already in $+1!".						-- enter

	HAS check_obj_not_at_hero_sg "$+1 is right here.".  							-- find, follow, go_to, where_is
	HAS check_obj_not_at_hero_pl "$+1 are right here.".
	HAS check_obj_in_hero "You don't have the $+1.".							-- drop, fire, fire_at, put, show
	HAS check_obj2_in_hero "You don't have the $+2.".							-- (numerous)
	HAS check_obj_not_in_hero1 "It doesn't make sense to $v something you're holding.".  	-- attack, attack_with, kick, lift, shoot, shoot_with
	HAS check_obj_not_in_hero2 "You already have $+1.".							-- take, take_from
	HAS check_obj2_not_in_hero1 "You are carrying $+2.".   						-- throw_at, throw_in, throw_to
	HAS check_obj2_not_in_hero2 "That would be futile.".							-- put_against, put_behind, put_near, put_under
	HAS check_obj2_not_in_hero3 "You already have $+2.".							-- ask_for


	-- checking whether an object is in a container or not:
	-------------------------------------------------------

	HAS check_cont_not_in_obj "That doesn't make sense.".							-- empty_in, pour_in, put_in
	HAS check_obj_in_cont_sg "$+1 is not in $+2.".								-- take_from
	HAS check_obj_in_cont_pl "$+1 are not in $+2.".
	HAS check_obj_not_in_cont_sg "$+1 is in $+2 already.".						-- put_in, throw_in
	HAS check_obj_not_in_cont_pl "$+1 are in $+2 already.".
	HAS check_obj_not_in_cont2 "$+1 is already full of $+2.".						-- fill_with


	-- checking whether an object is on a surface or not:
	-----------------------------------------------------
	
	HAS check_obj_on_surface_sg "$+1 is not on $+2.".							-- take_from
	HAS check_obj_on_surface_pl "$+1 are not on $+2.".
	HAS check_obj_not_on_surface_sg "$+1 is already on $+2.".						-- put_on
	HAS check_obj_not_on_surface_pl "$+1 are already on $+2.".
	

	-- checking whether an object is worn or not:
	---------------------------------------------	

	HAS check_obj_in_worn "You are not wearing $+1.".    							-- remove, take_off ('classes.i')
	HAS check_obj_not_in_worn1 "You are already wearing $+1.".   					-- put_on, wear ('classes.i')
    	HAS check_obj_not_in_worn2 "It doesn't make sense to $v something you're wearing.".	-- attack, attack_with, kick, shoot, shoot_with
	HAS check_obj_not_in_worn3 "You'll have to take off $+1 first.".				-- drop
    	

	-- c) checking location states
	------------------------------	

    	HAS check_current_loc_lit "It is too dark to see.".						-- (numerous)


	-- d) checks guarding against actions directed at the hero him-/herself
	-----------------------------------------------------------------------

	HAS check_obj_not_hero1 "It doesn't make sense to $v yourself.".			-- ask, ask_for, attack, attack_with, catch, follow
																   -- kick, listen, pull, push, push_with, take, 
																   -- take_from,tell 
	HAS check_obj_not_hero2 "There is no need to be that desperate.".  			-- fire_at, kill, kill_with, shoot, shoot_with
	HAS check_obj_not_hero3 "That wouldn't accomplish anything.".				-- scratch, touch
	HAS check_obj_not_hero4 "You're right here.".							-- find, go_to
	HAS check_obj_not_hero5 "You don't need to be freed.".					-- free
	HAS check_obj_not_hero6 "There is no time for that now.".								-- kiss, play_with, rub
	HAS check_obj_not_hero7 "Turning your head, you notice nothing unusual behind yourself.".		-- look_behind 
	HAS check_obj_not_hero8 "You notice nothing unusual under yourself.".						-- look_under
	HAS check_obj2_not_hero1 "That doesn't make sense.".						-- say_to, show, take_from, touch_with, throw_at/in/to
	HAS check_obj2_not_hero2 "That would be futile.".						-- put_against, put_behind, put_near, put_under
	HAS check_obj2_not_hero3 "You can't $v things to yourself.".				-- give, tie_to


	-- e) checks guarding against actions where an object is used with itself
	-------------------------------------------------------------------------

	HAS check_obj_not_obj2_at "It doesn't make sense to $v something at itself.".		-- fire_at, throw_at
	HAS check_obj_not_obj2_from "It doesn't make sense to $v something from itself.".	-- take_from
	HAS check_obj_not_obj2_in "It doesn't make sense to $v something into itself.".		-- empty_in, pour_in, put_in, throw_in
	HAS check_obj_not_obj2_on "It doesn't make sense to $v something onto itself.".		-- empty_on, pour_on, put_on
	HAS check_obj_not_obj2_to "It doesn't make sense to $v something to itself.".		-- give, show, throw_to, tie_to
	HAS check_obj_not_obj2_with "It doesn't make sense to $v something with itself.".  	-- attack_with, break_with, burn_with, close_with, 																		   -- + cut_with, fill_with, lock_with, 
																	   -- + open_with, push_with, pry_with, shoot_with,  
																	   -- + touch_withm unlock_with, use_with
	
	HAS check_obj_not_obj2_put "That doesn't make sense."	.					-- put_against, put_behind, put_near, put_under

	
    	-- f) additional checks for classes:
	------------------------------------

	HAS check_clothing_sex "On second thoughts you decide $+1 won't really suit you.".			-- clothing: wear
	HAS check_cont_not_supporter "You can't put $+1 inside $+2.".							-- supporter: put_in
	HAS check_device_on_sg "$+1 is already off.". 										-- device: turn_off, switch_off
	HAS check_device_on_pl "$+1 are already off.".
	HAS check_device_not_on_sg "$+1 is already on.". 									-- device: turn_on, switch_on
	HAS check_device_not_on_pl "$+1 are already on.".
	HAS check_door_matching_key "You can't use $+2 to $v $+1.".							-- door: lock_with, unlock_with	
	HAS check_lightsource_lit_sg "But $+1 is not lit.".									-- lightsource: extinguish, turn_off
	HAS check_lightsource_lit_pl "But $+1 are not lit.".
	HAS check_lightsource_not_lit_sg "$+1 is already lit.".								-- lightsource: light, turn_on
	HAS check_lightsource_not_lit_pl "$+1 are already lit.".
	HAS check_lightsource_switchable_sg "That's not something you can switch on and off."	.		-- lightsource: switch 
	HAS check_lightsource_switchable_pl "Those are not something you can switch on and off.".
	HAS check_liquid_vessel_not_cont "You can't carry $+1 around in your bare hands.".			-- liquid: take_from
	HAS check_obj_not_broken "Nothing happens.".										-- device, lightsource: light, switch, turn_on


	-- messages for implicit taking:
	-- =============================

    	HAS implicit_taking_message "(taking $+1 first)$n".	

	-- The following verbs are preceded by implicit taking:
			-- bite, drink, eat, empty, empty_in, empty_on, give, pour, pour_in, pour_on,  
			-- put_in, put_on, throw, throw_at, throw_in, throw_to, tie_to.
	-- In ditransitive verbs, only the first parameter (the direct object) is taken implicitly.
			-- For example, >push door with pole  won't work if the hero is not carrying the pole.




   -- ==========================================================================================

	-- These three attributes, as well as the 'schedule' statement following them,
	-- are needed for the 'notify' command ('lib_verbs.i'); ignore.

	HAS oldscore 0. 		
			-- Records previous score so 'checkscore' event
 			-- can compare with the current score 
	IS notify_turned_on. 		
			-- Set by 'notify' verb, records whether 
			-- player wants to see score messages or not. 
	IS NOT seen_notify. 	
			-- Records whether player has seen the notify verb 
			-- instructions yet. 

	INITIALIZE
		SCHEDULE check_score AT hero AFTER 0.		

   -- ==========================================================================================


-- The my_game instance defined as a meta-location (ignore):

				FOR EACH l ISA LOCATION
					DO
						EXCLUDE nowhere FROM nested OF l. 
						IF COUNT ISA LOCATION, AT l > 0
							THEN 
								FOR EACH x ISA LOCATION, AT l
									DO
										INCLUDE x IN nested OF l.
								END FOR.
						END IF.
				END FOR.
							
				FOR EACH l ISA LOCATION
					DO
			  			IF l <> my_game AND l <> nowhere
							THEN LOCATE l AT my_game.
						END IF.
				END FOR.
		
				FOR EACH r1 ISA ROOM
					DO 
						LOCATE r1 AT indoor.		
				END FOR.

				FOR EACH s1 ISA SITE
					DO 
						LOCATE s1 AT outdoor.
				END FOR.

				FOR EACH l ISA LOCATION 
					DO
  					IF nested OF l <> {} AND l <> my_game AND l <> nowhere
						THEN
    			   				FOR EACH x ISA LOCATION, IN nested OF l 
								DO
									IF l <> my_game AND x <> my_game
      									THEN LOCATE x AT l.
									END IF.	
    							END FOR.
  			 		END IF.
				END FOR.
	
						

-- We ensure that the 'visited' and 'described' attributes of the starting location 
-- are correct at the start of the game:

		SET visited OF location OF hero TO 1.
 		SET described OF location OF hero TO 1.


-- Finally, for restricted actions, we implement the following attributes, corresponding to the library verbs.
	-- If you change any of these to CAN NOT..., for examle "CAN NOT attack.", that verb, together with its
	-- synonyms, is disabled in-game. The restricted_response of the my_game instance will be shown instead.


	CAN about.       
	CAN 'again'.
	CAN answer.      -- (+ reply)                                       
	CAN ask.         -- (+ enquire, inquire, interrogate)               
	CAN ask_for.                                                     
	CAN attack.      -- (+ beat, fight, hit, punch)                     
	CAN attack_with.                                                 
	CAN bite.        -- (+ chew)		                                  
	CAN break.       -- (+ destroy)                                     
	CAN break_with.                                                  
	CAN 'brief'.                                                       
	CAN burn.                                                        
	CAN burn_with.                                                   
	CAN buy.         -- (+ purchase)                                    			
	CAN catch.                                                                                
	CAN clean.       -- (+ polish, wipe)                                                        
	CAN climb.                                                                                 
	CAN climb_on.                                                    						
	CAN climb_through.                                                                
	CAN close.       -- (+ shut)                                                                
	CAN close_with.                                                             
	CAN consult.                                                     			
	CAN credits.     -- (+ acknowledgments, author, copyright)                                      					
	CAN cut.                                                                                   
	CAN cut_with.                                                                 
	CAN dance.                                                       
	CAN dig.                                                                                  
	CAN dive.                                                        
	CAN dive_in.                                                     
	CAN drink.                                                       
	CAN drive.                                                       
	CAN drop.        -- (+ discard, dump, reject)                                               
	CAN eat.                                                         		
	CAN 'empty'.                                                       
	CAN empty_in.                                                    
	CAN empty_on.                                                    
	CAN enter.                                                       
	CAN examine.     -- (+ check, inspect, observe, x)                  
	CAN 'exit'.                                                        
	CAN extinguish.  -- (+ put out, quench)                              
	CAN fill.                                                        			
	CAN fill_with.                                                   			
	CAN find.        -- (+ locate)                                                              
	CAN fire.                                                        
	CAN fire_at.										  
	CAN fix.		  -- (+ mend, repair)						  					 
	CAN follow.                                                      		
	CAN free.        -- (+ release)                                     
	CAN get_up.                                                      					
	CAN get_off.                                                     
	CAN give.                                                        
	CAN go_to.                                                       			
	CAN hint.        -- (+ hints)                                       
	CAN i.   		 -- (+ inv, inventory)                                      
	CAN jump.                                                        
	CAN jump_in.                                                     			
	CAN jump_on.                                                     			
	CAN kick.                                                        
	CAN kill.        -- (+ murder)                                      			
	CAN kill_with.                                                   			
	CAN kiss.        -- (+ hug, embrace)  
	CAN knock.                              
	CAN lie_down.                                                    
	CAN lie_in.                                                           
	CAN lie_on.                                                      
	CAN lift.                                                        
	CAN light.       -- (+ lit)                                         
	CAN listen0.                                                     
	CAN listen.                                                      
	CAN lock.                                                        
	CAN lock_with.                                                   
	CAN 'look'.        -- (+ gaze, peek)                                  						
	CAN look_at.                                                     
	CAN look_behind.                                                 			
	CAN look_in.                                                     			
	CAN look_out_of.                                                 				
	CAN look_through.                                                		
	CAN look_under.                                                  			
	CAN look_up.                                                     
	CAN 'no'.                                                          						
	CAN 'notify'. 
	CAN notify_on.
	CAN notify_off.                                            
	CAN open.                                                                                  
	CAN open_with.                                                               	
	CAN 'play'.                                                                                  
	CAN play_with.                                                   
	CAN pour.                          
	CAN pour_in.                                     
	CAN pour_on.                               
	CAN pray.                                                        
	CAN pry.                                                         
	CAN pry_with.                                                    
	CAN pull.                                                        
	CAN push.                                                        
	CAN push_with.                                                   	
	CAN put.         -- (+ lay, place)                                  
	CAN put_against.									  
	CAN put_behind.                                                 			
	CAN put_down.                                                    
	CAN put_in.      -- (+ insert)                                      
	CAN put_near.                                                                  
	CAN put_on.                                                                  
	CAN put_under.  
	CAN 'quit'.                                                             
	CAN read.                                                        
	CAN remove.										  				
	CAN 'restart'.                                                     					
	CAN 'restore'.                                                     		
	CAN rub.                                                                             
	CAN 'save'.                                                        
	CAN 'say'.                                                          	
	CAN say_to.                                                      
	CAN 'score'.                                                        
	CAN scratch.                                                                          
	CAN 'script'.    
	CAN script_on.
	CAN script_off.                                                  
	CAN search.                                                                              
	CAN sell.                                                        
	CAN shake.                                                                              
	CAN shoot. -- (at)                                                  
	CAN shoot_with.                                                  			
	CAN shout.       -- (+ scream, yell)                                                               
	CAN 'show'.      -- (+ reveal)                                                
	CAN sing.                                                        						
	CAN sip.                                                        
	CAN sit. -- (down)                                                  
	CAN sit_on.                                                      
	CAN sleep.       -- (+ rest)                                        
	CAN smell0.                                                      
	CAN smell.                                                       
	CAN squeeze.                                                                          
	CAN stand. -- (up)                                                  
	CAN stand_on.                                                    
	CAN swim.                                                        
	CAN swim_in.   
	CAN switch.                                                  
	CAN switch_on.	  			  
	CAN switch_off.  			  
	CAN take.        -- (+ carry, get, grab, hold, obtain)              
	CAN take_from.   -- (+ remove from)                                 
	CAN talk.                                                        						
	CAN talk_to.     -- (+ speak)                                       
	CAN taste.       -- (+ lick)                                        
	CAN tear.        -- (+ rip)                                         
	CAN tell.        -- (+ enlighten, inform)                           	
	CAN think.                                                       
	CAN think_about.                                                 
	CAN throw.                                                       
	CAN throw_at.                                                    	
	CAN throw_in.                                                    
	CAN throw_to.                                                     
	CAN tie.                                                                                    
	CAN tie_to.                                                                    
	CAN touch.       -- (+ feel)
	CAN touch_with.                                                                			
	CAN turn.        -- (+ rotate)                                                                
	CAN turn_on.                                                                    
	CAN turn_off.                                                    
	CAN undress.										  
	CAN unlock.                                                      
	CAN unlock_with.                                                 	
	CAN 'use'.                                                         
	CAN use_with.                                                    
	CAN 'verbose'.                                                     
	CAN 'wait'.        -- (+ z)                                           
	CAN wear.											  
	CAN what_am_i.                                                   	
	CAN what_is.                                                     	
	CAN where_am_i.                                                  
	CAN where_is.                                                    
	CAN who_am_i.                                                    
	CAN who_is.
	CAN write.
	CAN yes.


END EVERY.





-- The banner:
-- ===========


THE banner ISA DEFINITION_BLOCK

  	DESCRIPTION
   		
		"$p" STYLE alert. SAY title OF my_game. STYLE normal. 
   		
		IF subtitle OF my_game <> ""
			THEN "$n" SAY subtitle OF my_game.
   		END IF.
   		
		"$n(C)" SAY year OF my_game. "by" SAY author OF my_game.
   		
		"$nProgrammed with the ALAN Interactive Fiction Language v3.0"
   		
		IF version OF my_game <> "0"
			THEN "$nVersion" SAY version OF my_game. 
   		END IF.	
   		
		"$nAll rights reserved."

END THE banner.











-- end of file.
