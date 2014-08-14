#ifndef JDataBase2640_hh
#define JDataBase2640_hh
#include "../../Globals.h"
#include "../../AnalysisGlobals.h"
#include <TCut.h>
#include <TLatex.h>
#define NCUTS 12

class JDataBase2640:public TObject{
  ClassDef(JDataBase2640,1);

 public:
  JDataBase2640();
  JDataBase2640(Int_t WhichCuts,TString WhichMode,Bool_t truthMatch);
  ~JDataBase2640();
  
  Int_t GetKLHBit(){return KLHBit;}
  Int_t GetPiLHBit(){return PiLHBit;} 
  Float_t GetD0pstarMin(){return D0pstarMin;}
  Float_t GetD0MassRangeMin(){return D0MassRangeMin;}
  Float_t GetD0MassRangeMax(){return D0MassRangeMax;} 
  Float_t GetD0MassMin(){return D0MassMin;}
  Float_t GetD0MassMax(){return D0MassMax;}
  Float_t GetD0MinLogProbab(){return D0MinLogProbab;}
  Float_t GetDstarDeltaMRangeMin(){return DstarDeltaMRangeMin;}
  Float_t GetDstarDeltaMRangeMax(){return DstarDeltaMRangeMax;}
  Float_t GetDstarDeltaMMin(){return DstarDeltaMMin;}
  Float_t GetDstarDeltaMMax(){return DstarDeltaMMax;}
  Float_t GetDstarMinProbab(){return DstarMinProbab;}
  Float_t GetDstarCosineMax(){return DstarCosineMax;}
  Float_t GetDst2640MassRangeMin(){return Dst2640MassRangeMin;}
  Float_t GetDst2640MassRangeMax(){return Dst2640MassRangeMax;}
  Float_t GetDst2640DeltaMRangeMin(){return Dst2640DeltaMRangeMin;}
  Float_t GetDst2640DeltaMRangeMax(){return Dst2640DeltaMRangeMax;} 
  Float_t GetDst2640DeltaMMin(){return Dst2640DeltaMMin;}
  Float_t GetDst2640DeltaMMax(){return Dst2640DeltaMMax;}
  Float_t GetDst2640DeltaMSignalMin(){return Dst2640DeltaMSignalMin;}
  Float_t GetDst2640DeltaMSignalMax(){return Dst2640DeltaMSignalMax;} 
  Int_t GetDst2640DeltaMNbins(){return Dst2640DeltaMNbins;} 
  Float_t GetDst2640MinLogProbab(){return Dst2640MinLogProbab;}
  Float_t GetDst2640pstarMin(){return Dst2640pstarMin;}
  Float_t GetDst2640pstarMax(){return Dst2640pstarMax;}
 

  void SetCuts(Int_t WhichCuts);
  TCut GetCut(Int_t cutidmin,Int_t cutidmax);
  Int_t GetNcuts(){return NCUTS;}
  void SetMode(TString WhichMode){_WhichMode=WhichMode;}
  void SetTruthMatch(Int_t truthMatch){TruthMatch=truthMatch;}
  Int_t GetParameters(TString filname,Float_t* par,Int_t Npars);
  
  void PrintCutList(TCanvas*C,TString filename);

 private: 
  
  //Default Cuts
  Int_t KLHBit;
  Int_t PiLHBit; 
  Int_t D0Pi2LHBit; 
  Int_t D0Pi3LHBit; 
  Float_t D0pstarMin;
  Float_t DstarDeltaMMin;
  Float_t DstarDeltaMMax;
  Float_t Dst2640MinLogProbab;
  Float_t D0MinLogProbab;
  Float_t D0MassMin;
  Float_t D0MassMax;
  Float_t Dst2640DeltaMMin;
  Float_t Dst2640DeltaMMax;
  Float_t Dst2640pstarMin;  
  Float_t Dst2640pstarMax;  
  Float_t DstarCosineMax;

  //other ranges/constants
  Float_t DstarDeltaMRangeMin;
  Float_t DstarDeltaMRangeMax;
  Float_t D0MassRangeMin;
  Float_t D0MassRangeMax;
  Float_t DstarMassWindow;
  Float_t Dst2640MassRangeMin;
  Float_t Dst2640MassRangeMax;
  Float_t Dst2640DeltaMRangeMin;
  Float_t Dst2640DeltaMRangeMax;
  Float_t Dst2640DeltaMSignalMin;
  Float_t Dst2640DeltaMSignalMax;
  Float_t D0Mass;
  Float_t DstarDeltaM;
  Int_t Dst2640DeltaMNbins;
  Float_t DstarMinProbab;


  //internal variables
  Int_t _WhichCuts;
  TString _WhichMode;
  Bool_t TruthMatch;

  char cutstring[NCUTS][1000];
  TCut totalcut;
  TCut cut[NCUTS];


};
  

#endif

