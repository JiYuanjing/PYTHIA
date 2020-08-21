#include <vector>
// #include "Pythia.h"
#include "Pythia8/Pythia.h"
#include "TTree.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TH3D.h"
//#include "Hists.h"
#include "Anacuts.h"
#define DEBUG 0
using namespace Pythia8; 

int main(int argc, char* argv[]) {

  if (argc != 5) {
    cout << "Usage: " << argv[0] << " runcard  rootfile xmldoc mode" << endl;
    return 2;
  }
  char* runcard  = argv[1];
  char* rootfile = argv[2];
  const string xmlDB = argv[3];
  const int mode = atoi(argv[4]);
  //--------------------------------------------------------------
  //  Initialization
  //--------------------------------------------------------------

  time_t now1 = time(0);
  cout << "============================================================================" << endl;
  cout << "Executing program '" << argv[0] << "', start at: " << ctime(&now1);
  cout << "Arguments: " << argv[1] << " " << argv[2]<<" " <<argv[3]<<" "<<argv[4]<< endl;
  cout << "============================================================================" << endl;

  //
  //  Create instance of Pythia 
  //
  cout <<xmlDB<<endl;
  Pythia pythia(xmlDB.c_str()); // the init parameters are read from xml files
  // stored in the xmldoc directory. This includes
  // particle data and decay definitions.
  cout <<"init pythia"<<endl; 
  //
  // Shorthand for (static) settings
  //
  Settings& settings = pythia.settings;

  //
  //  Read in runcard
  //
  pythia.readFile(runcard);  
  cout << "Runcard '" << runcard << "' loaded." << endl;
  // //alice setting
  // pythia.readString("HardQCD:gg2ccbar = on"); // For test 
  // pythia.readString("HardQCD:qqbar2ccbar = on"); // For test 
  // pythia.readString("Tune:pp = 14"); // Monash 2013
  // if(mode==1){
  //   pythia.readString("StringPT:sigma = 0.335");
  //   pythia.readString("StringZ:aLund = 0.36");
  //   pythia.readString("StringZ:bLund = 0.56");
  //   pythia.readString("StringFlav:probQQtoQ = 0.078");
  //   pythia.readString("StringFlav:probStoUD = 0.2");
  //   pythia.readString("StringFlav:probQQ1toQQ0join = 0.0275,0.0275,0.0275,0.0275");
  //   pythia.readString("MultiPartonInteractions:pT0Ref = 2.12");
  //   pythia.readString("BeamRemnants:remnantMode = 1");
  //   pythia.readString("BeamRemnants:saturation = 5");
  //   pythia.readString("ColourReconnection:mode = 1");
  //   pythia.readString("ColourReconnection:allowDoubleJunRem = off");
  //   pythia.readString("ColourReconnection:m0 = 2.9");
  //   pythia.readString("ColourReconnection:allowJunctions = on");
  //   pythia.readString("ColourReconnection:junctionCorrection = 1.43");
  //   pythia.readString("ColourReconnection:timeDilationMode = 0");
  //   //pythia.readString("ColourReconnection:timeDilationPar = 0");
  // }else if(mode==2){
  //   pythia.readString("StringPT:sigma = 0.335");
  //   pythia.readString("StringZ:aLund = 0.36");
  //   pythia.readString("StringZ:bLund = 0.56");
  //   pythia.readString("StringFlav:probQQtoQ = 0.078");
  //   pythia.readString("StringFlav:probStoUD = 0.2");
  //   pythia.readString("StringFlav:probQQ1toQQ0join = 0.0275,0.0275,0.0275,0.0275");
  //   pythia.readString("MultiPartonInteractions:pT0Ref = 2.15");
  //   pythia.readString("BeamRemnants:remnantMode = 1");
  //   pythia.readString("BeamRemnants:saturation = 5");
  //   pythia.readString("ColourReconnection:mode = 1");
  //   pythia.readString("ColourReconnection:allowDoubleJunRem = off");
  //   pythia.readString("ColourReconnection:m0 = 0.3");
  //   pythia.readString("ColourReconnection:allowJunctions = on");
  //   pythia.readString("ColourReconnection:junctionCorrection = 1.20");
  //   pythia.readString("ColourReconnection:timeDilationMode = 2");
  //   pythia.readString("ColourReconnection:timeDilationPar = 0.18");
  // }else if(mode==3){
  //   pythia.readString("StringPT:sigma = 0.335");
  //   pythia.readString("StringZ:aLund = 0.36");
  //   pythia.readString("StringZ:bLund = 0.56");
  //   pythia.readString("StringFlav:probQQtoQ = 0.078");
  //   pythia.readString("StringFlav:probStoUD = 0.2");
  //   pythia.readString("StringFlav:probQQ1toQQ0join = 0.0275,0.0275,0.0275,0.0275");
  //   pythia.readString("MultiPartonInteractions:pT0Ref = 2.05");
  //   pythia.readString("BeamRemnants:remnantMode = 1");
  //   pythia.readString("BeamRemnants:saturation = 5");
  //   pythia.readString("ColourReconnection:mode = 1");
  //   pythia.readString("ColourReconnection:allowDoubleJunRem = off");
  //   pythia.readString("ColourReconnection:m0 = 0.3");
  //   pythia.readString("ColourReconnection:allowJunctions = on");
  //   pythia.readString("ColourReconnection:junctionCorrection = 1.15");
  //   pythia.readString("ColourReconnection:timeDilationMode = 3");
  //   pythia.readString("ColourReconnection:timeDilationPar = 0.073");
  // }
  //
  //
  //  Retrieve number of events and other parameters from the runcard.
  //  We need to deal with those settings ourself. Getting
  //  them through the runcard just avoids recompiling.
  //
  int  maxNumberOfEvents = settings.mode("Main:numberOfEvents");
  int  maxErrors = settings.mode("Main:timesAllowErrors");
  bool showCS    = settings.flag("Init:showChangedSettings");
  bool showAS    = settings.flag("Init:showAllSettings");

  //  Initialize Pythia, ready to go
  pythia.init();
  // List changed or all data
  //
  if (showCS) settings.listChanged();
  if (showAS) settings.listAll();

  //--------------------------------------------------------------
  //  Event loop
  //--------------------------------------------------------------

  int iErrors = 0;
  double pt;
  double eta;
  double y;

  InitHists(); 

  for(int ievent = 0;ievent < maxNumberOfEvents;ievent++) {
      hNevents->Fill(1);
    if (ievent%10000==0) cout<<"event"<<ievent<<endl;
    if (!pythia.next()) {
      if (++iErrors < maxErrors) continue;
      cout << "Error: too many errors in event generation - check your settings & code" << endl;
      break;
    }
    //count the multipilicity

    int nCharge_fwd = 0;
    int nCharge_mid = 0;
    for(int i=0;i<pythia.event.size();i++){
      bool isfinal = pythia.event[i].isFinal();  
      bool ischarge = pythia.event[i].isCharged();
      if (!isfinal || !ischarge) continue;
      double eta = pythia.event[i].eta();
      bool isFwd = fabs(eta)>Cuts::fwdEta[0]&&fabs(eta)<Cuts::fwdEta[1];
      bool isMid = fabs(eta)<Cuts::eta;

      if (isFwd) nCharge_fwd++;
      if (isMid) nCharge_mid++;

      hdNdeta->Fill(eta);
    }
    hmult[0]->Fill(nCharge_fwd);
    hmult[1]->Fill(nCharge_mid);
    for(int i=0;i<pythia.event.size();i++){
      pt = pythia.event[i].pT();
      eta = pythia.event[i].eta();
      y = pythia.event[i].y();

      // bool isinit = pythia.event[i].status();
      // if (fabs(eta)>=Cuts::eta) continue;
      if (fabs(y)>=Cuts::rapidity) continue;
      int id = pythia.event[i].id();
      bool findparticle = histmap.find(id)!=histmap.end();
      bool findantiparticle = histmap.find(-id)!=histmap.end();
      if (DEBUG) cout<<"start to fill hists"<<endl; 
      if (findparticle||findantiparticle) {
        if (DEBUG) cout<<"find "<<ParticleName[id]<<endl;
        if (id>0 && findantiparticle) continue;  //ignore the proton and pion
        else if (findparticle) histmap[id]->Fill(pt,nCharge_fwd);
        else if (id<0 && findantiparticle) histmap[-id]->Fill(pt,nCharge_fwd);
      }
      else continue;
    }

  }//end of event loop

  //--------------------------------------------------------------
  //  Finish up
  //--------------------------------------------------------------
  pythia.stat();
  cout << "Writing File" << endl;
  TFile* mFile = new TFile(rootfile,"recreate");
  WriteFile(mFile);
  mFile->Close();

  time_t now2 = time(0);
  cout << "============================================================================" << endl;
  cout << "Program finished at: " << ctime(&now2);
  cout << "============================================================================" << endl;

  return 0;
}
void InitHists(){
  int ip=0;
  std::map<int,std::string>::iterator it;
  cout<<"start initialize hists..."<<endl;
  typedef std::pair<int ,TH2*> pairhist;
  for (it=ParticleName.begin();it!=ParticleName.end();it++){
    hparticle[ip] = new TH2D(("h"+it->second).c_str(), (it->second+";p_{T}(GeV);Counts").c_str(),200,0,20,200,0,200);
    histmap.insert(pairhist(it->first,hparticle[ip]));
    hparticle[ip]->SetDirectory(0);
    cout<<"initialize "<<it->second<<endl;
    ip++;
  }
  hmult[0] = new TH1D("hnChargeMid","nChargeMid;Nch(|#eta|<1);Counts",200,0,200);
  hmult[0]->SetDirectory(0);
  hmult[1] = new TH1D("hnChargeFwd","nChargeFwd;Nch(2<|#eta|<5);Counts",200,0,200);
  hmult[1]->SetDirectory(0);
  hNevents = new TH1D("hNevents","hNevents",1,0,2);
  hdNdeta = new TH1D("hdNdeta","hdNdeta;#eta",150,-3,3);
}
void WriteFile(TFile* f){
  f->cd();
  for (int ip=0;ip<NParticle;ip++){
    hparticle[ip]->Write();
  }
  hmult[0]->Write();
  hmult[1]->Write();
  hNevents->Write();
  hdNdeta->Write();
}
