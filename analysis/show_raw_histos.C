#include "Riostream.h"
#include <string>
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TNtuple.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TF1.h"
#include "TGraph.h"
#include "TROOT.h"
#include "TString.h"

using namespace std;

typedef struct{
  double x,y,z,edep,time;
  //string int_name, det_name;
  int part_ID, evnt_ID;
} HIT;


void show_raw_histos(TString rootfilename)
{
  //GLOBAL ROOT SETTINGS

 gROOT->Reset();
 gROOT->SetStyle("Plain");
 gROOT->ForceStyle();

 //GLOBAL HISTO OPTIONS

 gStyle -> SetStatW(0.28);
 gStyle -> SetStatH(0.13);
 gStyle -> SetStatColor(0);
 gStyle -> SetStatX(0.87);
 gStyle -> SetStatY(0.85);
 gStyle -> SetStatFont(0);
 gStyle -> SetOptStat(111);
 gStyle -> SetPalette(1);

 Int_t N_primaries = 20;
 Int_t N_evt = 0;

 //----------------------SIMULATION GEOMETRY-----------------//
 const Int_t N_scatt = 7;
 //----------------------------------------------------------//
 

 static HIT sc_hit;
 static HIT ab_hit;
 std::string  *scatt_process = 0;
 std::string *scatt_plane=0;
 std::string  *abs_process = 0;
 std::string *abs_name=0;

 TFile *in_file = TFile::Open(rootfilename);


 TTree *scattererData;
 in_file->GetObject("Scatterer_base_tree", scattererData);
 TTree *absorberData;
 in_file->GetObject("Absorber_base_tree", absorberData);

 TBranch *sp_branch = 0;
 TBranch *sv_branch = 0;

 TBranch *ap_branch = 0;
 TBranch *av_branch = 0;

 scattererData->SetBranchAddress("scatt_raw_hits", &sc_hit.x);
 scattererData->SetBranchAddress("scatt_raw_hits_int_name", &scatt_process, &sp_branch);
 scattererData->SetBranchAddress("scatt_raw_hits_vol_name", &scatt_plane, &sv_branch);

 absorberData->SetBranchAddress("abs_raw_hits", &ab_hit.x);
 absorberData->SetBranchAddress("abs_raw_hits_int_name", &abs_process, &ap_branch);
 absorberData->SetBranchAddress("abs_raw_hits_vol_name", &abs_name, &av_branch);
  
 cout<<"Branch data type: "<<endl;
 cout<<scattererData->GetBranch("scatt_raw_hits_int_name")->GetClassName()<<endl;

 //---------------------------HISTOGRAMS----------------------------//
 //Impact positions in scatterer and absorber
 TH1F* impact_distr_scatt[N_scatt][3];
 TH1F* impact_distr_abs[3];
 
  for(Int_t xyz = 0; xyz<3; xyz++){	
    if(xyz==0)impact_distr_abs[xyz] = new TH1F("absorber_X", "absorber_X", 280,-140, 140);
    if(xyz==1)impact_distr_abs[xyz] = new TH1F("absorber_Y", "absorber_Y", 190,-95, 95);
    if(xyz==2)impact_distr_abs[xyz] = new TH1F("absorber_Z", "absorber_Z", 35,-(99+10*(N_scatt-1)+132.5),-(99+10*(N_scatt-1)+167.5));
    for(Int_t hh = 0; hh<N_scatt; hh++){
      if(xyz==0)impact_distr_scatt[hh][xyz] = new TH1F(Form("plane %d_X", hh + 1), Form("plane %d_X", hh + 1), 90, -45, 45);
      if(xyz==1)impact_distr_scatt[hh][xyz] = new TH1F(Form("plane %d_Y", hh + 1), Form("plane %d_Y", hh + 1), 90, -45, 45);
      if(xyz==2)impact_distr_scatt[hh][xyz] = new TH1F(Form("plane %d_Z", hh + 1), Form("plane %d_Z", hh + 1), 20, -(99+hh*12), -(101+hh*12));
    }
  } 

  //Energy deposited and time in scatterer and absorber
  TH1F* edep_scatt[N_scatt];
  TH1F* edep_abs;
  TH1F* time_scatt[N_scatt];
  TH1F* time_abs;

  for(Int_t ed = 0; ed<N_scatt; ed++){
    edep_scatt[ed]= new TH1F(Form("edep scatt_plane %d", ed+1), Form("edep scatt_plane %d", ed+1), 100, 0., 1000.);
    edep_scatt[ed]->GetXaxis()->SetTitle("Edep (keV)");
    edep_scatt[ed]->GetYaxis()->SetTitle("#");
    
    time_scatt[ed]= new TH1F(Form("time scatt_plane %d", ed+1), Form("time scatt_plane %d", ed+1), 100, 0., 10.);
    time_scatt[ed]->GetXaxis()->SetTitle("time (ns)");
    time_scatt[ed]->GetYaxis()->SetTitle("#");
  }
  
  edep_abs = new TH1F("edep abs", "edep abs", 100, 0., 1000.);
  edep_abs->GetXaxis()->SetTitle("Edep (keV)");
  edep_abs->GetYaxis()->SetTitle("#");
  
  time_abs= new TH1F("time abs","time abs", 100, 0., 50.);
  time_abs->GetXaxis()->SetTitle("time (ns)");
  time_abs->GetYaxis()->SetTitle("#");
 
  

 //Analysis scatterer hits
 Int_t scatt_entries = scattererData->GetEntries();
 int planeN = 0;
 char pN_c;
 for(Int_t s_ev = 0; s_ev<scatt_entries; s_ev++)
   {
     planeN = 0;
     scattererData->GetEntry(s_ev);
     if(sc_hit.evnt_ID != N_evt){
       N_evt = sc_hit.evnt_ID; 
       cout<<"Start new event: analysis event number  "<<N_evt<<endl;
     }
     pN_c = scatt_plane->at(scatt_plane->length() - 1);
     planeN = pN_c - '0';
     planeN-=1;
     edep_scatt[planeN]->Fill(sc_hit.edep);
     impact_distr_scatt[planeN][0]->Fill(sc_hit.x);
     impact_distr_scatt[planeN][1]->Fill(sc_hit.y);
     impact_distr_scatt[planeN][2]->Fill(sc_hit.z);
     time_scatt[planeN]->Fill(sc_hit.time);
   }
 
 //Analysis absorber hits
 Int_t abs_entries = absorberData->GetEntries();


 TFile *out_file = new TFile("output_analysis.root", "RECREATE");
 for(Int_t pl = 0; pl<N_scatt; pl++)
   {
     edep_scatt[pl]->Write(Form("edep plane %d", pl+1));
     impact_distr_scatt[pl][0]->Write(Form("X plane %d", pl+1));
     impact_distr_scatt[pl][1]->Write(Form("Y plane %d", pl+1));
     impact_distr_scatt[pl][2]->Write(Form("Z plane %d", pl+1));
     time_scatt[pl]->Write(Form("time plane %d", pl+1));
     
   }
 

 cout<<"Entries in the scatterer detector "<<scatt_entries<<endl;
 cout<<"Entries in the absorber detector "<<abs_entries<<endl;

 out_file->Close();

 return;
}
