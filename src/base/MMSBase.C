#include "MMSBase.h"

template <class T>
InputParameters
MMSBase<T>::validParams()
{
  InputParameters params = ::validParams<T>();
  params.addRequiredParam<Real>("M", "Mobility");
  params.addRequiredParam<Real>("kappa", "");
  params.addRequiredParam<Real>("omega", "");
  params.addRequiredParam<Real>("ca", "");
  params.addRequiredParam<Real>("cb", "");
  params.addRequiredParam<Real>("w", "");
  params.addRequiredParam<Real>("kx", "");
  params.addRequiredParam<Real>("ky", "");
  return params;
}

template <class T>
MMSBase<T>::MMSBase(const InputParameters & parameters) :
    T(parameters),
    _M(this->template getParam<Real>("M")),
    _kappa(this->template getParam<Real>("kappa")),
    _ca(this->template getParam<Real>("ca")),
    _cb(this->template getParam<Real>("cb")),
    _w(this->template getParam<Real>("w")),
    _kx(this->template getParam<Real>("kx")),
    _ky(this->template getParam<Real>("ky")),
    _t(this->_fe_problem.time()),
    _A(1.0)
{
}

template <class T>
Real
MMSBase<T>::computeD2fDc2(const Point & p)
{
 Real c = computeC(p);
 return - 2.0 * _w * ((_cb - c) * (_ca + _cb - 2.0 * c) +
                      (_ca - c) * (_ca + _cb - 2.0 * c) +
                      2.0 * (_ca - c) * (_cb - c));
}

template <class T>
Real
MMSBase<T>::computeD3fDc3(const Point & p)
{
 Real c = computeC(p);
 return  2.0 * _w * ((_ca + _cb - 2.0 * c) + 2.0 * (_cb - c) +
                     (_ca + _cb - 2.0 * c) + 2.0 * (_ca - c) +
                     2.0 * (_cb - c) + 2.0 * (_ca - c));
}

template <class T>
Real
MMSBase<T>::computeLaplacian2C(const Point & p)
{
  const Real x = p(0);
  const Real y = p(1);

  Real r = _kx * x * x + _ky * y * y;
  Real s = _omega * _t;

  Real C = std::sin(r + s);
  Real S = std::cos(r + s);

  Real cxxxx = -12.0 * _kx * _kx * S - 48.0 * _kx*_kx*_kx * x*x * C + 16 * _kx * _kx * _kx * _kx * x*x*x*x * S;
  Real cxxyy = -4.0 * _kx * _ky * S - 8.0 * _kx * _kx * x*x * _ky * C - 8.0 * _kx * _ky * _ky * y*y * C +16*_kx * _kx * x*x * _ky * _ky * y*y * S;
  Real cyyyy = -12.0 * _ky * _ky * S - 48.0  *_ky * _ky * _ky  * y*y * C + 16 * _kx * _kx * _kx * _kx * x*x*x*x * S;

  return _A * (cxxxx + 2.0 * cxxyy + cyyyy);
}

template <class T>
Point
MMSBase<T>::computeGradC(const Point & p)
{
  const Real x = p(0);
  const Real y = p(1);

  Real r = _kx * x * x + _ky * y * y;
  Real s = _omega * _t;

  Real C = std::cos(r + s);

  Real cx = 2.0 * _kx * x * C;
  Real cy = 2.0 * _ky * y * C;

  return _A * RealGradient(cx, cy);
}


template <class T>
Real
MMSBase<T>::computeC(const Point & p)
{
  const Real x = p(0);
  const Real y = p(1);

  return _A * (std::sin(_kx * x * x + _ky * y * y + _omega * _t));
}

template <class T>
Real
MMSBase<T>::computeLaplacianC(const Point & p)
{
  const Real x = p(0);
  const Real y = p(1);

  Real r = _kx * y*y + _ky * y*y;
  Real s = _omega * _t;

  Real cxx = 2.0 * _kx * std::cos(r + s) - 4.0 * _kx * _kx * x * x * std::sin(r + s);
  Real cyy = 2.0 * _ky * std::cos(r + s) - 4.0 * _ky * _ky * y * y * std::sin(r + s);

  return _A * (cxx + cyy);
}

template <class T>
Real
MMSBase<T>::computeDfDc(const Point & p)
{
  const Real c = computeC(p);
  return - _w * 2.0 * (c - _ca) * (_cb - c) * (_ca + _cb - 2.0 * c);
}
