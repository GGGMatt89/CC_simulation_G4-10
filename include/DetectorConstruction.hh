#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "ScattererSD.hh"
#include "AbsorberSD.hh"
#include "globals.hh"
#include "DetectorConstructionMessenger.hh"

class G4LogicalVolume;
class DetectorConstructionMessenger;

/// Detector construction class to define materials and geometry.
///

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction(G4double d0, G4double d1, G4double d2,G4double x_scatt, G4double y_scatt, G4double z_scatt, G4int n_block_absorber_x, G4int n_block_absorber_y, G4double l_block, G4double z_block, G4int n_Scatt);
    virtual ~DetectorConstruction();

  public:
    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();

  void UpdateGeometry();

  //Set methods
  void Setd0(G4double); 
  void Setd1(G4double);
  void Setd2(G4double);
  void SetX_scatt(G4double);
  void SetY_scatt(G4double);
  void SetZ_scatt(G4double);
  void SetL_block(G4double);
  void SetZ_block(G4double);
  void SetN_block_x(G4int);
  void SetN_block_y(G4int);
  void SetN_scatt(G4int);

  //Get methods
  G4double Getd0() const {return fD0;}; 
  G4double Getd1() const {return fD1;};
  G4double Getd2() const {return fD2;};
  G4double GetX_scatt() const {return fX_scatt;};
  G4double GetY_scatt() const {return fY_scatt;};
  G4double GetZ_scatt() const {return fZ_scatt;};
  G4double GetL_block() const {return fL_block;};
  G4double GetZ_block() const {return fZ_block;};
  G4int GetN_block_x() const {return fN_block_absorber_x;};
  G4int GetN_block_y() const {return fN_block_absorber_y;};
  G4int GetN_scatt() const {return fN_Scatt;};
  

  private:

    DetectorConstructionMessenger* theMessenger; 

    //inter detectors distances and sizes, geometrical parameters
    //
  //G4String fScatt_Mat, fAbs_Mat;
    G4double fD0, fD1, fD2;
    G4double fX_scatt,fY_scatt,fZ_scatt,fL_block,fZ_block;
    G4int fN_Scatt,fN_block_absorber_x, fN_block_absorber_y;
  
    //Logical volumes
    G4LogicalVolume* logicWorld;
    G4LogicalVolume** logicScatterer;
    G4LogicalVolume* logicAbsorber;
    
    //Physical volumes
    G4VPhysicalVolume* physWorld;
    G4VPhysicalVolume** physScatterer;
    G4VPhysicalVolume* physAbsorber;
   

    //sensitive detectors
    ScattererSD** scattererSD;
    AbsorberSD* absorberSD;
    
    //Geometry overlap check 
    G4bool  fCheckOverlaps;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
