#include "ROOTuple.hh"

#include "G4UnitsTable.hh"
#include "G4SDManager.hh"
#include "G4Track.hh"
#include "G4VSensitiveDetector.hh"
#include "G4StepPoint.hh"
#include "G4VProcess.hh"
#include "G4ParticleDefinition.hh"
#include "G4TrackVector.hh"
#include "Randomize.hh"
#include "CLHEP/Random/RandGauss.h"
#include "CLHEP/Random/RanecuEngine.h"
#include "TRandom.h"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>


#define zero G4ThreeVector(0.0,0.0,0.0)//to reset the vectors used for storing positions


ROOTuple::ROOTuple(G4String root_out)
{//DEFAULT CONSTRUCTOR
  fROOT_outname = root_out;    //output ROOT file name
  fScattHit = {};
  fAbsHit = {};

  //Storing ROOT trees
  fTree_Scatt = NULL;
  fTree_Abs   = NULL;


}

ROOTuple::~ROOTuple()
{//DEFAULT DESTRUCTOR
  delete fHit_Scatt;
  delete fHit_Abs;
  delete fTree_Scatt;
  delete fTree_Abs;
  //Closing and deleting the output ROOT file
  if(fOutROOT!=NULL){
    fOutROOT->Close();
    delete fOutROOT;
  }
}

//****************************************************************************************************************************
//-------------------Run----------------------------------------------------------------------------------------------------

void ROOTuple::RecordBeginOfRun(const G4Run* r)
{
  G4cout<<"This is the beginnin of run "<<r->GetRunID()<<G4endl;
  fOutROOT = new TFile(Form("%s_%d.root", fROOT_outname.c_str(), r->GetRunID()),"RECREATE");  //opening ROOT output file

  //Declation of storage ROOT trees
  //SCATTERER base TREE - store interaction kind, position, E deposited, time, particle ID, and event ID
  fTree_Scatt = new TTree("Scatterer_base_tree", "Scatterer_base_tree");
  fTree_Scatt->Branch("scatt_raw_hits", &fScattHit.x,"x/D:y:z:edep:time:part_ID/I:evnt_ID:det_ID" );
  fTree_Scatt->Branch("scatt_raw_hits_int_name", &fIntName_Scatt);


  //ABSORBER base TREE - store interaction kind, position, E deposited, time, particle ID, and event ID
  fTree_Abs = new TTree("Absorber_base_tree", "Absorber_base_tree");
  fTree_Abs->Branch("abs_raw_hits", &fAbsHit.x,"x/D:y:z:edep:time:part_ID/I:evnt_ID:det_ID" );
  fTree_Abs->Branch("abs_raw_hits_int_name",  &fIntName_Abs);
}



void ROOTuple::RecordEndOfRun(const G4Run* r)
{
  fOutROOT->Write();

  delete fTree_Scatt;
  delete fTree_Abs;
  G4cout<<"This is the end of run "<<r->GetRunID()<<G4endl;
}

//****************************************************************************************************************************
//--------------------------------------------------PrimaryAction-------------------------------------------------------------

void ROOTuple::RecordPrimaries(G4ParticleGun* p)
{

}

//*******************************************************************************************************************************
//--------------------------------------------------Event-----------------------------------------------------------------------


void ROOTuple::RecordBeginOfEvent(const G4Event* anEvent)
{
  G4cout<<"This is the beginnin of event "<<anEvent->GetEventID()<<G4endl;
  //clear structures for hit storage
  fScattHit = {};//structure for hits in the scatterer stack
  fAbsHit = {};//structure for hits in the absorber
  fEvent_ID = anEvent->GetEventID();//this is the ID of the event

}

void ROOTuple::RecordEndOfEvent(const G4Event* anEvent)
{
  G4cout<<"This is the end of event "<<anEvent->GetEventID()<<G4endl;
}


//***********************************************************************************************************************************************
//------------------------------Step-------------------------------------------------------------------------------------------------------------


void ROOTuple::RecordStepDE(const G4Step* aStep)
{



}


//----------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------Hit------------------------------------------------------------------------------------

void ROOTuple::RecordHitScatt(const G4Event* anEvent, ScattererHitsCollection * SHC)
{
  fScattHit = {};//clear structure for hit storage
  //int n_hit = SHC->entries();
  G4ThreeVector pos;
  G4int Nevt = anEvent->GetEventID();
  string process;
  for(auto hitS : *SHC->GetVector())
    {
      pos = hitS->GetPosition();
      fScattHit.x = pos.getX() / mm;
      fScattHit.y = pos.getY() / mm;
      fScattHit.z = pos.getZ() / mm;
      fScattHit.edep = hitS->GetEdep() / keV;
      fScattHit.time = hitS->GetTime() / ns;
      fScattHit.part_ID = hitS->GetParticle();
      fScattHit.evnt_ID = Nevt;
      process = hitS->GetProcess();
      fIntName_Scatt = process;
      fScattHit.det_ID = hitS->GetDetectorID();

      fTree_Scatt->Fill();
    }

}

void ROOTuple::RecordHitAbs(const G4Event* anEvent, AbsorberHitsCollection * AHC)
{
  fAbsHit = {};//clear structure for hit storage
  //int n_hit = AHC->entries();
  G4ThreeVector pos;
  G4int Nevt = anEvent->GetEventID();
  string process;
  for(auto hitA : *AHC->GetVector())
    {
      pos = hitA->GetPosition();
      fAbsHit.x = pos.getX() / mm;
      fAbsHit.y = pos.getY() / mm;
      fAbsHit.z = pos.getZ() / mm;
      fAbsHit.edep =  hitA->GetEdep() / keV;
      fAbsHit.time =  hitA->GetTime() / ns;
      process = hitA->GetProcess();
      fIntName_Abs =  process;
      fAbsHit.det_ID = 99;
      fAbsHit.part_ID =  hitA->GetParticle();
      fAbsHit.evnt_ID = Nevt;

      fTree_Abs->Fill();
    }

}
//-------------------------------------------------------------Track------------------------------------------------

void ROOTuple::RecordTrack()
{

}
