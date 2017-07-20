
#
# compare to a reference solution
#

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

[Functions]
  [./p_reference]
    type = SolutionFunction
    solution = reference
    from_variable = p
  [../]
  [./vel_x_reference]
    type = SolutionFunction
    solution = reference
    from_variable = vel_x
  [../]
  [./vel_y_reference]
    type = SolutionFunction
    solution = reference
    from_variable = vel_y
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
    function = vel_x_reference
    coupled_variable = vel_x
  [../]
  [./vel_y_diff]
    type = ElementL2ErrorFunctionAux
    variable = vel_y_diff
    function = vel_y_reference
    coupled_variable = vel_y
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
    function = vel_x_reference
    variable = vel_x
  [../]
  [./vel_y_err]
    type = ElementL2Error
    function = vel_y_reference
    variable = vel_y
  [../]
[]

[UserObjects]
  [./reference]
    type = SolutionUserObject
    system_variables = 'p vel_x vel_y'
  [../]
[]

[Debug]
  show_var_residual_norms = true
[]
