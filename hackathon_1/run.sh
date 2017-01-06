#!/bin/bash

INPUT=$1
DIR=${INPUT/.i/}_`date +%s`

mkdir $DIR
cd $DIR
echo $DIR

cp ../$INPUT .
cp -a ../.jitcache .

{ time mpirun -n 8 ../../CHiMaD_Hackathon-opt -i $@ | tee output.out; } 2> output.err

cd ..
