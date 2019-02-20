#include "AppFactory.h"
#include "ChimadHackathonApp.h"
#include "ModulesApp.h"

template <>
InputParameters
validParams<ChimadHackathonApp>()
{
  InputParameters params = validParams<MooseApp>();
  return params;
}

ChimadHackathonApp::ChimadHackathonApp(InputParameters parameters) : MooseApp(parameters)
{
  ChimadHackathonApp::registerAll(_factory, _action_factory, _syntax);
}

ChimadHackathonApp::~ChimadHackathonApp() {}

// External entry point for dynamic application loading
extern "C" void
ChimadHackathonApp__registerApps()
{
  ChimadHackathonApp::registerApps();
}
void
ChimadHackathonApp::registerApps()
{
  registerApp(ChimadHackathonApp);
}

// External entry point for object registration
extern "C" void
ChimadHackathonApp__registerAll(Factory & factory, ActionFactory & action_factory, Syntax & syntax)
{
  ChimadHackathonApp::registerAll(factory, action_factory, syntax);
}
void
ChimadHackathonApp::registerAll(Factory & factory, ActionFactory & action_factory, Syntax & syntax)
{
  Registry::registerObjectsTo(factory, {"ChimadHackathonApp"});
  Registry::registerActionsTo(action_factory, {"ChimadHackathonApp"});
  ChimadHackathonApp::associateSyntax(syntax, action_factory);

  ModulesApp::registerAll(factory, action_factory, syntax);
}

// External entry point for dynamic syntax association
extern "C" void
ChimadHackathonApp__associateSyntax(Syntax & syntax, ActionFactory & action_factory)
{
  ChimadHackathonApp::associateSyntax(syntax, action_factory);
}
void
ChimadHackathonApp::associateSyntax(Syntax & syntax, ActionFactory & action_factory)
{
  //   syntax.registerActionSyntax("OstRipACKernelAction", "Kernels/OstRipACKernel");
  //   registerAction(OstRipACKernelAction, "add_kernel");
  //
  //   syntax.registerActionSyntax("OstRipICAction", "ICs/OstRipIC");
  //   registerAction(OstRipICAction, "add_ic");
}
