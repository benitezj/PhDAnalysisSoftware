#include "../Globals.h"
#include "TreeChain.C"
#include "../FUNCTIONS/MoveStatsBox.h"
#include "../FUNCTIONS/Fits.h"
#include "../FUNCTIONS/DetermineFWHM.C"
#include "../FUNCTIONS/SetHisto.C"
#include <TLatex.h>


void plotDstPi0( TString Dir){

  //-------------------------------------------------------
  //Dstars were reconstructed with the following cuts
  //------------------------------------------------------
  //---: prob(D*)>5e-4
  //---: 1.78<mass(D0)<1.94  : SkimCuts are at 1.8:1.93
  //---: .133<deltaM(D*)<.160
  //---: pstar(D*)>1.5
     
  Float_t binsize=.0001;
  Float_t histmin=.130;
  Float_t histmax=.160;
  Int_t nhistbins=(int)((histmax-histmin)/binsize);

  Float_t fitmin=.135;
  Float_t fitmax=.160;

  //build the chain
  TChain tree("ntp1");
  TreeChain(&tree,1,27,Dir+"/DATARun1On");
//   TreeChain(&tree,1,88,Dir+"/DATARun2On");
//   TreeChain(&tree,1,47,Dir+"/DATARun3On");
//   TreeChain(&tree,1,142,Dir+"/DATARun4On");
//   TreeChain(&tree,1,188,Dir+"/DATARun5On");
//   TreeChain(&tree,1,114,Dir+"/DATARun6On");
  
//   TreeChain(&tree,1,4,Dir+"/DATARun1Off");
//   TreeChain(&tree,1,10,Dir+"/DATARun2Off");
//   TreeChain(&tree,1,4,Dir+"/DATARun3Off");
//   TreeChain(&tree,1,14,Dir+"/DATARun4Off");
//   TreeChain(&tree,1,19,Dir+"/DATARun5Off");
//   TreeChain(&tree,1,10,Dir+"/DATARun6Off");
  

  TCanvas Canvas("Canvas","Canvas");
  TString psfile=Dir+"/CountDstars.ps";
  Canvas.Print(psfile+"[");

  tree.SetBranchStatus("*",0);
  tree.SetBranchStatus("XMass",1);
  tree.SetBranchStatus("DstarMass",1);
  tree.SetBranchStatus("D0Mass",1);
  tree.SetBranchStatus("Dstarp3CM",1);
  tree.SetBranchStatus("D0nDaus",1);

  TCut DstarCut="abs(DstarMass-D0Mass-.14207)<(.00118*2)";
  TCut KpiCut="D0nDaus==2";
  TCut K3piCut="D0nDaus==4";
  TCut DstpCut="Dstarp3CM>3&&(DstarMass-D0Mass)<.160";
  TCut KpiD0Cut="abs(D0Mass-1.864)<.020";
  TCut K3piD0Cut="abs(D0Mass-1.864)<.015";
  
  //TTree* cleantree=tree.CopyTree(DstpCut);


  TH1F HDstarDM("HDstarDM","HDstarDM",nhistbins,histmin,histmax);
  SetHistoXYLabels(&HDstarDM,"m(K#pi#pi^{0}) - m(K#pi)  (GeV/c^{2})","GeV");
  HDstarDM.SetStats(0);
  tree.Draw("DstarMass-D0Mass>>HDstarDM",DstpCut+KpiD0Cut);
  TF1 sig;
  TF1 bkg;
  TF1* fitF=FitHistogramDstar0DM(&HDstarDM,fitmin,fitmax,&sig,&bkg); 
  if(!fitF) return;
  

  Float_t fwhm,mean,xmax;
  Float_t integral;

  Char_t txt[100];
  TLatex ltext;
  ltext.SetTextColor(1);
  ltext.SetTextSize(.04);
  
  Canvas.Clear(); 
  HDstarDM.Draw("pe");
  bkg.Draw("same");
  //sig.Draw("same");
  DetermineFWHM(&sig,&fwhm,&mean,&xmax); 
  integral=sig.Integral(fitmin,fitmax); 
  sprintf(txt,"Mean=%.2f MeV",mean*1000);
  ltext.DrawTextNDC(.15,.75,txt);
  sprintf(txt,"HWHM=%.2f MeV",1000*fwhm/2);
  ltext.DrawTextNDC(.15,.7,txt);
  sprintf(txt,"Yield=%.2e",integral/HDstarDM.GetBinWidth(1));
  ltext.DrawTextNDC(.15,.65,txt);
  Canvas.Print(psfile);

 

  TH1F HDstarDMKpi("HDstarDMKpi","",nhistbins,histmin,histmax);
  SetHistoXYLabels(&HDstarDMKpi,"m(K#pi#pi^{0}) - m(K#pi)  (GeV/c^{2})","GeV");
  HDstarDMKpi.SetStats(0);
  tree.Draw("DstarMass-D0Mass>>HDstarDMKpi",DstpCut+KpiCut+KpiD0Cut);
  fitF=FitHistogramDstar0DM(&HDstarDMKpi,fitmin,fitmax,&sig,&bkg); 
  if(!fitF) return;

  
  Canvas.Clear();
  HDstarDMKpi.Draw("pe");
  bkg.Draw("same");
  //sig.Draw("same");
  DetermineFWHM(&sig,&fwhm,&mean,&xmax);
  integral=sig.Integral(fitmin,fitmax); 
  sprintf(txt,"Mean=%.2f MeV",mean*1000);
  ltext.DrawTextNDC(.15,.75,txt);
  sprintf(txt,"HWHM=%.2f MeV",1000*fwhm/2);
  ltext.DrawTextNDC(.15,.7,txt);
  sprintf(txt,"Yield=%.2e",integral/HDstarDM.GetBinWidth(1));
  ltext.DrawTextNDC(.15,.65,txt);
  Canvas.Print(psfile);


  TH1F HDstarDMK3pi("HDstarDMK3pi","",nhistbins,histmin,histmax);
  SetHistoXYLabels(&HDstarDMK3pi,"m(K#pi#pi^{0}) - m(K#pi)  (GeV/c^{2})","GeV");
  HDstarDMK3pi.SetStats(0);
  tree.Draw("DstarMass-D0Mass>>HDstarDMK3pi",DstpCut+K3piCut+K3piD0Cut);
  fitF=FitHistogramDstar0DM(&HDstarDMK3pi,fitmin,fitmax,&sig,&bkg); 
  if(!fitF) return;
  Canvas.Clear();
  HDstarDMK3pi.Draw("pe");
  bkg.Draw("same");
  //sig.Draw("same");
  DetermineFWHM(&sig,&fwhm,&mean,&xmax);
  integral=sig.Integral(fitmin,fitmax); 
  sprintf(txt,"Mean=%.2f MeV",mean*1000);
  ltext.DrawTextNDC(.15,.75,txt);
  sprintf(txt,"HWHM=%.2f MeV",1000*fwhm/2);
  ltext.DrawTextNDC(.15,.7,txt);
  sprintf(txt,"Yield=%.2e",integral/HDstarDM.GetBinWidth(1));
  ltext.DrawTextNDC(.15,.65,txt);
  Canvas.Print(psfile);



  //The D0 mass 
  TLine cutline;
  Float_t d0histbinsize=.001; 
  Float_t d0histmin=1.864-.05;
  Float_t d0histmax=1.864+.05;
  Int_t  d0histbins=(int)((d0histmax-d0histmin)/d0histbinsize); 

  TH1F HD0Mass("HD0Mass","HD0Mass",d0histbins,d0histmin,d0histmax);
  SetHistoXYLabels(&HD0Mass,"m(K#pi)  (GeV/c^{2})","GeV");
  //HD0Mass.SetStats(0);
  tree.Draw("D0Mass>>HD0Mass",DstpCut+DstarCut);  				 
  Canvas.Clear();
  HD0Mass.Draw("pe");
  Canvas.Print(psfile);

  TH1F HD0MassKpi("HD0MassKpi","HD0MassKpi",d0histbins,d0histmin,d0histmax);
  SetHistoXYLabels(&HD0MassKpi,"m(K#pi)  (GeV/c^{2})","GeV");
  //HD0MassKpi.SetStats(0);
  tree.Draw("D0Mass>>HD0MassKpi",DstpCut+KpiCut+DstarCut);
  TF1 Bkg;
  TF1*fitFD0=FitHistogram2gausWithSameMeanPlusPolN(&HD0MassKpi,1.83,1.90,1,0,&Bkg);
  Canvas.Clear();
  HD0MassKpi.Draw("pe");
  Bkg.Draw("same");
  cutline.DrawLine(1.864-.020,0,1.864-.020,HD0MassKpi.GetMaximum());
  cutline.DrawLine(1.864+.020,0,1.864+.020,HD0MassKpi.GetMaximum());
  Canvas.Print(psfile);

  TH1F HD0MassK3pi("HD0MassK3pi","HD0MassK3pi",d0histbins,d0histmin,d0histmax);
  SetHistoXYLabels(&HD0MassK3pi,"m(K#pi)  (GeV/c^{2})","GeV");
  //HD0MassK3pi.SetStats(0);
  tree.Draw("D0Mass>>HD0MassK3pi",DstpCut+K3piCut+DstarCut);
  fitFD0=FitHistogram2gausWithSameMeanPlusPolN(&HD0MassK3pi,1.83,1.90,1,0,&Bkg);
  Canvas.Clear();
  HD0MassK3pi.Draw("pe");
  Bkg.Draw("same");
  cutline.DrawLine(1.864-.015,0,1.864-.015,HD0MassK3pi.GetMaximum());
  cutline.DrawLine(1.864+.015,0,1.864+.015,HD0MassK3pi.GetMaximum());
  Canvas.Print(psfile);
         
  Canvas.Print(psfile+"]");

  //delete cleantree;

  return;
}



