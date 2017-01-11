#!/bin/bash

for dir in problem_1b_148*
do 
  L=$(grep Elems -A1 $dir/output.out | grep Total | awk '{ print sqrt($2) }')
  err=$(grep '^10000,' $dir/problem_1b_err.csv | cut -d, -f2)
  echo $L $err $dir
  echo -n $L > $dir/L.txt
done | sort -n
