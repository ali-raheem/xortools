#!/bin/sh

gcc -O2 -o bin/xorkey src/xorkey.c
gcc -O2 -o bin/xorfile src/xorfile.c
gcc -O2 -o bin/xorcrack src/xorcrack.c
gcc -O2 -fopenmp -o bin/xorioc src/xorioc.c
gcc -O2 -o bin/xor src/xor.c
