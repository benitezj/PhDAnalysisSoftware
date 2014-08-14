#include "../Globals.h"
#include "../AnalysisGlobals.h"
#include "../CLASSES/Roo2BodyMassPhaseSpace.h"
#include "../CLASSES/RooSwaveBreitWigner.h"
#include "../CLASSES/RooDwaveBreitWigner.h"
#include "RooBreitWigner.h"
#include "../CLASSES/RooRelBreitWigner.h"
#include "RooPlot.h"
#include "RooRealVar.h"
#include "RooProdPdf.h"
#include "RooGlobalFunc.h"
using namespace RooFit;

void CompareBreitWigners(){
   
  RooRealVar dau1m("dau1m","dau1m",DPlusPDGMass);
  RooRealVar dau2m("dau2m","dau2m",PiPDGMass);
  RooRealVar mass("mass","mass",2.3,2.7);
  Roo2BodyMassPhaseSpace TwoBodyPS("TwoBodyPS","TwoBodyPS",mass,dau1m,dau2m);

  RooRealVar mean("mean","mean",2.460);
  RooRealVar width("width","width",.04);

  RooBreitWigner bw("bw","bw",mass,mean,width);
  RooRelBreitWigner rbw("rbw","rbw",mass,mean,width);
  
  RooRealVar radius("radius","radius",5.);
  RooSwaveBreitWigner sbw("sbw","sbw",&mass,mean,width,&TwoBodyPS);
  RooDwaveBreitWigner dbw("dbw","dbw",&mass,mean,width,&TwoBodyPS,radius);

  RooProdPdf pbw("pbw","pbw",TwoBodyPS,bw);
  RooProdPdf psbw("psbw","psbw",TwoBodyPS,sbw);
  RooProdPdf pdbw("pdbw","pdbw",TwoBodyPS,dbw);
  


  TCanvas C("C");
  RooPlot*pl=mass.frame();
  pbw.plotOn(pl,LineColor(1));
  psbw.plotOn(pl,LineColor(2));
  pdbw.plotOn(pl,LineColor(3));
  pl->SetTitle("");
  pl->GetXaxis()->SetTitle("");
  pl->GetXaxis()->SetTitle("x");
  pl->Draw();

  C.Print("CompareBreitWigners.ps");
   
  delete pl;
}
