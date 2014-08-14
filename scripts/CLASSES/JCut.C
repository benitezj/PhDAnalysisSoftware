#include "JCut.h"

ClassImp(JCut);
JCut::JCut():
  CutName("0"),
  CutType(1),
  _InsideOrOutside(1)
{}

JCut::JCut(TString cutname,Int_t cuttype,Bool_t InsideOrOutside){
  CutName=cutname;
  CutType=cuttype;
  _InsideOrOutside=InsideOrOutside;
}
JCut::~JCut(){ 
}



