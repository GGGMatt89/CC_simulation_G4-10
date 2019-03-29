#include "PrimaryGeneratorMessenger.hh"
#include "PrimaryGeneratorAction.hh"
#include "G4ThreeVector.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3Vector.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include <iostream>


PrimaryGeneratorMessenger::PrimaryGeneratorMessenger(PrimaryGeneratorAction *prtlGun)
: G4UImessenger(),
  fParticleGun(prtlGun),
  primGenDirectory(0),
  EnergyCmd(0),
  //particleNameCmd(0),
  updateGunCmd(0)
{

  //particleTable = G4ParticleTable::GetParticleTable();
  //Scatterer directory
  primGenDirectory = new G4UIdirectory("/primaryGen/");
  primGenDirectory->SetGuidance("Primary particle generator control commands.");
 
  //particle kind
  /* particleNameCmd = new G4UIcmdWithAString("/primaryGen/particle",this);
  particleNameCmd->SetGuidance("Set primar particle.");
  particleNameCmd->SetGuidance(" (gamma is default)");
  particleNameCmd->SetParameterName("particle",true);
  particleNameCmd->SetDefaultValue("gamma");
  particleNameCmd->SetCandidates("gamma e- e+ proton alpha"); 
  */
  //energy - > primary particle energy
  EnergyCmd = new G4UIcmdWithADoubleAndUnit("/primaryGen/energy",this);
  EnergyCmd->SetGuidance("Set primary particle energy.");
  EnergyCmd->SetParameterName("energy",true,true);
  EnergyCmd->SetDefaultUnit("MeV");
  EnergyCmd->SetUnitCandidates("eV keV MeV GeV");

  //direction -> primary particle momentum direction
  DirectionCmd=new G4UIcmdWithAString("/primaryGen/direction",this);
  DirectionCmd->SetGuidance("Set primary particle momentum direction.");
  DirectionCmd->SetGuidance("By default limited to the first scatterer plane acceptance solide angle");
  DirectionCmd->SetParameterName("direction",true);
  DirectionCmd->SetDefaultValue("psi_lim");
  DirectionCmd->SetCandidates("psi_lim iso"); 
  
  
 
  //Primary generator update after changements
  updateGunCmd = new G4UIcmdWithoutParameter("/primaryGen/updateGun",this);
  updateGunCmd->SetGuidance("Reinitialize primary generator after changements.");
}

PrimaryGeneratorMessenger::~PrimaryGeneratorMessenger()
{
  delete primGenDirectory;
  //delete particleNameCmd;
  delete EnergyCmd;
  delete DirectionCmd;
  delete updateGunCmd;
}

void PrimaryGeneratorMessenger::SetNewValue(G4UIcommand * command,G4String newValues)
{
   if( command==EnergyCmd )
    {fParticleGun->SetEnergy(EnergyCmd->GetNewDoubleValue(newValues)); }
  else if( command==DirectionCmd )
    {fParticleGun->SetDirection(newValues); }  
  else if( command==updateGunCmd )
    {fParticleGun->UpdateGun(); }
  else {G4cout<< "Command not valid! "<<G4endl; }
}


G4String PrimaryGeneratorMessenger::GetCurrentValue(G4UIcommand * command)
{
  G4String cv;

  if( command==EnergyCmd )
    {cv = EnergyCmd->ConvertToString(fParticleGun->GetEnergy(), "keV"); }
  else if( command==DirectionCmd )
    {cv = DirectionCmd->ConvertToString(fParticleGun->GetDirection()); }
  
    return cv;
}
