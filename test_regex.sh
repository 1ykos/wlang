#!/bin/zsh
# ccache
g++ -std=c++23 -Wfatal-errors -O3 -march=native regex.cpp -o regex || exit
echo -ne ' x ( y* ) z\ny\na(bcd)e' | ./regex # expect success
echo -ne 'x y* x z*\nz\nlalala' | ./regex # expect success
echo -ne 'x y x y*\nz\nlalala' | ./regex # expect fail
