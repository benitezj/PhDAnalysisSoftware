//#include <iomanip>
//#include <stdio.h>
#include <TString.h>
#include <TCanvas.h>
#include <TLatex.h>
#include <TGraphErrors.h>
#include <TH1F.h>
//#include "TROOT.h"
#include "Riostream.h"
//#include <fstream>
//using namespace std;
//using std::ostream;
//using std::streambuf;



void PlotDMesonSpectrum(){



  TString Title[20];
  TString PDGName[20];
  TString Name[20];
  TString DPiDecays[20];
  TString DstPiDecays[20];
  Float_t Mass[20];
  TString Width[20];
  Float_t MassMeas[20];
  Float_t MassMeasEr[20];
  Float_t WidthMeas[20];
  Float_t WidthMeasEr[20];
  Int_t Spin[20];
  Int_t Parity[20];
  TString SpecNot[20];
  TString Helicity[20];
  Float_t SpinParXcord[20];
  for(Int_t i=0;i<20;i++){
    Name[i]==""; 
  }


  Float_t MCMassNeu[20];  //Neutral states
  Float_t MCMassChg[20];  //Charged states
  Float_t MCWidthNeu[20];  //Neutral states
  Float_t MCWidthChg[20];  //Charged states




  ///////////////////////////L=0 states
  //D0
  PDGName[0]="$D^{0}$";
  Title[0]="$D$";
  Name[0]="D("; 
  Mass[0]=1.880;
  Width[0]="0";
  MassMeasEr[0]=.0001;
  WidthMeas[0]=0.;
  WidthMeasEr[0]=0.;
  Spin[0]=0;
  Parity[0]=-1;
  SpecNot[0]="1^{1}S_{0}";
  MassMeas[0]=1.8645;
  DPiDecays[0]="-";
  DstPiDecays[0]="-";
  Helicity[0]="-";
  MCMassNeu[0]=1.8845;
  MCMassChg[0]=1.8869;
  MCWidthNeu[0]=0.;
  MCWidthChg[0]=0.;

  //D*0
  PDGName[1]="$D^{*0}$";
  Title[1]="$D^{*}$";
  Name[1]="D^{*}(";
  Mass[1]=2.040;
  Width[1]="0";
  MassMeasEr[1]=.0;
  WidthMeas[1]=0.0;
  Spin[1]=1;
  Parity[1]=-1;
  SpecNot[1]="1^{3}S_{1}";
  MassMeas[1]=2.007;
  DPiDecays[1]="-";
  DstPiDecays[1]="-";
  Helicity[1]="-";
  MCMassNeu[1]=2.007;
  MCMassChg[1]=2.010;
  MCWidthNeu[1]=0.;
  MCWidthChg[1]=0.;

  //D0 Radial excitation
  PDGName[2]="-";
  Title[2]="$D_{0}(2S)$";
  Name[2]="D_{0}(";
  Mass[2]=2.580;
  Width[2]="-";
  MassMeasEr[2]=0.;
  WidthMeas[2]=0.;
  WidthMeasEr[2]=0.;
  Spin[2]=0;
  Parity[2]=-1;
  SpecNot[2]="2^{1}S_{1}";  
  MassMeas[2]=0.;
  DPiDecays[2]="-";
  DstPiDecays[2]="P";
  Helicity[2]="$\\propto cos^2(\\theta)$";
  MCMassNeu[2]=0.;
  MCMassChg[2]=0.;
  MCWidthNeu[2]=0.;
  MCWidthChg[2]=0.;


  //D1* Radial excitation 
  PDGName[3]="-";
  Title[3]="$D^{*}_{1}(2S)$";
  Name[3]="D^{*}_{1}(";
  Mass[3]=2.640;
  Width[3]="-";
  MassMeasEr[3]=0.;
  WidthMeas[3]=0.;
  WidthMeasEr[3]=0.;
  Spin[3]=1;
  Parity[3]=-1;
  SpecNot[3]="1^{3}D_{1}"; 
  MassMeas[3]=0.;
  DPiDecays[3]="P";
  DstPiDecays[3]="P";
  Helicity[3]="$\\propto sin^2(\\theta)$";
  MCMassNeu[3]=0.;
  MCMassChg[3]=0.;
  MCWidthNeu[3]=0.;
  MCWidthChg[3]=0.;

  ///////////////////////////L=1 states 
// D_0*+ 	10411 	2.308 	0.276 	0.303
// D_0*0 	10421 	2.308 	0.276 	0.299
// D_1+ 	10413 	2.427 	0.028 	0.28
// D_10 	10423 	2.4222 	0.0189 	0.272
// D_2*+ 	415 	2.459 	0.025 	0.312
// D_2*0 	425 	2.4589 	0.0299 	0.309
// D'_1+ 	20413 	2.461 	0.29 	0.314
// D'_10 	20423 	2.461 	0.29 	0.311


  //D1
  PDGName[4]="$D_{1}(2420)$";
  Title[4]="$D_{1}(1P)$";
  Name[4]="D_{1}(";
  Mass[4]=2.440;
  Width[4]="\\mathcal{O}(10)";
  MassMeasEr[4]=.002;
  WidthMeas[4]=0.020;
  WidthMeasEr[4]=0.002;
  Spin[4]=1;
  Parity[4]=+1;
  SpecNot[4]="1^{1}P_{1}";  
  MassMeas[4]=2.422;
  DPiDecays[4]="-";
  DstPiDecays[4]="S,D";
  Helicity[4]="$\\propto 1+Acos^2(\\theta)$";
  MCMassNeu[4]=2.4222;
  MCMassChg[4]=2.427;
  MCWidthNeu[4]=.0189;
  MCWidthChg[4]=.028;


  //D0(2400)
  PDGName[5]="$D^{*}_{0}(2400)$";
  Title[5]="$D^{*}_{0}(1P)$";
  Name[5]="D^{*}_{0}(";
  Mass[5]=2.400;
  Width[5]="\\mathcal{O}(100)";
  MassMeasEr[5]=.015;
  WidthMeas[5]=0.310;
  WidthMeasEr[5]=0.015;
  Spin[5]=0;
  Parity[5]=+1;
  SpecNot[5]="1^{3}P_{0}";  
  MassMeas[5]=2.300;
  DPiDecays[5]="S";
  DstPiDecays[5]="-";
  Helicity[5]="-";
  MCMassNeu[5]=2.308;
  MCMassChg[5]=2.308;
  MCWidthNeu[5]=.276;
  MCWidthChg[5]=.276;


  //D1*
  PDGName[6]="$D^{'}_{1}(2430)$";
  Title[6]="$D^{*}_{1}(1P)$";
  Name[6]="D^{*}_{1}(";
  Mass[6]=2.490;
  Width[6]="\\mathcal{O}(100)";
  MassMeasEr[6]=.03;
  WidthMeas[6]=0.380;
  WidthMeasEr[6]=0.1;
  Spin[6]=1;
  Parity[6]=+1;
  SpecNot[6]="1^{3}P_{1}";  
  MassMeas[6]=2.427;
  DPiDecays[6]="-";
  DstPiDecays[6]="S,D";
  Helicity[6]="$\\propto 1+Acos^2(\\theta)$";
  MCMassNeu[6]=2.461;
  MCMassChg[6]=2.461;
  MCWidthNeu[6]=.29;
  MCWidthChg[6]=.29;


  //D2*
  PDGName[7]="$D^{*}_{2}(2460)$";
  Title[7]="$D^{*}_{2}(1P)$";
  Name[7]="D^{*}_{2}(";
  Mass[7]=2.500;
  Width[7]="\\mathcal{O}(10)";
  MassMeasEr[7]=.001;
  WidthMeas[7]=0.044;
  WidthMeasEr[7]=.002;
  Spin[7]=2;
  Parity[7]=+1;
  SpecNot[7]="1^{3}P_{2}";  
  MassMeas[7]=2.459;
  DPiDecays[7]="D";
  DstPiDecays[7]="D";
  Helicity[7]="$\\propto sin^2(\\theta)$";
  MCMassNeu[7]=2.459;
  MCMassChg[7]=2.459;
  MCWidthNeu[7]=.030;
  MCWidthChg[7]=.025;

  
  ///////////////////////////L=2 states 
  //D2(2825)
  PDGName[8]="-";
  Title[8]="$D_{2}(1D)$";
  Name[8]="D_{2}(";
  Mass[8]=2.825;
  Width[8]="-";
  MassMeasEr[8]=0.;
  WidthMeas[8]=0.;
  WidthMeasEr[8]=0.;
  Spin[8]=2;
  Parity[8]=-1;
  SpecNot[8]="1^{1}D_{2}"; 
  MassMeas[8]=0.;
  DPiDecays[8]="-";
  DstPiDecays[8]="P,F";
  Helicity[8]="$\\propto 1+Acos^2(\\theta)$";
  MCMassNeu[8]=0.;
  MCMassChg[8]=0.;
  MCWidthNeu[8]=0.;
  MCWidthChg[8]=0.;

  
  //D1(2820)
  PDGName[9]="-";
  Title[9]="$D^{*}_{1}(1D)$";
  Name[9]="D^{*}_{1}(";
  Mass[9]=2.820;
  Width[9]="-";
  MassMeasEr[9]=0.;
  WidthMeas[9]=0.;
  WidthMeasEr[9]=0.;
  Spin[9]=1;
  Parity[9]=-1;
  SpecNot[9]="1^{3}D_{1}"; 
  MassMeas[9]=0.;
  DPiDecays[9]="P";
  DstPiDecays[9]="P";
  Helicity[9]="$\\propto sin^2(\\theta)$";
  MCMassNeu[9]=0.;
  MCMassChg[9]=0.;
  MCWidthNeu[9]=0.;
  MCWidthChg[9]=0.;


  //D2(2825)
  PDGName[10]="-";
  Title[10]="$D^{*}_{2}(1D)$";
  Name[10]="D^{*}_{2}(";
  Mass[10]=2.830;
  Width[10]="-";
  MassMeasEr[10]=0.;
  WidthMeas[10]=0.;
  WidthMeasEr[10]=0.;
  Spin[10]=2;
  Parity[10]=-1;
  SpecNot[10]="1^{3}D_{2}"; 
  MassMeas[10]=0.;
  DPiDecays[10]="-";
  DstPiDecays[10]="P,F";
  Helicity[10]="$\\propto 1+Acos^2(\\theta)$";
  MCMassNeu[10]=0.;
  MCMassChg[10]=0.;
  MCWidthNeu[10]=0.;
  MCWidthChg[10]=0.;


  //D3(2830)
  PDGName[11]="-";
  Title[11]="$D^{*}_{3}(1D)$";
  Name[11]="D^{*}_{3}(";
  Mass[11]=2.830;
  Width[11]="-";
  MassMeasEr[11]=0.;
  WidthMeas[11]=0.;
  WidthMeasEr[11]=0.;
  Spin[11]=3;
  Parity[11]=-1;
  SpecNot[11]="1^{3}D_{3}"; 
  MassMeas[11]=0.;
  DPiDecays[11]="F";
  DstPiDecays[11]="F";
  Helicity[11]="$\\propto sin^2(\\theta)$";
  MCMassNeu[11]=0.;
  MCMassChg[11]=0.;
  MCWidthNeu[11]=0.;
  MCWidthChg[11]=0.;


  ///////////////////////////L=3 states 
  //D3()
  PDGName[12]="-";
  Title[12]="$D_{3}(1F)$";
  Name[12]="D_{3}(";
  Mass[12]=3.10;
  Width[12]="-";
  MassMeasEr[12]=0.;
  WidthMeas[12]=0.;
  WidthMeasEr[12]=0.;
  Spin[12]=3;
  Parity[12]=+1;
  SpecNot[12]="1^{1}D_{3}"; 
  MassMeas[12]=0.;
  DPiDecays[12]="-";
  DstPiDecays[12]="";
  Helicity[12]="$$";
  MCMassNeu[12]=0.;
  MCMassChg[12]=0.;
  MCWidthNeu[12]=0.;
  MCWidthChg[12]=0.;
  
  //D2()
  PDGName[13]="-";
  Title[13]="$D^{*}_{2}(1F)$";
  Name[13]="D^{*}_{2}(";
  Mass[13]=3.10;
  Width[13]="-";
  MassMeasEr[13]=0.;
  WidthMeas[13]=0.;
  WidthMeasEr[13]=0.;
  Spin[13]=2;
  Parity[13]=+1;
  SpecNot[13]="1^{3}D_{2}"; 
  MassMeas[13]=0.;
  DPiDecays[13]="";
  DstPiDecays[13]="";
  Helicity[13]="$$";
  MCMassNeu[13]=0.;
  MCMassChg[13]=0.;
  MCWidthNeu[13]=0.;
  MCWidthChg[13]=0.;

  //D3(2825)
  PDGName[14]="-";
  Title[14]="$D^{*}_{3}(1F)$";
  Name[14]="D^{*}_{3}(";
  Mass[14]=3.105;
  Width[14]="-";
  MassMeasEr[14]=0.;
  WidthMeas[14]=0.;
  WidthMeasEr[14]=0.;
  Spin[14]=3;
  Parity[14]=+1;
  SpecNot[14]="1^{3}D_{3}"; 
  MassMeas[14]=0.;
  DPiDecays[14]="-";
  DstPiDecays[14]="";
  Helicity[14]="$$";
  MCMassNeu[14]=0.;
  MCMassChg[14]=0.;
  MCWidthNeu[14]=0.;
  MCWidthChg[14]=0.;

  //D4()
  PDGName[15]="-";
  Title[15]="$D^{*}_{4}(1F)$";
  Name[15]="D^{*}_{4}(";
  Mass[15]=3.11;
  Width[15]="-";
  MassMeasEr[15]=0.;
  WidthMeas[15]=0.;
  WidthMeasEr[15]=0.;
  Spin[15]=4;
  Parity[15]=+1;
  SpecNot[15]="1^{3}D_{4}"; 
  MassMeas[15]=0.;
  DPiDecays[15]="";
  DstPiDecays[15]="";
  Helicity[15]="$$";
  MCMassNeu[15]=0.;
  MCMassChg[15]=0.;
  MCWidthNeu[15]=0.;
  MCWidthChg[15]=0.;








  Float_t D0Mass=1.8645;
  Float_t Dstar0Mass=2.00697;
  Float_t offsetMass=D0Mass-Mass[0];
  Float_t scaleMass=D0Mass/Mass[0];
  Float_t stretchMass=(Dstar0Mass-D0Mass)/(Mass[1]-Mass[0]);
  for(Int_t i=0;i<20;i++){
    if(Name[i]!=""){
      SpinParXcord[i]=.5+Spin[i]+4*(Parity[i]+1)/2;
      //Mass[i]=Mass[i]+offsetMass;//offset only
      Mass[i]=Mass[i]*scaleMass;//Scale only
      //Mass[i]=D0Mass+(Mass[i]-1.88)*stretchMass;//offset and Stretch masses  
      Name[i]+=TString("")+(long)(((int)(Mass[i]*1000))/1000)+"."+(long)(((int)(Mass[i]*10))%10)+(long)(((int)(Mass[i]*100))%10)+(long)(((int)(Mass[i]*1000))%10)+")";
    }
  }

  TH1F HSpectrum("HSpectrum","",20,0,20);
  TGraphErrors GSpectrum;
  for(Int_t i=0;i<20;i++){
    if(Name[i]!=""){
      GSpectrum.SetPoint(i,SpinParXcord[i],Mass[i]);
      GSpectrum.SetPointError(i,.5,0);
    }
  }
   
  Float_t YMin=1.7;
  
  TCanvas C;
  C.Clear();
  HSpectrum.SetStats(0);
  HSpectrum.GetYaxis()->SetRangeUser(YMin,3.2);
  HSpectrum.GetXaxis()->SetRangeUser(0,9.5);
  HSpectrum.GetXaxis()->SetNdivisions(0,0);
  HSpectrum.GetYaxis()->SetTitle("Mass   (GeV/c^{2})");
  HSpectrum.GetXaxis()->SetTitle("J^{P}            ");
  HSpectrum.Draw();  
  GSpectrum.SetMarkerSize(.01);
  GSpectrum.SetLineWidth(2);
  GSpectrum.Draw("pe same");
  TLatex textname;textname.SetTextSize(.025);
  for(Int_t i=0;i<20;i++)
    if(Name[i]!=""){
      //text.DrawLatex(SpinParXcord[i]-.1,YMin-.1,TString("")+(long)Spin[i]+"^{"+(Parity[i]==-1?"-":"+")+"}");
      if((Name[i].Contains("D^{*}_{1}")&&Parity[i]==+1&&Spin[i]==1)
	 ||(Name[i].Contains("D^{*}_{2}")&&Parity[i]==-1&&Spin[i]==2)
	 ||(Name[i].Contains("D^{*}_{3}")&&Parity[i]==+1&&Spin[i]==3))
	 textname.DrawLatex(SpinParXcord[i]-.45,Mass[i]+.02,Name[i]);
      else textname.DrawLatex(SpinParXcord[i]-.45,Mass[i]-.06,Name[i]);
      
    }
  //labels
  TLatex text;text.SetTextSize(.05);
  for(Int_t i=0;i<9;i++)
    if(i<4)
      //text.DrawLatex(SpinParXcord[i]-.1,YMin-.1,TString("")+(long)Spin[i]+"^{"+(Parity[i]==-1?"-":"+")+"}");
      text.DrawLatex(i+.5-.1,YMin-.1,TString("")+(long)i+"^{-}");
    else text.DrawLatex(i+.5-.1,YMin-.1,TString("")+(long)(i-4)+"^{+}"); 

  //nL levels
  TLatex textnL;textnL.SetTextSize(.06);
  textnL.DrawLatex(2.2,Mass[0]+.02,"}1S");
  textnL.DrawLatex(7.2,Mass[4]-.05,"}1P");
  textnL.DrawLatex(4.2,Mass[8]-.04,"}1D");
  textnL.DrawLatex(9.05,Mass[15]-.04,"}1F");
  textnL.DrawLatex(2.2,Mass[2]-.03,"}2S");

  C.Print("DMesonSpectrum.ps");





  //////////////////Latex Tables 
  cout<<"Masses"<<endl;
  for(Int_t i=2;i<20;i++)
    if(Name[i]!=""){
      cout<<Title[i]<<" & "<<PDGName[i]
	  <<" & "<<Form("%.3f",Mass[i])<<" & "<<(MassMeas[i]==0. ? "-" : Form("$%.3f\\pm%.3f$",MassMeas[i],MassMeasEr[i]))
	  <<" & "<<(WidthMeas[i]==0.? "-" :Form("$%.0f\\pm%.0f$",WidthMeas[i]*1000,WidthMeasEr[i]*1000))
	  <<" \\\\"<<endl;
    }

  cout<<endl<<endl<<"Angular Distributions"<<endl;
  for(Int_t i=2;i<20;i++)
    if(Name[i]!=""){
      cout<<Title[i]<<" & "<<TString("$")+(long)(Spin[i])+"^{"+(Parity[i]==-1?"-":"+")+"}$"
	  <<" & "<<DPiDecays[i]
	  <<" & "<<DstPiDecays[i]
	  <<" & "<<Helicity[i]
	  <<" \\\\"<<endl;
    }
  





  ///Latex format the Resonance parameters in the generated MC 
  cout<<"MC Masses"<<endl;
  for(Int_t i=2;i<20;i++)
    if(Name[i]!=""&&MCMassNeu[i]!=0.){//
      cout<<Title[i]
	  <<" & "<<Form("%.3f",MCMassNeu[i])<<" & "<<Form("%.3f",MCWidthNeu[i])
	  <<" & "<<Form("%.3f",MCMassChg[i])<<" & "<<Form("%.3f",MCWidthChg[i])
	  <<" \\\\"<<endl;
    }
  
 
}

