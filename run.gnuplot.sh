#!/bin/sh
#g++ main.cpp -o main.exe ;
#./main.exe file1 file2;

#cat file3 > plot.gnp;
gnuplot plot.gnp;
ps2pdf plot.eps;

exit 0
