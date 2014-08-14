#include "DataBaseBToDstPiPi.h"


ClassImp(DataBaseBToDstPiPi);
DataBaseBToDstPiPi::DataBaseBToDstPiPi()
{
  SetNameType("DCDataBase",0,0);
}

DataBaseBToDstPiPi::DataBaseBToDstPiPi(const char* name,Bool_t mcOrData,Bool_t truthMatch)
{
  SetNameType(name,mcOrData,truthMatch);
}

DataBaseBToDstPiPi::~DataBaseBToDstPiPi(){ 
}

void DataBaseBToDstPiPi::SetCutsBToDstPiPi(){
  
  SetCuts();//D+ cuts
  
  ///////common cuts
  BToDstPiPiMassMin=2.41;//define signal region 
  BToDstPiPiMassMax=2.51;
 
  ///define the cuts  
  char tempstring[1000];
  sprintf(tempstring,"abs(xcharge)==1");
  AddCut(tempstring,"xcharge");

  sprintf(tempstring,"abs(xcharge)==2");
  AddCut(tempstring,"xchargews");
  DisableCut("xchargews");

  sprintf(tempstring,"%.5f<xmdm&&xmdm<%.5f",GetBToDstPiPiMassMin(),GetBToDstPiPiMassMax());
  AddCut(tempstring,"xmdm");
  DisableCut("xmdm");

  sprintf(tempstring,"(%.5f<xmdm&&xmdm<%.5f)||(%.5f<xmdm&&xmdm<%.5f)",
	  2.460-3.*(GetBToDstPiPiMassMax()-GetBToDstPiPiMassMin())/2.,
	  2.460-2.*(GetBToDstPiPiMassMax()-GetBToDstPiPiMassMin())/2.,
	  2.460+2.*(GetBToDstPiPiMassMax()-GetBToDstPiPiMassMin())/2.,
	  2.460+3.*(GetBToDstPiPiMassMax()-GetBToDstPiPiMassMin())/2.);
  AddCut(tempstring,"xmdmsb");
  DisableCut("xmdmsb");

  sprintf(tempstring,"(%.5f<xmdm&&xmdm<%.5f)",
	  2.460-3.*(GetBToDstPiPiMassMax()-GetBToDstPiPiMassMin())/2.,
	  2.460-2.*(GetBToDstPiPiMassMax()-GetBToDstPiPiMassMin())/2.);
  AddCut(tempstring,"xmdmlsb");
  DisableCut("xmdmlsb");

  sprintf(tempstring,"(%.5f<xmdm&&xmdm<%.5f)",
	  2.460+2.*(GetBToDstPiPiMassMax()-GetBToDstPiPiMassMin())/2.,
	  2.460+3.*(GetBToDstPiPiMassMax()-GetBToDstPiPiMassMin())/2.);
  AddCut(tempstring,"xmdmhsb");
  DisableCut("xmdmhsb");

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


  sprintf(tempstring,"%.3f<xpstar",2.2);
  AddCut(tempstring,"xpstar");

  sprintf(tempstring,"%.3f<xpstar",3.5);
  AddCut(tempstring,"xpstartight");
  DisableCut("xpstartight");

  sprintf(tempstring,"%.3f<xlogL",0.);
  AddCut(tempstring,"xlogLtight");
  DisableCut("xlogLtight");

//   sprintf(tempstring,"%.3f<xenergyfrac",.6);
//   AddCut(tempstring,"xenergy");

//   sprintf(tempstring,"%.3f<xenergyfrac",.8);
//   AddCut(tempstring,"xenergytight");
//   DisableCut("xenergytight");


  sprintf(tempstring,"dcostheta<%.2f",.5);
  AddCut(tempstring,"dcostheta");
  DisableCut("dcostheta");

  sprintf(tempstring,"dflights>%.2f",0.);
  AddCut(tempstring,"dflights");
  DisableCut("dflights");

  sprintf(tempstring,"xmctrue==1"); 
  AddCut(tempstring,"xmctrue");
  if(!TruthMatch)
    DisableCut("xmctrue");


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


