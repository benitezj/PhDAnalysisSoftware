#include "../Globals.h"

#ifndef REBINHISTO_H
#define REBINHISTO_H 1

TH1* RebinHistoX(TH1* oldhisto,Int_t nbins,Float_t min,Float_t max);

TH2F* RebinHistoXY(TH2* oldhisto,Int_t nbins,Float_t min,Float_t max,Int_t prevBinning);

#endif 
