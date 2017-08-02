#ifndef GEOMETRYBASE_H
#define GEOMETRYBASE_H

#include "GeneralUserObject.h"

class GeometryBase;

template <>
InputParameters validParams<GeometryBase>();

/**
 *
 */
class GeometryBase : public GeneralUserObject
{
public:
  GeometryBase(const InputParameters & parameters);

  virtual void initialize() final;
  virtual void execute() final;
  virtual void finalize() final;

  virtual void meshChanged() final;

protected:
  virtual void snapNode(Node & node) = 0;

  MooseMesh & _mesh;
  const std::vector<BoundaryID> _boundary_ids;
};

#endif // GEOMETRYBASE_H
