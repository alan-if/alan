-- give.i
-- Library version 0.4.1

-- 0.4.1 - converted to ALANv3



Syntax
  give = 'give' (obj) 'to' (recip)
    Where obj Isa object
      Else "You can only give away objects."
    And recip Isa thing
      Else "You can't give things to that!"
    And recip Isa Container
      Else "You can't give things to that!"

Add To Every object
  Verb give
    When obj
      Check obj In hero
	Else
	  "You don't have"
	  If obj Is named Then
	    Say obj.
	  Else
	    "the $1"
	  End If.
	  "$$."
      Does
	If recip=hero Then
	  "You already have"
	  If obj Is named Then
	    Say obj.
	  Else
	    "the $o"
	  End If.
	  "$$!"
	Else
	  "You give"
	  If obj Is named Then
	    Say obj.
	    "to"
	  Else
	    "the $1 to"
	  End If.
	  If recip Is named Then
	    Say recip.
	  Else
	    "the $2"
	  End If.
	  "$$."
	  Locate obj In recip.
	End If.
  End Verb.
End Add To.


