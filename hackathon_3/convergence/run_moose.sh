#!/bin/bash

#
# prepare reference mesh
#

BASE=problem1b_moose
REF=${BASE}_reference.e
if [ ! -s $REF ]
then
  ny=320
  # 1128502 DOFs
  mpirun -n 12 ../../CHiMaD_Hackathon-opt -i $BASE.i Mesh/ny=$ny Mesh/nx=$(expr $ny \* 5)
  mv ${BASE}_out.e $REF
fi

#
# run on coarser meshes
#

cat $BASE.i convergence.inc > ${BASE}_convergence.i
rm ${BASE}_convergence.csv
for ny in 160 120 80 60 40 20 10 8 
do
  mpirun -n 4 ../../CHiMaD_Hackathon-opt -i ${BASE}_convergence.i Mesh/ny=$ny Mesh/nx=$(expr $ny \* 5) UserObjects/reference/mesh=$REF

  OUT=outputs/${BASE}_$ny
  mkdir -p $OUT
  mv ${BASE}_convergence_out* $OUT
  
  grep '^1,' ${BASE}_convergence_out.csv | cut -d, -f2- >>${BASE}_convergence.csv
done
