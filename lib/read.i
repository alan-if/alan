-- read.i
-- Library version 0.4.1

-- 0.4.1 - converted to ALANv3


ADD TO EVERY OBJECT 
IS
	NOT readable.
END ADD TO OBJECT.


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


