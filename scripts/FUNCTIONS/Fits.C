#include "Fits.h"


TF1* FitHistogram1gaus(TH1F* h,Float_t low,Float_t high){
  Axis_t Mean,Sigma,Norm,fitxmin,fitxmax;

  Mean=(float)h->GetBinCenter(h->GetMaximumBin());
  Sigma=(float)h->GetRMS();  
  Norm=(float)h->GetBinContent(h->GetMaximumBin());
  fitxmin=low;
  fitxmax=high;
  
  TF1* f= new TF1("f","[0]*exp(-.5*(x-[1])**2/[2]**2)",fitxmin-(fitxmax-fitxmin)*10,fitxmax+(fitxmax-fitxmin)*10);
  f->SetParNames("Norm","Mean","Sigma");
  f->SetLineColor(2);   
  f->SetLineWidth(2);
  f->SetParameters(Norm,Mean,.2*Sigma);

  h->Fit("f","QI","pe",fitxmin,fitxmax); 
  if(f->GetParameter("Sigma")<0)f->SetParameter("Sigma",-f->GetParameter("Sigma"));

  return f;
}


TF1* FitHistogram2gaus(TH1F* h,Float_t low,Float_t high){
  Float_t Mean,Sigma,Norm,fitxmin,fitxmax;
 
  Mean=(float)h->GetBinCenter(h->GetMaximumBin()); 
  Sigma=(float)h->GetRMS();
  Norm=(float)h->GetBinContent(h->GetMaximumBin());
  fitxmin=low;
  fitxmax=high;
 
  TF1* f=new TF1(TString("f")+h->GetName(),"[0]*exp(-.5*(x-[1])**2/[2]**2)+[3]*exp(-.5*(x-[4])**2/[5]**2)",fitxmin-(fitxmax-fitxmin)*10,fitxmax+(fitxmax-fitxmin)*10);
  f->SetParNames("NormNarrow","MeanNarrow","SigmaNarrow","NormWide","MeanWide","SigmaWide");
  f->SetLineColor(2);   
  f->SetLineWidth(1);
  f->SetParameters(Norm,Mean,Sigma/3,Norm/5,Mean,Sigma);
  f->SetParLimits(3,0,Norm/10);
  f->SetParLimits(5,2*Sigma,100*Sigma);
  h->Fit(f,"QI","pe",fitxmin,fitxmax);
  if(f->GetParameter(2)<0)f->SetParameter(2,-f->GetParameter(2));
  if(f->GetParameter(5)<0)f->SetParameter(5,-f->GetParameter(5));
  //switch parameters
  if(f->GetParameter("SigmaNarrow")>f->GetParameter("SigmaWide")){
    Norm=f->GetParameter("NormNarrow");    
    Mean=f->GetParameter("MeanNarrow");
    Sigma=f->GetParameter("SigmaNarrow");
    f->SetParameter("NormNarrow",f->GetParameter("NormWide"));
    f->SetParameter("MeanNarrow",f->GetParameter("MeanWide"));
    f->SetParameter("SigmaNarrow",f->GetParameter("SigmaWide"));
    f->SetParameter("NormWide",Norm);
    f->SetParameter("MeanWide",Mean);
    f->SetParameter("SigmaWide",Sigma);        
  }
  
  
  return f;
}


void Draw2gaus(TF1* f){
  int color1=3,color2=4;  

  
  TF1*Fgaus1=new TF1(TString("Fg1")+f->GetName(),"[0]*exp(-.5*(x-[1])**2/[2]**2)",f->GetXmin(),f->GetXmax());
  TF1*Fgaus2=new TF1(TString("Ff")+f->GetName(),"[0]*exp(-.5*(x-[1])**2/[2]**2)",f->GetXmin(),f->GetXmax());
  

  Fgaus1->SetParameters(f->GetParameter(0),f->GetParameter(1),f->GetParameter(2));
  Fgaus1->SetLineColor(color1);
  Fgaus1->SetLineWidth(2);
  Fgaus1->SetLineStyle(1);

  Fgaus2->SetParameters(f->GetParameter(3),f->GetParameter(4),f->GetParameter(5));
  Fgaus2->SetLineColor(color2);
  Fgaus2->SetLineWidth(2);
  Fgaus2->SetLineStyle(1);

  Fgaus1->Draw("LSAME");
  Fgaus2->Draw("LSAME");
  f->SetLineWidth(2);
  f->Draw("LSAME");


  return;
}


TF1* FitHistogram1gausPlusBoltzmanModified(TH1F* h,Float_t low,Float_t high){//this is best match found for topmeas-topexp(410)
  Float_t Mean,Sigma,Norm,fitxmin,fitxmax;
 
  Mean=(float)h->GetBinCenter(h->GetMaximumBin()); 
  Sigma=(float)h->GetRMS();
  Norm=(float)h->GetBinContent(h->GetMaximumBin());
  fitxmin=Mean-low*Sigma;
  fitxmax=Mean+high*Sigma;
   
  TF1* f=new TF1("f"," [0]*exp(-.5*(x-[1])**2/[2]**2) +[3]*((x-[4])**3)*exp(-.5*(x-[4])**2/[5]**2)",fitxmin-(fitxmax-fitxmin)*10,fitxmax+(fitxmax-fitxmin)*10);
  f->SetParNames("GaussNorm","GuassMean","GuassSigma","BoltzNorm","BoltzX0","BoltzSigma");
  f->SetLineColor(2);   
  f->SetLineWidth(1);
  f->SetParameters(Norm/40,Mean,2*Sigma,Norm/4,Mean-2*Sigma,Sigma);
  h->Fit(f,"QI","pe",fitxmin,fitxmax);
  if(f->GetParameter(2)<0)f->SetParameter(2,-f->GetParameter(2));
   
  return f;
}


void Draw1gausPlusBoltzmanModified(TF1* f){
  int color1=3,color2=4;  


  TF1*Fgaus=new TF1(TString("Fgaus")+f->GetName(),"[0]*exp(-.5*(x-[1])**2/[2]**2)",f->GetXmin(),f->GetXmax());
  TF1*FBoltzModified=new TF1(TString("Fboltz")+f->GetName(),"([0]*(x-[1])**3)*exp(-.5*(x-[1])**2/[2]**2)",f->GetXmin(),f->GetXmax());

  Fgaus->SetParameters(f->GetParameter(0),f->GetParameter(1),f->GetParameter(2));
  Fgaus->SetLineColor(color2);
  Fgaus->SetLineWidth(2);
  Fgaus->SetLineStyle(4);
  
  FBoltzModified->SetParameters(f->GetParameter(3),f->GetParameter(4),f->GetParameter(5));
  FBoltzModified->SetLineColor(color1);
  FBoltzModified->SetLineWidth(2);
  FBoltzModified->SetLineStyle(2);

  TText*t=new TText();
  t->SetTextSize(.03);
  char s[20];
  if(FBoltzModified->GetParameter(0)>Fgaus->GetParameter(0))sprintf(s,"PrimaryMax %.3f",FBoltzModified->GetParameter(1)+sqrt(3.)*FBoltzModified->GetParameter(2));
  else sprintf(s,"PrimaryMax %.3f",Fgaus->GetParameter(1));
  
  FBoltzModified->Draw("LSAME");
  t->DrawTextNDC(.65,.3,s);
  Fgaus->Draw("LSAME");
  f->SetLineWidth(2);
  f->Draw("LSAME");

  return;
}



TF1* FitHistogram3gaus(TH1F* h,Float_t fitxmin,Float_t fitxmax){
  Float_t Mean,Sigma,Norm;
 
  Mean=(float)h->GetBinCenter(h->GetMaximumBin()); 
  Sigma=(float)h->GetRMS();
  Norm=(float)h->GetBinContent(h->GetMaximumBin());
 
  TF1* f=new TF1(TString("f")+h->GetName(),"[0]*exp(-.5*(x-[1])**2/[2]**2)+[3]*exp(-.5*(x-[4])**2/[5]**2)+[6]*exp(-.5*(x-[7])**2/[8]**2)",fitxmin,fitxmax);
  f->SetParNames("Norm1","Mean1","Sigma1","Norm2","Mean2","Sigma2","Norm3","Mean3","Sigma3");
  f->SetLineColor(2);   
  f->SetLineWidth(1);
  f->SetParameters(Norm,Mean,Sigma/3,Norm/5,Mean,2*Sigma,Norm/10,Mean,3*Sigma);
  h->Fit(f,"QI","pe",fitxmin,fitxmax);
  if(f->GetParameter(2)<0)f->SetParameter(2,-f->GetParameter(2));
  if(f->GetParameter(5)<0)f->SetParameter(5,-f->GetParameter(5));
  if(f->GetParameter(8)<0)f->SetParameter(8,-f->GetParameter(8));
   
  
  return f;
}


void Draw3gaus(TF1* f){
  int color1=3,color2=4,color3=5;  

  TF1*Fgaus1=new TF1(TString("Fg1")+f->GetName(),"[0]*exp(-.5*(x-[1])**2/[2]**2)",f->GetParameter(1)-3*f->GetParameter(2),f->GetParameter(1)+3*f->GetParameter(2));
  TF1*Fgaus2=new TF1(TString("Ff")+f->GetName(),"[0]*exp(-.5*(x-[1])**2/[2]**2)",f->GetParameter(4)-3*f->GetParameter(5),f->GetParameter(4)+3*f->GetParameter(5));
  TF1*Fgaus3=new TF1(TString("Ff")+f->GetName(),"[0]*exp(-.5*(x-[1])**2/[2]**2)",f->GetParameter(7)-3*f->GetParameter(8),f->GetParameter(7)+3*f->GetParameter(8));
  

  Fgaus1->SetParameters(f->GetParameter(0),f->GetParameter(1),f->GetParameter(2));
  Fgaus1->SetLineColor(color1);
  Fgaus1->SetLineWidth(2);
  Fgaus1->SetLineStyle(2);

  Fgaus2->SetParameters(f->GetParameter(3),f->GetParameter(4),f->GetParameter(5));
  Fgaus2->SetLineColor(color2);
  Fgaus2->SetLineWidth(2);
  Fgaus2->SetLineStyle(4);

  Fgaus3->SetParameters(f->GetParameter(6),f->GetParameter(7),f->GetParameter(8));
  Fgaus3->SetLineColor(color3);
  Fgaus3->SetLineWidth(2);
  Fgaus3->SetLineStyle(4);

  Fgaus1->Draw("LSAME");
  Fgaus2->Draw("LSAME");
  Fgaus3->Draw("LSAME");
  f->SetLineWidth(2);
  f->Draw("LSAME");


  return;
}


TF1* FitHistogram3gausPlusPolN(TH1F* h,Float_t fitmin,Float_t fitmax,Int_t N,Float_t* initpar){
  
  if(N<0||N>50){
    cout<<"Fits.C: Invalid polynomial degree."<<endl; 
    return NULL;
  }
  
  TString formula="[0]*exp(-.5*(x-[1])**2/[2]**2)+[3]*exp(-.5*(x-[4])**2/[5]**2)+[6]*exp(-.5*(x-[7])**2/[8]**2)+[9]";
  if(N==1||N>1)
    formula+="+[10]*x";
  if(N>1){   
    for(Int_t n=2;n<=N;n++)
      formula+=TString("+[")+(long)(n+9)+"]*x**"+(long)n;
  } 


  TF1* f=new TF1((const char*)(TString("f")+h->GetName()),(const char*)formula,fitmin,fitmax);
  f->SetParNames("Norm1","Mean1","Sigma1","Norm2","Mean2","Sigma2","Norm3","Mean3","Sigma3");
  for(Int_t n=0;n<=N;n++)
    f->SetParName(n+9,TString("c_{")+(long)(n)+"}");
  f->SetLineColor(2);   
  f->SetLineWidth(1);
 
  if(h==NULL)return f;
  
  if(initpar==0){
    f->SetParameters(h->GetMaximum(),h->GetMean(),h->GetRMS(),h->GetMaximum(),h->GetMean(),h->GetRMS(),h->GetMaximum(),h->GetMean(),h->GetRMS());
    for(Int_t n=9;n<=N;n++)
      f->SetParameter(n,n);
  }else {
    for(Int_t n=0;n<=N;n++)
      f->SetParameter(n,initpar[n]);
  }

  h->Fit(f,"QI","pe",fitmin,fitmax);
  if(f->GetParameter(2)<0)f->SetParameter(2,-f->GetParameter(2));
  if(f->GetParameter(5)<0)f->SetParameter(5,-f->GetParameter(5));
  if(f->GetParameter(8)<0)f->SetParameter(8,-f->GetParameter(8));
    
  return f;
}


TF1* FitHistogram2gausWith1GaussFixed(TH1F* h,Float_t backNorm,Float_t backMean,Float_t backSigma,Float_t low,Float_t high){
  Float_t Mean,Sigma,Norm,fitxmin,fitxmax;
 
  Mean=(float)h->GetBinCenter(h->GetMaximumBin()); 
  Sigma=(float)h->GetRMS();
  Norm=(float)h->GetBinContent(h->GetMaximumBin());
  fitxmin=low;
  fitxmax=high;
 
  TF1* f=new TF1((const char*)(TString("f")+h->GetName()),"[0]*exp(-.5*(x-[1])**2/[2]**2)+[3]*exp(-.5*(x-[4])**2/[5]**2)",fitxmin-(fitxmax-fitxmin)*10,fitxmax+(fitxmax-fitxmin)*10);
  f->SetParNames("NormNarrow","MeanNarrow","SigmaNarrow","NormWide","MeanWide","SigmaWide");
  f->SetLineColor(2);   
  f->SetLineWidth(1);
  f->SetParameters(Norm,Mean,Sigma/3,Norm/5,backMean,Sigma);
  if(backNorm!=0)f->FixParameter(3,backNorm);
  if(backMean!=0)f->FixParameter(4,backMean);
  if(backSigma!=0)f->FixParameter(5,backSigma);

  
  h->Fit(f,"QI","pe",fitxmin,fitxmax);
  if(f->GetParameter(2)<0)f->SetParameter(2,-f->GetParameter(2));
  if(f->GetParameter(5)<0)f->SetParameter(5,-f->GetParameter(5));
   
  //switch parameters
  if(f->GetParameter("SigmaNarrow")>f->GetParameter("SigmaWide")){
    Norm=f->GetParameter("NormNarrow");    
    Mean=f->GetParameter("MeanNarrow");
    Sigma=f->GetParameter("SigmaNarrow");
    f->SetParameter("NormNarrow",f->GetParameter("NormWide"));
    f->SetParameter("MeanNarrow",f->GetParameter("MeanWide"));
    f->SetParameter("SigmaNarrow",f->GetParameter("SigmaWide"));
    f->SetParameter("NormWide",Norm);
    f->SetParameter("MeanWide",Mean);
    f->SetParameter("SigmaWide",Sigma);        
  }
  


  return f;
}



TF1* FitHistogram1gausPlusConstant(TH1F* h,Float_t low,Float_t high){
  Float_t Mean,Sigma,Norm,fitxmin,fitxmax;
 
  Mean=(float)h->GetBinCenter(h->GetMaximumBin()); 
  Sigma=(float)h->GetRMS();
  Norm=(float)h->GetBinContent(h->GetMaximumBin());
  fitxmin=Mean-low*Sigma;
  fitxmax=Mean+high*Sigma;
 
  TF1* f=new TF1((const char*)(TString("f")+h->GetName()),"[0]*exp(-.5*(x-[1])**2/[2]**2)+[3]",fitxmin-(fitxmax-fitxmin)*10,fitxmax+(fitxmax-fitxmin)*10);
  f->SetParNames("Norm","Mean","Sigma","Constant");
  f->SetLineColor(2);   
  f->SetLineWidth(1);
  f->SetParameters(Norm,Mean,Sigma/3,Norm/25);
  f->SetParLimits(3,0,Norm/10);
  h->Fit(f,"QI","pe",fitxmin,fitxmax);
  if(f->GetParameter(2)<0)f->SetParameter(2,-f->GetParameter(2));
    
  return f;
}

void Draw1gausPlusConstant(TF1* f){
  int color1=3,color2=4;

  TF1*Fgaus1=new TF1((const char*)(TString("Fg1")+f->GetName()),"[0]*exp(-.5*(x-[1])**2/[2]**2)",f->GetParameter(1)-3*f->GetParameter(2),f->GetParameter(1)+3*f->GetParameter(2));
  TF1*FConstant=new TF1((const char*)(TString("FConstant")+f->GetName()),"[0]",f->GetParameter(1)-10*f->GetParameter(2),f->GetParameter(1)+10*f->GetParameter(2));
  
  Fgaus1->SetParameters(f->GetParameter(0),f->GetParameter(1),f->GetParameter(2));
  Fgaus1->SetLineColor(color1);
  Fgaus1->SetLineWidth(1);
  FConstant->SetParameters(0,f->GetParameter(3));
  FConstant->SetLineColor(color2);
  FConstant->SetLineWidth(1);

  Fgaus1->Draw("LSAME");
  FConstant->Draw("LSAME");
  f->SetLineWidth(2);
  f->Draw("Lsame");

 
  return;

}


void Draw1gausPlusPolN(TF1* f,Int_t N){
  int color1=3,color2=4;

  TF1*Fgaus1=new TF1((const char*)(TString("Fg1")+f->GetName()),"[0]*exp(-.5*(x-[1])**2/[2]**2)",f->GetXmin(),f->GetXmax());
  TF1*FPolN=new TF1((const char*)(TString("FPolN")+f->GetName()),"[0]+[1]*x+[2]*x**2+[3]*x**3+[4]*x**4+[5]*x**5",f->GetXmin(),f->GetXmax());
    
  Fgaus1->SetParameters(f->GetParameter(0),f->GetParameter(1),f->GetParameter(2));
  Fgaus1->SetLineColor(color1);
  Fgaus1->SetLineWidth(1);
  for(Int_t i=0;i<=N&&i<=5;i++)
    FPolN->SetParameter(i,f->GetParameter(i+3));
  FPolN->SetLineColor(color2);
  FPolN->SetLineWidth(1);

  Fgaus1->Draw("LSAME");
  FPolN->Draw("LSAME");
  f->SetLineWidth(2);
  f->Draw("Lsame");

 
  return;

}



TF1* FitHistogram2gausPlusConstant(TH1F* h,Float_t low,Float_t high){
  Float_t Mean,Sigma,Norm,fitxmin,fitxmax;
 
  Mean=(float)h->GetBinCenter(h->GetMaximumBin()); 
  Sigma=(float)h->GetRMS();
  Norm=(float)h->GetBinContent(h->GetMaximumBin());
  fitxmin=Mean-low*Sigma;
  fitxmax=Mean+high*Sigma;
 
  TF1* f=new TF1((const char*)(TString("f")+h->GetName()),"[0]*exp(-.5*(x-[1])**2/[2]**2)+[3]*exp(-.5*(x-[4])**2/[5]**2)+[6]",fitxmin-(fitxmax-fitxmin)*10,fitxmax+(fitxmax-fitxmin)*10);
  f->SetParNames("NormNarrow","MeanNarrow","SigmaNarrow","NormWide","MeanWide","SigmaWide","Constant");
  f->SetLineColor(2);   
  f->SetLineWidth(1);
  f->SetParameters(Norm,Mean,Sigma/3,Norm/5,Mean,Sigma);
  f->SetParLimits(3,0,Norm/10);
  f->SetParLimits(5,2*Sigma,100*Sigma);
  f->SetParLimits(6,0,Norm/10);
  h->Fit(f,"QI","pe",fitxmin,fitxmax);
  if(f->GetParameter(2)<0)f->SetParameter(2,-f->GetParameter(2));
  if(f->GetParameter(5)<0)f->SetParameter(5,-f->GetParameter(5));

  //switch parameters
  if(f->GetParameter("SigmaNarrow")>f->GetParameter("SigmaWide")){
    Norm=f->GetParameter("NormNarrow");    
    Mean=f->GetParameter("MeanNarrow");
    Sigma=f->GetParameter("SigmaNarrow");
    f->SetParameter("NormNarrow",f->GetParameter("NormWide"));
    f->SetParameter("MeanNarrow",f->GetParameter("MeanWide"));
    f->SetParameter("SigmaNarrow",f->GetParameter("SigmaWide"));
    f->SetParameter("NormWide",Norm);
    f->SetParameter("MeanWide",Mean);
    f->SetParameter("SigmaWide",Sigma);        
  }

    
  return f;
}

void Draw2gausPlusConstant(TF1* f){
  int color1=3,color2=4,color3=5; 

  TF1*Fgaus1=new TF1((const char*)(TString("Fg1")+f->GetName()),"[0]*exp(-.5*(x-[1])**2/[2]**2)",
		     f->GetParameter(1)-3*f->GetParameter(2),f->GetParameter(1)+3*f->GetParameter(2));
  TF1*Fgaus2=new TF1((const char*)(TString("Ff")+f->GetName()),"[0]*exp(-.5*(x-[1])**2/[2]**2)",
		     f->GetParameter(4)-3*f->GetParameter(5),f->GetParameter(4)+3*f->GetParameter(5));
  TF1*FConstant=new TF1((const char*)(TString("FConstant")+f->GetName()),"[0]",
			f->GetParameter(4)-10*f->GetParameter(5),f->GetParameter(4)+10*f->GetParameter(5));
  

  Fgaus1->SetParameters(f->GetParameter(0),f->GetParameter(1),f->GetParameter(2));
  Fgaus1->SetLineColor(color1);
  Fgaus1->SetLineWidth(1);
  Fgaus2->SetParameters(f->GetParameter(3),f->GetParameter(4),f->GetParameter(5));
  Fgaus2->SetLineColor(color2);
  Fgaus2->SetLineWidth(1);
  FConstant->SetParameters(0,f->GetParameter(6));
  FConstant->SetLineColor(color3);
  FConstant->SetLineWidth(1);


  Fgaus1->Draw("LSAME");
  Fgaus2->Draw("LSAME");
  FConstant->Draw("LSAME");
  f->SetLineWidth(2);
  f->Draw("Lsame");

  

  return;

}

  
void Draw2gausPlusPolN(TF1* f,Int_t N){
  int color1=3,color2=4,color3=6; 

  TF1*Fgaus1=new TF1((const char*)(TString("Fg1")+f->GetName()),"[0]*exp(-.5*(x-[1])**2/[2]**2)",f->GetXmin(),f->GetXmax());
  TF1*Fgaus2=new TF1((const char*)(TString("Fg2")+f->GetName()),"[0]*exp(-.5*(x-[1])**2/[2]**2)",f->GetXmin(),f->GetXmax());
  TF1*FPolN=new TF1((const char*)(TString("FPolN")+f->GetName()),"[0]+[1]*x+[2]*x**2+[3]*x**3+[4]*x**4+[5]*x**5",f->GetXmin(),f->GetXmax());     

  Fgaus1->SetParameters(f->GetParameter(0),f->GetParameter(1),f->GetParameter(2));
  Fgaus1->SetLineColor(color1);
  Fgaus1->SetLineWidth(1);
  Fgaus2->SetParameters(f->GetParameter(3),f->GetParameter(4),f->GetParameter(5));
  Fgaus2->SetLineColor(color2);
  Fgaus2->SetLineWidth(1);
  for(Int_t i=0;i<=N&&i<=5;i++)
    FPolN->SetParameter(i,f->GetParameter(i+6));
  FPolN->SetLineColor(color3);
  FPolN->SetLineWidth(1);


  Fgaus1->Draw("LSAME");
  Fgaus2->Draw("LSAME");
  FPolN->Draw("LSAME");
  f->SetLineWidth(2);
  f->Draw("Lsame");

  

  return;

}

  


TF1* FitHistogram1gausPlusPol1(TH1F* h,Float_t fitmin,Float_t fitmax){
 
  TF1* f=new TF1((const char*)(TString("f")+h->GetName()),"[0]*exp(-.5*(x-[1])**2/[2]**2)+[3]+[4]*x",fitmin,fitmax);
  f->SetParNames("Norm","Mean","Sigma","offset","slope");
  f->SetLineColor(2);   
  f->SetLineWidth(1);
  f->SetParameters(h->GetMaximum(),h->GetMean(),h->GetRMS(),100,10);
  h->Fit(f,"QI","pe",fitmin,fitmax);
  if(f->GetParameter(2)<0)f->SetParameter(2,-f->GetParameter(2));
    
  return f;
}


TF1* FitHistogram1gausPlusPolN(TH1F* h,Float_t fitmin,Float_t fitmax,Int_t N,Float_t* initpar){

  if(N<0){
    cout<<"Fits.C: Invalid polynomial degree."<<endl; 
    return NULL;
  }
  
  TString formula="[0]*exp(-.5*(x-[1])**2/[2]**2)+[3]";
  if(N==1||N>1)
    formula+="+[4]*x";
  if(N>1){   
    for(Int_t n=2;n<=N;n++)
      formula+=TString("+[")+(long)(n+3)+"]*x**"+(long)n;
  } 


  TF1* f=new TF1((const char*)(TString("f")+h->GetName()),(const char*)(formula),fitmin,fitmax);
  f->SetParNames("Norm","Mean","Sigma");
  for(Int_t n=3;n<=N;n++)
    f->SetParName(n,TString("c_{")+(long)(n-3)+"}");
  f->SetLineColor(2);   
  f->SetLineWidth(1);
 
  if(initpar==0){
    f->SetParameters(h->GetMaximum(),h->GetMean(),h->GetRMS());
    for(Int_t n=3;n<=N;n++)
      f->SetParameter(n,n);
  }else {
    for(Int_t n=0;n<=N;n++)
      f->SetParameter(n,initpar[n]);
  }



  h->Fit(f,"QI","pe",fitmin,fitmax);
  if(f->GetParameter(2)<0)f->SetParameter(2,-f->GetParameter(2));
    
  return f;
}


TF1* FitHistogram2gausPlusPolN(TH1F* h,Float_t fitmin,Float_t fitmax,Int_t N,Float_t* initpar){

  if(N<0||N>50){
    cout<<"Fits.C: Invalid polynomial degree."<<endl; 
    return NULL;
  }
  
  TString formula="[0]*exp(-.5*(x-[1])**2/[2]**2)+[3]*exp(-.5*(x-[4])**2/[5]**2)+[6]";
  if(N==1||N>1)
    formula+="+[7]*x";
  if(N>1){   
    for(Int_t n=2;n<=N;n++)
      formula+=TString("+[")+(long)(n+6)+"]*x**"+(long)n;
  } 


  TF1* f=new TF1((const char*)(TString("f")+h->GetName()),(const char*)(formula),fitmin,fitmax);
  f->SetParNames("Norm1","Mean1","Sigma1","Norm2","Mean2","Sigma2");
  for(Int_t n=0;n<=N;n++)
    f->SetParName(n+6,TString("c_{")+(long)(n)+"}");
  f->SetLineColor(2);   
  f->SetLineWidth(1);
  
  //initialize the parameters
  f->SetParLimits(0,.01*h->GetMaximum(),1.5*h->GetMaximum());
  f->SetParLimits(1,h->GetXaxis()->GetXmin(),h->GetXaxis()->GetXmax());
  f->SetParLimits(2,0,2*h->GetRMS());
  f->SetParLimits(3,.01*h->GetMaximum(),1.5*h->GetMaximum());
  f->SetParLimits(4,h->GetXaxis()->GetXmin(),h->GetXaxis()->GetXmax());
  f->SetParLimits(5,0,2*h->GetRMS());
  if(initpar==0){
    f->SetParameters(h->GetMaximum(),h->GetMean(),h->GetRMS(),h->GetMaximum(),h->GetMean(),h->GetRMS());

    for(Int_t n=6;n<=N;n++)
      f->SetParameter(n,n);
  }else {
    for(Int_t n=0;n<=N;n++)
      f->SetParameter(n,initpar[n]);
  }

    
  h->Fit(f,"QI","pe",fitmin,fitmax);
  if(f->GetParameter(2)<0)f->SetParameter(2,-f->GetParameter(2));
  if(f->GetParameter(5)<0)f->SetParameter(5,-f->GetParameter(5));
    
  return f;
}



TF1* FitHistogram2gausWithSameMeanPlusPolN(TH1F* h,Float_t fitmin,Float_t fitmax,Int_t N,Float_t* initpar,TF1*Bkg){

  if(N<0||N>50){
    cout<<"Fits.C: Invalid polynomial degree."<<endl; 
    return NULL;
  }
  
  TString sigformula="[0]*exp(-.5*(x-[1])**2/[2]**2)+[3]*exp(-.5*(x-[1])**2/[4]**2)";  
  TString bkgformula="[5]";
  if(N>=1){   
    for(Int_t n=1;n<=N;n++)
      bkgformula+=TString("+[")+(long)(n+5)+"]*x**"+(long)n;
  } 


  TF1* f=new TF1((const char*)(TString("f")+h->GetName()),(const char*)(sigformula+"+"+bkgformula),fitmin,fitmax);
  f->SetParNames("Norm1","Mean1","Sigma1","Norm2","Sigma2");
  for(Int_t n=0;n<=N;n++)
    f->SetParName(n+5,TString("c_{")+(long)(n)+"}");
  f->SetLineColor(4);   
  f->SetLineWidth(1);
  
  //initialize the parameters
  if(initpar==0){
    f->SetParameters(h->GetMaximum(),h->GetMean(),h->GetRMS(),h->GetMaximum(),h->GetRMS());
    for(Int_t n=5;n<=N;n++)
      f->SetParameter(n,n);
  }else {
    for(Int_t n=0;n<=N;n++)
      f->SetParameter(n,initpar[n]);
  }


  h->Fit(f,"QI","pe",fitmin,fitmax);
  if(f->GetParameter(2)<0)f->SetParameter(2,-f->GetParameter(2));
  if(f->GetParameter(4)<0)f->SetParameter(4,-f->GetParameter(4));
  
  TF1 bkg("bkg",(const char*)(bkgformula),fitmin,fitmax);
  bkg.SetLineStyle(2);
  bkg.SetLineColor(1);
  for(Int_t n=0;n<=N;n++)
    bkg.SetParameter(n+5,f->GetParameter(n+5));

  *Bkg=bkg;

  return f;
}




TF1* FitHistogramPolN(TH1F* h,Float_t fitmin,Float_t fitmax,Int_t N,Float_t* initpar){
  if(N<0){
    cout<<"Fits.C: Invalid polynomial degree."<<endl; 
    return NULL;
  }
  
  TString formula="[0]";
  if(N==1||N>1)
    formula+="+[1]*x";
  if(N>1){   
    for(Int_t n=2;n<=N;n++)
      formula+=TString("+[")+(long)n+"]*x**"+(long)n;
  }
  
  TF1* f=new TF1((const char*)(TString("f")+h->GetName()),(const char*)(formula),fitmin,fitmax);
  for(Int_t n=0;n<=N;n++)
    f->SetParName(n,TString("c_{")+(long)n+"}");
  f->SetLineColor(2);   
  f->SetLineWidth(1);
  
  if(initpar==0){
    for(Int_t n=0;n<=N;n++)
      f->SetParameter(n,n);
  }else {
    for(Int_t n=0;n<=N;n++)
      f->SetParameter(n,initpar[n]);
  }

  
  h->Fit(f,"QI","pe",fitmin,fitmax);
    
  return f;
}

TF1* FitHistogramExponential(TH1F* h,Float_t fitmin,Float_t fitmax){
 
  TF1* f=new TF1((const char*)(TString("f")+h->GetName()),"[0]*exp([1]*x)",fitmin,fitmax); 
  f->SetParNames("Norm","Constant");
  f->SetParameters(h->GetBinContent(h->GetMaximumBin()),-1/h->GetMean());
  f->SetLineColor(2);   
  f->SetLineWidth(1);

  h->Fit(f,"QI","pe",fitmin,fitmax);
    
  return f;
}

TF1* FitHistogramRelBreitWigner(TH1F* h,Float_t fitmin,Float_t fitmax){

  TF1* f=new TF1((const char*)(TString("f")+h->GetName()),
		 "[0]*x*[1]*[2]*(2/3.14159265358979312)/((x*x-[1]*[1])**2+([1]*[1])*([2]*[2]))",fitmin,fitmax); 
  f->SetParNames("Norm","M","#Gamma");
  f->SetParameters(.5*h->GetBinContent(h->GetMaximumBin())*h->GetRMS()*h->GetRMS()/4,h->GetBinCenter(h->GetMaximumBin()),h->GetRMS()/2);
  f->SetLineColor(2);   
  f->SetLineWidth(1);

  h->Fit(f,"QI","pe",fitmin,fitmax);

  if(f->GetParameter(2)<0)f->SetParameter(2,-f->GetParameter(2));
    
  return f;

}
TF1* FitHistogramBreitWigner(TH1F* h,Float_t fitmin,Float_t fitmax){

  TF1* f=new TF1((const char*)(TString("f")+h->GetName()),"[0]*([2]/(2*3.14159265358979312))/((x-[1])**2+.25*([2]**2))",fitmin,fitmax); 
  f->SetParNames("Norm","M","#Gamma"); 
  f->SetParameters(.5*h->GetBinContent(h->GetMaximumBin())*h->GetRMS()*h->GetRMS()/4,h->GetBinCenter(h->GetMaximumBin()),h->GetRMS()/2);
  f->SetLineColor(2);   
  f->SetLineWidth(1);
   
  h->Fit(f,"QI","pe",fitmin,fitmax);

  if(f->GetParameter(2)<0)f->SetParameter(2,-f->GetParameter(2));

  return f;

}
TF1* FitHistogramRelBreitWignerPolN(TH1F* h,Float_t fitmin,Float_t fitmax,Int_t N,Float_t* initpar){

  if(N<0||N>50){
    cout<<"Fits.C: Invalid polynomial degree."<<endl; 
    return NULL;
  }
  
  TString formula="[0]*x*[1]*[2]*(2/3.14159265358979312)/((x**2-[1]**2)**2+([1]**2)*([2]**2))";
  Int_t primpar=2;
  if(N==0||N>0)
    formula+=TString("+[")+(long)(primpar+1)+"]";
  if(N==1||N>1)
    formula+=TString("+[")+(long)(primpar+2)+"]*x";
  if(N>1){   
    for(Int_t n=2;n<=N;n++)
      formula+=TString("+[")+(long)(n+primpar+1)+"]*x**"+(long)n;
  } 
 
 
  TF1* f=new TF1((const char*)(TString("f")+h->GetName()),(const char*)(formula),fitmin,fitmax);
  f->SetParNames("Norm","M","#Gamma");
  for(Int_t n=0;n<=N;n++)
    f->SetParName(n+1+primpar,TString("c_{")+(long)(n)+"}");
  f->SetLineColor(2);   
  f->SetLineWidth(1);

  for(Int_t n=0;n<=N+primpar;n++)
    f->SetParameter(n,initpar[n]);
  

  h->Fit(f,"Q","pe",fitmin,fitmax);
    
  return f;

}
TF1* FitHistogramBreitWignerPolN(TH1F* h,Float_t fitmin,Float_t fitmax,Int_t N,Float_t* initpar){

  if(N<0||N>50){
    cout<<"Fits.C: Invalid polynomial degree."<<endl; 
    return NULL;
  }
  
  TString formula="[0]*([2]/(2*3.14159265358979312))/((x-[1])**2+([2]**2)/4)";
  Int_t primpar=2;
  if(N==0||N>0)
    formula+=TString("+[")+(long)(primpar+1)+"]";
  if(N==1||N>1)
    formula+=TString("+[")+(long)(primpar+2)+"]*x";
  if(N>1){   
    for(Int_t n=2;n<=N;n++)
      formula+=TString("+[")+(long)(n+primpar+1)+"]*x**"+(long)n;
  } 
 
  cout<<"formula ="<<formula<<endl;
  TF1* f=new TF1((const char*)(TString("f")+h->GetName()),(const char*)(formula),fitmin,fitmax);
  f->SetParNames("Norm","M","#Gamma");
  for(Int_t n=0;n<=N;n++)
    f->SetParName(n+1+primpar,TString("c_{")+(long)(n)+"}");
  f->SetLineColor(2);   
  f->SetLineWidth(1);

  
  for(Int_t n=0;n<=N+primpar;n++)
    if(initpar)f->SetParameter(n,initpar[n]);
    else f->SetParameter(n,1);
  

  h->Fit(f,"Q","pe",fitmin,fitmax);
    
  return f;

}

TF1* FitHistogramDs1Resolution(TH1F* h,Float_t fitxmin,Float_t fitxmax){
  Float_t Mean,Sigma,Norm;
   
  Mean=(float)h->GetBinCenter(h->GetMaximumBin()); 
  Sigma=(float)h->GetRMS();
  Norm=(float)h->GetBinContent(h->GetMaximumBin());

 

  ////////3 gaussians
  TF1* f=new TF1((const char*)(TString("f")+h->GetName()),
		 "[0]*exp(-.5*(x-[1])**2/[2]**2)+[3]*exp(-.5*(x-[1])**2/[4]**2)+[5]*exp(-.5*(x-[1])**2/[6]**2)",
		 fitxmin,fitxmax);


  f->SetParLimits(0,Norm/100,5*Norm);
  f->SetParLimits(1,Mean-Sigma,Mean+Sigma);
  f->SetParLimits(2,Sigma/20,10*Sigma);
  f->SetParLimits(3,Norm/100,5*Norm);
  f->SetParLimits(4,Sigma/20,10*Sigma);
  f->SetParLimits(5,Norm/100,5*Norm);
  f->SetParLimits(6,Sigma/20,10*Sigma);

  f->SetParNames("Norm1","Mean1","Sigma1","Norm2","Sigma2","Norm3","Sigma3");
  f->SetLineColor(2);   
  f->SetLineWidth(1);
  f->SetParameters(Norm,Mean,Sigma,Norm/2,2*Sigma,Norm/4,3*Sigma);



  h->Fit(f,"QI","pe",fitxmin,fitxmax);
  f->SetParameter(2,fabs(f->GetParameter(2)));
  f->SetParameter(4,fabs(f->GetParameter(4)));
  f->SetParameter(6,fabs(f->GetParameter(6)));
  //f->SetParameter(8,fabs(f->GetParameter(8)));
   
  
  return f;
}

void DrawDs1Resolution(TF1*f){

  int color1=3,color2=4,color3=5; 

  TF1*Fgaus1=new TF1((const char*)(TString("Fg1")+f->GetName()),"[0]*exp(-.5*(x-[1])**2/[2]**2)",f->GetXmin(),f->GetXmax());
  TF1*Fgaus2=new TF1((const char*)(TString("Ff")+f->GetName()),"[0]*exp(-.5*(x-[1])**2/[2]**2)",f->GetXmin(),f->GetXmax());
  TF1*Fgaus3=new TF1((const char*)(TString("Ff")+f->GetName()),"[0]*exp(-.5*(x-[1])**2/[2]**2)",f->GetXmin(),f->GetXmax());
  
  Fgaus1->SetParameters(f->GetParameter(0),f->GetParameter(1),f->GetParameter(2));
  Fgaus1->SetLineColor(color1);
  Fgaus1->SetLineWidth(1);
  Fgaus2->SetParameters(f->GetParameter(3),f->GetParameter(1),f->GetParameter(4));
  Fgaus2->SetLineColor(color2);
  Fgaus2->SetLineWidth(1);
  Fgaus3->SetParameters(f->GetParameter(5),f->GetParameter(1),f->GetParameter(6));
  Fgaus3->SetLineColor(color3);
  Fgaus3->SetLineWidth(1);


  Fgaus1->Draw("LSAME");
  Fgaus2->Draw("LSAME");
  Fgaus3->Draw("LSAME");
  f->SetLineWidth(2);
  f->Draw("Lsame");

  
  return;
}

#include "../CLASSES/JResiduals.h"

Float_t FitHistogram2BWPlusPolN(TH1F*h,Float_t fitmin,Float_t fitmax,Int_t N,TString phasefactor){
  if(h==NULL){cout<<"No histo"<<endl;return 0;}
  if(N<0||N>12){cout<<"N too large"<<endl;return 0;}
  
  //TString phasefactor=TString(PSFACTOR);

  TF1 F((const char*)(TString("F")+h->GetName()+(long)N),(const char*)(phasefactor+"*([0]*x/(pow((x*x-[1]*[1])/([1]*[2]),2) + 1) + [3]*x/(pow((x*x-[4]*[4])/([4]*[5]),2) + 1) + [6]+[7]*x+[8]*x**2+[9]*x**3+[10]*x**4+[11]*x**5+[12]*x**6+[13]*x**7+[14]*x**8+[15]*x**9+[16]*x**10+[17]*x**11+[18]*x**12)"),fitmin,fitmax);
  F.SetLineColor(4);
  F.SetLineWidth(2);
  F.SetNpx(10000);
  F.SetParLimits(0,0,1e6); 
  F.SetParLimits(1,.4,.42);    
  F.SetParLimits(2,.01,.1); 
  F.SetParLimits(3,0,1e6); 
  F.SetParLimits(4,.44,.46);    
  F.SetParLimits(5,.01,.1); 
  F.SetParLimits(6,-1e7,2e7);   
  F.SetParLimits(7,-1e7,2e7);    
  F.SetParLimits(8,-1e7,2e7); 
  F.SetParLimits(9,-1e7,2e7); 
  F.SetParLimits(10,-1e7,2e7);    
  F.SetParLimits(11,-1e7,2e7); 
  F.SetParLimits(12,-1e7,2e7); 
  F.SetParLimits(13,-1e7,2e7);    
  F.SetParLimits(14,-1e7,2e7);
  F.SetParLimits(15,-1e7,2e7);
  F.SetParLimits(16,-1e7,2e7); 
  F.SetParLimits(17,-1e7,2e7);
  F.SetParLimits(18,-1e7,2e7); 


  for(Int_t n=N+1;n<=12;n++)
    F.FixParameter(n+6,0);

  delete h->FindObject("stats");
  Int_t fitstatus=h->Fit(&F,"I","pe",fitmin,fitmax);
  if(fitstatus!=0){
    F.SetParLimits(6,-1e7,3e7);  
    fitstatus=h->Fit(&F,"I","pe",fitmin,fitmax);
  }

  JResiduals Residuals("res",h,&F);
  Residuals.GetResidualHisto()->DrawClone("pe");
  TLine line;
  line.DrawLine(fitmin,0,fitmax,0);

  Float_t chisquare=Residuals.GetChisquare();//  chisquare=F.GetChisquare();
  Int_t ndof=h->GetNbinsX()-F.GetNumberFreeParameters();
  if(ndof<=0)return 0;
  Float_t chiperdf=chisquare/ndof;

  if(fitstatus!=0)chiperdf=0;

  cout<<N<<" "<<chisquare<<" "<<ndof<<" "<<chiperdf<<endl;

  return chiperdf;
}

TF1*  FitHistogramDstarDM(TH1F*h,Float_t fitmin,Float_t fitmax,TF1* Signal,TF1* Bkg){
  if(h==NULL){cout<<"No histo"<<endl;return 0;}

  //TString phasefactor="((1/(2*(x+1.867)**2))*sqrt(((x+1.867)*(x+1.867) - 4.024)*((x+1.867)*(x+1.867)-2.986)))";
  TString phasefactor="(x>.139)*pow((x-.139)/(.145-.139),.5)";
  
  //BW +  gauss with same mean //x*[2]/((x*x-[1]*[1])**2+([1]*[2])**2)
  
  //2 BW
  //  TF1 signal((const char*)(TString(h->GetName())+"Signal"),(const char*)(phasefactor+"*[0]*((.5*[2])**2/((x-[1])**2+(.5*[2])**2)+[3]*(.5*[4])**2/((x-[1])**2+(.5*[4])**2))"),fitmin,fitmax);

  //BW+Gauss
  TF1 signal((const char*)(TString(h->GetName())+"Signal"),(const char*)(phasefactor+"*[0]*((.5*[2])**2/((x-[1])**2+(.5*[2])**2)+[3]*exp(-.5*(x-[1])**2/[4]**2))"),fitmin,fitmax);


  TF1 bkg((const char*)(TString(h->GetName())+"Bkg"),(const char*)(phasefactor+"*[5]*(1+[6]*x+[7]*x**2+[8]*x**3+[9]*x**4+[10]*x**5)"),fitmin,fitmax);
  bkg.SetLineColor(1);
  bkg.SetLineStyle(2);

  TF1* FitFunc=new TF1((const char*)(TString(h->GetName())+"FitFunc"),(const char*)(signal.GetExpFormula()+"+"+bkg.GetExpFormula()),fitmin,fitmax);
  FitFunc->SetParNames("norm1","mean","width","norm2","sigma");
  FitFunc->SetLineColor(4);
  FitFunc->SetNpx(10000);

  FitFunc->SetParLimits(0,.001,h->GetMaximum()*10.);
  FitFunc->SetParLimits(1,.145,.146);//FitFunc->FixParameter(1,.1454);//pdg value
  FitFunc->SetParLimits(2,.00001,.001);
  FitFunc->SetParLimits(3,.01,1);//FitFunc->FixParameter(3,0); 
  FitFunc->SetParLimits(4,.0001,.002);//FitFunc->FixParameter(4,.05); 

  //the background  
  FitFunc->SetParLimits(5,0,h->GetBinContent(h->GetXaxis()->GetNbins())); 
  FitFunc->SetParLimits(6,-1e6,2e6);
  FitFunc->SetParLimits(7,-1e6,2e6);
  FitFunc->SetParLimits(8,-1e6,2e6);
  FitFunc->SetParLimits(9,-1e6,2e6); 
  FitFunc->SetParLimits(10,-1e6,2e6);   
  if(!Bkg){
    FitFunc->FixParameter(5,0);  
    FitFunc->FixParameter(6,0);   
    FitFunc->FixParameter(7,0); 
    FitFunc->FixParameter(8,0);  
  }  
  FitFunc->FixParameter(9,0); 
  FitFunc->FixParameter(10,0);
  

  Int_t status=h->Fit(FitFunc,"","",fitmin,fitmax); 
  if(status!=0){
    FitFunc->SetParameter(1,.14411);
    h->Fit(FitFunc,"QL","",fitmin,fitmax); 
    cout<<"fit did not succed once"<<endl;
  }
  if(status!=0){
    FitFunc->SetParameter(1,.14412);
    h->Fit(FitFunc,"QL","",fitmin,fitmax); 
    cout<<"fit did not succed twice "<<endl;
  }
  if(status!=0){
    FitFunc->SetParameter(1,.14405);
    h->Fit(FitFunc,"QL","",fitmin,fitmax); 
    cout<<"fit did not succed three times"<<endl;
  }
  if(status!=0)cout<<" Fit failed++++++++++++++++++++++++++++"<<endl;
  else cout<<" Fit Succeded++++++++++++++++++++++++++++"<<endl;

 
  signal.SetParameters(FitFunc->GetParameter(0),
		       FitFunc->GetParameter(1),
		       FitFunc->GetParameter(2),
		       FitFunc->GetParameter(3),
		       FitFunc->GetParameter(4));  
  signal.SetParError(0,FitFunc->GetParError(0));
  signal.SetParError(1,FitFunc->GetParError(1));
  signal.SetParError(2,FitFunc->GetParError(2));
  signal.SetParError(3,FitFunc->GetParError(3));
  signal.SetParError(4,FitFunc->GetParError(4));
  *Signal=signal;

  bkg.SetParameters(0,0,0,0,0,
		    FitFunc->GetParameter(5),
		    FitFunc->GetParameter(6),
		    FitFunc->GetParameter(7),
		    FitFunc->GetParameter(8),
		    FitFunc->GetParameter(9),
		    FitFunc->GetParameter(10));

  bkg.SetParError(5,FitFunc->GetParError(5));
  bkg.SetParError(6,FitFunc->GetParError(6));
  bkg.SetParError(7,FitFunc->GetParError(7));
  bkg.SetParError(8,FitFunc->GetParError(8));
  bkg.SetParError(9,FitFunc->GetParError(9));
  bkg.SetParError(10,FitFunc->GetParError(10));
  if(Bkg)*Bkg=bkg;



  return FitFunc;
}


TF1*  FitHistogramDstar0DM(TH1F*h,Float_t fitmin,Float_t fitmax,TF1* Signal,TF1* Bkg){
  if(h==NULL){cout<<"No histo"<<endl;return 0;}
 
  TString phasefactor="((1/(2*(x+1.867)**2))*sqrt(((x+1.867)*(x+1.867) - 4.008)*((x+1.867)*(x+1.867)-2.9998)))";//D0 pi0: 2-body
  
  ///double gauss with same mean
  TF1 signal((const char*)(TString(h->GetName())+"Signal"),(const char*)(phasefactor+"*[0]*(exp(-.5*((x-[1])/[2])**2)+[3]*exp(-.5*((x-[1])/([4]))**2))"),fitmin,fitmax);

  TF1 bkg((const char*)(TString(h->GetName())+"Bkg"),(const char*)(phasefactor+"*[5]*(1+[6]*x+[7]*x**2+[8]*x**3+[9]*x**4+[10]*x**5)"),fitmin,fitmax);
  bkg.SetLineColor(1);
  bkg.SetLineStyle(2);

  TF1* FitFunc=new TF1((const char*)(TString(h->GetName())+"FitFunc"),(const char*)(signal.GetExpFormula()+"+"+bkg.GetExpFormula()),fitmin,fitmax);
  FitFunc->SetLineColor(4);
  FitFunc->SetNpx(10000);

  FitFunc->SetParLimits(0,1e2,1e9);
  FitFunc->SetParLimits(1,.141,.143);
  //FitFunc->FixParameter(1,.14212);//pdg value
  FitFunc->SetParLimits(2,.0001,.005);
  FitFunc->SetParLimits(3,.01,10);
  FitFunc->SetParLimits(4,.0001,.05);  


  //the background  
  FitFunc->SetParLimits(5,1e2,1e9);
  FitFunc->SetParLimits(6,0,2e4);
  FitFunc->SetParLimits(7,-1e4,0);
  FitFunc->SetParLimits(8,-1e4,0);
  FitFunc->SetParLimits(9,0,2e4);
  FitFunc->SetParLimits(10,-1e4,2e4);
  
  
  Int_t status=h->Fit(FitFunc,"","",fitmin,fitmax); 
  if(status!=0){
    FitFunc->SetParameter(1,.14211);
    h->Fit(FitFunc,"","",fitmin,fitmax); 
  }
  if(status!=0){
    FitFunc->SetParameter(1,.14212);
    h->Fit(FitFunc,"","",fitmin,fitmax); 
  }
  if(status!=0){
    FitFunc->SetParameter(1,.14205);
    h->Fit(FitFunc,"","",fitmin,fitmax); 
  }
 
  signal.SetParameters(FitFunc->GetParameter(0),FitFunc->GetParameter(1),FitFunc->GetParameter(2),FitFunc->GetParameter(3),FitFunc->GetParameter(4));  
  signal.SetParError(0,FitFunc->GetParError(0));
  signal.SetParError(1,FitFunc->GetParError(1));
  signal.SetParError(2,FitFunc->GetParError(2));
  signal.SetParError(3,FitFunc->GetParError(3));
  signal.SetParError(4,FitFunc->GetParError(4));
  *Signal=signal;

  bkg.SetParameters(0,0,0,0,0,FitFunc->GetParameter(5),FitFunc->GetParameter(6),FitFunc->GetParameter(7),FitFunc->GetParameter(8),FitFunc->GetParameter(9),FitFunc->GetParameter(10));
  bkg.SetParError(5,FitFunc->GetParError(5));
  bkg.SetParError(6,FitFunc->GetParError(6));
  bkg.SetParError(7,FitFunc->GetParError(7));
  bkg.SetParError(8,FitFunc->GetParError(8));
  bkg.SetParError(9,FitFunc->GetParError(9));
  bkg.SetParError(10,FitFunc->GetParError(10));
  *Bkg=bkg;
  
  return FitFunc;
}


