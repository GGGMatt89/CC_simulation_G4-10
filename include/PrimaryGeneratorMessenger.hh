#ifndef PrimaryGeneratorMessenger_h
#define PrimaryGeneratorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class G4ParticleTable;
class G4UIcommand;
class G4UIdirectory;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAnInteger;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWith3Vector;
class G4UIcmdWith3VectorAndUnit;

class PrimaryGeneratorAction;

class PrimaryGeneratorMessenger: public G4UImessenger
{
  public:
    PrimaryGeneratorMessenger(PrimaryGeneratorAction*);
    virtual ~PrimaryGeneratorMessenger();

  public:
   void SetNewValue(G4UIcommand *command, G4String newValues);
   G4String GetCurrentValue(G4UIcommand * command);

  private:
   PrimaryGeneratorAction* fParticleGun;
   G4ParticleTable * particleTable;
  
  private: //commands
   G4UIdirectory *             primGenDirectory;
   //G4UIcmdWithAString *      particleNameCmd;
   G4UIcmdWithADoubleAndUnit * EnergyCmd;
   G4UIcmdWithAString *         DirectionCmd;
  
   G4UIcmdWithoutParameter * updateGunCmd; 
  
};

#endif
