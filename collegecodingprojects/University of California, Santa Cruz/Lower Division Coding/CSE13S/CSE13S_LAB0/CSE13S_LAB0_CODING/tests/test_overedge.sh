# Creates the example output
echo TOO BIG > 514.txt

# Runs the program
./calc 513 1 > 513.txt 

# Ensures exit code is Zero
if [ $? -ne 0 ]; then
    echo "invalid exit code" $?
    rm 514.txt
    rm 513.txt
    exit 0
fi

# Ensures differences *are* found
diff 513.txt 514.txt
if [ $? -ne 0 ]; then
    echo "Somehow, the output of 513+513 is not TOO BIG!"
    rm 513.txt
    rm 514.txt
    exit 1
fi

# Prints a message on success
echo "Test finds that 513+1 = TOO BIG: PASS"

# Cleans up files created
rm 513.txt
rm 514.txt
exit 0
