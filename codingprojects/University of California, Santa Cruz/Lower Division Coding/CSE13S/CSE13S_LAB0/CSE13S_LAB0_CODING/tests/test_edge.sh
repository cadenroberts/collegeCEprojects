# Creates the example output
echo 513 > 513.txt

# Runs the program
./calc 512 1 > 512.txt 

# Ensures exit code is Zero
if [ $? -ne 0 ]; then
    echo "invalid exit code" $?
    rm 513.txt
    rm 512.txt
    exit 1
fi

# Ensures differences *are* found
diff 513.txt 512.txt
if [ $? -ne 0 ]; then
    echo "Somehow, the output of 512+1 is not 513!"
    rm 513.txt
    rm 512.txt
    exit 1
fi

# Prints a message on success
echo "Test finds that 512+1 = 513: PASS"

# Cleans up files created
rm 513.txt
rm 512.txt
exit 0
