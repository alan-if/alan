
SYNTAX
      throw = throw (obj) *
		WHERE obj ISA OBJECT
			ELSE "You can't throw people."

      throw_at = throw (obj1) 'at' (obj2)
		WHERE obj1 ISA OBJECT
			ELSE "You can't throw people."
		AND obj2 ISA OBJECT OR ACTOR
			ELSE "You can't throw anything at that."

      throw_to = throw (obj1) 'to' (obj2)
		WHERE obj1 ISA OBJECT
			ELSE "You can't be seriuos."
		AND obj2 ISA OBJECT OR ACTOR
			ELSE "You can't throw anything at that."

      throw_in = throw (obj1) 'in' (obj2)
		WHERE obj1 ISA OBJECT
			ELSE "Don't be silly."
	        AND obj2 ISA CONTAINER
		      ELSE "You can't throw anything in the $2."



VERB throw
      CHECK obj IN inventory
	      ELSE "You haven't got that!"
      DOES
	      "You can't throw very far, the $2 and ends up on the ground."
	      LOCATE obj HERE.
END VERB.

--

VERB throw_at, throw_to 
      CHECK obj1 IN inventory
	      ELSE "You haven't got that!"
      AND obj2 NOT IN inventory
	      ELSE "You are carrying the $2!"
      DOES
	      "The $1 bounces harmlessly off the $2 and ends up on the ground."
	      LOCATE obj1 HERE.
END VERB.

--

VERB throw_in
      CHECK obj1 IN inventory
	      ELSE "You haven't got that!"
      AND obj2 HERE AND obj2 NOT IN inventory
	      ELSE "I don't see any $2 here."
      AND obj1 <> obj2
	      ELSE "Now, that would be a good trick!"
      DOES
	      LOCATE obj1 IN obj2.
	      "Done."
END VERB.
