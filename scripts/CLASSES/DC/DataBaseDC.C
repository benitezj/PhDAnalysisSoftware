#include "DataBaseDC.h"


ClassImp(DataBaseDC);
DataBaseDC::DataBaseDC()
{
  SetNameType("DCDataBase",0,0);
}

DataBaseDC::DataBaseDC(const char* name,Bool_t mcOrData,Bool_t truthMatch)
{
  SetNameType(name,mcOrData,truthMatch);
}

DataBaseDC::~DataBaseDC(){ 
}
void DataBaseDC::SetCuts(){
  
  DMass=DPlusPDGMass;//PDG=1.8692
  if(MCorData){//DATA
    DMass=1.8693;
    cout<<"Data Type: Data"<<endl;
  }

  DMassRangeNbins=130;//1MeV bins
  DMassRangeMin=1.805;
  DMassRangeMax=1.935;


  ///////common cuts
//   DMinLogProbab=log10(.005);//.005 at skim level
//   DPstarMin=1.5;//2.7 , 1.5 at skim level
//   DPstarMax=5.5;
  
  DMassMin=DMass-.03;
  DMassMax=DMass+.03;
   
  ///define the cuts  
  char tempstring[1000]; 
  sprintf(tempstring,"abs(dcharge)==1");
  AddCut(tempstring,"dcharge");

  sprintf(tempstring,"%.5f<dmass&&dmass<%.5f",GetDMassMin(),GetDMassMax());
  AddCut(tempstring,"dmass");
 
  sprintf(tempstring,"%.5f<dmass&&dmass<%.5f",DMass-.015,DMass-.015);
  AddCut(tempstring,"dmasssig");
  DisableCut("dmasssig");
  

  sprintf(tempstring,"(%.5f<dmass&&dmass<%.5f)||(%.5f<dmass&&dmass<%.5f)",
	  GetDMassMin()-2.*(GetDMassMax()-GetDMassMin())/2.,
	  GetDMassMin()-1.*(GetDMassMax()-GetDMassMin())/2.,	  
	  GetDMassMax()+1.*(GetDMassMax()-GetDMassMin())/2.,
	  GetDMassMax()+2.*(GetDMassMax()-GetDMassMin())/2.);
  AddCut(tempstring,"dmasssb");
  DisableCut("dmasssb");

  sprintf(tempstring,"(%.5f<dmass&&dmass<%.5f)",
	  GetDMassMin()-2.*(GetDMassMax()-GetDMassMin())/2.,
	  GetDMassMin()-1.*(GetDMassMax()-GetDMassMin())/2.);
  AddCut(tempstring,"dmasslsb");
  DisableCut("dmasslsb");

  sprintf(tempstring,"(%.5f<dmass&&dmass<%.5f)",	  
	  GetDMassMax()+1.*(GetDMassMax()-GetDMassMin())/2.,
	  GetDMassMax()+2.*(GetDMassMax()-GetDMassMin())/2.);
  AddCut(tempstring,"dmasshsb");
  DisableCut("dmasshsb");


//   sprintf(tempstring,"%.3f<dpstar&&dpstar<%.3f",0.,5.5);
//   AddCut(tempstring,"dpstar");


  ShowCuts();

}

