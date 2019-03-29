#include "TrackingAction.hh"
#include "G4TrackingManager.hh"
#include "G4Track.hh"

TrackingAction::TrackingAction()
{
  
}

TrackingAction::TrackingAction(RecorderBase* r)
{
  fRec=r;
}

TrackingAction::~TrackingAction()
{

}

void TrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{
  fpTrackingManager->G4TrackingManager::SetStoreTrajectory(1);
  
  if(fRec!=NULL)
    {
      fRec->RecordTrack();
    }
}

void TrackingAction::PostUserTrackingAction(const G4Track* aTrack)
{
}
