#include "EventAction.hh"
#include "ScattererHit.hh"
#include "AbsorberHit.hh"

#include "G4Event.hh"
#include "G4VTrajectory.hh"
#include "G4VVisManager.hh"
#include "G4UnitsTable.hh"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4THitsMap.hh"

#include "Randomize.hh"

#include <sstream>
#include <iostream>


using namespace std;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
EventAction::EventAction()

{
  EventAction(NULL,"0",0);    
}

EventAction::EventAction(RecorderBase* r,G4String verbose,G4int N_Scatt)

{
  fN_scatt=N_Scatt;
  //Initialization of hit collections for all the detectors
  scattererCollID = new G4int[N_Scatt];
  for(G4int i=0;i<N_Scatt;i++)
    {
      scattererCollID[i] = -1;
    }
  absorberCollID = -1;
  myverbose=verbose;
  fRec=r;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{
  ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......



void EventAction::BeginOfEventAction(const G4Event* evt)
{  	
  
  if (fRec!=NULL)
    {
     fRec->RecordBeginOfEvent(evt);
     }
  NoEvent = evt->GetEventID();
  if(NoEvent%1000==0) G4cout<<"Processing event : "<<NoEvent<<G4endl;
    
    }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* evt)
{

  G4SDManager * SDman = G4SDManager::GetSDMpointer();
  //Get the absorber hit collection
  if((absorberCollID<0))
    {
      G4String colNam;
      absorberCollID =   SDman->GetCollectionID(colNam="absorber/absorberCollection");
    }
  for(G4int i=0;i<fN_scatt;i++)
    {
      if((scattererCollID[i]<0))
	{
	  G4String colNam;
	  std::ostringstream obuffer;
	  obuffer<<(1+(G4double)i);
	  G4String scattererSDname="scatterer"+obuffer.str();
	  scattererCollID[i] =   SDman->GetCollectionID(colNam=scattererSDname+"/"+scattererSDname+"Collection");
	}
    }
  
  
  //If some collections are missing, return
  for(G4int i=0;i<fN_scatt;i++)
    {
      if((scattererCollID[i]<0)) {
	G4cout<<"WARNING : one of the scatterer hit collection not found "<<G4endl;
	return;
      }
    }
  if((absorberCollID<0)) {
	G4cout<<"WARNING : absorber hit collection not found "<<G4endl;
	return;
  }

  //Get the hit collections
  G4HCofThisEvent * HCE = evt->GetHCofThisEvent();
  ScattererHitsCollection** SHC = new ScattererHitsCollection*[fN_scatt];
  for(G4int i=0;i<fN_scatt;i++) SHC[i] = 0;
  
  AbsorberHitsCollection* AHC = 0;   //Absorber HitsCollection
  
  if(HCE)
    {
      for(G4int i=0;i<fN_scatt;i++)SHC[i] = (ScattererHitsCollection*)(HCE->GetHC(scattererCollID[i])); 
      AHC = (AbsorberHitsCollection*)(HCE->GetHC(absorberCollID));
    }
  else
  {
      G4cout<<"WARNING : no hit collection for event "<<evt->GetEventID() <<G4endl;
    }
  

  ////////////////////////////////////////////////////
  ////////////------------STORAGE-----------//////////
  ///////////////////////////////////////////////////
 
  for(G4int i=0;i<fN_scatt;i++)
    {    
      if((SHC[i])&&(fRec!=NULL))
	{
	  fRec->RecordHitScatt(evt, SHC[i]);
	  G4cout<<"Scatterer data recorded"<<G4endl;
	}
    }
  
  if(AHC && fRec!=NULL) {fRec->RecordHitAbs(evt, AHC);
    G4cout<<"Absorber data recorded"<<G4endl;}
  
   if (fRec!=NULL)
    {
      fRec->RecordEndOfEvent(evt);
    }
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
