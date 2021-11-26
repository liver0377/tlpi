#!/bin/bash
gcc -o svshm_xfr_reader.out svshm_xfr_reader.c ../../lib/error_functions.c ../../svsem/binary_sems.c

gcc -o svshm_xfr_writer.out svshm_xfr_writer.c ../../lib/error_functions.c ../../svsem/binary_sems.c

wc -c /etc/services

./svshm_xfr_writer.out < /etc/services & 
time ./svshm_xfr_reader.out > out.txt
