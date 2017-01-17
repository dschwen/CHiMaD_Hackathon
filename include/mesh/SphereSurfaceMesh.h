#ifndef SPHERESURFACEMESH_H
#define SPHERESURFACEMESH_H

#include "MooseMesh.h"
#include <array>

class SphereSurfaceMesh;

template <>
InputParameters validParams<SphereSurfaceMesh>();

/**
 *
 */
class SphereSurfaceMesh : public MooseMesh
{
public:
  SphereSurfaceMesh(const InputParameters & parameters);
  SphereSurfaceMesh(const SphereSurfaceMesh & other_mesh) = default;

  // No copy
  SphereSurfaceMesh & operator=(const SphereSurfaceMesh & other_mesh) = delete;

  virtual MooseMesh & clone() const override;

  virtual void buildMesh() override;

protected:
  void subdivide(unsigned int v1, unsigned int v2, unsigned int v3, unsigned int depth);

  /// sphere radius
  const Real _radius;

  /// number of nodes added
  unsigned int _n_points;

  /// iteration depth
  const unsigned int _depth;

  /// underlying unstructured mesh
  UnstructuredMesh & _umesh;
};

#endif // SPHERESURFACEMESH_H
