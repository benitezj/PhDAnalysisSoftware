#include "../../MACROS/TreeChain.C"
#include "DstPi0Analysis.h"

void analyzeDstPi0(TString file="0"){
  
  ifstream ParameterInput;
  if(file!="0"){
    cout<<"Going to use config file: "<<file<<endl;
    ParameterInput.open((const char*)(file));      
  }

  ////read in preferences
  Int_t WhichDATASet=-1;
  
  if(file!="0")ParameterInput>>WhichDATASet;
  if(WhichDATASet==-1){  
    cout<<"Which Data Set?"<<endl;
    cout<<" "<<"0 =KPi OnPeak+OffPeak data"<<endl;
    cout<<" "<<"1 =K3pi OnPeak+OffPeak data"<<endl;
    // 	cout<<" "<<"2 =Kpi from D2420"<<endl;
    // 	cout<<" "<<"3 =Kpi from D2460"<<endl;
    // 	cout<<" "<<"4 =K3pi from D2420"<<endl;
    // 	cout<<" "<<"5 =K3pi from D2460"<<endl;
    cout<<"Enter id>";
    cin>>WhichDATASet;
    cout<<endl;
  }
  
  Int_t ReduceNtuple=-1;
  if(file!="0")ParameterInput>>ReduceNtuple;
  while(ReduceNtuple!=1&&ReduceNtuple!=0){
    cout<<"Reduce ntuple?(1/0)"<<endl;
    cin>>ReduceNtuple;
  }
  
  TChain* chain=new TChain("ntp1"); 
  Int_t MatterOrAntiMatter=0;
  Int_t DstPiLund=0;
  TString OutputDir;
  Int_t D0Mode=0;
  Int_t TruthMatch=0;
  TString myntupledir="DstarCharged/DstarPi0";
  

  if(WhichDATASet==0){
    OutputDir=myntupledir+"/Kpi";
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
      TreeChain(chain,1,53,myntupledir+"/DATARun6On");
      TreeChain(chain,1,4,myntupledir+"/DATARun6Off");
    }
    MatterOrAntiMatter=0;   
    TruthMatch=0;
    DstPiLund=523;//B*+
    D0Mode=1;
  }

  if(WhichDATASet==1){
    OutputDir=myntupledir+"/K3pi";
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
      TreeChain(chain,1,53,myntupledir+"/DATARun6On");
      TreeChain(chain,1,4,myntupledir+"/DATARun6Off");
    }
    MatterOrAntiMatter=0;   
    TruthMatch=0;
    DstPiLund=523;//B*+
    D0Mode=2;
  }

 //  if(WhichDATASet==1){//for resolution vs. mass
//     OutputDir=myntupledir+"/MC/DstRandPi";
//     if(1==ReduceNtuple){
//       TreeChain(chain,1,8,OutputDir+"/SP-1138");
//       TreeChain(chain,1,24,OutputDir+"/SP-1139");
//     }
//     MatterOrAntiMatter=0;   
//     TruthMatch=1;
//     DstPiLund=523;//B*+
//     D0Mode=0;
//   }
//   if(WhichDATASet==2){
//     OutputDir=myntupledir+"/MC/D2420/Kpi";
//     if(1==ReduceNtuple){
//       TreeChain(chain,1,24,OutputDir+"/SP-1139");
//       TreeChain(chain,1,22,OutputDir+"/SP-1140");
      
//     }
//     MatterOrAntiMatter=0;   
//     TruthMatch=1;
//     DstPiLund=10423;
//     D0Mode=1;
//   }
//   if(WhichDATASet==3){
//     OutputDir=myntupledir+"/MC/D2460/Kpi";
//     if(1==ReduceNtuple){
//       TreeChain(chain,1,24,OutputDir+"/SP-1139");
//       TreeChain(chain,1,22,OutputDir+"/SP-1140");
//     }
//     MatterOrAntiMatter=0;   
//     TruthMatch=1;
//     DstPiLund=425;
//     D0Mode=1;
//   }
//   if(WhichDATASet==4){
//     OutputDir=myntupledir+"/MC/D2420/K3pi";
//     if(1==ReduceNtuple){
//       TreeChain(chain,1,8,OutputDir+"/SP-1137");
//       TreeChain(chain,1,8,OutputDir+"/SP-1138");
//     }
//     MatterOrAntiMatter=0;   
//     TruthMatch=1;
//     DstPiLund=10423;
//     D0Mode=2;
//   }
//   if(WhichDATASet==5){
//     OutputDir=myntupledir+"/MC/D2460/K3pi";
//     if(1==ReduceNtuple){
//       TreeChain(chain,1,8,OutputDir+"/SP-1137");
//       TreeChain(chain,1,8,OutputDir+"/SP-1138");
//     }
//     MatterOrAntiMatter=0;   
//     TruthMatch=1;
//     DstPiLund=425;
//     D0Mode=2;
//   }

  

  if(WhichDATASet==8){
    OutputDir="DstarCharged/DstarPi0/KpiAndK3pi";
    MatterOrAntiMatter=0;   
    TruthMatch=0;
    DstPiLund=myDstPiChargedLund;
    D0Mode=1;
  }



//Ask if default plots are wanted
//  Int_t PlotNtuple=-1;
//   while(PlotNtuple!=1&&PlotNtuple!=0){
//     cout<<"Plot the Ntuple?(1/0)"<<endl;
//     cin>>PlotNtuple;
//   }

//   //For truth matched MC ask if resolution should be determined 
//   Int_t DetermineResolution=-1;
//   if(TruthMatch){
//     if(file!="0")ParameterInput>>DetermineResolution;
//     while(DetermineResolution!=1&&DetermineResolution!=0){
//       cout<<"Determine DstPi0 DeltaM resolution?(1/0)"<<endl;
//       cin>>DetermineResolution; 
//     }
//   }

//   /////////Ask if we need to fit the mass peak
//   Int_t DrawFitDstPi0DM=-1; 
//   if(!TruthMatch){
//     if(file!="0")ParameterInput>>DrawFitDstPi0DM;
//     while(DrawFitDstPi0DM!=1&&DrawFitDstPi0DM!=0){
//       cout<<"Draw the Fit of DstPi0 DeltaM?(1/0)"<<endl;
//       cin>>DrawFitDstPi0DM;
//     }
//   }
//   Bool_t fit=0;
//   Int_t shape1=0;
//   Int_t shape2=0;
//   Bool_t res=0;
//   Bool_t eff=0;
//   Bool_t AddK3pi=0;
//   Int_t fileid=0;
//   if(1==DrawFitDstPi0DM){
//     cout<<"fit?(1/0)"<<endl;
//     if(file!="0")ParameterInput>>fit;
//     else cin>>fit;   
//     if(fit){   
//       cout<<"shape1?(0,1,2,3)"<<endl;
//       if(file!="0")ParameterInput>>shape1;
//       else cin>>shape1;
//       cout<<"shape2?(0,1,2,3)"<<endl;
//       if(file!="0")ParameterInput>>shape2;
//       else cin>>shape2;
//       cout<<"resolution?(1/0)"<<endl;
//       if(file!="0")ParameterInput>>res;
//       else cin>>res;
//       if(res){
// 	cout<<"efficiency?(1/0)"<<endl;
// 	if(file!="0")ParameterInput>>eff;
// 	else cin>>eff;
//       }
//       cout<<"AddK3pi?(1/0)"<<endl;
//       if(file!="0")ParameterInput>>AddK3pi;
//       else cin>>AddK3pi;
 
//       //cout<<"fileid?"<<endl;
//       //cin>>fileid;
//       //Determine the file id
//       if(shape1==0&&shape2==0)fileid=0;
//       if(shape1==1&&shape2==0)fileid=1;
//       if(shape1==0&&shape2==1)fileid=2;
//       if(shape1==1&&shape2==1)fileid=3;
//       if(shape1==2&&shape2==1)fileid=4;
//       if(shape1==1&&shape2==2)fileid=5;
//       if(shape1==2&&shape2==2)fileid=6;
//       if(shape1==3&&shape2==2)fileid=7;
//       if(shape1==2&&shape2==3)fileid=8;
//       if(shape1==3&&shape2==3)fileid=9; 
//       if(res){
// 	fileid+=20;
// 	if(eff)fileid+=20;
//       }
      
//     }
//   }


//   Int_t DrawFitDstPi0DM2=-1; 
//   if(!TruthMatch){
//     if(file!="0")ParameterInput>>DrawFitDstPi0DM2;
//     while(DrawFitDstPi0DM2!=1&&DrawFitDstPi0DM2!=0){
//       cout<<"Draw the Fit of DstPi0 DeltaM With Wrong Sign?(1/0)"<<endl;
//       cin>>DrawFitDstPi0DM2;
//     }
//   }

  Int_t PlotDMVsPstarCosHel=-1; 
  if(file!="0")ParameterInput>>PlotDMVsPstarCosHel;
  while(PlotDMVsPstarCosHel!=1&&PlotDMVsPstarCosHel!=0){
    cout<<"Draw deltaM vs p* and coshel?(1/0)"<<endl;
    cin>>PlotDMVsPstarCosHel;
  }
  
//   Int_t DrawFitWS=-1; 
//   if(!TruthMatch){
//     if(file!="0")ParameterInput>>DrawFitWS;
//     while(DrawFitWS!=1&&DrawFitWS!=0){
//       cout<<"Draw the Fit of Wrong Sign?(1/0)"<<endl;
//       cin>>DrawFitWS;
//     }
//   }

  
//   Int_t DrawFitDMLowCosHel=-1;
//   Int_t WhichBumpShape=0;
//   if(!TruthMatch){
//     if(file!="0")ParameterInput>>DrawFitDMLowCosHel;
//     while(DrawFitDMLowCosHel!=1&&DrawFitDMLowCosHel!=0){
//       cout<<"Draw the Fit deltaM for Low CosHel?(1/0)"<<endl;
//       cin>>DrawFitDMLowCosHel;
//     }
//     if(DrawFitDMLowCosHel){
//       cout<<"shape?(0,1,2)"<<endl;
//       cin>>WhichBumpShape;
//     }
//   }

  
  //For signal MC draw the corresponding background plot
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
    


  Bool_t RunInTestMode=0;
  if(ReduceNtuple){
    cout<<"Run In Test Mode?(1/0)"<<endl;
    if(file!="0")ParameterInput>>RunInTestMode;
    else cin>>RunInTestMode;
  }
  
  ParameterInput.close();
  ParameterInput.clear();

  //create the analysis object
  DstPi0Analysis dstpianalysis(chain,TruthMatch,MatterOrAntiMatter,DstPiLund,OutputDir,D0Mode);

  Int_t pass=1;
  
  if(1==ReduceNtuple){
    if(1==pass)pass=dstpianalysis.ReduceNtuple(RunInTestMode);
    delete chain;
  }

//   if(1==PlotNtuple)
//     if(1==pass)pass=dstpianalysis.ReadReducedNtuple();

//   if(1==DetermineResolution&&TruthMatch)
//     if(1==pass)pass=dstpianalysis.ResolutionEff();

//   if(1==DrawFitDstPi0DM){
//     if(1==pass)pass=dstpianalysis.RooFitMassPeak(fit,shape1,shape2,res,eff,AddK3pi,fileid);
//   }

//   if(1==DrawFitDstPi0DM2){
//     if(1==pass)pass=dstpianalysis.FitMassPeak(1,0,1,1,1,1);
//   }

  if(1==PlotDMVsPstarCosHel){
    if(1==pass)pass=dstpianalysis.PlotDMVsPstarCosHel();
  }
  
//   if(1==DrawFitWS){
//     if(1==pass)pass=dstpianalysis.RooFitWS();
//   }

//   if(1==DrawFitDMLowCosHel){   
//     if(1==pass)pass=dstpianalysis.RooFitDMLowCosHel(WhichBumpShape);
//   }

 
  //if(1==DrawBkgPlot)
  //if(1==pass)pass=dstpianalysis.DrawBkgPlot(1,1,1,1,1);
  
//   if(1==DrawBkgSubMass){
//     cout<<"pstar cut="<<endl;
//     Float_t pstarcut=3.2;
//     cin>>pstarcut;
//     cout<<"helicity cut="<<endl;
//     Float_t helicitycut=.5;
//     cin>>helicitycut;
//     if(1==pass)pass=dstpianalysis.DrawBkgSubMass(.01,pstarcut,helicitycut);
//   }

  if(1==pass)cout<<"Analysis passed."<<endl;
  else cout<<"Analysis failed."<<endl;


  return;
}
