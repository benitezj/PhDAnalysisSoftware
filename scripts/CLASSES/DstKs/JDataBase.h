#ifndef JDataBase_hh
#define JDataBase_hh
#include "../Globals.h"
#include "../AnalysisGlobals.h"
#include <TCut.h>

#define NCUTS 11

class JDataBase:public TObject{
  ClassDef(JDataBase,1);

 public:
  JDataBase();
  JDataBase(Int_t WhichCuts,TString WhichMode,Bool_t truthMatch);
  ~JDataBase();
  
  Int_t GetKLHBit(){return KLHBit;}
  Int_t GetPiLHBit(){return PiLHBit;} 
  Float_t GetD0pstarMin(){return D0pstarMin;}
  Float_t GetD0MassMin(){return D0MassMin;}
  Float_t GetD0MassMax(){return D0MassMax;}
  Float_t GetD0MassRangeWindow(){return D0MassRangeWindow;}
  Float_t GetD0MinLogProbab(){return D0MinLogProbab;}
  Float_t GetDstarDeltaMMin(){return DstarDeltaMMin;}
  Float_t GetDstarDeltaMMax(){return DstarDeltaMMax;}
  Float_t GetDstarMassWindow(){return DstarMassWindow;}
  Float_t GetDstarMinProbab(){return DstarMinProbab;}
  Float_t GetKsMassMin(){return KsMassMin;}
  Float_t GetKsMassMax(){return KsMassMax;}
  Float_t GetKsMassRangeWindow(){return KsMassRangeWindow;}
  Float_t GetKsCosineMin(){return KsCosineMin;}
  Float_t GetDs1pDeltaMMin(){return Ds1pDeltaMMin;}
  Float_t GetDs1pDeltaMMax(){return Ds1pDeltaMMax;}
  Float_t GetDs1pDeltaMRangeMin(){return Ds1pDeltaMRangeMin;}
  Float_t GetDs1pDeltaMRangeMax(){return Ds1pDeltaMRangeMax;} 
  Float_t GetDs1pDeltaMSignalMin(){return Ds1pDeltaMSignalMin;}
  Float_t GetDs1pDeltaMSignalMax(){return Ds1pDeltaMSignalMax;} 
  Int_t GetDs1pDeltaMNbins(){return Ds1pDeltaMNbins;} 
  Float_t GetDs1pMinLogProbab(){return Ds1pMinLogProbab;}
  Float_t GetDs1ppstarMin(){return Ds1ppstarMin;}
 
  void SetCuts(Int_t WhichCuts);
  TCut GetCut(Int_t cutidmin,Int_t cutidmax);
  Int_t GetNcuts(){return NCUTS;}
  void SetMode(TString WhichMode){_WhichMode=WhichMode;}
  void SetTruthMatch(Int_t truthMatch){TruthMatch=truthMatch;}

  Int_t GetParameters(TString filname,Float_t* par,Int_t Npars);
 
 private: 
  
  

  Int_t KLHBit;
  Int_t PiLHBit; 
  Int_t D0Pi2LHBit; 
  Int_t D0Pi3LHBit; 
  Float_t D0pstarMin;
  Float_t DstarDeltaMMin;
  Float_t DstarDeltaMMax;
  Float_t KsMassMin;
  Float_t KsMassMax;
  Float_t D0MassMin;
  Float_t D0MassMax;
  Float_t D0MassRangeWindow;
  Float_t DstarMassWindow;
  Float_t Ds1pDeltaMMin;
  Float_t Ds1pDeltaMMax;
  Float_t Ds1pDeltaMRangeMin;
  Float_t Ds1pDeltaMRangeMax;
  Float_t Ds1pDeltaMSignalMin;
  Float_t Ds1pDeltaMSignalMax;
  Float_t Ds1ppstarMin;
  
  Int_t Ds1pDeltaMNbins;
  Float_t KsMassRangeWindow;
  Float_t KsCosineMin;
  Float_t Ds1pMinLogProbab;
  Float_t DstarMinProbab;
  Float_t D0MinLogProbab;


  Float_t D0TorstenMass;
  Float_t KsTorstenMass;
  Float_t DstarTorstenDeltaM;

  Int_t _WhichCuts;
  TString _WhichMode;
  Bool_t TruthMatch;


  TCut totalcut;
  TCut cut[NCUTS];

  
};
  

#endif

