#include "OstRipICAction.h"
#include "Factory.h"
#include "Conversion.h"
#include "FEProblem.h"

template<>
InputParameters validParams<OstRipICAction>()
{
  InputParameters params = validParams<Action>();
  params.addRequiredParam<unsigned int>("op_num", "specifies the number of grains to create");
  params.addRequiredParam<std::string>("var_name_base", "specifies the base name of the variables");
  return params;
}

OstRipICAction::OstRipICAction(const InputParameters & params) :
    Action(params),
    _op_num(getParam<unsigned int>("op_num")),
    _var_name_base(getParam<std::string>("var_name_base"))
{
}

void
OstRipICAction::act()
{
  for (unsigned int op = 0; op < _op_num; ++op)
  {
    // Create variable names
    std::string var_name = _var_name_base + Moose::stringify(op);

    InputParameters params = _factory.getValidParams("OstRipPseudoRand");
    params.set<NonlinearVariableName>("variable") = var_name;
    params.set<unsigned int>("i") = op;
    std::string ic_name = "ostripic_" + var_name;
    _problem->addInitialCondition("OstRipPseudoRand", ic_name, params);
  }
}
