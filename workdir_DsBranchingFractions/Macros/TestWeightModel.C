#include "../tmp/Linux26SL5_i386_gcc412/DRecoilToolsUser/NtupleCode/DRecoEvtAnalysis.hh"
#define NXT 6
#include "RooRealVar.h"
#include "RooArgSet.h"
#include "RooGenericPdf.h"
#include "RooRealIntegral.h"
#include "RooPlot.h"
#include "TH1F.h"

void TestWeightModel(){
  RooRealVar nxt("nxt","nxt",-.5,NXT-.5);
  nxt.setBins(NXT);

  RooArgSet nset(nxt);

  RooRealVar mean1("mean1","mean1",1,-.5,NXT-.5);
  RooRealVar sigma1("sigma1","sigma1",2,0,NXT);
  RooRealVar frac("frac","frac",.5,0.,1.);
  RooRealVar mean2("mean2","mean2",3,-.5,NXT-.5);
  RooRealVar sigma2("sigma2","sigma2",2,0,NXT);

  RooGenericPdf weihtPdf("weihtPdf","weihtPdf","frac*exp(-.5*(nxt-mean1)**2/sigma1**2)/(sigma1*2.5066)+(1.-frac)*exp(-.5*(nxt-mean2)**2/sigma2**2)/(sigma2*2.5066)",RooArgSet(nxt,mean1,sigma1,frac,mean2,sigma2));

  RooRealIntegral* weight[NXT];
  Float_t totintegral=0.;
  TH1F*HWeights=new TH1F("HWeights","HWeights",NXT,-.5,NXT-.5);
  for(Int_t j=0;j<NXT;j++){
    nxt.setRange(TString("wrange")+(long)j,j-.5,j+.5);
    weight[j]=new RooRealIntegral(TString("weight")+(long)j,TString("weight")+(long)j,
				  weihtPdf,RooArgSet(nxt),&nset,0,TString("wrange")+(long)j);

    totintegral+=weight[j]->getVal();
    cout<<j<<" "<<weight[j]->getVal()<<endl;
    HWeights->SetBinContent(j+1,weight[j]->getVal());
    HWeights->SetBinError(j+1,.001);
  }
  cout<<"Total="<<totintegral<<endl;
 
  RooPlot*pl=nxt.frame();
  weihtPdf.plotOn(pl);
  pl->addTH1(HWeights,"p hist");
  pl->Draw("same");
  

}
