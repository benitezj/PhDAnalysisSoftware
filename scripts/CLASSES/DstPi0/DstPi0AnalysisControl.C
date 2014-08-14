#include "../../MACROS/TreeChain.C"
#include "DstPi0Analysis.h"

DstPi0Analysis* analyzeDstPi0(Int_t WhichDATASet,Bool_t ReduceNtuple){
  
  TChain* chain=new TChain("ntp1"); 
  TString myntupledir="DstarCharged/DstarPi0";
  DstPi0Analysis* a=0;
  TString OutputDir;

  ///Small Sample
  if(WhichDATASet==1){
    a=new DstPi0Analysis(1,myntupledir+"/KpiAndK3pi");
  
    if(ReduceNtuple){
      TreeChain(chain,1,33,myntupledir+"/DATARun3On");    
      a->SetChain(chain);
      a->SetReduceCuts(1,1,1,1,1,1);
    } 
  }

  //Full Dataset
  if(WhichDATASet>=10&&WhichDATASet<20){
    if(WhichDATASet==10)OutputDir=myntupledir+"/KpiAndK3pi";
    if(WhichDATASet==11)OutputDir=myntupledir+"/KpiAndK3piDMass";
    if(WhichDATASet==12)OutputDir=myntupledir+"/KpiAndK3piDstMass";
    if(WhichDATASet==13)OutputDir=myntupledir+"/KpiAndK3piPi0Mass";
    if(WhichDATASet==14)OutputDir=myntupledir+"/KpiAndK3piDstMassPi0Mass";
    if(WhichDATASet==15)OutputDir=myntupledir+"/KpiAndK3piSide";
    if(WhichDATASet==16)OutputDir=myntupledir+"/KpiAndK3piDstSide";
    if(WhichDATASet==17)OutputDir=myntupledir+"/KpiAndK3piPi0Side";
    a=new DstPi0Analysis(1,OutputDir);
  
    if(ReduceNtuple){
      TreeChain(chain,1,19,myntupledir+"/DATARun1On");
      TreeChain(chain,1,3,myntupledir+"/DATARun1Off");
      TreeChain(chain,1,62,myntupledir+"/DATARun2On");
      TreeChain(chain,1,7,myntupledir+"/DATARun2Off"); 
      TreeChain(chain,1,33,myntupledir+"/DATARun3On");
      TreeChain(chain,1,3,myntupledir+"/DATARun3Off");
      TreeChain(chain,1,99,myntupledir+"/DATARun4On");
      TreeChain(chain,1,10,myntupledir+"/DATARun4Off");
      TreeChain(chain,1,131,myntupledir+"/DATARun5On");
      TreeChain(chain,1,14,myntupledir+"/DATARun5Off");
      TreeChain(chain,1,53,myntupledir+"/DATARun6On");
      TreeChain(chain,1,4,myntupledir+"/DATARun6Off");
      a->SetChain(chain);
      if(WhichDATASet==10)a->SetReduceCuts(1,1,1,1,1,1);
      if(WhichDATASet==11)a->SetReduceCuts(0,1,1,1,1,1);
      if(WhichDATASet==12)a->SetReduceCuts(1,0,1,1,1,1);
      if(WhichDATASet==13)a->SetReduceCuts(1,1,1,0,1,1);
      if(WhichDATASet==14)a->SetReduceCuts(1,0,1,0,1,1);
      if(WhichDATASet==15)a->SetReduceCuts(1,3,1,3,1,1);
      if(WhichDATASet==16)a->SetReduceCuts(1,2,1,1,1,1);
      if(WhichDATASet==17)a->SetReduceCuts(1,1,1,2,1,1);
    } 
  }


  return a;
}
