#include "ACBulk.h"

#ifndef OstRipACKernel_H
#define OstRipACKernel_H

//Forward Declarations
class OstRipACKernel;

template<>
InputParameters validParams<OstRipACKernel>();

/**
 * This kernel calculates the residual for grain growth during sintering.
 * It calculates the residual of the ith order parameter, and the values of
 * all other order parameters are coupled variables and are stored in vals.
 */
class OstRipACKernel : public ACBulk<Real>
{
public:
  OstRipACKernel(const InputParameters & parameters);

protected:
  virtual Real computeDFDOP(PFFunctionType type);
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

private:
  std::vector<const VariableValue *> _vals;
  std::vector<unsigned int> _vals_var;

  const VariableValue & _c;
  unsigned int _c_var;
  const MaterialProperty<Real> & _gamma;
  Real _beta;
  Real _conc_alpha;
  Real _epsilon;

  unsigned int _ncrys;

};

#endif //OstRipACKernel_H
