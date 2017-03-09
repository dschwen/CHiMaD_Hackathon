#ifndef MMSBASE_H
#define MMSBASE_H

#include "MaterialPropertyInterface.h"

/**
 * Method of manufactured solutions: Split Cahn-Hilliard
 */
template <class T>
class MMSBase : public T
{
public:
  MMSBase(const InputParameters & parameters);

  static InputParameters validParams();

protected:
  Real computeC(const Point & p);
  Real computeDfDc(const Point & p);
  Real computeLaplacianC(const Point & p);

  Point computeGradC(const Point & p);
  Real computeLaplacian2C(const Point & p);
  Real computeD2fDc2(const Point & p);
  Real computeD3fDc3(const Point & p);

  const MaterialProperty<Real> & _M;
  const MaterialProperty<Real> & _kappa;
  const MaterialProperty<Real> & _omega;
  const MaterialProperty<Real> & _ca;
  const MaterialProperty<Real> & _cb;
  const MaterialProperty<Real> & _w;

  const MaterialProperty<Real> & _kx;
  const MaterialProperty<Real> & _ky;

  const Real & _t;

  const Real _A;
};

#endif // MMSBASE_H
