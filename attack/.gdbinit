# for ctarget's segment fault
# set environment LD_PRELOAD=./printf.so

# set args -q < ./solution/ct1_raw.txt

# set args -q < ./solution/ct2_raw.txt

# set args -q < ./solution/ct3_raw.txt 

# set args -q < ./solution/rt2_raw.txt

set args -q < ./solution/rt3_raw.txt

b getbuf
