#[Mesh]
#  type = GeneratedMesh
#  dim = 2
#  nx = 45
#  ny = 9
#  xmin = 0
#  xmax = 30
#  ymin = 0
#  ymax = 6
#  elem_type = TRI6
#[]
[Mesh]
  type = FileMesh
  file = mesh/problem1b.msh
[]

[GlobalParams]
  gravity = '0 -0.001 0'
  convective_term = false
  integrate_p_by_parts = false
[]

[MeshModifiers]
  [./corner_node]
    type = AddExtraNodeset
    new_boundary = top_right
    coord = '30 6'
  [../]
  #[./hole]
  #  type = CutHole
  #  center = '7 2.5 0'
  #  r = '1 1.5 1'
  #  new_boundary = obstruction
  #[../]
  #[./improve]
  #  type = ImproveElements
  #  iterations = 1
  #  depends_on = hole
  #[../]
[]

[Variables]
  [./vel_x]
    order = SECOND
    family = LAGRANGE
  [../]
  [./vel_y]
    order = SECOND
    family = LAGRANGE
  [../]
  [./p]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[Kernels]
  [./mass]
    type = INSMass
    variable = p
    u = vel_x
    v = vel_y
  [../]
  [./x_momentum_space]
    type = INSMomentumLaplaceForm
    variable = vel_x
    u = vel_x
    v = vel_y
    p = p
    component = 0
  [../]
  [./y_momentum_space]
    type = INSMomentumLaplaceForm
    variable = vel_y
    u = vel_x
    v = vel_y
    p = p
    component = 1
  [../]
[]

[BCs]
  [./x_no_slip]
    type = DirichletBC
    variable = vel_x
    boundary = 'obstruction top bottom'
    value = 0.0
  [../]
  [./y_no_slip]
    type = DirichletBC
    variable = vel_y
    boundary = 'obstruction left top bottom'
    value = 0.0
  [../]
  [./x_inlet]
    type = FunctionDirichletBC
    variable = vel_x
    boundary = 'left'
    function = 'inlet_func'
  [../]
  [./p_corner]
    # Since the pressure is not integrated by parts in this example,
    # it is only specified up to a constant by the natural outflow BC.
    # Therefore, we need to pin its value at a single location.
    type = DirichletBC
    boundary = top_right
    value = 0
    variable = p
  [../]
[]

[Materials]
  [./const]
    type = GenericConstantMaterial
    prop_names = 'rho mu'
    prop_values = '100  1'
  [../]
[]

[Preconditioning]
  [./SMP_PJFNK]
    type = SMP
    full = true
    solve_type = NEWTON
  [../]
[]

[VectorPostprocessors]
  [./x7a]
    type = LineValueSampler
    variable = 'p vel_x vel_y'
    start_point = '7 0 0'
    end_point = '7 1 0'
    num_points = '20'
    sort_by = y
  [../]
  [./x7b]
    type = LineValueSampler
    variable = 'p vel_x vel_y'
    start_point = '7 4 0'
    end_point = '7 6 0'
    num_points = '40'
    sort_by = y
  [../]
  [./y5]
    type = LineValueSampler
    variable = 'p vel_x vel_y'
    start_point = '0 5 0'
    end_point = '30 5 0'
    num_points = '500'
    sort_by = x
  [../]
[]

[Adaptivity]
  [./Markers]
    [./marker]
      type = UniformMarker
      mark = REFINE
    [../]
  [../]
  steps = 6
  marker = marker
[]

[Executioner]
  type = Steady
  petsc_options_iname = '-ksp_gmres_restart -pc_type -sub_pc_type -sub_pc_factor_levels'
  petsc_options_value = '300                bjacobi  ilu          4'
  line_search = none
  nl_rel_tol = 1e-12
  nl_abs_tol = 1e-12
  nl_max_its = 15
  l_tol = 1e-6
  l_max_its = 1000
[]

[Outputs]
  exodus = true
  csv = true
  execute_on = TIMESTEP_END
[]

[UserObjects]
  [./snap]
    type = GeometryStokes
    boundary = obstruction
  [../]
[]

[Functions]
  [./inlet_func]
    type = ParsedFunction
    value = '-0.001 * (y-3)^2 + 0.009'
  [../]
[]
