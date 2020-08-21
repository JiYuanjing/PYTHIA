#include "../compare/rootlogon.h"

void plotCMS(){
  TFile* fpy =  TFile::Open("test_7TeV_ratio.root"); 
  TH1D* hKs = (TH1D*)fpy->Get("lambda/lambdatok_pt_whole_rebin");
  hKs->SetDirectory(0);
  fpy->Close();
  hKs->Draw();
  int const NMAX = 100;
  double x[NMAX],y[NMAX],xerrh[NMAX],xerrl[NMAX],ystaterrh[NMAX],ystaterrl[NMAX],ysyserrl[NMAX], ysyserrh[NMAX];
  int num=0;
  ifstream read;
  read.open("cmsdata.txt");
  while (read.good()){
    read>>x[num]>>xerrl[num]>>xerrh[num]>>y[num]>>ystaterrh[num]>>ystaterrl[num]>>ysyserrh[num]>>ysyserrl[num];
    xerrh[num]=xerrh[num]-x[num];
    xerrl[num]=x[num]-xerrl[num];
    num++;
  }
  TGraphAsymmErrors* g = new TGraphAsymmErrors(num-1,x,y,xerrl,xerrh,ystaterrl,ystaterrh); 
  g->Draw("same");
  gPad->SaveAs("cms.png");
}
