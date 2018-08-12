#! /bin/sh
#
# This is a helper script for updating Gargoyle installations on Linux
# with the latest Alan3 interpreter.
#
# It seems that Gargoyle might be installed in two different places:
#
# - /usr/games, and then all terps are there too
# - /usr/lib/gargoyle, or at least this is where terps are stored
#
# So we choose where to copy the new alan3
if [ -d /usr/lib/gargoyle ] ; then
    sudo cp alan3 /usr/lib/gargoyle
else
    sudo cp alan3 /usr/games
fi
echo "Gargoyle updated."
