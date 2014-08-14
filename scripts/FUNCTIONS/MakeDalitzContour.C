#include "MakeDalitzContour.h" 

TGraph* MakeDalitzContour(Float_t M,Float_t m1,Float_t m2,Float_t m3,Int_t Npoints){

  TGraph* Contour=new TGraph(2*Npoints+1); 
  
  Double_t x=0;
  Double_t y=0;
  Float_t xmin=(m1+m2)*(m1+m2);
  Float_t xmax=(M-m3)*(M-m3);
  Float_t xstep=(xmax-xmin)/Npoints;
  Float_t x0=0;
  Float_t y0=0;

  Float_t E2,E3;
  
  //First do the top points from left to right
  for(Int_t p=0;p<Npoints;p++){
    x=xmin+p*xstep;
    E2=(x-m1*m1+m2*m2)/(2*sqrt(x));
    E3=(M*M-x-m3*m3)/(2*sqrt(x));
    y=(E2+E3)*(E2+E3)-pow((sqrt(E2*E2-m2*m2)-sqrt(E3*E3-m3*m3)),2);      
    Contour->SetPoint(p,x,y);
    if(p==0){x0=x;y0=y;}
  }

  //Now do the bottom points from right to left
  for(Int_t p=0;p<Npoints;p++){
    x=xmax-p*xstep;//move backwards to draw with line   
    E2=(x-m1*m1+m2*m2)/(2*sqrt(x));
    E3=(M*M-x-m3*m3)/(2*sqrt(x));
    y=(E2+E3)*(E2+E3)-pow((sqrt(E2*E2-m2*m2)+sqrt(E3*E3-m3*m3)),2);  
    Contour->SetPoint(p+Npoints,x,y);    
  }
  
  //join the ends, so you and draw with a lines and there will be no gap
  Contour->SetPoint(2*Npoints,x0,y0); 


  return Contour;
}
