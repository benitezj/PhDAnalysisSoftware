/*****************************************************************************
 * Project: RooFit                                                           *
 *****************************************************************************/

// -- CLASS DESCRIPTION [PDF] --
// Class RooCutOptimizer implements a simple chi^2 calculation from a binned dataset
// and a PDF. The chi^2 is calculated as 
//
//             / (f_PDF * N_tot/ V_bin) - N_bin \+2
//  Sum[bins] |  ------------------------------ |
//             \         err_bin                /
//
// If no user-defined errors are defined for the dataset, poisson errors
// are used. In extended PDF mode, N_tot is substituted with N_expected.

#include "RooFit.h"

#include "RooCutOptimizer.h"
#include "RooDataHist.h"
#include "RooAbsPdf.h"
#include "RooCmdConfig.h"
#include "RooMsgService.h"
#include "RooRealVar.h"
#include "Riostream.h"
#include "TIterator.h"



ClassImp(RooCutOptimizer)
;

RooArgSet RooCutOptimizer::_emptySet ;

RooCutOptimizer::RooCutOptimizer(const char *name, const char* title, RooAbsPdf& pdf, RooDataHist& data,TTree* TreeSig,TTree* TreeBkg,TTree* TreeBkg2,TTree* TreeBkg3,RooArgList* Cuts,
		       const RooCmdArg& arg1,const RooCmdArg& arg2,const RooCmdArg& arg3,
		       const RooCmdArg& arg4,const RooCmdArg& arg5,const RooCmdArg& arg6,
		       const RooCmdArg& arg7,const RooCmdArg& arg8,const RooCmdArg& arg9) :
  RooAbsOptTestStatistic(name,title,pdf,data,
			 *(const RooArgSet*)RooCmdConfig::decodeObjOnTheFly("RooCutOptimizer::RooCutOptimizer","ProjectedObservables",0,&_emptySet,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9),
			 RooCmdConfig::decodeStringOnTheFly("RooCutOptimizer::RooCutOptimizer","RangeWithName",0,"",arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9),
			 RooCmdConfig::decodeStringOnTheFly("RooCutOptimizer::RooCutOptimizer","AddCoefRange",0,"",arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9),
			 RooCmdConfig::decodeIntOnTheFly("RooCutOptimizer::RooCutOptimizer","NumCPU",0,1,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9),
			 RooCmdConfig::decodeIntOnTheFly("RooCutOptimizer::RooCutOptimizer","Verbose",0,1,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9),
			 RooCmdConfig::decodeIntOnTheFly("RooCutOptimizer::RooCutOptimizer","SplitRange",0,0,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9)),
  _extended(0),
  treesig(TreeSig),
  treebkg(TreeBkg),
  treebkg2(TreeBkg2),
  treebkg3(TreeBkg3),
  cutlist(Cuts)											   
  //  RooCutOptimizer constructor. Optional arguments taken
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
  RooCmdConfig pc("RooCutOptimizer::RooCutOptimizer") ;
  pc.defineInt("extended","Extended",0,kFALSE) ;
  pc.defineInt("etype","DataError",0,(Int_t)RooDataHist::Poisson) ;  

  pc.process(arg1) ;  pc.process(arg2) ;  pc.process(arg3) ;
  pc.process(arg4) ;  pc.process(arg5) ;  pc.process(arg6) ;
  pc.process(arg7) ;  pc.process(arg8) ;  pc.process(arg9) ;

  _extended = pc.getInt("extended") ;
  _etype = (RooDataHist::ErrorType) pc.getInt("etype") ;
  
}


RooCutOptimizer::RooCutOptimizer(const char *name, const char *title, RooAbsPdf& pdf, RooDataHist& data,TTree* TreeSig,TTree* TreeBkg,TTree* TreeBkg2,TTree* TreeBkg3,RooArgList* Cuts,
		     Bool_t extended, const char* cutRange, const char* addCoefRange,Int_t nCPU, Bool_t verbose, Bool_t splitCutRange) : 
  RooAbsOptTestStatistic(name,title,pdf,data,RooArgSet(),cutRange,addCoefRange,nCPU,verbose,splitCutRange),
   _etype(RooAbsData::Poisson), _extended(extended),
  treesig(TreeSig),
  treebkg(TreeBkg),
  treebkg2(TreeBkg2),
  treebkg3(TreeBkg3),
  cutlist(Cuts)
{
  
}


RooCutOptimizer::RooCutOptimizer(const char *name, const char *title, RooAbsPdf& pdf, RooDataHist& data,TTree* TreeSig,TTree* TreeBkg,TTree* TreeBkg2,TTree* TreeBkg3,RooArgList* Cuts,
		     const RooArgSet& projDeps, Bool_t extended, const char* cutRange, const char* addCoefRange, Int_t nCPU, Bool_t verbose, Bool_t splitCutRange) : 
  RooAbsOptTestStatistic(name,title,pdf,data,projDeps,cutRange,addCoefRange,nCPU,verbose,splitCutRange),
  _etype(RooAbsData::Poisson), _extended(extended),
  treesig(TreeSig),
  treebkg(TreeBkg),
  treebkg2(TreeBkg2),
  treebkg3(TreeBkg3),
  cutlist(Cuts)
{
  
}


RooCutOptimizer::RooCutOptimizer(const RooCutOptimizer& other, const char* name) : 
  RooAbsOptTestStatistic(other,name),
  _etype(other._etype),
  _extended(other._extended)
{
}


RooCutOptimizer::~RooCutOptimizer()
{
}


Double_t RooCutOptimizer::evaluatePartition(Int_t firstEvent, Int_t lastEvent) const 
{
  firstEvent=lastEvent;

  if(cutlist->getSize()%2!=0){
    cout<<"number of cut vars must be pairs : (min,max).  Mins first half, Maxs second half.)"<<endl; 
    return 0;
  }
  RooRealVar* var=NULL;
  TCut cut;
  char textl[100];
  TIterator*iter=cutlist->createIterator();
  for(Int_t i=1;i<=cutlist->getSize()/2;i++){
    
    //low cut
    var=(RooRealVar*)iter->Next();
    sprintf(textl,"%3f<%s",var->getVal(),var->GetTitle());
    cut+=TCut(textl);

    //if(!var->isConstant())cout<<" "<<var->GetName()<<" "<<var->getVal();

    //high cut
    var=(RooRealVar*)iter->Next();
    sprintf(textl,"%s<%3f",var->getUnit(),var->getVal());
    cut+=TCut(textl);

    //if(!var->isConstant())cout<<" "<<var->GetName()<<" "<<var->getVal();
  }


  
  //method 1: use raw integrals
  Float_t SB=(float)treesig->GetEntries(cut);
  Float_t B1=(float)treebkg->GetEntries(cut);
  Float_t B2=(float)treebkg2->GetEntries(cut);
  Float_t B3=(float)treebkg3->GetEntries(cut);
  Float_t B=B1-2.*(B2-B1-5.*(((B2-B1)-(B3-B1)/2.)/5.))-4.*(((B2-B1)-(B3-B1)/2.)/5.);
  //Float_t fcn=sqrt((double)(treesig->GetEntries("")*SB))/fabs((double)(SB-B));
  Float_t fcn=sqrt(SB)/fabs(SB-B);

  //cout<<" "<<B3<<" "<<B2<<" "<<B1<<" "<<B<<" "<<SB<<" "<<fcn<<endl;

  return fcn;
  //return 1.0e3*sqrt((double)SB)/fabs((double)(SB-(B1+B1-B2)));
  //return sqrt(SB)/fabs(SB-B1);//minimize sqrt(S+B)/S
  //return SB/fabs(SB-B1);//minimize S+B/S

}


TGraph* RooCutOptimizer::ScanVariable(RooRealVar* Var){

  TGraph G;

  Int_t N=50;
  Float_t step=(Var->getMax()-Var->getMin())/N;
  for(Int_t i=0;i<N;i++){
    Var->setVal(Var->getMin()+i*step);
    G.SetPoint(i,Var->getVal(),1./evaluatePartition(1,1));
    //cout<<i<<" "<<Var->getVal()<<" "<<evaluatePartition(1,1)<<endl;
  }
  
  G.GetYaxis()->SetTitle("Significance");
  G.GetXaxis()->SetTitle(Var->GetTitle());
  
  return (TGraph*)G.Clone();
}
