#/bin/sh
# Script to update version marking in all library files to the current
# which is found in the alanlib.version file
# Only updates those that have changed since last CVS TAG (=release)
# Requires the venum program (Thomas' "proprietary" version number manager)
# and CVS access
#
VERSION=`venum alanlib -print "\\$v.\\$r.\\$c"`
TAG=`cvs -f status -v alanlib.version| grep \(revision | head -1|awk 'BEGIN{FS=" "};{print $1}'`
for f in `cvs diff -r $TAG *.i | grep Index | awk '{print $2}'`
do
  echo $f
  cp $f $f.tmp
  sed -e "/Library version/s/[0-9].*/$VERSION/g" $f.tmp > $f
#  sed -e "/Library version/s/[0-9].*/$VERSION/g" $f.tmp > /dev/null
  rm -f $f.tmp
done
