/*****************************************************************************
 * Project: RooFit                                                           *
 * Package: RooFitCore                                                       *
 *    File: $Id: RooNLLVar.h,v 1.10 2007/07/21 21:32:52 wouter Exp $
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
#ifndef ROO_NLL_VAR1D
#define ROO_NLL_VAR1D

#include "RooAbsOptTestStatistic.h"
#include "RooCmdArg.h"
#include "RooAbsPdf.h"



class RooNLLVar1D : public RooAbsOptTestStatistic {
public:

  // Constructors, assignment etc
  RooNLLVar1D() {} ;  
  RooNLLVar1D(const char *name, const char* title, RooAbsPdf& pdf, RooAbsData& data,
	    const RooCmdArg& arg1=RooCmdArg::none(), const RooCmdArg& arg2=RooCmdArg::none(),const RooCmdArg& arg3=RooCmdArg::none(),
	    const RooCmdArg& arg4=RooCmdArg::none(), const RooCmdArg& arg5=RooCmdArg::none(),const RooCmdArg& arg6=RooCmdArg::none(),
	    const RooCmdArg& arg7=RooCmdArg::none(), const RooCmdArg& arg8=RooCmdArg::none(),const RooCmdArg& arg9=RooCmdArg::none()) ;

  RooNLLVar1D(const char *name, const char *title, RooAbsPdf& pdf, RooAbsData& data,TH1* HEfficiency,
	    Bool_t extended=kFALSE, const char* rangeName=0, const char* addCoefRangeName=0, 
	    Int_t nCPU=1, Bool_t verbose=kTRUE, Bool_t splitRange=kFALSE) ;

  RooNLLVar1D(const char *name, const char *title, RooAbsPdf& pdf, RooAbsData& data,
	    const RooArgSet& projDeps, Bool_t extended=kFALSE, const char* rangeName=0, 
	    const char* addCoefRangeName=0, Int_t nCPU=1, Bool_t verbose=kTRUE, Bool_t splitRange=kFALSE) ;

  RooNLLVar1D(const RooNLLVar1D& other, const char* name=0);

  virtual TObject* clone(const char* newname) const { return new RooNLLVar1D(*this,newname); }
  
  virtual RooAbsTestStatistic* create(const char *name, const char *title, RooAbsReal& pdf, RooAbsData& data,
				      const RooArgSet& projDeps, const char* rangeName, const char* addCoefRangeName=0,
				      Int_t nCPU=1, Bool_t interleave=kFALSE, Bool_t verbose=kTRUE, Bool_t splitRange=kFALSE) {
    return new RooNLLVar1D(name,title,(RooAbsPdf&)pdf,data,projDeps,_extended,rangeName,addCoefRangeName,
			   nCPU,verbose,splitRange) ;
  }
  
  virtual ~RooNLLVar1D();

  virtual Double_t defaultErrorLevel() const { return 0.5 ; }

  //void InitEff();//call for Efficiency
protected:

  TH1* _HEfficiency;//pointer to efficiency

  static RooArgSet _emptySet ; // Supports named argument constructor

  Bool_t _extended ;
  virtual Double_t evaluatePartition(Int_t firstEvent, Int_t lastEvent, Int_t stepSize) const ;

  //Bool_t EffInit; 
  
  ClassDef(RooNLLVar1D,1) // Abstract real-valued variable
};

#endif
