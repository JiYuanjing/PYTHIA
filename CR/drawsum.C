#include "myfunction.h"
void addHist(TFile* f, string* s, int hnum, TH2D* h1,TH2D* h2);
void drawHist(TH1* const h,TPDF* const pdf,TCanvas* c, int mode);
void processDivision(TH2* const h1,TH2* const h2,TPDF* const pdf,TCanvas *c,TFile* f);
void addpdf(TPDF* const  pdf,TCanvas* c);

void drawsum(){
  TFile* f = TFile::Open("test_200GeVCR.root");
  string s_b[4]={"hLb","hB0","hBplus","hBs"}; 
  string s_c[4]={"hLc","hD0","hDplus","hDs"};
  TH2D *hLb,*hB,*hLc,*hD;
 // addHist(f,s_b,4,hLb,hB);
 // addHist(f,s_c,4,hLc,hD);
  f->cd();
  hLb = (TH2D*)f->Get(s_b[0].c_str());
  cout<<"book "<<s_b[0]<<endl;
  hLb->SetDirectory(0);
  for (int i=1;i<4;i++){
    if (i==1) hB = (TH2D*)f->Get(s_b[i].c_str())->Clone((s_b[i].replace(s_b[i].find("0"),1,"")+"mesons").c_str());
    if (i>1) hB->Add((TH2D*)f->Get(s_b[i].c_str()));
    cout<<"book "<<s_b[i]<<endl;
  }
  // h2->SetName("Bmesons");
  hB->SetDirectory(0);
  f->cd();
  hLc = (TH2D*)f->Get(s_c[0].c_str());
  cout<<"book "<<s_c[0]<<endl;
  hLc->SetDirectory(0);
  for (int i=1;i<4;i++){
    if (i==1) hD = (TH2D*)f->Get(s_c[i].c_str())->Clone((s_c[i].replace(s_c[i].find("0"),1,"")+"mesons").c_str());
    if (i>1) hD->Add((TH2D*)f->Get(s_c[i].c_str()));
    cout<<"book "<<s_c[i]<<endl;
  }
  // h2->SetName("Bmesons");
  hD->SetDirectory(0);
  // hB->Scale(1./3.);
  // hD->Scale(1./3.);
  TFile* fout = new TFile("BandDratio.root","recreate");  
  TPDF* pdf = new TPDF("BandDratio.pdf");
  pdf->On();
  TCanvas* c = new TCanvas("c","c",800,1000);
  processDivision(hLb,hB,pdf,c,fout);
  processDivision(hLc,hD,pdf,c,fout);
  cout<<"a"<<endl;
  
  pdf->Off();
  pdf->Close(); 
}

void addHist(TFile* f, string* s, int hnum, TH2D* h1,TH2D* h2){
  f->cd();
  h1 = (TH2D*)f->Get(s[0].c_str());
  cout<<"book "<<s[0]<<endl;
  h1->SetDirectory(0);
  for (int i=1;i<hnum;i++){
    if (i==1) h2 = (TH2D*)f->Get(s[i].c_str())->Clone((s[i].replace(s[i].find("0"),1,"")+"mesons").c_str());
    if (i>1) h2->Add((TH2D*)f->Get(s[i].c_str()));
    cout<<"book "<<s[i]<<endl;
  }
  // h2->SetName("Bmesons");
  h2->SetDirectory(0);
}
void processDivision(TH2* const h1,TH2* const h2,TPDF* const pdf,TCanvas *c,TFile* f){
  f->cd(); 
  TString name1(h1->GetName());
  name1.ReplaceAll("h","");

  TDirectory* d = f->mkdir(name1);
  d->cd();
  TString name2(h2->GetName());
  name2.ReplaceAll("h","");
  int const Nbins=4;
  double NchBinHigh[Nbins]={10,20,40}; //last bin is left for whole axis projection
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
    cout<< (name1+"/"+name2+" ratio("+BinsName[ibin]+"): ").Data()<<val1/val2 <<endl;
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

void drawHist(TH1* const h,TPDF* const pdf,TCanvas* c, int mode){
  double xhigh[2]={12,60};
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

