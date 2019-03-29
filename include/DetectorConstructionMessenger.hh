#ifndef DetectorConstructionMessenger_h
#define DetectorConstructionMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class G4UIcommand;
class G4UIdirectory;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAnInteger;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWith3Vector;
class G4UIcmdWith3VectorAndUnit;

class DetectorConstruction;

class DetectorConstructionMessenger: public G4UImessenger
{
  public:
    DetectorConstructionMessenger(DetectorConstruction*);
    virtual ~DetectorConstructionMessenger();

  public:
    void SetNewValue(G4UIcommand *command, G4String newValues);
    G4String GetCurrentValue(G4UIcommand * command);

  private:
    DetectorConstruction* fGeometry;
  

  private: //commands
    G4UIdirectory *             scattererDirectory;
    G4UIdirectory *             absorberDirectory;
    G4UIdirectory *             geometryDirectory;
  //G4UIcmdWithAString *        ScattMaterialCmd;
  //G4UIcmdWithAString *        AbsMaterialCmd;
    G4UIcmdWithADoubleAndUnit * d0Cmd;
    G4UIcmdWithADoubleAndUnit * d1Cmd;
    G4UIcmdWithADoubleAndUnit * d2Cmd;
    G4UIcmdWithADoubleAndUnit * x_scattCmd;
    G4UIcmdWithADoubleAndUnit * y_scattCmd;
    G4UIcmdWithADoubleAndUnit * z_scattCmd;
    G4UIcmdWithAnInteger * n_block_xCmd;
    G4UIcmdWithAnInteger * n_block_yCmd; 
    G4UIcmdWithAnInteger * n_scattCmd;
    G4UIcmdWithADoubleAndUnit * l_blockCmd;
    G4UIcmdWithADoubleAndUnit * z_blockCmd;

    G4UIcmdWithoutParameter * updateGeomCmd; 
 
  //G4UIcmdWith3VectorAndUnit * ScattPositionCmd;
  //G4UIcmdWith3VectorAndUnit * AbsPositionCmd; 
 
};

#endif
