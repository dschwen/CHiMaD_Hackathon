#ifndef CUTHOLE_H
#define CUTHOLE_H

#include "ElementDeleterBase.h"

class CutHole;

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
};

#endif // CUTHOLE_H
