#!/bin/bash

for dir in ${1}_14*
do 
  L=$(grep Elems -A1 $dir/output.out | grep Total | awk '{ print sqrt($2) }')
  err=$(grep '^3000,' $dir/${1}_err.csv | cut -d, -f2)
  echo $L $err $dir
  echo -n $L > $dir/L.txt
done | sort -n
