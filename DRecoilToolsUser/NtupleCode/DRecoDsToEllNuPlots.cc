#include "DRecoilToolsUser/NtupleCode/DRecoDsToEllNuPlots.hh"
#include "DRecoilToolsUser/NtupleCode/DRecoResiduals.hh"
#include "DRecoilToolsUser/NtupleCode/DRecoHistPdfChi2Var.hh"
#include "DRecoilToolsUser/NtupleCode/DRecoHistPdf.hh"
#include "DRecoilToolsUser/NtupleCode/DRecoAddPdf.hh"
#include "DRecoilToolsUser/NtupleCode/DRecoDsInclusiveAnalysis.hh"

#include "TAxis.h"
#include "TRandom.h"
#include "TRandom3.h"
#include "RooBreitWigner.h"
#include "RooChebychev.h"
#include "RooDataHist.h"
#include "RooExponential.h"
#include "RooGaussian.h"
#include "RooPolynomial.h"
#include "RooHistPdf.h"
#include "RooKeysPdf.h"
#include "RooChi2Var.h"
#include "RooMinuit.h"
#include "RooFitResult.h"
#include "RooGenericPdf.h"
#include "RooProdPdf.h"
#include "RooPlot.h"
#include "RooAddPdf.h"
#include "RooGlobalFunc.h"
#include "RooCmdArg.h"
#include "TH2F.h"
#include "TList.h"
#include "TFile.h"
#include "TTree.h"
#include "THStack.h"
#include "TStyle.h"
#include "TPaveLabel.h"
#include "TPaveText.h"

using namespace RooFit ;

ClassImp(DRecoDsToEllNuPlots) ;
DRecoDsToEllNuPlots::DRecoDsToEllNuPlots():DRecoDsInclusiveAnalysis(1,"DsInclusiveAnalysis",0)
{
  // Style information
  gROOT->SetStyle("BABAR") ;
  gStyle->SetOptStat(kFALSE) ;
  gStyle->SetPalette(1,0) ;
  gStyle->SetMarkerStyle(20) ;

  // Just declare a few useful things here
  SPModeName[0]  = "SP998"  ;
  SPModeName[1]  = "SP1005" ;
  SPModeName[2]  = "SP1235" ;
  SPModeName[3]  = "SP1237" ;
  SPModeName[4]  = "SP3429" ;
  SPModeName[5]  = "SP3046" ;
  SPModeName[6]  = "SP3047" ;
  SPModeName[7]  = "SP9756" ;
  SPModeName[8]  = "SP9757" ;
  SPModeName[9]  = "SP9758" ;
  SPModeName[10] = "Data"   ;
  
  RunName[0]  = "Run1On"    ;
  RunName[1]  = "Run1Off"   ;
  RunName[2]  = "Run2On"    ;
  RunName[3]  = "Run2Off"   ;
  RunName[4]  = "Run3On"    ;
  RunName[5]  = "Run3Off"   ;
  RunName[6]  = "Run4On"    ;
  RunName[7]  = "Run4Off"   ;
  RunName[8]  = "Run5On"    ;
  RunName[9]  = "Run5Off"   ;
  RunName[10] = "Run6On"    ;
  RunName[11] = "Run6Off"   ;
  RunName[12] = "RunY3SOn"  ;
  RunName[13] = "RunY3SOff" ;
  RunName[14] = "RunY2SOn"  ;
  RunName[15] = "RunY2SOff" ;
  RunName[16] = "RunAll"    ;

  NXRName[0] = "NXR0"   ; NXRTitle[0] = "0"   ;
  NXRName[1] = "NXR1"   ; NXRTitle[1] = "1"   ;
  NXRName[2] = "NXR2"   ; NXRTitle[2] = "2"   ;
  NXRName[3] = "NXR3"   ; NXRTitle[3] = "3"   ;
  NXRName[4] = "NXRAll" ; NXRTitle[4] = "All" ;

  SignalModeName[0]  = "DsToMuNu"             ; SignalModeTitle[0] = "D_{s}^{+}#rightarrow#mu#nu"                           ; SignalModeColor[0] =  3 ; SignalModeNameLaTeX[0] = "\\DsToMuNu"             ;
  SignalModeName[1]  = "DsToENu"              ; SignalModeTitle[1] = "D_{s}^{+}#rightarrowe#nu"                             ; SignalModeColor[1] = 51 ; SignalModeNameLaTeX[1] = "\\DsToENu"              ;
  SignalModeName[2]  = "DsToTauNuTauToMuNuNu" ; SignalModeTitle[2] = "D_{s}^{+}#rightarrow#tau#nu;#tau#rightarrow#mu#nu#nu" ; SignalModeColor[2] =  6 ; SignalModeNameLaTeX[2] = "\\DsToTauNuTauToMuNuNu" ;
  SignalModeName[3]  = "DsToTauNuTauToENuNu"  ; SignalModeTitle[3] = "D_{s}^{+}#rightarrow#tau#nu;#tau#rightarrowe#nu#nu"   ; SignalModeColor[3] = 93 ; SignalModeNameLaTeX[3] = "\\DsToTauNuTauToENuNu"  ;
  SignalModeName[4]  = "DsToTauNuTauToPiNu"   ; SignalModeTitle[4] = "D_{s}^{+}#rightarrow#tau#nu;#tau#rightarrow#pi#nu"    ; SignalModeColor[4] =  4 ; SignalModeNameLaTeX[4] = "\\DsToTauNuTauToPiNu"   ;
  SignalModeName[5]  = "DsToKsK"              ; SignalModeTitle[5] = "D_{s}^{+}#rightarrowK^{0}K"                           ; SignalModeColor[5] =  1 ; SignalModeNameLaTeX[5] = "\\DsToKSK"              ;

  NEventsGenerated[0] = 13084000 + 13158000 ;
  NEventsGenerated[1] =  5372000 +  5269000 ;
  NEventsGenerated[2] =  7047000 +  7047000 + 803000 + 795000 ;
  NEventsGenerated[3] =  7047000 +  7047000 + 803000 + 811000 ;
  NEventsGenerated[4] =  6293000 +  6177000 ;
  NEventsGenerated[5] = 1 ;

  //                OnPeak                   OffPeak
  luminosity[0]  =  20.595 ; luminosity[1]  =  2.621 ;
  luminosity[2]  =  62.066 ; luminosity[3]  =  7.030 ;
  luminosity[4]  =  32.668 ; luminosity[5]  =  2.495 ;
  luminosity[6]  = 100.776 ; luminosity[7]  = 10.228 ;
  luminosity[8]  = 133.854 ; luminosity[9]  = 14.546 ;
  luminosity[10] =  79.030 ; luminosity[11] =  7.886 ;
  luminosity[12] =  12.954 ; luminosity[13] =  1.511 ;
  luminosity[14] =  28.494 ; luminosity[15] =  2.657 ;
  // Total           425.8                      44.4
  // Grand total     470.2
  luminosity[16] =     0.0 ;
  for(Int_t i=0 ; i<NRuns-1 ; i++) luminosity[16] += luminosity[i] ;

  EExtraCutString[0] = "" ;
  EExtraCutString[1] = "_EExtraCut" ;

  TDatime theDate ;
  Int_t theYear  = theDate.GetYear() ;
  Int_t theMonth = theDate.GetMonth() ;
  Int_t theDay   = theDate.GetDay() ;
  TString days[31] = { "01" , "02" , "03" , "04" , "05" , "06" , "07" , "08" , "09" , "10" 
             , "11" , "12" , "13" , "14" , "15" , "16" , "17" , "18" , "19" , "20" 
             , "21" , "22" , "23" , "24" , "25" , "26" , "27" , "28" , "29" , "30" 
             , "31" } ;
  TString months[12] = { "01" , "02" , "03" , "04" , "05" , "06" , "07" , "08" , "09" , "10" 
               , "11" , "12" } ;
  date = "" ;
  date = TString(date+(long)theYear+months[theMonth-1]+days[theDay-1]) ;

  TFile* f = new TFile("~/ana50/AWG90/aidanrc/output/reduced/All-R24.root") ;
  TTree* tree = (TTree*) f->Get("Ntuple") ;
  chain = (TChain*) tree ;

  // Generally useful variables
  chain->SetBranchAddress( "EllMode"       , &EllMode       ) ;
  chain->SetBranchAddress( "EllLund"       , &EllLund       ) ;
  chain->SetBranchAddress( "SignalMode"    , &SignalMode    ) ;
  chain->SetBranchAddress( "SPMode"        , &SPMode        ) ;
  chain->SetBranchAddress( "run"           , &run           ) ;
  chain->SetBranchAddress( "weight"        , &weight        ) ;
  chain->SetBranchAddress( "EExtraNeutral" , &EExtraNeutral ) ;
  // Optimized using SPRBumpHunter to get rid of hadronic background
  //EExtraNeutralUpper = 1 ;

  cout << "Done Initializing the DRecoDsToEllNuPlots analysis" << endl ;
}

DRecoDsToEllNuPlots::~DRecoDsToEllNuPlots()
{
}

void DRecoDsToEllNuPlots::ColorScheme()
{
  TCanvas* cColorScheme = new TCanvas("cColorScheme","",100,100,500,200) ;
  cColorScheme->Divide(2,1) ;
  TH1F* hColorScheme[NSignalMode] ;
  TLegend legend1 = MakeLegend(0.0,0.0,1.0,1.0) ; legend1.SetBorderSize(0) ; legend1.SetTextSize(0.1) ;
  TLegend legend2 = MakeLegend(0.0,0.0,1.0,1.0) ; legend2.SetBorderSize(0) ; legend2.SetTextSize(0.1) ;
  for(SignalMode=0 ; SignalMode<4 ; SignalMode++)
  {
    hColorScheme[SignalMode] = new TH1F(SignalModeName[SignalMode],SignalModeName[SignalMode],1,0,1) ;
    hColorScheme[SignalMode]->SetFillColor(SignalModeColor[SignalMode]) ;
    switch(SignalMode)
    {
    case 0:
    case 1:
      legend1.AddEntry(hColorScheme[SignalMode],SignalModeTitle[SignalMode],"f") ;
      break ;
    case 2:
    case 3:
      legend2.AddEntry(hColorScheme[SignalMode],SignalModeTitle[SignalMode],"f") ;
      break ;
    }
  }
  cColorScheme->cd(1) ; legend1.Draw() ;
  cColorScheme->cd(2) ; legend2.Draw() ;
  cColorScheme->Print(TString("../AWG90/aidanrc/BAD/note2170/plots/Numerator/ColorScheme.eps")) ;
}

void DRecoDsToEllNuPlots::PrintCanvas(TCanvas* canvas, TString printName)
{
  TString printNamePlotsdir = TString(TString("../AWG90/aidanrc/plots/")                        +printName+"_"+SignalModeName[SignalMode]+EExtraCutString[EExtraCut]+"_"+date+".png") ;
  TString printNameBAD      = TString(TString("../AWG90/aidanrc/BAD/note2170/plots/Numerator/") +printName+"_"+SignalModeName[SignalMode]+EExtraCutString[EExtraCut]+".eps") ;
  canvas->Print(printNamePlotsdir) ;
  canvas->Print(printNameBAD) ;
}

TString DRecoDsToEllNuPlots::suffix(){ return TString(SignalModeName[SignalMode]+EExtraCutString[EExtraCut]) ;}

TLegend DRecoDsToEllNuPlots::MakeLegend(Float_t left, Float_t top, Float_t right, Float_t bottom)
{
  TLegend legend(left, top, right, bottom) ;
  legend.SetFillColor(kWhite) ;
  legend.SetShadowColor(kWhite) ;
  return legend ;
}

TString DRecoDsToEllNuPlots::FloatToString(Float_t value, Int_t NDP)
{
  char buffer[100] ;
  TString str =  "" ;
  sprintf(buffer, TString(TString("%.")+(long)NDP + "f"), value);
  str = TString(str + buffer) ;
  return str ;
}

TString DRecoDsToEllNuPlots::FloatToString(Float_t value){ return FloatToString(value, 2) ;}

void DRecoDsToEllNuPlots::ThesisTH1F(TH1* base)
{
  Float_t labelSize   = 0.04 ;
  Float_t titleSize   = 0.04 ;
  Float_t titleOffset = 1.50 ;
  base->GetYaxis()->SetLabelSize(labelSize) ;
  base->GetYaxis()->SetTitleSize(titleSize) ;
  base->GetYaxis()->SetTitleOffset(titleOffset) ;
  base->GetXaxis()->SetLabelSize(labelSize) ;
  base->GetXaxis()->SetTitleSize(titleSize) ;
  base->GetXaxis()->SetTitleOffset(titleOffset) ;
}

void DRecoDsToEllNuPlots::ThesisTHStack(THStack* base)
{
  Float_t labelSize   = 0.04 ;
  Float_t titleSize   = 0.04 ;
  Float_t titleOffset = 1.50 ;
  base->GetYaxis()->SetLabelSize(labelSize) ;
  base->GetYaxis()->SetTitleSize(titleSize) ;
  base->GetYaxis()->SetTitleOffset(titleOffset) ;
  base->GetXaxis()->SetLabelSize(labelSize) ;
  base->GetXaxis()->SetTitleSize(titleSize) ;
  base->GetXaxis()->SetTitleOffset(titleOffset) ;
}

void DRecoDsToEllNuPlots::ThesisPlots()
{
  Bool_t       doPStar = kTRUE  ;
  Bool_t doDenominator = kFALSE ;
  Bool_t        doTags = kFALSE ;
  Bool_t     doWeights = kFALSE ;
  Bool_t    doVarPlots = kFALSE ;
  
  //TFile* fBADPlots    = new TFile("BADPlots.root","READ") ;
  TFile* fThesisTree  = new TFile("../AWG90/aidanrc/thesis.root","READ") ;
  TFile* fJoseData    = new TFile("~benitezj/anaR24Skim/workdir/DsTolnu/Data/Final/Ntuple.root"               ,"READ") ;
  TFile* fJose1005    = new TFile("~benitezj/anaR24Skim/workdir/DsTolnu/SP1005Skim/DsReco/Final/Ntuple.root"  ,"READ") ;
  TFile* fJose1005WS  = new TFile("~benitezj/anaR24Skim/workdir/DsTolnu/SP1005Skim/DsReco/FinalWS/Ntuple.root","READ") ;
  TFile* fJoseDsSt    = new TFile("/nfs/farm/g/eb/u16/benitezj/DsTolnu/SP1005Sig/DsReco/Final/Ntuple.root"    ,"READ") ;
  TTree* tJoseData   = (TTree*)   fJoseData->Get("Ntuple") ;
  TTree* tJose1005   = (TTree*)   fJose1005->Get("Ntuple") ;
  TTree* tJose1005WS = (TTree*) fJose1005WS->Get("Ntuple") ;
  TTree* tJoseDsSt   = (TTree*)   fJoseDsSt->Get("Ntuple") ;
  TTree* tThesisTree = (TTree*) fThesisTree->Get("thesis") ;
  TFile* fThesisPlots = new TFile("ThesisPlots.root","RECREATE") ;
  TCanvas thesisCanvas("thesisCanvas","Thesis plot",100,100,500,500) ;
  gROOT->cd() ;
  
  const Int_t       NTags = 38 ;
  const Int_t   NContours = 11 ;  
  
  TH1I* hNXCrossfeed[NNXT] ;
  TH1F* hTag[NTags] ;
  TH1F* hWeightsMC ;
  TH1F* hWeightsData ;
  TH1F*  hBeta[NContours] ;
  TH1F* hGamma[NContours] ;
  
  const Int_t NThesisComponents = 9 ;
  Int_t thesisHatch[NThesisComponents] ;
  Int_t thesisColor[NThesisComponents] ;
  thesisHatch[0] = 3445 ; thesisColor[0] = SignalModeColor[0] ; // DsToMuNu
  thesisHatch[1] = 3006 ; thesisColor[1] = SignalModeColor[1] ; // DsToENu
  thesisHatch[2] = 3454 ; thesisColor[2] = SignalModeColor[2] ; // DsToTauNuTauToMuNuNu
  thesisHatch[3] = 3007 ; thesisColor[3] = SignalModeColor[3] ; // DsToTauNuTauToENuNu
  thesisHatch[4] = 3490 ; thesisColor[4] =                  2 ; // DsToEllNuX   Red
  thesisHatch[5] = 3444 ; thesisColor[5] =                  4 ; // DsToHadrons  Blue
  thesisHatch[6] =    0 ; thesisColor[6] =                204 ; // Other        Grey
  thesisHatch[7] =    0 ; thesisColor[7] =                  0 ; // AllMC        Blank
  thesisHatch[8] =    0 ; thesisColor[8] =                  0 ; // Data         Blank
  
  if(doVarPlots)
  {
    //ComponentName[0]  = "DsToMuNu"             ; ComponentTitle[0]  = "D_{s}#rightarrow#mu#nu"                           ;
    //ComponentName[1]  = "DsToENu"              ; ComponentTitle[1]  = "D_{s}#rightarrowe#nu"                             ;
    //ComponentName[2]  = "DsToTauNuTauToMuNuNu" ; ComponentTitle[2]  = "D_{s}#rightarrow#tau#nu;#tau#rightarrow#mu#nu#nu" ;
    //ComponentName[3]  = "DsToTauNuTauToENuNu"  ; ComponentTitle[3]  = "D_{s}#rightarrow#tau#nu;#tau#rightarrowe#nu#nu"   ;
    //ComponentName[4]  = "DsToTauNuTauToPiNu"   ; ComponentTitle[4]  = "D_{s}#rightarrow#tau#nu;#tau#rightarrow#pi#nu"    ;
    //ComponentName[5]  = "DsToTauNuTauToOther"  ; ComponentTitle[5]  = "D_{s}#rightarrow#tau#nu;#tau#rightarrowOther"     ;
    //ComponentName[6]  = "DsSemileptonic"       ; ComponentTitle[6]  = "D_{s}#rightarrowl#nuZ"                            ;
    //ComponentName[7]  = "DsHadronic"           ; ComponentTitle[7]  = "D_{s}#rightarrowHadrons"                          ;
    //ComponentName[8]  = "VirtualPhoton"        ; ComponentTitle[8]  = "#gamma_{virtual}#rightarrow#piZ"                  ;
    //ComponentName[9]  = "D0ToKEllNu"           ; ComponentTitle[9]  = "D^{0}#rightarrowKl#nu"                            ;
    //ComponentName[10] = "DToK0EllNu"           ; ComponentTitle[10] = "D#rightarrowK^{0}l#nu"                            ;
    //ComponentName[11] = "OtherCc"              ; ComponentTitle[11] = "Other c#bar{c}"                                   ;
    //ComponentName[12] = "uds"                  ; ComponentTitle[12] = "uds"                                              ;
    //ComponentName[13] = "Other"                ; ComponentTitle[13] = "Other"                                            ;
    
    const Int_t       NVars = 5 ;
    const Int_t NComponents = 7 ;
    TString varNames[NVars] ;
    TString varXaxis[NSignalMode][NVars] ;
    TString varYaxis[NSignalMode][NVars] ;
    varNames[0] = "weight"   ;
    varNames[1] = "ellp3Lab" ;
    varNames[2] = "mm2"      ;    
    varNames[3] = "eextra"   ;
    varNames[4] = "DsMass"   ;
    Int_t   varNBins[NVars] = { 100 ,  50 ,   40 ,  50 ,    72 } ;
    Float_t varLower[NVars] = { 0.0 , 0.0 , -0.5 , 0.0 , 1.934 } ;
    Float_t varUpper[NVars] = { 1.0 , 5.0 ,  3.5 , 5.0 , 2.012 } ;
    TH1F*        hVar[NSignalMode][NEExtraCut][NVars][NComponents] ;
    TH1F*  hVarSignal[NSignalMode][NEExtraCut][NVars] ;
    THStack* stackVar[NSignalMode][NEExtraCut][NVars] ;
    
    // mm2, eextra, ellp3lab, dsmass
    TCanvas varCanvas("varCanvas","",100,100,500,500) ;
    
    varXaxis[0][0] = "weight" ;
    varXaxis[0][1] = varXaxis[0][0] ;
    varXaxis[0][2] = varXaxis[0][0] ;
    varXaxis[0][3] = varXaxis[0][1] ;
    varYaxis[0][0] = TString("entries") ;
    varYaxis[0][1] = varYaxis[0][0] ;
    varYaxis[0][2] = varYaxis[0][0] ;
    varYaxis[0][3] = varYaxis[0][0] ;
    
    varXaxis[1][0] = "p_{#mu}^{lab} (GeVc^{ -1})" ;
    varXaxis[1][1] = "p_{#mu}^{lab} (GeVc^{ -1})" ;
    varXaxis[1][2] = varXaxis[1][0] ;
    varXaxis[1][3] = varXaxis[1][1] ;
    varYaxis[1][0] = TString(TString("entries / ")+FloatToString((varUpper[1]-varLower[1])*1000/varNBins[1])+" MeVc^{ -1})") ;
    varYaxis[1][1] = varYaxis[1][0] ;
    varYaxis[1][2] = varYaxis[1][0] ;
    varYaxis[1][3] = varYaxis[1][0] ;
    
    varXaxis[2][0] = "m_{recoil}^{2}(DKX#gamma#mu) (GeV^{2}c^{ -4})" ;
    varXaxis[2][1] = "m_{recoil}^{2}(DKX#gammae) (GeV^{2}c^{ -4})" ;
    varXaxis[2][2] = varXaxis[2][0] ;
    varXaxis[2][3] = varXaxis[2][1] ;
    varYaxis[2][0] = TString(TString("entries / ")+FloatToString((varUpper[2]-varLower[2])*1000/varNBins[2])+" MeV^{2}c^{ -4}") ;
    varYaxis[2][1] = TString(TString("entries / ")+FloatToString((varUpper[2]-varLower[2])*1000/varNBins[2])+" MeV^{2}c^{ -4}") ;
    varYaxis[2][2] = varYaxis[2][0] ;
    varYaxis[2][3] = varYaxis[2][1] ;
    
    varXaxis[3][0] = "E_{extra} (GeV)" ;
    varXaxis[3][1] = varXaxis[3][0] ;
    varXaxis[3][2] = varXaxis[3][0] ;
    varXaxis[3][3] = varXaxis[3][0] ;
    varYaxis[3][0] = TString(TString("entries / ")+FloatToString((varUpper[3]-varLower[3])*1000/varNBins[3])+" MeV") ;
    varYaxis[3][1] = varYaxis[3][0] ;
    varYaxis[3][2] = varYaxis[3][0] ;
    varYaxis[3][3] = varYaxis[3][0] ;
    
    varXaxis[4][0] = "m_{D_{s}} (GeVc^{ -2})" ;
    varXaxis[4][1] = varXaxis[4][0] ;
    varXaxis[4][2] = varXaxis[4][0] ;
    varXaxis[4][3] = varXaxis[4][0] ;
    varYaxis[4][0] = TString(TString("entries / ")+FloatToString((varUpper[4]-varLower[4])*1000/varNBins[4])+" MeVc^{ -2}") ;
    varYaxis[4][1] = varYaxis[4][0] ;
    varYaxis[4][2] = varYaxis[4][0] ;
    varYaxis[4][3] = varYaxis[4][0] ;
    
    TString componentNames[NComponents] ;
    componentNames[0] = "signal"       ;
    componentNames[1] = "otherSignal"  ;
    componentNames[2] = "semileptonic" ;
    componentNames[3] = "hadronic"     ;
    componentNames[4] = "other"        ;
    componentNames[5] = "AllMC"         ;
    componentNames[6] = "data"         ;
    TString componentCut[NSignalMode][NComponents] ;
    componentCut[0][0] = "&&component==0" ;
    componentCut[0][1] = "&&component==2" ;
    componentCut[0][2] = "&&component==6" ;
    componentCut[0][3] = "&&component==7" ;
    componentCut[0][4] = "&&(component==1||component==3||component==4||component>7)" ;
    componentCut[0][5] = "" ;
    componentCut[0][6] = "" ;
    
    componentCut[1][0] = "&&component==1" ;
    componentCut[1][1] = "&&component==3" ;
    componentCut[1][2] = "&&component==6" ;
    componentCut[1][3] = "&&component==7" ;
    componentCut[1][4] = "&&(component==0||component==2||component==4||component>7)" ;
    componentCut[1][5] = "" ;
    componentCut[1][6] = "" ;
    
    componentCut[2][0] = "&&component==2" ;
    componentCut[2][1] = "&&component==0" ;
    componentCut[2][2] = "&&component==6" ;
    componentCut[2][3] = "&&component==7" ;
    componentCut[2][4] = "&&(component==1||component==3||component==4||component>7)" ;
    componentCut[2][5] = "" ;
    componentCut[2][6] = "" ;
    
    componentCut[3][0] = "&&component==3" ;
    componentCut[3][1] = "&&component==1" ;
    componentCut[3][2] = "&&component==6" ;
    componentCut[3][3] = "&&component==7" ;
    componentCut[3][4] = "&&(component==0||component==2||component==4||component>7)" ;
    componentCut[3][5] = "" ;
    componentCut[3][6] = "" ;
    
    TString EExtraCutStringVar[NSignalMode][NEExtraCut] ; // Make sure we don't double count events
    EExtraCutStringVar[0][0] = "0.5" ;
    EExtraCutStringVar[0][1] = "(eextra<1)*0.5*(1+(eextra>1.0))" ;
    EExtraCutStringVar[1][0] = "0.5" ;
    EExtraCutStringVar[1][1] = "1.0" ;
    EExtraCutStringVar[2][0] = "0.5" ;
    EExtraCutStringVar[2][1] = "1.0" ;
    EExtraCutStringVar[3][0] = "0.5" ;
    EExtraCutStringVar[3][1] = "1.0" ;
    
    for(SignalMode=0 ; SignalMode<4 ; SignalMode++)
    {
      for(EExtraCut=0 ; EExtraCut<NEExtraCut ; EExtraCut++)
      {
        for(Int_t var=0 ; var<NVars ; var++)
        {
          TString histogramName = TString(TString("hSignalVar_") + varNames[var] + "_" + suffix()) ;
          hVarSignal[SignalMode][EExtraCut][var] = new TH1F(histogramName,histogramName,varNBins[var],varLower[var],varUpper[var]) ;
          ThesisTH1F(hVarSignal[SignalMode][EExtraCut][var]) ;
          hVarSignal[SignalMode][EExtraCut][var]->SetFillStyle(thesisHatch[SignalMode]) ;
          hVarSignal[SignalMode][EExtraCut][var]->SetFillColor(thesisColor[SignalMode]) ;
          hVarSignal[SignalMode][EExtraCut][var]->GetXaxis()->SetTitle(varXaxis[var][SignalMode]) ;
          hVarSignal[SignalMode][EExtraCut][var]->GetYaxis()->SetTitle(varYaxis[var][SignalMode]) ;
          TString SPModeCut ;
          switch(SignalMode)
          {
            case 0: SPModeCut = "(SPMode==5||SPMode==6)"   ; break ;
            case 1: SPModeCut = "(SPMode==11||SPMode==12)" ; break ;
            case 2: SPModeCut = "(SPMode==13||SPMode==14)" ; break ;
            case 3: SPModeCut = "(SPMode==15||SPMode==16)" ; break ;
          }
          TString cutString = TString(TString("weight*(SignalMode==") + (long)SignalMode+"&&"+SPModeCut + ")*" + EExtraCutStringVar[SignalMode][EExtraCut]) ;
          tThesisTree->Draw(TString(varNames[var]+">>"+histogramName),cutString) ;
          TString printName = TString(TString("hVarSignal_") + varNames[var] + "_" + suffix()) ;
          varCanvas.Print(TString(TString("~/public_html/thesis/vars/")+printName+".png")) ;
          varCanvas.Print(TString(TString("~/public_html/thesis/vars/")+printName+".eps")) ;
          
          for(Int_t c=0 ; c<NComponents ; c++)
          {
            histogramName = TString(TString("hVar_") + varNames[var] + "_" + componentNames[c] + "_" + suffix()) ;
            hVar[SignalMode][EExtraCut][var][c] = new TH1F(histogramName,histogramName,varNBins[var],varLower[var],varUpper[var]) ;
            ThesisTH1F(hVar[SignalMode][EExtraCut][var][c]) ;
            hVar[SignalMode][EExtraCut][var][c]->GetXaxis()->SetTitle(varXaxis[var][SignalMode]) ;
            hVar[SignalMode][EExtraCut][var][c]->GetYaxis()->SetTitle(varYaxis[var][SignalMode]) ;
            switch(c)
            {
            case 0:
              hVar[SignalMode][EExtraCut][var][c]->SetFillStyle(thesisHatch[SignalMode]) ;
              hVar[SignalMode][EExtraCut][var][c]->SetFillColor(thesisColor[SignalMode]) ;
              break ;
            case 1:
            switch(SignalMode)
            {
              case 0:
                hVar[SignalMode][EExtraCut][var][c]->SetFillStyle(thesisHatch[2]) ;
                hVar[SignalMode][EExtraCut][var][c]->SetFillColor(thesisColor[2]) ;
                break ;
              case 1:
                hVar[SignalMode][EExtraCut][var][c]->SetFillStyle(thesisHatch[3]) ;
                hVar[SignalMode][EExtraCut][var][c]->SetFillColor(thesisColor[3]) ;
                break ;
              case 2:
                hVar[SignalMode][EExtraCut][var][c]->SetFillStyle(thesisHatch[0]) ;
                hVar[SignalMode][EExtraCut][var][c]->SetFillColor(thesisColor[0]) ;
                break ;
              case 3:
                hVar[SignalMode][EExtraCut][var][c]->SetFillStyle(thesisHatch[1]) ;
                hVar[SignalMode][EExtraCut][var][c]->SetFillColor(thesisColor[1]) ;
                break ;
            }
            break ;
            default:
              hVar[SignalMode][EExtraCut][var][c]->SetFillStyle(thesisHatch[c+2]) ;
              hVar[SignalMode][EExtraCut][var][c]->SetFillColor(thesisColor[c+2]) ;
              break ;
            }
            SPModeCut = (c==6) ? "SPMode==10" : "SPMode<5" ;
            cutString = TString(TString("weight*(SignalMode==") + (long)SignalMode+"&&"+SPModeCut+componentCut[SignalMode][c] + ")*" + EExtraCutStringVar[SignalMode][EExtraCut]) ;
            tThesisTree->Draw(TString(varNames[var]+">>"+histogramName),cutString) ;
            varCanvas.Print(TString(TString("~/public_html/thesis/vars/")+histogramName+".png")) ;
            varCanvas.Print(TString(TString("~/public_html/thesis/vars/")+histogramName+".eps")) ;
            varCanvas.Clear() ;
          }
          
        }
      }
    }
    for(SignalMode=0 ; SignalMode<4 ; SignalMode++)
    {
      for(EExtraCut=0 ; EExtraCut<NEExtraCut ; EExtraCut++)
      {
        for(Int_t var=0 ; var<NVars ; var++)
        {
          TString stackName = TString(TString("hVar_") + varNames[var] + "_" + suffix()) ;
          stackVar[SignalMode][EExtraCut][var] = new THStack(stackName,"") ;
          for(Int_t c=4 ; c>-1 ; c--)
          {
            hVar[SignalMode][EExtraCut][var][c]->Scale(hVar[SignalMode][EExtraCut][var][6]->GetSumOfWeights()/hVar[SignalMode][EExtraCut][var][5]->GetSumOfWeights()) ;
            stackVar[SignalMode][EExtraCut][var]->Add(hVar[SignalMode][EExtraCut][var][c]) ;
          }
          // Stacked MC plot          
          stackVar[SignalMode][EExtraCut][var]->Draw() ;
          cout << stackVar[SignalMode][EExtraCut][var] << " " << stackVar[SignalMode][EExtraCut][var]->GetXaxis() << endl ;
          ThesisTHStack(stackVar[SignalMode][EExtraCut][var]) ;
          stackVar[SignalMode][EExtraCut][var]->GetXaxis()->SetTitle(varXaxis[var][SignalMode]) ;
          stackVar[SignalMode][EExtraCut][var]->GetYaxis()->SetTitle(varYaxis[var][SignalMode]) ;

          TString printName = TString(TString("stackVar_") + varNames[var] + "_" + suffix()) ;
          varCanvas.Print(TString(TString("~/public_html/thesis/stacks/")+printName+".png")) ;
          varCanvas.Print(TString(TString("~/public_html/thesis/stacks/")+printName+".eps")) ;
          
          // Plot with data
          hVar[SignalMode][EExtraCut][var][6]->Draw("sames:pe") ;
          varCanvas.Print(TString(TString("~/public_html/thesis/stacks/")+printName+"_Data.png")) ;
          varCanvas.Print(TString(TString("~/public_html/thesis/stacks/")+printName+"_Data.eps")) ;
          varCanvas.Clear() ;
          cout << endl ;
        }
      }
    }
    // Cut plots    
    TH1F* hDsToMuNuCut = new TH1F("hDsToMuNuCut", "hDsToMuNuCut", 100*varNBins[3],varLower[3],varUpper[3]) ;
    hDsToMuNuCut->Fill(1.0,10000) ;
    stackVar[0][0][3]->Draw() ;
    hDsToMuNuCut->Draw("same") ;
    varCanvas.Print(TString("~/public_html/thesis/stacks/stackVar_eextra_DsToMuNu_eextraCut.png")) ;
    varCanvas.Print(TString("~/public_html/thesis/stacks/stackVar_eextra_DsToMuNu_eextraCut.eps")) ;
    varCanvas.Clear() ;
    
    TH1F* hDsToENuCut = new TH1F("hDsToENuCut", "hDsToENuCut", 100*varNBins[3],varLower[3],varUpper[3]) ;
    hDsToENuCut->Fill(1.0,10000) ;
    stackVar[1][0][3]->Draw() ;
    hDsToENuCut->Draw("same") ;
    varCanvas.Print(TString("~/public_html/thesis/stacks/stackVar_eextra_DsToENu_eextraCut.png")) ;
    varCanvas.Print(TString("~/public_html/thesis/stacks/stackVar_eextra_DsToENu_eextraCut.eps")) ;
    varCanvas.Clear() ;
    
    TH1F* hDsToTauNuTauToMuNuNuCut = new TH1F("hDsToTauNuTauToMuNuNuCut", "hDsToTauNuTauToMuNuNuCut", 100*varNBins[2],varLower[2],varUpper[2]) ;
    hDsToTauNuTauToMuNuNuCut->Fill(0.5,10000) ;
    stackVar[2][0][2]->Draw() ;
    hDsToTauNuTauToMuNuNuCut->Draw("same") ;
    varCanvas.Print(TString("~/public_html/thesis/stacks/stackVar_mm2_DsToTauNuTauToMuNuNu_mm2Cut.png")) ;
    varCanvas.Print(TString("~/public_html/thesis/stacks/stackVar_mm2_DsToTauNuTauToMuNuNu_mm2Cut.eps")) ;
    varCanvas.Clear() ;
    
    // Legend
    TCanvas varLegendCanvas("varLegendCanvas","",100,100,400,500) ;
    TLegend varLegend = MakeLegend(0.0,0.0,1.0,1.0) ;
    varLegend.AddEntry(hVar[0][0][0][0],"D_{s}#rightarrow#mu#nu","f") ;
    varLegend.AddEntry(hVar[1][0][0][0],"D_{s}#rightarrowe#nu","f") ;
    varLegend.AddEntry(hVar[2][0][0][0],"D_{s}#rightarrow#tau#nu;#tau#rightarrow#mu#nu#nu","f") ;
    varLegend.AddEntry(hVar[3][0][0][0],"D_{s}#rightarrow#tau#nu;#tau#rightarrowe#nu#nu","f") ;
    varLegend.AddEntry(hVar[0][0][0][2],"D_{s}#rightarrowl#nuZ","f") ;
    varLegend.AddEntry(hVar[0][0][0][3],"D_{s}#rightarrowhadrons","f") ;
    varLegend.AddEntry(hVar[0][0][0][4],"Other backgrounds","f") ;
    varLegend.AddEntry(hVar[0][0][0][8],"Data","p") ;
    varLegend.Draw() ;
    varLegendCanvas.Print(TString("~/public_html/thesis/stacks/stackVar_legend.png")) ;
    varLegendCanvas.Print(TString("~/public_html/thesis/stacks/stackVar_legend.eps")) ;
    varLegendCanvas.Clear() ;
  }
  
  if(doPStar)
  {
    // Pstar cut
    const Int_t NPStarTypes = 5 ;
    const Int_t  NPStarCuts = 5 ;
    const Int_t  NPStarVars = 2 ;
    TH1F* hPStar[NPStarVars][NPStarTypes][NPStarCuts] ;
    TCanvas pStarCanvas("pStarCanvas","pStarCanvas",100,100,500,500) ;
    TString pStarTypeNames[NPStarTypes] ;
    pStarTypeNames[0] = "SP-998" ;
    pStarTypeNames[1] = "SP-1005" ;
    pStarTypeNames[2] = "SP-123" ;
    pStarTypeNames[3] = "AllEvents-Run3-OnPeak" ;
    pStarTypeNames[4] = "AllEvents-Run3-OffPeak" ;
    Int_t pStarTypeColor[NPStarTypes][NPStarCuts] ;
    pStarTypeColor[0][0] = 205 ;
    pStarTypeColor[0][1] = 206 ;
    pStarTypeColor[0][2] =   2 ;
    pStarTypeColor[0][3] = 207 ;
    pStarTypeColor[0][4] = 208 ;
    
    pStarTypeColor[1][0] = 209 ;
    pStarTypeColor[1][1] = 210 ;
    pStarTypeColor[1][2] =   3 ;
    pStarTypeColor[1][3] = 211 ;
    pStarTypeColor[1][4] = 212 ;
    
    pStarTypeColor[2][0] = 213 ;
    pStarTypeColor[2][1] = 214 ;
    pStarTypeColor[2][2] =   4 ;
    pStarTypeColor[2][3] = 215 ;
    pStarTypeColor[2][4] = 216 ;
    
    pStarTypeColor[3][0] = 221 ;
    pStarTypeColor[3][1] = 222 ;
    pStarTypeColor[3][2] =   6 ;
    pStarTypeColor[3][3] = 223 ;
    pStarTypeColor[3][4] = 224 ;
    
    pStarTypeColor[4][0] = 225 ;
    pStarTypeColor[4][1] = 226 ;
    pStarTypeColor[4][2] =   7 ;
    pStarTypeColor[4][3] = 227 ;
    pStarTypeColor[4][4] = 228 ;
    
    TChain* pStarChain[NPStarTypes] ;
    for(Int_t type=0 ; type<NPStarTypes ; type++)
    {
      pStarChain[type] = new TChain("ntp1") ;
      TString fileString = TString(TString("../AWG90/aidanrc/output/tags/*")+pStarTypeNames[type]+"*" ) ;
      //cout << fileString << " " << pStarChain[type]->Add(fileString) << endl ;
      pStarChain[type]->Add(fileString) ;
    }
    
    TString pStarNames[NPStarVars]    = { "DMass"   , "pStar"   } ;
    TString pStarBranches[NPStarVars] = { "TagMass" , "Tagp3CM" } ;
    TString pStarXaxis[NPStarVars] = { "m_{C_{tag}} (GeVc^{ -2})" , "p_{C_{tag}}^{CM} (GeVc^{ -1})" } ;
    TString pStarYaxis[NPStarVars] ;
    Int_t   pStarNBins[NPStarVars] = {  100 , 100 } ;
    Float_t pStarLower[NPStarVars] = {  1.8 ,   0 } ;
    Float_t pStarUpper[NPStarVars] = { 1.95 ,   5 } ;
    pStarYaxis[0] = TString(TString("1000 entries / 1.5 MeVc^{ -2}")) ;
    pStarYaxis[1] = TString(TString("1000 entries / 50 MeVc^{ -1}" )) ;
    
    for(Int_t var=0 ; var<NPStarVars ; var++)
    {
      for(Int_t type=0 ; type<NPStarTypes ; type++)
      {
        for(Int_t cut=0 ; cut<NPStarCuts ; cut++)
        {
          TString histogramName = TString(TString("h_") + pStarNames[var] + "_" + pStarTypeNames[type] + "_pStarCut" + (long)cut) ;
          hPStar[var][type][cut] = new TH1F(histogramName,histogramName,pStarNBins[var],pStarLower[var],pStarUpper[var]) ;
          hPStar[var][type][cut]->SetMinimum(0.0) ;
          ThesisTH1F(hPStar[var][type][cut]) ;
          hPStar[var][type][cut]->SetFillColor(pStarTypeColor[type][cut]) ;
          hPStar[var][type][cut]->GetXaxis()->SetTitle(pStarXaxis[var]) ;
          hPStar[var][type][cut]->GetYaxis()->SetTitle(pStarYaxis[var]) ;
          TString cutString = TString(TString("0.001*(Tagp3CM>") + FloatToString(pStarLower[1]+(pStarUpper[1]-pStarLower[1])*cut/NPStarCuts,3)+")") ;
          TString drawString = TString(pStarBranches[var]+">>"+histogramName) ;
          cout << pStarChain[type]->Draw(drawString,cutString,"goff") << endl ;
        }
      }
    }
    for(Int_t type=0 ; type<NPStarTypes ; type++)
    {
      hPStar[0][type][0]->Draw("") ;
      for(Int_t cut=1 ; cut<NPStarCuts ; cut++)
      {
        hPStar[0][type][cut]->Draw("same") ;
      }
      //hPStar[0][type][0]->Draw("AXIS") ;
      pStarCanvas.Print(TString(TString("~/public_html/thesis/pStar/pStar_dsmass_")+pStarTypeNames[type]+".png")) ;
      pStarCanvas.Print(TString(TString("~/public_html/thesis/pStar/pStar_dsmass_")+pStarTypeNames[type]+".eps")) ;
      pStarCanvas.Clear() ;
      hPStar[1][type][0]->Draw() ;
      pStarCanvas.Print(TString(TString("~/public_html/thesis/pStar/pStar_pStar_")+pStarTypeNames[type]+".png")) ;
      pStarCanvas.Print(TString(TString("~/public_html/thesis/pStar/pStar_pStar_")+pStarTypeNames[type]+".eps")) ;
      pStarCanvas.Clear() ;
    }
  }
  
  if(doDenominator)
  {
    // Denominator plots
    Int_t   nBinsDsMass =  100 ;
    Float_t lowerDsMass = 1.85 ;
    Float_t upperDsMass = 2.10 ;
    
    // DsSt and Ds mass plots
    TH1F* hDsStMass = new TH1F("hDsStMass","hDsStMass",50,1.8,2.4) ;
    TCanvas DsStMassCanvas("DsStMass","",100,100,500,500) ;
    cout << tJoseDsSt->Draw("dsstuncmass>>hDsStMass","0.001") << endl ;
    ThesisTH1F(hDsStMass) ;
    hDsStMass->SetFillColor(7) ;
    hDsStMass->GetXaxis()->SetTitle("m_{D_{s}^{*}} (GeVc^{ -2})") ;
    hDsStMass->GetYaxis()->SetTitle(TString(TString("1000 entries / ") + FloatToString((2.4-1.8)/50.0,3) + " GeVc^{ -2}")) ;
    hDsStMass->Draw() ;
    DsStMassCanvas.Print(TString("~/public_html/thesis/dsmass/DsStMass.png"));
    DsStMassCanvas.Print(TString("~/public_html/thesis/dsmass/DsStMass.eps"));
    DsStMassCanvas.Clear() ;
    
    TH1F* hDsMass   = new TH1F("hDsMass","hDsMass",nBinsDsMass,lowerDsMass,upperDsMass) ;
    TCanvas DsMassCanvas("DsMass","",100,100,500,500) ;
    cout << tJoseData->Draw("dsmass>>hDsMass","0.001") << endl ;
    ThesisTH1F(hDsMass) ;
    hDsMass->SetFillColor(7) ;
    TString DsMassXaxis = "m_{D_{s}} (GeVc^{ -2})" ;
    TString DsMassYaxis = TString(TString("1000 entries / ") + FloatToString((2.5-1.8)/100.0,3) + " GeVc^{ -2}") ;
    hDsMass->GetXaxis()->SetTitle(DsMassXaxis) ;
    hDsMass->GetYaxis()->SetTitle(DsMassYaxis) ;
    hDsMass->Draw() ;
    DsMassCanvas.Print(TString("~/public_html/thesis/dsmass/DsMass.png"));
    DsMassCanvas.Print(TString("~/public_html/thesis/dsmass/DsMass.eps"));
    DsMassCanvas.Clear() ;
    
    TH1F* hDsMassMC = new TH1F("hDsMassMC","hDsMassMC",nBinsDsMass,lowerDsMass,upperDsMass) ;
    cout << tJose1005->Draw("dsmass>>hDsMassMC","0.001") << endl ;
    ThesisTH1F(hDsMassMC) ;
    hDsMassMC->SetFillColor(7) ;
    hDsMassMC->GetXaxis()->SetTitle(DsMassXaxis) ;
    hDsMassMC->GetYaxis()->SetTitle(DsMassYaxis) ;
    hDsMassMC->Draw() ;
    DsMassCanvas.Print(TString("~/public_html/thesis/dsmass/DsMassMC.png"));
    DsMassCanvas.Print(TString("~/public_html/thesis/dsmass/DsMassMC.eps"));
    DsMassCanvas.Clear() ;
    
    hDsMassMC->Scale(hDsMass->GetSumOfWeights()/hDsMassMC->GetSumOfWeights()) ;
    hDsMassMC->Draw() ;
    hDsMass->Draw("p:sames") ;
    DsMassCanvas.Print(TString("~/public_html/thesis/dsmass/DsMassDataMC.png"));
    DsMassCanvas.Print(TString("~/public_html/thesis/dsmass/DsMassDataMC.eps"));
    
    // RS/WS
    TCanvas RSWSCanvas("RSWSCanvas","",100,100,500,500) ;
    const Int_t nNXR = 5 ; // 0 1 2 3 All
    TH1F* hDsMassRSWS[2][nNXR][2] ; // RS/WS , NXR , Signal
    THStack* stackDsMassRSWS[2][nNXR] ; // RS/WS , NXR
    TString hasSignalName[2] = { "noSignal" , "signal" } ;
    TString RSWSName[2] = { "RS" , "WS" } ;
    TString nXRName[nNXR] = { "0" , "1" , "2" , "3" , "All" } ;
    TString nXRCut[nNXR] = { "fragndaus==0" , "fragndaus==1" , "fragndaus==2" , "fragndaus==3" , "fragndaus>-1" } ;
    TString signalCut[2] = { "evthassignal==0" , "evthassignal==1" } ;
    Int_t DsMassSignalColor[2] = { 0 , 8 } ;
    
    for(Int_t RSWS=0 ; RSWS<2 ; RSWS++)
    {
      for(Int_t nXR=0 ; nXR<nNXR ; nXR++)
      {
        TString stackName = TString(TString("stack_") + RSWSName[RSWS] + "_" + nXRName[nXR]) ;
        stackDsMassRSWS[RSWS][nXR] = new THStack(stackName,"") ;
        for(Int_t signal=0 ; signal<2 ; signal++)
        {
          TString histogramName = TString(TString("h_") + hasSignalName[signal] + "_" + RSWSName[RSWS] + "_NXR" + nXRName[nXR]) ;
          hDsMassRSWS[RSWS][nXR][signal] = new TH1F(histogramName,histogramName,nBinsDsMass,lowerDsMass,upperDsMass) ;
          hDsMassRSWS[RSWS][nXR][signal]->GetXaxis()->SetTitle(DsMassXaxis) ;
          hDsMassRSWS[RSWS][nXR][signal]->GetYaxis()->SetTitle(DsMassYaxis) ;
          ThesisTH1F(hDsMassRSWS[RSWS][nXR][signal]) ;
          hDsMassRSWS[RSWS][nXR][signal]->SetFillColor(DsMassSignalColor[signal]) ;
          TString cutString = TString(TString("0.001*(")+nXRCut[nXR]+"&&"+signalCut[signal]+")") ;
          if(RSWS==0)
          {
            tJose1005->Draw(TString(TString("dsmass>>")+histogramName),cutString) ;
          }
          else
          {
            tJose1005WS->Draw(TString(TString("dsmass>>")+histogramName),cutString) ;
          }
          stackDsMassRSWS[RSWS][nXR]->Add(hDsMassRSWS[RSWS][nXR][signal]) ;
          RSWSCanvas.Print(TString(TString("~/public_html/thesis/dsmass/")+histogramName+".png")) ;
          RSWSCanvas.Print(TString(TString("~/public_html/thesis/dsmass/")+histogramName+".eps")) ;
          RSWSCanvas.Clear() ;
        }
        stackDsMassRSWS[RSWS][nXR]->Draw() ;
        ThesisTHStack(stackDsMassRSWS[RSWS][nXR]) ;
        stackDsMassRSWS[RSWS][nXR]->GetXaxis()->SetTitle(DsMassXaxis) ;
        stackDsMassRSWS[RSWS][nXR]->GetYaxis()->SetTitle(DsMassYaxis) ;
        RSWSCanvas.Print(TString(TString("~/public_html/thesis/dsmass/")+stackName+".png")) ;
        RSWSCanvas.Print(TString(TString("~/public_html/thesis/dsmass/")+stackName+".eps")) ;
        RSWSCanvas.Clear() ;
      }
    }
    
    // X crossfeed
    TCanvas XCanvas("XCanvas","",100,100,500,500) ;
    tJose1005->Draw("1.0*fragnpartgen") ;
    XCanvas.Print(TString("~/public_html/thesis/NXT/NXT.png"));
    XCanvas.Print(TString("~/public_html/thesis/NXT/NXT.eps"));
    XCanvas.Clear() ;
    for(Int_t i=0 ; i<NNXT ; i++)
    {
      TString histogramName = TString(TString("hNXTCrossfeed_NXT")+(long)i) ;
      hNXCrossfeed[i] = new TH1I(histogramName,histogramName,4,0,4) ;
      hNXCrossfeed[i]->GetXaxis()->SetBinLabel(1,"0") ;
      hNXCrossfeed[i]->GetXaxis()->SetBinLabel(2,"1") ;
      hNXCrossfeed[i]->GetXaxis()->SetBinLabel(3,"2") ;
      hNXCrossfeed[i]->GetXaxis()->SetBinLabel(4,"3") ;
      hNXCrossfeed[i]->SetMinimum(0) ;
      ThesisTH1F(hNXCrossfeed[i]) ;
      hNXCrossfeed[i]->GetXaxis()->SetTitle("n_{X}^{R}") ;
      hNXCrossfeed[i]->GetYaxis()->SetTitle("entries") ;
      hNXCrossfeed[i]->SetFillColor(51+i*8) ;
      TString drawString = TString(TString("fragndaus>>")+histogramName) ;
      TString cutString  = TString(TString("fragnpartgen>")+FloatToString(i-0.1)+"&&fragnpartgen<"+FloatToString(i+0.1)) ;
      cout << tJose1005->Draw(drawString,cutString) << endl ;
      hNXCrossfeed[i]->Draw() ;
      XCanvas.Print(TString(TString("~/public_html/thesis/NXT/XCrossfeed_NXT")+(long)i+".png")) ;
      XCanvas.Print(TString(TString("~/public_html/thesis/NXT/XCrossfeed_NXT")+(long)i+".eps")) ;
      XCanvas.Clear() ;
    }
  }
  
  if(doTags)
  {
    // Tag modes
    TCanvas tagCanvas("tagCanvas","tagCanvas",100,100,500,500) ;
    
    Float_t tagDMassLower = 1.775 ;
    Float_t tagDMassUpper = 1.955 ;
    Int_t   tagDMassNBins =   180 ;
    TFile* fTag = NULL ;
    TTree* tTag = NULL ;
    for(Int_t tag=1 ; tag<NTags+1 ; tag++)
    {
      // Broken tag modes
      if(tag==7) { tag=11 ; continue ; }
      if(tag==16){ tag=21 ; continue ; }
      if(tag==28){ tag=31 ; continue ; }
      
      TString histogramName = TString(TString("hTag")+(long)tag) ;
      hTag[tag] = new TH1F(histogramName,histogramName,tagDMassNBins,tagDMassLower,tagDMassUpper) ;
      fTag = new TFile(TString(TString("../AWG90/gjackson/ana24.3.6a/workdir/DsTolnu/TagOptRaw/Mode")+(long)tag+"/Ntuple.root"),"READ") ; 
      tTag = (TTree*) fTag->Get("Ntuple") ;
      gROOT->cd() ;
      cout << tTag->Draw(TString(TString("tagmass>>")+histogramName),"") << endl ;
      delete tTag ;
      delete fTag ;
      ThesisTH1F(hTag[tag]) ;
      hTag[tag]->GetXaxis()->SetTitle("m_{C_{tag}} (GeVc^{ -2})") ;
      hTag[tag]->GetYaxis()->SetTitle(TString(TString("entries / ") + FloatToString((tagDMassUpper-tagDMassLower)/tagDMassNBins) + " GeVc^{ -2}")) ;
      hTag[tag]->SetFillColor(7) ;
      hTag[tag]->Draw() ;
      cout << tag << " " << hTag[tag]->GetSumOfWeights() << endl ;
      tagCanvas.Print(TString(TString("~/public_html/thesis/tag/tag")+(long)tag+".png")) ;
      tagCanvas.Print(TString(TString("~/public_html/thesis/tag/tag")+(long)tag+".eps")) ;
    }
    cout << "Set up tag plots" << endl ;
  }
  
  if(doWeights)
  {
    fThesisPlots->cd() ;
    // beta, gamma weights model
    TCanvas cWeights("cWeights","",100,100,500,500) ;
    Int_t   nBins = 1000 ;
    Float_t lower = 0 ;
    Float_t upper = 6 ;
    hWeightsMC = new TH1F("hWeightsMC","hMC",nBins,lower,upper) ;
    ThesisTH1F(hWeightsMC) ;
    hWeightsMC->SetLineWidth(2) ;
    hWeightsMC->GetXaxis()->SetTitle("n_{X}^{T}") ;
    hWeightsMC->GetYaxis()->SetTitle("weight") ;
    hWeightsData  = (TH1F*)hWeightsMC->Clone("hWeightsData" ) ;  
    hWeightsMC->SetLineColor(2) ;
    
    Float_t alpha = -1.32 ;
    Float_t beta  =  3.38 ;
    Float_t gamma =  1.15 ;
  
    Float_t nXTf = 0 ;
    Float_t weight = 0 ;
    Float_t total = 0 ;
    for(Int_t i=0 ; i<7 ; i++)
    {
      total += pow(i-alpha,beta)*exp(-gamma*i) ;
    }
    for(Int_t i=0 ; i<nBins ; i++)
    {
      nXTf = lower + (upper-lower)*(Float_t)i/(1.0*nBins) ;
      weight = pow(nXTf-alpha,beta)*exp(-gamma*nXTf)/total ;
      hWeightsMC->SetBinContent(i+1,weight) ;
    }
    hWeightsMC->Draw("l") ;
    cWeights.Print("~/public_html/thesis/weights/MCWeights.eps") ;
    cWeights.Print("~/public_html/thesis/weights/MCWeights.png") ;
    cWeights.Clear() ;
  
    alpha = -1.32 ;
    beta  =  0.27 ;
    gamma =  0.28 ;
    Float_t  beta_error = 0.17 ;
    Float_t gamma_error = 0.07 ;
    for(Int_t i=0 ; i<7 ; i++)
    {
      total += pow(i-alpha,beta)*exp(-gamma*i) ;
    }
    for(Int_t i=0 ; i<nBins ; i++)
    {
      nXTf = lower + (upper-lower)*(Float_t)i/(1.0*nBins) ;
      weight = pow(nXTf-alpha,beta)*exp(-gamma*nXTf)/total ;
      hWeightsData->SetBinContent(i+1,weight) ;
    }
    hWeightsData->SetLineColor(3) ;
    hWeightsData->Draw("l") ;
    cWeights.Print("~/public_html/thesis/weights/DataWeights.eps") ;
    cWeights.Print("~/public_html/thesis/weights/DataWeights.png") ;
    cWeights.Clear() ;
    
    Float_t  beta_temp = beta ;
    Float_t gamma_temp = gamma ;
    for(Int_t contour=0 ; contour<NContours ; contour++)
    {
      TString histogramName ;
      histogramName = TString(TString("hBeta") +(long)contour) ;
      hBeta[contour] = (TH1F*) hWeightsMC->Clone(histogramName) ;
      hBeta[contour]->GetXaxis()->SetTitle("n_{X}^{T}(#beta)") ;
      hBeta[contour]->SetLineColor(51+contour) ;
      beta_temp  = beta -beta_error  +  2*beta_error*(Float_t)contour/NContours ;
      gamma_temp = gamma-gamma_error + 2*gamma_error*(Float_t)contour/NContours ;
      for(Int_t i=0 ; i<nBins ; i++)
      {
        nXTf = lower + (upper-lower)*(Float_t)i/(1.0*nBins) ;
        weight = pow(nXTf-alpha,beta_temp)*exp(-gamma*nXTf)/total ;
        hBeta[contour]->SetBinContent(i+1,weight) ;
      }
      histogramName = TString(TString("hGamma")+(long)contour) ;
      hGamma[contour] = (TH1F*) hWeightsMC->Clone(histogramName) ;
      hGamma[contour]->GetXaxis()->SetTitle("n_{X}^{T}(#gamma)") ;
      hGamma[contour]->SetLineColor(90+contour) ;
      for(Int_t i=0 ; i<nBins ; i++)
      {
        nXTf = lower + (upper-lower)*(Float_t)i/(1.0*nBins) ;
        weight = pow(nXTf-alpha,beta)*exp(-gamma_temp*nXTf)/total ;
        hGamma[contour]->SetBinContent(i+1,weight) ;
      }
    }
    hBeta[0]->Draw("l") ;
    for(Int_t contour=1 ; contour<NContours ; contour++){ hBeta[contour]->Draw("l:same") ; }
    cWeights.Print("~/public_html/thesis/weights/betaWeights.eps") ;
    cWeights.Print("~/public_html/thesis/weights/betaWeights.png") ;
    cWeights.Clear() ;
    hGamma[0]->Draw("l") ;
    for(Int_t contour=1 ; contour<NContours ; contour++){ hGamma[contour]->Draw("l:same") ; }
    cWeights.Print("~/public_html/thesis/weights/gammaWeights.eps") ;
    cWeights.Print("~/public_html/thesis/weights/gammaWeights.png") ;
    cWeights.Clear() ;
  }
  
  fThesisPlots->Write() ;
  fThesisPlots->Close() ;
  cout << "Written file out" << endl ;
}

void DRecoDsToEllNuPlots::weightsPlots()
{
}

Float_t DRecoDsToEllNuPlots::fDsFromBF(Float_t BF, Float_t StatError, Float_t SystError, Int_t SignalMode)
{
  Float_t hBar = 6.5821189e-25 ; // GeVs
  Float_t tDs  = 5.0e-13/hBar ;  // s
  Float_t Vcs  = 0.97425 ;       // 0.957 ;
  Float_t GF   = 1.16637e-5 ;    // GeV^-2
  Float_t mDs  = 1.96849 ;       // GeVc^-2
  Float_t mTau = 1.777 ;         // GeVc^-2
  Float_t mMu  = 0.105658367 ;   // GeVc^-2
  Float_t mE   = 0.000511 ;      // GeVc^-2
  Float_t mEll = 0 ;             // GeVc^-2
  switch(SignalMode)
  {
  case 0: mEll = mMu ; break ;
  case 1: mEll = mE  ; break ;
  case 2:
  case 3:
  case 4:
    mEll = mTau ;
    break ;
  default:
    cout << "Invalid SignalMode (" << SignalMode << ")" << endl ;
    return -1 ;
    break ;
  }
  Float_t scale = 1000 ;
  Float_t fDs          = scale*(1.0/(GF*mEll*(1-(mEll*mEll)/(mDs*mDs))*Vcs))*sqrt(8*TMath::Pi()*BF/(mDs*tDs)) ;
  Float_t fDsStatError = fDs*sqrt(StatError*StatError/(4*BF*BF)) ;
  Float_t fDsSystError = fDs*SystError/(2*BF) ;
  Float_t fDsTheoError = 0.007*fDs ;
  Float_t fDsError     = fDs*sqrt(fDsStatError*fDsStatError + fDsSystError*fDsSystError + fDsTheoError*fDsTheoError) ;
  cout << "B(" << SignalModeName[SignalMode] << ") = " << BF << " +/- " << StatError << " +/- " << SystError << endl ;
  cout << "fDs = " << fDs << " +/- " << fDsError << endl ;
  cout << "    = " << fDs << " +/- " << fDsStatError << " +/- " << fDsSystError << " +/- " << fDsTheoError << endl ;
  return fDs ;
}

Float_t DRecoDsToEllNuPlots::LimitExtraction()
{
  TCanvas cLimit("cLimit","cLimit",100,100,500,500) ;
  // Take number of Ds mesons, efficiency and number of DsToENu events as inputs
  // Scan in number of Ds mesons, efficiency and branching fraction
  // B = Branching fraction
  // N(DsToENu) = nDs*eff*

  Float_t totalLambda      = 47362.8 ;
  Float_t totalLambdaError =  2832.0 ;
  // NDsE PDF is Gaussian
  // n(DsToENu) is Gaussian
  // B is the Branching Fraction
  Float_t nSigma     =        5 ;
  Float_t nDsE       =      0.0 ; 
  Float_t nDsEHat    = totalLambda ;
  Float_t sigma_nDsE = totalLambdaError ;
  Float_t nDsE_lower = nDsEHat-nSigma*sigma_nDsE ;
  Float_t nDsE_upper = nDsEHat+nSigma*sigma_nDsE ;
  Float_t nDsToENu   =    1.94 ; //6.09 ;
  Float_t sigma_n    =    3.35 ; //5.18 ;
  Float_t B          =     0.0 ;
  Float_t B_lower    =     0.0 ;
  Float_t B_upper    =  5.0e-4 ;
  Float_t grain      =   101.0 ;
  Float_t L_temp     =     0.0 ;
  Float_t L_MC       =     0.0 ;
  Float_t L_max      =    -1.0 ;
  Int_t nPoints      = 100000000 ;

  if(true){ L_max = 1 ; }
  else
  {
    for(Float_t i=0 ; i<grain ; i++)
    {
      nDsE = nDsE_lower + (nDsE_upper-nDsE_lower)*i/grain ;
      for(Float_t j=0 ; j<grain ; j++)
      {
    B = B_lower + (B_upper-B_lower)*j/grain ;
    L_temp = L(nDsE,nDsEHat,sigma_nDsE,B,nDsToENu,sigma_n) ;
    if(L_temp>L_max) L_max = L_temp ;
      }
    }
  }
  cout << "L_max = " <<  L_max << endl ;
  Float_t integral = 0.0 ;
  TRandom3 random ;
  random.SetSeed(1) ;

  const Int_t nIntegrals = 1000 ;
  Float_t integrals[nIntegrals] ;
  for(Int_t j=0 ; j<nIntegrals ; j++){ integrals[j] = 0 ; }
  for(Int_t i=0 ; i<nPoints ; i++)
  {
    if(i%(nPoints/100)==0) cout << i << "/" << nPoints << endl ;
    nDsE   = nDsE_lower + random.Uniform(0.0,nDsE_upper-nDsE_lower) ;
    B      =   B_lower + random.Uniform(0.0,B_upper-B_lower) ;
    L_MC   = random.Uniform(0.0,L_max) ;
    L_temp = L(nDsE,nDsEHat,sigma_nDsE,B,nDsToENu,sigma_n) ;
    if(L_MC<L_temp)
    {
      for(Int_t j=0 ; j<nIntegrals ; j++)
      {
        if(B<B_lower+(B_upper-B_lower)*j/nIntegrals) integrals[j]++ ;
      }
      integral++ ;
    }
  }
  integral = integral/nPoints ;
  
  Float_t scale = 10000.0 ;
  TH1F hCDF("hCDF","hCDF",nIntegrals-1,B_lower*scale,B_upper*scale) ;
  hCDF.GetXaxis()->SetTitle("B(D_{S}^{+}#rightarrow e#nu) x 10^{ -4}") ;
  hCDF.GetYaxis()->SetTitle("Proability density") ;
  ThesisTH1F(&hCDF) ;
  hCDF.SetLineWidth(2) ;
  hCDF.SetLineColor(SignalModeColor[1]) ;
  TH1F* hPDF   = (TH1F*)hCDF.Clone("hPDF") ;
  TH1F* hPDFCL = (TH1F*)hCDF.Clone("hPDFCL") ;
  hPDFCL->SetFillStyle(3006) ;
  hPDFCL->SetFillColor(SignalModeColor[1]) ;
  //hPDF->SetLineStyle(2) ;
  //hPDFCL->SetLineStyle(2) ;
  
  Bool_t    Success68 = kFALSE ;
  Int_t    jSuccess68 = -1 ;
  Float_t  BSuccess68 =  0 ;
  Bool_t    Success90 = kFALSE ;
  Int_t    jSuccess90 = -1 ;
  Float_t  BSuccess90 =  0 ;
  Bool_t    Success95 = kFALSE ;
  Int_t    jSuccess95 = -1 ;
  Float_t  BSuccess95 =  0 ;
  for(Int_t j=0 ; j<nIntegrals ; j++)
  {
    integrals[j] = integrals[j]/(nPoints*integral) ;
    cout << "B < " ;
    cout.width(8) ;
    B = B_lower+(B_upper-B_lower)*j/nIntegrals ;
    
    hCDF.Fill(scale*B,integrals[j]) ;
    if(j>0) hPDF->Fill(scale*B,integrals[j]-integrals[j-1]) ;
    if(j>0 && integrals[j]<0.9) hPDFCL->Fill(scale*B,integrals[j]-integrals[j-1]) ;
    cout << B ;
    cout << "   " ;
    cout.width(8) ;
    cout << integrals[j] << endl ;
    if(integrals[j]>0.68 && Success68==kFALSE)
    {
      Success68  = kTRUE ;
      jSuccess68 = j ;
      BSuccess68 = B ;
    }
    if(integrals[j]>0.9 && Success90==kFALSE)
    {
      Success90  = kTRUE ;
      jSuccess90 = j ;
      BSuccess90 = B ;
    }
    if(integrals[j]>0.95 && Success95==kFALSE)
    {
      Success95  = kTRUE ;
      jSuccess95 = j ;
      BSuccess95 = B ;
    }
  }
  cout << "68% confidence interval occurs at B<" << BSuccess68 << " where the integral is " << (100*(integrals[jSuccess68]))  << "%" <<  endl ;
  cout << "90% confidence interval occurs at B<" << BSuccess90 << " where the integral is " << (100*(integrals[jSuccess90]))  << "%" <<  endl ;
  cout << "95% confidence interval occurs at B<" << BSuccess95 << " where the integral is " << (100*(integrals[jSuccess95]))  << "%" <<  endl ;
  hPDF->Scale(hCDF.GetMaximum()/hPDF->GetMaximum()) ;
  hPDFCL->Scale(hCDF.GetMaximum()/hPDFCL->GetMaximum()) ;
  TH1F hLine("hLine","hLine",nIntegrals-1,B_lower,B_upper) ;
  hLine.Fill(scale*BSuccess90,10) ;
  SignalMode = 1 ;
  hPDFCL->Draw() ;
  PrintCanvas(&cLimit,"LimitPDF") ;
  hCDF.Draw("l:sames") ;
  hPDF->Draw("same") ;
  hLine.Draw("same:l") ;  
  PrintCanvas(&cLimit,"Limit") ;
  return BSuccess90 ;
}

Float_t DRecoDsToEllNuPlots::L(Float_t nDsE, Float_t nDsEHat, Float_t sigma_nDsE, Float_t B, Float_t nDsToENu,Float_t sigma_n)
{
  Float_t mu       = nDsE*B ;
  Float_t muPart   = exp(-(mu-nDsToENu)*(mu-nDsToENu)/(2*sigma_n*sigma_n)) ;
  Float_t nDsEPart = exp(-(nDsE-nDsEHat)*(nDsE-nDsEHat)/(2*sigma_nDsE*sigma_nDsE)) ;
  Float_t L        = muPart*nDsEPart ;
  //cout << L << " " << mu << " " << muPart << " " << ePart << " " << nDsPart << " " << constant << endl ;
  return L ;
}

void DRecoDsToEllNuPlots::DetermineBF(Int_t SignalMode, Bool_t useTrueValues)
{
  TString Denomfilename="/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/" ;
  if(useTrueValues==0) _DataType = 2 ;
  if(useTrueValues==1) _DataType = 1 ;
  if(_DataType==1) Denomfilename += "DenominatorFitResult_MC.root"  ;
  if(_DataType==2) Denomfilename += "DenominatorFitResult_Data.root";
  TFile FDsFit(Denomfilename,"read") ;
  
  TString filename = "/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/" ;
  if(_DataType==1) filename += TString(SignalModeName[SignalMode]+"FitResults_MC.root") ;
  if(_DataType==2) filename += TString(SignalModeName[SignalMode]+"FitResults_Data.root") ;
  TFile FNumeratorFit(filename,"read") ;
  TFile FDenominatorEff("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/DenominatorEff.root","r")        ;
  TFile   FNumeratorEff("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/NumeratorEfficiencies.root","r") ;
  gROOT->cd() ;
  
  ComputeBF(&FDsFit,&FNumeratorFit,&FDenominatorEff,&FNumeratorEff,SignalMode,useTrueValues);
}

void DRecoDsToEllNuPlots::DenominatorSystematics(Int_t SignalMode)
{
  _DataType = 2 ;
  const Int_t NSystematic = 8 ;
  TString systematicName[NSystematic] ;
  systematicName[0] = "BackgroundModel" ;
  systematicName[1] = "DsSigShape"      ;
  systematicName[2] = "Nominal"         ;
  systematicName[3] = "PhotonPi0Bkg"    ;
  systematicName[4] = "RSWeights"       ;
  systematicName[5] = "WSSigYield"      ;
  systematicName[6] = "WSWeights"       ;
  systematicName[7] = "nXResolution"    ;
  TString prefix = "/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/" ;
  TFile* fNumeratorFit   = new TFile(TString(prefix+SignalModeName[SignalMode]+"FitResults_Data.root"),"READ") ;
  TFile* fNumeratorEff   = new TFile(TString(prefix+"NumeratorEfficiencies.root"),"READ") ;
  TFile* fDenominatorEff = new TFile(TString(prefix+"DenominatorEff.root"),"READ") ;
  TString denominatorFileName = TString(TString("~benitezj/anaR24Skim/workdir/DsTolnu/Data/Final/Systematics/")+systematicName[2]+"/FitResult.root") ;
  TFile* fDenominatorFitNominal = new TFile(denominatorFileName,"READ") ;
  Float_t nDs = GetNDs(fDenominatorFitNominal) ;
  Float_t eff = GetEfficiency(fDenominatorFitNominal,fNumeratorFit,fDenominatorEff,fNumeratorEff,SignalMode) ;
  Float_t lambda = nDs*eff ;
  Float_t lambda_nominal = lambda ;
  Float_t totalError = 0 ;
  cout.width(15) ; cout << systematicName[2] << ": " << nDs << " " << eff << " " << lambda << endl ;
  for(Int_t i=0 ; i<NSystematic ; i++)
  {
    if(i==2) continue ;
    denominatorFileName = TString(TString("~benitezj/anaR24Skim/workdir/DsTolnu/Data/Final/Systematics/")+systematicName[i]+"/FitResult_Minus.root") ;
    TFile* fDenominatorFit = new TFile(denominatorFileName,"READ") ;
    nDs = GetNDs(fDenominatorFit) ;
    eff = GetEfficiency(fDenominatorFit,fNumeratorFit,fDenominatorEff,fNumeratorEff,SignalMode) ;
    lambda = nDs*eff ;
    Float_t error = fabs(lambda-lambda_nominal) ;
    totalError += error*error ;
    Float_t relativeError = 100*error/lambda ;
    cout.width(15) ; cout << systematicName[i] << ": " << nDs << " " << eff << " " << lambda << " " << error << " " << relativeError << "%" << endl ;
    delete fDenominatorFit ;
  }
  totalError = sqrt(totalError) ;
  cout << endl << lambda_nominal << " +/- " << totalError << " " << 100*totalError/lambda_nominal << "%" <<  endl ;
}

Float_t DRecoDsToEllNuPlots::GetNDs(TFile* FDenomFitResults)
{
  RooFitResult* DsFitResult = (RooFitResult*) FDenomFitResults->Get("fitresult");
  TH2F* HDsMassData         = (TH2F*) FDenomFitResults->Get("HDsMassData") ;
  TH2F* TotSigPdfHistoFit   = (TH2F*) FDenomFitResults->Get("TotSigPdfHistoFit") ;
  RooRealVar* DsYield       = (RooRealVar*) DsFitResult->floatParsFinal().find("TotYield") ;
  //------------------------------
  /// Ds yield 
  //------------------------------
  Float_t NRecoDs    =   HDsMassData->Integral()*DsYield->getVal()*TotSigPdfHistoFit->Integral(1,4,DenominatorDsMassBinMin,DenominatorDsMassBinMax)/TotSigPdfHistoFit->Integral();
  //Float_t NRecoDsErr = HDsMassData->Integral()*DsYield->getError()*TotSigPdfHistoFit->Integral(1,4,DenominatorDsMassBinMin,DenominatorDsMassBinMax)/TotSigPdfHistoFit->Integral();
  
  return NRecoDs ;
}

Float_t DRecoDsToEllNuPlots::GetEfficiency(TFile* FDenomFitResults,TFile* FNumeratorFitResults,TFile* FEffDenom,TFile* FEffNumerator, Int_t SignalMode)
{
  //------------------------------
  // Numerator efficiencies
  //------------------------------
  if(FNumeratorFitResults->IsZombie()){ cout << "Bad NuFit File"         << endl ; return 0 ; } gROOT->cd() ;
  if(FEffNumerator->IsZombie())       { cout << "Bad FNumeratorEff File" << endl ; return 0 ; } gROOT->cd() ;
  if(FEffDenom->IsZombie())           { cout << "Bad FDsEff File"        << endl ; return 0 ; } gROOT->cd() ;
  if(FDenomFitResults->IsZombie())    { cout << "Bad DsFit File"         << endl ; return 0 ; } gROOT->cd() ;
  Float_t    NumeratorEff[NXT] ;
  Float_t NumeratorEffErr[NXT] ;
  
  TH1F* HnXTWMPars = (TH1F*) FDenomFitResults->Get("HnXTWMPars") ;
  
  TString fitResultsName ;
  if(_DataType==1) fitResultsName = TString(TString("fitResults")     + SignalModeName[SignalMode] + "RunAll") ;
  if(_DataType==2) fitResultsName = TString(TString("fitResultsData") + SignalModeName[SignalMode] + "RunAll") ;
  TString fractionName ;
  if(_DataType==1) fractionName = TString(TString("fSignalRunAll") + SignalModeName[SignalMode] + EExtraCutString[1]) ;
  if(_DataType==2) fractionName = TString(TString("fDataRunAll")   + SignalModeName[SignalMode] + EExtraCutString[1]) ;
  
  TString histogramName ;
  if(_DataType==1) histogramName = TString(SignalModeName[SignalMode] + "_RunAll"      + EExtraCutString[1]) ;
  if(_DataType==2) histogramName = TString(SignalModeName[SignalMode] + "_Data_RunAll" + EExtraCutString[1]) ;
  RooFitResult* NuFitResult = (RooFitResult*) FNumeratorFitResults->Get(fitResultsName) ;
  RooRealVar*       NuYield = (RooRealVar*)   NuFitResult->floatParsFinal().find(fractionName) ;
  TH2F* HNuMassData = (TH2F*) FNumeratorFitResults->Get(histogramName) ;
  if(!NuFitResult){ cout << "No NuFitResult" << endl ; return 0 ; }
  if(!NuYield)    { cout << "fSig not found" << endl ; return 0 ; }
  if(!HNuMassData){ cout << "No HNuMassData" << endl ; return 0 ; }
  
  RooFitResult* DsFitResult = (RooFitResult*) FDenomFitResults->Get("fitresult");
  RooRealVar* nXTbFit       = (RooRealVar*)   DsFitResult->floatParsFinal().find("nXTb");
  RooRealVar* nXTgFit       = (RooRealVar*)   DsFitResult->floatParsFinal().find("nXTg") ;
  RooRealVar* DsYield       = (RooRealVar*) DsFitResult->floatParsFinal().find("TotYield") ;
  TH2F* HDsMassData         = (TH2F*) FDenomFitResults->Get("HDsMassData") ;
  TH2F* TotSigPdfHistoFit   = (TH2F*) FDenomFitResults->Get("TotSigPdfHistoFit") ;
  if(!nXTbFit)          { cout << "nXTb par not found"   << endl ; return 0 ; }
  if(!nXTgFit)          { cout << "nXTg par not found"   << endl ; return 0 ; }
  if(!DsFitResult)      { cout << "No DsFitResult"       << endl ; return 0 ; }
  if(!HDsMassData)      { cout << "No HDsMassData "      << endl ; return 0 ; }
  if(!DsYield)          { cout << "DsYield not found"    << endl ; return 0 ; }
  if(!TotSigPdfHistoFit){ cout << "No TotSigPdfHistoFit" << endl ; return 0 ; }
  
  //declare text files with produced Ds and Muon yields as a function of nXT
  ifstream DsProducedFile; 
  DsProducedFile.open("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/DsProduced.txt");
  
  Float_t TotDsProd = 0 ;
  Float_t  TotDsRec = 0 ;
  Float_t TotNuProd = NEventsGenerated[SignalMode] ;
  Float_t  TotNuRec = 0 ;
  Float_t   DsProd[NXT] ;
  Float_t   NuProd[NXT] ;
  Float_t    DsRec[NXT] ;
  Float_t    NuRec[NXT] ;
  Float_t DsRecErr[NXT] ;
  Float_t NuRecErr[NXT] ;
  
  Float_t multiplication[NSignalMode] ; Float_t correctionFactor[NSignalMode] ; TString units[NSignalMode] ; TString trueValue[NSignalMode] ;
  multiplication[0] = 1000 ; correctionFactor[0] = 1000.0       ; units[0] = "10^{-3}" ; trueValue[0] = "6.16" ;
  multiplication[1] = 1000 ; correctionFactor[1] = 1000.0       ; units[1] = "10^{-3}" ; trueValue[1] = "0.00" ;
  multiplication[2] =  100 ; correctionFactor[2] = 100.0/0.1736 ; units[2] = "10^{-2}" ; trueValue[2] = "6.40" ;
  multiplication[3] =  100 ; correctionFactor[3] = 100.0/0.1785 ; units[3] = "10^{-2}" ; trueValue[3] = "6.40" ;
  multiplication[4] =  100 ; correctionFactor[4] = 100.0/0.1091 ; units[4] = "10^{-2}" ; trueValue[4] = "6.40" ;
  
  for(Int_t j=0 ; j<NXT ; j++)
  {
    //Generated Ds
    Int_t indexcheckn ;
    Float_t NProdDs = 0.0 ;
    DsProducedFile >> indexcheckn >> NProdDs ;   
    if(indexcheckn!=j){ cout << " NXT index does not match for Ds produced j=" << j << endl ; exit(0) ; }
    DsProd[j] =  NProdDs ;
    TotDsProd += NProdDs ;
    
    //Reconstructed Ds
    TH2F* HDsEff = (TH2F*) FEffDenom->Get(TString("HDsMassTrue2D_nXT")+(long)j);
    if(!HDsEff){ cout << "No HDsEff" << j << endl ; return 0 ; }
    DsRec[j]     =      HDsEff->Integral(1,4,DenominatorDsMassBinMin,DenominatorDsMassBinMax) ;
    DsRecErr[j]  = sqrt(HDsEff->Integral(1,4,DenominatorDsMassBinMin,DenominatorDsMassBinMax));
    TotDsRec    +=      HDsEff->Integral(1,4,DenominatorDsMassBinMin,DenominatorDsMassBinMax) ;

    //Reconstructed MuNu
    TH1F* HNumeratorEff = (TH1F*) FEffNumerator->Get(TString(SignalModeName[SignalMode]+"NXT"+(long)j));
    if(!HNumeratorEff){ cout << "No HNumeratorEff" << j <<  endl ; return 0 ; }
    NuRec[j]     =      HNumeratorEff->Integral();
    NuRecErr[j]  = sqrt(HNumeratorEff->Integral());
    TotNuRec    +=      HNumeratorEff->Integral();
  }
     
  // Determine nXT gen weights and the distribution of Numerator generated
  Float_t GenNXTWeights[NXT];
  for(Int_t j=0 ; j<NXT ; j++)
  {
    GenNXTWeights[j] = DsProd[j]/TotDsProd ;
    NuProd[j] = TotNuProd*GenNXTWeights[j] ;
  }
  
  for(Int_t j=0 ; j<NXT ; j++)
  {
    NumeratorEff[j]    = (_DataType==2 ? 0.999837 : 1.0 )*(NuRec[j]*DsProd[j]/(NuProd[j]*DsRec[j]));
    NumeratorEffErr[j] = (_DataType==2 ? 0.999837 : 1.0 )*NumeratorEff[j]*sqrt(pow(NuRecErr[j]/NuRec[j],2) + pow(DsRecErr[j]/DsRec[j],2)) ;
  }
    
  // True MC Reco Weights
  Float_t totMCweight     = 0.0 ;
  Float_t TotNumeratorEff = 0.0 ;
  for(Int_t j=0 ; j<NXT ; j++)
  {
    totMCweight     += truew[j] ;
    TotNumeratorEff += truew[j]*NumeratorEff[j] ;
  }
  if(fabs(totMCweight-1.0)>1e-5) { cout << " MC weights do not add up to 1." << endl ; return 0 ; }
    
  //------------------------------
  // Fitted nXT  weights
  //------------------------------
  if(HnXTWMPars) nXTa->setVal(HnXTWMPars->GetBinContent(1)) ;
  // Value of nXTa was changed from its nominal value --> must be set properly here 
  nXTb->setVal(  nXTbFit->getVal()  ) ;
  nXTb->setError(nXTbFit->getError()) ;
  nXTg->setVal(  nXTgFit->getVal()  ) ;
  nXTg->setError(nXTgFit->getError()) ;
  
  //------------------------------
  // Compute the Data weighted efficiency
  //------------------------------
  Float_t totDataweight = 0.0 ;
  TotNumeratorEff = 0.0 ;
  for(Int_t j=0 ; j<NXT ; j++)
  {
    totDataweight += nXTCoefWM[j]->getVal() ;
    // Note that these coefficients are being updated as long as the first three are updated
    TotNumeratorEff += nXTCoefWM[j]->getVal()*NumeratorEff[j] ;
  }
  if(fabs(totDataweight-1.)>1e-5){ cout << " Fit weights do not add up to 1." << endl ; return 0 ; }
  
  return TotNumeratorEff ;
}

Float_t DRecoDsToEllNuPlots::ComputeBF(TFile* FDenomFitResults,TFile* FNumeratorFitResults,TFile* FEffDenom,TFile* FEffNumerator, Int_t SignalMode, Bool_t useTrueValues)
{
  cout << endl << _DataType << endl ;
  
  //------------------------------
  // Numerator efficiencies
  //------------------------------
  if(FNumeratorFitResults->IsZombie()){ cout << "Bad NuFit File"         << endl ; return 0 ; } gROOT->cd() ;
  if(FEffNumerator->IsZombie())       { cout << "Bad FNumeratorEff File" << endl ; return 0 ; } gROOT->cd() ;
  if(FEffDenom->IsZombie())           { cout << "Bad FDsEff File"        << endl ; return 0 ; } gROOT->cd() ;
  if(FDenomFitResults->IsZombie())    { cout << "Bad DsFit File"         << endl ; return 0 ; } gROOT->cd() ;
  Float_t    NumeratorEff[NXT] ;
  Float_t NumeratorEffErr[NXT] ;
  
  TH1F* HnXTWMPars = (TH1F*) FDenomFitResults->Get("HnXTWMPars") ;
  
  TString fitResultsName ;
  if(_DataType==1) fitResultsName = TString(TString("fitResults")     + SignalModeName[SignalMode] + "RunAll") ;
  if(_DataType==2) fitResultsName = TString(TString("fitResultsData") + SignalModeName[SignalMode] + "RunAll") ;
  TString fractionName ;
  if(_DataType==1) fractionName = TString(TString("fSignalRunAll") + SignalModeName[SignalMode] + EExtraCutString[1]) ;
  if(_DataType==2) fractionName = TString(TString("fDataRunAll")   + SignalModeName[SignalMode] + EExtraCutString[1]) ;
  TString histogramName ;
  if(_DataType==1) histogramName = TString(SignalModeName[SignalMode] + "_RunAll"      + EExtraCutString[1]) ;
  if(_DataType==2) histogramName = TString(SignalModeName[SignalMode] + "_Data_RunAll" + EExtraCutString[1]) ;
  RooFitResult* NuFitResult = (RooFitResult*) FNumeratorFitResults->Get(fitResultsName) ;
  RooRealVar*       NuYield = (RooRealVar*)   NuFitResult->floatParsFinal().find(fractionName) ;
  TH2F* HNuMassData = (TH2F*) FNumeratorFitResults->Get(histogramName) ;
  if(!NuFitResult){ cout << "No NuFitResult" << endl ; return 0 ; }
  if(!NuYield)    { cout << "fSig not found" << endl ; return 0 ; }
  if(!HNuMassData){ cout << "No HNuMassData" << endl ; return 0 ; }
  
  RooFitResult* DsFitResult = (RooFitResult*) FDenomFitResults->Get("fitresult");
  RooRealVar* nXTbFit       = (RooRealVar*)   DsFitResult->floatParsFinal().find("nXTb");
  RooRealVar* nXTgFit       = (RooRealVar*)   DsFitResult->floatParsFinal().find("nXTg") ;
  RooRealVar* DsYield       = (RooRealVar*) DsFitResult->floatParsFinal().find("TotYield") ;
  TH1F* HnXTWeights         = (TH1F*) FDenomFitResults->Get("HnXTweights") ;
  TH2F* HDsMassData         = (TH2F*) FDenomFitResults->Get("HDsMassData") ;
  TH2F* TotSigPdfHistoFit   = (TH2F*) FDenomFitResults->Get("TotSigPdfHistoFit") ;
  if(!nXTbFit)          { cout << "nXTb par not found"   << endl ; return 0 ; }
  if(!nXTgFit)          { cout << "nXTg par not found"   << endl ; return 0 ; }
  if(!DsFitResult)      { cout << "No DsFitResult"       << endl ; return 0 ; }
  if(!HDsMassData)      { cout << "No HDsMassData "      << endl ; return 0 ; }
  if(!DsYield)          { cout << "DsYield not found"    << endl ; return 0 ; }
  if(!TotSigPdfHistoFit){ cout << "No TotSigPdfHistoFit" << endl ; return 0 ; }
  
  //declare text files with produced Ds and Muon yields as a function of nXT
  ifstream DsProducedFile; 
  DsProducedFile.open("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/DsProduced.txt");
  
  Float_t TotDsProd = 0 ;
  Float_t  TotDsRec = 0 ;
  Float_t TotNuProd = NEventsGenerated[SignalMode] ;
  Float_t  TotNuRec = 0 ;
  Float_t   DsProd[NXT] ;
  Float_t   NuProd[NXT] ;
  Float_t    DsRec[NXT] ;
  Float_t    NuRec[NXT] ;
  Float_t DsRecErr[NXT] ;
  Float_t NuRecErr[NXT] ;
  
  Float_t multiplication[NSignalMode] ; Float_t correctionFactor[NSignalMode] ; TString units[NSignalMode] ; TString trueValue[NSignalMode] ;
  multiplication[0] = 1000 ; correctionFactor[0] = 1000.0       ; units[0] = "10^{-3}" ; trueValue[0] = "6.16" ;
  multiplication[1] = 1000 ; correctionFactor[1] = 1000.0       ; units[1] = "10^{-3}" ; trueValue[1] = "0.00" ;
  multiplication[2] =  100 ; correctionFactor[2] = 100.0/0.1736 ; units[2] = "10^{-2}" ; trueValue[2] = "6.40" ;
  multiplication[3] =  100 ; correctionFactor[3] = 100.0/0.1785 ; units[3] = "10^{-2}" ; trueValue[3] = "6.40" ;
  multiplication[4] =  100 ; correctionFactor[4] = 100.0/0.1091 ; units[4] = "10^{-2}" ; trueValue[4] = "6.40" ;
  
  for(Int_t j=0 ; j<NXT ; j++)
  {
    //Generated Ds
    Int_t indexcheckn ;
    Float_t NProdDs = 0.0 ;
    DsProducedFile >> indexcheckn >> NProdDs ;   
    if(indexcheckn!=j){ cout << " NXT index does not match for Ds produced j=" << j << endl ; exit(0) ; }
    DsProd[j] =  NProdDs ;
    TotDsProd += NProdDs ;
    
    //Reconstructed Ds
    TH2F* HDsEff = (TH2F*) FEffDenom->Get(TString("HDsMassTrue2D_nXT")+(long)j);
    if(!HDsEff)
    {
      cout << "No HDsEff" << j << endl ;
      return 0 ;
    }
    DsRec[j]     =      HDsEff->Integral(1,4,DenominatorDsMassBinMin,DenominatorDsMassBinMax) ;
    DsRecErr[j]  = sqrt(HDsEff->Integral(1,4,DenominatorDsMassBinMin,DenominatorDsMassBinMax));
    TotDsRec    +=      HDsEff->Integral(1,4,DenominatorDsMassBinMin,DenominatorDsMassBinMax) ;

    //Reconstructed MuNu
    TH1F* HNumeratorEff = (TH1F*) FEffNumerator->Get(TString(SignalModeName[SignalMode]+"NXT"+(long)j));
    if(!HNumeratorEff){ cout << "No HNumeratorEff" << j <<  endl ; return 0 ; }
    NuRec[j]     =      HNumeratorEff->Integral();
    NuRecErr[j]  = sqrt(HNumeratorEff->Integral());
    TotNuRec    +=      HNumeratorEff->Integral();
    cout << "Number of events for " << SignalModeName[SignalMode] << " for NXT=" << j << " is " << NuRec[j] << endl ;
  }
  cout << "Number of events for " << SignalModeName[SignalMode] << " for all values of NXT is " << TotNuRec << endl ;
  cout << "Number of generated events for " << SignalModeName[SignalMode] << " is " << TotNuProd << endl ;
     
  // Determine nXT gen weights and the distribution of Numerator generated
  Float_t GenNXTWeights[NXT];
  cout<<" Ds Gen weights = ";
  for(Int_t j=0 ; j<NXT ; j++)
  {
    GenNXTWeights[j] = DsProd[j]/TotDsProd ;
    cout << " , " << GenNXTWeights[j] << endl ; 
    NuProd[j] = TotNuProd*GenNXTWeights[j] ;
  }
  
  cout << " nXT efficiencies: " << endl ;
  for(Int_t j=0 ; j<NXT ; j++)
  {
    NumeratorEff[j]    = (_DataType==2 ? 0.999837 : 1.0 )*(NuRec[j]*DsProd[j]/(NuProd[j]*DsRec[j]));
    NumeratorEffErr[j] = (_DataType==2 ? 0.999837 : 1.0 )*NumeratorEff[j]*sqrt(pow(NuRecErr[j]/NuRec[j],2) + pow(DsRecErr[j]/DsRec[j],2)) ;
    cout << setprecision(3) << fixed << " & " << NumeratorEff[j] << " $\\pm$ " << NumeratorEffErr[j] << endl ;
  }
    
  cout << " \\\\" << endl;
  cout << scientific << "The Ds efficiency is " << TotDsRec/TotDsProd << endl ;
  cout << scientific << "The " << SignalModeName[SignalMode] << " efficiency is " << TotNuRec/TotNuProd << endl ;
  
  // True MC Reco Weights
  Float_t totMCweight     = 0.0 ;
  Float_t TotNumeratorEff = 0.0 ;
  for(Int_t j=0 ; j<NXT ; j++)
  {
    totMCweight     += truew[j] ;
    TotNumeratorEff += truew[j]*NumeratorEff[j] ;
    cout << " MCweight" << j << " = " << truew[j] << endl ;
  }
  if(fabs(totMCweight-1.0)>1e-5) { cout << " MC weights do not add up to 1." << endl ; return 0 ; }
  cout << "The MC true weighted " << SignalModeName[SignalMode] << " efficiency is " << TotNumeratorEff << endl ;
  
  cout << endl;
  //------------------------------
  /// Ds yield 
  //------------------------------
  TotYield.setVal(DsYield->getVal()); 
  TotYield.setError(DsYield->getError());
  Float_t NRecoDs    =   HDsMassData->Integral()*TotYield.getVal()*TotSigPdfHistoFit->Integral(1,4,DenominatorDsMassBinMin,DenominatorDsMassBinMax)/TotSigPdfHistoFit->Integral();
  Float_t NRecoDsErr = HDsMassData->Integral()*TotYield.getError()*TotSigPdfHistoFit->Integral(1,4,DenominatorDsMassBinMin,DenominatorDsMassBinMax)/TotSigPdfHistoFit->Integral();

  if(useTrueValues)
  {
    // Use true Ds yield  
    TFile FDenomTrue("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/DenominatorEff.root","read");
    NRecoDs = ((TH1F*)FDenomTrue.Get("HDsMassTrue"))->Integral(DenominatorDsMassBinMin,DenominatorDsMassBinMax) ;
    cout << "!! Using true Ds Yield !!" << endl ;
  }
  cout << "The total Ds yield is " << NRecoDs << " +- " << NRecoDsErr << " --> " << NRecoDsErr*100.0/NRecoDs << "%" << endl << endl ;
  
  if(!useTrueValues)
  {
    // For MC lumi weight the Ds yield because Aidan has lumi weighted his MC
    if(_DataType==1)
    {
      NRecoDs    =    NRecoDs*(1.0/2.04+0.0974/1.93)/(1+0.0974);
      NRecoDsErr = NRecoDsErr*(1.0/2.04+0.0974/1.93)/(1+0.0974);
    }
    cout << "The total Ds yield after Lumi Weighting is " << NRecoDs << " +- " << NRecoDsErr << " --> " << NRecoDsErr*100.0/NRecoDs << "%" << endl ;
  }
  cout << endl;
  
  //------------------------------
  // Fitted nXT  weights
  //------------------------------
  if(HnXTWMPars) nXTa->setVal(HnXTWMPars->GetBinContent(1)) ;
  // Value of nXTa was changed from its nominal value --> must be set properly here 
  nXTb->setVal(  nXTbFit->getVal()  ) ;
  nXTb->setError(nXTbFit->getError()) ;
  nXTg->setVal(  nXTgFit->getVal()  ) ;
  nXTg->setError(nXTgFit->getError()) ;
  
  //------------------------------
  // Compute the Data weighted efficiency
  //------------------------------
  Float_t totDataweight = 0.0 ;
  TotNumeratorEff = 0.0 ;
  for(Int_t j=0 ; j<NXT ; j++)
  {
    totDataweight += nXTCoefWM[j]->getVal() ;
    // Note that these coefficients are being updated as long as the first three are updated
    TotNumeratorEff += nXTCoefWM[j]->getVal()*NumeratorEff[j] ;
    cout << " FitWeight " << j << " = " << nXTCoefWM[j]->getVal() << "   hist = " << HnXTWeights->GetBinContent(j+1) << endl;
  }
  if(fabs(totDataweight-1.)>1e-5){ cout << " Fit weights do not add up to 1." << endl ; return 0 ; }
  
  if(useTrueValues)
  {
    // Use true weights
    cout << "!! Using true weights !!" << endl ;
    TotNumeratorEff = 0.0 ;
    for(Int_t j=0 ; j<NXT ; j++)
    {
      TotNumeratorEff += truew[j]*NumeratorEff[j];
    }
    // Use total efficiency
    TotNumeratorEff = (TotNuRec*TotDsProd)/(TotNuProd*TotDsRec);
    cout << " !! Using Total Efficiency (No nXT reweighting) !!    = " << TotNumeratorEff << endl ;
  }

  cout << "The reweighted " << SignalModeName[SignalMode] << " efficiency is " << TotNumeratorEff << endl ;
  
  //------------------------------
  // Numerator yield
  //------------------------------
  cout << "extracted " << SignalModeName[SignalMode] << " Data: " << HNuMassData->GetName() << endl;
  
  Float_t    NRecoNu =   NuYield->getVal()*HNuMassData->Integral();
  Float_t NRecoNuErr = NuYield->getError()*HNuMassData->Integral();
  
  if(useTrueValues)
  {
    TFile  FNumTrue("/u/br/aidanrc/ana50/AWG90/aidanrc/Roofit/Roofit.root","read");
    TTree* TNumTrue = (TTree*) FNumTrue.Get("tRoofit");
    TH1F   HNumTrue("HNumTrue","",300,-1,4.);
    TNumTrue->Draw("mm2>>HNumTrue",TString(TString("(SignalMode==")+(long)SignalMode+"&&type==2)"));
    NRecoNu = HNumTrue.Integral();
    cout << "!! Using true MuNu Yield !!" << endl;
  }
  cout << "The total neutrino yield is " << NRecoNu << " +- " << NuYield->getError()*HNuMassData->Integral() << " --> " << NRecoNuErr*100.0/NRecoNu << "%" << endl;
  cout << endl;
  
  //---------------------------
  // Compute the Branching fraction
  //----------------------------
  Float_t BF = NRecoNu/(NRecoDs*TotNumeratorEff);
  cout << "-----------------------------------------------" << endl ;
  cout << fixed << " The Branching fraction for " << SignalModeName[SignalMode] << " is (" << BF*multiplication[SignalMode] << " +/- " <<  BF*multiplication[SignalMode]*NRecoNuErr/NRecoNu << ") X " << units[SignalMode] << endl ;
  cout << "-----------------------------------------------" << endl ;
  switch(SignalMode)
  {
  case 2:
  case 3:
  case 4:
     cout << "-----------------------------------------------" << endl;
     cout << fixed<<" The Branching fraction for " << SignalModeName[SignalMode] << " after tau nu correction is (" << (BF*correctionFactor[SignalMode]) <<" +/- " << (BF*correctionFactor[SignalMode]*NRecoNuErr/NRecoNu) << ") X " << units[SignalMode] << endl ;
     cout << "-----------------------------------------------" << endl;
     break ;
  }
  if(useTrueValues) cout << "True value = " << trueValue[SignalMode] << " X " << units[SignalMode] << endl ;
  
  cout << endl;
  //----------------------------------
  // Compute the error
  //----------------------------------
  // Error on Denominator: DsYield*(weighted NumeratorEfficiency)
  // Set these pointers for later calculations
  RooRealVar* FitVar[3] ;
  FitVar[0] = &TotYield ;
  FitVar[1] = nXTb ;
  FitVar[2] = nXTg ;
  
  // Read in the full correlation matrix
  cout << "Covariance Matrix:" << endl ;
  Float_t CovMatrix[3][3] ;
  for(Int_t j=0 ; j<3 ; j++)
  {
    cout << FitVar[j]->GetName() << "     " ;
    for(Int_t k=0 ; k<3 ; k++)
    {
      CovMatrix[j][k] = DsFitResult->correlation(*FitVar[j],*FitVar[k]) * (FitVar[j]->getError()) * (FitVar[k]->getError()) ;
      cout << scientific << " (" << CovMatrix[j][k] << ")" ;
    }
    cout << endl ;
  }
  return BF ;
}

Float_t DRecoDsToEllNuPlots::Lambda()
{
  SignalMode = 1 ;
  _DataType = 2 ;
  TString prefix = "/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/" ;
  TFile* FDenomFitResults     = new TFile(TString(prefix+"DenominatorFitResult_Data.root"),"read") ;
  TFile* FNumeratorFitResults = new TFile(TString(prefix+"DsToENuFitResults_Data.root")   ,"read") ;
  TFile* FEffDenom            = new TFile(TString(prefix+"DenominatorEff.root")           ,"read") ;
  TFile* FEffNumerator        = new TFile(TString(prefix+"NumeratorEfficiencies.root")    ,"read") ;
  gROOT->cd() ;

  //------------------------------
  // Numerator efficiencies
  //------------------------------
  if(FNumeratorFitResults->IsZombie()){ cout << "Bad NuFit File"         << endl ; return 0 ; } gROOT->cd() ;
  if(FEffNumerator->IsZombie())       { cout << "Bad FNumeratorEff File" << endl ; return 0 ; } gROOT->cd() ;
  if(FEffDenom->IsZombie())           { cout << "Bad FDsEff File"        << endl ; return 0 ; } gROOT->cd() ;
  if(FDenomFitResults->IsZombie())    { cout << "Bad DsFit File"         << endl ; return 0 ; } gROOT->cd() ;
  Float_t    NumeratorEff[NXT] ;
  Float_t NumeratorEffErr[NXT] ;
  
  TH1F* HnXTWMPars = (TH1F*) FDenomFitResults->Get("HnXTWMPars") ;
  
  TString fitResultsName = TString("fitResultsDataDsToENuRunAll")  ;
  TString fractionName   = TString("fDataRunAllDsToENu_EExtraCut") ;
  TString histogramName = TString("DsToENu_Data_RunAll_EExtraCut") ;
  RooFitResult* NuFitResult = (RooFitResult*) FNumeratorFitResults->Get(fitResultsName) ;
  RooRealVar*       NuYield = (RooRealVar*)   NuFitResult->floatParsFinal().find(fractionName) ;
  TH2F* HNuMassData = (TH2F*) FNumeratorFitResults->Get(histogramName) ;
  if(!NuFitResult){ cout << "No NuFitResult" << endl ; return 0 ; }
  if(!NuYield)    { cout << "fSig not found" << endl ; return 0 ; }
  if(!HNuMassData){ cout << "No HNuMassData" << endl ; return 0 ; }
  
  RooFitResult* DsFitResult = (RooFitResult*) FDenomFitResults->Get("fitresult");
  RooRealVar* nXTbFit       = (RooRealVar*)   DsFitResult->floatParsFinal().find("nXTb");
  RooRealVar* nXTgFit       = (RooRealVar*)   DsFitResult->floatParsFinal().find("nXTg") ;
  RooRealVar* DsYield       = (RooRealVar*) DsFitResult->floatParsFinal().find("TotYield") ;
  TH1F* HnXTWeights         = (TH1F*) FDenomFitResults->Get("HnXTweights") ;
  TH2F* HDsMassData         = (TH2F*) FDenomFitResults->Get("HDsMassData") ;
  TH2F* TotSigPdfHistoFit   = (TH2F*) FDenomFitResults->Get("TotSigPdfHistoFit") ;
  if(!nXTbFit)          { cout << "nXTb par not found"   << endl ; return 0 ; }
  if(!nXTgFit)          { cout << "nXTg par not found"   << endl ; return 0 ; }
  if(!DsFitResult)      { cout << "No DsFitResult"       << endl ; return 0 ; }
  if(!HDsMassData)      { cout << "No HDsMassData "      << endl ; return 0 ; }
  if(!DsYield)          { cout << "DsYield not found"    << endl ; return 0 ; }
  if(!TotSigPdfHistoFit){ cout << "No TotSigPdfHistoFit" << endl ; return 0 ; }
  
  //declare text files with produced Ds and Muon yields as a function of nXT
  ifstream DsProducedFile; 
  DsProducedFile.open("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/DsProduced.txt");
  
  Float_t TotDsProd = 0 ;
  Float_t  TotDsRec = 0 ;
  Float_t TotNuProd = NEventsGenerated[SignalMode] ;
  Float_t  TotNuRec = 0 ;
  Float_t   DsProd[NXT] ;
  Float_t   NuProd[NXT] ;
  Float_t    DsRec[NXT] ;
  Float_t    NuRec[NXT] ;
  Float_t DsRecErr[NXT] ;
  Float_t NuRecErr[NXT] ;
  
  Float_t multiplication[NSignalMode] ; Float_t correctionFactor[NSignalMode] ; TString units[NSignalMode] ; TString trueValue[NSignalMode] ;
  multiplication[0] = 1000 ; correctionFactor[0] = 1000.0       ; units[0] = "10^{-3}" ; trueValue[0] = "6.16" ;
  multiplication[1] = 1000 ; correctionFactor[1] = 1000.0       ; units[1] = "10^{-3}" ; trueValue[1] = "0.00" ;
  multiplication[2] =  100 ; correctionFactor[2] = 100.0/0.1736 ; units[2] = "10^{-2}" ; trueValue[2] = "6.40" ;
  multiplication[3] =  100 ; correctionFactor[3] = 100.0/0.1785 ; units[3] = "10^{-2}" ; trueValue[3] = "6.40" ;
  multiplication[4] =  100 ; correctionFactor[4] = 100.0/0.1091 ; units[4] = "10^{-2}" ; trueValue[4] = "6.40" ;
  
  for(Int_t j=0 ; j<NXT ; j++)
  {
    //Generated Ds
    Int_t indexcheckn ;
    Float_t NProdDs = 0.0 ;
    DsProducedFile >> indexcheckn >> NProdDs ;   
    if(indexcheckn!=j){ cout << " NXT index does not match for Ds produced j=" << j << endl ; exit(0) ; }
    DsProd[j] =  NProdDs ;
    TotDsProd += NProdDs ;
    
    //Reconstructed Ds
    TH2F* HDsEff = (TH2F*) FEffDenom->Get(TString("HDsMassTrue2D_nXT")+(long)j);
    if(!HDsEff)
    {
      cout << "No HDsEff" << j << endl ;
      return 0 ;
    }
    DsRec[j]     =      HDsEff->Integral(1,4,DenominatorDsMassBinMin,DenominatorDsMassBinMax) ;
    DsRecErr[j]  = sqrt(HDsEff->Integral(1,4,DenominatorDsMassBinMin,DenominatorDsMassBinMax));
    TotDsRec    +=      HDsEff->Integral(1,4,DenominatorDsMassBinMin,DenominatorDsMassBinMax) ;

    //Reconstructed MuNu
    TH1F* HNumeratorEff = (TH1F*) FEffNumerator->Get(TString(SignalModeName[SignalMode]+"NXT"+(long)j));
    if(!HNumeratorEff){ cout << "No HNumeratorEff" << j <<  endl ; return 0 ; }
    NuRec[j]     =      HNumeratorEff->Integral();
    NuRecErr[j]  = sqrt(HNumeratorEff->Integral());
    TotNuRec    +=      HNumeratorEff->Integral();
    cout << "Number of events for " << SignalModeName[SignalMode] << " for NXT=" << j << " is " << NuRec[j] << endl ;
  }
  cout << "Number of events for " << SignalModeName[SignalMode] << " for all values of NXT is " << TotNuRec << endl ;
  cout << "Number of generated events for " << SignalModeName[SignalMode] << " is " << TotNuProd << endl ;
     
  // Determine nXT generated weights and the distribution of Numerator generated
  Float_t GenNXTWeights[NXT];
  cout<<" Ds Gen weights = ";
  for(Int_t j=0 ; j<NXT ; j++)
  {
    GenNXTWeights[j] = DsProd[j]/TotDsProd ;
    cout << " , " << GenNXTWeights[j] << endl ; 
    NuProd[j] = TotNuProd*GenNXTWeights[j] ;
  }
  
  cout << " nXT efficiencies: " << endl ;
  for(Int_t j=0 ; j<NXT ; j++)
  {
    NumeratorEff[j]    = (_DataType==2 ? 0.999837 : 1.0 )*(NuRec[j]*DsProd[j]/(NuProd[j]*DsRec[j]));
    NumeratorEffErr[j] = (_DataType==2 ? 0.999837 : 1.0 )*NumeratorEff[j]*sqrt(pow(NuRecErr[j]/NuRec[j],2) + pow(DsRecErr[j]/DsRec[j],2)) ;
    cout << setprecision(3) << fixed << " & " << NumeratorEff[j] << " $\\pm$ " << NumeratorEffErr[j] << endl ;
  }
    
  cout << " \\\\" << endl;
  cout << scientific << "The Ds efficiency is " << TotDsRec/TotDsProd << endl ;
  cout << scientific << "The " << SignalModeName[SignalMode] << " efficiency is " << TotNuRec/TotNuProd << endl ;
  
  // True MC Reco Weights
  Float_t totMCweight     = 0.0 ;
  Float_t TotNumeratorEff = 0.0 ;
  for(Int_t j=0 ; j<NXT ; j++)
  {
    totMCweight     += truew[j] ;
    TotNumeratorEff += truew[j]*NumeratorEff[j] ;
    cout << " MCweight" << j << " = " << truew[j] << endl ;
  }
  if(fabs(totMCweight-1.0)>1e-5) { cout << " MC weights do not add up to 1." << endl ; return 0 ; }
  cout << "The MC true weighted " << SignalModeName[SignalMode] << " efficiency is " << TotNumeratorEff << endl ;
  
  cout << endl;
  //------------------------------
  /// Ds yield 
  //------------------------------
  TotYield.setVal(DsYield->getVal()); 
  TotYield.setError(DsYield->getError());
  Float_t NRecoDs    =   HDsMassData->Integral()*TotYield.getVal()*TotSigPdfHistoFit->Integral(1,4,DenominatorDsMassBinMin,DenominatorDsMassBinMax)/TotSigPdfHistoFit->Integral();
  Float_t NRecoDsErr = HDsMassData->Integral()*TotYield.getError()*TotSigPdfHistoFit->Integral(1,4,DenominatorDsMassBinMin,DenominatorDsMassBinMax)/TotSigPdfHistoFit->Integral();

  // For MC lumi weight the Ds yield because Aidan has lumi weighted his MC
  if(_DataType==1)
  {
    NRecoDs    =    NRecoDs*(1.0/2.04+0.0974/1.93)/(1+0.0974);
    NRecoDsErr = NRecoDsErr*(1.0/2.04+0.0974/1.93)/(1+0.0974);
  }
  cout << "The total Ds yield after Lumi Weighting is " << NRecoDs << " +- " << NRecoDsErr << " --> " << NRecoDsErr*100.0/NRecoDs << "%" << endl ;
  cout << endl;
  
  //------------------------------
  // Fitted nXT  weights
  //------------------------------
  if(HnXTWMPars) nXTa->setVal(HnXTWMPars->GetBinContent(1)) ;
  // Value of nXTa was changed from its nominal value --> must be set properly here 
  nXTb->setVal(  nXTbFit->getVal()  ) ;
  nXTb->setError(nXTbFit->getError()) ;
  nXTg->setVal(  nXTgFit->getVal()  ) ;
  nXTg->setError(nXTgFit->getError()) ;
  
  //----------------------------------
  // Compute the error
  //----------------------------------
  // Error on Denominator: DsYield*(weighted NumeratorEfficiency)
  // Set these pointers for later calculations
  Float_t alpha = -1.32 ;
  Float_t beta  = nXTb->getVal() ;
  Float_t gamma = nXTg->getVal() ;
  Float_t weight[NNXT] ;
  Float_t sumWeight = 0 ;
  Float_t weightEff[NNXT] ;
  Float_t totalWeightEff = 0 ;
  for(Int_t i=0 ; i<NNXT ; i++)
  {
    weight[i] = pow(i-alpha,beta)*exp(-i*gamma) ;
    sumWeight += weight[i] ;
  }
  for(Int_t i=0 ; i<NNXT ; i++)
  {
    weight[i] = weight[i]/sumWeight ;
    weightEff[i] = weight[i]*NumeratorEff[i] ;
    totalWeightEff += weightEff[i] ;
    cout << "weight" << i << " " << weight[i] << endl ;
  }
  
  
  Float_t SumW = 0.0 ;
  Float_t SumB = 0.0 ;
  Float_t SumG = 0.0 ;
  for(Int_t k=0 ; k<NXT ; k++)
  { // Sum over nXT
    SumW += pow(k-alpha,beta)*exp(-gamma*k) ;
    SumB += pow(k-alpha,beta)*exp(-gamma*k)*log(k-alpha) ;
    SumG += pow(k-alpha,beta)*exp(-gamma*k)*k ;
  }
  //derivatives w_j w.r.t to the beta and gamma
  Float_t DerivWeightB[NNXT] ;
  Float_t DerivWeightG[NNXT] ;
  for(Int_t j=0 ; j<NXT ; j++)
  { // Sum over nXT
    DerivWeightB[j] = (nXTCoefWM[j]->getVal()/SumW)*(SumW*log(j-nXTa->getVal()) - SumB) ;
    DerivWeightG[j] = nXTCoefWM[j]->getVal()*(SumG/SumW-j) ;
  }

  // Finally derivatives of the denominator N*epsilon w.r.t to the floated parameters
  Float_t DerivDenom[3] ;
  DerivDenom[0] = totalWeightEff ;
  DerivDenom[1] = 0.0 ;
  DerivDenom[2] = 0.0 ;
  for(Int_t j=0 ; j<NNXT ; j++)
  {
    DerivDenom[1] += NRecoDs*DerivWeightB[j]*weightEff[j] ;
    DerivDenom[2] += NRecoDs*DerivWeightG[j]*weightEff[j] ;
  }
  cout << "Total derivatives for beta, gamma: " << DerivDenom[1] << " " << DerivDenom[2] << endl ;
  
  const Int_t NPars = 10 ; // NDs , beta , gamma , efficiencies
  Float_t derivative[NPars] ;
  Float_t error[NPars] ;
  derivative[0] = 0 ; for(Int_t i=0 ; i<NNXT ; i++) derivative[0] += weightEff[i] ;
  //derivative[1] = 0 ; for(Int_t i=0 ; i<NNXT ; i++) derivative[1] += NRecoDs*weightEff[i]*log(i-alpha) ;
  //derivative[2] = 0 ; for(Int_t i=0 ; i<NNXT ; i++) derivative[2] += NRecoDs*weightEff[i]*i ;
  derivative[1] = DerivDenom[1] ;
  derivative[2] = DerivDenom[1] ;
  for(Int_t d=3 ; d<NPars ; d++){ derivative[d] = NRecoDs*weight[d-3] ; }
  error[0] = TotYield.getError() ;
  error[1] = nXTb->getError() ;
  error[2] = nXTg->getError() ;
  for(Int_t e=3 ; e<NPars ; e++){ error[e] = NumeratorEffErr[e-3] ; }
  for(Int_t i=0 ; i<NPars ; i++){ cout << "Parameter " << i << " derivative " << derivative[i] << " error " << error[i] << endl ; }
  
  RooRealVar* FitVar[3] ;
  FitVar[0] = &TotYield ;
  FitVar[1] = nXTb ;
  FitVar[2] = nXTg ;
  
  // Read in the full correlation matrix
  Float_t correlationMatrix[NPars][NPars] ;
  Float_t       errorMatrix[NPars][NPars] ;
  Float_t totalError = 0 ;
  cout << "Error matrix:" << endl ;
  for(Int_t i=0 ; i<NPars ; i++)
  {
    for(Int_t j=0 ; j<NPars ; j++)
    {
      correlationMatrix[i][j] = (i==j) ? 1 : 0 ;
      if(i<3 && j<3) correlationMatrix[i][j] = DsFitResult->correlation(*FitVar[i],*FitVar[j]) ;
      errorMatrix[i][j] = fabs(correlationMatrix[i][j]*error[i]*error[j]*derivative[i]*derivative[j]) ;
      totalError += errorMatrix[i][j] ;
      cout.width(12) ;
      cout << errorMatrix[i][j] ;
    }
    cout << endl ;
  }
  cout << "Correlation matrix:" << endl ;
  for(Int_t i=0 ; i<NPars ; i++)
  {
    for(Int_t j=0 ; j<NPars ; j++)
    {
      cout.width(12) ;
      cout << correlationMatrix[i][j] ;
    }
    cout << endl ;
  }
  cout << "Covariance matrix:" << endl ;
  for(Int_t i=0 ; i<NPars ; i++)
  {
    for(Int_t j=0 ; j<NPars ; j++)
    {
      cout.width(12) ;
      cout << correlationMatrix[i][j]*error[i]*error[j] ;
    }
    cout << endl ;
  }
  cout << "Covariance matrix relative:" << endl ;
  for(Int_t i=0 ; i<3 ; i++)
  {
    for(Int_t j=0 ; j<3 ; j++)
    {
      cout.width(12) ;
      cout << correlationMatrix[i][j]*FitVar[i]->getError()*FitVar[j]->getError() ;
    }
    cout << endl ;
  }
  for(Int_t i=0 ; i<3 ; i++)
  {
    cout << FitVar[i]->getVal() << " +/- " << FitVar[i]->getError() << endl ;
  }
  Float_t lambda = NRecoDs*totalWeightEff ;
  totalError = sqrt(totalError) ;
  cout << "lambda = " << lambda << " +/- " << totalError << endl ;
  

  //------------------------------
  // Compute the Data weighted efficiency
  //------------------------------
  Float_t totDataweight = 0.0 ;
  TotNumeratorEff = 0.0 ;
  for(Int_t j=0 ; j<NXT ; j++)
  {
    totDataweight += nXTCoefWM[j]->getVal() ;
    // Note that these coefficients are being updated as long as the first three are updated
    TotNumeratorEff += nXTCoefWM[j]->getVal()*NumeratorEff[j] ;
    cout << " FitWeight " << j << " = " << nXTCoefWM[j]->getVal() << "   hist = " << HnXTWeights->GetBinContent(j+1) << endl;
  }
  if(fabs(totDataweight-1.)>1e-5){ cout << " Fit weights do not add up to 1." << endl ; return 0 ; }

  cout << "The reweighted " << SignalModeName[SignalMode] << " efficiency is " << TotNumeratorEff << endl ;
  
  //------------------------------
  // Numerator yield
  //------------------------------
  cout << "extracted " << SignalModeName[SignalMode] << " Data: " << HNuMassData->GetName() << endl;
  
  Float_t    NRecoNu =   NuYield->getVal()*HNuMassData->Integral();
  Float_t NRecoNuErr = NuYield->getError()*HNuMassData->Integral();
  
  cout << "The total neutrino yield is " << NRecoNu << " +- " << NuYield->getError()*HNuMassData->Integral() << " --> " << NRecoNuErr*100.0/NRecoNu << "%" << endl;
  cout << endl;
  
  //---------------------------
  // Compute the Branching fraction
  //----------------------------
  Float_t BF = NRecoNu/(NRecoDs*TotNumeratorEff);
  cout << "-----------------------------------------------" << endl ;
  cout << fixed << " The Branching fraction for " << SignalModeName[SignalMode] << " is (" << BF*multiplication[SignalMode] << " +/- " <<  BF*multiplication[SignalMode]*NRecoNuErr/NRecoNu << ") X " << units[SignalMode] << endl ;
  cout << "-----------------------------------------------" << endl ;
  switch(SignalMode)
  {
  case 2:
  case 3:
  case 4:
     cout << "-----------------------------------------------" << endl;
     cout << fixed<<" The Branching fraction for " << SignalModeName[SignalMode] << " after tau nu correction is (" << (BF*correctionFactor[SignalMode]) <<" +/- " << (BF*correctionFactor[SignalMode]*NRecoNuErr/NRecoNu) << ") X " << units[SignalMode] << endl ;
     cout << "-----------------------------------------------" << endl;
     break ;
  }
  
  cout << endl;
  
  return BF ;
}

void DRecoDsToEllNuPlots::TrackingError()
{
  Float_t          OnPeakLuminosity[8] ;
  Float_t         OffPeakLuminosity[8] ;
  Float_t                correction[8] ;
  Float_t         trackingErrorStat[8] ;
  Float_t         trackingErrorSyst[8] ;
  Float_t        weightedCorrection[8] ;
  Float_t weightedTrackingErrorStat[8] ;
  Float_t weightedTrackingErrorSyst[8] ;
  Float_t                    weight[8] ;
  
  // R24 ChargedTracks
  correction[0] = 0.130 ; trackingErrorStat[0] = 0.061 ; trackingErrorSyst[0] = 0.118 ;
  correction[1] = 0.010 ; trackingErrorStat[1] = 0.038 ; trackingErrorSyst[1] = 0.093 ;
  correction[2] = 0.009 ; trackingErrorStat[2] = 0.043 ; trackingErrorSyst[2] = 0.192 ;
  correction[3] = 0.200 ; trackingErrorStat[3] = 0.025 ; trackingErrorSyst[3] = 0.094 ;
  correction[4] = 0.362 ; trackingErrorStat[4] = 0.022 ; trackingErrorSyst[4] = 0.086 ;
  correction[5] = 0.194 ; trackingErrorStat[5] = 0.028 ; trackingErrorSyst[5] = 0.125 ;
  correction[6] = 0.130 ; trackingErrorStat[6] = 0.222 ; trackingErrorSyst[6] = 0.073 ;
  correction[7] = 0.000 ; trackingErrorStat[7] = 0.000 ; trackingErrorSyst[7] = 0.000 ;
  weight[7]     = 0 ;

  OnPeakLuminosity[0] =  20.595 ; OffPeakLuminosity[0] =  2.621 ;
  OnPeakLuminosity[1] =  62.066 ; OffPeakLuminosity[1] =  7.030 ;
  OnPeakLuminosity[2] =  32.668 ; OffPeakLuminosity[2] =  2.495 ;
  OnPeakLuminosity[3] = 100.776 ; OffPeakLuminosity[3] = 10.228 ;
  OnPeakLuminosity[4] = 133.854 ; OffPeakLuminosity[4] = 14.546 ;
  OnPeakLuminosity[5] =  79.030 ; OffPeakLuminosity[5] =  7.886 ;
  OnPeakLuminosity[6] =0*41.449 ; OffPeakLuminosity[6] =0*4.168 ;
  OnPeakLuminosity[7] =     0.0 ; OffPeakLuminosity[7] =    0.0 ;
  for(Int_t i=0 ; i<6 ; i++)
  {
    OnPeakLuminosity[7]  +=  OnPeakLuminosity[i] ;
    OffPeakLuminosity[7] += OffPeakLuminosity[i] ;
  }
  Float_t totalLumi = OnPeakLuminosity[7]+OffPeakLuminosity[7] ;

  Int_t width = 16 ;
  cout << endl ;
  cout.width(6) ; cout << "Run" ;
  cout.width(width) ; cout << "Correction"     ;
  cout.width(width) ; cout << "Tracking stat"  ;
  cout.width(width) ; cout << "Tracking syst"  ;
  cout.width(width) ; cout << "Luminosity"     ;
  cout.width(width) ; cout << "Weight"         ;
  cout.width(width) ; cout << "wCorrection"    ;
  cout.width(width) ; cout << "wTracking stat" ;
  cout.width(width) ; cout << "wTracking syst" ;
  cout << endl ;
  for(Int_t i=0 ; i<6 ; i++)
  {
    Float_t lumi                 = OnPeakLuminosity[i]+OffPeakLuminosity[i] ;
    weight[i]                    = (lumi/totalLumi) ;
    weightedCorrection[i]        =        correction[i]*weight[i] ;
    weightedTrackingErrorStat[i] = trackingErrorStat[i]*weight[i] ;
    weightedTrackingErrorSyst[i] = trackingErrorSyst[i]*weight[i] ;
    correction[7]               += weightedCorrection[i]*weightedCorrection[i] ;
    trackingErrorStat[7]        += weightedTrackingErrorStat[i]*weightedTrackingErrorStat[i] ;
    trackingErrorSyst[7]        += weightedTrackingErrorSyst[i] ;
    weight[7]                   += weight[i] ;
    cout.width(6) ; cout << (i+1) ;
    cout.width(width) ; cout << correction[i]                ;
    cout.width(width) ; cout << trackingErrorStat[i]         ;
    cout.width(width) ; cout << trackingErrorSyst[i]         ;
    cout.width(width) ; cout << lumi                         ;
    cout.width(width) ; cout << weight[i]                    ;
    cout.width(width) ; cout << weightedCorrection[i]        ;
    cout.width(width) ; cout << weightedTrackingErrorStat[i] ;
    cout.width(width) ; cout << weightedTrackingErrorSyst[i] ;
    cout << endl ;
  }
  //correction[7] = sqrt(correction[7]) ;
  trackingErrorStat[7] = sqrt(trackingErrorStat[7]) ;
  Float_t trackingError = sqrt(trackingErrorStat[7]+trackingErrorSyst[7]*trackingErrorSyst[7]) ;
  cout.width(6) ; cout << "Total" ;
  cout.width(width) ; cout << ""                   ;
  cout.width(width) ; cout << ""                   ;
  cout.width(width) ; cout << ""                   ;
  cout.width(width) ; cout << totalLumi            ;
  cout.width(width) ; cout << weight[7]            ;
  cout.width(width) ; cout << correction[7]        ;
  cout.width(width) ; cout << trackingErrorStat[7] ;
  cout.width(width) ; cout << trackingErrorSyst[7] ;
  cout << endl <<endl ;
  Float_t totalError = sqrt(trackingError*trackingError+correction[7]*correction[7]) ;
  cout << "Total tracking efficiency error = " << (totalError) << "%" << endl ;
}

void DRecoDsToEllNuPlots::MakePIDTree()
{
  TFile* PIDFile = new TFile(TString("../AWG90/aidanrc/PID.root"), "RECREATE") ;
  TTree* PIDTree = new TTree("PIDTree", "PID tree") ;
  
  const Int_t NMuSelector = 26 ;
  TString muSelectorName[NMuSelector] ;
  Int_t muSelectorValue[NMuSelector] ;
  muSelectorName[0]  = "MuMinimumIonizing"      ;
  muSelectorName[1]  = "MuVeryLoose"            ;
  muSelectorName[2]  = "MuLoose"                ;
  muSelectorName[3]  = "MuTight"                ;
  muSelectorName[4]  = "MuVeryTight"            ;
  muSelectorName[5]  = "MuNNVeryLoose"          ;
  muSelectorName[6]  = "MuNNLoose"              ;
  muSelectorName[7]  = "MuNNTight"              ;
  muSelectorName[8]  = "MuNNVeryTight"          ;
  muSelectorName[9]  = "MuNNVeryLooseFakeRate"  ;
  muSelectorName[10] = "MuNNLooseFakeRate"      ;
  muSelectorName[11] = "MuNNTightFakeRate"      ;
  muSelectorName[12] = "MuNNVeryTightFakeRate"  ;
  muSelectorName[13] = "MuLikeVeryLoose"        ;
  muSelectorName[14] = "MuLikeLoose"            ;
  muSelectorName[15] = "MuLikeTight"            ;
  muSelectorName[16] = "MuBDTVeryLoose"         ;
  muSelectorName[17] = "MuBDTLoose"             ;
  muSelectorName[18] = "MuBDTTight"             ;
  muSelectorName[19] = "MuBDTVeryTight"         ;
  muSelectorName[20] = "MuBDTVeryLooseFakeRate" ;
  muSelectorName[21] = "MuBDTLooseFakeRate"     ;
  muSelectorName[22] = "MuBDTTightFakeRate"     ;
  muSelectorName[23] = "MuBDTVeryTightFakeRate" ;
  muSelectorName[24] = "MuBDTLoPLoose"          ;
  muSelectorName[25] = "MuBDTLoPTight"          ;
  for(Int_t i=0 ; i<NMuSelector ; i++)
  {
    chain->SetBranchAddress(muSelectorName[i], &muSelectorValue[i]) ;
    PIDTree->Branch(muSelectorName[i], &muSelectorValue[i], TString(muSelectorName[i]+"/I")) ;
  }
  const Int_t NESelector = 12 ;
  TString eSelectorName[NESelector] ;
  Int_t eSelectorValue[NESelector] ;
  eSelectorName[0]  = "ENoCal"         ;
  eSelectorName[1]  = "EVeryLoose"     ;
  eSelectorName[2]  = "ELoose"         ;
  eSelectorName[3]  = "ETight"         ;
  eSelectorName[4]  = "EVeryTight"     ;
  eSelectorName[5]  = "PidLHElectrons" ;
  eSelectorName[6]  = "EKMSuperLoose"  ;
  eSelectorName[7]  = "EKMVeryLoose"   ;
  eSelectorName[8]  = "EKMLoose"       ;
  eSelectorName[9]  = "EKMTight"       ;
  eSelectorName[10] = "EKMVeryTight"   ;
  eSelectorName[11] = "EKMSuperTight"  ;
  for(Int_t i=0 ; i<NESelector ; i++)
  {
    chain->SetBranchAddress(eSelectorName[i], &eSelectorValue[i]) ;
    PIDTree->Branch(eSelectorName[i], &eSelectorValue[i], TString(eSelectorName[i]+"/I")) ;
  }
  const Int_t NPiSelector = 17 ;
  TString piSelectorName[NPiSelector] ;
  Int_t piSelectorValue[NPiSelector] ;
  piSelectorName[0]  = "PiRoyLoose"     ;
  piSelectorName[1]  = "PiRoyNotAKaon"  ;
  piSelectorName[2]  = "PiLHVeryLoose"  ;
  piSelectorName[3]  = "PiLHLoose"      ;
  piSelectorName[4]  = "PiLHTight"      ;
  piSelectorName[5]  = "PiLHVeryTight"  ;
  piSelectorName[6]  = "PiGLHVeryLoose" ;
  piSelectorName[7]  = "PiGLHLoose"     ;
  piSelectorName[8]  = "PiGLHTight"     ;
  piSelectorName[9]  = "PiGLHVeryTight" ;
  piSelectorName[10] = "PiKMSuperLoose" ;
  piSelectorName[11] = "PiKMVeryLoose"  ;
  piSelectorName[12] = "PiKMLoose"      ;
  piSelectorName[13] = "PiKMTight"      ;
  piSelectorName[14] = "PiKMVeryTight"  ;
  piSelectorName[15] = "PiKMSuperTight" ;
  piSelectorName[16] = "PiSelector"     ;
  for(Int_t i=0 ; i<NPiSelector ; i++)
  {
    chain->SetBranchAddress(piSelectorName[i], &piSelectorValue[i]) ;
    PIDTree->Branch(piSelectorName[i], &piSelectorValue[i], TString(piSelectorName[i]+"/I")) ;
  }
  Int_t    charge ;
  PIDTree->Branch("charge"     , &charge     , "charge/I"     ) ;
  PIDTree->Branch("run"        , &run        , "run/I"        ) ;
  PIDTree->Branch("SignalMode" , &SignalMode , "SignalMode/I" ) ;

  Double_t p3Lab    ;
  Double_t costhLab ;
  Double_t phiLab   ;
  Double_t pxLab    ;
  Double_t pyLab    ;
  Double_t pzLab    ;
  PIDTree->Branch("p3Lab"    , &p3Lab    , "p3Lab/D"    ) ;
  PIDTree->Branch("costhLab" , &costhLab , "costhLab/D" ) ;
  PIDTree->Branch("phiLab"   , &phiLab   , "phiLab/D"   ) ;
  PIDTree->Branch("pxLab"    , &pxLab    , "pxLab/D"    ) ;
  PIDTree->Branch("pyLab"    , &pyLab    , "pyLab/D"    ) ;
  PIDTree->Branch("pzLab"    , &pzLab    , "pzLab/D"    ) ;

  Float_t MupxLab ;
  Float_t MupyLab ;
  Float_t MupzLab ;
  Float_t  EpxLab ;
  Float_t  EpyLab ; 
  Float_t  EpzLab ;
  Float_t PipxLab ;
  Float_t PipyLab ;
  Float_t PipzLab ;
  chain->SetBranchAddress("MupxLabRaw"  , &MupxLab ) ;
  chain->SetBranchAddress("MupyLabRaw"  , &MupyLab ) ;
  chain->SetBranchAddress("MupzLabRaw"  , &MupzLab ) ;
  chain->SetBranchAddress("EpxLabRaw"   , &EpxLab  ) ;
  chain->SetBranchAddress("EpyLabRaw"   , &EpyLab  ) ;
  chain->SetBranchAddress("EpzLabRaw"   , &EpzLab  ) ;
  chain->SetBranchAddress("PipxLabRaw"  , &PipxLab ) ;
  chain->SetBranchAddress("PipyLabRaw"  , &PipyLab ) ;
  chain->SetBranchAddress("PipzLabRaw"  , &PipzLab ) ;
  
  for(Int_t i=0 ; i<chain->GetEntries() ; i++)
  {
    chain->GetEntry(i) ;
    if((SignalMode==0||SignalMode==2) && abs(EllLund)!=13 ) continue ;
    if((SignalMode==1||SignalMode==3) && abs(EllLund)!=11 ) continue ;
    if((SignalMode==4)                && abs(EllLund)!=211) continue ;
    charge = ((SPMode%2)==0) ? -1 : 1 ;      
    // Cast as double for the PIDTables
    switch(SPMode)
    {
    case 29: // DsToTauNuTauToPiNu   +
    case 30: // DsToTauNuTauToPiNu   -
      pxLab = PipxLab ;
      pyLab = PipyLab ;
      pzLab = PipzLab ;
      break ;
    case  5: // DsToMuNu             +
    case  6: // DsToMuNu             -
    case 13: // DsToTauNuTauToMuNuNu +
    case 14: // DsToTauNuTauToMuNuNu -
      pxLab = MupxLab ;
      pyLab = MupyLab ;
      pzLab = MupzLab ;
      break ;
    case 15: // DsToTauNuTauToENuNu  +
    case 16: // DsToTauNuTauToENuNu  -
    case 11: // DsToENu              +
    case 12: // DsToENu              -
      pxLab = EpxLab ;
      pyLab = EpyLab ;
      pzLab = EpzLab ;
    }
    TVector3 pLab(pxLab, pyLab, pzLab) ;
    p3Lab    = pLab.Mag()      ;
    costhLab = pLab.CosTheta() ;
    phiLab   = pLab.Phi()      ;
    switch(SPMode)
    {
    case 29: // DsToTauNuTauToPiNu   +
    case 30: // DsToTauNuTauToPiNu   -
    case 13: // DsToTauNuTauToMuNuNu +
    case 14: // DsToTauNuTauToMuNuNu -
    case 15: // DsToTauNuTauToENuNu  +
    case 16: // DsToTauNuTauToENuNu  -
    case  5: // DsToMuNu             +
    case  6: // DsToMuNu             -
      if(EExtraNeutral>1.0) continue ;
    case 11: // DsToENu              +
    case 12: // DsToENu              -
      PIDTree->Fill() ;
      break ;
    default: break ;
    }
  }
  PIDTree->Write() ;
  PIDFile->Close() ;
}

void DRecoDsToEllNuPlots::PIDStudy()
{
  TString prefix = "/afs/slac.stanford.edu/g/babar/physicstools/pid/tables/" ;
  TFile* fPID = new TFile("../AWG90/aidanrc/PIDHistograms.root","READ") ;
  TCanvas c1("c1", "c1", 100, 100, 500, 500) ;
  Int_t selectorPerSignalMode[NSignalMode] = { 2 , 1 , 2 , 1 , 1 } ;
  ofstream fBAD ;
  ofstream fBADCommands ;
  fBADCommands.open("../AWG90/aidanrc/BAD/note2170/PIDCommands.tex") ;

  // Runs and luminosity
  // luminosity gives lumi for each on/off running period
  // runLuminosity gives the lumi for combined on/off running periods for a given run
  const Int_t NRunsPID = 8 ;
  Float_t luminosity[17] ;
  luminosity[0]  =  20.595 ; luminosity[1]  =  2.621 ;
  luminosity[2]  =  62.066 ; luminosity[3]  =  7.030 ;
  luminosity[4]  =  32.668 ; luminosity[5]  =  2.495 ;
  luminosity[6]  = 100.776 ; luminosity[7]  = 10.228 ;
  luminosity[8]  = 133.854 ; luminosity[9]  = 14.546 ;
  luminosity[10] =  79.030 ; luminosity[11] =  7.886 ;
  luminosity[12] =  12.954 ; luminosity[13] =  1.511 ;
  luminosity[14] =  28.494 ; luminosity[15] =  2.657 ;
  luminosity[16] = 0.0 ;
  for(Int_t i=0 ; i<16 ; i++){ luminosity[16] += luminosity[i] ; } //cout << luminosity[16] << " " ; } cout << endl ;
  Float_t runLuminosity[NRunsPID] ;
  for(Int_t i=0 ; i<NRunsPID-1 ; i++){ runLuminosity[i] = luminosity[2*i] + luminosity[2*i+1] ; }
  runLuminosity[NRunsPID-2] += luminosity[14] ;
  runLuminosity[NRunsPID-2] += luminosity[15] ;
  runLuminosity[NRunsPID-1]  = luminosity[16] ;
  TString runCut[NRunsPID-1] ; TString runName[NRunsPID-1] ;
  runCut[0] = "(run==0  || run==1 )" ; runName[0] = "Run1" ;
  runCut[1] = "(run==2  || run==3 )" ; runName[1] = "Run2" ;
  runCut[2] = "(run==4  || run==5 )" ; runName[2] = "Run3" ;
  runCut[3] = "(run==6  || run==7 )" ; runName[3] = "Run4" ;
  runCut[4] = "(run==8  || run==9 )" ; runName[4] = "Run5" ;
  runCut[5] = "(run==10 || run==11)" ; runName[5] = "Run6" ;
  runCut[6] = "(run==12 || run==13 || run==14 || run==15)" ;  runName[6] = "Run7" ;

  // Selectors
  const Int_t NMuSelector = 26 ;
  TString muSelectorFilename[NMuSelector] ;
  TString muSelectorName[NMuSelector] ;
  muSelectorName[0]  = "MuMinimumIonizing"      ; muSelectorFilename[0]  = "Micro.MinimumIonizing" ;
  muSelectorName[1]  = "MuVeryLoose"            ; muSelectorFilename[1]  = "VeryLoose" ;
  muSelectorName[2]  = "MuLoose"                ; muSelectorFilename[2]  = "Loose" ;
  muSelectorName[3]  = "MuTight"                ; muSelectorFilename[3]  = "Tight" ;
  muSelectorName[4]  = "MuVeryTight"            ; muSelectorFilename[4]  = "VeryTight" ;
  muSelectorName[5]  = "MuNNVeryLoose"          ; muSelectorFilename[5]  = "NN.VeryLoose" ;
  muSelectorName[6]  = "MuNNLoose"              ; muSelectorFilename[6]  = "NN.Loose" ;
  muSelectorName[7]  = "MuNNTight"              ; muSelectorFilename[7]  = "NN.Tight" ;
  muSelectorName[8]  = "MuNNVeryTight"          ; muSelectorFilename[8]  = "NN.VeryTight" ;
  muSelectorName[9]  = "MuNNVeryLooseFakeRate"  ; muSelectorFilename[9]  = "NN.VeryLooseFakeRate" ;
  muSelectorName[10] = "MuNNLooseFakeRate"      ; muSelectorFilename[10] = "NN.LooseFakeRate" ;
  muSelectorName[11] = "MuNNTightFakeRate"      ; muSelectorFilename[11] = "NN.TightFakeRate" ;
  muSelectorName[12] = "MuNNVeryTightFakeRate"  ; muSelectorFilename[12] = "NN.VeryTightFakeRate" ;
  muSelectorName[13] = "MuLikeVeryLoose"        ; muSelectorFilename[13] = "Micro.VeryLoose" ;
  muSelectorName[14] = "MuLikeLoose"            ; muSelectorFilename[14] = "Micro.Loose" ;
  muSelectorName[15] = "MuLikeTight"            ; muSelectorFilename[15] = "Micro.Tight" ;
  muSelectorName[16] = "MuBDTVeryLoose"         ; muSelectorFilename[16] = "BDT.VeryLoose" ;
  muSelectorName[17] = "MuBDTLoose"             ; muSelectorFilename[17] = "BDT.Loose" ;
  muSelectorName[18] = "MuBDTTight"             ; muSelectorFilename[18] = "BDT.Tight" ;
  muSelectorName[19] = "MuBDTVeryTight"         ; muSelectorFilename[19] = "BDT.VeryTight" ;
  muSelectorName[20] = "MuBDTVeryLooseFakeRate" ; muSelectorFilename[20] = "BDT.VeryLooseFakeRate" ;
  muSelectorName[21] = "MuBDTLooseFakeRate"     ; muSelectorFilename[21] = "BDT.LooseFakeRate" ;
  muSelectorName[22] = "MuBDTTightFakeRate"     ; muSelectorFilename[22] = "BDT.TightFakeRate" ;
  muSelectorName[23] = "MuBDTVeryTightFakeRate" ; muSelectorFilename[23] = "BDT.VeryTightFakeRate" ;
  muSelectorName[24] = "MuBDTLoPLoose"          ; muSelectorFilename[24] = "BDTLoP.Loose" ;
  muSelectorName[25] = "MuBDTLoPTight"          ; muSelectorFilename[25] = "BDTLoP.Tight" ;

  const Int_t NESelector = 12 ;
  TString eSelectorFilename[NESelector] ;
  TString eSelectorName[NESelector] ;
  eSelectorName[0]  = "ENoCal"         ; eSelectorFilename[0]  = "Micro.NoCal"     ;
  eSelectorName[1]  = "EVeryLoose"     ; eSelectorFilename[1]  = "Micro.VeryLoose" ;
  eSelectorName[2]  = "ELoose"         ; eSelectorFilename[2]  = "Micro.Loose"     ;
  eSelectorName[3]  = "ETight"         ; eSelectorFilename[3]  = "Micro.Tight"     ;
  eSelectorName[4]  = "EVeryTight"     ; eSelectorFilename[4]  = "Micro.VeryTight" ;
  eSelectorName[5]  = "PidLHElectrons" ; eSelectorFilename[5]  = ""                ;
  eSelectorName[6]  = "EKMSuperLoose"  ; eSelectorFilename[6]  = "KM.SuperLoose"   ;
  eSelectorName[7]  = "EKMVeryLoose"   ; eSelectorFilename[7]  = "KM.VeryLoose"    ;
  eSelectorName[8]  = "EKMLoose"       ; eSelectorFilename[8]  = "KM.Loose"        ;
  eSelectorName[9]  = "EKMTight"       ; eSelectorFilename[9]  = "KM.Tight"        ;
  eSelectorName[10] = "EKMVeryTight"   ; eSelectorFilename[10] = "KM.VeryTight"    ;
  eSelectorName[11] = "EKMSuperTight"  ; eSelectorFilename[11] = "KM.SuperTight"   ;

  const Int_t NPiSelector = 16 ;
  TString piSelectorFilename[NPiSelector] ;
  TString piSelectorName[NPiSelector] ;
  piSelectorName[0]  = "PiRoyLoose"     ; piSelectorFilename[0]  = ""              ;
  piSelectorName[1]  = "PiRoyNotAKaon"  ; piSelectorFilename[1]  = ""              ;
  piSelectorName[2]  = "LHVeryLoose"    ; piSelectorFilename[2]  = "LH.VeryLoose"  ;
  piSelectorName[3]  = "LHLoose"        ; piSelectorFilename[3]  = "LH.Loose"      ;
  piSelectorName[4]  = "LHTight"        ; piSelectorFilename[4]  = "LH.Tight"      ;
  piSelectorName[5]  = "LHVeryTight"    ; piSelectorFilename[5]  = "LH.VeryTight"  ;
  piSelectorName[6]  = "PiGLHVeryLoose" ; piSelectorFilename[6]  = "GLH.VeryLoose" ;
  piSelectorName[7]  = "PiGLHLoose"     ; piSelectorFilename[7]  = "GLH.Loose"     ;
  piSelectorName[8]  = "PiGLHTight"     ; piSelectorFilename[8]  = "GLH.Tight"     ;
  piSelectorName[9]  = "PiGLHVeryTight" ; piSelectorFilename[9]  = "GLH.VeryTight" ;
  piSelectorName[10] = "PiKMSuperLoose" ; piSelectorFilename[10] = "KM.SuperLoose" ;
  piSelectorName[11] = "PiKMVeryLoose"  ; piSelectorFilename[11] = "KM.VeryLoose"  ;
  piSelectorName[12] = "PiKMLoose"      ; piSelectorFilename[12] = "KM.Loose"      ;
  piSelectorName[13] = "PiKMTight"      ; piSelectorFilename[13] = "KM.Tight"      ;
  piSelectorName[14] = "PiKMVeryTight"  ; piSelectorFilename[14] = "KM.VeryTight"  ;
  piSelectorName[15] = "PiKMSuperTight" ; piSelectorFilename[15] = "KM.SuperTight" ;

  const Int_t NSelector = 2 ;
  TString     selectorName[NSignalMode][NSelector] ;
  TString selectorFilename[NSignalMode][NSelector] ;
  TString particleName[NSignalMode] = { "Muon"      , "Electron"  , "Muon"      , "Electron"  , "Pion"     } ;
  selectorName[0][0] = muSelectorName[16]   ; selectorFilename[0][0] = muSelectorFilename[16]   ; 
  selectorName[0][1] = muSelectorName[24]   ; selectorFilename[0][1] = muSelectorFilename[24]   ; 
  selectorName[1][0] =  eSelectorName[6]    ; selectorFilename[1][0] =  eSelectorFilename[6]    ; 
  selectorName[1][1] = ""                   ; selectorFilename[1][1] = ""                       ;
  selectorName[2][0] =   selectorName[0][0] ; selectorFilename[2][0] =   selectorFilename[0][0] ;
  selectorName[2][1] =   selectorName[0][1] ; selectorFilename[2][1] =   selectorFilename[0][1] ;
  selectorName[3][0] =   selectorName[1][0] ; selectorFilename[3][0] =   selectorFilename[1][0] ;
  selectorName[3][1] =   selectorName[1][1] ; selectorFilename[3][1] =   selectorFilename[1][1] ;
  selectorName[4][0] = piSelectorName[10]   ; selectorFilename[4][0] = piSelectorFilename[10]   ;
  selectorName[4][1] = ""                   ; selectorFilename[4][1] = "" ;

  TFile f("../AWG90/aidanrc/PID.root", "READ") ;
  TTree* tree = (TTree*)f.Get("PIDTree") ;
  TString rel = "r24c" ;

  const Int_t NCharge = 2 ;
  TString chargeName[NCharge] = { "+"    , "-"     } ;
  TString chargeWord[NCharge] = { "plus" , "minus" } ;
  TString  chargeCut[NCharge] = { "1"    , "-1"    } ;
  TString EllName[NSignalMode] ; TString EllTitle[NSignalMode] ; TString EllNameLaTeX[NSignalMode] ;
  EllName[0] = "muon"     ; EllTitle[0] = "#mu" ; EllNameLaTeX[0] = "\\mu" ;
  EllName[1] = "electron" ; EllTitle[1] = "e"   ; EllNameLaTeX[1] = "e"    ;
  EllName[2] = "muon"     ; EllTitle[2] = "#mu" ; EllNameLaTeX[2] = "\\mu" ;
  EllName[3] = "electron" ; EllTitle[3] = "e"   ; EllNameLaTeX[3] = "e"    ;
  EllName[4] = "pion"     ; EllTitle[4] = "#pi" ; EllNameLaTeX[4] = "\\pi" ;
  TString PIDSampleName[4] ;
  PIDSampleName[0] = "mu.mumug2" ;  
  PIDSampleName[1] = "e.emcrad"  ;
  PIDSampleName[2] = "mu.mumug2" ;
  PIDSampleName[3] = "e.emcrad"  ;

  // Arrays of yields
  Float_t               nEll[NSignalMode][NSelector][NCharge][NRunsPID-1] ;
  Float_t            nEllAll[NSignalMode][NSelector][NRunsPID-1] ;
  Float_t             weight[NSignalMode][NSelector][NCharge][NRunsPID-1] ;
  Float_t                  n[NSignalMode][NSelector][NCharge][NRunsPID-1] ;
  Float_t             nRatio[NSignalMode][NSelector][NCharge][NRunsPID-1] ;
  Float_t       overallRatio[NSignalMode][NSelector][NCharge][NRunsPID-1] ;
  Float_t                nCT[NSignalMode][NSelector][NCharge][NRunsPID-1] ;
  Float_t          nSelector[NSignalMode][NSelector][NCharge][NRunsPID-1] ;
  Float_t   statisticalError[NSignalMode][NSelector][NCharge][NRunsPID-1] ;
  Float_t environmentalError[NSignalMode][NSelector][NCharge][NRunsPID-1] ;
  Float_t      weightedError[NSignalMode][NSelector][2][NCharge][NRunsPID-1] ;
  Float_t         totalError[NSignalMode][NSelector][3] ; // Stat , Env, Combined

  // Histograms
  TH1I* hCT[NSignalMode][NSelector] ;
  TH1I* hSelector[NSignalMode][NSelector] ;
  // Momentum spectrum histograms
  TH1D*           hMomentumRaw[NSignalMode][NSelector][NCharge][NRunsPID-1] ;
  TH1D*           hMomentumPID[NSignalMode][NSelector][NCharge][NRunsPID-1] ;
  TH1D*        hMomentumWeight[NSignalMode][NSelector][NCharge][NRunsPID-1] ;
  TString    histogramNamePRaw[NSignalMode][NSelector][NCharge][NRunsPID] ;
  TString    histogramNamePPID[NSignalMode][NSelector][NCharge][NRunsPID] ;
  TString histogramNamePWeight[NSignalMode][NSelector][NCharge][NRunsPID] ;
  TString    histogramNamePAll[NSignalMode][NSelector][NCharge][NRunsPID] ;
    
  TLegend legend = MakeLegend(0.3, 0.9, 0.9, 0.8) ;
  for(Int_t SignalMode=0 ; SignalMode<4 ; SignalMode++)
  {
    for(Int_t selector=0 ; selector<selectorPerSignalMode[SignalMode] ; selector++)
    {
      Float_t nCTAll       = 0 ;
      Float_t nSelectorAll = 0 ;
      totalError[SignalMode][selector][0] = 0.0 ;
      totalError[SignalMode][selector][1] = 0.0 ;
      totalError[SignalMode][selector][2] = 0.0 ;
      Float_t wTemp     = 0 ;
      Float_t wStatTemp = 0 ;
      Float_t wEnvTemp  = 0 ;
      for(Int_t charge=0 ; charge<2 ; charge++) // Charge
      {
    for(Int_t run=0 ; run<NRunsPID-1 ; run++)
    {
      cout << runName[run] << " " << SignalModeName[SignalMode] << chargeName[charge] << " " << selectorName[SignalMode][selector] << endl ;
      // Define cut for the projection
      TString cut = TString(runCut[run] + " && charge==" + chargeCut[charge] + " && SignalMode==" + (long)SignalMode) ;
      
      // Get base histogram
      TString histogramName   = TString(particleName[SignalMode] + "_" + chargeWord[charge] + "_" + runName[run] + "_" + PIDSampleName[SignalMode] + "_" + selectorName[SignalMode][selector] + "_") ;
      cout << histogramName << endl ;
      TString h2DPIDEffName   = TString(histogramName + "PID2DEff"  ) ;
      TString h1DPIDEffName   = TString(histogramName + "PID1DEff"  ) ;
      TString h2DRatioErrName = TString(histogramName + "Ratio2DErr") ;
      TString h2DRatioEffName = TString(histogramName + "Ratio2DEff") ;
      TString h1DRatioEffName = TString(histogramName + "Ratio1DEff") ;
      
      TH2D* h2DPIDEff   = (TH2D*) fPID->Get(h2DPIDEffName  ) ;
      TH1D* h1DPIDEff   = (TH1D*) fPID->Get(h1DPIDEffName  ) ;
      TH2D* h2DRatioErr = (TH2D*) fPID->Get(h2DRatioErrName) ;
      TH2D* h2DRatioEff = (TH2D*) fPID->Get(h2DRatioEffName) ;
      TH1D* h1DRatioEff = (TH1D*) fPID->Get(h1DRatioEffName) ;
      
      TH2D* h2DRatioPIDEff = (TH2D*) h2DRatioEff->Clone("h2DRatioPIDEff") ;
      for(Int_t binX=1 ; binX<h2DRatioEff->GetNbinsX() ; binX++)
      {
        for(Int_t binY=1 ; binY<h2DRatioEff->GetNbinsY() ; binY++)
        {
          h2DRatioPIDEff->SetBinContent(binX,binY,h2DRatioEff->GetBinContent(binX,binY)*h2DPIDEff->GetBinContent(binX,binY)) ;
          //cout << binX << " " << binY << " " << h2DPIDEff->GetBinContent(binX,binY) << " " << h2DRatioEff->GetBinContent(binX,binY) << " " << h2DRatioPIDEff->GetBinContent(binX,binY) << endl ;
        }
      }

      TH2D* hBase       = (TH2D*) h2DPIDEff->Clone("hBase") ;
      hBase->Reset() ;
      tree->Project("hBase", "p3Lab:acos(costhLab)*180.0/TMath::Pi()", cut) ;
      hBase->Sumw2() ;
      Float_t unweightedN = hBase->Integral() ;
      cout << "Events passing cuts (unweighted MC): " << unweightedN << endl ;
      n[SignalMode][selector][charge][run] = unweightedN ;
      
      TH2D* hCuts = (TH2D*) hBase->Clone("hCuts") ;
      Int_t nBins = hCuts->GetNbinsX()*hCuts->GetNbinsY() ;
      for(Int_t bin=1 ; bin<nBins+1 ; bin++)
      {
        hCuts->SetBinError(bin, 0.0) ;
        h2DRatioEff->SetBinError(bin, h2DRatioErr->GetBinContent(bin)) ;
      }
      hCuts->Multiply(h2DRatioEff) ;
      nRatio[SignalMode][selector][charge][run] = hCuts->Integral() ;
      cout << "Events passing cuts (weighted to data): " << nRatio[SignalMode][selector][charge][run] << endl ;
      overallRatio[SignalMode][selector][charge][run] = nRatio[SignalMode][selector][charge][run]/n[SignalMode][selector][charge][run] ;
      
      // Get statistical error
      Double_t sumErrSq = 0 ;
      for(Int_t bin=1 ; bin<nBins+1 ; bin++)
      {
        if(hCuts->GetBinError(bin)<10) sumErrSq += hCuts->GetBinError(bin)*hCuts->GetBinError(bin) ;
        if(hCuts->GetBinError(bin)>10) cout << "  SUSPICIOUS BIN: " << charge << " " << run << " " << bin << " " << hCuts->GetBinError(bin) << endl ;
        //if(charge==1&&run==2&&selector==0&&SignalMode==0) cout << "  " << charge << " " << run << " " << bin << " " << hCuts->GetBinError(bin) << endl ; // Run 3 Mu- MuBDTVeryLoose
      }
      
      statisticalError[SignalMode][selector][charge][run] = (hCuts->Integral()!=0) ? sqrt(sumErrSq)/hCuts->Integral() : 0 ;
      cout << "sqrt(Sum error^2)   = " << sqrt(sumErrSq) << endl ;
      cout << "sqrt(Sum error^2)/N = " << statisticalError[SignalMode][selector][charge][run] << endl ;      
      
      // Momentum spectrum histograms
      histogramNamePRaw[SignalMode][selector][charge][run]    = TString(TString("PID_MomentumSpectrum_Raw_")   +selectorName[SignalMode][selector]+chargeWord[charge]+"_Run"+(long)(run+1)+"_"+SignalModeName[SignalMode]) ;
      histogramNamePPID[SignalMode][selector][charge][run]    = TString(TString("PID_MomentumSpectrum_PID_")   +selectorName[SignalMode][selector]+chargeWord[charge]+"_Run"+(long)(run+1)+"_"+SignalModeName[SignalMode]) ;
      histogramNamePWeight[SignalMode][selector][charge][run] = TString(TString("PID_MomentumSpectrum_Weight_")+selectorName[SignalMode][selector]+chargeWord[charge]+"_Run"+(long)(run+1)+"_"+SignalModeName[SignalMode]) ;
      histogramNamePAll[SignalMode][selector][charge][run]    = TString(TString("PID_MomentumSpectrum_All_")   +selectorName[SignalMode][selector]+chargeWord[charge]+"_Run"+(long)(run+1)+"_"+SignalModeName[SignalMode]) ;

      h1DRatioEff->GetXaxis()->SetRange(0,h1DRatioEff->GetNbinsX()-1) ;
      h1DPIDEff->GetXaxis()->SetRange(0,h1DPIDEff->GetNbinsX()-1) ;
      
      hMomentumRaw[SignalMode][selector][charge][run] = (TH1D*) h1DRatioEff->Clone(histogramNamePRaw[SignalMode][selector][charge][run]) ;
      hMomentumRaw[SignalMode][selector][charge][run]->UseCurrentStyle() ;
      hMomentumRaw[SignalMode][selector][charge][run]->GetXaxis()->SetTitle("p^{lab} / GeVc^{ -1}") ;
      hMomentumRaw[SignalMode][selector][charge][run]->GetYaxis()->SetTitle("entries / 0.1 GeVc^{ -1}") ;
      hMomentumRaw[SignalMode][selector][charge][run]->GetYaxis()->SetLabelSize(0.04) ;
      hMomentumRaw[SignalMode][selector][charge][run]->SetLineWidth(2) ;
      hMomentumRaw[SignalMode][selector][charge][run]->SetLineColor(kRed) ;
      hMomentumRaw[SignalMode][selector][charge][run]->SetMarkerColor(kRed) ;
      hMomentumRaw[SignalMode][selector][charge][run]->SetMarkerStyle(20) ;
      ThesisTH1F(hMomentumRaw[SignalMode][selector][charge][run]) ;
      
      hMomentumPID[SignalMode][selector][charge][run] = (TH1D*) h1DRatioEff->Clone(histogramNamePPID[SignalMode][selector][charge][run]) ;
      hMomentumPID[SignalMode][selector][charge][run]->UseCurrentStyle() ;
      hMomentumPID[SignalMode][selector][charge][run]->GetXaxis()->SetTitle("p^{lab} / GeVc^{ -1}") ;
      hMomentumPID[SignalMode][selector][charge][run]->GetYaxis()->SetTitle("entries / 0.1 GeVc^{ -1}") ;
      hMomentumPID[SignalMode][selector][charge][run]->GetYaxis()->SetLabelSize(0.04) ;
      hMomentumPID[SignalMode][selector][charge][run]->SetLineWidth(2) ;
      hMomentumPID[SignalMode][selector][charge][run]->SetLineColor(kBlue) ;
      hMomentumPID[SignalMode][selector][charge][run]->SetLineStyle(kDashed) ;
      hMomentumPID[SignalMode][selector][charge][run]->SetMarkerColor(kBlue) ;
      hMomentumPID[SignalMode][selector][charge][run]->SetMarkerStyle(21) ;
      ThesisTH1F(hMomentumRaw[SignalMode][selector][charge][run]) ;
      
      hMomentumWeight[SignalMode][selector][charge][run] = (TH1D*) h1DRatioEff->Clone(histogramNamePWeight[SignalMode][selector][charge][run]) ;
      hMomentumWeight[SignalMode][selector][charge][run]->UseCurrentStyle() ;
      hMomentumWeight[SignalMode][selector][charge][run]->GetXaxis()->SetTitle("p^{lab} / GeVc^{ -1}") ;
      hMomentumWeight[SignalMode][selector][charge][run]->GetYaxis()->SetTitle("entries / 0.1 GeVc^{ -1}") ;
      hMomentumWeight[SignalMode][selector][charge][run]->GetYaxis()->SetLabelSize(0.04) ;
      hMomentumWeight[SignalMode][selector][charge][run]->SetLineWidth(2) ;
      hMomentumWeight[SignalMode][selector][charge][run]->SetLineColor(kGreen) ;
      hMomentumWeight[SignalMode][selector][charge][run]->SetLineStyle(kDotted) ;
      hMomentumWeight[SignalMode][selector][charge][run]->SetMarkerColor(kGreen) ;
      hMomentumWeight[SignalMode][selector][charge][run]->SetMarkerStyle(22) ;
      ThesisTH1F(hMomentumRaw[SignalMode][selector][charge][run]) ;
      
      if(SignalMode==0 && selector==0 && charge==0 && run==0)
      {
        legend.AddEntry(   hMomentumRaw[SignalMode][selector][charge][run], "Raw number of candidates"     , "pl") ;
        legend.AddEntry(   hMomentumPID[SignalMode][selector][charge][run], "Number passing selector"      , "pl") ;
        legend.AddEntry(hMomentumWeight[SignalMode][selector][charge][run], "Number weighted using tables" , "pl") ;
      }
      
      tree->Draw(TString(TString("p3Lab")+">>"+histogramNamePRaw[SignalMode][selector][charge][run]), TString(TString("p3Lab>0")+"&&"+cut),"goff") ;
      tree->Draw(TString(TString("p3Lab")+">>"+histogramNamePPID[SignalMode][selector][charge][run]), TString(TString("p3Lab>0")+"&&"+cut+"&&"+selectorName[SignalMode][selector]+"==1"), "goff") ;
      tree->Draw(TString(TString("p3Lab")+">>"+histogramNamePWeight[SignalMode][selector][charge][run]),TString(TString("p3Lab>0")+"&&"+cut), "goff") ;
      hMomentumWeight[SignalMode][selector][charge][run]->Multiply(h1DPIDEff) ;
      
      cout << histogramNamePRaw[SignalMode][selector][charge][run] << endl ;
      hMomentumRaw[SignalMode][selector][charge][run]->Draw()    ; c1.Print(TString(TString("../AWG90/aidanrc/PIDPlots/")+histogramNamePRaw[SignalMode][selector][charge][run]+".eps"))    ; c1.Clear() ;
      hMomentumPID[SignalMode][selector][charge][run]->Draw()    ; c1.Print(TString(TString("../AWG90/aidanrc/PIDPlots/")+histogramNamePPID[SignalMode][selector][charge][run]+".eps"))    ; c1.Clear() ;
      hMomentumWeight[SignalMode][selector][charge][run]->Draw() ; c1.Print(TString(TString("../AWG90/aidanrc/PIDPlots/")+histogramNamePWeight[SignalMode][selector][charge][run]+".eps")) ; c1.Clear() ;
      hMomentumRaw[SignalMode][selector][charge][run]->Draw("p")          ;
      hMomentumPID[SignalMode][selector][charge][run]->Draw("sames:P")    ;
      hMomentumWeight[SignalMode][selector][charge][run]->Draw("sames:p") ;
      legend.Draw() ;
      c1.Print(TString(TString("../AWG90/aidanrc/PIDPlots/")+histogramNamePAll[SignalMode][selector][charge][run]+".eps")) ;
      c1.Print(TString(TString("~/public_html/thesis/PIDPlots/")+histogramNamePAll[SignalMode][selector][charge][run]+".png")) ;
      c1.Print(TString(TString("~/public_html/thesis/PIDPlots/")+histogramNamePAll[SignalMode][selector][charge][run]+".eps")) ;
      if(charge==0 && run==4) c1.Print(TString(TString("../AWG90/aidanrc/BAD/note2170/plots/Numerator/")+histogramNamePAll[SignalMode][selector][charge][run]+".eps")) ;
      c1.Clear() ;
      h1DPIDEff->Draw() ; c1.Print(TString(TString("../AWG90/aidanrc/PIDPlots/")+histogramNamePAll[SignalMode][selector][charge][run]+"Eff.eps")) ; c1.Clear() ;
      
      cout << "     Raw: " <<    hMomentumRaw[SignalMode][selector][charge][run]->GetSumOfWeights() << endl ;
      cout << "     PID: " <<    hMomentumPID[SignalMode][selector][charge][run]->GetSumOfWeights() << endl ;
      cout << "Weighted: " << hMomentumWeight[SignalMode][selector][charge][run]->GetSumOfWeights() << endl ;

      // Get environmental uncertainty
      hBase->Reset() ;
      tree->Project("hBase", "p3Lab:acos(costhLab)*180.0/TMath::Pi()", cut) ;
      hBase->Multiply(h2DPIDEff) ;
      nCT[SignalMode][selector][charge][run] = hBase->Integral() ;
      hBase->Reset() ;
      cout << "Number of ChargedTracks candidates passing cuts through PID table: " << nCT[SignalMode][selector][charge][run] << endl ;
      cut = TString(cut + " && " + selectorName[SignalMode][selector] + "==1") ;
      tree->Project("hBase", "p3Lab:acos(costhLab)*180.0/TMath::Pi()", cut) ;
      nSelector[SignalMode][selector][charge][run] = hBase->Integral() ;
      cout << "Number of ChargedTracks candidates passing cuts through " << selectorName[SignalMode][selector] << " list: " << nSelector[SignalMode][selector][charge][run] << endl ;
      environmentalError[SignalMode][selector][charge][run] = (nSelector[SignalMode][selector][charge][run]!=0) ? 0.5*fabs(nCT[SignalMode][selector][charge][run]-nSelector[SignalMode][selector][charge][run])/nSelector[SignalMode][selector][charge][run] : 0 ;
      cout << "Environment error = " << environmentalError[SignalMode][selector][charge][run] << endl ;
      
      nCTAll       +=       nCT[SignalMode][selector][charge][run] ;
      nSelectorAll += nSelector[SignalMode][selector][charge][run] ;

      nEll[SignalMode][selector][charge][run] = tree->Draw(selectorName[SignalMode][selector], TString(TString("run==") + (long)run + " && charge==" + chargeCut[charge] + " && SignalMode==" + (long)SignalMode), "goff") ;
      nEllAll[SignalMode][selector][run] = tree->Draw(selectorName[SignalMode][selector], TString(TString("run==") + (long)run + " && SignalMode==" + (long)SignalMode), "goff") ;
      
      weight[SignalMode][selector][charge][run] = (nEll[SignalMode][selector][charge][run]/nEllAll[SignalMode][selector][run])*(runLuminosity[run]/runLuminosity[NRunsPID-1]) ;
      cout << "weight: " << weight[SignalMode][selector][charge][run] << endl ;
      weightedError[SignalMode][selector][0][charge][run] = weight[SignalMode][selector][charge][run]*statisticalError[SignalMode][selector][charge][run] ;
      weightedError[SignalMode][selector][1][charge][run] = weight[SignalMode][selector][charge][run]*environmentalError[SignalMode][selector][charge][run] ;
      totalError[SignalMode][selector][0] += weightedError[SignalMode][selector][0][charge][run]*weightedError[SignalMode][selector][0][charge][run] ;
      totalError[SignalMode][selector][1] += weightedError[SignalMode][selector][1][charge][run] ;
      
      cout << endl << endl ;
      delete hBase ;
      delete h2DRatioErr ;
      delete h2DRatioEff;
      delete h1DPIDEff ;
      delete hCuts ;
    }
      }
      totalError[SignalMode][selector][0] = sqrt(totalError[SignalMode][selector][0]) ;
      
      Int_t width = 16 ;
      cout << "                    Statistical    Environmental     weight (LxC)     wStatistical   wEnvironmental       Correction      Uncertainty" << endl <<endl ;
      for(Int_t charge=0 ; charge<2 ; charge++)
      {
    for(Int_t run=0 ; run<NRunsPID-1 ; run++)
    {
      cout << "Run " << (run+1) << " (" << EllName[SignalMode] << chargeName[charge] << ")  " ;
      Float_t errTemp = sqrt(statisticalError[SignalMode][selector][charge][run]*statisticalError[SignalMode][selector][charge][run]+environmentalError[SignalMode][selector][charge][run]*environmentalError[SignalMode][selector][charge][run]) ;
      cout.width(width) ; cout <<   statisticalError[SignalMode][selector][charge][run]    ; cout << " " ;
      cout.width(width) ; cout << environmentalError[SignalMode][selector][charge][run]    ; cout << " " ;
      cout.width(width) ; cout <<             weight[SignalMode][selector][charge][run]    ; cout << " " ;
      cout.width(width) ; cout <<      weightedError[SignalMode][selector][0][charge][run] ; cout << " " ;
      cout.width(width) ; cout <<      weightedError[SignalMode][selector][1][charge][run] ; cout << " " ;
      cout.width(width) ; cout <<       overallRatio[SignalMode][selector][charge][run]    ; cout << " " ;
      cout.width(width) ; cout <<            errTemp                                       ; cout << " " ;
      cout << endl ;
    }
    cout << endl ;
    cout << "Total          " ;
    wTemp     = 0 ;
    wStatTemp = 0 ;
    wEnvTemp  = 0 ;
    for(Int_t run=0 ; run<NRunsPID-1 ; run++)
    {
      wTemp     += weight[SignalMode][selector][charge][run] ;
      wStatTemp += weightedError[SignalMode][selector][0][charge][run]*weightedError[SignalMode][selector][0][charge][run] ;
      wEnvTemp  += weightedError[SignalMode][selector][1][charge][run] ;
    }
    wStatTemp = sqrt(wStatTemp) ;
    cout << "                 " ;
    cout << "                 " ;
    cout.width(width) ; cout << wTemp     ; cout << " " ;
    cout.width(width) ; cout << wStatTemp ; cout << " " ;
    cout.width(width) ; cout << wEnvTemp  ; cout << " " ;
    cout << endl << endl ;
      }
      cout << "nCT:       " << nCTAll       << endl ;
      cout << "nSelector: " << nSelectorAll << endl ;
      Float_t environmentalErrorAll = (nCTAll-nSelectorAll)/nSelectorAll ;
      cout << "Environmental error: " << environmentalErrorAll << endl ;
      cout << "Grand total          " ;
      wTemp     = 0 ;
      wStatTemp = 0 ;
      wEnvTemp  = 0 ;
      for(Int_t charge=0 ; charge<2 ; charge++)
      {
    for(Int_t run=0 ; run<NRunsPID-1 ; run++)
    {
      wTemp     += weight[SignalMode][selector][charge][run] ;
      wStatTemp += weightedError[SignalMode][selector][0][charge][run]*weightedError[SignalMode][selector][0][charge][run] ;
      wEnvTemp  += weightedError[SignalMode][selector][1][charge][run] ;
    }
      }
      wEnvTemp = environmentalErrorAll ;
      wStatTemp = sqrt(wStatTemp) ;
      cout << "              " ;
      cout << "              " ;
      cout.width(width) ; cout << wTemp     ; cout << " " ;
      cout.width(width) ; cout << wStatTemp ; cout << " " ;
      cout.width(width) ; cout << wEnvTemp  ; cout << " " ;
      cout << endl << endl ;
      
      totalError[SignalMode][selector][2] = sqrt(totalError[SignalMode][selector][0]*totalError[SignalMode][selector][0]+totalError[SignalMode][selector][1]*totalError[SignalMode][selector][1]) ;
      cout << "==============================" << endl ;
      cout <<         " Total uncertainty: " ;
      cout.width(8) ; cout << (totalError[SignalMode][selector][2]*100) << "%" << endl ;
      cout << "==============================" << endl ;
      cout << "Total " << EllName[SignalMode] << "+: " << tree->Draw("charge", TString(TString("charge==1  && SignalMode==") + (long)SignalMode), "goff") << endl ;
      cout << "Total " << EllName[SignalMode] << "-: " << tree->Draw("charge", TString(TString("charge==-1 && SignalMode==") + (long)SignalMode), "goff") << endl ;
      cout << "Total " << EllName[SignalMode] << " : " << tree->Draw("charge", TString(              TString("SignalMode==") + (long)SignalMode), "goff") << endl ;
      cout << endl << endl << endl << endl << endl ;

      TString histogramName ;
      histogramName = TString(TString("hCT")      +SignalModeName[SignalMode]+selectorName[SignalMode][selector])  ;
      hCT[SignalMode][selector]       = new TH1I(histogramName, "", 14, 0, 7) ;
      histogramName = TString(TString("hSelector")+SignalModeName[SignalMode]+selectorName[SignalMode][selector])  ;
      hSelector[SignalMode][selector] = new TH1I(histogramName, "", 14, 0, 7) ;
      hCT[SignalMode][selector]->SetLineColor(kRed) ;
      hCT[SignalMode][selector]->SetLineWidth(2) ;
      hCT[SignalMode][selector]->GetXaxis()->SetLabelSize(0.025) ;
      hSelector[SignalMode][selector]->SetLineColor(kBlue) ;    
      hSelector[SignalMode][selector]->SetLineWidth(2) ;
      hSelector[SignalMode][selector]->GetXaxis()->SetLabelSize(0.025) ;

      for(Int_t bin=1 ; bin<hCT[SignalMode][selector]->GetNbinsX()+1 ; bin++)
      {
    Int_t run = (bin-1)/2 ;
    Int_t charge = (bin-1)%2 ;
    TString binLabel = TString(runName[run] + EllTitle[SignalMode] + "^{" + chargeName[charge] + "}") ;
    hCT[SignalMode][selector]->SetBinContent(                bin , nCT[SignalMode][selector][charge][run]) ;
    hCT[SignalMode][selector]->GetXaxis()->SetBinLabel(      bin , binLabel) ;
    hSelector[SignalMode][selector]->SetBinContent(          bin , nSelector[SignalMode][selector][charge][run]) ;
        hSelector[SignalMode][selector]->GetXaxis()->SetBinLabel(bin , binLabel) ;
      }
      if(hCT[SignalMode][selector]->GetMaximum()>hSelector[SignalMode][selector]->GetMaximum())
      {
    hCT[SignalMode][selector]->Draw("l") ;
    hSelector[SignalMode][selector]->Draw("l:sames") ;
      }
      else
      {
    hSelector[SignalMode][selector]->Draw("l") ;
    hCT[SignalMode][selector]->Draw("l:sames") ;
      }
      c1.Print(TString(SignalModeName[SignalMode] + "_" + selectorName[SignalMode][selector] + ".eps")) ;
    }
    
    // Make tables for the BAD    
    fBAD.open(TString(TString("../AWG90/aidanrc/BAD/note2170/Numerator")+SignalModeName[SignalMode]+"PID.tex")) ;
    for(Int_t selector=0 ; selector<selectorPerSignalMode[SignalMode] ; selector++)
    {
      fBAD << "\\begin{table}" << endl ;
      fBAD << "  \\begin{center}" << endl ;
      fBAD << "    \\begin{tabular}{cccc}" << endl ;
      fBAD << "      \\hline" << endl ;
      fBAD << "      & Statistical & Environmental & Weight  \\\\" << endl ;
      fBAD << "      & uncertainty & uncertainty   &         \\\\" << endl ;
      fBAD << "      \\hline" << endl ;
      for(Int_t row=0 ; row<2*(NRunsPID-1) ; row++)
      {
    Int_t run    = row/2 ;
    Int_t charge = row%2 ;
    fBAD << "      " << runName[run] << " $" << EllNameLaTeX[SignalMode] << "^" << chargeName[charge] << "$ & $" ;
    fBAD << FloatToString(100*statisticalError[SignalMode][selector][charge][run],3) ;
    fBAD << "\\%$ & $ " ;
    fBAD << FloatToString(100*environmentalError[SignalMode][selector][charge][run],3) ;
    fBAD << "\\%$ & $ " ;
    fBAD << FloatToString(weight[SignalMode][selector][charge][run],3) ;
    fBAD << "$ \\\\" << endl ; 
      }
      fBAD << "      \\hline" << endl ;
      fBAD << "    \\end{tabular}" << endl ;
      fBAD << "  \\end{center}" << endl ;
      fBAD << "  \\label{tab:Numerator_PID_" << SignalModeName[SignalMode] << "_" << selectorName[SignalMode][selector] << "}" << endl ;
      fBAD << "  \\caption{Particle identification systematic uncertainties for the \\texttt{" << selectorName[SignalMode][selector] << "} selector for the $" << SignalModeNameLaTeX[SignalMode] << "$ mode.  The weight corresponds to the total expected fraction of events in data for the given running period and charge.}" << endl ;
      fBAD << "\\end{table}" << endl ;

      fBAD << "\\begin{figure}" << endl ;
      fBAD << "  \\begin{center}" << endl ;      
      fBAD << "    \\includegraphics[width=0.4\\textwidth]{plots/Numerator/" <<  histogramNamePAll[SignalMode][selector][0][4] << ".eps}" << endl ;
      fBAD << "  \\label{fig:Numerator_PID_" ;
      fBAD << SignalModeName[SignalMode] << "_" ;
      fBAD << selectorName[SignalMode][selector] << "}" << endl ;
      fBAD << "  \\caption[\\texttt{" << selectorName[SignalMode][selector] << "} selector efficiency for Run 5.]" ;
      fBAD << "{The selector efficiencies as a function of momentum (in the lab frame) for positively charged " ;
      fBAD << EllName[SignalMode] ;
      fBAD << "s for run 5 using the \\texttt{" ;
      fBAD << selectorName[SignalMode][selector] ;
      fBAD << "} selector.  The plot shows the number of events as a function of momentum for all signal MC reconstructions passing selection criteria (red circles), for all signal MC reconstructions passing selection criteria and passing the \\texttt{" ;
      fBAD << selectorName[SignalMode][selector] ;
      fBAD << "} selector (blue squares), and for all signal reconstructions passing the selection criteria and weighted according to the particle identification tables provided by the Particle Identification group (green triangles).}" << endl ;
      fBAD << "  \\end{center}" << endl ;
      fBAD << "\\end{figure}" << endl ;
      if(selectorPerSignalMode[SignalMode]==1) fBADCommands << "\\newcommand{\\" << SignalModeName[SignalMode] << "PID}[0]{" << FloatToString(100*totalError[SignalMode][selector][2],2) << "} % Units of percent" << endl ;
    }
    if(selectorPerSignalMode[SignalMode]>1)
    {
      Float_t nTotal[2] ;
      Float_t grandTotalError = 0 ;
      for(Int_t selector=0 ; selector<selectorPerSignalMode[SignalMode] ; selector++)
      {
        nTotal[selector] = tree->Draw(selectorName[SignalMode][selector], TString(selectorName[SignalMode][selector] + "==1") , "goff") ;
        cout << "Total number of " << selectorName[SignalMode][selector] << "candidates: " << nTotal[selector] <<endl ;
      }
      Float_t w[2] = { nTotal[0]/(nTotal[0]+nTotal[1]) , nTotal[1]/(nTotal[0]+nTotal[1]) } ;
      cout << "Weights: " << w[0] << " " << w[1] << endl ;
      grandTotalError = w[0]*totalError[SignalMode][0][2] + w[1]*totalError[SignalMode][1][2] ;
      cout << "Grand total for both selectors: " << grandTotalError*100 << "%" << endl ;
      fBADCommands << "\\newcommand{\\" << SignalModeName[SignalMode] << "PID}[0]{" << FloatToString(grandTotalError*100,2) << "} % Units of percent" << endl ;

      fBAD << "\\begin{table}" << endl ;
      fBAD << "  \\begin{center}" << endl ;
      fBAD << "    \\begin{tabular}{ccc}" << endl ;
      fBAD << "      \\hline" << endl ;
      fBAD << "      & Uncertainty & Weight\\\\" << endl ;
      fBAD << "      \\hline" << endl ;
      for(Int_t selector=0 ; selector<selectorPerSignalMode[SignalMode] ; selector++)
      {
    fBAD << "      \\texttt{" << selectorName[SignalMode][selector] << "} & $" ;
    fBAD << FloatToString(100*totalError[SignalMode][selector][2],3) ;
    fBAD << "\\%$ & $ " ;
    fBAD << FloatToString(w[selector],3) ;
    fBAD << "$ \\\\" << endl ;
      }
      fBAD << "      \\hline" << endl ;
      fBAD << "      Total: & $" ;
      fBAD << FloatToString(100*grandTotalError) ;
      fBAD << "\\%$ & \\\\" << endl ;
      fBAD << "      \\hline" << endl ;
      fBAD << "    \\end{tabular}" << endl ;
      fBAD << "  \\end{center}" << endl ;
      fBAD << "  \\label{tab:Numerator_PIDCombined_" << SignalModeName[SignalMode] << "}" << endl ;
      fBAD << "  \\caption{Combined particle identification systematic uncertainties for the $" << SignalModeNameLaTeX[SignalMode] << "$ mode.  The weight corresponds to the total fraction of events in signal MC for the given selector.}" << endl ;
      fBAD << "\\end{table}" << endl ;
    }
    fBAD.close() ;
  }
  fBADCommands.close() ;
}

void DRecoDsToEllNuPlots::PlotConfidenceInterval()
{
  SignalMode = 1 ;
  EExtraCut  = 0 ;
  TCanvas* c1 = new TCanvas("c1", "c1", 0, 0, 500, 300) ;
  Float_t sigma = 0.5 ;  
  Float_t mean  = 1.28*sigma ;
  Float_t lower = -1 ;
  Float_t upper =  2 ;
  Int_t nBins = 10000 ;
  TH1F* gaussEmpty = new TH1F("gaussEmpty", "gaussEmpty", nBins, lower, upper) ;
  TH1F* gaussFull  = new TH1F("gaussFull" , "gaussFull" , nBins, lower, upper) ;
  TH1F* lines      = new TH1F("lines"     , "lines"     , nBins, lower, upper) ;
  for(Int_t i=0 ; i<nBins ; i++)
  {
    Float_t floatI = i ;
    Float_t floatNBins = (Float_t)nBins ;
    Float_t x = lower + (upper-lower)*(floatI)/floatNBins ;
    gaussEmpty->SetBinContent(i+1, exp(-(x-mean)*(x-mean)/(2*sigma*sigma))) ;
    if(x<0) gaussFull->SetBinContent(i+1, exp(-(x-mean)*(x-mean)/(2*sigma*sigma))) ;
    //if(x>-tolerance && x<tolerance)              lines->Fill(x, 100) ;
    //if(x>mean-tolerance && x<mean+tolerance)     lines->Fill(x, 100) ;
    //if(x>2*mean-tolerance && x<2*mean+tolerance) lines->Fill(x, 100) ;
  }
  lines->Fill(0.0,    100.0) ;
  lines->Fill(mean,   100.0) ;
  lines->Fill(mean*2, 100.0) ;
  gaussEmpty->SetLineWidth(2) ;
  gaussEmpty->GetXaxis()->SetTitle("#theta_{obs} ( #times 10^{ - 5 } )") ;
  gaussEmpty->GetYaxis()->SetLabelColor(kWhite) ;
  gaussEmpty->GetYaxis()->SetTitle("PDF") ;
  gaussFull->SetFillColor(kBlue) ;
  lines->SetLineColor(kRed) ;
  lines->SetLineStyle(kDashed) ;
  lines->SetLineWidth(1) ;
  
  TLegend legend = MakeLegend(0.62, 0.9, 0.92, 0.75) ;
  legend.AddEntry(gaussEmpty, "Probability density"       , "l") ;
  legend.AddEntry(lines     , "90% confidence interval" , "l") ;
  //legend.AddEntry(gaussFull , "5% confidence interval", "f") ;
  
  gaussEmpty->Draw("L") ;
  gaussFull->Draw("same") ;
  lines->Draw("same") ;
  gaussEmpty->Draw("L:same") ;
  gaussEmpty->Draw("axis:same") ;
  legend.Draw() ;
  PrintCanvas(c1, TString("ConfidenceInterval")) ;
}

void DRecoDsToEllNuPlots::CutUncertainties()
{
  ofstream fBADCommands ;
  fBADCommands.open("../AWG90/aidanrc/BAD/note2170/CutUncertaintiesCommands.tex") ;
  Float_t e ;
  Int_t MuBDTVeryLoose ;
  Int_t MuBDTLoPLoose  ;
  Int_t EKMSuperLoose  ;
  Int_t PiKMSuperLoose ;
  chain->SetBranchAddress("EExtraNeutral"  , &e) ;
  chain->SetBranchAddress("MuBDTVeryLoose" , &MuBDTVeryLoose) ;
  chain->SetBranchAddress("MuBDTLoPLoose"  , &MuBDTLoPLoose ) ;
  chain->SetBranchAddress("EKMSuperLoose"  , &EKMSuperLoose ) ;
  chain->SetBranchAddress("PiKMSuperLoose" , &PiKMSuperLoose) ;

  TString cuts[4] ;
  cuts[0] = "SignalMode==0&&(SPMode==5 ||SPMode==6 )&&(MuBDTVeryLoose==1||MuBDTLoPLoose==1)" ;
  cuts[1] = "SignalMode==1&&(SPMode==11||SPMode==12)&&EKMSuperLoose==1" ;
  cuts[2] = "SignalMode==2&&(SPMode==13||SPMode==14)&&(MuBDTVeryLoose==1||MuBDTLoPLoose==1)" ;
  cuts[3] = "SignalMode==3&&(SPMode==15||SPMode==16)&&EKMSuperLoose==1" ;

  Float_t eCuts[4][4] ; // SignalMode , cut value
  Float_t mCuts[4][4] ; // SignalMode , cut value
  for(Int_t S=0 ; S<4 ; S++)
  {
    eCuts[S][0] = chain->Draw("mm2",cuts[S],"goff") ;
    mCuts[S][0] = chain->Draw("mm2",cuts[S],"goff") ;
    eCuts[S][1] = eCuts[S][0] ;
    eCuts[S][2] = eCuts[S][0] ;
    eCuts[S][3] = eCuts[S][0] ;
    mCuts[S][1] = mCuts[S][0] ;
    mCuts[S][2] = mCuts[S][0] ;
    mCuts[S][3] = mCuts[S][0] ;
    switch(S)
    {
    case 0:
    case 1:
      eCuts[S][1] = chain->Draw("mm2",TString(cuts[S]+"&&EExtraNeutral<1.1"),"goff") ;
      eCuts[S][2] = chain->Draw("mm2",TString(cuts[S]+"&&EExtraNeutral<1.0"),"goff") ;
      eCuts[S][3] = chain->Draw("mm2",TString(cuts[S]+"&&EExtraNeutral<0.9"),"goff") ;
      break ;
    case 2:
      mCuts[S][1] = chain->Draw("mm2",TString(cuts[S]+"&&mm2>0.45"),"goff") ;
      mCuts[S][2] = chain->Draw("mm2",TString(cuts[S]+"&&mm2>0.50"),"goff") ;
      mCuts[S][3] = chain->Draw("mm2",TString(cuts[S]+"&&mm2>0.55"),"goff") ;
    case 3:
      eCuts[S][1] = chain->Draw("mm2",TString(cuts[S]+"&&EExtraNeutral<5.5"),"goff") ;
      eCuts[S][2] = chain->Draw("mm2",TString(cuts[S]+"&&EExtraNeutral<5.0"),"goff") ;
      eCuts[S][3] = chain->Draw("mm2",TString(cuts[S]+"&&EExtraNeutral<4.5"),"goff") ;
      break ;
    default:
      break ;
    }
  }

  Float_t eStatisticalError[NSignalMode] ;
  Float_t eVariationalError[NSignalMode] ;
  Float_t       eTotalError[NSignalMode] ;
  Float_t mStatisticalError[NSignalMode] ;
  Float_t mVariationalError[NSignalMode] ;
  Float_t       mTotalError[NSignalMode] ;
  Float_t        totalError[NSignalMode] ;
  ofstream fBAD ;
  for(Int_t S=0 ; S<4 ; S++)
  {
    eStatisticalError[S] = sqrt(eCuts[S][2]/eCuts[S][0]*(1-eCuts[S][2]/eCuts[S][0])/eCuts[S][0]) ;
    mStatisticalError[S] = sqrt(mCuts[S][2]/mCuts[S][0]*(1-mCuts[S][2]/mCuts[S][0])/mCuts[S][0]) ;
    
    eVariationalError[S] = (fabs(eCuts[S][1]-eCuts[S][2])>fabs(eCuts[S][3]-eCuts[S][2])) ? fabs(eCuts[S][1]-eCuts[S][2])/eCuts[S][2] : fabs(eCuts[S][3]-eCuts[S][2])/eCuts[S][2] ;
    mVariationalError[S] = (fabs(mCuts[S][1]-mCuts[S][2])>fabs(mCuts[S][3]-mCuts[S][2])) ? fabs(mCuts[S][1]-mCuts[S][2])/mCuts[S][2] : fabs(mCuts[S][3]-mCuts[S][2])/mCuts[S][2] ;
    
    eTotalError[S] = sqrt(pow(eVariationalError[S],2)+pow(eStatisticalError[S],2)) ;
    mTotalError[S] = sqrt(pow(mVariationalError[S],2)+pow(mStatisticalError[S],2)) ;
    totalError[S]  = sqrt(pow(eTotalError[S],2)+pow(mTotalError[S],2)) ;
    
    Float_t grandTotal = totalError[S] ;
    
    cout << "=========================================" << endl ;
    cout << "  " << SignalModeName[S]          << endl ;
    cout << "=========================================" << endl ;    
    cout << "Cut uncertainties" << endl ;    
    cout << "  EExtra" << endl ;
    cout << "  Variational: " << eVariationalError[S]*100 << "%"  << endl ;
    cout << "  Statistical: " << eStatisticalError[S]*100 << "%"  << endl ;
    cout << "  Total:       " <<       eTotalError[S]*100 << "%"  << endl << endl ;

    cout << "  mm2" << endl ;
    cout << "  Variational: " << mVariationalError[S]*100 << "%"  << endl ;
    cout << "  Statistical: " << mStatisticalError[S]*100 << "%"  << endl ;
    cout << "  Total:       " <<       mTotalError[S]*100 << "%"  << endl << endl ;
    
    cout << "Grand total: " << totalError[S]*100 << "%" << endl ;
    
    fBAD.open(TString(TString("../AWG90/aidanrc/BAD/note2170/Numerator")+SignalModeName[S]+"CutUncertainties.tex")) ;
    fBAD << "\\begin{table}[!hb]" << endl ;
    fBAD << "  \\begin{center}" << endl ;
    fBAD << "    \\begin{tabular}{cccccc}" << endl ;
    fBAD << "      \\hline" << endl ;
    fBAD << "      Variable & Central   & Cut range & Variational & Statistical & Cut       \\\\" << endl ;
    fBAD << "               & cut value &           & uncertainty & uncertainty & uncertainty \\\\" << endl ;
    fBAD << "      \\hline" << endl ;
    switch(S)
    {
    case 0:
    case 1:
      fBAD << "      $\\eextra$ & $1 \\gev$ & $0.9-1.1\\mathrm{GeV}$ & " 
       << FloatToString(eVariationalError[S]*100) << "\\% & " 
       << FloatToString(eStatisticalError[S]*100) << "\\% & " 
       << FloatToString(      eTotalError[S]*100) << "\\% \\\\" << endl ;
      break ;
    case 2:
      fBAD << "      $\\mmtwo$ & $0.5 \\gev^2c^{-4}$ & $0.45-0.55\\mathrm{GeV}$ & " 
       << FloatToString(mVariationalError[S]*100) << "\\% & " 
       << FloatToString(mStatisticalError[S]*100) << "\\% & " 
       << FloatToString(      mTotalError[S]*100) << "\\% \\\\" << endl ;
    case 3:
      fBAD << "      $\\eextra$ & $5\\gev$ & $4.5-5.5\\mathrm{GeV}$ & " 
       << FloatToString(eVariationalError[S]*100) << "\\% & " 
       << FloatToString(eStatisticalError[S]*100) << "\\% & " 
       << FloatToString(      eTotalError[S]*100) << "\\% \\\\" << endl ;
      break ;

    default:
      break ;
    }
    fBAD << "    &&&& Total & $" << FloatToString(totalError[S]*100) << "\\%$ \\\\" << endl ;
    fBAD << "    \\end{tabular}" << endl ;
    fBAD << "  \\end{center}" << endl ;
    fBAD << "  \\caption[Cut uncertainties for the numerator for the $" << SignalModeNameLaTeX[S] << "$ mode]{Cut uncertainties for the numerator for the $" <<  SignalModeNameLaTeX[S] << "$ mode.}" << endl ;
    fBAD << "  \\label{tab:Numerator_Cuts_" << SignalModeName[S] << "}" << endl ;
    fBAD << "\\end{table}" << endl ;
    fBAD.close() ;  
    fBADCommands << "\\newcommand{\\" << SignalModeName[S] << "CutUncertainties}[0]{" << FloatToString(grandTotal*100,2) << "} % Units of percent" << endl ;
  }
  fBADCommands.close() ;
}

void DRecoDsToEllNuPlots::Mm2TrueParticles()
{
  TCanvas cMm2("cMm2","cMm2",100,100,500,500) ;
  TH1F*   hMm2TrueParticle[2][5] ;
  TString       cutStrings[2] ;
  cutStrings[0] = "SignalMode==0&&(MuBDTVeryLoose==1||MuBDTLoPLoose==1)&&EExtraNeutral<1&&SPMode<5" ;
  cutStrings[1] = "SignalMode==1&&EKMSuperLoose==1&&SPMode<5" ;
  TString trueParticleName[5] = { TString("Electron") , TString("Muon") , TString("Pion") , TString("Kaon") , TString("Proton") } ;
  Int_t   trueParticleLund[5] = { 11 , 13 , 211 , 321 , 2212 } ;
  Int_t trueParticleColors[5] = { 1 , 2 , 3 , 0 ,6 } ;
  for(Int_t trueParticle=0 ; trueParticle<5 ; trueParticle++)
  {
    for(Int_t s=0 ; s<2 ; s++)
    {
      SignalMode = s ;
      TString histogramName = TString(TString("hMm2")+SignalModeName[SignalMode]+trueParticleName[trueParticle]) ;
      hMm2TrueParticle[SignalMode][trueParticle] = new TH1F(histogramName,histogramName,40,-0.3,3.7) ;
      hMm2TrueParticle[SignalMode][trueParticle]->GetXaxis()->SetTitle(TString(TString("mm2 (")+trueParticleName[trueParticle]+")")) ;
      hMm2TrueParticle[SignalMode][trueParticle]->GetYaxis()->SetTitle("entries") ;
      hMm2TrueParticle[SignalMode][trueParticle]->SetFillColor(trueParticleColors[trueParticle]) ;
      TString cut = TString(cutStrings[SignalMode]+"&&abs(EllLund)=="+(long)trueParticleLund[trueParticle]) ;
      cout << s << " " << SignalMode << " " <<  histogramName << " " << SignalModeName[SignalMode] << " " << cut << " " << chain->Draw(TString(TString("mm2>>")+histogramName),cut) << endl ;
      EExtraCut = (SignalMode!=1) ;
      SignalMode = 1 ;
      PrintCanvas(&cMm2,TString(TString("mm2TrueType")+trueParticleName[trueParticle])) ;
    }
  }
  return ;
}

void DRecoDsToEllNuPlots::mm2DsToENu()
{
  TFile* fThesis = new TFile("../AWG90/aidanrc/thesis.root","READ") ;
  TTree* tThesis = (TTree*) fThesis->Get("thesis") ;
  TCanvas canvas("canvas","canvas",100,100,1000,2000) ;
  canvas.Divide(2,4) ;
  TH1F* hMm2[2][2] ; // Data/MC , EExtraCut
  TH1F* hResiduals[2] ;
  hMm2[0][0] = new TH1F("hMm2MCNoCut","",45,-0.5,4.0) ;
  hMm2[0][0]->GetXaxis()->SetTitle("m_{m}^{2} (GeV^{2})") ;
  hResiduals[0] = (TH1F*) hMm2[0][0]->Clone("hResidualsMC") ;
  hResiduals[1] = (TH1F*) hMm2[0][0]->Clone("hResidualsData") ;
  hResiduals[0]->GetYaxis()->SetTitle("residuals") ;
  hResiduals[1]->GetYaxis()->SetTitle("residuals") ;
  hResiduals[0]->SetMarkerSize(0.2) ;
  hResiduals[1]->SetMarkerSize(0.2) ;
  hMm2[0][0]->GetYaxis()->SetTitle("entries per 0.1 GeV^{2}") ;
  hMm2[1][0] = (TH1F*) hMm2[0][0]->Clone("hMm2DataNoCut") ;
  hMm2[0][1] = (TH1F*) hMm2[0][0]->Clone("hMm2MCCut") ;
  hMm2[1][1] = (TH1F*) hMm2[0][0]->Clone("hMm2DataCut") ;

  hMm2[0][0]->SetLineColor(kBlue) ; hMm2[0][0]->SetLineWidth(1) ;
  hMm2[0][1]->SetLineColor(kBlue) ; hMm2[0][1]->SetLineWidth(2) ;
  hMm2[1][0]->SetLineColor(kRed)  ; hMm2[1][0]->SetLineWidth(1) ;
  hMm2[1][1]->SetLineColor(kRed)  ; hMm2[1][1]->SetLineWidth(2) ;

  cout << tThesis->Draw("mm2>>hMm2MCNoCut"  ,"weight*(SignalMode==1 && SPMode<5)"              ,"goff") << endl ;
  cout << tThesis->Draw("mm2>>hMm2MCCut"    ,"weight*(SignalMode==1 && SPMode<5 && eextra<1)"  ,"goff") << endl ;
  cout << tThesis->Draw("mm2>>hMm2DataNoCut","weight*(SignalMode==1 && SPMode==10)"            ,"goff") << endl ;
  cout << tThesis->Draw("mm2>>hMm2DataCut"  ,"weight*(SignalMode==1 && SPMode==10 && eextra<1)","goff") << endl ;

  hMm2[0][0]->Scale(hMm2[1][0]->GetSumOfWeights()/hMm2[0][0]->GetSumOfWeights()) ;
  hMm2[0][1]->Scale(hMm2[1][1]->GetSumOfWeights()/hMm2[0][1]->GetSumOfWeights()) ;

  for(Int_t bin=1 ; bin<hMm2[0][0]->GetNbinsX()+1 ; bin++)
  {
    Float_t error = sqrt(hMm2[0][0]->GetBinContent(bin)+hMm2[1][0]->GetBinContent(bin)) ;
    Float_t difference = hMm2[0][0]->GetBinContent(bin)-hMm2[1][0]->GetBinContent(bin) ;
    Float_t residual = (error==0) ? 0 : difference/error ;
    hResiduals[0]->SetBinContent(bin,residual) ;
    error = sqrt(hMm2[0][1]->GetBinContent(bin)+hMm2[1][1]->GetBinContent(bin)) ;
    difference = hMm2[0][1]->GetBinContent(bin)-hMm2[1][1]->GetBinContent(bin) ;
    residual = (error==0) ? 0 : difference/error ;
    hResiduals[1]->SetBinContent(bin,residual) ;
    cout << bin << " " << hResiduals[0]->GetBinContent(bin) << " " << hResiduals[1]->GetBinContent(bin) << " - " << hMm2[0][1]->GetBinContent(bin) << " " <<  hMm2[1][1]->GetBinContent(bin) << endl ;
  }

  TLegend legend = MakeLegend(0.2,0.9,0.4,0.6) ;
  legend.AddEntry(hMm2[0][0],"MC, without cut"  ,"l") ;
  legend.AddEntry(hMm2[0][1],"MC, with cut"     ,"l") ;
  legend.AddEntry(hMm2[1][0],"Data, without cut","l") ;
  legend.AddEntry(hMm2[1][1],"Data, with cut"   ,"l") ;

  canvas.cd(1) ; hMm2[0][0]->Draw() ; legend.Draw() ;
  canvas.cd(2) ; hMm2[0][1]->Draw() ;
  canvas.cd(3) ; hMm2[1][0]->Draw() ;
  canvas.cd(4) ; hMm2[1][1]->Draw() ;
  canvas.cd(5) ; hMm2[1][0]->Draw() ; hMm2[0][0]->Draw("sames") ; hMm2[1][0]->Draw("sames") ;
  canvas.cd(6) ; hMm2[1][1]->Draw() ; hMm2[0][1]->Draw("sames") ; hMm2[1][1]->Draw("sames") ;
  canvas.cd(7) ; hResiduals[0]->Draw("p") ;
  canvas.cd(8) ; hResiduals[1]->Draw("p") ;
  canvas.Print("DsToENu_EExtraCut.eps") ;
  canvas.Print("~/public_html/DsToENu_EExtraCut.png") ;
  canvas.Clear() ;
  hResiduals[1]->Draw("p") ;
  canvas.Print("residuals.eps") ;
  return ;
}

void DRecoDsToEllNuPlots::PlotAll()
{
  Float_t nXTYields[NSignalMode][NNXT] ;
  Float_t nXTWeights[NNXT] ;
  TFile* fNXTWeights = new TFile("../docs/Denominator/NominalFitResults/DenominatorFitResult_Data.root","READ") ;
  TH1F* hNXTWeightsData = (TH1F*) fNXTWeights->Get("HnXTweights") ;
  TH1F* hNXTWeightsMC   = (TH1F*) fNXTWeights->Get("HnXTweightsTrue") ;
  Float_t weightSumData = 0 ;
  Float_t weightSumMC   = 0 ;
  for(Int_t nXT=0 ; nXT<NNXT ; nXT++)
  {
    for(SignalMode=0 ; SignalMode<NSignalMode ; SignalMode++)
    {
      nXTYields[SignalMode][nXT] = 0 ;
    }
    weightSumData += hNXTWeightsData->GetBinContent(nXT+1) ;
    weightSumMC   +=   hNXTWeightsMC->GetBinContent(nXT+1) ;
    nXTWeights[nXT] = hNXTWeightsData->GetBinContent(nXT+1)/hNXTWeightsMC->GetBinContent(nXT+1) ;
    cout << "nXT = " << nXT << " weights: " << hNXTWeightsData->GetBinContent(nXT+1) << " " << hNXTWeightsMC->GetBinContent(nXT+1) << " " << hNXTWeightsData->GetBinContent(nXT+1)/hNXTWeightsMC->GetBinContent(nXT+1) << endl ;
  }
  cout << "Sums of weights: " << weightSumData << " " << weightSumMC << endl ;

  Int_t SignalModeStart = 0 ;
  Int_t SignalModeEnd   = 4 ;
  TCanvas              varCanvas(            "varCanvas", "Variables"        , 0 , 0 , 500 , 500 ) ;
  TCanvas        SPModeVarCanvas(     "SPModeVarCanvas" , "SPMode Variables" , 0 , 0 , 500 , 500 ) ;
  TCanvas     multiplicityCanvas(   "multiplicityCanvas", "Multiplicity"     , 0 , 0 , 500 , 500 ) ;
  TCanvas           PHOTOSCanvas(         "PHOTOSCanvas", "PHOTOS"           , 0 , 0 , 500 , 500 ) ;
  TCanvas  mm2EExtraSlicesCanvas("mm2EExtraSlicesCanvas", "EExtra slices"    , 0 , 0 , 500 , 500 ) ;
  
  // Thesis tree
  TFile* fThesis    = new TFile("../AWG90/aidanrc/thesis.root","RECREATE") ;
  TTree* thesisTree = new TTree("thesis","Thesis tree") ;
  Float_t thesisWeight     ;
  Float_t thesisWeightDsBF ;
  Float_t thesisMm2        ;
  Float_t thesisEExtra     ;
  Float_t thesisEllp3Lab   ;
  Float_t thesisDsMass     ;
  Int_t   thesisComponent  ;
  Int_t   EllMothLund      ;
  Int_t   EllMothDauLen    ;
  Int_t   EllSib1Lund      ;
  Int_t   EllSib2Lund      ;
  Int_t   EllSib3Lund      ;
  Int_t   EllSib4Lund      ;
  Int_t   EllSib5Lund      ;
  Int_t   EllSibNGamma     ;
  thesisTree->Branch("SPMode"       , &SPMode           , "SPMode/I"       ) ;
  thesisTree->Branch("SignalMode"   , &SignalMode       , "SignalMode/I"   ) ;
  thesisTree->Branch("weight"       , &thesisWeight     , "weight/F"       ) ;
  thesisTree->Branch("weightDsBF"   , &thesisWeightDsBF , "weightDsBF/F"   ) ;
  thesisTree->Branch("mm2"          , &thesisMm2        , "mm2/F"          ) ;
  thesisTree->Branch("eextra"       , &thesisEExtra     , "eextra/F"       ) ;
  thesisTree->Branch("ellp3Lab"     , &thesisEllp3Lab   , "ellp3Lab/F"     ) ;
  thesisTree->Branch("run"          , &run              , "run/I"          ) ;
  thesisTree->Branch("DsMass"       , &thesisDsMass     , "DsMass/F"       ) ;
  thesisTree->Branch("EllMothLund"  , &EllMothLund      , "EllMothLund/I"  ) ;
  thesisTree->Branch("EllMothDauLen", &EllMothDauLen    , "EllMothDauLen/I") ;
  thesisTree->Branch("EllSib1Lund"  , &EllSib1Lund      , "EllSib1Lund/I"  ) ;
  thesisTree->Branch("EllSib2Lund"  , &EllSib2Lund      , "EllSib2Lund/I"  ) ;
  thesisTree->Branch("EllSib3Lund"  , &EllSib3Lund      , "EllSib3Lund/I"  ) ;
  thesisTree->Branch("EllSib4Lund"  , &EllSib4Lund      , "EllSib4Lund/I"  ) ;
  thesisTree->Branch("EllSib5Lund"  , &EllSib5Lund      , "EllSib5Lund/I"  ) ;
  thesisTree->Branch("component"    , &thesisComponent  , "component/I"    ) ;
  
  const Int_t NSelectors = 3 ;
  const Int_t NRunsPID   = 7 ;
  TString PIDSelectorName[NSelectors] ;
  PIDSelectorName[0] = "MuBDTVeryLoose" ;
  PIDSelectorName[1] = "MuBDTLoPLoose"  ;
  PIDSelectorName[2] = "EKMSuperLoose"  ;
  TString PIDRunName[NRunsPID] ;
  PIDRunName[0] = "Run1" ;
  PIDRunName[1] = "Run2" ;
  PIDRunName[2] = "Run3" ;
  PIDRunName[3] = "Run4" ;
  PIDRunName[4] = "Run5" ;
  PIDRunName[5] = "Run6" ;
  PIDRunName[6] = "Run7" ;
  TString PIDChargeName[2] = { "plus" , "minus" } ;
  TString PIDParticleName[4] ;
  TString PIDSampleName[4] ;
  PIDParticleName[0] = "Muon"     ; PIDSampleName[0] = "mu.mumug2" ;
  PIDParticleName[1] = "Electron" ; PIDSampleName[1] = "e.emcrad"  ;
  PIDParticleName[2] = "Muon"     ; PIDSampleName[2] = "mu.mumug2" ;
  PIDParticleName[3] = "Electron" ; PIDSampleName[3] = "e.emcrad"  ;
  TH2D* hPID[NSelectors][NRunsPID][2] ; // 2 for charge
  TFile* fPID = new TFile(TString("../AWG90/aidanrc/PIDHistograms.root"),TString("READ")) ;
  for(Int_t PIDParticle=0 ; PIDParticle<4 ; PIDParticle++)
  {
    for(Int_t selector=0 ; selector<NSelectors ; selector++)
    {
      if(PIDParticle%2==0 && selector==2) continue ;
      if(PIDParticle%2==1 && selector!=2) continue ;
      for(Int_t PIDRun=0 ; PIDRun<NRunsPID ; PIDRun++)
      {
    for(Int_t charge=0 ; charge<2 ; charge++)
    {
      TString histogramName = TString(PIDParticleName[PIDParticle]+"_"+PIDChargeName[charge]+"_"+PIDRunName[PIDRun]+"_"+PIDSampleName[PIDParticle]+"_"+PIDSelectorName[selector]+"_Ratio2DEff") ;
      hPID[selector][PIDRun][charge] = (TH2D*)fPID->Get(histogramName) ;
      //cout << histogramName << " " << hPID[selector][PIDRun][charge] << " " << (TH2D*)fPID->Get(histogramName) << endl ;
    }
      }
    }
  }
  
  Int_t   evthassignal            ;
  Int_t   DsPresence              ;
  Int_t   HasDsSt                 ;
  Int_t   HasDs                   ;
  Int_t   HasDsToMuNu             ;
  Int_t   HasDsToENu              ;
  Int_t   HasDsToTauNu            ;
  Int_t   HasDsToTauNuTauToMuNuNu ;
  Int_t   HasDsToTauNuTauToENuNu  ;
  Int_t   HasDsToTauNuTauToPiNu   ;
  Int_t   MuBDTVeryLoose          ;
  Int_t   MuBDTVeryTight          ;
  Int_t   MuBDTLoPLoose           ;
  Int_t   EKMSuperLoose           ;
  Int_t   PiKMSuperLoose          ;
  Int_t   PiKMSuperTight          ;
  Int_t   PiKMLoose               ;
  Int_t   goodeventid             ;
  Int_t   NExtraTracks            ;
  Float_t DsDecayRateWeight       ;
  Int_t   DsDecayRateType         ;
  Float_t MupxLabRaw              ;
  Float_t MupyLabRaw              ;
  Float_t MupzLabRaw              ;
  Float_t EpxLabRaw               ;
  Float_t EpyLabRaw               ;
  Float_t EpzLabRaw               ;
  chain->SetBranchAddress("evthassignal"            , &evthassignal            ) ;
  chain->SetBranchAddress("DsPresence"              , &DsPresence              ) ;
  chain->SetBranchAddress("HasDsSt"                 , &HasDsSt                 ) ;
  chain->SetBranchAddress("HasDs"                   , &HasDs                   ) ;
  chain->SetBranchAddress("HasDsToMuNu"             , &HasDsToMuNu             ) ;
  chain->SetBranchAddress("HasDsToENu"              , &HasDsToENu              ) ;
  chain->SetBranchAddress("HasDsToTauNu"            , &HasDsToTauNu            ) ;
  chain->SetBranchAddress("HasDsToTauNuTauToMuNuNu" , &HasDsToTauNuTauToMuNuNu ) ;
  chain->SetBranchAddress("HasDsToTauNuTauToENuNu"  , &HasDsToTauNuTauToENuNu  ) ;
  chain->SetBranchAddress("HasDsToTauNuTauToPiNu"   , &HasDsToTauNuTauToPiNu   ) ;
  chain->SetBranchAddress("MuBDTVeryLoose"          , &MuBDTVeryLoose          ) ;
  chain->SetBranchAddress("MuBDTVeryTight"          , &MuBDTVeryTight          ) ;
  chain->SetBranchAddress("MuBDTLoPLoose"           , &MuBDTLoPLoose           ) ;
  chain->SetBranchAddress("EKMSuperLoose"           , &EKMSuperLoose           ) ;
  chain->SetBranchAddress("PiKMLoose"               , &PiKMLoose               ) ;
  chain->SetBranchAddress("PiKMSuperLoose"          , &PiKMSuperLoose          ) ;
  chain->SetBranchAddress("PiKMSuperTight"          , &PiKMSuperTight          ) ;
  chain->SetBranchAddress("EllMothLund"             , &EllMothLund             ) ;
  chain->SetBranchAddress("EllMothDauLen"           , &EllMothDauLen           ) ;
  chain->SetBranchAddress("EllSib1Lund"             , &EllSib1Lund             ) ;
  chain->SetBranchAddress("EllSib2Lund"             , &EllSib2Lund             ) ;
  chain->SetBranchAddress("EllSib3Lund"             , &EllSib3Lund             ) ;
  chain->SetBranchAddress("EllSib4Lund"             , &EllSib4Lund             ) ;
  chain->SetBranchAddress("EllSib5Lund"             , &EllSib5Lund             ) ;
  chain->SetBranchAddress("EllSibNGamma"            , &EllSibNGamma            ) ;
  chain->SetBranchAddress("goodeventid"             , &goodeventid             ) ;
  chain->SetBranchAddress("NExtraTracks"            , &NExtraTracks            ) ;
  chain->SetBranchAddress("DsDecayRateWeight"       , &DsDecayRateWeight       ) ;
  chain->SetBranchAddress("DsDecayRateType"         , &DsDecayRateType         ) ;
  chain->SetBranchAddress("DsDecayRateType"         , &DsDecayRateType         ) ;
  chain->SetBranchAddress("MupxLabRaw"              , &MupxLabRaw              ) ;
  chain->SetBranchAddress("MupyLabRaw"              , &MupyLabRaw              ) ;
  chain->SetBranchAddress("MupzLabRaw"              , &MupzLabRaw              ) ;
  chain->SetBranchAddress("EpxLabRaw"               , &EpxLabRaw               ) ;
  chain->SetBranchAddress("EpyLabRaw"               , &EpyLabRaw               ) ;
  chain->SetBranchAddress("EpzLabRaw"               , &EpzLabRaw               ) ;
  
  // Branching fraction calculation ntuples
  TFile* fBF = new TFile("../docs/Denominator/NominalFitResults/NumeratorEfficiencies.root","RECREATE") ;
  Int_t nXT ;
  chain->SetBranchAddress("fragnpartgen", &nXT) ;
  TString nXTString[NNXT+1] ;
  nXTString[0] = "NXT0" ;
  nXTString[1] = "NXT1" ;
  nXTString[2] = "NXT2" ;
  nXTString[3] = "NXT3" ;
  nXTString[4] = "NXT4" ;
  nXTString[5] = "NXT5" ;
  nXTString[6] = "NXT6" ;
  nXTString[7] = "NXTAll" ;
  TH1F* hBFSignal[NSignalMode][NNXT+1] ;
  TH1F* hBFYield[NSignalMode] ;
  for(SignalMode=0 ; SignalMode<NSignalMode ; SignalMode++)
  {
    TString histogramName = TString(SignalModeName[SignalMode]+"Yield") ;
    hBFYield[SignalMode] = new TH1F(histogramName,histogramName,100,-1,4) ;
    hBFYield[SignalMode]->SetFillColor(SignalModeColor[SignalMode]) ;
    for(nXT=0 ; nXT<NNXT+1 ; nXT++)
    {
      TString histogramName = TString(SignalModeName[SignalMode]+nXTString[nXT]) ;
      hBFSignal[SignalMode][nXT] = new TH1F(histogramName,histogramName,100,-1,4) ;
      hBFSignal[SignalMode][nXT]->SetFillColor(SignalModeColor[SignalMode]) ;
    }
  }
  cout << "Set up BF histograms" << endl ;

  TFile* fOut = new TFile("BADPlots.root", "RECREATE") ;
  // Multiplicity
  TString DataOrMCString[3] = { "Data" , "SignalMC"  , "BackgroundMC"  } ;
  TString DataOrMCTitle[3]  = { "Data" , "Signal MC" , "Background MC" } ;
  TH1F* hMultiplicity[2][NSignalMode][3] ;
  for(EExtraCut=0 ; EExtraCut<NEExtraCut ; EExtraCut++)
  {
    for(SignalMode=SignalModeStart ; SignalMode<SignalModeEnd+1 ; SignalMode++)
    {
      for(Int_t DataOrMC=0 ; DataOrMC<3 ; DataOrMC++)
      {
    TString histogramName = TString(TString("hMultiplicity")+DataOrMCString[DataOrMC]+SignalModeName[SignalMode]+EExtraCutString[EExtraCut]) ;
    hMultiplicity[EExtraCut][SignalMode-SignalModeStart][DataOrMC] = new TH1F(histogramName, histogramName, 4, 1, 5) ;
    hMultiplicity[EExtraCut][SignalMode-SignalModeStart][DataOrMC]->GetXaxis()->SetTitle(TString("Reconstructions per event for ")+DataOrMCTitle[DataOrMC]) ;
    hMultiplicity[EExtraCut][SignalMode-SignalModeStart][DataOrMC]->GetYaxis()->SetTitle("Number of events") ;
    hMultiplicity[EExtraCut][SignalMode-SignalModeStart][DataOrMC]->SetFillColor(SignalModeColor[SignalMode]) ;
    hMultiplicity[EExtraCut][SignalMode-SignalModeStart][DataOrMC]->GetYaxis()->SetLabelSize(0.025) ;
    hMultiplicity[EExtraCut][SignalMode-SignalModeStart][DataOrMC]->SetMinimum(0) ;
      }
    }
  }
  cout << "Set up multiplicity plots" << endl ;
  
  // Roofit stuff  
  const Int_t RoofitNType = 4 ;
  TString RoofitTypeName[RoofitNType] ;
  RoofitTypeName[0] = "Var"        ;
  RoofitTypeName[1] = "Signal"     ;
  RoofitTypeName[2] = "TrueSignal" ;
  RoofitTypeName[3] = "Background" ;
  Int_t skipSPMode[NSPModes] ;
  skipSPMode[0]  = 0 ;
  skipSPMode[1]  = 0 ;
  skipSPMode[2]  = 0 ;
  skipSPMode[3]  = 0 ;
  skipSPMode[4]  = 0 ;
  skipSPMode[5]  = 0 ;
  skipSPMode[6]  = 0 ;
  skipSPMode[7]  = 0 ;
  skipSPMode[8]  = 0 ;
  skipSPMode[9]  = 0 ;
  skipSPMode[10] = 1 ;
  TString RoofitPrefix = TString("../AWG90/aidanrc/Roofit/") ;
  
  Float_t     wRoofit ;
  Float_t wLumiRoofit ;
  Float_t   wDsRoofit ;
  Float_t     mRoofit ;
  Float_t     eRoofit ;
  Float_t     dRoofit ;
  Int_t       rRoofit ;
  Int_t       nRoofit ;
  Int_t       SRoofit ;
  Float_t     pRoofit ;
  Int_t       tRoofit ;
  Int_t       gRoofit ;
  Int_t  DsTypeRoofit ;
  Int_t    ModeRoofit ;
  Int_t  SPModeRoofit ;
  Int_t    lundRoofit ;
  TString fileName = TString(RoofitPrefix +"Roofit.root") ;
  TString treeName = TString("tRoofit") ;
  TFile* RoofitF = new TFile(fileName, "RECREATE") ;
  TTree* RoofitT = new TTree(treeName, treeName) ;
  RoofitT->Branch("mm2"        , &mRoofit      , "mm2/F"       ) ;
  RoofitT->Branch("dsmass"     , &dRoofit      , "dsmass/F"    ) ;
  RoofitT->Branch("eextra"     , &eRoofit      , "eextra/F"    ) ;
  RoofitT->Branch("p3lab"      , &pRoofit      , "p3lab/F"     ) ;
  RoofitT->Branch("weight"     , &wRoofit      , "weight/F"    ) ;
  RoofitT->Branch("weightDs"   , &wDsRoofit    , "weightDs/F"  ) ;
  RoofitT->Branch("weightLumi" , &wLumiRoofit  , "weightLumi/F") ;
  RoofitT->Branch("run"        , &rRoofit      , "run/I"       ) ;
  RoofitT->Branch("nxr"        , &nRoofit      , "nxr/I"       ) ;
  RoofitT->Branch("SignalMode" , &SRoofit      , "SignalMode/I") ;
  RoofitT->Branch("type"       , &tRoofit      , "type/I"      ) ;
  RoofitT->Branch("nPhotons"   , &gRoofit      , "nPhotons/I"  ) ;
  RoofitT->Branch("DsType"     , &DsTypeRoofit , "DsType/I"    ) ;
  RoofitT->Branch("modeNumber" , &ModeRoofit   , "ModeNumber/I") ;
  RoofitT->Branch("SPMode"     , &SPModeRoofit , "SPMode/I"    ) ;
  RoofitT->Branch("lund"       , &lundRoofit   , "lund/I"      ) ;
  fOut->cd() ;
  cout << "Set up Roofit plots" << endl ;
  
  // Variables stuff
  const Int_t NVar = 25 ;
  Int_t varStart =  0 ;
  Int_t varEnd   = 10 ;
  TString      varName[NVar] ;
  TString varNameLaTeX[NVar] ;
  Float_t     varLower[NVar] ;
  Float_t     varUpper[NVar] ;
  Int_t       varNBins[NVar] ;
  Int_t        VarType[NVar] ; // 0 for Evt, 1 for Reconstruction
  TString     varXaxis[NVar] ;
  TString     varYaxis[NVar] ;
  Float_t     varValue[NVar] ;
  Bool_t varSignalMode[NVar][NSignalMode] ;
  Int_t n = 0 ;
  varName[n] = "mm2"             ; varLower[n] = -0.3   ; varUpper[n] = 3.7   ; varNBins[n] =  40 ; VarType[n] = 0 ; n++ ;
  varName[n] = "EExtraNeutral"   ; varLower[n] =  0     ; varUpper[n] = 5     ; varNBins[n] =  50 ; VarType[n] = 1 ; n++ ;
  varName[n] = "ProbChi2"        ; varLower[n] =  0     ; varUpper[n] = 1     ; varNBins[n] =  25 ; VarType[n] = 0 ; n++ ;
  varName[n] = "DsToEllNuDsMass" ; varLower[n] =  1.934 ; varUpper[n] = 2.012 ; varNBins[n] =  25 ; VarType[n] = 0 ; n++ ;
  varName[n] = "fragndaus"       ; varLower[n] =  0     ; varUpper[n] = 4     ; varNBins[n] =   4 ; VarType[n] = 0 ; n++ ;
  varName[n] = "Ellp3Lab"        ; varLower[n] =  0     ; varUpper[n] = 6.0   ; varNBins[n] =  25 ; VarType[n] = 0 ; n++ ; //  5
  varName[n] = "deltap3Fit"      ; varLower[n] =  0     ; varUpper[n] = 6.0   ; varNBins[n] =  25 ; VarType[n] = 0 ; n++ ;
  varName[n] = "PMissCosTh"      ; varLower[n] =  0     ; varUpper[n] = 1     ; varNBins[n] =  25 ; VarType[n] = 0 ; n++ ;
  varName[n] = "PMiss"           ; varLower[n] =  0     ; varUpper[n] = 6.0   ; varNBins[n] =  25 ; VarType[n] = 0 ; n++ ;
  varName[n] = "EMiss"           ; varLower[n] =  0     ; varUpper[n] = 6.0   ; varNBins[n] =  25 ; VarType[n] = 0 ; n++ ;
  varName[n] = "PMissPhi"        ; varLower[n] = -3.15  ; varUpper[n] = 3.15  ; varNBins[n] =  25 ; VarType[n] = 0 ; n++ ; // 10
  varName[n] = "R0"              ; varLower[n] =  0.9   ; varUpper[n] = 1     ; varNBins[n] =  25 ; VarType[n] = 1 ; n++ ;
  varName[n] = "R1"              ; varLower[n] =  0     ; varUpper[n] = 0.2   ; varNBins[n] =  25 ; VarType[n] = 1 ; n++ ;
  varName[n] = "R2"              ; varLower[n] =  0     ; varUpper[n] = 1     ; varNBins[n] =  25 ; VarType[n] = 1 ; n++ ;
  varName[n] = "R3"              ; varLower[n] =  0     ; varUpper[n] = 0.5   ; varNBins[n] =  25 ; VarType[n] = 1 ; n++ ;
  varName[n] = "R4"              ; varLower[n] =  0     ; varUpper[n] = 1     ; varNBins[n] =  25 ; VarType[n] = 1 ; n++ ; // 15
  varName[n] = "thetaDsGam"      ; varLower[n] =  0     ; varUpper[n] = 1.5   ; varNBins[n] =  25 ; VarType[n] = 0 ; n++ ;
  varName[n] = "thetaDsK"        ; varLower[n] =  0     ; varUpper[n] = 3.15  ; varNBins[n] =  25 ; VarType[n] = 0 ; n++ ;
  varName[n] = "thetaDsNu"       ; varLower[n] =  0     ; varUpper[n] = 1.5   ; varNBins[n] =  25 ; VarType[n] = 0 ; n++ ;
  varName[n] = "thetaDsTag"      ; varLower[n] =  2.0   ; varUpper[n] = 3.15  ; varNBins[n] =  25 ; VarType[n] = 0 ; n++ ;
  varName[n] = "thetaDsMu"       ; varLower[n] =  0     ; varUpper[n] = 2.00  ; varNBins[n] =  25 ; VarType[n] = 0 ; n++ ; // 20
  varName[n] = "thetaDsE"        ; varLower[n] =  0     ; varUpper[n] = 6.28  ; varNBins[n] =  25 ; VarType[n] = 0 ; n++ ;
  varName[n] = "thetaDsPi"       ; varLower[n] =  0     ; varUpper[n] = 6.28  ; varNBins[n] =  25 ; VarType[n] = 0 ; n++ ;
  varName[n] = "thetaPiNu"       ; varLower[n] =  0     ; varUpper[n] = 6.28  ; varNBins[n] =  25 ; VarType[n] = 0 ; n++ ;
  varName[n] = "ThrustMag"       ; varLower[n] =  0     ; varUpper[n] = 1     ; varNBins[n] =  25 ; VarType[n] = 0 ; n++ ;
  n = 0 ;
  varXaxis[n] = "m_{m}(DKX#gamma#mu)^{2} (GeV^{2})" ; varYaxis[n] = "Entries per 0.050 Gev^{2}/c^{4}" ; varNameLaTeX[n] = "\\mmtwo"                         ; n++ ;
  varXaxis[n] = "E_{extra} (GeV)"                   ; varYaxis[n] = "Entries per 120 Mev/c"           ; varNameLaTeX[n] = "\\eextra"                        ; n++ ;
  varXaxis[n] = "ln(1+P(X^{2}|n)_{fit})/ln(2)"      ; varYaxis[n] = "Entries"                         ; varNameLaTeX[n] = "\\ln(P(\\chi^2|n))/\\ln2"        ; n++ ;
  varXaxis[n] = "m(D_{s}) (GeV)"                    ; varYaxis[n] = "Entries per 6 MeV/c^{2}"         ; varNameLaTeX[n] = "m_{D_s^+}"                       ; n++ ;
  varXaxis[n] = "n_{X}^{R}"                         ; varYaxis[n] = "Entries"                         ; varNameLaTeX[n] = "n_X^R"                           ; n++ ;
  varXaxis[n] = "p^{3}_{#mu} (lab) (GeV)"           ; varYaxis[n] = "Entries per 60 Mev/c"            ; varNameLaTeX[n] = "p_\\ell^{lab}"                   ; n++ ; //  5
  varXaxis[n] = "#Delta p3 fit (GeV)"               ; varYaxis[n] = "Entries per 60 Mev/c"            ; varNameLaTeX[n] = "|\\vec{p}_{raw}-\\vec{p}_{fit}|" ; n++ ;
  varXaxis[n] = "cos#theta_{Miss}"                  ; varYaxis[n] = "Entries"                         ; varNameLaTeX[n] = "\\cos\\theta_{miss}"             ; n++ ;
  varXaxis[n] = "P_{Miss} (Gev)"                    ; varYaxis[n] = "Entries per 120 Mev/c"           ; varNameLaTeX[n] = "p_{miss}"                        ; n++ ;
  varXaxis[n] = "E_{Miss} (GeV)"                    ; varYaxis[n] = "Entries per 120 Mev"             ; varNameLaTeX[n] = "E_{miss}"                        ; n++ ;
  varXaxis[n] = "#phi_{Miss}"                       ; varYaxis[n] = "Entries"                         ; varNameLaTeX[n] = "\\phi_{miss}"                    ; n++ ; // 10
  varXaxis[n] = "R_{0}"                             ; varYaxis[n] = "Entries"                         ; varNameLaTeX[n] = "R_0"                             ; n++ ;
  varXaxis[n] = "R_{1}/R_{0}"                       ; varYaxis[n] = "Entries"                         ; varNameLaTeX[n] = "R_1/R_0"                         ; n++ ;
  varXaxis[n] = "R_{2}/R_{0}"                       ; varYaxis[n] = "Entries"                         ; varNameLaTeX[n] = "R_2/R_0"                         ; n++ ;
  varXaxis[n] = "R_{3}/R_{0}"                       ; varYaxis[n] = "Entries"                         ; varNameLaTeX[n] = "R_3/R_0"                         ; n++ ;
  varXaxis[n] = "R_{4}/R_{0}"                       ; varYaxis[n] = "Entries"                         ; varNameLaTeX[n] = "R_4/R_0"                         ; n++ ; // 15
  varXaxis[n] = "#theta_{D_{s}#gamma}"              ; varYaxis[n] = "Entries"                         ; varNameLaTeX[n] = "\\theta_{D_s\\gamma}"            ; n++ ;
  varXaxis[n] = "#theta_{D_{s}K}"                   ; varYaxis[n] = "Entries"                         ; varNameLaTeX[n] = "\\theta_{D_sK}"                  ; n++ ;
  varXaxis[n] = "#theta_{D_{s}#nu}"                 ; varYaxis[n] = "Entries"                         ; varNameLaTeX[n] = "\\theta_{D_s\\nu}"               ; n++ ;
  varXaxis[n] = "#theta_{D_{s}Tag}"                 ; varYaxis[n] = "Entries"                         ; varNameLaTeX[n] = "\\theta_{D_s Tag}"               ; n++ ;
  varXaxis[n] = "#theta_{D_{s}#mu}"                 ; varYaxis[n] = "Entries"                         ; varNameLaTeX[n] = "\\theta_{D_s\\mu}"               ; n++ ; // 20
  varXaxis[n] = "#theta_{D_{s}e}"                   ; varYaxis[n] = "Entries"                         ; varNameLaTeX[n] = "\\theta_{D_s e}"                 ; n++ ;
  varXaxis[n] = "#theta_{D_{s}#pi}"                 ; varYaxis[n] = "Entries"                         ; varNameLaTeX[n] = "\\theta_{D_s\\pi}"               ; n++ ;
  varXaxis[n] = "#theta_{#pi#nu}"                   ; varYaxis[n] = "Entries"                         ; varNameLaTeX[n] = "\\theta_{\\pi\\nu}"              ; n++ ;
  varXaxis[n] = "Thrust"                            ; varYaxis[n] = "Entries"                         ; varNameLaTeX[n] = "Thrust"                          ; n++ ;
  n = 0 ;
  varSignalMode[n][0] = 1 ; varSignalMode[n][1] = 1 ; varSignalMode[n][2] = 1 ; varSignalMode[n][3] = 1 ; varSignalMode[n][4] = 1 ; varSignalMode[n][5] = 0 ; n++ ;
  varSignalMode[n][0] = 1 ; varSignalMode[n][1] = 1 ; varSignalMode[n][2] = 1 ; varSignalMode[n][3] = 1 ; varSignalMode[n][4] = 1 ; varSignalMode[n][5] = 1 ; n++ ;
  varSignalMode[n][0] = 1 ; varSignalMode[n][1] = 1 ; varSignalMode[n][2] = 1 ; varSignalMode[n][3] = 1 ; varSignalMode[n][4] = 1 ; varSignalMode[n][5] = 1 ; n++ ;
  varSignalMode[n][0] = 1 ; varSignalMode[n][1] = 1 ; varSignalMode[n][2] = 1 ; varSignalMode[n][3] = 1 ; varSignalMode[n][4] = 1 ; varSignalMode[n][5] = 1 ; n++ ;
  varSignalMode[n][0] = 1 ; varSignalMode[n][1] = 1 ; varSignalMode[n][2] = 1 ; varSignalMode[n][3] = 1 ; varSignalMode[n][4] = 1 ; varSignalMode[n][5] = 1 ; n++ ;
  varSignalMode[n][0] = 1 ; varSignalMode[n][1] = 1 ; varSignalMode[n][2] = 1 ; varSignalMode[n][3] = 1 ; varSignalMode[n][4] = 1 ; varSignalMode[n][5] = 0 ; n++ ; //  5
  varSignalMode[n][0] = 1 ; varSignalMode[n][1] = 1 ; varSignalMode[n][2] = 1 ; varSignalMode[n][3] = 1 ; varSignalMode[n][4] = 1 ; varSignalMode[n][5] = 1 ; n++ ;
  varSignalMode[n][0] = 1 ; varSignalMode[n][1] = 1 ; varSignalMode[n][2] = 1 ; varSignalMode[n][3] = 1 ; varSignalMode[n][4] = 1 ; varSignalMode[n][5] = 1 ; n++ ;
  varSignalMode[n][0] = 1 ; varSignalMode[n][1] = 1 ; varSignalMode[n][2] = 1 ; varSignalMode[n][3] = 1 ; varSignalMode[n][4] = 1 ; varSignalMode[n][5] = 1 ; n++ ;
  varSignalMode[n][0] = 1 ; varSignalMode[n][1] = 1 ; varSignalMode[n][2] = 1 ; varSignalMode[n][3] = 1 ; varSignalMode[n][4] = 1 ; varSignalMode[n][5] = 1 ; n++ ;
  varSignalMode[n][0] = 1 ; varSignalMode[n][1] = 1 ; varSignalMode[n][2] = 1 ; varSignalMode[n][3] = 1 ; varSignalMode[n][4] = 1 ; varSignalMode[n][5] = 1 ; n++ ; // 10
  varSignalMode[n][0] = 1 ; varSignalMode[n][1] = 1 ; varSignalMode[n][2] = 1 ; varSignalMode[n][3] = 1 ; varSignalMode[n][4] = 1 ; varSignalMode[n][5] = 1 ; n++ ;
  varSignalMode[n][0] = 1 ; varSignalMode[n][1] = 1 ; varSignalMode[n][2] = 1 ; varSignalMode[n][3] = 1 ; varSignalMode[n][4] = 1 ; varSignalMode[n][5] = 1 ; n++ ;
  varSignalMode[n][0] = 1 ; varSignalMode[n][1] = 1 ; varSignalMode[n][2] = 1 ; varSignalMode[n][3] = 1 ; varSignalMode[n][4] = 1 ; varSignalMode[n][5] = 1 ; n++ ;
  varSignalMode[n][0] = 1 ; varSignalMode[n][1] = 1 ; varSignalMode[n][2] = 1 ; varSignalMode[n][3] = 1 ; varSignalMode[n][4] = 1 ; varSignalMode[n][5] = 1 ; n++ ;
  varSignalMode[n][0] = 1 ; varSignalMode[n][1] = 1 ; varSignalMode[n][2] = 1 ; varSignalMode[n][3] = 1 ; varSignalMode[n][4] = 1 ; varSignalMode[n][5] = 1 ; n++ ; // 15
  varSignalMode[n][0] = 1 ; varSignalMode[n][1] = 1 ; varSignalMode[n][2] = 1 ; varSignalMode[n][3] = 1 ; varSignalMode[n][4] = 1 ; varSignalMode[n][5] = 1 ; n++ ;
  varSignalMode[n][0] = 1 ; varSignalMode[n][1] = 1 ; varSignalMode[n][2] = 1 ; varSignalMode[n][3] = 1 ; varSignalMode[n][4] = 1 ; varSignalMode[n][5] = 1 ; n++ ;
  varSignalMode[n][0] = 1 ; varSignalMode[n][1] = 1 ; varSignalMode[n][2] = 1 ; varSignalMode[n][3] = 1 ; varSignalMode[n][4] = 1 ; varSignalMode[n][5] = 1 ; n++ ;
  varSignalMode[n][0] = 1 ; varSignalMode[n][1] = 1 ; varSignalMode[n][2] = 1 ; varSignalMode[n][3] = 1 ; varSignalMode[n][4] = 1 ; varSignalMode[n][5] = 1 ; n++ ;
  varSignalMode[n][0] = 1 ; varSignalMode[n][1] = 0 ; varSignalMode[n][2] = 1 ; varSignalMode[n][3] = 0 ; varSignalMode[n][4] = 0 ; varSignalMode[n][5] = 0 ; n++ ; // 20
  varSignalMode[n][0] = 0 ; varSignalMode[n][1] = 1 ; varSignalMode[n][2] = 0 ; varSignalMode[n][3] = 1 ; varSignalMode[n][4] = 0 ; varSignalMode[n][5] = 0 ; n++ ;
  varSignalMode[n][0] = 0 ; varSignalMode[n][1] = 0 ; varSignalMode[n][2] = 0 ; varSignalMode[n][3] = 0 ; varSignalMode[n][4] = 1 ; varSignalMode[n][5] = 0 ; n++ ;
  varSignalMode[n][0] = 0 ; varSignalMode[n][1] = 0 ; varSignalMode[n][2] = 0 ; varSignalMode[n][3] = 0 ; varSignalMode[n][4] = 1 ; varSignalMode[n][5] = 0 ; n++ ;
  varSignalMode[n][0] = 1 ; varSignalMode[n][1] = 1 ; varSignalMode[n][2] = 1 ; varSignalMode[n][3] = 1 ; varSignalMode[n][4] = 1 ; varSignalMode[n][5] = 1 ; n++ ;
  for(Int_t var=0 ; var<NVar ; var++){ chain->SetBranchAddress( varName[var], &varValue[var]) ; }
  Int_t nXR ; chain->SetBranchAddress( varName[4], &nXR ) ;
  
  const Int_t NComponents = 14 ;
  TString  ComponentName[NComponents] ;
  TString ComponentTitle[NComponents] ;
  Int_t   ComponentColor[NComponents] ;
  ComponentName[0]  = "DsToMuNu"             ; ComponentTitle[0]  = "D_{s}#rightarrow#mu#nu"                           ; ComponentColor[0]  = SignalModeColor[0] ;
  ComponentName[1]  = "DsToENu"              ; ComponentTitle[1]  = "D_{s}#rightarrowe#nu"                             ; ComponentColor[1]  = SignalModeColor[1] ;
  ComponentName[2]  = "DsToTauNuTauToMuNuNu" ; ComponentTitle[2]  = "D_{s}#rightarrow#tau#nu;#tau#rightarrow#mu#nu#nu" ; ComponentColor[2]  = SignalModeColor[2] ;
  ComponentName[3]  = "DsToTauNuTauToENuNu"  ; ComponentTitle[3]  = "D_{s}#rightarrow#tau#nu;#tau#rightarrowe#nu#nu"   ; ComponentColor[3]  = SignalModeColor[3] ;
  ComponentName[4]  = "DsToTauNuTauToPiNu"   ; ComponentTitle[4]  = "D_{s}#rightarrow#tau#nu;#tau#rightarrow#pi#nu"    ; ComponentColor[4]  = SignalModeColor[4] ;
  ComponentName[5]  = "DsToTauNuTauToOther"  ; ComponentTitle[5]  = "D_{s}#rightarrow#tau#nu;#tau#rightarrowOther"     ; ComponentColor[5]  = 205 ;
  ComponentName[6]  = "DsSemileptonic"       ; ComponentTitle[6]  = "D_{s}#rightarrowl#nuZ"                            ; ComponentColor[6]  =   2 ;
  ComponentName[7]  = "DsHadronic"           ; ComponentTitle[7]  = "D_{s}#rightarrowHadrons"                          ; ComponentColor[7]  =   7 ;
  ComponentName[8]  = "VirtualPhoton"        ; ComponentTitle[8]  = "#gamma_{virtual}#rightarrow#piZ"                  ; ComponentColor[8]  = 144 ;
  ComponentName[9]  = "D0ToKEllNu"           ; ComponentTitle[9]  = "D^{0}#rightarrowKl#nu"                            ; ComponentColor[9]  = 225 ;
  ComponentName[10] = "DToK0EllNu"           ; ComponentTitle[10] = "D#rightarrowK^{0}l#nu"                            ; ComponentColor[10] = 226 ;
  ComponentName[11] = "OtherCc"              ; ComponentTitle[11] = "Other c#bar{c}"                                   ; ComponentColor[11] = 208 ;
  ComponentName[12] = "uds"                  ; ComponentTitle[12] = "uds"                                              ; ComponentColor[12] =   5 ;
  ComponentName[13] = "Other"                ; ComponentTitle[13] = "Other"                                            ; ComponentColor[13] = 217 ;
  
  TH1F*       hVar[NEExtraCut][NVar][NSignalMode][NComponents][2] ; // With and without Ds BF weighting
  Float_t      nMC[NEExtraCut][NVar][NSignalMode][2] ;
  Float_t    nData[NEExtraCut][NVar][NSignalMode] ;
  TH1F*   hVarData[NEExtraCut][NVar][NSignalMode] ;
  TH1F* hVarSignal[NEExtraCut][NVar][NSignalMode] ;
  TString DsBFString[2] = { "" , "_NoDsBFWeighting" } ;
  TLegend varLegend = MakeLegend(0.7 , 0.5 , 0.95 , 0.95 ) ;
  for(EExtraCut=0 ; EExtraCut<NEExtraCut ; EExtraCut++)
  {
    for(Int_t var=varStart ; var<varEnd+1 ; var++)
    {
      for(SignalMode=SignalModeStart ; SignalMode<SignalModeEnd+1 ; SignalMode++)
      {
        TString histogramName = TString(TString("h_") + varName[var] + "_Data_" + suffix()) ;
        hVarData[EExtraCut][var][SignalMode-SignalModeStart] = new TH1F(histogramName, histogramName, varNBins[var], varLower[var], varUpper[var]) ;
        hVarData[EExtraCut][var][SignalMode-SignalModeStart]->SetFillColor(1) ;
        hVarData[EExtraCut][var][SignalMode-SignalModeStart]->SetLineWidth(2) ;
        hVarData[EExtraCut][var][SignalMode-SignalModeStart]->GetXaxis()->SetTitle(varXaxis[var]) ;
        hVarData[EExtraCut][var][SignalMode-SignalModeStart]->GetYaxis()->SetTitle(varYaxis[var]) ;   
        hVarData[EExtraCut][var][SignalMode-SignalModeStart]->GetYaxis()->SetTitleOffset(1.0) ;
        hVarData[EExtraCut][var][SignalMode-SignalModeStart]->GetYaxis()->SetLabelSize(0.025) ;
        hVarData[EExtraCut][var][SignalMode-SignalModeStart]->SetMarkerStyle(20) ;
        hVarData[EExtraCut][var][SignalMode-SignalModeStart]->SetMinimum(0) ;

    histogramName = TString(TString("h_") + varName[var] + "_Signal_" + suffix()) ;
    hVarSignal[EExtraCut][var][SignalMode-SignalModeStart] = new TH1F(histogramName, histogramName, varNBins[var], varLower[var], varUpper[var]) ;
    hVarSignal[EExtraCut][var][SignalMode-SignalModeStart]->SetFillColor(ComponentColor[SignalMode]) ;
    hVarSignal[EExtraCut][var][SignalMode-SignalModeStart]->SetLineWidth(2) ;
    hVarSignal[EExtraCut][var][SignalMode-SignalModeStart]->GetXaxis()->SetTitle(varXaxis[var]) ;
    hVarSignal[EExtraCut][var][SignalMode-SignalModeStart]->GetYaxis()->SetTitle(varYaxis[var]) ;     
    hVarSignal[EExtraCut][var][SignalMode-SignalModeStart]->GetYaxis()->SetTitleOffset(1.0) ;
    hVarSignal[EExtraCut][var][SignalMode-SignalModeStart]->GetYaxis()->SetLabelSize(0.025) ;
    hVarSignal[EExtraCut][var][SignalMode-SignalModeStart]->SetMinimum(0) ;
    for(Int_t Component=0 ; Component<NComponents ; Component++)
    {
      for(Int_t DsBF=0 ; DsBF<2 ; DsBF++)
      {
        nMC[EExtraCut][var][SignalMode][DsBF] = 0.0 ;
        nData[EExtraCut][var][SignalMode]     = 0.0 ;
        TString histogramName = TString(TString("h_") + varName[var] + "_" + ComponentName[Component] + DsBFString[DsBF] + "_" + suffix()) ;
        hVar[EExtraCut][var][SignalMode-SignalModeStart][Component][DsBF] = new TH1F(histogramName, histogramName, varNBins[var], varLower[var], varUpper[var]) ;
        hVar[EExtraCut][var][SignalMode-SignalModeStart][Component][DsBF]->SetFillColor(ComponentColor[Component]) ;
        hVar[EExtraCut][var][SignalMode-SignalModeStart][Component][DsBF]->SetLineWidth(2) ;
        hVar[EExtraCut][var][SignalMode-SignalModeStart][Component][DsBF]->GetXaxis()->SetTitle(varXaxis[var]) ;
        hVar[EExtraCut][var][SignalMode-SignalModeStart][Component][DsBF]->GetYaxis()->SetTitle(varYaxis[var]) ;
        hVar[EExtraCut][var][SignalMode-SignalModeStart][Component][DsBF]->GetYaxis()->SetTitleOffset(1.0) ;
        hVar[EExtraCut][var][SignalMode-SignalModeStart][Component][DsBF]->GetYaxis()->SetLabelSize(0.025) ;
        hVar[EExtraCut][var][SignalMode-SignalModeStart][Component][DsBF]->SetMinimum(0) ;
      }
    }
      }
    }
  }
  for(Int_t Component=0 ; Component<NComponents ; Component++){ varLegend.AddEntry( hVar[0][0][0][Component][0] , ComponentTitle[Component] , "f") ; }
  varLegend.AddEntry( hVarData[0][0][0] , "Data" , "p") ;
  THStack varStack[NEExtraCut][NVar][NSignalMode][2] ;
  cout << "Set up vars plots" << endl ;

  // SPMode plots
  const Int_t NSPModePlots = 14 ;
  TString SPModePlotsName[NSPModePlots] ; Int_t SPModeColor[NSPModePlots] ; Int_t SPSignalMode[NSPModePlots][NSignalMode] ;
  SPModePlotsName[0]  = "uds"                  ; SPModeColor[0]  =  2 ;
  SPModePlotsName[1]  = "ccbar"                ; SPModeColor[1]  =  3 ;
  SPModePlotsName[2]  = "BBTau"                ; SPModeColor[2]  =  4 ;
  SPModePlotsName[3]  = "DsToMuNu"             ; SPModeColor[3]  =  5 ;
  SPModePlotsName[4]  = "DsToENu"              ; SPModeColor[4]  =  6 ;
  SPModePlotsName[5]  = "DsToTauNuTauToMuNuNu" ; SPModeColor[5]  =  7 ;
  SPModePlotsName[6]  = "DsToTauNuTauToENuNu"  ; SPModeColor[6]  =  8 ;
  SPModePlotsName[7]  = "DsToTauNuTauToPiNu"   ; SPModeColor[7]  =  9 ;
  SPModePlotsName[8]  = "DsToK0MuNu"           ; SPModeColor[8]  = 10 ;
  SPModePlotsName[9]  = "DsToK0ENu"            ; SPModeColor[9]  = 11 ;
  SPModePlotsName[10] = "DsToPhiMuNu"          ; SPModeColor[10] = 12 ;
  SPModePlotsName[11] = "DsToPhiENu"           ; SPModeColor[11] = 13 ;
  SPModePlotsName[12] = "DsToEtaMuNu"          ; SPModeColor[12] = 14 ;
  SPModePlotsName[13] = "DsToEtaENu"           ; SPModeColor[13] = 15 ;
  SPSignalMode[0][0]  = 1 ; SPSignalMode[0][1]  = 1 ; SPSignalMode[0][2]  = 1 ; SPSignalMode[0][3]  = 1 ; SPSignalMode[0][4]  = 1 ; SPSignalMode[0][5]  = 1 ;
  SPSignalMode[1][0]  = 1 ; SPSignalMode[1][1]  = 1 ; SPSignalMode[1][2]  = 1 ; SPSignalMode[1][3]  = 1 ; SPSignalMode[1][4]  = 1 ; SPSignalMode[1][5]  = 1 ;
  SPSignalMode[2][0]  = 1 ; SPSignalMode[2][1]  = 1 ; SPSignalMode[2][2]  = 1 ; SPSignalMode[2][3]  = 1 ; SPSignalMode[2][4]  = 1 ; SPSignalMode[2][5]  = 1 ;
  SPSignalMode[3][0]  = 1 ; SPSignalMode[3][1]  = 1 ; SPSignalMode[3][2]  = 1 ; SPSignalMode[3][3]  = 1 ; SPSignalMode[3][4]  = 1 ; SPSignalMode[3][5]  = 1 ;
  SPSignalMode[4][0]  = 1 ; SPSignalMode[4][1]  = 1 ; SPSignalMode[4][2]  = 1 ; SPSignalMode[4][3]  = 1 ; SPSignalMode[4][4]  = 1 ; SPSignalMode[4][5]  = 1 ;
  SPSignalMode[5][0]  = 1 ; SPSignalMode[5][1]  = 1 ; SPSignalMode[5][2]  = 1 ; SPSignalMode[5][3]  = 1 ; SPSignalMode[5][4]  = 1 ; SPSignalMode[5][5]  = 1 ;
  SPSignalMode[6][0]  = 1 ; SPSignalMode[6][1]  = 1 ; SPSignalMode[6][2]  = 1 ; SPSignalMode[6][3]  = 1 ; SPSignalMode[6][4]  = 1 ; SPSignalMode[6][5]  = 1 ;
  SPSignalMode[7][0]  = 1 ; SPSignalMode[7][1]  = 1 ; SPSignalMode[7][2]  = 1 ; SPSignalMode[7][3]  = 1 ; SPSignalMode[7][4]  = 1 ; SPSignalMode[7][5]  = 1 ;
  SPSignalMode[8][0]  = 1 ; SPSignalMode[8][1]  = 0 ; SPSignalMode[8][2]  = 1 ; SPSignalMode[8][3]  = 0 ; SPSignalMode[8][4]  = 1 ; SPSignalMode[8][5]  = 0 ;
  SPSignalMode[9][0]  = 0 ; SPSignalMode[9][1]  = 1 ; SPSignalMode[9][2]  = 0 ; SPSignalMode[9][3]  = 1 ; SPSignalMode[9][4]  = 0 ; SPSignalMode[9][5]  = 0 ;
  SPSignalMode[10][0] = 1 ; SPSignalMode[10][1] = 0 ; SPSignalMode[10][2] = 1 ; SPSignalMode[10][3] = 0 ; SPSignalMode[10][4] = 1 ; SPSignalMode[10][5] = 0 ;
  SPSignalMode[11][0] = 0 ; SPSignalMode[11][1] = 1 ; SPSignalMode[11][2] = 0 ; SPSignalMode[11][3] = 1 ; SPSignalMode[11][4] = 0 ; SPSignalMode[11][5] = 0 ;
  SPSignalMode[12][0] = 1 ; SPSignalMode[12][1] = 0 ; SPSignalMode[12][2] = 1 ; SPSignalMode[12][3] = 0 ; SPSignalMode[12][4] = 1 ; SPSignalMode[12][5] = 0 ;
  SPSignalMode[13][0] = 0 ; SPSignalMode[13][1] = 1 ; SPSignalMode[13][2] = 0 ; SPSignalMode[13][3] = 1 ; SPSignalMode[13][4] = 0 ; SPSignalMode[13][5] = 0 ;
  
  Int_t SPModeMap[NSPModes] ;
  SPModeMap[0]  =  0 ;
  SPModeMap[1]  =  1 ;
  SPModeMap[2]  =  2 ;
  SPModeMap[3]  =  2 ;
  SPModeMap[4]  =  2 ;
  SPModeMap[5]  =  3 ;
  SPModeMap[6]  =  3 ;
  SPModeMap[7]  = -1 ;
  SPModeMap[8]  = -1 ;
  SPModeMap[9]  = -1 ;
  SPModeMap[10] = -1 ;
  SPModeMap[11] =  4 ;
  SPModeMap[12] =  4 ;
  SPModeMap[13] =  5 ;
  SPModeMap[14] =  5 ;
  SPModeMap[15] =  6 ;
  SPModeMap[16] =  6 ;
  SPModeMap[17] =  8 ;
  SPModeMap[18] =  8 ;
  SPModeMap[19] =  9 ;
  SPModeMap[20] =  9 ;
  SPModeMap[21] = 10 ;
  SPModeMap[22] = 10 ;
  SPModeMap[23] = 11 ;
  SPModeMap[24] = 11 ;
  SPModeMap[25] = 12 ;
  SPModeMap[26] = 12 ;
  SPModeMap[27] = 13 ;
  SPModeMap[28] = 13 ;
  SPModeMap[29] =  7 ;
  SPModeMap[30] =  7 ;
  TH1F* hSPModeVar[NSignalMode][NSPModePlots][NVar] ;
  for(SignalMode=0 ; SignalMode<NSignalMode ; SignalMode++)
  {
    for(Int_t i=0 ; i<NSPModePlots ; i++)
    {
      for(Int_t var=varStart ; var<varEnd+1 ; var++)
      {
    TString histogramName = TString(TString("hSPModeVar")+SignalModeName[SignalMode]+(long)i+"_"+varName[var]) ;
    hSPModeVar[SignalMode][i][var] = new TH1F(histogramName, histogramName, varNBins[var], varLower[var], varUpper[var]) ;
    hSPModeVar[SignalMode][i][var]->SetFillColor(SPModeColor[i]) ;
    hSPModeVar[SignalMode][i][var]->SetLineWidth(2) ;
    hSPModeVar[SignalMode][i][var]->GetXaxis()->SetTitle(varXaxis[var]) ;
    hSPModeVar[SignalMode][i][var]->GetYaxis()->SetTitle(varYaxis[var]) ;
    hSPModeVar[SignalMode][i][var]->GetYaxis()->SetTitleOffset(1.0) ;
    hSPModeVar[SignalMode][i][var]->GetYaxis()->SetLabelSize(0.025) ;
    hSPModeVar[SignalMode][i][var]->SetMinimum(0) ;
      }
    }
  }
  cout << "Set up SPModes" << endl ;

  // PHOTOS
  const Int_t    NNGamma = 6 ;
  const Int_t NPHOTOSVar = 3 ;
  TH1F* hPHOTOS[NEExtraCut][NSignalMode][NNGamma][NPHOTOSVar] ;
  Int_t PHOTOSVar[NPHOTOSVar] = { 0 , 3 , 1 } ; // mm2, dsmass, eextra
  for(EExtraCut=0 ; EExtraCut<NEExtraCut ; EExtraCut++)
  {
    for(SignalMode=SignalModeStart ; SignalMode<SignalModeEnd+1 ; SignalMode++)
    {
      for(Int_t nGamma=0 ; nGamma<NNGamma ; nGamma++)
      {
    for(Int_t i=0 ; i<3 ; i++)
    {
      TString histogramName = TString(TString("hPHOTOS")+(long)nGamma+varName[PHOTOSVar[i]]+SignalModeName[SignalMode]+EExtraCutString[EExtraCut]) ;
      hPHOTOS[EExtraCut][SignalMode][nGamma][i] = new TH1F(histogramName,histogramName,varNBins[PHOTOSVar[i]],varLower[PHOTOSVar[i]],varUpper[PHOTOSVar[i]]) ;
      hPHOTOS[EExtraCut][SignalMode][nGamma][i]->GetXaxis()->SetTitle(varXaxis[PHOTOSVar[i]]) ;
      hPHOTOS[EExtraCut][SignalMode][nGamma][i]->GetYaxis()->SetTitle(varYaxis[PHOTOSVar[i]]) ;
      hPHOTOS[EExtraCut][SignalMode][nGamma][i]->GetYaxis()->SetLabelSize(0.025) ;
      hPHOTOS[EExtraCut][SignalMode][nGamma][i]->SetFillColor(SignalModeColor[SignalMode]) ;
    }
      }
    }
  }
  cout << "Set up PHOTOS plots" << endl ;

  const Int_t NEExtraSlices = 10 ; 
  TH1F* hMm2InEExtra[NSignalMode][NEExtraSlices] ;
  for(SignalMode=0 ; SignalMode<NSignalMode-1 ; SignalMode++)
  {
    for(Int_t slice=0 ; slice<NEExtraSlices ; slice++)
    {
      TString histogramName = TString(TString("hEExtraSlice")+SignalModeName[SignalMode]+(long)slice) ;
      hMm2InEExtra[SignalMode][slice] = new TH1F(histogramName,histogramName,varNBins[0],varLower[0],varUpper[0]) ;
      hMm2InEExtra[SignalMode][slice]->GetXaxis()->SetTitle(TString(TString("m_{m}(DKX#gamma#mu)^{2} [") + FloatToString(1.0*slice/NEExtraSlices,1) + "<E_{Extra}<" + FloatToString(1.0*(slice+1)/NEExtraSlices,1) + " GeV] (GeV^{2})")) ;
      hMm2InEExtra[SignalMode][slice]->GetYaxis()->SetTitle(varYaxis[0]) ;
      hMm2InEExtra[SignalMode][slice]->SetFillColor(202) ;
    }
  }
  cout << "Set up mm2 EExtra slices plots" << endl ;

  // Loop  over reconstructions
  Double_t m ;
  Double_t d ;
  Double_t e ;
  Double_t p ;
  Double_t w ;
  Int_t    s ;
  Float_t weight ;
  chain->SetBranchAddress("weight",&weight) ;
  Int_t    currentGoodeventid = -1 ;
  Int_t    currentRun         = -1 ;
  Int_t    currentSPMode      = -1 ;
  Int_t    startEntry         = -1 ;
  Int_t    endEntry           = -1 ;
  Int_t    NEntries           = chain->GetEntries() ;
  Int_t    counter            = 0 ;
  Float_t  DsMassWindowLower  = 1.934 ;
  Float_t  DsMassWindowUpper  = 2.012 ;
  Float_t  multiplicity[NSignalMode] ;

  for(Int_t i=0 ; i<NEntries ; i++)
    //for(Int_t i=614624 ; i<800000 ; i++)
  {
    if(i%10000==0) cout << i << " / " << NEntries << endl ;
    chain->GetEntry(i) ;    
    for(EExtraCut=0 ; EExtraCut<NEExtraCut ; EExtraCut++)
    {
      currentRun         = run         ;
      currentSPMode      = SPMode      ;
      currentGoodeventid = goodeventid ;
      for(SignalMode=0 ; SignalMode<NSignalMode ; SignalMode++) multiplicity[SignalMode] = 0 ;
      if(EExtraCut==0) startEntry = i ;
      endEntry = i ;
      for(Int_t j=startEntry ; j<NEntries ; j++)
      {
    chain->GetEntry(j) ;
    e = varValue[1] ;
    s = SignalMode-SignalModeStart ;
    if(SignalMode<SignalModeStart) continue ;
    if(SignalMode>SignalModeEnd)   continue ;
    if( goodeventid!=currentGoodeventid ) break ;
    if(         run!=currentRun         ) break ;
    if(      SPMode!=currentSPMode      ) break ;
    endEntry = j ;
    if(NExtraTracks!=0)     continue ;
    if(s<0||s>=NSignalMode) continue ;
    if((SignalMode==0||SignalMode==2) && (MuBDTVeryLoose+MuBDTLoPLoose)==0) continue ;
    if((SignalMode==1||SignalMode==3) && EKMSuperLoose==0)                  continue ;
    if(SignalMode==4 && PiKMSuperLoose==0)                                  continue ;
    if(EExtraCut==kTRUE && SignalMode<2 && e>1.0) continue ;
    if(EExtraCut==kTRUE && SignalMode>1 && e>5.0) continue ;
    if(varValue[3]<DsMassWindowLower) continue ;
    if(varValue[3]>DsMassWindowUpper) continue ;
    multiplicity[s]++ ;
      }
      i = endEntry ;
      Float_t totalMultiplicity = 0 ;
      for(SignalMode=0 ; SignalMode<5 ; SignalMode++) totalMultiplicity += multiplicity[SignalMode] ;
      //if(totalMultiplicity==0) continue ;      
      for(Int_t j=startEntry ; j<endEntry+1 ; j++)
      {
    chain->GetEntry(j) ;
    m = varValue[0] ;
    e = varValue[1] ;
    p = varValue[2] ;
    d = varValue[3] ;
    s = SignalMode-SignalModeStart ;
    if(SignalMode<SignalModeStart) continue ;
    if(SignalMode>SignalModeEnd)   continue ;
    if(NExtraTracks!=0)     continue ;
    if(s<0||s>=NSignalMode) continue ;
    if((SignalMode==0||SignalMode==2) && (MuBDTVeryLoose+MuBDTLoPLoose)==0) continue ;
    if((SignalMode==1||SignalMode==3) && EKMSuperLoose==0)                  continue ;
    if(SignalMode==4 && PiKMSuperLoose==0)                                  continue ;
    if(varValue[3]<DsMassWindowLower) continue ;
    if(varValue[3]>DsMassWindowUpper) continue ;
    varValue[4] = nXR ;
    counter++ ;
    varValue[2] = log(varValue[2]+1)/log(2.0) ;
    if(SPMode==10)
    {
      w = (multiplicity[s]!=0) ? 1.0/multiplicity[s] : 1 ;
      hMultiplicity[EExtraCut][SignalMode-SignalModeStart][0]->Fill(multiplicity[SignalMode],1.0/multiplicity[SignalMode]) ;
      for(Int_t var=varStart ; var<varEnd+1 ; var++)
      {
        if(SignalMode==0 && e>1.0 && var!=1 && EExtraCut==1) continue ; // EExtra cut for DsToMuNu
        if(SignalMode>2  && e>5.0 && var!=1 && EExtraCut==1) continue ; // EExtra cut for DsToTauNu
        if(SignalMode==2 && var==1 && m<0.5) continue ;
        hVarData[EExtraCut][var][s]->Fill( varValue[var] , w ) ;
        nData[EExtraCut][var][s] += w ;
      }
      if(SignalMode==2 && m<0.5) continue ;
      
      thesisWeight    = w ;
      thesisMm2       = m ;
      thesisEExtra    = e ;
      thesisEllp3Lab  = varValue[5] ;
      thesisDsMass    = d ;
      thesisComponent = -1 ;
      if(EExtraCut==0)thesisTree->Fill() ;
      
      if(EExtraCut==0) continue ;
      // Fill Roofit trees
      mRoofit     = m   ;
      dRoofit     = d   ;
      eRoofit     = e   ;
      wLumiRoofit = 1   ;
      rRoofit     = run ;
      wRoofit     = (multiplicity[s]==0) ? 0 : 1.0/multiplicity[s] ;
      SRoofit     = SignalMode ;
      nRoofit     = nXR ;
      tRoofit     = 10 ;
      pRoofit     = varValue[5] ;
      gRoofit     = -1 ;
      lundRoofit  = 0 ;
      RoofitT->Fill() ;
    }
    else
    {
      w = (multiplicity[s]!=0) ? weight/multiplicity[s] : weight ;
      
      // Get PID weight
      Int_t selector = -1 ;
      if(SignalMode%2==0)
      {
        if(MuBDTVeryLoose==1) selector = 0 ;
        if(MuBDTLoPLoose==1 ) selector = 1 ;
      }
      if(SignalMode%2==1)
      {
        if(EKMSuperLoose==1) selector = 2 ;
      }
      if(selector>-1 && SignalMode<4)
      {
        Int_t PIDRun = -1 ;
        switch(run)
        {
        case 0:
        case 1:
          PIDRun = 0 ;
          break ;
        case 2:
        case 3:
          PIDRun = 1 ;
          break ;
        case 4:
        case 5:
          PIDRun = 2 ;
          break ;
        case 6:
        case 7:
          PIDRun = 3 ;
          break ;
        case 8:
        case 9:
          PIDRun = 4 ;
          break ;
        case 10:
        case 11:
          PIDRun = 5 ;
          break ;
        case 12:
        case 13:
        case 14:
        case 15:
          PIDRun = 6 ;
          break ;
        default:
          break ;
        }
        if(PIDRun>-1)
        {
          Int_t charge = (EllLund>0) ;
          Float_t pxLabRaw = EpxLabRaw + MupxLabRaw ;
          Float_t pyLabRaw = EpyLabRaw + MupyLabRaw ;
          Float_t pzLabRaw = EpzLabRaw + MupzLabRaw ;
          Float_t  pLabRaw = sqrt(pxLabRaw*pxLabRaw + pyLabRaw*pyLabRaw + pzLabRaw*pzLabRaw) ;
          Float_t theta = acos(pzLabRaw/pLabRaw) ;
          theta = theta*180.0/3.14159 ;
          //cout << selector << " " << PIDRun << " " << charge << " " << hPID[selector][PIDRun][charge] << endl ;
          TH2D* hClone = (TH2D*)hPID[selector][PIDRun][charge]->Clone("hClone") ;
          hClone->Reset() ;
          hClone->Fill(theta,pLabRaw) ;
          hClone->Multiply(hPID[selector][PIDRun][charge]) ;
          w = w*hClone->GetSumOfWeights() ;
          delete hClone ;
        }
      }
      if(HasDs)
      {
        w = w*nXTWeights[nXT] ;
        nXTYields[SignalMode][nXT] +=w ;
      }
      
      Int_t HasSignal = 0 ;
      switch(SignalMode)
      {
      case 0: HasSignal = HasDsToMuNu             ; break ;
      case 1: HasSignal = HasDsToENu              ; break ;
      case 2: HasSignal = HasDsToTauNuTauToMuNuNu ; break ;
      case 3: HasSignal = HasDsToTauNuTauToENuNu  ; break ;
      case 4: HasSignal = HasDsToTauNuTauToPiNu   ; break ;
      }
      
      if(HasSignal==1)
      {
        hPHOTOS[EExtraCut][SignalMode-SignalModeStart][EllSibNGamma][0]->Fill(m,w) ;
        hPHOTOS[EExtraCut][SignalMode-SignalModeStart][EllSibNGamma][1]->Fill(d,w) ;
        hPHOTOS[EExtraCut][SignalMode-SignalModeStart][EllSibNGamma][2]->Fill(e,w) ;
      }
      
      Int_t ModeNumber = -1 ;
      if     ( HasDsToMuNu==1                            ) ModeNumber = 0  ;
      else if( HasDsToENu==1                             ) ModeNumber = 1  ;
      else if( HasDsToTauNuTauToMuNuNu==1                ) ModeNumber = 2  ;
      else if( HasDsToTauNuTauToENuNu==1                 ) ModeNumber = 3  ;
      else if( HasDsToTauNuTauToPiNu==1                  ) ModeNumber = 4  ;
      else if( abs(EllMothLund)==15                      ) ModeNumber = 5  ;
      else if( abs(EllMothLund)==431 && abs(EllLund)<100 ) ModeNumber = 6  ;
      else if( abs(EllMothLund)==431 && abs(EllLund)>100 ) ModeNumber = 7  ;
      else if( abs(EllMothLund)==10022                   ) ModeNumber = 8  ;
      else if( abs(EllMothLund)==421 && (abs(EllSib1Lund*EllSib2Lund*EllSib3Lund)==11*12*321 || abs(EllSib1Lund*EllSib2Lund*EllSib3Lund)==13*14*321) && EllMothDauLen==3) ModeNumber = 9  ;
      else if( abs(EllMothLund)==411 && (abs(EllSib1Lund*EllSib2Lund*EllSib3Lund)==11*12*311 || abs(EllSib1Lund*EllSib2Lund*EllSib3Lund)==13*14*311) && EllMothDauLen==3) ModeNumber = 10 ;
      else if( SPMode==1                                 ) ModeNumber = 11 ;
      else if( SPMode==0                                 ) ModeNumber = 12 ;
      else                                                 ModeNumber = 11 ;
      // Fill MC per reconstruction
      if(ModeNumber==-1)
      {
        cout << "Error: Unclassified mode.  " 
         << SignalMode  << " "
         << SPMode      << " "
         << EllMode     << " "
         << ModeNumber  << " "
         << " MC truth information: "
         << EllMothLund << " "
         << EllSib1Lund << " "
         << EllSib2Lund << " "
         << EllSib3Lund << " "
         << EllSib4Lund << " "
         << EllSib5Lund << " "
         << goodeventid << endl ;
      }
      for(Int_t var=varStart ; var<varEnd+1 ; var++)
      {
        if(SignalMode==0 && e>1.0 && var!=1 && EExtraCut==1) continue ; // EExtra cut for DsToMuNu
        if(SignalMode>1  && e>5.0 && var!=1 && EExtraCut==1) continue ; // EExtra cut for DsToTauNu
        if(SignalMode==2 && var==1 && m<0.5) continue ;
        if(SPMode<5)
        {
          hVar[EExtraCut][var][SignalMode-SignalModeStart][ModeNumber][0]->Fill(varValue[var],w*DsDecayRateWeight) ;
          hVar[EExtraCut][var][SignalMode-SignalModeStart][ModeNumber][1]->Fill(varValue[var],w) ;
          nMC[EExtraCut][var][SignalMode-SignalModeStart][0] += w*DsDecayRateWeight ;
          nMC[EExtraCut][var][SignalMode-SignalModeStart][0] += w ;
        }
        if(SignalMode==0 && (SPMode==5 ||SPMode==6 )) hVarSignal[EExtraCut][var][SignalMode-SignalModeStart]->Fill(varValue[var],w) ;
        if(SignalMode==1 && (SPMode==11||SPMode==12)) hVarSignal[EExtraCut][var][SignalMode-SignalModeStart]->Fill(varValue[var],w) ;
        if(SignalMode==2 && (SPMode==13||SPMode==14)) hVarSignal[EExtraCut][var][SignalMode-SignalModeStart]->Fill(varValue[var],w) ;
        if(SignalMode==3 && (SPMode==15||SPMode==16)) hVarSignal[EExtraCut][var][SignalMode-SignalModeStart]->Fill(varValue[var],w) ;
        if(SignalMode==4 && (SPMode==29||SPMode==30)) hVarSignal[EExtraCut][var][SignalMode-SignalModeStart]->Fill(varValue[var],w) ;
        if(SPModeMap[SPMode]!=-1) hSPModeVar[SignalMode][SPModeMap[SPMode]][var]->Fill(varValue[var]) ;
      }
      if(multiplicity[s]==0) continue ;
      
      //if(EExtraCut==(SignalMode==1)) continue ;
      //if(SignalMode==4 && varValue[8] >0.9 ) continue ; // PMissCosTh cut for DsToTauNuTauToPiNu
      //if(SignalMode==4 && varValue[13]<0.02) continue ; // R1 cut for DsToTauNuTauToPiNu
      
      if(SPMode<5 && HasSignal==0)
      {
        for(Int_t slice=0 ; slice<NEExtraSlices ; slice++)
        {
          Float_t lower = 1.0*slice/NEExtraSlices ;
          Float_t upper = 1.0*(slice+1)/NEExtraSlices ;
          if(lower<=e && e<upper){ hMm2InEExtra[SignalMode][slice]->Fill(m,w) ; }
        }
      }

      thesisWeight     = w ;
      thesisWeightDsBF = (DsDecayRateWeight==0) ? w : w*DsDecayRateWeight ;
      thesisMm2        = m ;
      thesisEExtra     = e ;
      thesisEllp3Lab   = varValue[5] ;
      thesisDsMass     = d ;
      thesisComponent  = ModeNumber ;
      if(EExtraCut==0)thesisTree->Fill() ;
      if(SignalMode==2 && m<0.5) continue ;
      // BF histograms
      if(EExtraCut==1)
      {
        switch(SignalMode)
        {
        case 0:
          if(SPMode==5 ||SPMode==6 )
          {
        hBFSignal[SignalMode][nXT]->Fill(m,1/multiplicity[s]) ;
        hBFSignal[SignalMode][NNXT]->Fill(m,1/multiplicity[s]) ;
          }
          break ;
        case 1:
          if(SPMode==11||SPMode==12)
          {
        hBFSignal[SignalMode][nXT]->Fill(m,1/multiplicity[s]) ;
        hBFSignal[SignalMode][NNXT]->Fill(m,1/multiplicity[s]) ;
          }
          break ;
        case 2:
          if(SPMode==13||SPMode==14)
          {
        hBFSignal[SignalMode][nXT]->Fill(m,1/multiplicity[s]) ;
        hBFSignal[SignalMode][NNXT]->Fill(m,1/multiplicity[s]) ;
          }
          break ;
        case 3:
          if(SPMode==15||SPMode==16)
          {
        hBFSignal[SignalMode][nXT]->Fill(m,1/multiplicity[s]) ;
        hBFSignal[SignalMode][NNXT]->Fill(m,1/multiplicity[s]) ;
          }
          break ;
        case 4:
          if(SPMode==29||SPMode==30)
          {
        hBFSignal[SignalMode][nXT]->Fill(m,1/multiplicity[s]) ;
        hBFSignal[SignalMode][NNXT]->Fill(m,1/multiplicity[s]) ;
          }
          break ;
        }
      }
      if(SPMode<5){ if(HasSignal) hBFYield[SignalMode]->Fill(m,1/multiplicity[s]) ;}
      
      hMultiplicity[EExtraCut][SignalMode-SignalModeStart][2-HasSignal]->Fill(multiplicity[SignalMode],1.0/multiplicity[SignalMode]) ;
      
      if(EExtraCut==1) continue ; // Only count each entry once
      // Fill Roofit trees
      mRoofit      = m ;
      dRoofit      = d ;
      eRoofit      = e ;
      wDsRoofit    = (DsDecayRateWeight>0) ? w*DsDecayRateWeight : w ;
      DsTypeRoofit = DsDecayRateType ;
      ModeRoofit   = ModeNumber ;
      wLumiRoofit  = w ;
      rRoofit      = run ;
      wRoofit      = 1.0/multiplicity[s] ;
      SRoofit      = SignalMode ;
      nRoofit      = nXR ;
      tRoofit      = -1 ;
      pRoofit      = varValue[5] ;
      gRoofit      = EllSibNGamma ;
      lundRoofit   = EllLund ;
      SPModeRoofit = SPMode ;
      switch(SPMode)
      {
        // Peaking backgrounds
      case 7:
      case 8:
      case 9:
        break ;
        // Signal
      case 5:
      case 6:
        if(SignalMode==0) tRoofit = 1 ;
        break ;
      case 11:
      case 12:
        if(SignalMode==1) tRoofit = 1 ;
        break ;
      case 13:
      case 14:
        if(SignalMode==2) tRoofit = 1 ;
        break ;
      case 15:
      case 16:
        if(SignalMode==3) tRoofit = 1 ;
        break ;
      case 29:
      case 30:
        if(SignalMode==4) tRoofit = 1 ;
        break ;
        // Data
      case 10: 
        break ;
        // Generic
      case 0:
      case 1:
      case 2:
      case 3:
      case 4:
        tRoofit = (HasSignal==1) ? 2 : 3 ;
        break ;
      case 17:
      case 18:
      case 19:
      case 20:
      case 21:
      case 22:
      case 23:
      case 24:
      case 25:
      case 26:
      case 27:
      case 28:
        tRoofit = 100 ;
        break ;
      default:
        break ;
      } // Switch(SPMode)
      // RoofitTypes:
      // 0: Var
      // 1: Signal
      // 2: True signal
      // 3: Background
      if(tRoofit!=-1)
      {
        RoofitT->Fill() ;
        if(tRoofit==2||tRoofit==3)
        {
          if(SignalMode==2&&tRoofit==3)
          {
        tRoofit = (HasDsToMuNu==1) ? 4 : 5 ;
        RoofitT->Fill() ;
          }
          tRoofit = 0 ;
          RoofitT->Fill() ;
        }
      } // RoofitType!=-1
      
    } // MC or data
      } // Event loop
    } // EExtra loop
  } // Entry loop

  // Multiplicity stuff
  multiplicityCanvas.cd() ;
  for(EExtraCut=0 ; EExtraCut<NEExtraCut ; EExtraCut++)
  {
    for(SignalMode=SignalModeStart ; SignalMode<SignalModeEnd+1 ; SignalMode++)
    {
      for(Int_t DataOrMC=0 ; DataOrMC<3 ; DataOrMC++)
      {
    TString printName = TString(TString("multiplicity") + DataOrMCString[DataOrMC]) ;
    hMultiplicity[EExtraCut][SignalMode-SignalModeStart][DataOrMC]->Draw() ;
    PrintCanvas(&multiplicityCanvas, printName) ;
    multiplicityCanvas.Clear() ;
      }
    }
  }
  cout << "Printed multiplicity plots" << endl ;
  
  // Roofit stuff
  RoofitF->Write() ;
  RoofitF->Close() ;
  cout << "Written Roofit file" << endl ;

  // Var stuff
  varCanvas.cd() ;
  for(EExtraCut=0 ; EExtraCut<NEExtraCut ; EExtraCut++)
  {
    for(Int_t var=varStart ; var<varEnd+1 ; var++)
    {
      for(SignalMode=SignalModeStart ; SignalMode<SignalModeEnd+1 ; SignalMode++)
      {
        //if(varSignalMode[var][SignalMode]==0) continue ;
        hVarData[EExtraCut][var][SignalMode-SignalModeStart]->Draw("pe") ;
        TString printName = TString(varName[var] + "_Data") ;
        PrintCanvas(&varCanvas, printName) ;
        varCanvas.Clear() ;
        hVarSignal[EExtraCut][var][SignalMode-SignalModeStart]->Draw() ;
        printName = TString(varName[var] + "_Signal") ;
        PrintCanvas(&varCanvas, printName) ;
        varCanvas.Clear() ;
        for(Int_t Component=NComponents-1 ; Component>-1 ; Component--)
        {
          if(nMC[EExtraCut][var][SignalMode-SignalModeStart][0]>0 && kFALSE)
          {
            hVar[EExtraCut][var][SignalMode-SignalModeStart][Component][0]->Scale(nData[EExtraCut][var][SignalMode-SignalModeStart] /nMC[EExtraCut][var][SignalMode-SignalModeStart][0]) ;
            hVar[EExtraCut][var][SignalMode-SignalModeStart][Component][1]->Scale(nData[EExtraCut][var][SignalMode-SignalModeStart] /nMC[EExtraCut][var][SignalMode-SignalModeStart][1]) ;
          }
          hVar[EExtraCut][var][SignalMode-SignalModeStart][Component][0]->Draw() ;      
          TString printName = TString(varName[var] + "_" + ComponentName[Component]) ;
          PrintCanvas(&varCanvas, printName) ;
          varCanvas.Clear() ;
          varStack[EExtraCut][var][SignalMode][0].Add(hVar[EExtraCut][var][SignalMode-SignalModeStart][Component][0]) ;
          varStack[EExtraCut][var][SignalMode][1].Add(hVar[EExtraCut][var][SignalMode-SignalModeStart][Component][1]) ;
        }
        varStack[EExtraCut][var][SignalMode-SignalModeStart][0].Draw() ;
        varStack[EExtraCut][var][SignalMode-SignalModeStart][0].GetXaxis()->SetTitle(hVar[EExtraCut][var][SignalMode-SignalModeStart][0][0]->GetXaxis()->GetTitle()) ;
        varStack[EExtraCut][var][SignalMode-SignalModeStart][0].GetYaxis()->SetTitle(hVar[EExtraCut][var][SignalMode-SignalModeStart][0][0]->GetYaxis()->GetTitle()) ;
        hVarData[EExtraCut][var][SignalMode-SignalModeStart]->Draw("sames:pe") ;
        varLegend.Draw() ;
        printName = TString(varName[var]) ;
        PrintCanvas(&varCanvas, printName) ;
        varCanvas.Clear() ;
    
        varStack[EExtraCut][var][SignalMode-SignalModeStart][1].Draw() ;
        varStack[EExtraCut][var][SignalMode-SignalModeStart][1].GetXaxis()->SetTitle(hVar[EExtraCut][var][SignalMode-SignalModeStart][0][0]->GetXaxis()->GetTitle()) ;
        varStack[EExtraCut][var][SignalMode-SignalModeStart][1].GetYaxis()->SetTitle(hVar[EExtraCut][var][SignalMode-SignalModeStart][0][0]->GetYaxis()->GetTitle()) ;
        hVarData[EExtraCut][var][SignalMode-SignalModeStart]->Draw("sames:pe") ;
        varLegend.Draw() ;
        printName = TString(TString(varName[var]+"_NoDsBFWeighting")) ;
        PrintCanvas(&varCanvas, printName) ;
        varCanvas.Clear() ;
      }
    }
  }
  if(true)
  {
    ofstream varFout ;
    varFout.open("../AWG90/aidanrc/BAD/note2170/NumeratorVariablesCrosscheck.tex") ;
    for(SignalMode=SignalModeStart ; SignalMode<4 ; SignalMode++)
    {
      varFout<< "\\subsubsection{Variable plots for the $" << SignalModeNameLaTeX[SignalMode] << "$ mode}" << endl ;
      Int_t varSpillover =  3 ;
      Int_t varCounter   =  0 ;
      Int_t varTracker   =  0 ;
      Int_t varBegin     = -1 ;
      Int_t varStop      = -1 ;
      for(Int_t var=varStart ; var<varEnd+1 ; var++)
      {
        if(varSignalMode[var][SignalMode]==0) continue ;
        varCounter++ ;
        if(varCounter==1)
        {
          varBegin = var ;
          varFout << "\\begin{figure}[!bh]" << endl ;
          varFout << "  \\begin{center}" << endl ;
          varFout << "    \\begin{tabular}{ccc}" << endl ;
        }
        EExtraCut = 1 ;
        varFout << "      \\includegraphics[width=0.3\\textwidth]{plots/Numerator/"<<varName[var]<<"_Signal_"<<SignalModeName[SignalMode]<<EExtraCutString[EExtraCut]<<".eps} &"  <<endl ;
        varFout << "      \\includegraphics[width=0.3\\textwidth]{plots/Numerator/"<<varName[var]<<"_"       <<SignalModeName[SignalMode]<<EExtraCutString[EExtraCut]<<".eps} &"   <<endl ;
        varFout << "      \\includegraphics[width=0.3\\textwidth]{plots/Numerator/"<<varName[var]<<"_Data_"  <<SignalModeName[SignalMode]<<EExtraCutString[EExtraCut]<<".eps} \\\\"<<endl ;
        if(varCounter==varSpillover || var==varEnd)
        {
          varSpillover = 4 ;
          varStop = var ;
          varFout << "    \\end{tabular}" << endl ;
          varFout << "    \\caption[Distributions for " ;
          for(Int_t i=varBegin ; i<varStop+1 ; i++)
          {
            if(varSignalMode[i][SignalMode]==1)
            {
              varFout << "$" << varNameLaTeX[i] << "$" ;
              if(i!=varStop) varFout << ", " ;
            }
          }
          varFout << " for the $" << SignalModeNameLaTeX[SignalMode] << "$ mode.]" ;
          varFout << "{Distributions of " ;
          for(Int_t i=varBegin ; i<varStop+1 ; i++)
          {
            if(varSignalMode[i][SignalMode]==1)
            {
              varFout << "$" << varNameLaTeX[i] << "$" ;
              if(i!=varStop) varFout << ", " ;
            }
          }
          varFout << " (top to bottom) for the $" << SignalModeNameLaTeX[SignalMode] << "$ mode for signal MC (left), generic MC (center) and data (right).}" << endl ;
          varFout << "    \\label{fig:Numerator_vars" << SignalModeName[SignalMode] << varTracker << "}" << endl ;
          varFout << "  \\end{center}" << endl ;
          varFout << "\\end{figure}" << endl << endl ;
          varTracker++ ;
          varCounter = 0 ;
        }
      }
      varFout << "\\clearpage" << endl << endl ;
    }
    varFout.close() ;
  }
  cout << "Printed var plots" << endl ;

  // SPMode var stuff
  if(false)
  {
    TString SPModeVarPrefix = TString("../docs/aidanrc/VarsBySPMode/") ;
    ofstream SPModeVarFoutHTML ;
    for(SignalMode=0 ; SignalMode<NSignalMode ; SignalMode++)
    {
      SPModeVarFoutHTML.open(TString(SPModeVarPrefix+"A"+SignalModeName[SignalMode])+".html") ;
      SPModeVarFoutHTML << "<html>\n<head>\n<title>Vars by SPMode</title>\n</head>\n<body>\n<h1>Vars by SPMode</h1>" << endl ;
      SPModeVarFoutHTML << "<h2>" << SignalModeName[SignalMode] << "</h2>" << endl ;
      SPModeVarFoutHTML << "<table><thead><tr><th></th>" << endl ;
      for(Int_t i=0 ; i<NSPModePlots ; i++)
      {
    if(SPSignalMode[i][SignalMode]==1) SPModeVarFoutHTML << "<th>" << SPModePlotsName[i] << "</th>" << endl ;
      }
      SPModeVarFoutHTML << "</tr></thead>\n<tbody>" << endl ;
      for(Int_t var=varStart ; var<varEnd+1 ; var++)
      {
    SPModeVarFoutHTML << "<tr><th>" << varName[var] << "</th>" << endl ;
    for(Int_t i=0 ; i<NSPModePlots ; i++)
    {
      hSPModeVar[SignalMode][i][var]->Draw() ;
      TString PrintName = TString(SPModePlotsName[i]+"_"+varName[var]) ;
      //PrintCanvas(&SPModeVarCanvas,PrintName) ;
      PrintName = TString(PrintName+"_"+SignalModeName[SignalMode]+".png") ;
      SPModeVarCanvas.Print(SPModeVarPrefix+PrintName) ;
      SPModeVarCanvas.Clear() ;
      if(SPSignalMode[i][SignalMode]==1) SPModeVarFoutHTML << "<td><a href=\"" << PrintName << "\"><img src=\"" << PrintName << "\" width=\"123px\" height=\"123px\" alt=\"\" /></a></td>" << endl ;
    }
    SPModeVarFoutHTML << "</tr>" << endl ;
      }
      SPModeVarFoutHTML << "</tbody></table>" << endl ;
      SPModeVarFoutHTML << "</body>\n</html>" << endl ;
      SPModeVarFoutHTML.close() ;
    }
  }
  cout << "Printed SPMode var plots" << endl ;
  
  // PHOTOS stuff
  PHOTOSCanvas.cd() ;
  for(EExtraCut=0 ; EExtraCut<NEExtraCut ; EExtraCut++)
  {
    for(SignalMode=SignalModeStart ; SignalMode<SignalModeEnd+1 ; SignalMode++)
    {
      for(Int_t nGamma=0 ; nGamma<NNGamma ; nGamma++)
      {
    for(Int_t i=0 ; i<3 ; i++)
    {
      hPHOTOS[EExtraCut][SignalMode][nGamma][i]->Draw() ;
      TString PrintName = TString(varName[PHOTOSVar[i]]+"_"+(long)nGamma+"Gamma") ;
      PrintCanvas(&PHOTOSCanvas,PrintName) ;
      PHOTOSCanvas.Clear() ;
    }
      }
    }
  }
  ofstream fPHOTOS ;
  fPHOTOS.open("../AWG90/aidanrc/BAD/note2170/NumeratorPHOTOSCrosscheck.tex") ;
  for(SignalMode=SignalModeStart ; SignalMode<SignalModeEnd+1 ; SignalMode++)
  {
    fPHOTOS << "\\subsubsection{Effects of \\texttt{PHOTOS} on $" << SignalModeNameLaTeX[SignalMode] <<  "$ distributions}" << endl ;
    EExtraCut = 1 ;
    for(Int_t var=0 ; var<3 ; var++)
    {
      fPHOTOS << "\\begin{figure}[!hb]" << endl ;
      fPHOTOS << "  \\begin{center}" << endl ;
      fPHOTOS << "    \\begin{tabular}{cc}" << endl ;
      for(Int_t nGamma=0 ; nGamma<NNGamma ; nGamma++)
      {
    fPHOTOS << "    \\includegraphics[width=0.3\\textwidth]{plots/Numerator/" << varName[PHOTOSVar[var]] << "_" << nGamma <<"Gamma_" << SignalModeName[SignalMode] << EExtraCutString[EExtraCut] << ".eps}" ;
    if(nGamma==1||nGamma==3) { fPHOTOS << "    \\\\" ; }
    else if(nGamma!=5){ fPHOTOS << "    &" ; }
    fPHOTOS << endl ;
      }
      fPHOTOS << "    \\end{tabular}" << endl ;
      fPHOTOS << "  \\end{center}" << endl ;
      fPHOTOS << "  \\label{fig:Numerator_PHOTOS_" << varName[PHOTOSVar[var]] << "_" <<  SignalModeName[SignalMode] << "}" << endl ;
      fPHOTOS << "  \\caption[The effects of \\texttt{PHOTOS} on the $" << varNameLaTeX[PHOTOSVar[var]] << "$ distribution for the $" << SignalModeNameLaTeX[SignalMode] << "$ mode.]{The $" << varNameLaTeX[PHOTOSVar[var]] << "$ distribution for the $" << SignalModeNameLaTeX[SignalMode] << "$ mode where the $D_s^+$ radiates $0\\gamma$ (top left), $1\\gamma$ (top center), $2\\gamma$ (top right), $3\\gamma$ (bottom left), $4\\gamma$ (bottom center) and $5\\gamma$ (bottom right).}" << endl ;
      fPHOTOS << "\\end{figure}" << endl << endl ;
    }
    fPHOTOS << "\\clearpage" << endl << endl ;
  }
  fPHOTOS.close() ;
  cout << "Printed PHOTOS plots" << endl ;
  
  mm2EExtraSlicesCanvas.cd() ;
  ofstream fHTML ;
  fHTML.open("../docs/aidanrc/slices/ASlices.html") ;
  fHTML << "<html><head><title>EExtra slices</title></head><body><h1>EExtra slices</h1>" << endl ;
  for(SignalMode=0 ; SignalMode<NSignalMode-1 ; SignalMode++)
  {
    fHTML << "<h2>" << SignalModeName[SignalMode] << "</h2>"  << endl ;
    for(Int_t slice=0 ; slice<NEExtraSlices ; slice++)
    {
      TString PrintName = TString(TString("EExtraSlice")+(long)slice) ;
      hMm2InEExtra[SignalMode][slice]->Draw() ;
      PrintCanvas(&mm2EExtraSlicesCanvas,PrintName) ;
      mm2EExtraSlicesCanvas.Print(TString(TString("../docs/aidanrc/slices/EExtraSlice")+(long)slice+SignalModeName[SignalMode]+".png")) ;
      fHTML << "<img src=\"EExtraSlice" << slice << SignalModeName[SignalMode] << ".png\" width=\"\" height=\"\" alt=\"\" />" << endl ;
      mm2EExtraSlicesCanvas.Clear() ;
    }
  }
  fHTML << "</body></html>" << endl ;
  fHTML.close() ;
  cout << "Printed mm2 in EExtra slices plots" << endl ;

  for(SignalMode=0 ; SignalMode<NSignalMode-1 ; SignalMode++)
  {
    cout << "nXT weights for " << SignalModeName[SignalMode] << ":" << endl ;
    for(Int_t nXT=0 ; nXT<NNXT ; nXT++)
    {
      cout << "  nXT = " << nXT << " has " << nXTYields[SignalMode][nXT] << " reconstructions" << endl ;
    }
  }
  
  fThesis->Write() ;
  fThesis->Close() ;
  cout << "Written thesis ntuple" << endl ;

  fOut->Write() ;
  fOut->Close() ;
  cout << "Written plots file" << endl ;

  fBF->Write() ;
  fBF->Close() ;
  cout << "Written BF file" << endl ;
  cout << "Processed " << counter << " events" << endl ;
}

void DRecoDsToEllNuPlots::Roofit()
{
  Bool_t            safeMode = kFALSE ;
  Bool_t              doToys = kTRUE  ;
  Bool_t doComponentsAndDsBF = kTRUE ;
  const Int_t NFit    =   22 ;
  const Int_t NPDF    =   23 ;
  const Int_t NRatios =   11 ;
  const Int_t NToys   = 1000 ;
  const Int_t nStat   =    2 ;
  TString weightString = "weightDs" ;

  Int_t SignalModeStart = 0 ;
  Int_t SignalModeEnd   = 3 ;
  Int_t FitStart        = 0*(NFit-1) ;
  Int_t FitEnd          = NFit-1 ;
  Int_t ratioStart      = 0*(NRatios-1)/2 ;
  Int_t ratioEnd        = (NRatios-1) ;
  TCanvas* cRun      = new TCanvas("cRun"     ,"",100,100,1000,500) ;
  TCanvas* cRoofit   = new TCanvas("cRoofit"  ,"",100,100, 500,500) ;

  TFile* fRoofit = new TFile(TString("../AWG90/aidanrc/Roofit/Roofit.root"),"READ") ;
  TTree* tree    = (TTree*) fRoofit->Get("tRoofit") ;
  TFile* fFitResults[NSignalMode] ;
  TFile* fFitResultsData[NSignalMode] ;
  TFile* fPlots ;
  ofstream fBAD ;
  ofstream fBADCommands ;
  ofstream fBADData ;
  if(safeMode==kTRUE)
  {
    for(SignalMode=SignalModeStart ; SignalMode<SignalModeEnd+1 ; SignalMode++)
    {
      fFitResults[SignalMode]     = new TFile(TString(SignalModeName[SignalMode]+"NumeratorFitResults_MC.root")  ,"RECREATE") ;
      fFitResultsData[SignalMode] = new TFile(TString(SignalModeName[SignalMode]+"NumeratorFitResults_Data.root"),"RECREATE") ;
    }
    fPlots          = new TFile(TString("../AWG90/aidanrc/RoofitPlotsTemp.root"),"RECREATE") ;
    fBAD.open("../AWG90/aidanrc/NumeratorRoofitCrosschecks.tex") ;
    fBADCommands.open("../AWG90/aidanrc/RoofitCommands.tex") ;
  }
  else
  {
    for(SignalMode=SignalModeStart ; SignalMode<SignalModeEnd+1 ; SignalMode++)
    {
      fFitResults[SignalMode]     = new TFile(TString(TString("../docs/Denominator/NominalFitResults/")+SignalModeName[SignalMode]+"FitResults_MC.root")  ,"RECREATE") ;
      fFitResultsData[SignalMode] = new TFile(TString(TString("../docs/Denominator/NominalFitResults/")+SignalModeName[SignalMode]+"FitResults_Data.root"),"RECREATE") ;
    }
    fPlots          = new TFile(TString("../AWG90/aidanrc/RoofitPlots.root"),"RECREATE") ;
    fBAD.open("../AWG90/aidanrc/BAD/note2170/NumeratorRoofitCrosschecks.tex") ;
    fBADCommands.open("../AWG90/aidanrc/BAD/note2170/RoofitCommands.tex") ;
  }
  RooFitResult* results[NSignalMode] ;
  RooFitResult* resultsData[NSignalMode] ;
  fPlots->cd() ;
  
  // Declare stuff
  TString FitName[NFit] ;
  FitName[0]  = RunName[0]  ;
  FitName[1]  = RunName[1]  ;
  FitName[2]  = RunName[2]  ;
  FitName[3]  = RunName[3]  ;
  FitName[4]  = RunName[4]  ;
  FitName[5]  = RunName[5]  ;
  FitName[6]  = RunName[6]  ;
  FitName[7]  = RunName[7]  ;
  FitName[8]  = RunName[8]  ;
  FitName[9]  = RunName[9]  ;
  FitName[10] = RunName[10] ;
  FitName[11] = RunName[11] ;
  FitName[12] = RunName[12] ;
  FitName[13] = RunName[13] ;
  FitName[14] = RunName[14] ;
  FitName[15] = RunName[15] ;
  FitName[16] = RunName[16] ;
  FitName[17] = NXRName[0]  ;
  FitName[18] = NXRName[1]  ;
  FitName[19] = NXRName[2]  ;
  FitName[20] = NXRName[3]  ;
  FitName[21] = NXRName[4]  ;
  
  TString PdfTypeName[NPDF] ; TString PdfTypeTitle[NPDF] ;
  PdfTypeName[0]  = "Var"                                ; PdfTypeTitle[0]  = "var"                               ;
  PdfTypeName[1]  = "Signal"                             ; PdfTypeTitle[1]  = "signalFromSignalMC"                ;
  PdfTypeName[2]  = "TrueSignal"                         ; PdfTypeTitle[2]  = "signalFromGenericMC"               ;
  PdfTypeName[3]  = "Background"                         ; PdfTypeTitle[3]  = "backgroundFromGenericMC"           ;
  PdfTypeName[4]  = "DsToMuNuBackground"                 ; PdfTypeTitle[4]  = "DsToMuNuBackgroundFromGenericMC"   ;
  PdfTypeName[5]  = "OtherBackground"                    ; PdfTypeTitle[5]  = "otherBackgroundFromGenericMC"      ;
  PdfTypeName[6]  = "Signal0Gamma"                       ; PdfTypeTitle[6]  = "signal0Gamma"                      ;
  PdfTypeName[7]  = "Signal1Gamma"                       ; PdfTypeTitle[7]  = "signal1Gamma"                      ;
  PdfTypeName[8]  = "SignalMoreGamma"                    ; PdfTypeTitle[8]  = "signalMoreGamma"                   ;
  PdfTypeName[9]  = "Signal1MoreGamma"                   ; PdfTypeTitle[9]  = "signalMoreGamma"                   ;
  PdfTypeName[10] = "Data"                               ; PdfTypeTitle[10] = "Data"                              ;
  PdfTypeName[11] = "backgroundExcludingSemileptonic"    ; PdfTypeTitle[11] = "backgroundExcudingSemileptonic"    ;
  PdfTypeName[12] = "backgroundSemileptonic"             ; PdfTypeTitle[12] = "backgroundSemileptnic"             ;
  PdfTypeName[13] = "backgroundDsToK0EllNu"              ; PdfTypeTitle[13] = "backgroundDsToK0EllNu"             ;
  PdfTypeName[14] = "backgroundDsToPhiEllNu"             ; PdfTypeTitle[14] = "backgroundDsToPhiEllNu"            ;
  PdfTypeName[15] = "backgroundDsToEtaEllNu"             ; PdfTypeTitle[15] = "backgroundDsToEtaEllNu"            ;
  PdfTypeName[16] = "backgroundExcludingSemileptonicAll" ; PdfTypeTitle[16] = "backgroundExcudingSemileptonicAll" ;
  PdfTypeName[17] = "backgroundDsToK0EllNuAll"           ; PdfTypeTitle[17] = "backgroundDsToK0EllNuAll"          ;
  PdfTypeName[18] = "backgroundDsToPhiEllNuAll"          ; PdfTypeTitle[18] = "backgroundDsToPhiEllNuAll"         ;
  PdfTypeName[19] = "backgroundDsToEtaEllNuAll"          ; PdfTypeTitle[19] = "backgroundDsToEtaEllNuAll"         ;
  PdfTypeName[20] = "electron"                           ; PdfTypeTitle[20] = "electron"                          ;
  PdfTypeName[21] = "pion"                               ; PdfTypeTitle[21] = "pion"                              ;
  PdfTypeName[22] = "muonKaonProton"                     ; PdfTypeTitle[22] = "muonKaonProton"                    ;

  // Variables
  const Int_t NVar = 9 ;
  Int_t nDimensions[NSignalMode] = { 1 , 1 , 1 , 1 , 2 , 0 } ;
  Int_t varX[NSignalMode] ; Int_t varY[NSignalMode] ;
  Int_t   varNBins[NVar] ; Float_t varLower[NVar] ; Float_t varUpper[NVar] ;
  TString  varName[NVar] ; TString varTitle[NVar] ; TString varNameLaTeX[NVar] ;
  RooRealVar* rrv[NVar] ;
  varNBins[0] =  80 ; varLower[0] =  -0.5 ; varUpper[0] =   3.5 ;
  varNBins[1] =  15 ; varLower[1] = 1.943 ; varUpper[1] = 2.003 ;
  varNBins[2] = 100 ; varLower[2] =   0.0 ; varUpper[2] =   5.0 ;
  varNBins[3] =  10 ; varLower[3] =   0.0 ; varUpper[3] =   6.0 ;
  varNBins[4] =  60 ; varLower[4] =   0.5 ; varUpper[4] =   3.5 ;
  varNBins[5] =  10 ; varLower[5] =   0.5 ; varUpper[5] =   3.5 ;
  varNBins[6] =  40 ; varLower[6] =   1.0 ; varUpper[6] =   5.0 ;
  varNBins[7] =  40 ; varLower[7] =  -0.5 ; varUpper[7] =   3.5 ;
  varNBins[8] = 100 ; varLower[8] =  0.03 ; varUpper[8] =   5.03 ;
  varName[0] = TString("mm2")    ; varTitle[0] = TString("m_{m}^{2} (GeV^{2})") ; varNameLaTeX[0] = TString("\\mmtwo")          ; // DsToMuNu
  varName[1] = TString("dsmass") ; varTitle[1] = TString("m_{D_{s}} (GeV)")     ; varNameLaTeX[1] = TString("$m_{D_s}$")        ; // None
  varName[2] = TString("eextra") ; varTitle[2] = TString("E_{extra} (GeV)")     ; varNameLaTeX[2] = TString("\\eextra")         ; // DsToTauNu
  varName[3] = TString("p3lab")  ; varTitle[3] = TString("p_{#pi}^{lab} (GeV)") ; varNameLaTeX[3] = TString("\\p_{\\pi}^{lab}") ; // DsToTauNuTauToPiNu alternate fit
  varName[4] = TString("mm2")    ; varTitle[4] = TString("m_{m}^{2} (GeV^{2})") ; varNameLaTeX[4] = TString("\\mmtwo")          ; // DsToTauNuTauToMuNuNu
  varName[5] = TString("mm2")    ; varTitle[5] = TString("m_{m}^{2} (GeV^{2})") ; varNameLaTeX[5] = TString("\\mmtwo")          ; // Sideband comparison
  varName[6] = TString("eextra") ; varTitle[6] = TString("E_{extra} (GeV)")     ; varNameLaTeX[6] = TString("\\eextra")         ; // Sideband comparison
  varName[7] = TString("mm2")    ; varTitle[7] = TString("m_{m}^{2} (GeV^{2})") ; varNameLaTeX[7] = TString("\\mmtwo")          ; // DsToENu
  varName[8] = TString("eextra") ; varTitle[8] = TString("E_{extra} (GeV)")     ; varNameLaTeX[8] = TString("\\eextra")         ; // DsToTauNu
  for(Int_t i=0 ; i<NVar ; i++)
  {
    rrv[i] = new RooRealVar(TString(varName[i]+(long)i),varTitle[i],varLower[i],varUpper[i]) ;
    rrv[i]->setBins(varNBins[i]) ;
  }
  varX[0] = 0 ; varY[0] = 0 ;
  //varX[1] = 7 ; varY[1] = 7 ;
  varX[1] = 0 ; varY[1] = 0 ;
  varX[2] = 2 ; varY[2] = 8 ;
  varX[3] = 2 ; varY[3] = 8 ;
  varX[4] = 2 ; varY[4] = 4 ;

  Int_t pullNBins = 20 ; Float_t pullLower = -2.0 ; Float_t pullUpper = 2.0 ;
  RooRealVar pullVar("pullVar","Pull",pullLower,pullUpper) ; pullVar.setBins(pullNBins) ;
  TH1F* hPull[NSignalMode][NRatios] ;
  
  // Variables for the BAD
  const Int_t NCombinedRuns = (NRuns-1)/2 ;
  TString CombinedRunNameLaTeX[NCombinedRuns] ;
  CombinedRunNameLaTeX[0] = "Run 1" ;
  CombinedRunNameLaTeX[1] = "Run 2" ;
  CombinedRunNameLaTeX[2] = "Run 3" ;
  CombinedRunNameLaTeX[3] = "Run 4" ;
  CombinedRunNameLaTeX[4] = "Run 5" ;
  CombinedRunNameLaTeX[5] = "Run 6" ;
  CombinedRunNameLaTeX[6] = "$\\Upsilon(3S)$" ;
  CombinedRunNameLaTeX[7] = "$\\Upsilon(2S)$" ;
  TString runTitle[NRuns] ;TString runNameLaTeX[NRuns] ;
  runTitle[0]  = "1 (On)"      ; runNameLaTeX[0]  = "Run 1 (OnPeak)"  ;
  runTitle[1]  = "1 (Off)"     ; runNameLaTeX[1]  = "Run 1 (OffPeak)" ;
  runTitle[2]  = "2 (On)"      ; runNameLaTeX[2]  = "Run 2 (OnPeak)"  ;
  runTitle[3]  = "2 (Off)"     ; runNameLaTeX[3]  = "Run 2 (OffPeak)" ;
  runTitle[4]  = "3 (On)"      ; runNameLaTeX[4]  = "Run 3 (OnPeak)"  ;
  runTitle[5]  = "3 (Off)"     ; runNameLaTeX[5]  = "Run 3 (OffPeak)" ;
  runTitle[6]  = "4 (On)"      ; runNameLaTeX[6]  = "Run 4 (OnPeak)"  ;
  runTitle[7]  = "4 (Off)"     ; runNameLaTeX[7]  = "Run 4 (OffPeak)" ;
  runTitle[8]  = "5 (On)"      ; runNameLaTeX[8]  = "Run 5 (OnPeak)"  ;
  runTitle[9]  = "5 (Off)"     ; runNameLaTeX[9]  = "Run 5 (OffPeak)" ;
  runTitle[10] = "6 (On)"      ; runNameLaTeX[10] = "Run 6 (OnPeak)"  ;
  runTitle[11] = "6 (Off)"     ; runNameLaTeX[11] = "Run 6 (OffPeak)" ;
  runTitle[12] = "Y(3S) (On)"  ; runNameLaTeX[12] = "$\\Upsilon(3S)$ (OnPeak)"  ;
  runTitle[13] = "Y(3S) (Off)" ; runNameLaTeX[13] = "$\\Upsilon(3S)$ (OffPeak)" ;
  runTitle[14] = "Y(2S) (On)"  ; runNameLaTeX[14] = "$\\Upsilon(2S)$ (OnPeak)"  ;
  runTitle[15] = "Y(2S) (Off)" ; runNameLaTeX[15] = "$\\Upsilon(2S)$ (OffPeak)" ;
  runTitle[16] = "All"         ; runNameLaTeX[16] = "All runs" ;
  
  TString ratioName[NRatios] ; Float_t ratios[NRatios] ; TString ratioTitle[NRatios] ; TString ratioNameLaTeX[NRatios] ;
  ratioName[0]  = "75percent"  ; ratios[0]  = 0.75 ; ratioTitle[0]  =  "75" ; ratioNameLaTeX[0]  =  "75\\%" ;
  ratioName[1]  = "80percent"  ; ratios[1]  = 0.80 ; ratioTitle[1]  =  "80" ; ratioNameLaTeX[1]  =  "80\\%" ;
  ratioName[2]  = "85percent"  ; ratios[2]  = 0.85 ; ratioTitle[2]  =  "85" ; ratioNameLaTeX[2]  =  "85\\%" ;
  ratioName[3]  = "90percent"  ; ratios[3]  = 0.90 ; ratioTitle[3]  =  "90" ; ratioNameLaTeX[3]  =  "90\\%" ;
  ratioName[4]  = "95percent"  ; ratios[4]  = 0.95 ; ratioTitle[4]  =  "95" ; ratioNameLaTeX[4]  =  "95\\%" ;
  ratioName[5]  = "100percent" ; ratios[5]  = 1.00 ; ratioTitle[5]  = "100" ; ratioNameLaTeX[5]  = "100\\%" ;
  ratioName[6]  = "105percent" ; ratios[6]  = 1.05 ; ratioTitle[6]  = "105" ; ratioNameLaTeX[6]  = "105\\%" ;
  ratioName[7]  = "110percent" ; ratios[7]  = 1.10 ; ratioTitle[7]  = "110" ; ratioNameLaTeX[7]  = "110\\%" ;
  ratioName[8]  = "115percent" ; ratios[8]  = 1.15 ; ratioTitle[8]  = "115" ; ratioNameLaTeX[8]  = "115\\%" ;
  ratioName[9]  = "120percent" ; ratios[9]  = 1.20 ; ratioTitle[9]  = "120" ; ratioNameLaTeX[9]  = "120\\%" ;
  ratioName[10] = "125percent" ; ratios[10] = 1.25 ; ratioTitle[10] = "125" ; ratioNameLaTeX[10] = "125\\%" ;
  TString nXRTitle[NNXR] ; TString nXRNameLaTeX[NNXR] ;
  nXRTitle[0] = "0"   ; nXRNameLaTeX[0] = "$n_X^R=0$" ;
  nXRTitle[1] = "1"   ; nXRNameLaTeX[1] = "$n_X^R=1$" ;
  nXRTitle[2] = "2"   ; nXRNameLaTeX[2] = "$n_X^R=2$" ;
  nXRTitle[3] = "3"   ; nXRNameLaTeX[3] = "$n_X^R=3$" ;
  nXRTitle[4] = "All" ; nXRNameLaTeX[4] = "All $n_X^R$" ;  
  TString EExtraCutSignalMode[NSignalMode] ;
  EExtraCutSignalMode[0] = "&&eextra<1.0" ;
  EExtraCutSignalMode[1] = "&&eextra<1.0" ;
  EExtraCutSignalMode[2] = "&&eextra<5.0" ;
  EExtraCutSignalMode[3] = "&&eextra<5.0" ;
  EExtraCutSignalMode[4] = "&&eextra<5.0" ;
  EExtraCutSignalMode[5] = "&&eextra<5.0" ;
  
  const Int_t NDsType = 16 ;
  Float_t DsTypeError[NDsType] ;
  //Float_t fittedYieldDsType[NSignalMode][NDsType] ;
  TString DsTypeLaTeX[NDsType] ;
  DsTypeLaTeX[0]  = "D_s^+\\to\\eta\\pi"         ; DsTypeError[0]  = 0.21/1.58 ;
  DsTypeLaTeX[1]  = "D_s^+\\to\\eta'\\pi"        ; DsTypeError[1]  =  0.4/3.8  ;
  DsTypeLaTeX[2]  = "D_s^+\\to\\omega\\pi"       ; DsTypeError[2]  =  0.9/2.5  ;
  DsTypeLaTeX[3]  = "D_s^+\\to f_0\\pi"          ; DsTypeError[3]  =  1.1/9.7  ;
  DsTypeLaTeX[4]  = "D_s^+\\to f_2\\pi"          ; DsTypeError[4]  =  0.6/1.1  ;
  DsTypeLaTeX[5]  = "D_s^+\\to\\pi\\pi\\pi"      ; DsTypeError[5]  = 0.08/1.11 ;
  DsTypeLaTeX[6]  = "D_s^+\\to K^{\\star 0}K"    ; DsTypeError[6]  =  0.4/2.6  ;
  DsTypeLaTeX[7]  = "D_s^+\\to K^0K"             ; DsTypeError[7]  = 0.09/1.49 ;
  DsTypeLaTeX[8]  = "D_s^+\\to \\eta'K"          ; DsTypeError[8]  = 0.31/1.41 ;
  DsTypeLaTeX[9]  = "D_s^+\\to K^0\\pi"          ; DsTypeError[9]  = 0.15/1.25 ;
  DsTypeLaTeX[10] = "D_s^+\\to\\rho^0\\pi"       ; DsTypeError[10] =  0.0/1.0  ;
  DsTypeLaTeX[11] = "D_s^+\\to K^{\\star 0}\\pi" ; DsTypeError[11] = 0.26/1.50 ;
  DsTypeLaTeX[12] = "D_s^+\\to KKK"              ; DsTypeError[12] =  1.7/4.9  ;
  DsTypeLaTeX[13] = "D_s^+\\to K^0K\\pi\\pi"     ; DsTypeError[13] = 0.12/1.64 ;
  DsTypeLaTeX[14] = "D_s^+\\to KK\\pi\\pi\\pi"   ; DsTypeError[14] =  1.6/8.8  ;
  DsTypeLaTeX[15] = "D_s^+\\to K^0 \\pi"         ; DsTypeError[15] = 0.15/1.25 ;

  Float_t Chi2Unblind[NSignalMode] ;
  Int_t   NDoFUnblind[NSignalMode] ;  
  const Int_t NComponent = 13 ;
  // Values to be saved
  Float_t                           dataYield[NSignalMode][NFit] ;
  Float_t                      dataYieldError[NSignalMode][NFit] ;
  Float_t                           trueYield[NSignalMode][NRatios][NFit] ;
  Float_t                         fittedYield[NSignalMode][NRatios][NFit] ;
  Float_t                    fittedYieldError[NSignalMode][NRatios][NFit] ;
  Float_t                         totalEvents[NSignalMode] ;
  Float_t                     backgroundYield[NSignalMode] ;
  Float_t                backgroundYieldError[NSignalMode] ;
  Float_t                   fittedYieldDsBFMC[NSignalMode][NDsType][2] ;
  Float_t              fittedYieldErrorDsBFMC[NSignalMode][NDsType][2] ;
  Float_t                 fittedYieldDsBFData[NSignalMode][NDsType][2] ;
  Float_t            fittedYieldErrorDsBFData[NSignalMode][NDsType][2] ;
  Float_t                            pullMean[NSignalMode][NRatios] ;
  Float_t                       pullMeanError[NSignalMode][NRatios] ;
  Float_t                           pullSigma[NSignalMode][NRatios] ;
  Float_t                      pullSigmaError[NSignalMode][NRatios] ;
  Int_t                          pullIntegral[NSignalMode][NRatios] ;
  Float_t                       DsToMuNuYield[NRatios] ;
  Float_t                  DsToMuNuYieldError[NRatios] ;
  Float_t              fittedYieldComponentMC[NSignalMode][NComponent][2] ;
  Float_t         fittedYieldErrorComponentMC[NSignalMode][NComponent][2] ;
  Float_t            fittedYieldComponentData[NSignalMode][NComponent][2] ;
  Float_t       fittedYieldErrorComponentData[NSignalMode][NComponent][2] ;
  Float_t                  fittedYieldShiftMC[NSignalMode] ;
  Float_t             fittedYieldErrorShiftMC[NSignalMode] ;
  Float_t                fittedYieldShiftData[NSignalMode] ;
  Float_t           fittedYieldErrorShiftData[NSignalMode] ;
  Float_t                  fittedYieldSmearMC[NSignalMode] ;
  Float_t             fittedYieldErrorSmearMC[NSignalMode] ;
  Float_t                fittedYieldSmearData[NSignalMode] ;
  Float_t           fittedYieldErrorSmearData[NSignalMode] ;
  Float_t            fittedYieldExponentialMC[NSignalMode] ;
  Float_t       fittedYieldErrorExponentialMC[NSignalMode] ;
  Float_t          fittedYieldExponentialData[NSignalMode] ;
  Float_t     fittedYieldErrorExponentialData[NSignalMode] ;
  Float_t              componentUncertaintyMC[NSignalMode] ;
  Float_t                   DsBFUncertaintyMC[NSignalMode] ;
  Float_t   backgroundModelStatisticalErrorMC[NSignalMode] ;
  Float_t            componentUncertaintyData[NSignalMode] ;
  Float_t                 DsBFUncertaintyData[NSignalMode] ;
  Float_t backgroundModelStatisticalErrorData[NSignalMode] ;
  Float_t                         statYieldMC[NSignalMode][nStat] ;
  Float_t                       statYieldData[NSignalMode][nStat] ;
  Float_t                             fitChi2[NSignalMode] ;
  Float_t                             fitNDoF[NSignalMode] ;
  
  Float_t DsToENuToyYield   = 0.0 ;
  Float_t nDsToEllNuToyPass = 0.0 ;

  // mm2 shape parameters
  Float_t       mm2Mean[NSignalMode] ;
  Float_t  mm2MeanError[NSignalMode] ;
  Float_t      mm2Sigma[NSignalMode] ;
  Float_t mm2SigmaError[NSignalMode] ;
  for(SignalMode=0 ; SignalMode<NSignalMode ; SignalMode++)
  {
    totalEvents[SignalMode]                     = 0 ;
    backgroundYield[SignalMode]                 = 0 ;
    backgroundYieldError[SignalMode]            = 0 ;
    mm2Mean[NSignalMode]                        = 0 ;
    mm2MeanError[NSignalMode]                   = 0 ;
    mm2Sigma[NSignalMode]                       = 0 ;
    mm2SigmaError[NSignalMode]                  = 0 ;
    fittedYieldShiftMC[SignalMode]              = 0 ;
    fittedYieldErrorShiftMC[SignalMode]         = 1 ;
    fittedYieldShiftData[SignalMode]            = 0 ;
    fittedYieldErrorShiftData[SignalMode]       = 1 ;
    fittedYieldSmearMC[SignalMode]              = 0 ;
    fittedYieldErrorSmearMC[SignalMode]         = 1 ;
    fittedYieldSmearData[SignalMode]            = 0 ;
    fittedYieldErrorSmearData[SignalMode]       = 1 ;
    fittedYieldExponentialMC[SignalMode]        = 0 ;
    fittedYieldErrorExponentialMC[SignalMode]   = 1 ;
    fittedYieldExponentialData[SignalMode]      = 0 ;
    fittedYieldErrorExponentialData[SignalMode] = 1 ;
    componentUncertaintyMC[SignalMode]          = 0 ;
    DsBFUncertaintyMC[SignalMode]               = 0 ;
    componentUncertaintyData[SignalMode]        = 0 ;
    DsBFUncertaintyData[SignalMode]             = 0 ;
    fitChi2[SignalMode]                         = 0 ;
    fitNDoF[SignalMode]                         = 0 ;

    for(Int_t ratio=0 ; ratio<NRatios ; ratio++)
    {
      DsToMuNuYield[ratio]              = 0 ;
      DsToMuNuYieldError[ratio]         = 1 ;
      pullMean[SignalMode][ratio]       = 0 ;
      pullMeanError[SignalMode][ratio]  = 1 ;
      pullSigma[SignalMode][ratio]      = 0 ;
      pullSigmaError[SignalMode][ratio] = 1 ;
      pullIntegral[SignalMode][ratio]   = 0 ;
      for(Int_t fit=0 ; fit<NFit ; fit++)
      {
    trueYield[SignalMode][ratio][fit]        = 0 ;
    fittedYield[SignalMode][ratio][fit]      = 0 ;
    fittedYieldError[SignalMode][ratio][fit] = 1 ;
    dataYield[SignalMode][fit]               = 0 ;
    dataYieldError[SignalMode][fit]          = 1 ;
      }
    }
    for(Int_t DsBF=0 ; DsBF<NDsType ; DsBF++)
    {
      fittedYieldDsBFMC[SignalMode][DsBF][0]        = 0 ;
      fittedYieldDsBFMC[SignalMode][DsBF][1]        = 0 ;
      fittedYieldErrorDsBFMC[SignalMode][DsBF][0]   = 1 ;
      fittedYieldErrorDsBFMC[SignalMode][DsBF][1]   = 1 ;
      fittedYieldDsBFData[SignalMode][DsBF][0]      = 0 ;
      fittedYieldDsBFData[SignalMode][DsBF][1]      = 0 ;
      fittedYieldErrorDsBFData[SignalMode][DsBF][0] = 1 ;
      fittedYieldErrorDsBFData[SignalMode][DsBF][1] = 1 ;
    }
    for(Int_t component=0 ; component<NComponent ; component++)
    {
      fittedYieldComponentMC[SignalMode][component][0]        = 0 ;
      fittedYieldComponentMC[SignalMode][component][1]        = 0 ;
      fittedYieldErrorComponentMC[SignalMode][component][0]   = 1 ;
      fittedYieldErrorComponentMC[SignalMode][component][1]   = 1 ;
      fittedYieldComponentData[SignalMode][component][0]      = 0 ;
      fittedYieldComponentData[SignalMode][component][1]      = 0 ;
      fittedYieldErrorComponentData[SignalMode][component][0] = 1 ;
      fittedYieldErrorComponentData[SignalMode][component][1] = 1 ;
    }
  }  
  
  // Histograms
  TH1I*   hTrueRunSummary[NSignalMode] ;
  TH1I*    hFitRunSummary[NSignalMode] ;
  TH1I*   hLumiRunSummary[NSignalMode] ;
  TH1I*   hAxisRunSummary[NSignalMode] ;
  TH1I*   hTrueNXRSummary[NSignalMode] ;
  TH1I*    hFitNXRSummary[NSignalMode] ;
  TH1I*   hAxisNXRSummary[NSignalMode] ;
  TH1I* hTrueRatioSummary[NSignalMode] ;
  TH1I*  hFitRatioSummary[NSignalMode] ;
  TH1I* hAxisRatioSummary[NSignalMode] ;
  
  // Filenames for various plots
  TString       RoofitDataPrintNameX[NSignalMode][NFit] ;
  TString           RoofitPrintNameX[NSignalMode][NFit][NRatios] ;
  TString           RoofitPrintNameY[NSignalMode][NFit][NRatios] ;
  //TString             DsBFPrintNameX[NSignalMode] ;
  //TString             DsBFPrintNameY[NSignalMode] ;
  TString              PullPrintName[NSignalMode][NRatios] ;
  TString               ToyPrintName[NSignalMode][NRatios] ;
  TString        RunSummaryPrintName[NSignalMode] ;
  TString        NXRSummaryPrintName[NSignalMode] ;
  TString      RatioSummaryPrintName[NSignalMode] ;
  TString SignalPDFGaussianPrintName[NSignalMode] ;
  TString             shiftPrintName[NSignalMode] ;
  TString             smearPrintName[NSignalMode] ;
  TString         shiftPrintNameData[NSignalMode] ;
  TString         smearPrintNameData[NSignalMode] ;
  TString       exponentialPrintName[NSignalMode] ;
  TString   exponentialPrintNameData[NSignalMode] ;

  cRoofit->cd() ;
  for(SignalMode=SignalModeStart ; SignalMode<SignalModeEnd+1 ; SignalMode++)
  {
    EExtraCut = 1 ;

    for(Int_t fit=FitStart ; fit<FitEnd+1 ; fit++)
    {
      TH1F*            h1PDF[NPDF] ;
      TH2F*            h2PDF[NPDF] ;
      RooDataHist*    rdhPDF[NPDF] ;
      RooHistPdf*     rhpPDF[NPDF] ;
      RooRealVar*       fPDF[NPDF] ;
      DRecoHistPdf*  drhpPDF[NPDF] ;
      RooArgSet set ;
      Float_t integral[NPDF] ;
      
      // Get pdfs
      for(Int_t pdf=0 ; pdf<NPDF ; pdf++)
      {
        h1PDF[pdf]    = NULL ;
        h2PDF[pdf]    = NULL ;
        rdhPDF[pdf]   = NULL ;
        rhpPDF[pdf]   = NULL ;
        fPDF[pdf]     = NULL ;
        integral[pdf] =    0 ;
        TString suffix = TString(PdfTypeName[pdf]+FitName[fit]+SignalModeName[SignalMode]+EExtraCutString[EExtraCut]) ;
        TString    histogramName = TString(TString("h")   +suffix) ;
        TString  rooDataHistName = TString(TString("rdh") +suffix) ;
        TString   rooHistPdfName = TString(TString("rhp") +suffix) ;
        TString drecoHistPdfName = TString(TString("drhp")+suffix) ;
        TString            fName = TString(TString("f")   +suffix) ;
        TString fitCut = "" ;
        if(fit<NRuns-1)                         { fitCut = TString(TString("&&run==")+(long)fit) ;         }
        else if(fit>NRuns-1 && fit<NRuns+NNXR-1){ fitCut = TString(TString("&&nxr==")+(long)(fit-NRuns)) ; }
        TString blindCut = TString("0.4") ;
        TString pdfCut ;
        switch(pdf)
        {
        case 6:  pdfCut = TString("&&type==1&&nPhotons==0") ; break ;
        case 7:  pdfCut = TString("&&type==1&&nPhotons==1") ; break ;
        case 8:  pdfCut = TString("&&type==1&&nPhotons>1")  ; break ;
        case 9:  pdfCut = TString("&&type==1&&nPhotons>0")  ; break ;
        case 11: pdfCut = TString(TString("&&modeNumber!=6&&mm2>") + blindCut) ; break ; // Not semileptonic sideband
        case 12: pdfCut = TString(TString("&&modeNumber==6&&type==3&&mm2>") + blindCut) ; break ; // Semileptonic sideband
        case 13: pdfCut = TString(TString("&&type==100&&(SPMode==(17+(SignalMode%2)*2)||SPMode==(18+(SignalMode%2)*2))&&mm2>") + blindCut) ; break ; // DsToK0EllNu sideband
        case 14: pdfCut = TString(TString("&&type==100&&(SPMode==(21+(SignalMode%2)*2)||SPMode==(22+(SignalMode%2)*2))&&mm2>") + blindCut) ; break ; // DsToPhiEllNu sideband
        case 15: pdfCut = TString(TString("&&type==100&&(SPMode==(25+(SignalMode%2)*2)||SPMode==(26+(SignalMode%2)*2))&&mm2>") + blindCut) ; break ; // DsToEtaEllNu sideband
        case 16: pdfCut = TString("&&modeNumber!=6&&type==3") ; break ; // Not semileptonic all sideband
        case 17: pdfCut = TString("&&type==100&&(SPMode==(17+(SignalMode%2)*2)||SPMode==(18+(SignalMode%2)*2))") ; break ; // DsToK0EllNu sideband
        case 18: pdfCut = TString("&&type==100&&(SPMode==(21+(SignalMode%2)*2)||SPMode==(22+(SignalMode%2)*2))") ; break ;// DsToPhiEllNu sideband
        case 19: pdfCut = TString("&&type==100&&(SPMode==(25+(SignalMode%2)*2)||SPMode==(26+(SignalMode%2)*2))") ; break ;// DsToEtaEllNu sideband
        case 20: pdfCut = TString("&&type==3&&abs(lund)==11" )   ; break ; // Electron
        case 21: pdfCut = TString("&&type==3&&abs(lund)==211")   ; break ; // Pion
        case 22: pdfCut = TString("&&type==3&&abs(lund)!=11" )   ; break ; // Muon, Kaon, Proton
        default: pdfCut = TString(TString("&&type==")+(long)pdf) ; break ;
        }
    
        TString theWeight = (pdf==10) ? TString("weight") : weightString ;
        TString cut = TString(theWeight+"*(SignalMode=="+(long)(SignalMode)+pdfCut+fitCut+EExtraCutSignalMode[SignalMode]+")") ;
        if(SignalMode==2&&pdf==4) cut = TString(theWeight+"*(SignalMode==0&&type==1"+fitCut+EExtraCutSignalMode[SignalMode]+")") ;
        set.add(*rrv[varX[SignalMode]]) ;
        h1PDF[pdf]  = new TH1F(histogramName,histogramName,varNBins[varX[SignalMode]],varLower[varX[SignalMode]],varUpper[varX[SignalMode]]) ;
        tree->Draw(TString(varName[varX[SignalMode]])+">>"+histogramName,cut,"goff") ;
        //cout << pdf << " " << h1PDF[pdf]->GetSumOfWeights() << " " << cut << endl ;
        integral[pdf] = h1PDF[pdf]->GetSumOfWeights() ;
        rdhPDF[pdf]   = new   RooDataHist( rooDataHistName, rooDataHistName,set,  h1PDF[pdf]) ;
        rhpPDF[pdf]   = new    RooHistPdf(  rooHistPdfName,  rooHistPdfName,set,*rdhPDF[pdf]) ;
        drhpPDF[pdf]  = new  DRecoHistPdf(drecoHistPdfName,drecoHistPdfName,set,*rdhPDF[pdf],0) ;
        fPDF[pdf] = (SignalMode==1 && pdf==1) ? new RooRealVar(fName, fName, 0.15, -1.0, 1.0) : new RooRealVar(fName, fName, 0.15, 0.0, 1.0) ;
      }

      TString semileptonicHistogramName    = TString(TString(   "hSemileptonic")+FitName[fit]+SignalModeName[SignalMode]+EExtraCutString[EExtraCut]) ;
      TString semileptonicHistogram2Name   = TString(TString(  "h2Semileptonic")+FitName[fit]+SignalModeName[SignalMode]+EExtraCutString[EExtraCut]) ;
      TString semileptonicRooDataHistName  = TString(TString( "rdhSemileptonic")+FitName[fit]+SignalModeName[SignalMode]+EExtraCutString[EExtraCut]) ;
      TString semileptonicRooHistPdfName   = TString(TString( "rhpSemileptonic")+FitName[fit]+SignalModeName[SignalMode]+EExtraCutString[EExtraCut]) ;
      TString semileptonicDRecoHistPdfName = TString(TString("rrhpSemileptonic")+FitName[fit]+SignalModeName[SignalMode]+EExtraCutString[EExtraCut]) ;
      TH1F hSemileptonic(semileptonicHistogramName,semileptonicHistogramName,varNBins[varX[SignalMode]],varLower[varX[SignalMode]],varUpper[varX[SignalMode]]) ;
      TH2F h2Semileptonic(semileptonicHistogram2Name,semileptonicHistogram2Name,varNBins[varX[SignalMode]],varLower[varX[SignalMode]],varUpper[varX[SignalMode]],varNBins[varY[SignalMode]],varLower[varY[SignalMode]],varUpper[varY[SignalMode]]) ;
      RooDataHist* rdhSemileptonic ;
      // Get weights from DECAY.DEC
      hSemileptonic.Add(h1PDF[17], 2.7/57.6) ;
      hSemileptonic.Add(h1PDF[18],24.2/57.6) ;
      hSemileptonic.Add(h1PDF[19],30.7/57.6) ;
      rdhSemileptonic = new RooDataHist(semileptonicRooDataHistName ,semileptonicRooDataHistName ,set, &hSemileptonic) ;
      RooHistPdf    rhpSemileptonic(semileptonicRooHistPdfName  ,semileptonicRooHistPdfName  ,set,*rdhSemileptonic) ;
      DRecoHistPdf drhpSemileptonic(semileptonicDRecoHistPdfName,semileptonicDRecoHistPdfName,set,*rdhSemileptonic) ;
      
      for(Int_t ratio=ratioStart ; ratio<ratioEnd+1 ; ratio++)
      {
        if(fit<NRuns-1&&ratio!=(NRatios-1)/2) continue ;
        // Make dataset
        TH1F* h1Dataset = NULL ;
        RooDataHist* rdhDataset ;
        RooHistPdf*  rhpDataset ;
        Float_t totalYield = 0 ;
        h1Dataset = (TH1F*)h1PDF[3]->Clone(TString(TString("h1Dataset")+ratioName[ratio])) ;
        h1Dataset->Add(h1PDF[2],ratios[ratio]) ;
        rdhDataset = new RooDataHist(TString("rdhDataset"),TString("rdhDataset"),*rrv[varX[SignalMode]],h1Dataset) ;
        rhpDataset = new  RooHistPdf(TString("rhpDataset"),TString("rhpDataset"),*rrv[varX[SignalMode]],*rdhDataset) ;
        totalYield = h1Dataset->GetSumOfWeights() ;

        // Create model and fit to dataset
        RooRealVar cheb0("cheb0", "cheb0", 0.0 , -10.0, 10.0) ;
        RooRealVar cheb1("cheb1", "cheb1", 0.0 , -10.0, 10.0) ;
        RooRealVar cheb2("cheb2", "cheb2", 0.0 , -10.0, 10.0) ;
        RooChebychev linear("linear", "cheb", *rrv[varX[3]], RooArgList(cheb0, cheb1)) ; //, cheb2));
        //RooPolynomial linear("linear","",*rrv[varX[3]],RooArgList(linearComponent,constantComponent),2)
        RooRealVar fLinear("fLinear","",0.0,1.0) ;
        RooArgList* pdfs      = NULL ;
        RooArgList* DRecoPdfs = NULL ;
        RooArgList* fractions = NULL ;
        RooRealVar fDsToMuNu("fDsToMuNu","fDsToMuNu",0.0,0.0,1.0) ;
        if(SignalMode==2 && kFALSE)
        {
          pdfs      = new RooArgList(*rhpPDF[1],*rhpPDF[4], *rhpPDF[5]) ;
          DRecoPdfs = new RooArgList(*drhpPDF[1],*drhpPDF[4],*drhpPDF[5]) ;
          fractions = new RooArgList(  *fPDF[1],  *fPDF[4]) ;
        }
        else if(SignalMode==3 && kFALSE)
        {
          pdfs      = new RooArgList(*rhpPDF[1],*rhpPDF[3],linear) ;
          DRecoPdfs = new RooArgList(*drhpPDF[1],*drhpPDF[3]) ;
          fractions = new RooArgList(*fPDF[1],fLinear) ;
        }
        else
        {
          pdfs      = new RooArgList(*rhpPDF[1],*rhpPDF[3]) ;
          DRecoPdfs = new RooArgList(*drhpPDF[1],*drhpPDF[3]) ;
          fractions = new RooArgList(*fPDF[1]) ;
          //pdfs      = new RooArgList(*rhpPDF[1],*rhpPDF[16],rhpSemileptonic) ;
          //fractions = new RooArgList(*fPDF[1],*fPDF[11]) ;
        }
        //RooArgList pdfs(*rhpPDF[2],*rhpPDF[3]) ; // Use signal from generic MC (Should get almost exactly right, even with a poor choice of fit variables!)
        DRecoAddPdf* DRecoModel ;
        DRecoAddPdf* DRecoModelData ;
        RooAddPdf*   model ;
        RooAddPdf*   modelData ;
        if(false)
        {
          DRecoModel     = new DRecoAddPdf("DRecoModelMC"  ,"Model_MC"  ,set,*DRecoPdfs,*fractions) ;
          DRecoModelData = new DRecoAddPdf("DRecoModelData","Model_Data",set,RooArgList(*drhpPDF[1],*drhpPDF[3]),RooArgList(*fPDF[10])) ;
          model          = new   RooAddPdf(     "modelMC"  ,"Model MC"  ,*pdfs,*fractions) ;
          modelData      = new   RooAddPdf(     "modelData","Model Data",RooArgList(*rhpPDF[1],*rhpPDF[3]),RooArgList(*fPDF[10])) ;
        }
        else if(SignalMode==1 && kFALSE)
        {
          DRecoModel     = new DRecoAddPdf("DRecoModelMC"  ,"Model MC"  , set,*DRecoPdfs,*fractions) ;
          DRecoModelData = new DRecoAddPdf("DRecoModelData","Model Data", set,RooArgList(*drhpPDF[1],drhpSemileptonic,*drhpPDF[16]),RooArgList(*fPDF[10],*fPDF[12])) ;
          model          = new   RooAddPdf(     "modelMC"  ,"Model MC"  , RooArgList(*rhpPDF[1],*rhpPDF[20],*rhpPDF[21],*rhpPDF[22]),RooArgList(*fPDF[1] ,*fPDF[20],*fPDF[21])) ;
          modelData      = new   RooAddPdf(     "modelData","Model Data", RooArgList(*rhpPDF[1],linear,*rhpPDF[20],*rhpPDF[21],*rhpPDF[22]), RooArgList(*fPDF[10],fLinear,*fPDF[20],*fPDF[21])) ;
        }
        else if(SignalMode==3 && kFALSE)
        {
          DRecoModel     = new DRecoAddPdf("DRecoModelMC"  ,"Model MC"  ,set,*DRecoPdfs,*fractions) ;
          DRecoModelData = new DRecoAddPdf("DRecoModelData","Model Data",set,RooArgList(*drhpPDF[1],drhpSemileptonic,*drhpPDF[16]),RooArgList(*fPDF[10],*fPDF[12])) ;
          model          = new   RooAddPdf(     "modelMC"  ,"Model MC"  ,*pdfs,*fractions) ;
          modelData      = new   RooAddPdf(     "modelData","Model Data",RooArgList(*rhpPDF[1],linear,rhpSemileptonic,*rhpPDF[16]),RooArgList(*fPDF[10],*fPDF[12])) ;     
        }
        else
        {
          DRecoModel     = new DRecoAddPdf("DRecoModelMC"  ,"Model MC"  ,set,*DRecoPdfs,*fractions) ;
          DRecoModelData = new DRecoAddPdf("DRecoModelData","Model Data",set,RooArgList(*drhpPDF[1],drhpSemileptonic,*drhpPDF[16]),RooArgList(*fPDF[10],*fPDF[12])) ;
          model          = new   RooAddPdf(     "modelMC"  ,"Model MC"  ,*pdfs,*fractions) ;
          modelData      = new   RooAddPdf(     "modelData","Model Data",RooArgList(*rhpPDF[1],rhpSemileptonic,*rhpPDF[16]),RooArgList(*fPDF[10],*fPDF[12])) ;
        }
        Float_t fractionSemileptonic         = 0 ;
        Float_t fractionErrorSemileptonic    = 1 ;
        Float_t fractionNotSemileptonic      = 0 ;
        Float_t fractionErrorNotSemileptonic = 1 ;
        // Perform fit to data
        if(ratio==(NRatios-1)/2)
        {
          // Taken form DRecoDsInclusiveAnalysis.cc
          // Create chisquared
          DRecoHistPdfChi2Var chi2Data("chi2Data","chi2Data",*DRecoModelData,*rdhPDF[10]) ;
          chi2Data.SetEType(RooAbsData::SumW2) ;
          // Minimize
          RooMinuit minuit(chi2Data) ;
          minuit.migrad() ;
          Float_t backgroundError = (SignalMode==1) ? fPDF[10]->getError() : fPDF[10]->getError()/fPDF[10]->getVal() ;
          cout << endl << endl << endl << fPDF[10]->getVal() <<  " +/- " << fPDF[10]->getError() << endl ;
          modelData->fitTo(*rdhPDF[10]) ;
          backgroundModelStatisticalErrorData[SignalMode] = (SignalMode==1) ? sqrt(fabs(backgroundError*backgroundError-fPDF[10]->getError()*fPDF[10]->getError()))*integral[10] : sqrt(backgroundError*backgroundError - (fPDF[10]->getError()/fPDF[10]->getVal())*(fPDF[10]->getError()/fPDF[10]->getVal())) ;
          cout << "Background model statistical error: " << backgroundError <<  "  " << fPDF[10]->getError() << " " << backgroundModelStatisticalErrorData[SignalMode] << endl ;
          
          cout << backgroundError << " " << fPDF[10]->getError()/fPDF[10]->getVal() << " " << backgroundModelStatisticalErrorData[SignalMode] << endl << endl << endl ;
          Bool_t success = (fPDF[10]->getVal()>fPDF[10]->getError()) ;
          dataYield[SignalMode][fit]      = fPDF[10]->getVal()*integral[10] ;
          dataYieldError[SignalMode][fit] = fPDF[10]->getError()*integral[10] ;
      
          TH1F* hFit = (TH1F*)modelData->createHistogram("hFit",*rrv[varX[SignalMode]]) ;
          hFit->Scale(h1PDF[10]->GetSumOfWeights()/hFit->GetSumOfWeights()) ;
          RoofitDataPrintNameX[SignalMode][fit] = TString(TString("RoofitData_") + varName[varX[SignalMode]] + "_" +FitName[fit]) ;
      
          Float_t residuals = 0 ;
          for(Int_t bin=1 ; bin<h1PDF[10]->GetNbinsX()+1 ; bin++)
          {
            Float_t difference = hFit->GetBinContent(bin)-h1PDF[10]->GetBinContent(bin) ;
            Float_t error      = hFit->GetBinError(bin)*hFit->GetBinError(bin)+h1PDF[10]->GetBinError(bin)*h1PDF[10]->GetBinError(bin) ;
            residuals += (error==0) ? 0 : difference*difference/error ;
          }
          fitChi2[SignalMode] = residuals ;
          fitNDoF[SignalMode] = h1PDF[10]->GetNbinsX()-1 ;
          cout << SignalModeName[SignalMode] << "Data chi2/n: " << FloatToString(fitChi2[SignalMode],2) << " / " << FloatToString(fitNDoF[SignalMode],2) << endl ;
          MakeResidualsPlot(rrv[varX[SignalMode]], *modelData, rdhPDF[10], NULL, rhpPDF[1], 0, SignalModeColor[SignalMode], h1PDF[10], hFit, success, cRoofit, RoofitDataPrintNameX[SignalMode][fit], kTRUE) ;
      
          for(Int_t bin=1 ; kFALSE && bin<h1PDF[10]->GetNbinsX()+1 ; bin++)
          {
            cout << bin << " " 
             << FloatToString(h1PDF[1]->GetBinContent(bin),3)     << "     " 
             << FloatToString(h1PDF[16]->GetBinContent(bin),3)    << "     " 
             << FloatToString(hSemileptonic.GetBinContent(bin),3) << "     " 
             << FloatToString(h1PDF[10]->GetBinContent(bin),3)    << "     " 
             << FloatToString(hFit->GetBinContent(bin),3)         << endl ;
          }
          cout << fPDF[10]->getVal() << " " << fPDF[10]->getError() << endl << endl << endl ;
      
          fractionSemileptonic         = (SignalMode<2) ? fPDF[12]->getVal()   : 0.134 ;
          fractionErrorSemileptonic    = (SignalMode<2) ? fPDF[12]->getError() :  0.01 ;
          fractionNotSemileptonic      = 1-(fractionSemileptonic+fPDF[1]->getVal()) ;
          fractionErrorNotSemileptonic = sqrt(fPDF[1]->getError()*fPDF[1]->getError()+fractionSemileptonic*fractionSemileptonic) ;
          delete hFit ;
        }
    
        cout << endl << endl << "Semileptonic check: " << fPDF[12]->getVal()*integral[10] << " " << integral[3]-integral[16] << endl << endl ;

        if(fit==NRuns-1 && ratio==(NRatios-1)/2)
        {
          fFitResults[SignalMode]->cd() ;
          TString histogramName = TString(SignalModeName[SignalMode]+"_"+FitName[fit]+EExtraCutString[EExtraCut]) ;
          TH1F* hFitOut = (TH1F*)model->createHistogram(TString(histogramName+"Temp"),*rrv[varX[SignalMode]])->Clone(histogramName) ;
          hFitOut->Scale(totalYield) ;
          hFitOut->Write() ;
          results[SignalMode] = model->fitTo(*rdhDataset,"r") ;
          TString resultsName = TString(TString("fitResults") + SignalModeName[SignalMode] + FitName[fit]) ;
          results[SignalMode]->SetName(resultsName) ;
          results[SignalMode]->Write() ;
          fPlots->cd() ;

          fFitResultsData[SignalMode]->cd() ;
          TString histogramDataName = TString(SignalModeName[SignalMode]+"_Data_"+FitName[fit]+EExtraCutString[EExtraCut]) ;
          TH1F* hDataFitOut = (TH1F*)modelData->createHistogram(TString(histogramDataName+"Temp"),*rrv[varX[SignalMode]])->Clone(histogramDataName) ;
          hDataFitOut->Scale(integral[10]) ;
          hDataFitOut->Write() ;
          resultsData[SignalMode] = modelData->fitTo(*rdhPDF[10],"r") ;
          TString resultsDataName = TString(TString("fitResultsData") + SignalModeName[SignalMode] + FitName[fit]) ;
          resultsData[SignalMode]->SetName(resultsDataName) ;
          resultsData[SignalMode]->Write() ;
          fPlots->cd() ;
        }
    
        // Taken form DRecoDsInclusiveAnalysis.cc
        // Create chisquared
        DRecoHistPdfChi2Var chi2("chi2","chi2",*DRecoModel,*rdhPDF[0]) ;
        chi2.SetEType(RooAbsData::SumW2);
        // Minimize
        RooMinuit minuit(chi2);
        minuit.migrad();      
        Float_t backgroundError = fPDF[1]->getError()/fPDF[1]->getVal() ;
        cout << endl << endl << endl << fPDF[1]->getVal() <<  " +/- " << fPDF[1]->getError() << endl ;
        model->fitTo(*rdhDataset) ;
        backgroundModelStatisticalErrorMC[SignalMode] = sqrt(backgroundError*backgroundError - (fPDF[1]->getError()/fPDF[1]->getVal())*(fPDF[1]->getError()/fPDF[1]->getVal())) ;
        cout << "MC " << integral[0] << " " << integral[1] << " " << integral[2] << " " << integral[3] << endl << fPDF[1]->getVal() <<  " +/- " << fPDF[1]->getError() << endl ;
        cout << backgroundError << " " << fPDF[1]->getError()/fPDF[1]->getVal() << " " << backgroundModelStatisticalErrorData[SignalMode] << endl << endl << endl ;
        Bool_t success = (fPDF[1]->getVal()>fPDF[1]->getError()) ;
    
        // Put results into arrays
        fittedYield[SignalMode][ratio][fit]      = fPDF[1]->getVal()*totalYield ;
        fittedYieldError[SignalMode][ratio][fit] = fPDF[1]->getError()*totalYield ;
        trueYield[SignalMode][ratio][fit]        = integral[2]*ratios[ratio] ;
        Float_t trueFraction = trueYield[SignalMode][ratio][fit]/totalYield ;
        cout << endl << "True fraction: " << trueFraction << " Total yield: " << totalYield << endl << endl ;
        if(SignalMode==2 && fit==NFit-1)
        {
          DsToMuNuYield[ratio]      = fPDF[4]->getVal()*totalYield ;
          DsToMuNuYieldError[ratio] = fPDF[4]->getError()*totalYield ;
        }
        if(ratio==(NRatios-1)/2 && fit==NFit-1)
        {
          totalEvents[SignalMode]           = integral[0] ;
          backgroundYield[NSignalMode]      = integral[0]*(1-fPDF[1]->getVal()) ;
          backgroundYieldError[NSignalMode] = integral[0]*(1-fPDF[1]->getError()) ;
        }
    
        // Make residual plots by creating a new histogram
        Float_t weight1 = (integral[1]!=0) ? fittedYield[SignalMode][ratio][fit]/integral[1]              : 0 ;
        Float_t weight3 = (integral[3]!=0) ? (totalYield-fittedYield[SignalMode][ratio][fit])/integral[3] : 0 ;
        TH1F* hFit = new TH1F("hFit","hFit",varNBins[varX[SignalMode]],varLower[varX[SignalMode]],varUpper[varX[SignalMode]]) ;
        hFit->Add(h1PDF[1],weight1) ;
        hFit->Add(h1PDF[3],weight3) ;
        RoofitPrintNameX[SignalMode][fit][ratio] = TString(TString("Roofit_") + varName[varX[SignalMode]] + "_" + FitName[fit] + "_" + ratioName[ratio]) ;
        MakeResidualsPlot(rrv[varX[SignalMode]], *model, rdhPDF[0], NULL, rhpPDF[1], 0, SignalModeColor[SignalMode], h1PDF[0], hFit, success, cRoofit,   RoofitPrintNameX[SignalMode][fit][ratio]) ;
        delete hFit ;
    
    if(fit==NFit-1)
    {
      // Different background models
      if(ratio==(NRatios-1)/2)
      {
        if(kTRUE)
        {
          // Unblind comparison
          TString histogramUnblindMCName   = TString("unblindMC") ;
          TString histogramUnblindDataName = TString("unblindData") ;
          TH1F* hUnblindMC   = new TH1F(histogramUnblindMCName  ,histogramUnblindMCName  ,varNBins[varX[SignalMode]],varLower[varX[SignalMode]],varUpper[varX[SignalMode]]) ;
          TH1F* hUnblindData = new TH1F(histogramUnblindDataName,histogramUnblindDataName,varNBins[varX[SignalMode]],varLower[varX[SignalMode]],varUpper[varX[SignalMode]]) ;
          hUnblindMC->SetMinimum(0) ;
          hUnblindData->SetMinimum(0) ;
          TString unblindCut ;
          if(SignalMode<2) { unblindCut = TString(TString("SignalMode==")+(long)SignalMode+"&&mm2>0.4&&mm2<25"+EExtraCutSignalMode[SignalMode]) ; }
          else { unblindCut = TString(TString("SignalMode==")+(long)SignalMode+"&&eextra>1"+EExtraCutSignalMode[SignalMode]) ; }

          if(SignalMode<2)
          {
            tree->Draw(TString(TString("mm2>>")+histogramUnblindMCName)  ,TString(weightString+"*("+unblindCut+"&&type==0)"),"goff")  ;
            tree->Draw(TString(TString("mm2>>")+histogramUnblindDataName),TString(unblindCut+"&&type==10"),"goff") ;
          }
          else
          {
            tree->Draw(TString(TString("eextra>>")+histogramUnblindMCName)  ,TString(weightString+"*("+unblindCut+"&&type==0)"),"goff")  ;
            tree->Draw(TString(TString("eextra>>")+histogramUnblindDataName),TString(unblindCut+"&&type==10"),"goff") ;
          }
          TString rdhName = TString("rdhDataSideband") ;
          RooDataHist rdhDataSideband(rdhName,rdhName,*rrv[varX[SignalMode]],hUnblindData) ;
          hUnblindMC->Scale(hUnblindData->GetSumOfWeights()/hUnblindMC->GetSumOfWeights()) ;
          TCanvas cUnblind("cUnblind","",100,100,500,500) ;
          Float_t x1 = (SignalMode<2) ? 0.2 : 0.7 ;
          TLegend unblindLegend = MakeLegend(x1, 0.7, x1+0.2, 0.9) ;
          hUnblindMC->SetFillColor(202) ;
          hUnblindMC->GetXaxis()->SetTitle(varTitle[varX[SignalMode]]) ;
          hUnblindData->GetXaxis()->SetTitle(varTitle[varX[SignalMode]]) ;
          if(SignalMode<2)
          {
            hUnblindMC->GetYaxis()->SetTitle(TString(TString("entries per ")  +FloatToString((varUpper[varX[SignalMode]]-varLower[varX[SignalMode]])/varNBins[varX[SignalMode]])+"GeV^{2}")) ;
            hUnblindData->GetYaxis()->SetTitle(TString(TString("entries per ")+FloatToString((varUpper[varX[SignalMode]]-varLower[varX[SignalMode]])/varNBins[varX[SignalMode]])+"GeV^{2}")) ;
          }
          else
          {
            hUnblindMC->GetYaxis()->SetTitle(TString(TString("entries per ")  +FloatToString((varUpper[varX[SignalMode]]-varLower[varX[SignalMode]])/varNBins[varX[SignalMode]])+"GeV")) ;
            hUnblindData->GetYaxis()->SetTitle(TString(TString("entries per ")+FloatToString((varUpper[varX[SignalMode]]-varLower[varX[SignalMode]])/varNBins[varX[SignalMode]])+"GeV")) ;
          }
          hUnblindData->SetMarkerStyle(20) ;

          Float_t chi2 = 0 ;
          Int_t   bins = 0 ;
          Float_t minChi2 = 10000 ;
          Int_t   minBins = 0 ;
          for(Float_t scaling=0.75 ; scaling<1.25 ; scaling+=0.01)
          {
            chi2 = 0 ;
            bins = 0 ;
            hUnblindMC->Scale(scaling) ;
            for(Int_t bin=1 ; bin<hUnblindMC->GetNbinsX()+1 ; bin++)
            {
              if((hUnblindMC->GetBinContent(bin)+hUnblindData->GetBinContent(bin))==0) continue ;
              Float_t diff  = hUnblindMC->GetBinContent(bin) - hUnblindData->GetBinContent(bin) ;
              Float_t sigma = hUnblindMC->GetBinError(bin)*hUnblindMC->GetBinError(bin) + hUnblindData->GetBinError(bin)*hUnblindData->GetBinError(bin) ;
              chi2 += diff*diff/sigma ;
              bins++ ;
            }
            cout << scaling << " " << chi2 << " " << bins << endl ;
            if(scaling==0.75)
            {
              unblindLegend.AddEntry(hUnblindData,"Data","p") ;
              unblindLegend.AddEntry(hUnblindMC  ,"MC"  ,"f") ;
            }
            if(chi2<minChi2)
            {         
              minChi2 = chi2 ;
              minBins = bins ;
              hUnblindData->Draw("p:e") ;
              hUnblindMC->Draw("sames") ;
              hUnblindData->Draw("sames:p:e") ;
              PrintCanvas(&cUnblind,"sidebandComparison") ;
              DRecoResiduals* residuals = new DRecoResiduals("residuals", hUnblindData, hUnblindMC) ;
              RooPlot* frame = rrv[varX[SignalMode]]->frame() ;
              residuals->MakeFitPlotWithResiduals(&cUnblind,frame) ;
              cUnblind.cd(1) ;
              hUnblindData->Draw("p:e") ;
              hUnblindMC->Draw("sames") ;
              hUnblindData->Draw("sames:p:e") ;
              unblindLegend.SetHeader(TString(TString("#chi^{2}/n=")+FloatToString(chi2,2)+"/"+(long)bins)) ;
              unblindLegend.Draw() ;
              PrintCanvas(&cUnblind,"sidebandComparison_residuals") ;
              delete residuals ;
              delete frame ;
              cUnblind.Clear() ;
            }
            hUnblindMC->Scale(1.0/scaling) ;
          }
          cout << endl << endl << minChi2 << " / " << minBins << endl << endl ;
          Chi2Unblind[SignalMode] = minChi2 ;
          NDoFUnblind[SignalMode] = minBins-1 ;       
          
          // Try multi component background model
          TString histogramName = TString("hSemileptonic") ;
          TH1F hSemileptonic(histogramName,histogramName,varNBins[varX[SignalMode]],varLower[varX[SignalMode]],varUpper[varX[SignalMode]]) ;
          // Get weights from DECAY.DEC
          hSemileptonic.Add(h1PDF[13], 2.7) ;
          hSemileptonic.Add(h1PDF[14],24.2) ;
          hSemileptonic.Add(h1PDF[15],30.7) ;
          TString rooDataHistName = TString("rdhSemileptonic") ;
          TString rooHistPdfName  = TString("rhpSemileptonic") ;
          RooDataHist rdhSemileptonic(rooDataHistName,rooDataHistName,*rrv[varX[SignalMode]],&hSemileptonic) ;
          RooHistPdf  rhpSemileptonic(rooHistPdfName ,rooHistPdfName ,*rrv[varX[SignalMode]],rdhSemileptonic) ;
          
          TString modelName = TString("modelMultiComponent") ;
          RooAddPdf modelMultiComponent(modelName,modelName,RooArgList(*rhpPDF[11],rhpSemileptonic),RooArgList(*fPDF[11])) ;
          modelMultiComponent.fitTo(rdhDataSideband) ;
          TH1F* hMultiComponentFit = (TH1F*)modelMultiComponent.createHistogram("hMultiComponentFit",*rrv[varX[SignalMode]]) ;
          hMultiComponentFit->Scale(hUnblindData->GetSumOfWeights()) ;
          
          chi2 = 0 ;
          bins = 0 ;
          for(Int_t bin=1 ; bin<hUnblindData->GetNbinsX()+1 ; bin++)
          {
            if((hMultiComponentFit->GetBinContent(bin)+hUnblindData->GetBinContent(bin))==0) continue ;
            Float_t diff  = hMultiComponentFit->GetBinContent(bin) - hUnblindData->GetBinContent(bin) ;
            Float_t sigma = hMultiComponentFit->GetBinError(bin)*hMultiComponentFit->GetBinError(bin) + hUnblindData->GetBinError(bin)*hUnblindData->GetBinError(bin) ;
            chi2 += diff*diff/sigma ;
            bins++ ;
          }
          //Chi2Unblind[SignalMode] = chi2 ;
          //NDoFUnblind[SignalMode] = bins-1 ;
          cout << endl << endl << chi2 << " / " << bins << endl << endl ;
          cout << endl << endl << endl << endl << endl ;
          DRecoResiduals* residualsMulticomponent = new DRecoResiduals("residuals", hUnblindData, hMultiComponentFit) ;
          RooPlot* frameMulticomponent = rrv[varX[SignalMode]]->frame() ;
          residualsMulticomponent->MakeFitPlotWithResiduals(&cUnblind,frameMulticomponent) ;
          cUnblind.cd(1) ;
          hMultiComponentFit->SetFillColor(202) ;
          hUnblindData->Draw("p:e") ;
          hMultiComponentFit->Draw("sames") ;
          hUnblindData->Draw("sames:p:e") ;
          unblindLegend.SetHeader(TString(TString("#chi^{2}/n = " + FloatToString(chi2) + "/" + (long)(bins-1) ))) ;
          unblindLegend.Draw() ;
          PrintCanvas(&cUnblind,"sidebandMulticomponentFit_residuals") ;
          delete residualsMulticomponent ;
          delete frameMulticomponent ;
        
          if(doComponentsAndDsBF==kTRUE)
          {
            // Vary DsBF
            for(Int_t DsBF=0 ; DsBF<NDsType ; DsBF++)
            {
              TString histogramSignalName     = TString("hDsBFSignal")     ;
              TString histogramOneName        = TString("hDsBFOne")        ;
              TString histogramOtherName      = TString("hDsBFOther")      ;
              TString histogramOtherDataName  = TString("hDsBFOtherData")  ;
              TString rooDataHistUpNameMC     = TString("rdhDsBFUpMC")     ;
              TString rooDataHistDownNameMC   = TString("rdhDsBFDownMC")   ;
              TString rooHistPdfUpNameMC      = TString("rhpDsBFUpMC")     ;
              TString rooHistPdfDownNameMC    = TString("rhpDsBFDownMC")   ;
              TString rooDataHistUpNameData   = TString("rdhDsBFUpData")   ;
              TString rooDataHistDownNameData = TString("rdhDsBFDownData") ;
              TString rooHistPdfUpNameData    = TString("rhpDsBFUpData")   ;
              TString rooHistPdfDownNameData  = TString("rhpDsBFDownData") ;
              TH1F        hSignal(histogramSignalName   ,histogramSignalName   ,varNBins[varX[SignalMode]],varLower[varX[SignalMode]],varUpper[varX[SignalMode]]) ;
              TH1F     hDsBFOther(histogramOtherName    ,histogramOtherName    ,varNBins[varX[SignalMode]],varLower[varX[SignalMode]],varUpper[varX[SignalMode]]) ;
              TH1F hDsBFOtherData(histogramOtherDataName,histogramOtherDataName,varNBins[varX[SignalMode]],varLower[varX[SignalMode]],varUpper[varX[SignalMode]]) ;
              TH1F       hDsBFOne(histogramOneName      ,histogramOneName      ,varNBins[varX[SignalMode]],varLower[varX[SignalMode]],varUpper[varX[SignalMode]]) ;
              TString signalCut    = TString(weightString+"*(type==1&&SignalMode=="+(long)SignalMode+"&&DsType!="+((long)DsBF)+EExtraCutSignalMode[SignalMode]+")") ;
              TString otherCut     = TString(weightString+"*(type==3&&SignalMode=="+(long)SignalMode+"&&DsType!="+((long)DsBF)+EExtraCutSignalMode[SignalMode]+")") ;
              TString otherDataCut = TString(weightString+"*(type==3&&modeNumber!=6&&SignalMode=="+(long)SignalMode+"&&DsType!="+((long)DsBF)+EExtraCutSignalMode[SignalMode]+")") ;
              TString oneCut       = TString(weightString+"*(type==3&&SignalMode=="+(long)SignalMode+"&&DsType=="+((long)DsBF)+EExtraCutSignalMode[SignalMode]+")") ;
              TString drawString = (SignalMode<2) ? "mm2" : "eextra" ;
              tree->Draw(TString(drawString+">>"+histogramSignalName)   ,   signalCut,"goff") ;
              tree->Draw(TString(drawString+">>"+histogramOtherName)    ,    otherCut,"goff") ;
              tree->Draw(TString(drawString+">>"+histogramOtherDataName),otherDataCut,"goff") ;
              tree->Draw(TString(drawString+">>"+histogramOneName)      ,      oneCut,"goff") ;
          
              // Get semileptonic DsBF for data
              TH1F* hSemileptonicClone = (TH1F*)hSemileptonic.Clone("hSemileptonicClone") ;
              hSemileptonicClone->Scale(fractionSemileptonic*h1PDF[10]->GetSumOfWeights()/hSemileptonic.GetSumOfWeights()) ;
          
              cout << endl << endl << "DsBF: " 
               << DsBF << " "  
               << hDsBFOne.GetSumOfWeights() << " " 
               << hDsBFOther.GetSumOfWeights() << " " 
               << hSignal.GetSumOfWeights() << " " 
               << integral[0] << " " 
               << integral[10] << " " 
               << "Semileptonic fraction: " << fractionSemileptonic << endl << endl ;
              Float_t scaleFactor = DsTypeError[DsBF] ;
              hDsBFOne.Scale(1.0+scaleFactor) ;
              TH1F* hDsBFUpMC   = new TH1F("hDsBFUpMC"  ,"hDsBFUpMC"  ,varNBins[varX[SignalMode]],varLower[varX[SignalMode]],varUpper[varX[SignalMode]]) ;
              TH1F* hDsBFUpData = new TH1F("hDsBFUpData","hDsBFUpData",varNBins[varX[SignalMode]],varLower[varX[SignalMode]],varUpper[varX[SignalMode]]) ;
              for(Int_t bin=1 ; bin<hDsBFUpMC->GetNbinsX()+1 ; bin++)
              {
                hDsBFUpMC->SetBinContent(bin,hDsBFOther.GetBinContent(bin)+hDsBFOne.GetBinContent(bin)) ;
                hDsBFUpData->SetBinContent(bin,hDsBFOtherData.GetBinContent(bin)+hDsBFOne.GetBinContent(bin)+hSemileptonicClone->GetBinContent(bin)) ;
                cout << bin << " " << hDsBFUpMC->GetBinContent(bin) << " " << hDsBFUpData->GetBinContent(bin) << " " << hDsBFOther.GetBinContent(bin) << " " << hDsBFOne.GetBinContent(bin) << " " << hSemileptonic.GetBinContent(bin) << endl ;
              }
              cout << hDsBFUpMC->GetSumOfWeights() << "    " << hDsBFUpData->GetSumOfWeights() << endl ;
          
              RooDataHist   rdhDsBFUpMC(rooDataHistUpNameMC,  rooDataHistUpNameMC,  *rrv[varX[SignalMode]],  hDsBFUpMC) ;
              RooHistPdf    rhpDsBFUpMC( rooHistPdfUpNameMC,   rooHistPdfUpNameMC,  *rrv[varX[SignalMode]],rdhDsBFUpMC) ;
              RooDataHist rdhDsBFUpData(rooDataHistUpNameData,rooDataHistUpNameData,*rrv[varX[SignalMode]],  hDsBFUpData) ;
              RooHistPdf  rhpDsBFUpData( rooHistPdfUpNameData, rooHistPdfUpNameData,*rrv[varX[SignalMode]],rdhDsBFUpData) ;
          
              RooAddPdf   modelDsBFUpMC("modelDsBFUpMC"  ,"",RooArgList(*rhpPDF[1],rhpDsBFUpMC),  RooArgList(*fPDF[1])) ;
              RooAddPdf modelDsBFUpData("modelDsBFUpData","",RooArgList(*rhpPDF[1],rhpDsBFUpData,rhpSemileptonic),RooArgList(*fPDF[10],*fPDF[16])) ;
          
              cout << endl << endl << "Fitting DsBF up to MC" << endl << endl ;
              fPDF[1]->setVal(0.15) ;
              modelDsBFUpMC.fitTo(*rdhPDF[0]) ;
              TH1F* hFitUpMC = (TH1F*)modelDsBFUpMC.createHistogram("hFitUpMC",*rrv[varX[SignalMode]]) ;
              hFitUpMC->Scale(h1PDF[0]->GetSumOfWeights()/hFitUpMC->GetSumOfWeights()) ;
              MakeResidualsPlot(rrv[varX[SignalMode]],modelDsBFUpMC,rdhPDF[0],rhpPDF[1],NULL,SignalModeColor[SignalMode],0,h1PDF[0],hFitUpMC,kTRUE,cRoofit,TString(TString("DsBFMCUp") + (long)DsBF)) ;

              fittedYieldDsBFMC[SignalMode][DsBF][0] = fPDF[1]->getVal()*h1PDF[0]->GetSumOfWeights() ;
              cout << endl << endl << "Fitting DsBF up to data" << endl << endl ;
              fPDF[1]->setVal(0.15) ;
              modelDsBFUpData.fitTo(*rdhPDF[10]) ;
              fittedYieldDsBFData[SignalMode][DsBF][0] = fPDF[10]->getVal()*h1PDF[10]->GetSumOfWeights() ;
          
              TH1F* hFitUpData = (TH1F*)modelDsBFUpData.createHistogram("hFitUpData",*rrv[varX[SignalMode]]) ;
              hFitUpData->Scale(h1PDF[10]->GetSumOfWeights()/hFitUpData->GetSumOfWeights()) ;
              MakeResidualsPlot(rrv[varX[SignalMode]],modelDsBFUpData,rdhPDF[10],rhpPDF[1],NULL,SignalModeColor[SignalMode],0,h1PDF[10],hFitUpData,kTRUE,cRoofit,TString(TString("DsBFDataUp") + (long)DsBF)) ;
          
              hDsBFOne.Scale((1.0-scaleFactor)/(1.0+scaleFactor)) ;
              TH1F* hDsBFDownMC   = new TH1F("hDsBFDownMC"  ,"hDsBFDownMC"  ,varNBins[varX[SignalMode]],varLower[varX[SignalMode]],varUpper[varX[SignalMode]]) ;
              TH1F* hDsBFDownData = new TH1F("hDsBFDownData","hDsBFDownData",varNBins[varX[SignalMode]],varLower[varX[SignalMode]],varUpper[varX[SignalMode]]) ;
              for(Int_t bin=1 ; bin<hDsBFDownMC->GetNbinsX()+1 ; bin++)
              {
                hDsBFDownMC->SetBinContent(bin,hDsBFOther.GetBinContent(bin)+hDsBFOne.GetBinContent(bin)) ;
                hDsBFDownData->SetBinContent(bin,hDsBFOther.GetBinContent(bin)+hDsBFOne.GetBinContent(bin)+hSemileptonicClone->GetBinContent(bin)) ;
                //cout << bin << " " << hDsBFUpMC->GetBinContent(bin) << " " << hDsBFUpData->GetBinContent(bin) << " " << hDsBFOther.GetBinContent(bin) << " " << hDsBFOne.GetBinContent(bin) << " " << hSemileptonic.GetBinContent(bin) << endl ;
              }
              RooDataHist rdhDsBFDownMC(rooDataHistDownNameMC,rooDataHistDownNameMC,*rrv[varX[SignalMode]],        hDsBFDownMC) ;
              RooHistPdf  rhpDsBFDownMC(rooHistPdfDownNameMC ,rooHistPdfDownNameMC ,*rrv[varX[SignalMode]],      rdhDsBFDownMC) ;
              RooDataHist rdhDsBFDownData(rooDataHistDownNameData,rooDataHistDownNameData,*rrv[varX[SignalMode]],  hDsBFDownData) ;
              RooHistPdf  rhpDsBFDownData(rooHistPdfDownNameData ,rooHistPdfDownNameData ,*rrv[varX[SignalMode]],rdhDsBFDownData) ;
              cout << endl << endl << "Fitting DsBF down to MC" << endl << endl ;
          
              RooAddPdf   modelDsBFDownMC("modelDsBFDownMC"  ,"",RooArgList(*rhpPDF[1],rhpDsBFDownMC),RooArgList(*fPDF[1])) ;
              RooAddPdf modelDsBFDownData("modelDsBFDownData","",RooArgList(*rhpPDF[10],rhpDsBFDownData,rhpSemileptonic),RooArgList(*fPDF[1],*fPDF[16])) ;
              fPDF[1]->setVal(0.15) ;
              modelDsBFDownMC.fitTo(*rdhPDF[0]) ;
              TH1F* hFitDownMC = (TH1F*)modelDsBFDownData.createHistogram("hFitDownMC",*rrv[varX[SignalMode]]) ;
              hFitDownMC->Scale(h1PDF[0]->GetSumOfWeights()/hFitDownMC->GetSumOfWeights()) ;
              MakeResidualsPlot(rrv[varX[SignalMode]],modelDsBFDownData,rdhPDF[0],rhpPDF[1],NULL,SignalModeColor[SignalMode],0,h1PDF[0],hFitDownMC,kTRUE,cRoofit,TString(TString("DsBFMCDown") + (long)DsBF)) ;

              fittedYieldDsBFMC[SignalMode][DsBF][1] = fPDF[1]->getVal()*h1PDF[0]->GetSumOfWeights() ;
              cout << endl << endl << "Fitting DsBF down to data" << endl << endl ;
              fPDF[1]->setVal(0.15) ;
              modelDsBFDownData.fitTo(*rdhPDF[10]) ;
              fittedYieldDsBFData[SignalMode][DsBF][1] = fPDF[10]->getVal()*h1PDF[10]->GetSumOfWeights() ;
          
              TH1F* hFitDownData = (TH1F*)modelDsBFDownData.createHistogram("hFitDownData",*rrv[varX[SignalMode]]) ;
              hFitDownData->Scale(h1PDF[10]->GetSumOfWeights()/hFitDownData->GetSumOfWeights()) ;
              MakeResidualsPlot(rrv[varX[SignalMode]],modelDsBFDownData,rdhPDF[10],rhpPDF[1],NULL,SignalModeColor[SignalMode],0,h1PDF[10],hFitDownData,kTRUE,cRoofit,TString(TString("DsBFDataDown") + (long)DsBF)) ;
              delete hFitUpMC ;
              delete hFitUpData ;
              delete hFitDownMC ;
              delete hFitDownData ;
              delete hDsBFUpMC ;
              delete hDsBFDownMC ;
              delete hDsBFUpData ;
              delete hDsBFDownData ;
          
              cout << endl << endl << "Results (DsBF): " << DsBF << " " << fittedYieldDsBFMC[SignalMode][DsBF][0] << " " << fittedYieldDsBFMC[SignalMode][DsBF][1] << " " << fittedYieldDsBFData[SignalMode][DsBF][0] << " " << fittedYieldDsBFData[SignalMode][DsBF][1] << endl ;
            }
        
            // Components
            for(Int_t component=0 ; component<NComponent ; component++)
            {
              if(component==SignalMode) continue ;
              TString histogramSignalName     = TString("hSignal")              ;
              TString histogramOneName        = TString("hComponentOne")        ;
              TString histogramOtherName      = TString("hComponentOther")      ;
              TString histogramOtherDataName  = TString("hComponentOtherData")  ;
              TString rooDataHistUpNameMC     = TString("rdhComponentUpMC")     ;
              TString rooDataHistDownNameMC   = TString("rdhComponentDownMC")   ;
              TString rooHistPdfUpNameMC      = TString("rhpComponentUpMC")     ;
              TString rooHistPdfDownNameMC    = TString("rhpComponentDownMC")   ;
              TString rooDataHistUpNameData   = TString("rdhComponentUpData")   ;
              TString rooDataHistDownNameData = TString("rdhComponentDownData") ;
              TString rooHistPdfUpNameData    = TString("rhpComponentUpData")   ;
              TString rooHistPdfDownNameData  = TString("rhpComponentDownData") ;
              TH1F             hSignal(histogramSignalName   ,histogramSignalName   ,varNBins[varX[SignalMode]],varLower[varX[SignalMode]],varUpper[varX[SignalMode]]) ;
              TH1F     hComponentOther(histogramOtherName    ,histogramOtherName    ,varNBins[varX[SignalMode]],varLower[varX[SignalMode]],varUpper[varX[SignalMode]]) ;
              TH1F hComponentOtherData(histogramOtherDataName,histogramOtherDataName,varNBins[varX[SignalMode]],varLower[varX[SignalMode]],varUpper[varX[SignalMode]]) ;
              TH1F       hComponentOne(histogramOneName      ,histogramOneName      ,varNBins[varX[SignalMode]],varLower[varX[SignalMode]],varUpper[varX[SignalMode]]) ;
          
              TString signalCut    = TString(weightString+"*(type==1&&SignalMode=="+(long)SignalMode+"&&modeNumber!="+((long)component)+               EExtraCutSignalMode[SignalMode]+")") ;
              TString otherCut     = TString(weightString+"*(type==3&&SignalMode=="+(long)SignalMode+"&&modeNumber!="+((long)component)+               EExtraCutSignalMode[SignalMode]+")") ;
              TString otherDataCut = TString(weightString+"*(type==3&&SignalMode=="+(long)SignalMode+"&&modeNumber!=6&&modeNumber!="+((long)component)+EExtraCutSignalMode[SignalMode]+")") ;
              TString oneCut       = TString(weightString+"*(type==3&&SignalMode=="+(long)SignalMode+"&&modeNumber=="+((long)component)+               EExtraCutSignalMode[SignalMode]+")") ;
              TString drawString = (SignalMode<2) ? "mm2" : "eextra" ;
              tree->Draw(TString(drawString+">>"+histogramSignalName),   signalCut,   "goff") ;
              tree->Draw(TString(drawString+">>"+histogramOtherName),    otherCut,    "goff") ;
              tree->Draw(TString(drawString+">>"+histogramOtherDataName),otherDataCut,"goff") ;
              tree->Draw(TString(drawString+">>"+histogramOneName),      oneCut,      "goff") ;
           
              // Get semileptonic component for data
              //Float_t fractionSemileptonic = 0 ;
              TH1F* hSemileptonicClone = (TH1F*)hSemileptonic.Clone("hSemileptonicClone") ;
              hSemileptonicClone->Scale(fractionSemileptonic*h1PDF[10]->GetSumOfWeights()/hSemileptonic.GetSumOfWeights()) ;
          
              cout << endl << endl << "Component: " 
               << component << " "  
               << hComponentOne.GetSumOfWeights() << " " 
               << hComponentOther.GetSumOfWeights() << " " 
               << hSignal.GetSumOfWeights() << " " 
               << integral[0]  << " " 
               << integral[10] << " " 
               << "Semileptonic fraction: " << fractionSemileptonic << endl << endl ;
              Float_t scaleFactor = 0.1 ;
              hComponentOne.Scale(1.0+scaleFactor) ;
              TH1F* hComponentUpMC   = new TH1F("hComponentUpMC"  ,"hComponentUpMC"  ,varNBins[varX[SignalMode]],varLower[varX[SignalMode]],varUpper[varX[SignalMode]]) ;
              TH1F* hComponentUpData = new TH1F("hComponentUpData","hComponentUpData",varNBins[varX[SignalMode]],varLower[varX[SignalMode]],varUpper[varX[SignalMode]]) ;
              if(component==6) hSemileptonicClone->Scale(1.0+scaleFactor) ;
              for(Int_t bin=1 ; bin<hComponentUpMC->GetNbinsX()+1 ; bin++)
              {
                hComponentUpMC->SetBinContent(bin,hComponentOther.GetBinContent(bin)+hComponentOne.GetBinContent(bin)) ;
                cout << hComponentOther.GetBinContent(bin) << " " << hComponentOne.GetBinContent(bin) << " " << hComponentUpMC->GetBinContent(bin) << endl ;
                if(component==6){ hComponentUpData->SetBinContent(bin,hComponentOtherData.GetBinContent(bin)+hSemileptonicClone->GetBinContent(bin)) ; }
                else{ hComponentUpData->SetBinContent(bin, hComponentOtherData.GetBinContent(bin) + hComponentOne.GetBinContent(bin) + hSemileptonicClone->GetBinContent(bin)) ;}
                }
                RooDataHist   rdhComponentUpMC(rooDataHistUpNameMC,  rooDataHistUpNameMC,  *rrv[varX[SignalMode]],  hComponentUpMC) ;
                RooHistPdf    rhpComponentUpMC( rooHistPdfUpNameMC,   rooHistPdfUpNameMC,  *rrv[varX[SignalMode]],rdhComponentUpMC) ;
                RooDataHist rdhComponentUpData(rooDataHistUpNameData,rooDataHistUpNameData,*rrv[varX[SignalMode]],  hComponentUpData) ;
                RooHistPdf  rhpComponentUpData( rooHistPdfUpNameData, rooHistPdfUpNameData,*rrv[varX[SignalMode]],rdhComponentUpData) ;
                RooAddPdf   modelComponentUpMC("modelComponentUpMC"  ,"",RooArgList(*rhpPDF[1],rhpComponentUpMC),  RooArgList(*fPDF[1])) ;
                RooAddPdf modelComponentUpData("modelComponentUpData","",RooArgList(*rhpPDF[1],rhpComponentUpData,rhpSemileptonic),RooArgList(*fPDF[10],*fPDF[16])) ;
          
                cout << endl << endl << "Fitting component up to MC" << endl << endl ;
                fPDF[1]->setVal(0.15) ;
                modelComponentUpMC.fitTo(*rdhPDF[0]) ;
                TH1F* hFitUpMC = (TH1F*)modelComponentUpMC.createHistogram("hFitUpMC",*rrv[varX[SignalMode]]) ;
                hFitUpMC->Scale(h1PDF[0]->GetSumOfWeights()/hFitUpMC->GetSumOfWeights()) ;
                MakeResidualsPlot(rrv[varX[SignalMode]], modelComponentUpMC, rdhPDF[0], rhpPDF[1], NULL, SignalModeColor[SignalMode], 0, h1PDF[0], hFitUpMC, kTRUE, cRoofit, TString(TString("componentMCUp") + (long)component)) ;

                fittedYieldComponentMC[SignalMode][component][0] = fPDF[1]->getVal()*h1PDF[0]->GetSumOfWeights() ;
                cout << endl << endl << "Fitting component up to data" << endl << endl ;
                fPDF[1]->setVal(0.15) ;
                modelComponentUpData.fitTo(*rdhPDF[10]) ;
                fittedYieldComponentData[SignalMode][component][0] = fPDF[10]->getVal()*h1PDF[10]->GetSumOfWeights() ;
          
                TH1F* hFitUpData = (TH1F*)modelComponentUpData.createHistogram("hFitUpData",*rrv[varX[SignalMode]]) ;
                hFitUpData->Scale(h1PDF[10]->GetSumOfWeights()/hFitUpData->GetSumOfWeights()) ;
                MakeResidualsPlot(rrv[varX[SignalMode]], modelComponentUpData, rdhPDF[10], rhpPDF[1], NULL, SignalModeColor[SignalMode], 0, h1PDF[10], hFitUpData, kTRUE, cRoofit, TString(TString("componentDataUp") + (long)component)) ;
          
                hComponentOne.Scale((1.0-scaleFactor)/(1.0+scaleFactor)) ;
                TH1F* hComponentDownMC   = new TH1F("hComponentDownMC"  ,"hComponentDownMC"  ,varNBins[varX[SignalMode]],varLower[varX[SignalMode]],varUpper[varX[SignalMode]]) ;
                TH1F* hComponentDownData = new TH1F("hComponentDownData", "hComponentDownData", varNBins[varX[SignalMode]], varLower[varX[SignalMode]], varUpper[varX[SignalMode]]) ;
                if(component==6) hSemileptonicClone->Scale((1-scaleFactor)/(1.0+scaleFactor)) ;
                for(Int_t bin=1 ; bin<hComponentDownMC->GetNbinsX()+1 ; bin++)
                {
                  hComponentDownMC->SetBinContent(bin,hComponentOther.GetBinContent(bin)+hComponentOne.GetBinContent(bin)) ;
                  if(component==6){hComponentDownData->SetBinContent(bin,hComponentOtherData.GetBinContent(bin)+hSemileptonicClone->GetBinContent(bin)) ;}
                  else{ hComponentDownData->SetBinContent(bin, hComponentOtherData.GetBinContent(bin) + hComponentOne.GetBinContent(bin) + hSemileptonicClone->GetBinContent(bin)) ;}
                }
                RooDataHist rdhComponentDownMC(rooDataHistDownNameMC,rooDataHistDownNameMC,*rrv[varX[SignalMode]],        hComponentDownMC) ;
                RooHistPdf  rhpComponentDownMC(rooHistPdfDownNameMC ,rooHistPdfDownNameMC ,*rrv[varX[SignalMode]],      rdhComponentDownMC) ;
                RooDataHist rdhComponentDownData(rooDataHistDownNameData,rooDataHistDownNameData,*rrv[varX[SignalMode]],  hComponentDownData) ;
                RooHistPdf  rhpComponentDownData(rooHistPdfDownNameData ,rooHistPdfDownNameData ,*rrv[varX[SignalMode]],rdhComponentDownData) ;
                cout << endl << endl << "Fitting component down to MC" << endl << endl ;
          
                RooAddPdf   modelComponentDownMC("modelComponentDownMC"  ,"",RooArgList(*rhpPDF[1],rhpComponentDownMC),RooArgList(*fPDF[1])) ;
                RooAddPdf modelComponentDownData("modelComponentDownData","",RooArgList(*rhpPDF[1],rhpComponentDownData,rhpSemileptonic),RooArgList(*fPDF[10],*fPDF[16])) ;
                fPDF[1]->setVal(0.15) ;
                modelComponentDownMC.fitTo(*rdhPDF[0]) ;
                TH1F* hFitDownMC = (TH1F*)modelComponentDownMC.createHistogram("hFitDownMC",*rrv[varX[SignalMode]]) ;
                hFitDownMC->Scale(h1PDF[0]->GetSumOfWeights()/hFitDownMC->GetSumOfWeights()) ;
                MakeResidualsPlot(rrv[varX[SignalMode]], modelComponentDownMC, rdhPDF[0], rhpPDF[1], NULL, SignalModeColor[SignalMode], 0, h1PDF[0], hFitDownMC, kTRUE, cRoofit, TString(TString("componentMCDown") + (long)component)) ;
                fittedYieldComponentMC[SignalMode][component][1] = fPDF[1]->getVal()*h1PDF[0]->GetSumOfWeights() ;
                cout << endl << endl << "Fitting component down to data" << endl << endl ;
                fPDF[1]->setVal(0.15) ;
                modelComponentDownData.fitTo(*rdhPDF[10]) ;
                fittedYieldComponentData[SignalMode][component][1] = fPDF[10]->getVal()*h1PDF[10]->GetSumOfWeights() ;
          
                // zzzz
                TH1F* hFitDownData = (TH1F*)modelComponentDownData.createHistogram("hFitDownData",*rrv[varX[SignalMode]]) ;
                hFitDownData->Scale(h1PDF[10]->GetSumOfWeights()/hFitDownData->GetSumOfWeights()) ;
                MakeResidualsPlot(rrv[varX[SignalMode]], modelComponentDownData, rdhPDF[10], rhpPDF[1], NULL, SignalModeColor[SignalMode], 0, h1PDF[10], hFitDownData, kTRUE, cRoofit, TString(TString("componentDataDown") + (long)component)) ;
                  delete hFitUpMC ;
                  delete hFitUpData ;
                  delete hFitDownMC ;
                  delete hFitDownData ;
                  delete hComponentUpMC ;
                  delete hComponentDownMC ;
                  delete hComponentUpData ;
                  delete hComponentDownData ;
          
                  cout << endl << endl << "Results: " << component << " " << fittedYieldComponentMC[SignalMode][component][0] << " " << fittedYieldComponentMC[SignalMode][component][1] << " " << fittedYieldComponentData[SignalMode][component][0] << " " << fittedYieldComponentData[SignalMode][component][1] << endl ;
                }
              }
          
              // Statistical variation
              if(kTRUE)
              {
                cout << endl << endl << endl ;
                Float_t statVaryErrorMC   = 0 ;
                Float_t statVaryErrorData = 0 ;
                TH1F* hToy = (TH1F*)h1Dataset->Clone("hToy") ;
                Int_t zMaxInt = (Int_t)h1Dataset->GetMaximum() ;
                for(Int_t i=1 ; i<nStat ; i++)
                {
                  TH1F* hClone = (TH1F*)h1PDF[3]->Clone("hClone") ;
                  hClone->Sumw2() ;
                  TRandom3 random ;
                  random.SetSeed(1) ;
                  for(Int_t bin=1 ; bin<hClone->GetNbinsX()+1 ; bin++)
                  {
                    Float_t newBinContent = -1 ;
                    while(newBinContent<0){ newBinContent = hClone->GetBinContent(bin)+random.Gaus(0,hClone->GetBinError(bin)) ; }
                    hClone->SetBinContent(bin,newBinContent) ;
                  }
          
                  TString fName = TString(TString("fStat")+(long)i) ;
                  RooRealVar fStat(fName,fName,0.125,0.0,1.0) ;
                  //while(fStat.getVal()>0.24)
                  {
                    // Empty toy histogram and fill with random contents
                    hToy->Reset() ;
                    Int_t counter = 0 ;
                    Float_t numberOfEvents = h1PDF[3]->GetSumOfWeights() + random.Gaus(0,sqrt(h1PDF[3]->GetSumOfWeights())) ;
                    while(counter<numberOfEvents)
                    {
                      Int_t binX = 1+rand()%(varNBins[varX[SignalMode]]) ;
                      Float_t z = 0.0001*(rand()%(zMaxInt*10000)) ;
                      if(z<h1PDF[3]->GetBinContent(binX))
                      {
                        hToy->SetBinContent(binX,hToy->GetBinContent(binX)+1) ;
                        counter++ ;
                      }
                    }
            
                    RooDataHist rdhClone("rdhClone","rdhClone",*rrv[varX[SignalMode]],hToy) ;
                    RooHistPdf  rhpClone("rhpClone","rhpClone",*rrv[varX[SignalMode]],rdhClone) ;
                    RooAddPdf modelClone("modelClone","modelClone",RooArgList(*rhpPDF[1],rhpClone),RooArgList(fStat)) ;
                    modelClone.fitTo(*rdhPDF[0]) ;
            
                    //cout << fStat.getVal() << " +/- " << fStat.getError() << " " << fStat.getVal()*h1PDF[0]->GetSumOfWeights() << " " << error << endl ;
            
                    statYieldMC[SignalMode][i] = fStat.getVal()*h1PDF[0]->GetSumOfWeights() ;
            
                    TH1F* hFitMC = (TH1F*) modelClone.createHistogram("hFitMC",*rrv[varX[SignalMode]]) ;
                    hFitMC->Scale(h1PDF[0]->GetSumOfWeights()/hFitMC->GetSumOfWeights()) ;
                    MakeResidualsPlot(rrv[varX[SignalMode]], modelClone, rdhPDF[0], rhpPDF[1], NULL, SignalModeColor[SignalMode], 0, h1PDF[0], hFitMC, kTRUE, cRoofit, TString(TString("MCStat")+(long)i)) ;
                    delete hFitMC ;
                  }
                  delete hClone ;
          
                  // Empty toy histograms and fill with random contents
                  TH1F* hSemileptonicClone    = (TH1F*)hSemileptonic.Clone("hSemileptonicClone") ;
                  TH1F* hNotSemileptonicClone = (TH1F*)h1PDF[16]->Clone("hNotSemileptonic") ;

                  hToy->Reset() ;
                  TH1F* hSpectrum = (TH1F*)hToy->Clone("hSpectrum") ;
          
                  hSpectrum->Add(&hSemileptonic,fractionSemileptonic*h1PDF[3]->GetSumOfWeights()/hSemileptonic.GetSumOfWeights()) ;
                  hSpectrum->Add(hNotSemileptonicClone,fractionNotSemileptonic*h1PDF[3]->GetSumOfWeights()/hNotSemileptonicClone->GetSumOfWeights()) ;
                  Float_t numberOfEvents = h1PDF[3]->GetSumOfWeights() + random.Gaus(0,sqrt(h1PDF[3]->GetSumOfWeights())) ;
                  Int_t counter = 0 ;
                  zMaxInt = (Int_t)hSpectrum->GetMaximum() ;
                  
                  cout << endl << endl << hSemileptonic.GetSumOfWeights() << " " << h1PDF[16]->GetSumOfWeights() << " " << hSpectrum->GetSumOfWeights() << " " << zMaxInt << " " << fractionSemileptonic << " " << fractionNotSemileptonic << endl << endl ;

                  while(counter<numberOfEvents)
                  {
                    Int_t binX = 1+rand()%(varNBins[varX[SignalMode]]) ;
                    Float_t z = 0.0001*(rand()%(zMaxInt*10000)) ;
                    if(z<hSpectrum->GetBinContent(binX))
                    {
                      hToy->SetBinContent(binX,hToy->GetBinContent(binX)+1) ;
                      counter++ ;
                    }
                  }
          
                  RooDataHist    rdhToy("rdhToy",   "rdhToy",   *rrv[varX[SignalMode]],     hToy) ;
                  RooHistPdf     rhpToy("rhpToy",   "rhpToy",   *rrv[varX[SignalMode]],   rdhToy) ;
                  RooAddPdf modelDataClone("modelClone","modelClone",RooArgList(*rhpPDF[1],rhpToy),RooArgList(fStat)) ;
                  modelDataClone.fitTo(*rdhPDF[10]) ;
                  Float_t error = (fStat.getVal()*h1PDF[10]->GetSumOfWeights()-dataYield[SignalMode][NFit-1])/dataYield[SignalMode][NFit-1] ;
                  if(fStat.getVal()<fStat.getError()) error = 0 ;
                  statVaryErrorData += error*error ;
                  cout << fStat.getVal() << " +/- " << fStat.getError() << " " << fStat.getVal()*h1PDF[10]->GetSumOfWeights() << " " << error << endl ;     
                  statYieldData[SignalMode][i] = fStat.getVal()*h1PDF[10]->GetSumOfWeights() ;
          
                  TH1F* hFitData = (TH1F*) modelDataClone.createHistogram("hFitData",*rrv[varX[SignalMode]]) ;
                  hFitData->Scale(h1PDF[10]->GetSumOfWeights()/hFitData->GetSumOfWeights()) ;
                  MakeResidualsPlot(rrv[varX[SignalMode]], modelDataClone, rdhPDF[10], rhpPDF[1], NULL, SignalModeColor[SignalMode], 0, h1PDF[10], hFitData, kTRUE, cRoofit, TString(TString("dataStat")+(long)i)) ;
                  delete hFitData ;
                  delete hSemileptonicClone ;
                }
                cout << sqrt(statVaryErrorMC) << " " << sqrt(statVaryErrorData) << endl << endl ;
                //backgroundModelStatisticalErrorMC[SignalMode]   = sqrt(statVaryErrorMC) ;
                //backgroundModelStatisticalErrorData[SignalMode] = sqrt(statVaryErrorData) ;
              }
          
              // Toy fits for DsToENu
              if(SignalMode==1)
              {
                srand(1) ;
                TRandom3 toyRandom ;
                toyRandom.SetSeed(1) ;
                DsToENuToyYield = 0 ;
                nDsToEllNuToyPass = 0.0 ;
                TH1F* hToy = (TH1F*)h1Dataset->Clone("hToy") ;
                Int_t zMaxInt = (Int_t)h1Dataset->GetMaximum() ;
                TString fractionName = TString("fToyDsToENu") ;
                RooRealVar fToy(fractionName,fractionName,0.25,0.0,1.0) ;
                for(Int_t toy=0 ; toy<NToys ; toy++)
                {
                  // Empty toy histogram and fill with random contents
                  hToy->Reset() ;
                  Int_t counter = 0 ;
                  Float_t numberOfEvents = h1PDF[10]->GetSumOfWeights() + toyRandom.Gaus(0,sqrt(h1PDF[10]->GetSumOfWeights())) ;
                  while(counter<numberOfEvents)
                  {
                    Int_t binX = 1+rand()%(varNBins[varX[SignalMode]]) ;
                    Float_t z = 0.0001*(rand()%(zMaxInt*10000)) ;
                    if(z<h1PDF[10]->GetBinContent(binX))
                    {
                      hToy->SetBinContent(binX,hToy->GetBinContent(binX)+1) ;
                      counter++ ;
                    }
                  }
                  //// h1Dataset may not have integer sum of weights, so scale hToy
                  //hToy->Scale(h1Dataset->GetSumOfWeights()/hToy->GetSumOfWeights()) ;
                  RooDataHist rdhToy("rdhToy","rdhToy",*rrv[varX[SignalMode]],hToy) ;
                  RooArgList pdfsToy(*rhpPDF[1],*rhpPDF[3]) ;
                  RooArgList fractionsToy(fToy) ;
                  RooAddPdf modelToy(*modelData,"modelToy") ;
                  modelToy.fitTo(rdhToy) ;
                  cout << " --- " << fPDF[10]->getVal()*hToy->GetSumOfWeights() << " +/- " << fPDF[10]->getError()*hToy->GetSumOfWeights() << " --- " << endl ;
                  if(fPDF[10]->getError()*hToy->GetSumOfWeights()<100)
                  {
                    DsToENuToyYield += fPDF[10]->getVal()*hToy->GetSumOfWeights() ;
                    nDsToEllNuToyPass += 1.0 ;
                  }
                }
                DsToENuToyYield = DsToENuToyYield/nDsToEllNuToyPass ;
                delete hToy ;
              }
          
              // Signal model
              if(SignalMode<2)
              {
                Float_t mm2LowerTemp = -0.4 ;
                Float_t mm2UpperTemp =  0.4 ;
                Int_t binsTemp = 100 ;
                RooRealVar mm2Temp("mm2Temp","mm2Temp",mm2LowerTemp,mm2UpperTemp) ;
                mm2Temp.setBins(binsTemp) ;
                TH1F hSignal("hSignal","hSignal",binsTemp,mm2LowerTemp,mm2UpperTemp) ;
                tree->Draw(TString("mm2>>hSignal"),TString(TString("SignalMode==")+(long)SignalMode+"&&type==1"+EExtraCutSignalMode[SignalMode]),"goff") ;
                hSignal.SetFillColor(SignalModeColor[SignalMode]) ;
                hSignal.GetXaxis()->SetTitle(varTitle[varX[SignalMode]]) ;
                hSignal.GetYaxis()->SetTitle(TString("entries per 0.008 GeV^{2}")) ;
                hSignal.Draw() ;
                PrintCanvas(cRoofit,"mm2SignalPeak") ;
                cRoofit->Clear() ;
        
                RooRealVar      mean1("mean1"    ,"mean" ,mm2LowerTemp,mm2UpperTemp) ;
                RooRealVar     sigma1("sigma1"   ,"sigma",0.0,2.0) ;
                RooGaussian gaussian1("gaussian1","Gaussian signal model",mm2Temp,mean1,sigma1) ;
                RooRealVar      mean2("mean2"    ,"mean" ,mm2LowerTemp,mm2UpperTemp) ;
                RooRealVar     sigma2("sigma2"   ,"sigma",0.1,2.0) ;
                RooGaussian gaussian2("gaussian2","Gaussian signal model",mm2Temp,mean2,sigma2) ;
                RooDataHist rdhSignal("rdhSignal","rfhSignal",mm2Temp,&hSignal) ;
                RooRealVar gaussianFraction("gaussianFraction","Gaussian Fraction",0.0,1.0) ;
                RooAddPdf gaussians("gaussians","gaussian",RooArgList(gaussian1,gaussian2),gaussianFraction) ;
                gaussians.fitTo(rdhSignal) ;
        
                mm2Mean[SignalMode]       =  mean1.getVal()   ;
                mm2MeanError[SignalMode]  =  mean1.getError() ;
                mm2Sigma[SignalMode]      = sigma1.getVal()   ;
                mm2SigmaError[SignalMode] = sigma1.getError() ;
        
                cRoofit->cd() ;
                TH1F* hFitGaussian1 = (TH1F*)gaussians.createHistogram("hGaussian",mm2Temp) ;
                TH1F* hFitGaussian2 = (TH1F*)gaussian2.createHistogram("hGaussian",mm2Temp) ;
                hFitGaussian1->Scale(hSignal.GetSumOfWeights()/hFitGaussian1->GetSumOfWeights()) ;
        
                chi2 = 0 ;
                bins = 0 ;
                for(Int_t bin=1 ; bin<hFitGaussian1->GetNbinsX()+1 ; bin++)
                {
                  if((hFitGaussian1->GetBinContent(bin)+hSignal.GetBinContent(bin))==0) continue ;
                  Float_t diff  = hFitGaussian1->GetBinContent(bin) - hSignal.GetBinContent(bin) ;
                  Float_t sigma = hFitGaussian1->GetBinError(bin)*hFitGaussian1->GetBinError(bin) + hSignal.GetBinError(bin)*hSignal.GetBinError(bin) ;
                  chi2 += diff*diff/sigma ;
                  bins++ ;
                }
                cout << endl << endl << chi2 << " / " << bins << endl << endl ;
        
                hFitGaussian2->Scale((1-gaussianFraction.getVal())*hSignal.GetSumOfWeights()/hFitGaussian2->GetSumOfWeights()) ;
                SignalPDFGaussianPrintName[SignalMode] = TString(TString("Roofit_")+FitName[fit]+"_SignalPDF_Gaussian") ;
                hFitGaussian1->SetLineWidth(2) ;
                hFitGaussian2->SetLineWidth(2) ;
                hSignal.GetYaxis()->SetLabelSize(0.04) ;
                hSignal.SetMinimum(0) ;
                hSignal.Draw() ;
                hFitGaussian2->Draw("sames:C") ;
                hFitGaussian1->Draw("sames:C") ;
                PrintCanvas(cRoofit,SignalPDFGaussianPrintName[SignalMode]) ;
                delete hFitGaussian1 ;
                delete hFitGaussian2 ;
                cRoofit->Clear() ;
        
                // Now shift the mean
                TH1F hShift("hShift","hShift",varNBins[varX[SignalMode]],varLower[varX[SignalMode]],varUpper[varX[SignalMode]]) ;
                //Float_t meanShift = mm2Mean[SignalMode] ;
                Float_t meanShift = 0.241848 - 0.248814 ;
                tree->Draw(TString(TString("mm2+")+FloatToString(meanShift)+">>hShift"), TString(TString("SignalMode==")+(long)SignalMode+"&&type==1" +EExtraCutSignalMode[SignalMode]),TString("goff")) ;
                hShift.Draw() ;
                PrintCanvas(cRoofit,"hShift") ;
                cout << "hShift: " << hShift.GetSumOfWeights() << endl ;
                RooDataHist rdhShift("rdhShift","rdhShift",*rrv[varX[SignalMode]],&hShift)  ;
                RooHistPdf  rhpShift("rhpShift","rhpShift",*rrv[varX[SignalMode]],rdhShift) ;
                RooAddPdf shiftModel("shiftModel","",RooArgList(rhpShift,*rhpPDF[3]),RooArgList(*fPDF[1])) ;
                shiftModel.fitTo(*rdhDataset) ;
                fittedYieldShiftMC[SignalMode]      = fPDF[1]->getVal()*integral[0] ;
                fittedYieldErrorShiftMC[SignalMode] = fPDF[1]->getError()*integral[0] ;
        
                RooAddPdf shiftModelData("shiftModelData","",RooArgList(rhpShift,*rhpPDF[16],rhpSemileptonic),RooArgList(*fPDF[10],*fPDF[16])) ;
                //RooAddPdf shiftModelData("shiftModelData","",RooArgList(rhpShift,*rhpPDF[3]),RooArgList(*fPDF[10])) ;
                fPDF[10]->setVal(0.15) ;
                shiftModelData.fitTo(*rdhPDF[10]) ;
                fittedYieldShiftData[SignalMode]      = fPDF[10]->getVal()*integral[10] ;
                fittedYieldErrorShiftData[SignalMode] = fPDF[10]->getError()*integral[10] ;
        
                TH1F* hFitShift = (TH1F*)shiftModel.createHistogram("hShift",*rrv[varX[SignalMode]]) ;
                hFitShift->Scale(h1PDF[0]->GetSumOfWeights()/hFitShift->GetSumOfWeights()) ;
                shiftPrintName[SignalMode] = TString(TString("Roofit_")+FitName[fit]+"_shift_Residuals") ;
                MakeResidualsPlot(rrv[varX[SignalMode]], shiftModel, rdhPDF[0], &rhpShift, NULL, SignalModeColor[SignalMode], 0, h1PDF[0], hFitShift, kTRUE, cRoofit, shiftPrintName[SignalMode]) ;
                delete hFitShift ;
        
                TH1F* hFitShiftData = (TH1F*)shiftModelData.createHistogram("hShiftData",*rrv[varX[SignalMode]]) ;
                hFitShiftData->Scale(h1PDF[10]->GetSumOfWeights()/hFitShiftData->GetSumOfWeights()) ;
                shiftPrintNameData[SignalMode] = TString(TString("RoofitData_")+FitName[fit]+"_shift_Residuals") ;
                MakeResidualsPlot(rrv[varX[SignalMode]], shiftModelData, rdhPDF[10], &rhpShift, NULL, SignalModeColor[SignalMode], 0, h1PDF[10], hFitShiftData, kTRUE, cRoofit, shiftPrintNameData[SignalMode]) ;
                delete hFitShiftData ;
        
                // Now smear the distribution
                TH1F hSmear("hSmear","hSmear",varNBins[varX[SignalMode]],varLower[varX[SignalMode]],varUpper[varX[SignalMode]]) ;
                TH1F hSmearSmall("hSmearSmall","hSmearSmall",binsTemp,mm2LowerTemp,mm2UpperTemp) ;
                TRandom3 random ;
                random.SetSeed(1) ;
                Int_t   tSignalMode ;
                Float_t tEExtra     ;
                Float_t tMm2        ;
                Int_t   tType       ;
                tree->SetBranchAddress("SignalMode",&tSignalMode) ;
                tree->SetBranchAddress("eextra"    ,&tEExtra    ) ;
                tree->SetBranchAddress("type"      ,&tType      ) ;
                tree->SetBranchAddress("mm2"       ,&tMm2       ) ;
                for(Int_t entry=0 ; entry<tree->GetEntries() ; entry++)
                {
                  tree->GetEntry(entry) ;
                  if(SignalMode!=tSignalMode) continue ;
                  if(tType!=1)  continue ;
                  if(tEExtra>1 && SignalMode==0) continue ;
                  //Float_t sigma = 0.1*mm2Signal[SignalMode] ;
                  Float_t sigma = 0.0158 ;
                  Float_t randomMm2 = tMm2+random.Gaus(0,sigma) ;
                  hSmear.Fill(randomMm2) ;
                  hSmearSmall.Fill(randomMm2) ;
                }
                TCanvas cSmear("cSmear","",100,100,1000,500) ;
                cSmear.Divide(2,1) ;
                cSmear.cd(1) ;
                Int_t fillStyle[4] = { 3445 , 3006 , 3454 , 3007 } ;
                ThesisTH1F(&hSignal) ;
                hSignal.SetFillStyle(fillStyle[SignalMode]) ;
                hSignal.Draw() ;
                cSmear.cd(2) ;
                hSmearSmall.SetFillColor(SignalModeColor[SignalMode]) ;
                if(SignalMode==0) hSmearSmall.GetXaxis()->SetTitle("m_{recoil}^{2}(DKX#gamma#mu) (GeV^{2}c^{ -4})") ;
                if(SignalMode==1) hSmearSmall.GetXaxis()->SetTitle("m_{recoil}^{2}(DKX#gammae) (GeV^{2}c^{ -4})") ;
                if(SignalMode==0) hSignal.GetXaxis()->SetTitle("m_{recoil}^{2}(DKX#gamma#mu) (GeV^{2}c^{ -4})") ;
                if(SignalMode==1) hSignal.GetXaxis()->SetTitle("m_{recoil}^{2}(DKX#gammae) (GeV^{2}c^{ -4})") ;
                hSmearSmall.GetYaxis()->SetTitle(TString("entries per 0.008 GeV^{2}c^{ -4}")) ;
                hSmearSmall.Draw() ;
                PrintCanvas(&cSmear,"smearing") ;
                cSmear.Clear() ;
        
                TCanvas cSmearSquare("cSmearSquare","",100,100,500,500) ;
                DRecoResiduals* residualsSmear = new DRecoResiduals("residualsSmear",&hSignal,&hSmearSmall) ;
                residualsSmear->H1NormResiduals->SetMarkerStyle(21) ;
                
                RooPlot* frameSmear = rrv[varX[SignalMode]]->frame() ;                                
                residualsSmear->MakeFitPlotWithResiduals(&cSmearSquare,frameSmear) ;
                residualsSmear->H1NormResiduals->GetYaxis()->SetTitle("") ;
                cSmearSquare.cd(1) ;
                hSignal.Draw() ;
                hSmearSmall.Draw("p:sames") ;
                TLegend smearLegend = MakeLegend(0.7,0.8,0.9,0.9) ;
                smearLegend.AddEntry(&hSignal    ,"Raw distribution"    ,"f") ;
                smearLegend.AddEntry(&hSmearSmall,"Smeared distribution","p") ;
                smearLegend.Draw() ;
                cSmearSquare.Update() ;
                PrintCanvas(&cSmearSquare,"smearingSuperimposed") ;
                cSmearSquare.Clear() ;
        
                RooDataHist rdhSmear("rdhSmear","rdhSmear",*rrv[varX[SignalMode]],&hSmear)  ;
                RooHistPdf  rhpSmear("rhpSmear","rhpSmear",*rrv[varX[SignalMode]],rdhSmear) ;
                RooAddPdf smearModel("smearModel","",RooArgList(rhpSmear,*rhpPDF[3]),RooArgList(*fPDF[1])) ;
                smearModel.fitTo(*rdhDataset) ;
                fittedYieldSmearMC[SignalMode]      = fPDF[1]->getVal()*integral[0] ;
                fittedYieldErrorSmearMC[SignalMode] = fPDF[1]->getError()*integral[0] ;
        
                fPDF[10]->setVal(0.15) ;
                RooAddPdf smearModelData("smearModelData","",RooArgList(rhpSmear,*rhpPDF[16],rhpSemileptonic),RooArgList(*fPDF[10],*fPDF[16])) ;
                cout << "smearStart" << endl ;
                smearModelData.fitTo(*rdhPDF[10]) ;
                cout << "smearEnd" << endl ;
                fittedYieldSmearData[SignalMode]      = fPDF[10]->getVal()*integral[10] ;
                fittedYieldErrorSmearData[SignalMode] = fPDF[10]->getError()*integral[10] ;
        
                TH1F* hFitSmear = (TH1F*)smearModel.createHistogram("hSmear",*rrv[varX[SignalMode]]) ;
                hFitSmear->Scale(h1PDF[0]->GetSumOfWeights()/hFitSmear->GetSumOfWeights()) ;
                smearPrintName[SignalMode] = TString(TString("Roofit_")+FitName[fit]+"_smear_Residuals") ;
                MakeResidualsPlot(rrv[varX[SignalMode]], smearModel, rdhPDF[0], &rhpSmear, NULL, SignalModeColor[SignalMode], 0, h1PDF[0], hFitSmear, kTRUE, cRoofit, smearPrintName[SignalMode]) ;
                delete hFitSmear ;
                TH1F* hFitSmearData = (TH1F*)smearModelData.createHistogram("hSmearData",*rrv[varX[SignalMode]]) ;
                hFitSmearData->Scale(h1PDF[10]->GetSumOfWeights()/hFitSmearData->GetSumOfWeights()) ;
                smearPrintNameData[SignalMode] = TString(TString("RoofitData_")+FitName[fit]+"_smear_Residuals") ;
                MakeResidualsPlot(rrv[varX[SignalMode]], smearModelData, rdhPDF[10], &rhpSmear, NULL, SignalModeColor[SignalMode], 0, h1PDF[10], hFitSmearData, kTRUE, cRoofit, smearPrintNameData[SignalMode]) ;
                delete hFitSmearData ;  
              }
              if(SignalMode>1)
              {
                cout << endl << endl << "Expotential model" << endl << endl ;
                RooRealVar decay("decay","decay",-5.0,-10.0,0.0) ;
                RooExponential exponential("exponential","exponential",*rrv[varX[SignalMode]],decay) ;
                TH1F hZeroBin("hZeroBin","",varNBins[varX[SignalMode]],varLower[varX[SignalMode]],varUpper[varX[SignalMode]]) ;
                hZeroBin.Fill(0.0,1.0) ;
                RooDataHist rdhZeroBin("rdhZeroBin","",*rrv[varX[SignalMode]],&hZeroBin) ;
                RooHistPdf  rhpZeroBin("rhpZeroBin","",*rrv[varX[SignalMode]],rdhZeroBin) ;
                RooRealVar fZeroBin("fZeroBin","",0.1,0.0,1.0) ;
                RooAddPdf   signalModel("signalModel","",RooArgList(exponential,rhpZeroBin),RooArgList(fZeroBin)) ;
        
                //RooAddPdf exponentialModel("exponentialModel","",RooArgList(exponentialSignalModel,*rhpPDF[3]),RooArgList(*fPDF[1])) ;
                RooRealVar fExponential("fExponential","fExponential",0.1,0.0,1.0) ;
                RooAddPdf exponentialModel("exponentialModel","",RooArgList(signalModel,*rhpPDF[3]),RooArgList(fExponential)) ;
                exponentialModel.fitTo(*rdhPDF[0]) ;
                fittedYieldExponentialMC[SignalMode]      = fExponential.getVal()*integral[0] ;
                fittedYieldErrorExponentialMC[SignalMode] = fExponential.getError()*integral[0] ;
                TH1F* hFitExponential       = (TH1F*)exponentialModel.createHistogram("hExponential",*rrv[varX[SignalMode]]) ;
                hFitExponential->Scale(h1PDF[0]->GetSumOfWeights()/hFitExponential->GetSumOfWeights()) ;
                exponentialPrintName[SignalMode] = TString(TString("Roofit_")+FitName[fit]+"_exponential_Residuals") ;
                TCanvas cExponential("cExponential","",100,100,500,500) ;
                MakeResidualsPlot(rrv[varX[SignalMode]], exponentialModel, rdhPDF[0], (RooAbsPdf*)&signalModel, NULL, SignalModeColor[SignalMode], 0, h1PDF[0], hFitExponential, kTRUE, &cExponential, exponentialPrintName[SignalMode]) ;
                delete hFitExponential ;
        
                RooRealVar decayData("decayData","decayData",decay.getVal(),decay.getVal()) ;
                RooExponential exponentialData("exponentialData","exponentialData",*rrv[varX[SignalMode]],decayData) ;
                RooAddPdf signalModelData("signalModel","",RooArgList(exponentialData,rhpZeroBin),RooArgList(fZeroBin)) ;
                RooAddPdf exponentialModelData("exponentialModelData","",RooArgList(signalModelData,*rhpPDF[16],rhpSemileptonic),RooArgList(*fPDF[10],*fPDF[16])) ;
                exponentialModelData.fitTo(*rdhPDF[10]) ;
                fittedYieldExponentialData[SignalMode]      = fPDF[10]->getVal()*integral[10] ;
                fittedYieldErrorExponentialData[SignalMode] = fPDF[10]->getError()*integral[10] ;
        
                TH1F* hFitExponentialData       = (TH1F*)exponentialModelData.createHistogram("hExponentialData",*rrv[varX[SignalMode]]) ;
                hFitExponentialData->Scale(h1PDF[10]->GetSumOfWeights()/hFitExponentialData->GetSumOfWeights()) ;
                exponentialPrintNameData[SignalMode] = TString(TString("RoofitData_")+FitName[fit]+"_exponential_Residuals") ;
                MakeResidualsPlot(rrv[varX[SignalMode]], exponentialModelData, rdhPDF[10], (RooAbsPdf*)&signalModel, NULL,SignalModeColor[SignalMode], 0, h1PDF[10], hFitExponentialData, kTRUE, &cExponential, exponentialPrintNameData[SignalMode]) ;
                delete hFitExponentialData ;
        
              }
            }
          }
      
          // Toys
          if(doToys)
          {
            srand(1) ;
            TRandom3 toyRandom ;
            toyRandom.SetSeed(1) ;
            // Make pull histogram
            TString histogramName = TString(TString("h")+SignalModeName[SignalMode]+ratioName[ratio]) ;
            hPull[SignalMode][ratio] = new TH1F(histogramName,histogramName,pullNBins,pullLower,pullUpper) ;
            hPull[SignalMode][ratio]->GetXaxis()->SetTitle(TString("Pull")) ;
            hPull[SignalMode][ratio]->GetYaxis()->SetTitle(TString("Entries")) ;
            
            TH1F* hToy = (TH1F*)h1Dataset->Clone("hToy") ;
            Int_t zMaxInt = (Int_t)h1Dataset->GetMaximum() ;
            for(Int_t toy=0 ; toy<NToys ; toy++)
            {
              // Empty toy histogram and fill with random contents
              hToy->Reset() ;
              Int_t counter = 0 ;
              Float_t numberOfEvents = h1Dataset->GetSumOfWeights() + toyRandom.Gaus(0,sqrt(h1Dataset->GetSumOfWeights())) ;
              while(counter<numberOfEvents)
              {
                Int_t binX = 1+rand()%(varNBins[varX[SignalMode]]) ;
                Float_t z = 0.0001*(rand()%(zMaxInt*10000)) ;
                if(z<h1Dataset->GetBinContent(binX))
                {
                  hToy->SetBinContent(binX,hToy->GetBinContent(binX)+1) ;
                  counter++ ;
                }
              }
              //// h1Dataset may not have integer sum of weights, so scale hToy
              //hToy->Scale(h1Dataset->GetSumOfWeights()/hToy->GetSumOfWeights()) ;
              RooDataHist rdhToy("rdhToy","rdhToy",*rrv[varX[SignalMode]],hToy) ;
              TString fractionName = TString(TString("fToy")+SignalModeName[SignalMode]+ratioName[ratio]+(long)toy) ;
              RooRealVar fToy(fractionName,fractionName,0.25,0.0,1.0) ;
              //RooArgList pdfsToy(*rhpPDF[1],*rhpPDF[3]) ;
              RooArgList pdfsToy(*rhpPDF[2],*rhpPDF[3]) ;
              RooArgList fractionsToy(fToy) ;
              RooAddPdf modelToy("modelToy","ModelToy",pdfsToy,fractionsToy) ;
              modelToy.fitTo(rdhToy) ;
              Float_t pull = (fToy.getVal()-trueFraction)/(fToy.getError()) ;
              if(fToy.getVal()>fToy.getError()) hPull[SignalMode][ratio]->Fill(pull) ;

              if(toy==0)
              {
                Bool_t success = (fToy.getVal()>fToy.getError()) ;
                Float_t weight1Toy = (integral[1]!=0) ? fToy.getVal()*totalYield/integral[1]              : 0 ;
                Float_t weight3Toy = (integral[3]!=0) ? (totalYield-fToy.getVal()*totalYield)/integral[3] : 0 ;
                TH1F* hFitToyX = new TH1F("hFitToyX","hFitToyX",varNBins[varX[SignalMode]],varLower[varX[SignalMode]],varUpper[varX[SignalMode]]) ;
                hFitToyX->Add(h2PDF[1],weight1Toy) ;
                hFitToyX->Add(h2PDF[3],weight3Toy) ;
                hFitToyX->Scale(hToy->GetSumOfWeights()/hFitToyX->GetSumOfWeights()) ;
                ToyPrintName[SignalMode][ratio] = TString(TString("Roofit_")+ratioName[ratio]+"_Toy_Residuals") ;
                MakeResidualsPlot(rrv[varX[SignalMode]], modelToy, &rdhToy, rhpPDF[1], NULL, SignalModeColor[SignalMode], -1, hToy,  hFitToyX, success, cRoofit, ToyPrintName[SignalMode][ratio]) ;
                delete hFitToyX ;
              }
            }
            delete hToy ;

            // Fit pull distribution
            RooDataHist rdhPull("rdhPull","rdhPull",pullVar,hPull[SignalMode][ratio]) ;
            RooRealVar  mean("mean","mean",0.0,pullLower,pullUpper) ;
            RooRealVar sigma("sigma","sigma",1.0,0.0,pullUpper) ;
            RooGaussian gaussPull("gaussPull","gaussPull",pullVar,mean,sigma) ;
            gaussPull.fitTo(rdhPull) ;
            pullMean[SignalMode][ratio]       =  mean.getVal()   ;
            pullMeanError[SignalMode][ratio]  =  mean.getError() ;
            pullSigma[SignalMode][ratio]      = sigma.getVal()   ;
            pullSigmaError[SignalMode][ratio] = sigma.getError() ;
            pullIntegral[SignalMode][ratio]   = (Int_t)hPull[SignalMode][ratio]->GetSumOfWeights() ;
      
            // Plot results
            TH1F hLine("hLine","hLine",100*pullNBins,pullLower,pullUpper) ;
            hLine.SetLineWidth(1) ;
            hLine.SetLineColor(kBlack) ;
            for(Int_t bin=1 ; bin<100*pullNBins+1 ; bin++){ hLine.SetBinContent(bin,-1) ; }
            hLine.Fill(0.0,100000.0) ;

            TH1F* hBlank = new TH1F("hBlank","",0,1,1) ;
            hBlank->SetLineColor(kWhite) ;
            TLegend label = MakeLegend(0.65,0.75,0.9,0.9) ;
            label.SetTextSize(0.04) ;
            label.SetBorderSize(0) ;
            label.AddEntry(hBlank, TString(TString("#mu = ")    + FloatToString( pullMean[SignalMode][ratio],3) + " #pm " + FloatToString( pullMeanError[SignalMode][ratio],3) + "   "),"l") ;
            label.AddEntry(hBlank, TString(TString("#sigma = ") + FloatToString(pullSigma[SignalMode][ratio],3) + " #pm " +  FloatToString(pullSigmaError[SignalMode][ratio],3) + "   "),"l") ;
        
            RooPlot* framePull = pullVar.frame() ;
            TH1F* hFitOutPull = (TH1F*)gaussPull.createHistogram(TString(TString("dataHistPull")),pullVar) ;
            hFitOutPull->Scale(pullIntegral[SignalMode][ratio]) ;
            rdhPull.plotOn(framePull, DataError(RooAbsData::SumW2)) ;
            gaussPull.plotOn(framePull) ;
            DRecoResiduals* residualsPull = new DRecoResiduals("hResidsPull", hPull[SignalMode][ratio], hFitOutPull) ;
            residualsPull->MakeFitPlotWithResiduals(cRoofit, framePull) ;
            cRoofit->cd(1) ;
            label.Draw() ;
            hLine.Draw("sames") ;
            hFitOutPull->Draw("a:sames") ;
            PullPrintName[SignalMode][ratio] = TString(TString("PullToy_") + ratioName[ratio] + "_Residuals") ;
            PrintCanvas(cRoofit,PullPrintName[SignalMode][ratio]) ;
            cRoofit->Clear() ;
            delete hBlank ;
            delete framePull ;
            delete hFitOutPull ;
            delete residualsPull ;
          }
          delete  DRecoModel ;
          delete  DRecoModelData ;
          delete  model ;
          delete  modelData ;
        }
        delete pdfs ;
        delete fractions ;
        delete rdhDataset ;
        delete rhpDataset ;
        if(h1Dataset) delete h1Dataset ;
      }
      for(Int_t pdf=0 ; pdf<NPDF ; pdf++)
      {
        if(h1PDF[pdf]) delete h1PDF[pdf] ;
        if(h2PDF[pdf]) delete h2PDF[pdf] ;
        delete rdhPDF[pdf] ;
        delete rhpPDF[pdf] ;
        delete   fPDF[pdf] ;
      }
    }

    cout << endl << endl ;

    // Make summary plots
    // Runs
    TString histogramName ;
    histogramName = TString(TString("hTrueRunSummary")+SignalModeName[SignalMode]) ;
    hTrueRunSummary[SignalMode] = new TH1I(histogramName,histogramName,NRuns,0,NRuns) ;
    hTrueRunSummary[SignalMode]->GetXaxis()->SetTitle(TString("Running period")) ;
    hTrueRunSummary[SignalMode]->GetYaxis()->SetTitle(TString("Expected yield")) ;
    hTrueRunSummary[SignalMode]->GetYaxis()->SetLabelSize(0.04) ;
    hTrueRunSummary[SignalMode]->SetFillColor(SignalModeColor[SignalMode]) ;
    hTrueRunSummary[SignalMode]->SetMinimum(0) ;

    histogramName = TString(TString("hFitRunSummary")+SignalModeName[SignalMode]) ;
    hFitRunSummary[SignalMode] = new TH1I(histogramName,histogramName,NRuns,0,NRuns) ;
    hFitRunSummary[SignalMode]->GetXaxis()->SetTitle(TString("Running period")) ;
    hFitRunSummary[SignalMode]->GetYaxis()->SetTitle(TString("Expected yield")) ;
    hFitRunSummary[SignalMode]->GetYaxis()->SetLabelSize(0.04) ;
    hFitRunSummary[SignalMode]->SetMinimum(0) ;
    
    histogramName = TString(TString("hLumiRunSummary")+SignalModeName[SignalMode]) ;
    hLumiRunSummary[SignalMode] = new TH1I(histogramName,histogramName,NRuns,0,NRuns) ;
    hLumiRunSummary[SignalMode]->GetXaxis()->SetTitle(TString("Running period")) ;
    hLumiRunSummary[SignalMode]->GetYaxis()->SetTitle(TString("Expected yield")) ;
    hLumiRunSummary[SignalMode]->GetYaxis()->SetLabelSize(0.04) ;
    hLumiRunSummary[SignalMode]->SetMarkerStyle(24) ;
    hLumiRunSummary[SignalMode]->SetMarkerColor(kRed) ;
    hLumiRunSummary[SignalMode]->SetMarkerSize(3) ;
    hLumiRunSummary[SignalMode]->SetMinimum(0) ;
    
    for(Int_t bin=1 ; bin<NRuns+1 ; bin++)
    {
      hTrueRunSummary[SignalMode]->GetXaxis()->SetBinLabel(bin,runTitle[bin-1]) ;
      hFitRunSummary[SignalMode]->GetXaxis()->SetBinLabel( bin,runTitle[bin-1])  ;
      hLumiRunSummary[SignalMode]->GetXaxis()->SetBinLabel(bin,runTitle[bin-1]) ;

      hTrueRunSummary[SignalMode]->SetBinContent(bin, trueYield[SignalMode][(NRatios-1)/2][bin-1]) ;
      hLumiRunSummary[SignalMode]->SetBinContent(bin, trueYield[SignalMode][(NRatios-1)/2][NRuns-1]*luminosity[bin-1]/luminosity[NRuns-1]) ;
      if(fittedYield[SignalMode][(NRatios-1)/2][bin-1]>0.1)
      {
        hFitRunSummary[SignalMode]->SetBinContent( bin,fittedYield[SignalMode][(NRatios-1)/2][bin-1]) ;
        hFitRunSummary[SignalMode]->SetBinError( bin,fittedYieldError[SignalMode][(NRatios-1)/2][bin-1]) ;
      }
    }
    hAxisRunSummary[SignalMode] = (TH1I*)hTrueRunSummary[SignalMode]->Clone(TString(TString("hRunSummaryAxis")+SignalModeName[SignalMode])) ;
    
    TLegend legendRun = MakeLegend(0.2,0.9,0.5,0.65) ;  
    legendRun.AddEntry(hTrueRunSummary[SignalMode],"True yield"               ,"f") ;
    legendRun.AddEntry( hFitRunSummary[SignalMode],"Fitted yield"             ,"p") ;
    legendRun.AddEntry(hLumiRunSummary[SignalMode],"Lumi weighted expectation","p") ;
    
    cRun->cd() ;
    if(hFitRunSummary[SignalMode]->GetSumOfWeights()>0.1)
    {
      hFitRunSummary[SignalMode]->Draw("pe") ;
      hTrueRunSummary[SignalMode]->Draw("sames") ;
      hLumiRunSummary[SignalMode]->Draw("p:sames") ;
      hFitRunSummary[SignalMode]->Draw("same:pe") ;
      hAxisRunSummary[SignalMode]->Draw("same:axis") ;
    }
    else
    {
      hFitRunSummary[SignalMode]->Draw("axis") ;
    }
    legendRun.Draw() ;
    RunSummaryPrintName[SignalMode] = TString("RunSummary") ;
    PrintCanvas(cRun,RunSummaryPrintName[SignalMode]) ;
    cRun->Clear() ;

    // NXR
    histogramName = TString(TString("hTrueNXRSummary")+SignalModeName[SignalMode]) ;
    hTrueNXRSummary[SignalMode] = new TH1I(histogramName,histogramName,NNXR,0,NNXR) ;
    hTrueNXRSummary[SignalMode]->GetXaxis()->SetTitle(TString("n_{X}^{R}")) ;
    hTrueNXRSummary[SignalMode]->GetYaxis()->SetTitle(TString("Expected yield")) ;
    hTrueNXRSummary[SignalMode]->GetYaxis()->SetLabelSize(0.04) ;
    hTrueNXRSummary[SignalMode]->SetFillColor(SignalModeColor[SignalMode]) ;
    hTrueNXRSummary[SignalMode]->SetMinimum(0) ;

    histogramName = TString(TString("hFitNXRSummary")+SignalModeName[SignalMode]) ;
    hFitNXRSummary[SignalMode] = new TH1I(histogramName,histogramName,NNXR,0,NNXR) ;
    hFitNXRSummary[SignalMode]->GetXaxis()->SetTitle(TString("n_{X}^{R}")) ;
    hFitNXRSummary[SignalMode]->GetYaxis()->SetTitle(TString("Expected yield")) ;
    hFitNXRSummary[SignalMode]->GetYaxis()->SetLabelSize(0.04) ;
    hFitNXRSummary[SignalMode]->SetMinimum(0) ;
    
    for(Int_t bin=1 ; bin<NNXR+1 ; bin++)
    {
      hTrueNXRSummary[SignalMode]->GetXaxis()->SetBinLabel(bin,nXRTitle[bin-1]) ;
      hFitNXRSummary[SignalMode]->GetXaxis()->SetBinLabel( bin,nXRTitle[bin-1])  ;

      hTrueNXRSummary[SignalMode]->SetBinContent(bin,       trueYield[SignalMode][(NRatios-1)/2][NRuns+bin-1]) ;
      if(fittedYield[SignalMode][(NRatios-1)/2][NRuns+bin-1]>0.1)
      {
        hFitNXRSummary[SignalMode]->SetBinContent( bin,     fittedYield[SignalMode][(NRatios-1)/2][NRuns+bin-1]) ;
        hFitNXRSummary[SignalMode]->SetBinError(   bin,fittedYieldError[SignalMode][(NRatios-1)/2][NRuns+bin-1]) ;
      }
    }
    hAxisNXRSummary[SignalMode] = (TH1I*)hTrueNXRSummary[SignalMode]->Clone(TString(TString("hAxisNXRSummary")+SignalModeName[SignalMode])) ;

    TLegend legendNXR = MakeLegend(0.2,0.9,0.5,0.8) ;  
    legendNXR.AddEntry(hTrueNXRSummary[SignalMode],"True yield"  ,"f") ;
    legendNXR.AddEntry( hFitNXRSummary[SignalMode],"Fitted yield","p") ;

    cRoofit->cd() ;
    if(hFitNXRSummary[SignalMode]->GetSumOfWeights()>0.1)
    {
      hFitNXRSummary[SignalMode]->Draw("pe") ;
      hTrueNXRSummary[SignalMode]->Draw("sames") ;
      hFitNXRSummary[SignalMode]->Draw("sames:pe") ;
      hFitNXRSummary[SignalMode]->Draw("same") ;
      hAxisNXRSummary[SignalMode]->Draw("same:axis") ;
    }
    else
    {
      hFitNXRSummary[SignalMode]->Draw("axis") ;
    }
    legendNXR.Draw() ;
    NXRSummaryPrintName[SignalMode] = TString("NXRSummary") ;
    PrintCanvas(cRoofit,NXRSummaryPrintName[SignalMode]) ;
    cRoofit->Clear() ;

    // Ratios
    histogramName = TString(TString("hTrueRatioSummary")+SignalModeName[SignalMode]) ;
    hTrueRatioSummary[SignalMode] = new TH1I(histogramName,histogramName,NRatios,1,NRatios) ;
    hTrueRatioSummary[SignalMode]->GetXaxis()->SetTitle(TString("n_{signal}/n_{signal}^{nominal}")) ;
    hTrueRatioSummary[SignalMode]->GetYaxis()->SetTitle(TString("Expected yield")) ;
    hTrueRatioSummary[SignalMode]->GetYaxis()->SetLabelSize(0.04) ;
    hTrueRatioSummary[SignalMode]->SetFillColor(SignalModeColor[SignalMode]) ;
    hTrueRatioSummary[SignalMode]->SetMinimum(0) ;

    histogramName = TString(TString("hFitRatioSummary")+SignalModeName[SignalMode]) ;
    hFitRatioSummary[SignalMode] = new TH1I(histogramName,histogramName,NRatios,1,NRatios) ;
    hFitRatioSummary[SignalMode]->GetXaxis()->SetTitle(TString("n_{signal}/n_{signal}^{nominal}")) ;
    hFitRatioSummary[SignalMode]->GetYaxis()->SetTitle(TString("Expected yield")) ;
    hFitRatioSummary[SignalMode]->GetYaxis()->SetLabelSize(0.04) ;
    hFitRatioSummary[SignalMode]->SetMinimum(0) ;
    
    for(Int_t bin=1 ; bin<NRatios+1 ; bin++)
    {
      hTrueRatioSummary[SignalMode]->GetXaxis()->SetBinLabel(bin,ratioTitle[bin-1]) ;
      hFitRatioSummary[SignalMode]->GetXaxis()->SetBinLabel( bin,ratioTitle[bin-1])  ;

      hTrueRatioSummary[SignalMode]->SetBinContent(bin,       trueYield[SignalMode][bin-1][NRuns-1]) ;
      hFitRatioSummary[SignalMode]->SetBinContent( bin,     fittedYield[SignalMode][bin-1][NRuns-1]) ;
      hFitRatioSummary[SignalMode]->SetBinError(   bin,fittedYieldError[SignalMode][bin-1][NRuns-1]) ;
    }
    hAxisRatioSummary[SignalMode] = (TH1I*)hTrueRatioSummary[SignalMode]->Clone(TString(TString("hAxisRatioSummary")+SignalModeName[SignalMode])) ;

    TLegend legendRatio = MakeLegend(0.6,0.5,0.9,0.25) ;  
    legendRatio.AddEntry(hTrueRatioSummary[SignalMode],"True yield"  ,"f") ;
    legendRatio.AddEntry( hFitRatioSummary[SignalMode],"Fitted yield","p") ;

    cRoofit->cd() ;
    if(hFitRatioSummary[SignalMode]->GetSumOfWeights()>0.5)
    {
      hFitRatioSummary[SignalMode]->Draw("pe") ;
      hTrueRatioSummary[SignalMode]->Draw("sames") ;
      hFitRatioSummary[SignalMode]->Draw("sames:pe") ;
      hAxisRatioSummary[SignalMode]->Draw("same:axis") ;
    }
    else
    {
      hFitRatioSummary[SignalMode]->Draw("axis") ;
    }
    legendRatio.Draw() ;
    RatioSummaryPrintName[SignalMode] = TString("RatioSummary") ;
    PrintCanvas(cRoofit,RatioSummaryPrintName[SignalMode]) ;
    cRoofit->Clear() ;
    
    // Results in data
    fBADData.open(TString(TString("../AWG90/aidanrc/BAD/note2170/Numerator")+SignalModeName[SignalMode]+"Results.tex")) ;
    fBADData << "\\subsection{Results in data for the $" << SignalModeNameLaTeX[SignalMode] <<"$}" << endl ;
    fBADData << "The yield obtained in the fit to data is $" << FloatToString(dataYield[SignalMode][NFit-1]) << " \\pm " << FloatToString(dataYieldError[SignalMode][NFit-1]) << "$ events.  Figure \\ref{fig:Numerator_Results_" << SignalModeName[SignalMode] << "} shows the fit for all running periods, and figures \\ref{fig:Numerator_results_" << RunName[0] << "_" << RunName[1] << "_" << SignalModeName[SignalMode] << "}-\\ref{fig:Numerator_results_" << RunName[NRuns-3] << "_" << RunName[NRuns-2] << "} show the results for each running period.  Table \\ref{tab:Numerator_Results_" << SignalModeName[SignalMode] << "} shows the yields as a function of running period." << endl ;
    
    fBADData << "\\begin{figure}" << endl ;
    fBADData << "  \\begin{center}" << endl ;
    fBADData << "    \\includegraphics[width=0.75\\textwidth]{plots/Numerator/" << RoofitDataPrintNameX[SignalMode][NFit-1] << "_" << SignalModeName[SignalMode] << EExtraCutString[EExtraCut] << ".eps}  " << endl ;
    fBADData << "  \\end{center}" << endl ;
    fBADData << "  \\caption[Yield in data for the $" << SignalModeNameLaTeX[SignalMode] << "$ mode.]{Yield by in data for the $" << SignalModeNameLaTeX[SignalMode] << "$ mode for all running periods.}" << endl ;
    fBADData << "  \\label{fig:Numerator_Results_" << SignalModeName[SignalMode] << "}" << endl ;
    fBADData << "\\end{figure}" << endl ;

    // Runs details
    if(false)
    {
      for(Int_t run=0 ; run<NRuns-1 ; run+=2)
      {
        fBADData << "\\begin{figure}" << endl ;
        fBADData << "  \\begin{center}" << endl ;
        fBADData << "    \\begin{tabular}{cc}" << endl ;
        fBADData << "    \\includegraphics[width=0.3\\textwidth]{plots/Numerator/" << RoofitDataPrintNameX[SignalMode][run]   << "_" << SignalModeName[SignalMode] << EExtraCutString[EExtraCut] << ".eps} &" << endl ;
        fBADData << "    \\includegraphics[width=0.3\\textwidth]{plots/Numerator/" << RoofitDataPrintNameX[SignalMode][run+1] << "_" << SignalModeName[SignalMode] << EExtraCutString[EExtraCut] << ".eps}" << endl ;
        fBADData << "    \\end{tabular}" << endl ;
        fBADData << "  \\caption[Yield in data for " << CombinedRunNameLaTeX[run/2] << " for the $" << SignalModeNameLaTeX[SignalMode] << "$ mode.]{Yield in data for " << runNameLaTeX[run] << " (left) and " << runNameLaTeX[run+1] << " (right) for the $" << SignalModeNameLaTeX[SignalMode] << "$ mode.  The empty histograms show the total PDF returned by the fit.  The shaded histograms show the signal component of the PDF returned by the fit." ;
        fBADData << "}" << endl ;
        fBADData << "  \\label{fig:Numerator_results_" << RunName[run] << "_" << RunName[run+1] << "_" << SignalModeName[SignalMode] << "}" << endl ;
        fBADData << "\\end{center}\\end{figure}" << endl ;
      }
    }

    fBADData << "\\begin{table}[bh]" << endl ;
    fBADData << "  \\begin{center}" << endl ;
    fBADData << "    \\begin{tabular}{cc}" << endl ;
    fBADData << "      \\hline" << endl ;
    fBADData << "    Running period & Fitted yield \\\\" << endl ;
    fBADData << "    \\hline" ;
    for(Int_t run=0 ; run<NRuns ; run++)
    {
      if(run==NRuns-1) fBADData << "    \\hline" << endl ;
      fBADData << "      " << runNameLaTeX[run] << " & $" ;
      if(fittedYield[SignalMode][(NRatios-1)/2][run]<0.0001){ fBADData << "0" ; }
      else{ fBADData << FloatToString(dataYield[SignalMode][run],2) ; }
      fBADData << " \\pm " ;
      fBADData << FloatToString(dataYieldError[SignalMode][run],2) ; 
      fBADData << "$ \\\\" << endl ;
    }
    fBADData << "    \\hline" << endl ;
    fBADData << "    \\end{tabular}" ;
    fBADData << "  \\label{tab:Numerator_Results_" << SignalModeName[SignalMode] << "}" << endl ;
    fBADData << "  \\caption[Yield by run for the $" << SignalModeNameLaTeX[SignalMode] << "$ mode]{Yield by running period in data for the $" << SignalModeNameLaTeX[SignalMode] << "$ mode for.}" << endl ;
    fBADData << "\\end{center}\\end{table}" << endl ;
    fBADData << "\\clearpage" << endl ;

    fBADData.close() ;

    // Runs
    fBAD << endl << "\\clearpage" << endl ;
    fBAD << "\\subsubsection{Fit crosschecks for the $" << SignalModeNameLaTeX[SignalMode] << "$ mode}" << endl ;
    fBAD << "\\label{sec:Numerator_RoofitCrosschecks_" << SignalModeName[SignalMode] <<  "}" << endl << endl ;
    fBAD << "Table \\ref{tab:Numerator_" << RunSummaryPrintName[SignalMode] << "_" << SignalModeName[SignalMode] << "} and figures \\ref{fig:Numerator_RunSummary_" << SignalModeName[SignalMode] << "}-\\ref{fig:Numerator_" << RunName[NRuns-1] << "_" << SignalModeName[SignalMode] << "} show the result of the fits for the $" << SignalModeNameLaTeX[SignalMode] << "$ mode as a function of running period for generic MC.  The fit results are consistent with the true number of signal events for all running periods." << endl ;

    // Runs table
    fBAD << "\\begin{table}[bh]" << endl ;
    fBAD << "  \\begin{center}" << endl ;
    fBAD << "    \\begin{tabular}{ccc}" << endl ;
    fBAD << "      \\hline" << endl ;
    fBAD << "    Running period & True yield & Fitted yield \\\\" << endl ;
    fBAD << "    \\hline" ;
    for(Int_t run=0 ; run<NRuns ; run++)
    {
      if(run==NRuns-1) fBAD << "    \\hline" << endl ;
      fBAD << "      " << runNameLaTeX[run] << " & $" ;
      if(trueYield[SignalMode][(NRatios-1)/2][run]<0.0001){ fBAD << "0" ; }
      else{ fBAD << FloatToString(trueYield[SignalMode][(NRatios-1)/2][run],2) ; }
      fBAD << "$ & $" ;
      if(fittedYield[SignalMode][(NRatios-1)/2][run]<0.0001){ fBAD << "0" ; }
      else{ fBAD << FloatToString(fittedYield[SignalMode][(NRatios-1)/2][run],2) ; }
      fBAD << " \\pm " ;
      fBAD << FloatToString(fittedYieldError[SignalMode][(NRatios-1)/2][run],2) ; 
      fBAD << "$ \\\\" << endl ;
    }
    fBAD << "    \\hline" << endl ;
    fBAD << "    \\end{tabular}" ;
    fBAD << "  \\label{tab:Numerator_" << RunSummaryPrintName[SignalMode] << "_" << SignalModeName[SignalMode] << "}" << endl ;
    fBAD << "  \\caption[Yield by run for the $" << SignalModeNameLaTeX[SignalMode] << "$ mode]{Yield by running period for the $" << SignalModeNameLaTeX[SignalMode] << "$ mode for generic MC.}" << endl ;
    fBAD << "\\end{center}\\end{table}" << endl ;    
    fBAD << "\\clearpage" << endl ;
    
    // Runs summary
    fBAD << "\\begin{figure}" << endl ;
    fBAD << "  \\begin{center}" << endl ;
    fBAD << "    \\includegraphics[width=0.75\\textwidth]{plots/Numerator/" << RunSummaryPrintName[SignalMode] << "_" << SignalModeName[SignalMode] << EExtraCutString[EExtraCut] << ".eps}  " << endl ;
    fBAD << "  \\end{center}" << endl ;
    fBAD << "  \\caption[Yield by run for the $" << SignalModeNameLaTeX[SignalMode] << "$ mode.]{Yield by running period for the $" << SignalModeNameLaTeX[SignalMode] << "$ mode for generic MC.}" << endl ;
    fBAD << "  \\label{fig:Numerator_RunSummary_" << SignalModeName[SignalMode] << "}" << endl ;
    fBAD << "\\end{figure}" << endl ;
    
    // Runs details
    for(Int_t run=0 ; run<NRuns-1 ; run+=2)
    {
      fBAD << "\\begin{figure}" << endl ;
      fBAD << "  \\begin{center}" << endl ;
      fBAD << "    \\begin{tabular}{cc}" << endl ;
      fBAD << "    \\includegraphics[width=0.3\\textwidth]{plots/Numerator/" << RoofitPrintNameX[SignalMode][run][(NRatios-1)/2] << "_" << SignalModeName[SignalMode] << EExtraCutString[EExtraCut] << ".eps} &" << endl ;
      fBAD << "    \\includegraphics[width=0.3\\textwidth]{plots/Numerator/" << RoofitPrintNameX[SignalMode][run+1][(NRatios-1)/2] << "_" << SignalModeName[SignalMode] << EExtraCutString[EExtraCut] << ".eps}" << endl ;
      fBAD << "    \\end{tabular}" << endl ;
      fBAD << "  \\caption[Yield for " << CombinedRunNameLaTeX[run/2] << " for the $" << SignalModeNameLaTeX[SignalMode] << "$ mode.]{Yield for " << runNameLaTeX[run] << " (left) and " << runNameLaTeX[run+1] << " (right) for the $" << SignalModeNameLaTeX[SignalMode] << "$ mode for generic MC.  The empty histograms show the total PDF returned by the fit.  The shaded histograms show the signal component of the PDF returned by the fit." ;
      if(SignalMode==2) fBAD << "  The green (upper) shaded histogram shows the contribution from $" << SignalModeNameLaTeX[0] << "$ events and the pink (lower) shaded histogram shows the contribution from $" << SignalModeNameLaTeX[2] << "$ events." ;
      fBAD << "}" << endl ;
      fBAD << "  \\label{fig:Numerator_" << RunName[run] << "_" << RunName[run+1] << "_" << SignalModeName[SignalMode] << "}" << endl ;
      fBAD << "\\end{center}\\end{figure}" << endl ;
    }
    // Run all
    Int_t run = NRuns-1 ;
    fBAD << "\\begin{figure}" << endl ;
    fBAD << "  \\begin{center}" << endl ;
    if(nDimensions[SignalMode]==1)
    {
      fBAD << "    \\includegraphics[width=0.8\\textwidth]{plots/Numerator/" << RoofitPrintNameX[SignalMode][run][(NRatios-1)/2] << "_" << SignalModeName[SignalMode] << EExtraCutString[EExtraCut] << ".eps} " << endl ;
    }
    else if(nDimensions[SignalMode]==2)
    {
      fBAD << "    \\begin{tabular}{cc}" << endl ;
      fBAD << "      \\includegraphics[width=0.4\\textwidth]{plots/Numerator/" << RoofitPrintNameX[SignalMode][run][(NRatios-1)/2] << "_" << SignalModeName[SignalMode] << EExtraCutString[EExtraCut] << ".eps} &" << endl ;
      fBAD << "      \\includegraphics[width=0.4\\textwidth]{plots/Numerator/" << RoofitPrintNameY[SignalMode][run][(NRatios-1)/2] << "_" << SignalModeName[SignalMode] << EExtraCutString[EExtraCut] << ".eps} " << endl ;
      fBAD << "    \\end{tabular}" << endl ;
    }
    fBAD << "  \\caption[Yield for all running periods for the $" << SignalModeNameLaTeX[SignalMode] << "$ mode.]{Yield for all running periods for the $" << SignalModeNameLaTeX[SignalMode] << "$ mode for generic MC.  The empty histograms show the total PDF returned by the fit.  The shaded histograms show the signal component of the PDF returned by the fit." ;
    if(SignalMode==2) fBAD << "  The green (upper) shaded histogram shows the contribution from $" << SignalModeNameLaTeX[0] << "$ events and the pink (lower) shaded histogram shows the contribution from $" << SignalModeNameLaTeX[2] << "$ events." ;
    fBAD << "}" << endl ;
    fBAD << "  \\label{fig:Numerator_" << RunName[run] << "_" << SignalModeName[SignalMode] << "}" << endl ;
    fBAD << "\\end{center}\\end{figure}" << endl ;

    // NXR
    fBAD << endl << "\\clearpage" << endl;
    fBAD << "Table \\ref{tab:Numerator_" << NXRSummaryPrintName[SignalMode] << "_" << SignalModeName[SignalMode] << "} and figure \\ref{fig:Numerator_NXRSummary_" << SignalModeName[SignalMode] << "} shows the result of the fits for the $" << SignalModeNameLaTeX[SignalMode] << "$ mode as a function of $n_X^R$ for generic MC.  The fit results are consistent with the true number of signal events for all bins of $n_X^R$." << endl ;

    // NXR table
    fBAD << "\\begin{table}[bh]" << endl ;
    fBAD << "  \\begin{center}" << endl ;
    fBAD << "    \\begin{tabular}{ccc}" << endl ;
    fBAD << "      \\hline" << endl ;
    fBAD << "    $n_X^R$ & True yield & Fitted yield \\\\" << endl ;
    fBAD << "    \\hline" << endl ;
    for(Int_t nXR=0 ; nXR<NNXR ; nXR++)
    {
      if(nXR==NNXR-1) fBAD << "    \\hline" << endl ;
      fBAD << "      " << NXRTitle[nXR] << " & $" ;
      if(trueYield[SignalMode][(NRatios-1)/2][NRuns+nXR]<0.0001){ fBAD << "0" ; }
      else{ fBAD << FloatToString(trueYield[SignalMode][(NRatios-1)/2][NRuns+nXR],2) ; }
      fBAD << "$ & $" ;
      if(fittedYield[SignalMode][(NRatios-1)/2][NRuns+nXR]<0.0001){ fBAD << "0" ; }
      else{ fBAD << FloatToString(fittedYield[SignalMode][(NRatios-1)/2][NRuns+nXR],2) ; }
      fBAD << " \\pm " ;
      fBAD << FloatToString(fittedYieldError[SignalMode][(NRatios-1)/2][NRuns+nXR],2) ; 
      fBAD << "$ \\\\" << endl ;
    }
    fBAD << "    \\hline" << endl ;
    fBAD << "    \\end{tabular}" ;
    fBAD << "  \\end{center}" << endl ;
    fBAD << "  \\caption[Yield by $n_X^R$ for the $" << SignalModeNameLaTeX[SignalMode] << "$ mode]{Yield by $n_X^R$ for the $" << SignalModeNameLaTeX[SignalMode] << "$ mode for generic MC.}" << endl ;
    fBAD << "  \\label{tab:Numerator_NXRSummary_" << SignalModeName[SignalMode] << "}" << endl ;
    fBAD << "\\end{table}" << endl ;

    // NXR summary
    fBAD << "\\begin{figure}[!hb]" << endl ;
    fBAD << "  \\begin{center}" << endl ;
    fBAD << "    \\includegraphics[width=0.5\\textwidth]{plots/Numerator/" << NXRSummaryPrintName[SignalMode] << "_" << SignalModeName[SignalMode] << EExtraCutString[EExtraCut] << ".eps}  " << endl ;
    fBAD << "    \\label{fig:Numerator_NXRSummary_" << SignalModeName[SignalMode] << "}" << endl ;
    fBAD << "    \\caption[Yield by $n_X^R$ for the $" << SignalModeNameLaTeX[SignalMode] << "$ mode.]{Yield by $n_X^R$ for the $" << SignalModeNameLaTeX[SignalMode] << "$ mode for generic MC.}" << endl ;
    fBAD << "\\end{center}\\end{figure}" << endl ;
    
    // NXR details
    fBAD << "\\begin{figure}[!hb]" << endl ;
    fBAD << "  \\begin{center}" << endl ;
    fBAD << "    \\begin{tabular}{cc}" << endl ;
    fBAD << "      \\includegraphics[width=0.3\\textwidth]{plots/Numerator/" << RoofitPrintNameX[SignalMode][NRuns+0][(NRatios-1)/2] << "_" << SignalModeName[SignalMode] << EExtraCutString[EExtraCut] << ".eps} & " << endl ;
    fBAD << "      \\includegraphics[width=0.3\\textwidth]{plots/Numerator/" << RoofitPrintNameX[SignalMode][NRuns+1][(NRatios-1)/2] << "_" << SignalModeName[SignalMode] << EExtraCutString[EExtraCut] << ".eps} \\\\ " << endl ;
    fBAD << "      \\includegraphics[width=0.3\\textwidth]{plots/Numerator/" << RoofitPrintNameX[SignalMode][NRuns+2][(NRatios-1)/2] << "_" << SignalModeName[SignalMode] << EExtraCutString[EExtraCut] << ".eps}  &" << endl ;
    fBAD << "      \\includegraphics[width=0.3\\textwidth]{plots/Numerator/" << RoofitPrintNameX[SignalMode][NRuns+3][(NRatios-1)/2] << "_" << SignalModeName[SignalMode] << EExtraCutString[EExtraCut] << ".eps}  " << endl ;
    fBAD << "    \\end{tabular}" << endl ;
    fBAD << "  \\caption[Fits by $n_X^R$ for the $" << SignalModeNameLaTeX[SignalMode] << "$ mode in " << varNameLaTeX[varX[SignalMode]] << ".]{Fits by $n_X^R$ for the $" << SignalModeNameLaTeX[SignalMode] << "$ mode for generic MC for $n_X^R=0$ (top left), $n_X^R=1$ (top right), $n_X^R=2$ (bottom left), $n_X^R=3$ (bottom right).  The empty histograms show the total PDF returned by the fit.  The shaded histograms show the signal component of the PDF returned by the fit.}" << endl ;
    fBAD << "  \\label{fig:Numerator_NXR_" << SignalModeName[SignalMode] << "}" << endl ;
    fBAD << "\\end{center}\\end{figure}" << endl ;
    
    // Ratio
    fBAD << endl << "\\clearpage" << endl;
    fBAD << "Table \\ref{tab:Numerator_" << RatioSummaryPrintName[SignalMode] << "_" << SignalModeName[SignalMode] << "} and figure \\ref{fig:Numerator_" << RatioSummaryPrintName[SignalMode] << "_" << SignalModeName[SignalMode] << "} shows the result of the fits for the $" << SignalModeNameLaTeX[SignalMode] << "$ mode as a function of the number of signal events in the sample for generic MC.  The fit results are consistent with the variation true number of signal events.  The proportions stated refer to the number of signal events relative to the nominal number of signal events in generic MC." << endl << endl ;

    // Ratio table
    fBAD << "\\begin{table}[bh]" << endl ;
    fBAD << "  \\begin{center}" << endl ;
    fBAD << "    \\begin{tabular}{ccc}" << endl ;
    fBAD << "      \\hline" << endl ;
    fBAD << "    Number of signal events & True yield & Fitted yield \\\\" << endl ;
    fBAD << "    \\hline" << endl ;
    for(Int_t ratio=0 ; ratio<NRatios ; ratio++)
    {
      if(ratio==(NRatios-1)/2||ratio==(NRatios+1)/2) fBAD << "    \\hline" << endl ;
      fBAD << "      " << ratioNameLaTeX[ratio] << " & $" ;
      if(trueYield[SignalMode][ratio][NFit-1]<0.0001){ fBAD << "0" ; }
      else{ fBAD << FloatToString(trueYield[SignalMode][ratio][NFit-1],2) ; }
      fBAD << "$ & $" ;
      if(fittedYield[SignalMode][ratio][NFit-1]<0.0001)
      { fBAD << "0" ; }
      else{ fBAD << FloatToString(fittedYield[SignalMode][ratio][NFit-1],2) ; }
      fBAD << " \\pm " ;
      fBAD << FloatToString(fittedYieldError[SignalMode][ratio][NFit-1],2) ; 
      fBAD << "$ \\\\" << endl ;
    }
    fBAD << "    \\hline" << endl ;
    fBAD << "    \\end{tabular}" ;
    fBAD << "  \\end{center}" << endl ;
    fBAD << "  \\caption[Yield as a function of the number of signal events for the $" << SignalModeNameLaTeX[SignalMode] << "$ mode]{Yield as a function of signal events for the $" << SignalModeNameLaTeX[SignalMode] << "$ mode for generic MC.}" << endl ;
    fBAD << "  \\label{tab:Numerator_RatioSummary_" << SignalModeName[SignalMode] << "}" << endl ;
    fBAD << "\\end{table}" << endl ;

    // Ratio summary
    fBAD << "\\begin{figure}[!hb]" << endl ;
    fBAD << "  \\begin{center}" << endl ;
    fBAD << "    \\includegraphics[width=0.5\\textwidth]{plots/Numerator/" << RatioSummaryPrintName[SignalMode] << "_" << SignalModeName[SignalMode] << EExtraCutString[EExtraCut] << ".eps}  " << endl ;
    fBAD << "  \\caption[Yield as a function of the number of signal events for the $" << SignalModeNameLaTeX[SignalMode] << "$ mode.]{Yield as a function of the number of signal events for the $" << SignalModeNameLaTeX[SignalMode] << "$ mode for generic MC.}" << endl ;
    fBAD << "  \\label{fig:Numerator_RatioSummary_" << SignalModeName[SignalMode] << "}" << endl ;
    fBAD << "\\end{center}\\end{figure}" << endl ;
    
    if(doToys)
    {
      // Toys
      Int_t toySpillOver =  4 ;
      fBAD << "Table \\ref{tab:Numerator_ToySummary_" << SignalModeName[SignalMode] <<"} and figures \\ref{fig:Numerator_Toy0_" << SignalModeName[SignalMode] << "}-\\ref{fig:Numerator_Toy" << (NRatios/toySpillOver) << "_" << SignalModeName[SignalMode] << "} show the result of toy experiments where the number of signal events is varied from $" << ratioNameLaTeX[0] << "$ to  $" << ratioNameLaTeX[NRatios-1] << "$ of the nominal MC value for the $" << SignalModeNameLaTeX[SignalMode] << "$ mode." << endl ;
      // Toys table
      fBAD << "\\begin{table}[bh]" << endl ;
      fBAD << "  \\begin{center}" << endl ;
      fBAD << "    \\begin{tabular}{cccc}" << endl ;
      fBAD << "      \\hline" << endl ;
      fBAD << "    Number of signal events & Pull mean & Pull $\\sigma$ & Number of successful fits \\\\" << endl ;
      fBAD << "    \\hline" << endl ;
      for(Int_t ratio=0 ; ratio<NRatios ; ratio++)
      {
        if(ratio==(NRatios-1)/2||ratio==(NRatios+1)/2) fBAD << "    \\hline" << endl ;
        fBAD << "      " << ratioNameLaTeX[ratio] << " & $" ;
        fBAD << FloatToString(pullMean[SignalMode][ratio],3) ;
        fBAD << " \\pm " ;
        fBAD << FloatToString(pullMeanError[SignalMode][ratio],3) ;
        fBAD << "$ & $" ;
        fBAD << FloatToString(pullSigma[SignalMode][ratio],3) ;
        fBAD << " \\pm " ;
        fBAD << FloatToString(pullSigmaError[SignalMode][ratio],3) ;
        fBAD << "$ & $" ;
        fBAD << pullIntegral[SignalMode][ratio] ;
        fBAD << "$ \\\\" << endl ;
      }
      fBAD << "    \\hline" << endl ;
      fBAD << "  \\end{tabular}" << endl ;
      fBAD << "  \\caption[Toy fits as a function of the number of signal events for the $" << SignalModeNameLaTeX[SignalMode] << "$ mode]{Toy fits as a function of signal events for the $" << SignalModeNameLaTeX[SignalMode] << "$ mode for generic MC.}" << endl ;
      fBAD << "  \\label{tab:Numerator_ToySummary_" << SignalModeName[SignalMode] << "}" << endl ;
      fBAD << "\\end{center}\\end{table}" << endl ;
      
      // Toys
      Int_t plotCounter  =  0 ;
      Int_t ratioStart   = -1 ;
      Int_t ratioStop    = -1 ;
      for(Int_t ratio=0 ; ratio<NRatios ; ratio++)
      {
        if(ratio%toySpillOver==0)
        {
          fBAD << "\\begin{figure}[!hb]" << endl ;
          fBAD << "  \\begin{center}" << endl ;
          fBAD << "    \\begin{tabular}{cc}" << endl ;
          ratioStart = ratio ;
        }
        fBAD << "    \\includegraphics[width=0.3\\textwidth]{plots/Numerator/" << PullPrintName[SignalMode][ratio] 
         << "_" << SignalModeName[SignalMode] << EExtraCutString[EExtraCut] << ".eps} & " << endl ;
        fBAD << "    \\includegraphics[width=0.3\\textwidth]{plots/Numerator/" <<  ToyPrintName[SignalMode][ratio] 
         << "_" << SignalModeName[SignalMode] << EExtraCutString[EExtraCut] << ".eps} \\\\ " << endl ;
        if((ratio+1)%toySpillOver==0||ratio==NRatios-1)
        {
          ratioStop = ratio ;
          fBAD << "  \\end{tabular}" << endl ;
          fBAD << "  \\caption[Toy studies for the $" << SignalModeNameLaTeX[SignalMode] << "$ for $" << ratioNameLaTeX[ratioStart] << "$ to $" <<  ratioNameLaTeX[ratioStop] << "$ nominal signal events.]{Toy studies for the $" << SignalModeNameLaTeX[SignalMode] << "$ for $" << ratioNameLaTeX[ratioStart] << "$ (top) to $" << ratioNameLaTeX[ratioStop] << "$ (bottom) nominal signal events.}" << endl ;
          fBAD << "  \\label{fig:Numerator_Toy" << plotCounter << "_" << SignalModeName[SignalMode] << "}" << endl ;
          fBAD << "\\end{center}\\end{figure}" << endl ;
          plotCounter++ ;
        }
      }
      fBAD << endl ;
    }
    
    if(SignalMode==2)
    {
      // Test of lepton universality table
      ofstream fLepton ;
      fLepton.open("../AWG90/aidanrc/BAD/note2170/NumeratorLeptonUniversality.tex") ;
      Float_t RTrue = 1.79 ;
      fLepton << "In table \\ref{tab:Numerator_LeptonicUnversality} we show the ratio of $\\DsToMuNu$ events to $\\DsToTauNuTauToMuNuNu$ events compared the expectation in MC.  $R$ is the ratio of these events where:" << endl ;
      fLepton << "\\begin{equation}" << endl ;
      fLepton << "  R = \\frac{N(\\DsToTauNuTauToMuNuNu)}{N(\\DsToMuNu)}" << endl ;
      fLepton << "\\end{equation}" << endl ;
      fLepton << "where $N(\\DsToTauNuTauToMuNuNu)$ is the yield of $\\DsToTauNuTauToMuNuNu$ events obtained from the fit and $N(\\DsToMuNu)$ is the yield of $\\DsToMuNu$ events obtained from the fit.  The expectation of this ration, $R_{true}$, is given by:" << endl ;
      fLepton << "\\begin{equation}" << endl ;
      fLepton << "  R_{true} = \\frac{\\BR(\\DsToTauNu)\\times\\BR(\\TauToMuNuNu)}{\\BR(\\DsToMuNu)}" << endl ;
      fLepton << "\\end{equation}" << endl ;
      fLepton << "where the various branching fractions are taken from \\DECAYDEC.  The value of $R_{true}$ is $1.79$ and does not correspond to the Standard Model expectation of $1.75$." << endl ;
      fLepton << "\\begin{table}[bh]" << endl ;
      fLepton << "  \\begin{center}" << endl ;
      fLepton << "    \\begin{tabular}{ccccc}" << endl ;
      fLepton << "      \\hline" << endl ;
      fLepton << "    Proportion of                   & Number of                      & Number of          & $R$ & $R/R_{true}$ \\\\" << endl ;
      fLepton << "    $\\DsToTauNuTauToMuNuNu$ events & $\\DsToTauNuTauToMuNuNu$ events & $\\DsToMuNu$ events &     &              \\\\" << endl ; 
      fLepton << "    \\hline" ;
      for(Int_t ratio=0 ; ratio<NRatios ; ratio++)
      {
        if(ratio==(NRatios-1)/2||ratio==(NRatios+1)/2) fLepton << "    \\hline" << endl ;
        fLepton << "      " << ratioNameLaTeX[ratio] << " & $" ;
        if(trueYield[SignalMode][ratio][NFit-1]<0.0001){ fLepton << "0" ; }
        else{ fLepton << FloatToString(fittedYield[SignalMode][ratio][NFit-1],2) ; }    
        fLepton << " \\pm " ;
        fLepton << FloatToString(fittedYieldError[SignalMode][ratio][NFit-1],2) ;
        fLepton << "$ & $" ;
        if(DsToMuNuYield[ratio]<0.0001)
        { fLepton << "0" ; }
        else{ fLepton << FloatToString(DsToMuNuYield[ratio],2) ; }
        fLepton << " \\pm " ;
        fLepton << FloatToString(DsToMuNuYieldError[ratio],2) ; 
        fLepton << "$ & $" ;
        Float_t R = 0 ;
        if(DsToMuNuYield[ratio]<0.0001)
        {
           fLepton << "-" ;
           fLepton << "$ & $" ;
           fLepton << "-" ;
        }
        else
        {
          R = fittedYield[SignalMode][ratio][NFit-1]/DsToMuNuYield[ratio] ;
          fLepton << FloatToString(R,2) ;
          fLepton << " \\pm " ;
          Float_t error1 = fittedYieldError[SignalMode][ratio][NFit-1]/fittedYield[SignalMode][ratio][NFit-1] ;
          Float_t error2 = DsToMuNuYieldError[ratio]/DsToMuNuYield[ratio] ;
          Float_t error  = sqrt(error1*error1+error2*error2) ;
          fLepton << FloatToString(error*R,2) ;
          fLepton << "$ & $" ;
          fLepton << FloatToString(R/RTrue,2) ;
          fLepton << " \\pm " ;
          fLepton << FloatToString(error*R/RTrue,2) ;
        }
        fLepton << "$ \\\\" << endl ;
      }
      fLepton << "    \\hline" << endl ;
      fLepton << "    \\end{tabular}" ;
      fLepton << "  \\caption[Test of lepton universality using the $" << SignalModeNameLaTeX[0] << "$ and $" << SignalModeNameLaTeX[2] << "$ modes]{Test of lepton universality using the $" << SignalModeNameLaTeX[0] << "$ and $" << SignalModeNameLaTeX[2] << "$ modes.  The number of $" << SignalModeNameLaTeX[2] << "$ events is varied from $75\\%$ to $125\\%$ of the nominal value in MC.}" << endl ;
      fLepton << "  \\label{tab:Numerator_LeptonicUnversality}" << endl ;
      fLepton << "\\end{center}\\end{table}" << endl ;
      fLepton.close() ;
    }
    
    Float_t  sxData = 0 ;
    Float_t sxxData = 0 ;
    Float_t    sxMC = 0 ;
    Float_t   sxxMC = 0 ;
    
    for(Int_t i=0 ; i<nStat ; i++)
    {
      sxMC    +=   statYieldMC[SignalMode][i] ;
      sxxMC   +=   statYieldMC[SignalMode][i]*statYieldMC[SignalMode][i] ;
      sxData  += statYieldData[SignalMode][i] ;
      sxxData += statYieldData[SignalMode][i]*statYieldData[SignalMode][i] ;
      cout << i << "   " << statYieldMC[SignalMode][i] << "    " << statYieldData[SignalMode][i] << endl ;
    }
    Float_t   MCVariance =   sxxMC/(nStat-1) -     sxMC*sxMC/(nStat*(nStat-1)) ;
    Float_t dataVariance = sxxData/(nStat-1) - sxData*sxData/(nStat*(nStat-1)) ;
    //backgroundModelStatisticalErrorMC[SignalMode]   = 100*sqrt(MCVariance)/fittedYield[SignalMode][(NRatios-1)/2][NFit-1] ;
    //backgroundModelStatisticalErrorData[SignalMode] = 100*sqrt(dataVariance)/dataYield[SignalMode][NFit-1] ;
    cout << endl << "Standard deviations: " << sqrt(MCVariance) << " " << sqrt(dataVariance) << endl ;
    cout << endl ;
    
    for(Int_t DsBF=0 ; DsBF<NDsType ; DsBF++)
    {
      Float_t nominal   =       fittedYield[SignalMode][(NRatios-1)/2][NFit-1] ;
      Float_t compUp    = fittedYieldDsBFMC[SignalMode][DsBF][0] ;
      Float_t compDown  = fittedYieldDsBFMC[SignalMode][DsBF][1] ;
      Float_t errorUp   = (compUp!=0)   ? fabs(compUp  -nominal)/nominal : 0 ;
      Float_t errorDown = (compDown!=0) ? fabs(compDown-nominal)/nominal : 0 ;
      Float_t error     = (errorDown>errorUp) ? errorDown : errorUp ;
      DsBFUncertaintyMC[SignalMode] += error*error ;
      cout << "DsBF: " << DsBF << "      " << nominal << "         " << 100*error << "%      " << fittedYieldDsBFMC[SignalMode][DsBF][0] << "      " << fittedYieldDsBFMC[SignalMode][DsBF][1] << endl ;

      nominal   =           dataYield[SignalMode][NFit-1] ;
      compUp    = fittedYieldDsBFData[SignalMode][DsBF][0] ;
      compDown  = fittedYieldDsBFData[SignalMode][DsBF][1] ;
      errorUp   = (compUp!=0)   ? fabs(compUp  -nominal)/nominal : 0 ;
      errorDown = (compDown!=0) ? fabs(compDown-nominal)/nominal : 0 ;
      error     = (errorDown>errorUp) ? errorDown : errorUp ;
      DsBFUncertaintyData[SignalMode] += error*error ;
      cout << "DsBF: " << DsBF << "      " << nominal << "         " << 100*error << "%      " << fittedYieldDsBFData[SignalMode][DsBF][0] << "      " << fittedYieldDsBFData[SignalMode][DsBF][1] << endl ;
    }
    DsBFUncertaintyMC[SignalMode]   = sqrt(DsBFUncertaintyMC[SignalMode]) ;
    DsBFUncertaintyData[SignalMode] = sqrt(DsBFUncertaintyData[SignalMode]) ;
    cout << endl ;
    
    componentUncertaintyMC[SignalMode] = 0 ;
    
    for(Int_t component=0 ; component<NDsType ; component++)
    {
      Float_t nominal   =            fittedYield[SignalMode][(NRatios-1)/2][NFit-1] ;
      Float_t compUp    = fittedYieldComponentMC[SignalMode][component][0] ;
      Float_t compDown  = fittedYieldComponentMC[SignalMode][component][1] ;
      Float_t errorUp   = (compUp!=0)   ? fabs(compUp  -nominal)/nominal : 0 ;
      Float_t errorDown = (compDown!=0) ? fabs(compDown-nominal)/nominal : 0 ;
      Float_t error     = (errorDown>errorUp) ? errorDown : errorUp ;
      componentUncertaintyMC[SignalMode] += error*error ;
      cout << "Component: " << component << "      " << nominal << "      " << 100*error << "%      " << fittedYieldComponentMC[SignalMode][component][0] << "      " << fittedYieldComponentMC[SignalMode][component][1] << endl ;
      
      nominal   =                dataYield[SignalMode][NFit-1] ;
      compUp    = fittedYieldComponentData[SignalMode][component][0] ;
      compDown  = fittedYieldComponentData[SignalMode][component][1] ;
      errorUp   = (compUp!=0)   ? fabs(compUp  -nominal)/nominal : 0 ;
      errorDown = (compDown!=0) ? fabs(compDown-nominal)/nominal : 0 ;
      error     = (errorDown>errorUp) ? errorDown : errorUp ;
      componentUncertaintyData[SignalMode] += error*error ;
      cout << "Component: " << component << "      " << nominal << "      " << 100*error << "%      " << fittedYieldComponentData[SignalMode][component][0] << "      " << fittedYieldComponentData[SignalMode][component][1] << endl ;
    }
    componentUncertaintyMC[SignalMode]   = sqrt(componentUncertaintyMC[SignalMode]) ;
    componentUncertaintyData[SignalMode] = sqrt(componentUncertaintyData[SignalMode]) ;
  }
  cout << endl ;

  fBAD.close() ;
  fPlots->Write() ;
  fPlots->Close() ;
  for(SignalMode=0 ; SignalMode<NSignalMode-1 ; SignalMode++)
  {
    Float_t shiftUncertaintyMC = 100*(fittedYieldShiftMC[SignalMode]-fittedYield[SignalMode][(NRatios-1)/2][NRuns-1])/trueYield[SignalMode][(NRatios-1)/2][NFit-1] ;
    Float_t smearUncertaintyMC = 100*(fittedYieldSmearMC[SignalMode]-fittedYield[SignalMode][(NRatios-1)/2][NRuns-1])/trueYield[SignalMode][(NRatios-1)/2][NFit-1] ;
    Float_t shiftUncertaintyData = 100*(fittedYieldShiftData[SignalMode]-dataYield[SignalMode][NRuns-1])/dataYield[SignalMode][NFit-1] ;
    Float_t smearUncertaintyData = 100*(fittedYieldSmearData[SignalMode]-dataYield[SignalMode][NRuns-1])/dataYield[SignalMode][NFit-1] ;
    Float_t exponentialUncertaintyMC   = 100*(fittedYieldExponentialMC[SignalMode]-fittedYield[SignalMode][(NRatios-1)/2][NRuns-1])/trueYield[SignalMode][(NRatios-1)/2][NFit-1] ;
    Float_t exponentialUncertaintyData = 100*(fittedYieldExponentialData[SignalMode]-dataYield[SignalMode][NRuns-1])/dataYield[SignalMode][NFit-1] ;
    fBADCommands << "\\newcommand{\\" << SignalModeName[SignalMode] << "MCYield}        [0]{" << FloatToString(      fittedYield[SignalMode][(NRatios-1)/2][NRuns-1],2) << "}" << endl ;
    fBADCommands << "\\newcommand{\\" << SignalModeName[SignalMode] << "MCYieldError}   [0]{" << FloatToString( fittedYieldError[SignalMode][(NRatios-1)/2][NRuns-1],2) << "}" << endl ;
    fBADCommands << "\\newcommand{\\" << SignalModeName[SignalMode] << "MCTrueYield}    [0]{" << FloatToString(        trueYield[SignalMode][(NRatios-1)/2][NRuns-1],2) << "}" << endl ;
    fBADCommands << "\\newcommand{\\" << SignalModeName[SignalMode] << "DataYield}        [0]{" << FloatToString(      dataYield[SignalMode][NRuns-1],2) << "}" << endl ;
    fBADCommands << "\\newcommand{\\" << SignalModeName[SignalMode] << "DataYieldError}   [0]{" << FloatToString( dataYieldError[SignalMode][NRuns-1],2) << "}" << endl ;
    fBADCommands << "\\newcommand{\\" << SignalModeName[SignalMode] << "PullMean}       [0]{" << FloatToString(         pullMean[SignalMode][(NRatios-1)/2],2) << "}" << endl ;
    fBADCommands << "\\newcommand{\\" << SignalModeName[SignalMode] << "PullMeanError}  [0]{" << FloatToString(    pullMeanError[SignalMode][(NRatios-1)/2],2) << "}" << endl ;
    fBADCommands << "\\newcommand{\\" << SignalModeName[SignalMode] << "PullSigma}      [0]{" << FloatToString(        pullSigma[SignalMode][(NRatios-1)/2],2) << "}" << endl ;
    fBADCommands << "\\newcommand{\\" << SignalModeName[SignalMode] << "PullSigmaError} [0]{" << FloatToString(   pullSigmaError[SignalMode][(NRatios-1)/2],2) << "}" << endl ;
    fBADCommands << "\\newcommand{\\" << SignalModeName[SignalMode] << "NConvergedToys} [0]{" << FloatToString(     pullIntegral[SignalMode][(NRatios-1)/2],0) << "}" << endl ;
    
    fBADCommands << "\\newcommand{\\" << SignalModeName[SignalMode] << "MmTwoMean}                 [0]{" << FloatToString(                      mm2Mean[SignalMode],2) << "}" << endl ;
    fBADCommands << "\\newcommand{\\" << SignalModeName[SignalMode] << "MmTwoMeanError}            [0]{" << FloatToString(                 mm2MeanError[SignalMode],2) << "}" << endl ;
    fBADCommands << "\\newcommand{\\" << SignalModeName[SignalMode] << "MmTwoSigma}                [0]{" << FloatToString(                     mm2Sigma[SignalMode],2) << "}" << endl ;
    fBADCommands << "\\newcommand{\\" << SignalModeName[SignalMode] << "MmTwoSigmaError}           [0]{" << FloatToString(                mm2SigmaError[SignalMode],2) << "}" << endl ;
    fBADCommands << "\\newcommand{\\" << SignalModeName[SignalMode] << "MCYieldShift}              [0]{" << FloatToString(           fittedYieldShiftMC[SignalMode],2) << "}" << endl ;
    fBADCommands << "\\newcommand{\\" << SignalModeName[SignalMode] << "MCYieldErrorShift}         [0]{" << FloatToString(      fittedYieldErrorShiftMC[SignalMode],2) << "}" << endl ;
    fBADCommands << "\\newcommand{\\" << SignalModeName[SignalMode] << "MCYieldSmear}              [0]{" << FloatToString(           fittedYieldSmearMC[SignalMode],2) << "}" << endl ;
    fBADCommands << "\\newcommand{\\" << SignalModeName[SignalMode] << "MCYieldErrorSmear}         [0]{" << FloatToString(      fittedYieldErrorSmearMC[SignalMode],2) << "}" << endl ;
    fBADCommands << "\\newcommand{\\" << SignalModeName[SignalMode] << "ShiftUncertaintyMC}        [0]{" << FloatToString(                       shiftUncertaintyMC,2) << "}" << endl ;
    fBADCommands << "\\newcommand{\\" << SignalModeName[SignalMode] << "SmearUncertaintyMC}        [0]{" << FloatToString(                       smearUncertaintyMC,2) << "}" << endl ;
    fBADCommands << "\\newcommand{\\" << SignalModeName[SignalMode] << "ShiftUncertaintyData}      [0]{" << FloatToString(                     shiftUncertaintyData,2) << "}" << endl ;
    fBADCommands << "\\newcommand{\\" << SignalModeName[SignalMode] << "SmearUncertaintyData}      [0]{" << FloatToString(                     smearUncertaintyData,2) << "}" << endl ;
    fBADCommands << "\\newcommand{\\" << SignalModeName[SignalMode] << "MCYieldExponential}        [0]{" << FloatToString(     fittedYieldExponentialMC[SignalMode],2) << "}" << endl ;
    fBADCommands << "\\newcommand{\\" << SignalModeName[SignalMode] << "MCYieldErrorExponential}   [0]{" << FloatToString(fittedYieldErrorExponentialMC[SignalMode],2) << "}" << endl ;
    fBADCommands << "\\newcommand{\\" << SignalModeName[SignalMode] << "ExponentialUncertaintyMC}  [0]{" << FloatToString(                 exponentialUncertaintyMC,2) << "}" << endl ;
    fBADCommands << "\\newcommand{\\" << SignalModeName[SignalMode] << "ExponentialUncertaintyData}[0]{" << FloatToString(               exponentialUncertaintyData,2) << "}" << endl ;

    fBADCommands << "\\newcommand{\\" << SignalModeName[SignalMode] << "DsBFUncertaintyMC}     [0]{" << FloatToString(      DsBFUncertaintyMC[SignalMode],2) << "}" << endl ;
    fBADCommands << "\\newcommand{\\" << SignalModeName[SignalMode] << "ComponentUncertaintyMC}[0]{" << FloatToString( componentUncertaintyMC[SignalMode],2) << "}" << endl ;

    fBADCommands << "\\newcommand{\\" << SignalModeName[SignalMode] << "DsBFUncertaintyData}     [0]{" << FloatToString(      DsBFUncertaintyData[SignalMode],2) << "}" << endl ;
    fBADCommands << "\\newcommand{\\" << SignalModeName[SignalMode] << "ComponentUncertaintyData}[0]{" << FloatToString( componentUncertaintyData[SignalMode],2) << "}" << endl ;
    fBADCommands << "\\newcommand{\\" << SignalModeName[SignalMode] << "BackgroundModelStatisticalUncertaintyMC} [0]{" << FloatToString(100*backgroundModelStatisticalErrorMC[SignalMode],2) << "}" << endl ;
    fBADCommands << "\\newcommand{\\" << SignalModeName[SignalMode] << "BackgroundModelStatisticalUncertaintyData} [0]{" << FloatToString(100*backgroundModelStatisticalErrorData[SignalMode],2) << "}" << endl ;

    fBADCommands << "\\newcommand{\\" << SignalModeName[SignalMode] << "UnblindChiTwo}[0]{" << FloatToString(Chi2Unblind[SignalMode],2) << "}" << endl ;
    fBADCommands << "\\newcommand{\\" << SignalModeName[SignalMode] << "UnblindNDoF}  [0]{" << FloatToString(NDoFUnblind[SignalMode],2) << "}" << endl ;
    
    fBADCommands << "\\newcommand{\\" << SignalModeName[SignalMode] << "FitChiTwo}[0]{" << FloatToString(fitChi2[SignalMode],2) << "}" << endl ;
    fBADCommands << "\\newcommand{\\" << SignalModeName[SignalMode] << "FitNDoF}  [0]{" << FloatToString(fitNDoF[SignalMode],2) << "}" << endl ;
  }
  fBADCommands.close() ;
  cout << endl ;
  
  for(SignalMode=SignalModeStart ; SignalMode<SignalModeEnd+1 ; SignalMode++)
  {
    cout << endl << SignalModeName[SignalMode] << endl ;
    
    Float_t DsBFErrorAbsoluteMC   = 0 ;
    Float_t DsBFErrorAbsoluteData = 0 ;
    for(Int_t DsBF=0 ; DsBF<NDsType ; DsBF++)
    {
      Float_t nominal   =       fittedYield[SignalMode][(NRatios-1)/2][NFit-1] ;
      Float_t compUp    = fittedYieldDsBFMC[SignalMode][DsBF][0] ;
      Float_t compDown  = fittedYieldDsBFMC[SignalMode][DsBF][1] ;
      Float_t errorUp   = (fabs(compUp)>1e-3)   ? fabs(compUp  -nominal)/nominal : 0 ;
      Float_t errorDown = (fabs(compDown)>1e-3) ? fabs(compDown-nominal)/nominal : 0 ;
      Float_t error     = (errorDown>errorUp) ? errorDown : errorUp ;
      Float_t errorUpAbsolute   = (fabs(compUp)>1e-3)   ? fabs(compUp  -nominal) : 0 ;
      Float_t errorDownAbsolute = (fabs(compDown)>1e-3) ? fabs(compDown-nominal) : 0 ;      
      Float_t errorAbsolute     = (errorDownAbsolute>errorUpAbsolute) ? errorDownAbsolute : errorUpAbsolute ;
      DsBFErrorAbsoluteMC += errorAbsolute*errorAbsolute ;
      cout << "DsBF: " << DsBF << "      " << FloatToString(nominal,2) << "         " << FloatToString(100*error,2) << "%      " << FloatToString(fittedYieldDsBFMC[SignalMode][DsBF][0],2) << "      " << FloatToString(fittedYieldDsBFMC[SignalMode][DsBF][1],2) << endl ;

      nominal   =           dataYield[SignalMode][NFit-1] ;
      compUp    = fittedYieldDsBFData[SignalMode][DsBF][0] ;
      compDown  = fittedYieldDsBFData[SignalMode][DsBF][1] ;
      errorUp   = (fabs(compUp)>1e-3)   ? fabs(compUp  -nominal)/nominal : 0 ;
      errorDown = (fabs(compDown)>1e-3) ? fabs(compDown-nominal)/nominal : 0 ;
      error     = (errorDown>errorUp) ? errorDown : errorUp ;
      errorUpAbsolute   = (fabs(compUp)>1e-3)   ? fabs(compUp  -nominal) : 0 ;
      errorDownAbsolute = (fabs(compDown)>1e-3) ? fabs(compDown-nominal) : 0 ;      
      errorAbsolute     = (errorDownAbsolute>errorUpAbsolute) ? errorDownAbsolute : errorUpAbsolute ;
      DsBFErrorAbsoluteData += errorAbsolute*errorAbsolute ;
      cout << "DsBF: " << DsBF << "      " << FloatToString(nominal,2) << "         " << FloatToString(100*error,2) << "%      " << FloatToString(fittedYieldDsBFData[SignalMode][DsBF][0],2) << "      " << FloatToString(fittedYieldDsBFData[SignalMode][DsBF][1],2) << endl ;
    }
    DsBFErrorAbsoluteMC   = sqrt(DsBFErrorAbsoluteMC) ;
    DsBFErrorAbsoluteData = sqrt(DsBFErrorAbsoluteData) ;
    cout << "Total absolute DsBF error (MC):   " << DsBFErrorAbsoluteMC   << endl ;
    cout << "Total absolute DsBF error (Data): " << DsBFErrorAbsoluteData << endl ;
    
    Float_t componentErrorAbsoluteMC   = 0 ;
    Float_t componentErrorAbsoluteData = 0 ;
    for(Int_t component=0 ; component<NDsType ; component++)
    {
      Float_t nominal   =            fittedYield[SignalMode][(NRatios-1)/2][NFit-1] ;
      Float_t compUp    = fittedYieldComponentMC[SignalMode][component][0] ;
      Float_t compDown  = fittedYieldComponentMC[SignalMode][component][1] ;
      Float_t errorUp   = (fabs(compUp)>1e-3)   ? fabs(compUp  -nominal)/nominal : 0 ;
      Float_t errorDown = (fabs(compDown)>1e-3) ? fabs(compDown-nominal)/nominal : 0 ;
      Float_t error     = (errorDown>errorUp) ? errorDown : errorUp ;
      Float_t errorUpAbsolute   = (fabs(compUp)>1e-3)   ? fabs(compUp  -nominal) : 0 ;
      Float_t errorDownAbsolute = (fabs(compDown)>1e-3) ? fabs(compDown-nominal) : 0 ;
      Float_t errorAbsolute     = (errorDownAbsolute>errorUpAbsolute) ? errorDownAbsolute : errorUpAbsolute ;
      componentErrorAbsoluteMC += errorAbsolute*errorAbsolute ;
      cout << "Component: " << component << "      " << FloatToString(nominal,2) << "      " << FloatToString(100*error,2) << "%      " << FloatToString(fittedYieldComponentMC[SignalMode][component][0],2) << "      " << FloatToString(fittedYieldComponentMC[SignalMode][component][1],2) << endl ;
      
      nominal   =                dataYield[SignalMode][NFit-1] ;
      compUp    = fittedYieldComponentData[SignalMode][component][0] ;
      compDown  = fittedYieldComponentData[SignalMode][component][1] ;
      errorUp   = (fabs(compUp)>1e-3)   ? fabs(compUp  -nominal)/nominal : 0 ;
      errorDown = (fabs(compDown)>1e-3) ? fabs(compDown-nominal)/nominal : 0 ;
      error     = (errorDown>errorUp) ? errorDown : errorUp ;
      errorUpAbsolute   = (fabs(compUp)>1e-3)   ? fabs(compUp  -nominal) : 0 ;
      errorDownAbsolute = (fabs(compDown)>1e-3) ? fabs(compDown-nominal) : 0 ;      
      errorAbsolute     = (errorDownAbsolute>errorUpAbsolute) ? errorDownAbsolute : errorUpAbsolute ;
      cout << "Component: " << component << "      " << FloatToString(nominal,2) << "      " << FloatToString(100*error,2) << "%      " << FloatToString(fittedYieldComponentData[SignalMode][component][0],2) << "      " << FloatToString(fittedYieldComponentData[SignalMode][component][1],2) << endl ;
      componentErrorAbsoluteData += errorAbsolute*errorAbsolute ;
    }
    
    cout << "(--) for true yield." << endl ;
    cout << "<--> for data yield." << endl ;
    cout << "[--] for signal models [" ;
    switch(SignalMode)
    {
    case 0:
      cout << "Shift , Smear" ;
      break ;
    case 3:
      cout << "Exponential" ;
      break ;
    default: break ;
    }
    cout << "]" << endl ;
    cout << endl ;
    for(Int_t fit=FitStart ; fit<FitEnd+1 ; fit++)
    {
      cout.width(15) ;
      cout << FitName[fit] << " " ;
      //for(Int_t ratio=0 ; ratio<NRatios ; ratio++)
      Int_t ratio = (NRatios-1)/2 ;
      {
        cout.width(6) ; cout << FloatToString(     fittedYield[SignalMode][ratio][fit],2) << " +/- " ;
        cout.width(6) ; cout << FloatToString(fittedYieldError[SignalMode][ratio][fit],2) << " ("    ;
        cout.width(6) ; cout << FloatToString(       trueYield[SignalMode][ratio][fit],2) << ")   "  ;
        if(SignalMode<4)
        {
          cout << "<" ;
          cout.width(6) ; cout << FloatToString(dataYield[SignalMode][fit],2) << " +/- "  ;
          cout.width(6) ; cout << FloatToString(dataYieldError[SignalMode][fit],2) ;
          cout << ">   "  ;
        }
        if(fit==NFit-1)
        {
          if(SignalMode<2)
          {
            cout << "[" ;
            cout.width(6) ; cout << FloatToString(fittedYieldShiftMC[SignalMode],2) << " +/- "  ;
            cout.width(6) ; cout << FloatToString(fittedYieldErrorShiftMC[SignalMode],2) ;
            cout << "]   "  ;

            cout << "[" ;
            cout.width(6) ; cout << FloatToString(fittedYieldSmearMC[SignalMode],2) << " +/- "  ;
            cout.width(6) ; cout << FloatToString(fittedYieldErrorSmearMC[SignalMode],2) ;
            cout << "]   "  ;
          }

          if(SignalMode>1)
          {
            cout << "[" ;
            cout.width(6) ; cout << FloatToString(fittedYieldExponentialMC[SignalMode]) << " +/- " ;
            cout.width(6) ; cout << FloatToString(fittedYieldErrorExponentialMC[SignalMode]) ;
            cout << "]" ;
          }
      
          if(SignalMode==2)
          {
            cout << "{" ;
            cout.width(6) ; cout << FloatToString(     DsToMuNuYield[ratio],2) << " +/- "  ;
            cout.width(6) ; cout << FloatToString(DsToMuNuYieldError[ratio],2) ;
            cout << "}   "  ;
          }
        }
      }
      cout << endl ;
    }

    cout << endl ;
    cout << "chi2/ndof: " << Chi2Unblind[SignalMode] << " / " << NDoFUnblind[SignalMode] << endl ;
    cout << "Results of studies in MC:" << endl ;
    cout << "Background model DsBF error: "        << FloatToString(fabs(100*DsBFUncertaintyMC[SignalMode]),5)                 << "%" << endl ;
    cout << "  Absolute DsBF error:   " << DsBFErrorAbsoluteMC   << endl ;
    cout << "Background model component error: "   << FloatToString(fabs(100*componentUncertaintyMC[SignalMode]),5)            << "%" << endl ;
    cout << "  Absolute component error:   " << componentErrorAbsoluteMC   << endl ;
    cout << "Background model statistical error: " << FloatToString(fabs(100*backgroundModelStatisticalErrorMC[SignalMode]),5) << "%" << endl ;
    Float_t mm2ShiftError = fabs(100*(fittedYieldShiftMC[SignalMode]-fittedYield[SignalMode][(NRatios-1)/2][NFit-1])/trueYield[SignalMode][(NRatios-1)/2][NFit-1]) ;
    Float_t mm2SmearError = fabs(100*(fittedYieldShiftMC[SignalMode]-fittedYield[SignalMode][(NRatios-1)/2][NFit-1])/trueYield[SignalMode][(NRatios-1)/2][NFit-1]) ;
    cout << "mm2 shift error: " << FloatToString(mm2ShiftError) << "%" << endl ;
    cout << "mm2 smear error: " << FloatToString(mm2SmearError) << "%" << endl ;
    cout << "mm2 shift yield: " << FloatToString(fittedYieldShiftMC[SignalMode]) << " +/-" << FloatToString(fittedYieldErrorShiftMC[SignalMode]) << endl ;
    cout << "mm2 smear yield: " << FloatToString(fittedYieldSmearMC[SignalMode]) << " +/-" << FloatToString(fittedYieldErrorSmearMC[SignalMode]) << endl ;
    Float_t exponentialError = fabs(100*(fittedYieldExponentialMC[SignalMode]-fittedYield[SignalMode][(NRatios-1)/2][NFit-1])/trueYield[SignalMode][(NRatios-1)/2][NFit-1]) ;
    cout << "exponetial error: " << FloatToString(exponentialError) << "%" << endl ;
    cout << "exponetial yield: " << FloatToString(fittedYieldExponentialMC[SignalMode]) << endl ;
    cout << "Total events: "               << totalEvents[SignalMode]          << endl ;
    cout << "Estimated background: "       << backgroundYield[SignalMode]      << endl ;
    cout << "Estimated background error: " << backgroundYieldError[SignalMode] << endl ;
    
    cout << endl << "Results of studies in data:"  << endl ;
    cout << "Background model DsBF error: "        << FloatToString(fabs(100*DsBFUncertaintyData[SignalMode]),5)                 << "%" << endl ;
    cout << "  Absolute DsBF error:   " << DsBFErrorAbsoluteData << endl ;
    cout << "Background model component error: "   << FloatToString(fabs(100*componentUncertaintyData[SignalMode]),5)            << "%" << endl ;
    cout << "  Absolute component error:   " << componentErrorAbsoluteData << endl ;
    cout << "Background model statistical error: " << FloatToString(fabs(100*backgroundModelStatisticalErrorData[SignalMode]),5) << "%" << endl ;
    Float_t mm2ShiftErrorData = fabs(100*(fittedYieldShiftData[SignalMode]-dataYield[SignalMode][NFit-1])/dataYield[SignalMode][NFit-1]) ;
    Float_t mm2SmearErrorData = fabs(100*(fittedYieldShiftData[SignalMode]-dataYield[SignalMode][NFit-1])/dataYield[SignalMode][NFit-1]) ;
    cout << "mm2 shift error: " << FloatToString(mm2ShiftErrorData) << "%" << endl ;
    cout << "mm2 smear error: " << FloatToString(mm2SmearErrorData) << "%" << endl ;
    cout << "mm2 shift yield: " << FloatToString(fittedYieldShiftData[SignalMode]) << " +/-" << FloatToString(fittedYieldErrorShiftData[SignalMode]) << endl ;
    cout << "mm2 smear yield: " << FloatToString(fittedYieldSmearData[SignalMode]) << " +/-" << FloatToString(fittedYieldErrorSmearData[SignalMode]) << endl ;
    Float_t exponentialErrorData = fabs(100*(fittedYieldExponentialData[SignalMode]-dataYield[SignalMode][NFit-1])/dataYield[SignalMode][NFit-1]) ;
    cout << "exponetial error: " << FloatToString(exponentialErrorData) << "%" << endl ;
    cout << "exponetial yield: " << FloatToString(fittedYieldExponentialData[SignalMode]) << endl ;
    cout << "fit chi2/n: " << FloatToString(fittedYieldExponentialData[SignalMode]) << endl ;
    cout << "exponetial yield: " << FloatToString(fitChi2[SignalMode]) << " / " << FloatToString(fitNDoF[SignalMode]) << endl ;   
    if(SignalMode==1) cout << "DsToENu toy yield: " << DsToENuToyYield << " from " << nDsToEllNuToyPass << " toys" << endl ;
    
    fFitResults[SignalMode]->Write() ;
    fFitResults[SignalMode]->Close() ;

    fFitResultsData[SignalMode]->Write() ;
    fFitResultsData[SignalMode]->Close() ;
  }
}

// Overload this because I'm too lazy to add a new function call everywhere
void DRecoDsToEllNuPlots::MakeResidualsPlot(RooRealVar* rrv,RooAddPdf model,RooDataHist* rdh,RooAbsPdf* rap1, RooAbsPdf* rap2,Int_t color1, Int_t color2, TH1F* hTrue, TH1F* hFit,Bool_t showComponents,TCanvas* canvas, TString printName)
{
  MakeResidualsPlot(rrv,model,rdh,rap1,rap2,color1,color2,hTrue,hFit,showComponents,canvas,printName,kFALSE) ;
}

void DRecoDsToEllNuPlots::MakeResidualsPlot(RooRealVar* rrv,RooAddPdf model,RooDataHist* rdh,RooAbsPdf* rap1, RooAbsPdf* rap2,Int_t color1, Int_t color2, TH1F* hTrue, TH1F* hFit,Bool_t showComponents,TCanvas* canvas, TString printName,Bool_t DataLabel)
{
  canvas->cd() ;
  TH1F* hTemp1 = new TH1F("hTemp1","hTemp1",10,0,1) ;
  TH1F* hTemp2 = new TH1F("hTemp2","hTemp2",10,0,1) ;
  TH1F* hTemp3 = new TH1F("hTemp3","hTemp3",10,0,1) ;
  hTemp1->Fill(0.5,1) ;
  hTemp2->Fill(0.5,1) ;
  hTemp3->Fill(0.5,1) ;
  hTemp1->SetLineWidth(2) ;
  hTemp1->SetLineColor(kBlue) ;
  hTemp2->SetFillColor(SignalModeColor[SignalMode]) ;
  hTemp3->SetMarkerStyle(20) ;
  hTemp1->Draw() ;
  hTemp2->Draw() ;
  hTemp3->Draw() ;
  canvas->Clear() ;
  
  RooPlot* frame = rrv->frame() ;
  frame->SetMinimum(0) ;
  rdh->plotOn(frame,DataError(RooAbsData::SumW2)) ;
  model.plotOn(frame) ;
  if(showComponents==kTRUE) 
  {
    if(rap2==NULL)
    {
      model.plotOn(frame,Components(RooArgList(*rap1)),FillColor(color1),DrawOption("F")) ;
    }
    else
    {
      model.plotOn(frame,Components(RooArgList(*rap1,*rap2)),FillColor(color2),DrawOption("FL")) ;
      model.plotOn(frame,Components(RooArgList(*rap1)),FillColor(color1),DrawOption("FL")) ;
    }
  }
  rdh->plotOn(frame,DataError(RooAbsData::SumW2)) ;
  DRecoResiduals* residuals = new DRecoResiduals("residuals", hTrue, hFit) ;
  residuals->MakeFitPlotWithResiduals(canvas,frame) ;
  Float_t left = (SignalMode==1) ? 0.2 : 0.7 ;
  TLegend dataLegend = MakeLegend(left,0.7,left+0.2,0.9) ;
  dataLegend.SetHeader("Legend") ;
  dataLegend.AddEntry(hTemp3,"Data"                 ,"PL") ;
  dataLegend.AddEntry(hTemp1,"Signal and background","L" ) ;
  dataLegend.AddEntry(hTemp2,"Signal"               ,"F" ) ;
  dataLegend.SetBorderSize(1) ;
  if(DataLabel)
  {
    canvas->cd(1) ;
    dataLegend.Draw() ;
  }
  PrintCanvas(canvas,printName) ;
  canvas->Clear() ;
  
  delete hTemp1 ;
  delete hTemp2 ;
  delete hTemp3 ;
  delete residuals ;
  delete frame ;
  
  
  return ;
  
  // Now make a plot for the thesis
  Int_t fillStyle[4] = { 3445 , 3006 , 3454 , 3007 } ;
  TString xAxisTitle ;
  TString yAxisTitle ;
  switch(SignalMode)
  {
  case 0: xAxisTitle = "m_{recoil}^{2}(DKX#gamma#mu) (GeV^{2}c^{ -4})" ; break ;
  case 1: xAxisTitle = "m_{recoil}^{2}(DKX#gammae) (GeV^{2}c^{ -4})"   ; break ;
  case 2: case 3: xAxisTitle = "E_{extra} (GeV)" ; break ;
  }
  yAxisTitle = (SignalMode<2) ? "entries / 0.05 GeV^{2}c^{ -4}" : "entries / 0.05 GeV" ;
  TH1F* hSignal = (rap1) ? (TH1F*)rap1->createHistogram("hSignal",*rrv) : (TH1F*)rap2->createHistogram("hSignal",*rrv) ;
  TH1F* hModel  = (TH1F*)model.createHistogram("hModel",*rrv) ;
  TH1F* hPDF    = (TH1F*)model.createHistogram("hSignal",*rrv) ;
  hTrue->GetXaxis()->SetTitle(xAxisTitle) ;
  hTrue->GetYaxis()->SetTitle(yAxisTitle) ;
  hSignal->SetFillColor(SignalModeColor[SignalMode]) ;
  hSignal->SetFillStyle(      fillStyle[SignalMode]) ;
  ThesisTH1F(hSignal) ;
  ThesisTH1F(hPDF) ;
  ThesisTH1F(hTrue) ;
  
  RooArgList args = model.coefList() ;
  RooRealVar* fSig = (RooRealVar*)args.at(0) ;
  hSignal->Scale(hTrue->GetSumOfWeights()*fSig->getVal()) ;
  DRecoResiduals* residualsThesis = new DRecoResiduals("residualsThesis", hTrue, hFit) ;
  residualsThesis->MakeFitPlotWithResiduals(canvas,NULL) ;
  TH1F* hResiduals = residualsThesis->H1NormResiduals ;
  hResiduals->SetMarkerStyle(21) ;
  hResiduals->GetYaxis()->SetTitle("") ;
  canvas->cd(1) ;
  hSignal->Draw("sames") ;
  hPDF->Draw("sames") ;
  hTrue->Draw("sames:pe") ;
  hTrue->Draw("sames:axis") ;
  left = (SignalMode==1) ? 0.2 : 0.6 ;
  TLegend thesisLegend = MakeLegend(left,0.7,left+0.3,0.9) ;
  thesisLegend.AddEntry(hTemp3    ,"Data"                 ,"PL") ;
  thesisLegend.AddEntry(hPDF      ,"Signal and background","L" ) ;
  thesisLegend.AddEntry(hSignal   ,"Signal"               ,"F" ) ;
  thesisLegend.AddEntry(hResiduals,"Residual differences" ,"p" ) ;
  thesisLegend.SetBorderSize(1) ;
  thesisLegend.Draw() ;
  canvas->Update() ;
  TString thesisPrintName = TString(TString("~/public_html/thesis/fitResults/thesis_")+printName+"_"+SignalModeName[SignalMode]+".png") ;
  canvas->Print(thesisPrintName) ;
  thesisPrintName = TString(TString("~/public_html/thesis/fitResults/thesis_")+printName+"_"+SignalModeName[SignalMode]+".eps") ;
  canvas->Print(thesisPrintName) ;
  
  
  delete fSig ;
  delete residualsThesis ;
  delete hPDF ;
  delete hModel ;
  delete hSignal ;
  delete hTemp1 ;
  delete hTemp2 ;
  delete hTemp3 ;
  delete residuals ;
  delete frame ;
  return ;
}


void DRecoDsToEllNuPlots::BumpPlots()
{
  const Int_t NW   =  11 ; // Number of weight values
  const Int_t NP   = 100 ; // Number of peel values
  const Int_t NVar =   4 ; // Number of variables
  Float_t pLower = 0 ;
  Float_t pUpper = 1 ;
  TFile* fBumps = new TFile("bumps.root", "READ") ;
  TTree* tBumps = (TTree*)fBumps->Get("bumps") ;
  Float_t w ;
  Float_t p ;
  Float_t EExtraNeutralLower   ;
  Float_t EExtraNeutralUpper   ;
  Float_t DsToEllNuDsMassLower ;
  Float_t DsToEllNuDsMassUpper ;
  Float_t Ellp3LabLower        ;
  Float_t Ellp3LabUpper        ;
  Float_t muSelectorLower      ;
  Float_t muSelectorUpper      ;
 
  TString variableName[NVar] ; TString variableTitle[NVar] ;
  variableName[0] = "EExtraNeutral"   ; variableTitle[0] = "E_{extra}"          ;
  variableName[1] = "DsToEllNuDsMass" ; variableTitle[1] = "m_{D_{s}}"          ;
  variableName[2] = "Ellp3Lab"        ; variableTitle[2] = "p^{3}_{#ell} (Lab)" ;
  variableName[3] = "MuSelector"      ; variableTitle[3] = "#ell selector"      ;
  TString limitName[2] = { "Lower" , "Upper" } ;

  tBumps->SetBranchAddress("weight"               , &w                    ) ;
  tBumps->SetBranchAddress("peel"                 , &p                    ) ;
  tBumps->SetBranchAddress("EExtraNeutralLower"   , &EExtraNeutralLower   ) ;
  tBumps->SetBranchAddress("EExtraNeutralUpper"   , &EExtraNeutralUpper   ) ;
  tBumps->SetBranchAddress("DsToEllNuDsMassLower" , &DsToEllNuDsMassLower ) ;
  tBumps->SetBranchAddress("DsToEllNuDsMassUpper" , &DsToEllNuDsMassUpper ) ;
  tBumps->SetBranchAddress("Ellp3LabLower"        , &Ellp3LabLower        ) ;
  tBumps->SetBranchAddress("Ellp3LabUpper"        , &Ellp3LabUpper        ) ;
  tBumps->SetBranchAddress("muSelectorLower"      , &muSelectorLower      ) ;
  tBumps->SetBranchAddress("muSelectorUpper"      , &muSelectorUpper      ) ;

  Float_t wValues[NW] ;
  Float_t pValues[NP] ;
  TH1F * hPeel[NW][2*NVar] ;
  for(Int_t i=0 ; i<NW ; i++)
  {
    tBumps->GetEntry(i*NP) ;
    wValues[i] = w ;
    cout << i << " " << wValues[i] << endl ;
  }
  for(Int_t i=0 ; i<NW ; i++)
  {
    for(Int_t j=0 ; j<NVar ; j++)
    {
      for(Int_t k=0 ; k<2 ; k++)
      {
    Int_t l = k + j*2 ;
    //cout << i << " " << j << " " << k << " " << l << endl ;
    TString histogramName = TString("h_" + variableName[j] + "_" + limitName[k] + "_" + FloatToString(wValues[i])) ;
    hPeel[i][l] = new TH1F(histogramName, histogramName, NP, pLower, pUpper) ;
    hPeel[i][l]->GetXaxis()->SetTitle(TString("Peel parameter (weight = " + FloatToString(wValues[i]) + ")")) ;
    hPeel[i][l]->GetYaxis()->SetTitle(variableTitle[j] + " " + limitName[k] + " limit") ;
    hPeel[i][l]->SetMarkerColor(4*i+56) ;
      }
    }
   
    for(Int_t j=0 ; j<NP ; j++)
    {
      tBumps->GetEntry(i*NP + j) ;
      pValues[j] = p ;
      if  (EExtraNeutralLower<90) hPeel[i][0]->Fill(p, EExtraNeutralLower)   ;
      if  (EExtraNeutralUpper<90) hPeel[i][1]->Fill(p, EExtraNeutralUpper)   ;      
      if(DsToEllNuDsMassLower<90) hPeel[i][2]->Fill(p, DsToEllNuDsMassLower) ;
      if(DsToEllNuDsMassUpper<90) hPeel[i][3]->Fill(p, DsToEllNuDsMassUpper) ;
      if       (Ellp3LabLower<90) hPeel[i][4]->Fill(p, Ellp3LabLower)        ;
      if       (Ellp3LabUpper<90) hPeel[i][5]->Fill(p, Ellp3LabUpper)        ;
      if     (muSelectorLower<90) hPeel[i][6]->Fill(p, muSelectorLower)      ;
      if     (muSelectorUpper<90) hPeel[i][7]->Fill(p, muSelectorUpper)      ;
    }
  }
  TCanvas cBumps("cBumps", "cBumps", 100 , 100, 500, 500) ;
  for(Int_t i=0 ; i<NW ; i++)
  {
    hPeel[i][0]->SetMaximum(1.5)  ;
    hPeel[i][1]->SetMaximum(1.5)  ;
    hPeel[i][2]->SetMaximum(3.0)  ;
    hPeel[i][3]->SetMaximum(3.0)  ;
    hPeel[i][4]->SetMaximum(0.5)  ;
    hPeel[i][5]->SetMaximum(0.5)  ;
    hPeel[i][6]->SetMaximum(32.0) ;
    hPeel[i][7]->SetMaximum(32.0) ;
    for(Int_t j=0 ; j<2*NVar ; j++)
    {
      TString printName = "../AWG90/aidanrc/plots/bump_" + date + "_" + hPeel[i][j]->GetName() + ".png" ;
      hPeel[i][j]->Draw("p") ;
      cBumps.Print(printName) ;
      cBumps.Clear() ;
    }
  }
  THStack stack[2*NVar] ;
  TLegend legend1 = MakeLegend(0.5, 0.6, 0.7, 0.2) ;
  TLegend legend2 = MakeLegend(0.7, 0.6, 0.9, 0.2) ;
  legend1.AddEntry(hPeel[0][0]  , "w=0.5" , "p") ;
  legend1.AddEntry(hPeel[1][0]  , "w=0.6" , "p") ;
  legend1.AddEntry(hPeel[2][0]  , "w=0.7" , "p") ;
  legend1.AddEntry(hPeel[3][0]  , "w=0.8" , "p") ;
  legend1.AddEntry(hPeel[4][0]  , "w=0.9" , "p") ;
  legend1.AddEntry(hPeel[5][0]  , "w=1.0" , "p") ;
  legend2.AddEntry(hPeel[6][0]  , "w=1.1" , "p") ;
  legend2.AddEntry(hPeel[7][0]  , "w=1.2" , "p") ;
  legend2.AddEntry(hPeel[8][0]  , "w=1.3" , "p") ;
  legend2.AddEntry(hPeel[9][0]  , "w=1.4" , "p") ;
  legend2.AddEntry(hPeel[10][0] , "w=1.5" , "p") ;
  
  TString plotsPrefix = "../AWG90/aidanrc/plots/" ;
  TString docsPrefix = "../docs/aidanrc/optimization/" ;

  ofstream fHTML ;
  fHTML.open("../docs/aidanrc/optimization/bumps.html") ;
  fHTML << "<p>These plots show the rectangular cuts around a box of background.</p>" << endl <<"<ul>" << endl ;
  fHTML << "<li>The FOM is significance = S/&radic;(S+B)</li>" << endl ;
  fHTML << "<li>The x-axis shows the peel parameter.</li>" << endl ;
  fHTML << "<li>The y-axis shows the optimal cut (defining a background box).</li>" << endl ;
  fHTML << "<li>The signal MC is given different weights.  The weight shown correspond to the BR in ccbar MC.</li>" << endl ;
  fHTML << "<li>Only the &mu; mode is used, choosing events from <tt>MuBDTVeryTight</tt>.</li>" << endl ;
  fHTML << "<li>The hunter is trained on Runs 1-4, 6 and tested on Run 5.</li>" << endl ;
  fHTML << "</ul>" << endl ;

  for(Int_t i=0 ; i<2*NVar ; i++)
  {
    for(Int_t j=0 ; j<NW ; j++)
    {
      stack[i].Add(hPeel[j][i]) ;
    }
    stack[i].Draw("nostack:p") ;
    stack[i].GetXaxis()->SetTitle("Peel parameter") ;
    stack[i].GetYaxis()->SetTitle(hPeel[0][i]->GetYaxis()->GetTitle()) ;
    if(i==0)
    {
      legend2.Draw() ;
      legend1.Draw() ;
    }

    TString imageName = TString(TString("bump_stack_") + date + "_" + variableName[i/2] + "_" + limitName[i%2] + ".png") ;
    TString printName = plotsPrefix + imageName ;
    cBumps.Print(printName) ;
    printName = docsPrefix + imageName ;
    cBumps.Print(printName) ;
    printName = TString(TString("BumpHunter_") + variableName[i/2] + "_" + limitName[i%2]) ;
    PrintCanvas(&cBumps, printName) ;
    cBumps.Clear() ;
    fHTML << "<img src=\"" << imageName << "\"/><br />" << endl ;
  }
  fHTML.close() ;
}

void DRecoDsToEllNuPlots::PrintBumpHunterFiles()
{
  ofstream f[2] ;
  TString fTrainName = "../AWG90/aidanrc/ana50/SPR_train.pat" ;
  TString fTestName  = "../AWG90/aidanrc/ana50/SPR_test.pat" ;
  f[0].open(fTrainName) ;
  f[1].open(fTestName) ;

  const Int_t NVarFloat = 3 ;
  const Int_t NVarInt   = 1 ;
  const Int_t NVar = NVarFloat + NVarInt ;
  Float_t mm2 ;
  chain->SetBranchAddress("mm2", &mm2 ) ;

  Float_t varFloat[NVarFloat] ;
  TString varFloatName[NVarFloat] ;
  varFloatName[0] = "EExtraNeutral"   ;
  varFloatName[1] = "DsToEllNuDsMass" ;
  varFloatName[2] = "Ellp3Lab"        ;

  Int_t varInt[NVarInt] ;
  TString varIntName[NVarInt] ;
  varIntName[0] = "MuSelector" ;

  Int_t muSel1 ;
  Int_t muSel2 ;
  Int_t muSel3 ;
  Int_t muSel4 ;
  chain->SetBranchAddress("MuBDTVeryLoose" , &muSel1) ;
  chain->SetBranchAddress("MuBDTLoose"     , &muSel2) ;
  chain->SetBranchAddress("MuBDTTight"     , &muSel3) ;
  chain->SetBranchAddress("MuBDTVeryTight" , &muSel4) ;

  f[0] << NVar << endl ;
  f[1] << NVar << endl ;
  for(Int_t i=0 ; i<NVarFloat ; i++)
  {
    chain->SetBranchAddress(varFloatName[i] , &varFloat[i] ) ;
    f[0] << varFloatName[i] << " " ;
    f[1] << varFloatName[i] << " " ;
  }
  for(Int_t i=0 ; i<NVarInt ; i++)
  {
    chain->SetBranchAddress(varIntName[i] , &varInt[i] ) ;
    f[0] << varIntName[i] << " " ;
    f[1] << varIntName[i] << " " ;
  }

  f[0] << endl ;
  f[1] << endl ;

  Int_t EllLund ;
  Int_t EKMSuperLoose ;
  chain->SetBranchAddress( "EllLund"       , &EllLund       ) ;
  chain->SetBranchAddress( "EKMSuperLoose" , &EKMSuperLoose ) ;
  chain->SetBranchAddress( "SignalMode"    , &SignalMode    ) ;

  for(Int_t i=0 ; i<chain->GetEntries() ; i++)
  {
    chain->GetEntry(i) ;
    varInt[0] = muSel1 + 2*muSel2 + 4*muSel3 + 8*muSel4 ;
    if(SignalMode==0 && muSel1==0)         continue ;
    if(SignalMode==1 && EKMSuperLoose==0)  continue ;

    if(SignalMode!=0) continue ;
    Int_t file = 0 ;
    if(run==8) file = 1 ;
    for(Int_t j=0 ; j<NVarFloat ; j++) { f[file] << varFloat[j] << " " ; }
    for(Int_t j=0 ; j<NVarInt ; j++)   { f[file] << varInt[j]   << " " ; }
    
    switch(abs(EllLund))
    {
    case 11:
    case 13:
      // Signal (leptons)
      f[file] << " 0" << endl ;
      break ;
      
    default:
      // Everything else (hadrons)
      f[file] << " 1" << endl ;
      break ;
    }
  }

  f[0].close() ;
  f[1].close() ;
  cout << "File written to " << fTrainName << endl ;
  cout << "File written to " << fTestName  << endl ;

  // Now create files to submit BumpHunter jobs
  // Vary peel, weight etc

  ofstream fJobs ;
  TString fJobsName = "../AWG90/aidanrc/ana50/SprBumpHunter.sh" ;
  fJobs.open(fJobsName) ;
  fJobs << "# Script to submit SprBumpHunter jobs" << endl ;

  Float_t wNominal = 274.0/(3224.0+274.0) ;
  const Int_t NW = 11 ;
  const Int_t NPeel = 100 ;
  Float_t w[NW] ;
  Float_t peel[NPeel] ;
  for(Int_t i=0 ; i<NW ; i++)
  {
    Float_t j = 0.5+0.1*i ;
    w[i] = wNominal*j ;
  }
  for(Int_t i=0 ; i<NPeel ; i++)
  {
    peel[i] = 0.01*i ;
  }

  for(Int_t i=0 ; i<NW ; i++)
  {
    char buffer[50] ;
    sprintf(buffer, "%.5f", w[i]);
    TString wString = TString(buffer) ;
    for(Int_t j=0 ; j<NPeel ; j++)
    {
      char buffer[50] ;
      sprintf(buffer, "%.3f", peel[j]);
      TString peelString =  TString(buffer) ;

      fJobs << 
    "bsub -q short" <<
    " -o SprBumpHunterLogs/w" << wString << "_p" << peelString << ".log" <<
    " \"" <<
    "SprBumpHunterApp SPR_train.pat " << 
    "-t SPR_test.pat " << " -x " << peel[j] << " -w " << w[i] << 
    " -o SprBumpHunterRoots/w" << wString << "_p" << peelString << ".root " <<
    " -f SprBumpHunterBumps/w" << wString << "_p" << peelString << ".bump " <<
    "\"" << endl ;
    }
  }
  fJobs.close() ;
  cout << "File written to " << fJobsName << endl ;
}

void DRecoDsToEllNuPlots::KKsRoofit()
{
  SignalMode = 5 ;
  EExtraCut  = 1 ;
  TCanvas *cKsK = new TCanvas("cKsK","cKsK",100,100,500,500) ;
  const Int_t NFits = 4 ;
  Float_t lower = 0 ;
  Float_t upper = 0 ;
  Float_t lowers[2] ;
  Float_t uppers[2] ;
  lowers[0] = 0.0  ; uppers[0] = 0.6  ;
  lowers[1] = 0.16 ; uppers[1] = 0.36 ;
  Int_t nBins = 80  ;

  TString KSelectorString[6] ;
  KSelectorString[0] = "ChargedTracks" ;
  KSelectorString[1] = "KBDTNotAPion" ;
  KSelectorString[2] = "KBDTVeryLoose" ;
  KSelectorString[3] = "KBDTLoose" ;
  KSelectorString[4] = "KBDTTight" ;
  KSelectorString[5] = "KBDTVeryTight" ;

  TString FitType[NFits] ;
  FitType[0] = "MMMC"     ;
  FitType[1] = "ListMC"   ;
  FitType[2] = "MMData"   ;
  FitType[3] = "ListData" ;

  TString SPModeName[7] ; 
  SPModeName[0] = "Data"   ;
  SPModeName[1] = "SP998"  ;
  SPModeName[2] = "SP1005" ;
  SPModeName[3] = "SP1235" ;
  SPModeName[4] = "SP1237" ;
  SPModeName[5] = "SP3429" ;
  SPModeName[6] = "AllMC"  ;

  TFile* file[NSPModes][NRuns-1] ;
  TTree* ntuple[NSPModes][NRuns-1] ;
  for(Int_t i=1 ; i<3 ; i++)
  {
    for(Int_t run=0 ; run<NRuns-5 ; run++) // Runs 1-6
    {
      if(i==1 && run!=8) continue ;
      TString fileName = TString(TString("~cmmalone/BaBar/ana50/workdir/DsToKKs/")+ SPModeName[i] + "/" + RunName[run] + "/Ntuple_New.root") ;
      file[i][run]   = new TFile(fileName, "read") ;
      ntuple[i][run] = (TTree*)file[i][run]->Get("Ntuple") ;
    }
  }
  TChain dataChain("Ntuple") ;
  cout << "Added " << dataChain.Add(TString("/a/sulky71/AWG90/aidanrc/output/R24/DsToKKs/R24-Data_*")) << " data files" << endl ;

  TString SPModeWeight[NSPModes] ;
  SPModeWeight[0] = "1.0" ;    //Data
  SPModeWeight[1] = "0.88" ;   //998
  SPModeWeight[2] = "0.4725" ; //1005
  SPModeWeight[3] = "1.0" ;    //1235
  SPModeWeight[4] = "1.0" ;    //1237
  SPModeWeight[5] = "1.0" ;    //3429

  RooDataHist*  r[NFits] ;
  RooRealVar*   f[NFits] ;
  RooRealVar*  f2[NFits] ;

  //TString WeightString[NSPModes] ;
  TH1F* histograms[NSPModes][NRuns-1][NFits] ;

  TCanvas cStack("cStack", "", 0, 0, 500, 500); 
  THStack stack[NFits] ;

  TH1F*              histToFit[NFits] ;
  DRecoResiduals*    residuals[NFits] ;
  TH1F*           h_datafitOut[NFits] ;
  TH1F*                 MCSum_Hist[2] ;
  TH1F*                  data_Hist[2] ;

  Float_t  meanValue[NFits] ;
  Float_t  meanError[NFits] ;
  Float_t sigmaValue[NFits] ;
  Float_t sigmaError[NFits] ;
  Int_t selector = 2 ;
  Float_t trueMCSignalYield          = -1 ;
  Float_t fittedMCSignalYield        = -1 ;
  Float_t fittedMCSignalYieldError   = -1 ;
  Float_t fittedDataSignalYield      = -1 ;
  Float_t fittedDataSignalYieldError = -1 ;

  for(Int_t i=0; i<NFits; i++)
  {
    lower = lowers[i%2] ;
    upper = uppers[i%2] ;
    
    RooRealVar KsMass("KsMass" , "m_{reco}^{2}(K_{S}) (GeV/c^{2})" , lower, upper ) ;
    KsMass.setBins(nBins) ;
    TString VarString = ((i%2)==0) ? "(ksmassMMCFit2)" : "(ksmassListCFit2)" ;
    TString CutString = ((i%2)==0) ? "((abs(dsmassMMRaw-1.968)<0.1)*(eextra<1)*(ksmassMMCFit2>0)*(probchi2MMCFit>0.00001))" 
      : TString(
        TString("(") + 
        TString("(eextra<1)")                              + TString("*") +
        TString("(abs(dsmassMMRaw-1.968)<0.1)")            + TString("*") +
        TString("(probchi2ListCFit>0.00001)")              + TString("*") + 
        TString("(ksmassListUFit2>0)")                     + TString("*") + 
        //TString("(FitStatusUFit==0)")                      + TString("*") +
        TString("(abs(sqrt(dsmassListRaw2)-1.968)<0.1)")  + TString("*") +
        TString("(abs(sqrt(ksmassListRaw2)-0.498)<0.1)")  + 
        TString(")")
        );
    if(i<2)
    {
      // Histogram and stack SP modes
      TH1F hCcBar("hCcBar", "", nBins, lower, upper) ;
      hCcBar.SetFillColor(kBlue) ;
      cout << endl << endl ;
      for(Int_t k=1 ; k<3 ; k++)
      {
        for(Int_t l=0 ; l<NRuns-5 ; l++)
        {
          if(k==1 && l!=8) continue ;
          TString histogramName = TString(TString("hKsK") + SPModeName[k] + RunName[l] + FitType[i] + (long)i) ;
          histograms[k][l][i] = new TH1F(histogramName, histogramName, nBins, lower, upper) ;
          TString weight = TString (SPModeWeight[k] + "*(" + KSelectorString[selector] + "==1)*" + CutString) ;
          cout << i << " " << k << " " << l << " " << weight << " " << ntuple[k][l]->Draw(TString(VarString + ">>" + histogramName), weight, "goff") << endl ;
          histograms[k][l][i]->SetFillColor(k+2) ;
          if(k==2)
          {
            for(Int_t bin=1 ; bin<nBins+1 ; bin++){ hCcBar.SetBinContent(bin, hCcBar.GetBinContent(bin)+histograms[k][l][i]->GetBinContent(bin)) ;
          }
        }
      }
    }
    cout << endl << endl ;
    stack[i].Add(histograms[1][8][i]) ;
    stack[i].Add(&hCcBar) ;
    // Printing stuff for stacked SP modes histogram
    stack[i].Draw() ;
    stack[i].GetXaxis()->SetTitle("K_{s} " + TString(FitType[i]) + " Mass (GeV/c^{2})") ;
    stack[i].GetYaxis()->SetTitle("entries / 0.02 GeV/c^{2}") ;
    TLegend legend(0.8, 0.8, 0.9, 0.9);
    legend.AddEntry( histograms[2][0][i] , "ccbar", "f");
    legend.AddEntry( histograms[1][8][i] , "uds"  , "f");
    legend.SetFillColor(kWhite) ;
    legend.Draw();
    PrintCanvas(cKsK, TString(TString("KsK_") + FitType[i])) ;     
      
    // Creates MC histogram we use for fitting  
    MCSum_Hist[i] = new TH1F(TString(TString("hMCSum")+FitType[i]), "MCSum_Hist", nBins, lower, upper) ;
    for(Int_t j=0 ; j<nBins+1 ; j++)
    {
      MCSum_Hist[i]->SetBinContent(j,0) ;
      for(Int_t k=1 ; k<3 ; k++)
      {
        for(Int_t l=0 ; l<NRuns-5 ; l++)
        {
          if(k==1 && l!=8) continue ;
          MCSum_Hist[i]->SetBinContent(j, MCSum_Hist[i]->GetBinContent(j)+histograms[k][l][i]->GetBinContent(j)) ;
        }
      }
      }
      MCSum_Hist[i]->Draw() ;
      histToFit[i] = MCSum_Hist[i] ;
    }
    else
    {
      // Fill data histogram
      cout << endl << endl ;
      data_Hist[i-2] = new TH1F(TString(TString("h")+FitType[i]), "", nBins, lower, upper) ;
      TString weight = TString(CutString + "*(" + KSelectorString[selector]+"==1)") ;
      cout << "Data " << i << " " << weight << " " << dataChain.Draw(TString( VarString + ">>h" + FitType[i]), weight, "goff") << endl ; 
      histToFit[i] = data_Hist[i-2] ;
      cout << endl << endl ;
    }
    
    TString rooDataHistName = "r" ;
    rooDataHistName = TString(rooDataHistName + FitType[i]) ;
    r[i] = new RooDataHist(rooDataHistName, rooDataHistName, KsMass, histToFit[i]) ;
    // r[i] is the RooDataHist we're going to fit to
    
    TString fName = "f" ;
    fName = TString(fName + FitType[i]) ;
    f[i] = new RooRealVar(fName, fName, 0.1, 0.0, 1.0) ;

    fName = TString(fName + FitType[i] +"2") ;
    f2[i] = new RooRealVar(fName, fName, 0.1, 0.0, 1.0) ;

    RooRealVar     mean("mean"   , "mean"   , 0.25, 0.1 , 0.5) ;
    RooRealVar    sigma("sigma"  , "sigma"  , 0.01, 0.0005 , 0.05 ) ;
    RooGaussian SigGaus("siggaus", "siggaus", KsMass, mean, sigma) ;

    RooRealVar     mean2("mean2"   , "mean2"   , 0.25, 0.1 , 0.5) ;
    RooRealVar    sigma2("sigma2"  , "sigma2"  , 0.01, 0.0005 , 0.05 ) ;
    RooGaussian SigGaus2("siggaus2", "siggaus2", KsMass, mean2, sigma2) ;

    RooRealVar cheb0("cheb0", "cheb0", 0 , -1, 1) ;
    RooRealVar cheb1("cheb1", "cheb1", 0 , -1, 1) ;
    RooRealVar cheb2("cheb2", "cheb2", 0 , -1, 1) ;
    RooChebychev BkgCheb("BkgCheb", "cheb", KsMass, RooArgList(cheb0, cheb1)) ; //, cheb2));

    RooAddPdf* model ;
    if(i%2==0) model = new RooAddPdf("model", "model", RooArgList(SigGaus, BkgCheb )           , RooArgList(*f[i])          ) ;
    //if(i==1) model = new RooAddPdf("model", "model", RooArgList(SigGaus, SigGaus2, BkgCheb ) , RooArgList(*f[i] , *f2[i]) ) ;
    if(i==1) model = new RooAddPdf("model", "model", RooArgList(SigGaus, BkgCheb ) , RooArgList(*f[i] ) ) ;
    if(i==3) model = new RooAddPdf("model", "model", RooArgList(SigGaus, BkgCheb ) , RooArgList(*f[i] ) ) ;
    model->fitTo( *r[i] ) ;
      
    RooPlot* KsMass_frame = KsMass.frame() ;
    Float_t integral = histToFit[i]->GetSumOfWeights() ;
    h_datafitOut[i] = (TH1F*)model->createHistogram("dataHist", KsMass) ;  
    h_datafitOut[i]->Scale(integral) ;
    r[i]->plotOn(KsMass_frame, DataError(RooAbsData::SumW2)) ;
    model->plotOn(KsMass_frame) ;
    model->plotOn(KsMass_frame, Components(SigGaus) , LineStyle(kSolid) , LineColor(kRed)) ;
    
    TCanvas cResid("cResid", "", 0, 0, 500, 500);
    TString residualsName = "resids" ;
    residuals[i] = new DRecoResiduals(TString(residualsName+FitType[i]), histToFit[i], h_datafitOut[i]) ;
    residuals[i]->MakeFitPlotWithResiduals(&cResid, KsMass_frame) ;
    PrintCanvas(&cResid, TString(TString("KsK_Roofit_") + FitType[i]) + "_residuals") ;
    
    meanValue[i]  =  mean.getVal()   ;
    meanError[i]  =  mean.getError() ;
    sigmaValue[i] = sigma.getVal()   ;
    sigmaError[i] = sigma.getError() ;

    if(i==2)
    {
      fittedDataSignalYield      = ( f[i]->getVal()   )*integral ;
      fittedDataSignalYieldError = ( f[i]->getError() )*integral ;
      cout << endl << endl << "Data yield: " << fittedDataSignalYield << " +/- " << fittedDataSignalYieldError << endl << endl ;
    }
    if(i==0)
    {
      fittedMCSignalYield      = ( f[i]->getVal()   )*integral ;
      fittedMCSignalYieldError = ( f[i]->getError() )*integral ;
      cout << endl << endl << "MC yield: " << fittedMCSignalYield << " +/- " << fittedMCSignalYieldError << endl << endl ;
    }
    delete model ;
  }

  TH1F* hMean = new TH1F("hMean", "hMean", 4, 0, 4) ;
  hMean->GetXaxis()->SetTitle("Fit") ;
  hMean->GetYaxis()->SetTitle("Mean value of gaussian fit / MeV/c^{2}") ;
  hMean->SetLineColor(kRed) ;
  hMean->SetLineWidth(2) ;
  hMean->GetYaxis()->SetLabelSize(0.02) ;

  TH1F* hSigma = new TH1F("hSigma", "hSigma", 4, 0, 4) ;
  hSigma->GetXaxis()->SetTitle("Fit") ;
  hSigma->GetYaxis()->SetTitle("#sigma of gaussian fit / MeV/c^{2}") ;
  hSigma->SetLineColor(kBlue) ;
  hSigma->SetLineWidth(2) ;

  cout << endl << endl << "==========" << endl << " Results:" << endl << "==========" << endl ;
  for(Int_t i=0 ; i<NFits ; i++)
  {
    hMean->SetBinContent( i+1, (Double_t) meanValue[i]) ;
    hMean->SetBinError(   i+1, (Double_t) meanError[i]) ;
    hSigma->SetBinContent(i+1, sigmaValue[i]) ;
    hSigma->SetBinError(  i+1, sigmaError[i]) ;

    hMean->GetXaxis()->SetBinLabel( i+1, FitType[i]) ;
    hSigma->GetXaxis()->SetBinLabel(i+1, FitType[i]) ;

    cout << FitType[i] << ":" << endl ;
    cout << "       mean = " <<  meanValue[i] << endl ;
    cout << "  mean error= " <<  meanError[i] << endl ;
    cout << "      sigma = " << sigmaValue[i] << endl ;
    cout << " sigma error= " << sigmaError[i] << endl ;
    cout << endl ;
  }

  hMean->Draw("p:e") ;
  PrintCanvas(cKsK,TString("KsK_Means")) ;
  cKsK->Clear() ;
  hSigma->Draw("p:e") ;
  PrintCanvas(cKsK,TString("KsK_Sigmas")) ;

  cout << "  True MC signal yield   = " << trueMCSignalYield*0.5      << endl ;
  cout << "Fitted MC signal yield   = " << fittedMCSignalYield        << endl ;
  cout << "Fitted MC yield error    = " << fittedMCSignalYieldError   << endl ;
  cout << "Fitted Data signal yield = " << fittedDataSignalYield      << endl ;
  cout << "Fitted Data yield error  = " << fittedDataSignalYieldError << endl ;

  TString prefix = TString("../AWG90/aidanrc/output/R24/DsToKKs/") ;
  TChain* MCChain = new TChain("Ntuple") ;
  MCChain->Add(TString(prefix+"R24-SP1005*")) ;
  TChain* DataChain = new TChain("Ntuple") ;
  DataChain->Add(TString(prefix+"R24-Data*")) ;

  Float_t fitLower = 0.0 ;
  Float_t fitUpper = 1.0 ;
  nBins = 50 ;
  RooRealVar mm2("mm2" , "m_{miss}^{2} (GeV^{2}/c^{4})" , fitLower, fitUpper ) ;
  mm2.setBins(nBins) ;

  TH1F*          hFit[2] ;
  RooDataHist*   rFit[2] ;
  RooHistPdf*  rhpFit[2] ;
  TString typeName[2] = { "MC" , "Data" } ;
  Int_t color[2] = { 2 , 3 } ;
  for(Int_t i=0 ; i<2 ; i++)
  {
    hFit[i]   = new        TH1F(TString(TString("h")   + typeName[i])  , TString(TString("h")   + typeName[i]) , nBins, fitLower, fitUpper) ;
    rFit[i]   = new RooDataHist(TString(TString("r")   + typeName[i])  , TString(TString("r")   + typeName[i]) , mm2 ,  hFit[0] ) ;
    rhpFit[i] = new  RooHistPdf(TString(TString("rhp") + typeName[i])  , TString(TString("rhp") + typeName[i]) , mm2 , *rFit[0] ) ;
    hFit[i]->GetXaxis()->SetTitle(TString("m_{rec}^{2}(DKX#gammaK) / GeV^{2}/c^{4}")) ;
    hFit[i]->GetYaxis()->SetTitle(TString(TString("entries per ") + FloatToString((fitUpper-fitLower)/nBins) + " per GeV^{2}/c^{4}")) ;
    hFit[i]->SetFillColor(color[i]) ;
  }
  MCChain->Draw(  TString("ksmassMMCFit2>>hMC")  , TString("sqrt(ksmassMMCFit2)<1 && KBDTLoose==1 && abs(dsmassMMRaw-1.968)<0.15 && probchi2MMCFit>1e-15")) ;
  PrintCanvas(cKsK, "KsK_MM_MC") ;
  cKsK->Clear() ;
  DataChain->Draw(TString("ksmassMMCFit2>>hData"), TString("sqrt(ksmassMMCFit2)<1 && KBDTLoose==1 && abs(dsmassMMRaw-1.968)<0.15 && probchi2MMCFit>1e-15")) ;
  PrintCanvas(cKsK, "KsK_MM_Data") ;
  cKsK->Clear() ;
}

void DRecoDsToEllNuPlots::EllTruth()
{
  TCanvas cBackgroundSources("cBackgroundSources", "Mother modes", 100, 100, 500, 500) ;
  ofstream fBAD ;
  const Int_t NCharged = 5 ;
  Int_t decay   ;
  Int_t Charged ;
  Int_t Mother  ;
  Int_t maxDauLen = 0 ;
  TString EllName[NSignalMode] ;
  EllName[0] = "Ds -> mu nu"                    ;
  EllName[1] = "Ds -> e nu"                     ;
  EllName[2] = "Ds -> tau nu ; tau -> mu nu nu" ;
  EllName[3] = "Ds -> tau nu ;  tau -> e nu nu" ;
  EllName[4] = "Ds -> tau nu ; tau -> pi nu"    ;
  TString EllDecayLaTeX[NSignalMode] ;
  EllDecayLaTeX[0] = "D_s^+\\to\\mu\\nu" ;
  EllDecayLaTeX[1] = "D_s^+\\to e\\nu"   ;
  EllDecayLaTeX[2] = "D_s^+\\to\\tau\\nu;\\tau\\to\\mu\\nu\\nu"  ;
  EllDecayLaTeX[3] = "D_s^+\\to\\tau\\nu;\\tau\\to e\\nu\\nu"    ;
  EllDecayLaTeX[4] = "D_s^+\\to\\tau\\nu;\\tau\\to\\pi\\nu"      ;
  cout << chain->GetEntries() << " entries" << endl ;
  
  // MC truth information
  Int_t   EllMothDauLen  = 0 ;
  Int_t   EllMothLund    = 0 ;
  Int_t   EllSib1Lund    = 0 ;
  Int_t   EllSib2Lund    = 0 ;
  Int_t   EllSib3Lund    = 0 ;
  Int_t   EllSib4Lund    = 0 ;
  Int_t   EllSib5Lund    = 0 ;
  Int_t   EllSib6Lund    = 0 ;
  Int_t   EllSib7Lund    = 0 ;
  Int_t   EllSib8Lund    = 0 ;
  Int_t   EllSib9Lund    = 0 ;
  Int_t   EllSib10Lund   = 0 ;
  Int_t   EllSib11Lund   = 0 ;
  Int_t   EllSib12Lund   = 0 ;
  Int_t   EllSib13Lund   = 0 ;
  Int_t   EllSib14Lund   = 0 ;
  Int_t   EllMode        = 0 ;
  Float_t mm2            = 0 ;
  Float_t EExtraNeutral  = 0 ;
  Int_t   NExtraTracks   = 0 ;
  Float_t dsmass         = 0 ;
  Float_t Pip3Ds         = 0 ;
  Int_t   MuBDTVeryLoose = 0 ;
  Int_t   MuBDTLoPLoose  = 0 ;
  Int_t   MuBDTVeryTight = 0 ;
  Int_t   EKMSuperLoose  = 0 ;
  Int_t   KGLHTight      = 0 ;
  Int_t   PiKMLoose      = 0 ;
  Int_t   PiKMSuperTight = 0 ;
  Int_t   KBDTVeryLoose  = 0 ;
  
  chain->SetBranchAddress("EllMothDauLen"  , &EllMothDauLen  ) ;
  chain->SetBranchAddress("EllMothLund"    , &EllMothLund    ) ;
  chain->SetBranchAddress("EllSib1Lund"    , &EllSib1Lund    ) ;
  chain->SetBranchAddress("EllSib2Lund"    , &EllSib2Lund    ) ;
  chain->SetBranchAddress("EllSib3Lund"    , &EllSib3Lund    ) ;
  chain->SetBranchAddress("EllSib4Lund"    , &EllSib4Lund    ) ;
  chain->SetBranchAddress("EllSib5Lund"    , &EllSib5Lund    ) ;
  chain->SetBranchAddress("EllSib6Lund"    , &EllSib6Lund    ) ;
  chain->SetBranchAddress("EllSib7Lund"    , &EllSib7Lund    ) ;
  chain->SetBranchAddress("EllSib8Lund"    , &EllSib8Lund    ) ;
  chain->SetBranchAddress("EllSib9Lund"    , &EllSib9Lund    ) ;
  chain->SetBranchAddress("EllSib10Lund"   , &EllSib10Lund   ) ;
  chain->SetBranchAddress("EllSib11Lund"   , &EllSib11Lund   ) ;
  chain->SetBranchAddress("EllSib12Lund"   , &EllSib12Lund   ) ;
  chain->SetBranchAddress("EllSib13Lund"   , &EllSib13Lund   ) ;
  chain->SetBranchAddress("EllSib14Lund"   , &EllSib14Lund   ) ;
  chain->SetBranchAddress("EllMode"        , &EllMode        ) ;
  chain->SetBranchAddress("mm2"            , &mm2            ) ;
  chain->SetBranchAddress("EExtraNeutral"  , &EExtraNeutral  ) ;
  chain->SetBranchAddress("NExtraTracks"   , &NExtraTracks   ) ;
  chain->SetBranchAddress("dsmass"         , &dsmass         ) ;
  chain->SetBranchAddress("Pip3Ds"         , &Pip3Ds         ) ;
  chain->SetBranchAddress("MuBDTVeryLoose" , &MuBDTVeryLoose ) ;
  chain->SetBranchAddress("MuBDTLoPLoose"  , &MuBDTLoPLoose  ) ;
  chain->SetBranchAddress("MuBDTVeryTight" , &MuBDTVeryTight ) ;
  chain->SetBranchAddress("EKMSuperLoose"  , &EKMSuperLoose  ) ;
  chain->SetBranchAddress("KGLHTight"      , &KGLHTight      ) ;
  chain->SetBranchAddress("PiKMLoose"      , &PiKMLoose      ) ;
  chain->SetBranchAddress("PiKMSuperTight" , &PiKMSuperTight ) ;
  chain->SetBranchAddress("KBDTVeryLoose"  , &KBDTVeryLoose  ) ;
  
  // General decay information
  Int_t nTooManyDaughters[NSignalMode] = { 0 , 0 , 0 , 0 , 0 } ;  

  TString chargedName[NCharged]      = { "       e" , "      mu" , "      pi" , "       K" , "       p" } ;
  TString chargedNameShort[NCharged] = {        "e" ,      "#mu" ,      "#pi" ,        "K" ,        "p" } ;
  Int_t chargedLund[NCharged] = { 11 , 13 , 211 , 321 , 2212 } ;
  const Int_t NMothType = 14 ;
  Int_t nMoth[NMothType][NSignalMode][NCharged][NEExtraCut] ;
  TString mothName[NMothType] ;
  mothName[0]  = "g"          ;
  mothName[1]  = "D+"         ;
  mothName[2]  = "D0"         ;
  mothName[3]  = "Ds"         ;
  mothName[4]  = "Lc"         ;
  mothName[5]  = "tau"        ;
  mothName[6]  = "h"          ;
  mothName[7]  = "K"          ;
  mothName[8]  = "D*"         ;
  mothName[9]  = "e"          ;
  mothName[10] = "baryon"     ;
  mothName[11] = "vpho"       ;
  mothName[12] = "B or J/psi" ;
  mothName[13] = "other"      ;
  //  0  Gamma
  //  1  DPlus
  //  2  D0
  //  3  Ds
  //  4  LambdaC
  //  5  Tau
  //  6  H
  //  7  K
  //  8  DStar
  //  9  E
  // 10  Baryon
  // 11  VirtualGamma
  // 12  B or J/psi
  // 13  Other
  Int_t nTrueLund[NSignalMode][NCharged][NEExtraCut] ;
  for(Mother=0 ; Mother<NMothType ; Mother++)
  {  
    for(SignalMode=0 ; SignalMode<NSignalMode-1 ; SignalMode++)
    {
      for(Charged=0 ; Charged<NCharged ; Charged++)
      {
    for(EExtraCut=0 ; EExtraCut<2 ; EExtraCut++) // EExta cut
    {
      nMoth[Mother][SignalMode][Charged][EExtraCut] = 0 ;
      nTrueLund[SignalMode][Charged][EExtraCut] = 0 ;
    }
      }
    }
  }
  Int_t n_EllIsUnknown[NSignalMode][2] ;
  for(Int_t i=0 ; i<NSignalMode-1 ; i++)
  {
    for(Int_t j=0 ; j<2 ; j++)
    {
      n_EllIsUnknown[i][j] = 0 ;
    }
  }
  
  // Specific decays
  const Int_t NDecayTypes = 10000 ;
  TString decayName[NDecayTypes] ;
  TString decayNameLaTeX[NDecayTypes] ;
  Int_t n[NSignalMode][NDecayTypes][NEExtraCut] ; // Ell, Decay, EExtra
  for(SignalMode=0 ; SignalMode<NSignalMode-1 ; SignalMode++)
  {
    for(decay=0 ; decay<NDecayTypes ; decay++)
    {
      for(EExtraCut=0 ; EExtraCut<2 ; EExtraCut++)
      {
    n[SignalMode][decay][EExtraCut] = 0 ;
      }
    }
  }
  
  // One prong
  decayName[0]  = "g --> e-"                  ; decayNameLaTeX[0]    = "\\gamma\\to e^-"                                             ;
  decayName[1]  = "g --> e+"                  ; decayNameLaTeX[1]    = "\\gamma\\to e^+"                                             ;
  decayName[2]  = "g --> mu-"                 ; decayNameLaTeX[2]    = "\\gamma\\to \\mu^-"                                          ;
  decayName[3]  = "g --> mu+"                 ; decayNameLaTeX[3]    = "\\gamma\\to \\mu^+"                                          ;
  decayName[4]  = "g --> pi+"                 ; decayNameLaTeX[4]    = "\\gamma\\to \\pi^+"                                          ;
  decayName[5]  = "g --> pi-"                 ; decayNameLaTeX[5]    = "\\gamma\\to \\pi^-"                                          ;
  decayName[6]  = "g --> K+"                  ; decayNameLaTeX[6]    = "\\gamma\\to K^+"                                             ;
  decayName[7]  = "g --> K-"                  ; decayNameLaTeX[7]    = "\\gamma\\to K^-"                                             ;
  decayName[8]  = "g --> p+"                  ; decayNameLaTeX[8]    = "\\gamma\\to p^+"                                             ;
  decayName[9]  = "g --> p-"                  ; decayNameLaTeX[9]    = "\\gamma\\to \\bar{p}^-"                                      ;
  decayName[10] = "g --> n"                   ; decayNameLaTeX[10]   = "\\gamma\\to n"                                               ;
  decayName[11] = "g --> anti-n"              ; decayNameLaTeX[11]   = "\\gamma\\to \\bar{n}"                                        ;
  decayName[12] = "g --> He++"                ; decayNameLaTeX[12]   = "\\gamma\\to He^{++}"                                         ;
  decayName[13] = "g --> He--"                ; decayNameLaTeX[13]   = "\\gamma\\to He^{--}"                                         ;
  // Compton scatter                                                                         
  // Lepton production                                                                       
  decayName[14] = "g --> g e-"                ; decayNameLaTeX[14]   = "\\gamma\\to\\gamma"                                          ;
  decayName[15] = "g --> g e+"                ; decayNameLaTeX[15]   = "\\gamma\\to\\gamma"                                          ;
  decayName[16] = "g --> g mu-"               ; decayNameLaTeX[16]   = "\\gamma\\to\\gamma"                                          ;
  decayName[17] = "g --> g mu+"               ; decayNameLaTeX[17]   = "\\gamma\\to\\gamma"                                          ;
  decayName[18] = "g --> e- e-"               ; decayNameLaTeX[18]   = "\\gamma\\to e^-e^-"                                          ;
  decayName[19] = "g --> e+ e+"               ; decayNameLaTeX[19]   = "\\gamma\\to e^+e^+"                                          ;
  decayName[20] = "g --> e+ e-"               ; decayNameLaTeX[20]   = "\\gamma\\to e^+e^-"                                          ;
  decayName[21] = "g --> mu- mu-"             ; decayNameLaTeX[21]   = "\\gamma\\to\\mu^-\\mu^-"                                     ;
  decayName[22] = "g --> mu+ mu+"             ; decayNameLaTeX[22]   = "\\gamma\\to\\mu^+\\mu^+"                                     ;
  decayName[23] = "g --> mu+ mu-"             ; decayNameLaTeX[23]   = "\\gamma\\to\\mu^+\\mu^-"                                     ;
  // Hadron production                                                                       
  decayName[24] = "g --> g pi+"               ; decayNameLaTeX[24]   = "\\gamma\\to\\gamma\\pi^+"                                    ;
  decayName[25] = "g --> g pi-"               ; decayNameLaTeX[25]   = "\\gamma\\to\\gamma\\pi^-"                                    ;
  decayName[26] = "g --> g K+"                ; decayNameLaTeX[26]   = "\\gamma\\to\\gamma K^+"                                      ;
  decayName[27] = "g --> g K-"                ; decayNameLaTeX[27]   = "\\gamma\\to\\gamma K^-"                                      ;
  decayName[28] = "g --> g p+"                ; decayNameLaTeX[28]   = "\\gamma\\to\\gamma p^+"                                      ;
  decayName[29] = "g --> g p-"                ; decayNameLaTeX[29]   = "\\gamma\\to\\gamma \\bar{p}^-"                               ;
  decayName[30] = "g --> g n"                 ; decayNameLaTeX[30]   = "\\gamma\\to\\gamma n"                                        ;
  decayName[31] = "g --> g anti-n"            ; decayNameLaTeX[31]   = "\\gamma\\to\\gamma \\bar{n}"                                 ;
  decayName[32] = "g --> g He++"              ; decayNameLaTeX[32]   = "\\gamma\\to\\gamma He^{++}"                                  ;
  decayName[33] = "g --> g He--"              ; decayNameLaTeX[33]   = "\\gamma\\to\\gamma He^{--}"                                  ;
  // Hadron pair production                                                                  
  decayName[34] = "g --> pi+ pi-"             ; decayNameLaTeX[34]   = "\\gamma\\to\\pi^+\\pi^-"                                     ;
  decayName[35] = "g --> K+ K-"               ; decayNameLaTeX[35]   = "\\gamma\\to K^+K^-"                                          ;
  decayName[36] = "g --> p+ p-"               ; decayNameLaTeX[36]   = "\\gamma\\to p^+\\bar{p}^-"                                   ;
  decayName[37] = "g --> n anti-n"            ; decayNameLaTeX[37]   = "\\gamma\\to n\\bar{n}"                                       ;
  decayName[38] = "g --> He++ He++"           ; decayNameLaTeX[38]   = "\\gamma\\to He^{++}He^{++}"                                  ;
  decayName[39] = "g --> He++ He--"           ; decayNameLaTeX[39]   = "\\gamma\\to He^{++}He^{--}"                                  ;
  decayName[40] = "g --> He-- He--"           ; decayNameLaTeX[40]   = "\\gamma\\to He^{--}He^{--}"                                  ;
  // More lepton production                                                                  
  decayName[41] = "g --> g g e-"              ; decayNameLaTeX[41]   = "\\gamma\\to\\gamma\\gamma e^-"                               ;
  decayName[42] = "g --> g g e+"              ; decayNameLaTeX[42]   = "\\gamma\\to\\gamma\\gamma e^+"                               ;
  decayName[43] = "g --> g e- e-"             ; decayNameLaTeX[43]   = "\\gamma\\to\\gamma e^-e^-"                                   ;
  decayName[44] = "g --> g e+ e+"             ; decayNameLaTeX[44]   = "\\gamma\\to\\gamma e^+e^+"                                   ;
  decayName[45] = "g --> g e+ e-"             ; decayNameLaTeX[45]   = "\\gamma\\to\\gamma e^+e^-"                                   ;
  decayName[46] = "g --> e- e+ e-"            ; decayNameLaTeX[46]   = "\\gamma\\to e^-e^+e^-"                                       ;
  decayName[47] = "g --> e+ e+ e-"            ; decayNameLaTeX[47]   = "\\gamma\\to e^+e^+e^-"                                       ;
  decayName[48] = "g --> e- p+"               ; decayNameLaTeX[48]   = "\\gamma\\to e^-p^+"                                          ;
  decayName[99] = "g --> too many"            ; decayNameLaTeX[99]   = "\\gamma\\to other"                                           ;
  // D+                                                                              
  // 2 body                                                                          
  // Leptonic                                                                            
  decayName[1200] = "D+ --> e+ nu_e"          ; decayNameLaTeX[1200] = "D^+\\to e^+         \\nu_e"                                  ;
  decayName[1201] = "D- --> e- nu_e"          ; decayNameLaTeX[1201] = "D^+\\to e^-   \\bar{\\nu}_e"                                 ;
  decayName[1202] = "D+ --> mu+ nu_mu"        ; decayNameLaTeX[1202] = "D^+\\to \\mu^+      \\nu_\\mu"                               ;
  decayName[1203] = "D- --> mu- nu_mu"        ; decayNameLaTeX[1203] = "D^+\\to \\mu^-\\bar{\\nu}_\\mu"                              ;
  // Hadronic                                                                        
  decayName[1225] = "D+ --> pi+ phi"          ; decayNameLaTeX[1225] = "D^\\pm\\to\\pi^\\pm\\phi" ;                          
  decayName[1226] = "D+ --> pi+ K0"           ; decayNameLaTeX[1226] = "D^\\pm\\to\\pi^\\pm        K^0"                              ;
  decayName[1227] = "D+ --> pi*+ K0"          ; decayNameLaTeX[1227] = "D^\\pm\\to h^{\\star \\pm} K^0"                              ;
  decayName[1228] = "D+ --> pi+ K0*"          ; decayNameLaTeX[1228] = "D^\\pm\\to \\pi\\pm        K^{\\star 0}"                     ;
  decayName[1229] = "D+ --> h*+ K0*"          ; decayNameLaTeX[1229] = "D^\\pm\\to h^{\\star \\pm} K^{\\star 0}"                     ;
  decayName[1230] = "D+ --> pi0 K-"           ; decayNameLaTeX[1230] = "D^\\pm\\to \\pi^0          K^\\pm"                           ;
  decayName[1231] = "D+ --> h*0 K-"           ; decayNameLaTeX[1231] = "D^\\pm\\to h^{\\star 0}    K^\\pm"                           ;
  decayName[1232] = "D+ --> pi0 K-*"          ; decayNameLaTeX[1232] = "D^\\pm\\to \\pi^0          K^{\\star \\pm}"                  ;
  decayName[1233] = "D+ --> h*0 K-*"          ; decayNameLaTeX[1233] = "D^\\pm\\to h^{\\star 0}    K^{\\star \\pm}"                  ;
  decayName[1234] = "D+ --> h h"              ; decayNameLaTeX[1234] = "D^\\pm\\to h^\\pm          h^0"                              ;
  decayName[1235] = "D+ --> K K"              ; decayNameLaTeX[1235] = "D^\\pm\\to K^\\pm          K^0"                              ;
  // 3 body                                                                      
  // Semileptonic                                                                    
  // Cabibbo favoured                                                                    
  decayName[1301] = "D+ --> e+ nu_e K0"       ; decayNameLaTeX[1301] = "D^\\pm\\to    e^\\pm \\nu_e    K^0"                          ;
  decayName[1302] = "D+ --> mu+ nu_mu K0"     ; decayNameLaTeX[1302] = "D^\\pm\\to \\mu^\\pm \\nu_\\mu K^0"                          ;
  decayName[1303] = "D+ --> e+ nu_e K0*"      ; decayNameLaTeX[1303] = "D^\\pm\\to    e^\\pm \\nu_e    K^{\\star 0}"                 ;
  decayName[1304] = "D+ --> mu+ nu_mu K0*"    ; decayNameLaTeX[1304] = "D^\\pm\\to \\mu^\\pm \\nu_\\mu K^{\\star 0}"                 ;
  // Cabibbo suppresed                                                                   
  decayName[1305] = "D+ --> e+ nu_e omega"    ; decayNameLaTeX[1305] = "D^\\pm\\to    e^\\pm    \\nu_e \\omega"                      ;
  decayName[1306] = "D+ --> mu+ nu_mu omega"  ; decayNameLaTeX[1306] = "D^\\pm\\to \\mu^\\pm \\nu_\\mu \\omega"                      ;
  decayName[1307] = "D+ --> e+ nu_e eta"      ; decayNameLaTeX[1307] = "D^\\pm\\to    e^\\pm    \\nu_e \\eta"                        ;
  decayName[1308] = "D+ --> mu+ nu_mu eta"    ; decayNameLaTeX[1308] = "D^\\pm\\to \\mu^\\pm \\nu_\\mu \\eta"                        ;
  decayName[1309] = "D+ --> e+ nu_e eta'"     ; decayNameLaTeX[1309] = "D^\\pm\\to    e^\\pm    \\nu_e \\eta'"                       ;
  decayName[1310] = "D+ --> mu+ nu_mu eta'"   ; decayNameLaTeX[1310] = "D^\\pm\\to \\mu^\\pm \\nu_\\mu \\eta'"                       ;
  decayName[1311] = "D+ --> e+ nu_e phi"      ; decayNameLaTeX[1311] = "D^\\pm\\to    e^\\pm    \\nu_e \\phi"                        ;
  decayName[1312] = "D+ --> mu+ nu_mu phi"    ; decayNameLaTeX[1312] = "D^\\pm\\to \\mu^\\pm \\nu_\\mu \\phi"                        ;
  decayName[1313] = "D+ --> e+ nu_e h"        ; decayNameLaTeX[1313] = "D^\\pm\\to    e^\\pm    \\nu_e h"                            ;
  decayName[1314] = "D+ --> mu+ nu_mu h"      ; decayNameLaTeX[1314] = "D^\\pm\\to \\mu^\\pm \\nu_\\mu h"                            ;
  decayName[1315] = "D+ --> e+ nu_e rho"      ; decayNameLaTeX[1315] = "D^\\pm\\to    e^\\pm    \\nu_e \\rho^0"                      ;
  decayName[1316] = "D+ --> mu+ nu_mu rho"    ; decayNameLaTeX[1316] = "D^\\pm\\to \\mu^\\pm \\nu_\\mu \\rho^0"                      ;
  // Hadronic                                                                        
  decayName[1325] = "D+ --> pi+ pi0 phi"      ; decayNameLaTeX[1325] = "D^\\pm\\to \\pi^\\pm \\pi^0 \\phi"                           ;
  decayName[1326] = "D+ --> h h K+"           ; decayNameLaTeX[1326] = "D^\\pm\\to hhK^\\pm"                                         ;
  decayName[1327] = "D+ --> h h K0"           ; decayNameLaTeX[1327] = "D^\\pm\\to hhK^0"                                            ;
  decayName[1328] = "D+ --> K K K"            ; decayNameLaTeX[1328] = "D^\\pm\\to K\\pmK\\pmK\\mp"                                  ;
  decayName[1329] = "D+ --> h h h"            ; decayNameLaTeX[1329] = "D^\\pm\\to hhh"                                              ;
  decayName[1330] = "D+ --> h K0 K0"          ; decayNameLaTeX[1330] = "D^\\pm\\to hK^0K^0"                                          ;
  decayName[1331] = "D+ --> h K+ K+"          ; decayNameLaTeX[1331] = "D^\\pm\\to hK^\\pmK^\\pm"                                    ;
  decayName[1332] = "D+ --> h K0 K+"          ; decayNameLaTeX[1332] = "D^\\pm\\to hK^0K^\\pm"                                       ;
  // Radiative                                                                       
  decayName[1375] = "D+ --> K+ pi0 g"         ; decayNameLaTeX[1375] = "D^\\pm\\to K^\\pm          \\pi^0    \\gamma"                ;
  decayName[1376] = "D+ --> K0 pi+ g"         ; decayNameLaTeX[1376] = "D^\\pm\\to K^0             \\pi^\\pm \\gamma"                ;
  decayName[1377] = "D+ --> K*+ pi0 g"        ; decayNameLaTeX[1377] = "D^\\pm\\to K^{\\star \\pm} \\pi^0    \\gamma"                ;
  decayName[1378] = "D+ --> K*0 pi+ g"        ; decayNameLaTeX[1378] = "D^\\pm\\to K^{\\star 0}    \\pi^\\pm \\gamma"                ;
  // 4 body                                                                  
  // Semileptonic                                                                
  decayName[1401] = "D+ --> K0 pi0 e+ nu"     ; decayNameLaTeX[1401] = "D^\\pm\\to K^0 \\pi^0 e^\\pm \\nu_e"                         ;
  decayName[1402] = "D+ --> K0 pi0 mu+ nu"    ; decayNameLaTeX[1402] = "D^\\pm\\to K^0 \\pi^0 \\mu^\\pm \\nu_\\mu"                   ;
  decayName[1403] = "D+ --> K- pi+ e+ nu"     ; decayNameLaTeX[1403] = "D^\\pm\\to K^0 \\pi^0 e^\\pm \\nu_e"                         ;
  decayName[1404] = "D+ --> K- pi+ mu+ nu"    ; decayNameLaTeX[1404] = "D^\\pm\\to K^0 \\pi^0 \\mu^\\pm \\nu_\\mu"                   ;
  // Hadronic
  decayName[1425] = "D+ --> K0 pi+ pi0 pi0"   ; decayNameLaTeX[1425] = "D^\\pm\\to K^0 \\pi^\\pm \\pi^0 \\pi^0"                      ;
  decayName[1426] = "D+ --> K- pi+ pi+ pi0"   ; decayNameLaTeX[1426] = "D^\\pm\\to K^\\mp \\pi^\\pm \\pi^\\pm \\pi^0"                ;
  decayName[1427] = "D+ --> eta pi+ pi0 pi0"  ; decayNameLaTeX[1427] = "D^\\pm\\to \\eta \\pi^\\pm \\pi^0 \\pi^0"                    ;
  decayName[1428] = "D+ --> pi+ pi0 pi0 pi0"  ; decayNameLaTeX[1428] = "D^\\pm\\to \\pi^\\pm \\pi^0 \\pi^0 \\pi^0"                   ;
  decayName[1429] = "D+ --> pi+ pi+ pi- pi0"  ; decayNameLaTeX[1429] = "D^\\pm\\to \\pi^\\pm \\pi^\\pm \\pi^\\mp \\pi^0"             ;
  decayName[1430] = "D+ --> eta pi+ pi- pi0"  ; decayNameLaTeX[1430] = "D^\\pm\\to \\rta \\pi^\\pm \\pi^\\pm \\pi^\\mp"              ;
  // 5 body
  // Hadronic
  decayName[1525] = "D+ --> K- pi+ pi+ pi0 pi0" ; decayNameLaTeX[1525] = "D^\\pm\\to K^\\mp \\pi^\\pm \\pi^\\pm \\pi^0 \\pi^0"       ;
  decayName[1526] = "D+ --> K0 pi+ pi+ pi- pi0" ; decayNameLaTeX[1526] = "D^\\pm\\to K^0 \\pi^\\pm \\pi^\\pm \\pi^\\mp \\pi^0"       ;
  decayName[1527] = "D+ --> K- pi+ pi+ pi+ pi-" ; decayNameLaTeX[1527] = "D^\\pm\\to K^\\mp \\pi^\\pm \\pi^\\pm \\pi^\\pm \\pi^\\mp" ;
  decayName[1528] = "D+ --> K0 pi+ pi0 pi0 pi0" ; decayNameLaTeX[1528] = "D^\\pm\\to K^0 \\pi^\\pm \\pi^0 \\pi^0 \\pi^0"             ;
  // Other
  decayName[1999] = "D+ --> too many"         ; decayNameLaTeX[1999] = "D^\\pm\\to\\,other"                                          ;
  // D0
  // 2 body
  // Leptonic
  // Hadronic
  decayName[2225] = "D0 --> pi+ K-"           ; decayNameLaTeX[2225] = "D^0\\to\\pi^\\pm K^\\mp"                                     ;
  decayName[2226] = "D0 --> h*+ K-"           ; decayNameLaTeX[2226] = "D^0\\to h^\\pm   K^\\mp"                                     ;
  decayName[2227] = "D0 --> pi+ K-*"          ; decayNameLaTeX[2227] = "D^0\\to\\pi^\\pm K^{\\star -}"                               ;
  decayName[2228] = "D0 --> h*+ K-*"          ; decayNameLaTeX[2228] = "D^0\\to h^\\pm   K^{\\star -}"                               ;
  decayName[2229] = "D0 --> pi0 K0"           ; decayNameLaTeX[2229] = "D^0\\to\\pi^0    K^0"                                        ;
  decayName[2230] = "D0 --> h*0 K0"           ; decayNameLaTeX[2230] = "D^0\\to h^0      K^0"                                        ;
  decayName[2231] = "D0 --> pi0 K0*"          ; decayNameLaTeX[2231] = "D^0\\to\\pi^0    K^{\\star 0}"                               ;
  decayName[2232] = "D0 --> h*0 K0*"          ; decayNameLaTeX[2232] = "D^0\\to h^0      K^{\\star 0}"                               ;
  decayName[2233] = "D0 --> h h"              ; decayNameLaTeX[2233] = "D^0\\to hh"                                                  ;
  decayName[2234] = "D0 --> K K"              ; decayNameLaTeX[2234] = "D^0\\to KK"                                                  ;
  // 3 body
  // Semileptonic
  // Cabibbo favoured
  decayName[2301] = "D0 --> e+ nu_e K-"       ; decayNameLaTeX[2301] = "D^0\\to    e^\\pm \\nu_e    K^\\mp"                          ;
  decayName[2302] = "D0 --> mu+ nu_mu K-"     ; decayNameLaTeX[2302] = "D^0\\to \\mu^\\pm \\nu_\\mu K^\\mp"                          ;
  decayName[2303] = "D0 --> e+ nu_e K-*"      ; decayNameLaTeX[2303] = "D^0\\to    e^\\pm \\nu_e    K^{\\star \\mp}"                 ;
  decayName[2304] = "D0 --> mu+ nu_mu K-*"    ; decayNameLaTeX[2304] = "D^0\\to \\mu^\\pm \\nu_\\mu K^{\\star \\mp}"                 ;
  // Cabibbo suppresed
  decayName[2305] = "D0 --> e+ nu_e pi-"      ; decayNameLaTeX[2305] = "D^0\\to    e^\\pm \\nu_e    \\pi^\\mp"                       ;
  decayName[2306] = "D0 --> mu+ nu_mu pi-"    ; decayNameLaTeX[2306] = "D^0\\to \\mu^\\pm \\nu_\\mu \\pi^\\mp"                       ;
  decayName[2307] = "D0 --> e+ nu_e rho-"     ; decayNameLaTeX[2307] = "D^0\\to    e^\\pm \\nu_e    \\rho^\\mp"                      ;
  decayName[2308] = "D0 --> mu+ nu_mu rho-"   ; decayNameLaTeX[2308] = "D^0\\to \\mu^\\pm \\nu_\\mu \\rho^\\mp"                      ;
  // Hadronic                                                                    
  decayName[2325] = "D0 --> h h K-"           ; decayNameLaTeX[2325] = "D^0\\to h h^\\pm K^\\mp"                                     ;
  decayName[2326] = "D0 --> h h K0"           ; decayNameLaTeX[2326] = "D^0\\to h h      K^0"                                        ;
  decayName[2327] = "D0 --> K K K"            ; decayNameLaTeX[2327] = "D^0\\to K K K^\\pm"                                          ;
  decayName[2328] = "D0 --> h h h"            ; decayNameLaTeX[2328] = "D^0\\to h h h"                                               ;
  decayName[2329] = "D0 --> h K0 K0"          ; decayNameLaTeX[2329] = "D^0\\to h K^0 K^0"                                           ;
  decayName[2330] = "D0 --> h K- K+"          ; decayNameLaTeX[2330] = "D^0\\to h K^+ K^-"                                           ;
  decayName[2331] = "D0 --> h K0 K+-"         ; decayNameLaTeX[2331] = "D^0\\to h K^0 K^\\pm"                                        ;
  // 4 body
  decayName[2401] = "D0 --> e+  nu_e  K- pi0" ; decayNameLaTeX[2401] = "D^0\\to    e^\\pm \\nu_e    K^\\mp \\pi^0"                   ;
  decayName[2402] = "D0 --> mu+ nu_mu K- pi0" ; decayNameLaTeX[2402] = "D^0\\to    e^\\pm \\nu_e    K^\\mp \\pi^0"                   ;
  decayName[2403] = "D0 --> e+  nu_e  K0 pi"  ; decayNameLaTeX[2403] = "D^0\\to    e^\\pm \\nu_e    K^0 \\pi^\\mp"                   ;
  decayName[2404] = "D0 --> mu+ nu_mu K0 pi"  ; decayNameLaTeX[2404] = "D^0\\to    e^\\pm \\nu_e    K^0 \\pi^\\mp"                   ;
  decayName[2425] = "D0 --> K- pi+ pi+ pi-"   ; decayNameLaTeX[4245] = "D^0 \\to K^\\mp \\pi^\\pm \\pi^\\pm \\pi^\\mp"               ;
  decayName[2426] = "D0 --> K0 pi+ pi- p0-"   ; decayNameLaTeX[4246] = "D^0 \\to K^0 \\pi^\\pm \\pi^\\mp \\pi^0"                     ;
  decayName[2427] = "D0 --> pi+ pi+ pi- pi-"  ; decayNameLaTeX[2427] = "D^0\\to \\pi^\\pm \\pi^\\pm \\pi^\\mp \\pi^\\mp"             ;
  decayName[2428] = "D0 --> pi+ pi- pi0 pi0"  ; decayNameLaTeX[2428] = "D^0\\to \\pi^\\pm \\pi^\\mp \\pi^0 \\pi^0"                   ;
  decayName[2429] = "D0 --> K K h h"          ; decayNameLaTeX[2429] = "D^0\\to K K h h"                                             ;
  decayName[2430] = "D0 --> K K K h"          ; decayNameLaTeX[2430] = "D^0\\to K K K h"                                             ;
  decayName[2431] = "D0 --> K h h h"          ; decayNameLaTeX[2431] = "D^0\\to K h h h"                                             ;
  // 5 body
  decayName[2525] = "D0 --> h h h h h"        ; decayNameLaTeX[2525] = "D^0\\to h h h h h"                                           ;
  decayName[2526] = "D0 --> K h h h h"        ; decayNameLaTeX[2526] = "D^0\\to K h h h h"                                           ;
  decayName[2527] = "D0 --> K K h h h"        ; decayNameLaTeX[2527] = "D^0\\to K K h h h"                                           ;
  decayName[2528] = "D0 --> K K K h h"        ; decayNameLaTeX[2528] = "D^0\\to K K K h h"                                           ;
  decayName[2529] = "D0 --> K K K K h"        ; decayNameLaTeX[2529] = "D^0\\to K K K K h"                                           ;
  decayName[2530] = "D0 --> K K K K K"        ; decayNameLaTeX[2530] = "D^0\\to K K K K K"                                           ;
  // 6 body
  decayName[2625] = "D0 --> h h h h h h"      ; decayNameLaTeX[2625] = "D^0\\to h h h h h h"                                         ;
  decayName[2626] = "D0 --> K h h h h h"      ; decayNameLaTeX[2626] = "D^0\\to K h h h h h"                                         ;
  decayName[2627] = "D0 --> K K h h h h"      ; decayNameLaTeX[2627] = "D^0\\to K K h h h h"                                         ;
  decayName[2628] = "D0 --> K K K h h h"      ; decayNameLaTeX[2628] = "D^0\\to K K K h h h"                                         ;
  decayName[2629] = "D0 --> K K K K h h"      ; decayNameLaTeX[2629] = "D^0\\to K K K K h h"                                         ;
  decayName[2630] = "D0 --> K K K K K h"      ; decayNameLaTeX[2630] = "D^0\\to K K K K K h"                                         ;
  decayName[2631] = "D0 --> K K K K K K"      ; decayNameLaTeX[2631] = "D^0\\to K K K K K K"                                         ;
  // 5 body
  decayName[2725] = "D0 --> h h h h h h h"    ; decayNameLaTeX[2725] = "D^0\\to h h h h h h h"                                       ;
  decayName[2726] = "D0 --> K h h h h h h"    ; decayNameLaTeX[2726] = "D^0\\to K h h h h h h"                                       ;
  decayName[2727] = "D0 --> K K h h h h h"    ; decayNameLaTeX[2727] = "D^0\\to K K h h h h h"                                       ;
  decayName[2728] = "D0 --> K K K h h h h"    ; decayNameLaTeX[2728] = "D^0\\to K K K h h h h"                                       ;
  decayName[2729] = "D0 --> K K K K h h h"    ; decayNameLaTeX[2729] = "D^0\\to K K K K h h h"                                       ;
  decayName[2730] = "D0 --> K K K K K h h"    ; decayNameLaTeX[2730] = "D^0\\to K K K K K h h"                                       ;
  decayName[2731] = "D0 --> K K K K K K h"    ; decayNameLaTeX[2731] = "D^0\\to K K K K K K h"                                       ;
  decayName[2732] = "D0 --> K K K K K K K"    ; decayNameLaTeX[2732] = "D^0\\to K K K K K K K"                                       ;
  decayName[2999] = "D0 --> too many"         ; decayNameLaTeX[2999] = "D^0 \\to\\,other"                                            ;
  // Ds+                                                                     
  // 2 body                                                                  
  // Leptonic                                                                    
  decayName[3200] = "Ds+ --> e+ nu_e"         ; decayNameLaTeX[3200] = "D_s^+ \\to    e^+ \\nu_e"                                    ;
  decayName[3201] = "Ds- --> e- nu_e"         ; decayNameLaTeX[3201] = "D_s^- \\to    e^- \\nu_e"                                    ;
  decayName[3202] = "Ds+ --> mu+ nu_mu"       ; decayNameLaTeX[3202] = "D_s^+ \\to \\mu^+ \\nu_\\mu"                                 ;
  decayName[3203] = "Ds- --> mu- nu_mu"       ; decayNameLaTeX[3203] = "D_s^_ \\to \\mu^_ \\nu_\\mu"                                 ;
  // Hadronic                                                                        
  decayName[3225] = "Ds+ --> pi+ phi"         ; decayNameLaTeX[3225] = "D_s^\\pm \\to \\phi     \\pi^\\pm"                           ;
  decayName[3226] = "Ds+ --> pi+ K0"          ; decayNameLaTeX[3226] = "D_s^\\pm \\to \\pi^\\pm K^0"                                 ;
  decayName[3227] = "Ds+ --> h+ K0"           ; decayNameLaTeX[3227] = "D_s^\\pm \\to h^\\pm    K^0"                                 ;
  decayName[3228] = "Ds+ --> pi+ K0*"         ; decayNameLaTeX[3228] = "D_s^\\pm \\to \\pi^\\pm K^{\\star 0}"                        ;
  decayName[3229] = "Ds+ --> h+ K0*"          ; decayNameLaTeX[3229] = "D_s^\\pm \\to h^\\pm    K^{\\star 0}"                        ;
  decayName[3230] = "Ds+ --> pi0 K-"          ; decayNameLaTeX[3230] = "D_s^\\pm \\to \\pi^0    K^\\pm"                              ;
  decayName[3231] = "Ds+ --> h0 K-"           ; decayNameLaTeX[3231] = "D_s^\\pm \\to h^0       K^\\pm"                              ;
  decayName[3232] = "Ds+ --> pi0 K-*"         ; decayNameLaTeX[3232] = "D_s^\\pm \\to \\pi^0    K^{\\star \\pm}"                     ;
  decayName[3233] = "Ds+ --> h0 K-*"          ; decayNameLaTeX[3233] = "D_s^\\pm \\to h^0       K^{\\star \\pm}"                     ;
  decayName[3234] = "Ds+ --> h h"             ; decayNameLaTeX[3234] = "D_s^\\pm \\to h^0       h^\\pm"                              ;
  decayName[3235] = "Ds+ --> K K"             ; decayNameLaTeX[3235] = "D_s^\\pm \\to K         K^\\pm"                              ;
  decayName[3236] = "Ds+ --> eta pi+"         ; decayNameLaTeX[3236] = "D_s^\\pm \\to \\eta     \\pi^\\pm"                           ;
  decayName[3237] = "Ds+ --> K*0 K+"          ; decayNameLaTeX[3237] = "D_s^\\pm \\to K^\\pm    K^{\\star 0}"                        ;
  // 3 body                                                                  
  // Semileptonic                                                                
  // Cabibbo favoured                                                                
  decayName[3301] = "Ds+ --> e+ nu_e K0"      ; decayNameLaTeX[3301] = "D_s^\\pm \\to    e^\\pm \\nu_e    K^0"                       ;
  decayName[3302] = "Ds+ --> mu+ nu_mu K0"    ; decayNameLaTeX[3302] = "D_s^\\pm \\to \\mu^\\pm \\nu_\\mu K^0"                       ;
  decayName[3303] = "Ds+ --> e+ nu_e K0*"     ; decayNameLaTeX[3303] = "D_s^\\pm \\to    e^\\pm \\nu_e    K^{\\star 0}"              ;
  decayName[3304] = "Ds+ --> mu+ nu_mu K0*"   ; decayNameLaTeX[3304] = "D_s^\\pm \\to \\mu^\\pm \\nu_\\mu K^{\\star 0}"              ;
  // Cabibbo suppresed                                                               
  decayName[3305] = "Ds+ --> e+ nu_e omega"   ; decayNameLaTeX[3305] = "D_s^\\pm \\to    e^\\pm \\nu_e    \\omega"                   ;
  decayName[3306] = "Ds+ --> mu+ nu_mu omega" ; decayNameLaTeX[3306] = "D_s^\\pm \\to \\mu^\\pm \\nu_\\mu \\omega"                   ;
  decayName[3307] = "Ds+ --> e+ nu_e eta"     ; decayNameLaTeX[3307] = "D_s^\\pm \\to    e^\\pm \\nu_e    \\eta"                     ;
  decayName[3308] = "Ds+ --> mu+ nu_mu eta"   ; decayNameLaTeX[3308] = "D_s^\\pm \\to \\mu^\\pm \\nu_\\mu \\eta"                     ;
  decayName[3309] = "Ds+ --> e+ nu_e eta'"    ; decayNameLaTeX[3309] = "D_s^\\pm \\to    e^\\pm \\nu_e    \\eta'"                    ;
  decayName[3310] = "Ds+ --> mu+ nu_mu eta'"  ; decayNameLaTeX[3310] = "D_s^\\pm \\to \\mu^\\pm \\nu_\\mu \\eta'"                    ;
  decayName[3311] = "Ds+ --> e+ nu_e phi"     ; decayNameLaTeX[3311] = "D_s^\\pm \\to    e^\\pm \\nu_e    \\phi"                     ;
  decayName[3312] = "Ds+ --> mu+ nu_mu phi"   ; decayNameLaTeX[3312] = "D_s^\\pm \\to \\mu^\\pm \\nu_\\mu \\phi"                     ;
  decayName[3313] = "Ds+ --> e+ nu_e pi0"     ; decayNameLaTeX[3313] = "D_s^\\pm \\to    e^\\pm \\nu_e    \\pi^0"                    ;
  decayName[3314] = "Ds+ --> mu+ nu_mu po0"   ; decayNameLaTeX[3314] = "D_s^\\pm \\to \\mu^\\pm \\nu_\\mu \\pi^0"                    ;
  decayName[3315] = "Ds+ --> e+ nu_e rho0"    ; decayNameLaTeX[3315] = "D_s^\\pm \\to    e^\\pm \\nu_e    \\rho^0"                   ;
  decayName[3316] = "Ds+ --> mu+ nu_mu rho0"  ; decayNameLaTeX[3316] = "D_s^\\pm \\to \\mu^\\pm \\nu_\\mu \\rho^0"                   ;
  // Hadronic                                                                    
  decayName[3325] = "Ds+ --> pi+ pi0 phi"     ; decayNameLaTeX[3325] = "D_s^\\pm \\to \\pi^\\pm \\pi^0 \\phi"                        ;
  decayName[3326] = "Ds+ --> h h K+-"         ; decayNameLaTeX[3326] = "D_s^\\pm \\to h h K^\\pm"                                    ;
  decayName[3327] = "Ds+ --> h h K0"          ; decayNameLaTeX[3327] = "D_s^\\pm \\to h h^\\pm K^0"                                  ;
  decayName[3328] = "Ds+ --> K K K"           ; decayNameLaTeX[3328] = "D_s^\\pm \\to K^\\pm K^\\pm K^\\mp"                          ;
  decayName[3329] = "Ds+ --> h h h"           ; decayNameLaTeX[3329] = "D_s^\\pm \\to h^\\pm h^\\pm h^\\mp"                          ;
  decayName[3330] = "Ds+ --> h K0 K0"         ; decayNameLaTeX[3330] = "D_s^\\pm \\to h^\\pm K^0 K^0"                                ;
  decayName[3331] = "Ds+ --> h K+ K-"         ; decayNameLaTeX[3331] = "D_s^\\pm \\to h^\\pm K^\\pm K^\\mp"                          ;
  decayName[3332] = "Ds+ --> h K0 K+-"        ; decayNameLaTeX[3332] = "D_s^\\pm \\to h^0 K^0 K^\\pm"                                ;
  // Radiative                                                                   
  decayName[3375] = "Ds+ --> K+ pi0 g"        ; decayNameLaTeX[3375] = "D_s^\\pm \\to K^\\pm \\pi^0 \\gamma"                         ;
  decayName[3376] = "Ds+ --> K0 pi+ g"        ; decayNameLaTeX[3376] = "D_s^\\pm \\to K^0 \\pi^\\pm \\gamma"                         ;
  decayName[3377] = "Ds+ --> K*+ pi0 g"       ; decayNameLaTeX[3377] = "D_s^\\pm \\to K^{\\star \\pm} \\pi^0 \\gamma"                ;
  decayName[3378] = "Ds+ --> K*0 pi+ g"       ; decayNameLaTeX[3378] = "D_s^\\pm \\to K^{\\star 0} \\pi^\\pm \\gamma"                ;
  decayName[3379] = "Ds+ --> eta K g"         ; decayNameLaTeX[3379] = "D_s^\\pm \\to \\eta K^\\pm \\gamma"                          ;
  // 4 body                                                                  
  // Hadronic                                                                    
  decayName[3425] = "Ds+ --> pi pi K K"       ; decayNameLaTeX[3425] = "D_s^\\pm \\to \\pi^\\pm \\pi^\\mp K^\\pm K^0"                ;
  decayName[3426] = "Ds+ --> h h h K"         ; decayNameLaTeX[3426] = "D_s^\\pm \\to hhhK"                                          ;
  decayName[3427] = "Ds+ --> h h h h"         ; decayNameLaTeX[3427] = "D_s^\\pm \\to hhhh"                                          ;
  decayName[3999] = "Ds+ --> too many"        ; decayNameLaTeX[3999] = "D_s^\\pm \\to\\,other"                                       ;
  // tau modes                                                               
  decayName[4101] = "tau --> e nu nu"         ; decayNameLaTeX[4101] = "\\tau^\\pm    \\to    e^\\pm \\nu_\\tau \\nu_e"              ;
  decayName[4102] = "tau --> mu nu nu"        ; decayNameLaTeX[4102] = "\\tau^\\pm    \\to \\mu^\\pm \\nu_\\tau \\nu_\\mu"           ;
  decayName[4103] = "tau --> pi nu"           ; decayNameLaTeX[4103] = "\\tau^\\pm    \\to \\pi^\\pm \\nu_\\tau"                     ;
  decayName[4104] = "tau --> K nu"            ; decayNameLaTeX[4104] = "\\tau^\\pm    \\to    K^\\pm \\nu_\\tau"                     ;
  decayName[4105] = "tau --> pi h0 nu"        ; decayNameLaTeX[4105] = "\\tau^\\pm    \\to  \\pi^\\pm h^0 \\nu_\\tau"                ;
  decayName[4106] = "tau --> pi h h nu"       ; decayNameLaTeX[4106] = "\\tau^\\pm    \\to  \\pi^\\pm h h \\nu_\\tau"                ;
  // Other misc modes
  decayName[5101] = "B+ --> anything"         ; decayNameLaTeX[5101] = "B^\\pm        \\to \\, anything"                             ;
  decayName[5201] = "B0 --> anything"         ; decayNameLaTeX[5201] = "B^0           \\to \\, anything"                             ;
  decayName[5301] = "J/Psi --> anything"      ; decayNameLaTeX[5301] = "J/\\psi       \\to \\, anything"                             ;
  decayName[5401] = "rho+ --> pi+ pi0"        ; decayNameLaTeX[5401] = "\\rho^\\pm    \\to \\pi^\\pm \\pi^0"                         ;
  decayName[5402] = "rho0 --> pi+ pi-"        ; decayNameLaTeX[5402] = "\\rho^0       \\to \\pi^+ \\pi^-"                            ;
  decayName[5403] = "Other h0* --> pi+ pi-"   ; decayNameLaTeX[5403] = "h^0           \\to \\pi^+ \\pi^-"                            ;
  decayName[5404] = "phi --> K+ K-"           ; decayNameLaTeX[5404] = "\\phi         \\to K^+ K^-"                                  ;
  decayName[5405] = "h0 --> K K"              ; decayNameLaTeX[5405] = "h^0           \\to K^+ K^-"                                  ;
  decayName[5406] = "h+ --> K K"              ; decayNameLaTeX[5406] = "h^\\pm        \\to K^\\pm K^0"                               ;
  decayName[5407] = "Other h0* --> omega pi0" ; decayNameLaTeX[5407] = "h^0           \\to \\omega \\pi^0"                           ;
  decayName[5408] = "Other h+* --> eta pi"    ; decayNameLaTeX[5408] = "h^\\pm        \\to \\eta   \\pi^\\pm"                        ;
  decayName[5409] = "Other h0* --> rho0 pi0"  ; decayNameLaTeX[5409] = "h^0           \\to \\rho^0 \\pi^0"                           ;
  decayName[5410] = "Other h0* --> rho pi"    ; decayNameLaTeX[5410] = "h^0           \\to \\rho^\\pm \\pi^\\mp"                     ;
  decayName[5411] = "Other h+* --> h0* pi"    ; decayNameLaTeX[5411] = "h^\\pm        \\to h^0 \\pi"                                 ;
  decayName[5412] = "Other h+* --> eta' pi"   ; decayNameLaTeX[5412] = "h^\\pm        \\to \\eta' \\pi^\\pm"                         ;
  decayName[5413] = "Other h+* --> rho0 pi"   ; decayNameLaTeX[5413] = "h^\\pm        \\to \\rho^0' \\pi^\\pm"                       ;
  decayName[5414] = "Other h0* --> pi pi pi0" ; decayNameLaTeX[5414] = "h^{0\\star}   \\to \\pi^\\pm \\pi^\\mp \\pi^0"               ;
  decayName[5415] = "Other h+* --> pi pi pi"  ; decayNameLaTeX[5415] = "h^{\\pm\\star}\\to \\pi^\\pm \\pi^\\pm \\pi^\\mp"            ;
  decayName[5416] = "Other h+* --> rho pi pi" ; decayNameLaTeX[5416] = "h^{\\pm\\star}\\to \\rho^\\pm \\pi^\\pm \\pi^\\mp"           ;
  decayName[5417] = "Other h0* --> rho0 pi pi"; decayNameLaTeX[5417] = "h^{0\\star}\\to \\rho^0 \\pi^\\pm \\pi^\\mp"                 ;
  decayName[5418] = "eta'  --> eta pi pi"     ; decayNameLaTeX[5418] = "\\eta '    \\to \\eta \\pi^+ \\pi^-"                         ;
  decayName[5419] = "h+  --> pi pi pi"        ; decayNameLaTeX[5419] = "h^\\pm     \\to \\pi^\\pm \\pi^\\pm \\pi^\\mp"               ;
  decayName[5450] = "pi --> pi g"             ; decayNameLaTeX[5450] = "\\pi^\\pm  \\to \\pi^\\pm \\gamma"                           ;
  decayName[5451] = "pi --> pi p"             ; decayNameLaTeX[5451] = "\\pi^\\pm  \\to \\pi^\\pm p"                                 ;
  decayName[5452] = "pi --> g p"              ; decayNameLaTeX[5452] = "\\pi^\\pm  \\to \\gamma p"                                   ;
  decayName[5453] = "pi --> p p"              ; decayNameLaTeX[5453] = "\\pi^\\pm  \\to p p"                                         ;
  decayName[5454] = "pi --> pi pi"            ; decayNameLaTeX[5454] = "\\pi^\\pm  \\to \\pi^+ \\pi^-"                               ;
  decayName[5455] = "pi --> pi n"             ; decayNameLaTeX[5455] = "\\pi^\\pm  \\to \\pi^\\pm n"                                 ;
  decayName[5456] = "pi --> pi e"             ; decayNameLaTeX[5456] = "\\pi^\\pm  \\to \\pi e"                                      ;
  decayName[5457] = "pi --> He++ p"           ; decayNameLaTeX[5457] = "\\pi^\\pm  \\to He^{++} p"                                   ;
  decayName[5458] = "pi --> p e"              ; decayNameLaTeX[5458] = "\\pi^\\pm  \\to p e^-"                                       ;
  decayName[5459] = "pi --> p n"              ; decayNameLaTeX[5459] = "\\pi^\\pm  \\to p n"                                         ;
  decayName[5460] = "pi --> pi"               ; decayNameLaTeX[5460] = "\\pi^\\pm  \\to \\pi^\\pm"                                   ;
  decayName[5490] = "h0 --> e e"              ; decayNameLaTeX[5490] = "h^0        \\to e^+e^-"                                      ;
  decayName[5491] = "h0 --> mu mu"            ; decayNameLaTeX[5491] = "h^0        \\to \\mu^+\\mu^-"                                ;
  decayName[5501] = "vpho --> pi+ anything"   ; decayNameLaTeX[5501] = "\\gamma_{virtual} \\to \\pi^\\pm \\,anything"                ;
  decayName[5502] = "vpho --> K+ anything"    ; decayNameLaTeX[5502] = "\\gamma_{virtual} \\to K^\\m     \\,anything"                ;
  decayName[5503] = "vpho --> p anything"     ; decayNameLaTeX[5503] = "\\gamma_{virtual} \\to p         \\,anything"                ;
  decayName[5601] = "baryon --> anything"     ; decayNameLaTeX[5601] = "baryon \\to \\, anything"                                    ;
  decayName[5701] = "D* --> anything"         ; decayNameLaTeX[5701] = "D^{\\star} \\to\\,anything"                                  ;
  // Pi and K to leptons
  decayName[6101] = "pi --> mu anything"      ; decayNameLaTeX[6101] = "\\pi^\\pm \\to \\mu^\\pm \\, anything"                       ;
  decayName[6102] = "K --> mu anything"       ; decayNameLaTeX[6102] = "   K^\\pm \\to \\mu^\\pm \\, anything"                       ;
  decayName[6103] = "pi --> e anything"       ; decayNameLaTeX[6103] = "\\pi^\\pm \\to    e^\\pm \\, anything"                       ;
  decayName[6104] = "K --> e anything"        ; decayNameLaTeX[6104] = "   K^\\pm \\to    e^\\pm \\, anything"                       ;
  decayName[6105] = "pi --> p anything"       ; decayNameLaTeX[6105] = "\\pi^\\pm \\to    p^\\pm \\, anything"                       ;
  decayName[6106] = "pi --> n anything"       ; decayNameLaTeX[6106] = "\\pi^\\pm \\to    n, anything"                               ;
  decayName[6107] = "K --> p anything"        ; decayNameLaTeX[6107] = "K^\\pm \\to    p^\\pm \\, anything"                          ;
  decayName[6108] = "K --> n anything"        ; decayNameLaTeX[6108] = "K^\\pm \\to    n, anything"                                  ;
  // K
  decayName[6251] = "K* --> K pi0"            ; decayNameLaTeX[6251] = "K^{\\star \\pm} \\to K^\\pm \\pi^0"                          ;
  decayName[6252] = "K* --> K0 pi"            ; decayNameLaTeX[6252] = "K^{\\star \\pm} \\to K^0    \\pi^\\pm"                       ;
  decayName[6253] = "K*0 --> K pi"            ; decayNameLaTeX[6253] = "K^{\\star 0}    \\to K^\\pm \\pi^\\mp"                       ;
  decayName[6254] = "K*+ --> K g"             ; decayNameLaTeX[6254] = "K^{\\star \\pm} \\to K^\\pm \\gamma"                         ;
  decayName[6255] = "K*0 --> rho K"           ; decayNameLaTeX[6255] = "K^{\\star 0}    \\to \\rho K"                                ;
  decayName[6256] = "K --> pi pi0"            ; decayNameLaTeX[6256] = "K^\\pm \\to \\pi^\\pm \\pi^0"                                ;
  decayName[6257] = "KShort --> pi pi"        ; decayNameLaTeX[6257] = "K_S^0 \\to \\pi^+ \\pi^-"                                    ;
  decayName[6258] = "K** --> K* pi"           ; decayNameLaTeX[6258] = "K^{\\star\\star} \\to K^\\star \\pi"                         ;
  decayName[6259] = "K --> pi g"              ; decayNameLaTeX[6259] = "K \\to \\pi \\gamma"                                         ;
  decayName[6260] = "K --> pi pi"             ; decayNameLaTeX[6260] = "K \\to \\pi \\pi"                                            ;
  decayName[6261] = "KLong --> pi pi"         ; decayNameLaTeX[6261] = "K_L^0 \\to \\pi \\pi"                                        ;
  // Misc K interactions (in the wrong place)
  decayName[6300] = "K --> K p"               ; decayNameLaTeX[6300] = "K^\\pm \\to K^\\pm p"                                        ;
  decayName[6301] = "K --> K n"               ; decayNameLaTeX[6301] = "K^\\pm \\to K^\\pm n"                                        ;
  decayName[6302] = "K --> p e"               ; decayNameLaTeX[6302] = "K^\\pm \\to K^\\pm e^-"                                      ;
  decayName[6303] = "K --> p pi"              ; decayNameLaTeX[6303] = "K^\\pm \\to K^\\pm \\pi"                                     ;
  decayName[6304] = "K --> p n"               ; decayNameLaTeX[6304] = "K^\\pm \\to p n"                                             ;
  // K 3 body
  decayName[6351] = "KLong --> pi+ pi- pi0"   ; decayNameLaTeX[6351] = "K_L^0 \\to \\pi^+ \\pi^- \\pi^0"                             ;
  decayName[6352] = "K*0 --> K+ pi- pi0"      ; decayNameLaTeX[6352] = "K^{\\star 0} \\to K^+ \\pi^- \\pi^0"                         ;
  decayName[6353] = "K*0 --> K0 pi+ pi-"      ; decayNameLaTeX[6353] = "K^{\\star 0} \\to K^0 \\pi^+ \\pi^-"                         ;
  decayName[6354] = "K*+ --> K+ pi+ pi-"      ; decayNameLaTeX[6354] = "K^{\\star +} \\to K^+ \\pi^+ \\pi^-"                         ;
  decayName[6355] = "K*+ --> K+ pi0 pi0"      ; decayNameLaTeX[6355] = "K^{\\star +} \\to K^+ \\pi^0 \\pi^0"                         ;
  decayName[6356] = "K*+ --> K0 pi+ pi0"      ; decayNameLaTeX[6356] = "K^{\\star +} \\to K^0 \\pi^+ \\pi^0"                         ;
  // Other misc modes
  decayName[6998] = "h --> too many"          ; decayNameLaTeX[6998] = "h\\to\\,other"                                               ;
  decayName[6999] = "K --> too many"          ; decayNameLaTeX[6999] = "K\\to\\,other"                                               ;
  decayName[7101] = "e --> anything"          ; decayNameLaTeX[7101] = "e\\to\\,anything"                                            ;

  Int_t counter[NSignalMode][NEExtraCut] ;
  for(SignalMode=0 ; SignalMode<NSignalMode-1 ; SignalMode++)
  {
    for(EExtraCut=0 ; EExtraCut<NEExtraCut ; EExtraCut++)
    {
      counter[SignalMode][EExtraCut] = 0 ;
    }
  }
  Int_t nEntries = chain->GetEntries() ;
  for(Int_t i=0 ; i<nEntries ; i++)
  {
    if(i>0 && i%100000==0) cout << i << "/" << nEntries << endl ;
    chain->GetEntry(i) ;
    if(SPMode==10)   continue ;
    if(SignalMode<0) continue ;
    if(SignalMode>4) continue ;
    if(SPMode>4)     continue ;
    if(NExtraTracks!=0) continue ;
    Int_t s = SignalMode ;
    if((s==0||s==2) && (MuBDTVeryLoose+MuBDTLoPLoose)==0) continue ;
    if((s==1||s==3) && EKMSuperLoose==0) continue ;
    if(s==4 && PiKMSuperTight==0)        continue ;
    
    Int_t count = 0 ;
    if(s==0 && EExtraNeutral<1.0) count = 1 ;
    if(s>1  && EExtraNeutral<1.0) count = 1 ;
    counter[s][0]++ ;
    counter[s][1]+=count ;
    //if(counter[s][0]>10000) break ;
    //cout << i << " " << EllMothLund << " --> " << EllSib1Lund << " " << EllSib2Lund << " " << EllSib3Lund << " " << EllSib4Lund << " " << EllSib5Lund << endl ;
    //if(counter>0 && counter%10000==0) cout << counter << " passed" << endl ;
    for(Charged=0 ; Charged<NCharged ; Charged++){if(abs(EllLund)==chargedLund[Charged]) { nTrueLund[s][Charged][0]++ ; nTrueLund[s][Charged][1]+=count ; }}
    if(abs(EllMothLund)==24) EllMothLund = -15*abs(EllMothLund)/EllMothLund ;
    
    Int_t     nhPlus = 0 ;
    Int_t nhPlusStar = 0 ;
    Int_t        nh0 = 0 ;
    Int_t    nh0Star = 0 ;
    Int_t     nKPlus = 0 ;
    Int_t nKPlusStar = 0 ;
    Int_t        nK0 = 0 ;
    Int_t    nK0Star = 0 ;
    Int_t         ng = 0 ;
    const Int_t nSibling = 14 ;
    Int_t rawSibling[nSibling] = { EllSib1Lund , EllSib2Lund , EllSib3Lund , EllSib4Lund , EllSib5Lund , EllSib6Lund , EllSib7Lund  , EllSib8Lund  , EllSib9Lund  , EllSib10Lund  , EllSib11Lund  , EllSib12Lund  , EllSib13Lund  , EllSib14Lund } ;
    Int_t    sibling[nSibling] = { EllSib1Lund , EllSib2Lund , EllSib3Lund , EllSib4Lund , EllSib5Lund , EllSib6Lund , EllSib7Lund  , EllSib8Lund  , EllSib9Lund  , EllSib10Lund  , EllSib11Lund  , EllSib12Lund  , EllSib13Lund  , EllSib14Lund } ;
    
    Int_t rawEllMothDauLen = 0 ;
    for(Int_t j=0 ; j<nSibling ; j++)
    {
      nhPlus     +=     ishPlus(sibling[j]) ;
      nhPlusStar += ishPlusStar(sibling[j]) ;
      nh0        +=        ish0(sibling[j]) ;
      nh0Star    +=    ish0Star(sibling[j]) ;
      nKPlus     +=     isKPlus(sibling[j]) ;
      nKPlusStar += isKPlusStar(sibling[j]) ;
      nK0        +=        isK0(sibling[j]) ;
      nK0Star    +=    isK0Star(sibling[j]) ;
      if(sibling[j]!=0) rawEllMothDauLen++ ;
      if(sibling[j]==22)
      {
    ng++ ;
    sibling[j] = 0 ;
    EllMothDauLen-- ;
      }
    }
    
    //for(Int_t j=0 ; j<nSibling ; j++) cout << sibling[j] << " " ;
    //cout << endl ;
    
    for(Int_t j=nSibling-2 ; j>-1 ; j--)
    {
      if(sibling[j]==0)
      {
    for(Int_t k=j ; k<nSibling-1 ; k++ )
    {
      sibling[k] = sibling[k+1] ; 
    }
    sibling[nSibling-1] = 0 ;
      }
    }
    EllMothDauLen = 0 ;
    for(Int_t j=0 ; j<nSibling ; j++) if(sibling[j]!=0) EllMothDauLen++ ;
    
    if(rawEllMothDauLen>maxDauLen)
    {
      maxDauLen = rawEllMothDauLen ;
      //cout << "rawEllMothDauLen = " << rawEllMothDauLen << endl ;
      //PrintDaughters(count, s, EllMothDauLen, rawEllMothDauLen, EllMothLund, sibling, rawSibling, nhPlus, nhPlusStar, nh0, nh0Star, nKPlus, nKPlusStar, nK0, nK0Star, ng) ;
    }

    // Start truthmatching
    if(EllMothLund==22)
    {
      for(Charged=0 ; Charged<NCharged ; Charged++){ if(abs(EllLund)==chargedLund[Charged] ){ nMoth[0][s][Charged][0]++ ; nMoth[0][s][Charged][1]+=count ; }}
      if(EllMothDauLen==1)
      {
    if(sibling[0]==11){ n[s][0][0]++ ; n[s][0][1]+=count ; }
    else if(sibling[0]==  -11){  n[s][1][0]++ ;  n[s][1][1]+=count ; }
    else if(sibling[0]==   13){  n[s][2][0]++ ;  n[s][2][1]+=count ; }
    else if(sibling[0]==  -13){  n[s][3][0]++ ;  n[s][3][1]+=count ; }
    else if(sibling[0]==  211){  n[s][4][0]++ ;  n[s][4][1]+=count ; }
    else if(sibling[0]== -211){  n[s][5][0]++ ;  n[s][5][1]+=count ; }
    else if(sibling[0]==  321){  n[s][6][0]++ ;  n[s][6][1]+=count ; }
    else if(sibling[0]== -321){  n[s][7][0]++ ;  n[s][7][1]+=count ; }
    else if(sibling[0]== 2212){  n[s][8][0]++ ;  n[s][8][1]+=count ; }
    else if(sibling[0]==-2212){  n[s][9][0]++ ;  n[s][9][1]+=count ; }
    else if(sibling[0]== 2112){ n[s][10][0]++ ; n[s][10][1]+=count ; }
    else if(sibling[0]==-2112){ n[s][11][0]++ ; n[s][11][1]+=count ; }
    else if(sibling[0]== 1022){ n[s][12][0]++ ; n[s][12][1]+=count ; }
    else if(sibling[0]==-1022){ n[s][13][0]++ ; n[s][13][1]+=count ; }
    else
    {
      n_EllIsUnknown[s][0]++ ; n_EllIsUnknown[s][1]+=count ;
      PrintDaughters(count, s, EllMothDauLen, rawEllMothDauLen, EllMothLund, sibling, rawSibling, nhPlus, nhPlusStar, nh0, nh0Star, nKPlus, nKPlusStar, nK0, nK0Star, ng) ;
    }
      }
      else if(EllMothDauLen==2)
      {
    // Photons and leptons
    if     ( match( 11,  22, sibling[0], sibling[1]) ){ n[s][14][0]++ ; n[s][14][1]+=count ; }
    else if( match(-11,  22, sibling[0], sibling[1]) ){ n[s][15][0]++ ; n[s][15][1]+=count ; }
    else if( match( 13,  22, sibling[0], sibling[1]) ){ n[s][16][0]++ ; n[s][16][1]+=count ; }
    else if( match(-13,  22, sibling[0], sibling[1]) ){ n[s][17][0]++ ; n[s][17][1]+=count ; }
    else if( match( 11,  11, sibling[0], sibling[1]) ){ n[s][18][0]++ ; n[s][18][1]+=count ; }
    else if( match(-11, -11, sibling[0], sibling[1]) ){ n[s][19][0]++ ; n[s][19][1]+=count ; }
    else if( match( 11, -11, sibling[0], sibling[1]) ){ n[s][20][0]++ ; n[s][20][1]+=count ; }
    else if( match( 13,  13, sibling[0], sibling[1]) ){ n[s][21][0]++ ; n[s][21][1]+=count ; }
    else if( match(-13, -13, sibling[0], sibling[1]) ){ n[s][22][0]++ ; n[s][22][1]+=count ; }
    else if( match( 13, -13, sibling[0], sibling[1]) ){ n[s][23][0]++ ; n[s][23][1]+=count ; }
    
    // Photons and hadrons
    else if( match( 22,   211, sibling[0], sibling[1]) ){ n[s][24][0]++ ; n[s][24][1]+=count ; }
    else if( match( 22,  -211, sibling[0], sibling[1]) ){ n[s][25][0]++ ; n[s][25][1]+=count ; }
    else if( match( 22,   321, sibling[0], sibling[1]) ){ n[s][26][0]++ ; n[s][26][1]+=count ; }
    else if( match( 22,  -321, sibling[0], sibling[1]) ){ n[s][27][0]++ ; n[s][27][1]+=count ; }
    else if( match( 22,  2112, sibling[0], sibling[1]) ){ n[s][28][0]++ ; n[s][28][1]+=count ; }
    else if( match( 22, -2112, sibling[0], sibling[1]) ){ n[s][29][0]++ ; n[s][29][1]+=count ; }
    else if( match( 22,  2212, sibling[0], sibling[1]) ){ n[s][30][0]++ ; n[s][30][1]+=count ; }
    else if( match( 22, -2212, sibling[0], sibling[1]) ){ n[s][31][0]++ ; n[s][31][1]+=count ; }
    else if( match( 22,  1022, sibling[0], sibling[1]) ){ n[s][32][0]++ ; n[s][32][1]+=count ; }
    else if( match( 22, -1022, sibling[0], sibling[1]) ){ n[s][33][0]++ ; n[s][33][1]+=count ; }
    
    // Hadrons
    else if( match(  211,  -211, sibling[0], sibling[1]) ){ n[s][34][0]++ ; n[s][34][1]+=count ; }
    else if( match(  321,  -321, sibling[0], sibling[1]) ){ n[s][35][0]++ ; n[s][35][1]+=count ; }
    else if( match( 2212, -2212, sibling[0], sibling[1]) ){ n[s][36][0]++ ; n[s][36][1]+=count ; }
    else if( match( 2112, -2112, sibling[0], sibling[1]) ){ n[s][37][0]++ ; n[s][37][1]+=count ; }
    else if( match( 1022,  1022, sibling[0], sibling[1]) ){ n[s][38][0]++ ; n[s][38][1]+=count ; }
    else if( match( 1022, -1022, sibling[0], sibling[1]) ){ n[s][39][0]++ ; n[s][39][1]+=count ; }
    else if( match(-1022, -1022, sibling[0], sibling[1]) ){ n[s][40][0]++ ; n[s][40][1]+=count ; }

    // Hadrons and leptons
    else if( match(11, 2212, sibling[0], sibling[1]) ){ n[s][48][0]++ ; n[s][48][1]+=count ; }
    else
    {
      n_EllIsUnknown[s][0]++ ; n_EllIsUnknown[s][1]+=count ;
      PrintDaughters(count, s, EllMothDauLen, rawEllMothDauLen, EllMothLund, sibling, rawSibling, nhPlus, nhPlusStar, nh0, nh0Star, nKPlus, nKPlusStar, nK0, nK0Star, ng) ;
    }
      }
      else if(EllMothDauLen==3)
      { 
    if(      match( 11, 11,-11, sibling[0], sibling[1], sibling[2]) ){ n[s][46][0]++ ; n[s][46][1]+=count ; }
    else if( match( 11,-11,-11, sibling[0], sibling[1], sibling[2]) ){ n[s][47][0]++ ; n[s][47][1]+=count ; }
    else
    {
      n_EllIsUnknown[s][0]++ ; n_EllIsUnknown[s][1]+=count ;
      PrintDaughters(count, s, EllMothDauLen, rawEllMothDauLen, EllMothLund, sibling, rawSibling, nhPlus, nhPlusStar, nh0, nh0Star, nKPlus, nKPlusStar, nK0, nK0Star, ng) ;
    }
      }
      else
      {
    n_EllIsUnknown[s][0]++ ; n_EllIsUnknown[s][1]+=count ;
    PrintDaughters(count, s, EllMothDauLen, rawEllMothDauLen, EllMothLund, sibling, rawSibling, nhPlus, nhPlusStar, nh0, nh0Star, nKPlus, nKPlusStar, nK0, nK0Star, ng) ;
    nTooManyDaughters[s]++ ;
    n[s][99][0]++ ; n[s][99][1]+=count ;
      }
    }
    else if(abs(EllMothLund)==411)
    {
      for(Charged=0 ; Charged<NCharged ; Charged++){ if(abs(EllLund)==chargedLund[Charged] ){ nMoth[1][s][Charged][0]++ ; nMoth[1][s][Charged][1]+=count ; }}
      if(EllMothDauLen==1)
      {
    cout << "D+ with one daughter.  Something has gone wrong!" << endl ;
    PrintDaughters(count, s, EllMothDauLen, rawEllMothDauLen, EllMothLund, sibling, rawSibling, nhPlus, nhPlusStar, nh0, nh0Star, nKPlus, nKPlusStar, nK0, nK0Star, ng) ;
      }
      else if(EllMothDauLen==2)
      {
    // Leptonic decays
    if     ( match( -11,     12, sibling[0], sibling[1]) ){ n[s][1200][0]++ ; n[s][1200][1]+=count ; }
    else if( match(  11,    -12, sibling[0], sibling[1]) ){ n[s][1201][0]++ ; n[s][1201][1]+=count ; }
    else if( match( -13,     14, sibling[0], sibling[1]) ){ n[s][1202][0]++ ; n[s][1202][1]+=count ; }
    else if( match(  13,    -14, sibling[0], sibling[1]) ){ n[s][1203][0]++ ; n[s][1203][1]+=count ; }
    // Hadronic decays
    else if( match( 333, 211, abs(sibling[0]), abs(sibling[1])) ){ n[s][1225][0]++ ; n[s][1225][1]+=count ; }
    else if(nhPlus==1     && nK0==1         )    { n[s][1226][0]++ ; n[s][1226][1]+=count ; }
    else if(nhPlusStar==1 && nK0==1         )    { n[s][1227][0]++ ; n[s][1227][1]+=count ; }
    else if(nhPlus==1     && nK0Star==1     )    { n[s][1228][0]++ ; n[s][1228][1]+=count ; }
    else if(nhPlusStar==1 && nK0Star==1     )    { n[s][1229][0]++ ; n[s][1229][1]+=count ; }
    else if(nh0==1        && nKPlus==1      )    { n[s][1230][0]++ ; n[s][1230][1]+=count ; }
    else if(nh0Star==1    && nKPlus==1      )    { n[s][1231][0]++ ; n[s][1231][1]+=count ; }
    else if(nh0==1        && nKPlusStar==1  )    { n[s][1232][0]++ ; n[s][1232][1]+=count ; }
    else if(nh0Star==1    && nKPlusStar==1  )    { n[s][1233][0]++ ; n[s][1233][1]+=count ; }
    else if(nhPlus+nhPlusStar+nh0+nh0Star==2)    { n[s][1234][0]++ ; n[s][1234][1]+=count ; }
    else if(nKPlus+nKPlusStar+nK0+nK0Star==2)    { n[s][1235][0]++ ; n[s][1235][1]+=count ; }
    else
    {
      n_EllIsUnknown[s][0]++ ; n_EllIsUnknown[s][1]+=count ;
      PrintDaughters(count, s, EllMothDauLen, rawEllMothDauLen, EllMothLund, sibling, rawSibling, nhPlus, nhPlusStar, nh0, nh0Star, nKPlus, nKPlusStar, nK0, nK0Star, ng) ;
    }
      }
      else if(EllMothDauLen==3)
      {
    // Semileptonic decays
    // K l nu
    if(      match( 11,  12,   311, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][1301][0]++ ; n[s][1301][1]+=count ; }
    else if( match( 13,  14,   311, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][1302][0]++ ; n[s][1302][1]+=count ; }
    else if( match( 11,  12,   313, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][1303][0]++ ; n[s][1303][1]+=count ; }
    else if( match( 11,  12,   315, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][1303][0]++ ; n[s][1303][1]+=count ; }
    else if( match( 11,  12,   317, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][1303][0]++ ; n[s][1303][1]+=count ; }
    else if( match( 11,  12, 10313, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][1303][0]++ ; n[s][1303][1]+=count ; }
    else if( match( 11,  12, 20313, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][1303][0]++ ; n[s][1303][1]+=count ; }
    else if( match( 13,  14,   313, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][1304][0]++ ; n[s][1304][1]+=count ; }
    else if( match( 13,  14,   315, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][1304][0]++ ; n[s][1304][1]+=count ; }
    else if( match( 13,  14,   317, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][1304][0]++ ; n[s][1304][1]+=count ; }
    else if( match( 13,  14, 10313, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][1304][0]++ ; n[s][1304][1]+=count ; }
    else if( match( 13,  14, 20313, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][1304][0]++ ; n[s][1304][1]+=count ; }
    
    // Cabibbo suppresed
    else if( match( 11,  12,  223, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][1305][0]++ ; n[s][1305][1]+=count ; }
    else if( match( 13,  14,  223, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][1306][0]++ ; n[s][1306][1]+=count ; }
    else if( match( 11,  12,  221, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][1307][0]++ ; n[s][1307][1]+=count ; }
    else if( match( 13,  14,  221, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][1308][0]++ ; n[s][1308][1]+=count ; }
    else if( match( 11,  12,  331, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][1309][0]++ ; n[s][1309][1]+=count ; }
    else if( match( 13,  14,  331, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][1310][0]++ ; n[s][1310][1]+=count ; }
    else if( match( 11,  12,  333, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][1311][0]++ ; n[s][1311][1]+=count ; }
    else if( match( 13,  14,  333, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][1312][0]++ ; n[s][1312][1]+=count ; }
    else if( match( 11,  12,  111, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][1313][0]++ ; n[s][1313][1]+=count ; }
    else if( match( 13,  14,  111, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][1314][0]++ ; n[s][1314][1]+=count ; }
    else if( match( 11,  12,  113, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][1315][0]++ ; n[s][1315][1]+=count ; }
    else if( match( 13,  14,  113, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][1316][0]++ ; n[s][1316][1]+=count ; } 
    else if( match(111, 211,  333, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][1325][0]++ ; n[s][1325][1]+=count ; }
    // Hadronic decays
    else if( (nhPlus+nhPlusStar+nh0+nh0Star)==2 && (nKPlus+nKPlusStar)==1 ){ n[s][1326][0]++ ; n[s][1326][1]+=count ; }
    else if( (nhPlus+nhPlusStar+nh0+nh0Star)==2 && (nK0+nK0Star)==1       ){ n[s][1327][0]++ ; n[s][1327][1]+=count ; }
    else if( (nKPlus+nKPlusStar+nK0+nK0Star)==3                           ){ n[s][1328][0]++ ; n[s][1328][1]+=count ; }
    else if( (nhPlus+nhPlusStar+nh0+nh0Star)==3                           ){ n[s][1329][0]++ ; n[s][1329][1]+=count ; }
    else if( (nhPlus+nhPlusStar+nh0+nh0Star)==1 && (nK0+nK0Star)==2       ){ n[s][1330][0]++ ; n[s][1330][1]+=count ; }
    else if( (nhPlus+nhPlusStar+nh0+nh0Star)==1 && (nKPlus+nKPlusStar)==2 ){ n[s][1331][0]++ ; n[s][1331][1]+=count ; }
    else if( (nhPlus+nhPlusStar+nh0+nh0Star)==1 && (nKPlus+nKPlusStar)==1 && (nK0+nK0Star)==1 ){ n[s][1332][0]++ ; n[s][1332][1]+=count ; }
    else
    {
      n_EllIsUnknown[s][0]++ ; n_EllIsUnknown[s][1]+=count ;
      PrintDaughters(count, s, EllMothDauLen, rawEllMothDauLen, EllMothLund, sibling, rawSibling, nhPlus, nhPlusStar, nh0, nh0Star, nKPlus, nKPlusStar, nK0, nK0Star, ng) ;
    }
      }
      else if(EllMothDauLen==4)
      {
    // Semileptonic decays
    // K l nu
    if(      match(  11,  12,   311, 111, abs(sibling[0]), abs(sibling[1]), abs(sibling[2]), abs(sibling[3])) ){ n[s][1401][0]++ ; n[s][1401][1]+=count ; }
    else if( match(  13,  14,   311, 111, abs(sibling[0]), abs(sibling[1]), abs(sibling[2]), abs(sibling[3])) ){ n[s][1402][0]++ ; n[s][1402][1]+=count ; }
    else if( match(  11,  12,   321, 211, abs(sibling[0]), abs(sibling[1]), abs(sibling[2]), abs(sibling[3])) ){ n[s][1403][0]++ ; n[s][1403][1]+=count ; }
    else if( match(  13,  14,   321, 211, abs(sibling[0]), abs(sibling[1]), abs(sibling[2]), abs(sibling[3])) ){ n[s][1404][0]++ ; n[s][1404][1]+=count ; }
    else if( match(  13,  12,   321, 211, abs(sibling[0]), abs(sibling[1]), abs(sibling[2]), abs(sibling[3])) ){ n[s][1404][0]++ ; n[s][1404][1]+=count ; } // Typo in DECAY.DEC

    // Hadronic decays
    else if( match( 311, 211,  111, 111, abs(sibling[0]), abs(sibling[1]), abs(sibling[2]), abs(sibling[3])) ){ n[s][1425][0]++ ; n[s][1425][1]+=count ; }
    else if( match( 321, 211,  211, 111, abs(sibling[0]), abs(sibling[1]), abs(sibling[2]), abs(sibling[3])) ){ n[s][1426][0]++ ; n[s][1426][1]+=count ; }
    else if( match( 221, 211,  111, 111, abs(sibling[0]), abs(sibling[1]), abs(sibling[2]), abs(sibling[3])) ){ n[s][1427][0]++ ; n[s][1427][1]+=count ; }
    else if( match( 211, 111,  111, 111, abs(sibling[0]), abs(sibling[1]), abs(sibling[2]), abs(sibling[3])) ){ n[s][1428][0]++ ; n[s][1428][1]+=count ; }
    else if( match( 211, 211,  211, 111, abs(sibling[0]), abs(sibling[1]), abs(sibling[2]), abs(sibling[3])) ){ n[s][1429][0]++ ; n[s][1429][1]+=count ; }
    else if( match( 221, 211,  211, 211, abs(sibling[0]), abs(sibling[1]), abs(sibling[2]), abs(sibling[3])) ){ n[s][1430][0]++ ; n[s][1430][1]+=count ; }
    else
    {
      n_EllIsUnknown[s][0]++ ; n_EllIsUnknown[s][1]+=count ;
      PrintDaughters(count, s, EllMothDauLen, rawEllMothDauLen, EllMothLund, sibling, rawSibling, nhPlus, nhPlusStar, nh0, nh0Star, nKPlus, nKPlusStar, nK0, nK0Star, ng) ;
    }
      }
      else if(EllMothDauLen==5)
      {
    // Hadronic
    if(      match(321, 211,  211, 111, 111, abs(sibling[0]), abs(sibling[1]), abs(sibling[2]), abs(sibling[3]), abs(sibling[4])) ){ n[s][1525][0]++ ; n[s][1525][1]+=count ; }
    else if( match(311, 211,  211, 211, 111, abs(sibling[0]), abs(sibling[1]), abs(sibling[2]), abs(sibling[3]), abs(sibling[4])) ){ n[s][1526][0]++ ; n[s][1526][1]+=count ; }
    else if( match(321, 211,  211, 211, 211, abs(sibling[0]), abs(sibling[1]), abs(sibling[2]), abs(sibling[3]), abs(sibling[4])) ){ n[s][1527][0]++ ; n[s][1527][1]+=count ; }
    else if( match(311, 211,  111, 111, 111, abs(sibling[0]), abs(sibling[1]), abs(sibling[2]), abs(sibling[3]), abs(sibling[4])) ){ n[s][1528][0]++ ; n[s][1528][1]+=count ; }
    else
    {
      n_EllIsUnknown[s][0]++ ; n_EllIsUnknown[s][1]+=count ;
      PrintDaughters(count, s, EllMothDauLen, rawEllMothDauLen, EllMothLund, sibling, rawSibling, nhPlus, nhPlusStar, nh0, nh0Star, nKPlus, nKPlusStar, nK0, nK0Star, ng) ;
    }
      }
      else
      {
    nTooManyDaughters[s]++ ;
    n[s][1999][0]++ ; n[s][1999][1]+=count ;
    n_EllIsUnknown[s][0]++ ; n_EllIsUnknown[s][1]+=count ;
    PrintDaughters(count, s, EllMothDauLen, rawEllMothDauLen, EllMothLund, sibling, rawSibling, nhPlus, nhPlusStar, nh0, nh0Star, nKPlus, nKPlusStar, nK0, nK0Star, ng) ;
      }
    }
    else if(abs(EllMothLund)==421)
    {
      for(Charged=0 ; Charged<NCharged ; Charged++){ if(abs(EllLund)==chargedLund[Charged] ){ nMoth[2][s][Charged][0]++ ; nMoth[2][s][Charged][1]+=count ; }}
      if(EllMothDauLen==1)
      {
    cout << "D0 with one daughter.  Something has gone wrong!" << endl ;
    PrintDaughters(count, s, EllMothDauLen, rawEllMothDauLen, EllMothLund, sibling, rawSibling, nhPlus, nhPlusStar, nh0, nh0Star, nKPlus, nKPlusStar, nK0, nK0Star, ng) ;
      }
      else if(EllMothDauLen==2)
      {
    if(     (nhPlus)==1     && (nKPlus)==1    ){ n[s][2225][0]++ ; n[s][2225][1]+=count ; }
    else if((nhPlusStar)==1 && (nKPlus)==1    ){ n[s][2226][0]++ ; n[s][2226][1]+=count ; }
    else if((nhPlus)==1     && (nKPlusStar)==1){ n[s][2227][0]++ ; n[s][2227][1]+=count ; }
    else if((nhPlusStar)==1 && (nKPlusStar)==1){ n[s][2228][0]++ ; n[s][2228][1]+=count ; }
    else if((nh0)==1        && (nK0)==1       ){ n[s][2229][0]++ ; n[s][2229][1]+=count ; }
    else if((nh0Star)==1    && (nK0)==1       ){ n[s][2230][0]++ ; n[s][2230][1]+=count ; }
    else if((nh0)==1        && (nK0Star)==1   ){ n[s][2231][0]++ ; n[s][2231][1]+=count ; }
    else if((nh0Star)==1    && (nK0Star)==1   ){ n[s][2232][0]++ ; n[s][2232][1]+=count ; }
    else if((nhPlus+nhPlusStar+nh0+nh0Star)==2){ n[s][2233][0]++ ; n[s][2233][1]+=count ; }
    else if((nKPlus+nKPlusStar+nK0+nK0Star)==2){ n[s][2234][0]++ ; n[s][2234][1]+=count ; } 
    else
    {
      n_EllIsUnknown[s][0]++ ; n_EllIsUnknown[s][1]+=count ;
      PrintDaughters(count, s, EllMothDauLen, rawEllMothDauLen, EllMothLund, sibling, rawSibling, nhPlus, nhPlusStar, nh0, nh0Star, nKPlus, nKPlusStar, nK0, nK0Star, ng) ;
    }
      }
      else if(EllMothDauLen==3)
      {
    // Semileptonic decays
    // K l nu
    if(      match( 11,  12,   321, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][2301][0]++ ; n[s][2301][1]+=count ; }
    else if( match( 13,  14,   321, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][2302][0]++ ; n[s][2302][1]+=count ; }
    else if( match( 11,  12,   323, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][2303][0]++ ; n[s][2303][1]+=count ; }
    else if( match( 11,  12,   325, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][2303][0]++ ; n[s][2303][1]+=count ; }
    else if( match( 11,  12,   327, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][2303][0]++ ; n[s][2303][1]+=count ; }
    else if( match( 11,  12, 10323, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][2303][0]++ ; n[s][2303][1]+=count ; }
    else if( match( 11,  12, 20323, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][2303][0]++ ; n[s][2303][1]+=count ; }
    else if( match( 13,  14,   323, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][2304][0]++ ; n[s][2304][1]+=count ; }
    else if( match( 13,  14,   325, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][2304][0]++ ; n[s][2304][1]+=count ; }
    else if( match( 13,  14,   327, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][2304][0]++ ; n[s][2304][1]+=count ; }
    else if( match( 13,  14, 10323, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][2304][0]++ ; n[s][2304][1]+=count ; }
    else if( match( 13,  14, 20323, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][2304][0]++ ; n[s][2304][1]+=count ; }
    
    // Cabibbo suppresed
    else if( match( 11,  12,   211, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][2305][0]++ ; n[s][2305][1]+=count ; }
    else if( match( 13,  14,   211, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][2306][0]++ ; n[s][2306][1]+=count ; }
    else if( match( 11,  12,   213, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][2307][0]++ ; n[s][2307][1]+=count ; }
    else if( match( 13,  14,   213, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][2308][0]++ ; n[s][2308][1]+=count ; }
    
    // Hadronic decays
    else if( (nhPlus+nhPlusStar+nh0+nh0Star)==2 && (nKPlus+nKPlusStar)==1 ){ n[s][2325][0]++ ; n[s][2325][1]+=count ; }
    else if( (nhPlus+nhPlusStar+nh0+nh0Star)==2 && (nK0+nK0Star)==1       ){ n[s][2326][0]++ ; n[s][2326][1]+=count ; }
    else if( (nKPlus+nKPlusStar+nK0+nK0Star)==3                           ){ n[s][2327][0]++ ; n[s][2327][1]+=count ; }
    else if( (nhPlus+nhPlusStar+nh0+nh0Star)==3                           ){ n[s][2328][0]++ ; n[s][2328][1]+=count ; }
    else if( (nhPlus+nhPlusStar+nh0+nh0Star)==1 && (nK0+nK0Star)==2       ){ n[s][2329][0]++ ; n[s][2329][1]+=count ; }
    else if( (nhPlus+nhPlusStar+nh0+nh0Star)==1 && (nKPlus+nKPlusStar)==2 ){ n[s][2330][0]++ ; n[s][2330][1]+=count ; }
    else if( (nhPlus+nhPlusStar+nh0+nh0Star)==1 && (nKPlus+nKPlusStar)==1 && (nK0+nK0Star)==1 ){ n[s][2331][0]++ ; n[s][2331][1]+=count ; }
    else
    {
      n_EllIsUnknown[s][0]++ ; n_EllIsUnknown[s][1]+=count ;
      PrintDaughters(count, s, EllMothDauLen, rawEllMothDauLen, EllMothLund, sibling, rawSibling, nhPlus, nhPlusStar, nh0, nh0Star, nKPlus, nKPlusStar, nK0, nK0Star, ng) ;
    }
      }
      else if(EllMothDauLen==4)
      {
    // Hadronic
    if(      match(211, 211, 211, 211, abs(sibling[0]), abs(sibling[1]), abs(sibling[2]), abs(sibling[3]))){ n[s][2427][0]++ ; n[s][2427][1]+=count ; }
    else if( match(211, 211, 111, 111, abs(sibling[0]), abs(sibling[1]), abs(sibling[2]), abs(sibling[3]))){ n[s][2428][0]++ ; n[s][2428][1]+=count ; }
    else if( (nhPlus+nhPlusStar+nh0+nh0Star)==2 && (nKPlus+nKPlusStar+nK0+nK0Star)==2 ){ n[s][2429][0]++ ; n[s][2429][1]+=count ; }
    else if( (nhPlus+nhPlusStar+nh0+nh0Star)==1 && (nKPlus+nKPlusStar+nK0+nK0Star)==3 ){ n[s][2430][0]++ ; n[s][2430][1]+=count ; }
    else if( (nhPlus+nhPlusStar+nh0+nh0Star)==3 && (nKPlus+nKPlusStar+nK0+nK0Star)==1 ){ n[s][2431][0]++ ; n[s][2431][1]+=count ; }

    // Semileptonic radiative
    else if( match( 11,  12,   321, 111, abs(sibling[0]), abs(sibling[1]), abs(sibling[2]), abs(sibling[3])) ){ n[s][2401][0]++ ; n[s][2401][1]+=count ; }
    else if( match( 13,  14,   321, 111, abs(sibling[0]), abs(sibling[1]), abs(sibling[2]), abs(sibling[3])) ){ n[s][2402][0]++ ; n[s][2402][1]+=count ; }
    else if( match( 11,  12,   311, 211, abs(sibling[0]), abs(sibling[1]), abs(sibling[2]), abs(sibling[3])) ){ n[s][2403][0]++ ; n[s][2403][1]+=count ; }
    else if( match( 13,  14,   311, 211, abs(sibling[0]), abs(sibling[1]), abs(sibling[2]), abs(sibling[3])) ){ n[s][2404][0]++ ; n[s][2404][1]+=count ; }
    else
    {
      n_EllIsUnknown[s][0]++ ; n_EllIsUnknown[s][1]+=count ;
      PrintDaughters(count, s, EllMothDauLen, rawEllMothDauLen, EllMothLund, sibling, rawSibling, nhPlus, nhPlusStar, nh0, nh0Star, nKPlus, nKPlusStar, nK0, nK0Star, ng) ;
    }
      }
      else if(EllMothDauLen==5)
      {
    // Hadronic
    if(      (nhPlus+nhPlusStar+nh0+nh0Star)==5 && (nKPlus+nKPlusStar+nK0+nK0Star)==0 ){ n[s][2525][0]++ ; n[s][2525][1]+=count ; }
    else if( (nhPlus+nhPlusStar+nh0+nh0Star)==4 && (nKPlus+nKPlusStar+nK0+nK0Star)==1 ){ n[s][2526][0]++ ; n[s][2526][1]+=count ; }
    else if( (nhPlus+nhPlusStar+nh0+nh0Star)==3 && (nKPlus+nKPlusStar+nK0+nK0Star)==2 ){ n[s][2527][0]++ ; n[s][2527][1]+=count ; }
    else if( (nhPlus+nhPlusStar+nh0+nh0Star)==2 && (nKPlus+nKPlusStar+nK0+nK0Star)==3 ){ n[s][2528][0]++ ; n[s][2528][1]+=count ; }
    else if( (nhPlus+nhPlusStar+nh0+nh0Star)==1 && (nKPlus+nKPlusStar+nK0+nK0Star)==4 ){ n[s][2529][0]++ ; n[s][2529][1]+=count ; }
    else if( (nhPlus+nhPlusStar+nh0+nh0Star)==0 && (nKPlus+nKPlusStar+nK0+nK0Star)==5 ){ n[s][2530][0]++ ; n[s][2530][1]+=count ; }
    else
    {
      n_EllIsUnknown[s][0]++ ; n_EllIsUnknown[s][1]+=count ;
      PrintDaughters(count, s, EllMothDauLen, rawEllMothDauLen, EllMothLund, sibling, rawSibling, nhPlus, nhPlusStar, nh0, nh0Star, nKPlus, nKPlusStar, nK0, nK0Star, ng) ;
    }
      }
      else if(EllMothDauLen==6)
      {
    // Hadronic
    if(      (nhPlus+nhPlusStar+nh0+nh0Star)==6 && (nKPlus+nKPlusStar+nK0+nK0Star)==0 ){ n[s][2625][0]++ ; n[s][2625][1]+=count ; }
    else if( (nhPlus+nhPlusStar+nh0+nh0Star)==5 && (nKPlus+nKPlusStar+nK0+nK0Star)==1 ){ n[s][2626][0]++ ; n[s][2626][1]+=count ; }
    else if( (nhPlus+nhPlusStar+nh0+nh0Star)==4 && (nKPlus+nKPlusStar+nK0+nK0Star)==2 ){ n[s][2627][0]++ ; n[s][2627][1]+=count ; }
    else if( (nhPlus+nhPlusStar+nh0+nh0Star)==3 && (nKPlus+nKPlusStar+nK0+nK0Star)==3 ){ n[s][2628][0]++ ; n[s][2628][1]+=count ; }
    else if( (nhPlus+nhPlusStar+nh0+nh0Star)==2 && (nKPlus+nKPlusStar+nK0+nK0Star)==4 ){ n[s][2629][0]++ ; n[s][2629][1]+=count ; }
    else if( (nhPlus+nhPlusStar+nh0+nh0Star)==1 && (nKPlus+nKPlusStar+nK0+nK0Star)==5 ){ n[s][2630][0]++ ; n[s][2630][1]+=count ; }
    else if( (nhPlus+nhPlusStar+nh0+nh0Star)==0 && (nKPlus+nKPlusStar+nK0+nK0Star)==6 ){ n[s][2631][0]++ ; n[s][2631][1]+=count ; }
    else 
    {
      n_EllIsUnknown[s][0]++ ; n_EllIsUnknown[s][1]+=count ;
      PrintDaughters(count, s, EllMothDauLen, rawEllMothDauLen, EllMothLund, sibling, rawSibling, nhPlus, nhPlusStar, nh0, nh0Star, nKPlus, nKPlusStar, nK0, nK0Star, ng) ;
    }
      }
      else if(EllMothDauLen==7)
      {
    // Hadronic
    if(      (nhPlus+nhPlusStar+nh0+nh0Star)==7 && (nKPlus+nKPlusStar+nK0+nK0Star)==0 ){ n[s][2725][0]++ ; n[s][2725][1]+=count ; }
    else if( (nhPlus+nhPlusStar+nh0+nh0Star)==6 && (nKPlus+nKPlusStar+nK0+nK0Star)==1 ){ n[s][2726][0]++ ; n[s][2726][1]+=count ; }
    else if( (nhPlus+nhPlusStar+nh0+nh0Star)==5 && (nKPlus+nKPlusStar+nK0+nK0Star)==2 ){ n[s][2727][0]++ ; n[s][2727][1]+=count ; }
    else if( (nhPlus+nhPlusStar+nh0+nh0Star)==4 && (nKPlus+nKPlusStar+nK0+nK0Star)==3 ){ n[s][2728][0]++ ; n[s][2728][1]+=count ; }
    else if( (nhPlus+nhPlusStar+nh0+nh0Star)==3 && (nKPlus+nKPlusStar+nK0+nK0Star)==4 ){ n[s][2729][0]++ ; n[s][2729][1]+=count ; }
    else if( (nhPlus+nhPlusStar+nh0+nh0Star)==2 && (nKPlus+nKPlusStar+nK0+nK0Star)==5 ){ n[s][2730][0]++ ; n[s][2730][1]+=count ; }
    else if( (nhPlus+nhPlusStar+nh0+nh0Star)==1 && (nKPlus+nKPlusStar+nK0+nK0Star)==6 ){ n[s][2731][0]++ ; n[s][2731][1]+=count ; }
    else if( (nhPlus+nhPlusStar+nh0+nh0Star)==0 && (nKPlus+nKPlusStar+nK0+nK0Star)==7 ){ n[s][2732][0]++ ; n[s][2732][1]+=count ; }
    else
    {
      n_EllIsUnknown[s][0]++ ; n_EllIsUnknown[s][1]+=count ;
      PrintDaughters(count, s, EllMothDauLen, rawEllMothDauLen, EllMothLund, sibling, rawSibling, nhPlus, nhPlusStar, nh0, nh0Star, nKPlus, nKPlusStar, nK0, nK0Star, ng) ;
    }
      }
      else
      {
    n_EllIsUnknown[s][0]++ ; n_EllIsUnknown[s][1]+=count ;
    nTooManyDaughters[s]++ ;
    n[s][2999][0]++ ; n[s][2999][1]+=count ;
    PrintDaughters(count, s, EllMothDauLen, rawEllMothDauLen, EllMothLund, sibling, rawSibling, nhPlus, nhPlusStar, nh0, nh0Star, nKPlus, nKPlusStar, nK0, nK0Star, ng) ;
      }
    }
    else if(abs(EllMothLund)==431)
    {
      for(Charged=0 ; Charged<NCharged ; Charged++){ if(abs(EllLund)==chargedLund[Charged] ){ nMoth[3][s][Charged][0]++ ; nMoth[3][s][Charged][1]+=count ; }}
      if(EllMothDauLen==1)
      {
    cout << "Ds+ with one daughter.  Something has gone wrong!" << endl ;
    PrintDaughters(count, s, EllMothDauLen, rawEllMothDauLen, EllMothLund, sibling, rawSibling, nhPlus, nhPlusStar, nh0, nh0Star, nKPlus, nKPlusStar, nK0, nK0Star, ng) ;
      }
      else if(EllMothDauLen==2)
      {
    // Leptonic decays
    if(      match( -11,  12, sibling[0], sibling[1]) ){ n[s][3200][0]++ ; n[s][3200][1]+=count ; }
    else if( match(  11, -12, sibling[0], sibling[1]) ){ n[s][3201][0]++ ; n[s][3201][1]+=count ; }
    else if( match( -13,  14, sibling[0], sibling[1]) ){ n[s][3202][0]++ ; n[s][3202][1]+=count ; }
    else if( match(  13, -14, sibling[0], sibling[1]) ){ n[s][3203][0]++ ; n[s][3203][1]+=count ; }
    // Hadronic decays
    else if( match( 333, 211, abs(sibling[0]), abs(sibling[1])) ){ n[s][3225][0]++ ; n[s][3225][1]+=count ; }
    else if( match( 221, 211, abs(sibling[0]), abs(sibling[1])) ){ n[s][3236][0]++ ; n[s][3226][1]+=count ; }
    else if( match( 313, 321, abs(sibling[0]), abs(sibling[1])) ){ n[s][3237][0]++ ; n[s][3227][1]+=count ; }
    else if(nhPlus==1     && nK0==1         )    { n[s][3226][0]++ ; n[s][3226][1]+=count ; }
    else if(nhPlusStar==1 && nK0==1         )    { n[s][3227][0]++ ; n[s][3227][1]+=count ; }
    else if(nhPlus==1     && nK0Star==1     )    { n[s][3228][0]++ ; n[s][3228][1]+=count ; }
    else if(nhPlusStar==1 && nK0Star==1     )    { n[s][3229][0]++ ; n[s][3229][1]+=count ; }
    else if(nh0==1        && nKPlus==1      )    { n[s][3230][0]++ ; n[s][3230][1]+=count ; }
    else if(nh0Star==1    && nKPlus==1      )    { n[s][3231][0]++ ; n[s][3231][1]+=count ; }
    else if(nh0==1        && nKPlusStar==1  )    { n[s][3232][0]++ ; n[s][3232][1]+=count ; }
    else if(nh0Star==1    && nKPlusStar==1  )    { n[s][3233][0]++ ; n[s][3233][1]+=count ; }
    else if(nhPlus+nhPlusStar+nh0+nh0Star==2){ n[s][3234][0]++ ; n[s][3234][1]+=count ;}
    else if(nKPlus+nKPlusStar+nK0+nK0Star==2){ n[s][3235][0]++ ; n[s][3235][1]+=count ;}
    else
    {
      n_EllIsUnknown[s][0]++ ; n_EllIsUnknown[s][1]+=count ;
      PrintDaughters(count, s, EllMothDauLen, rawEllMothDauLen, EllMothLund, sibling, rawSibling, nhPlus, nhPlusStar, nh0, nh0Star, nKPlus, nKPlusStar, nK0, nK0Star, ng) ;
    }
      }
      else if(EllMothDauLen==3)
      {
    // Semileptonic decays
    // phi etc l nu
    if(      match( 11,  12,   311, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][3301][0]++ ; n[s][3301][1]+=count ; }
    else if( match( 13,  14,   311, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][3302][0]++ ; n[s][3302][1]+=count ; }
    else if( match( 11,  12,   313, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][3303][0]++ ; n[s][3303][1]+=count ; }
    else if( match( 11,  12,   315, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][3303][0]++ ; n[s][3303][1]+=count ; }
    else if( match( 11,  12,   317, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][3303][0]++ ; n[s][3303][1]+=count ; }
    else if( match( 11,  12, 10313, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][3303][0]++ ; n[s][3303][1]+=count ; }
    else if( match( 11,  12, 20313, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][3303][0]++ ; n[s][3303][1]+=count ; }
    else if( match( 13,  14,   313, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][3304][0]++ ; n[s][3304][1]+=count ; }
    else if( match( 13,  14,   315, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][3304][0]++ ; n[s][3304][1]+=count ; }
    else if( match( 13,  14,   317, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][3304][0]++ ; n[s][3304][1]+=count ; }
    else if( match( 13,  14, 10313, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][3304][0]++ ; n[s][3304][1]+=count ; }
    else if( match( 13,  14, 20313, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][3304][0]++ ; n[s][3304][1]+=count ; }
    
    else if( match( 11,  12,  223, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][3305][0]++ ; n[s][3305][1]+=count ; }
    else if( match( 13,  14,  223, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][3306][0]++ ; n[s][3306][1]+=count ; }
    else if( match( 11,  12,  221, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][3307][0]++ ; n[s][3307][1]+=count ; }
    else if( match( 13,  14,  221, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][3308][0]++ ; n[s][3308][1]+=count ; }
    else if( match( 11,  12,  331, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][3309][0]++ ; n[s][3309][1]+=count ; }
    else if( match( 13,  14,  331, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][3310][0]++ ; n[s][3310][1]+=count ; }
    else if( match( 11,  12,  333, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][3311][0]++ ; n[s][3311][1]+=count ; }
    else if( match( 13,  14,  333, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][3312][0]++ ; n[s][3312][1]+=count ; }
    else if( match( 11,  12,  111, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][3313][0]++ ; n[s][3313][1]+=count ; }
    else if( match( 13,  14,  111, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][3314][0]++ ; n[s][3314][1]+=count ; }
    else if( match( 11,  12,  113, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][3315][0]++ ; n[s][3314][1]+=count ; }
    else if( match( 13,  14,  113, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][3316][0]++ ; n[s][3316][1]+=count ; }
    
    else if( match(111, 211,  333, abs(sibling[0]), abs(sibling[1]), abs(sibling[2])) ){ n[s][3325][0]++ ; n[s][3325][1]+=count ; }
    // Hadronic decays
    else if( (nhPlus+nhPlusStar+nh0+nh0Star)==2 && (nKPlus+nKPlusStar)==1 ){ n[s][3326][0]++ ; n[s][3326][1]+=count ; }
    else if( (nhPlus+nhPlusStar+nh0+nh0Star)==2 && (nK0+nK0Star)==1       ){ n[s][3327][0]++ ; n[s][3327][1]+=count ; }
    else if( (nKPlus+nKPlusStar+nK0+nK0Star)==3                           ){ n[s][3328][0]++ ; n[s][3328][1]+=count ; }
    else if( (nhPlus+nhPlusStar+nh0+nh0Star)==3                           ){ n[s][3329][0]++ ; n[s][3329][1]+=count ; }
    else if( (nhPlus+nhPlusStar+nh0+nh0Star)==1 && (nK0+nK0Star)==2       ){ n[s][3330][0]++ ; n[s][3330][1]+=count ; }
    else if( (nhPlus+nhPlusStar+nh0+nh0Star)==1 && (nKPlus+nKPlusStar)==2 ){ n[s][3331][0]++ ; n[s][3331][1]+=count ; }
    else if( (nhPlus+nhPlusStar+nh0+nh0Star)==1 && (nKPlus+nKPlusStar)==1 && (nK0+nK0Star)==1 ){ n[s][3332][0]++ ; n[s][3332][1]+=count ; }
    else
    {
      n_EllIsUnknown[s][0]++ ; n_EllIsUnknown[s][1]+=count ;
      PrintDaughters(count, s, EllMothDauLen, rawEllMothDauLen, EllMothLund, sibling, rawSibling, nhPlus, nhPlusStar, nh0, nh0Star, nKPlus, nKPlusStar, nK0, nK0Star, ng) ;
    }
      }
      else if(EllMothDauLen==4)
      {
    // Hadronic decays
    if     ( match( 211, 211, 321, 311, abs(sibling[0]), abs(sibling[1]), abs(sibling[2]), abs(sibling[3])) ){ n[s][3425][0]++ ; n[s][3425][1]+=count ; }
    else if( match( 211, 111, 321, 321, abs(sibling[0]), abs(sibling[1]), abs(sibling[2]), abs(sibling[3])) ){ n[s][3425][0]++ ; n[s][3425][1]+=count ; }
    else if( match( 211, 111, 311, 311, abs(sibling[0]), abs(sibling[1]), abs(sibling[2]), abs(sibling[3])) ){ n[s][3425][0]++ ; n[s][3425][1]+=count ; }
    else if( match( 111, 111, 321, 311, abs(sibling[0]), abs(sibling[1]), abs(sibling[2]), abs(sibling[3])) ){ n[s][3425][0]++ ; n[s][3425][1]+=count ; }
    else if( (nhPlus+nhPlusStar+nh0+nh0Star)==3 && (nKPlus+nKPlusStar)==1 && ng==0){ n[s][3426][0]++ ; n[s][3426][1]+=count ; }
    else if( (nhPlus+nhPlusStar+nh0+nh0Star)==4 && (nKPlus+nKPlusStar)==0 && ng==0){ n[s][3427][0]++ ; n[s][3427][1]+=count ; }
    
    // Radiative hadronic decays
    else if( match( 333, 211, abs(sibling[0]), abs(sibling[1])) ){ n[s][3225][0]++ ; n[s][322][1]+=count ; }
    else if( match( 221, 211, abs(sibling[0]), abs(sibling[1])) ){ n[s][3236][0]++ ; n[s][3226][1]+=count ; }
    else if( match( 313, 321, abs(sibling[0]), abs(sibling[1])) ){ n[s][3237][0]++ ; n[s][3227][1]+=count ; }
    else
    {
      n_EllIsUnknown[s][0]++ ; n_EllIsUnknown[s][1]+=count ;
      PrintDaughters(count, s, EllMothDauLen, rawEllMothDauLen, EllMothLund, sibling, rawSibling, nhPlus, nhPlusStar, nh0, nh0Star, nKPlus, nKPlusStar, nK0, nK0Star, ng) ;
    }
      }
      else
      {
    n_EllIsUnknown[s][0]++ ; n_EllIsUnknown[s][1]+=count ;
    nTooManyDaughters[s]++ ;
    n[s][3999][0]++ ; n[s][3999][1]+=count ;
    PrintDaughters(count, s, EllMothDauLen, rawEllMothDauLen, EllMothLund, sibling, rawSibling, nhPlus, nhPlusStar, nh0, nh0Star, nKPlus, nKPlusStar, nK0, nK0Star, ng) ;
      }
    }
    else if( abs(EllMothLund)>999 && abs(EllMothLund)<10000 )
    {
      for(Charged=0 ; Charged<NCharged ; Charged++){ if(abs(EllLund)==chargedLund[Charged] ){ nMoth[4][s][Charged][0]++ ; nMoth[4][s][Charged][1]+=count ; }}
      n[s][5601][0]++ ; n[s][5601][1]+=count ;
    }
    else if(abs(EllMothLund)==15)
    {
      for(Charged=0 ; Charged<NCharged ; Charged++){ if(abs(EllLund)==chargedLund[Charged] ){ nMoth[5][s][Charged][0]++ ; nMoth[5][s][Charged][1]+=count ; }}
      if(     abs(EllLund)==11)  { n[s][4101][0]++ ; n[s][4101][1]+=count ; }
      else if(abs(EllLund)==13)  { n[s][4102][0]++ ; n[s][4102][1]+=count ; }
      else if(EllMothDauLen==2 && abs(EllLund)==211) { n[s][4103][0]++ ; n[s][4103][1]+=count ; }
      else if(EllMothDauLen==3 && abs(EllLund)==211) { n[s][4105][0]++ ; n[s][4105][1]+=count ; }
      else if(EllMothDauLen==4 && abs(EllLund)==211) { n[s][4106][0]++ ; n[s][4106][1]+=count ; }
      else if(abs(EllLund)==321) { n[s][4104][0]++ ; n[s][4104][1]+=count ; }
      else
      {
    n_EllIsUnknown[s][0]++ ; n_EllIsUnknown[s][1]+=count ;
    PrintDaughters(count, s, EllMothDauLen, rawEllMothDauLen, EllMothLund, sibling, rawSibling, nhPlus, nhPlusStar, nh0, nh0Star, nKPlus, nKPlusStar, nK0, nK0Star, ng) ;
      }
    }
    else if(abs(EllMothLund)==521)
    {
      for(Charged=0 ; Charged<NCharged ; Charged++){ if(abs(EllLund)==chargedLund[Charged] ){ nMoth[12][s][Charged][0]++ ; nMoth[12][s][Charged][1]+=count ; }}
      n[s][5101][0]++ ; n[s][5101][1]+=count ;
    }
    else if(abs(EllMothLund)==511)
    {
      for(Charged=0 ; Charged<NCharged ; Charged++){ if(abs(EllLund)==chargedLund[Charged] ){ nMoth[12][s][Charged][0]++ ; nMoth[12][s][Charged][1]+=count ; }}
      n[s][5201][0]++ ; n[s][5201][1]+=count ;
    }
    else if(abs(EllMothLund)==443)
    {
      for(Charged=0 ; Charged<NCharged ; Charged++){ if(abs(EllLund)==chargedLund[Charged] ){ nMoth[12][s][Charged][0]++ ; nMoth[12][s][Charged][1]+=count ; }}
      n[s][5301][0]++ ; n[s][5301][1]+=count ;
    }
    else if(ish(EllMothLund))
    {
      for(Charged=0 ; Charged<NCharged ; Charged++){ if(abs(EllLund)==chargedLund[Charged] ){ nMoth[6][s][Charged][0]++ ; nMoth[6][s][Charged][1]+=count ; }}
      if(EllMothDauLen==1)
      {
    if(abs(EllMothLund)==211&&abs(EllSib1Lund)==211) { n[s][5460][0]++ ; n[s][5460][1]+=count ; }
    else if(   sibling[0]==2212
        || sibling[1]==2212
        || sibling[2]==2212
        || sibling[3]==2212
        || sibling[4]==2212
        || sibling[5]==2212
        || sibling[6]==2212
        || sibling[7]==2212
        || sibling[8]==2212
        || sibling[9]==2212) { n[s][6105][0]++ ; n[s][6105][1]+=count ; }
    else if(   sibling[0]==2112
        || sibling[1]==2112
        || sibling[2]==2112
        || sibling[3]==2112
        || sibling[4]==2112
        || sibling[5]==2112
        || sibling[6]==2112
        || sibling[7]==2112
        || sibling[8]==2112
        || sibling[9]==2112) { n[s][6106][0]++ ; n[s][6106][1]+=count ; }
    else
    {
      n_EllIsUnknown[s][0]++ ; n_EllIsUnknown[s][1]+=count ;
      PrintDaughters(count, s, EllMothDauLen, rawEllMothDauLen, EllMothLund, sibling, rawSibling, nhPlus, nhPlusStar, nh0, nh0Star, nKPlus, nKPlusStar, nK0, nK0Star, ng) ;
    }
      }
      else if(EllMothDauLen==2)
      {
    if     (abs(EllMothLund)==213 && match(211, 111, abs(sibling[0]), abs(sibling[1]))){ n[s][5401][0]++ ; n[s][5401][1]+=count ; }
    else if(abs(EllMothLund)==113 && match(211, 211, abs(sibling[0]), abs(sibling[1]))){ n[s][5402][0]++ ; n[s][5402][1]+=count ; }
    else if(match(321, 321, abs(sibling[0]), abs(sibling[1]) ))        { n[s][5405][0]++ ; n[s][5405][1]+=count ; }
    else if(match(321, 311, abs(sibling[0]), abs(sibling[1]) ))        { n[s][5406][0]++ ; n[s][5406][1]+=count ; }
    else if(abs(EllMothLund)==211)
    {
      if     (abs(EllLund)==13)                    { n[s][6101][0]++ ; n[s][6101][1]+=count ; }
      else if(abs(EllLund)==11)                    { n[s][6103][0]++ ; n[s][6103][1]+=count ; }
      else if(match( 211,   22, abs(sibling[0]), abs(sibling[1]) )){ n[s][5450][0]++ ; n[s][5450][1]+=count ; }
      else if(match( 211, 2212, abs(sibling[0]), abs(sibling[1]) )){ n[s][5451][0]++ ; n[s][5451][1]+=count ; }
      else if(match(  22, 2212, abs(sibling[0]), abs(sibling[1]) )){ n[s][5452][0]++ ; n[s][5452][1]+=count ; }
      else if(match(2212, 2212, abs(sibling[0]), abs(sibling[1]) )){ n[s][5453][0]++ ; n[s][5453][1]+=count ; }
      else if(match( 221,  221, abs(sibling[0]), abs(sibling[1]) )){ n[s][5454][0]++ ; n[s][5454][1]+=count ; }
      else if(match( 211, 2112, abs(sibling[0]), abs(sibling[1]) )){ n[s][5455][0]++ ; n[s][5455][1]+=count ; }
      else if(match( 211,   11, abs(sibling[0]), abs(sibling[1]) )){ n[s][5456][0]++ ; n[s][5456][1]+=count ; }
      else if(match(2212, 1022, abs(sibling[0]), abs(sibling[1]) )){ n[s][5457][0]++ ; n[s][5457][1]+=count ; }
      else if(match(2212,   22, abs(sibling[0]), abs(sibling[1]) )){ n[s][5458][0]++ ; n[s][5458][1]+=count ; }
      else if(match(2212, 2112, abs(sibling[0]), abs(sibling[1]) )){ n[s][5459][0]++ ; n[s][5459][1]+=count ; }
    else if(   sibling[0]==2212
        || sibling[1]==2212
        || sibling[2]==2212
        || sibling[3]==2212
        || sibling[4]==2212
        || sibling[5]==2212
        || sibling[6]==2212
        || sibling[7]==2212
        || sibling[8]==2212
        || sibling[9]==2212) { n[s][6105][0]++ ; n[s][6105][1]+=count ; }
    else if(   sibling[0]==2112
        || sibling[1]==2112
        || sibling[2]==2112
        || sibling[3]==2112
        || sibling[4]==2112
        || sibling[5]==2112
        || sibling[6]==2112
        || sibling[7]==2112
        || sibling[8]==2112
        || sibling[9]==2112) { n[s][6106][0]++ ; n[s][6106][1]+=count ; }
      else
      {
        n_EllIsUnknown[s][0]++ ; n_EllIsUnknown[s][1]+=count ;      
        PrintDaughters(count, s, EllMothDauLen, rawEllMothDauLen, EllMothLund, sibling, rawSibling, nhPlus, nhPlusStar, nh0, nh0Star, nKPlus, nKPlusStar, nK0, nK0Star, ng) ;
      }
    }
    else if(
        abs(EllMothLund)==111   || 
        abs(EllMothLund)==115   || 
        abs(EllMothLund)==221   || 
        abs(EllMothLund)==223   || 
        abs(EllMothLund)==215   || 
        abs(EllMothLund)==225   || 
        abs(EllMothLund)==331   || 
        abs(EllMothLund)==333   || 
        abs(EllMothLund)==10111 || 
        abs(EllMothLund)==10211 || 
        abs(EllMothLund)==10213 || 
        abs(EllMothLund)==10221 || 
        abs(EllMothLund)==10223 || 
        abs(EllMothLund)==10331 || 
        abs(EllMothLund)==20113 || 
        abs(EllMothLund)==20213 ||
        abs(EllMothLund)==20223
        )
    {
      if     (match(  211, 211, abs(sibling[0]), abs(sibling[1]) )){ n[s][5403][0]++ ; n[s][5403][1]+=count ; }
      else if(match(  321, 321, abs(sibling[0]), abs(sibling[1]) )){ n[s][5404][0]++ ; n[s][5404][1]+=count ; }
      else if(match(  223, 111, abs(sibling[0]), abs(sibling[1]) )){ n[s][5407][0]++ ; n[s][5407][1]+=count ; }
      else if(match(  221, 211, abs(sibling[0]), abs(sibling[1]) )){ n[s][5408][0]++ ; n[s][5408][1]+=count ; }
      else if(match(  113, 111, abs(sibling[0]), abs(sibling[1]) )){ n[s][5409][0]++ ; n[s][5409][1]+=count ; }
      else if(match(  213, 211, abs(sibling[0]), abs(sibling[1]) )){ n[s][5410][0]++ ; n[s][5410][1]+=count ; }
      else if(match(10211, 211, abs(sibling[0]), abs(sibling[1]) )){ n[s][5411][0]++ ; n[s][5411][1]+=count ; }
      else if(match(  223, 211, abs(sibling[0]), abs(sibling[1]) )){ n[s][5412][0]++ ; n[s][5412][1]+=count ; }
      else if(match(  113, 211, abs(sibling[0]), abs(sibling[1]) )){ n[s][5413][0]++ ; n[s][5413][1]+=count ; }
      else if(match(   11,  11, abs(sibling[0]), abs(sibling[1]) )){ n[s][5490][0]++ ; n[s][5490][1]+=count ; }
      else if(match(   13,  13, abs(sibling[0]), abs(sibling[1]) )){ n[s][5491][0]++ ; n[s][5491][1]+=count ; }
      else
      {
        n_EllIsUnknown[s][0]++ ; n_EllIsUnknown[s][1]+=count ;
        PrintDaughters(count, s, EllMothDauLen, rawEllMothDauLen, EllMothLund, sibling, rawSibling, nhPlus, nhPlusStar, nh0, nh0Star, nKPlus, nKPlusStar, nK0, nK0Star, ng) ;
      }
    }
    else if(   sibling[0]==2212
        || sibling[1]==2212
        || sibling[2]==2212
        || sibling[3]==2212
        || sibling[4]==2212
        || sibling[5]==2212
        || sibling[6]==2212
        || sibling[7]==2212
        || sibling[8]==2212
        || sibling[9]==2212) { n[s][6105][0]++ ; n[s][6105][1]+=count ; }
    else if(   sibling[0]==2112
        || sibling[1]==2112
        || sibling[2]==2112
        || sibling[3]==2112
        || sibling[4]==2112
        || sibling[5]==2112
        || sibling[6]==2112
        || sibling[7]==2112
        || sibling[8]==2112
        || sibling[9]==2112) { n[s][6106][0]++ ; n[s][6106][1]+=count ; }
    else
    {
      n_EllIsUnknown[s][0]++ ; n_EllIsUnknown[s][1]+=count ;
      PrintDaughters(count, s, EllMothDauLen, rawEllMothDauLen, EllMothLund, sibling, rawSibling, nhPlus, nhPlusStar, nh0, nh0Star, nKPlus, nKPlusStar, nK0, nK0Star, ng) ;
    }
      }
      else if(EllMothDauLen==3)
      {
    if(     match(  211, 211, 111, abs(sibling[0]), abs(sibling[1]), abs(sibling[2]))){ n[s][5414][0]++ ; n[s][5414][1]+=count ; }
    else if(match(  211, 211, 111, abs(sibling[0]), abs(sibling[1]), abs(sibling[2]))){ n[s][5414][0]++ ; n[s][5414][1]+=count ; }
    else if(match(  211, 211, 211, abs(sibling[0]), abs(sibling[1]), abs(sibling[2]))){ n[s][5415][0]++ ; n[s][5415][1]+=count ; }
    else if(match(  213, 211, 211, abs(sibling[0]), abs(sibling[1]), abs(sibling[2]))){ n[s][5416][0]++ ; n[s][5416][1]+=count ; }
    else if(match(  211, 211, 211, abs(sibling[0]), abs(sibling[1]), abs(sibling[2]))){ n[s][5418][0]++ ; n[s][5418][1]+=count ; }
    else if(abs(EllMothLund)==331 && match(  221, 211, 211, abs(sibling[0]), abs(sibling[1]), abs(sibling[2]))){ n[s][5418][0]++ ; n[s][5418][1]+=count ; }
    else if(   sibling[0]==2212
        || sibling[1]==2212
        || sibling[2]==2212
        || sibling[3]==2212
        || sibling[4]==2212
        || sibling[5]==2212
        || sibling[6]==2212
        || sibling[7]==2212
        || sibling[8]==2212
        || sibling[9]==2212) { n[s][6105][0]++ ; n[s][6105][1]+=count ; }
    else if(   sibling[0]==2112
        || sibling[1]==2112
        || sibling[2]==2112
        || sibling[3]==2112
        || sibling[4]==2112
        || sibling[5]==2112
        || sibling[6]==2112
        || sibling[7]==2112
        || sibling[8]==2112
        || sibling[9]==2112) { n[s][6106][0]++ ; n[s][6106][1]+=count ; }
    else
    {
      n_EllIsUnknown[s][0]++ ; n_EllIsUnknown[s][1]+=count ;
      nTooManyDaughters[s]++ ;
      n[s][6998][0]++ ; n[s][6998][1]+=count ;
      PrintDaughters(count, s, EllMothDauLen, rawEllMothDauLen, EllMothLund, sibling, rawSibling, nhPlus, nhPlusStar, nh0, nh0Star, nKPlus, nKPlusStar, nK0, nK0Star, ng) ;
    }
      }
      else
      {
    if     (abs(EllLund)==13) { n[s][6101][0]++ ; n[s][6101][1]+=count ; }
    else if(abs(EllLund)==11) { n[s][6103][0]++ ; n[s][6103][1]+=count ; }
    else if(   sibling[0]==2212
        || sibling[1]==2212
        || sibling[2]==2212
        || sibling[3]==2212
        || sibling[4]==2212
        || sibling[5]==2212
        || sibling[6]==2212
        || sibling[7]==2212
        || sibling[8]==2212
        || sibling[9]==2212) { n[s][6105][0]++ ; n[s][6105][1]+=count ; }
    else if(   sibling[0]==2112
        || sibling[1]==2112
        || sibling[2]==2112
        || sibling[3]==2112
        || sibling[4]==2112
        || sibling[5]==2112
        || sibling[6]==2112
        || sibling[7]==2112
        || sibling[8]==2112
        || sibling[9]==2112) { n[s][6106][0]++ ; n[s][6106][1]+=count ; }
    else
    {
      n_EllIsUnknown[s][0]++ ; n_EllIsUnknown[s][1]+=count ;
      nTooManyDaughters[s]++ ;
      n[s][6998][0]++ ; n[s][6998][1]+=count ;
      PrintDaughters(count, s, EllMothDauLen, rawEllMothDauLen, EllMothLund, sibling, rawSibling, nhPlus, nhPlusStar, nh0, nh0Star, nKPlus, nKPlusStar, nK0, nK0Star, ng) ;
    }
      }
    }
    else if(isK(EllMothLund))
    {
      for(Charged=0 ; Charged<NCharged ; Charged++){ if(abs(EllLund)==chargedLund[Charged] ){ nMoth[7][s][Charged][0]++ ; nMoth[7][s][Charged][1]+=count ; }}
      if(EllMothDauLen==2)
      {
    if(abs(EllMothLund)==321)
    {
      if     ( match( 13,   14, abs(sibling[0]), abs(sibling[1]))){ n[s][6102][0]++ ; n[s][6102][1]+=count ; }  
      else if( match(321, 2212, abs(sibling[0]), abs(sibling[1]))){ n[s][6300][0]++ ; n[s][6300][1]+=count ; }
      else if( match(321, 2112, abs(sibling[0]), abs(sibling[1]))){ n[s][6301][0]++ ; n[s][6301][1]+=count ; }
      else if( match( 11, 2212, abs(sibling[0]), abs(sibling[1]))){ n[s][6302][0]++ ; n[s][6302][1]+=count ; }
      else if( match(211, 2212, abs(sibling[0]), abs(sibling[1]))){ n[s][6303][0]++ ; n[s][6303][1]+=count ; }
      else if( match(2212,2112, abs(sibling[0]), abs(sibling[1]))){ n[s][6304][0]++ ; n[s][6304][1]+=count ; }
      else if( match(211,   22, abs(sibling[0]), abs(sibling[1]))){ n[s][6259][0]++ ; n[s][6259][1]+=count ; }
      else if( match(211,  211, abs(sibling[0]), abs(sibling[1]))){ n[s][6260][0]++ ; n[s][6260][1]+=count ; }
      else if( match( 211, 111, abs(sibling[0]), abs(sibling[1]))){ n[s][6256][0]++ ; n[s][6256][1]+=count ; }
      else if(   sibling[0]==2212
        || sibling[1]==2212
        || sibling[2]==2212
        || sibling[3]==2212
        || sibling[4]==2212
        || sibling[5]==2212
        || sibling[6]==2212
        || sibling[7]==2212
        || sibling[8]==2212
        || sibling[9]==2212) { n[s][6107][0]++ ; n[s][6107][1]+=count ; }
    else if(   sibling[0]==2112
        || sibling[1]==2112
        || sibling[2]==2112
        || sibling[3]==2112
        || sibling[4]==2112
        || sibling[5]==2112
        || sibling[6]==2112
        || sibling[7]==2112
        || sibling[8]==2112
        || sibling[9]==2112) { n[s][6108][0]++ ; n[s][6108][1]+=count ; }
      else
      {
        n_EllIsUnknown[s][0]++ ; n_EllIsUnknown[s][1]+=count ;
        PrintDaughters(count, s, EllMothDauLen, rawEllMothDauLen, EllMothLund, sibling, rawSibling, nhPlus, nhPlusStar, nh0, nh0Star, nKPlus, nKPlusStar, nK0, nK0Star, ng) ;
      }
    }
        else if(
        abs(EllMothLund)==323   || 
        abs(EllMothLund)==325   || 
        abs(EllMothLund)==10321 || 
        abs(EllMothLund)==10323 || 
        abs(EllMothLund)==20323
        )
    {
      if     ( match( 321, 111, abs(sibling[0]), abs(sibling[1])) ){ n[s][6251][0]++ ; n[s][6251][1]+=count ; }
      else if( match( 311, 211, abs(sibling[0]), abs(sibling[1])) ){ n[s][6252][0]++ ; n[s][6252][1]+=count ; }
      else if( match( 321,  22, abs(sibling[0]), abs(sibling[1])) ){ n[s][6254][0]++ ; n[s][6254][1]+=count ; }
      else if( match( 313, 211, abs(sibling[0]), abs(sibling[1])) ){ n[s][6258][0]++ ; n[s][6258][1]+=count ; }
      else
      {
        n_EllIsUnknown[s][0]++ ; n_EllIsUnknown[s][1]+=count ;
        PrintDaughters(count, s, EllMothDauLen, rawEllMothDauLen, EllMothLund, sibling, rawSibling, nhPlus, nhPlusStar, nh0, nh0Star, nKPlus, nKPlusStar, nK0, nK0Star, ng) ;
      }
    }
    else if(
        abs(EllMothLund)==313   || 
        abs(EllMothLund)==315   || 
        abs(EllMothLund)==10311 || 
        abs(EllMothLund)==10313 || 
        abs(EllMothLund)==20313
        )
    {
      if     ( match( 321, 211, abs(sibling[0]), abs(sibling[1])) ){ n[s][6253][0]++ ; n[s][6253][1]+=count ; }
      else if( match( 321, 213, abs(sibling[0]), abs(sibling[1])) ){ n[s][6255][0]++ ; n[s][6255][1]+=count ; }
      else if( match( 323, 211, abs(sibling[0]), abs(sibling[1])) ){ n[s][6258][0]++ ; n[s][6258][1]+=count ; }
      else
      {
        n_EllIsUnknown[s][0]++ ; n_EllIsUnknown[s][1]+=count ;
        PrintDaughters(count, s, EllMothDauLen, rawEllMothDauLen, EllMothLund, sibling, rawSibling, nhPlus, nhPlusStar, nh0, nh0Star, nKPlus, nKPlusStar, nK0, nK0Star, ng) ;
      }
    }
    else if(abs(EllMothLund)==310 && match( 211, 211, abs(sibling[0]), abs(sibling[1])) ){ n[s][6257][0]++ ; n[s][6257][1]+=count ; }
    else if(abs(EllMothLund)==130 && match( 211, 211, abs(sibling[0]), abs(sibling[1])) ){ n[s][6261][0]++ ; n[s][6261][1]+=count ; }
    else
    {
      n_EllIsUnknown[s][0]++ ; n_EllIsUnknown[s][1]+=count ;
      PrintDaughters(count, s, EllMothDauLen, rawEllMothDauLen, EllMothLund, sibling, rawSibling, nhPlus, nhPlusStar, nh0, nh0Star, nKPlus, nKPlusStar, nK0, nK0Star, ng) ;
    }
      }
      else if(EllMothDauLen==3)
      {
    if     ( EllMothLund==310 && match(111,211,211,abs(sibling[0]),abs(sibling[1]),abs(sibling[2]))){ n[s][6351][0]++ ; n[s][6351][1]+=count ; }
    else if( match(321,211,111,abs(sibling[0]),abs(sibling[1]),abs(sibling[2]))){ n[s][6352][0]++ ; n[s][6352][1]+=count ; }
    else if( match(311,211,211,abs(sibling[0]),abs(sibling[1]),abs(sibling[2]))){ n[s][6353][0]++ ; n[s][6353][1]+=count ; }
    else if( match(310,211,211,abs(sibling[0]),abs(sibling[1]),abs(sibling[2]))){ n[s][6353][0]++ ; n[s][6353][1]+=count ; }
    else if( match(130,211,211,abs(sibling[0]),abs(sibling[1]),abs(sibling[2]))){ n[s][6353][0]++ ; n[s][6353][1]+=count ; }
    else if( match(321,211,211,abs(sibling[0]),abs(sibling[1]),abs(sibling[2]))){ n[s][6354][0]++ ; n[s][6354][1]+=count ; }
    else if( match(321,111,111,abs(sibling[0]),abs(sibling[1]),abs(sibling[2]))){ n[s][6355][0]++ ; n[s][6355][1]+=count ; }
    else if( match(311,211,111,abs(sibling[0]),abs(sibling[1]),abs(sibling[2]))){ n[s][6356][0]++ ; n[s][6356][1]+=count ; }
    else if( match(310,211,111,abs(sibling[0]),abs(sibling[1]),abs(sibling[2]))){ n[s][6356][0]++ ; n[s][6356][1]+=count ; }
    else if( match(130,211,111,abs(sibling[0]),abs(sibling[1]),abs(sibling[2]))){ n[s][6356][0]++ ; n[s][6356][1]+=count ; }
    else if(   sibling[0]==2212
        || sibling[1]==2212
        || sibling[2]==2212
        || sibling[3]==2212
        || sibling[4]==2212
        || sibling[5]==2212
        || sibling[6]==2212
        || sibling[7]==2212
        || sibling[8]==2212
        || sibling[9]==2212) { n[s][6107][0]++ ; n[s][6107][1]+=count ; }
    else if(   sibling[0]==2112
        || sibling[1]==2112
        || sibling[2]==2112
        || sibling[3]==2112
        || sibling[4]==2112
        || sibling[5]==2112
        || sibling[6]==2112
        || sibling[7]==2112
        || sibling[8]==2112
        || sibling[9]==2112) { n[s][6108][0]++ ; n[s][6108][1]+=count ; }
    else
    {
      n_EllIsUnknown[s][0]++ ; n_EllIsUnknown[s][1]+=count ;
      PrintDaughters(count, s, EllMothDauLen, rawEllMothDauLen, EllMothLund, sibling, rawSibling, nhPlus, nhPlusStar, nh0, nh0Star, nKPlus, nKPlusStar, nK0, nK0Star, ng) ;
    }
      }
      else
      {
    if(        sibling[0]==2212
        || sibling[1]==2212
        || sibling[2]==2212
        || sibling[3]==2212
        || sibling[4]==2212
        || sibling[5]==2212
        || sibling[6]==2212
        || sibling[7]==2212
        || sibling[8]==2212
        || sibling[9]==2212) { n[s][6107][0]++ ; n[s][6107][1]+=count ; }
    else if(   sibling[0]==2112
        || sibling[1]==2112
        || sibling[2]==2112
        || sibling[3]==2112
        || sibling[4]==2112
        || sibling[5]==2112
        || sibling[6]==2112
        || sibling[7]==2112
        || sibling[8]==2112
        || sibling[9]==2112) { n[s][6108][0]++ ; n[s][6108][1]+=count ; }
    else
    {
      n_EllIsUnknown[s][0]++ ; n_EllIsUnknown[s][1]+=count ;
      nTooManyDaughters[s]++ ;
      n[s][6999][0]++ ; n[s][6999][1]+=count ;
      PrintDaughters(count, s, EllMothDauLen, rawEllMothDauLen, EllMothLund, sibling, rawSibling, nhPlus, nhPlusStar, nh0, nh0Star, nKPlus, nKPlusStar, nK0, nK0Star, ng) ;
    }
      }
    }
    else if( isDStar(EllMothLund))
    {
      for(Charged=0 ; Charged<NCharged ; Charged++){ if(abs(EllLund)==chargedLund[Charged] ){ nMoth[8][s][Charged][0]++ ; nMoth[8][s][Charged][1]+=count ; }}
      n[s][5701][0]++ ; n[s][5701][1]+=count ;
    }
    else if( abs(EllMothLund)==11)
    {
      for(Charged=0 ; Charged<NCharged ; Charged++){ if(abs(EllLund)==chargedLund[Charged] ){ nMoth[9][s][Charged][0]++ ; nMoth[9][s][Charged][1]+=count ; }}
      n[s][7101][0]++ ; n[s][7101][1]+=count ;
    }
    else if( isBaryon(EllMothLund))
    {
      for(Charged=0 ; Charged<NCharged ; Charged++){ if(abs(EllLund)==chargedLund[Charged] ){ nMoth[10][s][Charged][0]++ ; nMoth[10][s][Charged][1]+=count ; }}
    }
    else if( abs(EllMothLund)==10022 )
    {
      for(Charged=0 ; Charged<NCharged ; Charged++){ if(abs(EllLund)==chargedLund[Charged] ){ nMoth[11][s][Charged][0]++ ; nMoth[11][s][Charged][1]+=count ; }}
      if(abs(EllLund)==211)      { n[s][5501][0]++ ; n[s][5501][1]+=count ; }
      else if(abs(EllLund)==321) { n[s][5502][0]++ ; n[s][5502][1]+=count ; }
      else if(abs(EllLund)==2212){ n[s][5503][0]++ ; n[s][5503][1]+=count ; }
      else
      {
    n_EllIsUnknown[s][0]++ ; n_EllIsUnknown[s][1]+=count ;
    PrintDaughters(count, s, EllMothDauLen, rawEllMothDauLen, EllMothLund, sibling, rawSibling, nhPlus, nhPlusStar, nh0, nh0Star, nKPlus, nKPlusStar, nK0, nK0Star, ng) ;
      }
    }
    else
    {
      for(Charged=0 ; Charged<NCharged ; Charged++){ if(abs(EllLund)==chargedLund[Charged] ){ nMoth[13][s][Charged][0]++ ; nMoth[13][s][Charged][1]+=count ; }}
      n_EllIsUnknown[s][0]++ ; n_EllIsUnknown[s][1]+=count ;
      cout << "MothLund not taken into account.  SPMode = " << SPMode << ", MothLund = " << EllMothLund << endl ;
      PrintDaughters(count, s, EllMothDauLen, rawEllMothDauLen, EllMothLund, sibling, rawSibling, nhPlus, nhPlusStar, nh0, nh0Star, nKPlus, nKPlusStar, nK0, nK0Star, ng) ;
    }
  }

  Int_t width = 10 ;
  for(SignalMode=0 ; SignalMode<NSignalMode-1 ; SignalMode++)
  {
    cout << "========================================"    << endl ;
    cout << " " << EllName[SignalMode] << " : " << endl ;
    cout << "========================================"    << endl ;
    for(Charged=0 ; Charged<NCharged ; Charged++)
    {
      cout << "  True particle is " ;
      cout.width(8) ; cout << chargedName[Charged] ; cout << ": " ;
      cout.width(6) ; cout << nTrueLund[SignalMode][Charged][0] ;
      cout << " " ;   cout.width(6) ;
      cout.width(6) ; cout << nTrueLund[SignalMode][Charged][1] ;
      cout << endl ;
    }
    cout << endl ;
  }
  
  cout << endl ;
  cout << "ell type is unclassified: " << endl ;
  for(SignalMode=0 ; SignalMode<NSignalMode-1 ; SignalMode++)
  {
    cout.width(30) ;
    cout << EllName[SignalMode] << " " ;
    cout.width(6) ; cout << n_EllIsUnknown[SignalMode][0] ; cout << " " ;
    cout.width(6) ; cout << n_EllIsUnknown[SignalMode][1] << endl ;
  }
  cout << endl ;
  
  TString EllNameLaTeX[NSignalMode] = { "\\mu" , "e" , "\\mu" , "e" , "\\pi" } ;
  // Print table of results
  // Rows of mothers
  // Columns of ell candidates
  for(SignalMode=0 ; SignalMode<NSignalMode-1 ; SignalMode++)
  {
    cout << "===================================" << endl ;
    cout << " " << EllName[SignalMode] << " : "   << endl ;
    cout << "===================================" << endl ;
    Int_t    mothTotal[NMothType][NEExtraCut] ;
    Int_t chargedTotal[NMothType][NEExtraCut] ;
    for(EExtraCut=0 ; EExtraCut<NEExtraCut ; EExtraCut++)
    {
      for(Mother=0 ; Mother<NMothType ; Mother++)
      {
    mothTotal[Mother][EExtraCut] = 0 ;
    for(Charged=0 ; Charged<NCharged ; Charged++) mothTotal[Mother][EExtraCut] += nMoth[Mother][SignalMode][Charged][EExtraCut] ;
      }
      for(Charged=0 ; Charged<NCharged ; Charged++)
      {
    chargedTotal[Charged][EExtraCut] = 0 ;
    for(Mother=0 ; Mother<NMothType ; Mother++) chargedTotal[Charged][EExtraCut] += nMoth[Mother][SignalMode][Charged][EExtraCut] ;
      }
    }
    cout.width(width+5) ; cout << "" ;
    for(Charged=0 ; Charged<NCharged ; Charged++)
    {
      cout.width(width) ;
      cout << chargedName[Charged] ;
    }
    cout << endl ;
    for(Mother=0 ; Mother<NMothType ; Mother++)
    {
      Int_t totalTemp = 0 ;
      for(Charged=0 ; Charged<NCharged ; Charged++) totalTemp += nMoth[Mother][SignalMode][Charged][0] ;
      if(totalTemp==0) continue ;
      for(EExtraCut=0 ; EExtraCut<NEExtraCut ; EExtraCut++)
      {
    if(EExtraCut==0){ cout << "n(" ; cout.width(10) ; cout << mothName[Mother] ; cout << "): " ; }
    else{ cout << "               " ; }
    for(Charged=0 ; Charged<NCharged ; Charged++){ cout.width(width) ; cout << nMoth[Mother][SignalMode][Charged][EExtraCut] ; }
    cout.width(width+5) ;
    cout << mothTotal[Mother][EExtraCut] ;
    cout << endl ;
    if(EExtraCut==1) cout << endl ;
      }
    }
    for(EExtraCut=0 ; EExtraCut<NEExtraCut ; EExtraCut++)
    {
      if(EExtraCut==0){ cout << "n(" ; cout.width(13) ; cout << "total): " ; }
      else{ cout << "               " ; }
      Int_t grandTotal = 0 ;
      for(Charged=0 ; Charged<NCharged ; Charged++)
      {
    cout.width(width) ;
    cout << chargedTotal[Charged][EExtraCut] ;
    grandTotal += chargedTotal[Charged][EExtraCut] ;
      }
      cout.width(width+5) ; cout << grandTotal ; cout << endl ;
    }
    cout << endl << endl ;
  }
  
  cout << "Total passed:" << endl ;
  for(SignalMode=0 ; SignalMode<NSignalMode-1 ; SignalMode++)
  {
    cout.width(30) ; cout << EllName[SignalMode] ; cout << ": " ;
    cout.width(6)  ; cout << counter[SignalMode][0] ;
    cout.width(6)  ; cout << counter[SignalMode][1] << endl ;
  }
  cout << endl ;
  
  cout << "Too many daughters: " << endl ;
  for(SignalMode=0 ; SignalMode<NSignalMode-1 ; SignalMode++)
  {
    cout.width(30) ; cout << EllName[SignalMode] ;
    cout << ": "   ; cout.width(6) ;
    cout << nTooManyDaughters[SignalMode] << endl ; 
  }
  cout << endl ;

  Int_t nTotal[NSignalMode][NEExtraCut] ;
  for(SignalMode=0 ; SignalMode<NSignalMode-1 ; SignalMode++)
  {
    Float_t counterFloat[NEExtraCut] ;
    for(EExtraCut=0 ; EExtraCut<NEExtraCut ; EExtraCut++)
    {
      nTotal[SignalMode][EExtraCut] = 0 ;
      counterFloat[EExtraCut] = counter[SignalMode][EExtraCut] ;
    }    
    cout << endl ;
    cout << "========================================"    << endl ;
    cout << " " << EllName[SignalMode] << " : "           << endl ;
    cout << "========================================"    << endl ;
    for(decay=0 ; decay<NDecayTypes ; decay++)
    {
      if(n[SignalMode][decay][0]>0)
      {
    Float_t nFloat[NEExtraCut] ;
    for(EExtraCut=0 ; EExtraCut<NEExtraCut ; EExtraCut++){ nFloat[EExtraCut] = n[SignalMode][decay][EExtraCut] ; }
    cout << "Number of " ;
    cout.width(25) ; cout << decayName[decay] ; cout << " :" ;
    for(EExtraCut=0 ; EExtraCut<NEExtraCut ; EExtraCut++)
    {
      cout.width(6) ; cout << n[SignalMode][decay][EExtraCut] ; cout << "  (" ;
      cout.width(8) ; cout << FloatToString((100.*nFloat[EExtraCut]/counterFloat[EExtraCut])) ; cout << " %)" ; cout << "  " ;
      cout.width(8) ; nTotal[SignalMode][EExtraCut] += n[SignalMode][decay][EExtraCut] ;
    }
    cout << endl ;
      }
    }
  }
  
  cout << "Total passed" << endl ;
  for(SignalMode=0 ; SignalMode<NSignalMode-1 ; SignalMode++)
  {
    cout.width(30); cout << EllName[SignalMode] ; cout << ": " ;
    cout.width(6) ; cout << nTotal[SignalMode][0] ;
    cout.width(6) ; cout << nTotal[SignalMode][1] << endl ;
  }
  cout << endl ;

  cout << "Important semileptonic Ds decays:" << endl ; 
  for(SignalMode=0 ; SignalMode<NSignalMode-1 ; SignalMode++)
  {
    cout << "=============================================" << endl ;
    cout << " " << EllName[SignalMode] << " : "             << endl ;
    cout << "=============================================" << endl ;
    for(decay=3301 ; decay<3317 ; decay++)
    {
      if(n[SignalMode][decay][0]==0) continue ;
      cout.width(25) ; cout << decayName[decay] ;
      for(EExtraCut=0 ; EExtraCut<NEExtraCut ; EExtraCut++){ cout << " " ; cout.width(6) ; cout << n[SignalMode][decay][EExtraCut] ; }
      cout << endl ;
    }
  }

  const Int_t numberOfDominants = 20 ;
  Int_t dominantModes[NSignalMode][NEExtraCut][numberOfDominants] ; // Ell, EExtra, decay
  cout << endl << "Dominant modes:" << endl ;
  Int_t NChargedToPlot = 5 ;
  TH1F* hMoth[NSignalMode][NCharged][NEExtraCut] ;
  TLegend* legend[NSignalMode] ;
  THStack stack[NSignalMode][NEExtraCut] ;
  for(SignalMode=0 ; SignalMode<NSignalMode-1 ; SignalMode++)
  {
    

    Int_t ceiling[2] = { 100000 , 100000 } ;
    for(Int_t i=0 ; i<numberOfDominants ; i++)
    {
      Int_t currentBiggestN[2] = { 0 , 0 } ;
      Int_t currentBiggestMode[2] = { 0 , 0 } ;
      for(decay=0 ; decay<NDecayTypes ; decay++)
      {
    for(EExtraCut=0 ; EExtraCut<2 ; EExtraCut++)
    {
      if(n[SignalMode][decay][EExtraCut]>currentBiggestN[EExtraCut] && n[SignalMode][decay][EExtraCut]<ceiling[EExtraCut])
      {
        currentBiggestN[EExtraCut] = n[SignalMode][decay][EExtraCut] ;
        currentBiggestMode[EExtraCut] = decay ;
      }
    }
      }
      for(EExtraCut=0 ; EExtraCut<NEExtraCut ; EExtraCut++)
      {
        ceiling[EExtraCut] = currentBiggestN[EExtraCut] ;
    dominantModes[SignalMode][EExtraCut][i] = currentBiggestMode[EExtraCut] ;
      }
    }

    legend[SignalMode] = new TLegend(0.75, 0.65, 0.9, 0.9) ;
    for(Charged=0 ; Charged<NChargedToPlot ; Charged++)
    {
      for(EExtraCut=0 ; EExtraCut<NEExtraCut ; EExtraCut++)
      {
    TString histogram_name = TString(TString("h_") + SignalModeName[SignalMode] + "_" + chargedName[Charged] + "_" + EExtraCutString[EExtraCut]) ;
    Int_t color = 205 + 4*Charged ;
    if(SignalMode==1||SignalMode==3) color += 3 ;
    if(SignalMode==4) color += 1 ;
    hMoth[SignalMode][Charged][EExtraCut] = new TH1F(histogram_name, "Mother of ell candidate", 10, 1, 11) ;
    hMoth[SignalMode][Charged][EExtraCut]->SetLineWidth(2) ;
    hMoth[SignalMode][Charged][EExtraCut]->SetFillColor(color) ;
    hMoth[SignalMode][Charged][EExtraCut]->GetXaxis()->SetLabelSize(0.03) ;
    hMoth[SignalMode][Charged][EExtraCut]->SetTitle("Mother of ell") ;
    //hMoth[SignalMode][Charged][EExtraCut]->GetYaxis()->SetTitle("Entries") ;
    //hMoth[SignalMode][Charged][EExtraCut]->Fill("#Lambda_{c}"        , nMoth[4][SignalMode][Charged][EExtraCut]) ;
    hMoth[SignalMode][Charged][EExtraCut]->Fill("D^{+}"              , nMoth[1][SignalMode][Charged][EExtraCut]) ;
    hMoth[SignalMode][Charged][EExtraCut]->Fill("D^{0}"              , nMoth[2][SignalMode][Charged][EExtraCut]) ;
    hMoth[SignalMode][Charged][EExtraCut]->Fill("D_{s}"              , nMoth[3][SignalMode][Charged][EExtraCut]) ;
    hMoth[SignalMode][Charged][EExtraCut]->Fill("Excited charm meson", nMoth[8][SignalMode][Charged][EExtraCut]) ;
    hMoth[SignalMode][Charged][EExtraCut]->Fill("Strange meson"      , nMoth[7][SignalMode][Charged][EExtraCut]) ;
    hMoth[SignalMode][Charged][EExtraCut]->Fill("Unflavored meson"   , nMoth[6][SignalMode][Charged][EExtraCut]) ;
    hMoth[SignalMode][Charged][EExtraCut]->Fill("#tau"               , nMoth[5][SignalMode][Charged][EExtraCut]) ;
    hMoth[SignalMode][Charged][EExtraCut]->Fill("#gamma_{virtual}"   , nMoth[11][SignalMode][Charged][EExtraCut]) ;
    hMoth[SignalMode][Charged][EExtraCut]->Fill("#gamma"             , nMoth[0][SignalMode][Charged][EExtraCut]) ;
    //hMoth[SignalMode][Charged][EExtraCut]->Fill("e"                  , nMoth[9][SignalMode][Charged][EExtraCut]) ;
    //hMoth[SignalMode][Charged][EExtraCut]->Fill("Baryon"             , nMoth[10][SignalMode][Charged][EExtraCut]) ;
    hMoth[SignalMode][Charged][EExtraCut]->Fill("Other"              , nMoth[12][SignalMode][Charged][EExtraCut]
                          + nMoth[4][SignalMode][Charged][EExtraCut]
                          + nMoth[9][SignalMode][Charged][EExtraCut]
                          +nMoth[10][SignalMode][Charged][EExtraCut]) ;
    if(EExtraCut==0) legend[SignalMode]->AddEntry(hMoth[SignalMode][Charged][EExtraCut], chargedNameShort[Charged], "f") ;
      }
    }
    for(EExtraCut=0 ; EExtraCut<NEExtraCut ; EExtraCut++)
    {
      for(Charged=NChargedToPlot-1 ; Charged>-1 ; Charged--)
      {
    stack[SignalMode][EExtraCut].Add(hMoth[SignalMode][Charged][EExtraCut]) ;
      }
      stack[SignalMode][EExtraCut].Draw() ;
      legend[SignalMode]->SetFillColor(kWhite) ;
      legend[SignalMode]->SetShadowColor(kWhite) ;
      legend[SignalMode]->Draw() ;
      cBackgroundSources.Update() ;
      PrintCanvas(&cBackgroundSources, TString(TString("TrueMother"))) ;
      cBackgroundSources.Clear() ;
    }

    cout << "=============================================" << endl ;
    cout << " " << EllName[SignalMode] << " : " << endl ;
    cout << "=============================================" << endl ;
    fBAD.open(TString(TString("../AWG90/aidanrc/BAD/note2170/Numerator") + SignalModeName[SignalMode] +  "BackgroundSources.tex")) ;
    fBAD << "\\begin{table}[!hb]\\begin{center}" << endl ;
    if(SignalMode!=1){ fBAD << "\\begin{tabular}{cc}" << endl ; }
    for(EExtraCut=0 ; EExtraCut<NEExtraCut ; EExtraCut++)
    {
      //if(&&EExtraCut==1) continue ;
      if(SignalMode!=1&&EExtraCut==1) fBAD << "    &" << endl ;
      fBAD << "    \\begin{tabular}{cc}" << endl ;
      fBAD << "      \\hline" << endl ;
      fBAD << "      Background & Proportion \\\\" << endl ;
      fBAD << "      \\hline" << endl ;
      for(Int_t j=0 ; j<numberOfDominants ; j++)
      {
    if(n[SignalMode][dominantModes[SignalMode][EExtraCut][j]][EExtraCut]==0) break ;
    cout.width(25) ; cout << decayName[dominantModes[SignalMode][EExtraCut][j]] ; cout << " :" ;
    cout.width(6)  ; cout << n[SignalMode][dominantModes[SignalMode][EExtraCut][j]][EExtraCut] ; cout << " (" ;
    cout.width(6)  ; cout << FloatToString((100*(Float_t)n[SignalMode][dominantModes[SignalMode][EExtraCut][j]][EExtraCut]/(Float_t)nTotal[SignalMode][EExtraCut])) ; cout << "%)" ;
    cout << endl ;
    fBAD << "        $" << decayNameLaTeX[dominantModes[SignalMode][EExtraCut][j]] << "$ & " << FloatToString((100*(Float_t)n[SignalMode][dominantModes[SignalMode][EExtraCut][j]][EExtraCut]/(Float_t)nTotal[SignalMode][EExtraCut])) << "\\%\\\\" << endl ;
      }
      fBAD << "      \\hline" << endl ;
      fBAD << "    \\end{tabular}" << endl ;
      cout << endl << endl ;
    }
    EExtraCut = (SignalMode!=1) ;
    if(SignalMode!=1){ fBAD << "\\end{tabular}" << endl ; }
    fBAD << "  \\end{center}" << endl ;
    fBAD << "  \\caption[Sources of signal and background for the numerator]{Sources of signal and background for the numerator for the $" << SignalModeNameLaTeX[SignalMode] << "$ mode" ;
    if(SignalMode!=1){ fBAD << " before (left) and after (right) the \\eextra\\ cut is placed" << endl ; }
    fBAD << ".  These MC events are not weighted.  $h$ refers to any unflavored meson.}" << endl ;
    fBAD << "  \\label{tab:Numerator_BackgroundSources_" << SignalModeName[SignalMode] << EExtraCutString[EExtraCut] <<"}" << endl ;
    fBAD << "\\end{table}" << endl << endl ;
    
    fBAD << "\\begin{figure}[!hb]\\begin{center}" << endl ;
    if(SignalMode!=1){ fBAD << "\\begin{tabular}{cc}" << endl ; }
    for(EExtraCut=0 ; EExtraCut<NEExtraCut ; EExtraCut++)
    {
      //if(SignalMode==1&&EExtraCut==1) continue ;
      if(SignalMode!=1&&EExtraCut==1) fBAD << "    &" << endl ;
      fBAD << "    \\includegraphics[width=0.4\\textwidth]{plots/Numerator/TrueMother_" << SignalModeName[SignalMode] << EExtraCutString[EExtraCut] << ".eps}" << endl ;
    }
    EExtraCut = (SignalMode!=1) ;
    if(SignalMode!=1){ fBAD << "\\end{tabular}{cc}" << endl ; }
    fBAD << "  \\caption[Sources of signal and background for the numerator]{Sources of signal and background for the numerator for the $" << SignalModeNameLaTeX[SignalMode] << "$ mode" ;
    if(SignalMode!=1){ fBAD << " before (left) and after (right) the \\eextra\\ cut is placed" << endl ; }
    fBAD << ".  These MC events are not weighted.  The x axis indicates the parent of the $" << EllNameLaTeX[SignalMode] << "$ candidate.  The stacked histograms indicate the true particle type of the $" << EllNameLaTeX[SignalMode] << "$ candidate.}" << endl ;
    fBAD << "  \\label{fig:Numerator_BackgroundSources_" << SignalModeName[SignalMode] << EExtraCutString[EExtraCut] <<"}" << endl ;
    fBAD << "  \\end{center}" << endl ;
    fBAD << "\\end{figure}" << endl ;
    fBAD << "\\clearpage" << endl ;
    fBAD.close() ;
  }
  cout << "maxDauLen: " << maxDauLen << endl ;
}

void  DRecoDsToEllNuPlots::PrintDaughters(Int_t count, Int_t SignalMode, Int_t EllMothDauLen, Int_t rawEllMothDauLen, Int_t EllMothLund, Int_t sibling[14], Int_t rawSibling[14], Int_t nhPlus, Int_t nhPlusStar, Int_t nh0, Int_t nh0Star, Int_t nKPlus, Int_t nKPlusStar, Int_t nK0, Int_t nK0Star, Int_t ng)
{
  if(count==0) return ; // Ignore things with no EExtra cut
  cout << "SignalMode: " << SignalMode    ;
  cout << ", DauLen: "   << EllMothDauLen ;
  cout << " (" ; cout << rawEllMothDauLen ; cout << ") " ;
  cout << ", MothLund: " ; cout.width(6) ; cout << EllMothLund ;
  cout << ", SibLunds: " ;        for(Int_t i=0 ; i<rawEllMothDauLen ; i++){ cout.width(9) ; cout <<    sibling[i] ; } cout << endl ;
  return ;  
  cout.width(10) ; cout << " (" ; for(Int_t i=0 ; i<rawEllMothDauLen ; i++){ cout.width(9) ; cout << rawSibling[i] ; } cout << ") " ; cout << endl ;
  cout << "  nhPlus: "     << nhPlus 
       << ", nhPlusStar: " << nhPlusStar 
       << ", nh0: "        << nh0 
       << ", nh0Star"      << nh0Star 
       << ", nKPlus: "     << nKPlus 
       << ", nKPlusStar: " << nKPlusStar 
       << ", nK0: "        << nK0 
       << ", nK0Star: "    << nK0Star 
       << ", ng: "         << ng 
       << endl ;
  return ;
}

Bool_t DRecoDsToEllNuPlots::match(Int_t d1, Int_t d2, Int_t d3, Int_t d4, Int_t d5, Int_t S1, Int_t S2, Int_t S3, Int_t S4, Int_t S5)
{
  if((d1*d2*d3*d4*d5)==(S1*S2*S3*S4*S5)) return true ;
  return false ;
}

Bool_t DRecoDsToEllNuPlots::match(Int_t d1, Int_t d2, Int_t d3, Int_t d4, Int_t S1, Int_t S2, Int_t S3, Int_t S4)
{
  if((d1*d2*d3*d4)==(S1*S2*S3*S4)) return true ;
  return false ;
}

Bool_t DRecoDsToEllNuPlots::match(Int_t d1, Int_t d2, Int_t d3, Int_t S1, Int_t S2, Int_t S3)
{
  if((d1*d2*d3)==(S1*S2*S3)) return true ;
  return false ;
}

Bool_t DRecoDsToEllNuPlots::match(Int_t d1, Int_t d2, Int_t S1, Int_t S2)
{
  if((d1*d2)==(S1*S2)) return true ;
  return false ;
}

Bool_t DRecoDsToEllNuPlots::ish(Int_t lund)
{
  if(ishPlus(lund))     return true ;
  if(ish0(lund))        return true ;
  if(ishPlusStar(lund)) return true ;
  if(ish0Star(lund))    return true ;
  return false ;
}

Bool_t DRecoDsToEllNuPlots::isK(Int_t lund)
{
  if(isKPlus(lund))     return true ;
  if(isK0(lund))        return true ;
  if(isKPlusStar(lund)) return true ;
  if(isK0Star(lund))    return true ;
  return false ;
}

Bool_t DRecoDsToEllNuPlots::ishPlus(Int_t lund)
{
  lund = abs(lund) ;
  if(lund==211) return true ;
  return false ;
}

Bool_t DRecoDsToEllNuPlots::ish0(Int_t lund)
{
  lund = abs(lund) ;
  if(lund==111) return true ;
  return false ;
}

Bool_t DRecoDsToEllNuPlots::ishPlusStar(Int_t lund)
{
  lund = abs(lund) ;
  if(lund==213)   return true ;
  if(lund==215)   return true ;
  if(lund==10211) return true ;
  if(lund==10213) return true ;
  if(lund==20213) return true ;
  // Add more here as needed
  return false ;
}

Bool_t DRecoDsToEllNuPlots::ish0Star(Int_t lund)
{
  lund = abs(lund) ;
  if(lund==113)   return true ;
  if(lund==115)   return true ;
  if(lund==221)   return true ;
  if(lund==223)   return true ;
  if(lund==225)   return true ;
  if(lund==331)   return true ;
  if(lund==333)   return true ;
  if(lund==335)   return true ;
  if(lund==10111) return true ;
  if(lund==10221) return true ;
  if(lund==10223) return true ;
  if(lund==10331) return true ;
  if(lund==20113) return true ;
  if(lund==20223) return true ;
  if(lund==20333) return true ;
  // Add more here as needed
  return false ;
}

Bool_t DRecoDsToEllNuPlots::isKPlus(Int_t lund)
{
  lund = abs(lund) ;
  if(lund==321) return true ;
  return false ;
}

Bool_t DRecoDsToEllNuPlots:: isK0(Int_t lund)
{
  lund = abs(lund) ;
  if(lund==311) return true ;
  if(lund==310) return true ;
  if(lund==130) return true ;
  return false ;
}

Bool_t DRecoDsToEllNuPlots::isKPlusStar(Int_t lund)
{
  lund = abs(lund) ;
  if(lund==323)    return true ;
  if(lund==325)    return true ;
  if(lund==10321)  return true ;
  if(lund==10323)  return true ;
  if(lund==20313)  return true ;
  if(lund==20323)  return true ;
  if(lund==100323) return true ;
  return false ;
}

Bool_t DRecoDsToEllNuPlots::isK0Star(Int_t lund)
{
  lund = abs(lund) ;
  if(lund==313)    return true ;
  if(lund==315)    return true ;
  if(lund==10311)  return true ;
  if(lund==10313)  return true ;
  if(lund==20313)  return true ;
  if(lund==100313) return true ;
  return false ;
}

Bool_t DRecoDsToEllNuPlots::isDStar(Int_t lund)
{
  lund = abs(lund) ;
  if(lund==10411) return true ;
  if(lund==10421) return true ;
  if(lund==413)   return true ;
  if(lund==423)   return true ;
  if(lund==10413) return true ;
  if(lund==10423) return true ;
  if(lund==20413) return true ;
  if(lund==20423) return true ;
  if(lund==415)   return true ;
  if(lund==425)   return true ;
  if(lund==10431) return true ;
  if(lund==433)   return true ;
  if(lund==10433) return true ;
  if(lund==20433) return true ;
  if(lund==435)   return true ;
  return false ;
}

Bool_t DRecoDsToEllNuPlots::isBaryon(Int_t lund)
{
  lund = abs(lund) ;
  if(lund>999 && lund<10000) return true ;
  return false ;
}

