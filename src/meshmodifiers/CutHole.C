#include "CutHole.h"
#include "MooseMesh.h"

#include "libmesh/mesh_modification.h"
#include "libmesh/mesh_refinement.h"
#include "libmesh/elem.h"
#include "libmesh/node.h"
#include "libmesh/sphere.h"
#include "libmesh/unstructured_mesh.h"

registerMooseObject("ChimadHackathonApp", CutHole);

template <>
InputParameters
validParams<CutHole>()
{
  InputParameters params = validParams<ElementDeleterBase>();
  params.addClassDescription("Cut an ellipsoidal hole in a generated mesh");
  params.addRequiredParam<Point>("center", "center of the hole");
  params.addRequiredParam<RealVectorValue>("r", "semimajor axis of the hole");
  params.addParam<unsigned int>(
      "refine", 0, "Refinement steps for elements cut be the surface of the hole");
  return params;
}

CutHole::CutHole(const InputParameters & parameters)
  : ElementDeleterBase(parameters),
    _center(getParam<Point>("center")),
    _r(getParam<RealVectorValue>("r")),
    _refinement_steps(getParam<unsigned int>("refine"))
{
}

bool
CutHole::shouldDelete(const Elem * elem)
{
  // delete if at least one node is contained
  return _delete_elems.count(elem) > 0;
}

void
CutHole::modify()
{
  // Check that we have access to the mesh
  if (!_mesh_ptr)
    mooseError("_mesh_ptr must be initialized before calling CutHole::modify()");
  MeshBase & mesh = _mesh_ptr->getMesh();

  _delete_elems.clear();
  _touched_nodes.clear();

  // mark elements and nodes to delete
  {
    const MeshBase::element_iterator end = mesh.elements_end();
    for (MeshBase::element_iterator elem_it = mesh.elements_begin(); elem_it != end; ++elem_it)
    {
      Elem * elem = *elem_it;
      const unsigned int n = nodesContained(elem);
      if (n > 0)
      {
        const unsigned int n = elem->n_nodes();
        for (unsigned int i = 0; i < n; ++i)
          _touched_nodes.insert(elem->node_ptr(i));

        _delete_elems.insert(elem);
      }
    }
  }

  // delete interior
  ElementDeleterBase::modify();

  // snap nodes to ellipsoid
  const MeshBase::node_iterator end = mesh.active_nodes_end();
  for (MeshBase::node_iterator node_it = mesh.active_nodes_begin(); node_it != end; ++node_it)
  {
    // get node reference and DOF id
    Node * node = *node_it;
    if (_touched_nodes.count(node))
    {
      const Point o = *node - _center;
      const Real d2 = ellipsoidDistance(o);
      const Real f = 1.0 / std::sqrt(d2);
      *node = o * f + _center;
    }
  }
}

Real
CutHole::ellipsoidDistance(const Point & o)
{
  const Real R = o(0) * o(0) / (_r(0) * _r(0))
#if (LIBMESH_DIM > 1)
                 + o(1) * o(1) / (_r(1) * _r(1))
#endif
#if (LIBMESH_DIM > 2)
                 + o(2) * o(2) / (_r(2) * _r(2))
#endif
      ;
  return R;
}

bool
CutHole::nodeContained(const Node * node)
{
  return ellipsoidDistance(*node - _center) <= 1.0;
}

unsigned int
CutHole::nodesContained(const Elem * elem)
{
  const unsigned int n = elem->n_nodes();
  unsigned int contained = 0;
  for (unsigned int i = 0; i < n; ++i)
    if (nodeContained(elem->node_ptr(i)))
      contained++;

  return contained;
}
