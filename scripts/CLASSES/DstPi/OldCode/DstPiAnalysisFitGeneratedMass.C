#include "DstPiAnalysis.h"
#include "../RooLwaveBreitWigner.h"
#include "../../MACROS/TreeChain.C"

Int_t  DstPiAnalysis::FitGeneratedMass(Int_t nfiles){
  
  Int_t whichpeak=0;
  if(_OutputDir=="DstarCharged/DstarPi/MC/D2420/Kpi")whichpeak=1;
  if(_OutputDir=="DstarCharged/DstarPi/MC/D2460/Kpi")whichpeak=2;
  if(whichpeak==0){
    cout<<"This operation only allowed for Signal MC Kpi mode"<<endl;
    return 0;
  }  


  //------------------------
  //choose the peak
  //-----------------------
  Int_t LundId=10423;
  Float_t meanmass=2.422;
  Int_t nwindows=75;
  Int_t L=0;
  
  if(whichpeak==1)
    cout<<"Going to fit the D(2420)"<<endl;

  if(whichpeak==2){
    cout<<"Going to fit the D(2460)"<<endl;
    LundId=425;
    meanmass=2.459;
    nwindows=100;
    L=2;
  }

  //----------------------------------------------------------------//

  TCut lundcut=TCut(TString("abs(mcLund)==")+(long)LundId);    
  TCut lundcutpos=TCut(TString("mcLund==")+(long)LundId);
  TCut lundcutneg=TCut(TString("mcLund==-")+(long)LundId);


  filename=_OutputDir+"FitGeneratedMass.ps"; 
  //TCanvas Canvas("Canvas");
  Canvas.Print(filename+"[");

  TChain tree("ntp1");
  TreeChain(&tree,1,nfiles,_OutputDir);
  
  //--------------------------
  // Notes:
  //1.  Turns out the mass values from mcMass are discretized in steps of about 1/7 MeV 
  //and not alinged with 1MeV values, should use bins which are big enough
  //2. Fit parameters are same for particles and anti-particles.  
  //-----------------------------
  Float_t binwidth=.001;
  Float_t window=nwindows*binwidth;
  Float_t min=meanmass-window;
  Float_t max=meanmass+window;
  Int_t nbins=(int)((max-min)/binwidth);
  TH1F HMass("HMass","HMass",nbins,min,max);
  tree.Draw("mcmass>>HMass",lundcut);
  TF1* MassFit=FitHistogramBreitWigner(&HMass,min,max);
  //delete HMass.FindObject(MassFit->GetName());
  JResiduals MassRes("res",&HMass,MassFit);     
  Canvas.Clear();
  MassRes.MakeFitPlotWithResiduals(&Canvas,0);
  Canvas.Print(filename);
  

  
  ///show discreteness of Mass
  TH1F HMassPos("HMassPos","HMassPos",500,2.422,2.4225);
  tree.Draw("mcmass>>HMassPos",lundcutpos);

  TH1F HMassNeg("HMassNeg","HMassNeg",500,2.422,2.4225);
  HMassNeg.SetMarkerColor(4);
  tree.Draw("mcmass>>HMassNeg",lundcutneg);
  Canvas.Clear();
  HMassPos.GetXaxis()->SetNdivisions(5);
  HMassPos.SetStats(0);
  HMassPos.Draw("pe");
  HMassNeg.Draw("pesame");
  Canvas.Print(filename);



  ///fill histo by randomizing the discrete mass 
  TH1F HMassNew("HMassNew","HMassNew",nbins,min,max);
  sprintf(textlabel,"1*(-%.5f<x&&x<%.5f)",binwidth/2.,binwidth/2.);
  TF1 Flat("Flat",textlabel,-binwidth/2,binwidth/2);
  
  Int_t mcLen;
  Float_t mcmass[1000];  
  Int_t mcLund[1000];
  tree.SetBranchAddress("mcLen",&mcLen);
  tree.SetBranchAddress("mcmass",mcmass);
  tree.SetBranchAddress("mcLund",mcLund);
  tree.SetBranchStatus("*",0);
  tree.SetBranchStatus("mcLen",1);
  tree.SetBranchStatus("mcmass",1);
  tree.SetBranchStatus("mcLund",1);

  Int_t i=0;
  while(tree.GetEntry(i,0)>0){    
    //cout<<i<<" "<<mcLen<<endl;
    
    for(Int_t m=0;m<mcLen;m++){
      if(abs(mcLund[m])==LundId)
	HMassNew.Fill(mcmass[m]+Flat.GetRandom());
    }
    
    i++;
    if(i%10000==0)cout<<i<<endl;
  } 
 

  //Fit with a non-relativistic BW
  TF1* MassNewFit=FitHistogramBreitWigner(&HMassNew,min,max);
  JResiduals MassNewRes("res2",&HMassNew,MassNewFit);     
  delete HMassNew.FindObject(MassNewFit->GetName());
  delete HMassNew.FindObject("stats");  
  HMassNew.SetStats(0);
  Canvas.Clear();
  MassNewRes.MakeFitPlotWithResiduals(&Canvas,0);
  Canvas.cd(0);
  TPaveText stats(.6,.6,.9,.9,"NDC");
  stats.SetFillStyle(3001);
  stats.SetFillColor(0);
  stats.AddText("FitResults:");
  sprintf(textlabel,"#chi^{2}/ndf=%.1f/%d",MassNewFit->GetChisquare(),MassNewFit->GetNDF());
  stats.AddText(textlabel);
  sprintf(textlabel,"M=%.5f+-%.5f",MassNewFit->GetParameter(1),MassNewFit->GetParError(1));
  stats.AddText(textlabel);
  sprintf(textlabel,"#Gamma=%.5f+-%.5f",MassNewFit->GetParameter(2),MassNewFit->GetParError(2));
  stats.AddText(textlabel);
  stats.Draw();
  Canvas.Print(filename);
  

  ///RooFit 
  RooRealVar mass("mass","mass",min,max);mass.setBins(nbins);
  RooRealVar mean("mean","mean",min,max);
  RooRealVar width("width","width",0,.1);
  RooLwaveBreitWigner BWPdf("BWPdf","BWPdf",mass,mean,width);
  BWPdf.Configure(DstarPDGMass,PiPDGMass,0,3,L,2);
  RooDataHist data("data","data",mass,&HMassNew);
  BWPdf.fitTo(data,"mhrt");
  RooPlot* frame=mass.frame();
  data.plotOn(frame);
  BWPdf.plotOn(frame);
  TH1*FitHisto=BWPdf.createHistogram("FitHisto",mass);//,Normalization(HMassNew.Integral()));
  FitHisto->Scale(HMassNew.Integral());
  JResiduals MassRooRes("res3",&HMassNew,FitHisto);     
  Canvas.Clear();
  MassRooRes.MakeFitPlotWithResiduals(&Canvas,frame);
  Canvas.cd(0);
  TPaveText roostats(.6,.6,.9,.9,"NDC");
  roostats.SetFillStyle(3001);
  roostats.SetFillColor(0);
  roostats.AddText("FitResults:");
  sprintf(textlabel,"#chi^{2}/ndf=%.1f/%d",MassRooRes.GetChisquare(),HMassNew.GetXaxis()->GetNbins()-3);
  roostats.AddText(textlabel);
  sprintf(textlabel,"M=%.5f+-%.5f",mean.getVal(),mean.getError());
  roostats.AddText(textlabel);
  sprintf(textlabel,"#Gamma=%.5f+-%.5f",width.getVal(),width.getError());
  roostats.AddText(textlabel);
  roostats.Draw();
  Canvas.Print(filename);


  Canvas.Print(filename+"]");
 
  return 1;
}
