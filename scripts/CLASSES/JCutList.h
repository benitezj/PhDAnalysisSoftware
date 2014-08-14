#ifndef JCutList_hh
#define JCutList_hh
#include "../Globals.h"
#include "JCut.h"

class JCutList:public TObject{
  ClassDef(JCutList,1);

 public:
  JCutList();
  ~JCutList();
  
  //Cuts will be applied in order of id
  void AddCut(JCut* cut);
  JCut* GetCut(Int_t cutnumber){return Cut[cutnumber];}
  Bool_t EvalCuts(Int_t idmin,Int_t idmax);//idmin must be >=1
  Int_t GetNCuts(){return CutCounter;}
 
 private:

  Int_t CutCounter;
  JCut* Cut[100];

};
  

#endif

