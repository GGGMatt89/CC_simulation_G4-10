#ifndef ScattererSD_h
#define ScattererSD_h 1

#include "G4VSensitiveDetector.hh"
#include "ScattererHit.hh"


class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class ScattererSD : public G4VSensitiveDetector
{
public:
  ScattererSD(G4String name);
  virtual ~ScattererSD();
  
  virtual void Initialize(G4HCofThisEvent*HCE);
  virtual G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist);
  virtual void EndOfEvent(G4HCofThisEvent*HCE);
  void clear();
  void DrawAll();
  void PrintAll();
  
private:
  ScattererHitsCollection *scattererCollection;
  G4int HCID { -1 };	//Hits Collection ID
};
#endif
