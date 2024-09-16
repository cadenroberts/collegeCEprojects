make all

bash delayinput.sh keyboard.txt | ./xd > out.txt
bash delayinput.sh keyboard.txt | xxd > out2.txt

diff out.txt out2.txt
if [ $? -eq 0 ]; then
        rm out.txt out2.txt
        make clean
        exit 0
fi

rm out.txt out2.txt
make clean
exit 1
