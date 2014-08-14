/*****************************************************************************
 * Project: RooFit                                                           *
 * Package: RooFitCore                                                       *
 * @(#)root/roofitcore:$Id: DRecoHistPdfChi2Var.cc,v 1.7 2010/06/07 18:31:59 benitezj Exp $
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
// Class DRecoHistPdfChi2Var implements a simple chi^2 calculation from a binned dataset
// and a PDF. The chi^2 is calculated as 
//
//             / (f_PDF * N_tot/ V_bin) - N_bin \+2
//  Sum[bins] |  ------------------------------ |
//             \         err_bin                /
//
// If no user-defined errors are defined for the dataset, poisson errors
// are used. In extended PDF mode, N_tot is substituted with N_expected.

#include "RooFit.h"

#include "DRecoilToolsUser/NtupleCode/DRecoHistPdfChi2Var.hh"
#include "DRecoilToolsUser/NtupleCode/DRecoAddPdf.hh"
#include "DRecoilToolsUser/NtupleCode/DRecoHistPdf.hh"
#include "DRecoilToolsUser/NtupleCode/DRecoProdPdf.hh"
#include "DRecoilToolsUser/NtupleCode/DRecoTranslationPdf.hh"
#include "RooRealVar.h"


#include "RooDataHist.h"
#include "RooAbsPdf.h"
#include "RooCmdConfig.h"
#include "RooMsgService.h"

#include "Riostream.h"


ClassImp(DRecoHistPdfChi2Var)
;

RooArgSet DRecoHistPdfChi2Var::_emptySet ;

DRecoHistPdfChi2Var::DRecoHistPdfChi2Var(const char *name, const char* title, RooAbsPdf& pdf, RooDataHist& data,
		       const RooCmdArg& arg1,const RooCmdArg& arg2,const RooCmdArg& arg3,
		       const RooCmdArg& arg4,const RooCmdArg& arg5,const RooCmdArg& arg6,
		       const RooCmdArg& arg7,const RooCmdArg& arg8,const RooCmdArg& arg9) :
  RooAbsOptTestStatistic(name,title,pdf,data,
			 *(const RooArgSet*)RooCmdConfig::decodeObjOnTheFly("DRecoHistPdfChi2Var::DRecoHistPdfChi2Var","ProjectedObservables",0,&_emptySet
									    ,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9),
			 RooCmdConfig::decodeStringOnTheFly("DRecoHistPdfChi2Var::DRecoHistPdfChi2Var","RangeWithName",0,"",arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9),
			 RooCmdConfig::decodeStringOnTheFly("DRecoHistPdfChi2Var::DRecoHistPdfChi2Var","AddCoefRange",0,"",arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9),
			 RooCmdConfig::decodeIntOnTheFly("DRecoHistPdfChi2Var::DRecoHistPdfChi2Var","NumCPU",0,1,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9),
			 RooCmdConfig::decodeIntOnTheFly("DRecoHistPdfChi2Var::DRecoHistPdfChi2Var","Verbose",0,1,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9),
			 RooCmdConfig::decodeIntOnTheFly("DRecoHistPdfChi2Var::DRecoHistPdfChi2Var","SplitRange",0,0,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9))             
  //  DRecoHistPdfChi2Var constructor. Optional arguments taken
  //
  //  Extended()   -- Include extended term in calculation
  //  DataError()  -- Choose between Poisson errors and Sum-of-weights errors
  //  NumCPU()     -- Activate parallel processing feature
  //  Range()      -- Fit only selected region
  //  SumCoefRange() -- Set the range in which to interpret the coefficients of RooAddPdf components 
  //  SplitRange() -- Fit range is split by index catory of simultaneous PDF
  //  ConditionalObservables() -- Define projected observables 
  //  Verbose()    -- Verbose output of GOF framework
{
  RooCmdConfig pc("DRecoHistPdfChi2Var::DRecoHistPdfChi2Var") ;
  pc.defineInt("extended","Extended",0,kFALSE) ;
  pc.defineInt("etype","DataError",0,(Int_t)RooDataHist::Poisson) ;  

  pc.process(arg1) ;  pc.process(arg2) ;  pc.process(arg3) ;
  pc.process(arg4) ;  pc.process(arg5) ;  pc.process(arg6) ;
  pc.process(arg7) ;  pc.process(arg8) ;  pc.process(arg9) ;

  _extended = pc.getInt("extended") ;
  _etype = (RooDataHist::ErrorType) pc.getInt("etype") ;
}


DRecoHistPdfChi2Var::DRecoHistPdfChi2Var(const char *name, const char *title, RooAbsPdf& pdf, RooDataHist& data,
		     Bool_t extended, const char* cutRange, const char* addCoefRange,Int_t nCPU, Bool_t verbose, Bool_t splitCutRange) : 
  RooAbsOptTestStatistic(name,title,pdf,data,RooArgSet(),cutRange,addCoefRange,nCPU,verbose,splitCutRange),
   _etype(RooAbsData::Poisson), _extended(extended)
{
  
}


DRecoHistPdfChi2Var::DRecoHistPdfChi2Var(const char *name, const char *title, RooAbsReal& pdf, RooDataHist& data,
		     const RooArgSet& projDeps, Bool_t extended, const char* cutRange, const char* addCoefRange, Int_t nCPU, Bool_t verbose, Bool_t splitCutRange) : 
  RooAbsOptTestStatistic(name,title,pdf,data,projDeps,cutRange,addCoefRange,nCPU,verbose,splitCutRange),
  _etype(RooAbsData::Poisson), _extended(extended)
{
  
}


DRecoHistPdfChi2Var::DRecoHistPdfChi2Var(const DRecoHistPdfChi2Var& other, const char* name) : 
  RooAbsOptTestStatistic(other,name),
  _etype(other._etype),
  _extended(other._extended)
{
}


DRecoHistPdfChi2Var::~DRecoHistPdfChi2Var()
{
}


Double_t DRecoHistPdfChi2Var::evaluatePartition(Int_t firstEvent, Int_t lastEvent, Int_t stepSize) const 
{
  stepSize=1;

  Int_t i ;
  Double_t result(0) ;

  // Determine total number of data events to be used for PDF normalization
  Double_t nDataTotal= _dataClone->sumEntries() ;


  // Loop over bins of dataset
  RooDataHist* data = (RooDataHist*) _dataClone ;
  for (i=firstEvent ; i<lastEvent ; i++) {
    
    // get the data values for this event
    data->get(i);
    Double_t nData = data->weight() ;
    Double_t nPdf = _funcClone->getVal(_normSet) * nDataTotal * data->binVolume() ;

    // Skip cases where pdf=0 and there is no data
    //if (eInt==0. && nData==0. && nPdf==0) continue ;
    //if (eInt==0. && nData==0.) continue ;
    if (nData==0. && nPdf==0.) continue ;//denominator will be 0

    Double_t eExt = nPdf-nData ;

    Double_t eIntLo,eIntHi ;
    data->weightError(eIntLo,eIntHi,_etype) ;
    Double_t eInt = (eExt>0) ? eIntHi : eIntLo ;
    

    //for histogram pdfs account for statistical errors
    Double_t nPdfErr=0.;
    if(!strcmp(_funcClone->ClassName(),"DRecoAddPdf")) nPdfErr=nPdf*(((DRecoAddPdf*)_funcClone)->getHistError());
    if(!strcmp(_funcClone->ClassName(),"DRecoHistPdf")) nPdfErr=nPdf*(((DRecoHistPdf*)_funcClone)->getHistError());
    if(!strcmp(_funcClone->ClassName(),"DRecoProdPdf")) nPdfErr=nPdf*(((DRecoProdPdf*)_funcClone)->getHistError());
    if(!strcmp(_funcClone->ClassName(),"DRecoTranslationPdf")) nPdfErr=nPdf*(((DRecoTranslationPdf*)_funcClone)->getHistError());


    // Return 0 if eInt=0, special handling in MINUIT will follow
    //if (eInt==0.) {
    if (eInt*eInt+nPdfErr*nPdfErr==0.){      
      coutE(Eval) << "DRecoHistPdfChi2Var::DRecoHistPdfChi2Var(" << GetName() << ") INFINITY ERROR: bin " << i 
		  << " has zero error" << endl ;
      return 0 ;
    }

    
//     cout<<i<<" nData="<<nData<<" DataErr="<<eInt
// 	<<" nPdf="<<nPdf<<" nPdfErr="<<nPdfErr
// 	<<" pdf="<<_funcClone->GetName()<<"="<<_funcClone->getVal(_normSet)
// 	<<endl;
    result += eExt*eExt/(eInt*eInt+nPdfErr*nPdfErr) ;
  }
  

  return result ;
}



