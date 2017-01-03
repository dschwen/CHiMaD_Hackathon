#include "ChimadHackathonApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "PhaseFieldApp.h"
#include "TensorMechanicsApp.h"
#include "MooseSyntax.h"

// Hackathon I - Ostwald ripening problem
#include "OstRipACKernel.h"
#include "OstRipFreeEnergy.h"
#include "OstRipACKernelAction.h"

template<>
InputParameters validParams<ChimadHackathonApp>()
{
  InputParameters params = validParams<MooseApp>();
  return params;
}

ChimadHackathonApp::ChimadHackathonApp(InputParameters parameters) :
    MooseApp(parameters)
{
  Moose::registerObjects(_factory);
  PhaseFieldApp::registerObjects(_factory);
  TensorMechanicsApp::registerObjects(_factory);
  ChimadHackathonApp::registerObjects(_factory);

  Moose::associateSyntax(_syntax, _action_factory);
  PhaseFieldApp::associateSyntax(_syntax, _action_factory);
  TensorMechanicsApp::associateSyntax(_syntax, _action_factory);
  ChimadHackathonApp::associateSyntax(_syntax, _action_factory);
}

ChimadHackathonApp::~ChimadHackathonApp()
{
}

// External entry point for dynamic application loading
extern "C" void ChimadHackathonApp__registerApps() { ChimadHackathonApp::registerApps(); }
void
ChimadHackathonApp::registerApps()
{
  registerApp(ChimadHackathonApp);
}

// External entry point for dynamic object registration
extern "C" void ChimadHackathonApp__registerObjects(Factory & factory) { ChimadHackathonApp::registerObjects(factory); }
void
ChimadHackathonApp::registerObjects(Factory & factory)
{
  registerKernel(OstRipACKernel);
  registerMaterial(OstRipFreeEnergy);
}

// External entry point for dynamic syntax association
extern "C" void ChimadHackathonApp__associateSyntax(Syntax & syntax, ActionFactory & action_factory) { ChimadHackathonApp::associateSyntax(syntax, action_factory); }
void
ChimadHackathonApp::associateSyntax(Syntax & syntax, ActionFactory & action_factory)
{
  syntax.registerActionSyntax("OstRipACKernelAction", "Kernels/OstRipACKernel");
  registerAction(OstRipACKernelAction, "add_kernel");
}
