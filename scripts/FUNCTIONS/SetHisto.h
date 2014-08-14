#ifndef SetHisto_HH
#define SetHisto_HH

#include "../Globals.h"

void SetHisto(TH1* h,TString title,
	      Int_t nbins,Float_t xmin,Float_t xmax,TString xtitle);

void SetHistoXY(TH1* h,TString title,
		Int_t nbins,Float_t xmin,
		Float_t xmax,TString xtitle,TString ytitle);

void SetHisto2D(TH2* h,TString title,
		Int_t nxbins,Float_t xmin,Float_t xmax,TString xtitle,
		Int_t nybins,Float_t ymin,Float_t ymax,TString ytitle,
		TString ztitle);

void SetHistoXYLabels(TH1* h,TString xtitle,TString ytitle);

#endif
