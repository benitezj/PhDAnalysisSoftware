#include "DstPiAnalysis.h"
#include "../JResiduals.h"
#include "../../FUNCTIONS/Fits.h"
#include "../../FUNCTIONS/DetermineFWHM.C"

Int_t DstPiAnalysis::Resolution(){
  cout<<"Going to determine the resolution as function of mass"<<endl;
  if(!_TruthMatch){
    cout<<"Error: This operation is only MC."<<endl;
    return 0;

  }

  //
  TString directory=_OutputDir;

  //ps file
  TString filename=directory+"/Resolution.ps";
  TCanvas Canvas(filename,filename);
  Canvas.Print(filename+"[");
  
  //apply cuts 
  TCut rscut="abs(dstpicharge)==1";
  TCut truecut="dstarmctrue==1&&pi1mctrue==1";
  TCut TotalCut=database.GetCut(2,100)+rscut+truecut;//cut should not include dstpimctrue==1
  
  ///Open the clean reduced Ntuple
  OpenReducedNtuple(1);
  
  //make a copy of the tree with some cuts
  TTree* tree=ReducedNtuple;ReducedNtuple->CopyTree(TotalCut);
  
  ///print the cuts into the file
  database.PrintCutList(&Canvas,filename);
  
  //----------
  //Plot the D* deltaM
  //----------
  TH1F HDstar("HDstar","HDstar",100,.14,.15);
  HDstar.GetXaxis()->SetTitle("#DeltaM_{D*}");
  HDstar.GetXaxis()->SetTitle("Entries /.1 MeV");
  tree->Draw("dstardeltam>>HDstar");
  Canvas.Clear();
  HDstar.Draw("pe");
  Canvas.Print(filename);

  TH1F HDstPiRes("HDstPiRes","HDstPiRes",200,-.01,.01);
  tree->Draw("dstpimass-dstpigenmass>>HDstPiRes");
  Canvas.Clear();
  HDstPiRes.Draw("pe");
  Canvas.Print(filename);

  TH1F HDstPiDMRes("HDstPiDMRes","HDstPiDMRes",200,-.01,.01);
  tree->Draw("dstpideltam-(dstpigenmass-2.01)>>HDstPiDMRes");
  Canvas.Clear();
  HDstPiDMRes.Draw("pe");
  Canvas.Print(filename);
  

  ///plot the resolution as a function of mass
  Float_t fitmin=-.015;
  Float_t fitmax=.015;
  Int_t nbins=120;
  Float_t dmmax=.95;
  Float_t dmmin=.25;
  Int_t npoints=14;
  Float_t dmstep=(dmmax-dmmin)/npoints;
  TString label;
  TH1F* HDstPiDMResVsM[10];
  TGraphErrors GResVsM;
  GResVsM.SetTitle("Resolution"); 
  TGraphErrors GMeanVsM;
  GMeanVsM.SetTitle("Mean");
  TGraphErrors GRatioVsM;
  GRatioVsM.SetTitle("Ratio");
  TGraphErrors GSigma1VsM;
  GSigma1VsM.SetTitle("Sigma1");
  TGraphErrors GSigma2VsM;
  GSigma2VsM.SetTitle("Sigma2");
  TGraphErrors GChisVsM;
  GChisVsM.SetTitle("#chi^{2}/ndf");
  TF1 FRes("FRes","[0]*(exp(-.5*(x-[1])**2/[2]**2)+[3]*exp(-.5*(x-[1])**2/[4]**2))",fitmin,fitmax);
  FRes.SetParameters(100,0,.001,.1,.01);
  FRes.SetParLimits(0,0,1e5);
  FRes.SetParLimits(1,-.001,.001);
  FRes.SetParLimits(2,.001,.02);
  FRes.SetParLimits(3,.01,10);
  FRes.SetParLimits(4,.001,.05);
  FRes.FixParameter(1,.000097);
  FRes.FixParameter(3,.153);
  FRes.SetNpx(10000);
  FRes.SetLineWidth(2);
  
  for(Int_t p=0;p<npoints;p++){    
    label=TString("HDstPiDMResVsM")+(long)p;
    nbins=120-(int)(110*p/npoints);
    HDstPiDMResVsM[p]=new TH1F(label,label,nbins,fitmin,fitmax);
    HDstPiDMResVsM[p]->GetXaxis()->SetTitle("deltaM_{gen} - deltaM_{rec}");
    sprintf(textlabel,"%.5f<(dstpigenmass-2.01)&&(dstpigenmass-2.01)<%.5f",dmmin+p*dmstep,dmmin+(p+1)*dmstep);
    tree->Draw("dstpideltam-(dstpigenmass-2.01)>>"+label,textlabel);
    HDstPiDMResVsM[p]->GetXaxis()->SetNdivisions(4,kFALSE);
    //HDstPiDMResVsM[p]->Fit(&FRes,"Q","pe",fitmin,fitmax);
    Int_t status=HDstPiDMResVsM[p]->Fit(&FRes,"Q","pe",-(2.2+log10((float)(p+1)))*HDstPiDMResVsM[p]->GetRMS(),(2.2+log10((float)(p+1)))*HDstPiDMResVsM[p]->GetRMS());
    if(status!=0)HDstPiDMResVsM[p]->Fit(&FRes,"Q","pe",-(2.2+log10((float)(p+1)))*HDstPiDMResVsM[p]->GetRMS(),(2.2+log10((float)(p+1)))*HDstPiDMResVsM[p]->GetRMS());

    Float_t mean;
    Float_t fwhm;
    Float_t xmax;
    DetermineFWHM(&FRes,&fwhm,&mean,&xmax);
    
    GResVsM.SetPoint(p,(dmmin+p*dmstep+dmmin+(p+1)*dmstep)/2,1000*fwhm);
    GResVsM.SetPointError(p,dmstep/2,1000*(FRes.GetParameter(2)*FRes.GetParError(2)+FRes.GetParameter(4)*FRes.GetParameter(3)*FRes.GetParError(4))/(FRes.GetParameter(2)+FRes.GetParameter(4)*FRes.GetParameter(3)));
    GMeanVsM.SetPoint(p,(dmmin+p*dmstep+dmmin+(p+1)*dmstep)/2,1000*FRes.GetParameter(1));
    GMeanVsM.SetPointError(p,dmstep/2,1000*FRes.GetParError(1));
    GSigma1VsM.SetPoint(p,(dmmin+p*dmstep+dmmin+(p+1)*dmstep)/2,1000*FRes.GetParameter(2));
    GSigma1VsM.SetPointError(p,dmstep/2,1000*FRes.GetParError(2));
    GRatioVsM.SetPoint(p,(dmmin+p*dmstep+dmmin+(p+1)*dmstep)/2,FRes.GetParameter(3));
    GRatioVsM.SetPointError(p,dmstep/2,FRes.GetParError(3));
    GSigma2VsM.SetPoint(p,(dmmin+p*dmstep+dmmin+(p+1)*dmstep)/2,1000*FRes.GetParameter(4));
    GSigma2VsM.SetPointError(p,dmstep/2,1000*FRes.GetParError(4));
  
    GChisVsM.SetPoint(p,(dmmin+p*dmstep+dmmin+(p+1)*dmstep)/2,FRes.GetChisquare()/FRes.GetNDF());

    Canvas.Clear();	
    HDstPiDMResVsM[p]->Draw("pe");	
    Canvas.Print(filename);

    delete  HDstPiDMResVsM[p];
  }  

  Canvas.Clear();
  GResVsM.SetMarkerSize(1.5);
  GResVsM.GetYaxis()->SetRangeUser(0,12);
  GResVsM.GetXaxis()->SetTitle(DstarPiXLabel);
  GResVsM.GetYaxis()->SetTitle("FWHM (MeV)");
  GResVsM.Draw("ape");
  Canvas.Print(filename);

  Float_t polfitmin=.3;
  Float_t polfitmax=.7;
  TF1 FPol("FPol","[0]+[1]*x",polfitmin,polfitmax);
  FPol.SetParameters(1,2);

  Canvas.Clear();
  GMeanVsM.Draw("ape");
  GMeanVsM.GetYaxis()->SetRangeUser(-.5,.5);
  GMeanVsM.GetXaxis()->SetTitle(DstarPiXLabel);
  GMeanVsM.GetYaxis()->SetTitle("mean (MeV)");
  GMeanVsM.Fit(&FPol,"Q","ape",polfitmin,polfitmax);
  Canvas.Print(filename);
  Canvas.Clear();
  GSigma1VsM.Draw("ape");
  GSigma1VsM.GetYaxis()->SetRangeUser(0,15);
  GSigma1VsM.GetXaxis()->SetTitle(DstarPiXLabel);
  GSigma1VsM.GetYaxis()->SetTitle("sigma1 (MeV)");
  GSigma1VsM.Fit(&FPol,"Q","ape",polfitmin,polfitmax);
  Canvas.Print(filename);
  Canvas.Clear();
  GRatioVsM.Draw("ape");
  GRatioVsM.GetYaxis()->SetRangeUser(0,.5);
  GRatioVsM.GetXaxis()->SetTitle(DstarPiXLabel);
  GRatioVsM.GetYaxis()->SetTitle("2nd Gaussian Norm/1st Gaussian Norm");
  GRatioVsM.Fit(&FPol,"Q","ape",polfitmin,polfitmax);
  Canvas.Print(filename);
  Canvas.Clear();
  GSigma2VsM.Draw("ape");
  GSigma2VsM.GetYaxis()->SetRangeUser(0,30);
  GSigma2VsM.GetXaxis()->SetTitle(DstarPiXLabel);
  GSigma2VsM.GetYaxis()->SetTitle("sigma2 (MeV)");
  GSigma2VsM.Fit(&FPol,"Q","ape",polfitmin,polfitmax);
  Canvas.Print(filename);
  Canvas.Clear();
  GChisVsM.Draw("ape");
  GChisVsM.GetYaxis()->SetRangeUser(0,5);
  GChisVsM.GetXaxis()->SetTitle(DstarPiXLabel);
  GChisVsM.GetYaxis()->SetTitle("#chi^{2}/ndf");
  Canvas.Print(filename);

  Canvas.Print(filename+"]");
  CloseReducedNtuple();

  return 1;

}


/*
  ////last thing determine the relative efficiency
  TCut gendistcut1="dstpigendist<.02";
  TCut gendistcut2="dstpigendist<.05";
  TCut gendistcut3="dstpigendist<.1";
  
  //TH1F HDstPiDM("HDstPiDM","HDstPiDM",npoints,dmmin,dmmax);
  TH1F HDstPiDM1("HDstPiDM1","HDstPiDM1",database.GetDstPiDeltaMNbins(),database.GetDstPiDeltaMMin(),database.GetDstPiDeltaMMax());
  HDstPiDM1.GetXaxis()->SetTitle(DstarPiXLabel);
  HDstPiDM1.GetYaxis()->SetTitle("Entries /50 MeV");
  HDstPiDM1.Sumw2();
  tree->Draw("dstpideltam>>HDstPiDM1",gendistcut1);
  TH1F HDstPiDM2("HDstPiDM2","HDstPiDM2",database.GetDstPiDeltaMNbins(),database.GetDstPiDeltaMMin(),database.GetDstPiDeltaMMax());
  HDstPiDM2.Sumw2();
  tree->Draw("dstpideltam>>HDstPiDM2",gendistcut2);
  TH1F HDstPiDM3("HDstPiDM3","HDstPiDM3",database.GetDstPiDeltaMNbins(),database.GetDstPiDeltaMMin(),database.GetDstPiDeltaMMax());
  HDstPiDM3.Sumw2();
  tree->Draw("dstpideltam>>HDstPiDM3",gendistcut3);
  Canvas.Clear();
  HDstPiDM3.Draw();
  HDstPiDM2.Draw();
  HDstPiDM1.Draw();
  Canvas.Print(filename);

  TH1F HDstPiGenDist("HDstPiGenDist","HDstPiGenDist",100,.000001,.2);
  HDstPiGenDist.GetXaxis()->SetTitle("Vtx-Vtx Distance (cm)");
  tree->Draw("dstpigendist>>HDstPiGenDist");
  Canvas.Clear();
  HDstPiGenDist.Draw();
  Canvas.Print(filename);

  TH1F* HGenDM1=(TH1F*)OutPutFile->Get("HMCDstRandPiDeltaM1");
  TH1F* HGenDM2=(TH1F*)OutPutFile->Get("HMCDstRandPiDeltaM2");
  TH1F* HGenDM3=(TH1F*)OutPutFile->Get("HMCDstRandPiDeltaM3");
  Canvas.Clear();
  HGenDM3->Draw();
  HGenDM2->Draw("same");
  HGenDM1->Draw("same");
  Canvas.Print(filename);


  TH1F* HMCDist=(TH1F*)OutPutFile->Get("HMCDstRandPiDistance");
  Canvas.Clear();
  HMCDist->GetXaxis()->SetRangeUser(0,.2);
  HMCDist->Draw();
  Canvas.Print(filename);

  TH1F*HDstPiDMCl1=(TH1F*)HDstPiDM1.Clone();
  HDstPiDMCl1->Divide(HGenDM1);
  TH1F*HDstPiDMCl2=(TH1F*)HDstPiDM2.Clone();
  HDstPiDMCl2->Divide(HGenDM2);
  TH1F*HDstPiDMCl3=(TH1F*)HDstPiDM3.Clone();
  HDstPiDMCl3->Divide(HGenDM3);
  Canvas.Clear();
  HDstPiDMCl3->GetYaxis()->SetRangeUser(0,.2);
  HDstPiDMCl3->Draw("pe");
  HDstPiDMCl2->Draw("pesame");
  HDstPiDMCl1->Draw("pesame");
  Canvas.Print(filename);

*/
