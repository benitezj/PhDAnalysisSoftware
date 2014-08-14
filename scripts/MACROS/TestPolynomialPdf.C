#include "RooRealVar.h"
#include "../CLASSES/RooPolynomialFixEdge.h"
#include "../CLASSES/DstPi/DstPiAnalysis.h"
#include "RooPlot.h"
#include  <TCanvas.h>

void testfixpol(Int_t nbins,Int_t norm,Float_t _c3,Float_t _c4,Float_t _c5){

  Float_t _x0=-10;
  Float_t _x1=10;
  Float_t binwidth=(_x1-_x0)/nbins;
  RooRealVar x("x","x",_x0,_x1);
  x.setBins(nbins);
  

  RooRealVar x0("x0","x0",-10);
  RooRealVar y0("y0","y0",10/binwidth);

  RooRealVar x1("x1","x1",10);
  RooRealVar y1("y1","y1",20/binwidth);

  RooRealVar c3("c3","c3",_c3);
  RooRealVar c4("c4","c4",_c4);
  RooRealVar c5("c5","c5",_c5);
  RooRealVar yield("yield","yield",norm);

  RooPolynomialFixEdge* pol=new RooPolynomialFixEdge("pol","pol",x,x0,y0,x1,y1,yield,c3,c4,c5);

  x.setVal(_x0);
  cout<<"x=x0="<<x.getVal()<<" y0="<<y0.getVal()<<" pdf="<<pol->evaluate()<<endl;
  x.setVal(_x1);
  cout<<"x=x1="<<x.getVal()<<" y1="<<y1.getVal()<<" pdf="<<pol->evaluate()<<endl;


  RooPlot* f=x.frame();
  if(!f)return; 
  pol->plotOn(f,Normalization(yield.getVal()));
  //TCanvas* C=new TCanvas("C","C");
  f->Draw();
 
  delete pol;
  //delete f;
}
