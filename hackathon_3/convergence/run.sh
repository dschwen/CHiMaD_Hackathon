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

#
# run on coarser meshes
#

cat problem1b_gmsh.i convergence.inc > problem1b_gmsh_convergence.i
rm problem1b_convergence.csv
for scale in 0.11 0.1 0.15 0.2 0.3 0.5 0.75 1.0 1.2 1.5 2
do
  cd mesh
  gmsh -2 -order 2 -setnumber scale $scale problem1b.geo
  cd ..

  mpirun -n 4 ../../CHiMaD_Hackathon-opt -i problem1b_gmsh_convergence.i Executioner/nl_rel_tol=1e-12 Executioner/nl_abs_tol=1e-16

  grep '^1,' problem1b_gmsh_convergence_out.csv | cut -d, -f2- >> problem1b_convergence.csv
done
