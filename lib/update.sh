#/bin/sh
# Script to update version marking in all library files to the current
# which is found in the alanlib.version file
# Requires the venum program (Thomas' "proprietary" version number manager)
VERSION=`venum alanlib -print "\\$v.\\$r.\\$c"`
for f in *.i
do
  echo $f
  cp $f $f.tmp
  sed -e "/Library version/s/[0-9].*/$VERSION/g" $f.tmp > $f
  rm -f $f.tmp
done
