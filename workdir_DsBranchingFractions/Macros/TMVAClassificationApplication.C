/**********************************************************************************
 * Project   : TMVA - a Root-integrated toolkit for multivariate data analysis    *
 * Package   : TMVA                                                               *
 * Exectuable: TMVAClassificationApplication                                      *
 *                                                                                *
 * This macro provides a simple example on how to use the trained classifiers     *
 * within an analysis module                                                      *
 **********************************************************************************/

#include <cstdlib>
#include <vector>
#include <iostream>
#include <map>
#include <string>

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TPluginManager.h"
#include "TStopwatch.h"

#include "TMVA/MethodCuts.h"
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"

void TMVAClassificationApplication(){   

  TMVA::Reader reader("!Color:!Silent");    

  // create a set of variables and declare them to the reader
  // - the variable names must corresponds in name and type to 
  // those given in the weight file(s) that you use
  Float_t dsmass;
  Float_t dspstar;
  Float_t gamenergy;
  Float_t gamanglelab;
  Float_t gamenergycm;
  Float_t gamangle;
  Float_t gamKangle;
  Float_t dsstdecayangle;
  Float_t dsangle;
  Float_t Kangle;
  Float_t Kpstar;
  Float_t fragpstar;
  Float_t fragenergy;
  Float_t fragangle;
  Float_t tagsidedeltae;
  Float_t evtpstar;
  Float_t evtcosstar;
  Float_t evtvtxprob;
  Int_t evthassignal;

  reader.AddVariable( "dspstar", &dspstar );
  reader.AddVariable( "dsangle", &dsangle );
  reader.AddVariable( "Kangle", &Kangle );
  reader.AddVariable( "Kpstar", &Kpstar );
  reader.AddVariable( "fragpstar", &fragpstar );
  reader.AddVariable( "fragenergy", &fragenergy );
  reader.AddVariable( "fragangle", &fragangle );
  reader.AddVariable( "tagsidedeltae", &tagsidedeltae );
  reader.AddVariable( "gamenergy", &gamenergy );
  reader.AddVariable( "gamanglelab", &gamanglelab );
  reader.AddVariable( "evtpstar",&evtpstar);
  reader.AddVariable( "evtcosstar",&evtcosstar);
  reader.AddVariable( "evtvtxprob",&evtvtxprob);

  cout<<" reader Added variables"<<endl;

  //
  // book the MVA methods
  //
  reader.BookMVA("BDT method","weights/TMVAClassification_BDT.weights.txt"); 

  // book output histograms
  UInt_t nbin = 100;
  TH1F *histBdt= new TH1F( "MVA_BDT","MVA_BDT",nbin, -0.8, 0.8 );
  TH1F *histDsMass= new TH1F( "DsMass","DsMass",50, 1.82, 2.12 );
  TH1F *histDsMassBkg= new TH1F( "DsMassBkg","DsMassBkg",50, 1.82, 2.12 );
   
  cout<<" booked histograms"<<endl;
 

  // Prepare input tree (this must be replaced by your data source)
  TFile *input(0);
  TString fname = "DsTolnu/SP1005/DsReco/Ntuple.root";   
  input = TFile::Open( fname ); // check if file in local directory exists
  if (!input){
    std::cout << "ERROR: could not open data file" << std::endl;
    exit(1);
  }
  std::cout << "--- TMVAClassificationApp    : Using input file: " << input->GetName() << std::endl;
  input->ls();

  //
  // prepare the tree
  TTree* theTree = (TTree*)input->Get("Ntuple");
  std::cout << "--- Select signal sample" << std::endl;
  theTree->SetBranchAddress( "dsmass", &dsmass );
  theTree->SetBranchAddress( "dspstar", &dspstar );
  theTree->SetBranchAddress( "gamenergy", &gamenergy );
  theTree->SetBranchAddress( "gamanglelab", &gamanglelab );
  theTree->SetBranchAddress( "dsangle", &dsangle );
  theTree->SetBranchAddress( "Kangle", &Kangle );
  theTree->SetBranchAddress( "Kpstar", &Kpstar );
  theTree->SetBranchAddress( "fragpstar", &fragpstar );
  theTree->SetBranchAddress( "fragenergy", &fragenergy );
  theTree->SetBranchAddress( "fragangle", &fragangle );
  theTree->SetBranchAddress( "tagsidedeltae", &tagsidedeltae);
  theTree->SetBranchAddress( "evtpstar", &evtpstar );
  theTree->SetBranchAddress( "evtcosstar", &evtcosstar );
  theTree->SetBranchAddress( "evtvtxprob", &evtvtxprob );
  theTree->SetBranchAddress( "evthassignal", &evthassignal);

  std::cout << "--- Processing: " << theTree->GetEntries() << " events" << std::endl;
  TStopwatch sw;
  sw.Start();
  for (Long64_t ievt=0; ievt<theTree->GetEntries();ievt++) {

    if (ievt%1000 == 0){
      std::cout << "--- ... Processing event: " << ievt << std::endl;
    }

    theTree->GetEntry(ievt);
    Float_t BDTOutPut=reader.EvaluateMVA("BDT method");
    histBdt->Fill(BDTOutPut); 
      
    if(BDTOutPut<-.19)continue;
    histDsMass->Fill(dsmass);
    if(!evthassignal)histDsMassBkg->Fill(dsmass);
  }
  // get elapsed time
  sw.Stop();
  std::cout << "--- End of event loop: "; sw.Print();


  //
  // write histograms
  //
  TFile *target  = new TFile("TMVApp.root","RECREATE" );
  histBdt ->Write();   
  histDsMass ->Write();
  histDsMassBkg ->Write();

  target->Close();

  std::cout << "--- Created root file: \"TMVApp.root\" containing the MVA output histograms" << std::endl;
  
      
  std::cout << "==> TMVAClassificationApplication is done!" << endl << std::endl;
} 
