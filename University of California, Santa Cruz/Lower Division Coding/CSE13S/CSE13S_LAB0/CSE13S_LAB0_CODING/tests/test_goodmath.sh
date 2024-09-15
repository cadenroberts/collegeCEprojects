echo 8 > 8.txt

./calc 4 4 > 4.txt

if [ $? -ne 0 ]; then
    echo "invalid exit code" $?
    rm 4.txt
    rm 8.txt
    exit 1
fi

# Ensures differences *are* found
diff 4.txt 8.txt
if [ $? -ne 0 ]; then
    echo "Somehow, the output of 4+4 is not 8!"
    rm 4.txt
    rm 8.txt
    exit 1
fi

# Prints a message on success
echo "Test finds that 4+4 = 8: PASS"

rm 4.txt
rm 8.txt
exit 0
