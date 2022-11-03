#!/bin/bash
gcc -o svshm_xfr_writer_vms.out svshm_xfr_writer.c ~/tlpi/expCodes/lib/error_functions.c ../../svsem/47_5.c ../../svsem/binary_sems.c
gcc -o svshm_xfr_reader_vms.out svshm_xfr_reader.c ~/tlpi/expCodes/lib/error_functions.c ../../svsem/47_5.c ../../svsem/binary_sems.c
wc -c /etc/services
./svshm_xfr_writer_vms.out < /etc/services &
./svshm_xfr_reader_vms.out > out.txt
