#include "../../FUNCTIONS/Compute2DEfficiency.C"
#include <TF2.h>

Int_t DCPiAnalysis::MakeEfficiency(){
  if(!_TruthMatch)return 0;
  
  TH2F*HMCMassVsPstar=(TH2F*)GetMCHisto("HMCMassVsPstar");
  
  if(OpenReducedNtuple()!=1) return 0;
  TFile file(_OutputDir+"/Efficiency.root","recreate");
  
  TH2F HRecMassVsPstar("HRecMassVsPstar","MC",
		       HMCMassVsPstar->GetXaxis()->GetNbins(),
		       HMCMassVsPstar->GetXaxis()->GetXmin(),
		       HMCMassVsPstar->GetXaxis()->GetXmax(),
		       HMCMassVsPstar->GetYaxis()->GetNbins(),
		       HMCMassVsPstar->GetYaxis()->GetXmin(),
		       HMCMassVsPstar->GetYaxis()->GetXmax());
  ReducedNtuple->Draw("xpstar:(xmdm-xdmres)>>HRecMassVsPstar");
  HRecMassVsPstar.Sumw2();
  
  if(!Compute2DEfficiencyFit(&HRecMassVsPstar,HMCMassVsPstar))return 0;

 
  file.ls();
  file.Close();

  delete HMCMassVsPstar;

  return 1;

}


//methods for using the Efficiency:
Int_t DCPiAnalysis::OpenEfficiency(){
  
  ///these are the the 3D efficiency maps: Eff vs. (p*,cos*,cos_D*)
  EffFile=new TFile("DCharged/DChargedPi/MC/SP-9015/KPiPi/Efficiency.root","read");
  if(!EffFile){cout<<"No Efficiency file"<<endl;return 0;}
  if(EffFile->IsZombie()){cout<<"No Efficiency file"<<endl;EffFile=0;return 0;}
  if(_MCorDATA==0){
    H2Efficiency=(TH2F*)EffFile->Get("H2EfficiencyMC");
    H2EfficiencyErr=(TH2F*)EffFile->Get("H2EfficiencyErrMC");
  }
  if(_MCorDATA==1){
    H2Efficiency=(TH2F*)EffFile->Get("H2EfficiencyMC");//includes pid weight correction
    H2EfficiencyErr=(TH2F*)EffFile->Get("H2EfficiencyErrMC");
  }
  if(!H2Efficiency||!H2EfficiencyErr){cout<<"No Detector Efficiency Histogram"<<endl;return 0;}

  FPCutEff=(TF2*)EffFile->Get("FPCutEff");
  if(!FPCutEff){cout<<"No p* cut Efficiency"<<endl;return 0;}
  gROOT->cd();

 
  //
  effpstarstep=H2Efficiency->GetYaxis()->GetBinWidth(1);
  effpstarmin=H2Efficiency->GetYaxis()->GetXmin();

  effmassstep=H2Efficiency->GetXaxis()->GetBinWidth(1);
  effmassmin=H2Efficiency->GetXaxis()->GetXmin();

  return 1;
}
Float_t DCPiAnalysis::GetEfficiency(){
  if(!H2Efficiency)return 0;
  return H2Efficiency->GetBinContent(1+(int)((xmdm-effmassmin)/effmassstep),1+(int)((xpstar-effpstarmin)/effpstarstep));  
}
Float_t DCPiAnalysis::GetEfficiencyErr(){
  if(!H2Efficiency)return 0;
  return H2Efficiency->GetBinError(1+(int)((xpstar-effpstarmin)/effpstarstep),1+(int)((xmdm-effmassmin)/effmassstep));
}
void DCPiAnalysis::CloseEfficiency(){
  if(!EffFile)return;
  EffFile->Close();
  delete EffFile;
}


Int_t DCPiAnalysis::PlotPstarCutEfficiency(){

  if(!_TruthMatch)return 0;
  
  TH2F*HMCMassVsPstar=(TH2F*)GetMCHisto("HMCMassVsPstar");
  TH1D*HMCMass=HMCMassVsPstar->ProjectionX("HMCMass",1,HMCMassVsPstar->GetNbinsY(),"");
   
  filename=_OutputDir+"/PstarCutEfficiency.ps";
  Canvas.Print(filename+"["); 
  
  Canvas.Clear();
  HMCMass->Draw("pe");
  Canvas.Print(filename);  

  TF1 Fpol1("Fpol1","[0]+[1]*x",HMCMass->GetXaxis()->GetXmin(),HMCMass->GetXaxis()->GetXmax());
  Fpol1.SetParameters(1,2);
  TGraphErrors GSlope;
  
  Canvas.Clear();
  Canvas.Divide(3,4);
  Int_t pad=1;
  TH1D* H[25];
  text.SetTextSize(.2);
  for(Int_t i=1;i<=HMCMassVsPstar->GetNbinsY();i++){
    H[i]=HMCMassVsPstar->ProjectionX(TString("HMassP")+(long)i,i,i,"");    
    H[i]->Sumw2();
    if(HMCMassVsPstar->GetYaxis()->GetBinCenter(i)<CutPMin||H[i]->Integral()<500)continue;    

    Canvas.cd(pad);
    H[i]->Divide(HMCMass);
    H[i]->GetXaxis()->SetTitle("m(D#pi)");
    H[i]->Fit(&Fpol1);
    
    GSlope.SetPoint(pad-1,HMCMassVsPstar->GetYaxis()->GetBinCenter(i),Fpol1.GetParameter(1));
    GSlope.SetPointError(pad-1,HMCMassVsPstar->GetYaxis()->GetBinWidth(i)/2,Fpol1.GetParError(1));
    
    text.DrawTextNDC(.15,.15,TString("")+(long)pad);

    pad++;

  }

  Canvas.Print(filename);  
  for(Int_t i=1;i<=HMCMassVsPstar->GetNbinsY();i++)
    if(H[i])delete H[i];


  TF1 Fpol2("Fpol2","[0]+[1]*x+[2]*x*x",CutPMin,5.);
  Canvas.Clear();
  GSlope.GetYaxis()->SetTitle("Efficiency Slope");
  GSlope.GetXaxis()->SetTitle("p*(D#pi) (GeV)");
  GSlope.Fit(&Fpol2);
  GSlope.Draw("ape");
  Canvas.Print(filename);  
  
  TF2 FPCutEff("FPCutEff","1+([0]+[1]*y+[2]*y*y)*x",EffMin,EffMax,CutPMin,5.);
  FPCutEff.SetParameters(Fpol2.GetParameter(0),Fpol2.GetParameter(1),Fpol2.GetParameter(2));
  Canvas.Clear();
  FPCutEff.SetTitle("");
  FPCutEff.GetZaxis()->SetTitle("Efficiency (Arbitrary normalization)");
  FPCutEff.GetYaxis()->SetTitle("p*(D#pi)(GeV)");
  FPCutEff.GetXaxis()->SetTitle("m(D#pi) (GeV)");
  FPCutEff.Draw("colz");  
  Canvas.Print(filename);  

  Canvas.Print(filename+"]");  
  delete HMCMassVsPstar;
  delete HMCMass;

  TFile file(_OutputDir+"/Efficiency.root","update");
  file.Delete("FPCutEff;1");
  FPCutEff.Write();
  file.ls();
  file.Close();


  return 1;   

}


Int_t DCPiAnalysis::PlotDetectorEfficiency(){

  if(!_TruthMatch)return 0;

  
  filename=_OutputDir+"/DetectorEfficiency.ps";
  Canvas.Print(filename+"["); 
  
  TFile file(_OutputDir+"/Efficiency.root","update");  
  file.ls();
  TH2F*HEffRaw=(TH2F*)file.Get("H2EfficiencyRawMC");
  Canvas.Clear();
  HEffRaw->SetTitle("");
  HEffRaw->SetStats(0);
  HEffRaw->GetYaxis()->SetTitle("p* (GeV/c)");
  HEffRaw->GetXaxis()->SetTitle("m(D#pi) (GeV/c^{2})");
  HEffRaw->GetZaxis()->SetTitle("Raw Efficiency");
  HEffRaw->GetYaxis()->SetRangeUser(3,5);
  HEffRaw->GetXaxis()->SetRangeUser(2,2.9);
  HEffRaw->Draw("colz");
  Canvas.Print(filename);
  
  TH2F*HEffRawErr=(TH2F*)file.Get("H2EfficiencyRawErrMC");
  Canvas.Clear();
  HEffRawErr->SetTitle("");
  HEffRawErr->SetStats(0);
  HEffRawErr->GetYaxis()->SetTitle("p* (GeV/c)");
  HEffRawErr->GetXaxis()->SetTitle("m(D#pi) (GeV/c^{2})");
  HEffRawErr->GetZaxis()->SetTitle("(Error on Efficiency) / Efficiency ");
  HEffRawErr->GetYaxis()->SetRangeUser(3,5);
  HEffRawErr->GetXaxis()->SetRangeUser(2,2.9);
  HEffRawErr->Draw("colz");
  Canvas.Print(filename);  



  HEffRaw=(TH2F*)file.Get("H2EfficiencyXFitMC");
  Canvas.Clear();
  HEffRaw->SetTitle("");
  HEffRaw->SetStats(0);
  HEffRaw->GetYaxis()->SetTitle("p* (GeV/c)");
  HEffRaw->GetXaxis()->SetTitle("m(D#pi) (GeV/c^{2})");
  HEffRaw->GetZaxis()->SetTitle("Efficiency After X fits");
  HEffRaw->GetYaxis()->SetRangeUser(3,5);
  HEffRaw->GetXaxis()->SetRangeUser(2,2.9);
  HEffRaw->Draw("colz");
  Canvas.Print(filename);
  
  HEffRawErr=(TH2F*)file.Get("H2EfficiencyXFitErrMC");
  Canvas.Clear();
  HEffRawErr->SetTitle("");
  HEffRawErr->SetStats(0);
  HEffRawErr->GetYaxis()->SetTitle("p* (GeV/c)");
  HEffRawErr->GetXaxis()->SetTitle("m(D#pi) (GeV/c^{2})");
  HEffRawErr->GetZaxis()->SetTitle("(Error on Efficiency) / Efficiency ");
  HEffRawErr->GetYaxis()->SetRangeUser(3,5);
  HEffRawErr->GetXaxis()->SetRangeUser(2,2.9);
  HEffRawErr->Draw("colz");
  Canvas.Print(filename);  



  file.Close();
  

  OpenEfficiency();

  Canvas.Clear();
  H2Efficiency->SetTitle("");
  H2Efficiency->SetStats(0);
  H2Efficiency->GetYaxis()->SetTitle("p* (GeV/c)");
  H2Efficiency->GetXaxis()->SetTitle("m(D#pi) (GeV/c^{2})");
  H2Efficiency->GetZaxis()->SetTitle("Fitted Efficiency");
  H2Efficiency->GetYaxis()->SetRangeUser(3,5);
  H2Efficiency->GetXaxis()->SetRangeUser(2,2.9);
  H2Efficiency->Draw("colz");
  Canvas.Print(filename);  



  Canvas.Clear();
  H2EfficiencyErr->SetTitle("");
  H2EfficiencyErr->SetStats(0);
  H2EfficiencyErr->GetYaxis()->SetTitle("p* (GeV/c)");
  H2EfficiencyErr->GetXaxis()->SetTitle("m(D#pi) (GeV/c^{2})");
  H2EfficiencyErr->GetZaxis()->SetTitle("(Error on Efficiency) / Efficiency ");
  H2EfficiencyErr->GetYaxis()->SetRangeUser(3,5);
  H2EfficiencyErr->GetXaxis()->SetRangeUser(2,2.9);
  H2EfficiencyErr->Draw("colz");
  Canvas.Print(filename);  





  Canvas.Print(filename+"]"); 


  CloseEfficiency();

  return 1;   

}


