#include "RooRealVar.h"
#include "../CLASSES/RooNRBreitWigner.h"
#include "RooGenericPdf.h"
#include "RooPlot.h"
#include "RooArgSet.h"
#include "../CLASSES/RooConvolutionPdf.h"
#include "RooNumConvPdf.h"
#include "RooFit.h"
#include "RooAddPdf.h"
#include "RooGlobalFunc.h"
#include <TCanvas.h>


void testRooConvolutionPdf(Float_t width,Float_t resol){
 

  RooRealVar x("x","x",-.1,.1);
  RooRealVar m("m","m",0);
  RooRealVar w("w","w",width);
  //RooBreitWigner bw("bw","bw",x,m,w);
  RooNRBreitWigner bw("bw","bw",&x,m,w);

  RooRealVar ResVar("ResVar","ResVar",-10*resol,10*resol);
  RooRealVar ResSigma("ResSigma","ResSigma",resol);
  RooGenericPdf respdf("respdf","respdf","exp(-.5*ResVar**2/ResSigma**2)",RooArgSet(ResVar,ResSigma));
  
  ///////////////To use a resolution sigma which is a fuction of x do something like this:
  //RooRealVar ResVarMin("ResVarMin","ResVarMin",ResVar.getMin());
  //RooRealVar ResVarMax("ResVarMax","ResVarMax",ResVar.getMax());
  //RooGenericPdf respdf("respdf","respdf","exp(-.5*ResVar**2/(ResSigma/2.+ResSigma*(x-ResVarMin)/(ResVarMax-ResVarMin))**2)",RooArgSet(ResVar,ResSigma,x,ResVarMin,ResVarMax));  

  RooConvolutionPdf myconv("myconv","myconv",&x,&bw,&ResVar,&respdf);
  
  //Test by difference with unconvolved pdf
  RooRealVar p0("p0","p0",1);
  RooRealVar p1("p1","p1",-1);
  RooRealVar p2("p2","p2",1);
  RooGenericPdf offset("offset","offset","1",RooArgSet(x)); 
  RooAddPdf myadd("myadd","myadd",RooArgList(offset,myconv,bw),RooArgList(p0,p1,p2));


  //Test by difference with RooNumConv
  RooGenericPdf gauspdf("gauspdf","gauspdf","exp(-.5*x**2/ResSigma**2)",RooArgSet(x,ResSigma));//
  RooNumConvPdf gausconv("gausconv","gausconv",x,bw,gauspdf);
  RooRealVar ResMid("ResMid","ResMid",0);
  gausconv.setConvolutionWindow(ResMid,ResSigma,10);

  RooAddPdf gausadd("gausadd","gausadd",RooArgList(offset,gausconv,bw),RooArgList(p0,p1,p2)); 
  RooAddPdf myadd2("myadd2","myadd2",RooArgList(offset,myconv,gausconv),RooArgList(p0,p1,p2));



  //Test numerically
  //test integral
  RooArgSet nset(x);

  x.setVal(0);
  cout<<" bw="<<bw.getVal(&nset)<<endl;
  cout<<" myconv="<<myconv.getVal()<<endl;
  cout<<" RooNumConv="<<gausconv.getVal(&nset)<<endl;

 

  Float_t integ=0;
  Float_t integnum=0;
  Float_t integbw=0;
  Float_t step=(x.getMax()-x.getMin())/100.;
 
  for(Int_t i=0;i<100;i++){
    x.setVal(x.getMin()+((float)i+.5)*step);
    integ+=myconv.getVal()*step;
    integnum+=gausconv.getVal(&nset)*step;
    integbw+=bw.getVal(&nset)*step;
  }

  cout<<" bw Integral="<< integbw<<endl;
  cout<<" myconv Integral="<< integ<<endl;
  cout<<" RooNumConv Integral="<< integnum<<endl;
    

 
 

  TCanvas*C=new TCanvas("C","C");
  C->Divide(2,2);


  C->cd(1);
  C->GetPad(1)->SetLogy(1);
  RooPlot*pl=x.frame();
  pl->SetTitle("BW and My convolution");
  bw.plotOn(pl,RooFit::LineColor(1));
  myconv.plotOn(pl,RooFit::LineColor(2));
  pl->Draw();
  

  C->cd(2);
  C->GetPad(2)->SetLogy(1);
  RooPlot*pl2=x.frame();
  pl2->SetTitle("BW and Gaus convolution");
  bw.plotOn(pl2,RooFit::LineColor(1));
  gausconv.plotOn(pl2,RooFit::LineColor(2));
  pl2->Draw();
  


  
  C->cd(3);
  //C->GetPad(3)->SetLogy(1);
  RooPlot*pl3=x.frame();
  //pl3->SetTitle("My convolution and Gauss Convolution");
  //myconv.plotOn(pl3,RooFit::LineColor(1));
  //gausconv.plotOn(pl3,RooFit::LineColor(2));
  myadd.plotOn(pl3,RooFit::LineColor(1));
  offset.plotOn(pl3,RooFit::LineColor(4));
  gausadd.plotOn(pl3,RooFit::LineColor(2));
  pl3->Draw();


  C->cd(4);
  //C->GetPad(3)->SetLogy(1);
  RooPlot*pl4=x.frame();
  myadd2.plotOn(pl4,RooFit::LineColor(1));
  offset.plotOn(pl4,RooFit::LineColor(4));
  pl4->Draw();
  
}
