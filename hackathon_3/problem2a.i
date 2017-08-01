#
#

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 160
  ny = 160
  xmax = 100
  ymax = 100
[]

[Variables]
  [./c]
  [../]
  [./w]
  [../]
  [./phi]
  [../]
[]

[AuxVariables]
  [./local_energy]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[ICs]
  [./cIC]
    type = FunctionIC
    variable = c
    function = ic_c_func
  [../]
[]

[BCs]
  [./left]
    type = DirichletBC
    variable = phi
    boundary = 'left'
    value = 0
  [../]
  [./right]
    type = FunctionDirichletBC
    variable = phi
    boundary = 'right'
    function = 'voltage_func'
  [../]
[]

[Functions]
  [./voltage_func]
    type = ParsedFunction
    value = 'sin(y/7.0)'
  [../]
  [./ic_c_func]
    type = ParsedFunction
    value = 'c0 + c1 * (cos(0.2 * x) * cos(0.11 * y) + (cos(0.13 * x) * cos(0.087 * y))^2
                + cos(0.025 * x - 0.15 * y) * cos(0.07 * x - 0.02 * y))'
    vars = 'c0   c1'
    vals = '0.5  0.04'
  [../]
[]


[Kernels]
  [./c_dot]
    type = CoupledTimeDerivative
    variable = w
    v = c
  [../]
  [./c_res] #Note that only the chemical and gradient terms are included here
    type = SplitCHParsed
    variable = c
    f_name = f_chem
    kappa_name = kappa_c
    w = w
    args = 'phi'
  [../]
  [./c_res_phi] #Here the electrostatic contribution is added to the Cahn-Hilliard equation
    type = MatReaction
    variable = c
    v = phi
    mob_name = minusk
  [../]
  [./w_res]
    type = SplitCHWRes
    variable = w
    mob_name = M
  [../]
  [./diff]
    type = Diffusion
    variable = phi
  [../]
  [./charge]
    type = MatReaction
    variable = phi
    v = c
    mob_name = kovereps
  [../]
[]

[AuxKernels]
  [./local_energy]
    type = TotalFreeEnergy
    variable = local_energy
    f_name = f_bulk
    interfacial_vars = c
    kappa_names = kappa_c
    execute_on = timestep_end
  [../]
[]


[Materials]
  [./mat]
    type = GenericConstantMaterial
    prop_names  = 'M   kappa_c  rho c_a c_b k     eps'
    prop_values = '5.0 2.0      5   0.3 0.7 0.09  90'
  [../]
  [./kovereps]
    type = DerivativeParsedMaterial
    f_name = kovereps
    material_property_names = 'k eps'
    function = 'k/eps'
  [../]
  [./minusk]
    type = DerivativeParsedMaterial
    f_name = minusk
    material_property_names = 'k'
    function = '-k'
  [../]
  [./free_energy_chem]
    type = DerivativeParsedMaterial
    f_name = f_chem
    args = c
    material_property_names = 'rho c_a c_b'
    function = 'rho*(c - c_a)^2*(c_b - c)^2'
    enable_jit = true
    #outputs = exodus
  [../]
  [./free_energy_elec]
    type = DerivativeParsedMaterial
    f_name = f_elec
    args = 'c phi'
    material_property_names = 'k'
    function = 'k*c*phi/2'
    enable_jit = true
    #outputs = exodus
  [../]
  [./free_energy_bulk]
    # add the chemical and electrostatic free energy contributions together
    type = DerivativeSumMaterial
    derivative_order = 2
    args = 'c phi'
    sum_materials = 'f_chem f_elec'
    f_name = f_bulk
  [../]
[]

[Preconditioning]
  [./cw_coupling]
    type = SMP
    full = true
  [../]
[]

[VectorPostprocessors]
  [./c_linecut]
    type = LineValueSampler
    start_point = '0 50 0'
    end_point = '100 50 0'
    variable = c
    sort_by = x
    num_points = 161
    execute_on = 'initial timestep_end'
  [../]
[]

[Postprocessors]
  [./total_free_energy]
    type = ElementIntegralVariablePostprocessor
    variable = local_energy
  [../]
  [./total_solute]
    type = ElementIntegralVariablePostprocessor
    variable = c
  [../]
  [./dt]
    type = TimestepSize
  [../]
  [./memory]
    type = MemoryUsage
    mem_type =  physical_memory
  [../]
[]

[Executioner]
  type = Transient
  solve_type = PJFNK
  scheme = bdf2

  petsc_options_iname = '-pc_type -sub_pc_type'
  petsc_options_value = 'asm      lu          '

  l_max_its = 30
  l_tol = 1e-4
  nl_max_its = 20
  nl_rel_tol = 1e-9
  nl_abs_tol = 1e-11

  [./TimeStepper]
    type = IterationAdaptiveDT
    dt = 1.0
    optimal_iterations = 8
    iteration_window = 2
  [../]

  end_time = 400
[]

[Outputs]
  exodus = true
  csv = true
  print_perf_log = true
  sync_times = '5 10 20 50 100 200 400'
[]
