void OptimizeKCuts(){

  TFile FMC("DsTolnu/SP1005/DsReco/Ntuple.root","Read");
  FMC.ls();
  gROOT->cd();
  TTree*t=(TTree*)FMC.Get("Ntuple");
  //TTree*tsig=t->CopyTree("evthassignal");
  
  TCanvas Canvas("Canvas");
  Canvas.Print("DsTolnu/SP1005/DsReco/OptimizeKCuts.ps[");
  
  Canvas.Clear();
  TH1F HFlight("HFlight","",50,0,50);
  t->Draw("Kflights>>HFlight","Kflights!=100.");
  HFlight.SetStats(0);
  HFlight.SetTitle("");
  HFlight.GetXaxis()->SetTitle("flight significance");
  HFlight.GetYaxis()->SetTitle("Entries/1");
  HFlight.Draw("pe");
  t->Draw("Kflights","Kflights!=100.&&Kmctrue>0","same");
  Canvas.Print("DsTolnu/SP1005/DsReco/OptimizeKCuts.ps");    

  Canvas.Clear();
  TH1F HPid("HPid","",4,1.5,5.5);
  t->Draw("Kpid>>HPid","Kflights==100.");
  HPid.SetStats(0);
  HPid.SetTitle("BDT Kaon Selector");
  HPid.GetXaxis()->SetNdivisions(0);
  HPid.GetXaxis()->SetTitle("VeryLoose        Loose             Tight       VeryTight   ");
  HPid.Draw("pe");
  t->Draw("Kpid","Kflights==100.&&Kmctrue>0","same");
  Canvas.Print("DsTolnu/SP1005/DsReco/OptimizeKCuts.ps");   
  
  Canvas.Print("DsTolnu/SP1005/DsReco/OptimizeKCuts.ps]");  
  //delete tsig;
}


void OptimizeXCuts(){

  TFile FMC("DsTolnu/SP1005/DsReco/Ntuple.root","Read");
  FMC.ls();
  gROOT->cd();
  TTree*t=(TTree*)FMC.Get("Ntuple");


  TCanvas Canvas("Canvas");
  Canvas.Print("DsTolnu/SP1005/DsReco/OptimizeXCuts.ps[");

  Canvas.Clear();
  TH1F HPid("HPid","",7,-.5,6.5);
  t->Draw("fragpipid>>HPid","fragnpis>0");
  HPid.SetStats(0);
  HPid.SetTitle("pion KM Selector");
  HPid.GetXaxis()->SetNdivisions(0);
  HPid.GetXaxis()->SetTitleSize(.035);
  HPid.GetXaxis()->SetTitle("NoCut  SuperLoose  VeryLoose    Loose      Tight  VeryTight  SuperTight ");
  HPid.Draw("pe");
  t->Draw("fragpipid","fragnpis>0&&fragpipidtrue>0","same");
  Canvas.Print("DsTolnu/SP1005/DsReco/OptimizeXCuts.ps");   
  

//   TTree*tsig=t->CopyTree("evthassignal"); 
//   TTree*tbkg=t->CopyTree("!evthassignal");
    
//   TH1F HSigEff("HSigEff","",7,-.5,6.5);
//   TH1F HBkgEff("HBkgEff","",7,-.5,6.5); 
//   Int_t NTotSig=tsig->GetEntries();
//   Int_t NTotBkg=tbkg->GetEntries();
//   Canvas.Clear();
//   for(Int_t s=0;s<=6;s++){
//     HSigEff.SetBinContent(s+1,(float)tsig->GetEntries(TString("fragpipid>=")+(long)s)/(float)NTotSig);
//     HBkgEff.SetBinContent(s+1,(float)tbkg->GetEntries(TString("fragpipid>=")+(long)s)/(float)NTotBkg);
//   }
//   HSigEff.SetStats(0);
//   HSigEff.GetYaxis()->SetRangeUser(.7,1);
//   HSigEff.GetYaxis()->SetTitle("Efficiency");
//   HSigEff.GetXaxis()->SetNdivisions(0);
//   HSigEff.GetXaxis()->SetTitleSize(.035);
//   HSigEff.GetXaxis()->SetTitle("NoCut  SuperLoose  VeryLoose    Loose      Tight  VeryTight  SuperTight ");
//   HSigEff.Draw("p");
//   HBkgEff.Draw("same");
//   Canvas.Print("DsTolnu/SP1005/DsReco/OptimizeXCuts.ps");   
  
  Canvas.Print("DsTolnu/SP1005/DsReco/OptimizeXCuts.ps]"); 
//   delete tsig;
//   delete tbkg;
}
