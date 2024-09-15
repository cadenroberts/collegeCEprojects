echo NOT ENOUGH INPUT > bi.txt

./calc 1 > 1.txt

if [ $? -eq 0 ]; then
    echo "improper exit code" $?
    rm 1.txt
    rm bi.txt
    exit 1
fi

# Ensures differences arent found
diff 1.txt bi.txt
if [ $? -ne 0 ]; then
    echo "Somehow, the output of a single input is not NOT ENOUGH INPUT!"
    rm 1.txt
    rm bi.txt
    exit 1
fi

# Prints a message on success
echo "Test finds that 1 input = NOT ENOUGH INPUT: PASS"

rm 1.txt
rm bi.txt
exit 0
