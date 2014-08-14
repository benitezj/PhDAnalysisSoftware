/*****************************************************************************
 * Project: RooFit                                                           *
 *****************************************************************************/

// -- CLASS DESCRIPTION [PDF] --
// Class RooChi2VarIntegral implements a simple chi^2 calculation from a binned dataset
// and a PDF. The chi^2 is calculated as 
//
//             / (f_PDF * N_tot/ V_bin) - N_bin \+2
//  Sum[bins] |  ------------------------------ |
//             \         err_bin                /
//
// If no user-defined errors are defined for the dataset, poisson errors
// are used. In extended PDF mode, N_tot is substituted with N_expected.

#include "RooFit.h"

#include "RooChi2VarIntegral.h"
#include "RooDataHist.h"
#include "RooAbsPdf.h"
#include "RooCmdConfig.h"
#include "RooMsgService.h"
#include "RooRealVar.h"
#include "Riostream.h"
#include "TIterator.h"


ClassImp(RooChi2VarIntegral)
;

RooArgSet RooChi2VarIntegral::_emptySet ;

RooChi2VarIntegral::RooChi2VarIntegral(const char *name, const char* title, RooAbsPdf& pdf, RooDataHist& data,
		       const RooCmdArg& arg1,const RooCmdArg& arg2,const RooCmdArg& arg3,
		       const RooCmdArg& arg4,const RooCmdArg& arg5,const RooCmdArg& arg6,
		       const RooCmdArg& arg7,const RooCmdArg& arg8,const RooCmdArg& arg9) :
  RooAbsOptTestStatistic(name,title,pdf,data,
			 *(const RooArgSet*)RooCmdConfig::decodeObjOnTheFly("RooChi2VarIntegral::RooChi2VarIntegral","ProjectedObservables",0,&_emptySet,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9),
			 RooCmdConfig::decodeStringOnTheFly("RooChi2VarIntegral::RooChi2VarIntegral","RangeWithName",0,"",arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9),
			 RooCmdConfig::decodeStringOnTheFly("RooChi2VarIntegral::RooChi2VarIntegral","AddCoefRange",0,"",arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9),
			 RooCmdConfig::decodeIntOnTheFly("RooChi2VarIntegral::RooChi2VarIntegral","NumCPU",0,1,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9),
			 RooCmdConfig::decodeIntOnTheFly("RooChi2VarIntegral::RooChi2VarIntegral","Verbose",0,1,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9),
			 RooCmdConfig::decodeIntOnTheFly("RooChi2VarIntegral::RooChi2VarIntegral","SplitRange",0,0,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9)),
  NIntegbins(1)           
  //  RooChi2VarIntegral constructor. Optional arguments taken
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
  RooCmdConfig pc("RooChi2VarIntegral::RooChi2VarIntegral") ;
  pc.defineInt("extended","Extended",0,kFALSE) ;
  pc.defineInt("etype","DataError",0,(Int_t)RooDataHist::Poisson) ;  

  pc.process(arg1) ;  pc.process(arg2) ;  pc.process(arg3) ;
  pc.process(arg4) ;  pc.process(arg5) ;  pc.process(arg6) ;
  pc.process(arg7) ;  pc.process(arg8) ;  pc.process(arg9) ;

  _extended = pc.getInt("extended") ;
  _etype = (RooDataHist::ErrorType) pc.getInt("etype") ;

}


RooChi2VarIntegral::RooChi2VarIntegral(const char *name, const char *title, RooAbsPdf& pdf, RooDataHist& data,
		     Bool_t extended, const char* cutRange, const char* addCoefRange,Int_t nCPU, Bool_t verbose, Bool_t splitCutRange) : 
  RooAbsOptTestStatistic(name,title,pdf,data,RooArgSet(),cutRange,addCoefRange,nCPU,verbose,splitCutRange),
  _etype(RooAbsData::Poisson), _extended(extended),NIntegbins(1)
{
  
}


RooChi2VarIntegral::RooChi2VarIntegral(const char *name, const char *title, RooAbsPdf& pdf, RooDataHist& data,
		     const RooArgSet& projDeps, Bool_t extended, const char* cutRange, const char* addCoefRange, Int_t nCPU, Bool_t verbose, Bool_t splitCutRange) : 
  RooAbsOptTestStatistic(name,title,pdf,data,projDeps,cutRange,addCoefRange,nCPU,verbose,splitCutRange),
  _etype(RooAbsData::Poisson), _extended(extended),NIntegbins(1)
{
  
}


RooChi2VarIntegral::RooChi2VarIntegral(const RooChi2VarIntegral& other, const char* name) : 
  RooAbsOptTestStatistic(other,name),
  _etype(other._etype),
  _extended(other._extended),
  NIntegbins(other.NIntegbins)
{
}


RooChi2VarIntegral::~RooChi2VarIntegral()
{
}


Double_t RooChi2VarIntegral::evaluatePartition(Int_t firstEvent, Int_t lastEvent, Int_t stepSize) const 
{
  Int_t i ;
  Double_t result(0) ;

  // Determine total number of data events to be used for PDF normalization
  Double_t nDataTotal ;
  if (_extended) {
    nDataTotal = ((RooAbsPdf*)_funcClone)->expectedEvents(_dataClone->get()) ;
  } else {
    nDataTotal = _dataClone->sumEntries() ;
  }

  // Loop over bins of dataset
  RooDataHist* data = (RooDataHist*) _dataClone ;
  for (i=firstEvent ; i<lastEvent ; i+=stepSize) {
    
    // get the data values for this event
    const RooArgSet*xcoords=data->get(i);
    Double_t nData = data->weight() ;
    Double_t nPdf = 0;

    Double_t eIntLo,eIntHi ;
    data->weightError(eIntLo,eIntHi,_etype) ;
    Double_t eInt = (eIntHi>eIntLo) ? eIntHi : eIntLo ;


    // Return 0 if eInt=0, special handling in MINUIT will follow
    if (eInt==0.&&nData!=0.) {
      coutE(Eval) << "RooChi2VarIntegral::RooChi2VarIntegral(" << GetName() << ") INFINITY ERROR: bin " << i 
		  << " has zero error, but data is not zero = " << nData << endl ;
      return 0 ;
    }

    
    if(_normSet->getSize()==1){//it is a 1-D PDF
      RooRealVar*onlyvar=(RooRealVar*)xcoords->first();
      Double_t binwidth= onlyvar->getBinWidth(i);       
      Double_t center=onlyvar->getVal();


      //--------------
      //Simple Rieman sum
      //--------------
      for(Int_t k=0;k<NIntegbins;k++){
	onlyvar->setVal(center-binwidth/2+(k+.5)*(binwidth/NIntegbins));
	nPdf+=((RooAbsPdf*)_funcClone)->getVal(_normSet);
      }

      nPdf=nPdf*(binwidth/NIntegbins)*nDataTotal;
                
      onlyvar->setVal(center);      
    }else if(_normSet->getSize()==2){//it is a 2-D Pdf
      TIterator*iter=xcoords->createIterator();
      RooRealVar*varx=(RooRealVar*)iter->Next();
      RooRealVar*vary=(RooRealVar*)iter->Next();
      Double_t centerx=varx->getVal();
      Double_t centery=vary->getVal();
      Double_t binwidthx=varx->getBinWidth(i);
      Double_t binwidthy=vary->getBinWidth(i);
      //--------------------
      //simple Riemman integral
      //--------------------
      for(Int_t k=0;k<NIntegbins;k++){
	varx->setVal(centerx-binwidthx/2+(k+.5)*binwidthx/NIntegbins);
	for(Int_t l=0;l<NIntegbins;l++){
	  vary->setVal(centery-binwidthy/2+(l+.5)*binwidthy/NIntegbins);
	  nPdf+=((RooAbsPdf*)_funcClone)->getVal(_normSet);
	}
      }
 
      nPdf=nPdf*(binwidthx/NIntegbins)*(binwidthy/NIntegbins)*nDataTotal;
             
      varx->setVal(centerx);  
      vary->setVal(centery);      
    }else nPdf = ((RooAbsPdf*)_funcClone)->getVal(_normSet) * nDataTotal * data->binVolume() ;//do not integrate for more dimensions
    
    
    if (nPdf==0&&nData==0) continue ;

    Double_t eExt = nPdf-nData ;
   
       
    //cout<<i<<"nData="<<nData<<"  Error="<<eInt<<endl;

    result += eExt*eExt/(eInt*eInt) ;
  }
  

  return result ;
}



