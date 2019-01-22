#!/bin/bash
gcc orbit.c -lm
./a.out
gnuplot orbit.gnu
