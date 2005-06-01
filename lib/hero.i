-- hero.i
-- Library version 0.6.1dev

Synonyms
  me, myself, self = hero.

The hero Isa actor
Container
  Limits
    Count 10 Then
      "You can't carry anything more. You have to drop something 
	first."
    weight 50 Then
      "You can't carry anything more. You have to drop something 
	first."
    Header
      "You are carrying"
    Else
      "You are empty-handed."
End The hero.
