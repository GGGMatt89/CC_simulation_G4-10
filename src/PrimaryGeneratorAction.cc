#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"

#include <G4ParticleTable.hh>
#include <G4ParticleDefinition.hh>
#include <G4RunManager.hh>
#include <G4Event.hh>
#include <G4SystemOfUnits.hh>
#include <G4ParticleGun.hh>
#include <G4GeneralParticleSource.hh>
#include <Randomize.hh>
#include <G4PhysicalConstants.hh>
#include <cmath>


using namespace std;

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
  fGun = new G4ParticleGun(); 
  //fGPS = new G4GeneralParticleSource();
  // default particle kinematic
  
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particle = particleTable->FindParticle("gamma");
  fGun->SetParticleDefinition(particle);
  fGun->SetParticlePosition(G4ThreeVector(0.,0.,0.));
  fGun->SetParticleEnergy(1.0*keV);
  fGun->SetParticleMomentumDirection(G4ThreeVector(1.,0.,0.));
  
  //Uncomment for GPS
  /*fGPS->SetParticleDefinition(particle);
    fGPS->GetCurretnSource()->GetEneDist()->SetMonoEnergy(100 * keV);
    fGPS->GetCurretnSource()->GetAngDist()->SetParticleMomentumDirection(G4ThreeVector(1.,0.,0.));*/

  theMessenger = new PrimaryGeneratorMessenger(this);

}

PrimaryGeneratorAction::PrimaryGeneratorAction(G4double energy, G4String dir)
{ fEnergy = energy;
  fPartDir = dir;
  fPartName = "gamma";
  fGun = new G4ParticleGun();
  // default particle kinematic
  
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particle = particleTable->FindParticle(fPartName);
  fGun->SetParticleDefinition(particle);
  fGun->SetParticlePosition(G4ThreeVector(0.,0.,0.));
  fGun->SetParticleEnergy(fEnergy);
  //fGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));

  theMessenger = new PrimaryGeneratorMessenger(this);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{  
  delete fGun;
  delete theMessenger;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  
  G4RunManager* theRunManager = G4RunManager::GetRunManager();
  const DetectorConstruction* myGeometry = (const DetectorConstruction*) theRunManager->GetUserDetectorConstruction();
  G4double d0 = myGeometry->Getd0();
  G4double x_scatt = myGeometry->GetX_scatt();
  G4double y_scatt = myGeometry->GetY_scatt();
  
  G4double psi= 4*asin((x_scatt*y_scatt)/(sqrt((pow(x_scatt,2)+4*pow(d0,2))*(pow(y_scatt,2)+4*pow(d0,2)))));
  G4double cosanglepsi;
  
  G4double anglephi= G4UniformRand()*2*pi;
  if(fPartDir=="psi_lim")cosanglepsi=(1-cos(psi))*G4UniformRand()-1; // cone on first Si plane
  if(fPartDir == "iso")cosanglepsi=2*G4UniformRand()-1; //isotrope 
  //G4double anglepsi=acos(cosanglepsi);

  // fGun->SetParticleEnergy(fEnergy);
  //particleGun->SetParticleEnergy(Emin*keV);
  G4double sinanglepsi = sqrt(1-cosanglepsi*cosanglepsi);
       
  G4ParticleDefinition* particle = fGun->GetParticleDefinition();
  fGun->SetParticleDefinition(particle);  
  fGun->SetParticlePosition(G4ThreeVector(0., 0., 0.));
  fGun->SetParticleMomentumDirection(G4ThreeVector(sinanglepsi*cos(anglephi),sinanglepsi*sin(anglephi),cosanglepsi)); 
  fGun->GeneratePrimaryVertex(anEvent);
}


void PrimaryGeneratorAction::UpdateGun()
{ 
  /* 
  G4RunManager* theRunManager = G4RunManager::GetRunManager();
  theRunManager->ReinitializeGeometry(true);
  */
}


void PrimaryGeneratorAction::SetEnergy(G4double newE)
{
  fEnergy = newE;
}

void PrimaryGeneratorAction::SetDirection(G4String newDir)
{
  fPartDir = newDir;
}

