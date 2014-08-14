#include "../Globals.h"


void GraphXvsY(TCanvas*C,Int_t N,Double_t* X,Double_t*Y,Double_t* XE,Double_t*YE,TString xtitle,TString ytitle,Float_t ymin,Float_t ymax,Bool_t logy,Float_t default,Float_t defaultEr,Float_t gen){
  C->Clear();
  C->SetLogy(logy);
  TGraphErrors Graph(N,X,Y,XE,YE);
  Graph.SetMarkerSize(2);
  Graph.Draw("ape");
  Graph.SetTitle("");
  Graph.GetYaxis()->SetTitle(ytitle);
  Graph.GetXaxis()->SetTitle(xtitle);
  Graph.GetYaxis()->SetRangeUser(ymin,ymax);
  Graph.GetYaxis()->SetNdivisions(4);
  TBox box(Graph.GetXaxis()->GetXmin(),default-defaultEr,Graph.GetXaxis()->GetXmax(),default+defaultEr);
  box.SetFillColor(kGreen-7);
  box.SetFillStyle(1001);  
  if(default!=0&&defaultEr!=0){     
    box.Draw();
  }
  Graph.Draw("pe");
  TLine line(Graph.GetXaxis()->GetXmin(),gen,Graph.GetXaxis()->GetXmax(),gen);
  line.SetLineColor(2);
  line.SetLineWidth(2);
  if(gen!=0)
    line.Draw();
  C->Update();
  C->Print("Systematics.ps");
  

  if(Y&&YE){
    Float_t avgdiff=0;
    Float_t totalweight=0;
    for(Int_t i=0;i<N;i++){     
      avgdiff+=pow((Y[i]-default)/sqrt(YE[i]*YE[i]+defaultEr*defaultEr),2);
      totalweight+=pow(1/sqrt(YE[i]*YE[i]+defaultEr*defaultEr),2);
    }
    cout<<" Weight Avg  "<<xtitle<<" "<<ytitle; printf("  %.1f\n",sqrt(avgdiff/totalweight));
  }
}
void GraphSystematics(){ 

  TCanvas C("C");  
  TString filename;
  C.Print("Systematics.ps["); 



  ///------------------
  //Generic MC
  //-------------------

  //----------------------------
  //Chisquare vs Polynomial order
  //----------------------------
  ///Fits to the un-truth matched ccbar 
  /////----Range 2.2->3.1
  //Int_t N=9;
  //Double_t X[9]={2,3,4,5,6,7,8,9,10};
  //Double_t Y[9]={21912.8/299,8827.07/298,1713.13/297,429.624/296,445.102/295,454.61/294,469.298/293,419.098/292,419.115/291};
  ////----Range 2.2->3.1;
  Int_t N=4;
  Double_t X[4]={2,3,4,5};
  Double_t Y[4]={465.124/99,112.579/98,112.572/97,112.572/96};
  GraphXvsY(&C,4,X,Y,0,0,"Polynomial Order","#chi^{2}/DF",.5,10,1,0,0,0);

 
    
  //-------------------------
  //Signal MC Systematics  
  ///-----------------------

  //--------------------
  ///---D2420
  
  //Default: mean0    2.4221e+00 +/-  4.79e-05  width0    1.9315e-02 +/-  1.03e-04
  Float_t defMean=2.4221e+00*1000;
  Float_t defMeanEr=4.79e-05*1000;
  Float_t defWidth=1.9315e-02*1000;
  Float_t defWidthEr=1.03e-04*1000;


  //----------------
  //mass-width vs p*
  //----------------  
  Double_t PX[4]={3.25,3.75,4.25,4.75};
  Double_t PXE[4]={.25,.25,.25,.25};
  Double_t PMeanY[4]={2.4221e+00*1000,2.4222e+00*1000,2.4219e+00*1000,2.4221e+00*1000};
  Double_t PMeanYE[4]={8.16e-05*1000,7.95e-05*1000,9.37e-05*1000,2.41e-04*1000};
  GraphXvsY(&C,4,PX,PMeanY,PXE,PMeanYE,"p* (GeV)","D_{1}(2420) Mass (MeV)",2421,2423,0,defMean,defMeanEr,2422.2);
  Double_t PWidthY[4]={1.9348e-02*1000,1.9324e-02*1000,1.8969e-02*1000,1.8438e-02*1000};
  Double_t PWidthYE[4]={ 1.78e-04*1000,1.72e-04*1000,1.94e-04*1000,4.66e-04*1000};
  GraphXvsY(&C,4,PX,PWidthY,PXE,PWidthYE,"p* (GeV)","D_{1}(2420) Width (MeV)",18,20,0,defWidth,defWidthEr,18.9);


  //----------------
  //mass-width vs phi*
  //----------------  
  //Double_t PhiX[4]={(-3*mypi/2)*180/mypi,(-mypi/2)*180/mypi,(mypi/2)*180/mypi,(3*mypi/2)*180/mypi};
  Double_t CosX[4]={-.75,-.25,.25,.75};
  Double_t CosXE[4]={.25,.25,.25,.25};
  Double_t CosMeanY[4]={2.4220e+0*1000,2.4222e+00*1000,2.4220e+00*1000,2.4222e+00*1000};
  Double_t CosMeanYE[4]={8.36e-05*1000,8.15e-05*1000,9.25e-05*1000,1.88e-04*1000};
  GraphXvsY(&C,4,CosX,CosMeanY,CosXE,CosMeanYE,"cos(#theta)","D_{1}(2420) Mass (MeV)",2421,2423,0,defMean,defMeanEr,2422.2);
  Double_t CosWidthY[4]={1.9308e-02*1000,1.9096e-02*1000,1.9241e-02*1000,1.9449e-02*1000};
  Double_t CosWidthYE[4]={1.79e-04*1000,1.74e-04*1000,2.03e-04*1000,3.67e-04*1000};
  GraphXvsY(&C,4,CosX,CosWidthY,CosXE,CosWidthYE,"cos(#theta)","D_{1}(2420) Width (MeV)",18,20,0,defWidth,defWidthEr,18.9);


  //----------------
  //mass-width vs phi*
  //----------------  
  Double_t PhiX[4]={-180*3/4,-180*1/4,180*1/4,180*3/4};
  Double_t PhiXE[4]={45,45,45,45};
  Double_t PhiMeanY[4]={2.4221e+00*1000,2.4220e+00*1000,2.4223e+00*1000,2.4220e+00*1000};
  Double_t PhiMeanYE[4]={9.52e-05*1000,9.60e-05*1000,9.52e-05*1000,9.51e-05*1000};
  GraphXvsY(&C,4,PhiX,PhiMeanY,PhiXE,PhiMeanYE,"#phi (degrees)","D_{1}(2420) Mass (MeV)",2421,2423,0,defMean,defMeanEr,2422.2);
  Double_t PhiWidthY[4]={1.9167e-02*1000,1.9247e-02*1000,1.8922e-02*1000,1.9515e-02*1000};
  Double_t PhiWidthYE[4]={2.04e-04*1000,2.09e-04*1000,1.99e-04*1000,2.06e-04*1000};
  GraphXvsY(&C,4,PhiX,PhiWidthY,PhiXE,PhiWidthYE,"#phi (degrees)","D_{1}(2420) Width (MeV)",18,20,0,defWidth,defWidthEr,18.9);



  //--------------------
  ///---D2460


  
  C.Print("Systematics.ps]"); 
  return;
}


