#include "OstRipFreeEnergy.h"
#include "libmesh/utility.h"

template<>
InputParameters validParams<OstRipFreeEnergy>()
{
  InputParameters params = validParams<DerivativeFunctionMaterialBase>();
  params.addClassDescription("Material that implements the two phase Ostwald Ripening free energy and its derivatives");
  params.addRequiredParam<MaterialPropertyName>("calpha", "Alpha phase concentration");
  params.addRequiredParam<MaterialPropertyName>("cbeta", "Beta phase concentration");
  params.addCoupledVar("c", "Concentration variable");
  params.addRequiredCoupledVarWithAutoBuild("v", "var_name_base", "op_num", "Array of coupled non-conserved order parameters");
  params.addParam<Real>("w", 1.0, "Barrier term coefficient");
  params.addParam<Real>("alpha", 1.0, "Co-efficient for cross term barrier term");
  return params;
}

OstRipFreeEnergy::OstRipFreeEnergy(const InputParameters & parameters) :
    DerivativeFunctionMaterialBase(parameters),
    _calpha(getParam<Real>("calpha")),
    _cbeta(getParam<Real>("cbeta")),
    _c(coupledValue("c")),
    _c_var(coupled("c")),
    _nop(coupledComponents("v")),
    _vals(_nop),
    _vals_var(_nop),
    _w(getParam<Real>("w")),
    _alpha(getParam<Real>("alpha"))
{
  // Loop through ops and load coupled variables into the arrays
  for (unsigned int i = 0; i < _nop; ++i)
  {
    _vals[i] = &coupledValue("v", i);
    _vals_var[i] = coupled("v", i);
  }
}

Real OstRipFreeEnergy::switchingFunction()
{
  Real h = 0.0;
  for (unsigned int i = 0; i < _nop; ++i)
  {
    const Real n = (*_vals[i])[_qp];
    const Real n2 = n * n;
    const Real n3 = n2 * n;
    h += n3 * (6.0 * n2 - 15.0 * n + 10.0);
  }

  return h;
}

Real OstRipFreeEnergy::barrierFunction()
{
  Real g1 = 0.0;
  Real g2 = 0.0;
  for (unsigned int i = 0; i < _nop; ++i)
  {
    const Real ni = (*_vals[i])[_qp];
    g1 += Utility::pow<2>(ni * (1.0 - ni));

    for (unsigned int j = 0; j < i; ++j)
    {
      const Real nj = (*_vals[j])[_qp];
      g2 += ni * ni * nj * nj;
    }
  }

  return g1 + _alpha * 2.0 * g2;
}

Real OstRipFreeEnergy::switchingFunctionD1(unsigned int di)
{
  Real h = 0.0;
  // for (unsigned int i = 0; i < _nop; ++i)
  // {
  //   const Real n = (*_vals[i])[_qp];
  //   const Real n2 = n * n;
  //   const Real n3 = n2 * n;
  //   h += n3 * (6.0 * n2 - 15.0 * n + 10.0);
  // }

  return h;
}

Real OstRipFreeEnergy::barrierFunctionD1(unsigned int di)
{
  Real g1 = 0.0;
  Real g2 = 0.0;
  // for (unsigned int i = 0; i < _nop; ++i)
  // {
  //   const Real ni = (*_vals[i])[_qp];
  //   g1 += Utility::pow<2>(ni * (1.0 - ni));
  //
  //   for (unsigned int j = 0; j < i; ++j)
  //   {
  //     const Real nj = (*_vals[j])[_qp];
  //     g2 += ni * ni * nj * nj;
  //   }
  // }

  return g1 + _alpha * 2.0 * g2;
}



Real
OstRipFreeEnergy::computeF()
{
  const Real h = switchingFunction();
  const Real g = barrierFunction();

  return Utility::pow<2>(_c[_qp] - _calpha) * (1.0 - h) + Utility::pow<2>(_c[_qp] - _cbeta) * h + _w * g;
}

Real
OstRipFreeEnergy::computeDF(unsigned int j_var)
{
  if (j_var == _c_var)
  {
    const Real h = switchingFunction();
    const Real g = barrierFunction();

    return 2.0 * (_c[_qp] - _calpha) * (1.0 - h) + 2.0 * (_c[_qp] - _cbeta) * h + _w * g;
  }

  for (unsigned int i = 0; i < _nop; ++i)
    if (j_var == _vals_var[i])
    {
      const Real h = switchingFunctionD1(i);
      const Real g = barrierFunctionD1(i);

      return Utility::pow<2>(_c[_qp] - _calpha) * (1.0 - h) + Utility::pow<2>(_c[_qp] - _cbeta) * h + _w * g;
    }

  return 0.0;
}

Real
OstRipFreeEnergy::computeD2F(unsigned int j_var, unsigned int k_var)
{
  if ( (j_var == _c_var) && (k_var == _c_var) )
  {
    const Real g = barrierFunction();
    return 2.0 + _w * g;
  }


  // todo: implement everything below
  for (unsigned int i = 0; i < _nop; ++i)
  {
    if ( (j_var == _c_var) && (k_var == _vals_var[i]))
      return 0;

    if ((j_var == _vals_var[i]) && (k_var == _vals_var[i]))
    {
      // Real SumEtaj = 0.0;
      // for (unsigned int j = 0; j < _nop; ++j)
      //   if (j != i)
      //     SumEtaj += (*_vals[j])[_qp] * (*_vals[j])[_qp]; //Sum all other order parameters
      //
      return 0;
    }
  }

  return 0.0;
}

Real
OstRipFreeEnergy::computeD3F(unsigned int j_var, unsigned int k_var, unsigned int l_var)
{
  if ((j_var == _c_var) && (k_var == _c_var) && (l_var == _c_var))
    return 0.0;

  // todo: implement everything below
  for (unsigned int i = 0; i < _nop; ++i)
  {
    if ((j_var == _c_var) && (k_var == _c_var) && (l_var == _vals_var[i]))
      return 0;

    if ((j_var == _c_var) && (k_var == _vals_var[i]) && (l_var == _vals_var[i]))
      return 0;

    if ((j_var == _vals_var[i]) && (k_var == _vals_var[i]) && (l_var == _vals_var[i]))
      return 0;
  }
  return 0.0;
}
