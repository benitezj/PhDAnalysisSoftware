#include "../Globals.h"
#include "RooAbsPdf.h"
#include "RooRealVar.h"

void DetermineFWHM(TF1* F,Float_t* fwhm,Float_t* mean,Float_t* xofymax){
  Int_t nbins=100000;
  Float_t xmaximum=F->GetXmin();
  Float_t xlow=F->GetXmin();
  Float_t xhigh=F->GetXmax();  
  Float_t xstep=(xhigh-xlow)/nbins;
  Float_t xmean=F->GetXmin();
  Float_t xfwhm=0;
  Float_t ymaximum=0;

  Float_t fvalue=0;
  Float_t x=0;
  
  Float_t tolerance=F->GetMaximum(xlow,xhigh)/1000;
  
  Float_t weightsum=0;
  Int_t npoints=0;
  //make first sweep to determine the mean and maximum
  for(Int_t b=1;b<nbins;b++){
    x=xlow+b*xstep;
    fvalue=F->Eval(x);

    //compute mean
    xmean+=x*fvalue;
    npoints++;
    weightsum+=fvalue;

    //find maximumn
    if(fvalue > F->Eval(xlow+(b-1)*xstep)){
      xmaximum=x;
      ymaximum=fvalue;
      //tolerance=2*fvalue/nbins;
    }
    
    //now determine the fwhm
    if(ymaximum>0 && fabs(fvalue-ymaximum/2.)<tolerance)
      xfwhm=x;
    
    //cout<<xmaximum<<" "<<ymaximum<<endl;
  }
  
  //compute mean
  xmean=xmean/weightsum;
  
  if(xfwhm==0||ymaximum==0)cout<<"No FWHM found"<<endl;


  *fwhm=2*(xfwhm-xmaximum);
  *mean=xmean;
  *xofymax=xmaximum;

  return;
}



void DetermineFWHM(RooAbsPdf* F,RooRealVar* var,Float_t* fwhm,Float_t* mean,Float_t* xofymax){

  if(!F||!var)return;

  Int_t nbins=10000;
  Float_t xmaximum=var->getMin();//initial value
  Float_t xlow=var->getMin();
  Float_t xhigh=var->getMax();  
  Float_t xstep=(xhigh-xlow)/nbins;
  Float_t xmean=var->getMin();
  Float_t xfwhm=0;
  Float_t ymaximum=0;

  Float_t fvalue=0;
  Float_t x=0;
  
  Float_t tolerance=1;
  
  Float_t weightsum=0;
  Int_t npoints=0;
  Float_t prevfval=0;
  //make first sweep to determine the mean and maximum
  for(Int_t b=1;b<nbins;b++){
    x=xlow+b*xstep;
    var->setVal(x);
    fvalue=F->getVal(new RooArgSet(*var));//

    //compute mean
    xmean+=x*fvalue;
    npoints++;
    weightsum+=fvalue;

    //find maximumn
    if(fvalue > prevfval){//
      xmaximum=x;
      ymaximum=fvalue;     
    }
    
    //now determine the fwhm
    tolerance=ymaximum/100.;
    if(ymaximum>0 && fabs(fvalue-ymaximum/2.)<tolerance)
      xfwhm=x;
       
    //cout<<b<<" "<<x<<" "<<fvalue<<" "<<prevfval<<" "<<xmaximum<<" "<<ymaximum<<endl;

    prevfval=fvalue;
  }
  
  //compute mean
  xmean=xmean/weightsum;
  
  if(xfwhm==0||ymaximum==0)cout<<"No FWHM found"<<endl;


  *fwhm=2*(xfwhm-xmaximum);
  if(*fwhm<0)*fwhm=0;
  *mean=xmean;
  *xofymax=xmaximum;

  return;
}


