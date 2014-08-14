#include "DstPiAnalysis.h"
#include "../../FUNCTIONS/Project2DHistoLegendre.h"
#include <TF2.h>
#define NPROJS 6


Int_t DstPiAnalysis::PlotWeightedMass(Int_t WhichHistos){
  filename=_OutputDir+"/WeightedMass.ps";
  Canvas.Print(filename+"[");
  
  TFile* File=NULL;
  if(WhichHistos==1)File=new TFile(_OutputDir+"/EffCorrHistos.root","read");
  if(WhichHistos==2)File=new TFile(_OutputDir+"/MassVsHelicity.root","read");
  if(!File)return 0;
  if(File->IsZombie())return 0;
  gROOT->cd();
    
 
  TH2* H2DstPiMVsDstarHel=(TH2*)File->Get("H2DstPiMVsDstarHel");
  if(!H2DstPiMVsDstarHel){cout<<"No H2DstPiMVsDstarHel"<<endl;return 0;}

  Canvas.Clear();
  H2DstPiMVsDstarHel->Draw("colz");
  Canvas.Print(filename);


  TH1D* HMass=H2DstPiMVsDstarHel->ProjectionX();
  HMass->SetName("HMass");
  HMass->GetYaxis()->SetRangeUser(0,1.1*HMass->GetMaximum());
  Canvas.Clear();
  HMass->Draw();
  Canvas.Print(filename);
  delete HMass;


  TH1D* HDstarHel=H2DstPiMVsDstarHel->ProjectionY();
  HDstarHel->SetName("HDstarHel");
  Canvas.Clear();
  HDstarHel->GetYaxis()->SetRangeUser(0,1.1*HDstarHel->GetMaximum());
  HDstarHel->Draw();
  Canvas.Print(filename);
  delete HDstarHel;

  ///save the projections
  Canvas.Clear();
  Canvas.Divide((int)sqrt((float)NPROJS)+1,(int)sqrt((float)NPROJS));
  text.SetTextSize(.1);
  text.SetTextColor(2);

  ///now create the Legendre projections
  TH1* HDstPiMVsDstarHelLegProj[NPROJS];

  for(Int_t p=0;p<NPROJS;p++){
    Canvas.cd(p+1);

    HDstPiMVsDstarHelLegProj[p]=project2DHistoLegendre(H2DstPiMVsDstarHel,p);
    if(!HDstPiMVsDstarHelLegProj[p])return 0;   
 
    
    ///----------------------------------------------------------
    ///Notes: if we know the underlying distribution then we can correct the scaling factor
    ///----------------------------------------------------------
    //if I(x)=1/2  then a0=1/2
    //if I(x)=3/4 * (1-x^2) then  a0=1/2, a2=-1/2
    //if I(x)=3/2 * x^2 then  a0=1/2, a2=1 

    HDstPiMVsDstarHelLegProj[p]->Draw("pe");
    text.DrawTextNDC(.5,.7,TString("l=")+(long)p);
    cutline.DrawLine(HDstPiMVsDstarHelLegProj[p]->GetXaxis()->GetXmin(),0,HDstPiMVsDstarHelLegProj[p]->GetXaxis()->GetXmax(),0);
    
  }
  Canvas.Print(filename);  


  Canvas.Print(filename+"]");
  
  File->Close();
  delete File;

  TFile FProjs(_OutputDir+"/LegendreMassProjs.root","recreate");
  for(Int_t p=0;p<NPROJS;p++) 
    HDstPiMVsDstarHelLegProj[p]->Write();
  FProjs.Close();

  for(Int_t p=0;p<NPROJS;p++)
    delete HDstPiMVsDstarHelLegProj[p];


  return 1;
}



Int_t DstPiAnalysis::CreateMassVsHelicity(Int_t DataOrToy){

  TH2F*H2DstPiMVsDstarHel=NULL;
  if(DataOrToy==1)H2DstPiMVsDstarHel=GetCosHelVsMass();   
  if(DataOrToy==2)H2DstPiMVsDstarHel=GetToyCosHelVsMass();
  if(!H2DstPiMVsDstarHel)return 0;

  //save the projections
  TFile File(_OutputDir+"/MassVsHelicity.root","recreate");  
  H2DstPiMVsDstarHel->Write();
  File.ls();
  File.Close();

  delete H2DstPiMVsDstarHel;

  return 1;
}




void DstPiAnalysis::Correct1DEfficiencyOn2DPlot(TH2F* h2,TH1F* heff){
  if(!h2||!heff)return;

  for(Int_t by=1;by<=h2->GetNbinsY();by++){
    for(Int_t bx=1;bx<=h2->GetNbinsX();bx++){//x-axis is the D0DstarAngle
      if(heff->GetBinContent(bx)>0)h2->SetBinContent(bx,by,h2->GetBinContent(bx,by)/heff->GetBinContent(bx));      
    }
  }
    
  return;
}


TH2F* DstPiAnalysis::GetCosHelVsMass(){
  ///Open the reduced Ntuple
  if(OpenReducedNtuple(2)!=1) return 0;

  TH2F H2DstPiMVsDstarHel("H2DstPiMVsDstarHel","H2DstPiMVsDstarHel",200,2.1,3.1,40,-1.00001,1.00001);
  ReducedNtuple->Draw("dstarhelicity:dstpideltam+2.01>>H2DstPiMVsDstarHel");
  if(CloseReducedNtuple()!=1) return 0;

  return (TH2F*)H2DstPiMVsDstarHel.Clone();
}

TH2F* DstPiAnalysis::GetToyCosHelVsMass(){
  //----------
  //toy tests 
  //----------
  TH2F H2DstPiMVsDstarHel("H2DstPiMVsDstarHel","H2DstPiMVsDstarHel",80,2.2,2.6,40,-1.00001,1.00001);

  //ths signal
  TF1 fm("fm","1./((x-2.420)*(x-2.420)+.25*.03*.03)",2.2,2.6);
  TF1 fx("fx","x*x",-1,1);
  for(Int_t p=0;p<40000;p++)
    H2DstPiMVsDstarHel.Fill(fm.GetRandom(),fx.GetRandom());

  //background
  TF1 fmb("fmb","1.",2.2,2.6);
  TF1 fxb("fxb","1+x",-1,1);
  for(Int_t p=0;p<100000;p++)
    H2DstPiMVsDstarHel.Fill(fmb.GetRandom(),fxb.GetRandom());


  return (TH2F*)H2DstPiMVsDstarHel.Clone();  
}







