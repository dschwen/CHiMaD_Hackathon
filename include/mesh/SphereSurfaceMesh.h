#ifndef SPHERESURFACEMESH_H
#define SPHERESURFACEMESH_H

#include "MooseMesh.h"

class SphereSurfaceMesh;

template <>
InputParameters validParams<SphereSurfaceMesh>();

/**
 * Create a sphere surface mesh based on the recursive subdovision of the faces
 * of a regular icosahedron.
 */
class SphereSurfaceMesh : public MooseMesh
{
public:
  SphereSurfaceMesh(const InputParameters & parameters);
  SphereSurfaceMesh(const SphereSurfaceMesh & other_mesh) = default;

  // No copy
  SphereSurfaceMesh & operator=(const SphereSurfaceMesh & other_mesh) = delete;
  virtual std::unique_ptr<MooseMesh> safeClone() const override;

  virtual void buildMesh() override;

protected:
  // refine a triange into four triangles
  void subdivide(unsigned int v1, unsigned int v2, unsigned int v3, unsigned int depth);

  // identify or add the midpoint of an edge given by two node indices
  unsigned int midPoint(unsigned int a, unsigned int b);

  /// sphere radius
  const Real _radius;

  /// recursion levels for triangle subdivision
  const unsigned int _depth;

  /// number of nodes added to keep track of node indices
  unsigned int _n_points;

  /// underlying unstructured mesh to build the sphere in
  UnstructuredMesh & _umesh;

  // midpoints are shared between adjacent parent triangles, this map helps avoid adding duplicate
  // nodes
  std::map<std::pair<unsigned int, unsigned int>, unsigned int> _midpoint;
};

#endif // SPHERESURFACEMESH_H
