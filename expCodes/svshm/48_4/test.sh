#!/bin/bash
gcc -o 48_4.out 48_4.c ~/tlpi/expCodes/lib/error_functions.c 

../svshm_create -p 102400

./48_4.out 0 
