#include "AbsDataBase.h"


ClassImp(AbsDataBase);
AbsDataBase::AbsDataBase(){
  SetNameType("database",0,0);
}

AbsDataBase::AbsDataBase(const char* name,Bool_t mcOrData,Bool_t truthMatch){
  SetNameType(name,mcOrData,truthMatch);
}

AbsDataBase::~AbsDataBase(){ 
}

void AbsDataBase::SetNameType(const char* name,Bool_t mcOrData,Bool_t truthMatch){
  SetNameTitle(name,name);
  MCorData=mcOrData;  
  TruthMatch=truthMatch;

  for(Int_t cutid=0;cutid<NCUTS;cutid++){
    //    sprintf(cutstring[cutid],"0");
    cut[cutid]=TCut(TString("")+(long)cutid,TString("")+(long)cutid);//cutstring[cutid];
    cutstatus[cutid]=0;
    cutname[cutid]=TString("")+(long)cutid;//cutstring[cutid];
  }
}

void AbsDataBase::ShowCuts(){
  cout<<"      Going to use the following Cuts:"<<endl;
  for(Int_t cutid=0;cutid<NCUTS;cutid++){        
    if(cutstatus[cutid]){     
      cout<<"Cut "<<cutid+1<<":  "<<cutname[cutid]<<" :  "<<cut[cutid].GetName()<<endl;    
    }
  }  

}

void AbsDataBase::AddCut(TString formula,TString name){
  ///define the cuts  
  Int_t cutid=0;
  while(TString(cut[cutid].GetName())!=(TString("")+(long)cutid)&&cutid<NCUTS)
    cutid++;
  if(cutid==NCUTS){cout<<" no place to put cuts"<<endl; return;}

  cut[cutid]=TCut(formula,formula);        
  cutname[cutid]=name; 
  cutstatus[cutid]=1;     
}

void AbsDataBase::RemoveCut(TString name){
  
  for(Int_t cutid=0;cutid<NCUTS;cutid++) 
    if(cutname[cutid]==name){
      cut[cutid]=TCut(TString("")+(long)cutid,TString("")+(long)cutid);
      cutstatus[cutid]=0;
      cutname[cutid]=TString("")+(long)cutid;
    }

}



TCut AbsDataBase::GetCut(TString CutName){

  for(Int_t cutid=0;cutid<NCUTS;cutid++) //start from 0
    if(cutname[cutid]==CutName)
      return cut[cutid];
  
  return TCut("");
}

TCut AbsDataBase::GetAllCuts(){
  TCut* cutsum=new TCut();
  for(Int_t cutid=0;cutid<NCUTS;cutid++) //start from 0
    if(cutstatus[cutid])
      *cutsum+=cut[cutid];
  
  return *cutsum;
}

void AbsDataBase::DisableCut(TString CutName){
  for(Int_t cutid=0;cutid<NCUTS;cutid++) //start from 0    
    if(cutstatus[cutid]==1)//check other wise get TString comparison error
      if(cutname[cutid]==CutName)
	cutstatus[cutid]=0;
      
}

void AbsDataBase::EnableCut(TString CutName){
  for(Int_t cutid=0;cutid<NCUTS;cutid++) //start from 0 
    if(cutstatus[cutid]==0)//check other wise get TString comparison error   
      if(cutname[cutid]==CutName)
	cutstatus[cutid]=1;
      
}



void AbsDataBase::PrintCutList(TCanvas*C,TString filename){

  TLatex text;
  text.SetTextSize(.04);
  if(C!=NULL)C->Clear();
  for(Int_t cutid=0;cutid<NCUTS;cutid++){
    if(cutstatus[cutid]!=0){
      if(C!=NULL)
	text.DrawTextNDC(.05,.95-.05*cutid,TString(" ")+(long)(cutid+1)+": "+cut[cutid].GetName());
      else
	cout<<"Cut "<<cutid+1<<" "<<cut[cutid].GetName()<<endl;
    }
  }
  if(C!=NULL) C->Print(filename);
 
}



