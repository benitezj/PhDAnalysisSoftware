/*****************************************************************************
 * Project: RooFit                                                           *
 *****************************************************************************/

#ifndef ROO_CHI2_VAR_INTEGRAL
#define ROO_CHI2_VAR_INTEGRAL

#include "RooAbsOptTestStatistic.h"
#include "RooCmdArg.h"
#include "RooDataHist.h"

class RooChi2VarIntegral : public RooAbsOptTestStatistic {
public:

  // Constructors, assignment etc
  RooChi2VarIntegral(const char *name, const char* title, RooAbsPdf& pdf, RooDataHist& data,
	     const RooCmdArg& arg1                , const RooCmdArg& arg2=RooCmdArg::none(),const RooCmdArg& arg3=RooCmdArg::none(),
	     const RooCmdArg& arg4=RooCmdArg::none(), const RooCmdArg& arg5=RooCmdArg::none(),const RooCmdArg& arg6=RooCmdArg::none(),
	     const RooCmdArg& arg7=RooCmdArg::none(), const RooCmdArg& arg8=RooCmdArg::none(),const RooCmdArg& arg9=RooCmdArg::none()) ;

  RooChi2VarIntegral(const char *name, const char *title, RooAbsPdf& pdf, RooDataHist& data,
	    Bool_t extended=kFALSE, const char* rangeName=0, const char* addCoefRangeName=0, Int_t nCPU=1, Bool_t verbose=kTRUE, Bool_t splitCutRange=kTRUE) ;

  RooChi2VarIntegral(const char *name, const char *title, RooAbsPdf& pdf, RooDataHist& data,
	    const RooArgSet& projDeps, Bool_t extended=kFALSE, const char* rangeName=0, const char* addCoefRangeName=0, Int_t nCPU=1, Bool_t verbose=kTRUE, Bool_t splitCutRange=kTRUE) ;

  RooChi2VarIntegral(const RooChi2VarIntegral& other, const char* name=0); 

  virtual TObject* clone(const char* newname) const { return new RooChi2VarIntegral(*this,newname); }

  virtual RooAbsTestStatistic* create(const char *name, const char *title, RooAbsReal& pdf, RooAbsData& data,
				      const RooArgSet& projDeps, const char* rangeName=0, const char* addCoefRangeName=0, Int_t nCPU=1,  Bool_t interleave=kFALSE, Bool_t verbose=kTRUE, Bool_t splitCutRange=kTRUE) {
    
    RooChi2VarIntegral* newchi2=new RooChi2VarIntegral(name,title,(RooAbsPdf&)pdf,(RooDataHist&)data,projDeps,_extended,rangeName,addCoefRangeName,nCPU, verbose, splitCutRange);
    newchi2->SetIntegralBins(NIntegbins);
    newchi2->SetEType(_etype);
    return newchi2 ;
  }
  
  virtual ~RooChi2VarIntegral();

  virtual Double_t defaultErrorLevel() const { return 1.0 ; }

  void SetIntegralBins(Int_t N){if(N<1)cout<<"RooChi2VarIntegral:: Invalid number of Integration bins"<<endl; else NIntegbins=N;}
  void SetEType(RooDataHist::ErrorType etype){_etype=etype;}
  
 protected:

  static RooArgSet _emptySet ; // Supports named argument constructor

  RooDataHist::ErrorType _etype ;
  Bool_t _extended ;
  virtual Double_t evaluatePartition(Int_t firstEvent, Int_t lastEvent, Int_t stepSize) const ;
  
  Int_t NIntegbins;  
  ClassDef(RooChi2VarIntegral,1) // Abstract real-valued variable
};


#endif
