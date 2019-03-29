#include "DetectorConstruction.hh"

#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Colour.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSEnergyDeposit.hh"
#include "G4PSDoseDeposit.hh"
#include "G4VisAttributes.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include <iostream>
#include <sstream>


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction(G4double d0, G4double d1, G4double d2,G4double x_scatt, G4double y_scatt, G4double z_scatt, G4int n_block_absorber_x, G4int n_block_absorber_y, G4double l_block, G4double z_block, G4int n_Scatt)
: G4VUserDetectorConstruction(),
  fCheckOverlaps(true)
{ 
  fD0=d0*cm;                                     // distance source-first scatterer plane center [cm]
  fD1=d1*cm;                                     // distance between scatterer plane centers [cm]
  fD2=d2*cm;                                     // distance last scatterer plane center-absorber center [cm]
  fX_scatt=x_scatt*cm;                           // scatterer side x [cm]
  fY_scatt=y_scatt*cm;                           // scatterer side y [cm]
  fZ_scatt=z_scatt*cm;                           // scatterer thickness along particle direction [cm]
  fN_block_absorber_x=n_block_absorber_x;     // blocks in the absorber in x
  fN_block_absorber_y=n_block_absorber_y;     // blocks in the absorber in y
  fL_block=l_block*cm;                           // block size [cm] -> side of an absorber block
  fZ_block=z_block*cm;                           // block thickness [cm]
  fN_Scatt=n_Scatt;                           // number of scatterer planes in stack

  theMessenger = new DetectorConstructionMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{
  delete theMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  //-----------------------------MATERIAL DEFINITION------------------------------
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  G4NistManager* man = G4NistManager::Instance();

  G4double A;  // atomic mass
  G4double Z;  // atomic number
  G4int x; // for compounds composition
  G4double density; //temperature, pressure;
  G4double fractionmass;

  //--------------------------ELEMENTS---------------------------------
  //NITROGEN
  A = 14.01*g/mole;
  G4Element* elN = new G4Element("Nitrogen","N",Z = 7.,A);
  //OXYGEN
  A = 16.00*g/mole;
  G4Element* elO = new G4Element("Oxygen","O",Z = 8.,A);
  //HYDROGEN
  A = 1*g/mole;
  G4Element* elH = new G4Element("Hydrogen","H",Z =1.,A);
  //LUTHETIUM
  A=174.97*g/mole;
  G4Element * elLu = new G4Element("Luthetium","Lu",Z=71.,A);
  //YTTRIUM
  A=88.90*g/mole;
  G4Element * elY = new G4Element("Yttrium","Y", Z=39.,A);
  //SILICON
  A=28.09*g/mole;
  G4Element * elSi = new G4Element("Silicium","Si",Z=14.,A);
  //BISMUTH
  A=208.98*g/mole;
  G4Element * elBi = new G4Element("Bismuth","Bi",Z=83.,A);
  //GERMANIUM
  A=72.64*g/mole;
  G4Element * elGe = new G4Element("Germanium","Ge",Z=32.,A);

  //--------------------------COMPOUNDS---------------------------------
  //Water
  density= 1.0*g/cm3;
  G4Material* Water =new G4Material("Water",density,2);
  Water->AddElement(elH,2);
  Water->AddElement(elO,1);
  // Air - user defined
  density = 1.290*mg/cm3;
  G4Material* Air = new G4Material("Air",density,2);
  // composition given in :
  // a) mass fraction if the second argument in AddElement() is a real number
  // b) number of atoms if the second argument in AddElement() is an integer number
  Air->AddElement(elN,fractionmass=0.7);
  Air->AddElement(elO,fractionmass=0.3);
  // Air -  G4 defined
  G4Material* AirG4  = man->FindOrBuildMaterial("G4_AIR");
  //SILICON - user defined density
  density=2.33*g/cm3;
  G4Material* Silicium=new G4Material("Silicium",density,1);
  Silicium->AddElement(elSi,1);
  //LYSO
  density=7.1*g/cm3;
  x=28;
  G4Material* LYSO = new G4Material("LYSO",density,4);
  LYSO->AddElement(elLu,200-x);
  LYSO->AddElement(elY,x);
  LYSO->AddElement(elSi,100);
  LYSO->AddElement(elO,500);
  //BGO
  density=7.13*g/cm3;
  G4Material* BGO = new G4Material("BGO",density,3);
  BGO->AddElement(elBi,4);
  BGO->AddElement(elGe,3);
  BGO->AddElement(elO,12);


  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  //-----------------------------GEOMETRY DEFINITION------------------------------
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  G4Colour color;

  //--------------------------------WORLD---------------------------------------
  //  G4Box size  = side length/2, watch out when varying  d0,d1 and d2, G4box size must be adapted
  G4double world_x = 100.0*cm;//cm
  G4double world_y = 100.0*cm;//cm
  G4double world_z = fD0+(fN_Scatt-1)*fD1+fD2+50.*cm;//cm
  G4Box* solidWorld  = new G4Box("World", world_x / 2,world_y / 2,world_z);
  logicWorld = new G4LogicalVolume(solidWorld,AirG4,"World",0,0,0,true);
  physWorld = new G4PVPlacement(0,G4ThreeVector(0.,0.,0.), logicWorld,"World",0,false,0, fCheckOverlaps);
  G4VisAttributes* World_vis = new G4VisAttributes(1);
  World_vis->SetColor(color.White());
  World_vis->SetVisibility(true);
  World_vis->SetForceSolid(false);
  logicWorld->SetVisAttributes(World_vis);

  //------------------------------------------ PATIENT/TARGET -----------------------------------------
  //----------------for applications with target
  //target position
  //G4double pos_p_x = 0.0;
  //G4double pos_p_y = 0.0;
  G4double pos_p_z = 0.0;//center of the target in z -> used to defined the distance from the scatterer stack

  //--------------------------------SCATTERER---------------------------------------
  G4double x_s = fX_scatt; //the side value given in tha main.cc must be divided by two, cause G4 wants the half side
  // watch out if detector sizes are modified, modify also DCCROOTuple.cc when adding noise to data when checking the data+noise are not outside the detectors
  G4double y_s = fY_scatt;
  G4double z_s = fZ_scatt;
  //G4double distance_p_s1=fD0;
  G4cout<<"Each scatterer plane is "<<x_s/cm<<" cm \t"<<y_s/cm<<" cm \t"<<z_s/cm<<" cm"<<G4endl; 
  //distance between the source (or the center of the target) and the center of the first Si plane
  G4Box * solidScatterer = new G4Box("Scatterer",x_s / 2,y_s / 2,z_s / 2);

  logicScatterer = new G4LogicalVolume *[fN_Scatt]; // stack of n scatterer planes
  physScatterer = new G4VPhysicalVolume *[fN_Scatt];

  G4String name_log, name_phys;
  G4VisAttributes* Scatt_vis = new G4VisAttributes(1);
  Scatt_vis->SetColor(color.Yellow());
  Scatt_vis->SetVisibility(true);
  Scatt_vis->SetForceSolid(true);
  

  G4double pos_s_x = 0.0;//cm
  G4double pos_s_y = 0.0;//cm
  G4double *pos_s_z = new G4double[fN_Scatt];//cm

  for(G4int i=0;i<fN_Scatt;i++) //create the desired number of scatterer planes in the world
    {
      std::ostringstream obuffer1;
      obuffer1<<(1+(G4double)i);
      name_log="logicScatterer"+obuffer1.str();
      name_phys="physicScatterer"+obuffer1.str();
      logicScatterer[i] = new G4LogicalVolume(solidScatterer,Silicium,name_log,0,0,0,true);
      logicScatterer[i]->SetVisAttributes(Scatt_vis);
      pos_s_z[i]= -(pos_p_z+fD0+i*fD1);
      physScatterer[i] = new G4PVPlacement(0,G4ThreeVector(pos_s_x,pos_s_y,pos_s_z[i]),logicScatterer[i],name_phys,logicWorld,false,0,fCheckOverlaps);
      G4cout<<"The scatterer plane "<<i+1<<" is in position "<<pos_s_x / cm <<" cm \t"<<pos_s_y / cm <<" cm \t"<<pos_s_z[i] / cm <<" cm"<<G4endl; 
    }

  //-----------------------------------------ABSORBER--------------------------------------------
  G4double x_a = fN_block_absorber_x*fL_block;//cm
  // watch out if detector sizes are modified, modify also DCCROOTuple.cc when adding noise to data when checking the data+nois are not outside the detectors
  G4double y_a = fN_block_absorber_y*fL_block;//cm
  G4double z_a = fZ_block;//cm
  G4VisAttributes* Abs_vis = new G4VisAttributes(1);
  Abs_vis->SetColor(color.Cyan());
  Abs_vis->SetVisibility(true);
  Abs_vis->SetForceSolid(true);

  G4Box *solidAbsorber = new G4Box("Absorber",x_a / 2,y_a / 2,z_a / 2);
  logicAbsorber = new G4LogicalVolume(solidAbsorber,BGO,"logAbsorber",0,0,0,true);
  logicAbsorber->SetVisAttributes(Abs_vis);
  G4cout<<"The absorber detector is "<<x_a / cm <<" cm \t"<<y_a / cm <<" cm \t"<<z_a / cm  <<" cm"<<G4endl; 
  
  G4double pos_a_x = 0.0; //it is possible to modify the absorber position with respect to the scatterer detector also in the transverse plane
  G4double pos_a_y = 0.0;
  G4double pos_a_z = -(pos_p_z+fD0+(fN_Scatt-1)*fD1+fD2);
  physAbsorber = new G4PVPlacement(0,G4ThreeVector(pos_a_x,pos_a_y,pos_a_z),logicAbsorber,"physAbsorber",logicWorld,false,0,fCheckOverlaps);
  G4cout<<"The absorber is in position "<<pos_a_x / cm <<" cm \t"<<pos_a_y / cm <<" cm \t"<<pos_a_z / cm <<" cm"<<G4endl; 
  // Print materials
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;

  //always return the physical World
  //
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructSDandField()
{
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  SDman->SetVerboseLevel(1);

  //Declaration of scatterer planes as SD
  G4String scattererSDname;
  scattererSD = new ScattererSD*[fN_Scatt];
  for(G4int i = 0; i < fN_Scatt; i++){
    std::ostringstream obuffer;
    obuffer<<(1+(G4double)i);
    scattererSDname="scatterer"+obuffer.str();
    scattererSD[i] = new ScattererSD(scattererSDname);
    SDman->AddNewDetector(scattererSD[i]);
    logicScatterer[i]->SetSensitiveDetector(scattererSD[i]);
  }

  //Declaration of the absorber as SD
  G4String absorberSDname = "absorber";
  absorberSD = new AbsorberSD(absorberSDname);
  SDman->AddNewDetector(absorberSD);
  logicAbsorber->SetSensitiveDetector(absorberSD);
  
}


void DetectorConstruction::UpdateGeometry()
{ 
  /* G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();


  Construct();
  ConstructSDandField();
  */
  
  G4RunManager* theRunManager = G4RunManager::GetRunManager();
  theRunManager->DefineWorldVolume(physWorld);
  theRunManager->GeometryHasBeenModified();
  theRunManager->ReinitializeGeometry(true);
  
}


void DetectorConstruction::Setd0(G4double newD0)
{
  fD0 = newD0;
}

void DetectorConstruction::Setd1(G4double newD1)
{
  fD1 = newD1;
}

void DetectorConstruction::Setd2(G4double newD2)
{
  fD2 = newD2;
}

void DetectorConstruction::SetX_scatt(G4double newX_scatt)
{
  fX_scatt = newX_scatt;
}

void DetectorConstruction::SetY_scatt(G4double newY_scatt)
{
  fY_scatt = newY_scatt;
}

void DetectorConstruction::SetZ_scatt(G4double newZ_scatt)
{
  fZ_scatt = newZ_scatt;
}

void DetectorConstruction::SetL_block(G4double newL_block)
{
  fL_block = newL_block;
}

void DetectorConstruction::SetZ_block(G4double newZ_block)
{
  fZ_block = newZ_block;
}

void DetectorConstruction::SetN_block_x(G4int newN_block_x)
{
  fN_block_absorber_x = newN_block_x;
}

void DetectorConstruction::SetN_block_y(G4int newN_block_y)
{
  fN_block_absorber_y = newN_block_y;
}

void DetectorConstruction::SetN_scatt(G4int newN_scatt)
{
  fN_Scatt = newN_scatt;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
