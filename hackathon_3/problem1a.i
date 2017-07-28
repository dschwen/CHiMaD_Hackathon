[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 60
  ny = 12
  xmin = 0
  xmax = 30
  ymin = 0
  ymax = 6
  elem_type = QUAD9
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

[AuxVariables]
  [./p_diff]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./vel_x_diff]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./vel_y_diff]
    order = CONSTANT
    family = MONOMIAL
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

[AuxKernels]
  [./p_diff]
    type = ElementL2ErrorFunctionAux
    variable = p_diff
    function = p_reference
    coupled_variable = p
  [../]
  [./vel_x_diff]
    type = ElementL2ErrorFunctionAux
    variable = vel_x_diff
    function = inlet_func
    coupled_variable = vel_x
  [../]
  [./vel_y_diff]
    type = ElementL2ErrorFunctionAux
    variable = vel_y_diff
    function = vel_y_reference
    coupled_variable = vel_y
  [../]
[]

[BCs]
  [./x_no_slip]
    type = DirichletBC
    variable = vel_x
    boundary = 'top bottom'
    value = 0.0
  [../]
  [./y_no_slip]
    type = DirichletBC
    variable = vel_y
    boundary = 'left top bottom'
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
    prop_names  = 'rho mu'
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
  [./x7]
    type = LineValueSampler
    variable = 'p vel_x vel_y'
    start_point = '7 0 0'
    end_point = '7 6 0'
    num_points = '100'
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

[Postprocessors]
  [./nodes]
    type = NumNodes
  [../]
  [./active_time]
    type = PerformanceData
    event = ACTIVE
  [../]
  [./p_err]
    type = ElementL2Error
    function = p_reference
    variable = p
  [../]
  [./vel_x_err]
    type = ElementL2Error
    function = inlet_func
    variable = vel_x
  [../]
  [./vel_y_err]
    type = ElementL2Error
    function = vel_y_reference
    variable = vel_y
  [../]
[]


[Executioner]
  type = Steady
  petsc_options_iname = '-ksp_gmres_restart -pc_type -sub_pc_type -sub_pc_factor_levels'
  petsc_options_value = '300                bjacobi  ilu          4'
  line_search = none
  nl_rel_tol = 1e-12
  nl_max_its = 6
  l_tol = 1e-6
  l_max_its = 300
[]

[Outputs]
  exodus = true
  csv = true
  execute_on = TIMESTEP_END
[]

[Functions]
  [./p_reference]
    type = ParsedFunction
    value = '((1.0-x/30.0)*0.06+(1-y/6.0)*0.6)'
  [../]
  [./vel_y_reference]
    type = ParsedFunction
    value = '0'
  [../]

  [./inlet_func] # also vel_x_reference
    type = ParsedFunction
    value = '-0.001 * (y-3)^2 + 0.009'
  [../]
[]
