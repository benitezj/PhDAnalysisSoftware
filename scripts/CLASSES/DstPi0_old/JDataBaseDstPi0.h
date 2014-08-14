#ifndef JDataBaseDstPi0_hh
#define JDataBaseDstPi0_hh
#include "../../Globals.h"
#include "../../AnalysisGlobals.h"
#include <TCut.h>
#include <TLatex.h>
#define NCUTS 100 //7

class JDataBaseDstPi0:public TObject{
  ClassDef(JDataBaseDstPi0,1);

 public:
  JDataBaseDstPi0();
  JDataBaseDstPi0(Int_t WhichMode,Bool_t truthMatch);
  ~JDataBaseDstPi0();

  Float_t GetDstPi0MinLogProbab(){return DstPi0MinLogProbab;}
  
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

  Int_t   GetDstPi0MassNbins(){return DstPi0MassNbins;}   
  Float_t GetDstPi0MassMin(){return DstPi0MassMin;}
  Float_t GetDstPi0MassMax(){return DstPi0MassMax;}

  Int_t   GetDstPi0DeltaMNbins(){return DstPi0DeltaMNbins;}   
  Float_t GetDstPi0DeltaMMin(){return DstPi0DeltaMMin;}
  Float_t GetDstPi0DeltaMMax(){return DstPi0DeltaMMax;}

  Int_t   GetDstPi0DeltaMRangeNbins(){return DstPi0DeltaMRangeNbins;} 
  Float_t GetDstPi0DeltaMRangeMin(){return DstPi0DeltaMRangeMin;}
  Float_t GetDstPi0DeltaMRangeMax(){return DstPi0DeltaMRangeMax;} 
  
  Int_t   GetDstPi0DeltaMSignalNbins(){return DstPi0DeltaMSignalNbins;} 
  Float_t GetDstPi0DeltaMSignalMin(){return DstPi0DeltaMSignalMin;}
  Float_t GetDstPi0DeltaMSignalMax(){return DstPi0DeltaMSignalMax;} 

  Float_t GetDstPi0pstarMin(){return DstPi0pstarMin;}
  Float_t GetDstPi0pstarMax(){return DstPi0pstarMax;}

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
  Float_t DstPi0MinLogProbab;
  Float_t DstarDeltaMMin;
  Float_t DstarDeltaMMax;
  Float_t D0MassMin;
  Float_t D0MassMax;
  Float_t DstPi0DeltaMMin;
  Float_t DstPi0DeltaMMax;
  Float_t DstPi0pstarMin;  
  Float_t DstPi0pstarMax;  
  Float_t DstarCosineMax;

  //other ranges/constants

  Int_t   DstPi0DeltaMNbins;

  Float_t DstarDeltaM;
  Int_t   DstarDeltaMRangeNbins;
  Float_t DstarDeltaMRangeMin;
  Float_t DstarDeltaMRangeMax;

  Float_t D0Mass;
  Int_t   D0MassRangeNbins;
  Float_t D0MassRangeMin;
  Float_t D0MassRangeMax;
   
  Int_t   DstPi0MassNbins;
  Float_t DstPi0MassMin;
  Float_t DstPi0MassMax;

  Int_t   DstPi0DeltaMRangeNbins;
  Float_t DstPi0DeltaMRangeMin;
  Float_t DstPi0DeltaMRangeMax;

  Int_t   DstPi0DeltaMSignalNbins;
  Float_t DstPi0DeltaMSignalMin;
  Float_t DstPi0DeltaMSignalMax;



  //internal variables
  Int_t _WhichCuts;
  Int_t _WhichMode;
  Bool_t TruthMatch;

  char cutstring[NCUTS][1000];
  TCut cut[NCUTS];


};
  

#endif

