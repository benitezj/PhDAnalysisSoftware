void PlotDsPeakVsPstar(){
  
  TFile File("DsTolnu/SP1005Skim/DsReco/Final/Ntuple.root","read");
  TTree* t=(TTree*)File.Get("Ntuple");
 
  TCanvas C("C");
  C.Print("DsTolnu/SP1005Skim/DsReco/Final/PlotDsPeakVsPstar.ps[");
  
  C.Clear();
  C.Divide(2,2); 
  TH1F* H[4];
  TH1F HMean("HMean","",4,3.,5.);
  TH1F HRMS("HRMS","",4,3.,5.);
  for(Int_t i=0;i<4;i++){
    C.cd(i+1);
    H[i]=new TH1F(TString("H")+(long)i,"",50,1.82,2.12);
    H[i]->GetXaxis()->SetTitle("m(D_{s})");
    t->Draw(TString("dsmass>>H")+(long)i,TString("gammctrue>0&&(3.+.5*")+(long)i+")<dspstar&&dspstar<(3.+.5+.5*"+(long)i+")");    
    HMean.SetBinContent(i+1,H[i]->GetMean());
    HRMS.SetBinContent(i+1,H[i]->GetRMS());
  }  
  C.Print("DsTolnu/SP1005Skim/DsReco/Final/PlotDsPeakVsPstar.ps");
  
  C.Clear();
  HMean.GetXaxis()->SetTitle("p*(D_{s})");
  HMean.GetYaxis()->SetRangeUser(1.968,1.974);
  HMean.GetYaxis()->SetTitle("Mean");
  HMean.SetStats(0);
  HMean.SetMarkerSize(2);
  HMean.Draw("p"); 
  C.Print("DsTolnu/SP1005Skim/DsReco/Final/PlotDsPeakVsPstar.ps");


  C.Clear();
  HRMS.GetXaxis()->SetTitle("p*(D_{s})");
  HRMS.GetYaxis()->SetRangeUser(.016,.028);
  HRMS.GetYaxis()->SetTitle("RMS");
  HRMS.SetStats(0);
  HRMS.SetMarkerSize(2);
  HRMS.Draw("p");  
  C.Print("DsTolnu/SP1005Skim/DsReco/Final/PlotDsPeakVsPstar.ps");


  C.Print("DsTolnu/SP1005Skim/DsReco/Final/PlotDsPeakVsPstar.ps]");     
}


void PlotDsPeakVsGamAngle(){
  
  TFile File("DsTolnu/SP1005Skim/DsReco/Final/Ntuple.root","read");
  TTree* t=(TTree*)File.Get("Ntuple");
 
  TCanvas C("C");
  C.Print("DsTolnu/SP1005Skim/DsReco/Final/PlotDsPeakVsGamAngle.ps[");
  
  C.Clear();
  C.Divide(2,2); 
  TH1F* H[4];
  TH1F HMean("HMean","",4,-1,0.);
  TH1F HRMS("HRMS","",4,-1.,0.);
  TH1F HInt("HInt","",4,-1.,0.);
  for(Int_t i=0;i<4;i++){
    C.cd(i+1);
    H[i]=new TH1F(TString("H")+(long)i,"",50,1.82,2.12);
    H[i]->GetXaxis()->SetTitle("m(D_{s})");
    t->Draw(TString("dsmass>>H")+(long)i,TString("gammctrue>0&&(-1.+.25*")+(long)i+")<gamangle&&gamangle<(-.1+.25+.25*"+(long)i+")");    
    HMean.SetBinContent(i+1,H[i]->GetMean());
    HRMS.SetBinContent(i+1,H[i]->GetRMS());
    HInt.SetBinContent(i+1,H[i]->Integral());
  }  
  C.Print("DsTolnu/SP1005Skim/DsReco/Final/PlotDsPeakVsGamAngle.ps");
  
  C.Clear();
  HMean.GetXaxis()->SetTitle("cos(#theta_#gamma)");
  HMean.GetYaxis()->SetRangeUser(1.955,1.975);
  HMean.GetYaxis()->SetTitle("Mean");
  HMean.SetStats(0);
  HMean.SetMarkerSize(2);
  HMean.Draw("p"); 
  C.Print("DsTolnu/SP1005Skim/DsReco/Final/PlotDsPeakVsGamAngle.ps");


  C.Clear();
  HRMS.GetXaxis()->SetTitle("cos(#theta_#gamma)");
  HRMS.GetYaxis()->SetRangeUser(.020,.032);
  HRMS.GetYaxis()->SetTitle("RMS");
  HRMS.SetStats(0);
  HRMS.SetMarkerSize(2);
  HRMS.Draw("p");  
  C.Print("DsTolnu/SP1005Skim/DsReco/Final/PlotDsPeakVsGamAngle.ps");

  for(Int_t i=0;i<4;i++)
    HInt.SetBinContent(i+1,H[i]->Integral()/(H[0]->Integral()+H[1]->Integral()+H[2]->Integral()+H[3]->Integral()));
  C.Clear();
  HInt.GetXaxis()->SetTitle("cos(#theta_#gamma)");
  HInt.GetYaxis()->SetRangeUser(0.,.8);
  HInt.GetYaxis()->SetTitle("Weight");
  HInt.SetStats(0);
  HInt.SetMarkerSize(2);
  HInt.Draw("p");  
  C.Print("DsTolnu/SP1005Skim/DsReco/Final/PlotDsPeakVsGamAngle.ps");


  C.Print("DsTolnu/SP1005Skim/DsReco/Final/PlotDsPeakVsGamAngle.ps]");     
}
