/*****************************************************************************
 * Project: RooFit                                                           *
 * Package: RooFitCore                                                       *
 *    File: $Id: RooAbsPdf.h,v 1.90 2007/07/21 21:32:52 wouter Exp $
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
#ifndef ROO_ABS_PDFN
#define ROO_ABS_PDFN

#include "RooAbsReal.h"
#include "RooRealIntegral.h"
#include "RooNameSet.h"
#include "RooObjCacheManager.h"
#include "RooCmdArg.h"

class RooDataSet;
class RooArgSet ;
class RooRealProxy ;
class RooAbsGenContext ;
class RooFitResult ;
class RooExtendPdf ;
class RooCategory ;
class TPaveText;
class TH1F;
class TH2F;
class TList ;
class RooLinkedList ;

class RooAbsPdfN : public RooAbsReal {
public:

  // Constructors, assignment etc
  RooAbsPdfN() ;
  RooAbsPdfN(const char *name, const char *title=0) ;
  RooAbsPdfN(const char *name, const char *title, Double_t minVal, Double_t maxVal) ;
  // RooAbsPdfN(const RooAbsPdfN& other, const char* name=0);
  virtual ~RooAbsPdfN();

  // Toy MC generation
  RooDataSet *generate(const RooArgSet &whatVars, Int_t nEvents, const RooCmdArg& arg1,
                       const RooCmdArg& arg2=RooCmdArg::none(), const RooCmdArg& arg3=RooCmdArg::none(),
                       const RooCmdArg& arg4=RooCmdArg::none(), const RooCmdArg& arg5=RooCmdArg::none()) ;
  RooDataSet *generate(const RooArgSet &whatVars,  
                       const RooCmdArg& arg1=RooCmdArg::none(),const RooCmdArg& arg2=RooCmdArg::none(),
                       const RooCmdArg& arg3=RooCmdArg::none(),const RooCmdArg& arg4=RooCmdArg::none(),
                       const RooCmdArg& arg5=RooCmdArg::none(),const RooCmdArg& arg6=RooCmdArg::none()) ;
  RooDataSet *generate(const RooArgSet &whatVars, Int_t nEvents = 0, Bool_t verbose=kFALSE) const;
  RooDataSet *generate(const RooArgSet &whatVars, const RooDataSet &prototype, Int_t nEvents= 0,
		       Bool_t verbose=kFALSE, Bool_t randProtoOrder=kFALSE, Bool_t resampleProto=kFALSE) const;

  virtual RooPlot* plotOn(RooPlot* frame, 
			  const RooCmdArg& arg1=RooCmdArg::none(), const RooCmdArg& arg2=RooCmdArg::none(),
			  const RooCmdArg& arg3=RooCmdArg::none(), const RooCmdArg& arg4=RooCmdArg::none(),
			  const RooCmdArg& arg5=RooCmdArg::none(), const RooCmdArg& arg6=RooCmdArg::none(),
			  const RooCmdArg& arg7=RooCmdArg::none(), const RooCmdArg& arg8=RooCmdArg::none(),
			  const RooCmdArg& arg9=RooCmdArg::none(), const RooCmdArg& arg10=RooCmdArg::none()
              ) const {
    return RooAbsReal::plotOn(frame,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10) ;
  }

  // Backward compatibility functions
  virtual RooPlot *plotCompOn(RooPlot *frame, const char* compNameList, Option_t* drawOptions="L",
			      Double_t scaleFactor= 1.0, ScaleType stype=Relative, 
			      const RooAbsData* projData=0, const RooArgSet* projSet=0) const ;

  virtual RooPlot *plotCompOn(RooPlot *frame, const RooArgSet& compSet, Option_t* drawOptions="L",
			      Double_t scaleFactor= 1.0, ScaleType stype=Relative, 
			      const RooAbsData* projData=0, const RooArgSet* projSet=0) const ;

  virtual RooPlot *plotCompSliceOn(RooPlot *frame, const char* compNameList, const RooArgSet& sliceSet,
				   Option_t* drawOptions="L", Double_t scaleFactor= 1.0, ScaleType stype=Relative, 
				   const RooAbsData* projData=0) const ;

  virtual RooPlot *plotCompSliceOn(RooPlot *frame, const RooArgSet& compSet, const RooArgSet& sliceSet,
				   Option_t* drawOptions="L", Double_t scaleFactor= 1.0, ScaleType stype=Relative, 
				   const RooAbsData* projData=0) const ;

// End backward compatibility functions


  virtual RooPlot* paramOn(RooPlot* frame, 
                           const RooCmdArg& arg1=RooCmdArg::none(), const RooCmdArg& arg2=RooCmdArg::none(), 
                           const RooCmdArg& arg3=RooCmdArg::none(), const RooCmdArg& arg4=RooCmdArg::none(), 
                           const RooCmdArg& arg5=RooCmdArg::none(), const RooCmdArg& arg6=RooCmdArg::none(), 
                           const RooCmdArg& arg7=RooCmdArg::none(), const RooCmdArg& arg8=RooCmdArg::none()) ;

  virtual RooPlot* paramOn(RooPlot* frame, const RooAbsData* data, const char *label= "", Int_t sigDigits = 2,
			   Option_t *options = "NELU", Double_t xmin=0.65,
			   Double_t xmax= 0.99,Double_t ymax=0.95) ;

  // Built-in generator support
  virtual Int_t getGenerator(const RooArgSet& directVars, RooArgSet &generateVars, Bool_t staticInitOK=kTRUE) const;
  virtual void initGenerator(Int_t code) ;
  virtual void generateEvent(Int_t code);  

  virtual Bool_t isDirectGenSafe(const RooAbsArg& arg) const ; 


  /*
  inline RooPlot *plotNLLOn(RooPlot* frame, RooDataSet* data, Option_t* drawOptions="L", 
                            Double_t prec=1e-2, Bool_t fixMinToZero=kTRUE) {
    return plotNLLOn(frame,data,kFALSE,RooArgSet(),drawOptions,prec,fixMinToZero) ;
  }
  virtual RooPlot *plotNLLOn(RooPlot* frame, RooDataSet* data, Bool_t extended, Option_t* drawOptions="L", 
                             Double_t prec=1e-2, Bool_t fixMinToZero=kTRUE) {
    return plotNLLOn(frame,data,extended,RooArgSet(),drawOptions,prec,fixMinToZero) ;
  }
  virtual RooPlot *plotNLLOn(RooPlot* frame, RooDataSet* data, Bool_t extended, const RooArgSet& projDeps,
                             Option_t* drawOptions="L", Double_t prec=1e-2, Bool_t fixMinToZero=kTRUE) ;
  

  // Interactions with a dataset  
  virtual RooFitResult* fitTo(RooAbsData& data, RooCmdArg arg1,  RooCmdArg arg2=RooCmdArg::none(),  
                              RooCmdArg arg3=RooCmdArg::none(),  RooCmdArg arg4=RooCmdArg::none(), RooCmdArg arg5=RooCmdArg::none(),  
                              RooCmdArg arg6=RooCmdArg::none(),  RooCmdArg arg7=RooCmdArg::none(), RooCmdArg arg8=RooCmdArg::none()) ;
  virtual RooFitResult* fitTo(RooAbsData& data, const RooLinkedList& cmdList) ;

  virtual RooFitResult* fitTo(RooAbsData& data, const RooArgSet& projDeps, 
			      Option_t *fitOpt = "", Option_t *optOpt = "c", const char* fitRange=0) ;
  virtual RooFitResult* fitTo(RooAbsData& data, Option_t *fitOpt = "", Option_t *optOpt = "c", const char* fitRange=0) ;

  */  

  // Project p.d.f into lower dimensional p.d.f
  virtual RooAbsPdfN* createProjection(const RooArgSet& iset) ;  


  // Function evaluation support
  virtual Bool_t traceEvalHook(Double_t value) const ;  
  virtual Double_t getVal(const RooArgSet* set=0) const ;
  virtual Double_t getLogVal(const RooArgSet* set=0) const ;

  Double_t getNorm(const RooArgSet& nset) const { return getNorm(&nset) ; }
  virtual Double_t getNorm(const RooArgSet* set=0) const ;

  virtual void resetErrorCounters(Int_t resetValue=10) ;
  void setTraceCounter(Int_t value, Bool_t allNodes=kFALSE) ;
  Bool_t traceEvalPdf(Double_t value) const ;

  Double_t analyticalIntegralWN(Int_t code, const RooArgSet* normSet, const char* rangeName=0) const ;

  virtual Bool_t selfNormalized() const { return kFALSE ; }

  // Support for extended maximum likelihood, switched off by default
  enum ExtendMode { CanNotBeExtended, CanBeExtended, MustBeExtended } ;
  virtual ExtendMode extendMode() const { return CanNotBeExtended ; } 
  inline Bool_t canBeExtended() const { return (extendMode() != CanNotBeExtended) ; }
  inline Bool_t mustBeExtended() const { return (extendMode() == MustBeExtended) ; }
  virtual Double_t expectedEvents(const RooArgSet* nset) const ; 
  virtual Double_t expectedEvents(const RooArgSet& nset) const { return expectedEvents(&nset) ; }

  // Printing interface (human readable)
  virtual void printToStream(ostream& stream, PrintOption opt=Standard, TString indent= "") const ;

  static void verboseEval(Int_t stat) ;
  static int verboseEval() ;

  Bool_t isSelectedComp() const ;

  virtual void fixAddCoefNormalization(const RooArgSet& addNormSet=RooArgSet(),Bool_t force=kTRUE) ;
  virtual void fixAddCoefRange(const char* rangeName=0,Bool_t force=kTRUE) ;

  virtual Double_t extendedTerm(UInt_t observedEvents, const RooArgSet* nset=0) const ;

  static void clearEvalError() ;
  static Bool_t evalError() ;
  
protected:

public:
  virtual const RooAbsReal* getNormObj(const RooArgSet* set, const RooArgSet* iset, const TNamed* rangeName=0) const ;
protected:

  // Implementation version
  virtual RooPlot* paramOn(RooPlot* frame, const RooArgSet& params, Bool_t showConstants=kFALSE,
                           const char *label= "", Int_t sigDigits = 2, Option_t *options = "NELU", Double_t xmin=0.65,
			   Double_t xmax= 0.99,Double_t ymax=0.95, const RooCmdArg* formatCmd=0) ;


  virtual RooPlot* plotOn(RooPlot* frame, RooLinkedList& cmdList) const ;
  void plotOnCompSelect(RooArgSet* selNodes) const ;

  virtual RooPlot *plotOn(RooPlot *frame, PlotOpt o) const;  

  friend class RooEffGenContext ;
  friend class RooAddGenContext ;
  friend class RooProdGenContext ;
  friend class RooSimGenContext ;
  friend class RooConvGenContext ;
  friend class RooSimultaneous ;
  friend class RooAddGenContextOrig ;
  friend class RooMCStudy ;

  Int_t* randomizeProtoOrder(Int_t nProto,Int_t nGen,Bool_t resample=kFALSE) const ;
  virtual RooAbsGenContext* genContext(const RooArgSet &vars, const RooDataSet *prototype=0, 
	                               const RooArgSet* auxProto=0, Bool_t verbose= kFALSE) const ;


  friend class RooExtendPdf ;
  // This also forces the definition of a copy ctor in derived classes 
  RooAbsPdfN(const RooAbsPdfN& other, const char* name = 0);

  friend class RooRealIntegral ;
  static Int_t _verboseEval ;

  virtual Bool_t syncNormalization(const RooArgSet* dset, Bool_t adjustProxies=kTRUE) const ;

  virtual RooPlot *plotCompOnEngine(RooPlot *frame, RooArgSet* selNodes, Option_t* drawOptions="L",
				    Double_t scaleFactor= 1.0, ScaleType stype=Relative, 
				    const RooAbsData* projData=0, const RooArgSet* projSet=0) const ;


  friend class RooAbsAnaConvPdf ;
  mutable Double_t _rawValue ;
  mutable RooAbsReal* _norm   ;      //! Normalization integral (owned by _normMgr)
  mutable RooArgSet* _normSet ;      //! Normalization set with for above integral

  class CacheElem : public RooAbsCacheElement {
  public:
    CacheElem(RooAbsReal& norm) : _norm(&norm) {} ;
    void operModeHook(RooAbsArg::OperMode) {} ;
    virtual ~CacheElem() ; 
    virtual RooArgList containedArgs(Action) { return RooArgList(*_norm) ; }
    RooAbsReal* _norm ;
  } ;
  mutable RooObjCacheManager _normMgr ; // The cache manager
  
  virtual Bool_t redirectServersHook(const RooAbsCollection&, Bool_t, Bool_t, Bool_t) { 
    _norm = 0 ;
    return kFALSE ; 
  } ;

  
  mutable Int_t _errorCount ;        // Number of errors remaining to print
  mutable Int_t _traceCount ;        // Number of traces remaining to print
  mutable Int_t _negCount ;          // Number of negative probablities remaining to print

  friend class RooAddPdf ;
  void selectComp(Bool_t flag) { _selectComp = flag ; }
  static void globalSelectComp(Bool_t flag) ;
  Bool_t _selectComp ;               // Component selection flag for RooAbsPdfN::plotCompOn
  static Bool_t _globalSelectComp ;  // Global activation switch for component selection

  static void raiseEvalError() ;
  
  static Bool_t _evalError ;
  
  ClassDef(RooAbsPdfN,1) // Abstract PDF with normalization support
};


#endif
