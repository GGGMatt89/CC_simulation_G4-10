#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "RecorderBase.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class SteppingAction : public G4UserSteppingAction
{
public:
  SteppingAction();
  SteppingAction(RecorderBase* r);
  virtual ~SteppingAction();
  
  void UserSteppingAction(const G4Step*);     
  
private:
  RecorderBase* fRec;
};

#endif
