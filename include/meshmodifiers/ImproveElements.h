#ifndef ImproveElements_H
#define ImproveElements_H

#include "MeshModifier.h"

class ImproveElements;

template <>
InputParameters validParams<ImproveElements>();

/**
 *
 */
class ImproveElements : public MeshModifier
{
public:
  ImproveElements(const InputParameters & parameters);

  virtual void modify() override;

protected:
  const unsigned int _iterations;
  const Real _relaxation_factor;
};

#endif // ImproveElements_H
