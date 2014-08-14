
void Plot_bb_uds(){
  TCanvas Canvas("Canvas");
  TString filename="Bkg_bb_uds.ps";  
  Canvas.Print(filename+"[");
  
  TFile FBpBm("DstarCharged/DstarPi/MC/SP-1235/K3pi/ReducedClean.root","read");
  gROOT->cd();
  TTree*tbpbm=(TTree*)FBpBm.Get("CleanNtuple");
  TH1F hbpbm("hbpbm","",300,2.1,3.0); 
  tbpbm->Draw("dstpideltam+2.01>>hbpbm");
  Float_t scalebpbm=470./222.;
  hbpbm.Scale(scalebpbm);
  SetHistoXYLabels(&hbpbm,"m(D*#pi)","(GeV/c^{2})");
  hbpbm.SetStats(0);
	      
  TFile FBzBzb("DstarCharged/DstarPi/MC/SP-1237/K3pi/ReducedClean.root","read");
  gROOT->cd();
  TTree*tbzbzb=(TTree*)FBzBzb.Get("CleanNtuple");
  TH1F hbzbzb("hbzbzb","",300,2.1,3.0); 
  tbzbzb->Draw("dstpideltam+2.01>>hbzbzb");
  Float_t scalebzbzb=470./213.;
  hbzbzb.Scale(scalebzbzb);
  SetHistoXYLabels(&hbzbzb,"m(D*#pi)","(GeV/c^{2})");
  hbzbzb.SetStats(0);

  TFile Fuds("DstarCharged/DstarPi/MC/SP-998/K3pi/ReducedClean.root","read");
  gROOT->cd();
  TTree*tuds=(TTree*)Fuds.Get("CleanNtuple");
  TH1F huds("huds","",300,2.1,3.0); 
  tuds->Draw("dstpideltam+2.01>>huds");
  Float_t scaleuds=470./152.; 
  huds.Scale(scaleuds);
  SetHistoXYLabels(&huds,"m(D*#pi)","(GeV/c^{2})");
  huds.SetStats(0);

  Canvas.Clear();
  hbpbm.Draw();
  Canvas.Print(filename);
	       
  Canvas.Clear();
  hbzbzb.Draw();
  Canvas.Print(filename);

  Canvas.Clear();
  huds.Draw();
  Canvas.Print(filename);
	
  Canvas.Print(filename+"]");
}
