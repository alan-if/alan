#!/bin/sh
SCRIPT_DIR=$(cd $(dirname "${BASH_SOURCE[0]}") && pwd)
ARUN_DIR=$SCRIPT_DIR/../Resources
echo $ARUN_DIR
cat > $TMPDIR/arun.command <<EOF
#!/bin/sh
$ARUN_DIR/arun $1
if [ \$? -ne 0 ]
then
    echo
    echo "<< Press enter to close this window >>"
    read
fi
EOF
chmod +x $TMPDIR/arun.command
open $TMPDIR/arun.command
#rm $TMPDIR/arun.command
