[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = ${L}
  ny = ${L}
  xmax = 200
  ymax = 200
[]

[GlobalParams]
  derivative_order = 2
[]

[Variables]
  [./c]
  [../]
  [./w]
  [../]
[]

[ICs]
  [./c_IC]
    type = FunctionIC
    variable = c
    function = ic_c_func
  [../]
[]

[Functions]
  [./ic_c_func]
    type = ParsedFunction
    value = 'c0 + eps * (cos(0.105 * x) * cos(0.11 * y) + (cos(0.13 * x) * cos(0.087 * y))^2
                + cos(0.025 * x - 0.15 * y) * cos(0.07 * x - 0.02 * y))'
    vars = 'c0   eps'
    vals = '0.5  0.01'
  [../]
[]

[AuxVariables]
  [./f]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./c_diff]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[Kernels]
  [./c_res]
    type = SplitCHParsed
    variable = c
    kappa_name = kappa
    f_name = fchem
    args = c
    w = w
  [../]
  [./w_res]
    type = SplitCHWRes
    variable = w
    mob_name = M
  [../]
  [./time]
    type = CoupledTimeDerivative
    variable = w
    v = c
  [../]
[]

[BCs]
  [./Periodic]
    [./all]
      auto_direction = 'x y'
    [../]
  [../]
[]

[Functions]
  [./reference]
    type = SolutionFunction
    solution = reference
    from_variable = c
  [../]
[]

[AuxKernels]
  [./free_en]
    type = TotalFreeEnergy
    variable = f
    f_name = fchem
    kappa_names = kappa
    interfacial_vars = c
    execute_on = 'initial timestep_end'
  [../]
  [./c_diff]
    type = ElementL2ErrorFunctionAux
    variable = c_diff
    function = reference
    coupled_variable = c
  [../]
[]

[Materials]
  [./constants]
    type = GenericConstantMaterial
    prop_names  = 'kappa  M'
    prop_values = '2.0    5'
  [../]
  [./fchem]
    type = DerivativeParsedMaterial
    f_name = fchem
    args = c
    constant_names       = 'qs   ca   cb'
    constant_expressions = '5.0  0.3  0.7'
    function = 'qs * (c - ca)^2 * (cb - c)^2'
  [../]
[]

[Postprocessors]
  [./F]
    type = ElementIntegralVariablePostprocessor
    variable = f
    execute_on = 'initial timestep_end'
  [../]
  [./C]
    type = ElementIntegralVariablePostprocessor
    variable = c
    execute_on = 'initial timestep_end'
  [../]
  [./dt]
    type = TimestepSize
  [../]
  [./nodes]
    type = NumNodes
  [../]
  [./active_time]
    type = RunTime
    time_type = active
  [../]
  [./err]
    type = ElementL2Error
    function = reference
    variable = c
  [../]
[]

[UserObjects]
  [./reference]
    type = SolutionUserObject
    mesh = ../problem_1a_reference.e
    system_variables = 'c'
  [../]
[]

[Preconditioning]
  [./SMP]
    type = SMP
    full = true
  [../]
[]

[Executioner]
  type = Transient
  scheme = bdf2
  solve_type = NEWTON

  #petsc_options_iname = '-pc_type -pc_factor_mat_solver_package'
  #petsc_options_value = 'lu       superlu_dist'

  #petsc_options_iname = '-pc_type  -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  #petsc_options_value = 'asm       preonly       ilu          1'

  l_max_its = 30
  l_tol = 1.0e-3

  nl_max_its = 15
  nl_rel_tol = 1e-8
  nl_abs_tol = 1e-11

  start_time = 0
  end_time = 10000

  [./TimeStepper]
    type = IterationAdaptiveDT
    dt = 1.0
    cutback_factor = .75
    growth_factor = 1.25
    optimal_iterations = 8
    iteration_window = 2
  [../]
[]

[Outputs]
  csv = true
  [./out]
    type = Exodus
    sync_times = '1 5 10 20 100 200 500 1000 2000 3000 10000'
    sync_only = true
  [../]
  [./err]
    type = CSV
    sync_times = '1 5 10 20 100 200 500 1000 2000 3000 10000'
    sync_only = true
    show = err
  [../]

  interval = 1
  print_mesh_changed_info = true
  print_perf_log = true
  print_linear_residuals = false
[]
