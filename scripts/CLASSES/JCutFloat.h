#ifndef JCutFloat_hh
#define JCutFloat_hh
#include "JCut.h"
#include "../Globals.h"

class JCutFloat:public JCut{
  ClassDef(JCutFloat,1);

 public:
  JCutFloat();
  JCutFloat(TString cutname,Float_t* discquantity,Float_t low,Float_t high,Bool_t InsideOrOutside);
  ~JCutFloat();

  void Init(TString cutname,Float_t* discquantity,Float_t low,Float_t high,Bool_t InsideOrOutside);
  Bool_t EvalCut();

 private: 
 
  Float_t* _discquantity;
  Float_t _high;
  Float_t _low;

};
  

#endif

