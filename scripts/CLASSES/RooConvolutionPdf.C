
#include "RooConvolutionPdf.h"
#include "RooRealVar.h"

ClassImp(RooConvolutionPdf)

RooConvolutionPdf::RooConvolutionPdf(const char *name, const char *title,
				     RooRealVar* Var, RooAbsPdf* pdf,
				     RooRealVar* resVar, RooAbsPdf* respdf) : 
  RooAbsPdf(name,title), 
  _Var(Var),
  _Pdf(pdf),
  _resVar(resVar),
  _resPdf(respdf),
  x("x","Dependent",this,*Var),
  _origpdf("_origpdf","_origpdf",this,*pdf)
{  
 
  if(_resVar->getBins()<10){cout<<"RooConvolutionPdf:: Warning !!!! resVar has less than 10 bins ???"<<endl;}
}

RooConvolutionPdf::RooConvolutionPdf(const RooConvolutionPdf& other, const char* name) :
  RooAbsPdf(other,name), 
  _Var(other._Var),
  _Pdf(other._Pdf),
  _resVar(other._resVar),
  _resPdf(other._resPdf),
  resvarstep(other.resvarstep),
  varstep(other.varstep),  
  x("x",this,other.x),
  _origpdf("_origpdf",this,other._origpdf)
{
  
  if(!_Var||!_Pdf||!_resVar||!_resPdf) return;
}


Double_t RooConvolutionPdf::evaluate() const 
{
  if(!_Var||!_Pdf||!_resVar||!_resPdf)return 0;  

  Float_t centralvalue=x;//x will change when _Var is changed
  Float_t Integ=0;
  Float_t dummy=0;
  RooArgSet pdfnset(*_Var);
  RooArgSet respdfnset(*_resVar);

  dummy=_resVar->getMin();
  for(Int_t j=1;j<=_resVar->getBins();j++){
    dummy+=.5*_resVar->getBinWidth(j);
    
    Float_t originalval=_Var->getVal();
    //edge depletion problem: "fix" by setting _Pdf constant outside limits
    if(centralvalue-dummy<_Var->getMin())
      _Var->setVal(_Var->getMin());// Note that for centralvalue-dummy<_Var->getMin() _Var will round up and produce cout
    else if(centralvalue-dummy>_Var->getMax())
      _Var->setVal(_Var->getMax());
    else 
      _Var->setVal(centralvalue-dummy);     	     
    
    Float_t originalresval=_resVar->getVal();  
    _resVar->setVal(dummy);
    Integ+=_Pdf->getVal(&pdfnset)*_resPdf->getVal(&respdfnset)*_resVar->getBinWidth(j);  
    
    //must return _Var to its original value immidiately, otherwise will interfere with outside operations
    _Var->setVal(originalval);
    _resVar->setVal(originalresval); 	
    
    dummy+=.5*_resVar->getBinWidth(j);
  } 
  
  return Integ;
}


// rescached(0),
// rescached(other.rescached),

//   if(other.rescached){
//     for(Int_t i=0;i<NRESCACHEBINS;i++){
//       respdfCacheNorm[i]=other.respdfCacheNorm[i];
//       for(Int_t j=0;j<NRESCACHEBINS;j++){            
// 	respdfCache[i][j]=other.respdfCache[i][j];      
//       }
//     }
//   }

// void RooConvolutionPdf::CacheResolution(){

//   if(!_Var||!_Pdf||!_resVar||!_resPdf) return;
//   cout<<"RooConvolutionPdf: will cache the resolution function in an array of "<<NRESCACHEBINS<<" x "<<NRESCACHEBINS<<endl;
//   cout<<"resolution parameters must not be modified"<<endl;

//   Float_t originalval=_Var->getVal();
//   Float_t originalresval=_resVar->getVal();


//   RooArgSet nset(*_resVar);
//   resvarstep=(_resVar->getMax()-_resVar->getMin())/NRESCACHEBINS;
//   varstep=(_Var->getMax()-_Var->getMin())/NRESCACHEBINS;
//   for(Int_t i=0;i<NRESCACHEBINS;i++){
//     _Var->setVal(_Var->getMin()+varstep*(i+.5));
    
//     respdfCacheNorm[i]=0;
//     for(Int_t j=0;j<NRESCACHEBINS;j++){            
//       _resVar->setVal(_resVar->getMin()+resvarstep*(j+.5));      
//       respdfCache[i][j]=_resPdf->getVal(&nset);
//       respdfCacheNorm[i]+=respdfCache[i][j];      
//     }
//     respdfCacheNorm[i]*=resvarstep;
//     //cout<<i<<" "<<respdfCacheNorm[i]<<endl;
//   }
    
//   _Var->setVal(originalval);
//   _resVar->setVal(originalresval);
//   rescached=1;
// }

//   if(rescached){
//     Int_t i=0;
//     for(Int_t j=0;j<NRESCACHEBINS;j++){
//       dummy=_resVar->getMin()+(j+.5)*resvarstep;

//       Float_t originalval=_Var->getVal();
//       //edge depletion problem: "fix" by setting _Pdf constant outside limits
//       if(centralvalue-dummy<_Var->getMin())
// 	_Var->setVal(_Var->getMin());// Note that for centralvalue-dummy<_Var->getMin() _Var will round up and produce cout
//       else if(centralvalue-dummy>_Var->getMax())
// 	_Var->setVal(_Var->getMax());
//       else 
// 	_Var->setVal(centralvalue-dummy);     	     


//       i=(int)((_Var->getVal()-_Var->getMin())/varstep);//no rouding should be done  
//       if(i<0||i>=NRESCACHEBINS)return 0;    
//       Integ+=resvarstep*_Pdf->getVal(&pdfnset)*respdfCache[i][j]/respdfCacheNorm[i];  

//       //must return _Var to its original value immidiately, otherwise will interfere with outside operations
//       _Var->setVal(originalval);
//     }
//   }else {  
    
//     dummy=_resVar->getMin();
//     for(Int_t j=1;j<=_resVar->getBins();j++){
//       dummy+=.5*_resVar->getBinWidth(j);

//       Float_t originalval=_Var->getVal();
//       //edge depletion problem: "fix" by setting _Pdf constant outside limits
//       if(centralvalue-dummy<_Var->getMin())
// 	_Var->setVal(_Var->getMin());// Note that for centralvalue-dummy<_Var->getMin() _Var will round up and produce cout
//       else if(centralvalue-dummy>_Var->getMax())
// 	_Var->setVal(_Var->getMax());
//       else 
// 	_Var->setVal(centralvalue-dummy);     	     
    
//       Float_t originalresval=_resVar->getVal();  
//       _resVar->setVal(dummy);
//       Integ+=_Pdf->getVal(&pdfnset)*_resPdf->getVal(&respdfnset)*_resVar->getBinWidth(j);  

//       //must return _Var to its original value immidiately, otherwise will interfere with outside operations
//       _Var->setVal(originalval);
//       _resVar->setVal(originalresval); 	

//       dummy+=.5*_resVar->getBinWidth(j);
//     } 
    
    
//   }


//   dummy=centralvalue+_resVar->getMin();//runs from x-q --> x+q
//   for(Int_t j=1;j<=_resVar->getBins();j++){
//     dummy+=.5*_resVar->getBinWidth(j);

//     Float_t originalval=_Var->getVal();
//     //edge depletion problem: "fix" by setting _Pdf constant outside limits
//     if(dummy<_Var->getMin())
//       _Var->setVal(_Var->getMin());// Note that for centralvalue-dummy<_Var->getMin() _Var will round up and produce cout
//     else if(dummy>_Var->getMax())
//       _Var->setVal(_Var->getMax());
//     else 
//       _Var->setVal(dummy);     	     
    
//     Float_t originalresval=_resVar->getVal();  
//     _resVar->setVal(centralvalue-dummy);

//     //evaluate Integrand
//     Double_t integrand=_Pdf->getVal(&pdfnset)*_resPdf->getVal(&respdfnset);
//     Integ+=integrand*_resVar->getBinWidth(j);  

//     //must return _Var to its original value immidiately, otherwise will interfere with outside operations
//     _Var->setVal(originalval);
//     _resVar->setVal(originalresval); 	

//     dummy+=.5*_resVar->getBinWidth(j);
//   } 
