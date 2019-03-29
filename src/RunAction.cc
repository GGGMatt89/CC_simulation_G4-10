#include "RunAction.hh"
#include "DetectorConstruction.hh"

#include "G4Run.hh"
#include "G4UImanager.hh"
#include "G4VVisManager.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"


//#include "RecorderBase.hh"

#include <iostream>
#include <sstream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction() : G4UserRunAction() 
{
  fRec = NULL;
}

RunAction::RunAction(RecorderBase * r)
{
  fRec = r;
  /*G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetFirstNtupleId(1);
  
  //Create Ntuple for storage
 analysisManager->CreateNtuple("detector_hits", "hit in the detectors");
 analysisManager->CreateNtupleDColumn("EnergyDeposit");
 analysisManager->CreateNtupleDColumn("X");
 analysisManager->CreateNtupleDColumn("Y");
 analysisManager->CreateNtupleDColumn("Z");
 analysisManager->CreateNtupleDColumn("Time");
 analysisManager->CreateNtupleSColumn("Particle");
 analysisManager->CreateNtupleSColumn("Process");
 analysisManager->CreateNtupleSColumn("Volume");
 analysisManager->FinishNtuple();

 analysisManager->OpenFile("outputNativeStorage");*/

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{
  /*G4AnalysisManager* man = G4AnalysisManager::Instance();
  man->Write();
  man->CloseFile();
  */
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//Definition of what you need at the beginning of the run - BeamOn -> useful to store information about the whole run
void RunAction::BeginOfRunAction(const G4Run* aRun)
{
  G4cout << "### Run " << aRun->GetRunID() << " starts..." << G4endl; 
  if (fRec != NULL) fRec->RecordBeginOfRun(aRun); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//Information stored at the end of the run
void RunAction::EndOfRunAction(const G4Run* aRun)
{

  G4int nEv = aRun->GetNumberOfEvent();
  
  G4cout << "### Run " << aRun->GetRunID() << " over!" << G4endl; 
  G4cout << "It was " << nEv  << "  events." << G4endl;

  if (fRec != NULL) fRec->RecordEndOfRun(aRun);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
