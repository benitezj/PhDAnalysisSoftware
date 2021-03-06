/*****************************************************************************
 * Project: RooFit                                                           *
 * Package: RooFitCore                                                       *
 * @(#)root/roofitcore:$Id: RooAbsOptTestStatistic.cxx 21730 2008-01-16 11:38:50Z brun $
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
// RooAbsOptTestStatistic is the abstract base class for goodness-of-fit
// variables that evaluate the PDF at each point of a given dataset.
// This class provides generic optimizations, such as caching and precalculation
// of constant terms that can be made for all such quantities
//
// Implementations should define evaluatePartition(), which calculates the
// value of a (sub)range of the dataset and optionally combinedValue(),
// which combines the values calculated for each partition. If combinedValue()
// is overloaded, the default implementation will add the partition results
// to obtain the combined result
//
// Support for calculation in partitions is needed to allow parallel calculation
// of goodness-of-fit values.

#include "RooFit.h"

#include "Riostream.h"
#include <string.h>


#include "RooAbsOptTestStatisticN.h"
#include "RooMsgService.h"
#include "RooAbsPdfN.h"
#include "RooAbsData.h"
#include "RooArgSet.h"
#include "RooRealVar.h"
#include "RooErrorHandler.h"
#include "RooGlobalFunc.h"

ClassImp(RooAbsOptTestStatisticN)
;

RooAbsOptTestStatisticN:: RooAbsOptTestStatisticN()
{
  _normSet = 0 ;
  _pdfCloneSet = 0 ;
  _dataClone = 0 ;
  _pdfClone = 0 ;
  _projDeps = 0 ;
}

RooAbsOptTestStatisticN::RooAbsOptTestStatisticN(const char *name, const char *title, RooAbsPdfN& pdf, RooAbsData& data,
					 const RooArgSet& projDeps, const char* rangeName, const char* addCoefRangeName,
					       Int_t nCPU, Bool_t verbose, Bool_t splitCutRange) : 
  RooAbsTestStatisticN(name,title,pdf,data,projDeps,rangeName, addCoefRangeName, nCPU, verbose, splitCutRange),
  _projDeps(0)
{

  // Don't do a thing in master mode
  if (operMode()!=Slave) {
    _normSet = 0 ;
    return ;
  }

  RooArgSet obs(*data.get()) ;
  obs.remove(projDeps,kTRUE,kTRUE) ;

  // Check that the PDF is valid for use with this dataset
  // Check if there are any unprotected multiple occurrences of dependents
  if (pdf.recursiveCheckObservables(&obs)) {
    coutE(InputArguments) << "RooAbsOptTestStatisticN: ERROR in PDF dependents, abort" << endl ;
    RooErrorHandler::softAbort() ;
    return ;
  }
  
  
  // Check if the fit ranges of the dependents in the data and in the PDF are consistent
  RooArgSet* pdfDepSet = pdf.getObservables(&data) ;
  const RooArgSet* dataDepSet = data.get() ;
  TIterator* iter = pdfDepSet->createIterator() ;
  RooAbsArg* arg ;
  while((arg=(RooAbsArg*)iter->Next())) {
    RooRealVar* pdfReal = dynamic_cast<RooRealVar*>(arg) ;
    if (!pdfReal) continue ;
    
    RooRealVar* datReal = dynamic_cast<RooRealVar*>(dataDepSet->find(pdfReal->GetName())) ;
    if (!datReal) continue ;
    
    if (pdfReal->getMin()<(datReal->getMin()-1e-6)) {
      coutE(InputArguments) << "RooAbsOptTestStatisticN: ERROR minimum of PDF variable " << arg->GetName() 
			    << "(" << pdfReal->getMin() << ") is smaller than that of " 
			    << arg->GetName() << " in the dataset (" << datReal->getMin() << ")" << endl ;
      RooErrorHandler::softAbort() ;
      return ;
    }
    
    if (pdfReal->getMax()>(datReal->getMax()+1e-6)) {
      coutE(InputArguments) << "RooAbsOptTestStatisticN: ERROR maximum of PDF variable " << arg->GetName() 
			    << " is smaller than that of " << arg->GetName() << " in the dataset" << endl ;
      RooErrorHandler::softAbort() ;
      return ;
    }
    
  }

  
  // Copy data and strip entries lost by adjusted fit range, _dataClone ranges will be copied from pdfDepSet ranges
  if (rangeName && strlen(rangeName)) {
    _dataClone = ((RooAbsData&)data).reduce(RooFit::SelectVars(*pdfDepSet),RooFit::CutRange(rangeName)) ;  
  } else {
    _dataClone = ((RooAbsData&)data).reduce(RooFit::SelectVars(*pdfDepSet)) ;  
  }
  
  if (rangeName && strlen(rangeName)) {
    
    cxcoutI(Fitting) << "RooAbsOptTestStatisticN::ctor(" << GetName() << ") constructing likelihood for sub-range named " << rangeName << endl ;

    // Adjust PDF normalization ranges to requested fitRange, store original ranges for RooAddPdf coefficient interpretation
    TIterator* iter2 = _dataClone->get()->createIterator() ;
    while((arg=(RooAbsArg*)iter2->Next())) {
      RooRealVar* pdfReal = dynamic_cast<RooRealVar*>(arg) ;
      if (pdfReal) {
        if (!(addCoefRangeName && strlen(addCoefRangeName))) {
	  pdfReal->setRange(Form("NormalizationRangeFor%s",rangeName),pdfReal->getMin(),pdfReal->getMax()) ;
	}
	pdfReal->setRange(pdfReal->getMin(rangeName),pdfReal->getMax(rangeName)) ;
      }
    }

    // Mark fitted range in original PDF dependents for future use
    if (!_splitRange) {
      iter->Reset() ;
      while((arg=(RooAbsArg*)iter->Next())) {      
	RooRealVar* pdfReal = dynamic_cast<RooRealVar*>(arg) ;
	if (pdfReal) {
	  pdfReal->setRange("fit",pdfReal->getMin(rangeName),pdfReal->getMax(rangeName)) ;
	}
      }
    }
    delete iter2 ;
  }
  delete iter ;

  delete pdfDepSet ;  

  setEventCount(_dataClone->numEntries()) ;
 
 
  // Clone all PDF compents by copying all branch nodes
  RooArgSet tmp("PdfBranchNodeList") ;
  pdf.branchNodeServerList(&tmp) ;
  _pdfCloneSet = (RooArgSet*) tmp.snapshot(kFALSE) ;
  
  // Find the top level PDF in the snapshot list
  _pdfClone = (RooAbsPdfN*) _pdfCloneSet->find(pdf.GetName()) ;

  // Fix RooAddPdf coefficients to original normalization range
  if (rangeName) {
    // WVE Remove projected dependents from normalization
    _pdfClone->fixAddCoefNormalization(*_dataClone->get()) ;
    
    if (addCoefRangeName) {
      cxcoutI(Fitting) << "RooAbsOptTestStatisticN::ctor(" << GetName() << ") fixing interpretation of coefficients of any RooAddPdf component to range " << addCoefRangeName << endl ;
      _pdfClone->fixAddCoefRange(addCoefRangeName,kFALSE) ;
    } else {
      cxcoutI(Fitting) << "RooAbsOptTestStatisticN::ctor(" << GetName() << ") fixing interpretation of coefficients of any RooAddPdf to full domain of observables " << endl ;
      _pdfClone->fixAddCoefRange(Form("NormalizationRangeFor%s",rangeName),kFALSE) ;
    }
  }

  // Attach PDF to data set
  _pdfClone->attachDataSet(*_dataClone) ;

  // Store normalization set  
  _normSet = (RooArgSet*) data.get()->snapshot(kFALSE) ;

  // Remove projected dependents from normalization set
  if (projDeps.getSize()>0) {

    _projDeps = (RooArgSet*) projDeps.snapshot(kFALSE) ;
    
    RooArgSet* tobedel = (RooArgSet*) _normSet->selectCommon(*_projDeps) ;
    _normSet->remove(*_projDeps,kTRUE,kTRUE) ;
    
    // Delete owned projected dependent copy in _normSet
    TIterator* ii = tobedel->createIterator() ;
    RooAbsArg* aa ;
    while((aa=(RooAbsArg*)ii->Next())) {
      delete aa ;
    }
    delete ii ;
    delete tobedel ;

    // Mark all projected dependents as such
    RooArgSet *projDataDeps = (RooArgSet*) _dataClone->get()->selectCommon(*_projDeps) ;
    projDataDeps->setAttribAll("projectedDependent") ;
    delete projDataDeps ;
  } 

  // Add parameters as servers
  RooArgSet* params = _pdfClone->getParameters(_dataClone) ;
  _paramSet.add(*params) ;
  delete params ;

  // Mark all projected dependents as such
  if (_projDeps) {
    RooArgSet *projDataDeps = (RooArgSet*) _dataClone->get()->selectCommon(*_projDeps) ;
    projDataDeps->setAttribAll("projectedDependent") ;
    delete projDataDeps ;
  }

  coutI(Optimization) << "RooAbsOptTestStatisticN::ctor(" << GetName() << ") optimizing internal clone of p.d.f for likelihood evaluation." 
			<< "Lazy evaluation and associated change tracking will disabled for all nodes that depend on observables" << endl ;

  optimizeCaching() ;
  
}




RooAbsOptTestStatisticN::RooAbsOptTestStatisticN(const RooAbsOptTestStatisticN& other, const char* name) : 
  RooAbsTestStatisticN(other,name)
{
  // Don't do a thing in master mode
  if (operMode()!=Slave) {
    _projDeps = 0 ;
    _normSet = other._normSet ? ((RooArgSet*) other._normSet->snapshot()) : 0 ;   
    return ;
  }

  _pdfCloneSet = (RooArgSet*) other._pdfCloneSet->snapshot(kFALSE) ;
  _pdfClone = (RooAbsPdfN*) _pdfCloneSet->find(other._pdfClone->GetName()) ;

  // Copy the operMode attribute of all branch nodes
  TIterator* iter = _pdfCloneSet->createIterator() ;
  RooAbsArg* branch ;
  while((branch=(RooAbsArg*)iter->Next())) {
    branch->setOperMode(other._pdfCloneSet->find(branch->GetName())->operMode()) ;
  }
  delete iter ;

  // WVE Must use clone with cache redirection here
  _dataClone = (RooAbsData*) other._dataClone->cacheClone(_pdfCloneSet) ;

  _pdfClone->attachDataSet(*_dataClone) ;
  _normSet = (RooArgSet*) other._normSet->snapshot() ;
  
  if (other._projDeps) {
    _projDeps = (RooArgSet*) other._projDeps->snapshot() ;
  } else {
    _projDeps = 0 ;
  }
}



RooAbsOptTestStatisticN::~RooAbsOptTestStatisticN()
{
  if (operMode()==Slave) {
    delete _pdfCloneSet ;
    delete _dataClone ;
    delete _projDeps ;
  } 
  delete _normSet ;
}



Double_t RooAbsOptTestStatisticN::combinedValue(RooAbsReal** array, Int_t n) const
{
  // Default implementation returns sum of components
  Double_t sum(0) ;
  Int_t i ;
  for (i=0 ; i<n ; i++) {
    Double_t tmp = array[i]->getVal() ;
    if (tmp==0) return 0 ;
    sum += tmp ;
  }
  return sum ;
}


Bool_t RooAbsOptTestStatisticN::redirectServersHook(const RooAbsCollection& newServerList, Bool_t mustReplaceAll, Bool_t nameChange, Bool_t isRecursive) 
{
  RooAbsTestStatisticN::redirectServersHook(newServerList,mustReplaceAll,nameChange,isRecursive) ;
  if (operMode()!=Slave) return kFALSE ;  
  Bool_t ret = _pdfClone->recursiveRedirectServers(newServerList,kFALSE,nameChange) ;
  return ret ;
}


void RooAbsOptTestStatisticN::printCompactTreeHook(ostream& os, const char* indent) 
{
  RooAbsTestStatisticN::printCompactTreeHook(os,indent) ;
  if (operMode()!=Slave) return ;
  TString indent2(indent) ;
  indent2 += ">>" ;
  _pdfClone->printCompactTree(os,indent2) ;
}



void RooAbsOptTestStatisticN::constOptimizeTestStatistic(ConstOpCode opcode) 
{
  // Driver function to propagate const optimization
  RooAbsTestStatisticN::constOptimizeTestStatistic(opcode);
  if (operMode()!=Slave) return ;

  switch(opcode) {
  case Activate:     
    cxcoutI(Optimization) << "RooAbsOptTestStatisticN::constOptimize(" << GetName() << ") optimizing evaluation of test statistic by finding all nodes in p.d.f that depend exclusively"
			    << " on observables and constant parameters and precalculating their values" << endl ;
    optimizeConstantTerms(kTRUE) ;
    break ;
  case DeActivate:  
    cxcoutI(Optimization) << "RooAbsOptTestStatisticN::constOptimize(" << GetName() << ") deactivating optimization of constant terms in test statistic" << endl ;
    optimizeConstantTerms(kFALSE) ;
    break ;
  case ConfigChange: 
    cxcoutI(Optimization) << "RooAbsOptTestStatisticN::constOptimize(" << GetName() << ") one ore more parameter were changed from constant to floating or vice versa, "
			    << "re-evaluating constant term optimization" << endl ;
    optimizeConstantTerms(kFALSE) ;
    optimizeConstantTerms(kTRUE) ;
    break ;
  case ValueChange: 
    cxcoutI(Optimization) << "RooAbsOptTestStatisticN::constOptimize(" << GetName() << ") the value of one ore more constant parameter were changed re-evaluating constant term optimization" << endl ;
    optimizeConstantTerms(kFALSE) ;
    optimizeConstantTerms(kTRUE) ;
    break ;
  }
}


void RooAbsOptTestStatisticN::optimizeCaching() 
{
  // This method changes the value caching logic for all nodes that depends on any of the observables
  // as defined by the given dataset. When evaluating a test statistic constructed from the RooAbsReal
  // with a dataset the observables are guaranteed to change with every call, thus there is no point
  // in tracking these changes which result in a net overhead. Thus for observable-dependent nodes, 
  // the evaluation mechanism is changed from being dependent on a 'valueDirty' flag to guaranteed evaluation. 
  // On the dataset side, the observables objects are modified to no longer send valueDirty messages
  // to their client 

  // Trigger create of all object caches now in nodes that have deferred object creation
  // so that cache contents can be processed immediately
  _pdfClone->getVal(_normSet) ;

  // Set value caching mode for all nodes that depend on any of the observables to ADirty
  _pdfClone->optimizeCacheMode(*_dataClone->get()) ;

  // Disable propagation of dirty state flags for observables
  _dataClone->setDirtyProp(kFALSE) ;  

  // Disable reading of observables that are not used
  _dataClone->optimizeReadingWithCaching(*_pdfClone, RooArgSet()) ;
}


void RooAbsOptTestStatisticN::optimizeConstantTerms(Bool_t activate)
{
  if(activate) {
    // Trigger create of all object caches now in nodes that have deferred object creation
    // so that cache contents can be processed immediately
    _pdfClone->getVal(_normSet) ;
    
    // Find all nodes that depend exclusively on constant parameters
    RooArgSet cacheableNodes ;
    _pdfClone->findConstantNodes(*_dataClone->get(),cacheableNodes) ;

    // Cache constant nodes with dataset 
    _dataClone->cacheArgs(cacheableNodes,_normSet) ;  
    
    // Put all cached nodes in AClean value caching mode so that their evaluate() is never called
    TIterator* cIter = cacheableNodes.createIterator() ;
    RooAbsArg *cacheArg ;
    while((cacheArg=(RooAbsArg*)cIter->Next())){
      cacheArg->setOperMode(RooAbsArg::AClean) ;
    }
    delete cIter ;  
    
    // Disable reading of observables that are no longer used
    _dataClone->optimizeReadingWithCaching(*_pdfClone, cacheableNodes) ;

  } else {
    
    // Delete the cache
    _dataClone->resetCache() ;
    
    // Reactivate all tree branches
    _dataClone->setArgStatus(*_dataClone->get(),kTRUE) ;
    
    // Reset all nodes to ADirty   
    optimizeCaching() ;
    
  }
}



