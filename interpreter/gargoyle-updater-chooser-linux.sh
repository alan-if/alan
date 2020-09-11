#! /bin/sh
#
# This is a helper script for updating Gargoyle installations on Linux
# with the latest Alan3 interpreter.
#
# It seems that Gargoyle might be installed in two different places:
#
# /usr/games
#		and then all terps are there too (when installed from
#		https://github.com/garglk/garglk)
#
# /usr/lib/gargoyle
#       or at least this is where terps are stored, when installed as
#		gargoyle-free - using a package manager
#
# So we copy the new alan3 to either or both of them
found=0
if [ -d /usr/lib/gargoyle ] ; then
    sudo cp alan3 /usr/lib/gargoyle
    echo "Gargoyle in /usr/lib/gargoyle updated."
    found=1
fi
if [ -f /usr/games/alan3 ] ; then
    sudo cp alan3 /usr/games
    echo "Gargoyle in /usr/games updated."
    found=1
fi
if [ $found -eq 0 ] ; then
    echo "No Gargoyle found in /usr/games or /usr/lib/gargoyle."
fi
