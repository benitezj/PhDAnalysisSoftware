// @(#)root/tmva $Id: TMVAClassification.C,v 1.36 2009-04-14 13:08:13 andreas.hoecker Exp $
/**********************************************************************************
 **********************************************************************************/

#include <cstdlib>
#include <iostream> 
#include <map>
#include <string>

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TPluginManager.h"

#include "TCanvas.h"
#include "TF1.h"
#include "TLine.h"

//#include "TMVAGui.C"

#if not defined(__CINT__) || defined(__MAKECINT__)
// needs to be included when makecint runs (ACLIC)
#include "TMVA/Factory.h"
#include "TMVA/Tools.h"
#endif

// read input data file with ascii format (otherwise ROOT) ?
Bool_t ReadDataFromAsciiIFormat = kFALSE;
   
void TMVAClassification(Int_t DataType){

  // this loads the library
  //TMVA::Tools::Instance();

  //---------------------------------------------------------------
  // default MVA methods to be trained + tested
  std::map<std::string,int> Use;

  // Use["Cuts"]            = 1;
  // Use["Likelihood"]      = 1;
  // Use["Fisher"]          = 1;
  Use["BDT"]             = 1;
  // ---------------------------------------------------------------




  // Create a new root output file.
  TString outFileName;
  if(DataType==1)outFileName="DsTolnu/SP1005/DsReco/Final/TMVA.root";
  if(DataType==2)outFileName="DsTolnu/Data/Final/TMVA.root";
  TFile outputFile(outFileName,"RECREATE" );
  TMVA::Factory factory("TMVAClassification",&outputFile, 
			 "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D" );

  // Define the input variables that shall be used for the MVA training
  //factory.AddVariable( "dsmass","dsmass", "", 'F' );

  //variables physics motivated
  factory.AddVariable( "dspstar", 'F' );  
  factory.AddVariable( "dsangle", 'F' );  
  factory.AddVariable( "Kangle",'F' );
  factory.AddVariable( "Kpstar",'F' );
  factory.AddVariable( "fragenergy", 'F' );
  factory.AddVariable( "fragangle", 'F' );
  factory.AddVariable( "tagsidedeltae", 'F' ); 
  factory.AddVariable( "evtvtxprob",  'F' );


  //factory.AddVariable( "fragpstar", 'F' );//no separation

  //factory.AddVariable( "gamenergy",'F' );//can enhance the peaking bkg
  //factory.AddVariable( "gamanglelab",'F' );//no separation 

  //factory.AddVariable( "evtpstar", 'F' );//dont have motivation for this one
  //factory.AddVariable( "evtcosstar",'F' );//dont have motivation for this one

  /////other variables that are probably 100% correleated
  //factory.AddVariable( "fragmass", 'F' );  
  //factory.AddVariable( "Kp", 'F' );

  ////Unwanted variables
  //factory.AddVariable( "gamangle",'F' );//biases bkg
  //factory.AddVariable( "gamenergycm",'F' );//enhances peaking bkg, 91% correlated with gamenergycm
  //factory.AddVariable( "dsstdecayangle", 'F' );//biases bkg
  //factory.AddVariable( "gamKangle",  'F' );//enhances peaking bkg,

 //  // You can add so-called "Spectator variables", which are not used in the MVA training, 
//   // but will appear in the final "TestTree" produced by TMVA. This TestTree will contain the 
//   // input variables, the response values of all trained MVAs, and the spectator variables
//   factory.AddSpectator( "dsmass:=dsmass",  "", "dsmass", 'F' );
//   factory.AddSpectator( "gammctrue:=gammctrue",  "gammctrue", "", 'F' );
//   factory.AddSpectator( "evthassignal:=evthassignal",  "evthassignal", "", 'F' );
//   factory.AddSpectator( "evthasD0gam:=evthasD0gam", "evthasD0gam","",'F');

  // load the signal and background event samples from ROOT trees
  TString inputFileName;
  if(DataType==1)inputFileName="DsTolnu/SP1005/DsReco/Final/TMVATrainingData.root";
  if(DataType==2)inputFileName="DsTolnu/Data/Final/TMVATrainingData.root";
  TFile input(inputFileName,"read" ); 
  std::cout << "--- TMVAClassification       : Using input file: " << input.GetName() << std::endl;

  TTree *signal     = (TTree*)input.Get("TreeS");
  TTree *background = (TTree*)input.Get("TreeB");
  cout<<"Signal Entries="<<signal->GetEntries()<<"  Bkg Entries="<<background->GetEntries()<<endl;

  // ====== register trees ====================================================
  factory.AddSignalTree    ( signal, 1.);
  factory.AddBackgroundTree( background, 1. ); 
 
  // This would set individual event weights (the variables defined in the 
  //factory.SetBackgroundWeightExpression("weight");
  if(DataType==2){
    factory.SetSignalWeightExpression("evtmcdatacorrweight");//for data training correct the signal p* distribution
    cout<<" --->>>> p* Correction to Signal sample will be applied"<<endl;
  }

  // tell the factory to use all remaining events in the trees after training for testing:
  factory.PrepareTrainingAndTestTree("","","nTrain_Signal=0:nTrain_Background=0:SplitMode=Random:NormMode=NumEvents:!V" );

  // Cut optimisation
  if (Use["Cuts"])
    factory.BookMethod( TMVA::Types::kCuts, "Cuts", 
			"!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart" );
  // Likelihood
  if (Use["Likelihood"])
    factory.BookMethod( TMVA::Types::kLikelihood, "Likelihood", 
			"H:!V:TransformOutput:PDFInterpol=Spline2:NSmoothSig[0]=20:NSmoothBkg[0]=20:NSmoothBkg[1]=10:NSmooth=1:NAvEvtPerBin=50" ); 

  // Fisher discriminant   
  if (Use["Fisher"])
    factory.BookMethod( TMVA::Types::kFisher, "Fisher", "H:!V:Fisher:CreateMVAPdfs:NbinsMVAPdf=60:NsmoothMVAPdf=10" );//PDFInterpolMVAPdf=Spline2

  // Adaptive Boost
  if (Use["BDT"])  
    factory.BookMethod( TMVA::Types::kBDT, "BDT", 
			"!H:!V:NTrees=400:nEventsMin=400:MaxDepth=3:BoostType=AdaBoost:SeparationType=GiniIndex:nCuts=20:PruneMethod=NoPruning" );
   

  // ---- Now you can tell the factory to train, test, and evaluate the MVAs

  // Train MVAs using the set of training events
  //factory.TrainAllMethodsForClassification();
  factory.TrainAllMethods();

  // ---- Evaluate all MVAs using the set of test events
  factory.TestAllMethods();

  // ----- Evaluate and compare performance of all configured MVAs
  factory.EvaluateAllMethods();    

  // --------------------------------------------------------------
     

  // Save the output
  outputFile.ls();
  outputFile.Close();

  std::cout << "==> Wrote root file: " << outputFile.GetName() << std::endl;
  std::cout << "==> TMVAClassification is done!" << std::endl;      

}

void plotTMVAClassification(Int_t DataType){

  TString outputFileName;
  if(DataType==1)outputFileName="DsTolnu/SP1005/DsReco/Final/TMVA.root";
  if(DataType==2)outputFileName="DsTolnu/Data/Final/TMVA.root";
  TFile outputFile(outputFileName,"read");

  //create a ps file with the background and signal response
  TH1F*HSResponse=(TH1F*)outputFile.Get("Method_BDT/BDT/MVA_BDT_S");
  TH1F*HBResponse=(TH1F*)outputFile.Get("Method_BDT/BDT/MVA_BDT_B");
  TH1F*HSEff=(TH1F*)outputFile.Get("Method_BDT/BDT/MVA_BDT_effS");
  TH1F*HBEff=(TH1F*)outputFile.Get("Method_BDT/BDT/MVA_BDT_effB");
  TH1F*MVA_BDT_rejBvsS=(TH1F*)outputFile.Get("Method_BDT/BDT/MVA_BDT_rejBvsS");

  TCanvas C("C");
  C.Clear();
  TString outPsFile;
  if(DataType==1)outPsFile="DsTolnu/SP1005/DsReco/Final/TMVA.ps";
  if(DataType==2)outPsFile="DsTolnu/Data/Final/TMVA.ps";
  C.Print(outPsFile+"[");
  
  C.Clear();
  HSResponse->SetStats(0);
  HSResponse->GetXaxis()->SetTitle("BDT output");
  HSResponse->SetTitle("");
  HSResponse->SetLineColor(2);
  HSResponse->SetTitle("BDT Response: signal=red  bkg=black");
  HSResponse->GetYaxis()->SetRangeUser(0,(HSResponse->GetMaximum()>HBResponse->GetMaximum())?HSResponse->GetMaximum():HBResponse->GetMaximum());
  HSResponse->Draw("hist");
  HBResponse->Draw("hist same");
  C.Print(outPsFile);
  
  C.Clear();
  C.SetGrid(1,1);
  HSEff->SetStats(0);
  HSEff->GetXaxis()->SetTitle("BDT output");
  HSEff->GetYaxis()->SetTitle("Efficiency");
  HSEff->SetTitle("");
  HSEff->SetLineColor(2);
  HSEff->Draw("l");
  HBEff->Draw("l same");
  C.Print(outPsFile);

  C.Clear();
  C.SetGrid(1,1);
  MVA_BDT_rejBvsS->GetYaxis()->SetRangeUser(0,1);
  MVA_BDT_rejBvsS->GetXaxis()->SetRangeUser(0,1);
  MVA_BDT_rejBvsS->SetTitle("");
  MVA_BDT_rejBvsS->SetStats(0);
  MVA_BDT_rejBvsS->Draw("l");
  C.Print(outPsFile);


  C.Print(outPsFile+"]");

}


void CreateTrainingData(Int_t DataType,Int_t signalDefinition,Int_t BkgDefinition){

  TString inputBkgFile;
  TString inputSignalFile="DsTolnu/SP1005/DsReco/Ntuple.root";
  TString outputFile;
  if(DataType==1){//for MC
    if(BkgDefinition==1||BkgDefinition==2||BkgDefinition==3)
      inputBkgFile="DsTolnu/SP1005/DsReco/Ntuple.root";
    if(BkgDefinition==4)
      inputBkgFile="DsTolnu/SP1005/DsReco/SideBand/Ntuple.root";    
     outputFile="DsTolnu/SP1005/DsReco/Final/TMVATrainingData.root";
  }
  if(DataType==2){//for Data
    if(BkgDefinition==2)
      inputBkgFile="DsTolnu/SP1005/DsReco/Ntuple.root";
    if(BkgDefinition==3)
      inputBkgFile="DsTolnu/Data/Ntuple.root";
    if(BkgDefinition==4)
      inputBkgFile="DsTolnu/Data/Sideband/Ntuple.root";   
    outputFile="DsTolnu/Data/Final/TMVATrainingData.root";
  }

  TFile inBkgFile(inputBkgFile,"read");
  TFile inSignalFile(inputSignalFile,"read"); 
  TFile OutFile(outputFile,"recreate");  
  TTree*treeBkgIn=(TTree*)inBkgFile.Get("Ntuple");
  TTree*treeSigIn=(TTree*)inSignalFile.Get("Ntuple");
  if(!treeBkgIn||!treeSigIn){cout<<"no input trees "<<endl;exit(0);}

  cout<<"Input files:  Signal="<<inputSignalFile<<"  -->"<<treeSigIn->GetEntries()<<" entries"<<endl;
  cout<<"Input files:  Bkg="<<inputBkgFile<<"  -->"<<treeBkgIn->GetEntries()<<" entries"<<endl;

  
  ////define signal sample
  TTree*treeSigOut=NULL;
  if(signalDefinition==1)treeSigOut=treeSigIn->CopyTree("gammctrue>0");//use signal peak 
  if(signalDefinition==2)treeSigOut=treeSigIn->CopyTree("evthassignal");//use full singal MC including tails
  if(!treeSigOut){cout<<"Wrong Signal definition"<<endl; exit(0);}
  
  //define the bkg sample
  TTree*treeBkgOut=NULL;
  if(DataType==1){
    if(BkgDefinition==1)treeBkgOut=treeBkgIn->CopyTree("gammctrue<0");//everything that is not in peak
    if(BkgDefinition==2)treeBkgOut=treeBkgIn->CopyTree("!evthassignal");//non signal events
    if(BkgDefinition==3)treeBkgOut=treeBkgIn->CopyTree("(1.875<dsmass&&dsmass<1.925)||(2.025<dsmass&&dsmass<2.075)");//sideband
    if(BkgDefinition==4)treeBkgOut=treeBkgIn->CopyTree("1.6<dsmass&&dsmass<1.8");//sideband
  }
  if(DataType==2){
    if(BkgDefinition==2)treeBkgOut=treeBkgIn->CopyTree("!evthassignal");//true bkg in MC
    if(BkgDefinition==3)treeBkgOut=treeBkgIn->CopyTree("(1.875<dsmass&&dsmass<1.925)||(2.025<dsmass&&dsmass<2.075)");//sides 
    if(BkgDefinition==4)treeBkgOut=treeBkgIn->CopyTree("1.6<dsmass&&dsmass<1.8");//low sideband
  } 
  if(!treeBkgOut){cout<<"Wrong Bkg definition"<<endl; exit(0);}
  cout<<"Initial Output: Signal="<<treeSigOut->GetEntries()<<" Bkg="<<treeBkgOut->GetEntries()<<endl;


  //use same amount as signal  
  TTree*treeBkgOutFinal=NULL;
  TTree*treeSigOutFinal=NULL;
  //treeSigOut=treeSigOut->CopyTree("","",treeSigOut->GetEntries()/2,0);//test performance with half the entries
  if(treeSigOut->GetEntries()<treeBkgOut->GetEntries()){
    treeBkgOutFinal=treeBkgOut->CopyTree("","",treeSigOut->GetEntries(),0);
    treeSigOutFinal=treeSigOut;
  }else {
    treeSigOutFinal=treeSigOut->CopyTree("","",treeBkgOut->GetEntries(),0);
    treeBkgOutFinal=treeBkgOut;
  }
  cout<<"Final Output: Signal="<<treeSigOutFinal->GetEntries()<<" Bkg="<<treeBkgOutFinal->GetEntries()<<endl;
  
  treeSigOutFinal->SetName("TreeS");
  treeBkgOutFinal->SetName("TreeB");
  treeSigOutFinal->Write();
  treeBkgOutFinal->Write();
  
  OutFile.ls();
  OutFile.Close();
}


//Weight correction for signal MC due to wrong p* distribution
void createMCDataPstarSpectrum(){

  TFile NtupleFile("DsTolnu/SP1005/DsReco/Ntuple.root","read");
  TTree*t=(TTree*)NtupleFile.Get("Ntuple");  

  TFile NtupleFileData("DsTolnu/Data/Ntuple.root","read");
  TTree*tData=(TTree*)NtupleFileData.Get("Ntuple");  

  TH1F HDsMassVsPstarTot("HDsMassVsPstarTot","",50,1.82,2.12);
  TH1F HDsMassVsPstarDataTot("HDsMassVsPstarDataTot","",50,1.82,2.12);
  t->Draw("dsmass>>HDsMassVsPstarTot","gamenergy>.14");
  tData->Draw("dsmass>>HDsMassVsPstarDataTot","gamenergy>.14");


  TH1F* HDsMassVsPstar[10];
  TH1F* HDsMassVsPstarData[10];
  for(Int_t i=0;i<10;i++){
    //TString cut=TString("gamenergy>.14&&dspstar>")
    //+(long)(2.5+i*.25)+"."+(long)(100*(2.5+i*.25))+"&&dspstar<"+(long)(2.5+(i+1)*.25)+"."+(long)(100*(2.5+(i+1)*.25));
    TString cut=Form("gamenergy>.14&&dspstar>%1.2f&&dspstar<%1.2f",2.5+i*.25,2.5+(i+1)*.25);
    cout<<i<<" "<<cut<<endl;
    
    HDsMassVsPstar[i+1]=new TH1F(TString("HDsMassVsPstar")+(long)i,"",50,1.82,2.12);
    t->Draw(TString("dsmass>>HDsMassVsPstar")+(long)i,cut);   
    
    HDsMassVsPstarData[i+1]=new TH1F(TString("HDsMassVsPstarData")+(long)i,"",50,1.82,2.12);
    tData->Draw(TString("dsmass>>HDsMassVsPstarData")+(long)i,cut); 
  } 
  
  TFile OutFile("DsTolnu/SP1005/DsReco/MCWeightCorrection.root","recreate");
  HDsMassVsPstarTot.Write();
  HDsMassVsPstarDataTot.Write();
  for(Int_t i=0;i<10;i++){
    HDsMassVsPstar[i+1]->Write();
    HDsMassVsPstarData[i+1]->Write();
  }  
  OutFile.ls();
  OutFile.Write();
  OutFile.Close();
}
void fitMCDataPstarSpectrum(){
  TH1F* HDsMassVsPstar[10];
  TH1F* HDsMassVsPstarData[10];
  TFile OutFile("DsTolnu/SP1005/DsReco/MCWeightCorrection.root","update");
  OutFile.ls();
  gROOT->cd();
 
  TH1F HDataPstar("HDataPstar","",9,2.5,4.75);
  TH1F HMCPstar("HMCPstar","",9,2.5,4.75);
  TH1F HPstarRatio("HPstarRatio","",9,2.5,4.75);
  TF1 FDsMass("FDsMass","[0]*((1-[3])*exp(-.5*(x-[1])**2/[2]**2)/([2]*2.507)+[3]*exp(-.5*(x-[1])**2/[4]**2)/([4]*2.507))+[5]+[6]*x+[7]*x*x",1.82,2.12);
  //  TF1 FDsMassSig("FDsMassSig","[0]*exp(-.5*(x-[1])**2/[2]**2)+[3]*exp(-.5*(x-[1])**2/[4]**2)",1.82,2.12);
  FDsMass.SetParLimits(0,1,100000);
  FDsMass.SetParLimits(3,.001,.9);
  FDsMass.SetParLimits(1,1.95,1.99);
  //FDsMass.SetParLimits(4,1.96,1.98);
  FDsMass.SetParLimits(2,.001,.04);
  FDsMass.SetParLimits(4,.001,.04);
  FDsMass.SetParLimits(5,-10000000,1000000);
  FDsMass.SetParLimits(6,-10000000,1000000);
  FDsMass.SetParLimits(7,-10000000,1000000);
 

  TCanvas C("C");
  C.Print("DsTolnu/SP1005/DsReco/MCWeightCorrection.ps[");

  C.Clear();
  TH1F* HDsMassVsPstarTot=(TH1F*)OutFile.Get("HDsMassVsPstarTot");
  HDsMassVsPstarTot->Fit(&FDsMass,"QI","",1.82,2.025);
  C.Print("DsTolnu/SP1005/DsReco/MCWeightCorrection.ps"); 
  FDsMass.FixParameter(1,FDsMass.GetParameter(1));
  FDsMass.FixParameter(2,FDsMass.GetParameter(2));
  FDsMass.FixParameter(3,FDsMass.GetParameter(3));
  FDsMass.FixParameter(4,FDsMass.GetParameter(4));
  C.Clear();
  C.Divide(3,3);
  for(Int_t i=0;i<9;i++){
    C.cd(i+1);
    HDsMassVsPstar[i+1]=(TH1F*)OutFile.Get(TString("HDsMassVsPstar")+(long)i);
    HDsMassVsPstar[i+1]->Fit(&FDsMass,"QI","",1.82,2.025);
//     FDsMassSig.SetParameters(FDsMass.GetParameter(0),
// 			     FDsMass.GetParameter(1),
// 			     FDsMass.GetParameter(2),
// 			     FDsMass.GetParameter(3),
// 			     FDsMass.GetParameter(4));
    HMCPstar.SetBinContent(i+1,FDsMass.GetParameter(0)/HDsMassVsPstarTot->GetBinWidth(1));
    
    HDsMassVsPstar[i+1]->Draw();

  }  
  C.Print("DsTolnu/SP1005/DsReco/MCWeightCorrection.ps"); 
 

  C.Clear();
  TH1F* HDsMassVsPstarDataTot=(TH1F*)OutFile.Get("HDsMassVsPstarDataTot");
  FDsMass.ReleaseParameter(1);FDsMass.ReleaseParameter(2);FDsMass.ReleaseParameter(3);FDsMass.ReleaseParameter(4);
  FDsMass.SetParLimits(3,.001,.9);
  FDsMass.SetParLimits(1,1.95,1.99);
  //FDsMass.SetParLimits(4,1.96,1.98);
  FDsMass.SetParLimits(2,.001,.04);
  FDsMass.SetParLimits(4,.001,.04);
  HDsMassVsPstarDataTot->Fit(&FDsMass,"QI","",1.82,2.025);
  C.Print("DsTolnu/SP1005/DsReco/MCWeightCorrection.ps"); 
  FDsMass.FixParameter(1,FDsMass.GetParameter(1));
  FDsMass.FixParameter(2,FDsMass.GetParameter(2));
  FDsMass.FixParameter(3,FDsMass.GetParameter(3));
  FDsMass.FixParameter(4,FDsMass.GetParameter(4));
  C.Clear();
  C.Divide(3,3);
  for(Int_t i=0;i<9;i++){
    C.cd(i+1);
    HDsMassVsPstarData[i+1]=(TH1F*)OutFile.Get(TString("HDsMassVsPstarData")+(long)i);
    HDsMassVsPstarData[i+1]->Fit(&FDsMass,"QI","",1.82,2.025);
//     FDsMassSig.SetParameters(FDsMass.GetParameter(0),
// 			     FDsMass.GetParameter(1),
// 			     FDsMass.GetParameter(2),
// 			     FDsMass.GetParameter(3),
// 			     FDsMass.GetParameter(4));
    HDataPstar.SetBinContent(i+1,FDsMass.GetParameter(0)/HDsMassVsPstarTot->GetBinWidth(1)); 
    HDsMassVsPstarData[i+1]->Draw("pe");
  }  
  C.Print("DsTolnu/SP1005/DsReco/MCWeightCorrection.ps"); 

  
  C.Clear();
  HMCPstar.SetLineColor(4);
  HMCPstar.SetMarkerColor(4);
  HMCPstar.Draw("hist");
  HDataPstar.Draw("hist same");
  C.Print("DsTolnu/SP1005/DsReco/MCWeightCorrection.ps");

  C.Clear();
  HDataPstar.Draw("hist");
  HMCPstar.Scale(HDataPstar.Integral()/(float)HMCPstar.Integral());
  HMCPstar.Draw("hist same");
  C.Print("DsTolnu/SP1005/DsReco/MCWeightCorrection.ps");
  
  HDataPstar.Divide(&HMCPstar);
  C.Clear();
  //HDataPstar.Scale(1./(float)HDataPstar.Integral());
  HDataPstar.SetStats(0);
  HDataPstar.Draw("pe");
  TLine line;
  line.DrawLine(HMCPstar.GetXaxis()->GetXmin(),1,HMCPstar.GetXaxis()->GetXmax(),1);
  C.Print("DsTolnu/SP1005/DsReco/MCWeightCorrection.ps");

  C.Print("DsTolnu/SP1005/DsReco/MCWeightCorrection.ps]");


  //save the correction histogram
  OutFile.cd();
  HDataPstar.SetName("HPstarCorrection");
  HDataPstar.Write();
  OutFile.Close();
}


void plotRejecVsEff(){//as a function of number of variables
  
  TCanvas C("C");
  C.Clear();
  TString outPsFile="DsTolnu/SP1005/DsReco/Final/TMVAPlots/RejectVsEff/TMVA.ps";
  C.Print(outPsFile+"[");  
  C.Clear();  
  C.SetGrid(1,1);
  for(Int_t i=2;i<=8;i++){
    TString outputFileName=TString("DsTolnu/SP1005/DsReco/Final/TMVAPlots/RejectVsEff/TMVA_")+(long)i+".root";
    TFile*outputFile=new TFile(outputFileName,"read");
    gROOT->cd();
    TH1F*MVA_BDT_rejBvsS=(TH1F*)outputFile->Get("Method_BDT/BDT/MVA_BDT_rejBvsS");
    MVA_BDT_rejBvsS->SetTitle("");
    MVA_BDT_rejBvsS->SetStats(0);
    MVA_BDT_rejBvsS->GetYaxis()->SetRangeUser(0,1);
    MVA_BDT_rejBvsS->GetXaxis()->SetRangeUser(0,1);
    if(i==2)MVA_BDT_rejBvsS->Draw("l");
    else MVA_BDT_rejBvsS->Draw("lsame");
  }
  C.Print(outPsFile);


  C.Print(outPsFile+"]");
}

void compareDsMassAfterBDT(Int_t DataType){

  TString beforeFileName;
  TString afterFileName;
  if(DataType==1){
    beforeFileName="DsTolnu/SP1005/DsReco/Ntuple.root";
    afterFileName="DsTolnu/SP1005/DsReco/Final/Ntuple.root";
  }
  if(DataType==2){
    beforeFileName="DsTolnu/Data/Ntuple.root";
    afterFileName="DsTolnu/Data/Final/Ntuple.root";
  }
  TFile beforeFile(beforeFileName,"read");
  TFile afterFile(afterFileName,"read");
  TTree*beforeTree=(TTree*)beforeFile.Get("Ntuple");
  TTree*afterTree=(TTree*)afterFile.Get("Ntuple");


  TCanvas C("C");
  C.Clear();
  TString outPsFile;
  if(DataType==1)outPsFile="DsTolnu/SP1005/DsReco/Final/TMVACompareDsMassAfterBDT.ps";
  if(DataType==2)outPsFile="DsTolnu/Data/Final/TMVACompareDsMassAfterBDT.ps";
  C.Print(outPsFile+"[");  
 
  TH1F HBeforeMass("HBeforeMass","",50,1.82,2.12);
  TH1F HAfterMass("HAfterMass","",50,1.82,2.12);
  beforeTree->Draw("dsmass>>HBeforeMass");
  afterTree->Draw("dsmass>>HAfterMass");


  TF1 FDsMass("FDsMass","[0]*((1-[3])*exp(-.5*(x-[1])**2/[2]**2)/([2]*2.507)+[3]*exp(-.5*(x-[1])**2/[4]**2)/([4]*2.507))+[5]+[6]*x+[7]*x*x",1.82,2.12);
  FDsMass.SetParLimits(0,1,100000);
  FDsMass.SetParLimits(1,1.95,1.99);//FDsMass.FixParameter(1,1.9708);
  FDsMass.SetParLimits(2,.001,.04);FDsMass.FixParameter(2,.01468);
  FDsMass.SetParLimits(3,.001,.9);FDsMass.FixParameter(3,.4258);
  FDsMass.SetParLimits(4,.001,.04);FDsMass.FixParameter(4,.00516);
  FDsMass.SetParLimits(5,-10000000,1000000);
  FDsMass.SetParLimits(6,-10000000,1000000);
  FDsMass.SetParLimits(7,-10000000,1000000);


  HBeforeMass.Fit(&FDsMass,"I","",1.83,2.036);
  Float_t BeforeYield=FDsMass.GetParameter(0);
  HAfterMass.Fit(&FDsMass,"I","",1.83,2.036);
  Float_t AfterYield=FDsMass.GetParameter(0);
  cout<<" Before="<<BeforeYield<<"  After="<<AfterYield<<"  ratio="<<AfterYield/(float)BeforeYield<<endl;
  C.Clear();  
  HBeforeMass.SetTitle(TString("Signal Retained=")+(long)(100*(AfterYield/(float)BeforeYield))+"%");
  HBeforeMass.Draw();
  HAfterMass.Draw("same");
  C.Print(outPsFile);

  //---------------------------------------
  //Compare with just the p* cut
  //---------------------------------------
  TH1F HBeforePstarMass("HBeforePstarMass","",50,1.82,2.12);
  TH1F HAfterPstarMass("HAfterPstarMass","",50,1.82,2.12);
  beforeTree->Draw("dsmass>>HBeforePstarMass");
  beforeTree->Draw("dsmass>>HAfterPstarMass","dspstar>3.");
  HBeforePstarMass.Fit(&FDsMass,"I","",1.83,2.036);
  Float_t BeforePstarYield=FDsMass.GetParameter(0);
  HAfterPstarMass.Fit(&FDsMass,"I","",1.83,2.036);
  Float_t AfterPstarYield=FDsMass.GetParameter(0);
  cout<<" BeforePstar="<<BeforePstarYield<<"  AfterPstar="<<AfterPstarYield<<"  ratio="<<AfterPstarYield/(float)BeforePstarYield<<endl;
  C.Clear();  
  HBeforePstarMass.SetTitle(TString("Signal Retained=")+(long)(100*(AfterPstarYield/(float)BeforePstarYield))+"%");
  HBeforePstarMass.Draw();
  HAfterPstarMass.Draw("same");
  C.Print(outPsFile);




  C.Print(outPsFile+"]");
}



void compareDsMassAfterBDT2(Int_t DataType){

  TString beforeFileName;
  TString afterFileName;
  if(DataType==1){
    beforeFileName="DsTolnu/SP1005/DsReco/Ntuple.root";
    afterFileName="DsTolnu/SP1005/DsReco/Final/Ntuple.root";
  }
  if(DataType==2){
    beforeFileName="DsTolnu/Data/Ntuple.root";
    afterFileName="DsTolnu/Data/Final/Ntuple.root";
  }
  TFile beforeFile(beforeFileName,"read");
  TFile afterFile(afterFileName,"read");
  TTree*beforeTree=(TTree*)beforeFile.Get("Ntuple");
  TTree*afterTree=(TTree*)afterFile.Get("Ntuple");


  TCanvas C("C");
  C.Clear();
  TString outPsFile;
  if(DataType==1)outPsFile="DsTolnu/SP1005/DsReco/Final/TMVACompareDsMassAfterBDT.ps";
  if(DataType==2)outPsFile="DsTolnu/Data/Final/TMVACompareDsMassAfterBDT.ps";
  C.Print(outPsFile+"[");  
 
  TH1F HBeforeMass("HBeforeMass","",50,1.82,2.12);
  TH1F HAfterMass("HAfterMass","",50,1.82,2.12);
  beforeTree->Draw("dsmass>>HBeforeMass");
  afterTree->Draw("dsmass>>HAfterMass");

  TH1F HBeforeMassFit("HBeforeMassFit","",50,1.82,2.12);
  TH1F HAfterMassFit("HAfterMassFit","",50,1.82,2.12);
  beforeTree->Draw("dsmass>>HBeforeMassFit","dsmass<1.928||2.012<dsmass");
  afterTree->Draw("dsmass>>HAfterMassFit","dsmass<1.928||2.012<dsmass");


  TF1 FDsMass("FDsMass","[0]+[1]*x+[2]*x*x",1.82,2.12);
  FDsMass.SetParLimits(0,-10000000,1000000);
  FDsMass.SetParLimits(1,-10000000,1000000);
  FDsMass.SetParLimits(2,-10000000,1000000);


  HBeforeMassFit.Fit(&FDsMass,"I","",1.82,2.036);
  TF1*FBDTB=(TF1*)FDsMass.Clone();
  Float_t BeforeYield=HBeforeMass.Integral(1,36)-FBDTB->Integral(1.82,2.036)/HBeforeMass.GetBinWidth(1);
  HAfterMassFit.Fit(&FDsMass,"I","",1.82,2.036);
  TF1*FBDTA=(TF1*)FDsMass.Clone();
  Float_t AfterYield=HAfterMass.Integral(1,36)-FBDTA->Integral(1.82,2.036)/HBeforeMass.GetBinWidth(1);
  cout<<" Before="<<BeforeYield<<"  After="<<AfterYield<<"  ratio="<<AfterYield/(float)BeforeYield<<endl;
  C.Clear();  
  HBeforeMass.SetTitle(TString("Signal Retained=")+(long)(100*(AfterYield/(float)BeforeYield))+"%");
  HBeforeMass.Draw();
  FBDTB->Draw("same");
  HAfterMass.Draw("same");
  FBDTA->Draw("same");
  C.Print(outPsFile);

  //---------------------------------------
  //Compare with just the p* cut
  //---------------------------------------
  TH1F HBeforePstarMass("HBeforePstarMass","",50,1.82,2.12);
  TH1F HAfterPstarMass("HAfterPstarMass","",50,1.82,2.12);
  beforeTree->Draw("dsmass>>HBeforePstarMass");
  beforeTree->Draw("dsmass>>HAfterPstarMass","dspstar>3.");
  TH1F HBeforePstarMassFit("HBeforePstarMassFit","",50,1.82,2.12);
  TH1F HAfterPstarMassFit("HAfterPstarMassFit","",50,1.82,2.12);
  beforeTree->Draw("dsmass>>HBeforePstarMassFit","dsmass<1.928||2.012<dsmass");
  beforeTree->Draw("dsmass>>HAfterPstarMassFit","dspstar>3.&&(dsmass<1.928||2.012<dsmass)");
  HBeforePstarMassFit.Fit(&FDsMass,"I","",1.82,2.036);
  TF1*FPstarB=(TF1*)FDsMass.Clone();
  Float_t BeforePstarYield=HBeforePstarMass.Integral(1,36)-FPstarB->Integral(1.82,2.036)/HBeforeMass.GetBinWidth(1);
  HAfterPstarMassFit.Fit(&FDsMass,"I","",1.82,2.036);
  TF1*FPstarA=(TF1*)FDsMass.Clone();
  Float_t AfterPstarYield=HAfterPstarMass.Integral(1,36)-FPstarA->Integral(1.82,2.036)/HBeforeMass.GetBinWidth(1);
  cout<<" BeforePstar="<<BeforePstarYield<<"  AfterPstar="<<AfterPstarYield<<"  ratio="<<AfterPstarYield/(float)BeforePstarYield<<endl;
  C.Clear();  
  HBeforePstarMass.SetTitle(TString("Signal Retained=")+(long)(100*(AfterPstarYield/(float)BeforePstarYield))+"%");
  HBeforePstarMass.Draw();
  FPstarB->Draw("same");
  HAfterPstarMass.Draw("same");
  FPstarA->Draw("same");
  C.Print(outPsFile);




  C.Print(outPsFile+"]");
}


