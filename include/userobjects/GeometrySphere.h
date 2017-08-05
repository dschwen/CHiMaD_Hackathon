#ifndef GEOMETRYSPHERE_H
#define GEOMETRYSPHERE_H

#include "GeometryBase.h"

class GeometrySphere;

template <>
InputParameters validParams<GeometrySphere>();

/**
 *
 */
class GeometrySphere : public GeometryBase
{
public:
  GeometrySphere(const InputParameters & parameters);

protected:
  virtual void snapNode(Node & node);

  const Point _center;
  const Real _radius;
};

#endif // GEOMETRYSPHERE_H
