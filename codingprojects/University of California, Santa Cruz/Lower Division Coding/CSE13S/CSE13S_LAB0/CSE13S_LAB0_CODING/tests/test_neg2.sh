echo 1 > 1.txt

./calc 3 -2 > neg32.txt

if [ $? -ne 0 ]; then
    echo "invalid exit code" $?
    rm neg32.txt
    rm 1.txt
    exit 1
fi

# Ensures differences *are* found
diff neg32.txt 1.txt
if [ $? -ne 0 ]; then
    echo "Somehow, the output of 3-2 is not 1!"
    rm neg32.txt
    rm 2.txt
    exit 1
fi

# Prints a message on success
echo "Test finds that 3-2 = 1: PASS"

rm neg32.txt
rm 1.txt
exit 0
