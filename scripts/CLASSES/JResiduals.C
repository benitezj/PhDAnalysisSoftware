#include "JResiduals.h"
#include "RooAbsReal.h"

ClassImp(JResiduals);
JResiduals::JResiduals(const char* name,TH1* H, TF1* F):
  TNamed(name,name),
  Histo(H),
  FitFunc(F),
  FitPdf(NULL),
  IntegralVariable(NULL),
  FitHisto(NULL),
  chisquare(0)
{
  if(H==NULL||F==NULL){
    cout<<"JResiduals: Bad inputs"<<endl; exit(0);
  }else {

    name=TString(H->GetName())+F->GetName()+"Residuals";
    ComputeResiduals();
  }
}

JResiduals::JResiduals(const char* name,TH1* H, RooAbsPdf* F,RooRealVar* Var):
  TNamed(name,name),
  Histo(H),
  FitFunc(NULL),
  FitPdf(F),
  IntegralVariable(Var),
  FitHisto(NULL),
  chisquare(0)
{
  if(H==NULL||F==NULL){
    cout<<"JResiduals: Bad inputs"<<endl; exit(0);
  }else {
  
    name=TString(H->GetName())+F->GetName()+"Residuals";    
    ComputeResiduals();
  }
}
JResiduals::JResiduals(const char* name,TH1* H, TH1* h):
  TNamed(name,name),
  Histo(H),
  FitFunc(NULL),
  FitPdf(NULL),
  IntegralVariable(NULL),
  FitHisto(h), 
  chisquare(0)
{
  if(H==NULL||h==NULL||h->GetNbinsX()!=H->GetNbinsX()){
    cout<<"JResiduals: Bad inputs"<<endl;  
    exit(0);
  }else {         
    name=TString(H->GetName())+h->GetName()+"Residuals";    
    ComputeResiduals();
  }
}


JResiduals::~JResiduals(){ 

  delete H1Residuals;
  delete H1NormResiduals;
  delete HChisquare;
}

void JResiduals::ComputeResiduals(){
  cout<<"Computing residuals"<<endl;
  
  H1Residuals=new TH1F(name,"Residuals",Histo->GetNbinsX(),Histo->GetXaxis()->GetXmin(),Histo->GetXaxis()->GetXmax());
  H1Residuals->SetStats(0);
  H1NormResiduals=new TH1F(name+"Norm","Residuals",Histo->GetNbinsX(),Histo->GetXaxis()->GetXmin(),Histo->GetXaxis()->GetXmax());
  H1NormResiduals->SetStats(0);
  HChisquare=new TH1F(name+"chisquare","#chi^{2} vs bin",Histo->GetXaxis()->GetNbins(),Histo->GetXaxis()->GetXmin(),Histo->GetXaxis()->GetXmax());
  Int_t nbins=Histo->GetXaxis()->GetNbins();
  Float_t residual=0;
  Float_t xlow=0;
  Float_t xhigh=0;
  Float_t fitvalue=0;
  Float_t residualerror=1;
  Float_t funclow=0;
  Float_t funchi=0;
  Float_t chiadd=0;
  Float_t normresidual=0;
  Float_t max=0;
  Float_t maxnorm=0;

  if(FitFunc!=NULL){
    funclow=FitFunc->GetXmin(); 
    funchi=FitFunc->GetXmax();   
    ndof=Histo->GetNbinsX()-FitFunc->GetNpar();
  }

  chisquare=0;
  Float_t bincontent=0;
  Bool_t binflag=1;

  for(Int_t bin=1;bin<=nbins;bin++){
    binflag=1;
    xlow=Histo->GetBinLowEdge(bin);
    xhigh=xlow+Histo->GetBinWidth(bin);
    bincontent=Histo->GetBinContent(bin);            
    residualerror=Histo->GetBinError(bin);

    //get the fit value 
    fitvalue=0;
    if(FitFunc!=NULL)fitvalue=FitFunc->Integral(xlow,xhigh)/Histo->GetBinWidth(bin);
    else if(FitPdf!=NULL)fitvalue=GetIntegralPdf(xlow,xhigh)*Histo->Integral();
    else if(FitHisto!=NULL)fitvalue=FitHisto->GetBinContent(bin);
    else binflag=0; 
       
    //check several errors
    if(!(residualerror>0&&residualerror<999999999)){  
      if(bin==1){//bad error from RooDataHist::createHistogram()	
	cout<<"Bin error for bin 1 got fixed: "<<residualerror<<" --> sqrt(bincontent)="<<sqrt(bincontent)<<endl;
	residualerror=sqrt(bincontent);
	Histo->SetBinError(bin,residualerror);
      } else if(residualerror==0&&(bincontent!=0||fitvalue!=0)){
	cout<<"JResiduals::Warning residualerror not good bin"<<bin
	    <<" error="<<residualerror
	    <<" bincontent"<<bincontent
	    <<" fitval="<<fitvalue<<endl;
	binflag=0; 
      }
    }  
    
    if(FitFunc!=NULL)//for a TF1 fit only do residuals in fit region      
      if(xlow<funclow||xhigh>funchi)
	binflag=0; 
    
    if(bincontent==0)
      binflag=0;  


    //calculate quantities
    if(binflag){
      residual=bincontent - fitvalue;
      chiadd=(residual*residual)/(residualerror*residualerror);
      normresidual=residual/residualerror; 
    }
    else{
      residual=0;
      residualerror=0;
      normresidual=0;
      chiadd=0;
    }
  
    //fill histograms
    H1Residuals->SetBinContent(bin,residual);
    H1Residuals->SetBinError(bin,residualerror);
    H1NormResiduals->SetBinContent(bin,normresidual);
    H1NormResiduals->SetBinError(bin,0);
    HChisquare->SetBinContent(bin,chiadd);
    chisquare+=chiadd;  
    
    if(max<fabs(residual)) max=fabs(residual);
    if(maxnorm<fabs(normresidual)) maxnorm=fabs(normresidual);
    //cout<<bin<<" "<<residual<<" "<<residualerror<<" "<<chiadd<<endl;
  }
 
   
  H1Residuals->GetYaxis()->SetRangeUser(-2*max,2*max);
  H1NormResiduals->GetYaxis()->SetRangeUser(-2*maxnorm,2*maxnorm);
  
}



Float_t JResiduals::GetIntegralPdf(Float_t xlow,Float_t xhigh){
  RooAbsReal* integral;
  IntegralVariable->setRange("binrange",xlow,xhigh);
  integral= FitPdf->createIntegral(*IntegralVariable,"binrange");
  return  (float)integral->getVal();
}


void JResiduals::MakeFitPlotWithResiduals(TCanvas*C, RooPlot* rooplot){
  if(!C)return;
  C->Divide(1,2);

  fitpad=(TPad*)C->GetPad(1);
  fitpad->SetPad(0.,.2,1.,1.);
  C->cd(1);
  if(!rooplot){
    Histo->Draw("pe");
    if(FitFunc!=NULL){
      FitFunc->SetNpx(5000);
      FitFunc->SetLineColor(4);
      FitFunc->Draw("lsame");
    }
    if(FitPdf!=NULL){
      cout<<"Sorry RooPdf not implemented."<<endl;
      return;
    }
    if(FitHisto!=NULL){
      FitHisto->SetLineColor(4);
      FitHisto->Draw("same");
    }
  }
  //if rooplot is provided the draw it instead
  if(rooplot)rooplot->Draw();


  
  respad=(TPad*)C->GetPad(2);
  respad->SetPad(.0,.0,1.,.2);
  C->cd(2);
  DrawNormResiduals();

}

void JResiduals::MakeFitPlotWithResiduals(TCanvas*C, RooPlot* rooplot,TH1* Hist){//draw Hist on top of rooplot
  if(!C||!rooplot)return;
  C->Divide(1,2);

  fitpad=(TPad*)C->GetPad(1);
  fitpad->SetPad(0.,.2,1.,1.);
  C->cd(1);
  rooplot->Draw();
  //Hist->GetYaxis()->SetRangeUser(0,Hist->GetMaximum()*1.1);
  //Hist->Draw();
  if(Hist)Hist->Draw("same");
  

  respad=(TPad*)C->GetPad(2);
  respad->SetPad(.0,.0,1.,.2);
  C->cd(2);
  DrawNormResiduals();

}


void JResiduals::DrawNormResiduals(){
 
  H1NormResiduals->GetYaxis()->SetTitle("Residuals");
  H1NormResiduals->GetYaxis()->SetTitleSize(.2);
  H1NormResiduals->GetYaxis()->SetTitleOffset(.2);
  H1NormResiduals->GetYaxis()->SetLabelSize(.15);
  H1NormResiduals->GetXaxis()->SetLabelSize(.15);
  H1NormResiduals->GetYaxis()->SetNdivisions(5);

  Float_t scale=fabs(H1NormResiduals->GetBinContent(H1NormResiduals->GetMaximumBin())); //
  if(scale<fabs(H1NormResiduals->GetBinContent(H1NormResiduals->GetMaximumBin())))//GetMinimumBin
    scale=fabs(H1NormResiduals->GetBinContent(H1NormResiduals->GetMinimumBin()));  

  H1NormResiduals->GetYaxis()->SetRangeUser(-floor(scale+2),floor(scale+2));
  H1NormResiduals->SetTitle(" ");
  H1NormResiduals->Draw("p");    

  TLine line;
  line.SetLineColor(2);
  line.DrawLine(H1NormResiduals->GetXaxis()->GetXmin(),2,H1NormResiduals->GetXaxis()->GetXmax(),2);
  line.DrawLine(H1NormResiduals->GetXaxis()->GetXmin(),-2,H1NormResiduals->GetXaxis()->GetXmax(),-2);
  H1NormResiduals->Draw("psame");  

}


TH1F* JResiduals::GetNormResidualProjection(){

  HNormResProj=new TH1F(name+"NormProj","Projection of Normalized Residuals",20,-5,5);

  for(Int_t b=1;b<=H1NormResiduals->GetNbinsX();b++)        
    HNormResProj->Fill(H1NormResiduals->GetBinContent(b));
    
  return HNormResProj;
}
