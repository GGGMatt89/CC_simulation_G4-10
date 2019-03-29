#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

#include "RecorderBase.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class EventAction : public G4UserEventAction
{
 public:
  EventAction();
  EventAction(RecorderBase* r,G4String, G4int);
  virtual ~EventAction();
 

  void  BeginOfEventAction(const G4Event*);
  void  EndOfEventAction(const G4Event*);
    
    
 private:
   
  RecorderBase * fRec;
  G4int fN_scatt;
  G4int* scattererCollID;
  G4int absorberCollID;
  G4String myverbose;
  G4int NoEvent;
   
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
