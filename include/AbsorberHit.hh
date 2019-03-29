#ifndef AbsorberHit_h
#define AbsorberHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

#include <string>

using namespace std;


class G4AttDef;

class AbsorberHit : public G4VHit
{
public:
  AbsorberHit(); 	//constructor
  virtual ~AbsorberHit();	//destructor
  AbsorberHit(const AbsorberHit &right);	//copy
  //--------------------operator definitions------------------------------------------
  const AbsorberHit& operator=(const AbsorberHit &right);	
  G4int operator==(const AbsorberHit &right) const;
  inline void *operator new(size_t);
  inline void operator delete(void *aHit);
  //----------------------------------------------------------------------------------
  void Draw();
  void Print();
  
  private:
  G4double edep; //deposited energy
  G4ThreeVector pos; //interation position
  G4ThreeVector momentum; //
  G4double time; //interaction time (from beginning of event - global)
  int particle; //PDG code of incident particle
  string processName; //interaction process name 
  string volumeName; //volume where interaction took place
  
  
public:
  inline void SetEdep(G4double de){ edep = de; }
  inline G4double GetEdep(){ return edep; }
  
  inline void SetPosition(G4ThreeVector xyz){ pos = xyz; }
  inline G4ThreeVector GetPosition(){ return pos; }
  
  inline void SetMomentum(G4ThreeVector pxpypz){ momentum = pxpypz; }
  inline G4ThreeVector GetMomentum(){ return momentum; }
  
  inline void SetTime(G4double t){ time=t; }
  inline G4double GetTime(){ return time; }
  
  inline void SetParticle(G4int part){ particle=part; }
  inline int GetParticle(){ return particle; }

  inline void SetProcess(G4String inter){ processName=inter; }
  inline string GetProcess(){ return processName; }

  inline void SetVolume(G4String volName){volumeName = volName; }
  inline string GetVolume(){return volumeName;}
  
};

typedef G4THitsCollection<AbsorberHit> AbsorberHitsCollection;
extern G4Allocator<AbsorberHit> AbsorberHitAllocator;
inline void* AbsorberHit::operator new(size_t)
{
  void *aHit;
  aHit = (void *) AbsorberHitAllocator.MallocSingle();
  return aHit;
}
inline void AbsorberHit::operator delete(void *aHit)
{
  AbsorberHitAllocator.FreeSingle((AbsorberHit*) aHit);
}

#endif
