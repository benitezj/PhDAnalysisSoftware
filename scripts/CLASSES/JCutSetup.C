#include "JCutSetup.h"

ClassImp(JCutSetup);
JCutSetup::JCutSetup():
  _mainquantity(NULL),
  _mainquantityint(NULL),
  floatorint(1),
  _title("title"),
  _nbins(0),
  _low(0),
  _high(0),
  _xtitle("x"),
  _ytitle("y"),
  CutCounter(0),
  MadeRatios(kFALSE)
{
  CreateHistos(CutCounter);
}

JCutSetup::JCutSetup(Float_t* mainquantity,TString title,Int_t nbins,Float_t low,Float_t high,TString xtitle,TString ytitle):
  _mainquantity(mainquantity),
  floatorint(1),
  _title(title),
  _nbins(nbins),
  _low(low),
  _high(high),
  _xtitle(xtitle),
  _ytitle(ytitle),
  CutCounter(0),
  MadeRatios(kFALSE)
{
  CreateHistos(CutCounter);
}

void JCutSetup::Init(Float_t* mainquantity,TString title,Int_t nbins,Float_t low,Float_t high,TString xtitle,TString ytitle){
  _mainquantity=mainquantity;
  floatorint=1;
  _title=title;
  _nbins=nbins;
  _low=low;
  _high=high;
  _xtitle=xtitle;
  _ytitle=ytitle;
  MadeRatios=kFALSE;
  CutCounter=0;
  CreateHistos(CutCounter);
}

void JCutSetup::InitInt(Int_t* mainquantity,TString title,Int_t nbins,Float_t low,Float_t high,TString xtitle,TString ytitle){
  _mainquantityint=mainquantity;
  floatorint=2;
  _title=title;
  _nbins=nbins;
  _low=low;
  _high=high;
  _xtitle=xtitle;
  _ytitle=ytitle;
  MadeRatios=kFALSE;
  CutCounter=0;
  CreateHistos(CutCounter);
}


JCutSetup::~JCutSetup(){ 

  for(Int_t id=0;id<=CutCounter;id++)
    delete HMainQuantity[id];

  for(Int_t id=0;id<=CutCounter;id++)
    delete HMainQuantityBackground[id];
  
  if(MadeRatios)
    for(Int_t id=0;id<=CutCounter;id++)
      delete HMainQuantityRatio[id];
}

void JCutSetup::CreateHistos(Int_t id){
  //uncut histograms
  TString cutname;
  if(id==0)cutname="0";
  else cutname=Cut[id]->GetCutName();
  
  HMainQuantity[id]=new TH1F();
  SetHistoXY(HMainQuantity[id],_title+" Cut:"+cutname,_nbins,_low,_high,_xtitle,_ytitle);
  HMainQuantityBackground[id]=new TH1F();
  SetHistoXY(HMainQuantityBackground[id],_title+" Bkg Cut:"+cutname,_nbins,_low,_high,_xtitle,_ytitle);
}
void JCutSetup::DeleteHistos(Int_t id){
  delete HMainQuantity[id];
  delete HMainQuantityBackground[id];
  if(MadeRatios)delete HMainQuantityRatio[id];
}

void JCutSetup::AddCut(JCut* cut){
  
  CutCounter++;//Note that array element CutCounter=0 is not filled
  Cut[CutCounter]=cut;

  CreateHistos(CutCounter);

}

void JCutSetup::RemoveCut(JCut* cutpointer){
  if(!cutpointer)return;

  //first identify which cut id
  Bool_t foundcut=0;
  for(Int_t c=1;c<=CutCounter;c++){
    if(Cut[c]==cutpointer){
      DeleteHistos(c);
      foundcut=1;
    }
    
    if(foundcut==1&&c<CutCounter){
      Cut[c]=Cut[c+1];//move cuts down;
      CreateHistos(c);//move histos  down
    }
    
  }
 
  //now that cuts have moved down delete the last one
  DeleteHistos(CutCounter);
  CutCounter--;//total number of cuts goes down
}

void JCutSetup::FillPlots(){
  if(_mainquantity==NULL&&_mainquantityint==NULL){
    cout<<"JCutSetup probably not initialized."<<endl;
    return;
  }

  Bool_t pass=kTRUE;
  Bool_t passprev=kTRUE;

  //fill the uncut histo
  if(floatorint==1)HMainQuantity[0]->Fill(*_mainquantity);
  if(floatorint==2)HMainQuantity[0]->Fill(*_mainquantityint);  
  
  if(CutCounter==0) return;
  
  for(Int_t cutid=1;cutid<=CutCounter;cutid++){
    pass=kTRUE;
    passprev=kTRUE;
    //background must all previous cuts
    for(Int_t id=1;id<=cutid-1;id++){
      if(Cut[id]->EvalCut()==kFALSE) passprev=kFALSE;
    }
    
    //signal must pass all previous plus this one cut
    for(Int_t id=1;id<=cutid;id++){
      if(Cut[id]->EvalCut()==kFALSE) pass=kFALSE;
    }
    
  
    if(pass){
      if(floatorint==1)
	HMainQuantity[cutid]->Fill(*_mainquantity);
      if(floatorint==2)
	HMainQuantity[cutid]->Fill(*_mainquantityint);
    }else{
      if(passprev){
	if(floatorint==1)
	  HMainQuantityBackground[cutid]->Fill(*_mainquantity);
	if(floatorint==2)
	  HMainQuantityBackground[cutid]->Fill(*_mainquantityint);
      }
    }
  }

}
void JCutSetup::DrawPlots(Int_t idmin,Int_t idmax,TString opt){

  if(idmax>CutCounter)idmax=CutCounter;
  if(idmin>CutCounter)idmin=CutCounter;
  if(idmin<0)idmin=CutCounter+idmin;
  if(idmax<0)idmax=CutCounter+idmax;
  if(idmin>idmax)idmax=idmin;

  HMainQuantity[idmin]->GetYaxis()->SetRangeUser(0.1,1.05*HMainQuantity[idmin]->GetMaximum());      
  HMainQuantity[idmin]->Draw(opt);
  
  for(Int_t id=idmin+1;id<=idmax;id++)
    HMainQuantity[id]->Draw("same");
    
}

void JCutSetup::DrawRatios(Int_t idmin,Int_t idmax,TH1F* href,TString opt){
  if(idmax>CutCounter)idmax=CutCounter;
  if(idmin>CutCounter)idmin=CutCounter;
  if(idmin<0)idmin=CutCounter+idmin;
  if(idmax<0)idmax=CutCounter+idmax;
  if(idmin>idmax)idmax=idmin;

  MadeRatios=kTRUE;

  ///First we compute all the ratios
  for(Int_t id=0;id<=CutCounter;id++){
    HMainQuantityRatio[id]=(TH1F*)HMainQuantity[id]->Clone();
    for(Int_t bin=1;bin<=HMainQuantityRatio[id]->GetNbinsX();bin++)
      if(href->GetBinContent(bin)>500)
	HMainQuantityRatio[id]->SetBinContent(bin,HMainQuantity[id]->GetBinContent(bin)/href->GetBinContent(bin));
      else HMainQuantityRatio[id]->SetBinContent(bin,0);  
    HMainQuantityRatio[id]->SetStats(kFALSE);    
    HMainQuantityRatio[id]->GetYaxis()->SetTitle("ratio");
  }
  
  
  HMainQuantityRatio[idmin]->GetYaxis()->SetRangeUser(0,HMainQuantityRatio[idmin]->GetBinContent(HMainQuantityRatio[idmin]->GetMaximumBin()));      
  HMainQuantityRatio[idmin]->Draw(opt);
  
  for(Int_t id=idmin+1;id<=idmax;id++)
    HMainQuantityRatio[id]->Draw("same");
    
}

void JCutSetup::DrawBackgrounds(Int_t idmin,Int_t idmax,TString opt){
  if(idmax>CutCounter)idmax=CutCounter;
  if(idmin>CutCounter)idmin=CutCounter;
  if(idmin<0)idmin=CutCounter+idmin;
  if(idmax<0)idmax=CutCounter+idmax;
  if(idmin>idmax)idmax=idmin;


  
  HMainQuantityBackground[idmax]->GetYaxis()->SetRangeUser(0.1,1.05*HMainQuantityBackground[idmax]->GetMaximum());      
  HMainQuantityBackground[idmax]->Draw(opt);
  
  for(Int_t id=idmax-1;id>=idmin;id--)
    HMainQuantityBackground[id]->Draw("same");
  
  

}


TH1F* JCutSetup::GetPlot(Int_t id){
 
  if(id>CutCounter)id=CutCounter;
  if(id<0)id=CutCounter+id;//start counting from the end down
  
  return HMainQuantity[id];//id=0 is no cuts,
}
TH1F* JCutSetup::GetPlotByName(TString cutname){
  TH1F* h=NULL;

  for(Int_t id=1;id<=CutCounter;id++){
    if(Cut[id]->GetCutName()==cutname)
      h=HMainQuantity[id];
    
  }
  if(h==NULL){
    cout<<"JCutSetup: plot "<<cutname<<" does not exist"<<endl;
    h=HMainQuantity[0];
  }
  h->GetYaxis()->SetRangeUser(.1,h->GetMaximum());
  return h;//id=0 is no cuts,
}
TH1F* JCutSetup::GetRatio(Int_t id){
 
  if(id>CutCounter)id=CutCounter;
  if(id<0)id=CutCounter+id;
 
  if(MadeRatios)
    return HMainQuantityRatio[id];
  
  cout<<"JCutSetup:Ratios have not been made"<<endl;
  return NULL;
}
TH1F* JCutSetup::GetBackground(Int_t id){
  if(id>CutCounter)id=CutCounter;
  if(id<0)id=CutCounter+id;
 
  return HMainQuantityBackground[id];
}
TH1F* JCutSetup::GetBackgroundByName(TString cutname){
  TH1F* h=NULL;
  for(Int_t id=1;id<=CutCounter;id++){
    if(Cut[id]->GetCutName()==cutname)
      h=HMainQuantityBackground[id];
    
  }
  
  if(h==NULL){
     cout<<"JCutSetup: plot "<<cutname<<" does not exist"<<endl;
     h=HMainQuantityBackground[0];
  }
  h->GetYaxis()->SetRangeUser(.1,h->GetMaximum());
  return h;//id=0 is no cuts,

}


void JCutSetup::AddCutsByList(JCutList* cutlist){

  for(Int_t cutid=1;cutid<=cutlist->GetNCuts();cutid++){
    AddCut(cutlist->GetCut(cutid));
  }

}
