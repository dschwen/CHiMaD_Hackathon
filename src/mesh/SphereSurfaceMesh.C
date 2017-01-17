#include "RankTwoTensor.h"
#include "SphereSurfaceMesh.h"

// libMesh includes
#include "libmesh/face_tri3.h"
#include "libmesh/face_tri6.h"
#include "libmesh/string_to_enum.h"
#include "libmesh/unstructured_mesh.h"

template <>
InputParameters
validParams<SphereSurfaceMesh>()
{
  InputParameters params = validParams<MooseMesh>();
  params.addParam<Real>("radius", 1.0, "Sphere radius");
  params.addParam<unsigned int>("depth", 3, "Iteration steps in the triangle bisection construction");
  // params.addParam<unsigned int>("relax", 20, "Relaxation cycles to equilibrate triangle sizes");
  return params;
}

SphereSurfaceMesh::SphereSurfaceMesh(const InputParameters & parameters)
  : MooseMesh(parameters),
    _radius(getParam<Real>("radius")),
    _n_points(0),
    _depth(getParam<unsigned int>("depth")),
    _umesh(dynamic_cast<UnstructuredMesh &>(getMesh()))
{
}

MooseMesh &
SphereSurfaceMesh::clone() const
{
  return *(new SphereSurfaceMesh(*this));
}

void
SphereSurfaceMesh::subdivide(unsigned int v1, unsigned int v2, unsigned int v3, unsigned int depth)
{
  if (depth == 0)
  {
    Elem * elem = _umesh.add_elem(new Tri3);
    elem->set_node(0) = _umesh.node_ptr(v1);
    elem->set_node(1) = _umesh.node_ptr(v2);
    elem->set_node(2) = _umesh.node_ptr(v3);
    return;
  }

  const Point p12 = *(_umesh.node_ptr(v1)) + *(_umesh.node_ptr(v2));
  const unsigned int v12 = _n_points + 1;
  _umesh.add_point(p12 / p12.norm() * _radius, v12);

  const Point p23 = *(_umesh.node_ptr(v2)) + *(_umesh.node_ptr(v3));
  const unsigned int v23 = _n_points + 2;
  _umesh.add_point(p23 / p23.norm() * _radius, v23);

  const Point p31 = *(_umesh.node_ptr(v3)) + *(_umesh.node_ptr(v1));
  const unsigned int v31 = _n_points + 3;
  _umesh.add_point(p31 / p31.norm() * _radius, v31);

  _n_points += 3;

  subdivide(v1, v12, v31, depth - 1);
  subdivide(v2, v23, v12, depth - 1);
  subdivide(v3, v31, v23, depth - 1);
  subdivide(v12, v23, v31, depth - 1);
}

void
SphereSurfaceMesh::buildMesh()
{
  // set up mesh
  _umesh.clear();
  _umesh.set_mesh_dimension(2);
  _umesh.set_spatial_dimension(3);

  // reserve space for objects in the mesh
  unsigned int elems = 20;
  unsigned int nodes = 12;
  for (unsigned int i = 0; i < _depth; ++i)
  {
    nodes += elems * 3;
    elems *= 4;
  }
  _umesh.reserve_elem(elems);
  _umesh.reserve_nodes(nodes);

  // icosahedron points
  const double X = 0.525731112119133606;
  const double Z = 0.850650808352039932;
  const Point vdata[12] = {
      {-X, 0.0, Z}, {X, 0.0, Z}, {-X, 0.0, -Z}, {X, 0.0, -Z}, {0.0, Z, X}, {0.0, Z, -X}, {0.0, -Z, X}, {0.0, -Z, -X}, {Z, X, 0.0}, {-Z, X, 0.0}, {Z, -X, 0.0}, {-Z, -X, 0.0}};
  for (unsigned int i = 0; i < 12; ++i)
    _umesh.add_point(vdata[i] * _radius, i);
  _n_points = 12;

  // icosahedron faces
  unsigned int tindices[20][3] = {
      {0, 4, 1}, {0, 9, 4}, {9, 5, 4}, {4, 5, 8}, {4, 8, 1}, {8, 10, 1}, {8, 3, 10}, {5, 3, 8}, {5, 2, 3}, {2, 7, 3}, {7, 10, 3}, {7, 6, 10}, {7, 11, 6}, {11, 0, 6}, {0, 1, 6}, {6, 1, 10}, {9, 0, 11}, {9, 11, 2}, {9, 2, 5}, {7, 2, 11}};
  for (unsigned int i = 0; i < 20; ++i)
    subdivide(tindices[i][0], tindices[i][1], tindices[i][2], _depth);

  _umesh.prepare_for_use(/*skip_renumber =*/false);
}
