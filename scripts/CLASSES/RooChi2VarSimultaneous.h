/*****************************************************************************
 * Project: RooFit                                                           *
 *****************************************************************************/

#ifndef RooChi2VarSimultaneous_HH
#define RooChi2VarSimultaneous_HH

#include "RooAbsOptTestStatistic.h"
#include "RooCmdArg.h"
#include "RooDataHist.h"
#include <TH1.h>

class RooChi2VarSimultaneous : public RooAbsOptTestStatistic {
public:

  // Constructors, assignment etc
  RooChi2VarSimultaneous(const char *name, const char* title, RooAbsPdf& pdf, RooDataHist& data,
			 TH1** Hists,RooAbsPdf** Pdfs,RooRealVar** Vars,Int_t NTot,
	     const RooCmdArg& arg1                , const RooCmdArg& arg2=RooCmdArg::none(),const RooCmdArg& arg3=RooCmdArg::none(),
	     const RooCmdArg& arg4=RooCmdArg::none(), const RooCmdArg& arg5=RooCmdArg::none(),const RooCmdArg& arg6=RooCmdArg::none(),
	     const RooCmdArg& arg7=RooCmdArg::none(), const RooCmdArg& arg8=RooCmdArg::none(),const RooCmdArg& arg9=RooCmdArg::none());

  RooChi2VarSimultaneous(const char *name, const char *title, RooAbsPdf& pdf, RooDataHist& data,
	    Bool_t extended=kFALSE, const char* rangeName=0, const char* addCoefRangeName=0, Int_t nCPU=1, Bool_t verbose=kTRUE, Bool_t splitCutRange=kTRUE) ;

  RooChi2VarSimultaneous(const char *name, const char *title, RooAbsPdf& pdf, RooDataHist& data,
	    const RooArgSet& projDeps, Bool_t extended=kFALSE, const char* rangeName=0, const char* addCoefRangeName=0, Int_t nCPU=1, Bool_t verbose=kTRUE, Bool_t splitCutRange=kTRUE) ;

  RooChi2VarSimultaneous(const RooChi2VarSimultaneous& other, const char* name=0); 

  virtual TObject* clone(const char* newname) const { return new RooChi2VarSimultaneous(*this,newname); }

  virtual RooAbsTestStatistic* create(const char *name, const char *title, RooAbsPdf& pdf, RooAbsData& data,
				      const RooArgSet& projDeps, const char* rangeName=0, const char* addCoefRangeName=0, Int_t nCPU=1, Bool_t verbose=kTRUE, Bool_t splitCutRange=kTRUE) {
    
    RooChi2VarSimultaneous* newchi2=new RooChi2VarSimultaneous(name,title,pdf,(RooDataHist&)data,projDeps,
							       _extended,rangeName,addCoefRangeName,nCPU, verbose, splitCutRange);
    newchi2->SetEType(_etype);
    newchi2->SetArrays(_Hists,_Pdfs,_Vars,_NTot);
    return newchi2 ;
  }
  
  virtual ~RooChi2VarSimultaneous();

  virtual Double_t defaultErrorLevel() const { return 1.0 ; }
  void SetEType(RooDataHist::ErrorType etype){_etype=etype;}

  void SetArrays(TH1** Hists,RooAbsPdf** Pdfs,RooRealVar** Vars,Int_t NTot){
    _Hists=Hists;_Pdfs=Pdfs;_Vars=Vars;_NTot=NTot;
  }
  
 protected:

  static RooArgSet _emptySet ; // Supports named argument constructor

  RooDataHist::ErrorType _etype ;
  Bool_t _extended ;
  virtual Double_t evaluatePartition(Int_t firstEvent, Int_t lastEvent) const ;
  
  TH1** _Hists;
  RooAbsPdf** _Pdfs;
  RooRealVar** _Vars;
  Int_t _NTot;


  ClassDef(RooChi2VarSimultaneous,1) // Abstract real-valued variable
};


#endif
