// Basic C++ classes
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <iomanip>
#include <regex>
#include <cmath>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>

// Select a proper single-/multithreaded run manager
#ifdef G4MULTITHREADED
    #include <G4MTRunManager.hh>
    using RunManager = G4MTRunManager;
#else
    #include <G4RunManager.hh>
    using RunManager = G4RunManager;
#endif

//Visualization modules
#ifdef G4VIS_USE
    #include <G4VisExecutive.hh>
#endif

#ifdef G4UI_USE
    #include <G4UIExecutive.hh>
#endif

//G4 classes
#include <G4String.hh>
#include <G4UImanager.hh>
#include <Randomize.hh>

//Simulation user defined classes
#include "RunAction.hh"
#include "SteppingAction.hh"
#include "TrackingAction.hh"
//#include "StackingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "PhysicsList.hh"
#include "ROOTuple.hh"

//Physics classes for built-in physics lists
#include "QGSP_BERT.hh"

using namespace std;

// External functions

//------------------------------read configuration file name.ini with regular expressions and fix simulation parameters (geometry, energy, resolutions, etc.)----------------------//
void simu_config(ifstream& myfile, vector <string> var, map<string, double>& conf, bool& error)
  {
    vector<regex> re;
    unsigned int N_par = var.size();
    vector <string> res_temp;// = new string[N_par];
    for(unsigned int x = 0; x < N_par; x++){
      re.push_back(regex(".*"+var.at(x)+".* = (\\d*.*)"));
    }
    smatch match;
    vector <string> lines;
    string line;
    if(myfile.is_open()){
      cout <<"CONFIGURATION FILE OPEN " <<endl;
        while(getline(myfile, line))
	  {
	    lines.push_back(line);
	  }
    }else{
      cout<<"PROBLEM OPENING CONFIGURATION FILE"<<endl;
      error = !error;
      return;
    }
    string temp;
    for(unsigned int ll = 0; ll<lines.size(); ll++){
      for(unsigned int mt = 0; mt < re.size(); mt++){
	if(regex_search (lines.at(ll), match, re.at(mt))){
	  temp = match[1];
	  res_temp.push_back(temp);
	  conf[var[mt]] = atof(temp.c_str());
	  //re.erase(re.begin()+mt);
	  break;
	}
      }
    }
    
    if(res_temp.size() != N_par){
      error = !error;
      cout<<N_par- res_temp.size()<<" configuration variable(s) not found! Check variable list and configuration file ..."<<endl;
      return;}
  }


/* Main function that enables to:
 * - run any number of macros (put them as command-line arguments)
 * - start interactive UI mode (no arguments or "-i")

Example of execution 
./Comptoncamera -m macroname.mac -c confname.ini 

 */
int main(int argc, char** argv)
{
  std::cout << "Application starting..." << std::endl;
  
  //choose the Random engine
  G4Random::setTheEngine(new CLHEP::RanecuEngine);
  //Random generator configuration
  timeval tim;
  gettimeofday(&tim, NULL);
  long seeds[2];
  //seeds[0] = tim.tv_usec;
  seeds[0] = 12345;
  seeds[1] = 1;
  long index = 2;
  CLHEP::HepRandom::setTheSeeds(seeds, index);
  const G4long* table_entry;
  table_entry = CLHEP::HepRandom::getTheSeeds();
  cout<<"RANDOM GENERATOR CORRECTLY SET UP with seed "<<seeds[0]<<endl;
  cout<<table_entry<<endl;
  

  vector<G4String> macros;
  bool interactive = false;
  //Configuration file name
  string conf_filename;

 // Parse command line arguments
  if(argc == 1)
    {//start interactive mode if no arguments
     interactive = true;
   }
 else if(argc == 2)
   {//if only 1 argument, it should be the name of a macro
     macros.push_back(argv[1]);
   }
 else
   {//argument treatment
     for (int i = 1; i < argc; i++)
       {
	 G4String arg = argv[i];
	 if (arg == "-i" || arg == "--interactive")
	   {//start interactive mode if argument -i is specified
	     interactive = true;
	     continue;
	   }
	 else if(arg == "-c" || arg == "--conf")
	   {//select configuration file with -c or --conf
	     conf_filename = argv[i+1];
	   }
	 else if(arg == "-m" || arg == "--macro")
	   {//select command macro file with -m or --macro
	     macros.push_back(argv[i+1]);
	   }
	 
       }
   }

  auto runManager = new RunManager();

 //-------------------------SIMULATION GEOMETRY------------------------------------
  //-------------------------used in DetectorConstruction--------------------------
  //d0 distance beam/source-first scatterer plane center [cm]
  //d1 distance between scatterer plane centers [cm]
  //d2 distance last scatterer plane center-absorber center [cm]
  //x_scatt scatterer x side [cm]
  //y_scatt scatterer y side [cm]
  //z_scatt scatterer thickness along particle direction [cm]
  //n_Scatt scatterer planes in stack
  //l_block absorber block size [cm]
  //z_block absorber block thickness [cm]
  //n_block_absorber_x blocks in the absorber in x direction [#]
  //n_block_absorber_y blocks in the absorber in y direction [#]
  //-----------------------------------------------------
  
  //Default parameters
  G4String verbose = "0";

  //Read the configuration file for the simulation geometry
  bool error = false;
  vector <string> var = {"d0", "d1", "d2", "x_scatt", "y_scatt", "z_scatt", "n_block_absorber_x", "n_block_absorber_y", "l_block", "z_block", "n_scatt", "energy"};//vector of parameters to be read from file
  map<string, double> conf;
  ifstream conf_file;
  //string conf_filename;
  if(argc>1){//in batch mode
    conf_file.open("./conf_files/"+conf_filename);
  }
  else{//in interactive mode
    cout<<"Type the configuration file name (with path and extension):"<<endl;
    cin>>conf_filename;
    conf_file.open("./conf_files/"+conf_filename);
  }
  simu_config(conf_file, var, conf, error);
  if(error){cout<<"Configuration problem!"<<endl;
    return 0;
  }else{
    //print parameters for check
    for(unsigned int s = 0; s<var.size(); s++){
      cout<<var[s]<<"\t";
      cout << setiosflags(ios::fixed | ios::showpoint)
	   << setprecision(2)
	   <<conf[var[s]]<<endl;
    }
    cout<<"Simulation configured ... "<<endl;
  }
  //end configuration
  
#ifdef G4VIS_USE
  G4VisManager* visManager = new G4VisExecutive();
  visManager->SetVerboseLevel(0);    // Default, you can always override this using macro commands
  visManager->Initialize();
#endif
  

  //PHYSICS LIST
  runManager->SetUserInitialization(new PhysicsList());	
  //G4VUserPhysicsList* physics = new QGSP_BERT();
  //runManager->SetUserInitialization(physics);
  cout<<"Physics list OK!"<<endl;
  //DETECTOR GEOMETRY
  DetectorConstruction* geometry = new DetectorConstruction(conf[var[0]],conf[var[1]],conf[var[2]],conf[var[3]],conf[var[4]],conf[var[5]],conf[var[6]],conf[var[7]],conf[var[8]],conf[var[9]],conf[var[10]]);
  //DetectorConstruction* geometry = new DetectorConstruction(10., 1., 15., 9., 9., 0.2, 10, 10, 3.5, 3., 7);
  runManager->SetUserInitialization(geometry);
  cout<<"Detector geometry OK!"<<endl;
  //PRIMARY GENERATOR
  PrimaryGeneratorAction* primary = new PrimaryGeneratorAction(conf[var[11]], "psi_lim");//choose emission direction ("iso" 4PI, "psi_lim" acceptance cone of first scatterer plane)
  //PrimaryGeneratorAction* primary = new PrimaryGeneratorAction(500 ,"psi_lim");
  runManager->SetUserAction(primary);
  cout<<"Primary generator OK!"<<endl;
  // runManager->SetUserInitialization(new ActionInitialization());
  //RECORDER->ROOTUPLE
  G4String root_output = "outroot.root";
  RecorderBase* records = new ROOTuple(root_output);
  cout<<"Recorder OK!"<<endl;
   
  //Action initialization
  //RUN ACTION
  RunAction* theRunAction = new RunAction(records);
  runManager->SetUserAction(theRunAction);
  //EVENT ACTION
  runManager->SetUserAction(new EventAction(records, verbose, conf[var[10]]));
  //runManager->SetUserAction(new EventAction(records, verbose, 7));
  //STEPPING ACTION
  runManager->SetUserAction(new SteppingAction(records));
  //STACKING ACTION
  //runManager->SetUserAction(new StackingAction(theRunAction));
  //TRACKING ACTION
  runManager->SetUserAction(new TrackingAction(records)); 
  cout<<"All user action objects OK!"<<endl;
  

 #ifdef G4UI_USE
  G4UIExecutive* ui;
  if (interactive)
    {
      ui = new G4UIExecutive(argc, argv);
    }
 #endif
  
  G4UImanager* UImanager = G4UImanager::GetUIpointer();
  
  if(macros.size()!=0){
    for (auto macro : macros)
      {
	G4String command = "/control/execute ./macros/";
	UImanager->ApplyCommand(command + macro);
      }
  }

 #ifdef G4UI_USE
  if (interactive)
    {//in case of interactive session, configure the session with the macro vis.mac
      UImanager->ApplyCommand("/control/execute ./macros/vis.mac");
      ui->SessionStart();
      delete ui;
    }
 #endif

  delete runManager;

  std::cout << "Application successfully ended.\nBye :-)" << std::endl;
  
  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
