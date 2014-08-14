#ifndef AbsDataBase_hh
#define AbsDataBase_hh
#include "../Globals.h"
#include "../AnalysisGlobals.h"
#include <TCut.h>
#include <TLatex.h>
#define NCUTS 100 

class AbsDataBase:public TNamed{
  ClassDef(AbsDataBase,1);

 public:
  AbsDataBase();
  AbsDataBase(const char* name,Bool_t mcOrData,Bool_t truthMatch);
  ~AbsDataBase();

  void SetNameType(const char* name,Bool_t mcOrData,Bool_t truthMatch);
  void ShowCuts();
  void AddCut(TString formula,TString name);
  void RemoveCut(TString name);

  TCut GetAllCuts();
  TCut GetCut(TString CutName);
  void DisableCut(TString CutName);
  void EnableCut(TString CutName);
  void SetMCorData(Bool_t mcOrData){MCorData=mcOrData;}//must be set befroe SetCuts();
  void SetTruthMatch(Bool_t truthMatch){TruthMatch=truthMatch;}  //must be set befroe SetCuts();
   
  void PrintCutList(TCanvas*C,TString filename);
    
 private: 
  
  //internal variables
  Bool_t MCorData;
  Bool_t TruthMatch;
  TCut cut[NCUTS];  
  Bool_t cutstatus[NCUTS];
  TString cutname[NCUTS];

};
  

#endif

