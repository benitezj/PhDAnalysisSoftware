#ifndef DataBaseBToDstPiPi_hh
#define DataBaseBToDstPiPi_hh

#include "../DC/DataBaseDC.h"

class DataBaseBToDstPiPi:public DataBaseDC{
  ClassDef(DataBaseBToDstPiPi,1);

 public:
  DataBaseBToDstPiPi();
  DataBaseBToDstPiPi(const char* name,Bool_t mcOrData,Bool_t truthMatch);
  ~DataBaseBToDstPiPi();

  void SetCutsBToDstPiPi();

/*   Float_t GetBToDstPiPiMinLogProbab(){return BToDstPiPiMinLogProbab;} */
/*   Float_t GetBToDstPiPiEnergyMin(){return BToDstPiPiEnergyMin;} */
/*   Float_t GetBToDstPiPiEnergyMax(){return BToDstPiPiEnergyMax;} */
/*   Float_t GetDCCosineMax(){return DCosineMax;} */

  Float_t GetBToDstPiPiMassMin(){return BToDstPiPiMassMin;}
  Float_t GetBToDstPiPiMassMax(){return BToDstPiPiMassMax;}
 

 private: 
  
  //Default Cuts
  Float_t BToDstPiPiMinLogProbab;
  Float_t BToDstPiPiEnergyMin;  
  Float_t BToDstPiPiEnergyMax;  
  Float_t DCosineMax;


  Float_t BToDstPiPiMassMin;
  Float_t BToDstPiPiMassMax;
 

};
  

#endif

