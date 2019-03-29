#include "DetectorConstructionMessenger.hh"
#include "DetectorConstruction.hh"
#include "G4ThreeVector.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3Vector.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include <iostream>


DetectorConstructionMessenger::DetectorConstructionMessenger(DetectorConstruction *geometry)
: G4UImessenger(),
  fGeometry(geometry),
  scattererDirectory(0),
  absorberDirectory(0),
  geometryDirectory(0),
  d0Cmd(0),
  d1Cmd(0),
  d2Cmd(0),
  x_scattCmd(0),
  y_scattCmd(0),
  z_scattCmd(0),
  n_block_xCmd(0),
  n_block_yCmd(0),
  n_scattCmd(0),
  l_blockCmd(0),
  z_blockCmd(0),
  updateGeomCmd(0)
{
  //Scatterer directory
  scattererDirectory = new G4UIdirectory("/scatt/");
  scattererDirectory->SetGuidance("Scatterer geometry control commands.");
  //Absorber directory
  absorberDirectory = new G4UIdirectory("/abs/");
  absorberDirectory->SetGuidance("Absorber geometry control commands.");
  //Geometry directory - for overall geometric parameters
  geometryDirectory = new G4UIdirectory("/geom/");
  geometryDirectory->SetGuidance("Overall geometry control commands.");

  
  /*  //Scatterer material
  ScattMaterialCmd = new G4UIcmdWithAString("/scatt/material",this);
  ScattMaterialCmd->SetGuidance("Set scatterer material -  (Silicium, CdTe, CZT)."); //to be updated in case of new materials
  ScattMaterialCmd->SetGuidance(" (Silicium is default)");
  ScattMaterialCmd->SetParameterName("scattMaterial",true);
  ScattMaterialCmd->SetDefaultValue("Silicium");
  ScattMaterialCmd->SetCandidates("Silicium CdTe CZT "); //to be updated in case of new materials
  //Absorber material
  AbsMaterialCmd = new G4UIcmdWithAString("/abs/material",this);
  AbsMaterialCmd->SetGuidance("Set absorber material -  (LYSO, BGO)."); //to be updated in case of new materials
  AbsMaterialCmd->SetGuidance(" (BGO is default)");
  AbsMaterialCmd->SetParameterName("absMaterial",true);
  AbsMaterialCmd->SetDefaultValue("BGO");
  AbsMaterialCmd->SetCandidates("LYSO BGO "); //to be updated in case of new materials
  */
  //Intra-detector distances - in /geom/
  //d0 - > distance beam/source - first scatterer plane
  d0Cmd = new G4UIcmdWithADoubleAndUnit("/geom/d0",this);
  d0Cmd->SetGuidance("Set distance beam axis/source - first scatterer plane (center).");
  d0Cmd->SetParameterName("d0",true,true);
  d0Cmd->SetDefaultUnit("cm");
  d0Cmd->SetUnitCandidates("mm cm m");
 
   //d1 - > distance between next scatterer planes (center to center)
  d1Cmd = new G4UIcmdWithADoubleAndUnit("/geom/d1",this);
  d1Cmd->SetGuidance("Set distance between next scatterer planes (center to center).");
  d1Cmd->SetParameterName("d1",true,true);
  d1Cmd->SetDefaultUnit("cm");
  d1Cmd->SetUnitCandidates("mm cm m");

   //d2 - > distance last scatterer plane - absorber (center to center)
  d2Cmd = new G4UIcmdWithADoubleAndUnit("/geom/d2",this);
  d2Cmd->SetGuidance("Set distance last scatterer plane - absorber (center to center).");
  d2Cmd->SetParameterName("d2",true,true);
  d2Cmd->SetDefaultUnit("cm");
  d2Cmd->SetUnitCandidates("mm cm m");
  
  //Detector sizes and configuration
  //Scatterer plane x side - in /scatt/
  x_scattCmd = new G4UIcmdWithADoubleAndUnit("/scatt/x_scatt",this);
  x_scattCmd->SetGuidance("Set x size of scatterer planes.");
  x_scattCmd->SetParameterName("x_scatt",true,true);
  x_scattCmd->SetDefaultUnit("cm");
  x_scattCmd->SetUnitCandidates("micron mm cm");

  //Scatterer plane y side - in /scatt/
  y_scattCmd = new G4UIcmdWithADoubleAndUnit("/scatt/y_scatt",this);
  y_scattCmd->SetGuidance("Set y size of scatterer planes.");
  y_scattCmd->SetParameterName("y_scatt",true,true);
  y_scattCmd->SetDefaultUnit("cm");
  y_scattCmd->SetUnitCandidates("micron mm cm");

  //Scatterer plane thickness - in /scatt/
  z_scattCmd = new G4UIcmdWithADoubleAndUnit("/scatt/z_scatt",this);
  z_scattCmd->SetGuidance("Set thickness of scatterer planes.");
  z_scattCmd->SetParameterName("z_scatt",true,true);
  z_scattCmd->SetDefaultUnit("cm");
  z_scattCmd->SetUnitCandidates("micron mm cm");
  
  //Absorber block thickness - in /abs/
  z_blockCmd = new G4UIcmdWithADoubleAndUnit("/abs/z_block",this);
  z_blockCmd->SetGuidance("Set thickness of absorber blocks.");
  z_blockCmd->SetParameterName("z_block",true,true);
  z_blockCmd->SetDefaultUnit("cm");
  z_blockCmd->SetUnitCandidates("micron mm cm");
  
  //Absorber block side - in /abs/
  l_blockCmd = new G4UIcmdWithADoubleAndUnit("/abs/l_block",this);
  l_blockCmd->SetGuidance("Set size of absorber blocks.");
  l_blockCmd->SetParameterName("l_block",true,true);
  l_blockCmd->SetDefaultUnit("cm");
  l_blockCmd->SetUnitCandidates("micron mm cm");
  
  //Number of absorber blocks in x direction - in /abs/
  n_block_xCmd = new G4UIcmdWithAnInteger("/abs/n_block_x",this);
  n_block_xCmd->SetGuidance("Set number of absorber blocks in x direction.");
  n_block_xCmd->SetParameterName("n_block_x",true,true);
 
  //Number of absorber blocks in y direction - in /abs/
  n_block_yCmd = new G4UIcmdWithAnInteger("/abs/n_block_y",this);
  n_block_yCmd->SetGuidance("Set number of absorber blocks in y direction.");
  n_block_yCmd->SetParameterName("n_block_y",true,true);
 
  //Number of scatterer planes - in /scatt/
  n_scattCmd = new G4UIcmdWithAnInteger("/scatt/n_scatt",this);
  n_scattCmd->SetGuidance("Set number of scatterer planes.");
  n_scattCmd->SetParameterName("n_scatt",true,true);


  //Geometry update after changements
  updateGeomCmd = new G4UIcmdWithoutParameter("/geom/updateGeom",this);
  updateGeomCmd->SetGuidance("Reinitialize geometry after changements.");
}

DetectorConstructionMessenger::~DetectorConstructionMessenger()
{
  delete scattererDirectory;
  delete absorberDirectory;
  delete geometryDirectory;
  //delete ScattMaterialCmd;
  //delete AbsorberMaterialCmd;
  delete d0Cmd;
  delete d1Cmd;
  delete d2Cmd;
  delete x_scattCmd;
  delete y_scattCmd;
  delete z_scattCmd;
  delete n_block_xCmd;
  delete n_block_yCmd;
  delete l_blockCmd;
  delete n_scattCmd;
  delete updateGeomCmd;
}

void DetectorConstructionMessenger::SetNewValue(G4UIcommand * command,G4String newValues)
{
  /* if( command==ScattMaterialCmd )
    { geometry->SetScattMaterial(ScattMaterialCmd->GetNewStringValue(newValues)); }
  else if( command==AbsMaterialCmd )
    {geometry->SetAbsMaterial(AbsMaterialCmd->GetNewStringValue(newValues)); }
    else*/
  if( command==d0Cmd )
    {fGeometry->Setd0(d0Cmd->GetNewDoubleValue(newValues)); }
  else if( command==d1Cmd )
    {fGeometry->Setd1(d1Cmd->GetNewDoubleValue(newValues)); }
  else if( command==d2Cmd )
    {fGeometry->Setd2(d2Cmd->GetNewDoubleValue(newValues)); }
  else if( command==x_scattCmd )
    {fGeometry->SetX_scatt(x_scattCmd->GetNewDoubleValue(newValues)); }
  else if( command==y_scattCmd )
    {fGeometry->SetY_scatt(y_scattCmd->GetNewDoubleValue(newValues)); }
  else if( command==z_scattCmd )
    {fGeometry->SetZ_scatt(z_scattCmd->GetNewDoubleValue(newValues)); }
  else if( command==l_blockCmd )
    {fGeometry->SetL_block(l_blockCmd->GetNewDoubleValue(newValues)); }
  else if( command==z_blockCmd )
    {fGeometry->SetZ_block(z_blockCmd->GetNewDoubleValue(newValues)); }
  else if( command==n_block_xCmd )
    {fGeometry->SetN_block_x(n_block_xCmd->GetNewIntValue(newValues)); }
  else if( command==n_block_yCmd )
    {fGeometry->SetN_block_y(n_block_yCmd->GetNewIntValue(newValues)); }
  else if( command==n_scattCmd )
    {fGeometry->SetN_scatt(n_scattCmd->GetNewIntValue(newValues)); }
  else if( command==updateGeomCmd )
    {fGeometry->UpdateGeometry(); }
  else {G4cout<< "Command not valid! "<<G4endl; }
}

G4String DetectorConstructionMessenger::GetCurrentValue(G4UIcommand * command)
{
  G4String cv;

  if( command==d0Cmd )
    {cv = d0Cmd->ConvertToString(fGeometry->Getd0(), "cm"); }
  else if( command==d1Cmd )
    {cv = d1Cmd->ConvertToString(fGeometry->Getd1(), "cm"); }
  else if( command==d2Cmd )
    {cv = d2Cmd->ConvertToString(fGeometry->Getd2(), "cm"); }
  else if( command==x_scattCmd )
    {cv = x_scattCmd->ConvertToString(fGeometry->GetX_scatt(), "cm"); }
  else if( command==y_scattCmd )
    {cv = y_scattCmd->ConvertToString(fGeometry->GetY_scatt(), "cm");  }
  else if( command==z_scattCmd )
    {cv = z_scattCmd->ConvertToString(fGeometry->GetZ_scatt(), "cm");  }
  else if( command==l_blockCmd )
    {cv = l_blockCmd->ConvertToString(fGeometry->GetL_block(), "cm");  }
  else if( command==z_blockCmd )
    {cv = z_blockCmd->ConvertToString(fGeometry->GetZ_block(), "cm");  }
  else if( command==n_block_xCmd )
    {cv = n_block_xCmd->ConvertToString(fGeometry->GetN_block_x());  }
  else if( command==n_block_yCmd )
    {cv = n_block_yCmd->ConvertToString(fGeometry->GetN_block_y()); }
  else if( command==n_scattCmd )
    {cv = n_scattCmd->ConvertToString(fGeometry->GetN_scatt());  }
  
    return cv;
}

