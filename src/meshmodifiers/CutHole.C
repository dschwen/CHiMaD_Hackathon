#include "CutHole.h"
#include "MooseMesh.h"

#include "libmesh/mesh_modification.h"
#include "libmesh/mesh_refinement.h"
#include "libmesh/sphere.h"
#include "libmesh/unstructured_mesh.h"

template <>
InputParameters
validParams<CutHole>()
{
  InputParameters params = validParams<ElementDeleterBase>();
  params.addClassDescription("Cut an ellipsoidal hole in a generated mesh");
  params.addRequiredParam<Point>("center", "center of the hole");
  params.addRequiredParam<RealVectorValue>("r", "semimajor axis of the hole");
  params.addParam<unsigned int>("refine", 0, "Refinement steps for elements cut be the surface of the hole");
  params.addParam<BoundaryName>("boundary", "Boundary name to assign to the hole surface");
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
  // delete if all nodes are fully contained in the hole
  return elem->n_nodes() == nodesContained(elem);
}

void
CutHole::modify()
{
  // Check that we have access to the mesh
  if (!_mesh_ptr)
    mooseError("_mesh_ptr must be initialized before calling CutHole::modify()");
  MeshBase & mesh = _mesh_ptr->getMesh();

  /*
  // refining the cut unfortunately does not work, because it leaves
  // unconstrained hanging nodes

  // Elements that will be refined
  MeshRefinement mesh_refinement(mesh);

  // First let's figure out which elements need to be refined
  for (unsigned int step = 0; step < _refinement_steps; ++step)
  {
    const MeshBase::const_element_iterator end = mesh.elements_end();
    for (MeshBase::const_element_iterator elem_it = mesh.elements_begin(); elem_it != end; ++elem_it)
    {
      Elem * elem = *elem_it;
      const unsigned int n = nodesContained(elem);
      if (n > 0 && n < elem->n_nodes())
        elem->set_refinement_flag(Elem::REFINE);
    }
    mesh_refinement.refine_and_coarsen_elements();
  }
  MeshTools::Modification::flatten(mesh);
*/

  // delete interior
  ElementDeleterBase::modify();

  // snap nodes to ellipsoid
  const MeshBase::element_iterator end = mesh.elements_end();
  for (MeshBase::element_iterator elem_it = mesh.elements_begin(); elem_it != end; ++elem_it)
  {
    Elem * elem = *elem_it;
    const unsigned int n = nodesContained(elem);
    if (n > 0 && n < elem->n_nodes())
      snapNodes(elem);
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

void
CutHole::snapNodes(Elem * elem)
{
  const unsigned int n = elem->n_nodes();
  for (unsigned int i = 0; i < n; ++i)
    if (nodeContained(elem->node_ptr(i)))
    {
      auto & node = elem->node_ref(i);
      const Point o = node - _center;
      const Real f = 1.0 / std::sqrt(ellipsoidDistance(o));
      node = o * f + _center;
    }
}
