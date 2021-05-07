clear
alan -charset utf long
rm long.a3t long.a3s
echo "----- arun ----"
arun -r -u -l -c long < input.txt
echo "----- logs ----"
cat long.a3t
cat long.a3s
rm long.txt
echo "------ glkarun -------"
glkarun -r -u -l long < input.txt
echo "------   logs  -------"
cat long.txt
