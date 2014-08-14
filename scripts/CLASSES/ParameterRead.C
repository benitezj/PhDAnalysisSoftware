#include "ParameterRead.h"
#include "../FUNCTIONS/Fits.h"

ClassImp(ParameterRead);
ParameterRead::ParameterRead():
  TNamed("ParameterRead","ParameterRead") 
{  
}

ParameterRead::~ParameterRead(){
}

Int_t ParameterRead::OpenFitPars(TString filename){
  InputStream.open((const char*)(filename));
  if(!InputStream.is_open()){cout<<"bad fit pars file:"<<filename<<endl; return 0;}
  cout<<" opened fit pars file "<<filename<<endl;
  return 1;
}
                                                                                                                                          
Int_t ParameterRead::CloseFitPars(){
  InputStream.close();
  InputStream.clear();
  return 1;
}
                                                                                                                                          
Float_t ParameterRead::GetFitPar(TString category,TString parametername){
                                                                                                                                          
  if(!InputStream.is_open()){cout<<"fit pars file not open"<<endl; return 0;}
  InputStream.clear();
  InputStream.seekg(0,fstream::beg);
                                                                                                                                          
  Float_t fitpar=0;
  TString cat;
  Int_t ntries=0;
  Bool_t foundpar=0;
  TString parname;
  Bool_t END=0;
  while(!foundpar&&!InputStream.eof()&&ntries<1000&&!END){
    InputStream>>cat>>parname>>fitpar;
    if(cat==category&&parname==parametername)
      foundpar=1;
                                                                                                                                          
    if(cat=="END")END=1;
    ntries++;
  }
  if(!foundpar){cout<<"fit par not found:"<<category<<" "<<parametername<<endl;return 0;}
                                                                                                                                          
  return fitpar;
}
