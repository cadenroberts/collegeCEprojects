echo TOO BIG > bi.txt

./calc -600 1 > n.txt

diff n.txt bi.txt
if [ $? -eq 0 ]; then
    rm n.txt
    rm bi.txt
    exit 0
fi

rm -511.txt
rm bi.txt
exit 1
