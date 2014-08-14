#include "Project2DHistoLegendre.h"
#include "Legendre.h"
#include <TString.h>
     
TH1* project2DHistoLegendre(TH2* h2,int l){
  if(!h2 || l<0)return NULL;

  //the projection will be on to the X axis
  //Also: The the signal is defined as  I(x,m)= A(m)*I(x)  where I(x)=sum(a_l * P_l(x))
  //where I(x) is normalized: integral(I(x)*dx,-1,1)=1   

  //get range binning
  int nbinsx=h2->GetXaxis()->GetNbins();
  int nbinsy=h2->GetYaxis()->GetNbins();
  float xmin=h2->GetXaxis()->GetXmin();
  float xmax=h2->GetXaxis()->GetXmax();

  //create a new histogram
  TH1F* h=new TH1F(TString(h2->GetName())+"_LegendreProj"+(long)l,TString(h2->GetName())+"_LegendreProj"+(long)l,nbinsx,xmin,xmax);
  float bincont=0;
  float polval=0;
  float add=0;
  float erradd=0;

  for(int bx=1;bx<=nbinsx;bx++){

    add=0;
    erradd=0;
    //perform the integral over angle: integral( ((2.*l+1.)/2.) * P_l(x)*I(x)*dx)
    for(int by=1;by<=nbinsy;by++){
      bincont=h2->GetBinContent(bx,by);//this equals I(m,x)*dm*dx= A(m)*dm*I(x)*dx
      polval=legendre(l,h2->GetYaxis()->GetBinCenter(by))*(2.*l+1.)/2.;//this equals P_l(x)
      add+=bincont*polval;
      erradd+= pow(h2->GetBinError(bx,by)*polval,2);
    }

    h->SetBinContent(bx,add);//fill with  A(m)*dm*a_l
    h->SetBinError(bx,sqrt(erradd));//error is error of each integral piece added in quadrature 
  }

  return (TH1*)h;
}
