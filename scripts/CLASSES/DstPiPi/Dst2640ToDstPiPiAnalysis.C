#include "../../MACROS/TreeChain.C"
#include "Dst2640Analysis.h"

Dst2640Analysis* analyzeDst2640(Int_t WhichHome, Int_t WhichDATASet,Int_t ReduceNtuple){
  ////read in preferences
 
  if(WhichDATASet>=0){
    cout<<"Which Data Set?"<<endl
	<<" "<<"-1=test"<<endl
	<<" "<<"0=all KPi data"<<endl
	<<" "<<"1=SP-7787"<<endl
	<<" "<<"2=all K3Pi DATA"<<endl
	<<" "<<"3=SP-7789"<<endl
	<<" "<<"4=KPi SP-1005"<<endl
	<<" "<<"5=KPi SP-998"<<endl
	<<" "<<"6=KPi SP-1235"<<endl
	<<" "<<"7=KPi SP-1237"<<endl
	<<" "<<"8=KPi SP-3429"<<endl
	<<" "<<"9=K3Pi SP-1005"<<endl
	<<" "<<"10=K3Pi SP-998"<<endl
	<<" "<<"11=K3Pi SP-1235"<<endl
	<<" "<<"12=K3Pi SP-1237"<<endl
	<<" "<<"13=K3Pi SP-3429"<<endl
	<<" "<<"100=all KPi OnPeak data from SKIM"<<endl
	<<" "<<"101=all KPi data Run1 (On+Off at CNF)"<<endl
	<<" "<<"102=all KPi OffPeak data from SKIM"<<endl
	<<" "<<"103=all K3Pi OnPeak data from SKIM"<<endl
	<<" "<<"104=all K3Pi OffPeak data from SKIM"<<endl
	<<" "<<"105=all K3Pi data Run1 (On+Off at CNF)"<<endl
	<<" "<<"106=all KPi+K3Pi OnPeak+OffPeak data from SKIM"<<endl
	<<" "<<"107=all KPi+K3Pi OnPeak+OffPeak data from SKIM (new Cuts)"<<endl
	<<" "<<"108=all KPi+K3Pi Run5OnPeak data from SKIM (new Cuts)"<<endl
	<<"Enter id>";
    cin>>WhichDATASet;
    cout<<endl;
  }


  while(ReduceNtuple!=1&&ReduceNtuple!=0){
    cout<<"Reduce ntuple?(1/0)"<<endl;
    cin>>ReduceNtuple;
  }


  
  TChain* chain=new TChain("ntp1"); 
  Int_t MatterOrAntiMatter=0;
  TString Mode;
  TString OutPutDir;
  Int_t TruthMatch=0;
  //TString PstarComparisonFile;
  TString myntupledir;
  if(WhichHome==1) myntupledir="/u2/benitezj/physics/NTUPLES/Dst2640ToDstPiPi";
  if(WhichHome==2) myntupledir="/mnt/babar/charm/benitezj/NTUPLES/Dst2640ToDstPiPi"; 
  TString DMResParsDir;
 
  
  if(WhichDATASet==-2){//Test dataset
    OutPutDir=myntupledir+"/D0ToKPi/SP-1005";
    if(1==ReduceNtuple)TreeChain(chain,1,1,OutPutDir);   
    MatterOrAntiMatter=0;
    Mode="D0ToKPi"; 
    TruthMatch=0;
    //PstarComparisonFile=myntupledir+"/D0ToKPi/SP-7787/Dst2640Reduced.root";
    DMResParsDir=myntupledir+"/D0ToKPi/SP-7787";
  }
  if(WhichDATASet==-1){//Test dataset
    OutPutDir=myntupledir+"/D0ToKPi/SP-7787";
    if(1==ReduceNtuple)TreeChain(chain,1,1,OutPutDir);   
    MatterOrAntiMatter=1;
    Mode="D0ToKPi"; 
    TruthMatch=1;
    //PstarComparisonFile=myntupledir+"/D0ToKPi/SP-7787/Dst2640Reduced.root";
    DMResParsDir=myntupledir+"/D0ToKPi/SP-7787";
  }
  if(WhichDATASet==0){
    if(1==ReduceNtuple){
      TreeChain(chain,1,4,myntupledir+"/D0ToKPi/DATARun1");//
      TreeChain(chain,1,12,myntupledir+"/D0ToKPi/DATARun2");//-
      TreeChain(chain,1,6,myntupledir+"/D0ToKPi/DATARun3");//-
      TreeChain(chain,1,20,myntupledir+"/D0ToKPi/DATARun4");//--
      TreeChain(chain,1,25,myntupledir+"/D0ToKPi/DATARun5");//--
    }
    MatterOrAntiMatter=0;
    Mode="D0ToKPi";
    OutPutDir=myntupledir+"/D0ToKPi";
    TruthMatch=0;
    //PstarComparisonFile=myntupledir+"/D0ToKPi/SP-7787/Dst2640Reduced.root";
    DMResParsDir=myntupledir+"/D0ToKPi/SP-7787";    
  }
  if(WhichDATASet==1){
    OutPutDir=myntupledir+"/D0ToKPi/SP-7787";
    if(1==ReduceNtuple)TreeChain(chain,1,22,OutPutDir);
    MatterOrAntiMatter=1;
    Mode="D0ToKPi";
    TruthMatch=1;
    //PstarComparisonFile=myntupledir+"/D0ToKPi/SP-7787/Dst2640Reduced.root";
    DMResParsDir=myntupledir+"/D0ToKPi/SP-7787";
  }
  if(WhichDATASet==2){
    if(1==ReduceNtuple){
      TreeChain(chain,1,6,myntupledir+"/D0ToK3Pi/DATARun1");//assuming 20k events/job 
      TreeChain(chain,1,17,myntupledir+"/D0ToK3Pi/DATARun2");//-
      TreeChain(chain,1,10,myntupledir+"/D0ToK3Pi/DATARun3");//-
      TreeChain(chain,1,26,myntupledir+"/D0ToK3Pi/DATARun4");//-
      TreeChain(chain,1,30,myntupledir+"/D0ToK3Pi/DATARun5");//-
    }   
    MatterOrAntiMatter=0;
    Mode="D0ToK3Pi";
    OutPutDir=myntupledir+"/D0ToK3Pi";
    TruthMatch=0;
    //PstarComparisonFile=myntupledir+"/D0ToK3Pi/SP-7789/Dst2640Reduced.root";
    DMResParsDir=myntupledir+"/D0ToK3Pi/SP-7789";
  }
  if(WhichDATASet==3){
    OutPutDir=myntupledir+"/D0ToK3Pi/SP-7789";
    if(1==ReduceNtuple)TreeChain(chain,1,22,OutPutDir);   
    MatterOrAntiMatter=1;
    Mode="D0ToK3Pi";
    TruthMatch=1;
    //PstarComparisonFile=myntupledir+"/D0ToK3Pi/SP-7789/Dst2640Reduced.root";
    DMResParsDir=myntupledir+"/D0ToK3Pi/SP-7789";
  }
  if(WhichDATASet==4){
    MatterOrAntiMatter=0;
    Mode="D0ToKPi";
    TruthMatch=0;
    OutPutDir=myntupledir+"/"+Mode+"/SP-1005";
    if(1==ReduceNtuple)TreeChain(chain,1,153,OutPutDir);
  }
  if(WhichDATASet==5){
    MatterOrAntiMatter=0;
    Mode="D0ToKPi";
    TruthMatch=0;
    OutPutDir=myntupledir+"/"+Mode+"/SP-998";
    if(1==ReduceNtuple)TreeChain(chain,1,56,OutPutDir);
  }
  if(WhichDATASet==6){
    MatterOrAntiMatter=0;
    Mode="D0ToKPi";
    TruthMatch=0;
    OutPutDir=myntupledir+"/"+Mode+"/SP-1235";
    if(1==ReduceNtuple)TreeChain(chain,1,10,OutPutDir);
  }
  if(WhichDATASet==7){
    MatterOrAntiMatter=0;
    Mode="D0ToKPi";
    TruthMatch=0;
    OutPutDir=myntupledir+"/"+Mode+"/SP-1237";
    if(1==ReduceNtuple)TreeChain(chain,1,5,OutPutDir);
  }
  if(WhichDATASet==8){
    MatterOrAntiMatter=0;
    Mode="D0ToKPi";
    TruthMatch=0;
    OutPutDir=myntupledir+"/"+Mode+"/SP-3429";
    if(1==ReduceNtuple)TreeChain(chain,1,2,OutPutDir);
  }

  if(WhichDATASet==9){
    MatterOrAntiMatter=0;
    Mode="D0ToK3Pi";
    TruthMatch=0;
    OutPutDir=myntupledir+"/"+Mode+"/SP-1005";
    if(1==ReduceNtuple)TreeChain(chain,1,93,OutPutDir);//total 93
    
  }
  if(WhichDATASet==10){
    MatterOrAntiMatter=0;
    Mode="D0ToK3Pi";
    TruthMatch=0;
    OutPutDir=myntupledir+"/"+Mode+"/SP-998";
    if(1==ReduceNtuple)TreeChain(chain,1,9,OutPutDir);
  }
  if(WhichDATASet==11){
    MatterOrAntiMatter=0;
    Mode="D0ToK3Pi";
    TruthMatch=0;
    OutPutDir=myntupledir+"/"+Mode+"/SP-1235";
    if(1==ReduceNtuple)TreeChain(chain,1,13,OutPutDir);
  }
  if(WhichDATASet==12){
    MatterOrAntiMatter=0;
    Mode="D0ToK3Pi";
    TruthMatch=0;
    OutPutDir=myntupledir+"/"+Mode+"/SP-1237";
    if(1==ReduceNtuple)TreeChain(chain,1,19,OutPutDir);
  }
  if(WhichDATASet==13){
    MatterOrAntiMatter=0;
    Mode="D0ToK3Pi";
    TruthMatch=0;
    OutPutDir=myntupledir+"/"+Mode+"/SP-3429";
    if(1==ReduceNtuple)TreeChain(chain,1,1,OutPutDir);
  }

  if(WhichDATASet==100){
    myntupledir="DstarCharged/DstarPiPi";
    if(1==ReduceNtuple){
      TreeChain(chain,1,24,myntupledir+"/DATARun1");//
    }
    MatterOrAntiMatter=0;
    Mode="D0ToKPi";
    OutPutDir=myntupledir+"/DATARun1";
    TruthMatch=0;
  }
  if(WhichDATASet==101){//Run1 (On+Off) at CNAF    
    if(1==ReduceNtuple){
      TreeChain(chain,1,4,myntupledir+"/D0ToKPi/DATARun1");//
    }
    MatterOrAntiMatter=0;
    Mode="D0ToKPi";
    OutPutDir=myntupledir+"/D0ToKPi/DATARun1";
    TruthMatch=0;
  }
  if(WhichDATASet==102){
    myntupledir="DstarCharged/DstarPiPi";
    if(1==ReduceNtuple){
      TreeChain(chain,1,2,myntupledir+"/DATARun1Off");//
    }
    MatterOrAntiMatter=0;
    Mode="D0ToKPi";
    OutPutDir=myntupledir+"/DATARun1Off";
    TruthMatch=0;
  }
  if(WhichDATASet==103){
    myntupledir="DstarCharged/DstarPiPi";
    if(1==ReduceNtuple){
      TreeChain(chain,1,94,myntupledir+"/DATARun1/K3Pi");//
    }
    MatterOrAntiMatter=0;
    Mode="D0ToK3Pi";
    OutPutDir=myntupledir+"/DATARun1/K3Pi";
    TruthMatch=0;
  }
  if(WhichDATASet==104){
    myntupledir="DstarCharged/DstarPiPi";
    if(1==ReduceNtuple){
      TreeChain(chain,1,11,myntupledir+"/DATARun1Off/K3Pi");//
    }
    MatterOrAntiMatter=0;
    Mode="D0ToK3Pi";
    OutPutDir=myntupledir+"/DATARun1Off/K3Pi";
    TruthMatch=0;
  }
  if(WhichDATASet==105){//Run1 (On+Off) at CNAF    
    if(1==ReduceNtuple){
      TreeChain(chain,1,4,myntupledir+"/D0ToK3Pi/DATARun1");//
    }
    MatterOrAntiMatter=0;
    Mode="D0ToK3Pi";
    OutPutDir=myntupledir+"/D0ToK3Pi/DATARun1";
    TruthMatch=0;
  }
  if(WhichDATASet==106){
    myntupledir="DstarCharged/DstarPiPi";
    if(1==ReduceNtuple){
      TreeChain(chain,1,19,myntupledir+"/DATARun1On");
      TreeChain(chain,1,3,myntupledir+"/DATARun1Off");
      TreeChain(chain,1,62,myntupledir+"/DATARun2On");
      TreeChain(chain,1,7,myntupledir+"/DATARun2Off"); 
      TreeChain(chain,1,33,myntupledir+"/DATARun3On");
      TreeChain(chain,1,3,myntupledir+"/DATARun3Off");
      TreeChain(chain,1,99,myntupledir+"/DATARun4On");
      TreeChain(chain,1,10,myntupledir+"/DATARun4Off");
      TreeChain(chain,1,131,myntupledir+"/DATARun5On");
      TreeChain(chain,1,14,myntupledir+"/DATARun5Off");
    }
    MatterOrAntiMatter=0;
    Mode="D0ToKPi";
    OutPutDir=myntupledir+"/";
    TruthMatch=0;
  }
  if(WhichDATASet==107){
    //myntupledir="DstarCharged/DstarPiPiNew";
    myntupledir="DstarCharged/DstarPiPi";
    if(1==ReduceNtuple){
      TreeChain(chain,1,19,myntupledir+"/DATARun1On");
      TreeChain(chain,1,3,myntupledir+"/DATARun1Off");
      TreeChain(chain,1,62,myntupledir+"/DATARun2On");
      TreeChain(chain,1,7,myntupledir+"/DATARun2Off"); 
      TreeChain(chain,1,33,myntupledir+"/DATARun3On");
      TreeChain(chain,1,3,myntupledir+"/DATARun3Off");
      TreeChain(chain,1,99,myntupledir+"/DATARun4On");
      TreeChain(chain,1,10,myntupledir+"/DATARun4Off");
      TreeChain(chain,1,131,myntupledir+"/DATARun5On");
      TreeChain(chain,1,14,myntupledir+"/DATARun5Off");
    }
    MatterOrAntiMatter=0;
    Mode="D0ToK3Pi";
    OutPutDir=myntupledir+"/";
    TruthMatch=0;
  }
  if(WhichDATASet==108){
    myntupledir="DstarCharged/DstarPiPi";
    if(1==ReduceNtuple){
      TreeChain(chain,1,131,myntupledir+"/DATARun5On");
    }
    MatterOrAntiMatter=0;
    Mode="D0ToKPi";
    OutPutDir=myntupledir+"/KpiRun5On";
    TruthMatch=0;
  }




//   //Ask if default plots are wanted
//   Int_t PlotNtuple=-1;
//   while(PlotNtuple!=1&&PlotNtuple!=0){
//     cout<<"Plot the Ntuple?(1/0)"<<endl;
//     cin>>PlotNtuple;
//   }

//   //For truth matched MC ask if resolution should be determined 
//   Int_t DetermineResolution=-1;
//   if(TruthMatch){
//     while(DetermineResolution!=1&&DetermineResolution!=0){
//       cout<<"Determine Dst2640 DeltaM resolution?(1/0)"<<endl;
//       cin>>DetermineResolution; 
//     }
//   }

//   //Ask if we need to fit the mass peak
//   Int_t DrawFitDst2640DM=-1; 
//   while(DrawFitDst2640DM!=1&&DrawFitDst2640DM!=0){
//     cout<<"Draw the Fit of Dst2640 DeltaM?(1/0)"<<endl;
//     cin>>DrawFitDst2640DM;
//   }
  
//   //if we want to fit mass peak
//   Int_t FitType;    
//   if(1==DrawFitDst2640DM){        
//     cout<<"Which Fit Type:(1=Binned,2=UnBinned)"<<endl;
//     cin>>FitType;
//   }

//   //For signal MC draw the corresponding background plot
//   Int_t DrawBkgPlot=0;    
//   if(WhichDATASet==1 || WhichDATASet==3){        
//     cout<<"DrawBkgPlot?"<<endl;
//     cin>>DrawBkgPlot;
//   }

//   //
//   Int_t DrawBkgSubMass=0;    
//   if(DrawBkgSubMass==0){        
//     //cout<<"DrawBkgSubMass?"<<endl;
//     //cin>>DrawBkgSubMass;
//   }
    

  //which cuts should be used
  Int_t WhichCuts=0;    
  //cout<<"Which Cuts?(0,1,..)"<<endl;
  //cin>>WhichCuts;

  Bool_t RunInTestMode=0;  
  if(1==ReduceNtuple){
    cout<<"Run In Test Mode?(1/0)"<<endl;
    cin>>RunInTestMode;
  }
 
  
  //create the analysis object
  Dst2640Analysis* dst2640analysis=new Dst2640Analysis(chain,TruthMatch,MatterOrAntiMatter,Mode,OutPutDir,WhichCuts,RunInTestMode);

//   Int_t pass=1;
  
//   if(1==ReduceNtuple){
//     if(1==pass)pass=dst2640analysis.ReduceNtuple();
//     delete chain;
//   }

//   if(1==PlotNtuple)
//     if(1==pass)pass=dst2640analysis.ReadReducedNtuple();

//   if(1==DetermineResolution&&TruthMatch)
//     if(1==pass)pass=dst2640analysis.FitDMResolution();

//   if(1==DrawFitDst2640DM){
//     Float_t helcut=-.2;
//     //cout<<"Enter Helicity Cosine cut:"<<endl;
//     //cin>>helcut;
//     //cout<<"Enter p* cut"<<endl;
//     Float_t pcut=3.5;
//     //cin>>pcut;
//     if(1==pass)pass=dst2640analysis.FitDst2640DM(FitType,DMResParsDir,helcut,pcut);
//   }
 
//   if(1==DrawBkgPlot)
//     if(1==pass)pass=dst2640analysis.DrawBkgPlot(1,1,1,1,1);
  
//   if(1==DrawBkgSubMass){
//     cout<<"pstar cut="<<endl;
//     Float_t pstarcut=3.2;
//     cin>>pstarcut;
//     cout<<"helicity cut="<<endl;
//     Float_t helicitycut=.5;
//     cin>>helicitycut;
//     if(1==pass)pass=dst2640analysis.DrawBkgSubMass(.01,pstarcut,helicitycut);
//   }

//   if(1==pass)cout<<"Analysis passed."<<endl;
//   else cout<<"Analysis failed."<<endl;


  return dst2640analysis;
}
