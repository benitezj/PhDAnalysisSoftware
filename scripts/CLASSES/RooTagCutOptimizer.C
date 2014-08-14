/*****************************************************************************
 * Project: RooFit                                                           *
 *****************************************************************************/



#include "RooFit.h"

#include "RooTagCutOptimizer.h"
#include "RooDataHist.h"
#include "RooAbsPdf.h"
#include "RooCmdConfig.h"
#include "RooMsgService.h"
#include "RooRealVar.h"
#include "Riostream.h"
#include "TIterator.h"



ClassImp(RooTagCutOptimizer)
;

RooArgSet RooTagCutOptimizer::_emptySet ;

RooTagCutOptimizer::RooTagCutOptimizer(const char *name, const char* title, RooAbsPdf& pdf, RooDataHist& data,TTree* TreeSig,TTree* TreeBkg,RooArgList* Cuts,
		       const RooCmdArg& arg1,const RooCmdArg& arg2,const RooCmdArg& arg3,
		       const RooCmdArg& arg4,const RooCmdArg& arg5,const RooCmdArg& arg6,
		       const RooCmdArg& arg7,const RooCmdArg& arg8,const RooCmdArg& arg9) :
  RooAbsOptTestStatistic(name,title,pdf,data,
			 *(const RooArgSet*)RooCmdConfig::decodeObjOnTheFly("RooTagCutOptimizer::RooTagCutOptimizer","ProjectedObservables",0,&_emptySet,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9),
			 RooCmdConfig::decodeStringOnTheFly("RooTagCutOptimizer::RooTagCutOptimizer","RangeWithName",0,"",arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9),
			 RooCmdConfig::decodeStringOnTheFly("RooTagCutOptimizer::RooTagCutOptimizer","AddCoefRange",0,"",arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9),
			 RooCmdConfig::decodeIntOnTheFly("RooTagCutOptimizer::RooTagCutOptimizer","NumCPU",0,1,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9),
			 RooCmdConfig::decodeIntOnTheFly("RooTagCutOptimizer::RooTagCutOptimizer","Verbose",0,1,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9),
			 RooCmdConfig::decodeIntOnTheFly("RooTagCutOptimizer::RooTagCutOptimizer","SplitRange",0,0,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9)),
  _extended(0),
  sigtype(1),
  treesig(TreeSig),
  treebkg(TreeBkg),
  cutlist(Cuts),
  sig0(0.)										   
{
  RooCmdConfig pc("RooTagCutOptimizer::RooTagCutOptimizer") ;
  pc.defineInt("extended","Extended",0,kFALSE) ;
  pc.defineInt("etype","DataError",0,(Int_t)RooDataHist::Poisson) ;  

  pc.process(arg1) ;  pc.process(arg2) ;  pc.process(arg3) ;
  pc.process(arg4) ;  pc.process(arg5) ;  pc.process(arg6) ;
  pc.process(arg7) ;  pc.process(arg8) ;  pc.process(arg9) ;

  _extended = pc.getInt("extended") ;
  _etype = (RooDataHist::ErrorType) pc.getInt("etype") ; 

  sig0=evaluatePartition(1,1);  
}


RooTagCutOptimizer::RooTagCutOptimizer(const char *name, const char *title, RooAbsPdf& pdf, RooDataHist& data,TTree* TreeSig,TTree* TreeBkg,RooArgList* Cuts,Bool_t extended, const char* cutRange, const char* addCoefRange,Int_t nCPU, Bool_t verbose, Bool_t splitCutRange):
  RooAbsOptTestStatistic(name,title,pdf,data,RooArgSet(),cutRange,addCoefRange,nCPU,verbose,splitCutRange),
  _etype(RooAbsData::Poisson), _extended(extended),
  sigtype(1),
  treesig(TreeSig),
  treebkg(TreeBkg),
  cutlist(Cuts),
  sig0(0.)
{
  sig0=evaluatePartition(1,1); 
}


RooTagCutOptimizer::RooTagCutOptimizer(const char *name, const char *title, RooAbsPdf& pdf, RooDataHist& data,TTree* TreeSig,TTree* TreeBkg,RooArgList* Cuts,
		     const RooArgSet& projDeps, Bool_t extended, const char* cutRange, const char* addCoefRange, Int_t nCPU, Bool_t verbose, Bool_t splitCutRange) : 
  RooAbsOptTestStatistic(name,title,pdf,data,projDeps,cutRange,addCoefRange,nCPU,verbose,splitCutRange),
  _etype(RooAbsData::Poisson), _extended(extended),
  sigtype(1),
  treesig(TreeSig),
  treebkg(TreeBkg),
  cutlist(Cuts),
  sig0(0.)
{
  sig0=evaluatePartition(1,1); 
}


RooTagCutOptimizer::RooTagCutOptimizer(const RooTagCutOptimizer& other, const char* name) : 
  RooAbsOptTestStatistic(other,name),
  _etype(other._etype),
  _extended(other._extended),
  sigtype(other.sigtype),
  treesig(other.treesig),
  treebkg(other.treebkg),
  cutlist(other.cutlist),
  sig0(other.sig0)
{
}


RooTagCutOptimizer::~RooTagCutOptimizer()
{
}


Double_t RooTagCutOptimizer::evaluatePartition(Int_t firstEvent, Int_t lastEvent) const 
{
  firstEvent=lastEvent;
  if(!treesig||!treebkg||!cutlist){cout<<"RooTagCutOptimizer: !treesig||!treebkg||!cutlist"<<endl;return 0.;}
  

  TCut sigcut;
  TCut bkgcut;
  char textl[100];
  TIterator*iter=cutlist->createIterator();
  while(RooRealVar* var=(RooRealVar*)iter->Next()){
    sprintf(textl,var->GetTitle(),var->getVal());
    sigcut+=TCut(textl); 
    
    if(TString(var->GetName())==TString("mwin"))
      sprintf(textl,var->GetTitle(),var->getVal()+.06);
    bkgcut+=TCut(textl); 
  }

  Float_t SplusB=(float)treesig->GetEntries(sigcut);
  Float_t B=(float)treebkg->GetEntries(bkgcut); 
  Float_t sig=(SplusB-B)/sqrt(SplusB);
  if(sigtype==1)sig=(SplusB-B)/sqrt(SplusB);//maximize significance
  if(sigtype==2)sig=(SplusB-B)/B;//maximize S/B
  if(sigtype==3)sig=(SplusB-B)/SplusB;//maximize purity
  if(sigtype==4)sig=sqrt(SplusB)/(SplusB-B);//minimize relative error on S yield, (Note this is the same as significance)
  
  //cout<<sigcut<<" "<<bkgcut<<" "<<sig<<endl;
  if(sig0==0.)return sig;
  return 1000.*sig0/sig;//normalize to initial value
}


TGraph* RooTagCutOptimizer::ScanVariable(RooRealVar* Var,Int_t Npoints){

  if(!Var||Npoints<1){cout<<"RooTagCutOptimizer::ScanVariable bad arguments"<<endl;return 0;}

  TGraph G;  
  Float_t step=(Var->getMax()-Var->getMin())/Npoints;
  for(Int_t i=0;i<Npoints;i++){
    Var->setVal(Var->getMin()+i*step);
    G.SetPoint(i,Var->getVal(),GetSignificance());
  }
  
  G.GetYaxis()->SetTitle("Significance");
  G.GetXaxis()->SetTitle(Var->getUnit());
  
  return (TGraph*)G.Clone();
}
