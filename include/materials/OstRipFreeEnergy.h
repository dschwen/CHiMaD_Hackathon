#ifndef OSTRIPFREEENERGYRGY_H
#define OSTRIPFREEENERGYRGY_H

#include "DerivativeFunctionMaterialBase.h"

//Forward Declarations
class OstRipFreeEnergy;

template<>
InputParameters validParams<OstRipFreeEnergy>();

/**
 * Material class that creates the math free energy and its derivatives
 * for use with CHParsed and SplitCHParsed. F = 1/4*(1 + c)^2*(1 - c)^2.
 */
class OstRipFreeEnergy : public DerivativeFunctionMaterialBase
{
public:
  OstRipFreeEnergy(const InputParameters & parameters);

protected:
  virtual Real computeF();
  virtual Real computeDF(unsigned int j_var);
  virtual Real computeD2F(unsigned int j_var, unsigned int k_var);
  virtual Real computeD3F(unsigned int j_var, unsigned int k_var, unsigned int l_var);

  Real switchingFunction();
  Real barrierFunction();

private:
  ///@{ Phase concentrations
  Real _calpha;
  Real _cbeta;
  ///@}

  ///@{ Coupled variable value for the concentration \f$ \c \f$.
  const VariableValue & _c;
  unsigned int _c_var;
  ///@}

  ///@{ Coupled non-conserved order parameters
  const unsigned int _nop;
  std::vector<const VariableValue *> _vals;
  std::vector<unsigned int> _vals_var;
  ///@}

  /// barrier coefficient
  Real _w;

  /// cross-term coefficient
  Real _alpha;
};

#endif //MATHFREEENERGY_H
