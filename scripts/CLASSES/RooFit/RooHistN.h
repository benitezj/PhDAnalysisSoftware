/*****************************************************************************
 * Project: RooFit                                                           *
 * Package: RooFitCore                                                       *
 *    File: $Id: RooHist.h,v 1.22 2007/05/11 09:11:30 verkerke Exp $
 * Authors:                                                                  *
 *   WV, Wouter Verkerke, UC Santa Barbara, verkerke@slac.stanford.edu       *
 *   DK, David Kirkby,    UC Irvine,         dkirkby@uci.edu                 *
 *                                                                           *
 * Copyright (c) 2000-2005, Regents of the University of California          *
 *                          and Stanford University. All rights reserved.    *
 *                                                                           *
 * Redistribution and use in source and binary forms,                        *
 * with or without modification, are permitted according to the terms        *
 * listed in LICENSE (http://roofit.sourceforge.net/license.txt)             *
 *****************************************************************************/
#ifndef ROO_HISTN
#define ROO_HISTN

#include "TGraphAsymmErrors.h"
#include "RooPlotable.h"
#include "RooAbsData.h"

class TH1;
class RooCurve ;

class RooHistN : public TGraphAsymmErrors, public RooPlotable {
public:
  RooHistN() {} ;
  RooHistN(Double_t nominalBinWidth, Double_t nSigma= 1, Double_t xErrorFrac=1.0);
  RooHistN(const TH1 &data, Double_t nominalBinWidth= 0, Double_t nSigma= 1, RooAbsData::ErrorType=RooAbsData::Poisson, Double_t xErrorFrac=1.0);
  RooHistN(const TH1 &data1, const TH1 &data2, Double_t nominalBinWidth= 0, Double_t nSigma= 1, Double_t xErrorFrac=1.0);
  RooHistN(const RooHistN& hist1, const RooHistN& hist2, Double_t wgt1=1.0, Double_t wgt2=1.0, RooAbsData::ErrorType etype=RooAbsData::Poisson, Double_t xErrorFrac=1.0) ;
  virtual ~RooHistN();

  // add a datapoint for a bin with n entries, using a Poisson error
  void addBin(Axis_t binCenter, Int_t n, Double_t binWidth= 0, Double_t xErrorFrac=1.0);
  // add a datapoint for a bin with n entries, using a given error
  void addBinWithError(Axis_t binCenter, Double_t n, Double_t elow, Double_t ehigh, Double_t binWidth= 0, Double_t xErrorFrac=1.0);
  // add a datapoint for the asymmetry (n1-n2)/(n1+n2), using a binomial error
  void addAsymmetryBin(Axis_t binCenter, Int_t n1, Int_t n2, Double_t binWidth= 0, Double_t xErrorFrac=1.0);

  virtual void printToStream(ostream& os, PrintOption opt= Standard, TString indent= "") const;
  inline virtual void Print(Option_t *options= 0) const {
    printToStream(defaultStream(),parseOptions(options));
  }

  Double_t getFitRangeNEvt() const;
  Double_t getFitRangeNEvt(Double_t xlo, Double_t xhi) const ;
  Double_t getFitRangeBinW() const;
  inline Double_t getNominalBinWidth() const { return _nominalBinWidth; }
  inline void setRawEntries(Double_t n) { _rawEntries = n ; } 

  Bool_t hasIdenticalBinning(const RooHistN& other) const ;

  RooHistN* makeResidHist(const RooCurve& curve,bool normalize=false) const;
  RooHistN* makePullHist(const RooCurve& curve) const {return makeResidHist(curve,false); }


  Bool_t isIdentical(const RooHistN& other, Double_t tol=1e-6) const ;


protected:
  void initialize();
  Int_t roundBin(Double_t y);

private:
  Double_t _nominalBinWidth,_nSigma,_entries,_rawEntries;
  ClassDef(RooHistN,1) // 1-dimensional histogram with error bars
};

#endif
