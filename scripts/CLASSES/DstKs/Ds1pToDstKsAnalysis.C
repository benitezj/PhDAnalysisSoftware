#include "../MACROS/TreeChain.C"
#include "../CLASSES/Ds1Analysis.h"

void analyzeDs1(Int_t WhichHome){
  ////read in preferences
  Int_t WhichDATASet=-1;
  cout<<"Which Data Set?"<<endl;
  cout<<" "<<"-1=test"<<endl;
  cout<<" "<<"0=KPi Run1-5"<<endl;
  cout<<" "<<"1=SP-7796"<<endl;
  cout<<" "<<"2=SP-7799"<<endl;
  cout<<" "<<"3=K3Pi Run1-5"<<endl;
  cout<<" "<<"4=SP-7798"<<endl;
  cout<<" "<<"5=SP-7801"<<endl;
  cout<<" "<<"6=KPi Run1-4 "<<endl;
  cout<<" "<<"7=K3Pi Run1-4 "<<endl;    
  cin>>WhichDATASet;

  
  Int_t ReduceNtuple=-1;
  while(ReduceNtuple!=1&&ReduceNtuple!=0){
    cout<<"Reduce ntuple?(1/0)"<<endl;
    cin>>ReduceNtuple;
  }


  /////
  TChain* chain=new TChain("ntp1");
  Int_t MatterOrAntiMatter=0;
  TString Mode;
  TString SubDir;
  Bool_t TruthMatch=false;
  TString PstarComparisonFile;
  TString myntupledir;
  if(WhichHome==1) myntupledir="/u2/benitezj/physics/NTUPLES";
  if(WhichHome==2) myntupledir="/mnt/babar/charm/benitezj/NTUPLES";
  TString DMResParsDir;
  Int_t WhichCuts=-1;

  if(WhichDATASet==-1){
    if(1==ReduceNtuple)TreeChain(chain,1,1,myntupledir+"/Ds1pToDstKs/D0ToKPi/SP-7796");
    MatterOrAntiMatter=1;
    Mode="D0ToKPi";
    SubDir=myntupledir+"/Ds1pToDstKs/D0ToKPi/SP-7796";
    TruthMatch=true;
    PstarComparisonFile=myntupledir+"/Ds1pToDstKs/D0ToKPi/SP-7796/Ds1pReduced.root";
    DMResParsDir=myntupledir+"/Ds1pToDstKs/D0ToKPi/SP-7796";
    WhichCuts=1;
  }
  if(WhichDATASet==0){
    if(1==ReduceNtuple){
      TreeChain(chain,1,4,myntupledir+"/Ds1pToDstKs/D0ToKPi/DATARun1");//28
      TreeChain(chain,1,12,myntupledir+"/Ds1pToDstKs/D0ToKPi/DATARun2");//84
      TreeChain(chain,1,6,myntupledir+"/Ds1pToDstKs/D0ToKPi/DATARun3");//45
      TreeChain(chain,1,20,myntupledir+"/Ds1pToDstKs/D0ToKPi/DATARun4");//141
      TreeChain(chain,1,25,myntupledir+"/Ds1pToDstKs/D0ToKPi/DATARun5");//174
    }
    MatterOrAntiMatter=0;
    Mode="D0ToKPi";
    SubDir=myntupledir+"/Ds1pToDstKs/D0ToKPi";
    TruthMatch=false;
    PstarComparisonFile=myntupledir+"/Ds1pToDstKs/D0ToKPi/SP-7796/Ds1pReduced.root";
    DMResParsDir=myntupledir+"/Ds1pToDstKs/D0ToKPi/SP-7796";
    WhichCuts=1;
  }
  if(WhichDATASet==1){
    if(1==ReduceNtuple)TreeChain(chain,1,31,myntupledir+"/Ds1pToDstKs/D0ToKPi/SP-7796");
    MatterOrAntiMatter=1;
    Mode="D0ToKPi";
    SubDir=myntupledir+"/Ds1pToDstKs/D0ToKPi/SP-7796";
    TruthMatch=true;
    PstarComparisonFile=myntupledir+"/Ds1pToDstKs/D0ToKPi/SP-7796/Ds1pReduced.root";
    DMResParsDir=myntupledir+"/Ds1pToDstKs/D0ToKPi/SP-7796";
  }
  if(WhichDATASet==2){
    if(1==ReduceNtuple)TreeChain(chain,1,31,myntupledir+"/Ds1pToDstKs/D0ToKPi/SP-7799");
    MatterOrAntiMatter=-1;
    Mode="D0ToKPi";
    SubDir=myntupledir+"/Ds1pToDstKs/D0ToKPi/SP-7799";
    TruthMatch=true;
    PstarComparisonFile=myntupledir+"/Ds1pToDstKs/D0ToKPi/SP-7796/Ds1pReduced.root";
    DMResParsDir=myntupledir+"/Ds1pToDstKs/D0ToKPi/SP-7799";

  }
  if(WhichDATASet==3){
    if(1==ReduceNtuple){
      TreeChain(chain,1,17,myntupledir+"/Ds1pToDstKs/D0ToK3Pi/DATARun1");//assuming 20k events/job 
      TreeChain(chain,1,48,myntupledir+"/Ds1pToDstKs/D0ToK3Pi/DATARun2");//-
      TreeChain(chain,1,25,myntupledir+"/Ds1pToDstKs/D0ToK3Pi/DATARun3");//-
      TreeChain(chain,1,73,myntupledir+"/Ds1pToDstKs/D0ToK3Pi/DATARun4");//-
      TreeChain(chain,1,85,myntupledir+"/Ds1pToDstKs/D0ToK3Pi/DATARun5");//-
    }
    MatterOrAntiMatter=0;
    Mode="D0ToK3Pi";
    SubDir=myntupledir+"/Ds1pToDstKs/D0ToK3Pi";
    TruthMatch=false;
    PstarComparisonFile=myntupledir+"/Ds1pToDstKs/D0ToK3Pi/SP-7798/Ds1pReduced.root";
    DMResParsDir=myntupledir+"/Ds1pToDstKs/D0ToK3Pi/SP-7798";
    WhichCuts=1;
  }
  if(WhichDATASet==4){
    if(1==ReduceNtuple)TreeChain(chain,1,31,myntupledir+"/Ds1pToDstKs/D0ToK3Pi/SP-7798");
    MatterOrAntiMatter=1;
    Mode="D0ToK3Pi";
    SubDir=myntupledir+"/Ds1pToDstKs/D0ToK3Pi/SP-7798";
    TruthMatch=true;
    PstarComparisonFile=myntupledir+"/Ds1pToDstKs/D0ToK3Pi/SP-7798/Ds1pReduced.root";
    DMResParsDir=myntupledir+"/Ds1pToDstKs/D0ToK3Pi/SP-7798";
  }
  if(WhichDATASet==5){
    if(1==ReduceNtuple)TreeChain(chain,1,31,myntupledir+"/Ds1pToDstKs/D0ToK3Pi/SP-7801");
    MatterOrAntiMatter=-1;
    Mode="D0ToK3Pi";
    SubDir=myntupledir+"/Ds1pToDstKs/D0ToK3Pi/SP-7801";
    TruthMatch=true;
    PstarComparisonFile=myntupledir+"/Ds1pToDstKs/D0ToK3Pi/SP-7798/Ds1pReduced.root";
    DMResParsDir=myntupledir+"/Ds1pToDstKs/D0ToK3Pi/SP-7801";    
  }
  if(WhichDATASet==6){
    if(1==ReduceNtuple){
      TreeChain(chain,1,7,myntupledir+"/Ds1pToDstKs/D0ToKPiTorsten/DATARun1");
      TreeChain(chain,1,20,myntupledir+"/Ds1pToDstKs/D0ToKPiTorsten/DATARun2");
      TreeChain(chain,1,11,myntupledir+"/Ds1pToDstKs/D0ToKPiTorsten/DATARun3");
      TreeChain(chain,1,29,myntupledir+"/Ds1pToDstKs/D0ToKPiTorsten/DATARun4");
      //TreeChain(chain,1,35,myntupledir+"/Ds1pToDstKs/D0ToKPiTorsten/DATARun5");
    }
    MatterOrAntiMatter=0;
    Mode="D0ToKPi";
    SubDir=myntupledir+"/Ds1pToDstKs/D0ToKPiTorsten";
    TruthMatch=false;
    PstarComparisonFile=myntupledir+"/Ds1pToDstKs/D0ToKPi/SP-7796/Ds1pReduced.root";
    DMResParsDir=myntupledir+"/Ds1pToDstKs/D0ToKPi/SP-7796";
    WhichCuts=0;
  }  
  if(WhichDATASet==7){
    if(1==ReduceNtuple){
      TreeChain(chain,1,7,myntupledir+"/Ds1pToDstKs/D0ToK3PiTorsten/DATARun1");
      TreeChain(chain,1,20,myntupledir+"/Ds1pToDstKs/D0ToK3PiTorsten/DATARun2");
      TreeChain(chain,1,11,myntupledir+"/Ds1pToDstKs/D0ToK3PiTorsten/DATARun3");
      TreeChain(chain,1,29,myntupledir+"/Ds1pToDstKs/D0ToK3PiTorsten/DATARun4");
      //TreeChain(chain,1,35,myntupledir+"/Ds1pToDstKs/D0ToK3PiTorsten/DATARun5");
    }
    MatterOrAntiMatter=0;
    Mode="D0ToK3Pi";
    SubDir=myntupledir+"/Ds1pToDstKs/D0ToK3PiTorsten";
    TruthMatch=false;
    PstarComparisonFile=myntupledir+"/Ds1pToDstKs/D0ToK3Pi/SP-7798/Ds1pReduced.root";
    DMResParsDir=myntupledir+"/Ds1pToDstKs/D0ToK3Pi/SP-7798";
    WhichCuts=0;
  }  


  Int_t PlotNtuple=-1;
  while(PlotNtuple!=1&&PlotNtuple!=0){
    cout<<"Plot the Ntuple?(1/0)"<<endl;
    cin>>PlotNtuple;
  }

  Int_t DetermineResolution=-1;
  if(TruthMatch){
    while(DetermineResolution!=1&&DetermineResolution!=0){
      cout<<"Determine Ds1 DeltaM resolution?(1/0)"<<endl;
      cin>>DetermineResolution; 
    }
  }

  Int_t DrawFitDs1DM=-1; 
  while(DrawFitDs1DM!=1&&DrawFitDs1DM!=0){
    cout<<"Draw the Fit of Ds1 DeltaM?(1/0)"<<endl;
    cin>>DrawFitDs1DM;
  }

  Int_t FitType;    
  if(1==DrawFitDs1DM){        
    cout<<"Which Fit Type:(1=Binned,2=UnBinned)"<<endl;
    cin>>FitType;
  }
  

//   Int_t WhichBinning=1;
//   if(!TruthMatch){
//     cout<<"WhichBinning?"<<endl;
//     cin>>WhichBinning;    
//   }
  

  if(WhichCuts==-1){
    cout<<"Which Cuts?"<<endl;
    cin>>WhichCuts;
  }



  //create the analysis object
  Ds1Analysis ds1analysis(chain,TruthMatch,MatterOrAntiMatter,Mode,SubDir,WhichCuts);

  Int_t pass=1;
  
  if(1==ReduceNtuple)
    if(1==pass)pass=ds1analysis.ReduceNtuple();

  if(1==PlotNtuple)
    if(1==pass)pass=ds1analysis.ReadReducedNtuple();

  if(1==DetermineResolution&&TruthMatch)
    if(1==pass)pass=ds1analysis.FitDMResolution();

  if(1==DrawFitDs1DM)
    if(1==pass)pass=ds1analysis.FitDs1DM(FitType,DMResParsDir,PstarComparisonFile);
  
  if(1==pass)cout<<"Analysis passed."<<endl;
  else cout<<"Analysis failed."<<endl;

  delete chain;

  return;
}
