#include "ScattererSD.hh"
#include "ScattererHit.hh"
#include "G4Step.hh"
#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4ios.hh"
#include "G4Track.hh"
#include "G4SDManager.hh"
#include "G4VProcess.hh"

ScattererSD::ScattererSD(G4String name):G4VSensitiveDetector(name),HCID(-1)
{
  G4String HCname;
  collectionName.insert(HCname=name+"Collection");

}

ScattererSD::~ScattererSD()
{
  ;
}

void ScattererSD::Initialize(G4HCofThisEvent* HCE)
{
  if(HCID < 0){
    HCID = GetCollectionID(0);
    //HCID = G4SDManager::GetSDMpointer()->GetCollectionID(scattererCollection);
  }
  scattererCollection = new ScattererHitsCollection(SensitiveDetectorName,collectionName[0]);
  HCE->AddHitsCollection(HCID, scattererCollection);
}


G4bool ScattererSD::ProcessHits(G4Step* aStep, G4TouchableHistory*ROhist)
{
  //Get energy deposited
  G4double edep = aStep->GetTotalEnergyDeposit();
  if(edep==0.) return false;

  //Get volume name
  G4StepPoint* preStep = aStep->GetPreStepPoint();
  ROhist = (G4TouchableHistory*)(preStep->GetTouchable());
  G4String physVolName = ROhist->GetVolume()->GetName();
  char pN_c;
  int detID = 0;

  pN_c = physVolName.at( physVolName.length() - 1);
  detID = pN_c - '0';
  G4Track* track = aStep->GetTrack();
  const G4VProcess* process = track->GetCreatorProcess();

  //Fill the Hit
  ScattererHit* newHit= new ScattererHit();
  newHit->SetEdep( edep );
  newHit->SetPosition( preStep->GetPosition() );
  newHit->SetTime(track->GetGlobalTime());
  newHit->SetParticle(track->GetDefinition()->GetPDGEncoding());
  newHit->SetProcess(process->GetProcessName());
  newHit->SetDetectorID(detID);

  scattererCollection->insert( newHit );

  return true;
}

void ScattererSD::EndOfEvent(G4HCofThisEvent* HCE)
{
  if(HCID<0)
    {
      HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    }

  HCE->AddHitsCollection(HCID,scattererCollection);
}

void ScattererSD::clear()
{
}

void ScattererSD::DrawAll()
{
}

void ScattererSD::PrintAll()
{
}
