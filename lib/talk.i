-- talk.i
-- Library version 0.5.0

-- 0.4.1 - converted to ALANv3


-- for verbs like 'ask' and 'tell' you need to individually program responses
-- to each topic (ie 'obj') for any actor who you want to respond to that
-- topic. 
-- Remember to use the 'when' clause so that actors only respond when they're
-- being talked to (and not when they're the *subject* of the conversation!)
-- see Section 3.7.1 - 'verb alternatives' - in the Alan manual.
-- eg:
-- ACTOR Simon
--    ....
--    VERB ask
--       WHEN act
--           DOES ONLY
--              IF topic = ball then
--                  "Simon replies ""I love playing ball sports. Football is
--                   my favourite."""
--              ELSIF obj = fred then
--                  .....
--              ELSE
--                  "Simon shrugs. ""Sorry, I don't know anything about 
--                  that."""
--              END IF.
--     END VERB ask.
--     ....
-- END ACTOR Simon.


Add To Every thing
  Is
    Not can_talk.
End Add To thing.

Add To Every actor 
  Is
     can_talk.
End Add To actor.


Synonyms
  yell = shout.
  scream = shout.

Syntax
  shout = shout.

Verb shout
  Does
    "You make a lot of noise."
End Verb.



Syntax
  say_word = 'say' (topic)!
    Where topic Isa thing
      Else "You can't say that."

Add To Every thing
  Verb say_word
    Does
      "$o? That's a nice word!"
  End Verb.
End Add To.



Syntax
  say_to = 'say' (topic)! 'to' (act)
    Where topic Isa thing
      Else "You can't say that."
    And act Isa thing
      Else "You can't talk to that."

Add To Every thing
  Verb say_to
    When act
      Check act Has can_talk
	Else "You can't talk to that."
      Does
	If act Is named Then
	  Say act.
	  "doesn't seem interested."
	Else
	  "The $2 doesn't seem interested."
	End If.
  End Verb.
End Add To.



Syntax
  ask = ask (act) about (topic)!
    Where topic ISA THING 
      Else "You can't ask about that."
    And act Isa thing
      Else "You can't talk to that."

Add To Every thing
  Verb ask
    When act
      Check act Has can_talk
        Else "You can't talk to that."
      Does
	If act Is named Then
	  Say act.
	  "says ""I don't know anything about"
	Else
	  "The $1 says ""I don't know anything about"
	End If.
	If topic Is named Then
	  "them!"""
	Else
	  "that!"""
	End If.
  End Verb.
End Add To.




Syntax
  talk_to = talk 'to' (act) about (topic)!
    Where topic Isa thing
      Else "You can't ask about that."
    And act Isa thing
      Else "You can't talk to that."

Syntax
  tell = tell (act) about (topic)!
    Where topic Isa thing
      Else "You can't ask about that."
    And act Isa thing
      Else "You can't talk to that."

Add To Every thing
  Verb tell, talk_to
    When  topic
      Check act Has can_talk
	Else "You can't talk to that."
      Does
	"""I don't think I need to know about"
	If topic Is named Then
	  Say topic.
	Else
	  "the $2"
	End If.
	","" says"
	If act Is named Then
	  Say act. 
	Else
	  "the $1"
	End If. 
	"$$."
  End Verb.
End Add To.




Syntax
  talk_to_a = talk 'to' (act)
    Where act Isa thing
      Else "You can't talk to that."

Add To Every thing
  Verb talk_to_a
    Check act Has can_talk
      Else "You can't talk to that."
    Does
      If act Is named Then
	Say act.
	"looks at you, seemingly wondering if you have anything specific
		to talk about."
      Else
	"The $1 looks at you, seemingly wondering if you have anything 
		specific to talk about."
      End If. 
  End Verb.
End Add To.
