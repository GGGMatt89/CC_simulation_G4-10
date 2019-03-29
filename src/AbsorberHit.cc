#include "AbsorberHit.hh"
#include "G4UnitsTable.hh"
#include "G4AttValue.hh"
#include "G4AttDef.hh"
#include "G4AttCheck.hh"


G4Allocator<AbsorberHit> AbsorberHitAllocator;

AbsorberHit::AbsorberHit()
{
  ;
}

AbsorberHit::~AbsorberHit()
{
  ;
}

//------operator definitions------------
AbsorberHit::AbsorberHit(const AbsorberHit &right)
  : G4VHit()
{
  edep = right.edep;
  pos = right.pos;
}

const AbsorberHit& AbsorberHit::operator=(const AbsorberHit &right)
{
  edep = right.edep;
  pos = right.pos;
  return *this;
}

G4int AbsorberHit::operator==(const AbsorberHit &right) const
{
  return (this==&right) ? 1 : 0;
}


void AbsorberHit::Draw()
{
}


void AbsorberHit::Print()
{
}


