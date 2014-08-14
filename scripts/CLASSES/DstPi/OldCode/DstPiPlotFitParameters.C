#include "../../Globals.h"
//#include "../CLASSES/DstPiAnalysis.h"
#include <TGraphErrors.h>

void PlotDstPiFitPars(TString dir){

  //TString dir="DstarCharged/DstarPi/Kpi";
  TString files;

  TGraphErrors GSig1Norm; 
  GSig1Norm.SetMarkerSize(2);
  TGraphErrors GSig1Mean;  
  GSig1Mean.SetMarkerSize(2);
  TGraphErrors GSig1Sigma; 
  GSig1Sigma.SetMarkerSize(2);

  TGraphErrors GSig2Norm;
  GSig2Norm.SetMarkerColor(4); 
  GSig2Norm.SetLineColor(4); 
  GSig2Norm.SetMarkerSize(2);
  TGraphErrors GSig2Mean;
  GSig2Mean.SetMarkerColor(4); 
  GSig2Mean.SetLineColor(4); 
  GSig2Mean.SetMarkerSize(2);
  TGraphErrors GSig2Sigma; 
  GSig2Sigma.SetMarkerColor(4); 
  GSig2Sigma.SetLineColor(4); 
  GSig2Sigma.SetMarkerSize(2);

  TGraphErrors GBkgNorm;
  GBkgNorm.SetMarkerSize(2);

  TGraphErrors GBkgPar;
  GBkgPar.SetMarkerSize(2);

  TGraphErrors GChisquare;
  GChisquare.SetMarkerSize(2);

  Float_t x;
  Float_t n;
  Float_t fitmin;
  Float_t fitmax;
  Int_t np=0;
  char name[100];
  Float_t maxchisq=0;

  ifstream input;
  for(Int_t s1=0;s1<=3;s1++){
    for(Int_t s2=0;s2<=3;s2++){
      Int_t f=s1*4+s2+1;
      //files=dir+"/FitPars_"+(long)f+".txt";
      files=dir+"/FitPars_"+(long)s1+"100_"+(long)s2+"100_5.txt";
      input.open((const char*)files);
      if(input.is_open()){

	input>>x;input>>n;///chisquare
	if(x/n<25){
	  GChisquare.SetPoint(np,f,x/n);
	  if(x/n>maxchisq)maxchisq=x/n;

	  //fit range
	  input>>fitmin;input>>fitmax;input>>x;      

	  input>>name;
	  input>>x;GSig1Norm.SetPoint(np,x,f);
	  input>>x;GSig1Norm.SetPointError(np,x,0);
	  
	  input>>x;GSig1Mean.SetPoint(np,x*1000,f);	 
	  input>>x;GSig1Mean.SetPointError(np,x*1000,0);
	  
	  input>>x;GSig1Sigma.SetPoint(np,x*1000,f);
	  input>>x;GSig1Sigma.SetPointError(np,x*1000,0);

	  input>>name;
	  input>>x;GSig2Norm.SetPoint(np,x,f);
	  input>>x;GSig2Norm.SetPointError(np,x,0);

	  input>>x;GSig2Mean.SetPoint(np,x*1000,f);
	  input>>x;GSig2Mean.SetPointError(np,x*1000,0);

	  input>>x;GSig2Sigma.SetPoint(np,x*1000,f);
	  input>>x;GSig2Sigma.SetPointError(np,x*1000,0);

	  input>>name;
	  input>>x;GBkgNorm.SetPoint(np,x,f);
	  input>>x;GBkgNorm.SetPointError(np,x,0);

	  input>>x;GBkgPar.SetPoint(np,x,f);
	  input>>x;GBkgPar.SetPointError(np,x,0);

	  // 	for(Int_t p=1;p<BkgPolNpowers;p++){
	  // 	  input>>x>>x>>x;
	  // 	}       
	

	  np++;
	}
	  
	input.close();
	input.clear();
      }
    }
  }

  TCanvas C("C","C");
  TString filename=dir+"/FitParsVsFit.ps";
  C.Print(filename+"[");


  C.Clear();
  GChisquare.Draw("ape");
  GChisquare.GetYaxis()->SetTitle("#chi^{2}/ndf");
  GChisquare.GetYaxis()->SetRangeUser(0,maxchisq*1.2);     
  C.Update();
  C.Print(filename);

  C.Clear();
  GSig1Norm.Draw("ape");
  //GSig1Norm.GetYaxis()->SetRangeUser(0,1e5);  
  C.Update();   
  //GSig2Norm.Draw("pesame");
  C.Print(filename);

  C.Clear();
  GSig1Mean.Draw("ape");
  //GSig1Mean.GetYaxis()->SetRangeUser(405,415);
  GSig1Mean.GetXaxis()->SetTitle("Mean (MeV)");
  C.Update();
  //GSig2Mean.Draw("pesame");     
  C.Print(filename);

  C.Clear();
  GSig1Sigma.Draw("ape");
  //GSig1Sigma.GetYaxis()->SetRangeUser(25,45);     
  GSig1Sigma.GetXaxis()->SetTitle("Width  (MeV)");
  C.Update();
  //GSig2Sigma.Draw("pesame");
  C.Print(filename);

  C.Clear();
  GSig2Norm.Draw("ape");
  //GSig2Norm.GetYaxis()->SetRangeUser(0,1e5);  
  C.Update(); 
  C.Print(filename);
  C.Clear();
  GSig2Mean.Draw("ape");
  //GSig2Mean.GetYaxis()->SetRangeUser(440,460);
  GSig2Mean.GetXaxis()->SetTitle("Mean (MeV)");
  C.Update();
  C.Print(filename);
  C.Clear();
  GSig2Sigma.Draw("ape");
  //GSig2Sigma.GetYaxis()->SetRangeUser(30,60);     
  GSig2Sigma.GetXaxis()->SetTitle("Width  (MeV)");
  C.Update();
  C.Print(filename);

  C.Clear();
  GBkgNorm.Draw("ape");
  C.Print(filename); 

  C.Clear();
  GBkgPar.Draw("ape");
  C.Print(filename);  


  C.Print(filename+"]");
  
}


void MoveSpinFits(TString Dir){
  
  for(Int_t s1=0;s1<=3;s1++){
    for(Int_t s2=0;s2<=3;s2++){
      system(TString("mv ") +Dir+"/DstPiRooFit_"+(long)s1+"100_"+(long)s2+"100_5.ps "+Dir+"/SpinFits/.");
      system(TString("mv ") +Dir+"/FitPars_"+(long)s1+"100_"+(long)s2+"100_5.txt "+Dir+"/SpinFits/.");
    }
  }

}
