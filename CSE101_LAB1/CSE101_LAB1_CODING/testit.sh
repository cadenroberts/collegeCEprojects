echo "Running make clean."
make clean

echo "Running make."
make

echo "Running ./bard simple-input.txt 1.txt"
./bard simple-input.txt 1.txt
diff simple-output.txt 1.txt
if [ $? -eq 0 ]; then
		echo "Passed simple."
else
		echo "Failed simple."
fi
rm 1.txt

echo "Running ./bard Tests/more-input.txt 2.txt"
./bard Tests/more-input.txt 2.txt
diff Tests/more-output.txt 2.txt
if [ $? -eq 0 ]; then
		echo "Passed more."
else
		echo "Failed more."
fi
rm 2.txt

echo "Running make clean."
make clean

echo "Making testit.sh non-executable."
chmod -x "$0"