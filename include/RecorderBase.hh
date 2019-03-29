#ifndef RECORDER_BASE_H_
#define RECORDER_BASE_H_

#include "globals.hh"
#include "G4Run.hh"
#include "G4Event.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4ParticleGun.hh"
#include "AbsorberHit.hh"
#include "ScattererHit.hh"

class RecorderBase {

public:

  virtual ~RecorderBase() {};

  // The following a list of methods that correspond to the available 
  // user action classes in Geant 4.   In this base class, the
  // methods are defined to do nothing.

  virtual void RecordBeginOfRun(const G4Run*) = 0;
  virtual void RecordEndOfRun(const G4Run*) = 0;
  virtual void RecordBeginOfEvent(const G4Event*) {};
  virtual void RecordEndOfEvent(const G4Event*) {};
  virtual void RecordTrack() {};
  virtual void RecordStepDE(const G4Step*) {};
  //virtual void RecordStepFrag(const G4Step*) {};
  //virtual void RecordStepFragReact(const G4Step*) {};
  //virtual void RecordCrystal(const G4Step*) {};
  virtual void RecordPrimaries(G4ParticleGun *) {};
  virtual void RecordHitScatt(const G4Event *, ScattererHitsCollection *){};
  virtual void RecordHitAbs(const G4Event *, AbsorberHitsCollection *){};
   
private:

};

#endif

