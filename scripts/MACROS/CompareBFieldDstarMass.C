#include "SOFTWARE/Globals.h"
#include "SOFTWARE/FUNCTIONS/Fits.h"
#include "SOFTWARE/MACROS/TreeChain.C"
#include <TLatex.h>

void MakeDstarBFieldPlots(){
  TChain cn("ntp1002");
//   TreeChain(&cn,2,3,"DstarCharged/DstarPi/BFieldChange/DATARun1Off");
//   TreeChain(&cn,1,1,"DstarCharged/DstarPi/BFieldChange/DATARun1On");
//   TreeChain(&cn,8,8,"DstarCharged/DstarPi/BFieldChange/DATARun1On");
//   TreeChain(&cn,10,10,"DstarCharged/DstarPi/BFieldChange/DATARun1On");
//   TreeChain(&cn,13,13,"DstarCharged/DstarPi/BFieldChange/DATARun1On");
//   TreeChain(&cn,15,15,"DstarCharged/DstarPi/BFieldChange/DATARun1On");
//   TreeChain(&cn,19,19,"DstarCharged/DstarPi/BFieldChange/DATARun1On");



  TChain co("ntp1");
//   TreeChain(&co,2,3,"DstarCharged/DstarPi/DATARun1Off");
//   TreeChain(&co,1,1,"DstarCharged/DstarPi/DATARun1On");
//   TreeChain(&co,8,8,"DstarCharged/DstarPi/DATARun1On");
//   TreeChain(&co,10,10,"DstarCharged/DstarPi/DATARun1On");
//   TreeChain(&co,13,13,"DstarCharged/DstarPi/DATARun1On");
//   TreeChain(&co,15,15,"DstarCharged/DstarPi/DATARun1On");
//   TreeChain(&co,19,19,"DstarCharged/DstarPi/DATARun1On");
  
  TFile F("CompareDstarMassBField.root","recreate"); 
  TH1F hn("hn","hn",300,.130,.160);
  cn.Draw("DstarMass-D0Mass>>hn");
  hn.Write();
  
  TH1F ho("ho","ho",300,.130,.160); 
  co.Draw("DstarMass-D0Mass>>ho");
  ho.Write();
  F.Close();

}

void FitDstarBFieldPlots(){
 TFile F("CompareDstarMassBField.root","read"); 
 TF1 sig;
 TF1 bkg;


 TCanvas C("C");
 C.Print("CompareDstarMassBField.ps[");

 TH1F* hn=(TH1F*)F.Get("hn");
 FitHistogramDstarDM(hn,.1415,.153,&sig,&bkg);
 C.Clear();
 hn->Draw();
 bkg.Draw("same");
 C.Print("CompareDstarMassBField.ps");


 TH1F* ho=(TH1F*)F.Get("ho");
 FitHistogramDstarDM(ho,.1415,.153,&sig,&bkg);
 C.Clear();
 ho->Draw();
 bkg.Draw("same");
 C.Print("CompareDstarMassBField.ps");

 C.Print("CompareDstarMassBField.ps]");
 F.Close();

}
