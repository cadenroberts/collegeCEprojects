./hangman "don't go in empty-handed" <tester.txt > out.txt
diff out.txt lose.txt
if [ $?-ne 0 ]; then
	rm out.txt
	exit 1
fi

rm out.txt
exit 0
