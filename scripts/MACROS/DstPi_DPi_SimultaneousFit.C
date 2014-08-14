#include "../Globals.h"
#include "../AnalysisGlobals.h"
#include "../CLASSES/RooPolynomialFixEdgeNew.h"
#include "../CLASSES/Roo2BodyMassPhaseSpace.h"
#include "../CLASSES/RooExpPiecePolyPdf.h"
#include "../CLASSES/RooAddPdfN.h"
#include "../CLASSES/RooChi2VarIntegral.h"
#include "../CLASSES/RooChi2VarSimultaneous.h"
#include "../CLASSES/JResiduals.h"

#include "RooRealVar.h"
#include "RooBreitWigner.h"
#include "RooAddPdf.h"
#include "RooFormula.h"
#include "RooProdPdf.h"
#include "RooDataHist.h"
#include "RooPlot.h"
#include "RooGenericPdf.h"
#include "RooMinuit.h"
#include "RooFitResult.h"

#define NHistos 3

void FitSimultaneous_DstPi_DPi(Int_t ncpu=1){

  //Get The histograms
  TFile DstPiFile("DstarCharged/DstarPi/KpiAndK3pi/Ntuple.root","read");
  TFile DPiFile("DCharged/DChargedPi/KPiPi/Ntuple.root","read");
  gROOT->cd();

  TTree* DstPiFileTree=(TTree*)DstPiFile.Get("Ntuple");
  TH1F HD1Heli("HD1Heli","HD1Heli",80,2.22,2.62);
  DstPiFileTree->Draw("dstpimdm>>HD1Heli","abs(dstarhelicity)>.9");
  TH1F HDstPi("HDstPi","HDstPi",260,2.15,3.45);
  DstPiFileTree->Draw("dstpimdm>>HDstPi");

  TTree* DPiFileTree=(TTree*)DPiFile.Get("Ntuple");
  TH1F HDPi("HDPi","HDPi",280,2.03,3.43);
  DPiFileTree->Draw("xmdm>>HDPi");
  
  DstPiFile.Close();
  DPiFile.Close();
  
  
  
  RooRealVar massHeli("massHeli","m(D*#pi)",2.22,2.62);massHeli.setUnit("GeV");massHeli.setBins(HD1Heli.GetNbinsX());
  //////////Now Create the Signal Pdfs
  RooRealVar D1Mass("D1Mass","D1Mass",2.410,2.430);
  RooRealVar D1Width("D1Width","D1Width",.001,.1);
  RooBreitWigner D1SigPdf("D1SigPdf","D1SigPdf",massHeli,D1Mass,D1Width);

  //-----------------------------------------------------
  ////Pdf for Helicity histogram
  //-------------------------------------------------
  RooRealVar D1HeliYield("D1HeliYield","D1HeliYield",.0,.3);  
  RooRealVar HeliC3("HeliC3","HeliC3",-10.,10.);
  RooRealVar HeliC4("HeliC4","HeliC4",-10.,10.);
  RooRealVar HeliC("HeliC","HeliC",0.);
  RooFormula BkgHeliYield("BkgHeliYield","1-D1HeliYield",RooArgList(D1HeliYield));
  RooPolynomialFixEdgeNew BkgHeliPdf("BkgHeliPdf","BkgHeliPdf",massHeli,&HD1Heli,&BkgHeliYield,
				      HeliC3,HeliC4,HeliC,HeliC,HeliC,HeliC,HeliC,HeliC);
	
  RooAddPdf HeliTotPdf("HeliTotPdf","HeliTotPdf",D1SigPdf,BkgHeliPdf,D1HeliYield);
  RooDataHist HeliDataSet("HeliDataSet","HeliDataSet",RooArgList(massHeli),&HD1Heli,1);

  
//   HeliTotPdf.fitTo(HeliDataSet,"sm");  
//   RooPlot* plot=massHeli.frame();  
//   HeliDataSet.plotOn(plot);
//   HeliTotPdf.plotOn(plot);
//   plot->Draw();
  
  
  //-----------------------------------------------------
  ////Pdf for DstPi histogram
  //-----------------------------------------------------
  RooRealVar massDstPi("massDstPi","m(D*#pi)",2.15,3.45);massDstPi.setUnit("GeV");massDstPi.setBins(HDstPi.GetNbinsX());
  RooRealVar DstMass("DstMass","DstMass",DstarPDGMass);
  RooRealVar PiMass("PiMass","PiMass",PiPDGMass);
  Roo2BodyMassPhaseSpace DstPiPS("DstPiPS","DstPiPS",massDstPi,DstMass,PiMass);

  RooBreitWigner D1DstPiSigPdf("D1DstPiSigPdf","D1DstPiSigPdf",massDstPi,D1Mass,D1Width);
  RooProdPdf D1DstPiPdfPS("D1DstPiPdfPS","D1DstPiPdfPS",DstPiPS,D1DstPiSigPdf);  
  RooRealVar D1DstPiYield("D1DstPiYield","D1DstPiYield",0.,.2);

  RooRealVar D2Mass("D2Mass","D2Mass",2.450,2.470);
  RooRealVar D2Width("D2Width","D2Width",.001,.1);
  RooBreitWigner D2DstPiSigPdf("D2DstPiSigPdf","D2DstPiSigPdf",massDstPi,D2Mass,D2Width);
  RooProdPdf D2DstPiPdfPS("D2DstPiPdfPS","D2DstPiPdfPS",DstPiPS,D2DstPiSigPdf);  
  RooRealVar D2DstPiYield("D2DstPiYield","D2DstPiYield",0.,.1);

  RooRealVar D3Mass("D3Mass","D3Mass",2.55,2.67);
  RooRealVar D3Width("D3Width","D3Width",.001,.3);
  RooBreitWigner D3DstPiSigPdf("D3DstPiSigPdf","D3DstPiSigPdf",massDstPi,D3Mass,D3Width);
  RooProdPdf D3DstPiPdfPS("D3DstPiPdfPS","D3DstPiPdfPS",DstPiPS,D3DstPiSigPdf);  
  RooRealVar D3DstPiYield("D3DstPiYield","D3DstPiYield",0.,.3);

  RooRealVar D4Mass("D4Mass","D4Mass",2.7,2.8);
  RooRealVar D4Width("D4Width","D4Width",.001,.3);
  RooBreitWigner D4DstPiSigPdf("D4DstPiSigPdf","D4DstPiSigPdf",massDstPi,D4Mass,D4Width);
  RooProdPdf D4DstPiPdfPS("D4DstPiPdfPS","D4DstPiPdfPS",DstPiPS,D4DstPiSigPdf);  
  RooRealVar D4DstPiYield("D4DstPiYield","D4DstPiYield",0.,.1);  
    
  RooRealVar DstPiC1("DstPiC1","DstPiC1",2.2,2.8);
  RooRealVar DstPiC2("DstPiC2","DstPiC2",0.,100);
  RooRealVar DstPiC3("DstPiC3","DstPiC3",-10.,0.);
  RooRealVar DstPiC4("DstPiC4","DstPiC4",-10.,10.);
  RooExpPiecePolyPdf BkgDstPiPdf("BkgDstPiPdf","BkgDstPiPdf",massDstPi,DstPiC1,DstPiC2,DstPiC3,DstPiC4);
  BkgDstPiPdf.RotateCoefs(-.2164);
  RooProdPdf BkgDstPiPdfPS("BkgDstPiPdfPS","BkgDstPiPdfPS",DstPiPS,BkgDstPiPdf);  


  RooAddPdf DstPiTotPdf("DstPiTotPdf","DstPiTotPdf",
			RooArgList(D1DstPiPdfPS,D2DstPiPdfPS,D3DstPiPdfPS,D4DstPiPdfPS,BkgDstPiPdfPS),
			RooArgList(D1DstPiYield,D2DstPiYield,D3DstPiYield,D4DstPiYield));   
  RooDataHist DstPiDataSet("DstPiDataSet","DstPiDataSet",RooArgList(massDstPi),&HDstPi,1);
   
  
  //RooChi2VarIntegral chi2("chi2Integral","chi2Integral",DstPiTotPdf,DstPiDataSet);
  //RooMinuit minuit(chi2);
  //minuit.migrad();
  //   RooPlot*plot=massDstPi.frame();  
  //   DstPiDataSet.plotOn(plot);
  //   DstPiTotPdf.plotOn(plot);
  //   //DstPiTotPdfPS.plotOn(plot,Components(BkgDstPiPdfPS));
  //   plot->Draw();
  
  


  //-----------------------------------------------------
  ////Pdf for D+pi- histogram
  //-----------------------------------------------------
  RooRealVar massDPi("massDPi","m(D#pi)",2.03,3.43);massDPi.setUnit("GeV");massDPi.setBins(HDPi.GetNbinsX());
  RooRealVar DMass("DMass","DMass",DPlusPDGMass);
  Roo2BodyMassPhaseSpace DPiPS("DPiPS","DPiPS",massDPi,DMass,PiMass);
  
  RooRealVar DPiFeedScale("DPiFeedScale","DPiFeedScale",0,1);

  RooFormulaVar DPiD1FeedMass("DPiD1FeedMass","D1Mass-.1434",RooArgList(D1Mass));
  RooFormulaVar DPiD1FeedWidth("DPiD1FeedWidth","D1Width*1.15",RooArgList(D1Width));//1.08
  RooBreitWigner D1FeedDPiSigPdf("D1FeedDPiSigPdf","D1FeedDPiSigPdf",massDPi,DPiD1FeedMass,DPiD1FeedWidth);
  RooProdPdf D1FeedDPiPdfPS("D1FeedDPiPdfPS","D1FeedDPiPdfPS",DPiPS,D1FeedDPiSigPdf);  
  RooFormulaVar  D1FeedDPiYield("D1FeedDPiYield","D1DstPiYield*DPiFeedScale",RooArgList(D1DstPiYield,DPiFeedScale));  

  RooFormulaVar DPiD2FeedMass("DPiD2FeedMass","D2Mass-.1434",RooArgList(D2Mass));
  RooFormulaVar DPiD2FeedWidth("DPiD2FeedWidth","D2Width*1.1",RooArgList(D2Width));//1.03
  RooBreitWigner D2FeedDPiSigPdf("D2FeedDPiSigPdf","D2FeedDPiSigPdf",massDPi,DPiD2FeedMass,DPiD2FeedWidth);
  RooProdPdf D2FeedDPiPdfPS("D2FeedDPiPdfPS","D2FeedDPiPdfPS",DPiPS,D2FeedDPiSigPdf);  
  RooFormulaVar  D2FeedDPiYield("D2FeedDPiYield","D2DstPiYield*DPiFeedScale",RooArgList(D2DstPiYield,DPiFeedScale));  

  RooFormulaVar DPiD3FeedMass("DPiD3FeedMass","D3Mass-.1434",RooArgList(D3Mass));
  RooFormulaVar DPiD3FeedWidth("DPiD3FeedWidth","D3Width*1.07",RooArgList(D3Width));//1.02
  RooBreitWigner D3FeedDPiSigPdf("D3FeedDPiSigPdf","D3FeedDPiSigPdf",massDPi,DPiD3FeedMass,DPiD3FeedWidth);
  RooProdPdf D3FeedDPiPdfPS("D3FeedDPiPdfPS","D3FeedDPiPdfPS",DPiPS,D3FeedDPiSigPdf);  
  RooFormulaVar  D3FeedDPiYield("D3FeedDPiYield","D3DstPiYield*DPiFeedScale",RooArgList(D3DstPiYield,DPiFeedScale));  

  RooFormulaVar DPiD4FeedMass("DPiD4FeedMass","D4Mass-.1434",RooArgList(D4Mass));
  RooFormulaVar DPiD4FeedWidth("DPiD4FeedWidth","D4Width*1.1",RooArgList(D4Width));//1.03
  RooBreitWigner D4FeedDPiSigPdf("D4FeedDPiSigPdf","D4FeedDPiSigPdf",massDPi,DPiD4FeedMass,DPiD4FeedWidth);
  RooProdPdf D4FeedDPiPdfPS("D4FeedDPiPdfPS","D4FeedDPiPdfPS",DPiPS,D4FeedDPiSigPdf);  
  RooFormulaVar  D4FeedDPiYield("D4FeedDPiYield","D4DstPiYield*DPiFeedScale",RooArgList(D4DstPiYield,DPiFeedScale));  

  RooBreitWigner D2DPiSigPdf("D2DPiSigPdf","D2DPiSigPdf",massDPi,D2Mass,D2Width);
  RooProdPdf D2DPiPdfPS("D2DPiPdfPS","D2DPiPdfPS",DPiPS,D2DPiSigPdf);  
  RooRealVar D2DPiYield("D2DPiYield","D2DPiYield",0.,.3);
  
  RooBreitWigner D3DPiSigPdf("D3DPiSigPdf","D3DPiSigPdf",massDPi,D3Mass,D3Width);
  RooProdPdf D3DPiPdfPS("D3DPiPdfPS","D3DPiPdfPS",DPiPS,D3DPiSigPdf);  
  RooRealVar D3DPiYield("D3DPiYield","D3DPiYield",0.,.1);
  
  RooBreitWigner D4DPiSigPdf("D4DPiSigPdf","D4DPiSigPdf",massDPi,D4Mass,D4Width);
  RooProdPdf D4DPiPdfPS("D4DPiPdfPS","D4DPiPdfPS",DPiPS,D4DPiSigPdf);  
  RooRealVar D4DPiYield("D4DPiYield","D4DPiYield",0.,.05);  
  
  RooRealVar D2400Mass("D2400Mass","D2400Mass",2.300);
  RooRealVar D2400Width("D2400Width","D2400Width",.273);
  RooBreitWigner D2400DPiSigPdf("D2400DPiSigPdf","D2400DPiSigPdf",massDPi,D2400Mass,D2400Width);
  RooProdPdf D2400DPiPdfPS("D2400DPiPdfPS","D2400DPiPdfPS",DPiPS,D2400DPiSigPdf);
  RooRealVar D2400DPiYield("D2400DPiYield","D2400DPiYield",0.,.3);  

  RooRealVar DPiC1("DPiC1","DPiC1",2.05,2.3);
  RooRealVar DPiC2("DPiC2","DPiC2",0.,100.);
  RooRealVar DPiC3("DPiC3","DPiC3",-10.,0.);
  RooRealVar DPiC4("DPiC4","DPiC4",-10,10.);
  RooExpPiecePolyPdf BkgDPiPdf("BkgDPiPdf","BkgDPiPdf",massDPi,DPiC1,DPiC2,DPiC3,DPiC4);
  BkgDPiPdf.RotateCoefs(-.2164);
  RooProdPdf BkgDPiPdfPS("BkgDPiPdfPS","BkgDPiPdfPS",DPiPS,BkgDPiPdf);  


  RooAddPdf DPiTotPdf("DPiTotPdf","DPiTotPdf",
		      RooArgList(D1FeedDPiPdfPS,D2FeedDPiPdfPS,D3FeedDPiPdfPS,D4FeedDPiPdfPS,D2DPiPdfPS,D3DPiPdfPS,D4DPiPdfPS,D2400DPiPdfPS,BkgDPiPdfPS),
		      RooArgList(D1FeedDPiYield,D2FeedDPiYield,D3FeedDPiYield,D4FeedDPiYield,D2DPiYield,D3DPiYield,D4DPiYield,D2400DPiYield));   
  RooDataHist DPiDataSet("DPiDataSet","DPiDataSet",RooArgList(massDPi),&HDPi,1);
   

//   RooChi2VarIntegral chi2("chi2Integral","chi2Integral",DPiTotPdf,DPiDataSet);
//   RooMinuit minuit(chi2);
//   minuit.migrad();
//   RooPlot*plot=massDPi.frame();  
//   DPiDataSet.plotOn(plot);
//   DPiTotPdf.plotOn(plot);
//   plot->Draw();


  


  //-----------------------------------------------------
  //Perform the simultaneous fit
  //-----------------------------------------------------
  RooAbsPdf* PdfArray[NHistos]={&HeliTotPdf,&DstPiTotPdf,&DPiTotPdf};
  TH1* HArray[NHistos]={&HD1Heli,&HDstPi,&HDPi};
  RooRealVar* VarArray[NHistos]={&massHeli,&massDstPi,&massDPi};

  RooRealVar dummyVar("dummyVar","dummyVar",0,1);
  Int_t NtotBins=0;
  for(Int_t h=0;h<NHistos;h++)
    NtotBins+=HArray[h]->GetXaxis()->GetNbins();
  TH1F Hdummy("Hdummy","Hdummy",NtotBins,0,1);
  RooDataHist dummyDataSet("dummyDataSet","dummyDataSet",RooArgList(dummyVar),&Hdummy,1);  
  TString dummyformula="dummyVar";
  RooArgList dummyvarlist;
  dummyvarlist.add(dummyVar);

  dummyvarlist.add(D1HeliYield);dummyformula+="+D1HeliYield";
  dummyvarlist.add(HeliC3);dummyformula+="+HeliC3";

  dummyvarlist.add(D1Mass);dummyvarlist.add(D1Width);dummyformula+="+D1Mass+D1Width";
  dummyvarlist.add(D2Mass);dummyvarlist.add(D2Width);dummyformula+="+D2Mass+D2Width";
  dummyvarlist.add(D3Mass);dummyvarlist.add(D3Width);dummyformula+="+D3Mass+D3Width";
  dummyvarlist.add(D4Mass);dummyvarlist.add(D4Width);dummyformula+="+D4Mass+D4Width";
  dummyvarlist.add(D1DstPiYield);dummyformula+="+D1DstPiYield";
  dummyvarlist.add(D2DstPiYield);dummyformula+="+D2DstPiYield";
  dummyvarlist.add(D3DstPiYield);dummyformula+="+D3DstPiYield";
  dummyvarlist.add(D4DstPiYield);dummyformula+="+D4DstPiYield";
  dummyvarlist.add(DstPiC1);dummyformula+="+DstPiC1";
  dummyvarlist.add(DstPiC2);dummyformula+="+DstPiC2";
  dummyvarlist.add(DstPiC3);dummyformula+="+DstPiC3";
  dummyvarlist.add(DstPiC4);dummyformula+="+DstPiC4";

  dummyvarlist.add(DPiFeedScale);dummyformula+="+DPiFeedScale";
  dummyvarlist.add(D2DPiYield);dummyformula+="+D2DPiYield";
  dummyvarlist.add(D3DPiYield);dummyformula+="+D3DPiYield";
  dummyvarlist.add(D4DPiYield);dummyformula+="+D4DPiYield";
  dummyvarlist.add(D2400DPiYield);dummyformula+="+D2400DPiYield";
  dummyvarlist.add(DPiC1);dummyformula+="+DPiC1";
  dummyvarlist.add(DPiC2);dummyformula+="+DPiC2";
  dummyvarlist.add(DPiC3);dummyformula+="+DPiC3";
  dummyvarlist.add(DPiC4);dummyformula+="+DPiC4";

  cout<<"the formula is "<<dummyformula<<endl;
  RooGenericPdf DummyPdf("DummyPdf","DummyPdf",dummyformula,dummyvarlist);
  
  RooChi2VarSimultaneous chi2("chi2","chi2",DummyPdf,dummyDataSet,HArray,PdfArray,VarArray,NHistos,RooFit::NumCPU(ncpu));
  RooMinuit minuit(chi2);
  minuit.simplex();
  minuit.migrad();
  RooFitResult*result=minuit.save();
  result->Print();  


  TCanvas C("C");
  TString psfile="SimultaneousFit.ps";
  C.Print(psfile+"[");
  //C.Clear();
  RooPlot* plotheli=massHeli.frame();  
  HeliDataSet.plotOn(plotheli);
  HeliTotPdf.plotOn(plotheli,Components(RooArgSet(D1SigPdf)),LineColor(3));
  HeliTotPdf.plotOn(plotheli,Components(RooArgSet(BkgHeliPdf)),LineColor(1));
  HeliTotPdf.plotOn(plotheli);
  //plotheli->Draw();
  //C.Print(psfile);
  TH1*HHeliFit=HeliTotPdf.createHistogram("HHeliFit",massHeli);
  HHeliFit->Scale(HD1Heli.Integral());
  JResiduals HeliResid("HeliResid",&HD1Heli,HHeliFit);
  C.Clear();
  plotheli->SetTitle("");
  HeliResid.MakeFitPlotWithResiduals(&C,plotheli);
  C.Print(psfile);
  delete plotheli;  


  //C.Clear();
  RooPlot*plotDstPi=massDstPi.frame();  
  DstPiDataSet.plotOn(plotDstPi);
  DstPiTotPdf.plotOn(plotDstPi,Components(RooArgSet(D1DstPiPdfPS)),LineColor(3));
  DstPiTotPdf.plotOn(plotDstPi,Components(RooArgSet(D2DstPiPdfPS)),LineColor(2));
  DstPiTotPdf.plotOn(plotDstPi,Components(RooArgSet(BkgDstPiPdfPS,D3DstPiPdfPS)),LineColor(3));
  DstPiTotPdf.plotOn(plotDstPi,Components(RooArgSet(BkgDstPiPdfPS,D4DstPiPdfPS)),LineColor(2));
  DstPiTotPdf.plotOn(plotDstPi,Components(RooArgSet(BkgDstPiPdfPS)),LineColor(1));
  DstPiTotPdf.plotOn(plotDstPi);
  //plotDstPi->Draw();
  //C.Print(psfile);
  TH1*HDstPiFit=DstPiTotPdf.createHistogram("HDstPiFit",massDstPi);
  HDstPiFit->Scale(HDstPi.Integral());
  JResiduals DstPiResid("DstPiResid",&HDstPi,HDstPiFit);
  C.Clear();
  plotDstPi->SetTitle("");
  DstPiResid.MakeFitPlotWithResiduals(&C,plotDstPi);
  C.Print(psfile);
  delete plotDstPi;


  //C.Clear();
  RooPlot*plotDPi=massDPi.frame();  
  DPiDataSet.plotOn(plotDPi);
  DPiTotPdf.plotOn(plotDPi,Components(RooArgSet(BkgDPiPdfPS,D1FeedDPiPdfPS,D2FeedDPiPdfPS,D3FeedDPiPdfPS,D4FeedDPiPdfPS)),LineColor(1));
  DPiTotPdf.plotOn(plotDPi,Components(RooArgSet(D2400DPiPdfPS)),LineColor(3));
  DPiTotPdf.plotOn(plotDPi,Components(RooArgSet(D2DPiPdfPS)),LineColor(2));
  DPiTotPdf.plotOn(plotDPi,Components(RooArgSet(BkgDPiPdfPS,D3DPiPdfPS)),LineColor(3));
  DPiTotPdf.plotOn(plotDPi,Components(RooArgSet(BkgDPiPdfPS,D4DPiPdfPS)),LineColor(2));
  DPiTotPdf.plotOn(plotDPi,Components(RooArgSet(BkgDPiPdfPS)),LineColor(1));
  DPiTotPdf.plotOn(plotDPi);
  //plotDPi->Draw();  
  //C.Print(psfile);
  TH1*HDPiFit=DPiTotPdf.createHistogram("HDPiFit",massDPi);
  HDPiFit->Scale(HDPi.Integral());
  JResiduals DPiResid("DPiResid",&HDPi,HDPiFit);
  C.Clear();
  plotDPi->SetTitle("");
  DPiResid.MakeFitPlotWithResiduals(&C,plotDPi);
  C.Print(psfile);
  delete plotDPi;

  C.Print(psfile+"]");
  
  ///Print out the parameters 
  ofstream ParameterOutput;
  ParameterOutput.open((const char*)("SimultaneousFitPars.txt"));
  ParameterOutput<<"HD1Heli"<<" "<<"xmin"<<" "<<HD1Heli.GetXaxis()->GetXmin()<<endl;
  ParameterOutput<<"HD1Heli"<<" "<<"xmax"<<" "<<HD1Heli.GetXaxis()->GetXmax()<<endl;
  ParameterOutput<<"HD1Heli"<<" "<<"integral"<<" "<<HD1Heli.Integral()<<endl;
  ParameterOutput<<"HDstPi"<<" "<<"xmin"<<" "<<HDstPi.GetXaxis()->GetXmin()<<endl;
  ParameterOutput<<"HDstPi"<<" "<<"xmax"<<" "<<HDstPi.GetXaxis()->GetXmax()<<endl;
  ParameterOutput<<"HDstPi"<<" "<<"integral"<<" "<<HDstPi.Integral()<<endl;
  ParameterOutput<<"HDPi"<<" "<<"xmin"<<" "<<HDPi.GetXaxis()->GetXmin()<<endl;
  ParameterOutput<<"HDPi"<<" "<<"xmax"<<" "<<HDPi.GetXaxis()->GetXmax()<<endl;
  ParameterOutput<<"HDPi"<<" "<<"integral"<<" "<<HDPi.Integral()<<endl;
  ParameterOutput<<"Range"<<" "<<"status"<<" "<<result->status()<<endl;
  ParameterOutput<<"Range"<<" "<<"covqual"<<" "<<result->covQual()<<endl;
  ParameterOutput<<"HD1Heli"<<" "<<"chisquare"<<" "<<HeliResid.GetChisquare()<<endl;
  ParameterOutput<<"HDstPi"<<" "<<"chisquare"<<" "<<DstPiResid.GetChisquare()<<endl;
  ParameterOutput<<"HDPi"<<" "<<"chisquare"<<" "<<DPiResid.GetChisquare()<<endl;

  Int_t i=0;
  while(RooRealVar*par=(RooRealVar*)dummyvarlist.at(++i)){
    ParameterOutput<<"Parameter"<<" "<<par->GetName()<<" "<<par->getVal()<<endl;    
    ParameterOutput<<"Parameter"<<" "<<par->GetName()<<"error "<<par->getError()<<endl;    
  }



  ParameterOutput<<"END"<<endl;
  ParameterOutput.close();
  ParameterOutput.clear();

  return ;
}
