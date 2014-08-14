/*****************************************************************************
 * Project: RooFit                                                           *
 * Package: RooFitCore                                                       *
 * @(#)root/roofitcore:$Id: DRecoSimulChi2Var.cc,v 1.5 2010/06/07 18:31:59 benitezj Exp $
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
#include "DRecoilToolsUser/NtupleCode/DRecoSimulChi2Var.hh"
#include "DRecoilToolsUser/NtupleCode/DRecoAddPdf.hh"
#include "DRecoilToolsUser/NtupleCode/DRecoHistPdf.hh"
#include "DRecoilToolsUser/NtupleCode/DRecoProdPdf.hh"
#include "DRecoilToolsUser/NtupleCode/DRecoTranslationPdf.hh"
#include <TH1.h>
#include "RooRealVar.h"

#include "RooDataHist.h"
#include "RooAbsPdf.h"
#include "RooCmdConfig.h"
#include "RooMsgService.h"

#include "Riostream.h"


ClassImp(DRecoSimulChi2Var)
;

RooArgSet DRecoSimulChi2Var::_emptySet ;

DRecoSimulChi2Var::DRecoSimulChi2Var(const char *name, const char* title, RooAbsPdf& pdf, RooDataHist& data,
		       TList* Hists,TList* Pdfs,TList* Vars,
		       const RooCmdArg& arg1,const RooCmdArg& arg2,const RooCmdArg& arg3,
		       const RooCmdArg& arg4,const RooCmdArg& arg5,const RooCmdArg& arg6,
		       const RooCmdArg& arg7,const RooCmdArg& arg8,const RooCmdArg& arg9) :
  RooAbsOptTestStatistic(name,title,pdf,data,
			 *(const RooArgSet*)RooCmdConfig::decodeObjOnTheFly("DRecoSimulChi2Var::DRecoSimulChi2Var","ProjectedObservables",0,&_emptySet
									    ,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9),
			 RooCmdConfig::decodeStringOnTheFly("DRecoSimulChi2Var::DRecoSimulChi2Var","RangeWithName",0,"",arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9),
			 RooCmdConfig::decodeStringOnTheFly("DRecoSimulChi2Var::DRecoSimulChi2Var","AddCoefRange",0,"",arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9),
			 RooCmdConfig::decodeIntOnTheFly("DRecoSimulChi2Var::DRecoSimulChi2Var","NumCPU",0,1,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9),
			 RooCmdConfig::decodeIntOnTheFly("DRecoSimulChi2Var::DRecoSimulChi2Var","Verbose",0,1,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9),
			 RooCmdConfig::decodeIntOnTheFly("DRecoSimulChi2Var::DRecoSimulChi2Var","SplitRange",0,0,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9))             
  //  DRecoSimulChi2Var constructor. Optional arguments taken
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
  RooCmdConfig pc("DRecoSimulChi2Var::DRecoSimulChi2Var") ;
  pc.defineInt("extended","Extended",0,kFALSE) ;
  pc.defineInt("etype","DataError",0,(Int_t)RooDataHist::Poisson) ;  
  pc.defineInt("nCPU","NumCPU",0,1) ;  

  pc.process(arg1) ;  pc.process(arg2) ;  pc.process(arg3) ;
  pc.process(arg4) ;  pc.process(arg5) ;  pc.process(arg6) ;
  pc.process(arg7) ;  pc.process(arg8) ;  pc.process(arg9) ;

  _extended = pc.getInt("extended") ;
  _etype = (RooDataHist::ErrorType) pc.getInt("etype") ;

  if(Hists->GetSize()!=Pdfs->GetSize()){// ||Pdfs->GetSize()!= Vars->GetSize()||Vars->GetSize()!Hists->GetSize()
    cout<<"DRecoChi2VarSimultaneous: sizes dont match"<<endl;exit(0);}
  _Hists=Hists;
  _Pdfs=Pdfs;
  _Vars=Vars;

  _HistsIter= _Hists->MakeIterator();
  _VarsIter= _Vars->MakeIterator();
  _PdfsIter= _Pdfs->MakeIterator();

  _ncpu=pc.getInt("nCPU") ;

}


DRecoSimulChi2Var::DRecoSimulChi2Var(const char *name, const char *title, RooAbsPdf& pdf, RooDataHist& data,
		     Bool_t extended, const char* cutRange, const char* addCoefRange,Int_t nCPU, Bool_t verbose, Bool_t splitCutRange) : 
  RooAbsOptTestStatistic(name,title,pdf,data,RooArgSet(),cutRange,addCoefRange,nCPU,verbose,splitCutRange),
  _etype(RooAbsData::Poisson),
  _extended(extended),
  _ncpu(nCPU)
{
  
}


DRecoSimulChi2Var::DRecoSimulChi2Var(const char *name, const char *title, RooAbsReal& pdf, RooDataHist& data,
		     const RooArgSet& projDeps, Bool_t extended, const char* cutRange, const char* addCoefRange, Int_t nCPU, Bool_t verbose, Bool_t splitCutRange) : 
  RooAbsOptTestStatistic(name,title,pdf,data,projDeps,cutRange,addCoefRange,nCPU,verbose,splitCutRange),
  _etype(RooAbsData::Poisson), 
  _extended(extended),
  _ncpu(nCPU)
{
  
}


DRecoSimulChi2Var::DRecoSimulChi2Var(const DRecoSimulChi2Var& other, const char* name) : 
  RooAbsOptTestStatistic(other,name),
  _etype(other._etype),
  _extended(other._extended),
  _Hists(other._Hists),
  _Pdfs(other._Pdfs),
  _Vars(other._Vars),
  _ncpu(other._ncpu)
{

  _HistsIter= _Hists->MakeIterator();
  _VarsIter= _Vars->MakeIterator();
  _PdfsIter= _Pdfs->MakeIterator();

}


DRecoSimulChi2Var::~DRecoSimulChi2Var()
{


  delete _HistsIter;
  delete _VarsIter;
  delete _PdfsIter;

}


Double_t DRecoSimulChi2Var::evaluatePartition(Int_t firstEvent, Int_t lastEvent, Int_t stepSize) const 
{
  ////cout<<firstEvent<<" "<<lastEvent<<endl;
  stepSize=1;//new option for RHEL5

  Double_t result(0);
  _HistsIter->Reset();
  _VarsIter->Reset();
  _PdfsIter->Reset();
  //cout<<" 1 "<<endl;

  //first histogram determines dimension
  TH1*hdim=(TH1*)_Hists->First();
   //cout<<" 2 "<<endl;
  if(hdim->GetDimension()==1){ ///1D Histograms
    if(_Vars->GetSize()!=_Hists->GetSize()){ 
      cout<<"DRecoSimulChi2Var::evaluatePartition number of variables does not match"<<endl;
      exit(0);
    }
    //cout<<" 3 "<<endl;

    // for multiple cpu sanity check
    if(hdim->GetXaxis()->GetNbins()%_ncpu!=0){
      cout<<"DRecoSimulChi2Var::evaluatePartition _nCPU must be a divisor of NbinsX()"<<endl;
      exit(0);
    }    

    //cout<<" 4 "<<endl;
    while(TH1*h=(TH1*)_HistsIter->Next()){
      RooRealVar*xvar=(RooRealVar*)_VarsIter->Next();
      RooAbsPdf*pdf=(RooAbsPdf*)_PdfsIter->Next();
      Double_t nDataTotal=h->Integral();      
      RooArgSet normset(*xvar);
      //cout<<" 5 "<<endl;    

      //add chisquare from this histogram
      //for(Int_t i=1;i<=h->GetXaxis()->GetNbins();i++){
      for(Int_t i=firstEvent+1;i<=lastEvent;i++){
	Double_t nData =h->GetBinContent(i);
	Double_t eInt =h->GetBinError(i);
      
	//cout<<" 6 "<<endl;
	Double_t origval=xvar->getVal();
	xvar->setVal(h->GetBinCenter(i));
	Double_t nPdf=pdf->getVal(&normset)*h->GetBinWidth(i)*nDataTotal;
      
	//cout<<" 7 "<<endl;
	//for histogram pdfs account for statistical errors
	Double_t nPdfErr=0.;	
	if(!strcmp(pdf->ClassName(),"DRecoAddPdf")) nPdfErr=nPdf*(((DRecoAddPdf*)pdf)->getHistError());
	if(!strcmp(pdf->ClassName(),"DRecoHistPdf")) nPdfErr=nPdf*(((DRecoHistPdf*)pdf)->getHistError());
	if(!strcmp(pdf->ClassName(),"DRecoProdPdf")) nPdfErr=nPdf*(((DRecoProdPdf*)pdf)->getHistError());
	if(!strcmp(pdf->ClassName(),"DRecoTranslationPdf")) nPdfErr=nPdf*(((DRecoTranslationPdf*)pdf)->getHistError());
   
	//cout<<" 8 "<<endl;
	//return original quickly so it does not interfere with multiple cpus
	//Note that this is method of splitting the chisquare into ncpus can in principle cause unpredictable behaviour because if two 
	//cpus will change the x variable at the same time then one pdf does not get evaluated at the desired value!!!!!!
      	xvar->setVal(origval);
      
	//cout<<" 9 "<<endl;
	if ((nPdf==0.&&nData==0.)||eInt<=0.) continue ;
	
	Double_t eExt = nPdf-nData ;
	//result += eExt*eExt/(eInt*eInt) ;
      
	//cout<<"hist="<<nData<<" hist error="<<eInt<<"  pdf="<<nPdf<<"  pdferr="<<nPdfErr<<"  pdferr/nPdf="<<nPdfErr/nPdf<<endl;
	result += eExt*eExt/(eInt*eInt+nPdfErr*nPdfErr) ;
      
	//cout<<" 10 "<<endl;
      }      
    }

  }else if(hdim->GetDimension()==2){///2D Histograms
  
    if(_Vars->GetSize()!=2*_Hists->GetSize()){ 
      cout<<"DRecoSimulChi2Var::evaluatePartition number of variables does not match"<<endl;
      exit(0);
    }

    //!!!!!!!!!!!!!!!Warning
    //Multiple cpus do not work: they increase the chisquare?? probably because they interfere with each other
    //only up to 2 cpus seem to work.
    // for multiple cpu sanity check
    if(((hdim->GetXaxis()->GetNbins())*(hdim->GetYaxis()->GetNbins()))%_ncpu != 0){
      cout<<"DRecoSimulChi2Var::evaluatePartition _nCPU must be a divisor of NbinsY()*NbinsX()"<<endl;
      exit(0);
    }  
    ///scale up to cover all histograms: original firstEvent/lastEvent are based on one histogram
    firstEvent=firstEvent*_Hists->GetSize()+1;
    lastEvent=lastEvent*_Hists->GetSize()+1;
    Int_t counter=0;
    

    while(TH1*h=(TH1*)_HistsIter->Next()){
      RooRealVar*xvar=(RooRealVar*)_VarsIter->Next();
      RooRealVar*yvar=(RooRealVar*)_VarsIter->Next();
      RooAbsPdf*pdf=(RooAbsPdf*)_PdfsIter->Next();
      Double_t nDataTotal=h->Integral();
      RooArgSet normset(*xvar);
      normset.add(*yvar);

      TAxis* Xaxis=h->GetXaxis();
      TAxis* Yaxis=h->GetYaxis();

      //add chisquare from this histogram
      for(Int_t i=1;i<=Xaxis->GetNbins();i++){	
	for(Int_t j=1;j<=Yaxis->GetNbins();j++){
	  counter++;
	  if(counter<firstEvent)continue;
	  if(counter==lastEvent)break;
	  
	  Double_t nData =h->GetBinContent(i,j);
	  Double_t eInt =h->GetBinError(i,j);     

	  Double_t origvalx=xvar->getVal();
	  Double_t origvaly=yvar->getVal();	
	  xvar->setVal(Xaxis->GetBinCenter(i));
	  yvar->setVal(Yaxis->GetBinCenter(j));
	  Double_t nPdf =pdf->getVal(&normset)*Xaxis->GetBinWidth(i)*Yaxis->GetBinWidth(j)*nDataTotal;     
      
	  //for histogram pdfs account for statistical errors
	  Double_t nPdfErr=0.;
	  if(!strcmp(pdf->ClassName(),"DRecoAddPdf")) nPdfErr=nPdf*(((DRecoAddPdf*)pdf)->getHistError());
	  if(!strcmp(pdf->ClassName(),"DRecoHistPdf")) nPdfErr=nPdf*(((DRecoHistPdf*)pdf)->getHistError());
	  if(!strcmp(pdf->ClassName(),"DRecoProdPdf")) nPdfErr=nPdf*(((DRecoProdPdf*)pdf)->getHistError());
	  if(!strcmp(pdf->ClassName(),"DRecoTranslationPdf")) nPdfErr=nPdf*(((DRecoTranslationPdf*)pdf)->getHistError());
    
	  xvar->setVal(origvalx);
	  yvar->setVal(origvaly); 

      
	  if ((nPdf==0.&&nData==0.)||(eInt*eInt+nPdfErr*nPdfErr)==0.) continue ;
	
	  Double_t eExt = nPdf-nData ;     
	  result += eExt*eExt/(eInt*eInt+nPdfErr*nPdfErr) ;

	  //cout<<counter<<" hist="<<nData<<" hist error="<<eInt<<"  pdf="<<nPdf<<"  pdferr="<<nPdfErr<<"  pdferr/nPdf="<<nPdfErr/nPdf<<" chisq="<<result<<endl;
	}

	if(counter==lastEvent)break;
      }


 
    }

  }else cout<<"DRecoSimulChi2Var::evaluatePartition  Bad histo dimension"<<endl;
   

  return result ;

}





TGraph* DRecoSimulChi2Var::ScanParameter(RooRealVar* Par){

  if(!Par){
    cout<<"Null input Par"<<endl;
    return 0;
  }

  if(Par->isConstant()){
    cout<<"Parameter:"<<Par->GetName()<<"  is constant"<<endl;
    return 0;
  }
    
  

  //scan parameter from to max
  Int_t Npoints=50;
  TGraph G(Npoints);
  Float_t parmin=Par->getVal()-5*Par->getError();
  Float_t parmax=Par->getVal()+5*Par->getError();
  if(parmin<Par->getMin())parmin=Par->getMin();
  if(parmax>Par->getMax())parmax=Par->getMax();
  Float_t parstep=(parmax-parmin)/Npoints;
  Float_t parinit=Par->getVal();

  for(Int_t i=0;i<Npoints;i++){
    
    Par->setVal(parmin+i*parstep);
    G.SetPoint(i,Par->getVal(),getVal());
  }

  Par->setVal(parinit);
  G.SetTitle("");
  G.GetYaxis()->SetTitle("#chi^{2}");
  G.GetXaxis()->SetTitle(Par->GetName());
  return (TGraph*)G.Clone();
}

TH2F* DRecoSimulChi2Var::ScanParameter2D(RooRealVar* Par1,RooRealVar* Par2){
 

  if(!Par1||!Par2){
    cout<<"Parameter(s) not found."<<endl;
    return 0;
  }

  if(Par1->isConstant()||Par2->isConstant()){
    cout<<"Parameter(s) is constant"<<endl;
    return 0;
  }
    
  Float_t par1init=Par1->getVal();
  Float_t par2init=Par2->getVal();  

  Int_t nbinsx=20;
  Int_t nbinsy=20;
  Float_t par1min=Par1->getVal()-5*Par1->getError();
  Float_t par1max=Par1->getVal()+5*Par1->getError();
  if(par1min<Par1->getMin())par1min=Par1->getMin();
  if(par1max>Par1->getMax())par1max=Par1->getMax();

  Float_t par2min=Par2->getVal()-5*Par2->getError();
  Float_t par2max=Par2->getVal()+5*Par2->getError();
  if(par2min<Par2->getMin())par2min=Par2->getMin();
  if(par2max>Par2->getMax())par2max=Par2->getMax();

  TH2F* H2=new TH2F(TString(GetName())+Par1->GetName()+Par2->GetName()+"_Scan","",
		    nbinsx,par1min,par1max,
		    nbinsy,par2min,par2max);
  

  for(Int_t i=1;i<=H2->GetXaxis()->GetNbins();i++){    
    Par1->setVal(H2->GetXaxis()->GetBinCenter(i));

    for(Int_t j=1;j<=H2->GetYaxis()->GetNbins();j++){      
      Par2->setVal(H2->GetYaxis()->GetBinCenter(j));

      H2->SetBinContent(i,j,1./getVal());
    }
  }

  H2->GetXaxis()->SetTitle(Par1->GetName());
  H2->GetYaxis()->SetTitle(Par2->GetName());
  H2->GetZaxis()->SetTitle("1/#chi^{2}");
  H2->SetStats(0);

  Par1->setVal(par1init);
  Par2->setVal(par2init);
  
  return H2;
}
