#ifndef JCutInt_hh
#define JCutInt_hh
#include "JCut.h"
#include "../Globals.h"

class JCutInt:public JCut{
  ClassDef(JCutInt,1);

 public:
  JCutInt();
  JCutInt(TString cutid,Int_t* discquantity,Int_t low,Int_t high,Bool_t InsideOrOutside);
  ~JCutInt();
  
  void Init(TString cutid,Int_t* discquantity,Int_t low,Int_t high,Bool_t InsideOrOutside);
  Bool_t EvalCut();

 private: 
  
  Int_t* _discquantity;
  Int_t _high;
  Int_t _low;
};
  

#endif

