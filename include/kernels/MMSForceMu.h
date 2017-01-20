#ifndef MMSFORCEMU_H
#define MMSFORCEMU_H

#include "Kernel.h"
#include "MMSBase.h"

class MMSForceMu;

template<>
InputParameters validParams<MMSForceMu>();

/**
 * Manufactured Solution forcing term
 */
class MMSForceMu : public MMSBase<Kernel>
{
public:
  MMSForceMu(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;
};

#endif // MMSFORCEMU_H
