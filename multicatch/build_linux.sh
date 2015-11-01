#!/bin/bash
gcc -c $1.c -o $1.o -fPIC
ld -shared -o $1.pd_linux $1.o -lc -lm

