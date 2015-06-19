#!/bin/bash

mkdir -p sudoku3_norvig

for i in {0..99}
do

  cat sudoku3/$i | tr ' ' '\0' | tr '\n' '\0' | tr '0' '.'
  echo

done


