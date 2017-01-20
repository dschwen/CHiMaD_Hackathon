#ifndef MMSFORCEC_H
#define MMSFORCEC_H

#include "Kernel.h"
#include "MMSBase.h"

class MMSForceC;

template<>
InputParameters validParams<MMSForceC>();

/**
 * Manufactured Solution forcing term
 */
class MMSForceC : public MMSBase<Kernel>
{
public:
  MMSForceC(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;
};

#endif // MMSFORCEC_H
