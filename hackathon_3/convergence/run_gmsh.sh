#!/bin/bash

#
# prepare reference mesh
#

BASE=problem1b_gmsh
REF=${BASE}_reference.e
if [ ! -s $REF ]
then
  cd mesh
  gmsh -2 -setnumber scale 0.1 problem1b.geo
  cd ..

  # 846730 DOFs
  mpirun -n 12 ../../CHiMaD_Hackathon-opt -i $BASE.i
  mv ${BASE}_out.e $REF
fi

#
# run on coarser meshes
#

cat $BASE.i convergence.inc > ${BASE}_convergence.i
rm ${BASE}_convergence.csv
for scale in 0.11 0.15 0.2 0.3 0.5 0.75 1.0 1.2 1.5 2 3 4 5 8 10 14
do
  cd mesh
  gmsh -2 -order 2 -setnumber scale $scale problem1b.geo
  cd ..

  mpirun -n 4 ../../CHiMaD_Hackathon-opt -i ${BASE}_convergence.i UserObjects/reference/mesh=$REF

  OUT=outputs/${BASE}_$scale
  mkdir -p $OUT
  mv ${BASE}_convergence_out* $OUT


  grep '^1,' ${BASE}_convergence_out.csv | cut -d, -f2- >>${BASE}_convergence.csv
done
