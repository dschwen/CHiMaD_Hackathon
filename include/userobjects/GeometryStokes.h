#ifndef GEOMETRYSTOKES_H
#define GEOMETRYSTOKES_H

#include "GeometryBase.h"

class GeometryStokes;

template <>
InputParameters validParams<GeometryStokes>();

/**
 *
 */
class GeometryStokes : public GeometryBase
{
public:
  GeometryStokes(const InputParameters & parameters);

protected:
  virtual void snapNode(Node & node);
};

#endif // GEOMETRYSTOKES_H
