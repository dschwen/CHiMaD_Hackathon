/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#include "OstRipFreeEnergy.h"

template<>
InputParameters validParams<OstRipFreeEnergy>()
{
  InputParameters params = validParams<DerivativeFunctionMaterialBase>();
  params.addClassDescription("Material that implements the Osttwald Ripening free energy and its derivatives");
  params.addRequiredCoupledVar("c", "Concentration variable");
  params.addCoupledVar("v", "Vector of all the coupled order parameters");
  params.addParam<MaterialPropertyName>("gamma", 1.0, "Material Proprty/co-effieient for free energy term");
  params.addParam<Real>("beta", 1.0, "Co-efficient for free energy term");
  params.addParam<Real>("conc_alpha", 0.05, "Equilibrium concentration for phase alpha");
  params.addParam<Real>("epsilon", 3.0, "Co-effient for free energy term");
  return params;
}

OstRipFreeEnergy::OstRipFreeEnergy(const InputParameters & parameters) :
    DerivativeFunctionMaterialBase(parameters),
    _c(coupledValue("c")),
    _c_var(coupled("c")),
    _gamma(getMaterialProperty<Real>("gamma")),
    _beta(getParam<Real>("beta")),
    _conc_alpha(getParam<Real>("conc_alpha")),
    _epsilon(getParam<Real>("conc_alpha")),
    _ncrys(coupledComponents("v"))
{
  // Array of coupled variables is created in the constructor
  _vals.resize(_ncrys); //Size variable arrays
  _vals_var.resize(_ncrys);

  //Loop through grains and load coupled variables into the arrays
  for (unsigned int i = 0; i < _ncrys; ++i)
  {
    _vals[i] = &coupledValue("v", i);
    _vals_var[i] = coupled("v", i);
  }
}

Real
OstRipFreeEnergy::computeF()
{
  Real SumEtai2 = 0.0;
  Real SumEtai4 = 0.0;
  Real SumEtai2j2 = 0.0;

  for (unsigned int i = 0; i < _ncrys; ++i)
  {
    SumEtai2 += (*_vals[i])[_qp] * (*_vals[i])[_qp]; //Sum all order parameters
    SumEtai4 += (*_vals[i])[_qp] * (*_vals[i])[_qp] * (*_vals[i])[_qp] * (*_vals[i])[_qp];;
    for (unsigned int j = 0; j < _ncrys; ++j)
    {
      if (j != i)
        SumEtai2j2 += (*_vals[i])[_qp] * (*_vals[i])[_qp]
                    * (*_vals[j])[_qp] * (*_vals[j])[_qp]; //Sum all other order parameters
    }
  }
  return - _gamma[_qp] / 2.0 * (_c[_qp] - _conc_alpha) * (_c[_qp] - _conc_alpha) * SumEtai2
         + _beta / 4.0 * SumEtai4
         + _epsilon / 2.0 * SumEtai2j2;
}

Real
OstRipFreeEnergy::computeDF(unsigned int j_var)
{
  if (j_var == _c_var)
  {
    // Sum all order parameters
    Real SumEtai2 = 0.0;
    for (unsigned int i = 0; i < _ncrys; ++i)
      SumEtai2 += (*_vals[i])[_qp] * (*_vals[i])[_qp];

    return -_gamma[_qp] * (_c[_qp] - _conc_alpha) * SumEtai2;
  }

  for (unsigned int i = 0; i < _ncrys; ++i)
  {
    if (j_var == _vals_var[i])
    {
      Real SumEtaj = 0.0;
      for (unsigned int j = 0; j < _ncrys; ++j)
      {
        if (j != i)
          SumEtaj += (*_vals[j])[_qp] * (*_vals[j])[_qp]; //Sum all other order parameters
      }
      return   -_gamma[_qp] * (_c[_qp] - _conc_alpha) * (_c[_qp] - _conc_alpha) * (*_vals[i])[_qp]
             + _beta * (*_vals[i])[_qp] * (*_vals[i])[_qp] * (*_vals[i])[_qp]
             + _epsilon * (*_vals[i])[_qp] * SumEtaj ;
    }
  }

  return 0.0;
}

Real
OstRipFreeEnergy::computeD2F(unsigned int j_var, unsigned int k_var)
{
  if ( (j_var == _c_var) && (k_var == _c_var) )
  {
    Real SumEtai = 0.0;
    for (unsigned int i = 0; i < _ncrys; ++i)
      SumEtai += (*_vals[i])[_qp] * (*_vals[i])[_qp]; //Sum all other order parameters

    return - _gamma[_qp] * SumEtai;
  }

  for (unsigned int i = 0; i < _ncrys; ++i)
  {
    if ( (j_var == _c_var) && (k_var == _vals_var[i]))
      return - 2.0 * _gamma[_qp] * (_c[_qp] - _conc_alpha) * (*_vals[i])[_qp];

    if ((j_var == _vals_var[i]) && (k_var == _vals_var[i]))
    {
      Real SumEtaj = 0.0;
      for (unsigned int j = 0; j < _ncrys; ++j)
        if (j != i)
          SumEtaj += (*_vals[j])[_qp] * (*_vals[j])[_qp]; //Sum all other order parameters

      return -_gamma[_qp] * (_c[_qp] - _conc_alpha) * (_c[_qp] - _conc_alpha)
             + 3.0 * _beta * (*_vals[i])[_qp] * (*_vals[i])[_qp] * (*_vals[i])[_qp]
             + _epsilon * SumEtaj;
    }
  }

  return 0.0;
}

Real
OstRipFreeEnergy::computeD3F(unsigned int j_var, unsigned int k_var, unsigned int l_var)
{
  if ((j_var == _c_var) && (k_var == _c_var) && (l_var == _c_var))
    return 0.0;

  for (unsigned int i = 0; i < _ncrys; ++i)
  {
    if ((j_var == _c_var) && (k_var == _c_var) && (l_var == _vals_var[i]))
      return - 2.0 * _gamma[_qp] * (*_vals[i])[_qp] ;

    if ((j_var == _c_var) && (k_var == _vals_var[i]) && (l_var == _vals_var[i]))
      return - 2.0 * _gamma[_qp] * (_c[_qp] - _conc_alpha);

    if ((j_var == _vals_var[i]) && (k_var == _vals_var[i]) && (l_var == _vals_var[i]))
      return _beta * (*_vals[i])[_qp];
  }
  return 0.0;
}
