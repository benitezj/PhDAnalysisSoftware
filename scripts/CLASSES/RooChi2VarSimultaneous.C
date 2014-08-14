/*****************************************************************************
 * Project: RooFit                                                           *
 *****************************************************************************/

// -- CLASS DESCRIPTION [PDF] --
// Class RooChi2VarSimultaneous implements a simple chi^2 calculation from a binned dataset
// and a PDF. The chi^2 is calculated as 
//
//             / (f_PDF * N_tot/ V_bin) - N_bin \+2
//  Sum[bins] |  ------------------------------ |
//             \         err_bin                /
//
// If no user-defined errors are defined for the dataset, poisson errors
// are used. In extended PDF mode, N_tot is substituted with N_expected.

#include "RooFit.h"

#include "RooChi2VarSimultaneous.h"
#include "RooDataHist.h"
#include "RooAbsPdf.h"
#include "RooCmdConfig.h"
#include "RooMsgService.h"
#include "RooRealVar.h"
#include "Riostream.h"
#include "TIterator.h"


ClassImp(RooChi2VarSimultaneous)
;

RooArgSet RooChi2VarSimultaneous::_emptySet ;

RooChi2VarSimultaneous::RooChi2VarSimultaneous(const char *name, const char* title, RooAbsPdf& pdf, RooDataHist& data,
				       TH1** Hists,RooAbsPdf** Pdfs,RooRealVar** Vars,Int_t NTot,
		       const RooCmdArg& arg1,const RooCmdArg& arg2,const RooCmdArg& arg3,
		       const RooCmdArg& arg4,const RooCmdArg& arg5,const RooCmdArg& arg6,
		       const RooCmdArg& arg7,const RooCmdArg& arg8,const RooCmdArg& arg9) :
  RooAbsOptTestStatistic(name,title,pdf,data,
			 *(const RooArgSet*)RooCmdConfig::decodeObjOnTheFly("RooChi2VarSimultaneous::RooChi2VarSimultaneous","ProjectedObservables",0,&_emptySet,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9),
			 RooCmdConfig::decodeStringOnTheFly("RooChi2VarSimultaneous::RooChi2VarSimultaneous","RangeWithName",0,"",arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9),
			 RooCmdConfig::decodeStringOnTheFly("RooChi2VarSimultaneous::RooChi2VarSimultaneous","AddCoefRange",0,"",arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9),
			 RooCmdConfig::decodeIntOnTheFly("RooChi2VarSimultaneous::RooChi2VarSimultaneous","NumCPU",0,1,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9),
			 RooCmdConfig::decodeIntOnTheFly("RooChi2VarSimultaneous::RooChi2VarSimultaneous","Verbose",0,1,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9),
			 RooCmdConfig::decodeIntOnTheFly("RooChi2VarSimultaneous::RooChi2VarSimultaneous","SplitRange",0,0,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9))
  //  RooChi2VarSimultaneous constructor. Optional arguments taken
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
  RooCmdConfig pc("RooChi2VarSimultaneous::RooChi2VarSimultaneous") ;
  pc.defineInt("extended","Extended",0,kFALSE) ;
  pc.defineInt("etype","DataError",0,(Int_t)RooDataHist::Poisson) ;  

  pc.process(arg1) ;  pc.process(arg2) ;  pc.process(arg3) ;
  pc.process(arg4) ;  pc.process(arg5) ;  pc.process(arg6) ;
  pc.process(arg7) ;  pc.process(arg8) ;  pc.process(arg9) ;

  _extended = pc.getInt("extended") ;
  _etype = (RooDataHist::ErrorType) pc.getInt("etype") ;

  if(!Hists||!Pdfs||NTot<1){cout<<"bad imputs !Hists||!Pdfs||NTot<1"<<endl;exit(0);}
  for(Int_t h=0;h<NTot;h++)
    if(!Hists[h]){cout<<"no hist["<<h<<"]"<<endl;exit(0);}
  for(Int_t h=0;h<NTot;h++)
    if(!Pdfs[h]){cout<<"no pdf["<<h<<"]"<<endl;exit(0);}
  for(Int_t h=0;h<NTot;h++)
    if(!Vars[h]){cout<<"no pdf["<<h<<"]"<<endl;exit(0);}
  _Hists=Hists;
  _Pdfs=Pdfs;
  _NTot=NTot;
  _Vars=Vars;

}


RooChi2VarSimultaneous::RooChi2VarSimultaneous(const char *name, const char *title, RooAbsPdf& pdf, RooDataHist& data,
		     Bool_t extended, const char* cutRange, const char* addCoefRange,Int_t nCPU, Bool_t verbose, Bool_t splitCutRange) : 
  RooAbsOptTestStatistic(name,title,pdf,data,RooArgSet(),cutRange,addCoefRange,nCPU,verbose,splitCutRange),
  _etype(RooAbsData::Poisson), _extended(extended)
{
  
}


RooChi2VarSimultaneous::RooChi2VarSimultaneous(const char *name, const char *title, RooAbsPdf& pdf, RooDataHist& data,
		     const RooArgSet& projDeps, Bool_t extended, const char* cutRange, const char* addCoefRange, Int_t nCPU, Bool_t verbose, Bool_t splitCutRange) : 
  RooAbsOptTestStatistic(name,title,pdf,data,projDeps,cutRange,addCoefRange,nCPU,verbose,splitCutRange),
  _etype(RooAbsData::Poisson), _extended(extended)
{
  
}


RooChi2VarSimultaneous::RooChi2VarSimultaneous(const RooChi2VarSimultaneous& other, const char* name) : 
  RooAbsOptTestStatistic(other,name),
  _etype(other._etype),
  _extended(other._extended),
  _Hists(other._Hists),
  _Pdfs(other._Pdfs),
  _Vars(other._Vars),
  _NTot(other._NTot)
{
}


RooChi2VarSimultaneous::~RooChi2VarSimultaneous()
{
}


Double_t RooChi2VarSimultaneous::evaluatePartition(Int_t firstEvent, Int_t lastEvent) const 
{
  Double_t result(0) ;
  Int_t counter=0;
  Int_t NtotBins=0;
  for(Int_t h=0;h<_NTot;h++)
    NtotBins+=_Hists[h]->GetXaxis()->GetNbins();  

  for(Int_t h=0;h<_NTot;h++){
    Double_t nDataTotal=_Hists[h]->Integral();
    RooRealVar*xvar=_Vars[h];
    Double_t origval=xvar->getVal();
    RooArgSet normset(*xvar);

    for(Int_t i=1;i<=_Hists[h]->GetXaxis()->GetNbins();i++){
      counter++;
      if(counter>=firstEvent&&counter<=lastEvent){

	Double_t nData =_Hists[h]->GetBinContent(i);
	Double_t eInt =_Hists[h]->GetBinError(i);
	Double_t binwidth=_Hists[h]->GetBinWidth(i);
	Double_t center=_Hists[h]->GetBinCenter(i);
 
	xvar->setVal(center);
	Double_t nPdf=_Pdfs[h]->getVal(&normset)*binwidth*nDataTotal;
      
	if ((nPdf==0.&&nData==0.)||eInt<=0.) continue ;

	Double_t eExt = nPdf-nData ;   
	result += eExt*eExt/(eInt*eInt) ;
      }
    }
    xvar->setVal(origval); 
  }
       

  return result ;
}



