# Creates the example output
echo 8 > 7.txt

# Runs the program
./calc 3 4 > 3.4.txt 

# Ensures exit code is Zero
if [ $? -ne 0 ]; then
    echo "invalid exit code" $?
    rm 3.4.txt
    rm 7.txt
    exit 1
fi

# Ensures differences *are* found
diff 3.4.txt 7.txt
if [ $? -eq 0 ]; then
    echo "Somehow, the output of 3+4 is 8!"
    rm 3.4.txt
    rm 7.txt
    exit 1
fi

# Prints a message on success
echo "Test does not find that 3+4 = 8: PASS"

# Cleans up files created
rm 3.4.txt
rm 7.txt
exit 0
