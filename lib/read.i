-- read.i
-- Library version 0.3

-- 0.3 
--    - added 'the' in read CHECK msg
--    - use named attribute in formatting text


SYNTAX
	read = read (obj)
		WHERE obj ISA OBJECT
			ELSE "You can't read that."


VERB read
	CHECK obj IS readable
		ELSE 
			"There is nothing written on"
			 IF obj IS named THEN
				SAY obj.
			ELSE
				"the $1"
			END IF.
			"$$."
	DOES
		"You read"
		IF obj IS named THEN
			SAY obj.
		ELSE
			"the $1"
		END IF.
		"$$."
END VERB.


