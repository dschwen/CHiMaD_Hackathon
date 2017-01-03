[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 50
  ny = 50
  nz = 0
  xmax = 200
  ymax = 200
  zmax = 0
  elem_type = QUAD4
  uniform_refine = 2
[]

[GlobalParams]
  block = 0
  derivative_order = 2
  op_num = 10
  var_name_base = eta
[]

[Variables]
  [./c]
  [../]
  [./w]
  [../]
  [./PolycrystalVariables]
  [../]
[]

[ICs]
  [./c_IC]
    type = FunctionIC
    variable = c
    function = ic_c_func
  [../]
  [./OstRipIC]
  [../]
[]

[Functions]
  [./ic_c_func]
    # Eq (12) from http://dx.doi.org/10.1016/j.commatsci.2016.09.022
    type = ParsedFunction
    value = 'c0 + e * (cos(0.105 * x) * cos(0.11 * y)
             + (cos(0.13 * x) * cos(0.087 * y))^2 + cos(0.025 * x - 0.15 * y)
             * cos(0.07 * x - 0.02 * y))'
    vars = 'c0  e'
    vals = '0.5 0.01'
  [../]
[]

[AuxVariables]
  [./free_en]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./bnds]
  [../]
[]

[Kernels]
  [./c_res]
    type = SplitCHParsed
    variable = c
    kappa_name = kappa_c
    w = w
    f_name = F
    args = 'c eta0 eta1'
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
  [./OstRipACKernel]
    gamma = gamma
    c = c
  [../]
[]

[AuxKernels]
  [./free_en]
    type = TotalFreeEnergy
    variable = free_en
    f_name = F
    kappa_names = 'kappa_c kappa_op kappa_op'
    interfacial_vars = 'c eta0 eta1'
    execute_on = 'initial timestep_end'
  [../]
  [./bnds]
    type = BndsCalcAux
    variable = bnds
    v = 'eta0 eta1'
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
    prop_names  = 'kappa_c M   L   kappa_op'
    prop_values = '3.0     5.0 5.0 3.0'
  [../]
  [./falpha]
    type = DerivativeParsedMaterial
    f_name = falpha
    args = c
    constant_names       = 'C_alpha sigma'
    constant_expressions = '0.3 sqrt(2)'
    function = 'sigma^2 * (C_alpha - c)^2'
  [../]
  [./fbeta]
    type = DerivativeParsedMaterial
    f_name = fbeta
    args = c
    constant_names       = 'C_beta sigma'
    constant_expressions = '0.7 sqrt(2)'
    function = 'sigma^2 * (C_beta - c)^2'
  [../]
[]

[Postprocessors]
  [./Total_Free_Energy]
    type = ElementIntegralVariablePostprocessor
    variable = free_en
    execute_on = 'initial TIMESTEP_END'
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
    #off_diag_row = 'c w eta0 eta1 c c'
    #off_diag_column = 'w c c c eta0 eta1'
  [../]
[]

[Executioner]
  # Preconditioned JFNK (default)
  type = Transient
  scheme = bdf2
  solve_type = NEWTON
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm         31   preonly   ilu      1'
  l_max_its = 20
  nl_max_its = 20
  l_tol = 1.0e-3
  nl_rel_tol = 1.0e-8
  start_time = 0.0
  num_steps = 1500
  nl_abs_tol = 1e-9
  [./TimeStepper]
    type = IterationAdaptiveDT
    cutback_factor = 0.5
    dt = 0.01
    growth_factor = 1.2
    optimal_iterations = 8
  [../]
  [./Adaptivity]
    refine_fraction = 0.7
    coarsen_fraction = 0.1
    max_h_level = 2
    initial_adaptivity = 2
  [../]
[]

[Outputs]
  #output_initial = true
  exodus = true
  csv = true
  #interval = 1
  gnuplot = true
  print_mesh_changed_info = true
  print_perf_log = true
[]
