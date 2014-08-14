#ifndef FRAGMENTATIONFITTER_H 
#define FRAGMENTATIONFITTER_H 
#include "../Globals.h"
#include "ParameterRead.h"


#define NMODELS 5

class FragmentationFitter:public TNamed{
  ClassDef(FragmentationFitter,1);

 public:
  FragmentationFitter();
  ~FragmentationFitter();
  
  
  Int_t Init(TH1* histo,TString OutDir);
  Int_t Fit();
  Int_t Save();

  //
  TF1* GetModel(TString model){
    for(Int_t i=0;i<NMODELS;i++) if(ModelName[i]==model) return FModel[i]; return 0;
  }
    

  //
  Int_t CreateTable();

 private:
  
  TH1* Histo;
  TString OutPutDir;
  
  TF1* FModel[NMODELS];
  TString ModelName[NMODELS];
  TString Formula[NMODELS];
  TString ParName[NMODELS][4];
  Int_t FitStatus[NMODELS];
  
  ParameterRead inputstream;
 
};
#endif
