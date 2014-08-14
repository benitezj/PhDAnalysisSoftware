#include "DataBaseDCPi.h"


ClassImp(DataBaseDCPi);
DataBaseDCPi::DataBaseDCPi()
{
  SetNameType("DCDataBase",0,0);
}

DataBaseDCPi::DataBaseDCPi(const char* name,Bool_t mcOrData,Bool_t truthMatch)
{
  SetNameType(name,mcOrData,truthMatch);
}

DataBaseDCPi::~DataBaseDCPi(){ 
}

void DataBaseDCPi::SetCutsDCPi(){
  

  DMassRangeNbins=130;//1MeV bins
  DMassRangeMin=1.805;
  DMassRangeMax=1.935;

  SetCuts();//D+ cuts
  
  ///////common cuts
  DCPiMassMin=2.41;//define plot region
  DCPiMassMax=2.51;
 
 
  ///define the cuts  
  char tempstring[1000];

  sprintf(tempstring,"xmctrue==1"); 
  AddCut(tempstring,"xmctrue");
  if(!TruthMatch)
    DisableCut("xmctrue");

  sprintf(tempstring,"abs(xcharge)==1");
  AddCut(tempstring,"xcharge");

  sprintf(tempstring,"abs(xcharge)==2");
  AddCut(tempstring,"xchargews");
  DisableCut("xchargews");

//   sprintf(tempstring,"%.5f<xmdm&&xmdm<%.5f",GetDCPiMassMin(),GetDCPiMassMax());
//   AddCut(tempstring,"xmdm");
//   DisableCut("xmdm");

//   sprintf(tempstring,"(%.5f<xmdm&&xmdm<%.5f)||(%.5f<xmdm&&xmdm<%.5f)",
// 	  2.460-3.*(GetDCPiMassMax()-GetDCPiMassMin())/2.,
// 	  2.460-2.*(GetDCPiMassMax()-GetDCPiMassMin())/2.,
// 	  2.460+2.*(GetDCPiMassMax()-GetDCPiMassMin())/2.,
// 	  2.460+3.*(GetDCPiMassMax()-GetDCPiMassMin())/2.);
//   AddCut(tempstring,"xmdmsb");
//   DisableCut("xmdmsb");

//   sprintf(tempstring,"(%.5f<xmdm&&xmdm<%.5f)",
// 	  2.460-3.*(GetDCPiMassMax()-GetDCPiMassMin())/2.,
// 	  2.460-2.*(GetDCPiMassMax()-GetDCPiMassMin())/2.);
//   AddCut(tempstring,"xmdmlsb");
//   DisableCut("xmdmlsb");

//   sprintf(tempstring,"(%.5f<xmdm&&xmdm<%.5f)",
// 	  2.460+2.*(GetDCPiMassMax()-GetDCPiMassMin())/2.,
// 	  2.460+3.*(GetDCPiMassMax()-GetDCPiMassMin())/2.);
//   AddCut(tempstring,"xmdmhsb");
//   DisableCut("xmdmhsb");

  Float_t width=.05;
  sprintf(tempstring,"%.5f<xmdm&&xmdm<%.5f",2.460-width/2,2.460+width/2);
  AddCut(tempstring,"xmdmcutopt");
  DisableCut("xmdmcutopt");

  sprintf(tempstring,"%.5f<xmdm&&xmdm<%.5f",2.560-width/2,2.560+width/2);
  AddCut(tempstring,"xmdmsbcutopt1");
  DisableCut("xmdmsbcutopt1");

  sprintf(tempstring,"%.5f<xmdm&&xmdm<%.5f",2.610-width/2,2.610+width/2);
  AddCut(tempstring,"xmdmsbcutopt2");
  DisableCut("xmdmsbcutopt2");

  sprintf(tempstring,"%.5f<xmdm&&xmdm<%.5f",2.660-width/2,2.660+width/2);
  AddCut(tempstring,"xmdmsbcutopt3");
  DisableCut("xmdmsbcutopt3");

  sprintf(tempstring,"%.3f<xpstar",PstarCut);
  AddCut(tempstring,"xpstar");

  sprintf(tempstring,"%.3f<xpstar",4.);
  AddCut(tempstring,"xpstartight");
  DisableCut("xpstartight");

  RemoveCut("dmass");
  sprintf(tempstring,"abs(dmass-%6f)>%.3f",D0PDGMass,DMassCut);
  AddCut(tempstring,"dmass");

  RemoveCut("dflights");
  sprintf(tempstring,"dflights>%.2f",DFlightCut);
  AddCut(tempstring,"dflights");

  sprintf(tempstring,"dcostheta<%.2f",DCosineCut);
  AddCut(tempstring,"dcostheta");
  DisableCut("dcostheta");

  sprintf(tempstring,"abs(pi1Kmass-%6f)>.02",D0PDGMass);//wrong sign cut
  AddCut(tempstring,"pi1Kmass");
  DisableCut("pi1Kmass");

  sprintf(tempstring,"abs(pi1Kpimass-%6f)>.02",DPlusPDGMass);//wrong sign cut
  AddCut(tempstring,"pi1Kpimass");
  DisableCut("pi1Kpimass");

  sprintf(tempstring,"abs(pi1Kpi2mass-%6f)>.02",DPlusPDGMass);//wrong sign cut
  AddCut(tempstring,"pi1Kpi2mass");
  DisableCut("pi1Kpi2mass");


  ShowCuts();
}


