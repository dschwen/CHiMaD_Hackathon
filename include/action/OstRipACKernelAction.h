#ifndef OstRipACKernelAction_H
#define OstRipACKernelAction_H

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

  VariableName _c;
  //
  // MaterialPropertyName _gamma;

};

template<>
InputParameters validParams<OstRipACKernelAction>();

#endif //POLYCRYSTALKERNELACTION_H
