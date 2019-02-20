// #include "RankTwoTensor.h"
#include "SphereSurfaceMesh.h"

// libMesh includes
#include "libmesh/face_tri3.h"
#include "libmesh/mesh_modification.h"
#include "libmesh/mesh_refinement.h"
#include "libmesh/sphere.h"
#include "libmesh/unstructured_mesh.h"

registerMooseObject("ChimadHackathonApp", SphereSurfaceMesh);

template <>
InputParameters
validParams<SphereSurfaceMesh>()
{
  InputParameters params = validParams<MooseMesh>();
  params.addParam<Real>("radius", 1.0, "Sphere radius");
  params.addParam<unsigned int>(
      "depth", 3, "Iteration steps in the triangle bisection construction");
  return params;
}

SphereSurfaceMesh::SphereSurfaceMesh(const InputParameters & parameters)
  : MooseMesh(parameters),
    _radius(getParam<Real>("radius")),
    _depth(getParam<unsigned int>("depth")),
    _n_points(0),
    _umesh(dynamic_cast<UnstructuredMesh &>(getMesh()))
{
}

std::unique_ptr<MooseMesh>
SphereSurfaceMesh::safeClone() const
{
  return libmesh_make_unique<SphereSurfaceMesh>(*this);
}

void
SphereSurfaceMesh::buildMesh()
{
  // set up mesh
  _umesh.clear();
  _umesh.set_mesh_dimension(2);
  _umesh.set_spatial_dimension(3);

  const Point cent;
  const Sphere sphere(cent, _radius);

  // icosahedron points (using golden ratio rectangle construction)
  const Real phi = (1.0 + std::sqrt(5.0)) / 2.0;
  const Real X = std::sqrt(1.0 / (phi * phi + 1.0));
  const Real Z = X * phi;
  const Point vdata[12] = {{-X, 0.0, Z},
                           {X, 0.0, Z},
                           {-X, 0.0, -Z},
                           {X, 0.0, -Z},
                           {0.0, Z, X},
                           {0.0, Z, -X},
                           {0.0, -Z, X},
                           {0.0, -Z, -X},
                           {Z, X, 0.0},
                           {-Z, X, 0.0},
                           {Z, -X, 0.0},
                           {-Z, -X, 0.0}};
  for (unsigned int i = 0; i < 12; ++i)
    _umesh.add_point(vdata[i] * _radius, i);

  // icosahedron faces
  const unsigned int tindices[20][3] = {{0, 4, 1},  {0, 9, 4},  {9, 5, 4},  {4, 5, 8},  {4, 8, 1},
                                        {8, 10, 1}, {8, 3, 10}, {5, 3, 8},  {5, 2, 3},  {2, 7, 3},
                                        {7, 10, 3}, {7, 6, 10}, {7, 11, 6}, {11, 0, 6}, {0, 1, 6},
                                        {6, 1, 10}, {9, 0, 11}, {9, 11, 2}, {9, 2, 5},  {7, 2, 11}};
  for (unsigned int i = 0; i < 20; ++i)
  {
    Elem * elem = _umesh.add_elem(new Tri3);
    elem->set_node(0) = _umesh.node_ptr(tindices[i][0]);
    elem->set_node(1) = _umesh.node_ptr(tindices[i][1]);
    elem->set_node(2) = _umesh.node_ptr(tindices[i][2]);
  }

  // Now we have the beginnings of a sphere.
  // Add some more elements by doing uniform refinements and
  // popping nodes to the boundary.
  MeshRefinement mesh_refinement(_umesh);

  // Loop over the elements, refine, pop nodes to boundary.
  for (unsigned int r = 0; r < _depth; ++r)
  {
    mesh_refinement.uniformly_refine(1);

    auto it = _umesh.active_nodes_begin();
    const auto end = _umesh.active_nodes_end();

    for (; it != end; ++it)
    {
      Node & node = **it;
      node = sphere.closest_point(node);
    }
  }

  MeshTools::Modification::flatten(_umesh);

  _umesh.prepare_for_use(/*skip_renumber =*/false);
}
