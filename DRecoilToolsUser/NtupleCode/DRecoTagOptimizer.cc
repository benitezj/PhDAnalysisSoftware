/*****************************************************************************
 * Project: RooFit                                                           *
 * Package: RooFitCore                                                       *
 * @(#)root/roofitcore:$Id: DRecoTagOptimizer.cc,v 1.7 2010/06/07 18:31:59 benitezj Exp $
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
// Class RooChi2Var implements a simple chi^2 calculation from a binned dataset
// and a PDF. The chi^2 is calculated as 
//
//             / (f_PDF * N_tot/ V_bin) - N_bin \+2
//  Sum[bins] |  ------------------------------ |
//             \         err_bin                /
//
// If no user-defined errors are defined for the dataset, poisson errors
// are used. In extended PDF mode, N_tot is substituted with N_expected.

#include "RooFit.h"

//#include "DRecoTagOptimizer.h"
#include "DRecoilToolsUser/NtupleCode/DRecoTagOptimizer.hh"
#include "RooRealVar.h"
#include <TTree.h>
#include <TCut.h>
#include <TAxis.h>
#include "RooDataHist.h"
#include "RooAbsPdf.h"
#include "RooCmdConfig.h"
#include "RooMsgService.h"

#include "Riostream.h"


ClassImp(DRecoTagOptimizer)
;

RooArgSet DRecoTagOptimizer::_emptySet ;

DRecoTagOptimizer::DRecoTagOptimizer(const char *name, const char* title, RooAbsPdf& pdf, RooDataHist& data,
				     TTree* TreeSig,TTree* TreeBkg,RooArgList* Cuts,
		       const RooCmdArg& arg1,const RooCmdArg& arg2,const RooCmdArg& arg3,
		       const RooCmdArg& arg4,const RooCmdArg& arg5,const RooCmdArg& arg6,
		       const RooCmdArg& arg7,const RooCmdArg& arg8,const RooCmdArg& arg9) :
  RooAbsOptTestStatistic(name,title,pdf,data,
			 *(const RooArgSet*)RooCmdConfig::decodeObjOnTheFly("DRecoTagOptimizer::DRecoTagOptimizer","ProjectedObservables",0,&_emptySet
									    ,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9),
			 RooCmdConfig::decodeStringOnTheFly("DRecoTagOptimizer::DRecoTagOptimizer","RangeWithName",0,"",arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9),
			 RooCmdConfig::decodeStringOnTheFly("DRecoTagOptimizer::DRecoTagOptimizer","AddCoefRange",0,"",arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9),
			 RooCmdConfig::decodeIntOnTheFly("DRecoTagOptimizer::DRecoTagOptimizer","NumCPU",0,1,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9),
			 RooCmdConfig::decodeIntOnTheFly("DRecoTagOptimizer::DRecoTagOptimizer","Verbose",0,1,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9),
			 RooCmdConfig::decodeIntOnTheFly("DRecoTagOptimizer::DRecoTagOptimizer","SplitRange",0,0,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9)) ,
  sigtype(1),
  treesig(TreeSig),
  treebkg(TreeBkg),
  cutlist(Cuts),
  sig0(0.),
  minSideBandLimit(.06)            
  //  DRecoTagOptimizer constructor. Optional arguments taken
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
  RooCmdConfig pc("DRecoTagOptimizer::DRecoTagOptimizer") ;
  pc.defineInt("extended","Extended",0,kFALSE) ;
  pc.defineInt("etype","DataError",0,(Int_t)RooDataHist::Poisson) ;  

  pc.process(arg1) ;  pc.process(arg2) ;  pc.process(arg3) ;
  pc.process(arg4) ;  pc.process(arg5) ;  pc.process(arg6) ;
  pc.process(arg7) ;  pc.process(arg8) ;  pc.process(arg9) ;

  _extended = pc.getInt("extended") ;
  _etype = (RooDataHist::ErrorType) pc.getInt("etype") ;

  sig0=0.;
  sig0=evaluatePartition(1,1,1);

}


DRecoTagOptimizer::DRecoTagOptimizer(const char *name, const char *title, RooAbsPdf& pdf, RooDataHist& data,
				     TTree* TreeSig,TTree* TreeBkg,RooArgList* Cuts,
		     Bool_t extended, const char* cutRange, const char* addCoefRange,Int_t nCPU, Bool_t verbose, Bool_t splitCutRange) : 
  RooAbsOptTestStatistic(name,title,pdf,data,RooArgSet(),cutRange,addCoefRange,nCPU,verbose,splitCutRange),
   _etype(RooAbsData::Poisson), _extended(extended),
  sigtype(1),
  treesig(TreeSig),
  treebkg(TreeBkg),
  cutlist(Cuts),
  sig0(0.),
  minSideBandLimit(.06)    
{
  
}


DRecoTagOptimizer::DRecoTagOptimizer(const char *name, const char *title, RooAbsReal& pdf, RooDataHist& data,
				     TTree* TreeSig,TTree* TreeBkg,RooArgList* Cuts,
				     const RooArgSet& projDeps, Bool_t extended, const char* cutRange, const char* addCoefRange, 
				     Int_t nCPU, Bool_t verbose, Bool_t splitCutRange) : 
  RooAbsOptTestStatistic(name,title,pdf,data,projDeps,cutRange,addCoefRange,nCPU,verbose,splitCutRange),
  _etype(RooAbsData::Poisson), _extended(extended),
  sigtype(1),
  treesig(TreeSig),
  treebkg(TreeBkg),
  cutlist(Cuts),
  sig0(0.),
  minSideBandLimit(.06)    
{
  
}


DRecoTagOptimizer::DRecoTagOptimizer(const DRecoTagOptimizer& other, const char* name) : 
  RooAbsOptTestStatistic(other,name),
  _etype(other._etype),
  _extended(other._extended),
  sigtype(other.sigtype),
  treesig(other.treesig),
  treebkg(other.treebkg),
  cutlist(other.cutlist),
  sig0(other.sig0),
  minSideBandLimit(other.minSideBandLimit)
{
}


DRecoTagOptimizer::~DRecoTagOptimizer()
{
}


Double_t DRecoTagOptimizer::evaluatePartition(Int_t firstEvent, Int_t lastEvent, Int_t stepSize) const 
{

  firstEvent=lastEvent=stepSize;//remove compile warnings
  if(!treesig||!treebkg||!cutlist){cout<<"DRecoTagOptimizer: !treesig||!treebkg||!cutlist"<<endl;return 0.;}
  
  
  TCut sigcut;
  TCut bkgcut;
  char textl[100];
  TIterator*iter=cutlist->createIterator();
  while(RooRealVar* var=(RooRealVar*)iter->Next()){
    sprintf(textl,var->GetTitle(),var->getVal());
    sigcut+=TCut(textl);
    
    if(TString(var->GetName())==TString("mwin")){//for bkg widen the cut
      sprintf(textl,var->GetTitle(),var->getVal()+minSideBandLimit);
    }
    bkgcut+=TCut(textl);
  }
  
  Float_t SplusB=(float)treesig->GetEntries(sigcut)+1.00001;//add to avoid trouble when analyzing low stats -->Still doesnt work
  Float_t B=treebkg->GetEntries(bkgcut)+1.;
  if(SplusB<=0){cout<<"DRecoTagOptimizer: SplusB is <=0"<<endl;return -1.;}
  if(B<=0){cout<<"DRecoTagOptimizer: B is <=0"<<endl;return -1.;}
  
  Double_t sig=(double)(fabs((float)(SplusB-B))/sqrt((float)(SplusB)));
  if(sigtype==1)sig=(double)(fabs((float)(SplusB-B))/sqrt((float)(SplusB)));//maximize significance
  if(sigtype==2)sig=(double)(fabs((float)(SplusB-B))/B);//maximize S/B
  if(sigtype==3)sig=(double)(fabs((float)(SplusB-B))/SplusB);//maximize purity
  
  //cout<<sigcut<<" "<<bkgcut<<" "<<sig<<endl;


  if(sig0==0.)return sig;
  return 1000.*sig0/sig;//normalize to initial value
}


TGraph* DRecoTagOptimizer::ScanVariable(RooRealVar* Var){
  
  if(!Var){cout<<"DRecoTagOptimizer::ScanVariable bad arguments"<<endl;return 0;}
  
  Float_t initval=Var->getVal();
  
  TGraph G;
  Int_t Npoints=(int)((Var->getMax()-Var->getMin())/Var->getError());
  for(Int_t i=0;i<=Npoints;i++){
    Var->setVal(Var->getMin()+i*Var->getError());
    G.SetPoint(i,Var->getVal(),GetSignificance());
  }


  if(sigtype==1)G.GetYaxis()->SetTitle("S/sqrt(S+B)");
  if(sigtype==2)G.GetYaxis()->SetTitle("S/B");
  if(sigtype==3)G.GetYaxis()->SetTitle("S/(S+B)");
  G.GetXaxis()->SetTitle(Var->getUnit());
  
  Var->setVal(initval);
  
  return (TGraph*)G.Clone();
}




