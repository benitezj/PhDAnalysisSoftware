/*****************************************************************************
 * Project: RooFit                                                           *
 *****************************************************************************/

#ifndef ROO_CUTOPTIMIZER
#define ROO_CUTOPTIMIZER

#include "RooAbsOptTestStatistic.h"
#include "RooCmdArg.h"
#include "RooDataHist.h"
#include "TTree.h"
#include "TCut.h"
#include "TH1F.h"
#include "MassFitter.h"
#include "TF1.h"
#include "TGraph.h"
#include "RooRealVar.h"

class RooCutOptimizer : public RooAbsOptTestStatistic {
public:

  // Constructors, assignment etc
  RooCutOptimizer(const char *name, const char* title, RooAbsPdf& pdf, RooDataHist& data,TTree* TreeSig,TTree* TreeBkg,TTree* TreeBkg2,TTree* TreeBkg3,RooArgList* Cuts,
	     const RooCmdArg& arg1                , const RooCmdArg& arg2=RooCmdArg::none(),const RooCmdArg& arg3=RooCmdArg::none(),
	     const RooCmdArg& arg4=RooCmdArg::none(), const RooCmdArg& arg5=RooCmdArg::none(),const RooCmdArg& arg6=RooCmdArg::none(),
	     const RooCmdArg& arg7=RooCmdArg::none(), const RooCmdArg& arg8=RooCmdArg::none(),const RooCmdArg& arg9=RooCmdArg::none()) ;

  RooCutOptimizer(const char *name, const char *title, RooAbsPdf& pdf, RooDataHist& data,TTree* TreeSig,TTree* TreeBkg,TTree* TreeBkg2,TTree* TreeBkg3,RooArgList* Cuts,
	    Bool_t extended=kFALSE, const char* rangeName=0, const char* addCoefRangeName=0, Int_t nCPU=1, Bool_t verbose=kTRUE, Bool_t splitCutRange=kTRUE) ;

  RooCutOptimizer(const char *name, const char *title, RooAbsPdf& pdf, RooDataHist& data,TTree* TreeSig,TTree* TreeBkg,TTree* TreeBkg2,TTree* TreeBkg3,RooArgList* Cuts,
	    const RooArgSet& projDeps, Bool_t extended=kFALSE, const char* rangeName=0, const char* addCoefRangeName=0, Int_t nCPU=1, Bool_t verbose=kTRUE, Bool_t splitCutRange=kTRUE) ;

  RooCutOptimizer(const RooCutOptimizer& other, const char* name=0);
  virtual TObject* clone(const char* newname) const { return new RooCutOptimizer(*this,newname); }

  virtual RooAbsTestStatistic* create(const char *name, const char *title, RooAbsPdf& pdf, RooAbsData& data,
				      const RooArgSet& projDeps, const char* rangeName=0, const char* addCoefRangeName=0, Int_t nCPU=1, Bool_t verbose=kTRUE, Bool_t splitCutRange=kTRUE) {
    return new RooCutOptimizer(name,title,pdf,(RooDataHist&)data,0,0,0,0,0,projDeps,_extended,rangeName,addCoefRangeName,nCPU, verbose, splitCutRange) ;
  }
  
  virtual ~RooCutOptimizer();

  virtual Double_t defaultErrorLevel() const { return 1.0 ; }
  TGraph* ScanVariable(RooRealVar* Var);

protected:

  static RooArgSet _emptySet ; // Supports named argument constructor

  RooDataHist::ErrorType _etype ;
  Bool_t _extended ;

  TTree* treesig;
  TTree* treebkg;
  TTree* treebkg2;
  TTree* treebkg3;
  RooArgList* cutlist;

  virtual Double_t evaluatePartition(Int_t firstEvent, Int_t lastEvent) const ;
  
  ClassDef(RooCutOptimizer,1) // Abstract real-valued variable
};


#endif
