-- help.i
-- Library version 0.1, 0.2

SYNONYMS
    h = help.

SYNTAX
    help = help.

VERB help
    DOES
	"In this game I am your eyes and your body. I will describe
    	the surroundings for you. You tell me what you want to do. For
    	example if you want to go north, then type 'north'. But there
    	are a lot of other things you may do, like picking up things,
    	examining them, opening doors and so on. Try anything you
    	would do if you really were standing here!$pRemember to study
    	the descriptions very carefully, they are sure to contain
    	clues!  $p'score' may be used to see how well you are
    	doing. 'save' and 'restore' allows you to save a game and
    	restore it later.  $pGood Luck!! You'll need it!" 
END VERB.


SYNONYMS
    hints = hint.

SYNTAX
    hint = hint.

VERB hint
    DOES
        "Unfortunately hints are not available in this game."
END VERB.


SYNONYMS
    info, notes, author = credits.

SYNTAX
    credits = credits.

VERB credits
    DOES
        "The author retains the copyright to this game.
        $pThis game was written using the ALAN Adventure Language. 
        ALAN is an interative fiction authoring system by Thomas Nilsson
        $nemail address: thoni@softlab.lejonet.se 
        $pFurther information about the ALAN system can be obtained from
        the World Wide Web Internet site
        $ihttp://www.pp.softlab.se/thomas.nilsson/alan"
END VERB.



