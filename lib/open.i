-- open.i
-- Library version 0.4.1

-- 0.4.1 - converted to ALANv3


add to every object 
is
	not Closeable.
	Closed.
end add to object. 

syntax
	Open = Open (Obj)
	where Obj isa object
		else "You can't open that."

add to every Object
  verb Open
	check Obj is Closeable
		else "You can't open that!"
	and Obj is Closed
		else "It's already open."
	does
		make Obj not Closed.
		if Obj is Named then
			say Obj.
			"is now open."
		else
			"The $o is now open."
		end if.
  end verb.
end add to.


syntax
	Open_With = open (Obj1) 'with' (Obj2)
		where Obj1 isa Object
			else "You can't open that."
		and Obj2 isa Object
			else "You can't open anything with that."

add to every Object
  verb Open_With
	check Obj2 in Hero
		else "You don't have"
		if Obj2 is Named then
			say Obj2.
		else
			"the $2"
		end if.
		"$$."
	does
		"You can't open"
		if Obj1 is Named then
			say Obj1.
		else
			"the $1"
		end if.
		"with"
		if Obj2 is Named then
			say obj2.
		else
			"the $2"
		end if.
		"$$."
  end verb.
end add to.



synonyms
	shut = close.

syntax
	close = close (obj)
		where Obj isa Object
			else "You can only close objects."
add to every Object
  verb Close
	check Obj is Closeable
		else "You can't close that."
	and Obj is not Closed
		else "It is not open."
	does
		make Obj Closed.
		if Obj is Named then
			say Obj.
			"is now closed."
		else
			"The $o is now closed."
		end if.
  end verb.
end add to.

syntax
	Close_With = close (obj1) 'with' (obj2)
		where Obj1 isa Object
			else "You can't close that."
		and Obj2 isa Object
			else "You can't close anything with that."

add to every Object
  verb Close_With
	check Obj2 in Hero
		else 
			"You don't have"
			if Obj2 is Named then
				say Obj2.
			else
				"the $2"
			end if.
			"$$."
	does
		"You can't close"
		if Obj1 is Named then
			say Obj1.
			"with"
		else
			"the $1 with"
		end if.
		if Obj2 is Named then
			say Obj2.
		else
			"the $2"
		end if.
		"$$."
  end verb.
end add to.

