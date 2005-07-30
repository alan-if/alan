-- person.i
-- Library version 0.6.0


-- Classes for people

Every person Isa actor
  Is Not named.				-- If it has a proper name
  Description
    If This Is Not named Then
      "There is" Say An This. "here."
    Else
      Say This. "is here."
    End If.
End Every person.

Every male Isa person
  Pronoun him.
End Every male.

Every female Isa person
  Pronoun her.
End Every female.
