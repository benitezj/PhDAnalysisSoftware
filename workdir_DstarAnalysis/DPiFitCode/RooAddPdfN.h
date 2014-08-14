/*****************************************************************************
 * Project: RooFit                                                           *
 * Package: RooFitCore                                                       *
 *    File: $Id: RooAddPdf.h,v 1.46 2007/07/12 20:30:28 wouter Exp $
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
#ifndef ROO_ADD_PDFN
#define ROO_ADD_PDFN

#include "RooAbsPdf.h"
#include "RooListProxy.h"
#include "RooAICRegistry.h"
#include "RooNormSetCache.h"
#include "RooNameSet.h"
#include "RooCacheManager.h"
#include "RooObjCacheManager.h"

class RooAddPdfN : public RooAbsPdf {
public:

  RooAddPdfN() ;
  RooAddPdfN(const char *name, const char *title=0);
  RooAddPdfN(const char *name, const char *title,
	    RooAbsPdf& pdf1, RooAbsPdf& pdf2, RooAbsReal& coef1) ;
  RooAddPdfN(const char *name, const char *title, const RooArgList& pdfList) ;
  RooAddPdfN(const char *name, const char *title, const RooArgList& pdfList, const RooArgList& coefList) ;
  RooAddPdfN(const RooAddPdfN& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new RooAddPdfN(*this,newname) ; }
  virtual ~RooAddPdfN() ;

  Double_t getVal(const RooArgSet* set=0) const ;
  Double_t evaluate() const ;
  virtual Bool_t checkObservables(const RooArgSet* nset) const ;	

  virtual Bool_t forceAnalyticalInt(const RooAbsArg& /*dep*/) const { return kTRUE ; }
  Int_t getAnalyticalIntegralWN(RooArgSet& allVars, RooArgSet& numVars, const RooArgSet* normSet, const char* rangeName=0) const ;
  Double_t analyticalIntegralWN(Int_t code, const RooArgSet* normSet, const char* rangeName=0) const ;
  virtual Bool_t selfNormalized() const { return kTRUE ; }

  virtual ExtendMode extendMode() const { return (_haveLastCoef || _allExtendable) ? MustBeExtended : CanNotBeExtended; }
  virtual Double_t expectedEvents(const RooArgSet* nset) const ;
  virtual Double_t expectedEvents(const RooArgSet& nset) const { return expectedEvents(&nset) ; }

  const RooArgList& pdfList() const { return _pdfList ; }
  const RooArgList& coefList() const { return _coefList ; }

  void fixCoefNormalization(const RooArgSet& refCoefNorm) ;
  void fixCoefRange(const char* rangeName) ;
  virtual void resetErrorCounters(Int_t resetValue=10) ;

protected:

  virtual void selectNormalization(const RooArgSet* depSet=0, Bool_t force=kFALSE) ;
  virtual void selectNormalizationRange(const char* rangeName=0, Bool_t force=kFALSE) ;

  mutable RooSetProxy _refCoefNorm ;   //!
  mutable TNamed* _refCoefRangeName ;  //!

  Bool_t _projectCoefs ;
  mutable Double_t* _coefCache ; //!


  class CacheElem : public RooAbsCacheElement {
  public:
    virtual ~CacheElem() {} ;

    RooArgList _suppNormList ; // Supplemental normalization list

    RooArgList _projList ; // Projection integrals to be multiplied with coefficients
    RooArgList _suppProjList ; // Projection integrals to be multiplied with coefficients for supplemental normalization terms
    RooArgList _refRangeProjList ; // Range integrals to be multiplied with coefficients (reference range)
    RooArgList _rangeProjList ; // Range integrals to be multiplied with coefficients (target range)

    virtual RooArgList containedArgs(Action) ;

  } ;
  mutable RooObjCacheManager _projCacheMgr ;  //
  CacheElem* getProjCache(const RooArgSet* nset, const RooArgSet* iset=0, const char* rangeName=0) const ;
  void updateCoefficients(CacheElem& cache, const RooArgSet* nset) const ;

  
  friend class RooAddGenContext ;
  //virtual RooAbsGenContext* genContext(const RooArgSet &vars, const RooDataSet *prototype=0, const RooArgSet* auxProto=0, Bool_t verbose= kFALSE) const ;


  mutable RooAICRegistry _codeReg ;  //! Registry of component analytical integration codes

  RooListProxy _pdfList ;   //  List of component PDFs
  RooListProxy _coefList ;  //  List of coefficients
  mutable RooArgList* _snormList ;  //!  List of supplemental normalization factors
  TIterator* _pdfIter ;     //! Iterator over PDF list
  TIterator* _coefIter ;    //! Iterator over coefficient list
  
  Bool_t _haveLastCoef ;    //  Flag indicating if last PDFs coefficient was supplied in the ctor
  Bool_t _allExtendable ;   //  Flag indicating if all PDF components are extendable

  mutable Int_t _coefErrCount ; //! Coefficient error counter

private:

  ClassDef(RooAddPdfN,1) // PDF representing a sum of PDFs
};

#endif
