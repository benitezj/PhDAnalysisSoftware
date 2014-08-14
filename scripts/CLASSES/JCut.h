#ifndef JCut_hh
#define JCut_hh
#include "../Globals.h"

class JCut:public TObject{
  ClassDef(JCut,1);

 public:
  JCut();
  JCut(TString cutname,Int_t cuttype,Bool_t InsideOrOutside);
  ~JCut();

  virtual Bool_t EvalCut(){cout<<"ERROR JCut::EvalCut"<<endl;return kFALSE;}
  TString GetCutName(){return CutName;}
  Int_t GetCutType(){return CutType;}
  Bool_t GetCutRegion(){return _InsideOrOutside;}
  
 protected: 

  TString CutName;
  Int_t CutType;//Float=1 , Int =2 
  Bool_t _InsideOrOutside; 
};
  

#endif

