/*****************************************************************************
 * Project: RooFit                                                           *
 * Package: RooFitCore                                                       *
 *    File: $Id: RooAbsGoodnessOfFit.h,v 1.15 2007/05/11 09:11:30 verkerke Exp $
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
#ifndef ROO_ABS_TEST_STATISTICN
#define ROO_ABS_TEST_STATISTICN

#include "Riosfwd.h"
#include "RooAbsReal.h"
#include "RooSetProxy.h"
#include <string>

#include "./RooAbsPdfN.h"

class RooArgSet ;
class RooAbsData ;
class RooSimultaneous ;
class RooRealMPFE ;

class RooAbsTestStatisticN ;
typedef RooAbsTestStatisticN* pRooAbsTestStatisticN ;
typedef RooAbsData* pRooAbsData ;
typedef RooRealMPFE* pRooRealMPFE ;

class RooAbsTestStatisticN : public RooAbsReal {
public:

  // Constructors, assignment etc
  RooAbsTestStatisticN() ;
  RooAbsTestStatisticN(const char *name, const char *title, RooAbsPdfN& pdf, RooAbsData& data,
		      const RooArgSet& projDeps, const char* rangeName=0, const char* addCoefRangeName=0, 
		      Int_t nCPU=1, Bool_t verbose=kTRUE, Bool_t splitCutRange=kTRUE) ;
  RooAbsTestStatisticN(const RooAbsTestStatisticN& other, const char* name=0);
  virtual ~RooAbsTestStatisticN();
  virtual RooAbsTestStatisticN* create(const char *name, const char *title, RooAbsPdfN& pdf, RooAbsData& data,
				      const RooArgSet& projDeps, const char* rangeName=0, const char* addCoefRangeName=0, 
				      Int_t nCPU=1, Bool_t verbose=kTRUE, Bool_t splitCutRange=kFALSE) = 0 ;

  virtual void constOptimizeTestStatistic(ConstOpCode opcode) ;
  virtual Double_t combinedValue(RooAbsReal** gofArray, Int_t nVal) const = 0 ;

protected:

  virtual void printCompactTreeHook(ostream& os, const char* indent="") ;

  virtual Bool_t redirectServersHook(const RooAbsCollection& newServerList, Bool_t mustReplaceAll, Bool_t nameChange, Bool_t isRecursive) ;
  virtual Double_t evaluate() const ;

  virtual Double_t evaluatePartition(Int_t firstEvent, Int_t lastEvent) const = 0 ;

  void setMPSet(Int_t setNum, Int_t numSets) ; 
  void setSimCount(Int_t simCount) { _simCount = simCount ; }
  void setEventCount(Int_t nEvents) { _nEvents = nEvents ; }
  
  RooSetProxy _paramSet ;

  enum GOFOpMode { SimMaster,MPMaster,Slave } ;
  GOFOpMode operMode() const { return _gofOpMode ; }

  // Original arguments
  RooAbsPdfN* _pdf ;
  RooAbsData* _data ;
  const RooArgSet* _projDeps ;
  std::string _rangeName ; 
  std::string _addCoefRangeName ; 
  Bool_t _splitRange ;
  Int_t _simCount ;
  Bool_t _verbose ;

private:  

  Bool_t initialize() ;
  void initSimMode(RooSimultaneous* pdf, RooAbsData* data, const RooArgSet* projDeps, const char* rangeName, const char* addCoefRangeName) ;    
  void initMPMode(RooAbsPdfN* pdf, RooAbsData* data, const RooArgSet* projDeps, const char* rangeName, const char* addCoefRangeName) ;

  mutable Bool_t _init ; //! 
  GOFOpMode   _gofOpMode ;

  Int_t       _nEvents ;
  Int_t       _setNum ;
  Int_t       _numSets ;

  // Simultaneous mode data
  Int_t          _nGof        ; // Number of sub-contexts 
  pRooAbsTestStatisticN* _gofArray ; //! Array of sub-contexts representing part of the total NLL

  // Parallel mode data
  Int_t          _nCPU ;
  pRooRealMPFE*  _mpfeArray ; //! Array of parallel execution frond ends

  ClassDef(RooAbsTestStatisticN,1) // Abstract real-valued variable
};

#endif
