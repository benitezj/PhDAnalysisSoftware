/*****************************************************************************
 * Project: RooFit                                                           *
 *****************************************************************************/

#ifndef RooTagCutOptimizer_HH
#define RooTagCutOptimizer_HH

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

class RooTagCutOptimizer : public RooAbsOptTestStatistic {
public:

  // Constructors, assignment etc
  RooTagCutOptimizer(const char *name, const char* title, RooAbsPdf& pdf, RooDataHist& data,TTree* TreeSig,TTree* TreeBkg,RooArgList* Cuts,const RooCmdArg& arg1, const RooCmdArg& arg2=RooCmdArg::none(),const RooCmdArg& arg3=RooCmdArg::none(),const RooCmdArg& arg4=RooCmdArg::none(), const RooCmdArg& arg5=RooCmdArg::none(),const RooCmdArg& arg6=RooCmdArg::none(),const RooCmdArg& arg7=RooCmdArg::none(), const RooCmdArg& arg8=RooCmdArg::none(),const RooCmdArg& arg9=RooCmdArg::none()) ;
  
  RooTagCutOptimizer(const char *name, const char *title, RooAbsPdf& pdf, RooDataHist& data,TTree* TreeSig,TTree* TreeBkg,RooArgList* Cuts,Bool_t extended=kFALSE, const char* rangeName=0, const char* addCoefRangeName=0, Int_t nCPU=1, Bool_t verbose=kTRUE, Bool_t splitCutRange=kTRUE) ;

  RooTagCutOptimizer(const char *name, const char *title, RooAbsPdf& pdf, RooDataHist& data,TTree* TreeSig,TTree* TreeBkg,RooArgList* Cuts,const RooArgSet& projDeps, Bool_t extended=kFALSE, const char* rangeName=0, const char* addCoefRangeName=0, Int_t nCPU=1, Bool_t verbose=kTRUE, Bool_t splitCutRange=kTRUE) ;

  RooTagCutOptimizer(const RooTagCutOptimizer& other, const char* name=0);
  virtual TObject* clone(const char* newname) const { return new RooTagCutOptimizer(*this,newname); }

  virtual RooAbsTestStatistic* create(const char *name, const char *title, RooAbsPdf& pdf, RooAbsData& data,
				      const RooArgSet& projDeps, const char* rangeName=0, const char* addCoefRangeName=0, Int_t nCPU=1, Bool_t verbose=kTRUE, Bool_t splitCutRange=kTRUE) {
    return new RooTagCutOptimizer(name,title,pdf,(RooDataHist&)data,0,0,0,projDeps,_extended,rangeName,addCoefRangeName,nCPU, verbose, splitCutRange) ;
  }
  
  virtual ~RooTagCutOptimizer();

  virtual Double_t defaultErrorLevel() const { return 1.0 ; }
  void SetSigType(Int_t type){
    if(type==1||type==2||type==3||type==4){
      sigtype=type;
      sig0=0.;
      sig0=evaluatePartition(1,1);//reset normalization
    }
  }
  TGraph* ScanVariable(RooRealVar* Var,Int_t Npoints);
  Double_t GetSignificance(){return 1000.*sig0/evaluatePartition(1,1);}
protected:

  static RooArgSet _emptySet ; // Supports named argument constructor

  RooDataHist::ErrorType _etype ;
  Bool_t _extended ;
  
  Int_t sigtype;
  TTree* treesig;
  TTree* treebkg;
  RooArgList* cutlist;
  Float_t sig0;

  virtual Double_t evaluatePartition(Int_t firstEvent, Int_t lastEvent) const ;
  
  ClassDef(RooTagCutOptimizer,1) // Abstract real-valued variable
};


#endif
