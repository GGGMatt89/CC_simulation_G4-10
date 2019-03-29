#include "G4UnitsTable.hh"
#include "G4ios.hh"
#include "G4VProcess.hh"
#include "G4ParticleTypes.hh"

#include "SteppingAction.hh"

#include "G4SteppingManager.hh"
#include "G4UnitsTable.hh"
#include "G4VProcess.hh"
using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction()
{
  fRec=NULL;		
}

SteppingAction::SteppingAction(RecorderBase* r)
{
  fRec=r;		
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{ 
  ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
  if (fRec!=NULL)
    {
      fRec->RecordStepDE(aStep);
    }
}	


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......



