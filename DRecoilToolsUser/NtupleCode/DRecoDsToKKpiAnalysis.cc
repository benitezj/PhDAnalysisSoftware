#include "DRecoilToolsUser/NtupleCode/DRecoDsToKKpiAnalysis.hh"
#include "DRecoilToolsUser/NtupleCode/Fits.hh"

#include "TH2F.h"
#include "TBox.h"

ClassImp(DRecoDsToKKpiAnalysis);
DRecoDsToKKpiAnalysis::DRecoDsToKKpiAnalysis(Int_t DataType,TString OutputDir):
  DRecoDsInclusiveAnalysis(DataType,OutputDir),
  Blind(1)
{
  
  TFile MCYieldsFile("DsTolnu/KKpi/SP1005/HistosForFit.root","read");
  if(!MCYieldsFile.IsZombie()){
    for(Int_t d=0;d<3;d++){
      TH1F*HData=(TH1F*)MCYieldsFile.Get(TString("HKKpiGamMassSignal_Dalitz")+(long)d);
      if(!HData){cout<<TString("HKKpiGamMassSignal_Dalitz")+(long)d<<" No MC yields hitos found"<<endl;}
      mcyields[d]=HData->Integral();
    }
  }
		      
 cout<<"Done Initializing the DRecoDsToKKpiAnalysis analysis"<<endl;
}

DRecoDsToKKpiAnalysis::~DRecoDsToKKpiAnalysis(){ 
}

Int_t DRecoDsToKKpiAnalysis::ReduceNtuple(){
  cout<<"Going to reduce the ntuple"<<endl;

  if(_fChain==NULL){
    cout<<"No chain."<<endl;
    return 0;
  } 

  ///link the branches in the input chain
  SetDsToKKpiBranches();

  //the file must be created here so the 'new' histos and Tree is created in hard drive
  TFile OutFile(_OutputDir+"/Ntuple_New.root","recreate");//1=minimal compression
  if(OutFile.IsZombie()){cout<<"Failed to create file"<<endl;return 0;}

  //create new reduced ntuple
  TTree Ntuple("Ntuple","Ntuple",0);
  ReducedNtuple=&Ntuple;
  //Long64_t maxsize=10000000000;//10Gb
  ReducedNtuple->SetMaxTreeSize(900000000);//split files
  //Long64_t fautosave=11000000000;//11Gb !!//disable autosaving 
  ReducedNtuple->SetAutoSave(900000000);


  //create the branches in the Ntuple
  MakeDsToKKpiReducedNtupleBranches();
 
  

  cout<<"Total Entries in Chain="<<_fChain->GetEntries()<<endl;
  cout<<"Going to start the event loop"<<endl;
  //Counters
  eventid=0;
  Int_t ngoodthisevent=0;
  goodeventid=0;    
  Int_t TotalSaved=0;
  Int_t TotalRawEvtCands=0;
  while(_fChain->GetEntry(eventid++,0)>0){   
    if(eventid%1000==0){
      cout<<eventid<<" Events done.   With "<<TotalSaved<<" Candidates saved."<<endl;    
    }
    
    if(!FillDsToKKpiEventVars()){cout<<"FillDsToKKpiEventVars()=0"<<endl;continue;}           
    if(!DsToKKpiEvtPassesCuts())continue;

    if(nEvt>NMAXEVT){cout<<"nEvt>NMAXEVT nEvt="<<nEvt<<endl;nEvt=NMAXEVT;}//protect array size
    ngoodthisevent=0; 
    goodrecoid=0;
    for(EvtIdx=0;EvtIdx<nEvt;EvtIdx++){
      TotalRawEvtCands++;
      //
      if(!FillDsToKKpiVars()){cout<<"FillDsToKKpiVars()=0"<<endl;continue;}   
      if(!DsToKKpiPassesCuts())continue;

      //save
      ngoodthisevent++;
      goodrecoid++;
      TotalSaved++;
      ReducedNtuple->Fill();
   
    }// loop    
    if(ngoodthisevent>0)goodeventid++;

  }

  //print summary
  cout<<"--------Summary-------"<<endl;
  cout<<"Total events = "<<eventid<<endl;
  cout<<"Total raw candidates ="<<TotalRawEvtCands<<endl;
  cout<<"Total cands saved ="<<TotalSaved<<endl;


  ////Save the Ntuple and histograms
  cout<<"Going to save the ntuple and histograms"<<endl; 

  //---
  if(ReducedNtuple->AutoSave("Overwrite")){ cout<<"Ntuple written."<<endl;}
  else{ cout<<"Failed to write ntuple"<<endl;return 0;}


  cout<<"Objects in the current file"<<endl;
  OutFile.ls();

  OutFile.Close(); //this will delete all objects created inside the file if they were not written  
  cout<<OutFile.GetName()<<" has been closed."<<endl;


  return 1;
}


Bool_t  DRecoDsToKKpiAnalysis::FillDsToKKpiEventVars(){

  ///Event vars
  if(!FillDsEventVars()){cout<<"FillDsEventVars()=0"<<endl;return 0;}
  return 1;
}

Bool_t DRecoDsToKKpiAnalysis::DsToKKpiEvtPassesCuts(){
  Bool_t pass=1;
  if(!DsEvtPassesCuts())pass=0;
  //if(pass)cout<<"passDsEvt"<<endl;
  //for signal make sure Ds decays to KKPi
  if(_DataType==0)
    if(evthasKKPi!=1)pass=0;
  //if(pass)cout<<"pass evthasKKPi"<<endl;

  return pass;
}

Bool_t DRecoDsToKKpiAnalysis::FillDsToKKpiVars(){
  
  if(!FillDsInclusiveVars()){cout<<"FillDsInclusiveVars()=0 "<<endl;return 0;}

  kkpimass=EvtKKpiMass[EvtIdx];
  nkkpi=EvtnKKpi[EvtIdx];
  kkpideltam=EvtKKpiDeltaM[EvtIdx];
  kkpidsstmass=EvtKKpiDsStMass[EvtIdx];
  kkpipstar=EvtKKpiPstar[EvtIdx];
  kkpiangle=EvtKKpiAngle[EvtIdx];
  kkmass=EvtKKMass[EvtIdx];
  kpimass=EvtKpiMass[EvtIdx];
  kkpinextrak=EvtKKpiNExtraTrk[EvtIdx];
  kkpinextneu=EvtKKpiNExtraNeutral[EvtIdx];
  kkpiextenergy=EvtKKpiExtraNeutralE[EvtIdx];
  kkpidsmctrue=(evthasKKPi ? EvtKKpiDsMCIdx[EvtIdx] : -1);//apply check that it is KKpi and not KKpipi0 or KK3pi
  kkpimctrue=(kkpidsmctrue>0 ? EvtKKpiDsStMCIdx[EvtIdx] : -1);
  
  return 1;
}



Bool_t DRecoDsToKKpiAnalysis::DsToKKpiPassesCuts(){
  Bool_t pass=1;

  if(!DsPassesCuts())pass=0;
  //if(pass)cout<<"passDs"<<endl;
  //Denominator is defined only in narrow region ---->> Cannot apply this cut, it biases the KKpi background.
  //-->Will need to define the Denominator in full mass region and take a larger systematic.
  //if(dsmass<DenominatorDsMassMin || DenominatorDsMassMax<dsmass)pass=0;

  //must select the fit region only for event multiplicity weighting to work properly
  if(kkpidsstmass<KKpiDsStFitMin || KKpiDsStFitMax<kkpidsstmass)pass=0;
  //if(pass)cout<<"pass mass cut"<<endl;  

//for signal MC must remove non-truth-matched reconstructions for multiplicity weighting to work properly
//   if(_DataType==0)
//     if(kkpimctrue<=0)pass=0;
///-->Doesnt work eff. becomes to large this is because in Generic MC or Data we cannot truth-match

  
  //for Data blind signal region
  if(_DataType==2&&Blind)
     if(2.056<kkpidsstmass && kkpidsstmass<2.152)pass=0;

  return pass;
}


Int_t DRecoDsToKKpiAnalysis::SetDsToKKpiBranches(){
  if(_fChain==NULL){cout<<"No Chain"<<endl;return 0;} 

  if(_fChain->GetBranch("EvtKKpiMass"))_fChain->SetBranchAddress("EvtKKpiMass",EvtKKpiMass);
  if(_fChain->GetBranch("EvtnKKpi"))_fChain->SetBranchAddress("EvtnKKpi",EvtnKKpi);
  if(_fChain->GetBranch("EvtKKpiDeltaM"))_fChain->SetBranchAddress("EvtKKpiDeltaM",EvtKKpiDeltaM);
  if(_fChain->GetBranch("EvtKKpiDsStMass"))_fChain->SetBranchAddress("EvtKKpiDsStMass",EvtKKpiDsStMass);
  if(_fChain->GetBranch("EvtKKpiPstar"))_fChain->SetBranchAddress("EvtKKpiPstar",EvtKKpiPstar);
  if(_fChain->GetBranch("EvtKKpiAngle"))_fChain->SetBranchAddress("EvtKKpiAngle",EvtKKpiAngle);
  if(_fChain->GetBranch("EvtKKMass"))_fChain->SetBranchAddress("EvtKKMass",EvtKKMass);
  if(_fChain->GetBranch("EvtKpiMass"))_fChain->SetBranchAddress("EvtKpiMass",EvtKpiMass);
  if(_fChain->GetBranch("EvtKKpiNExtraTrk"))_fChain->SetBranchAddress("EvtKKpiNExtraTrk",EvtKKpiNExtraTrk);
  if(_fChain->GetBranch("EvtKKpiNExtraNeutral"))_fChain->SetBranchAddress("EvtKKpiNExtraNeutral",EvtKKpiNExtraNeutral);
  if(_fChain->GetBranch("EvtKKpiExtraNeutralE"))_fChain->SetBranchAddress("EvtKKpiExtraNeutralE",EvtKKpiExtraNeutralE);
  if(_fChain->GetBranch("EvtKKpiDsStMCIdx"))_fChain->SetBranchAddress("EvtKKpiDsStMCIdx",EvtKKpiDsStMCIdx);
  if(_fChain->GetBranch("EvtKKpiDsMCIdx"))_fChain->SetBranchAddress("EvtKKpiDsMCIdx",EvtKKpiDsMCIdx);

  cout<<"Done linking SetDsToKKpi Branches"<<endl;
 
  return SetDsInclusiveBranches();
}



Int_t DRecoDsToKKpiAnalysis::MakeDsToKKpiReducedNtupleBranches(){
 
  ReducedNtuple->Branch("kkpimass",&kkpimass,"kkpimass/F");
  ReducedNtuple->Branch("nkkpi",&nkkpi,"nkkpi/I");
  ReducedNtuple->Branch("kkpideltam",&kkpideltam,"kkpideltam/F");
  ReducedNtuple->Branch("kkpidsstmass",&kkpidsstmass,"kkpidsstmass/F");
  ReducedNtuple->Branch("kkpipstar",&kkpipstar,"kkpipstar/F");
  ReducedNtuple->Branch("kkpiangle",&kkpiangle,"kkpiangle/F");
  ReducedNtuple->Branch("kkmass",&kkmass,"kkmass/F");
  ReducedNtuple->Branch("kpimass",&kpimass,"kpimass/F");
  ReducedNtuple->Branch("kkpinextrak",&kkpinextrak,"kkpinextrak/I");
  ReducedNtuple->Branch("kkpinextneu",&kkpinextneu,"kkpinextneu/I");
  ReducedNtuple->Branch("kkpiextenergy",&kkpiextenergy,"kkpiextenergy/F");
  ReducedNtuple->Branch("kkpimctrue",&kkpimctrue,"kkpimctrue/I");
  ReducedNtuple->Branch("kkpidsmctrue",&kkpidsmctrue,"kkpidsmctrue/I");
  ReducedNtuple->Branch("KKpiGenMCIdx",&KKpiGenMCIdx,"KKpiGenMCIdx/I");

  cout<<"Done Making  DsToKKpi Reduced Ntuple Branches"<<endl;
  MakeDsInclusiveReducedNtupleBranches();

  return 1;
}



Int_t DRecoDsToKKpiAnalysis::DefaultPlots(){

  if(!OpenReducedNtuple())return 0;
 

  filename=_OutputDir+"/DefaultPlotsKKpi.ps";
  Canvas->Print(filename+"[");
  
  
  TH1F HKKpiMass("HKKpiMass","",70,1.90,2.04);
  ReducedNtuple->Draw("kkpimass>>HKKpiMass");
  HKKpiMass.GetXaxis()->SetTitle("m(KK#pi)  (GeV/c^{2})");
  HKKpiMass.GetYaxis()->SetTitle("Entries/2 MeV");
  TH1F HKKpiMassBkg("HKKpiMassBkg","",HKKpiMass.GetXaxis()->GetNbins(),
		    HKKpiMass.GetXaxis()->GetXmin(),HKKpiMass.GetXaxis()->GetXmax());
  HKKpiMassBkg.SetFillColor(1);
  Canvas->Clear();
  HKKpiMass.Draw("pe");
  if(_DataType==0||_DataType==1){
    ReducedNtuple->Draw("kkpimass>>HKKpiMassBkg","kkpimctrue<0","same");
  }
  Canvas->Print(filename);

  
  TH1F HKKpiDeltaM("HKKpiDeltaM","",40,.0,.240);
  ReducedNtuple->Draw("kkpideltam>>HKKpiDeltaM");
  HKKpiDeltaM.GetXaxis()->SetTitle("m(KK#pi#gamma)-m(KK#pi)  (GeV/c^{2})");
  HKKpiDeltaM.GetYaxis()->SetTitle("Entries/8 MeV");  
  TH1F HKKpiDeltaMBkg("HKKpiDeltaMBkg","",40,.0,.240);

  HKKpiDeltaMBkg.SetFillColor(1);  
  Canvas->Clear();
  HKKpiDeltaM.Draw("pe");
  if(_DataType==0||_DataType==1){
    ReducedNtuple->Draw("kkpideltam>>HKKpiDeltaMBkg","kkpimctrue<0","same");
  }
  Canvas->Print(filename);

  TH1F HKKpiGamMass("HKKpiGamMass","",NKKpiDsStFitBins,KKpiDsStFitMin,KKpiDsStFitMax);
  ReducedNtuple->Draw("kkpidsstmass>>HKKpiGamMass");
  HKKpiGamMass.GetXaxis()->SetTitle("m(KK#pi#gamma)  (GeV/c^{2})");
  HKKpiGamMass.GetYaxis()->SetTitle("Entries/8 MeV");
  TH1F HKKpiGamMassBkg("HKKpiGamMassBkg","",NKKpiDsStFitBins,KKpiDsStFitMin,KKpiDsStFitMax);
  HKKpiGamMassBkg.SetFillColor(1);  
  Canvas->Clear(); 
  HKKpiGamMass.Draw("pe"); 
  if(_DataType==0||_DataType==1){
    ReducedNtuple->Draw("kkpidsstmass>>HKKpiGamMassBkg","kkpimctrue<0","same");
  }
  Canvas->Print(filename);

  //dalits plot
  TH2F HDalitsPlot("HDalitsPlot","",130,.9,3.5,100,.3,2.3);
  ReducedNtuple->Draw("kpimass:kkmass>>HDalitsPlot");
  HDalitsPlot.SetTitle("");
  HDalitsPlot.GetXaxis()->SetTitle("m^{2}(K^{+}K^{-})  (GeV^{2})");
  HDalitsPlot.GetYaxis()->SetTitle("m^{2}(K^{-}#pi^{+})  (GeV^{2})");
  HDalitsPlot.SetStats(0);
  Canvas->Clear(); 
  if(_DataType>0)Canvas->SetLogz(1);
  HDalitsPlot.Draw("colz");
  DrawDalitzRegions();
  Canvas->Print(filename);  
  Canvas->SetLogz(0);

  //extra energy
  Canvas->Clear(); 
  TH1F HExtraE("HExtraE","",70,0.,2.1);//use 30MeV bins because min energy in GoodPhotonLoose is 30MeV
  ReducedNtuple->Draw("kkpiextenergy>>HExtraE");
  HExtraE.GetXaxis()->SetTitle("Extra Neutral Energy  (GeV/c^{2})");
  HExtraE.GetYaxis()->SetTitle("Entries/30 MeV");
  TH1F HExtraEBkg("HExtraEBkg","",100,0.,2.); 
  HExtraEBkg.SetFillColor(1);    
  HExtraE.Draw("pe");
  if(_DataType==0||_DataType==1){
    ReducedNtuple->Draw("kkpiextenergy>>HExtraEBkg","kkpimctrue<0","same");
  }
  Canvas->Print(filename);
    
  ///
  TH1F HEvtId("HEvtId","",10000,0,10000);
  ReducedNtuple->Draw("goodeventid>>HEvtId");
  TH1F HEvtMultiplicity("HEvtMultiplicity","",5,.5,5.5);
  for(Int_t bin=1;bin<=10000;bin++)
    if(0.<HEvtId.GetBinContent(bin)&&HEvtId.GetBinContent(bin)<=5.)
      HEvtMultiplicity.AddBinContent((int)HEvtId.GetBinContent(bin),1);
  Canvas->Clear();
  HEvtMultiplicity.GetYaxis()->SetRangeUser(0,HEvtMultiplicity.GetMaximum()*1.1);
  HEvtMultiplicity.GetXaxis()->SetTitle("KK#pi Reconstructions/event");
  HEvtMultiplicity.Draw();
  Canvas->Print(filename); 

  if(_DataType==0){
    ///event multiplicity in Signal MC
    TH1F HEvtIdSig("HEvtIdSig","",10000,0,10000);
    ReducedNtuple->Draw("goodeventid>>HEvtIdSig","kkpimctrue>0");
    TH1F HEvtMultiplicitySig("HEvtMultiplicitySig","",5,.5,5.5);
    for(Int_t bin=1;bin<=10000;bin++)
      if(0.<HEvtIdSig.GetBinContent(bin)&&HEvtIdSig.GetBinContent(bin)<=5.)
	HEvtMultiplicitySig.AddBinContent((int)HEvtIdSig.GetBinContent(bin),1);
    Canvas->Clear();
    HEvtMultiplicitySig.GetYaxis()->SetRangeUser(0,HEvtMultiplicitySig.GetMaximum()*1.1);
    HEvtMultiplicitySig.GetXaxis()->SetTitle("KK#pi Reconstructions/event");
    HEvtMultiplicitySig.Draw();
    Canvas->Print(filename); 

    //gen dalits plot
    TH2F HDalitsPlotGen("HDalitsPlotGen","",130,.9,3.5,100,.3,2.3);
    ReducedNtuple->Draw("kpimassgen:kkmassgen>>HDalitsPlotGen","kkpimctrue>0");
    HDalitsPlotGen.SetTitle("");
    HDalitsPlotGen.GetXaxis()->SetTitle("m^{2}(K^{+}K^{-})  (GeV^{2})");
    HDalitsPlotGen.GetYaxis()->SetTitle("m^{2}(K^{-}#pi^{+})  (GeV^{2})");
    //HDalitsPlotGen.SetStats(0);
    Canvas->Clear(); 
    HDalitsPlotGen.Draw("colz");
    DrawDalitzRegions();
    Canvas->Print(filename);  
         
  }


  Canvas->Print(filename+"]");  

  return 1;
}



Int_t DRecoDsToKKpiAnalysis::CreateHistosForFit(){
      
  //Ds* mass 
  //Main histo to be fitted
  TH1F HKKpiGamMass("HKKpiGamMass","",NKKpiDsStFitBins,KKpiDsStFitMin,KKpiDsStFitMax);
  HKKpiGamMass.Sumw2();
  //signal Pdf from MC
  TH1F HKKpiGamMassSignal("HKKpiGamMassSignal","",NKKpiDsStFitBins,KKpiDsStFitMin,KKpiDsStFitMax);
  HKKpiGamMassSignal.Sumw2();
  //signal Pdf from MC
  TH1F HKKpiGamMassSignalShift("HKKpiGamMassSignalShift","",NKKpiDsStFitBins,KKpiDsStFitMin,KKpiDsStFitMax);
  HKKpiGamMassSignalShift.Sumw2();
  //bkg Pdf from generic MC
  TH1F HKKpiGamMassBkg("HKKpiGamMassBkg","",NKKpiDsStFitBins,KKpiDsStFitMin,KKpiDsStFitMax);
  HKKpiGamMassBkg.Sumw2();
  //KKpi events
  TH1F HKKpiGamMassTrue("HKKpiGamMassTrue","",NKKpiDsStFitBins,KKpiDsStFitMin,KKpiDsStFitMax);
  HKKpiGamMassTrue.Sumw2();

  //Ds* vs nXT 
  TH1F* HKKpiGamMassvsnXT[NXT];
  TH1F* HKKpiGamMassSignalvsnXT[NXT];
  TH1F* HKKpiGamMassBkgvsnXT[NXT];
  for(Int_t j=0;j<NXT;j++){
    HKKpiGamMassvsnXT[j]=new TH1F(TString("HKKpiGamMass_nXT")+(long)j,"",NKKpiDsStFitBins,KKpiDsStFitMin,KKpiDsStFitMax);
    HKKpiGamMassvsnXT[j]->Sumw2();
    HKKpiGamMassSignalvsnXT[j]=new TH1F(TString("HKKpiGamMassSignal_nXT")+(long)j,"",NKKpiDsStFitBins,KKpiDsStFitMin,KKpiDsStFitMax); 
    HKKpiGamMassSignalvsnXT[j]->Sumw2();
    HKKpiGamMassBkgvsnXT[j]=new TH1F(TString("HKKpiGamMassBkg_nXT")+(long)j,"",NKKpiDsStFitBins,KKpiDsStFitMin,KKpiDsStFitMax); 
    HKKpiGamMassBkgvsnXT[j]->Sumw2();
  }

  //for each region on the Dalitz plot
  TH1F* HKKpiGamMassDalitz[3];
  TH1F* HKKpiGamMassSignalDalitzvsnXT[3][NXT];
  TH1F* HKKpiGamMassSignalDalitz[3];//signal pdfs for each Dalitz region
  TH1F* HKKpiGamMassTrueDalitz[3];//signal pdfs for each Dalitz region
  TH1F* HKKpiGamMassTrueDalitzvsnXT[3][NXT];//signal pdfs for each Dalitz region and nXT
  for(Int_t d=0;d<3;d++){
    HKKpiGamMassDalitz[d]=new TH1F(TString("HKKpiGamMass_Dalitz")+(long)d,"",NKKpiDsStFitBins,KKpiDsStFitMin,KKpiDsStFitMax); 
    HKKpiGamMassDalitz[d]->Sumw2();
 
    HKKpiGamMassSignalDalitz[d]=new TH1F(TString("HKKpiGamMassSignal_Dalitz")+(long)d,"",NKKpiDsStFitBins,KKpiDsStFitMin,KKpiDsStFitMax); 
    HKKpiGamMassSignalDalitz[d]->Sumw2();

    HKKpiGamMassTrueDalitz[d]=new TH1F(TString("HKKpiGamMassTrue_Dalitz")+(long)d,"",NKKpiDsStFitBins,KKpiDsStFitMin,KKpiDsStFitMax); 
    HKKpiGamMassTrueDalitz[d]->Sumw2();
 

    for(Int_t j=0;j<NXT;j++){  
      HKKpiGamMassSignalDalitzvsnXT[d][j]=new TH1F(TString("HKKpiGamMassSignal_Dalitz")+(long)d+"_nXT"+(long)j,"",NKKpiDsStFitBins,KKpiDsStFitMin,KKpiDsStFitMax); 
      HKKpiGamMassSignalDalitzvsnXT[d][j]->Sumw2();
      HKKpiGamMassTrueDalitzvsnXT[d][j]=new TH1F(TString("HKKpiGamMassTrue_Dalitz")+(long)d+"_nXT"+(long)j,"",NKKpiDsStFitBins,KKpiDsStFitMin,KKpiDsStFitMax); 
      HKKpiGamMassTrueDalitzvsnXT[d][j]->Sumw2();
    }
  }
  

 
  if(!OpenReducedNtuple())return 0;

  ReducedNtuple->SetBranchStatus("*",0);
  ReducedNtuple->SetBranchStatus("goodeventid",1);ReducedNtuple->SetBranchAddress("goodeventid",&goodeventid);
  ReducedNtuple->SetBranchStatus("kkpimctrue",1);ReducedNtuple->SetBranchAddress("kkpimctrue",&kkpimctrue);
  ReducedNtuple->SetBranchStatus("fragndaus",1);ReducedNtuple->SetBranchAddress("fragndaus",&fragndaus);
  ReducedNtuple->SetBranchStatus("fragnpartgen",1);ReducedNtuple->SetBranchAddress("fragnpartgen",&fragnpartgen);
  ReducedNtuple->SetBranchStatus("kkpidsstmass",1);ReducedNtuple->SetBranchAddress("kkpidsstmass",&kkpidsstmass);
  ReducedNtuple->SetBranchStatus("kkmassgen",1);ReducedNtuple->SetBranchAddress("kkmassgen",&kkmassgen);
  ReducedNtuple->SetBranchStatus("kpimassgen",1);ReducedNtuple->SetBranchAddress("kpimassgen",&kpimassgen);
  ReducedNtuple->SetBranchStatus("kkmass",1);ReducedNtuple->SetBranchAddress("kkmass",&kkmass);
  ReducedNtuple->SetBranchStatus("kpimass",1);ReducedNtuple->SetBranchAddress("kpimass",&kpimass);
  ReducedNtuple->SetBranchStatus("evthasKKPi",1);ReducedNtuple->SetBranchAddress("evthasKKPi",&evthasKKPi);

  ///Loop over reduced ntuple
  Float_t priorkkpidsstmass[NMAXDSCANDS];
  Int_t priorfragndaus[NMAXDSCANDS];
  Int_t priorfragnpartgen[NMAXDSCANDS];
  Int_t priorkkpimctrue[NMAXDSCANDS];
  Float_t priorkkmass[NMAXDSCANDS];
  Float_t priorkpimass[NMAXDSCANDS];
  Int_t priorevthasKKPi=0;
  Int_t nDsCands=0;
  Int_t prioreventid=1;
  goodeventid=0;
  eventid=0;
  while(ReducedNtuple->GetEntry(eventid++,0)){
    if(eventid%10000==0)cout<<eventid<<" events done."<<endl;

    if(goodeventid!=prioreventid){//Fill histogram with masses from previous event
      for(Int_t i=0;i<nDsCands;i++){

	//
        HKKpiGamMass.Fill(priorkkpidsstmass[i],1./nDsCands);//total
        if(_DataType==0||_DataType==1){
          if(priorkkpimctrue[i]>0){
	    HKKpiGamMassSignal.Fill(priorkkpidsstmass[i],1./nDsCands);//signal
	    HKKpiGamMassSignalShift.Fill(priorkkpidsstmass[i]+.0005,1./nDsCands);//signal Data has shift
	  }
	  if(priorevthasKKPi==1)HKKpiGamMassTrue.Fill(priorkkpidsstmass[i],1./nDsCands);//KKpi event
          if(priorkkpimctrue[i]<=0)HKKpiGamMassBkg.Fill(priorkkpidsstmass[i],1./nDsCands);//background

          if(priorkkpimctrue[i]>0)HKKpiGamMassSignalvsnXT[priorfragnpartgen[i]]->Fill(priorkkpidsstmass[i],1./nDsCands);
          if(priorkkpimctrue[i]<=0)HKKpiGamMassBkgvsnXT[priorfragnpartgen[i]]->Fill(priorkkpidsstmass[i],1./nDsCands);
	  for(Int_t d=0;d<3;d++) 
	    if(DsToKKpiPassesDalitzCut(d)){
	      if(priorkkpimctrue[i]>0)HKKpiGamMassSignalDalitzvsnXT[d][priorfragnpartgen[i]]->Fill(priorkkpidsstmass[i],1./nDsCands);
	      if(priorkkpimctrue[i]>0)HKKpiGamMassSignalDalitz[d]->Fill(priorkkpidsstmass[i],1./nDsCands);
	      if(priorevthasKKPi==1)HKKpiGamMassTrueDalitz[d]->Fill(priorkkpidsstmass[i],1./nDsCands);
	      if(priorevthasKKPi==1)HKKpiGamMassTrueDalitzvsnXT[d][priorfragnpartgen[i]]->Fill(priorkkpidsstmass[i],1./nDsCands);
	    }
        } 
 
	
	//divide the data into each Dalitz region use the reco masses (-->resolution doesnt matter)
	Float_t tempkkmassgen=priorkkmassgen;
	Float_t tempkpimassgen=priorkpimassgen;
	priorkkmassgen=priorkkmass[i];
	priorkpimassgen=priorkpimass[i];
	for(Int_t d=0;d<3;d++) 	  	  	  
	  if(DsToKKpiPassesDalitzCut(d))
	    HKKpiGamMassDalitz[d]->Fill(priorkkpidsstmass[i],1./nDsCands);		  
	priorkkmassgen=tempkkmassgen;
	priorkpimassgen=tempkpimassgen;
	
      }
      
      ///-------------------------
      ///Get ready for next event
      //--------------------------
      nDsCands=0;
      prioreventid=goodeventid;
      priorkkmassgen=kkmassgen;      
      priorkpimassgen=kpimassgen;
      priorevthasKKPi=evthasKKPi;
    }
    
    //collect the masses from all Ds cands in this event
    priorkkpidsstmass[nDsCands]=kkpidsstmass;
    priorfragndaus[nDsCands]=fragndaus;
    priorfragnpartgen[nDsCands]=fragnpartgen;
    priorkkpimctrue[nDsCands]=kkpimctrue;
    priorkkmass[nDsCands]=kkmass;      
    priorkpimass[nDsCands]=kpimass;
    nDsCands++;
  }
  
  CloseReducedNtuple();

  //print out some numbers
  cout<<" Truth-matched Signal Event Yield After Weighting = "<<HKKpiGamMassSignal.Integral()<<endl;
  cout<<" Signal Event Yield After Weighting = "<<HKKpiGamMassTrue.Integral()<<endl;
  for(Int_t j=0;j<NXT;j++){
    cout<<" True Signal Event Yield After Weighting nXT: "<<j<<"  = "<<HKKpiGamMassSignalvsnXT[j]->Integral()<<endl;
  }


  TFile HistosFile(_OutputDir+"/HistosForFit.root","recreate");

  HKKpiGamMass.Write();
  HKKpiGamMassSignal.Write();
  HKKpiGamMassSignalShift.Write();
  HKKpiGamMassTrue.Write();
  HKKpiGamMassBkg.Write();  
  
  for(Int_t j=0;j<NXT;j++){
    HKKpiGamMassvsnXT[j]->Write();
    HKKpiGamMassSignalvsnXT[j]->Write();
    HKKpiGamMassBkgvsnXT[j]->Write();
  } 
  
  for(Int_t d=0;d<3;d++){
    HKKpiGamMassDalitz[d]->Write();
    HKKpiGamMassSignalDalitz[d]->Write();
    HKKpiGamMassTrueDalitz[d]->Write();
    for(Int_t j=0;j<NXT;j++){
      HKKpiGamMassSignalDalitzvsnXT[d][j]->Write();
      HKKpiGamMassTrueDalitzvsnXT[d][j]->Write();
    }
  }


  HistosFile.ls();
  HistosFile.Close();  


  return 1;
}
  
#include "RooMinuit.h"
#include "RooChi2Var.h"
#include "RooFitResult.h"
#include "DRecoilToolsUser/NtupleCode/DRecoHistPdfChi2Var.hh"
#include "DRecoilToolsUser/NtupleCode/DRecoResiduals.hh"
#include "DRecoilToolsUser/NtupleCode/DRecoTranslationPdf.hh"
using namespace RooFit;

Int_t DRecoDsToKKpiAnalysis::RooFitKKPiYieldDsSt(){


  filename=_OutputDir+"/RooFitYieldDsSt.ps";
  Canvas->Print(filename+"[");

  

  RooRealVar x("x","m(KK#pi#gamma)",KKpiDsStFitMin,KKpiDsStFitMax);//
  x.setBins(NKKpiDsStFitBins);
  x.setUnit("GeV/c^{2}");
  RooArgSet normset(x);

  //get the signal pdf 
  TFile SignalFile("DsTolnu/KKpi/Signal/HistosForFit.root","read");
  TH1F* HSignal=0;
  if(_DataType==1)HSignal=(TH1F*)SignalFile.Get("HKKpiGamMassSignal");
  if(_DataType==2)HSignal=(TH1F*)SignalFile.Get("HKKpiGamMassSignalShift");
  if(!HSignal){cout<<" No Signal histogram"<<endl;return 0;}
  Canvas->Clear();
  HSignal->Draw();
  Canvas->Print(filename); 
  RooDataHist SigDataHist("SigDataHist","SigDataHist",RooArgSet(x),HSignal);
  DRecoHistPdf SigPdf("SigPdf","SigPdf",normset,SigDataHist,2);
  //RooRealVar delta("delta","delta",-.0005);//shift upward
  //DRecoTranslationPdf SigPdfT("SigPdfT","SigPdfT",normset,SigPdf,x,delta);

//   //get the bkg pdf from MC
//   TFile BkgFile("DsTolnu/KKpi/SP1005/HistosForFit.root","read");
//   TH1F* HBkg=(TH1F*)BkgFile.Get("HKKpiGamMassBkg");
//   if(!HBkg){cout<<" No Bkg histogram"<<endl;return 0;}
//   RooDataHist BkgDataHist("BkgDataHist","BkgDataHist",RooArgSet(x),HBkg);
//   DRecoHistPdf BkgPdf("BkgPdf","BkgPdf",normset,BkgDataHist);

  //Use linear bkg pdf
  RooRealVar slope("slope","slope",-1,1);
  RooRealVar xmin("xmin","xmin",KKpiDsStFitMin);
  RooRealVar xmax("xmax","xmax",KKpiDsStFitMax);
  RooGenericPdf BkgPdf("BkgPdf","BkgPdf","1+slope*(x-xmin)/(xmax-xmin)",RooArgSet(x,xmin,xmax,slope));


  //get the data
  TFile HistosFile(_OutputDir+"/HistosForFit.root","read");
  TH1F* HData=(TH1F*)HistosFile.Get("HKKpiGamMass");
  if(!HData){cout<<" No Data histogram"<<endl;return 0;}  
  RooDataHist DataDataHist("DataDataHist","DataDataHist",normset,HData);


  if(_DataType==1){//plot the signal MC and bkg    
    Canvas->Clear();
    TH1F* HDataSignal=(TH1F*)HistosFile.Get("HKKpiGamMassSignal");
    HDataSignal->Draw();
    Canvas->Print(filename);
    Canvas->Clear();
    TH1F* HDataBkg=(TH1F*)HistosFile.Get("HKKpiGamMassBkg");
    HDataBkg->Draw();
    Canvas->Print(filename);
  }

  if(_DataType==2&&Blind){
    HData->Fit("pol1");
    Canvas->Print(filename);
    Canvas->Print(filename+"]");
    return 1;
  }

  //construct total pdf
  RooRealVar SigYield("SigYield","SigYield",.01,.9);
  DRecoAddPdf* TotPdf=new DRecoAddPdf("TotPdf","TotPdf",normset,SigPdf,BkgPdf,SigYield);
  
  //fit
  DRecoHistPdfChi2Var chi2("chi2","chi2",*TotPdf,DataDataHist,RooFit::DataError(RooAbsData::SumW2));  
  chi2.SetEType(RooAbsData::SumW2);
  RooMinuit minuit(chi2);
  minuit.migrad();
  RooFitResult*fitresult=minuit.save();
  fitresult->Print();
  cout<<" Signal Yield = "<<SigYield.getVal()*HData->Integral()<<" $\\pm$ "<<SigYield.getError()*HData->Integral()<<endl;

  //plot
  RooPlot* plot=x.frame();
  DataDataHist.plotOn(plot,RooFit::DataError(RooAbsData::SumW2));
  plot->SetTitle("");
  TH1* HFit=TotPdf->createHistogram("HFit",x);
  HFit->Scale(HData->Integral()/HFit->Integral());
  DRecoResiduals residuals("resid",HData,HFit);
  Canvas->Clear();
  HFit->SetLineColor(2);
  plot->addTH1(HFit,"hist same");
  residuals.MakeFitPlotWithResiduals(Canvas,plot);
  Canvas->cd(1);//draw the background
  TH1* hfitbkg=BkgPdf.createHistogram("hfitbkg",x);
  hfitbkg->Scale((1-SigYield.getVal())*HData->Integral()/hfitbkg->Integral());
  //hfitbkg->SetFillColor(1);
  //hfitbkg->SetFillStyle(3003);
  hfitbkg->Draw("hist same");
  Canvas->Print(filename);  

  //plot for paper
  //RooPlot* plotpaper=x.frame();
  //DataDataHist.plotOn(plotpaper,RooFit::DataError(RooAbsData::SumW2));
  //plotpaper->SetTitle("");
  Canvas->Clear();
  //plotpaper->addTH1(HFit,"hist same");
  //plot->Draw();
  HFit->SetTitle("");
  HFit->GetYaxis()->SetRangeUser(0,HFit->GetMaximum()*1.2);
  delete HFit->GetListOfFunctions()->FindObject("stats");
  HFit->GetYaxis()->SetTitle("Events / 8 MeV");
  HFit->SetStats(0);
  HFit->Draw("hist");
  hfitbkg->SetLineColor(0);
  hfitbkg->SetFillColor(4);
  hfitbkg->Draw("hist same");
  HData->Draw("pE1 same");
  Canvas->Print(filename);  


  Canvas->Print(filename+"]");

  TFile FitResults(_OutputDir+"/FitResults.root","recreate");
  HData->Write();
  HFit->SetName("HFit");
  HFit->Write();
  hfitbkg->SetName("HFitBkg");
  hfitbkg->Write();
  fitresult->SetName("FitResult");
  fitresult->Write();
  plot->SetName("fitplot");
  plot->Write();
  FitResults.ls();
  FitResults.Close();
  delete plot;

  return 1;  

}


Int_t DRecoDsToKKpiAnalysis::PlotEfficiency(){
  if(!OpenReducedNtuple())return 0;

  TH1F HRunData("HRunData","",120,.5,120.5);
  HRunData.GetXaxis()->SetTitle("Month");
  HRunData.GetYaxis()->SetTitle("Events");

  ReducedNtuple->Draw("((int(date/100)-2000)*12+(date%100))*(date<999999)+((int(int(date/10.)/100)-2000)*12+(int(date/10.)%100))*(date>999999)>>HRunData","kkpimctrue>0&&goodrecoid==1");

  filename=_OutputDir+"/PlotEfficiency.ps";
  Canvas->Print(filename+"[");
  
  Canvas->Clear();
  HRunData.Draw("hist");
  Canvas->Print(filename);

  ///////////////////////////////Efficiency correct vs Run
  TH1F HProd("HProd","",7,.5,7.5);
  HProd.GetXaxis()->SetTitle("Run");
  HProd.GetYaxis()->SetTitle("Events");
  TH1F HRec("HRec","",7,.5,7.5);
  HRec.GetXaxis()->SetTitle("Run");
  HRec.GetYaxis()->SetTitle("Events");
  //KKpi produced does not include DamageFilter
  Float_t SigPlusProdOn[7]={0.9790 , 2.8480 , 1.6540 , 5.0520 , 6.6540 , 3.9120 , 0.0000 + 1.3290};
  Float_t SigPlusProdOff[7]={0.1230 , 0.3390 , 0.1270 , 0.4070 , 0.7250 , 0.3950 , 0.0340 + 0.1130};
  Float_t SigMinusProdOn[7]={0.9790 , 2.8400 , 1.6540 , 5.0600 , 6.6620 , 3.9200 , 0.2270 + 1.1100};
  Float_t SigMinusProdOff[7]={0.1230 , 0.3470 , 0.1270 , 0.4070 , 0.7250 , 0.3950 , 0.0100 + 0.1370};
  Float_t CCProdOn[7]={55.254 , 164.722 , 88.321 , 267.308 , 344.579 , 208.664 , 37.684+ 88.516};
  Float_t CCProdOff[7]={ 5.585 , 17.560 , 6.532 , 21.292 , 37.813 , 20.540 , 3.900 + 9.880 };
  Int_t monthcuts[8]={0,13,33,43,56,83,93,103};
  for(Int_t r=0;r<7;r++){
    if(_DataType==0)HProd.SetBinContent(r+1,((24439880+24409072)/49439345.93)*1000000*(SigPlusProdOn[r]+SigPlusProdOff[r]+SigMinusProdOn[r]+SigMinusProdOff[r]));//correct for DamageFilter
    if(_DataType==1)HProd.SetBinContent(r+1,1000000*(CCProdOn[r]+CCProdOff[r]));
    HProd.SetBinError(r+1,sqrt( HProd.GetBinContent(r+1)));
    HRec.SetBinContent(r+1,ReducedNtuple->GetEntries(Form("(((int(date/100)-2000)*12+(date%100))*(date<999999)+((int(int(date/10.)/100)-2000)*12+(int(date/10.)%100))*(date>999999))>%d&&(((int(date/100)-2000)*12+(date%100))*(date<999999)+((int(int(date/10.)/100)-2000)*12+(int(date/10.)%100))*(date>999999))<=%d&&kkpimctrue>0&&goodrecoid==1",monthcuts[r],monthcuts[r+1])));

    HRec.SetBinError(r+1,sqrt(HRec.GetBinContent(r+1)));
  }
  
  Canvas->Clear();
  HProd.GetYaxis()->SetRangeUser(0,HProd.GetMaximum()*1.2);
  HProd.Draw("hist");
  Canvas->Print(filename);

  Canvas->Clear();
  HRec.GetYaxis()->SetRangeUser(0,HRec.GetMaximum()*1.2);
  HRec.Draw("hist");
  Canvas->Print(filename);

  delete HRec.GetListOfFunctions()->FindObject("stats");
  HRec.Divide(&HProd);
  HRec.SetStats(0);
  HRec.GetYaxis()->SetTitle(_DataType==0?"efficiency":"yield");
  Canvas->Clear();
  HRec.Draw();  
  //if(_DataType==0)HRec.GetYaxis()->SetRangeUser(0,1.e-3);
  //if(_DataType==1)HRec.GetYaxis()->SetRangeUser(0,.2e-3);
  HRec.GetYaxis()->SetRangeUser(0,HRec.GetBinContent(1)*1.4);
  HRec.Draw("pe");
  Canvas->Print(filename);

  if(_DataType==0){
    //print out the efficiencies
    for(Int_t r=0;r<7;r++)
      cout<<","<<HRec.GetBinContent(r+1);
    cout<<endl;
    for(Int_t r=0;r<7;r++)
      cout<<","<<HRec.GetBinError(r+1);
    cout<<endl;
  }
  if(_DataType==1){
    //plot the efficiency corrected yield
    Float_t eff[7]={0.000607527,0.000593056,0.000596682,0.000556712,0.000518518,0.000497357,0.000593575};
    Float_t efferr[7]={1.67077e-05,9.70688e-06,1.30245e-05,7.18314e-06,5.96309e-06,7.6427e-06,1.42505e-05};
    TH1F HEff("HEff","",7,.5,7.5);
    for(Int_t r=0;r<7;r++){
      HEff.SetBinContent(r+1,eff[r]);
      HEff.SetBinError(r+1,efferr[r]);
    }
    HRec.Divide(&HEff);
    HRec.SetStats(1);
    HRec.GetYaxis()->SetTitle("efficiency corrected yield");
    Canvas->Clear();
    HRec.GetYaxis()->SetRangeUser(0,HRec.GetBinContent(1)*1.7);
    HRec.Fit("pol0");
    HRec.Draw();
    Canvas->Print(filename);
  }



  ///////////////////////////////Efficiency correct vs nXT
  TH1F HRecNXT("HRecNXT","",NXT,-.5,NXT-.5);
  HRecNXT.GetXaxis()->SetTitle("n_{X}^{T}");
  HRecNXT.GetYaxis()->SetTitle("Reco. Events");
  for(Int_t j=0;j<7;j++){
    HRecNXT.SetBinContent(j+1,ReducedNtuple->GetEntries(Form("fragnpartgen==%d&&kkpimctrue>0&&goodrecoid==1",j)));
    HRecNXT.SetBinError(j+1,sqrt(HRecNXT.GetBinContent(j+1)));
  }
  Canvas->Clear();
  HRecNXT.Draw();
  Canvas->Print(filename);
  if(_DataType==0){
    TH1F HProdNXT("HProdNXT","",NXT,-.5,NXT-.5);
    HProdNXT.GetXaxis()->SetTitle("n_{X}^{T}");
    HProdNXT.GetYaxis()->SetTitle("Prod. Events");
    Float_t SigProd=24439880+24409072;//49439345.93;
    Float_t SigProdWeight[7]={0.0526159,0.087773,0.131337,0.16068,0.158826,0.136413,0.272355};//Weights from Ds produced
    for(Int_t j=0;j<7;j++){
      HProdNXT.SetBinContent(j+1,SigProd*SigProdWeight[j]);
    }
    Canvas->Clear();
    HProdNXT.GetYaxis()->SetRangeUser(0, HProdNXT.GetBinContent(NXT)*1.1);
    HProdNXT.Draw();
    Canvas->Print(filename);

    HRecNXT.Divide(&HProdNXT);
    delete HRecNXT.GetListOfFunctions()->FindObject("stats");
    Canvas->Clear();
    HRecNXT.SetStats(0);
    HRecNXT.GetYaxis()->SetTitle("efficiency");
    HRecNXT.Draw("pe");
    Canvas->Print(filename);
    for(Int_t j=0;j<7;j++)
      cout<<","<<HRecNXT.GetBinContent(j+1);
    cout<<endl;
    for(Int_t j=0;j<7;j++)
      cout<<","<<HRecNXT.GetBinError(j+1);
    cout<<endl;
  }
  
    
  if(_DataType==1){
    TH1F HEff("HEff","",NXT,-.5,NXT-.5);
    HEff.GetXaxis()->SetTitle("n_{X}^{T}");
    HEff.GetYaxis()->SetTitle("efficiency");
    Float_t Eff[7]={0.00138859,0.00162678,0.001061,0.000615234,0.000358317,0.000189386,3.63793e-05 };
    Float_t Efferr[7]={2.32596e-05,1.94944e-05,1.28667e-05,8.85615e-06,6.7971e-06,5.33163e-06,1.65364e-06};
    for(Int_t j=0;j<7;j++){
      HEff.SetBinContent(j+1,Eff[j]);
      HEff.SetBinError(j+1,Efferr[j]);
    }
    HRecNXT.Divide(&HEff);
    delete HRecNXT.GetListOfFunctions()->FindObject("stats");
    HRecNXT.SetStats(1);
    HRecNXT.GetYaxis()->SetRangeUser(0, HRecNXT.GetBinContent(NXT)*1.1);
    Canvas->Clear();
    //HRecNXT.Fit("pol0");
    HRecNXT.Draw("pe");
    Canvas->Print(filename);        
  }


  Canvas->Print(filename+"]");
  return 1;
}


Float_t DRecoDsToKKpiAnalysis::KKpiDalitzCorrFactor(){
 
  ifstream KKpiProducedFile; 
  KKpiProducedFile.open("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResultsKKpi/KKpiProduced.txt");
  TFile FNumeratorEff("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResultsKKpi/KKpiEffNumerator.root","r");//KKpi efficiency 
 
  Float_t TotKKpiProd=0;
  Float_t TotKKpiRec=0; 
  Float_t KKpiProd[3];
  Float_t KKpiRec[3]={0,0,0}; 
  Float_t KKpiEff[3]={0,0,0};
  Float_t KKpiEffErr[3]={0,0,0};
  for(Int_t d=0;d<3;d++){
    for(Int_t j=0;j<NXT;j++){
      Int_t indexcheckn;
      Int_t indexcheckd;
      Float_t NProdKKpi=0.;
      KKpiProducedFile>>indexcheckd>>indexcheckn>>NProdKKpi;   
      if(indexcheckd!=(d+1)||indexcheckn!=j){cout<<"d,j"<<d<<","<<j<<endl;exit(0);}
      TotKKpiProd+=NProdKKpi;             
    }

    TH1F*HKKpiRec=(TH1F*)FNumeratorEff.Get(TString("HKKpiGamMassSignal_Dalitz")+(long)d);
    if(!HKKpiRec){cout<<"No "<<d<<endl;return 0;}
    KKpiRec[d]=HKKpiRec->Integral();
    TotKKpiRec+=HKKpiRec->Integral();        
  }

  Float_t KKpiGenDalitzWeight[3]={0.0556567,0.291852,0.652491};
  cout<<"Dalitz effs: "<<endl;
  for(Int_t d=0;d<3;d++){
    //cout<<"Gen Flat Dalitz weight d="<<d<<"  w="<<KKpiGenDalitzWeight[d]<<endl;
    KKpiProd[d]=TotKKpiProd*KKpiGenDalitzWeight[d];
   
    KKpiEff[d]=KKpiRec[d]/KKpiProd[d];
    KKpiEffErr[d]=sqrt(KKpiRec[d])/KKpiProd[d];
    cout<<fixed<<setprecision(3)<<" & "<<KKpiEff[d]*10000<<" $\\pm$ "<<KKpiEffErr[d]*10000;
  }
  cout<<" \\\\"<<endl;

  TFile FDalitzWeight("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResultsKKpi/DalitzWeights.root","r");//Dalitz weights contains both MC and Data
  Float_t dalitzweight[3];
  Float_t totDalitzweight=0.;
  cout<<" Dalitz Weights: "<<endl;
  for(Int_t d=0;d<3;d++){
    TH1F*HDalitzWeight=NULL;
    if(_DataType==1)HDalitzWeight=(TH1F*)FDalitzWeight.Get(TString("HDsMassMCDalitz")+(long)d);
    if(_DataType==2)HDalitzWeight=(TH1F*)FDalitzWeight.Get(TString("HDsMassDataDalitz")+(long)d);
    if(!HDalitzWeight)cout<<"Bad Dalitz Histo"<<d<<endl;
    dalitzweight[d]=HDalitzWeight->GetBinContent(0);//ignore weight errors
    totDalitzweight+= dalitzweight[d];
    cout<<" "<<dalitzweight[d];
  }
  if(fabs(totDalitzweight-1.)>1e-5){cout<<" Dalitz weights do not add up to 1."<<endl; return 0;}
  cout<<endl;


  //Compute raw efficiency;
  Float_t rawEfficiency=TotKKpiRec/TotKKpiProd;
  cout<<"The raw efficiency = "<<10000*rawEfficiency<<" $\\times 10^-4$"<<endl;

  //Compute the weighted efficiency
  Float_t corrEfficiency=0;
  for(Int_t d=0;d<3;d++){
    corrEfficiency+=dalitzweight[d]* KKpiEff[d];
  }
  cout<<"The corrected efficiency = "<<10000*corrEfficiency<<" $\\times 10^-4$"<<endl;

  return corrEfficiency/rawEfficiency;
}


// Int_t DRecoDsToKKpiAnalysis::RooFitKKPiYieldDsStDalitz(){


//   filename=_OutputDir+"/RooFitYieldDsStDalitz.ps";
//   Canvas->Print(filename+"[");

//   RooPlot* plot;

//   RooRealVar x("x","m(KK#pi#gamma)",KKpiDsStFitMin,KKpiDsStFitMax);
//   x.setUnit("GeV/c^{2}");
//   x.setBins(NKKpiDsStFitBins);
//   RooArgSet normset(x);


//   //get the signal pdf 
//   TFile SignalFile("DsTolnu/KKpi/Signal/HistosForFit.root","read");
//   TH1F* HSignal=(TH1F*)SignalFile.Get("HKKpiGamMassSignal");
//   if(!HSignal){cout<<" No Signal histogram"<<endl;return 0;}
//   RooDataHist SigDataHist("SigDataHist","SigDataHist",RooArgSet(x),HSignal);
//   DRecoHistPdf SigPdf("SigPdf","SigPdf",normset,SigDataHist);
//   Canvas->Clear();
//   plot=x.frame();
//   SigPdf.plotOn(plot);  
//   plot->GetYaxis()->SetTitle("");
//   plot->SetTitle("");
//   plot->Draw();
//   Canvas->Print(filename);  
//   delete plot;

//   //get the bkg pdf 
// //   TFile BkgFile("DsTolnu/KKpi/SP1005/HistosForFit.root","read");
// //   TH1F* HBkg=(TH1F*)BkgFile.Get("HKKpiGamMassBkg");
// //   if(!HBkg){cout<<" No Bkg histogram"<<endl;return 0;}
// //   RooDataHist BkgDataHist("BkgDataHist","BkgDataHist",RooArgSet(x),HBkg);
// //   DRecoHistPdf BkgPdf("BkgPdf","BkgPdf",normset,BkgDataHist);
//   RooRealVar slope("slope","slope",-1,1);
//   RooRealVar xmin("xmin","xmin",KKpiDsStFitMin);
//   RooRealVar xmax("xmax","xmax",KKpiDsStFitMax);
//   RooGenericPdf BkgPdf("BkgPdf","BkgPdf","1+slope*(x-xmin)/(xmax-xmin)",RooArgSet(x,xmin,xmax,slope));
//   Canvas->Clear();
//   plot=x.frame();
//   BkgPdf.plotOn(plot); 
//   plot->GetYaxis()->SetTitle("");
//   plot->SetTitle(""); 
//   plot->Draw();
//   Canvas->Print(filename);  
//   delete plot; 

//   TFile FitResults(_OutputDir+"/FitResultsDalitz.root","recreate");
//   gROOT->cd();

//   //get the data
//   TFile HistosFile(_OutputDir+"/HistosForFit.root","read");


//   Canvas->Clear();
//   Canvas->Divide(2,2);

//   TH1F* hfitbkg[3];
//   TVirtualPad* pad[3];
//   RooFitResult* fitresult[3];
//   RooPlot* plots[3];
//   RooRealVar* SigYield[3];
//   TH1F* HData[3];
//   DRecoResiduals* residuals[3];
//   text.SetTextSize(.09);
//   for(Int_t d=0;d<3;d++){
//     pad[d]=Canvas->cd(d+1);
//     pad[d]->SetTitle(TString("pad")+(long)d);

//     HData[d]=(TH1F*)HistosFile.Get(TString("HKKpiGamMass_Dalitz")+(long)d);
//     if(!HData[d]){cout<<" No Data histogram"<<endl;return 0;}

//     RooDataHist DataDataHist("DataDataHist","DataDataHist",normset,HData[d]);

//     if(_DataType==2){
// //       //TH1F* HBkgC=(TH1F*)HBkg->Clone(TString("HBkgC")+(long)d); 
// //       TH1F* HBkgC=(TH1F*)(BkgPdf.createHistogram(TString("HBkgC")+(long)d,x));     
// //       for(Int_t i=1;i<=HBkgC->GetNbinsX();i++) if(HData[d]->GetBinContent(i)==0) HBkgC->SetBinContent(i,0);//for normalization below.
// //       TH1F* HBkgPlot=(TH1F*)(BkgPdf.createHistogram(TString("HBkgPlot")+(long)d,x));     
// //       HBkgPlot->Scale(HData[d]->Integral()/HBkgC->Integral());
// //       residuals[d]=new DRecoResiduals(TString("resid")+(long)d,HData[d],HBkgPlot);
// //       residuals[d]->FitHasError(1);
// //       //Canvas->Clear();
// //       plots[d]=x.frame();plots[d]->SetName(TString("DataPlot")+(long)d);
// //       DataDataHist.plotOn(plots[d],RooFit::DataError(RooAbsData::SumW2));  
// //       HBkgPlot->SetLineColor(4);
// //       plots[d]->addTH1(HBkgPlot,"hist same");
// //       plots[d]->SetTitle(TString("Dalitz Region")+(long)d);
// //       residuals[d]->MakeFitPlotWithResiduals((TCanvas*)pad[d],plots[d]);

//       HData[d]->Fit("pol1");
//       continue;
//     }


//     //construct total pdf
//     SigYield[d]=new RooRealVar(TString("SigYield")+(long)d,TString("SigYield")+(long)d,.01,.9);
//     DRecoAddPdf TotPdf(TString("TotPdf")+(long)d,TString("TotPdf")+(long)d,normset,SigPdf,BkgPdf,*SigYield[d]);
 


//     //fit
//     DRecoHistPdfChi2Var chi2(TString("chi2")+(long)d,TString("chi2")+(long)d,TotPdf,DataDataHist,RooFit::DataError(RooAbsData::SumW2));  
//     chi2.SetEType(RooAbsData::SumW2);
//     RooMinuit minuit(chi2);
//     //minuit.SetName(TString("minuit")+(long)d);
//     minuit.migrad();
//     fitresult[d]=minuit.save();
//     fitresult[d]->SetName(TString("FitResult")+(long)d);
//     fitresult[d]->Print();

//     //plot
//     plots[d]=x.frame();
//     plots[d]->SetName(TString("RooPlot")+(long)d);
//     DataDataHist.plotOn(plots[d],RooFit::DataError(RooAbsData::SumW2));
//     TotPdf.plotOn(plots[d]);
//     plots[d]->SetTitle("");

//     hfitbkg[d]=(TH1F*)(BkgPdf.createHistogram(TString("hfitbkg")+(long)d,x));//(TH1F*)HBkg->Clone(TString("hfitbkg")+(long)d);
//     hfitbkg[d]->Scale((1-SigYield[d]->getVal())*HData[d]->Integral()/hfitbkg[d]->Integral());

//     //Make Residuals
//     TH1* HFit=TotPdf.CreateHistogram(TString("HFit")+(long)d);
//     HFit->Scale(HData[d]->Integral());
//     residuals[d]=new DRecoResiduals(TString("resid")+(long)d,HData[d],HFit);
//     residuals[d]->FitHasError(1);
//     plots[d]->addTH1(hfitbkg[d],"hist same");
//     plots[d]->GetYaxis()->SetLabelSize(.06);
//     plots[d]->GetYaxis()->SetTitleSize(.075);
//     plots[d]->GetYaxis()->SetTitleOffset(.6);
//     plots[d]->GetXaxis()->SetTitleSize(.07);
//     plots[d]->GetXaxis()->SetLabelSize(.06);
//     residuals[d]->MakeFitPlotWithResiduals((TCanvas*)pad[d],plots[d]);
//     pad[d]->cd(1);
//     text.DrawText(HData[d]->GetXaxis()->GetXmin()+ (HData[d]->GetXaxis()->GetXmax()-HData[d]->GetXaxis()->GetXmin())/2,.9*HData[d]->GetMaximum(),TString("Dalitz Region ")+(long)d);

    
//     FitResults.cd();
//     HData[d]->Write();
//     HFit->Write();   
//     hfitbkg[d]->Write();
//     fitresult[d]->Write();
//     plots[d]->Write();
//     gROOT->cd();

//     //delete plot;//will remove plots    
//     delete HFit;
//     //delete hfitbkg;//will remove bkg from hists
//   }
//   Canvas->Print(filename);
//   Canvas->Print(filename+"]");

  
//   //if(_DataType==2)return 1;
  
//   FitResults.ls();
//   FitResults.Close(); 
  
//   Float_t TotTrueYield=0;
//   Float_t TotFitYield=0;
//   Float_t TotFitYieldErr=0;
//   for(Int_t d=0;d<3;d++){
//     cout<<"Dalitz Region "<<d<<setprecision(2)<<fixed<<" & "<<mcyields[d]<<" &  $"<<SigYield[d]->getVal()*HData[d]->Integral()<<" \\pm "<<SigYield[d]->getError()*HData[d]->Integral()<<"$ \\\\"<<endl;
    
//     TotTrueYield+=mcyields[d];
//     TotFitYield+=SigYield[d]->getVal()*HData[d]->Integral();
//     TotFitYieldErr+=pow(SigYield[d]->getError()*HData[d]->Integral(),2);

//     //delete hfitbkg[d];//belongs to plots[d]
//     delete pad[d];
//     delete fitresult[d];
//     delete plots[d];
//     delete residuals[d];
//   }
//   cout<<"\\hline"<<endl;
//   cout<<"Total & "<<TotTrueYield<<" &  $"<<TotFitYield<<" \\pm "<<sqrt(TotFitYieldErr)<<"$ \\\\"<<endl;


//   return 1;  

// }





// Int_t DRecoDsToKKpiAnalysis::RooFitKKPiYieldDeltaM(){


//   filename=_OutputDir+"/RooFitYieldDeltaM.ps";
//   Canvas->Print(filename+"[");

//   RooPlot* plot;

//   //get the data
//   TFile HistosFile(_OutputDir+"/HistosForFit.root","read");
//   TH1F* HData=(TH1F*)HistosFile.Get("HKKpiDeltaM");
//   if(!HData){cout<<" No Data histogram"<<endl;return 0;}
//   RooRealVar x("x","m(KK#pi#gamma)-m(KK#pi)",HData->GetXaxis()->GetXmin(),HData->GetXaxis()->GetXmax());
//   x.setUnit("GeV/c^{2}");
//   x.setBins(HData->GetXaxis()->GetNbins());
//   RooArgSet normset(x);

//   RooDataHist DataDataHist("DataDataHist","DataDataHist",normset,HData);
//   Canvas->Clear();
//   plot=x.frame();
//   DataDataHist.plotOn(plot);  
//   plot->Draw();
//   Canvas->Print(filename);  
//   delete plot;

//   //get the signal pdf 
//   TFile SignalFile("DsTolnu/KKpi/SP1005/HistosForFit.root","read");
//   TH1F* HSignal=(TH1F*)HistosFile.Get("HKKpiDeltaMSignal");
//   if(!HSignal){cout<<" No Signal histogram"<<endl;return 0;}
//   RooDataHist SigDataHist("SigDataHist","SigDataHist",RooArgSet(x),HSignal);
//   DRecoHistPdf SigPdf("SigPdf","SigPdf",normset,SigDataHist);
//   Canvas->Clear();
//   plot=x.frame();
//   SigPdf.plotOn(plot);  
//   plot->Draw();
//   Canvas->Print(filename);  
//   delete plot;

//   //get the bkg pdf 
//   TFile BkgFile("DsTolnu/KKpi/SP1005/HistosForFit.root","read");
//   TH1F* HBkg=(TH1F*)HistosFile.Get("HKKpiDeltaMBkg");
//   if(!HBkg){cout<<" No Bkg histogram"<<endl;return 0;}
//   RooDataHist BkgDataHist("BkgDataHist","BkgDataHist",RooArgSet(x),HBkg);
//   DRecoHistPdf BkgPdf("BkgPdf","BkgPdf",normset,BkgDataHist);
//   Canvas->Clear();
//   plot=x.frame();
//   BkgPdf.plotOn(plot);  
//   plot->Draw();
//   Canvas->Print(filename);  
//   delete plot; 

//   //construct total pdf
//   RooRealVar SigYield("SigYield","SigYield",0,.7);
//   DRecoAddPdf TotPdf("TotPdf","TotPdf",normset,SigPdf,BkgPdf,SigYield);
 
//   //fit
//   DRecoHistPdfChi2Var chi2("chi2","chi2",TotPdf,DataDataHist);//,RooFit::DataError(RooAbsData::SumW2)
//   chi2.SetEType(RooAbsData::SumW2);
//   RooMinuit minuit(chi2);
//   minuit.migrad();
  
//   //plot
//   plot=x.frame();
//   DataDataHist.plotOn(plot);
//   TotPdf.plotOn(plot);
 
//   plot->SetTitle(TString("Fit Yield=")+(long)(SigYield.getVal()*HData->Integral())+" +- "+(long)(SigYield.getError()*HData->Integral()));
//   //plot->Draw();
//   TH1F* hfitbkg=(TH1F*)HBkg->Clone("hfitbkg");
//   hfitbkg->Scale((1-SigYield.getVal())*HData->Integral()/HBkg->Integral());
//   hfitbkg->SetFillColor(1);
//   hfitbkg->SetFillStyle(3003);
//   //hfitbkg->Draw("same");
 

//   //Make Residuals
//   TH1* HFit=TotPdf.createHistogram("HFit",x);
//   HFit->Scale(HData->Integral());
//   DRecoResiduals residuals("resid",HData,HFit);
//   Canvas->Clear();
//   residuals.MakeFitPlotWithResiduals(Canvas,plot);
//   Canvas->cd(1);
//   hfitbkg->Draw("same");
//   Canvas->Print(filename);  
//   delete plot;
  

//   Canvas->Print(filename+"]");  

//   return 1;  

// }

