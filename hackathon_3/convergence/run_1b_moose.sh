#!/bin/bash

#
# prepare reference mesh
#

BASE=problem1b_moose
REF=${BASE}_reference.e
if [ ! -s $REF ]
then
  ny=160
  # 1128502 DOFs
  mpirun -n 12 ../../CHiMaD_Hackathon-opt -i $BASE.i Mesh/ny=$ny Mesh/nx=$(expr $ny \* 5)
  mv ${BASE}_out.e $REF
fi

#
# run on coarser meshes
#

cat $BASE.i convergence.inc > ${BASE}_convergence.i
#for ny in 80 60 40 30 20 15 10 8 6
for ny in 15 10 8 6
do
  mpirun -n 4 ../../CHiMaD_Hackathon-opt -i ${BASE}_convergence.i Mesh/ny=$ny Mesh/nx=$(expr $ny \* 5) UserObjects/reference/mesh=$REF
  # MeshModifiers/hole/r='1.01 1.51 1'

  grep '^1,' ${BASE}_convergence_out.csv | cut -d, -f2- >>${BASE}_convergence.csv
done
