Hello to all,

The four files belonging to the new proposed library have now been
uploaded to the 'files' area of this group. The library is numbered
0.9, with the intention that once any most immediate bugs and
misfeatures are taken care of, Standard Library v1.0 will be released
at the ALAN website.

Please take a look at how the library feels and please send comments
to the mailing list. Now is the time to make comments and suggestions!


Acknowledgements: Thanks to Thomas Nilsson and Göran Forslund for
ALAN. Also thanks to Thomas for continous and patient support and
encouragement throughout the project.  Thanks to Steve Griffiths for
feedback and suggestions for improvement, the earlier versions of the
library on which this version is based, and for the Score notification
code snippet.  Thanks to Alan Bampton for the 'xwear.i' extension
which has been assimilated in this library.



General features:

- the library consists of four files: classes.i, locations.i,
  messages.i and verbs.i.

- classes.i defines various object and actor classes

- locations.i defines the default directions (exits) and also various
  location classes and attributes

- messages.i lists all the standard runtime messages in the ALAN
  system, for easy editing and translation

- verbs.i lists all the verbs, which are considerably more numerous
  than previously. Their number is now somewhat on par with verbs in
  the libraries of Inform and TADS. Also commands other than verbs are
  here, such as 'verbose', 'inventory' and 'about'.

- in labeling parameters in syntax statements, tailored (mnemonic)
  parameters are used as much as possible to ease up code-reading and
  for clearer intention-revealing: e.g. eat (food), kill (victim) with
  (weapon), push (obj) with (instr).  Many verbs still have (obj) as
  the direct object parameter, but in many cases this is something
  different.

- objects are now implicitly taken if they are not carried already and
  if they pass all the usual checks: e.g. 'eat apple' -> (taking the
  apple first) You eat the apple.

- Alan Bamptons 'xwear' extension has been assimilated, disallowing
  clothes to be put on or taken off in an improper oder, e.g. you
  can't put on a shirt if you are wearing a jacket, etc.

- Steve Griffiths's score notification code has been assimilated. You
  can disable score change messages through the 'notify' command.

- liquid handling should now be quite thorough:

- taking a liquid will take the container of the liquid
  automatically. Both 'take juice' and 'take bottle' have the same
  effect.

    > take juice
    (the bottle of juice) Taken.

  In the same way, 'pour' (but not 'empty') works for both liquids and
  containers ('empty' only works for containers):

    > pour bottle on floor
    You pour the contents of the bottle onto the floor.

    > pour juice on floor
    You pour the juice on the floor.

    > empty juice on floor
    You can only empty containers.

- also it is possible make mixtures: commands like 'pour blue potion
  in red potion' are enabled

- actors have postures standing (the default), sitting and lying_down,
  with checks disabling many actions when sitting or lying_down.

- also in a dark_location, actions requiring seeing are disabled.

- distant objects cannot be manipulated (through the attribute '(NOT) reachable')

- NPCs will automatically follow the hero if they are given the
  attribute 'following'.

- verb checks are mostly listed so that logically improper actions are
  disabled first:

    CHECK obj1 <> obj2
      ELSE "You can't put something into itself."

  and after that, checks requiring seeing are listed:

    AND obj1 NOT IN obj2
      ELSE SAY THE obj1. "is in" SAY THE obj2. "already."

- some verbs accept new syntax: it is possible to say both 'look at
  object' and 'look object' (according to what e.g. the latest version
  of Inform allows, as well), and the verb 'switch' (without 'on' or
  'off') will switch an object on if it is off, and vice versa.

- plural handling is now implemented throughout the library (e.g. 'You
  see nothing special about the door(s). It is (They are) currently
  closed' etc.)

- optional classes for indoor and outdoor locations: all indoor
  locations (rooms) automatically have floor, walls and ceiling, and
  all outdoor locations have ground and sky.

- several new object classes: background, scenery, lightsource, supporter, weapon, sound, door, window...

- examples of use in between the codes, to facilitate the author's work process.

+ lots more. See for yourself and have fun!


Comments and questions, any kind of feedback is welcome, even crucial for further improvements.

Please comment on for example

1) how does the library feel in general? is it still relatively easy
to grasp and use, despite the size?  What should be changed?

2) Is the number of files good? Should the files be divided into
smaller ones?

3) Is the parameter solution good? Do you prefer obj1, obj2?

4) What else would you like to see?

5) there are examples of code usage scattered throughout the
library. This is to have the instructions close to the code. Also,
even if the game file size grows a bit because of this, it doesn't
matter as compiling is usually immediate anyway. However, if you
prefer that the library wouldn't contain so many instructions/ or that
the instructions are in a separate file, please let us know.

6) Should some default messages be changed? Are some clumsy?

7) Of course, point out any bugs.

etc. etc.



-Anssi
