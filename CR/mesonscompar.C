#include "../compare/rootlogon.h"
void mesonscompar(){
  myStyle();
  TFile* f1 = TFile::Open("BandDratio.root");
  TH1D* hb = (TH1D*)f1->Get("Lb/LbtoBmesons_pt_whole_rebin");
  hb->Draw();
  hb->SetLineColor(kRed);
  hb->GetXaxis()->SetRangeUser(0,10);
  hb->GetYaxis()->SetTitle("ratio");
  hb->GetXaxis()->SetTitle("p_{T}(GeV)");
  
  hb->GetYaxis()->SetTitle("ratio");
  hb->GetYaxis()->SetRangeUser(0,0.8);
  TH1D* hc = (TH1D*)f1->Get("Lc/LctoDmesons_pt_whole_rebin");
  hc->Draw("same");
  hc->SetLineColor(kBlue);
  TFile* f2 = TFile::Open("200GeVratio1029.root");
  TH1D* hs = (TH1D*)f2->Get("lambda/lambdatok_pt_whole_rebin");
  hs->Scale(0.5);
  hs->Draw("same");
  hs->SetDirectory(0);
  TLegend* l = new TLegend(0.6,0.6,0.85,0.85);
  l->AddEntry(hb,"#Lambda_{b} / (B^{+}+B^{0}+B_{s})","l");
  l->AddEntry(hc,"#Lambda_{c} / (D^{+}+D^{0}+D_{s})","l");
  l->AddEntry(hs,"#Lambda / (K_{S}*2.)","l");
  l->Draw();
  gPad->SaveAs("comparemesons.png");
}
