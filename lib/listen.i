-- listen.i
-- Library version 0.4.1

-- 0.4.1 - converted to ALANv3



SYNTAX
	listen_to = listen 'to' (obj)
		WHERE obj ISA THING
			ELSE "You can't listen to that!"

VERB listen_to
	DOES
		"You listen to"
		IF obj IS named THEN
			SAY obj.
		ELSE
			"the $1"
		END IF.
		"$$."
END VERB.



SYNTAX
	listen = listen.


VERB listen
	DOES
		"You hear nothing unusual."
END VERB.