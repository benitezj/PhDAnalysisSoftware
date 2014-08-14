#include "DstPiAnalysis.h"



Int_t DstPiAnalysis::FitVsPolOrder(Bool_t fit){
  if(_TruthMatch)return 0;
  cout<<" Fit vs  Polynomial order. "<<endl;
 
  Bool_t nofile=0;
  
  TGraph GChisquare;
  Float_t chi=0;
  Float_t ndf=0;
  TGraph GD2420Mean;
  TGraph GD2420Width;
  TGraph GD2460Mean;
  TGraph GD2460Width;

  TString oldoutdir=_OutputDir;
  _OutputDir=_OutputDir+"/PolOrderFits";

  
  for(Int_t i=2;i<=7;i++){
    roofitconfigfile=TString("InitRooFitPars_")+(long)i+".txt";
    nofile=!OpenFitPars(TString("FitPars_")+(long)i+".txt");
    if(fit||nofile){
      RooFitMassPeak(5);
      system(TString("mv ")+_OutputDir+"/FitPars"+roofitfileid+".txt "+_OutputDir+"/FitPars_"+(long)i+".txt");
      system(TString("mv ")+_OutputDir+"/DstPiRooFit"+roofitfileid+".ps "+_OutputDir+"/FitPars_"+(long)i+".ps");
      if(!OpenFitPars(TString("FitPars_")+(long)i+".txt"))return 0;  
    }    
    GD2420Mean.SetPoint(i-2,(double)i,GetFitPar("D2420","mean")*1000);
    GD2420Width.SetPoint(i-2,(double)i,GetFitPar("D2420","width")*1000);
    GD2460Mean.SetPoint(i-2,(double)i,GetFitPar("D2460","mean")*1000);
    GD2460Width.SetPoint(i-2,(double)i,GetFitPar("D2460","width")*1000);

    chi=GetFitPar("Range","chisquare");
    ndf=GetFitPar("Range","ndf");
    GChisquare.SetPoint(i-2,i,chi/ndf); 
    CloseFitPars();
  }


  //save plots
  filename=_OutputDir+"/PolOrderFits.ps";
  Canvas.Print(filename+"["); 

  Canvas.Clear();
  GChisquare.Draw("ape");
  GChisquare.SetMarkerSize(2);  
  GChisquare.GetXaxis()->SetTitle("polynomial order");
  GChisquare.GetYaxis()->SetTitle("#chi^{2}/DF");
  GChisquare.GetYaxis()->SetNdivisions(6);
  Canvas.Update();
  Canvas.Print(filename);

  Canvas.Clear();
  GD2420Mean.Draw("ape");
  GD2420Mean.SetMarkerSize(2);  
  GD2420Mean.GetXaxis()->SetTitle("polynomial order");
  GD2420Mean.GetYaxis()->SetTitle("D_{1}(2420) Mass (MeV)");
  GD2420Mean.GetYaxis()->SetNdivisions(6);
  Canvas.Update();
  Canvas.Print(filename);
  Canvas.Clear();
  GD2420Width.Draw("ape");
  GD2420Width.SetMarkerSize(2);  
  GD2420Width.GetXaxis()->SetTitle("polynomial order");
  GD2420Width.GetYaxis()->SetTitle("D_{1}(2420) Width (MeV)");
  GD2420Width.GetYaxis()->SetNdivisions(6);
  Canvas.Update();
  Canvas.Print(filename);
  Canvas.Clear();
  GD2460Mean.Draw("ape");
  GD2460Mean.SetMarkerSize(2);  
  GD2460Mean.GetXaxis()->SetTitle("polynomial order");
  GD2460Mean.GetYaxis()->SetTitle("D_{2}*(2460) Mass (MeV)");
  GD2460Mean.GetYaxis()->SetNdivisions(6);
  Canvas.Update();
  Canvas.Print(filename);
  Canvas.Clear();
  GD2460Width.Draw("ape");
  GD2460Width.SetMarkerSize(2);  
  GD2460Width.GetXaxis()->SetTitle("polynomial order");
  GD2460Width.GetYaxis()->SetTitle("D_{2}*(2460) Width (MeV)");
  GD2460Width.GetYaxis()->SetNdivisions(6);
  Canvas.Update();
  Canvas.Print(filename);

  Canvas.Print(filename+"]"); 

  roofitconfigfile="InitRooFitPars.txt";
  _OutputDir=oldoutdir;
  return 1;
}

