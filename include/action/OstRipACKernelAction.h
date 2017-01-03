#ifndef OSTRIPACKERNELACTION_H
#define OSTRIPACKERNELACTION_H

#include "Action.h"

class OstRipACKernelAction: public Action
{
public:
  OstRipACKernelAction(const InputParameters & params);
  virtual void act();

private:
  unsigned int _op_num;
  std::string _var_name_base;
  MaterialPropertyName _f_name;

  /// concentration variable
  VariableName _c;
};

template<>
InputParameters validParams<OstRipACKernelAction>();

#endif //OSTRIPACKERNELACTION_H
