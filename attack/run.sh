# code injection

# lv1
# ./hex2raw < ./solution/ct1.txt | LD_PRELOAD=./printf.so ./ctarget -q

# lv2
# ./hex2raw < ./solution/ct2.txt | LD_PRELOAD=./printf.so ./ctarget -q

# lv3
# ./hex2raw < ./solution/ct3.txt | LD_PRELOAD=./printf.so ./ctarget -q

# return program

# lv2
# ./hex2raw < ./solution/rt2.txt | ./rtarget -q

# lv3
./hex2raw < ./solution/rt3.txt | ./rtarget -q
