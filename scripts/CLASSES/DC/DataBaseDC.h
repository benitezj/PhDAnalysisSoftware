#ifndef DataBaseDC_hh
#define DataBaseDC_hh
#include "../../Globals.h"
#include "../../AnalysisGlobals.h"
#include "../AbsDataBase.h"
#include <TCut.h>
#include <TLatex.h>

class DataBaseDC:public AbsDataBase{
  ClassDef(DataBaseDC,1);

 public:
  DataBaseDC();
  DataBaseDC(const char* name,Bool_t mcOrData,Bool_t truthMatch);
  ~DataBaseDC();

  void SetCuts();

  //  Float_t GetDMinLogProbab(){return DMinLogProbab;}
  
  Float_t GetDMass(){return DMass;}
/*   Float_t GetDPstarMin(){return DPstarMin;} */
/*   Float_t GetDPstarMax(){return DPstarMax;} */
  Float_t GetDMassMin(){return DMassMin;}
  Float_t GetDMassMax(){return DMassMax;}
  Int_t   GetDMassRangeNbins(){return DMassRangeNbins;} 
  Float_t GetDMassRangeMin(){return DMassRangeMin;}
  Float_t GetDMassRangeMax(){return DMassRangeMax;} 

 private: 
  
  Float_t DMass;

  //Default Cuts
/*   Float_t DMinLogProbab; */
/*   Float_t DPstarMin; */
/*   Float_t DPstarMax; */
  Float_t DMassMin;
  Float_t DMassMax; 
 
  Int_t   DMassRangeNbins;
  Float_t DMassRangeMin;
  Float_t DMassRangeMax;
  
  
};
  

#endif

