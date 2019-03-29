#include "ScattererHit.hh"
#include "G4UnitsTable.hh"
#include "G4AttValue.hh"
#include "G4AttDef.hh"
#include "G4AttCheck.hh"


G4Allocator<ScattererHit> ScattererHitAllocator;

ScattererHit::ScattererHit()
{
  ;
}

ScattererHit::~ScattererHit()
{
  ;
}

ScattererHit::ScattererHit(const ScattererHit &right): G4VHit()
{
  edep = right.edep;
  pos = right.pos;
}

const ScattererHit& ScattererHit::operator=(const ScattererHit &right)
{
  edep = right.edep;
  pos = right.pos;
  return *this;
}

G4int ScattererHit::operator==(const ScattererHit &right) const
{
  return (this==&right) ? 1 : 0;
}

void ScattererHit::Draw()
{
}


void ScattererHit::Print()
{
  
}


