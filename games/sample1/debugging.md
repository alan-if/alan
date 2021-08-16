# Sample1 debugging

Ok, Anssis problem was that running the sample1.rec (recorded
commands) would not take the white key with the final command.

I have found out that this is caused by the Check on line 325 which
seems to cause the possible() check to fail since that builds a list
of all checks and tries them to remove all impossible choices.

However this should not occur since it is completely possible to take
the white key as a separate command.

# 2021-08-16

Ok, whatever the text above tried to say, with beta8 (almost anyway)
the problem is that "take all" after all keys are dropped tries to
take the wooden bowl which fails, and then the player command is
aborted:

```
> w
The Middle Room
You are in a featureless room expect for a small, round table in the middle of the room. A red door leads north, a green door leads south, a blue
door leads east and a white door leads west.

> drop all
(red key) Dropped.

(green key) Dropped.

(blue key) Dropped.

(white key) Dropped.

> take all
(red key) Taken.

(green key) Taken.

(blue key) Taken.

(wooden bowl) That wouldn't accomplish anything.

> l
The Middle Room
You are in a featureless room expect for a small, round table in the middle of the room. A red door leads north, a green door leads south, a blue
door leads east and a white door leads west. There is a white key here.

> i
You are carrying a blue key, a green key and a red key.

> take white key
Taken.

> take wooden bowl
That wouldn't accomplish anything.

>
```

I don't know if this is a problem that we can solve. A failed check
means something went wrong and in a long list of commands we do want
to abort at that point.

But perhaps "all" is different, maybe it should continue with all the
items.
