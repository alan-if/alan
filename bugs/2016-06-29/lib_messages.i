-- ALAN Standard Library v2.0
-- Messages (file name: 'lib_messages.i')


-- All runtime messages are listed below.  

-- Many of these messages are not in their default form as described
-- in the ALAN language manual, but have been slightly edited for this library.


MESSAGE 
	AFTER_BUT: "You must give at least one object after '$1'."
	AGAIN: ""
	BUT_ALL: "You can only use '$1' AFTER '$2'."
	CAN_NOT_CONTAIN: "$+1 can not contain $+2."
	CANT0: "You can't do that."    
		 -- note that the fifth token in CANT0 is a zero, not an 'o'.
	CARRIES: 
		IF parameter1 IS NOT plural
			THEN "$+1 is carrying"
			ELSE "$+1 are carrying"
		END IF.
	CONTAINMENT_LOOP:
		"Putting $+1 in" 
			IF parameter1 IS NOT plural
				THEN "itself"
				ELSE "themselves"
			END IF.
		"is impossible."	   
	CONTAINMENT_LOOP2: "Putting $+1 in $+2 is impossible since $+2 already" 
					IF parameter2 IS NOT plural
						THEN "is"
						ELSE "are"
					END IF. 
				    "inside $+1."
	'CONTAINS': 
		IF parameter1 IS NOT plural
			THEN "$+1 contains"
			ELSE "$+1 contain"
		END IF.
	CONTAINS_COMMA: "$01" 
		IF parameter1 ISA CLOTHING
			THEN 
				-- the following snippet adds "(being worn)" after all
				-- pieces of clothing worn by an NPC, at 'x [actor]'
				IF parameter1 IS donned 
					THEN "(being worn)" 
				END IF.
		END IF.
		"$$,"
    	CONTAINS_AND: "$01"
		IF parameter1 ISA CLOTHING
			THEN 
				-- the following snippet adds "(being worn)" after all
				-- pieces of clothing worn by an NPC, after 'x [npc]'
				IF parameter1 IS donned 
					THEN "(being worn)" 
				END IF.
		END IF.
		"and"
	CONTAINS_END: "$01"
		IF parameter1 ISA CLOTHING
			THEN 
				-- the following snippet adds "(being worn)" after all
				-- pieces of clothing worn by an NPC, after 'x [npc]'
				IF parameter1 IS donned 
					THEN "(being worn)" 
				END IF.
		END IF.
		"." 
	EMPTY_HANDED: 
		IF parameter1 IS NOT plural
			THEN "$+1 is empty-handed."
			ELSE "$+1 are empty-handed."
		END IF.
			
	HAVE_SCORED: "You have scored $1 points out of $2."
    	IMPOSSIBLE_WITH: "That's impossible with $+1."
	IS_EMPTY: 
		IF parameter1 IS NOT plural
			THEN "$+1 is empty."
			ELSE "$+1 are empty."
		END IF.
	MORE: "<More>"
	MULTIPLE: "You can't refer to multiple objects with '$v'."
	NO_SUCH: "You can't see any $1 here."
	NO_WAY: "You can't go that way."
	NOT_MUCH: "That doesn't leave much to $v!"
	NOUN: "You must supply a noun."
    	NOT_A_SAVEFILE: "That file does not seem to be an Alan game save file."
    	QUIT_ACTION: "Do you want to RESTART, RESTORE, QUIT or UNDO? "
		-- these four alternatives are hardwired to the interpreter and cannot be changed.
	REALLY: "Are you sure (press ENTER to confirm)?"
	RESTORE_FROM: "Enter file name to restore from"
	SAVE_FAILED: "Sorry, save failed."
	SAVE_MISSING: "Sorry, could not open the save file."	
	SAVE_NAME: "Sorry, the save file did not contain a save for this adventure."
   	SAVE_OVERWRITE: "That file already exists, overwrite (y)?"
	SAVE_VERSION: "Sorry, the save file was created by a different version."
	SAVE_WHERE: "Enter file name to save in"
	SEE_START: 
		IF parameter1 IS NOT plural
			THEN "There is $01"
			ELSE "There are $01"
		END IF.
	SEE_COMMA: ", $01"
	SEE_AND: "and $01"
	SEE_END: "here."
	NO_UNDO: "No further undo available."
	UNDONE: "'$1' undone."
	UNKNOWN_WORD: "The word '$1' is not in your vocabulary."
	WHAT: "That was not understood."
    	WHAT_WORD: "It is not clear what you mean by '$1'."
	WHICH_PRONOUN_START: "It is not clear if you by '$1'"
	WHICH_PRONOUN_FIRST: "mean $+1"
	WHICH_START: "It is not clear if you mean $+1"
	WHICH_COMMA: ", $+1"
	WHICH_OR: "or $+1."


-- end of file.
