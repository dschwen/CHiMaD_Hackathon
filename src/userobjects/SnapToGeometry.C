#include "SnapToGeometry.h"
#include "MooseMesh.h"
#include "libmesh/mesh_base.h"

template <>
InputParameters
validParams<SnapToGeometry>()
{
  InputParameters params = validParams<GeneralUserObject>();
  params.addClassDescription("Snap refined nodes on a given boundary to a given geometry");
  params.addParam<std::vector<BoundaryName>>(
      "boundary", "List of boundaries whose nodes are snapped to a given geometry");
  return params;
}

SnapToGeometry::SnapToGeometry(const InputParameters & parameters)
  : GeneralUserObject(parameters),
    _mesh(_subproblem.mesh()),
    _boundary_ids(_mesh.getBoundaryIDs(getParam<std::vector<BoundaryName>>("boundary")))
{
}

void
SnapToGeometry::initialize()
{
}

void
SnapToGeometry::execute()
{
}

void
SnapToGeometry::finalize()
{
}

void
SnapToGeometry::meshChanged()
{
  auto & mesh = _mesh.getMesh();

  for (auto & boundary_id : _boundary_ids)
  {
    auto node_ids = _mesh.getNodeList(boundary_id);
    for (auto & node_id : node_ids)
    {
      auto & node = mesh.node_ref(node_id);

      const Point center(7, 2.5, 0);
      const Point r(1.5, 1, 1);

      const Point o = node - center;
      const Real R =
          o(0) * o(0) / (r(0) * r(0)) + o(1) * o(1) / (r(1) * r(1)) + o(2) * o(2) / (r(2) * r(2));
      node = o * 1.0 / R + center;
    }
  }
}
