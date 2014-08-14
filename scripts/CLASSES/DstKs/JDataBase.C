#include "JDataBase.h"

ClassImp(JDataBase);
JDataBase::JDataBase()
{
  _WhichMode="D0ToKPi";//Default
  TruthMatch=kFALSE;
  SetCuts(0);
}


JDataBase::~JDataBase(){ 
}
JDataBase::JDataBase(Int_t WhichCuts,TString WhichMode,Bool_t truthMatch)
{
  _WhichMode=WhichMode;
  TruthMatch=truthMatch;
  SetCuts(WhichCuts);
}

void JDataBase::SetCuts(Int_t WhichCuts){
  _WhichCuts=WhichCuts;

  D0MassRangeWindow=.07;
  Ds1pDeltaMRangeMin=0.;
  Ds1pDeltaMRangeMax=.06;
  DstarMinProbab=.005;
  DstarMassWindow=.03;
  KsMassRangeWindow=.025;
  Ds1pDeltaMNbins=100;
  Ds1pDeltaMSignalMin=.022;
  Ds1pDeltaMSignalMax=.033;
  Ds1pDeltaMMin=.015;//torsten fit cut 
  Ds1pDeltaMMax=.045;//torsten fit cut
  

  cout<<"      Going to use the following Cuts:"<<endl;

  if(TruthMatch){//MC: my own 2 (or 3) gauss fits 
    D0TorstenMass=1.86497;
    KsTorstenMass=.498372;
    DstarTorstenDeltaM=.14508;
    cout<<"Data Type: MC"<<endl;
  }else{//DATA: mean values taken from Torsten paper
    D0TorstenMass=1.8636;
    KsTorstenMass=.4972;
    DstarTorstenDeltaM=.1454;
    cout<<"Data Type: Data"<<endl;
  }

  //common cuts
  Ds1pMinLogProbab=-3;//.1% prob 
  KsMassMin=KsTorstenMass-.010;
  KsMassMax=KsTorstenMass+.010;
  KsCosineMin=.98877;//.98877=8.6deg=.15rad;


  if(_WhichMode=="D0ToKPi"){
      D0Pi2LHBit=0;
      D0Pi3LHBit=0;
    if(_WhichCuts==0){
      cout<<"Torsten Cuts"<<endl; 
      DstarDeltaMMin=DstarTorstenDeltaM-.0015;//.145421-.0015
      DstarDeltaMMax=DstarTorstenDeltaM+.0015;
      D0pstarMin=0;     
      D0MassMin=D0TorstenMass-.018;
      D0MassMax=D0TorstenMass+.018; 
      D0MinLogProbab=-100;
      KLHBit=11;//VeryLoose LH
      PiLHBit=2;//VeryLoose LH
      Ds1ppstarMin=2.7;
    }
    if(_WhichCuts==1){//tight
      cout<<"Torsten & skim Cuts"<<endl; 
      DstarDeltaMMin=DstarTorstenDeltaM-.0015;//.145421-.0015
      DstarDeltaMMax=DstarTorstenDeltaM+.0015;
      D0pstarMin=2.4;     
      D0MassMin=D0TorstenMass-.018;
      D0MassMax=D0TorstenMass+.018;
      D0MinLogProbab=-2.30;//.5% prob
      KLHBit=13;//tight LH
      PiLHBit=4; //tight LH
      Ds1ppstarMin=0.;
    }
  }
  if(_WhichMode=="D0ToK3Pi"){
    if(_WhichCuts==0){
      cout<<"Torsten Cuts"<<endl; 
      DstarDeltaMMin=DstarTorstenDeltaM-.0015;//.145421-.0015
      DstarDeltaMMax=DstarTorstenDeltaM+.0015;
      D0pstarMin=0;     
      D0MassMin=D0TorstenMass-.014;
      D0MassMax=D0TorstenMass+.014;
      D0MinLogProbab=-100;
      KLHBit=11;//VeryLoose LH
      PiLHBit=2;//VeryLoose LH
      D0Pi2LHBit=2;
      D0Pi3LHBit=2;
      Ds1ppstarMin=2.7;
    }
    if(_WhichCuts==1){//tight
      cout<<"Tight (Torsten) Cuts"<<endl; 
      DstarDeltaMMin=DstarTorstenDeltaM-.0015;//.145421-.0015
      DstarDeltaMMax=DstarTorstenDeltaM+.0015;
      D0pstarMin=2.2;
      D0MassMin=D0TorstenMass-.014;
      D0MassMax=D0TorstenMass+.014;
      D0MinLogProbab=-2.30;//.5% prob
      KLHBit=13;//tight LH
      PiLHBit=4; //tight LH
      D0Pi2LHBit=0;
      D0Pi3LHBit=0;
      Ds1ppstarMin=0.;
    }
  }
  
  char cutstring[NCUTS][1000];
  ///----------------define the cuts-------------------------------------------------------
  ///IMPORTANT: The order of these cuts CANNOT be changed, programs rely on these order
  ///How to Add one more cut: add one more line here and update NCUTS, everything else should be OK.
  ///-----------------------------------------------------------------------
  sprintf(cutstring[0],"ds1pmctrue==1");
  sprintf(cutstring[1],"%.5f<ds1pdeltam2&&ds1pdeltam2<%.5f",GetDs1pDeltaMMin(),GetDs1pDeltaMMax());
  sprintf(cutstring[2],"d0pstar>%.3f&&ds1ppstar>%.3f",GetD0pstarMin(),GetDs1ppstarMin());
  sprintf(cutstring[3],"%.5f<dstardeltam&&dstardeltam<%.5f",GetDstarDeltaMMin(),GetDstarDeltaMMax());
  sprintf(cutstring[4],"klh>=%d",GetKLHBit());
  if(_WhichMode=="D0ToKPi")sprintf(cutstring[5],"pilh>=%d",GetPiLHBit());
  if(_WhichMode=="D0ToK3Pi")sprintf(cutstring[5],"pilh>=%d&&d0pi2lh>=%d&&d0pi3lh>=%d",GetPiLHBit(),D0Pi2LHBit,D0Pi3LHBit);
  sprintf(cutstring[6],"%.5f<d0mass&&d0mass<%.5f",GetD0MassMin(),GetD0MassMax());
  sprintf(cutstring[7],"d0logvtxprob>%.5f",GetD0MinLogProbab());
  sprintf(cutstring[8],"%.4f<ksmass&&ksmass<%.4f",GetKsMassMin(),GetKsMassMax());
  sprintf(cutstring[9],"kscosine>%.5f",GetKsCosineMin());
  sprintf(cutstring[10],"ds1plogvtxprob>%.2f",GetDs1pMinLogProbab());

  for(Int_t cutid=0;cutid<NCUTS;cutid++){
    cut[cutid]=cutstring[cutid];
    cout<<"Cut "<<cutid+1<<":  "<<cutstring[cutid]<<endl;
    totalcut+=cut[cutid];
  }
  
}

TCut JDataBase::GetCut(Int_t cutidmin,Int_t cutidmax){
  if(cutidmin<1||cutidmax<cutidmin){
    cout<<"JDataBase: bad cutids"<<endl;
    return totalcut;
  } 
  if(cutidmax>NCUTS)cutidmax=NCUTS;
  
  TCut* cutsum=new TCut();
  
  for(Int_t i=cutidmin;i<=cutidmax;i++)
    *cutsum+=cut[i-1];


  return *cutsum;
}
Int_t JDataBase::GetParameters(TString filename,Float_t* par,Int_t Npars){
  

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

