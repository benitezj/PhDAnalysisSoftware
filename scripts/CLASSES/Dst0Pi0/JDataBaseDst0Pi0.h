#ifndef JDataBaseDst0Pi0_hh
#define JDataBaseDst0Pi0_hh
#include "../../Globals.h"
#include "../../AnalysisGlobals.h"
#include <TCut.h>
#include <TLatex.h>
#define NCUTS 100

class JDataBaseDst0Pi0:public TObject{
  ClassDef(JDataBaseDst0Pi0,1);

 public:
  JDataBaseDst0Pi0();
  JDataBaseDst0Pi0(Int_t WhichMode,Bool_t truthMatch);
  ~JDataBaseDst0Pi0();

  Float_t GetDst0Pi0MinLogProbab(){return Dst0Pi0MinLogProbab;}
  
  Float_t GetD0Mass(){return D0Mass;}
  Float_t GetD0MassMin(){return D0MassMin;}
  Float_t GetD0MassMax(){return D0MassMax;}

  Float_t GetDstarDeltaM(){return DstarDeltaM;}
  Float_t GetDstarDeltaMMin(){return DstarDeltaMMin;}
  Float_t GetDstarDeltaMMax(){return DstarDeltaMMax;}
  Float_t GetDstarCosineMax(){return DstarCosineMax;}

  Int_t   GetD0MassRangeNbins(){return D0MassRangeNbins;} 
  Float_t GetD0MassRangeMin(){return D0MassRangeMin;}
  Float_t GetD0MassRangeMax(){return D0MassRangeMax;} 

  Int_t   GetDstarDeltaMRangeNbins(){return DstarDeltaMRangeNbins;} 
  Float_t GetDstarDeltaMRangeMin(){return DstarDeltaMRangeMin;}
  Float_t GetDstarDeltaMRangeMax(){return DstarDeltaMRangeMax;}

  Int_t   GetDst0Pi0MassNbins(){return Dst0Pi0MassNbins;}   
  Float_t GetDst0Pi0MassMin(){return Dst0Pi0MassMin;}
  Float_t GetDst0Pi0MassMax(){return Dst0Pi0MassMax;}

  Int_t   GetDst0Pi0DeltaMNbins(){return Dst0Pi0DeltaMNbins;}   
  Float_t GetDst0Pi0DeltaMMin(){return Dst0Pi0DeltaMMin;}
  Float_t GetDst0Pi0DeltaMMax(){return Dst0Pi0DeltaMMax;}

  Int_t   GetDst0Pi0DeltaMRangeNbins(){return Dst0Pi0DeltaMRangeNbins;} 
  Float_t GetDst0Pi0DeltaMRangeMin(){return Dst0Pi0DeltaMRangeMin;}
  Float_t GetDst0Pi0DeltaMRangeMax(){return Dst0Pi0DeltaMRangeMax;} 
  
  Int_t   GetDst0Pi0DeltaMSignalNbins(){return Dst0Pi0DeltaMSignalNbins;} 
  Float_t GetDst0Pi0DeltaMSignalMin(){return Dst0Pi0DeltaMSignalMin;}
  Float_t GetDst0Pi0DeltaMSignalMax(){return Dst0Pi0DeltaMSignalMax;} 

  Float_t GetDst0Pi0pstarMin(){return Dst0Pi0pstarMin;}
  Float_t GetDst0Pi0pstarMax(){return Dst0Pi0pstarMax;}

  TCut GetCut(Int_t cutidmin,Int_t cutidmax);
  Int_t GetNcuts(){return NCUTS;}  
  void DisableCut(Int_t cutid){cut[cutid-1]="0";}
  void EnableCut(Int_t cutid){cut[cutid-1]=cutstring[cutid-1];}

  void SetCuts();
  void SetD0Mode(Int_t WhichMode){_WhichMode=WhichMode;}
  void SetTruthMatch(Int_t truthMatch){TruthMatch=truthMatch;}

  Int_t GetParameters(TString filname,Float_t* par,Int_t Npars);
  
  void PrintCutList(TCanvas*C,TString filename);

  TString GetDstPiDeltaMLabel(){return TString("m(D*^{+}#pi^{0}) - m(D*^{+})  (GeV/c^{2})");}  

 private: 
  
  //Default Cuts
  Float_t Dst0Pi0MinLogProbab;
  Float_t DstarDeltaMMin;
  Float_t DstarDeltaMMax;
  Float_t D0MassMin;
  Float_t D0MassMax;
  Float_t Dst0Pi0DeltaMMin;
  Float_t Dst0Pi0DeltaMMax;
  Float_t Dst0Pi0pstarMin;  
  Float_t Dst0Pi0pstarMax;  
  Float_t DstarCosineMax;

  //other ranges/constants

  Int_t   Dst0Pi0DeltaMNbins;

  Float_t DstarDeltaM;
  Int_t   DstarDeltaMRangeNbins;
  Float_t DstarDeltaMRangeMin;
  Float_t DstarDeltaMRangeMax;

  Float_t D0Mass;
  Int_t   D0MassRangeNbins;
  Float_t D0MassRangeMin;
  Float_t D0MassRangeMax;
   
  Int_t   Dst0Pi0MassNbins;
  Float_t Dst0Pi0MassMin;
  Float_t Dst0Pi0MassMax;

  Int_t   Dst0Pi0DeltaMRangeNbins;
  Float_t Dst0Pi0DeltaMRangeMin;
  Float_t Dst0Pi0DeltaMRangeMax;

  Int_t   Dst0Pi0DeltaMSignalNbins;
  Float_t Dst0Pi0DeltaMSignalMin;
  Float_t Dst0Pi0DeltaMSignalMax;



  //internal variables
  Int_t _WhichCuts;
  Int_t _WhichMode;
  Bool_t TruthMatch;

  char cutstring[NCUTS][1000];
  TCut cut[NCUTS];


};
  

#endif

