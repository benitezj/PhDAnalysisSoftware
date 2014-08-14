#ifndef DataBaseDCPi_hh
#define DataBaseDCPi_hh

#include "../DC/DataBaseDC.h"

class DataBaseDCPi:public DataBaseDC{
  ClassDef(DataBaseDCPi,1);

 public:
  DataBaseDCPi();
  DataBaseDCPi(const char* name,Bool_t mcOrData,Bool_t truthMatch);
  ~DataBaseDCPi();

  void SetCutsDCPi();
  void SetCutValues(Float_t pmin,Float_t dmassmax,Float_t flightmin,Float_t dcosmax){
    PstarCut=pmin;DMassCut=dmassmax;DFlightCut=flightmin;DCosineCut=dcosmax;
  }  
  Float_t GetDCPiPstarCut(){return PstarCut;}
  Float_t GetDCPiDMassCut(){return DMassCut;}
  Float_t GetDCPiDFlightCut(){return DFlightCut;}
  Float_t GetDCPiDCosCut(){return DCosineCut;}

  Float_t GetDCPiMassMin(){return DCPiMassMin;}
  Float_t GetDCPiMassMax(){return DCPiMassMax;}
 
  
 private: 
  
  //Default Cuts
  Float_t PstarCut;
  Float_t DMassCut;  
  Float_t DFlightCut;  
  Float_t DCosineCut;

  Float_t DCPiMassMin;
  Float_t DCPiMassMax;
 

};
  

#endif

