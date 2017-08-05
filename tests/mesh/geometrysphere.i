[Mesh]
  type = GeneratedMesh
  dim = 2
  uniform_refine = 3
[]

[Variables]
  [./u]
    initial_condition = 1
  [../]
[]

[Problem]
  kernel_coverage_check = false
  solve = false
[]

[Executioner]
  type = Steady
[]

[UserObjects]
  [./sphere]
    type = GeometrySphere
    boundary = 'left right top bottom'
    center = '0.5 0.5 0'
    radius = 0.7071
  [../]
[]

#[Adaptivity]
#  [./Markers]
#    [./const]
#      type = UniformMarker
#      mark = REFINE
#    [../]
#  [../]
#  marker = const
#  steps = 5
#[]

[Outputs]
  [./out]
    type = Exodus
  [../]
[]
