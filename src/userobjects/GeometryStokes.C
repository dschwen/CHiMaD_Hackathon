#include "GeometryStokes.h"

registerMooseObject("ChimadHackathonApp", GeometryStokes);

template <>
InputParameters
validParams<GeometryStokes>()
{
  InputParameters params = validParams<GeometryBase>();
  return params;
}

GeometryStokes::GeometryStokes(const InputParameters & parameters) : GeometryBase(parameters) {}

void
GeometryStokes::snapNode(Node & node)
{
  const Point center(7, 2.5, 0);
  const Point r(1, 1.5, 1);

  const Point o = node - center;
  const Real R =
      o(0) * o(0) / (r(0) * r(0)) + o(1) * o(1) / (r(1) * r(1)) + o(2) * o(2) / (r(2) * r(2));

  node = o * 1.0 / std::sqrt(R) + center;
}
