[Mesh]
  type = SphereSurfaceMesh
  radius = 2
  depth = 3
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

[Outputs]
  [./out]
    type = Exodus
    use_problem_dimension = false
  [../]
[]
