#include "JCutFloat.h"

ClassImp(JCutFloat);
JCutFloat::JCutFloat():
  JCut(),
  _discquantity(NULL),
  _high(0),
  _low(0)
{}

JCutFloat::JCutFloat(TString cutname,Float_t* discquantity,Float_t low,Float_t high,Bool_t InsideOrOutside):
  JCut(cutname,1,InsideOrOutside)
{
  Init(cutname,discquantity,low,high,InsideOrOutside);
}

JCutFloat::~JCutFloat(){ 
}

void  JCutFloat::Init(TString cutname,Float_t* discquantity,Float_t low,Float_t high,Bool_t InsideOrOutside){  

  if(discquantity==NULL){
    cout<<"Error JCutFloat:: variable is null, cannot create cut:  "<<cutname<<" address:"<<discquantity<<endl;
  }
  else {
    CutName=cutname;
    CutType=1;
    _InsideOrOutside=InsideOrOutside;
    _discquantity=discquantity;
    _high=high;
    _low=low;
    cout<<"New JCutFloat:: "<<cutname<<" address:"<<discquantity<<" "<<_low<<" "<<_high<<" "<<_InsideOrOutside<<endl;
  }
}


Bool_t JCutFloat::EvalCut(){
  if(_discquantity==NULL)
    return kFALSE;

  Bool_t pass=kTRUE;
  
  if((_InsideOrOutside > 0 )?(_low<=*_discquantity&&*_discquantity<=_high):(_low>*_discquantity||*_discquantity>_high))
    pass=kTRUE;     
  else pass=kFALSE;
 
 
  return pass;
}
