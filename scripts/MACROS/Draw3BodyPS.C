//This Macro computes the 3-Body phase space as a function of the mass of the Mother mass (M)
//Then it makes a fits with a 3rd order polynomial.
//It shows a plot of the integration region as a function of M. The phase space is equal the area of this region.

#include "../Globals.h"
#include "../AnalysisGlobals.h"
#include <TGraphErrors.h>

void  Draw3BodyPS(Float_t m1,Float_t m2,Float_t m3,Float_t Mmin,Float_t Mmax,Int_t nMpoints){
  
  Int_t nintpoints=500;
  Double_t M;
  Double_t MassMin=m1+m2+m3;
  if(Mmin>MassMin)MassMin=Mmin;
  //Double_t Mmax=MassMin+dM;
  Double_t Mstep=(Mmax-MassMin)/nMpoints;
  

  TGraphErrors GPS;
  TGraphErrors Gdomain0;Int_t N1=(int)(.4*nMpoints);
  TGraphErrors Gdomain1;Int_t N2=(int)(.5*nMpoints);
  TGraphErrors Gdomain2;Int_t N3=(int)(.6*nMpoints);

  Float_t M1=MassMin+.4*(Mmax-MassMin);
  Float_t M2=MassMin+.5*(Mmax-MassMin);
  Float_t M3=MassMin+.6*(Mmax-MassMin);

  Double_t x;
  Double_t nxpoints=nintpoints; 
  Double_t nypoints=nintpoints;
  Double_t y; 
  Double_t f;

  for(Int_t nm=0;nm<=nMpoints;nm++){
    M=MassMin+nm*Mstep;
    Double_t r1=m1/M;
    Double_t r2=m2/M;
    Double_t r3=m3/M;
    Double_t rbar=.5*(1+r3*r3-r1*r1-r2*r2);
    Double_t xmin=r1;
    Double_t xmax=1-r2-r3;//this is probably to large; does not impose conservation of momentum: This is isnt really working for large M integrals two discrete integration regions are seen!
    Double_t xstep=(xmax-xmin)/nxpoints;
    Double_t ymin=r2;
    Double_t ymax=1-r1-r3; 
    Double_t ystep=(ymax-ymin)/nypoints;
    Double_t integral=0;
        
    Int_t ninsidepoints=0;

    for(Int_t nx=1;nx<=nxpoints;nx++){
      x=xmin+nx*xstep;//energy of M1
      for(Int_t ny=1;ny<=nypoints;ny++){
	y=ymin+ny*ystep;//energy of M2
	
	//check if (x,y) are in the integral limits
	f=((x-1)*(y-1)-rbar)/(sqrt(x*x-r1*r1)*sqrt(y*y-r2*r2));      
	if(-1<f&&f<1){	 
	  integral+=xstep*ystep;
	  if(nm==N1)Gdomain0.SetPoint(ninsidepoints,x,y);
	  if(nm==N2)Gdomain1.SetPoint(ninsidepoints,x,y);
	  if(nm==N3)Gdomain2.SetPoint(ninsidepoints,x,y);
	  ninsidepoints++;
	}
      }
    }
    GPS.SetPoint(nm,M,M*M*integral);
  }
  
  //scale the graph to unity at highest point  
  //Double_t* y=GPS.GetY();
  Double_t& u=x;
  Double_t& v=y;
  GPS.GetPoint(nMpoints,u,v);
  Double_t norm=y;
  for(Int_t n=0;n<=nMpoints;n++){
    GPS.GetPoint(n,u,v);
    GPS.SetPoint(n,x,y/norm);
    GPS.SetPointError(n,0,.01*y/norm);
  }
    

  TF1 F3bodyps("F3bodyps","[0]+[1]*x+[2]*x**2+[3]*x**3+[4]*x**4",MassMin+(Mmax-MassMin)*.05,Mmax);
  F3bodyps.SetLineWidth(2);
  F3bodyps.SetLineColor(2);
  F3bodyps.SetNpx(10000);
  F3bodyps.SetParLimits(0,-10,20);
  F3bodyps.SetParLimits(1,-10,20);
  F3bodyps.SetParLimits(2,-10,20);
  F3bodyps.SetParLimits(3,-10,20);
  F3bodyps.SetParLimits(4,-10,20);
  F3bodyps.SetParameters(1,2,3,4,5);  
  GPS.Fit(&F3bodyps,"R");
  
 
  TCanvas C("C3bodyps","C3bodyps");
  C.Print("C3body.ps[");

  C.Clear();
  //C.Divide(1,2);
  //C.cd(1);
  char txt[100];
  sprintf(txt,"m1=%.3f  m2=%.3f  m3=%.3f",m1,m2,m3);
  GPS.SetTitle(txt);
  GPS.GetXaxis()->SetTitle("M GeV/c^{2}");
  GPS.GetYaxis()->SetTitle("3-Body Phase Space");
  GPS.Draw("ape");
  C.Update();
  TPaveStats*st=(TPaveStats*)GPS.FindObject("stats");
  st->SetX1NDC(.15);
  st->SetX2NDC(.45);
  F3bodyps.Draw("same");
  //C.GetPad(1)->SetLogy(1);
  C.Print("C3body.ps");
  C.SetLogy(1);
  st->SetX1NDC(.45);
  st->SetX2NDC(.75);
  st->SetY1NDC(.2);
  st->SetY2NDC(.6);
  C.Print("C3body.ps");
  
  C.Clear();
  C.SetLogy(0); 
  //C.cd(2);
  char tempstring[100];
  sprintf(tempstring,"3 consecutive M: %.3f, %.3f, %.3f",M1,M2,M3);
  Gdomain2.SetTitle(tempstring);
  Gdomain2.GetXaxis()->SetTitle("E_{1}/M");
  Gdomain2.GetYaxis()->SetTitle("E_{2}/M");
  Gdomain2.Draw("ap");
  Gdomain1.SetMarkerColor(2);
  Gdomain1.Draw("psame");
  Gdomain0.SetMarkerColor(3);
  Gdomain0.Draw("psame");
  C.Update();
  C.Print("C3body.ps");


  C.Print("C3body.ps]");
 
 // C.Print("ThreeBodyPhaseSpce.gif");



  // printf("3-body phase space for \n m1 = %.3f m2 = %.3f m3 = %.3f is \n  %.3e + %.3e*x + %.3e*x**2 + %.3e*x**3 \n ",m1,m2,m3,F3bodyps.GetParameter(0),F3bodyps.GetParameter(1),F3bodyps.GetParameter(2),F3bodyps.GetParameter(3));

  return;
}
