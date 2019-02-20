#include "ImproveElements.h"
#include "MooseMesh.h"

registerMooseObject("ChimadHackathonApp", ImproveElements);

template <>
InputParameters
validParams<ImproveElements>()
{
  InputParameters params = validParams<MeshModifier>();
  params.addClassDescription("Improves the interior node distribution in a mesh to avoid elements "
                             "with pathological aspect ratios etc.");
  params.addParam<unsigned int>(
      "iterations", 1, "Iterate this many times over all nodes to balance positions");
  params.addParam<Real>(
      "relaxation_factor",
      1.0,
      "How far should the node be moved toward the center of mass of its neighboring nodes ");
  return params;
}

ImproveElements::ImproveElements(const InputParameters & parameters)
  : MeshModifier(parameters),
    _iterations(getParam<unsigned int>("iterations")),
    _relaxation_factor(getParam<Real>("relaxation_factor"))
{
}

void
ImproveElements::modify()
{
  // Check that we have access to the MOOSE mesh
  if (!_mesh_ptr)
    mooseError("_mesh_ptr must be initialized before calling CutHole::modify()");

  // fetch node to element map
  const auto & node_to_elem = _mesh_ptr->nodeToElemMap();

  // get the libMesh mesh
  MeshBase & mesh = _mesh_ptr->getMesh();

  // connected nodes set
  std::set<const Node *> connected;

  // node connectivity
  const std::map<ElemType, std::vector<std::vector<unsigned int>>> connections = {
      {QUAD4, {{1, 2, 3}, {0, 2, 3}, {0, 1, 3}, {0, 1, 2}}},
      {QUAD9,
       {{7, 4, 8},
        {4, 5, 8},
        {5, 6, 8},
        {6, 7, 8},
        {0, 1, 5, 7, 8},
        {1, 2, 4, 6, 8},
        {2, 3, 5, 7, 8},
        {0, 3, 4, 6, 8},
        {0, 1, 2, 3, 4, 5, 6, 7}}},
      {TRI3, {{1, 2}, {0, 2}, {0, 1}}},
      {TRI6, {{3, 5}, {3, 4}, {4, 5}, {0, 1, 4, 5}, {1, 2, 3, 5}, {0, 2, 3, 4}}}};

  // main iterations
  for (unsigned step = 0; step < _iterations; ++step)
  {
    // iterate over all nodes in the mesh
    const MeshBase::node_iterator end = mesh.active_nodes_end();
    for (MeshBase::node_iterator node_it = mesh.active_nodes_begin(); node_it != end; ++node_it)
    {
      // get node reference and DOF id
      Node & node = **node_it;
      auto node_id = node.id();

      // find adjacent elements
      auto adjacent_elem_it = node_to_elem.find(node_id);
      if (adjacent_elem_it == node_to_elem.end())
        mooseError("No adjacent element found for node ", node_id);

      // clear connected nodes
      connected.clear();

      // loop over adjacent elements
      bool is_interior_node = true;
      for (const auto & elem_id : adjacent_elem_it->second)
      {
        // quit if this node has been found to be exterior on a prior adjacent element
        if (!is_interior_node)
          break;

        // get element and local node id
        const Elem & elem = mesh.elem_ref(elem_id);
        auto ln = elem.local_node(node_id);

        // loop over all sides of the element
        const unsigned int n_sides = elem.n_sides();
        for (unsigned int s = 0; s != n_sides; ++s)
          if (!elem.neighbor_ptr(s) && elem.is_node_on_side(ln, s))
          {
            is_interior_node = false;
            break;
          }

        // add connected nodes, this is element specific!
        auto conn_it = connections.find(elem.type());
        if (conn_it == connections.end())
          mooseError("Unsupported element type");
        for (auto cln : conn_it->second[ln])
          connected.insert(elem.node_ptr(cln));
      }

      // move node if it is interior
      if (is_interior_node)
      {
        Point average;
        unsigned int nc = 0;
        for (auto cnode : connected)
        {
          average += *cnode;
          nc++;
        }
        node = _relaxation_factor * (average * (1.0 / Real(nc)) - node) + node;
      }
    }
  }
}
