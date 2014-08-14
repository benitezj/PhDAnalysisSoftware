#include "SetHisto.h"

void SetHisto(TH1* h, TString title,Int_t nbins,Float_t xmin,Float_t xmax,TString xtitle){
  if(!h)return;
  h->SetName(title);	
  h->SetTitle(title);
  h->SetBins(nbins,xmin,xmax);
  h->SetXTitle(xtitle);

  return;
}
  

void SetHistoXY(TH1* h, TString title,Int_t nbins,Float_t xmin,Float_t xmax,TString xtitle,TString ytitle){
  if(!h)return;

  h->SetName(title);	
  h->SetTitle(title);
  h->SetBins(nbins,xmin,xmax);
  h->SetXTitle(xtitle);
  h->SetYTitle(ytitle);

  return;
}
  


void SetHisto2D(TH2* h, TString title,Int_t nxbins,Float_t xmin,Float_t xmax,TString xtitle,Int_t nybins,Float_t ymin,Float_t ymax,TString ytitle,TString ztitle){
  if(!h)return;

  h->SetName(title);	
  h->SetTitle(title);
  h->SetBins(nxbins,xmin,xmax,nybins,ymin,ymax);
  h->SetXTitle(xtitle); 
  h->SetYTitle(ytitle);
  h->SetZTitle(ztitle);
  h->SetTitleOffset(.68,"Z");

  return;
}
  
void SetHistoXYLabels(TH1* h,TString xtitle,TString yunits){
  if(!h)return;

  h->SetXTitle(xtitle+"   "+yunits);
  
  char txt[20];
  sprintf(txt,"Entries/%.1e ",h->GetBinWidth(1));
  h->SetYTitle(TString(txt)+yunits);

}
