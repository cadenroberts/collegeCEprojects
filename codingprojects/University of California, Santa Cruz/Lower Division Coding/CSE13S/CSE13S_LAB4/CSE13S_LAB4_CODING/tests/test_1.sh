make all

./calc < input.txt > out.txt
./calc_arm < input.txt > out2.txt

diff out.txt out2.txt
if [ $? -eq 0 ]; then
        rm out.txt out2.txt
        make clean
        exit 0
fi

rm out.txt out2.txt
make clean
exit 1

