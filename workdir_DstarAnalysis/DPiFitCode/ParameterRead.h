#ifndef PARAMETERREAD_H 
#define PARAMETERREAD_H 
#include "../Globals.h"

class ParameterRead:public TNamed{
  ClassDef(ParameterRead,1);

 public:
  ParameterRead();
  ~ParameterRead();

  //
  Int_t OpenFitPars(TString filename);
  Int_t CloseFitPars();
  Float_t GetFitPar(TString category,TString parametername);
  
  //

  private:

  ifstream InputStream;

};
#endif
