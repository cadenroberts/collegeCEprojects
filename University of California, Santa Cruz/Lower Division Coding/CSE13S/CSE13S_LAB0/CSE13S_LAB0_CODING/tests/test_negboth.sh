echo -7 > neg7.txt

./calc -3 -4 > neg34.txt

if [ $? -ne 0 ]; then
    echo "invalid exit code" $?
    rm neg34.txt
    rm neg7.txt
    exit 1
fi

# Ensures differences arent found
diff neg34.txt neg7.txt
if [ $? -ne 0 ]; then
    echo "Somehow, the output of -3-4 isnt -7!"
    rm neg34.txt
    rm neg7.txt
    exit 1
fi

# Prints a message on success
echo "Test finds that -3-4 = -7: PASS"

# Cleans up files created
rm neg34.txt
rm neg7.txt
exit 0
