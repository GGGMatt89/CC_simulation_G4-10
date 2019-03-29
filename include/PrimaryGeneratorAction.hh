#ifndef PRIMARY_GENERATOR_ACTION_HH
#define PRIMARY_GENERATOR_ACTION_HH

#include <G4VUserPrimaryGeneratorAction.hh>
#include "globals.hh"
#include "PrimaryGeneratorMessenger.hh"

class G4ParticleGun;
class G4GeneralParticleSource;
class PrimaryGeneratorMessenger;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  PrimaryGeneratorAction();
  PrimaryGeneratorAction(G4double, G4String);
  ~PrimaryGeneratorAction();
  void GeneratePrimaries(G4Event* anEvent);

void UpdateGun();
  
//Methods for messenger
  //Set methods
  void SetEnergy(G4double); 
  void SetDirection(G4String);
  //Get methods
  G4double GetEnergy() const {return fEnergy;};
  G4String GetDirection() const {return fPartDir;};
  

private:

  PrimaryGeneratorMessenger* theMessenger;

  G4ParticleGun* fGun;
  G4GeneralParticleSource* fGPS;
  
  G4String fPartName;
  G4double fEnergy;
  G4String fPartDir;
};

#endif
