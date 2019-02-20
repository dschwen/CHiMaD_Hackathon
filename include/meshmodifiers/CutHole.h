#ifndef CUTHOLE_H
#define CUTHOLE_H

#include "ElementDeleterBase.h"
#include "libmesh/point.h"

class CutHole;
namespace libmesh
{
class Elem;
class Node;
} // namespace libmesh

template <>
InputParameters validParams<CutHole>();

/**
 * Cut an ellipsoidal hole in the mesh
 */
class CutHole : public ElementDeleterBase
{
public:
  CutHole(const InputParameters & parameters);

protected:
  virtual void modify() override;
  virtual bool shouldDelete(const Elem * elem) override;

  Real ellipsoidDistance(const Point & o);
  bool nodeContained(const Node * node);
  unsigned int nodesContained(const Elem * elem);
  void snapNodes(Elem * elem);

  const Point _center;
  RealVectorValue _r;
  const unsigned int _refinement_steps;

  std::set<const Elem *> _delete_elems;
  std::set<const Node *> _touched_nodes;
};

#endif // CUTHOLE_H
