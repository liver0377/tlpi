#!/bin/bash
gcc -c -fPIC p1.c p2.c
gcc -shared -o libp1.so.1 p1.o
gcc -shared -o libp2.so.1 p2.o
gcc -o prog.out main.c ../../lib/error_functions.c libp2.so.1 libp1.so.1 -ldl
LD_LIBRARY_PATH=. ./prog.out

rm *.o 
rm lib*.so.1


