#include "JDataBaseDst0Pi0.h"


ClassImp(JDataBaseDst0Pi0);
JDataBaseDst0Pi0::JDataBaseDst0Pi0()
{
  _WhichMode=1;//Default
  TruthMatch=kFALSE;
  SetCuts();
}

JDataBaseDst0Pi0::JDataBaseDst0Pi0(Int_t WhichMode,Bool_t truthMatch)
{
  _WhichMode=WhichMode;
  TruthMatch=truthMatch;
  SetCuts();
}

JDataBaseDst0Pi0::~JDataBaseDst0Pi0(){ 
}
void JDataBaseDst0Pi0::SetCuts(){

  Dst0Pi0MassNbins=120;//5MeV bins
  Dst0Pi0MassMin=2.2;
  Dst0Pi0MassMax=2.8;

  Dst0Pi0DeltaMNbins=90;//5MeV bins
  Dst0Pi0DeltaMMin=.2;
  Dst0Pi0DeltaMMax=.65;
  
  Dst0Pi0DeltaMRangeNbins=280;//5MeV bins
  Dst0Pi0DeltaMRangeMin=.1;
  Dst0Pi0DeltaMRangeMax=1.5; 

  Dst0Pi0DeltaMSignalNbins=30; 
  Dst0Pi0DeltaMSignalMin=.35;
  Dst0Pi0DeltaMSignalMax=.5;
  
  D0MassRangeNbins=100;//1MeV bins
  D0MassRangeMin=D0PDGMass-.050;
  D0MassRangeMax=D0PDGMass+.050;
  
  DstarDeltaMRangeNbins=300;//.1MeV bins
  DstarDeltaMRangeMin=.13;
  DstarDeltaMRangeMax=.16;

  if(TruthMatch){//MC: my own 2 (or 3) gauss fits 
    D0Mass=1.8650;
    DstarDeltaM=.14207;
    cout<<"Data Type: MC"<<endl;
  }else{//DATA: mean values taken from Torsten paper
    D0Mass=1.8636;
    DstarDeltaM=.14206;
    cout<<"Data Type: Data"<<endl;
  }


  ///////common cuts
  Dst0Pi0MinLogProbab=-3;//.1% prob 
  Dst0Pi0pstarMin=3.;
  Dst0Pi0pstarMax=5;
  DstarCosineMax=.5;
  DstarDeltaMMin=DstarDeltaM-.002;
  DstarDeltaMMax=DstarDeltaM+.002;

  if(_WhichMode==1){
    D0MassMin=D0Mass-.02;
    D0MassMax=D0Mass+.02;
  }
  if(_WhichMode==2){
    D0MassMin=D0Mass-.015;
    D0MassMax=D0Mass+.015;
  }

  ///define the cuts  
  for(Int_t cutid=0;cutid<NCUTS;cutid++){
    sprintf(cutstring[cutid],"0");
  }
  sprintf(cutstring[0],"dstpimctrue==1");
  sprintf(cutstring[1],"dstpilogvtxprob>%.2f&&gam1energy>.09&&gam2energy>.09",GetDst0Pi0MinLogProbab());
  sprintf(cutstring[2],"%.5f<dstpideltam&&dstpideltam<%.5f",GetDst0Pi0DeltaMRangeMin(),GetDst0Pi0DeltaMRangeMax());
  sprintf(cutstring[3],"%.5f<dstardeltam&&dstardeltam<%.5f",GetDstarDeltaMMin(),GetDstarDeltaMMax());
  sprintf(cutstring[4],"%.5f<d0mass&&d0mass<%.5f",GetD0MassMin(),GetD0MassMax());
  sprintf(cutstring[5],"%.3f<dstpipstar&&dstpipstar<%.3f",GetDst0Pi0pstarMin(),GetDst0Pi0pstarMax());
  //sprintf(cutstring[6],"dstarcostheta<%.2f",GetDstarCosineMax());
 
  cout<<"      Going to use the following Cuts:"<<endl;
  for(Int_t cutid=0;cutid<NCUTS;cutid++){    
    cut[cutid]=cutstring[cutid];   
    if(TString(cut[cutid])!="0"){     
      cout<<"Cut "<<cutid+1<<":  "<<cutstring[cutid]<<endl;    
    }
  }  
  
}

TCut JDataBaseDst0Pi0::GetCut(Int_t cutidmin,Int_t cutidmax){
  //cutidmin starts from 1 

  if(cutidmin<1||cutidmax<cutidmin){
    cout<<"JDataBaseDst0Pi: bad cutids"<<endl;
    return cut[99];
  } 
  if(cutidmax>=NCUTS)cutidmax=NCUTS;  
  
  TCut* cutsum=new TCut();
  for(Int_t cutid=cutidmin-1;cutid<cutidmax;cutid++) //start from 0
    if(TString(cut[cutid])!="0")*cutsum+=cut[cutid];
  
  return *cutsum;
}

void JDataBaseDst0Pi0::PrintCutList(TCanvas*C,TString filename){

  TLatex text;
  text.SetTextSize(.04);
  if(C!=NULL)C->Clear();
  for(Int_t cutid=0;cutid<NCUTS;cutid++){
    if(TString(cut[cutid])!="0"){
      if(C!=NULL)
	text.DrawTextNDC(.05,.95-.05*cutid,TString(" ")+(long)(cutid+1)+": "+cutstring[cutid]);
      else
	cout<<"Cut "<<cutid+1<<" "<<cutstring[cutid]<<endl;
    }
  }
  if(C!=NULL) C->Print(filename);
 
 
}

Int_t JDataBaseDst0Pi0::GetParameters(TString filename,Float_t* par,Int_t Npars){
  

  ifstream ParameterInput;
  ParameterInput.open(filename);
  if(!ParameterInput){
    cout<<"Parameters file not opened"<<endl;
    return 0;
  }
  cout<<"Parameters File:"<<filename<<"  has been opened."<<endl;
  for(Int_t p=0;p<Npars;p++){
    ParameterInput>>par[p];
    cout<<"par"<<p<<"="<<par[p]<<endl;
  }
  
  ParameterInput.close();
  ParameterInput.clear();
  cout<<"Parameters file closed."<<endl;

  return 1;
}

