#include "JCutInt.h"

ClassImp(JCutInt);
JCutInt::JCutInt():
  JCut(),
  _discquantity(NULL),
  _high(0),
  _low(0)
{}

JCutInt::JCutInt(TString cutname,Int_t* discquantity,Int_t low,Int_t high,Bool_t InsideOrOutside):
JCut(cutname,2,InsideOrOutside)
{
  Init(cutname,discquantity,low,high,InsideOrOutside);
}

void  JCutInt::Init(TString cutname,Int_t* discquantity,Int_t low,Int_t high,Bool_t InsideOrOutside){
  if(discquantity==NULL){
    cout<<"Error JCut:: variable is null, cannot create cut:  "<<cutname<<" address:"<<discquantity<<endl;
  } 
  else {
    CutName=cutname;
    CutType=1;
    _InsideOrOutside=InsideOrOutside;
    _discquantity=discquantity;
    _high=high;
    _low=low;
    cout<<"New JCut:: "<<cutname<<" address:"<<_discquantity<<" "<<_low<<" "<<_high<<" "<<_InsideOrOutside<<endl;
  }
}



JCutInt::~JCutInt(){ 
}

Bool_t JCutInt::EvalCut(){
  if(_discquantity==NULL)
    return kFALSE;
  

  Bool_t pass=kTRUE;	
  if((_InsideOrOutside > 0 )?(_low<=*_discquantity&&*_discquantity<=_high):(_low>*_discquantity||*_discquantity>_high))
    pass=kTRUE;     
  else pass=kFALSE;
  
  return pass;
}
