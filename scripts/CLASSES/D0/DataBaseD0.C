#include "DataBaseD0.h"


ClassImp(DataBaseD0);
DataBaseD0::DataBaseD0()
{
  SetNameType("D0DataBase",0,0);
}

DataBaseD0::DataBaseD0(const char* name,Bool_t mcOrData,Bool_t truthMatch)
{
  SetNameType(name,mcOrData,truthMatch);
}

DataBaseD0::~DataBaseD0(){ 
}
void DataBaseD0::SetCuts(){
  
  DMass=DPlusPDGMass;//PDG=1.8692
  if(MCorData){//DATA
    DMass=1.8693;
    cout<<"Data Type: Data"<<endl;
  }

  DMassRangeNbins=100;//1MeV bins
  DMassRangeMin=1.820;
  DMassRangeMax=1.920;


  ///////common cuts
//   DMinLogProbab=log10(.005);//.005 at skim level
//   DPstarMin=1.5;//2.7 , 1.5 at skim level
//   DPstarMax=5.5;
  
  DMassMin=DMass-.015;
  DMassMax=DMass+.015;
   
  ///define the cuts  
  char tempstring[1000]; 
  sprintf(tempstring,"abs(dcharge)==1");
  AddCut(tempstring,"dcharge");

  sprintf(tempstring,"%.5f<dmass&&dmass<%.5f",GetDMassRangeMin(),GetDMassRangeMax());
  AddCut(tempstring,"dmass");
 

  sprintf(tempstring,"%.5f<dmass&&dmass<%.5f",GetDMassMin(),GetDMassMax());
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

