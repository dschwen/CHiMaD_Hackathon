#ifndef SNAPTOGEOMETRY_H
#define SNAPTOGEOMETRY_H

#include "GeneralUserObject.h"

class SnapToGeometry;

template <>
InputParameters validParams<SnapToGeometry>();

/**
 *
 */
class SnapToGeometry : public GeneralUserObject
{
public:
  SnapToGeometry(const InputParameters & parameters);

  virtual void initialize() override;
  virtual void execute() override;
  virtual void finalize() override;

  virtual void meshChanged() override;

protected:
  MooseMesh & _mesh;
  const std::vector<BoundaryID> _boundary_ids;
};

#endif // SNAPTOGEOMETRY_H
