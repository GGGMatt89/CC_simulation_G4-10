#ifndef ROOTuple_h
#define ROOTuple_h 1

#include "RecorderBase.hh"
#include "TROOT.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TNtuple.h"
#include "TTree.h"
#include "TBranch.h"
#include "G4ios.hh"
#include "globals.hh"
#include "G4Run.hh"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4VProcess.hh"
#include "G4HCofThisEvent.hh"
#include "G4ParticleGun.hh"
#include "ScattererHit.hh"
#include "AbsorberHit.hh"
#include "Randomize.hh"
//#include "CLHEP/Random/RandGauss.h" 
//#include "G4RandGauss.hh"
#include "CLHEP/Random/RanecuEngine.h"

//#include "StoreTrajectories.hh"
#include "G4StepPoint.hh"
#include "G4Track.hh"
#include "G4TrackVector.hh"
#include "G4VSensitiveDetector.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4UImanager.hh"

#include <vector>
using namespace std;

typedef struct {
  G4double x,y,z,edep,time;
  int part_ID, evnt_ID, det_ID;
} HIT;

class ROOTuple : public RecorderBase

{
public:
  ROOTuple(G4String);
  virtual ~ROOTuple();
  
  void RecordBeginOfRun(const G4Run *);
  void RecordEndOfRun(const G4Run *);
  void RecordBeginOfEvent(const G4Event *);
  void RecordEndOfEvent(const G4Event *);
  void RecordTrack();
  void RecordStepDE(const G4Step*);
  void RecordPrimaries(G4ParticleGun *);
  void RecordHitScatt(const G4Event *, ScattererHitsCollection *);
  void RecordHitAbs(const G4Event *, AbsorberHitsCollection * AHC); 
  
private:

  //----------------------------------FILE STORAGE---------------------//
  G4String fROOT_outname; //output ROOT file name
  TFile* fOutROOT; //output ROOT file
  TTree* fTree_Scatt; //scatterer base storage tree
  TTree* fTree_Abs; //absorber base storage tree

  //---------------------------------STORING VARIABLES-----------------//
  
  //----------------event------------------
  int fEvent_ID; //primary event ID
  
  //-----------------step-------------------
  //Scatterer
  // static HIT fScatt_hit;
  std::string fIntName_Scatt; //scatterer hit interaction kind - EX: PhotoEl, Compton, PairCreat for photons
  std::string fVolume_Scatt; //name of scatterer detector where interaction took place
  G4double fPos_Scatt[3]; //scatterer hit position
  G4double fEdep_Scatt; //scatterer hit energy deposited
  G4double fTime_Scatt; //scatterer hit global time - from the event creation
  G4int fPartID_Scatt; //scatterer hit particle ID - PDG code

  HIT fScattHit;
  
  //Absorber
  // static HIT fAbs_hit;
  std::string fIntName_Abs; //absorber hit interaction kind - EX: PhotoEl, Compton, PairCreat for photons
  std::string fVolume_Abs; //name of absorber detector where interaction took place (if one day it will be divided in blocks)
  G4double fPos_Abs[3]; //absorber hit position
  G4double fEdep_Abs; //absorber hit energy deposited
  G4double fTime_Abs; //absorber hit global time - from the event creation
  G4int fPartID_Abs; //absorber hit particle ID - PDG code
  
  HIT fAbsHit;
  
  //HIT COLLECTIONS
  ScattererHit* fHit_Scatt;
  AbsorberHit* fHit_Abs;

};

#endif
