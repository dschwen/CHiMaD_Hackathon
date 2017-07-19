#!/bin/bash

#
# prepare reference mesh
#

if [ ! -e problem1b_reference.e ]
then
  cd mesh
  gmsh -2 -setnumber scale 0.1 problem1b.geo
  cd ..

  mpirun -n 12 ../../CHiMaD_Hackathon-opt -i problem1b_gmsh.i
  mv problem1b_gmsh_out.e problem1b_reference.e
fi
