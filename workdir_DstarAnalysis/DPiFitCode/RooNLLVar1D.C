/*****************************************************************************
 * Project: RooFit                                                           *
 * Package: RooFitCore                                                       *
 * @(#)root/roofitcore:$Id: RooNLLVar.cxx 21379 2007-12-14 15:12:29Z wouter $
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
// Class RooNLLVar implements a a -log(likelihood) calculation from a dataset
// and a PDF. The NLL is calculated as 
//
//  Sum[data] -log( pdf(x_data) )
//
// In extended mode, a (Nexpect - Nobserved*log(NExpected) term is added

#include "RooFit.h"
#include "Riostream.h"

#include "RooNLLVar1D.h"
#include "RooAbsData.h"
#include "RooAbsPdf.h"
#include "RooCmdConfig.h"
#include "RooMsgService.h"
#include "RooRealVar.h"
#include "RooDataHist.h"
#include "TH1.h"

ClassImp(RooNLLVar1D)
;

RooArgSet RooNLLVar1D::_emptySet ;

RooNLLVar1D::RooNLLVar1D(const char *name, const char* title, RooAbsPdf& pdf, RooAbsData& data,
		     const RooCmdArg& arg1, const RooCmdArg& arg2,const RooCmdArg& arg3,
		     const RooCmdArg& arg4, const RooCmdArg& arg5,const RooCmdArg& arg6,
		     const RooCmdArg& arg7, const RooCmdArg& arg8,const RooCmdArg& arg9) :
  RooAbsOptTestStatistic(name,title,pdf,data,
			 *(const RooArgSet*)RooCmdConfig::decodeObjOnTheFly("RooNLLVar1D::RooNLLVar1D","ProjectedObservables",0,&_emptySet
									    ,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9),
			 RooCmdConfig::decodeStringOnTheFly("RooNLLVar1D::RooNLLVar1D","RangeWithName",0,"",arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9),
			 RooCmdConfig::decodeStringOnTheFly("RooNLLVar1D::RooNLLVar1D","AddCoefRange",0,"",arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9),
			 RooCmdConfig::decodeIntOnTheFly("RooNLLVar1D::RooNLLVar1D","NumCPU",0,1,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9),
			 RooCmdConfig::decodeIntOnTheFly("RooNLLVar1D::RooNLLVar1D","Verbose",0,1,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9),
			 RooCmdConfig::decodeIntOnTheFly("RooNLLVar1D::RooNLLVar1D","SplitRange",0,0,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9))          
{
  // RooNLLVar1D constructor. Optional arguments taken
  //
  //  Extended()     -- Include extended term in calculation
  //  NumCPU()       -- Activate parallel processing feature
  //  Range()        -- Fit only selected region
  //  SumCoefRange() -- Set the range in which to interpret the coefficients of RooAddPdf components 
  //  SplitRange()   -- Fit range is split by index catory of simultaneous PDF
  //  ConditionalObservables() -- Define conditional observables 
  //  Verbose()      -- Verbose output of GOF framework classes

  RooCmdConfig pc("RooNLLVar1D::RooNLLVar1D") ;
  pc.allowUndefined() ;
  pc.defineInt("extended","Extended",0,kFALSE) ;

  pc.process(arg1) ;  pc.process(arg2) ;  pc.process(arg3) ;
  pc.process(arg4) ;  pc.process(arg5) ;  pc.process(arg6) ;
  pc.process(arg7) ;  pc.process(arg8) ;  pc.process(arg9) ;

  _extended = pc.getInt("extended") ;

 
}



RooNLLVar1D::RooNLLVar1D(const char *name, const char *title, RooAbsPdf& pdf, RooAbsData& data,TH1* HEfficiency,
			 Bool_t extended, const char* rangeName, const char* addCoefRangeName,Int_t nCPU, Bool_t verbose, Bool_t splitRange) : 
  RooAbsOptTestStatistic(name,title,pdf,data,RooArgSet(),rangeName,addCoefRangeName,nCPU,verbose,splitRange), 
  _HEfficiency(HEfficiency),
  _extended(extended)
{
  
}


RooNLLVar1D::RooNLLVar1D(const char *name, const char *title, RooAbsPdf& pdf, RooAbsData& data,
		     const RooArgSet& projDeps, Bool_t extended, const char* rangeName,const char* addCoefRangeName, Int_t nCPU,Bool_t verbose, Bool_t splitRange) : 
  RooAbsOptTestStatistic(name,title,pdf,data,projDeps,rangeName,addCoefRangeName,nCPU,verbose,splitRange),
  _extended(extended) 
{
  
}


RooNLLVar1D::RooNLLVar1D(const RooNLLVar1D& other, const char* name) : 
  RooAbsOptTestStatistic(other,name),
  _extended(other._extended)
{
}


RooNLLVar1D::~RooNLLVar1D()
{
}



Double_t RooNLLVar1D::evaluatePartition(Int_t firstEvent, Int_t lastEvent, Int_t stepSize) const 
{
    
  Double_t result(0) ;
  for (Int_t i=firstEvent ; i<lastEvent ; i+=stepSize) {
    _dataClone->get(i); // get the data values for this event
    Float_t ndata=_dataClone->weight();
    if(ndata<0){
      cout<<"Error: Bin Content is negative"<<endl; 
      return 0;
    }
    

    //----------------
    //minimize -Log(P)
    //----------------
    Double_t fval=((RooAbsPdf*)_funcClone)->getVal(_normSet);
    
    //cout<<i<<" "<<_HEfficiency<<" "<<fval<<" "<<endl;
    if(_HEfficiency){    //get efficiency
      Double_t eff=1;
      if(_HEfficiency->GetYaxis()->GetNbins()==1&&_HEfficiency->GetZaxis()->GetNbins()==1)//1D
	eff=_HEfficiency->GetBinContent(i);
      else if(_HEfficiency->GetZaxis()->GetNbins()==1)//2D//This map between data still needs to be verified!
	eff=_HEfficiency->GetBinContent(1+(i-1)/_HEfficiency->GetYaxis()->GetNbins(),1+(i-1)%_HEfficiency->GetYaxis()->GetNbins());
      else  //3D not supported
	eff=1;

      fval*=eff;
    }
    
    if(fval<1e-300){
      continue;
    }
    
    result+= ndata * log(fval);
  }
 

  //cout<<result<<endl;

  return -result ;

}

