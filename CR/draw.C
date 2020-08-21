void setPad(Double_t left, Double_t right, Double_t top, Double_t bottom, int color=10)
{
  gPad->SetFillColor(color);
  gPad->SetBorderMode(0);
  gPad->SetBorderSize(0);
  gPad->SetFrameFillColor(10);
  gPad->SetFrameBorderMode(0);
  gPad->SetFrameBorderSize(0);
  gPad->SetLeftMargin(left);
  gPad->SetRightMargin(right);
  gPad->SetTopMargin(top);
  gPad->SetBottomMargin(bottom);
}

void addpdf(TPDF* pdf,TCanvas* c)
{
  c->cd();
  pdf->On();
  c->Update();
  pdf->NewPage();
  pdf->Off();
}
void drawtitle(TPDF* pdf,TCanvas* c,string s){
  c->cd();
  c->Draw();
  setPad(0.1,0.1,0.05,0.12);
  TLatex t;
  t.SetTextSize(0.05);
  t.DrawText(0.2,0.5,s.c_str());
  TLatex la;
  la.SetTextSize(0.035);
  la.DrawText(0.1,0.3,(new TDatime())->AsSQLString());
  la.DrawText(0.1,0.2,"by Yuanjing");
  pdf->On();
  pdf->NewPage();
  gPad->Update();
  pdf->Off();
}
void drawHist(TH1* h,TPDF* pdf,TCanvas* c, int mode){
  double xhigh[2]={15,200};
  string xtitle[2]={"p_{T}","N_{ch}"};
  h->GetXaxis()->SetRangeUser(0,xhigh[mode]);
  h->GetYaxis()->SetRangeUser(0,1);
  h->GetXaxis()->SetTitle(xtitle[mode].c_str());
  h->GetYaxis()->SetTitle("ratio");
  h->SetTitle(h->GetName());
  h->Draw();
  addpdf(pdf,c);
}
void processDivision(const TH1* h1,const TH1* h2,TPDF* pdf,TCanvas *c,TFile* f){
  f->cd(); 
  TString name1(h1->GetName());
    name1.ReplaceAll("h","");

  TDirectory* d = f->mkdir(name1);
  d->cd();
  TString name2(h2->GetName());
    name2.ReplaceAll("h","");
  TH1D* pt1=(TH1D*)h1->Clone((name1+"to"+name2+"_pt").Data()); 
  pt1->Divide(h2);
  drawHist(pt1,pdf,c,0);
  pt1->Write();
  // f->Close(); 
}
void draw(){
  TFile* f = TFile::Open("mode2.root");
  TH1D* hD0 = (TH1D*)f->Get("hD0");
  hD0->SetDirectory(0);
  TH1D* hDstar = (TH1D*)f->Get("hDstar");
  hDstar->SetDirectory(0);
  TH1D* hDs = (TH1D*)f->Get("hDs");
  hDs->SetDirectory(0);
  TH1D* hLc = (TH1D*)f->Get("hLc");
  hLc->SetDirectory(0);
  TH1D* hDplus = (TH1D*)f->Get("hDplus");
  hDplus->SetDirectory(0);
  f->Close();
  f = new TFile("ratio.root","recreate");
  
  TPDF* pdf = new TPDF("ratio.pdf");
  TCanvas* c = new TCanvas("c","c");
  drawtitle(pdf,c,"Charm hardrons ratio in PYTHIA 8.2 (tune)");
  processDivision(hDstar,hD0,pdf,c,f);  
  processDivision(hDs,hD0,pdf,c,f);  
  processDivision(hDplus,hD0,pdf,c,f);  
  processDivision(hLc,hD0,pdf,c,f);  
  pdf->On();
  pdf->Close();
  f->Close();
}
