#!/bin/bash

make clean

make

rm -f result

for i in {1..10000}
do
  echo $i
  ./sudoku < ../puzzles/sudoku22/$i > temp
done