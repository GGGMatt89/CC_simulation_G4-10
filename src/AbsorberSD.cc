#include "AbsorberSD.hh"
#include "AbsorberHit.hh"
#include "G4Step.hh"
#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4ios.hh"
#include "G4Track.hh"
#include "G4SDManager.hh"
#include "G4VProcess.hh"

AbsorberSD::AbsorberSD(G4String name):G4VSensitiveDetector(name),HCID(-1)
{
  G4String HCname;
  collectionName.insert(HCname=name+"Collection");

}

AbsorberSD::~AbsorberSD()
{;}

void AbsorberSD::Initialize(G4HCofThisEvent* HCE)
{
  if(HCID < 0){
    HCID = GetCollectionID(0);
    //HCID = G4SDManager::GetSDMpointer()->GetCollectionID(AbsorberCollection);
  }
  AbsorberCollection = new AbsorberHitsCollection(SensitiveDetectorName,collectionName[0]);
  HCE->AddHitsCollection(HCID, AbsorberCollection);

}


G4bool AbsorberSD::ProcessHits(G4Step* aStep, G4TouchableHistory*ROhist)
{
  //Get energy deposited
  G4double edep = aStep->GetTotalEnergyDeposit();
  //Get volume name
  G4StepPoint* preStep = aStep->GetPreStepPoint();
  ROhist = (G4TouchableHistory*)(preStep->GetTouchable());
  G4String physVolName = ROhist->GetVolume()->GetName();
  G4Track* track = aStep->GetTrack();
  const G4VProcess* process = track->GetCreatorProcess();
  G4String pname;
  if(process){ pname = process->GetProcessName();}
  else{G4cout<<"Problem with process definition - event rejected"<<G4endl;
    return false;}
  //Fill the Hit
  AbsorberHit* newHit= new AbsorberHit();
  newHit->SetEdep( edep );
  newHit->SetPosition( preStep->GetPosition() );
  newHit->SetTime(track->GetGlobalTime());
  newHit->SetParticle(track->GetDefinition()->GetPDGEncoding());
  newHit->SetProcess(process->GetProcessName());
  newHit->SetVolume(physVolName);
  if(edep==0.) { G4cout<<"no energy deposit in abs"<<G4endl; G4cout<<"Step generated for "<<pname<<" by particle "<<track->GetDefinition()->GetPDGEncoding()<<" with ID "<<track->GetTrackID()<<G4endl; return false;}
  else{G4cout<<"Event with energy deposit in abs"<<G4endl;}
  AbsorberCollection->insert( newHit );

  return true;
}


void AbsorberSD::EndOfEvent(G4HCofThisEvent* HCE)
{
  if(HCID<0)
    {
      HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    }

  HCE->AddHitsCollection(HCID,AbsorberCollection);
}

void AbsorberSD::clear()
{
}

void AbsorberSD::DrawAll()
{
}

void AbsorberSD::PrintAll()
{
}
