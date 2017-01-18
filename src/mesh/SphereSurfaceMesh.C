// #include "RankTwoTensor.h"
#include "SphereSurfaceMesh.h"

// libMesh includes
#include "libmesh/face_tri3.h"
#include "libmesh/unstructured_mesh.h"

template <>
InputParameters
validParams<SphereSurfaceMesh>()
{
  InputParameters params = validParams<MooseMesh>();
  params.addParam<Real>("radius", 1.0, "Sphere radius");
  params.addParam<unsigned int>("depth", 3, "Iteration steps in the triangle bisection construction");
  return params;
}

SphereSurfaceMesh::SphereSurfaceMesh(const InputParameters & parameters)
  : MooseMesh(parameters),
    _radius(getParam<Real>("radius")),
    _depth(getParam<unsigned int>("depth")),
    _n_points(0),
    _umesh(dynamic_cast<UnstructuredMesh &>(getMesh()))
{
  mooseAssert(_umesh, "No underlying unstructured mesh found. This is an internal MOOSE/libMesh error.");
}

MooseMesh &
SphereSurfaceMesh::clone() const
{
  return *(new SphereSurfaceMesh(*this));
}

unsigned int
SphereSurfaceMesh::midPoint(unsigned int a, unsigned int b)
{
  auto ab = _midpoint.find(std::make_pair(std::min(a, b), std::max(a, b)));
  if (ab != _midpoint.end())
    return ab->second;

  const Point p = *(_umesh.node_ptr(a)) + *(_umesh.node_ptr(b));
  const unsigned int v = _n_points;
  _midpoint.insert(std::make_pair(std::make_pair(std::min(a, b), std::max(a, b)), v));
  _umesh.add_point(p / p.norm() * _radius, _n_points++);
  return v;
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

  const unsigned int v12 = midPoint(v1, v2);
  const unsigned int v23 = midPoint(v2, v3);
  const unsigned int v31 = midPoint(v3, v1);

  //
  //         2
  //       /   \
  //     12 -- 23
  //    /  \  /   \
  //  1 --- 31 --- 3
  //

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
  unsigned int nodes = 0;
  for (unsigned int i = 0; i < _depth; ++i)
  {
    nodes += elems * 3;
    elems *= 4;
  }
  _umesh.reserve_elem(elems);
  // nodes added by subdivide are shared between neighboring parent triangles
  _umesh.reserve_nodes(nodes / 2 + 12);

  // icosahedron points
  const double X = 0.525731112119133606;
  const double Z = 0.850650808352039932;
  const Point vdata[12] = {
      {-X, 0.0, Z}, {X, 0.0, Z}, {-X, 0.0, -Z}, {X, 0.0, -Z},
      {0.0, Z, X}, {0.0, Z, -X}, {0.0, -Z, X}, {0.0, -Z, -X},
      {Z, X, 0.0}, {-Z, X, 0.0}, {Z, -X, 0.0}, {-Z, -X, 0.0}};
  for (unsigned int i = 0; i < 12; ++i)
    _umesh.add_point(vdata[i] * _radius, i);
  _n_points = 12;

  // icosahedron faces
  unsigned int tindices[20][3] = {
      {0, 4, 1}, {0, 9, 4}, {9, 5, 4}, {4, 5, 8},
      {4, 8, 1}, {8, 10, 1}, {8, 3, 10}, {5, 3, 8},
      {5, 2, 3}, {2, 7, 3}, {7, 10, 3}, {7, 6, 10},
      {7, 11, 6}, {11, 0, 6}, {0, 1, 6}, {6, 1, 10},
      {9, 0, 11}, {9, 11, 2}, {9, 2, 5}, {7, 2, 11}};
  for (unsigned int i = 0; i < 20; ++i)
    subdivide(tindices[i][0], tindices[i][1], tindices[i][2], _depth);

  _umesh.prepare_for_use(/*skip_renumber =*/false);
}
