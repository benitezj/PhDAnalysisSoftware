#include "JDataBase2640.h"

ClassImp(JDataBase2640);
JDataBase2640::JDataBase2640()
{
  _WhichMode="D0ToKPi";//Default
  TruthMatch=kFALSE;
  SetCuts(0);
}


JDataBase2640::~JDataBase2640(){ 
}
JDataBase2640::JDataBase2640(Int_t WhichCuts,TString WhichMode,Bool_t truthMatch)
{
  _WhichMode=WhichMode;
  TruthMatch=truthMatch;
  SetCuts(WhichCuts);
}

void JDataBase2640::SetCuts(Int_t WhichCuts){
  _WhichCuts=WhichCuts;

  Dst2640DeltaMNbins=65*2;
  
  if(TruthMatch){
    Dst2640MassRangeMin=2.600;
    Dst2640MassRangeMax=2.680;
    Dst2640DeltaMRangeMin=.630-.04;
    Dst2640DeltaMRangeMax=.630+.04;
    Dst2640DeltaMMin=.630-.030;
    Dst2640DeltaMMax=.630+.030;
    Dst2640DeltaMSignalMin=.630-.010;
    Dst2640DeltaMSignalMax=.630+.010;
  }else {
    Dst2640MassRangeMin=2.2;
    Dst2640MassRangeMax=3.5;
    Dst2640DeltaMRangeMin=.1;
    Dst2640DeltaMRangeMax=1.49;
    Dst2640DeltaMMin=.19;//2.2 to 3.5 GeV
    Dst2640DeltaMMax=1.49;
    Dst2640DeltaMSignalMin=.627-.010;
    Dst2640DeltaMSignalMax=.627+.010;
  }


  DstarMinProbab=.005;
  DstarMassWindow=.03;
  D0MassRangeMin=D0PDGMass-.050;
  D0MassRangeMax=D0PDGMass+.050;
  DstarDeltaMRangeMin=.140;
  DstarDeltaMRangeMax=.150;

  if(TruthMatch){//MC: my own 2 (or 3) gauss fits 
    D0Mass=1.8650;
    DstarDeltaM=.1451;
    cout<<"Data Type: MC"<<endl;
  }else{//DATA: mean values taken from Torsten paper
    D0Mass=1.8636;
    DstarDeltaM=.1454;
    cout<<"Data Type: Data"<<endl;
  }


  ///////common cuts
  Dst2640MinLogProbab=-3;//.1% prob 
  D0Pi2LHBit=2;
  D0Pi3LHBit=2;
  KLHBit=13;
  PiLHBit=4;  
  D0MinLogProbab=-2.3; 
  Dst2640pstarMin=2.5;
  Dst2640pstarMax=4.8;
  DstarCosineMax=.5;
  D0pstarMin=2.4; 
  
  cout<<"      Going to use the following Cuts:"<<endl;
//   if(_WhichMode=="D0ToKPi"){
//     //D0pstarMin=2.4; 
//     if(_WhichCuts==0){//tight
//       cout<<"Tight Cuts"<<endl; 
//       DstarDeltaMMin=DstarDeltaM-.0015;//.0015;
//       DstarDeltaMMax=DstarDeltaM+.0015;
//       D0MassMin=D0Mass-.018;
//       D0MassMax=D0Mass+.018;
//     }
//   }
//   if(_WhichMode=="D0ToK3Pi"){
//     //D0pstarMin=2.2; //changed 10/14/2007 to make both modes similar
//     if(_WhichCuts==0){//tight
//       cout<<"Tight Cuts"<<endl; 
//       DstarDeltaMMin=DstarDeltaM-.0008;
//       DstarDeltaMMax=DstarDeltaM+.0008;
//       D0MassMin=D0Mass-.014;
//       D0MassMax=D0Mass+.014;
//     }
//   }
  
  DstarDeltaMMin=DstarDeltaM-.0015;//.0015;
  DstarDeltaMMax=DstarDeltaM+.0015;
  D0MassMin=D0Mass-.018;
  D0MassMax=D0Mass+.018;

  

  ///define the cuts
  ///IMPORTANT: The order of these cuts CANNOT be changed, programs rely on these order
  sprintf(cutstring[0],"dst2640mctrue==1&&pi1pluspi2charge==0");
  sprintf(cutstring[1],"%.5f<dst2640deltam&&dst2640deltam<%.5f",GetDst2640DeltaMMin(),GetDst2640DeltaMMax());
  sprintf(cutstring[2],"d0pstar>%.3f",GetD0pstarMin());
  sprintf(cutstring[3],"%.5f<dstardeltam&&dstardeltam<%.5f",GetDstarDeltaMMin(),GetDstarDeltaMMax());
  sprintf(cutstring[4],"klh>=%d",GetKLHBit());
  //if(_WhichMode=="D0ToKPi")sprintf(cutstring[5],"pilh>=%d",GetPiLHBit());
  //if(_WhichMode=="D0ToK3Pi")sprintf(cutstring[5],"pilh>=%d&&d0pi2lh>=%d&&d0pi3lh>=%d",GetPiLHBit(),D0Pi2LHBit,D0Pi3LHBit);
  sprintf(cutstring[5],"pilh>=%d&&d0pi2lh>=%d&&d0pi3lh>=%d",GetPiLHBit(),D0Pi2LHBit,D0Pi3LHBit);
  sprintf(cutstring[6],"%.5f<d0mass&&d0mass<%.5f",GetD0MassMin(),GetD0MassMax());
  sprintf(cutstring[7],"d0logvtxprob>%.5f",GetD0MinLogProbab());
  sprintf(cutstring[8],"dst2640logvtxprob>%.2f",GetDst2640MinLogProbab());
  sprintf(cutstring[9],"pi1lh>=2&&pi2lh>=2");
  sprintf(cutstring[10],"%.3f<dst2640pstar&&dst2640pstar<%.3f",GetDst2640pstarMin(),GetDst2640pstarMax());
  sprintf(cutstring[11],"dstarcostheta<%.2f",GetDstarCosineMax());
 
  for(Int_t cutid=0;cutid<NCUTS;cutid++){
    cut[cutid]=cutstring[cutid];
    cout<<"Cut "<<cutid+1<<":  "<<cutstring[cutid]<<endl;
    totalcut+=cut[cutid];
  }
  
}

TCut JDataBase2640::GetCut(Int_t cutidmin,Int_t cutidmax){
  if(cutidmin<1||cutidmax<cutidmin){
    cout<<"JDataBase2640: bad cutids"<<endl;
    return totalcut;
  } 
  if(cutidmax>NCUTS)cutidmax=NCUTS;
  
  TCut* cutsum=new TCut();
  
  for(Int_t i=cutidmin;i<=cutidmax;i++)
    *cutsum+=cut[i-1];


  return *cutsum;
}

void JDataBase2640::PrintCutList(TCanvas*C,TString filename){

  TLatex text;
  text.SetTextSize(.04);
  if(C!=NULL)C->Clear();
  for(Int_t cutid=0;cutid<NCUTS;cutid++){
    if(C!=NULL)
      text.DrawTextNDC(.05,.95-.05*cutid,TString(" ")+(long)(cutid+1)+": "+cutstring[cutid]);
    else
      cout<<"Cut "<<cutid+1<<" "<<cutstring[cutid]<<endl;
  }
  if(C!=NULL) C->Print(filename);
 
}
Int_t JDataBase2640::GetParameters(TString filename,Float_t* par,Int_t Npars){
  

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

