#include "AppFactory.h"
#include "ChimadHackathonApp.h"
#include "ModulesApp.h"
#include "Moose.h"
#include "MooseSyntax.h"

// Hackathon I - Ostwald ripening problem
#include "CutHole.h"
#include "ImproveElements.h"
#include "OstRipACKernel.h"
#include "OstRipACKernelAction.h"
#include "OstRipFreeEnergy.h"
#include "OstRipICAction.h"
#include "OstRipPseudoRand.h"
#include "SphereSurfaceMesh.h"

template <>
InputParameters
validParams<ChimadHackathonApp>()
{
  InputParameters params = validParams<MooseApp>();
  return params;
}

ChimadHackathonApp::ChimadHackathonApp(InputParameters parameters)
  : MooseApp(parameters)
{
  Moose::registerObjects(_factory);
  ModulesApp::registerObjects(_factory);
  ChimadHackathonApp::registerObjects(_factory);

  Moose::associateSyntax(_syntax, _action_factory);
  ModulesApp::associateSyntax(_syntax, _action_factory);
  ChimadHackathonApp::associateSyntax(_syntax, _action_factory);
}

ChimadHackathonApp::~ChimadHackathonApp()
{
}

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

// External entry point for dynamic object registration
extern "C" void
ChimadHackathonApp__registerObjects(Factory & factory)
{
  ChimadHackathonApp::registerObjects(factory);
}
void
ChimadHackathonApp::registerObjects(Factory & factory)
{
  registerMeshModifier(CutHole);
  registerMeshModifier(ImproveElements);
  registerKernel(OstRipACKernel);
  registerMaterial(OstRipFreeEnergy);
  registerInitialCondition(OstRipPseudoRand);
  registerMesh(SphereSurfaceMesh);
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
  syntax.registerActionSyntax("OstRipACKernelAction", "Kernels/OstRipACKernel");
  registerAction(OstRipACKernelAction, "add_kernel");

  syntax.registerActionSyntax("OstRipICAction", "ICs/OstRipIC");
  registerAction(OstRipICAction, "add_ic");
}
