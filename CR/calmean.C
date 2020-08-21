void calmean(string filename="test_200GeVCR.root", double offlow=1,double offhigh=8){
  TFile* f = TFile::Open(filename.c_str());
  // TH2D* h = (TH2D*)f->Get("hLc");
  TH2D* h = (TH2D*)f->Get("hD0");
  TH1D* hX = (TH1D*)h->ProjectionX(Form("%s_pt",h->GetName()), 1,h->GetYaxis()->GetNbins());
  hX->GetXaxis()->SetTitle("p_{T}(GeV)");
  int low = hX->GetXaxis()->FindBin(offlow);
  int high = hX->GetXaxis()->FindBin(offhigh);
  double tot = hX->Integral(low,high);
  double alltot = hX->Integral(1,high);
  double ptsum=0;
  for (int i=low;i<high;i++){
     ptsum+=hX->GetBinCenter(i+1)*hX->GetBinContent(i+1);
  }
  double mean = ptsum/tot;
  cout <<"tot evt: "<<tot<<" mean pt: "<< mean<<" cross section fraction: "<<tot/alltot <<endl;
}
