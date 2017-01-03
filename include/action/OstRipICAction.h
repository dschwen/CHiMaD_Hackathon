#ifndef OSTRIPICACTION_H
#define OSTRIPICACTION_H

#include "Action.h"

class OstRipICAction: public Action
{
public:
  OstRipICAction(const InputParameters & params);
  virtual void act();

private:
  const unsigned int _op_num;
  const std::string _var_name_base;
};

template<>
InputParameters validParams<OstRipICAction>();

#endif //OSTRIPICACTION_H
