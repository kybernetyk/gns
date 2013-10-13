#!/bin/sh
# this is better than make and friends!

clang -c termbox/src/*.c && clang++ -I/usr/include/c++/v1/ -std=c++11 -stdlib=libc++ -lpthread -lc++ -o gns *.cpp *.o && rm *.o
