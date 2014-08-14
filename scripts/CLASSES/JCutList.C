#include "JCutList.h"

ClassImp(JCutList);
JCutList::JCutList():
  CutCounter(0)
{
}

JCutList::~JCutList(){ 
}

void JCutList::AddCut(JCut* cut){
  CutCounter++;
  Cut[CutCounter]=cut;
  cout<<"cut"<<CutCounter<<" "<<cut->GetCutName()<<" added"<<endl;
}

Bool_t JCutList::EvalCuts(Int_t idmin,Int_t idmax){
 
  Bool_t pass=kTRUE;
  if(idmin<1||idmin>CutCounter){cout<<"JCutList:bad index"<<endl;pass=kFALSE;}
  if(idmax<idmin){cout<<"JCutList:bad index"<<endl;pass=kFALSE;}
  if(idmax>CutCounter){idmax=CutCounter;}
  
 
  //must pass all cuts
  for(Int_t id=idmin;id<=idmax;id++){
    if(!Cut[id]->EvalCut())    
      pass=kFALSE;
  }
    
  return pass;
}
