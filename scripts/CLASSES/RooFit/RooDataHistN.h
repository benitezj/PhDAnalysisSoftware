/*****************************************************************************
 * Project: RooFit                                                           *
 * Package: RooFitCore                                                       *
 *    File: $Id: RooDataHist.h,v 1.37 2007/05/11 09:11:30 verkerke Exp $
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
#ifndef ROO_DATA_HISTN
#define ROO_DATA_HISTN

#include "RooTreeData.h"
#include "RooDirItem.h"
#include "RooArgSet.h"
#include "RooNameSet.h"
#include "RooCacheManager.h"
#include <vector>
#include <list>

class TObject ;
class RooAbsArg;
class RooAbsReal ;
class RooAbsCategory ;
class Roo1DTable ;
class RooPlot;
class RooArgSet ;
class RooLinkedList ;
class RooAbsLValue ;

class RooDataHistN : public RooTreeData, public RooDirItem {
public:

  // Constructors, factory methods etc.
  RooDataHistN() ; 
  RooDataHistN(const char *name, const char *title, const RooArgSet& vars) ;
  RooDataHistN(const char *name, const char *title, const RooArgSet& vars, const RooAbsData& data, Double_t initWgt=1.0) ;
  RooDataHistN(const char *name, const char *title, const RooArgList& vars, const TH1* hist, Double_t initWgt=1.0) ;
  RooDataHistN(const RooDataHistN& other, const char* newname = 0) ;
  virtual TObject* Clone(const char* newname=0) const { return new RooDataHistN(*this,newname?newname:GetName()) ; }
  virtual ~RooDataHistN() ;

  virtual RooAbsData* emptyClone(const char* newName=0, const char* newTitle=0, const RooArgSet*vars=0) const {
    return new RooDataHistN(newName?newName:GetName(),newTitle?newTitle:GetTitle(),vars?*vars:*get()) ; 
  }

  // Add one ore more rows of data
  virtual void add(const RooArgSet& row, Double_t weight=1.0) { add(row,weight,-1.) ; }
  virtual void add(const RooArgSet& row, Double_t weight, Double_t sumw2) ;
  void set(Double_t weight, Double_t wgtErr=-1) ;
  void set(const RooArgSet& row, Double_t weight, Double_t wgtErr=-1) ;
  void set(const RooArgSet& row, Double_t weight, Double_t wgtErrLo, Double_t wgtErrHi) ;

  void add(const RooAbsData& dset, const RooFormulaVar* cutVar=0, Double_t weight=1.0 ) ;
  void add(const RooAbsData& dset, const char* cut, Double_t weight=1.0 ) ;

  virtual const RooArgSet* get() const { return &_vars ; } 
  virtual const RooArgSet* get(Int_t masterIdx) const ;
  virtual const RooArgSet* get(const RooArgSet& coord) const ;
  virtual Int_t numEntries(Bool_t useWeights=kFALSE) const ; 
  virtual Double_t sumEntries(const char* cutSpec=0, const char* cutRange=0) const ;
  virtual Bool_t isWeighted() const { return kTRUE ; }

  Double_t sum(Bool_t correctForBinSize) const ;
  Double_t sum(const RooArgSet& sumSet, const RooArgSet& sliceSet, Bool_t correctForBinSize) ;

  virtual Double_t weight() const { return _curWeight ; }
  Double_t weight(const RooArgSet& bin, Int_t intOrder=1, Bool_t correctForBinSize=kFALSE) ;   
  Double_t binVolume() const { return _curVolume ; }
  Double_t binVolume(const RooArgSet& bin) ; 

  TIterator* sliceIterator(RooAbsArg& sliceArg, const RooArgSet& otherArgs) ;
  
  virtual void weightError(Double_t& lo, Double_t& hi, ErrorType etype=Poisson) const ;
  virtual Double_t weightError(ErrorType etype=Poisson) const { 
    Double_t lo,hi ;
    weightError(lo,hi,etype) ;
    return (lo+hi)/2 ;
  }

  using RooTreeData::plotOn ;
  virtual RooPlot *plotOn(RooPlot *frame, PlotOpt o) const;
  
  virtual void reset() ;
  void dump2() ;

  void SetName(const char *name) ;
  void SetNameTitle(const char *name, const char* title) ;

protected:

  friend class RooAbsCachedPdf ;
  friend class RooDataHistNSliceIter ;
  void setAllWeights(Double_t value) ;
 
  void initialize(Bool_t fillTree=kTRUE) ;
  RooDataHistN(const char* name, const char* title, RooDataHistN* h, const RooArgSet& varSubset, 
	      const RooFormulaVar* cutVar, const char* cutRange, Int_t nStart, Int_t nStop, Bool_t copyCache) ;
  RooAbsData* reduceEng(const RooArgSet& varSubset, const RooFormulaVar* cutVar, const char* cutRange=0, 
	                Int_t nStart=0, Int_t nStop=2000000000, Bool_t copyCache=kTRUE) ;
  Double_t interpolateDim(RooRealVar& dim, Double_t xval, Int_t intOrder, Bool_t correctForBinSize) ;
  void calculatePartialBinVolume(const RooArgSet& dimSet) const ;

  virtual RooAbsData* cacheClone(const RooArgSet* newCacheVars, const char* newName=0) ;

  Int_t calcTreeIndex() const ;

  Int_t       _arrSize ; //  Size of the weight array
  Int_t*      _idxMult ; //! Multiplier jump table for index calculation
  Double_t*       _wgt ; //[_arrSize] Weight array
  Double_t*     _errLo ; //[_arrSize] Low-side error on weight array
  Double_t*     _errHi ; //[_arrSize] High-side error on weight array
  Double_t*     _sumw2 ; //[_arrSize] Sum of weights^2
  Double_t*      _binv ; //[_arrSize] Bin volume array  
  RooArgSet  _realVars ; // Real dimensions of the dataset 
  TIterator* _realIter ; //! Iterator over realVars
 
  mutable Double_t _curWeight ; // Weight associated with the current coordinate
  mutable Double_t _curWgtErrLo ; // Error on weight associated with the current coordinate
  mutable Double_t _curWgtErrHi ; // Error on weight associated with the current coordinate
  mutable Double_t _curSumW2 ; // Current sum of weights^2
  mutable Double_t _curVolume ; // Volume of bin enclosing current coordinate
  mutable Int_t    _curIndex ; // Current index

  mutable std::vector<Double_t>* _pbinv ; //! Partial bin volume array
  mutable RooCacheManager<std::vector<Double_t> > _pbinvCacheMgr ; // Cache manager for arrays of partial bin volumes
  std::list<RooAbsLValue*> _lvvars ; //!

private:

  ClassDef(RooDataHistN,1) // Binned data set
};

#endif

