#include "OstRipACKernelAction.h"
#include "Factory.h"
#include "Parser.h"
#include "FEProblem.h"

template<>
InputParameters validParams<OstRipACKernelAction>()
{
  InputParameters params = validParams<Action>();
  params.addRequiredParam<unsigned int>("op_num", "specifies the number of grains to create");
  params.addRequiredParam<std::string>("var_name_base", "specifies the base name of the variables");
  params.addParam<VariableName>("c", "NONE", "Name of coupled concentration variable");
  // params.addParam<MaterialPropertyName>("gamma", 1.0, "Material Proprty/co-effieient for free energy term");
  params.addParam<MaterialPropertyName>("f_name", "F", "Base name of the free energy function F defined in a DerivativeParsedMaterial");
  params.addParam<std::vector<VariableName> >("args", "Vector of nonlinear variable arguments this object depends ons");
  params.addParam<bool>("use_displaced_mesh", false, "Whether to use displaced mesh in the kernels");
  return params;
}

OstRipACKernelAction::OstRipACKernelAction(const InputParameters & params) :
    Action(params),
    _op_num(getParam<unsigned int>("op_num")),
    _var_name_base(getParam<std::string>("var_name_base")),
    _f_name(getParam<MaterialPropertyName>("f_name")),
    _c(getParam<VariableName>("c"))
{
}

void
OstRipACKernelAction::act()
{
#ifdef DEBUG
  Moose::err << "Inside the OstRipACKernelAction Object\n";
  Moose::err << "var name base:" << _var_name_base;
#endif

  for (unsigned int op = 0; op < _op_num; ++op)
  {
    //Create variable names
    std::string var_name = _var_name_base;
    std::stringstream out;
    out << op;
    var_name.append(out.str());

    std::vector<VariableName> v;
    v.resize(_op_num);

    for (unsigned int j = 0; j < _op_num; ++j)
    {
      if (j != op)
      {
        std::stringstream out2;
        out2 << _var_name_base << j;
        v[j] = out2.str();
      }
      else
        v[j] = _c;
    }

    // InputParameters poly_params = _factory.getValidParams("OstRipACKernel");
    InputParameters poly_params = _factory.getValidParams("ACParsed");
    poly_params.set<NonlinearVariableName>("variable") = var_name;
    poly_params.set<MaterialPropertyName>("f_name") = _f_name;
    poly_params.set<std::vector<VariableName> >("args") = v;
    poly_params.set<bool>("use_displaced_mesh") = getParam<bool>("use_displaced_mesh");

    std::string kernel_name = "ACBulk_" + var_name;

    _problem->addKernel("ACParsed", kernel_name, poly_params);

    //********************************************

    poly_params = _factory.getValidParams("ACInterface");
    poly_params.set<NonlinearVariableName>("variable") = var_name;
    poly_params.set<bool>("use_displaced_mesh") = getParam<bool>("use_displaced_mesh");

    kernel_name = "ACInt_" + var_name;

    _problem->addKernel("ACInterface", kernel_name, poly_params);

    //*****************************************

    poly_params = _factory.getValidParams("TimeDerivative");
    poly_params.set<NonlinearVariableName>("variable") = var_name;
    poly_params.set<bool>("use_displaced_mesh") = getParam<bool>("use_displaced_mesh");

    kernel_name = "IE_";
    kernel_name.append(var_name);

    _problem->addKernel("TimeDerivative", kernel_name, poly_params);
  }
}
