alan -pack ok.alan
strings ok.a3c | grep -q "refer to multiple objects with"
if [ "$?" -eq 0 ]
then
    echo "ERROR: -pack leaves readable strings in .a3c"
else
    echo "OK: -pack puts encoded text strings in .a3c"
fi
