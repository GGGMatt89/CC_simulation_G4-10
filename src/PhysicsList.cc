#include "PhysicsList.hh"

#include <G4EmStandardPhysics.hh>
#include <G4DecayPhysics.hh> 
#include <G4ProductionCutsTable.hh>
#include <G4SystemOfUnits.hh>

#include <G4EmLivermorePhysics.hh>
#include <G4EmExtraPhysics.hh>
#include <G4HadronPhysicsFTFP_BERT.hh>
#include <G4HadronElasticPhysics.hh>

PhysicsList::PhysicsList() : G4VModularPhysicsList()
{
  // Standard EM physics 
  // Replace G4EmStandardPhysics with G4EmLivermorePhysics
  RegisterPhysics(new G4EmStandardPhysics());
  
  // Default Decay Physics
  RegisterPhysics(new G4DecayPhysics());
  
  //Extra electromagnetic physics
  RegisterPhysics(new G4EmExtraPhysics());

  //Complete hadronic physics
  RegisterPhysics(new G4HadronElasticPhysics());
  RegisterPhysics(new G4HadronPhysicsFTFP_BERT());
}

void PhysicsList::SetCuts()
{  
  G4VUserPhysicsList::SetCuts();
  
  //Possible to update the production cut table energy range
  //G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(100.*eV, 100.*GeV);

  // In addition, dump the full list of cuts for the materials used in 
  // the setup
  DumpCutValuesTable();
}
