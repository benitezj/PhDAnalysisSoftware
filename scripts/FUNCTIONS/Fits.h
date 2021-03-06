#ifndef Fits_hh
#define Fits_hh

#include <TROOT.h>
#include <iostream>
#include <math.h>
#include <TH1F.h>
#include <TF1.h>
#include <TLatex.h>

using namespace std;


TF1* FitHistogram1gaus(TH1F* h,Float_t low,Float_t high);
TF1* FitHistogram1gausPlusConstant(TH1F* h,Float_t low,Float_t high);
TF1* FitHistogram1gausPlusPol1(TH1F* h,Float_t low,Float_t high);
TF1* FitHistogram1gausPlusPolN(TH1F* h,Float_t low,Float_t high,Int_t N,Float_t* initpar);
void Draw1gausPlusConstant(TF1* f);
void Draw1gausPlusPolN(TF1* f,Int_t N);

TF1* FitHistogram1gausPlusBoltzmanModified(TH1F* h,Float_t low,Float_t high);
void Draw1gausPlusBoltzmanModified(TF1* f);


TF1* FitHistogram2gaus(TH1F* h,Float_t low,Float_t high);
TF1* FitHistogram2gausPlusConstant(TH1F* h,Float_t low,Float_t high);
TF1* FitHistogram2gausPlusPolN(TH1F* h,Float_t low,Float_t high,Int_t N,Float_t* initpar);
TF1* FitHistogram2gausWithSameMeanPlusPolN(TH1F* h,Float_t low,Float_t high,Int_t N,Float_t* initpar,TF1*Bkg);
TF1* FitHistogram2gausWith1GaussFixed(TH1F* h,Float_t backnorm,Float_t backmean,Float_t backsigma,Float_t low,Float_t high);
void Draw2gaus(TF1* f);
void Draw2gausPlusConstant(TF1* f);
void Draw2gausPlusPolN(TF1* f,Int_t N);


TF1* FitHistogram3gaus(TH1F* h,Float_t low,Float_t high);
TF1* FitHistogram3gausPlusPolN(TH1F* h,Float_t low,Float_t high,Int_t N,Float_t* initpar);
void Draw3gaus(TF1* f);
//void Draw3gausPlusPolN(TF1* f);


TF1* FitHistogramPolN(TH1F* h,Float_t low,Float_t high,Int_t N,Float_t* initpar);
TF1* FitHistogramExponential(TH1F* h,Float_t low,Float_t high);


///Ds1p Fits
TF1* FitHistogramRelBreitWignerPolN(TH1F* h,Float_t low,Float_t high,Int_t N,Float_t* initpar);
TF1* FitHistogramBreitWignerPolN(TH1F* h,Float_t low,Float_t high,Int_t N,Float_t* initpar);
TF1* FitHistogramRelBreitWigner(TH1F* h,Float_t low,Float_t high);
TF1* FitHistogramBreitWigner(TH1F* h,Float_t low,Float_t high);

TF1* FitHistogramDs1Resolution(TH1F* h,Float_t fitxmin,Float_t fitxmax);
void DrawDs1Resolution(TF1*f);


//DstPi fits
Float_t FitHistogram2BWPlusPolN(TH1F*h,Float_t fitmin,Float_t fitmax,Int_t N,TString phasefactor);


//Dstar Fit
TF1*  FitHistogramDstarDM(TH1F*h,Float_t fitmin,Float_t fitmax,TF1* Signal,TF1* Bkg);
TF1*  FitHistogramDstar0DM(TH1F*h,Float_t fitmin,Float_t fitmax,TF1* Signal,TF1* Bkg);


#endif
