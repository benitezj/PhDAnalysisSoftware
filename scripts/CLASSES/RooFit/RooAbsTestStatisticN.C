/*****************************************************************************
 * Project: RooFit                                                           *
 * Package: RooFitCore                                                       *
 * @(#)root/roofitcore:$Id: RooAbsTestStatistic.cxx 21379 2007-12-14 15:12:29Z wouter $
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

// -- CLASS DESCRIPTION [PDF] --
// RooAbsTestStatistic is the abstract base class for goodness-of-fit
// for all goodness-of-fit variables. Goodness-of-fit quantities that
// evaluate the PDF at each data point should inherit from the RooAbsOptTestStatistic
// class which implements several generic optimizations that can be done for
// such quantities.
//
// This base class organizes calculation of GOF values for RooSimultaneous PDF
// as a sum of GOF-values for the PDF components of the simultaneous PDF
// (more efficient) and organizes parallel calculation of GOF values.
// For the latter, the GOF values is calculated in partitions in parallel executing
// processes and later combined in the main thread.


#include "RooFit.h"

#include "Riostream.h"


#include "RooAbsTestStatisticN.h"
#include "RooAbsPdfN.h"
#include "RooSimultaneous.h"
#include "RooAbsData.h"
#include "RooArgSet.h"
#include "RooRealVar.h"
#include "RooNLLVar.h"
#include "RooRealMPFE.h"
#include "RooErrorHandler.h"
#include "RooMsgService.h"

ClassImp(RooAbsTestStatisticN)
;

RooAbsTestStatisticN::RooAbsTestStatisticN() 
{ 
  _init = kFALSE ; 
  _gofArray = 0 ;
  _mpfeArray = 0 ;
}


RooAbsTestStatisticN::RooAbsTestStatisticN(const char *name, const char *title, RooAbsPdfN& pdf, RooAbsData& data,
					 const RooArgSet& projDeps, const char* rangeName, const char* addCoefRangeName, 
					 Int_t nCPU, Bool_t verbose, Bool_t splitCutRange) : 
  RooAbsReal(name,title),
  _paramSet("paramSet","Set of parameters",this),
  _pdf(&pdf),
  _data(&data),
  _projDeps((RooArgSet*)projDeps.Clone()),
  _rangeName(rangeName?rangeName:""),
  _addCoefRangeName(addCoefRangeName?addCoefRangeName:""),
  _splitRange(splitCutRange),
  _simCount(1),
  _verbose(verbose),
  _nGof(0),
  _gofArray(0),
  _nCPU(nCPU),
  _mpfeArray(0)
{
  // Register all parameters as servers 
  RooArgSet* params = pdf.getParameters(&data) ;
  _paramSet.add(*params) ;
  delete params ;

  if (nCPU>1) {

    _gofOpMode = MPMaster ;

  } else {

    // Determine if PDF is a RooSimultaneous
    Bool_t simMode = dynamic_cast<RooSimultaneous*>(&pdf)?kTRUE:kFALSE ;
    
    if (simMode) {
      _gofOpMode = SimMaster ;
    } else {
      _gofOpMode = Slave ;
    }
  }

  _setNum = 0 ;
  _numSets = 1 ;
  _init = kFALSE ;
  _nEvents = data.numEntries() ;
}



RooAbsTestStatisticN::RooAbsTestStatisticN(const RooAbsTestStatisticN& other, const char* name) : 
  RooAbsReal(other,name), 
  _paramSet("paramSet",this,other._paramSet),
  _pdf(other._pdf),
  _data(other._data),
  _projDeps((RooArgSet*)other._projDeps->Clone()),
  _rangeName(other._rangeName),
  _splitRange(other._splitRange),
  _simCount(other._simCount),
  _verbose(other._verbose),
  _init(other._init),
  _gofOpMode(other._gofOpMode),
  _nEvents(other._nEvents),
  _setNum(other._setNum),
  _numSets(other._numSets),
  _nCPU(other._nCPU)
{
  if (operMode()==SimMaster) {
    _nGof = other._nGof ; 
    _gofArray = new pRooAbsTestStatisticN[_nGof] ;
    Int_t i ;
    for (i=0 ; i<_nGof ; i++) {
      _gofArray[i] = (RooAbsTestStatisticN*) other._gofArray[i]->Clone() ;
    }
  }

  if (operMode()==MPMaster) {
    _nCPU = other._nCPU ; 
    _mpfeArray = new pRooRealMPFE[_nCPU] ;
    Int_t i ;
    for (i=0 ; i<_nCPU ; i++) {
      _mpfeArray[i] = (RooRealMPFE*) other._mpfeArray[i]->Clone() ;
    }
  }

  
}



RooAbsTestStatisticN::~RooAbsTestStatisticN()
{
  if (_gofOpMode==MPMaster && _init) {
    Int_t i ;
    for (i=0 ; i<_nCPU ; i++) {
      delete _mpfeArray[i] ;
    }
    delete[] _mpfeArray ;
  }

  if (_gofOpMode==SimMaster && _init) {
    Int_t i ;
    for (i=0 ; i<_nGof ; i++) {
      delete _gofArray[i] ;      
    }
    delete[] _gofArray ;
  }

  delete _projDeps ;
}



Double_t RooAbsTestStatisticN::evaluate() const
{
  // One-time Initialization
  if (!_init) {
    const_cast<RooAbsTestStatisticN*>(this)->initialize() ;
  }

  if (_gofOpMode==SimMaster) {

    // Evaluate array of owned GOF objects
    Double_t ret = combinedValue((RooAbsReal**)_gofArray,_nGof) ;
    return ret ;

  } else if (_gofOpMode==MPMaster) {

    // Start calculations in parallel 
    Int_t i ;
    for (i=0 ; i<_nCPU ; i++) {
      _mpfeArray[i]->calculate() ;
    }
    Double_t ret = combinedValue((RooAbsReal**)_mpfeArray,_nCPU) ;
    return ret ;

  } else {

    // Evaluate as straight PDF
    Int_t nFirst = _nEvents * _setNum / _numSets ;
    Int_t nLast = _nEvents * (_setNum+1) / _numSets ;
    Double_t ret =  evaluatePartition(nFirst,nLast) ;
    return ret ;

  }
}



Bool_t RooAbsTestStatisticN::initialize() 
{
  if (_init) return kFALSE ;

  if (_gofOpMode==MPMaster) {
    initMPMode(_pdf,_data,_projDeps,_rangeName.size()?_rangeName.c_str():0,_addCoefRangeName.size()?_addCoefRangeName.c_str():0) ;
  } else if (_gofOpMode==SimMaster) {
    initSimMode((RooSimultaneous*)_pdf,_data,_projDeps,_rangeName.size()?_rangeName.c_str():0,_addCoefRangeName.size()?_addCoefRangeName.c_str():0) ;
  }
  _init = kTRUE ;
  return kFALSE ;
}



Bool_t RooAbsTestStatisticN::redirectServersHook(const RooAbsCollection& newServerList, Bool_t mustReplaceAll, Bool_t nameChange, Bool_t) 
{
  if (_gofOpMode==SimMaster) {
    // Forward to slaves
    Int_t i ;
    for (i=0 ; i<_nGof ; i++) {
      if (_gofArray[i]) {
	_gofArray[i]->recursiveRedirectServers(newServerList,mustReplaceAll,nameChange) ;
      }
    }
  } else if (_gofOpMode==MPMaster) {
    // WVE implement this
  }
  return kFALSE ;
}


void RooAbsTestStatisticN::printCompactTreeHook(ostream& os, const char* indent) 
{
  if (_gofOpMode==SimMaster) {
    // Forward to slaves
    Int_t i ;
    os << indent << "RooAbsTestStatisticN begin GOF contents" << endl ;
    for (i=0 ; i<_nGof ; i++) {
      if (_gofArray[i]) {
	TString indent2(indent) ;
	indent2 += Form("[%d] ",i) ;
	_gofArray[i]->printCompactTreeHook(os,indent2) ;
      }
    }
    os << indent << "RooAbsTestStatisticN end GOF contents" << endl ;
  } else if (_gofOpMode==MPMaster) {
    // WVE implement this
  }
}


void RooAbsTestStatisticN::constOptimizeTestStatistic(ConstOpCode opcode) 
{
  Int_t i ;
  initialize() ;
  if (_gofOpMode==SimMaster) {
    // Forward to slaves    
    for (i=0 ; i<_nGof ; i++) {
      if (_gofArray[i]) _gofArray[i]->constOptimizeTestStatistic(opcode) ;
    }
  } else if (_gofOpMode==MPMaster) {
    for (i=0 ; i<_nCPU ; i++) {
      _mpfeArray[i]->constOptimizeTestStatistic(opcode) ;
    }
  }
}



void RooAbsTestStatisticN::setMPSet(Int_t setNum, Int_t numSets) 
{
  _setNum = setNum ; _numSets = numSets ;
  if (_gofOpMode==SimMaster) {
    // Forward to slaves
    initialize() ;
    Int_t i ;
    for (i=0 ; i<_nGof ; i++) {
      if (_gofArray[i]) _gofArray[i]->setMPSet(setNum,numSets) ;
    }
  } 
}


void RooAbsTestStatisticN::initMPMode(RooAbsPdfN* pdf, RooAbsData* data, const RooArgSet* projDeps, const char* rangeName, const char* addCoefRangeName)
{
  Int_t i ;
  _mpfeArray = new pRooRealMPFE[_nCPU] ;

  // Create proto-goodness-of-fit 
  //cout << "initMPMode -- creating prototype gof" << endl ;
  RooAbsTestStatisticN* gof = create(GetName(),GetTitle(),*pdf,*data,*projDeps,rangeName,addCoefRangeName,_verbose,1,_splitRange) ;

  for (i=0 ; i<_nCPU ; i++) {

    //cout << "initMPMode -- tayloring prototype for partition " << i << endl ;
    gof->setMPSet(i,_nCPU) ;
    gof->SetName(Form("%s_GOF%d",GetName(),i)) ;
    gof->SetTitle(Form("%s_GOF%d",GetTitle(),i)) ;
    
    Bool_t doInline = (i==_nCPU-1) ;
    if (!doInline) coutI(Generation) << "RooAbsTestStatisticN::initMPMode: starting remote GOF server process #" << i << endl ; 
    //cout << "initMPMode -- creating MP front-end" << endl ;
    _mpfeArray[i] = new RooRealMPFE(Form("%s_MPFE%d",GetName(),i),Form("%s_MPFE%d",GetTitle(),i),*gof,doInline) ;    
    //cout << "initMPMode -- initializing MP front-end" << endl ;
    _mpfeArray[i]->initialize() ;
  }
  //cout << "initMPMode --- done" << endl ;
  return ;
}


void RooAbsTestStatisticN::initSimMode(RooSimultaneous* simpdf, RooAbsData* data, const RooArgSet* projDeps, const char* rangeName, const char* addCoefRangeName)
{
  RooAbsCategoryLValue& simCat = (RooAbsCategoryLValue&) simpdf->indexCat() ;


  TString simCatName(simCat.GetName()) ;
  TList* dsetList = const_cast<RooAbsData*>(data)->split(simCat) ;
  if (!dsetList) {
    coutE(Fitting) << "RooAbsTestStatisticN::initSimMode(" << GetName() << ") unable to split dataset, abort" << endl ;
    RooErrorHandler::softAbort() ;
  }

  // Count number of used states
  Int_t n(0) ;
  _nGof = 0 ;
  RooCatType* type ;
  TIterator* catIter = simCat.typeIterator() ;
  while((type=(RooCatType*)catIter->Next())){

    // Retrieve the PDF for this simCat state
    RooAbsPdfN* pdf =  (RooAbsPdfN*)simpdf->getPdf(type->GetName()) ;
    RooAbsData* dset = (RooAbsData*) dsetList->FindObject(type->GetName()) ;

    if (pdf && dset && dset->numEntries(kTRUE)!=0.) {      
      _nGof++ ;
    }
  }

  // Allocate arrays 
  _gofArray = new pRooAbsTestStatisticN[_nGof] ;

  // Create array of regular fit contexts, containing subset of data and single fitCat PDF
  catIter->Reset() ;
  while((type=(RooCatType*)catIter->Next())){

    // Retrieve the PDF for this simCat state
    RooAbsPdfN* pdf =  (RooAbsPdfN*)simpdf->getPdf(type->GetName()) ;
    RooAbsData* dset = (RooAbsData*) dsetList->FindObject(type->GetName()) ;

    if (pdf && dset && dset->numEntries(kTRUE)!=0.) {      
      coutE(Fitting) << "RooAbsTestStatisticN::initSimMode: creating slave GOF calculator #" << n << " for state " << type->GetName() 
		     << " (" << dset->numEntries() << " dataset entries)" << endl ;
      if (_splitRange && rangeName) {
	_gofArray[n] = create(type->GetName(),type->GetName(),*pdf,*dset,*projDeps,Form("%s_%s",rangeName,type->GetName()),addCoefRangeName,_nCPU,_verbose,_splitRange) ;
      } else {
	_gofArray[n] = create(type->GetName(),type->GetName(),*pdf,*dset,*projDeps,rangeName,addCoefRangeName,_nCPU,_verbose,_splitRange) ;
      }
      _gofArray[n]->setSimCount(_nGof) ;
      
      // Servers may have been redirected between instantiation and (deferred) initialization
      _gofArray[n]->recursiveRedirectServers(_paramSet) ;
      n++ ;
    } else {
      if ((!dset || dset->numEntries(kTRUE)==0.) && pdf) {
	if (_verbose) {
	  coutI(Fitting) << "RooAbsTestStatisticN::initSimMode: state " << type->GetName() 
			 << " has no data entries, no slave GOF calculator created" << endl ;
	}
      }      
    }
  }

  dsetList->Delete() ;
  delete dsetList ;
  delete catIter ;
}





