#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include "RecorderBase.hh"

class G4Run;

class RunAction : public G4UserRunAction
{
public:
  RunAction();
  RunAction(RecorderBase * r);
  virtual ~RunAction();
  
public:
  virtual void BeginOfRunAction(const G4Run*);
  virtual void EndOfRunAction(const G4Run*);
  
private:
  
  RecorderBase * fRec;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
