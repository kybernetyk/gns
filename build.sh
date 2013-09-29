#!/bin/sh
# this is better than make and friends!

clang -c termbox/src/*.c
clang++ -o gns -std=c++11 *.o *.cpp
rm *.o
