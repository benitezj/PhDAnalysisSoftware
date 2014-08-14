#include "../../Globals.h"
#include "../../MACROS/TreeChain.C"
#include "../../FUNCTIONS/MoveStatsBox.h"
#include "../../FUNCTIONS/Fits.h"
#include "../../FUNCTIONS/DetermineFWHM.C"
#include "../../FUNCTIONS/SetHisto.C"
#include <TLatex.h>

#define DSTARDELTAM .14205
#define DSTARWINDOW .002
#define D0PEAKMASS 1.864
#define D0WINDOWKPI .020
#define D0WINDOWK3PI .015
//#define GAMMAECUT .090

void MakeDstar0Plots(Float_t GAMMAECUT, TString fileid){

  TString Dir="Dstar0/Dstar";

  //-------------------------------------------------------
  //Dstars were reconstructed with the following cuts
  //------------------------------------------------------

  Float_t binsize=.00001;//same binning as D*+
  Float_t histmin=.130;
  Float_t histmax=.160;
  Int_t nhistbins=(int)((histmax-histmin)/binsize);
 
  Float_t d0histbinsize=.001; 
  Float_t d0histmin=1.865-.05;
  Float_t d0histmax=1.865+.05;
  Int_t  d0histbins=(int)((d0histmax-d0histmin)/d0histbinsize); 

  //build the chain
  TChain tree("ntp1");
  TreeChain(&tree,1,27,Dir+"/DATARun1On");//27
  TreeChain(&tree,1,88,Dir+"/DATARun2On");
  TreeChain(&tree,1,47,Dir+"/DATARun3On");
  TreeChain(&tree,1,142,Dir+"/DATARun4On");
  TreeChain(&tree,1,188,Dir+"/DATARun5On");
  TreeChain(&tree,1,114,Dir+"/DATARun6On");
  
  TreeChain(&tree,1,4,Dir+"/DATARun1Off");
  TreeChain(&tree,1,10,Dir+"/DATARun2Off");
  TreeChain(&tree,1,4,Dir+"/DATARun3Off");
  TreeChain(&tree,1,14,Dir+"/DATARun4Off");
  TreeChain(&tree,1,19,Dir+"/DATARun5Off");
  TreeChain(&tree,1,10,Dir+"/DATARun6Off");
  
  tree.SetBranchStatus("*",0);
  tree.SetBranchStatus("DstarVtxStatus",1);
  tree.SetBranchStatus("DstarMass",1);
  tree.SetBranchStatus("D0Mass",1);
  tree.SetBranchStatus("Dstarp3CM",1);
  tree.SetBranchStatus("Pi0d1Idx",1);
  tree.SetBranchStatus("Pi0d2Idx",1);
  tree.SetBranchStatus("Gamp3",1);
  tree.SetBranchStatus("nDstar",1);
  tree.SetBranchStatus("D0nDaus",1);


  Float_t DstarVtxStatus[100];
  Float_t DstarMass[100];
  Float_t D0Mass[100];
  Int_t Pi0d1Idx[100];
  Int_t Pi0d2Idx[100];
  Float_t Dstarp3CM[100];
  Float_t Gamp3[100];
  Int_t nDstar;
  Int_t D0nDaus[100];  
  tree.SetBranchAddress("DstarVtxStatus",DstarVtxStatus);
  tree.SetBranchAddress("DstarMass",DstarMass);
  tree.SetBranchAddress("D0Mass",D0Mass);
  tree.SetBranchAddress("Pi0d1Idx",Pi0d1Idx);
  tree.SetBranchAddress("Pi0d2Idx",Pi0d2Idx);
  tree.SetBranchAddress("Dstarp3CM",Dstarp3CM);
  tree.SetBranchAddress("Gamp3",Gamp3);
  tree.SetBranchAddress("nDstar",&nDstar);
  tree.SetBranchAddress("D0nDaus",D0nDaus);  


  TH1F HDstarDMKpi("HDstarDMKpi","HDstarDMKpi",nhistbins,histmin,histmax);  
  TH1F HDstarDMK3pi("HDstarDMK3pi","HDstarDMK3pi",nhistbins,histmin,histmax);
  
  TH1F HGamE1("HGamE1","HGamE1",50,0,.5);
  TH1F HGamE2("HGamE2","HGamE2",50,0,.5);

  //The D0 mass Histograms  
  TH1F HD0MassKpi("HD0MassKpi","HD0MassKpi",d0histbins,d0histmin,d0histmax);
  TH1F HD0MassK3pi("HD0MassK3pi","HD0MassK3pi",d0histbins,d0histmin,d0histmax);
        
  //loop
  Int_t evid=0;//5170000;
  while(tree.GetEntry(evid,0)>0){    
    if(evid%10000==0)cout<<evid<<" events done"<<endl;
    evid++;
    
    if(nDstar>50)continue;
    
    for(Int_t DstarIdx=0;DstarIdx<nDstar;DstarIdx++){
     
      if(DstarVtxStatus[DstarIdx]!=0||Dstarp3CM[DstarIdx]<3||(DstarMass[DstarIdx]-D0Mass[DstarIdx])>.160)continue;

      if(fabs(DstarMass[DstarIdx]-D0Mass[DstarIdx]-DSTARDELTAM) < DSTARWINDOW)
	if(fabs(D0Mass[DstarIdx]-D0PEAKMASS)<D0WINDOWKPI){
	  HGamE1.Fill(Gamp3[Pi0d1Idx[DstarIdx]]);
	  HGamE2.Fill(Gamp3[Pi0d2Idx[DstarIdx]]);
	}

      if(Gamp3[Pi0d1Idx[DstarIdx]]<GAMMAECUT||Gamp3[Pi0d2Idx[DstarIdx]]<GAMMAECUT)continue;      
      if(D0nDaus[DstarIdx]==2){
	if(fabs(DstarMass[DstarIdx]-D0Mass[DstarIdx]-DSTARDELTAM) < DSTARWINDOW) HD0MassKpi.Fill(D0Mass[DstarIdx]);
	if(fabs(D0Mass[DstarIdx]-D0PEAKMASS)<D0WINDOWKPI)HDstarDMKpi.Fill(DstarMass[DstarIdx]-D0Mass[DstarIdx]);
      }
      if(D0nDaus[DstarIdx]==4){
	if(fabs(DstarMass[DstarIdx]-D0Mass[DstarIdx]-DSTARDELTAM)<DSTARWINDOW)HD0MassK3pi.Fill(D0Mass[DstarIdx]);
	if(fabs(D0Mass[DstarIdx]-D0PEAKMASS)<D0WINDOWK3PI)HDstarDMK3pi.Fill(DstarMass[DstarIdx]-D0Mass[DstarIdx]);
      }
      
    }
  }
  
  //save the histograms for later fitting
  TFile SavedPlots(Dir+"/SavedPlots_"+fileid+".root","recreate",Dir+"SavedPlots_"+fileid+".root",4);  
      
  HDstarDMKpi.Write();  
  HDstarDMK3pi.Write();  
  HD0MassKpi.Write();
  HD0MassK3pi.Write();
  HGamE1.Write();
  HGamE2.Write();

  SavedPlots.ls();//turns out this is quite important for the objects to be written.
  SavedPlots.Close();

  
  return;
}

void FitDstar0Plots(TString fileid){
  TString Dir="Dstar0/Dstar";

  TFile SavedPlots(Dir+"/SavedPlots_"+fileid+".root","read");  
  SavedPlots.ls();
  gROOT->cd();

  Float_t fitmin=.135;
  Float_t fitmax=.160;

  Char_t txt[100];
  TLatex ltext;
  ltext.SetTextColor(1);
  ltext.SetTextSize(.04);
  
  TLine cutline;

  TCanvas Canvas("Canvas","Canvas");
  TString psfile=Dir+"/CountDstars_"+fileid+".ps";
  Canvas.Print(psfile+"[");


  TF1 sig;
  TF1 bkg;
  TF1* fitF=NULL;
  Float_t fwhm,mean,xmax;
  Float_t integral;  
  Float_t bkgintegral;  
  Float_t hwhmerror;
  Float_t meanerror;

  TH1F* HDstarDMKpi=(TH1F*)SavedPlots.Get("HDstarDMKpi");
  SetHistoXYLabels(HDstarDMKpi,"m(K^{-}#pi^{+}#pi^{0}) - m(K^{-}#pi^{+})  (GeV/c^{2})","GeV");
  HDstarDMKpi->SetStats(0);
  fitF=FitHistogramDstar0DM(HDstarDMKpi,fitmin,fitmax,&sig,&bkg); 
  if(!fitF) return;  
  Canvas.Clear();
  HDstarDMKpi->Draw("pe");
  bkg.Draw("same");
  //sig.Draw("same");
  DetermineFWHM(&sig,&fwhm,&mean,&xmax);
  hwhmerror=(sig.GetParameter(2)*sig.GetParError(2) + (sig.GetParameter(3)*sig.GetParameter(4))*sig.GetParError(4))/(sig.GetParameter(2)+(sig.GetParameter(3)*sig.GetParameter(4)));//this returns the error on the "sigma"  
  hwhmerror=1.18*hwhmerror;//HWHM is 1.18*sigma
  meanerror=sig.GetParError(1);
  integral=sig.Integral(fitmin,fitmax); 
  bkgintegral=bkg.Integral(DSTARDELTAM-DSTARWINDOW,DSTARDELTAM+DSTARWINDOW);
  sprintf(txt,"Mean=%.3f +- %.3f MeV",mean*1000,meanerror*1000);
  ltext.DrawTextNDC(.45,.75,txt);
  sprintf(txt,"HWHM=%.3f +- %.3f MeV",1000*fwhm/2,hwhmerror*1000);
  ltext.DrawTextNDC(.45,.7,txt);
  sprintf(txt,"Yield=%.2e +- %.2e",integral/HDstarDMKpi->GetBinWidth(1),sqrt(integral/HDstarDMKpi->GetBinWidth(1)));
  ltext.DrawTextNDC(.45,.65,txt);
  sprintf(txt,"Bkg Yield=%.2e +- %.2e",bkgintegral/HDstarDMKpi->GetBinWidth(1),sqrt(bkgintegral/HDstarDMKpi->GetBinWidth(1)));
  ltext.DrawTextNDC(.45,.55,txt);
  sprintf(txt,"(in +-2MeV window)");
  ltext.DrawTextNDC(.45,.5,txt);
  Canvas.Print(psfile);


  TH1F* HDstarDMK3pi=(TH1F*)SavedPlots.Get("HDstarDMK3pi");
  SetHistoXYLabels(HDstarDMK3pi,"m(K^{-}#pi^{+}#pi^{-}#pi^{+}#pi^{0}) - m(K^{-}#pi^{+}#pi^{-}#pi^{+})  (GeV/c^{2})","GeV");
  HDstarDMK3pi->SetStats(0);
  fitF=FitHistogramDstar0DM(HDstarDMK3pi,fitmin,fitmax,&sig,&bkg); 
  if(!fitF) return;
  Canvas.Clear();
  HDstarDMK3pi->Draw("pe");
  bkg.Draw("same");
  //sig.Draw("same");
  DetermineFWHM(&sig,&fwhm,&mean,&xmax);
  hwhmerror=(sig.GetParameter(2)*sig.GetParError(2) + (sig.GetParameter(3)*sig.GetParameter(4))*sig.GetParError(4))/(sig.GetParameter(2)+(sig.GetParameter(3)*sig.GetParameter(4)));//this returns the error on the "sigma"
  hwhmerror=1.18*hwhmerror;//HWHM is 1.18*sigma
  meanerror=sig.GetParError(1);

  integral=sig.Integral(fitmin,fitmax); 
  bkgintegral=bkg.Integral(DSTARDELTAM-DSTARWINDOW,DSTARDELTAM+DSTARWINDOW);
  sprintf(txt,"Mean=%.3f +- %.3f MeV",mean*1000,meanerror*1000);
  ltext.DrawTextNDC(.45,.75,txt);
  sprintf(txt,"HWHM=%.3f +- %.3f MeV",1000*fwhm/2,hwhmerror*1000);
  ltext.DrawTextNDC(.45,.7,txt);
  sprintf(txt,"Yield=%.2e +- %.2e",integral/HDstarDMKpi->GetBinWidth(1),sqrt(integral/HDstarDMKpi->GetBinWidth(1)));
  ltext.DrawTextNDC(.45,.65,txt);
  sprintf(txt,"Bkg Yield=%.2e +- %.2e",bkgintegral/HDstarDMKpi->GetBinWidth(1),sqrt(bkgintegral/HDstarDMKpi->GetBinWidth(1)));
  ltext.DrawTextNDC(.45,.55,txt);
  sprintf(txt,"(in +-2MeV window)");
  ltext.DrawTextNDC(.45,.5,txt);
  Canvas.Print(psfile);


  TH1F* HD0MassKpi=(TH1F*)SavedPlots.Get("HD0MassKpi");
  SetHistoXYLabels(HD0MassKpi,"m(K^{-}#pi^{+})  (GeV/c^{2})","GeV");
  //HD0MassKpi->SetStats(0);
  TF1 Bkg;
  TF1*fitFD0=FitHistogram2gausWithSameMeanPlusPolN(HD0MassKpi,1.83,1.90,1,0,&Bkg);
  Canvas.Clear();
  HD0MassKpi->Draw("pe");
  Bkg.Draw("same");
  cutline.DrawLine(D0PEAKMASS-D0WINDOWKPI,0,D0PEAKMASS-D0WINDOWKPI,HD0MassKpi->GetMaximum());
  cutline.DrawLine(D0PEAKMASS+D0WINDOWKPI,0,D0PEAKMASS+D0WINDOWKPI,HD0MassKpi->GetMaximum());
  Canvas.Print(psfile);

  TH1F* HD0MassK3pi=(TH1F*)SavedPlots.Get("HD0MassK3pi");
  SetHistoXYLabels(HD0MassK3pi,"m(K^{-}#pi^{+}#pi^{-}#pi^{+})  (GeV/c^{2})","GeV");
  //HD0MassK3pi->SetStats(0);
  fitFD0=FitHistogram2gausWithSameMeanPlusPolN(HD0MassK3pi,1.83,1.90,1,0,&Bkg);
  Canvas.Clear();
  HD0MassK3pi->Draw("pe");
  Bkg.Draw("same");
  cutline.DrawLine(D0PEAKMASS-D0WINDOWK3PI,0,D0PEAKMASS-D0WINDOWK3PI,HD0MassK3pi->GetMaximum());
  cutline.DrawLine(D0PEAKMASS+D0WINDOWK3PI,0,D0PEAKMASS+D0WINDOWK3PI,HD0MassK3pi->GetMaximum());
  Canvas.Print(psfile);
         


  TH1F* HGamE1=(TH1F*)SavedPlots.Get("HGamE1");
  TH1F* HGamE2=(TH1F*)SavedPlots.Get("HGamE2");
  Canvas.Clear();
  HGamE1->Draw();
  HGamE2->Draw("same");
  //cutline.DrawLine(GAMMAECUT,0,GAMMAECUT,HGamE1->GetMaximum());
  Canvas.Print(psfile);

  Canvas.Print(psfile+"]");

  SavedPlots.Close();
  

  return;
}




