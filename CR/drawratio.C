#include <map>
#include "Anacuts.h"
#include "myfunction.h"
using namespace std;

void drawHist(TH1* const h,TPDF* const pdf,TCanvas* c, int mode);
void processDivision(TH2* const h1,TH2* const h2,TPDF* const pdf,TCanvas *c,TFile* f);
void ProjectionAndScale(const TH2* const h, TPDF* const pdf, TCanvas* c, TFile* const f, double scale);
void drawratio(){
  // TString name = "test_aliceCR.root";
  // TString name = "test_200GeV_sphenix2.root";
  // TString name = "test_200GeVCR.root";
  // TString name = "200GeVratio1029.root";
  // TString name = "test_wei.root";
  // TString name = "test_200mode2.root";
  // TString name = "test_200mode2_8_24.root";
  TString name = "test_200mode2_8_24_noetacut.root";
  // TString name = "test_0820.root";
  // TString name = "test_200GeVCR_nodecay.root";
  // TString name = "test_200GeVCR_mode0.root";
  // TString name = "test_7TeV.root";
  TFile* f = TFile::Open(name);
  bookHists(f);
  f->Close();
  f = new TFile(name.ReplaceAll(".root","_ratio.root"),"recreate");

  TPDF* pdf = new TPDF(name.ReplaceAll(".root",".pdf"));
  TCanvas* c = new TCanvas("c","c",800,800);
  drawtitle(pdf,c,"c/b/s/u hardrons ratio in PYTHIA 8.2 (tune)");
  hmult[0]->Draw();
  gPad->SetLogy(true);
  addpdf(pdf,c);
  hmult[1]->Draw();
  addpdf(pdf,c);
  gPad->SetLogy(false);
  double scale = hmult[0]->GetEntries();
  //charm hardrons
  ProjectionAndScale(hnamemap[charms[0]],pdf,c,f,scale);
  ProjectionAndScale(hnamemap[charms[4]],pdf,c,f,scale);
  for (int ic=1;ic<charms.size();ic++){
    processDivision(hnamemap[charms[ic]],hnamemap[charms[0]],pdf,c,f);  
  }
  //bottom hardrons
  ProjectionAndScale(hnamemap[beautys[0]],pdf,c,f,scale);
  for (int ic=1;ic<beautys.size();ic++){
    processDivision(hnamemap[beautys[ic]],hnamemap[beautys[0]],pdf,c,f);  
  }
  // //strange hardrons
  ProjectionAndScale(hnamemap[stranges[0]],pdf,c,f,scale);
  for (int ic=1;ic<stranges.size();ic++){
    processDivision(hnamemap[stranges[ic]],hnamemap[stranges[0]],pdf,c,f);  
  }
  // //up hardrons
  // ProjectionAndScale(hnamemap[ups[0]],pdf,c,f,scale);
  // for (int ic=1;ic<ups.size();ic++){
  //   processDivision(hnamemap[ups[ic]],hnamemap[ups[0]],pdf,c,f);  
  // }
  pdf->On();
  pdf->Close();
  f->Close();
}
void bookHists(TFile* f){
  int ip=0;
  std::map<int,std::string>::iterator it;
  cout<<"start booking hists..."<<endl;
  typedef std::pair<string,TH2*> pairhist;
  for (it=ParticleName.begin();it!=ParticleName.end();it++){
    hparticle[ip] = (TH2D*)f->Get(("h"+it->second).c_str());
    hnamemap.insert(pairhist(it->second,hparticle[ip]));
    hparticle[ip]->SetDirectory(0);
    cout<<"book "<<it->second<<endl;
    ip++;
  }
  hmult[0] = (TH1D*)f->Get("hnChargeMid");
  hmult[0]->SetDirectory(0);
  hmult[1] = (TH1D*)f->Get("hnChargeFwd");
  hmult[1]->SetDirectory(0);
}
void drawHist(TH1* const h,TPDF* const pdf,TCanvas* c, int mode){
  double xhigh[2]={15,100};
  string xtitle[2]={"p_{T}","N_{ch}(2<|#eta|<5)"};
  h->GetXaxis()->SetRangeUser(0,xhigh[mode]);
  double mean = h->Integral(3,13)/10.;
  if (mean>0.8)
    h->GetYaxis()->SetRangeUser(0,1.25);
  else 
    h->GetYaxis()->SetRangeUser(0,1);
  h->GetXaxis()->SetTitle(xtitle[mode].c_str());
  // h->GetYaxis()->SetTitle("ratio");
  h->SetTitle(h->GetName());
  h->Draw();
  addpdf(pdf,c);
}
void processDivision(TH2* const h1,TH2* const h2,TPDF* const pdf,TCanvas *c,TFile* f){
  f->cd(); 
  TString name1(h1->GetName());
  name1.ReplaceAll("h","");
  h1->Sumw2();
  TDirectory* d = f->mkdir(name1);
  d->cd();
  TString name2(h2->GetName());
  name2.ReplaceAll("h","");
  int const Nbins=4;
  double NchBinHigh[Nbins]={10,20,100}; //last bin is left for whole axis projection
  double NchBinLow[Nbins]={0,10,20};
  string BinsName[Nbins]={"0_10","10_20","20_40","whole"};
  TH1D *pt1[Nbins], *pt2[Nbins];
  for (int ibin=0;ibin<Nbins;ibin++){
    int lowbin=0,highbin=0;
    if (ibin<Nbins-1) {
      highbin=h1->GetYaxis()->FindBin(NchBinHigh[ibin]);
      lowbin=h1->GetYaxis()->FindBin(NchBinLow[ibin]);
    }
    else if (ibin==Nbins-1) {
      lowbin=1;
      highbin=h1->GetYaxis()->GetNbins();
    }
    pt1[ibin] =(TH1D*)h1->ProjectionX((name1+"to"+name2+"_pt_"+BinsName[ibin].c_str()).Data(),lowbin,highbin); 
    pt1[ibin]->GetYaxis()->SetTitle((name1+"/"+name2+"ratio").Data());
    double val1 = pt1[ibin]->Integral(pt1[ibin]->FindBin(3),pt1[ibin]->FindBin(6));
    pt2[ibin]=(TH1D*)h2->ProjectionX((name2+"pt_"+BinsName[ibin].c_str()).Data(),lowbin,highbin); 
    double val2 = pt2[ibin]->Integral(pt2[ibin]->FindBin(3),pt2[ibin]->FindBin(6));
    cout<< (name1+"/"+name2+" ratio("+BinsName[ibin]+"): ").Data()<<" 3<pt<6 "<<val1/val2 <<endl;
    // pt1[ibin]->Rebin();
    // pt2[ibin]->Rebin();
    double point[]={0,5,20};
    int edge[]={4,8};
    pt1[ibin]=(TH1D*)RebinHist(pt1[ibin],point,edge,2);
    pt2[ibin] =(TH1D*) RebinHist(pt2[ibin],point,edge,2);
    pt1[ibin]->Divide(pt2[ibin]);
    drawHist(pt1[ibin],pdf,c,0);
    pt1[ibin]->Write();
  }
  TH1D* mult1=(TH1D*)h1->ProjectionY((name1+"to"+name2+"_mult").Data(),h1->GetXaxis()->FindBin(2),h1->GetXaxis()->FindBin(8)); 
  mult1->GetYaxis()->SetTitle((name1+"/"+name2+"ratio").Data());
  TH1D* mult2=(TH1D*)h2->ProjectionY(name2+"_mult",h1->GetXaxis()->FindBin(2),h1->GetXaxis()->FindBin(8)); 

  mult1->Divide(mult2);
  drawHist(mult1,pdf,c,1);
  mult1->Write();
  // f->Close(); 
}
void ProjectionAndScale(const TH2* const h, TPDF* const pdf, TCanvas* c, TFile* const f, double scale){
  TH1D* hX = (TH1D*)h->ProjectionX(Form("%s_pt",h->GetName()), 1,h->GetYaxis()->GetNbins());
  hX->GetXaxis()->SetTitle("p_{T}(GeV)");
  TH1D* hXsc=(TH1D*)hX->Clone(Form("%s_scale",hX->GetName()));
  hXsc->Scale(1./scale);
  TH1D* hY = (TH1D*)h->ProjectionY(Form("%s_Nch",h->GetName()), 1,h->GetXaxis()->GetNbins());
  hY->GetXaxis()->SetTitle("Nch (2<|#eta|<5)");
  TH1D* hYsc=(TH1D*)hY->Clone(Form("%s_scale",hY->GetName()));
  hYsc->Scale(1./scale);
  hXsc->Draw();
  addpdf(pdf,c);
  hYsc->Draw();
  addpdf(pdf,c);
  f->cd();
  hX->Write();
  hXsc->Write();
  hY->Write();
  hYsc->Write();
}

