echo 1 > 1.txt

./calc -3 4 > neg34.txt

if [ $? -ne 0 ]; then
    echo "invalid exit code" $?
    rm neg34.txt
    rm 1.txt
    exit 1
fi

# Ensures differences arent found
diff neg34.txt 1.txt
if [ $? -ne 0 ]; then
    echo "Somehow, the output of -3+4 is not 1!"
    rm neg34.txt
    rm 1.txt
    exit 1
fi

# Prints a message on success
echo "Test finds that -3+4 = 1: PASS"

rm neg34.txt
rm 1.txt

exit 0
