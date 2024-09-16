# Creates the example output
echo BAD INPUT > bi.txt

# Runs the program
./calc 2 b > ab.txt 

# Ensures exit code is Zero
if [ $? -eq 0 ]; then
    echo "improper exit code" $?
    rm ab.txt
    rm bi.txt
    exit 1
fi

# Ensures differences arent found
diff ab.txt bi.txt
if [ $? -ne 0 ]; then
    echo "Somehow, the output of a+b is not BAD INPUT!"
    rm ab.txt
    rm bi.txt
    exit 1
fi

# Prints a message on success
echo "Test does not find that a+b = any real number: PASS"

# Cleans up files created
rm ab.txt
rm bi.txt
exit 0
