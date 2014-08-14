Int_t Ds1Analysis::DetermineEfficiencyVspstar(Int_t Nsignal){
    
  cout<<"Going to determine the Ds1 Efficiency correction"<<endl;
  if(TruthMatch){
    cout<<"This action is only for DATA"<<endl;
    return 0;
  }

  
  /////first we have to open the MC file to get generated p*
  TString MCfile;
  if(_Mode=="D0ToKPi")MCfile="/u2/benitezj/physics/NTUPLES/Ds1pToDstKs/D0ToKPi/SP-7796/Ds1pReduced.root";
  if(_Mode=="D0ToK3Pi")MCfile="/u2/benitezj/physics/NTUPLES/Ds1pToDstKs/D0ToK3Pi/SP-7798/Ds1pReduced.root";
  TFile* temp;
  temp=new TFile(MCfile,"read");
  if(temp->IsZombie()){
    cout<<"could not open file "<<endl;
    return 0;   
  }
  gROOT->cd();
  cout<<"Going to use MC from file:"<<temp->GetName()<<endl;
  

  TTree* tree=(TTree*)temp->Get("Ds1pNtuple");
  if(tree==NULL){
    cout<<"No ntuple."<<endl;
    return 0;
  }

 

  TCanvas Canvas("Canvas","Canvas");
  TString fname=_OutputDir+"/"+"Ds1Effpstar"+(long)_WhichCuts+".ps";
  Canvas.Print(fname+"[");



  //Define the Binning to match Torstens pstar binning
  Int_t npstarbins=Ds1pp3CMNbins;
  Float_t pstarmin=Ds1pp3CMmin;
  Float_t pstarmax=Ds1pp3CMmax;
  npstarbins=11;
  pstarmin=2.7;
  pstarmax=4.79;//4.6

  
  //----------
  //Retrieve the Gen. pstar
  //-----------
  sprintf(textlabel,"HMCDs1pp3CMTorsten");
  TH1F*HMCpstarGen=NULL;
  if(!(HMCpstarGen=(TH1F*)temp->Get(textlabel))){
    cout<<"No saved histogram "<<textlabel<<endl;return 0;
  }
  HMCpstarGen->SetDirectory(0);

  
  //----------
  //Get the Torstens reco. pstar using his cuts
  //-----------
  database.SetCuts(0);//set to Torstens cuts
  sprintf(textlabel,"2.7<ds1ppstar");//extra Torsten cut  
  TCut torstenpstarcut=textlabel;
 
  TH1F HDs1Torsten_pstar("HDs1Torsten_pstar","MC Ds1 p* (Torstens Cuts)",npstarbins,pstarmin,pstarmax);
  HDs1Torsten_pstar.SetXTitle(" p* [GeV]");
  HDs1Torsten_pstar.SetYTitle("");  
  tree->Draw("ds1ppstar>>HDs1Torsten_pstar",database.GetCut(1,100)+torstenpstarcut);


  ////Now compare My MC to Torstens MC    /
  TH1F HTorstenMCpstar("HTorstenMCpstar","Torstens MC p*",npstarbins,pstarmin,pstarmax);
  Float_t z[11]={187,224,260,287,311,319,314,286,185,104,0};
  Float_t z2[11]={159,206,262,323,376,429,445,423,313,214,0};
  for(Int_t i=0;i<npstarbins;i++){    
    if(_Mode=="D0ToKPi")HTorstenMCpstar.SetBinContent(i+1,z[i]);  
    if(_Mode=="D0ToK3Pi")HTorstenMCpstar.SetBinContent(i+1,z2[i]);  
  }

  
  Canvas.Clear();
  HTorstenMCpstar.GetYaxis()->SetRangeUser(0,1.2*HTorstenMCpstar.GetMaximum());
  HTorstenMCpstar.SetMarkerColor(1);
  HTorstenMCpstar.SetMarkerStyle(26);
  HTorstenMCpstar.SetMarkerSize(1.7);
  HTorstenMCpstar.SetStats(kFALSE); 

  HDs1Torsten_pstar.SetMarkerColor(2);
  HDs1Torsten_pstar.SetMarkerStyle(4);
  HDs1Torsten_pstar.SetMarkerSize(1.7);
  HDs1Torsten_pstar.SetStats(kFALSE); 
  HDs1Torsten_pstar.SetNormFactor(HTorstenMCpstar.Integral());
  
  HTorstenMCpstar.Draw("p");
  HDs1Torsten_pstar.Draw("psame");
  Canvas.Print(fname);
  HDs1Torsten_pstar.SetNormFactor(0);//return to previous normalization

  //-------------
  //Determine Torstens p* dep eff
  //-------------
  TH1F* HDs1Torsten_pstareff=(TH1F*)HDs1Torsten_pstar.Clone();
  HDs1Torsten_pstareff->SetTitle("Torstens Efficiency");
  for(Int_t bin=1;bin<=npstarbins-1;bin++){
    HDs1Torsten_pstareff->SetBinContent(bin,(double)HDs1Torsten_pstar.GetBinContent(bin) /(double)HMCpstarGen->GetBinContent(bin));
    HDs1Torsten_pstareff->SetBinError(bin,0);
  }
  HDs1Torsten_pstareff->SetBinContent(npstarbins,HDs1Torsten_pstareff->GetBinContent(npstarbins-1));
  HDs1Torsten_pstareff->SetBinError(npstarbins,0);

  //-----
  //Important:return to default cuts
  //-----
  database.SetCuts(_WhichCuts);

  //----------
  //Get my MC reco. pstar
  //----------- 
  TH1F HDs1_pstar("HDs1_pstar","MC Ds1 p* (my cuts)",npstarbins,pstarmin,pstarmax);
  HDs1_pstar.SetXTitle(" p* [GeV]");
  HDs1_pstar.SetYTitle("");  
  tree->Draw("ds1ppstar>>HDs1_pstar",database.GetCut(1,100));
   
  
  //----------------
  //calculate My p* dep eff.
  //-----------------
  TH1F* HDs1_pstareff=(TH1F*)HDs1_pstar.Clone();
  HDs1_pstareff->SetTitle("My Efficiency");
  for(Int_t bin=1;bin<=npstarbins-1;bin++){
    HDs1_pstareff->SetBinContent(bin,(double)HDs1_pstar.GetBinContent(bin)/(double)HMCpstarGen->GetBinContent(bin));
    HDs1_pstareff->SetBinError(bin,0);
  }
  HDs1_pstareff->SetBinContent(npstarbins,HDs1_pstareff->GetBinContent(npstarbins-1));
  HDs1_pstareff->SetBinError(npstarbins,0);

  ////--plot the Gen. and Reco p* distributions
  Canvas.Clear();
  HMCpstarGen->GetYaxis()->SetRangeUser(0,1.1*HMCpstarGen->GetMaximum());
  HMCpstarGen->Draw();
  HDs1Torsten_pstar.Draw("same");
  HDs1_pstar.Draw("same");
  Canvas.Print(fname);

  ////--plot the efficiencies
  Canvas.Clear();
  HDs1Torsten_pstareff->GetYaxis()->SetRangeUser(0,1.1*HDs1Torsten_pstareff->GetMaximum());
  HDs1Torsten_pstareff->Draw();
  HDs1_pstareff->Draw("same");
  Canvas.Print(fname);
 

  //-------------
  //Determine transformation factor from my p* to Torstens p*
  //-------------
  TH1F* HDs1_pstarTransform=(TH1F*)HDs1Torsten_pstareff->Clone();
  HDs1_pstarTransform->Divide(HDs1_pstareff);
  
  Canvas.Clear();
  HDs1_pstarTransform->GetYaxis()->SetRangeUser(0,19);
  HDs1_pstarTransform->Draw();
  Canvas.Print(fname);
  
  //------------------------------------------------------------------
  ///now correct the data: need to open the DATA reduced Ntuple
  //-------------------------------------------------------------------
  if(!OpenReducedNtuple()){
    cout<<"Failed to open the data file"<<endl;
    return 0;
  }

  sprintf(textlabel,"%.5f<ds1pdeltam2&&ds1pdeltam2<%.5f",database.GetDs1pDeltaMSignalMin(),database.GetDs1pDeltaMSignalMax());  
  TCut signalcut=textlabel;

  TH1F HDs1_pstarDATASigPlusBkg("HDs1_pstarDATASigPlusBkg","DATA Ds1 p* (My Cuts)",npstarbins,pstarmin,pstarmax);
  HDs1_pstarDATASigPlusBkg.SetXTitle(" p* [GeV]");
  HDs1_pstarDATASigPlusBkg.SetYTitle("");  
  ReducedNtuple->Draw("ds1ppstar>>HDs1_pstarDATASigPlusBkg",database.GetCut(1,100)+signalcut);

  //-----------
  //Subtract the background
  //-----------
  Int_t NbkgCands = (int)HDs1_pstarDATASigPlusBkg.GetEntries()-Nsignal;
  cout<<"N bkg cands="<<NbkgCands<<endl;
  //86=in full p* spectrum,
  // cout<<"Enter Number of background Cands: (86=KPi Cuts1,273=TorstenKPi Cuts0,58=TorstenKPi cuts1)"<<endl;
  //cin>>NbkgCands;

  //Background on the low side
  sprintf(textlabel,"ds1pdeltam2<%.5f",database.GetDs1pDeltaMSignalMin());  
  TCut lowbkgcut=textlabel;

  TH1F HDs1_pstarDATABkgLow("HDs1_pstarDATABkgLow","DATA Bkg Ds1 p* (My Cuts)",npstarbins,pstarmin,pstarmax);
  HDs1_pstarDATABkgLow.SetXTitle(" p* [GeV]");
  HDs1_pstarDATABkgLow.SetYTitle("");  
  ReducedNtuple->Draw("ds1ppstar>>HDs1_pstarDATABkgLow",database.GetCut(1,100)+lowbkgcut);
  HDs1_pstarDATABkgLow.SetNormFactor(NbkgCands);

  //Background on the high side
  sprintf(textlabel,"ds1pdeltam2>%.5f",database.GetDs1pDeltaMSignalMax());  
  TCut highbkgcut=textlabel;

  TH1F HDs1_pstarDATABkgHigh("HDs1_pstarDATABkgHigh","DATA Bkg Ds1 p* (My Cuts)",npstarbins,pstarmin,pstarmax);
  HDs1_pstarDATABkgHigh.SetXTitle(" p* [GeV]");
  HDs1_pstarDATABkgHigh.SetYTitle("");  
  ReducedNtuple->Draw("ds1ppstar>>HDs1_pstarDATABkgHigh",database.GetCut(1,100)+highbkgcut);
  HDs1_pstarDATABkgHigh.SetNormFactor(NbkgCands);
 
  //now average the bkgs
  TH1F HDs1_pstarDATABkg("HDs1_pstarDATABkg","DATA Bkg Ds1 p* (My Cuts)",npstarbins,pstarmin,pstarmax);
  HDs1_pstarDATABkg.SetXTitle(" p* [GeV]");
  HDs1_pstarDATABkg.SetYTitle("");  
  HDs1_pstarDATABkg.Add(&HDs1_pstarDATABkgLow,.5);
  HDs1_pstarDATABkg.Add(&HDs1_pstarDATABkgHigh,.5);
  HDs1_pstarDATABkg.SetNormFactor(NbkgCands);
   
  Canvas.Clear();
  HDs1_pstarDATABkgLow.GetYaxis()->SetRangeUser(0,1.1*HDs1_pstarDATABkgLow.GetMaximum());
  HDs1_pstarDATABkgLow.SetMarkerStyle(2);
  HDs1_pstarDATABkgLow.SetMarkerSize(1.2);
  HDs1_pstarDATABkgLow.Draw("p");
  HDs1_pstarDATABkgHigh.SetMarkerStyle(3);
  HDs1_pstarDATABkgHigh.SetMarkerSize(1.2);
  HDs1_pstarDATABkgHigh.Draw("psame");
  //HDs1_pstarDATABkg.Smooth(10);
  HDs1_pstarDATABkg.Draw("same");
  Canvas.Print(fname);
 
  ///now make the subtraction
  TH1F* HDs1_pstarDATA=(TH1F*)HDs1_pstarDATASigPlusBkg.Clone();
  HDs1_pstarDATA->Add(&HDs1_pstarDATABkg,-1);
 
  Canvas.Clear();
  HDs1_pstarDATASigPlusBkg.GetYaxis()->SetRangeUser(0,HDs1_pstarDATASigPlusBkg.GetMaximum());
  HDs1_pstarDATASigPlusBkg.Draw();
  HDs1_pstarDATA->Draw("same");
  HDs1_pstarDATABkg.Draw("same");
  Canvas.Print(fname);
  
 
  //-----------------------
  //Determine the corrected p* distribution
  //------------
  TH1F* HDs1_pstarCorr=(TH1F*)HDs1_pstarDATA->Clone();
  HDs1_pstarCorr->Divide(HDs1_pstareff);
  
  //----------------------
  //Determine Torstens  p* distribution
  //---------------------
  Float_t lumiscalefactor=.59; //Run1-4 is 59% of Run1-5 Data
  cout<<"Enter Luminosity correction factor: (.59 for Run1-5 ->Run1-4)"<<endl;
  cin>>lumiscalefactor;


  TH1F* HDs1Torsten_pstarUncorr=(TH1F*)HDs1_pstarCorr->Clone();
  HDs1Torsten_pstarUncorr->Multiply(HDs1Torsten_pstareff);
  HDs1Torsten_pstarUncorr->Scale(lumiscalefactor);//scale down to Run1-4 Lumi 
  //if(_Mode=="D0ToKPi")HDs1Torsten_pstarUncorr->SetBinContent(1,0);
  //if(_Mode=="D0ToKPi")HDs1Torsten_pstarUncorr->SetBinContent(1,.9*HDs1Torsten_pstarUncorr->GetBinContent(2));//extrapolate first bin
  cout<<"My Eff. corrected Total count="<< HDs1Torsten_pstarUncorr->Integral()<<endl;  
  if(_Mode=="D0ToKPi")cout<<"Torstens Total count="<<2401<<endl;
  if(_Mode=="D0ToK3Pi")cout<<"Torstens Total count="<<2959<<endl;  
 
  //calculate The error in HDs1Torsten_pstarUncorr
  //Torstens distribution is  (mypstar-mybkg)*transform*lumiscalefactor 
  Float_t ErrorMyBkg;
  //   Float_t ErrorMyData;
  Float_t totalerror;

  Float_t RecoTo;
  Float_t RecoMy;
  Float_t ErrorRecoTo;
  Float_t ErrorRecoMy;
  Float_t transform=0;
  Float_t ErrorTransform;
  for(Int_t i=1;i<=HDs1Torsten_pstarUncorr->GetNbinsX();i++){

    RecoTo=HDs1Torsten_pstar.GetBinContent(i);
    RecoMy=HDs1_pstar.GetBinContent(i);
    ErrorRecoTo=sqrt(RecoTo);
    ErrorRecoMy=sqrt(RecoMy);   
    if(RecoTo>0&&RecoMy>0){
      transform=lumiscalefactor*RecoTo/RecoMy;
      ErrorTransform=sqrt( pow(ErrorRecoTo/RecoTo,2)+ pow(ErrorRecoMy/RecoMy,2));//fractional error
    }else ErrorTransform=0;
       
    ErrorMyBkg=sqrt(HDs1_pstarDATABkg.GetBinContent(i));
          
    //totalerror=sqrt(pow(HDs1Torsten_pstarUncorr->GetBinContent(i)*ErrorTransform,2) +  pow(ErrorMyBkg,2)) ;//fractional error
    if(HDs1_pstarDATA->GetBinContent(i)>0)
      totalerror=HDs1Torsten_pstarUncorr->GetBinContent(i)*sqrt( pow(ErrorMyBkg/HDs1_pstarDATA->GetBinContent(i),2)+ ErrorTransform*ErrorTransform);
    else totalerror=0;
    
    
    //HDs1Torsten_pstarUncorr->SetBinContent(i, HDs1Torsten_pstarUncorr->GetBinContent(i));
    HDs1Torsten_pstarUncorr->SetBinError(i,totalerror);
    cout<<(int) HDs1Torsten_pstarUncorr->GetBinContent(i)<<",";
  }
  cout<<endl;
  
  

  Canvas.Clear();
  if(HDs1_pstarDATA->GetMaximum()>HDs1Torsten_pstarUncorr->GetMaximum()){
    HDs1_pstarDATA->Draw();
    HDs1Torsten_pstarUncorr->Draw("same");  
  }else {
    HDs1Torsten_pstarUncorr->Draw();  
    HDs1_pstarDATA->Draw("same");
  }
  Canvas.Print(fname);
  
//   Canvas.Clear();
//   HDs1_pstarDATA->GetYaxis()->SetRangeUser(0,HDs1_pstarDATA->GetMaximum());
//   HDs1_pstarDATA->Draw("pe");
//   Canvas.Print(fname);


  //Compare Torstens DATA p* distribution with my eff. corrected DATA 
  TH1F HTorstenpstar("HTorstenpstar","Torsten p*",npstarbins,pstarmin,pstarmax);
  Float_t y[11]={130,144,150,197,235,300,0/*330*/,300,276,260,0}; 
  Float_t y2[11]={158,160,226,267,295,364,382,429,410,274,0};
  for(Int_t i=0;i<npstarbins;i++){    
    if(_Mode=="D0ToKPi")HTorstenpstar.SetBinContent(i+1,y[i]);  
    if(_Mode=="D0ToK3Pi")HTorstenpstar.SetBinContent(i+1,y2[i]);  
  }
  

  ///not p* eff correction
  Canvas.Clear();
  HDs1_pstarDATA->Scale(lumiscalefactor*.16/.083);
  HDs1_pstarDATA->SetMarkerSize(1);
  HDs1_pstarDATA->SetMarkerColor(2);
  HDs1_pstarDATA->Draw("p");
  HTorstenpstar.SetMarkerSize(1);
  HTorstenpstar.Draw("psame");
  Canvas.Print(fname);


  //set content of some bins (which Torsten doest not show) to 0
  Int_t modeKPibin7Content=0;
  Int_t modeKPibin11Content=0;
  Int_t modeK3Pibin11Content=0;
  if(_Mode=="D0ToKPi"){
    modeKPibin7Content=(int)HDs1Torsten_pstarUncorr->GetBinContent(7);
    //HDs1Torsten_pstarUncorr->SetBinContent(7,0);
    //HDs1Torsten_pstarUncorr->SetBinError(7,0);
    modeKPibin11Content=(int)HDs1Torsten_pstarUncorr->GetBinContent(11);
    //HDs1Torsten_pstarUncorr->SetBinContent(11,0);
    //HDs1Torsten_pstarUncorr->SetBinError(11,0);
  }
  if(_Mode=="D0ToK3Pi"){   
    modeK3Pibin11Content=(int)HDs1Torsten_pstarUncorr->GetBinContent(11);
    //HDs1Torsten_pstarUncorr->SetBinContent(11,0);
    //HDs1Torsten_pstarUncorr->SetBinError(11,0);   
  }
 
  Canvas.Clear();
  HDs1Torsten_pstarUncorr->GetYaxis()->SetRangeUser(0,1.1*HDs1Torsten_pstarUncorr->GetMaximum());
  HDs1Torsten_pstarUncorr->SetLineColor(2);
  HDs1Torsten_pstarUncorr->SetMarkerColor(2);
  HDs1Torsten_pstarUncorr->SetMarkerStyle(4);
  HDs1Torsten_pstarUncorr->SetMarkerSize(1.7);
  HDs1Torsten_pstarUncorr->SetStats(kFALSE); 
  HTorstenpstar.GetYaxis()->SetRangeUser(0,1.1*HTorstenpstar.GetMaximum());
  HTorstenpstar.SetMarkerColor(1);
  HTorstenpstar.SetMarkerStyle(26);
  HTorstenpstar.SetMarkerSize(1.7);
  HTorstenpstar.SetStats(kFALSE); 
  
  HDs1Torsten_pstarUncorr->Draw("pe");
  HTorstenpstar.Draw("psame");
  Canvas.Print(fname);
  
  //print out the integrals
  cout<<"My p* comparable count="<< HDs1Torsten_pstarUncorr->Integral()<<endl;
  cout<<"Torstens p* comparable count="<<HTorstenpstar.Integral()<<endl;

  //-------------------------------------
  //Compare the Eff corrected D0To2ProngDcj to Torstens Skim
  //-------------------------------------------
  TH1F HMyEffCorrpstar("HMyEffCorrpstar","Torsten p*",npstarbins,pstarmin,pstarmax);
  Float_t w[11]={104,116,173,215,252,284,343,338,293,276,183};
  Float_t w2[11]={72,111,173,208,230,288,301,318,332,238,171};
  for(Int_t i=0;i<npstarbins;i++){    
    if(_Mode=="D0ToKPi")HMyEffCorrpstar.SetBinContent(i+1,w[i]);  
    if(_Mode=="D0ToK3Pi")HMyEffCorrpstar.SetBinContent(i+1,w2[i]);  
  }
   
  //return bin contents to normal
  if(_Mode=="D0ToKPi"){
    //HDs1Torsten_pstarUncorr->SetBinContent(7,modeKPibin7Content);
    //HDs1Torsten_pstarUncorr->SetBinContent(11,modeKPibin11Content);
  }
  if(_Mode=="D0ToK3Pi"){   
    //HDs1Torsten_pstarUncorr->SetBinContent(11,modeK3Pibin11Content);
  }
  
  Canvas.Clear();
  HDs1Torsten_pstarUncorr->GetYaxis()->SetRangeUser(0,1.1*HDs1Torsten_pstarUncorr->GetMaximum());
  HDs1Torsten_pstarUncorr->SetMarkerColor(4);
  HDs1Torsten_pstarUncorr->SetMarkerStyle(26);
  HDs1Torsten_pstarUncorr->SetMarkerSize(1.7);
  HDs1Torsten_pstarUncorr->SetStats(kFALSE); 
  HMyEffCorrpstar.GetYaxis()->SetRangeUser(0,1.1*HMyEffCorrpstar.GetMaximum());
  HMyEffCorrpstar.SetMarkerColor(2);
  HMyEffCorrpstar.SetMarkerStyle(4);
  HMyEffCorrpstar.SetMarkerSize(1.7);
  HMyEffCorrpstar.SetStats(kFALSE); 
  
  HDs1Torsten_pstarUncorr->Draw("p");
  HMyEffCorrpstar.Draw("psame");
  Canvas.Print(fname);
  

//   Int_t firstintegralbin=1;
//   cout<<"Enter First bin to integrate:(1=TorstenKPi,2=KPi)"<<endl;
//   cin>>firstintegralbin;

//   cout<<"Torsten Count="<< HTorstenpstar.Integral(firstintegralbin,npstarbins)<<"  MyCount="<<HDs1Torsten_pstarUncorr->Integral(firstintegralbin,npstarbins)<<endl;


  Canvas.Print(fname+"]");
  
  temp->Close();
  Close();

  delete HDs1_pstareff;
  delete HDs1Torsten_pstareff;
  delete HDs1_pstarDATA;
  delete HDs1_pstarCorr;
  delete HDs1Torsten_pstarUncorr;
  delete HDs1_pstarTransform;

  return 1;
}
