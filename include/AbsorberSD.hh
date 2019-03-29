#ifndef AbsorberSD_h
#define AbsorberSD_h 1

#include "G4VSensitiveDetector.hh"
#include "AbsorberHit.hh"


class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class AbsorberSD : public G4VSensitiveDetector
{
  
public:
  AbsorberSD(G4String name);
  virtual ~AbsorberSD();
  
  virtual void Initialize(G4HCofThisEvent*HCE) override;
  virtual G4bool ProcessHits(G4Step*aStep, G4TouchableHistory*ROhist) override;
  virtual void EndOfEvent(G4HCofThisEvent*HCE);
  void clear();
  void DrawAll();
  void PrintAll();
  
private:
  AbsorberHitsCollection *AbsorberCollection;
  G4int HCID { -1 };
 
};
#endif
