#include "../Globals.h"
#include "../FUNCTIONS/SetHisto.C"
#include <TF2.h>


Float_t xmin=2.260;
Float_t xmax=2.660;
Float_t M=2.460; 
Float_t W=.05;  
#define NPHIS 12
#define NSBR 12

TF1 SigRe("SigRe","[0]*(([1]*[1]-x*x)*cos([3])+x*[2]*sin([3]))/((x*x-[1]*[1])*(x*x-[1]*[1])+x*x*[2]*[2])",xmin,xmax);
TF1 SigIm("SigIm","[0]*(-x*[2]*cos([3])+([1]*[1]-x*x)*sin([3]))/((x*x-[1]*[1])*(x*x-[1]*[1])+x*x*[2]*[2])",xmin,xmax);  
TF1 FBkg("FBkg","[0]*([1]+[2]*(x-[3])*exp(-x/[4]))",xmin,xmax);
TF1 Func("Func","[0]+[1]*x+[2]/((x-[3])**2+(.5*[4])**2)",xmin,xmax);
TF1 FuncBkg("Func","[0]+[1]*x",xmin,xmax);
TF1 FuncSig("Func","[0]/((x-[1])**2+(.5*[2])**2)",xmin,xmax);

TH1F HMean("HMean","",NPHIS,0,mypi);

void Interfere(Float_t Sig1=1.,Float_t P1=0,Float_t Sig2=1.,Float_t P2=.0,Float_t Sig3=1., Float_t P3=.0,Float_t Bkg=1.,Float_t BkgPhase=0.){

  
  Float_t Mass[3];
  Mass[0]=2.42;
  Mass[1]=2.46;
  Mass[2]=2.43;
  Float_t Width[3];     
  Width[0]=.03;  
  Width[1]=.05;  
  Width[2]=.3; 


  Float_t S[3];
  S[0]=Sig1;
  S[1]=Sig2;
  S[2]=Sig3;
  

  Float_t P[3];
  P[0]=P1;
  P[1]=P2;
  P[2]=P3;


  //Signal= Breit-Wigner Amplitude= 1./(M*M-x*x+i*x*W)
  TF1 SigRe("SigRe","[0]*(([1]*[1]-x*x)*cos([3])+x*[2]*sin([3]))/((x*x-[1]*[1])*(x*x-[1]*[1])+x*x*[2]*[2])",xmin,xmax);
  TF1 SigIm("SigIm","[0]*(-x*[2]*cos([3])+([1]*[1]-x*x)*sin([3]))/((x*x-[1]*[1])*(x*x-[1]*[1])+x*x*[2]*[2])",xmin,xmax);  
  
  TH1F* HSig[3];
  for(Int_t i=0;i<3;i++){
    HSig[i]=new TH1F(TString("HSig")+(long)i,"",1000,xmin,xmax);    
    cout<<" Model "<<i<<" Ampl="<<S[i]<<" Mass="<<Mass[i]<<"  Width="<<Width[i]<<"  Phase="<<P[i]<<endl;
  }

  //Background=Bkg*exp(i*BkgP)
  TF1 FBkg("FBkg","[0]*([1]+[2]*(x-[3])*exp(-x/[4]))",xmin,xmax);
  FBkg.SetParameters(Bkg,1,0,0,0);
  if(Bkg<0)FBkg.SetParameters(-Bkg,0,1,xmin,.25);
    
  TH1F HBkg("HBkg","Background",1000,xmin,xmax);
  cout<<" Bkg Phase"<<BkgPhase<<endl;


  TH1F HI("HI","Intensity",1000,xmin,xmax);
  
  for(Int_t i=1;i<=HI.GetXaxis()->GetNbins();i++){
    Float_t x=HI.GetXaxis()->GetBinCenter(i); 

    Float_t Re=0;
    Float_t Im=0;

    //Add sig1   
    for(Int_t j=0;j<3;j++){
      if(S[j]>0){
	SigRe.SetParameters(S[j],Mass[j],Width[j],P[j]);
	SigIm.SetParameters(S[j],Mass[j],Width[j],P[j]);   
	Re+=SigRe.Eval(x);
	Im+=SigIm.Eval(x);    
	HSig[j]->SetBinContent(i,SigRe.Eval(x)*SigRe.Eval(x)+SigIm.Eval(x)*SigIm.Eval(x));
      }
    }


    Re+=FBkg.Eval(x)*cos(BkgPhase);
    Im+=FBkg.Eval(x)*sin(BkgPhase);
    HBkg.SetBinContent(i,FBkg.Eval(x)*FBkg.Eval(x));


    //The Intensity=|Signal+Background|^2
    Float_t Intensity=Re*Re+Im*Im;
    HI.SetBinContent(i,Intensity);

  }
  

  //Make plots
  TCanvas C("Interference");
  C.Print("Interference.ps[");

  TLine Line;

  C.Clear();
  HBkg.GetYaxis()->SetRangeUser(0,HBkg.GetMaximum());  
  HSig[0]->GetYaxis()->SetRangeUser(0,HSig[0]->GetMaximum());
  if(HBkg.GetMaximum()>HSig[0]->GetMaximum()){
    HBkg.Draw();
    HSig[0]->Draw("same");  
  }else{
    HSig[0]->Draw();  
    HBkg.Draw("same");
  }
  HSig[1]->Draw("same");  
  HSig[2]->Draw("same");  
  C.Print("Interference.ps");

  
  C.Clear();
  HI.GetYaxis()->SetRangeUser(0,HI.GetMaximum());
  HI.Draw(); 
  Line.DrawLine(xmin,0,xmax,0);
  C.Print("Interference.ps");


  C.Print("Interference.ps]");
  
}


TH1F* InterfereRandom(Float_t Bkg=1.,Float_t PhaseMean=2.,Float_t PhaseSpread=1.,Int_t N=1000,TF1** FP=0){
  
  Float_t Sig1=1.; 
  Float_t S=Sig1;
  Float_t P=0;
  
  Int_t nbins=200;

  //Signal= Breit-Wigner Amplitude= 1./(M*M-x*x+i*x*W)

  ////the phase distribution
  //TF1 SigP("SigP","(1+[1]*sin((x-[0]+3.14159)/2))**2+(1+[1]*sin(-(x-[0]+3.14159)/2))**2",0,2*mypi);//single peak 
  //TF1 SigP("SigP","(1+[1]*sin((x-[0]+3.14159/2)))**2+(1+[1]*sin(-(x-[0]+3.14159/2)))**2+(1+[1]*sin((x-[0]-3.14159/2)))**2+(1+[1]*sin(-(x-[0]-3.14159/2)))**2",0,2*mypi);//peak with reflection traveling in same direction

  TF1* SigP=new TF1("SigP","([1]+cos(x-[0]))**2+([1]+cos(-x-[0]))**2",0,2*mypi);//peak with reflection traveling in oposite direction
  SigP->SetParameter(0,PhaseMean);
  SigP->SetParameter(1,PhaseSpread);
  //SigP->GetXaxis()->SetTitle("phase difference");
  
  TH1F HSig(TString("HSig"),"",nbins,xmin,xmax);
    

  FBkg.SetParameters(Bkg,1,0,0,0);
  //FBkg.SetParameters(Bkg,0,1,xmin,.25);
  Float_t BkgPhase=0.;
  TH1F HBkg("HBkg","Background",nbins,xmin,xmax);

  TH1F HIntensity("HIntensity","Intensity: with interference",nbins,xmin,xmax);
  SetHistoXYLabels(&HIntensity,"m","");
  TH1F HData("HData","Distribution",nbins,xmin,xmax);
  SetHistoXYLabels(&HData,"m","");

  TH1F HIntensityN("HIntensityN","Intensity: without interference",nbins,xmin,xmax);
  SetHistoXYLabels(&HIntensityN,"m","");
  TH1F HDataN("HDataN","Distribution",nbins,xmin,xmax);
  SetHistoXYLabels(&HDataN,"m","");
  
  for(Int_t e=0;e<N;e++){ 
    
    //randomize the phase;
    P=SigP->GetRandom();
    
    SigRe.SetParameters(S,M,W,P);
    SigIm.SetParameters(S,M,W,P);   
  
    for(Int_t i=1;i<=HIntensity.GetXaxis()->GetNbins();i++){
      
      Float_t x=HIntensity.GetXaxis()->GetBinCenter(i); 
      Float_t SRe=0;
      Float_t SIm=0;
      Float_t BRe=0;
      Float_t BIm=0;
      Float_t Re=0;
      Float_t Im=0;

      SRe=SigRe.Eval(x);
      SIm=SigIm.Eval(x);    	
 
      BRe=FBkg.Eval(x)*cos(BkgPhase);
      BIm=FBkg.Eval(x)*sin(BkgPhase);      
       
      HIntensityN.SetBinContent(i,SRe*SRe+SIm*SIm+BRe*BRe+BIm*BIm);

      //The Intensity=|Signal+Background|^2
      Re=SRe+BRe;
      Im=SIm+BIm;      
      HIntensity.SetBinContent(i,Re*Re+Im*Im);
    }
   
    
    HIntensityN.Scale(1./HIntensityN.Integral());  
    HDataN.Fill(HIntensityN.GetRandom());

    HIntensity.Scale(1./HIntensity.Integral());    
    HData.Fill(HIntensity.GetRandom());
    
  }
    

  //Make plots
  TCanvas C("Interference");
  C.Print("Interference.ps[");


  C.Clear();
  SigP->GetYaxis()->SetRangeUser(0,1.2*SigP->GetMaximum());
  SigP->GetXaxis()->SetTitle("#phi (rad)");
  SigP->Draw();
  C.Print("Interference.ps");
  *FP=SigP;

  //interference shapes
  TObject* Htemp[6];
  C.Clear();
  for(Int_t e=0;e<=5;e++){ 
    P=e*mypi/5.;    
    SigRe.SetParameters(S,M,W,P);
    SigIm.SetParameters(S,M,W,P);     
    for(Int_t i=1;i<=HIntensity.GetXaxis()->GetNbins();i++){      
      Float_t x=HIntensity.GetXaxis()->GetBinCenter(i); 
      Float_t Re=0;
      Float_t Im=0;
      Re=SigRe.Eval(x)+FBkg.Eval(x)*cos(BkgPhase);
      Im=SigIm.Eval(x)+FBkg.Eval(x)*sin(BkgPhase);      
      HIntensity.SetBinContent(i,Re*Re+Im*Im);
    }   
    HIntensity.Scale(1./HIntensity.Integral());    
    HIntensity.GetYaxis()->SetRangeUser(0,HIntensity.GetMaximum()*1.5);
    HIntensity.SetLineColor(e+1);
    Htemp[e]=HIntensity.DrawClone((e==0)?"":"same");     
  }
  C.Print("Interference.ps");  
  for(Int_t e=0;e<=5;e++)
    delete Htemp[e];


  
  Func.SetParNames("p0","p1","N","M","W");
  Func.SetParLimits(0,0,1000);
  Func.SetParLimits(1,-100,100);
  Func.SetParLimits(2,.001,100.);
  Func.SetParLimits(3,2.360,2.560);
  Func.SetParLimits(4,.001,.1);
  Func.SetParameters(1,1,1,2.46,.05);
  
  
  HData.GetYaxis()->SetRangeUser(0,HData.GetMaximum());
  HData.Fit(&Func);
  C.Clear();
  HData.Draw();
  C.Print("Interference.ps");


  // //no interference
  C.Clear();
  HIntensityN.GetYaxis()->SetRangeUser(0,HIntensityN.GetMaximum());
  HIntensityN.Draw(); 
  C.Print("Interference.ps");

  
  HDataN.GetYaxis()->SetRangeUser(0,HDataN.GetMaximum());
  HDataN.Fit(&Func);
  C.Clear();
  HDataN.Draw();
  C.Print("Interference.ps");

  C.Print("Interference.ps]");
  

  return (TH1F*)HData.Clone();
}


void MakeInterferencePars(Int_t Nevents=1000,Float_t spread=1,TString nametag=""){

  TFile File(TString("InterferencePars")+nametag+".root","recreate");
  
  TF1* FP=0;
  
  for(Int_t i=1;i<=NPHIS;i++){
    TH1F*H=InterfereRandom(10,HMean.GetBinCenter(i),spread,Nevents,&FP);
    TF1*F=(TF1*)H->FindObject("Func");
    if(!F)return;

    H->SetName(TString("Histo")+(long)i);
    H->Write();
    F->SetName(TString("Func")+(long)i);
    F->Write();
    FP->SetName(TString("PhaseFunc")+(long)i);
    FP->Write();
  }  

  File.ls();
  File.Close();
  
}

void PlotInterferencePars(TString nametag=""){

  TFile File(TString("InterferencePars")+nametag+".root","read");
  File.ls();

  HMean.GetYaxis()->SetTitle("Mean (GeV)");
  HMean.GetXaxis()->SetTitle("#phi");

  TH1F HWidth("HWidth","",NPHIS,0,mypi);
  HWidth.GetYaxis()->SetTitle("Width (GeV)");
  HWidth.GetXaxis()->SetTitle("#phi");

  TH1F HNorm("HNorm","",NPHIS,0,mypi);
  HNorm.GetYaxis()->SetTitle("Yield");
  HNorm.GetXaxis()->SetTitle("#phi");

  TH1F*H=0;
  TF1*F=0;
  TF1* FP=0;
  
  //Make plots
  TCanvas C("InterferencePars");
  TCanvas C2("InterferencePars2");
  TString filename=TString("InterferencePars")+nametag+".ps";
  C.Print(filename+"[");
  
  C.Clear();
  C.Divide(3,(int)NPHIS/3);
  C2.Clear();
  C2.Divide(3,(int)NPHIS/3);

  for(Int_t i=1;i<=NPHIS;i++){
    H=(TH1F*)File.Get(TString("Histo")+(long)i);
    F=(TF1*)File.Get(TString("Func")+(long)i);
    FP=(TF1*)File.Get(TString("PhaseFunc")+(long)i);
    if(!F||!H||!FP)return;
    HMean.SetBinContent(i,F->GetParameter("M"));
    HMean.SetBinError(i,F->GetParError(F->GetParNumber("M")));
    HWidth.SetBinContent(i,F->GetParameter("W"));
    HWidth.SetBinError(i,F->GetParError(F->GetParNumber("W")));
    HNorm.SetBinContent(i,F->GetParameter("N"));
    HNorm.SetBinError(i,F->GetParError(F->GetParNumber("N")));

    
    C.cd(i);
    FP->SetTitle("");
    FP->GetXaxis()->SetTitle("#phi (rad)");
    FP->Draw();
    FP->GetYaxis()->SetRangeUser(0,1.2*FP->GetMaximum());
 
    C2.cd(i);
    H->SetTitle("");
    H->Draw();

  }  
  C.Print(filename);
  C2.Print(filename);

  File.Close();

  C.cd();
  C.Clear();
  HMean.GetYaxis()->SetRangeUser(2.440,2.480);
  HMean.Draw("pe");
  C.Print(filename);
  
  C.Clear();
  HWidth.GetYaxis()->SetRangeUser(.04,.06);
  HWidth.Draw("pe");
  C.Print(filename);

  C.Clear();
  HNorm.GetYaxis()->SetRangeUser(.0,1.2*HNorm.GetMaximum());
  HNorm.Draw("pe");
  C.Print(filename);
  

  C.Print(filename+"]"); 
}

void MakeInterferenceParsVsBkg(Int_t Nevents=1000,Float_t spread=10,TString nametag=""){
  Float_t phicenter=mypi;

  TFile File(TString("InterferenceParsVsBkg")+nametag+".root","recreate");
  
  TF1*FP;
  TF1*F;
  TH1F*H;
  for(Int_t i=1;i<=NSBR;i++){
    H=InterfereRandom(.5*(i+6),phicenter,spread,Nevents,&FP);
    F=(TF1*)H->FindObject("Func");
    if(!F)return;

    H->SetName(TString("Histo")+(long)i);
    H->Write();
    F->SetName(TString("Func")+(long)i);
    F->Write();
    FP->SetName(TString("PhaseFunc")+(long)i);
    FP->Write();

  }  
  
  
  File.ls();
  File.Close();
}


#include "TGraphErrors.h"

void PlotInterferenceParsVsBkg(TString nametag=""){


  TFile File(TString("InterferenceParsVsBkg")+nametag+".root","read");
  File.ls();
  

  //Make plots
  TCanvas C("InterferenceParsVsBkg");
  TString filename=TString("InterferenceParsVsBkg")+nametag+".ps";
  C.Print(filename+"[");
  
  TH1F HMean("HMean","",NSBR,.5,NSBR+.5);
  HMean.GetYaxis()->SetTitle("Mean (GeV)");
  HMean.GetXaxis()->SetTitle("Background Strength (arbitrary units)");

  TGraphErrors GMeanSB;


  TH1F HWidth("HWidth","",NSBR,.5,NSBR+.5);
  HWidth.GetYaxis()->SetTitle("Width (GeV)");
  HWidth.GetXaxis()->SetTitle("Background Strength (arbitrary units)");

  TH1F HNorm("HNorm","",NSBR,.5,NSBR+.5);
  HNorm.GetYaxis()->SetTitle("Yield");
  HNorm.GetXaxis()->SetTitle("Background Strength (arbitrary units)");

  
  C.Clear();
  C.Divide(3,NSBR/3);
  TText text;
  text.SetTextSize(.2);
 
  TH1F*H=0;
  TF1*F=0;
  TF1* FP=0;
  
  Int_t ngood=0;

  for(Int_t i=1;i<=NSBR;i++){
        
    H=(TH1F*)File.Get(TString("Histo")+(long)i);
    F=(TF1*)File.Get(TString("Func")+(long)i);
    FP=(TF1*)File.Get(TString("PhaseFunc")+(long)i);
    if(!F||!H||!FP)return;
    
    HMean.SetBinContent((i),F->GetParameter("M"));
    HMean.SetBinError((i),F->GetParError(F->GetParNumber("M")));
    HWidth.SetBinContent((i),F->GetParameter("W"));
    HWidth.SetBinError((i),F->GetParError(F->GetParNumber("W")));
    HNorm.SetBinContent((i),F->GetParameter("N"));
    HNorm.SetBinError((i),F->GetParError(F->GetParNumber("N")));

    
    FuncBkg.SetParameters(F->GetParameter(0),F->GetParameter(1));
    FuncSig.SetParameters(F->GetParameter(2),F->GetParameter(3),F->GetParameter(4));    
    

    C.cd((i));
    C.GetPad(i)->SetTopMargin(.09);
    C.GetPad(i)->SetBottomMargin(.2);
    C.GetPad(i)->SetLeftMargin(.05);
    C.GetPad(i)->SetRightMargin(.18);    

    H->SetTitle("");
    H->Draw();
    text.DrawTextNDC(.1,.7,TString("")+(long)(i));

    Float_t SB=FuncSig.Eval(F->GetParameter("M"))/FuncBkg.Eval(F->GetParameter("M"));
    if(SB<.1||SB>10)continue;
    GMeanSB.SetPoint(ngood,SB,F->GetParameter("M"));
    GMeanSB.SetPointError(ngood,0,F->GetParError(F->GetParNumber("M")));
    ngood++;
  
  }  
  C.Print(filename);


  File.Close();

  C.Clear();
  FP->SetTitle("");
  FP->GetYaxis()->SetTitle("Arbitrary units");
  FP->GetXaxis()->SetTitle("#phi (rad)");
  FP->Draw(); 
  FP->GetYaxis()->SetRangeUser(0,1.2*FP->GetMaximum());
  C.Print(filename);
  
  C.Clear();
  HMean.GetYaxis()->SetRangeUser(2.440,2.480);
  HMean.Draw("pe");
  C.Print(filename);
  

  GMeanSB.GetYaxis()->SetTitle("Mean (GeV)");
  GMeanSB.GetXaxis()->SetTitle("Signal/Background ratio");
  GMeanSB.GetYaxis()->SetRangeUser(2.440,2.480);
  C.Clear();
  GMeanSB.Draw("ape");
  C.Print(filename);

  C.Clear();
  HWidth.GetYaxis()->SetRangeUser(.04,.06);
  HWidth.Draw("pe");
  C.Print(filename);

  C.Clear();
  HNorm.GetYaxis()->SetRangeUser(.0,1.2*HNorm.GetMaximum());
  HNorm.Draw("pe");
  C.Print(filename);
  

  C.Print(filename+"]"); 


}



void PlotInterferenceVsAngle(Float_t B,Float_t phase){


  TCanvas C("Interference");
  TString filename=TString("InterferenceVsAngle.ps");
  C.Print(filename+"[");

  Float_t mean=2.46;
  Float_t width=.03;
  TF2 FI("FI","[0]**2 + (1-2*[0]*(1-y**2)*((x-[1])*cos([3])-[2]*sin([3])/2.))/((x-[1])**2+.25*[2]**2)",2.26,2.66,-1,1);
  FI.SetParameters(B,mean,width,phase);
 
  TH2F HI("HI","HI",200,2.26,2.66,100,-1,1);
  for(Int_t i=1;i<=200;i++)
    for(Int_t j=1;j<=100;j++)
      HI.SetBinContent(i,j,FI.Eval(HI.GetXaxis()->GetBinCenter(i),HI.GetYaxis()->GetBinCenter(j)));
		       
  C.Clear();
  HI.Draw("colz");
  C.Print(filename);

  TH1D* HM[10];
  C.Clear();
  TLine line;line.SetLineColor(1);
  for(Int_t i=0;i<5;i++){       
    HM[i]=HI.ProjectionX(TString("HM")+(long)i,1+i*10,(i+1)*10);
    HM[i]->SetLineColor(i+1);  
    if(i==0)HM[i]->GetYaxis()->SetRangeUser(0,HM[0]->GetMaximum()*2.);
    HM[i]->SetStats(0);
    HM[i]->Draw((i==0)?"":"same");  
  }
  line.DrawLine(mean,0,mean,HM[0]->GetMaximum());
  C.Print(filename);


  C.Clear();
  for(Int_t i=5;i<10;i++){       
    HM[i]=HI.ProjectionX(TString("HM")+(long)i,1+i*10,(i+1)*10);
    HM[i]->SetLineColor(5-(i-5));  
    if(i==5)HM[i]->GetYaxis()->SetRangeUser(0,HM[0]->GetMaximum());//SetRangeUser changes Maximum
    HM[i]->SetStats(0);
    HM[i]->Draw((i==5)?"":"same");
  }               
  line.DrawLine(mean,0,mean,HM[0]->GetMaximum());
  C.Print(filename);

  for(Int_t i=0;i<10;i++)
    delete HM[i];

  C.Print(filename+"]"); 
}



void PlotInterferenceWithBkg(Float_t B,Float_t phase,Float_t Smear){

  TCanvas C("Interference");
  TString filename=TString("InterferenceVsAngle.ps");
  C.Print(filename+"[");

  Float_t mean=2.46;
  Float_t width=.03;
  TF2 FSR("FSR","(3*cos(y)*cos(y)-1)*(x-[0])/((x-[0])**2+.25*[1]**2)",2.26,2.66,-1,1);
  FSR.SetParameters(mean,width);
  TF2 FSI("FSI","(3*cos(y)*cos(y)-1)*[1]/((x-[0])**2+.25*[1]**2)",2.26,2.66,-1,1);
  FSI.SetParameters(mean,width);
  
  TF2 FBR("FBR","cos([0])*(1+y+y*y)*(1-.5*(x-2.26)/(2.66-2.26))",2.26,2.66,-1,1);
  FBR.SetParameter(0,phase);
  TF2 FBI("FBI","sin([0])*(1+y+y*y)*(1-.5*(x-2.26)/(2.66-2.26))",2.26,2.66,-1,1);
  FBI.SetParameter(0,phase);

  
  TF1* SigP=new TF1("SigP","([1]+cos(x-[0]))**2+([1]+cos(-x-[0]))**2",0,2*mypi);//peak with reflection traveling in oposite direction
  SigP->SetParameter(0,phase);
  SigP->SetParameter(1,Smear);



  Float_t IR=0;
  Float_t II=0;
  TH2F HI("HI","HI",200,2.26,2.66,100,-1,1);
  for(Int_t i=1;i<=200;i++)
    for(Int_t j=1;j<=100;j++){
 
      for(Int_t p=0;p<100;p++){
	FBR.SetParameter(0,-mypi+p*mypi*2./100.);	
	FBI.SetParameter(0,-mypi+p*mypi*2./100.);
	
	IR=FSR.Eval(HI.GetXaxis()->GetBinCenter(i),HI.GetYaxis()->GetBinCenter(j))+B*FBR.Eval(HI.GetXaxis()->GetBinCenter(i),HI.GetYaxis()->GetBinCenter(j));
	II=FSI.Eval(HI.GetXaxis()->GetBinCenter(i),HI.GetYaxis()->GetBinCenter(j))+B*FBI.Eval(HI.GetXaxis()->GetBinCenter(i),HI.GetYaxis()->GetBinCenter(j));
	
	HI.SetBinContent(i,j,HI.GetBinContent(i,j)+SigP->Eval(-mypi+p*mypi*2./100.)*(IR*IR+II*II));
      }

     
    }

  C.Clear();
  C.Divide(2,2);
  C.cd(1);
  HI.Draw("colz");
  C.cd(2);
  TH1D*HP=HI.ProjectionY();
  HP->GetYaxis()->SetRangeUser(0,HP->GetBinContent(HP->GetMaximumBin()));
  HP->Draw();
  C.cd(3);
  HP=HI.ProjectionX();
  HP->GetYaxis()->SetRangeUser(0,HP->GetBinContent(HP->GetMaximumBin()));
  HP->Draw();
  C.Print(filename);

  TH2F HIS("HIS","HIS",200,2.26,2.66,100,-1,1);
  for(Int_t i=1;i<=200;i++)
    for(Int_t j=1;j<=100;j++){
      IR=FSR.Eval(HIS.GetXaxis()->GetBinCenter(i),HIS.GetYaxis()->GetBinCenter(j));
      II=FSI.Eval(HIS.GetXaxis()->GetBinCenter(i),HIS.GetYaxis()->GetBinCenter(j));
      HIS.SetBinContent(i,j,IR*IR+II*II);
    }

  C.Clear();
  C.Divide(2,2);
  C.cd(1);
  HIS.Draw("colz");
  C.cd(2);
  HP=HIS.ProjectionY();
  HP->GetYaxis()->SetRangeUser(0,HP->GetBinContent(HP->GetMaximumBin()));
  HP->Draw();
  C.cd(3);
  HP=HIS.ProjectionX();
  HP->GetYaxis()->SetRangeUser(0,HP->GetBinContent(HP->GetMaximumBin()));
  HP->Draw();  
  C.Print(filename);



  TH2F HIB("HIB","HIB",200,2.26,2.66,100,-1,1);
  for(Int_t i=1;i<=200;i++)
    for(Int_t j=1;j<=100;j++){
      IR=FBR.Eval(HIB.GetXaxis()->GetBinCenter(i),HIB.GetYaxis()->GetBinCenter(j));
      II=FBI.Eval(HIB.GetXaxis()->GetBinCenter(i),HIB.GetYaxis()->GetBinCenter(j));
      HIB.SetBinContent(i,j,IR*IR+II*II);
    }


  C.Clear();
  C.Divide(2,2);
  C.cd(1);
  HIB.Draw("colz");
  C.cd(2);
  HP=HIB.ProjectionY();
  HP->GetYaxis()->SetRangeUser(0,HP->GetBinContent(HP->GetMaximumBin()));
  HP->Draw();
  C.cd(3);
  HP=HIB.ProjectionX();
  HP->GetYaxis()->SetRangeUser(0,HP->GetBinContent(HP->GetMaximumBin()));
  HP->Draw();  
  C.Print(filename);

  C.Clear();
  SigP->GetYaxis()->SetRangeUser(0,SigP->GetMaximum());
  SigP->Draw("ap");
  C.Print(filename);



  TH1D* HM[10];
  Float_t Ymax=0;
  TGraph GPeak;
  for(Int_t i=0;i<10;i++){       
    HM[i]=HI.ProjectionX(TString("HM")+(long)i,1+i*10,(i+1)*10);
    if(Ymax<HM[i]->GetMaximum())Ymax=HM[i]->GetMaximum();
    GPeak.SetPoint(i,-1+.2*(i+.5),HM[i]->GetBinCenter(HM[i]->GetMaximumBin()));
  }




  C.Clear();
  TLine line;line.SetLineColor(1);
  for(Int_t i=0;i<10;i++){       
    HM[i]->SetLineColor(i+1);  
    if(i==0)HM[i]->GetYaxis()->SetRangeUser(0,Ymax);
    HM[i]->SetStats(0);
    HM[i]->Draw((i==0)?"":"same");  
  }
  line.DrawLine(mean,0,mean,HM[0]->GetMaximum());
  C.Print(filename);


//   C.Clear();
//   for(Int_t i=5;i<10;i++){       
//     HM[i]->SetLineColor(5-(i-5));  
//     if(i==5)HM[i]->GetYaxis()->SetRangeUser(0,HM[0]->GetMaximum());//SetRangeUser changes Maximum
//     HM[i]->SetStats(0);
//     HM[i]->Draw((i==5)?"":"same");
//   }               
//   line.DrawLine(mean,0,mean,HM[0]->GetMaximum());
//   C.Print(filename);

  for(Int_t i=0;i<10;i++)
    delete HM[i];



  C.Clear();
  GPeak.GetYaxis()->SetRangeUser(mean-.015,mean+.015);
  GPeak.Draw("ap");
  line.DrawLine(-1,mean,1,mean);
  C.Print(filename);
  

  C.Print(filename+"]"); 
}


void PlotInterferenceVsAngle(Float_t B,Float_t phase,Float_t Smear){

  TCanvas C("Interference");
  TString filename=TString("InterferenceVsAngle.ps");
  C.Print(filename+"[");

  //Signal functions pieces
  Float_t mean=2.46;
  Float_t width=.03;
  TF1 FBW("FBW","1./((x-[0])**2+.25*[1]**2)",2.26,2.66);
  FBW.SetParameters(mean,width);
  
  TF1 FY20("FY20","sqrt(5.)*(cos(x)*cos(x)-1./3.)",-1,1);

  //Background function pieces
  TF2 FBf("FBf","(1+(y+1)**3+y*y)*(1-.5*(x-2.26)/(2.66-2.26))",2.26,2.66,-1,1);
  
  //Interference term
  TF1 FInt("FInt","(x-[0])*cos([2])-.5*[1]*sin([2])",2.26,2.66);
  FInt.SetParameters(mean,width,phase);

  //the phase distribution
  //TF1 SigP("SigP","([1]+cos(x-[0]))**2+([1]+cos(-x-[0]))**2",0,2*mypi);//peak with reflection traveling in oposite direction
  TF1 SigP("SigP","([1]+cos(x-[0]))**2",0,2*mypi);//peak with reflection traveling in oposite direction
  SigP.SetParameter(0,phase);
  SigP.SetParameter(1,Smear);


  TH2F HI("HI","HI",200,2.26,2.66,100,-1,1);
  for(Int_t i=1;i<=200;i++)
    for(Int_t j=1;j<=100;j++){
 
      for(Int_t p=0;p<100;p++){
	FBf.SetParameter(0,-mypi+p*mypi*2./100.);	
	FInt.SetParameter(2,-mypi+p*mypi*2./100.);
	Float_t x=HI.GetXaxis()->GetBinCenter(i);
	Float_t y=HI.GetYaxis()->GetBinCenter(j);

	HI.SetBinContent(i,j,HI.GetBinContent(i,j)
			 +SigP.Eval(-mypi+p*mypi*2./100.)*(
							   FBW.Eval(x)
							   +(B*FBf.Eval(x,y))*(B*FBf.Eval(x,y))
							   +B*FBf.Eval(x,y)*FY20.Eval(y)*FInt.Eval(x)*FBW.Eval(x)
							   )
			 );//
      }

     
    }

  
  C.Clear();
  C.Divide(2,2);
  C.cd(1);
  HI.Draw("colz");
  C.cd(2);
  TH1D*HP=HI.ProjectionY();
  HP->GetYaxis()->SetRangeUser(0,HP->GetBinContent(HP->GetMaximumBin()));
  HP->Draw();
  C.cd(3);
  HP=HI.ProjectionX();
  HP->GetYaxis()->SetRangeUser(0,HP->GetBinContent(HP->GetMaximumBin()));
  HP->Draw();
  C.Print(filename);

  /*
  TH2F HIS("HIS","HIS",200,2.26,2.66,100,-1,1);
  for(Int_t i=1;i<=200;i++)
    for(Int_t j=1;j<=100;j++){
      IR=FBW.Eval(HIS.GetXaxis()->GetBinCenter(i),HIS.GetYaxis()->GetBinCenter(j));
      II=FY20.Eval(HIS.GetXaxis()->GetBinCenter(i),HIS.GetYaxis()->GetBinCenter(j));
      HIS.SetBinContent(i,j,IR*IR+II*II);
    }

  C.Clear();
  C.Divide(2,2);
  C.cd(1);
  HIS.Draw("colz");
  C.cd(2);
  HP=HIS.ProjectionY();
  HP->GetYaxis()->SetRangeUser(0,HP->GetBinContent(HP->GetMaximumBin()));
  HP->Draw();
  C.cd(3);
  HP=HIS.ProjectionX();
  HP->GetYaxis()->SetRangeUser(0,HP->GetBinContent(HP->GetMaximumBin()));
  HP->Draw();  
  C.Print(filename);



  TH2F HIB("HIB","HIB",200,2.26,2.66,100,-1,1);
  for(Int_t i=1;i<=200;i++)
    for(Int_t j=1;j<=100;j++){
      IR=FBf.Eval(HIB.GetXaxis()->GetBinCenter(i),HIB.GetYaxis()->GetBinCenter(j));
      II=FBg.Eval(HIB.GetXaxis()->GetBinCenter(i),HIB.GetYaxis()->GetBinCenter(j));
      HIB.SetBinContent(i,j,IR*IR+II*II);
    }


  C.Clear();
  C.Divide(2,2);
  C.cd(1);
  HIB.Draw("colz");
  C.cd(2);
  HP=HIB.ProjectionY();
  HP->GetYaxis()->SetRangeUser(0,HP->GetBinContent(HP->GetMaximumBin()));
  HP->Draw();
  C.cd(3);
  HP=HIB.ProjectionX();
  HP->GetYaxis()->SetRangeUser(0,HP->GetBinContent(HP->GetMaximumBin()));
  HP->Draw();  
  C.Print(filename);


  */

  C.Clear();
  SigP.GetYaxis()->SetRangeUser(0,SigP.GetMaximum());
  SigP.Draw("ap");
  C.Print(filename);




  TH1D* HM[10];
  Float_t Ymax=0;
  TGraph GPeak;
  for(Int_t i=0;i<10;i++){       
    HM[i]=HI.ProjectionX(TString("HM")+(long)i,1+i*10,(i+1)*10);
    if(Ymax<HM[i]->GetMaximum())Ymax=HM[i]->GetMaximum();
    GPeak.SetPoint(i,-1+.2*(i+.5),HM[i]->GetBinCenter(HM[i]->GetMaximumBin()));
  }




  C.Clear();
  TLine line;line.SetLineColor(1);
  for(Int_t i=0;i<10;i++){       
    HM[i]->SetLineColor(i+1);  
    if(i==0)HM[i]->GetYaxis()->SetRangeUser(0,Ymax);
    HM[i]->SetStats(0);
    HM[i]->Draw((i==0)?"":"same");  
  }
  line.DrawLine(mean,0,mean,HM[0]->GetMaximum());
  C.Print(filename);


//   C.Clear();
//   for(Int_t i=5;i<10;i++){       
//     HM[i]->SetLineColor(5-(i-5));  
//     if(i==5)HM[i]->GetYaxis()->SetRangeUser(0,HM[0]->GetMaximum());//SetRangeUser changes Maximum
//     HM[i]->SetStats(0);
//     HM[i]->Draw((i==5)?"":"same");
//   }               
//   line.DrawLine(mean,0,mean,HM[0]->GetMaximum());
//   C.Print(filename);

  for(Int_t i=0;i<10;i++)
    delete HM[i];



  C.Clear();
  GPeak.GetYaxis()->SetRangeUser(mean-.015,mean+.015);
  GPeak.Draw("ap");
  line.DrawLine(-1,mean,1,mean);
  C.Print(filename);
  

  C.Print(filename+"]"); 
}


#include <TComplex.h>
void PlotSPDInterference(Float_t R,Float_t phase){

  TCanvas C("Interference");
  TString filename=TString("InterferenceSPD.ps");
  C.Print(filename+"[");

  //Signal functions pieces
//   Float_t meanS=2.400;
//   Float_t widthS=.200;
//   Float_t meanP=2.400;
//   Float_t widthP=.200;
  Float_t meanD=2.460;
  Float_t widthD=.030;
//   TF1 FSR("FSR","(x-[0])/((x-[0])**2+.25*[1]**2)",2.26,2.66);
//   FSR.SetParameters(meanS,widthS);
//   TF1 FSI("FSI","[1]/((x-[0])**2+.25*[1]**2)",2.26,2.66);
//   FSI.SetParameters(meanS,widthS);
//   TF1 FST("FST","1./((x-[0])**2+.25*[1]**2)",2.26,2.66);
//   FST.SetParameters(meanS,widthS);

  TF1 FSR("FSR","1",2.26,2.66);
  TF1 FSI("FSI","1",2.26,2.66);
  TF1 FST("FST","sqrt(2)",2.26,2.66);


//   TF2 FPR("FPR","y*(x-[0])/((x-[0])**2+.25*[1]**2)",2.26,2.66,-1,1);
//   FPR.SetParameters(meanP,widthP);
//   TF2 FPI("FPI","y*[1]/((x-[0])**2+.25*[1]**2)",2.26,2.66,-1,1);
//   FPI.SetParameters(meanP,widthP);
//   TF1 FPT("FPT","1./((x-[0])**2+.25*[1]**2)",2.26,2.66);
//   FPT.SetParameters(meanP,widthP);


  TF2 FPR("FPR","y*(1-.5*(x-2.26)/(2.66-2.26))",2.26,2.66,-1,1);
  TF2 FPI("FPI","y*(1-.5*(x-2.26)/(2.66-2.26))",2.26,2.66,-1,1);
  TF1 FPT("FPT","(1-.5*(x-2.26)/(2.66-2.26))**2",2.26,2.66);


  TF2 FDR("FDR","(3*y*y-1)*(x-[0])/((x-[0])**2+.25*[1]**2)",2.26,2.66,-1,1);
  FDR.SetParameters(meanD,widthD);
  TF2 FDI("FDI","(3*y*y-1)*[1]/((x-[0])**2+.25*[1]**2)",2.26,2.66,-1,1);
  FDI.SetParameters(meanD,widthD);
  TF1 FDT("FDT","1./((x-[0])**2+.25*[1]**2)",2.26,2.66);
  FDT.SetParameters(meanD,widthD);

  Int_t NXbins=1000;
  TH2F HI("HI","HI",NXbins,2.26,2.66,100,-1,1);
  for(Int_t i=1;i<=NXbins;i++)
    for(Int_t j=1;j<=100;j++){
      
      Float_t x=HI.GetXaxis()->GetBinCenter(i);
      Float_t y=HI.GetYaxis()->GetBinCenter(j);
      TComplex SA(FSR.Eval(x),FSI.Eval(x));
      TComplex PA(FPR.Eval(x,y),FPI.Eval(x,y));
      TComplex DA(FDR.Eval(x,y),FDI.Eval(x,y));
      TComplex Phase(cos(phase),sin(phase));

      //TComplex AInt=SA.Conjugate(SA)*DA*Phase;
      //HI.SetBinContent(i,j,FDT.Eval(x)+R*R*FST.Eval(x)+(2./(5.))*R*AInt.Re());//2=CrossTerms, 1/5=Avg.D      
      
      TComplex AInt=PA.Conjugate(PA)*DA*Phase;
      HI.SetBinContent(i,j,FDT.Eval(x)+R*R*FPT.Eval(x,y)+(2./(3.*5.))*R*AInt.Re());//2=CrossTerms, 1/3=Avg.P, 1/5=Avg.D

      //TComplex AInt=SA.Conjugate(SA)*PA*Phase;      
      //HI.SetBinContent(i,j,FPT.Eval(x)+R*R*FST.Eval(x)+(2./(3.))*R*AInt.Re());//2=CrossTerms, 1/3=Avg.P 


 //      TComplex ASPInt=SA.Conjugate(SA)*PA;
//       TComplex ASDInt=SA.Conjugate(SA)*DA;
//       TComplex APDInt=PA.Conjugate(PA)*DA;
//       HI.SetBinContent(i,j,FDT.Eval(x)+R*R*FPT.Eval(x)+R*R*FST.Eval(x)
// 		       +(2./(3.))*R*ASPInt.Re()+(2./(5.))*R*ASDInt.Re()+(2./(3.*5.))*R*APDInt.Re());

    }
  
  
  C.Clear();
  C.Divide(2,2);
  C.cd(1);
  HI.Draw("colz");
  C.cd(2);
  TH1D*HP=HI.ProjectionY();
  HP->GetYaxis()->SetRangeUser(0,HP->GetBinContent(HP->GetMaximumBin()));
  HP->Draw();
  C.cd(3);
  HP=HI.ProjectionX();
  HP->GetYaxis()->SetRangeUser(0,HP->GetBinContent(HP->GetMaximumBin()));
  HP->Draw();
  C.Print(filename);





  TH1D* HM[10];
  Float_t Ymax=0;
  TGraph GPeak;
  for(Int_t i=0;i<10;i++){       
    HM[i]=HI.ProjectionX(TString("HM")+(long)i,1+i*10,(i+1)*10);
    if(Ymax<HM[i]->GetMaximum())Ymax=HM[i]->GetMaximum();
    GPeak.SetPoint(i,-1+.2*(i+.5),HM[i]->GetBinCenter(HM[i]->GetMaximumBin()));
  }




  C.Clear();
  TLine line;line.SetLineColor(1);
  for(Int_t i=0;i<10;i++){       
    HM[i]->SetLineColor(i+1);  
    if(i==0)HM[i]->GetYaxis()->SetRangeUser(0,Ymax);
    HM[i]->SetStats(0);
    HM[i]->Draw((i==0)?"":"same");  
  }
  line.DrawLine(meanD,0,meanD,HM[0]->GetMaximum());
  C.Print(filename);



  for(Int_t i=0;i<10;i++)
    delete HM[i];



  C.Clear();
  GPeak.GetYaxis()->SetRangeUser(meanD-.015,meanD+.015);
  GPeak.Draw("ap");
  line.DrawLine(-1,meanD,1,meanD);
  C.Print(filename);
  

  C.Print(filename+"]"); 
}
