SYNONYMS
	place = put.

SYNTAX
        put = put (obj) *
		WHERE obj ISA OBJECT
			ELSE "You can't put that anywhere."

VERB put
	CHECK obj IN inventory
		ELSE "You haven't got that."
	DOES
		LOCATE obj HERE.
		"Dropped."
END VERB.


SYNTAX
        put_in = put (obj1) 'in' (obj2)
		WHERE obj1 ISA OBJECT
			ELSE "You can't put that."
                AND obj2 ISA CONTAINER
                      ELSE "You can't put anything in the $2."

VERB put_in
      CHECK obj1 IN inventory
	      ELSE "You haven't got the $1!"
      AND obj1 <> obj2
	      ELSE "Now, that would be a good trick!"
      DOES
	      LOCATE obj1 IN obj2.
	      "Done."
END VERB.


SYNTAX
        put_near = put (obj1) near (obj2)
		WHERE obj1 ISA OBJECT
			ELSE "You can't put that."
                AND obj2 ISA OBJECT
                      ELSE "You can't put anything near the $2."

        put_behind = put (obj1) behind (obj2)
		WHERE obj1 ISA OBJECT
			ELSE "You can't put that."
                AND obj2 ISA OBJECT
                      ELSE "You can't put anything behind the $2."

        put_on = put (obj1) 'on' (obj2)
		WHERE obj1 ISA OBJECT
			ELSE "You can't put that."
                AND obj2 ISA OBJECT
                      ELSE "You can't put anything on the $2."

        put_under = put (obj1) under (obj2)
		WHERE obj1 ISA OBJECT
			ELSE "You can't put that."
                AND obj2 ISA OBJECT
                      ELSE "You can't put anything under the $2."

VERB put_near, put_behind, put_on, put_under 
      CHECK obj1 IN inventory
	      ELSE "You haven't got the $1!"
      AND obj2 NOT IN inventory
	      ELSE "You are carrying the $2!"
      DOES
	      "Naaah. I'd rather just put it down here."
	      LOCATE obj1 AT obj2.
END VERB.

