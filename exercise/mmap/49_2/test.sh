#!/bin/bash
./mmap_xfr_writer.out  < /etc/services &
./mmap_xfr_reader.out  > out.txt
