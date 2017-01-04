[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 20
  ny = 20
  xmax = 200
  ymax = 200
  uniform_refine = 1
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

[AuxKernels]
  [./free_en]
    type = TotalFreeEnergy
    variable = f
    f_name = fchem
    kappa_names = kappa
    interfacial_vars = c
    execute_on = 'initial timestep_end'
  [../]
[]

[BCs]
  [./Periodic]
    [./all]
      auto_direction = 'x y'
    [../]
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

  # use 8 mpi processes
  petsc_options_iname = '-pc_type  -sub_ksp_type -sub_pc_type '
  petsc_options_value = 'asm       preonly       ilu'

  l_max_its = 30
  l_tol = 1.0e-3

  nl_max_its = 10
  nl_rel_tol = 1e-9
  nl_abs_tol = 1e-8

  start_time = 0
  end_time = 10000

  [./TimeStepper]
    type = IterationAdaptiveDT
    cutback_factor = .75
    dt = 1.0
    growth_factor = 1.2
    optimal_iterations = 8
    iteration_window = 2
  [../]
  [./Adaptivity]
    coarsen_fraction = 0.2
    refine_fraction = 0.85
    max_h_level = 2
    initial_adaptivity = 1
    interval = 2
  [../]
[]

[Outputs]
  csv = true
  [./out]
    type = Exodus
    sync_times = '1 5 10 20 100 200 500 1000 2000 3000 10000'
  [../]

  interval = 1
  print_mesh_changed_info = true
  print_perf_log = true
  print_linear_residuals = false
[]
