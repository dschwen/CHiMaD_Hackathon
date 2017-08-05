#include "GeometrySphere.h"

template <>
InputParameters
validParams<GeometrySphere>()
{
  InputParameters params = validParams<GeometryBase>();
  params.addParam<Point>("center", "Sphere center");
  params.addParam<Real>("radius", "Sphere radius");
  return params;
}

GeometrySphere::GeometrySphere(const InputParameters & parameters)
  : GeometryBase(parameters), _center(getParam<Point>("center")), _radius(getParam<Real>("radius"))
{
}

void
GeometrySphere::snapNode(Node & node)
{
  const Point o = node - _center;
  const Real r = o.norm();
  node = o * _radius / r + _center;
}
