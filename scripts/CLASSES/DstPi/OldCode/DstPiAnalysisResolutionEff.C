#include "DstPiAnalysis.h"
#include "../JResiduals.h"
#include "../../FUNCTIONS/Fits.h"
#include "../../FUNCTIONS/DetermineFWHM.C"

#define MINENTRIES 500
#define NDMPOINTS 9

#define XLabel "deltaM_{Gen}"

Int_t DstPiAnalysis::ResolutionEff(){
  cout<<"Going to determine the resolution "<<endl;
  if(!_TruthMatch){
    cout<<"Error: This operation is only MC."<<endl;
    return 0;
  }

  ///Open the reduced Ntuple 
  if(!OpenReducedFile())return 0;
  if(!OpenReducedNtuple())return 0; 

  //ps file
  filename=_OutputDir+"/ResolutionEff.ps";
  Canvas.Print(filename+"[");
  
  //create a Tfile where the efficiency parameters will be saved.
  TFile EffFile(_OutputDir+"/ResolutionEff.root","recreate");
  if(!ReducedNtuple){cout<<"no ReducedNtuple"<<endl;return 0;}


  if(!DrawResolutionVsMass())return 0;

  if(!DrawEfficiencyVsMass())return 0;     
  if(!DrawEfficiencyVsPstar())return 0;  
  //if(!DrawEfficiencyVsCosstar())return 0;  
  //if(!Draw2DEfficiency())return 0;
  if(!DrawEfficiencyVsDstarHelicity())return 0;    
  if(!DrawEfficiencyVsDstarAngle())return 0;    
  //if(!Draw3DEfficiency())return 0;
  //if(!Draw3DEfficiencyFit()return 0);
  if(!Draw2DEfficiencyFit())return 0;
  
  EffFile.Close();
  
  Canvas.Print(filename+"]");
  if(CloseReducedNtuple()!=1)return 0;

  if(!CloseReducedFile())return 0;
  
  return 1;

}



TH1* Compute1Defficiency(TH1* HGen,TH1* HRec){
  if(HGen->GetNbinsX()!=HRec->GetNbinsX()){
    cout<<"Compute1Defficiency: histos do not match bins"<<endl; return 0;
  }

    
  TH1*HEff=(TH1*)HRec->Clone();
  HEff->Sumw2();
  delete HEff->FindObject("stats");
  HEff->Divide(HGen);  

  for(Int_t b=1;b<=HEff->GetNbinsX();b++){
    if(HEff->GetBinContent(b)>.0)
      if(HEff->GetBinError(b)/HEff->GetBinContent(b)<.1) continue;
    HEff->SetBinContent(b,0);
    HEff->SetBinError(b,0);
  }
  
  return HEff;
}

TH1* DstPiAnalysis::Plot1DEfficiency(TH1* HGen,TH1* HRec,Bool_t fit){
 
  TH1* HEff=Compute1Defficiency(HGen,HRec);
  if(!HEff){cout<<"Compute1Defficiency: bad imputs"<<endl; return 0;}

  Canvas.Clear(); 
  TF1 fitf("fitf","[0]+[1]*x",HGen->GetXaxis()->GetXmin(),HGen->GetXaxis()->GetXmax());
  if(fit)HEff->Fit("fitf","Q","ape",HGen->GetXaxis()->GetXmin(),HGen->GetXaxis()->GetXmax());
  HEff->GetYaxis()->SetRangeUser(0,1.2*HEff->GetMaximum());
  HEff->SetMarkerSize(1.5);
  HEff->SetLineWidth(2);
  HEff->SetStats(0);
  HEff->SetTitle("");
  HEff->Draw("pe");
  text.SetTextSize(.05);
  text.SetTextColor(2);
  sprintf(textlabel,"Y=(%.3f #pm %.3f) + (%.3f #pm %.3f)*X",fitf.GetParameter(0),fitf.GetParError(0),fitf.GetParameter(1),fitf.GetParError(1));
  text.DrawLatex(HGen->GetXaxis()->GetXmin()+.05*(HGen->GetXaxis()->GetXmax()-HGen->GetXaxis()->GetXmin()),.1*HEff->GetMaximum(),textlabel);
  //Canvas.Update();
  //MoveStatsBox(HEff,1,-1);
  //Canvas.Update();
  Canvas.Print(filename);

  return HEff;

}


Int_t DstPiAnalysis::DrawEfficiencyVsMass(){
  cout<<" DrawEfficiencyVsMass"<<endl;

  ///----------------------------------
  /// rec mass  /gen mass
  //------------------------------------
  TH1F* HGenDM=(TH1F*)ReducedFile->Get("HMCXMass");
  if(!HGenDM){cout<<" Not HMCXMass"<<endl; return 0;}
  HGenDM=(TH1F*)CreateRebinnedDstPiHisto(HGenDM,40,2.1,3.1);
  TH1F HDstPiDM("HDstPiDM","",HGenDM->GetNbinsX(),HGenDM->GetXaxis()->GetXmin(),HGenDM->GetXaxis()->GetXmax());
  ReducedNtuple->Draw("dstpimdm>>HDstPiDM");

//   Canvas.Clear();
//   HGenDM->Draw();
//   HDstPiDM.Draw("same");
//   Canvas.Print(filename);
//   HDstPiDM.GetYaxis()->SetTitle("Efficiency");
//   HDstPiDM.GetXaxis()->SetTitle("m(D*#pi) (GeV/c^2)");
//   delete Plot1DEfficiency(HGenDM,&HDstPiDM,1);

  ///----------------------------------
  ///rec mass - resmas/ gen mass
  //-----------------------------------
  TH1F HDstPiDMRes("HDstPiDMRes","",HGenDM->GetNbinsX(),HGenDM->GetXaxis()->GetXmin(),HGenDM->GetXaxis()->GetXmax());
  ReducedNtuple->Draw("dstpimdm-dstpidmres>>HDstPiDMRes");
  Canvas.Clear();
  HGenDM->Draw();
  HDstPiDMRes.Draw("same");
  Canvas.Print(filename);
  HDstPiDMRes.GetYaxis()->SetTitle("Efficiency");
  HDstPiDMRes.GetXaxis()->SetTitle("m(D*#pi) (GeV/c^2)");
  delete Plot1DEfficiency(HGenDM,&HDstPiDMRes,1);

  return 1;
}

Int_t DstPiAnalysis::DrawResolutionVsMass(){
  cout<<" DrawResolutionVsMass"<<endl;
  
 
  ///-----------------------------
  //Resolution vs Mass
  //------------------------------


  ///plot the resolution as a function of mass
  Float_t fitmin=-.05;
  Float_t fitmax=.05;
  Int_t nbins=200;  
  Float_t dmmin=2.29;//2.430
  Float_t dmmax=2.56;
  Int_t npoints=NDMPOINTS;//check array size below
  Float_t dmstep=(dmmax-dmmin)/npoints;
  TString label;
  TH1F* HDstPiDMResVsM[NDMPOINTS];//array size should be consistent with npoints
  TGraphErrors GResVsM;
  TGraphErrors GAvgVsM;
  TGraphErrors GMeanVsM;
  TGraphErrors GSigma1VsM; 
  TGraphErrors GRatioVsM;
  TGraphErrors GMean2VsM; 
  TGraphErrors GSigma2VsM;
  TGraphErrors GRatio2VsM;
  TGraphErrors GMean3VsM; 
  TGraphErrors GSigma3VsM;
  TGraphErrors GChisVsM;
  Float_t Mean[NDMPOINTS];

 
  //TF1 FRes("FRes","[0]*([1]*[2]*3.14/((x-[1])**2+(.5*[2])**2)+[3]*[1]*[4]*[2]*3.14/((x-[1])**2+(.5*[4]*[2])**2))",fitmin,fitmax);
  //TF1 FRes("FRes","[0]*([1]*[2]*3.14/((x-[1])**2+(.5*[2])**2)+[3]*exp(-.5*(x-[1])**2/([4]*[2])**2))",fitmin,fitmax);
  //TF1 FRes("FRes","[0]*exp(-.5*(x-[1])**2/[2]**2)",fitmin,fitmax);
  /*
  TF1 FRes("FRes","[0]*(exp(-.5*(x-[1])**2/[2]**2)+[3]*exp(-.5*(x-[1])**2/([4]*[2])**2))",fitmin,fitmax);
  FRes.SetParameters(100,0,.001,.1,10);  
  FRes.SetParLimits(0,0,1e6);
  FRes.SetParLimits(1,-.001,.001);
  FRes.SetParLimits(2,.0001,.02);
  FRes.SetParLimits(3,.01,1);
  FRes.SetParLimits(4,1,10);
  FRes.FixParameter(1,0.);
  FRes.FixParameter(3,.06);
  FRes.FixParameter(4,2.5);
  */
 
  //2gaus to fit core only is wrong need to fit tails
  TF1 FRes2("FRes2","[0]*(((1.-[3])/([2]*2.5066))*exp(-.5*(x-[1])**2/[2]**2) + [3]*(1./([5]*[2]*2.5066))*exp(-.5*(x-[1]-[4])**2/([5]*[2])**2))",fitmin,fitmax);
  FRes2.SetParameters(100,0,.001,.1,-.0006,2);  
  FRes2.SetParLimits(0,0,1e6);
  FRes2.SetParLimits(1,fitmin,fitmax);
  FRes2.SetParLimits(2,.000001,.2);
  FRes2.SetParLimits(3,.001,10);
  FRes2.SetParLimits(4,-.002,.002);
  FRes2.SetParLimits(5,1,10);
  FRes2.FixParameter(1,.000040);//.00003
  FRes2.FixParameter(3,.2);//.06 without normalization
  FRes2.FixParameter(4,-.00050);//-.0006
  FRes2.FixParameter(5,2);//2.5

  ///gaus+2BW
  TF1 FRes("FRes","[0]*((1.-[3]-[6])*exp(-.5*(x-[1])**2/[2]**2)/([2]*2.507)+[3]*([5]*[2]/3.14159)/((x-[1]-[4])**2+([5]*[2])**2)+[6]*([8]*[2]/3.14159)/((x-[1]-[7])**2+([8]*[2])**2))",fitmin,fitmax);//  
  FRes.SetParLimits(0,0,1e6);
  FRes.SetParLimits(1,fitmin,fitmax);
  FRes.SetParLimits(2,.0001,.2);
  FRes.SetParLimits(3,.001,10);
  FRes.SetParLimits(4,-.02,.02);
  FRes.SetParLimits(5,.1,10);
  FRes.SetParLimits(6,.001,10);
  FRes.SetParLimits(7,-.02,.02);
  FRes.SetParLimits(8,.1,10);    
  FRes.FixParameter(1,.0000045);
  FRes.FixParameter(3,.044);  
  FRes.FixParameter(4,-0.0041);  
  FRes.FixParameter(5,2.03);   
  FRes.FixParameter(6,0.167);
  FRes.FixParameter(7,0.00026);
  FRes.FixParameter(8,0.63);



  FRes.SetNpx(10000);
  FRes.SetLineWidth(2);
  
  TH1F HDstPiRes("HDstPiRes","HDstPiRes",nbins,fitmin,fitmax);
  ReducedNtuple->Draw("dstpimass-(dstpimdm-dstpidmres)>>HDstPiRes");
  Canvas.Clear();
  HDstPiRes.GetYaxis()->SetRangeUser(0,HDstPiRes.GetMaximum()*1.2);
  HDstPiRes.Draw("pe");
  Canvas.Print(filename);


  TH1F HDstPiDMRes("HDstPiDMRes","HDstPiDMRes",nbins,fitmin,fitmax);
  ReducedNtuple->Draw("dstpidmres>>HDstPiDMRes");
  HDstPiDMRes.GetXaxis()->SetNdivisions(5);
  SetHistoXYLabels(&HDstPiDMRes,"m_{reco.}-m_{gen.}","GeV/c^{2}");
  Canvas.Clear();
  HDstPiDMRes.Draw("pe");
  Canvas.Print(filename);
  HDstPiDMRes.Write();

  TH1F HDstPiDMResSlice("HDstPiDMResSlice","HDstPiDMResSlice",nbins,fitmin,fitmax);
  ReducedNtuple->Draw("dstpidmres>>HDstPiDMResSlice","2.41<dstpimdm-dstpidmres&&dstpimdm-dstpidmres<2.44");
  HDstPiDMResSlice.Fit(&FRes,"QI","pe",fitmin,fitmax); 
  HDstPiDMResSlice.GetXaxis()->SetNdivisions(7);
  SetHistoXYLabels(&HDstPiDMResSlice,"m_{reco.}-m_{gen.}","GeV/c^{2}");
  Canvas.Clear();
  HDstPiDMResSlice.Draw("pe");
  Canvas.SetLogy(1);
  Canvas.Print(filename);
  Canvas.SetLogy(1);
  HDstPiDMResSlice.Write();

  Canvas.Clear();
  HDstPiDMResSlice.Fit(&FRes2,"QI","pe",HDstPiDMResSlice.GetMean()-HDstPiDMResSlice.GetRMS()*4,HDstPiDMResSlice.GetMean()+HDstPiDMResSlice.GetRMS()*4); 
  HDstPiDMResSlice.Draw("pe");
  Canvas.Print(filename);
  Canvas.SetLogy(1);
  HDstPiDMResSlice.Write();
  
  
  text.SetTextSize(.09);
  Int_t pgood=0; 
  Canvas.Clear();
  Canvas.Divide(3,npoints/3);
  Int_t status=0;
  for(Int_t p=0;p<npoints;p++){
    Canvas.cd(p+1);    
    Canvas.GetPad(p+1)->SetLogy(1);
    sprintf(textlabel,"%.5f<dstpimdm-dstpidmres&&dstpimdm-dstpidmres<%.5f",dmmin+p*dmstep,dmmin+(p+1)*dmstep);
    label=TString("HDstPiDMResVsM")+(long)p+"_"+textlabel;    
    HDstPiDMResVsM[p]=new TH1F(label,"",nbins,fitmin,fitmax); 
    ReducedNtuple->Draw("dstpidmres>>"+label,textlabel);
    Mean[p]=HDstPiDMResVsM[p]->GetMean();
    status=HDstPiDMResVsM[p]->Fit(&FRes,"QI","pe",
			   HDstPiDMResVsM[p]->GetMean()-HDstPiDMResVsM[p]->GetRMS()*10,
			   HDstPiDMResVsM[p]->GetMean()+HDstPiDMResVsM[p]->GetRMS()*10); 

    Float_t mean;
    Float_t fwhm;
    Float_t xmax;
    DetermineFWHM(&FRes,&fwhm,&mean,&xmax);
    fwhm=fwhm/2;//plot half width

    if(HDstPiDMResVsM[p]->Integral()>MINENTRIES){
      GAvgVsM.SetPoint(pgood,(dmmin+p*dmstep+dmmin+(p+1)*dmstep)/2,Mean[p]*1000);
      //GAvgVsM.SetPointError(pgood,0,fprim->GetParError(1)*1000);
      GResVsM.SetPoint(pgood,(dmmin+p*dmstep+dmmin+(p+1)*dmstep)/2,1000*fwhm);
      GResVsM.SetPointError(pgood,0,1000*fwhm*(FRes.GetParError(2)/FRes.GetParameter(2)));
      GMeanVsM.SetPoint(pgood,(dmmin+p*dmstep+dmmin+(p+1)*dmstep)/2,1000*FRes.GetParameter(1));
      GMeanVsM.SetPointError(pgood,0,1000*FRes.GetParError(1));
      GSigma1VsM.SetPoint(pgood,(dmmin+p*dmstep+dmmin+(p+1)*dmstep)/2,1000*FRes.GetParameter(2));
      GSigma1VsM.SetPointError(pgood,0,1000*FRes.GetParError(2));
      GRatioVsM.SetPoint(pgood,(dmmin+p*dmstep+dmmin+(p+1)*dmstep)/2,FRes.GetParameter(3));
      GRatioVsM.SetPointError(pgood,0,FRes.GetParError(3));
      GMean2VsM.SetPoint(pgood,(dmmin+p*dmstep+dmmin+(p+1)*dmstep)/2,1000*FRes.GetParameter(4));
      GMean2VsM.SetPointError(pgood,0,1000*FRes.GetParError(4));
      GSigma2VsM.SetPoint(pgood,(dmmin+p*dmstep+dmmin+(p+1)*dmstep)/2,FRes.GetParameter(5));
      GSigma2VsM.SetPointError(pgood,0,FRes.GetParError(5));      
      
      GRatio2VsM.SetPoint(pgood,(dmmin+p*dmstep+dmmin+(p+1)*dmstep)/2,FRes.GetParameter(6));
      GRatio2VsM.SetPointError(pgood,0,FRes.GetParError(6));
      GMean3VsM.SetPoint(pgood,(dmmin+p*dmstep+dmmin+(p+1)*dmstep)/2,1000*FRes.GetParameter(7));
      GMean3VsM.SetPointError(pgood,0,1000*FRes.GetParError(7));
      GSigma3VsM.SetPoint(pgood,(dmmin+p*dmstep+dmmin+(p+1)*dmstep)/2,FRes.GetParameter(8));
      GSigma3VsM.SetPointError(pgood,0,FRes.GetParError(8));      

      GChisVsM.SetPoint(pgood,(dmmin+p*dmstep+dmmin+(p+1)*dmstep)/2,FRes.GetChisquare()/FRes.GetNDF());

      pgood++;      
    }



    HDstPiDMResVsM[p]->Draw("pe");
    Canvas.Update();
    TPaveStats*stats=(TPaveStats*)HDstPiDMResVsM[p]->FindObject("stats");
    if(stats){
      stats->SetX1NDC(.6);stats->SetY1NDC(.2);
      stats->SetX2NDC(.78);stats->SetY2NDC(.82);
    }
    SetHistoXYLabels(HDstPiDMResVsM[p],"","");
    HDstPiDMResVsM[p]->GetXaxis()->SetNdivisions(5,1);
    HDstPiDMResVsM[p]->GetXaxis()->SetLabelSize(.08);
    HDstPiDMResVsM[p]->GetYaxis()->SetNdivisions(7,0);
    HDstPiDMResVsM[p]->GetYaxis()->SetLabelSize(.07);
    HDstPiDMResVsM[p]->GetYaxis()->SetTitleSize(.08);
    HDstPiDMResVsM[p]->GetYaxis()->SetTitleOffset(.65);
    HDstPiDMResVsM[p]->SetStats(0);
    HDstPiDMResVsM[p]->SetTitle("");
    HDstPiDMResVsM[p]->Draw("pe");	
    sprintf(textlabel,"M=[%.2f,%.2f]",dmmin+p*dmstep,dmmin+(p+1)*dmstep);
    text.DrawTextNDC(.0,.9,textlabel);
    sprintf(textlabel,"HWHM=%.2f MeV",fwhm*1000);
    text.DrawTextNDC(.45,.9,textlabel);     
    cout<<p<<" done"<<" "<<fwhm<<"  status="<<status<<" chisq/ndf="<<FRes.GetChisquare()/FRes.GetNDF()<<" "<<HDstPiDMResVsM[p]->Integral()<<endl;
    cout<<p<<" "<<FRes.GetParameter(1)<<" "<<FRes.GetParameter(2)<<" "<<FRes.GetParameter(3)<<" "<<FRes.GetParameter(4)<<" "<<FRes.GetParameter(5)<<" "<<FRes.GetParameter(6)<<" "<<FRes.GetParameter(7)<<" "<<FRes.GetParameter(8)<<endl;
  }  
  Canvas.Print(filename);
  Canvas.SetLogy(0);

  for(Int_t p=0;p<npoints;p++)
    delete  HDstPiDMResVsM[p];    


  Float_t polfitmin=dmmin;
  Float_t polfitmax=dmmax;
  TF1 FPol("FPol","[0]+[1]*x",polfitmin,polfitmax);
  FPol.SetParameters(1,2);

    
  //
  Canvas.Clear();
  GResVsM.SetMarkerSize(1.5);
  GResVsM.GetYaxis()->SetRangeUser(0,6);
  //GResVsM.GetXaxis()->SetRangeUser(dmmin,dmmax);
  GResVsM.GetXaxis()->SetTitle("m(D*#pi) (GeV/c^2)");
  GResVsM.GetYaxis()->SetTitle("HWHM (MeV)");
  FPol.SetParameters(-8,4.);
  GResVsM.Fit(&FPol,"Q","ape",polfitmin,polfitmax);
  GResVsM.Draw("ape");
  Canvas.Print(filename);


  Canvas.Clear();
  GAvgVsM.Draw("ape");
  GAvgVsM.SetMarkerSize(1.5);
  //  GAvgVsM.GetYaxis()->SetRangeUser(-.5,.5);
  GAvgVsM.GetXaxis()->SetTitle("m(D*#pi) (GeV/c^2)");
  GAvgVsM.GetYaxis()->SetTitle("Mean (MeV)");
  FPol.SetParameters(1,2);
  GAvgVsM.Fit(&FPol,"Q","ape",polfitmin,polfitmax);
  Canvas.Print(filename);


  Canvas.Clear();
  GMeanVsM.Draw("ape");
  GMeanVsM.SetMarkerSize(1.5);
  //GMeanVsM.GetYaxis()->SetRangeUser(-.5,.5);
  GMeanVsM.GetXaxis()->SetTitle("m(D*#pi) (GeV/c^2)");
  GMeanVsM.GetYaxis()->SetTitle("mean1 (MeV)");
  FPol.SetParameters(1,2);
  GMeanVsM.Fit(&FPol,"Q","ape",polfitmin,polfitmax);
  Canvas.Print(filename);

  Canvas.Clear();
  GSigma1VsM.Draw("ape");
  GSigma1VsM.SetMarkerSize(1.5);
  //GSigma1VsM.GetYaxis()->SetRangeUser(0,5);
  GSigma1VsM.GetXaxis()->SetTitle("m(D*#pi) (GeV/c^{2})");
  GSigma1VsM.GetYaxis()->SetTitle("sigma1 (MeV)");
  GSigma1VsM.Fit(&FPol,"Q","ape",polfitmin,polfitmax);
  Canvas.Print(filename);

  Canvas.Clear();
  GRatioVsM.Draw("ape");
  GRatioVsM.SetMarkerSize(1.5);
  //GRatioVsM.GetYaxis()->SetRangeUser(0,1);
  GRatioVsM.GetXaxis()->SetTitle("m(D*#pi) (GeV/c^2)");
  GRatioVsM.GetYaxis()->SetTitle("Norm2/Norm1");
  FPol.SetParameters(1,2);
  GRatioVsM.Fit(&FPol,"Q","ape",polfitmin,polfitmax);
  Canvas.Print(filename); 

  Canvas.Clear();
  GMean2VsM.Draw("ape");
  GMean2VsM.SetMarkerSize(1.5);
  //GMean2VsM.GetYaxis()->SetRangeUser(-1.5,.5);
  GMean2VsM.GetXaxis()->SetTitle("m(D*#pi) (GeV/c^2)");
  GMean2VsM.GetYaxis()->SetTitle("mean2 (MeV)");
  FPol.SetParameters(1,2);
  GMean2VsM.Fit(&FPol,"Q","ape",polfitmin,polfitmax);
  Canvas.Print(filename);

  Canvas.Clear();
  GSigma2VsM.Draw("ape");
  GSigma2VsM.SetMarkerSize(1.5);
  //GSigma2VsM.GetYaxis()->SetRangeUser(1,3);
  GSigma2VsM.GetXaxis()->SetTitle("m(D*#pi) (GeV/c^2)");
  GSigma2VsM.GetYaxis()->SetTitle("sigma2/sigma1 (MeV)");
  FPol.SetParameters(1,2);
  GSigma2VsM.Fit(&FPol,"Q","ape",polfitmin,polfitmax);
  Canvas.Print(filename);


  Canvas.Clear();
  GRatio2VsM.Draw("ape");
  GRatio2VsM.SetMarkerSize(1.5);
  //GRatio2VsM.GetYaxis()->SetRangeUser(0,1);
  GRatio2VsM.GetXaxis()->SetTitle("m(D*#pi) (GeV/c^2)");
  GRatio2VsM.GetYaxis()->SetTitle("Norm3/Norm1");
  FPol.SetParameters(1,2);
  GRatio2VsM.Fit(&FPol,"Q","ape",polfitmin,polfitmax);
  Canvas.Print(filename); 

  Canvas.Clear();
  GMean3VsM.Draw("ape");
  GMean3VsM.SetMarkerSize(1.5);
  //GMean3VsM.GetYaxis()->SetRangeUser(-1.5,.5);
  GMean3VsM.GetXaxis()->SetTitle("m(D*#pi) (GeV/c^2)");
  GMean3VsM.GetYaxis()->SetTitle("mean3 (MeV)");
  FPol.SetParameters(1,2);
  GMean3VsM.Fit(&FPol,"Q","ape",polfitmin,polfitmax);
  Canvas.Print(filename);

  Canvas.Clear();
  GSigma3VsM.Draw("ape");
  GSigma3VsM.SetMarkerSize(1.5);
  //GSigma3VsM.GetYaxis()->SetRangeUser(1,3);
  GSigma3VsM.GetXaxis()->SetTitle("m(D*#pi) (GeV/c^2)");
  GSigma3VsM.GetYaxis()->SetTitle("sigma3/sigma1 (MeV)");
  FPol.SetParameters(1,2);
  GSigma3VsM.Fit(&FPol,"Q","ape",polfitmin,polfitmax);
  Canvas.Print(filename);

 
  Canvas.Clear();
  GChisVsM.Draw("ape");
  GChisVsM.SetMarkerSize(1.5);
  GChisVsM.GetYaxis()->SetRangeUser(0,3);
  GChisVsM.GetXaxis()->SetTitle("m(D*#pi) (GeV/c^2)");
  GChisVsM.GetYaxis()->SetTitle("#chi^{2}/ndf");
  Canvas.Print(filename);
  return 1;
}


Int_t DstPiAnalysis::DrawEfficiencyVsPstar(){
  cout<<"DrawEfficiencyVsPstar "<<endl;
  ///efficiency vs pstar
  TH1F* HGenPstar=(TH1F*)ReducedFile->Get("HMCXp3CM");
  if(!HGenPstar)return 0; 
  
  TH1F HDstPiPstar("HDstPiPstar","HDstPiPstar",HGenPstar->GetXaxis()->GetNbins(),HGenPstar->GetXaxis()->GetXmin(),HGenPstar->GetXaxis()->GetXmax());
  SetHistoXYLabels(&HDstPiPstar,"p*(D*#pi)","GeV"); 
  ReducedNtuple->Draw("dstpipstar>>HDstPiPstar");
  
 
  Canvas.Clear();
  HGenPstar->SetTitle("");
  HGenPstar->Draw();
  HDstPiPstar.Draw("same");
  Canvas.Print(filename);
    

  Plot1DEfficiency(HGenPstar,&HDstPiPstar,1);
  return 1;
}

Int_t DstPiAnalysis::DrawEfficiencyVsCosstar(){
  cout<<" DrawEfficiencyVsCosstar"<<endl;
  ///efficiency vs pstar
  TH1F* HGenCosstar=(TH1F*)ReducedFile->Get("HMCXcosthCM");
  if(!HGenCosstar)return 0;
  
  TH1F HDstPiCosstar("HDstPiCosstar","HDstPiCosstar",HGenCosstar->GetXaxis()->GetNbins(),HGenCosstar->GetXaxis()->GetXmin(),HGenCosstar->GetXaxis()->GetXmax());
  SetHistoXYLabels(&HDstPiCosstar,"cos(#theta*)",""); 
  ReducedNtuple->Draw("dstpicosstar>>HDstPiCosstar");
   
  Canvas.Clear();
  HGenCosstar->GetYaxis()->SetRangeUser(0,HGenCosstar->GetMaximum()*1.1);
  HGenCosstar->Draw();
  HDstPiCosstar.Draw("same");
  Canvas.Print(filename);
    

  Plot1DEfficiency(HGenCosstar,&HDstPiCosstar,0);


//   ///resolutionn
//   TH1F HCosstarRes("HCosstarRes","HCosstarRes",200,-.01,.01);
//   HCosstarRes.GetXaxis()->SetNdivisions(4);
//   SetHistoXYLabels(&HCosstarRes,"cos(#theta*)_{Rec}-cos(#theta*)_{Gen}",""); 
//   Canvas.Clear();
//   ReducedNtuple->Draw("dstpicosstarres>>HCosstarRes");
//   Canvas.Print(filename);

  return 1;
}


Int_t DstPiAnalysis::DrawEfficiencyVsDstarHelicity(){
  cout<<"Doing the Dstar Helicity Efficiency"<<endl;
 
  HMCDstarHelicity=(TH1F*)ReducedFile->Get("HMCDstarHelicity");
  if(!HMCDstarHelicity){
    cout<<"No HMCDstarHelicity histogram found"<<endl;
    return 0;
  }
  
  TH1F HDstarHelicity("HDstarHelicity","HDstarHelicity",HMCDstarHelicity->GetXaxis()->GetNbins(),HMCDstarHelicity->GetXaxis()->GetXmin(),HMCDstarHelicity->GetXaxis()->GetXmax());
  SetHistoXYLabels(&HDstarHelicity,"cos(#theta_{H})",""); 
  ReducedNtuple->Draw("dstarhelicity>>HDstarHelicity");
   
  Canvas.Clear();
  HMCDstarHelicity->GetYaxis()->SetRangeUser(0,HMCDstarHelicity->GetMaximum()*1.1);
  HMCDstarHelicity->Draw();
  HDstarHelicity.Draw("same");
  Canvas.Print(filename);


  TH1*HEff=Plot1DEfficiency(HMCDstarHelicity,&HDstarHelicity,1);
  HEff->SetName("HDstarHeicityEfficiency");
  HEff->Write();

//   ///resolution
//   TH1F HDstarHelicityRes("HDstarHelicityRes","HDstarHelicityRes",200,-.1,.1);
//   HDstarHelicityRes.GetXaxis()->SetNdivisions(4);
//   SetHistoXYLabels(&HDstarHelicityRes,"cos(#theta)_{Rec}-cos(#theta)_{Gen}",""); 
//   Canvas.Clear();
//   ReducedNtuple->Draw("dstarhelicityres>>HDstarHelicityRes");
//   Canvas.Print(filename);

  return 1;
}

Int_t DstPiAnalysis::DrawEfficiencyVsDstarAngle(){
  cout<<"Doing the X-Dstar Angle Efficiency"<<endl;

  //Efficiency 
  HMCDstarAngle=(TH1F*)ReducedFile->Get("HMCDstarAngle");
  if(!HMCDstarAngle){
    cout<<"No histogram found"<<endl;
    return 0;
  }
  
  TH1F HDstarAngle("HDstarAngle","HDstarAngle",HMCDstarAngle->GetXaxis()->GetNbins(),HMCDstarAngle->GetXaxis()->GetXmin(),HMCDstarAngle->GetXaxis()->GetXmax());
  SetHistoXYLabels(&HDstarAngle,"cos(#theta_{D*})",""); 
  ReducedNtuple->Draw("dstarcostheta>>HDstarAngle");
   
  Canvas.Clear();
  HMCDstarAngle->GetYaxis()->SetRangeUser(0,HMCDstarAngle->GetMaximum()*1.1);
  HMCDstarAngle->Draw();
  HDstarAngle.Draw("same");
  Canvas.Print(filename);
    
  TH1*HEff=Plot1DEfficiency(HMCDstarAngle,&HDstarAngle,1);
  HEff->SetName("HDstarAngleEfficiency");
  HEff->Write();

//   //look at the dependence on p*
//   ReducedNtuple->Draw("dstarcostheta>>HDstarAngle","dstpipstar>4");    
//   TH1*HEffhighp=Plot1DEfficiency(HMCDstarAngle,&HDstarAngle,1);
//   ReducedNtuple->Draw("dstarcostheta>>HDstarAngle","dstpipstar<4");    
//   TH1*HEfflowp=Plot1DEfficiency(HMCDstarAngle,&HDstarAngle,1);
//   Canvas.Clear();
//   HEfflowp->Add(HEffhighp,-1);
//   HEfflowp->Draw();  
//   Canvas.Print(filename);    
//   delete HEffhighp;
//   delete HEfflowp;


//   //look at the dependence on cos*
//   ReducedNtuple->Draw("dstarcostheta>>HDstarAngle","dstpicosstar>0");    
//   TH1*HEffhighc=Plot1DEfficiency(HMCDstarAngle,&HDstarAngle,1);
//   ReducedNtuple->Draw("dstarcostheta>>HDstarAngle","dstpicosstar<0");    
//   TH1*HEfflowc=Plot1DEfficiency(HMCDstarAngle,&HDstarAngle,1);
//   Canvas.Clear();
//   HEfflowc->Add(HEffhighc,-1);
//   HEfflowc->Draw();  
//   Canvas.Print(filename);    
//   delete HEffhighc;
//   delete HEfflowc;
  

  ///resolution
  TH1F HDstarCosthetaRes("HDstarCosthetaRes","HDstarCosthetaRes",200,-.03,.03);
  HDstarCosthetaRes.GetXaxis()->SetNdivisions(4);
  SetHistoXYLabels(&HDstarCosthetaRes,"cos(#theta)_{Rec}-cos(#theta)_{Gen}",""); 
  Canvas.Clear();
  ReducedNtuple->Draw("dstarcosthetares>>HDstarCosthetaRes");
  Canvas.Print(filename);

  return 1;
}

Int_t DstPiAnalysis::Draw2DEfficiency(){
  cout<<"Drawing 2-D Efficiency: cos* vs p*"<<endl;

  H2MCXCMPvsTheta=(TH2F*)ReducedFile->Get("H2MCXCMPvsTheta");
  if(!H2MCXCMPvsTheta){
    cout<<"No histogram found"<<endl;
    return 0;
  }
  H2MCXCMPvsTheta=(TH2F*)H2MCXCMPvsTheta->Rebin2D(5,4,"2DRebin");
  
  
  TH2F H2XCMPvsTheta("H2XCMPvsTheta","H2XCMPvsTheta",
		     H2MCXCMPvsTheta->GetXaxis()->GetNbins(),H2MCXCMPvsTheta->GetXaxis()->GetXmin(),H2MCXCMPvsTheta->GetXaxis()->GetXmax(),
		     H2MCXCMPvsTheta->GetYaxis()->GetNbins(),H2MCXCMPvsTheta->GetYaxis()->GetXmin(),H2MCXCMPvsTheta->GetYaxis()->GetXmax());  
  ReducedNtuple->Draw("dstpicosstar:dstpipstar>>H2XCMPvsTheta");


  Canvas.Clear();
  Canvas.Divide(2,2);
  Canvas.cd(1);  
  H2MCXCMPvsTheta->Draw("colz");
  Canvas.cd(2);  
  H2XCMPvsTheta.Draw("colz");

  
  //calculate the efficiency
  TH2*H2Eff=(TH2*)H2XCMPvsTheta.Clone();
  H2Eff->Sumw2();
  H2Eff->Divide(H2MCXCMPvsTheta);
  H2Eff->SetTitle("Efficiency");

  //plot the errors on the 2D efficiency  
  TH2*H2EffErr=(TH2*)H2Eff->Clone();
  for(Int_t x=1;x<=H2Eff->GetNbinsX();x++)
    for(Int_t y=1;y<=H2Eff->GetNbinsY();y++){

      if(H2Eff->GetBinContent(x,y)>0){
	if(H2Eff->GetBinError(x,y)/H2Eff->GetBinContent(x,y)<.1)
	  H2EffErr->SetBinContent(x,y,H2Eff->GetBinError(x,y)/H2Eff->GetBinContent(x,y));
	else{
	  H2Eff->SetBinContent(x,y,0);
	  H2EffErr->SetBinContent(x,y,0);
	}
      }else{
	H2Eff->SetBinContent(x,y,0);
	H2EffErr->SetBinContent(x,y,0);
      }
      
    }
   
  H2EffErr->SetTitle("EfficiencyError/Efficiency");  


  Canvas.cd(3);    
  H2Eff->Draw("colz");

  Canvas.cd(4); 
  H2EffErr->Draw("colz");
  

  Canvas.Print(filename);
  H2Eff->SetName("H2DEfficiency");
  H2Eff->Write();

  delete H2Eff;
  delete H2EffErr;
  
  return 1;
}



void Fill2DRelativeEffErr(TH2F* heff,TH2F* hefferr){

    for(Int_t x=1;x<=heff->GetNbinsX();x++){
      for(Int_t y=1;y<=heff->GetNbinsY();y++){
	if(heff->GetBinContent(x,y)>0)
	  if(heff->GetBinError(x,y)/heff->GetBinContent(x,y)<.2)
	    hefferr->SetBinContent(x,y,heff->GetBinError(x,y)/heff->GetBinContent(x,y));      
	  else hefferr->SetBinContent(x,y,0);
	else hefferr->SetBinContent(x,y,0);
      }
    }        
  
}




TH2F* DstPiAnalysis::Interpolate2DEfficiencyFitX(TH2F* h2,TH1F* HEffChiX){
  if(!h2){
    cout<<"No histo"<<endl;
    return 0;
  }
   
  TString name=TString(h2->GetName())+"InterX";
  
  Int_t nbinsx=h2->GetNbinsX();
  Int_t nbinsy=h2->GetNbinsY();  
  TH2F* H2Interpolation=new TH2F(name,name,
				 40,h2->GetXaxis()->GetXmin(),h2->GetXaxis()->GetXmax(),
				 nbinsy,h2->GetYaxis()->GetXmin(),h2->GetYaxis()->GetXmax());
  //2000/40=50MeV binning in p*

  TH1F HSlice("HSlice","HSlice",nbinsx,h2->GetXaxis()->GetXmin(),h2->GetXaxis()->GetXmax());  
  TF1 FSlice("FSlice","[0]+[1]*(x-[2])",h2->GetXaxis()->GetXmin(),h2->GetXaxis()->GetXmax());
  FSlice.SetParameters(.5,.01,4);

  (*HEffChiX).SetNameTitle(name+"chi",name+"chi");
  (*HEffChiX).SetBins(nbinsy,h2->GetYaxis()->GetXmin(),h2->GetYaxis()->GetXmax());
  
  Float_t eff;
  Float_t error;
  Float_t pars[3];
  Float_t chiperdf;
  Int_t ngood=0;
  for(Int_t by=1;by<=nbinsy;by++){
    ngood=0;
    for(Int_t bx=1;bx<=nbinsx;bx++){
      if(h2->GetBinContent(bx,by)>0){
	ngood++;
	HSlice.SetBinContent(bx,h2->GetBinContent(bx,by));
	HSlice.SetBinError(bx,h2->GetBinError(bx,by));
      }else{
	HSlice.SetBinContent(bx,0);
	HSlice.SetBinError(bx,0);
      }          
    }     
    HSlice.SetBinContent(nbinsx,0);//do not use last bin in fit
    HSlice.SetBinError(nbinsx,0);
    FSlice.FixParameter(2,HSlice.GetMean());
    if(ngood<3||HSlice.Fit(&FSlice,"QCF","",h2->GetXaxis()->GetXmin(),h2->GetXaxis()->GetXmax())!=0){
      if(ngood>=3)cout<<"*********fit failed: bin "<<name<<" "<<by<<endl;
      FSlice.SetParameter(0,0);
      FSlice.SetParameter(1,0);
      FSlice.SetParError(0,0);
      FSlice.SetParError(1,0);
      FSlice.SetChisquare(0);
      chiperdf=0;
    }else 
      chiperdf=FSlice.GetChisquare()/FSlice.GetNDF(); 
    
    (*HEffChiX).SetBinContent(by,chiperdf);

    pars[0]=FSlice.GetParameter(0);
    pars[1]=FSlice.GetParameter(1);
    pars[2]=FSlice.GetParameter(2);

    //fill the new histo
    for(Int_t bx=1;bx<=H2Interpolation->GetNbinsX();bx++){ 
      FSlice.SetParameters(pars[0],pars[1],pars[2]);
      eff=FSlice.Eval(H2Interpolation->GetXaxis()->GetBinCenter(bx));
      H2Interpolation->SetBinContent(bx,by,eff);
      
      error=sqrt(FSlice.GetParError(0)*FSlice.GetParError(0)
      		 +(H2Interpolation->GetXaxis()->GetBinCenter(bx)-FSlice.GetParameter(2))
		 *(H2Interpolation->GetXaxis()->GetBinCenter(bx)-FSlice.GetParameter(2))
		 *FSlice.GetParError(1)
		 *FSlice.GetParError(1));
      
      H2Interpolation->SetBinError(bx,by,error);
    }
    
  }
  

  
  return H2Interpolation;
}




TH2F* DstPiAnalysis::Interpolate2DEfficiencyFitY(TH2F* h2,TH1F* HEffChiY){
  if(!h2){
    cout<<"No histo"<<endl;
    return 0;
  }
   
  TString name=TString(h2->GetName())+"InterY";
  
  Int_t nbinsx=h2->GetNbinsX();
  Int_t nbinsy=h2->GetNbinsY();  
  TH2F* H2Interpolation=new TH2F(name,name,
				 nbinsx,h2->GetXaxis()->GetXmin(),h2->GetXaxis()->GetXmax(),
				 500,h2->GetYaxis()->GetXmin(),h2->GetYaxis()->GetXmax());
  //1000/500=2MeV
  
  //there are 40 bins in 2.1-->3.1: use 5 bins 2.375-->2.500 => bins 12->16
  TH1F HSlice("HSlice","HSlice",nbinsy,h2->GetYaxis()->GetXmin(),h2->GetYaxis()->GetXmax());  
  TF1 FSlice("FSlice","[0]+[1]*(x-[2])",h2->GetYaxis()->GetXmin(),h2->GetYaxis()->GetXmax());
  FSlice.SetParameters(.5,-.01,2.44);

  //
  (*HEffChiY).SetNameTitle(name+"chi",name+"chi");
  (*HEffChiY).SetBins(nbinsx,h2->GetXaxis()->GetXmin(),h2->GetXaxis()->GetXmax());

  Float_t eff;
  Float_t error;
  Float_t pars[3];
  Float_t chiperdf;
  Int_t ngood=0;
  for(Int_t bx=1;bx<=nbinsx;bx++){
    //fill the slice then fit
    ngood=0;
    for(Int_t by=1;by<=nbinsy;by++){
      if(h2->GetBinContent(bx,by)>0){
	ngood++;
	HSlice.SetBinContent(by,h2->GetBinContent(bx,by));	
	//if(bx==20)cout<<" "<<by<<" "<<h2->GetBinContent(bx,by)<<endl;
	HSlice.SetBinError(by,h2->GetBinError(bx,by));
      }else{
	HSlice.SetBinContent(by,0);
	HSlice.SetBinError(by,0);
      }            
    }
    FSlice.FixParameter(2,HSlice.GetMean());
    if(ngood<3||HSlice.Fit(&FSlice,"QCF","",h2->GetYaxis()->GetXmin(),h2->GetYaxis()->GetXmax())!=0){
      if(ngood>=3)cout<<"*********fit failed: bin "<<bx<<endl; 
      FSlice.SetParameter(0,0);//HSlice.Integral(),0);
      FSlice.SetParameter(1,0);
      FSlice.SetParError(0,0);
      FSlice.SetParError(1,0);
      FSlice.SetChisquare(0);
      chiperdf=0;
    }else 
      chiperdf=FSlice.GetChisquare()/FSlice.GetNDF();
 
    (*HEffChiY).SetBinContent(bx,chiperdf);   
        
    pars[0]=FSlice.GetParameter(0);
    pars[1]=FSlice.GetParameter(1);
    pars[2]=FSlice.GetParameter(2);
    
    //fill the new histo
    for(Int_t by=1;by<=H2Interpolation->GetNbinsY();by++){ 
      FSlice.SetParameters(pars[0],pars[1],pars[2]);
      eff=FSlice.Eval(H2Interpolation->GetYaxis()->GetBinCenter(by));
      H2Interpolation->SetBinContent(bx,by,eff);
     
      error=sqrt(FSlice.GetParError(0)*FSlice.GetParError(0)
      		 +(H2Interpolation->GetYaxis()->GetBinCenter(by)-FSlice.GetParameter(2))
		 *(H2Interpolation->GetYaxis()->GetBinCenter(by)-FSlice.GetParameter(2))
		 *FSlice.GetParError(1)
		 *FSlice.GetParError(1));
      H2Interpolation->SetBinError(bx,by,error);//*sqrt(chiperdf));

     
    }
    
      
  }
  

  
  return H2Interpolation;
}




Int_t DstPiAnalysis::Draw2DEfficiencyFit(){
  cout<<"Computing 2-D Efficiency pstar vs mass "<<endl;
    
  H2MCPvsMass=(TH2F*)ReducedFile->Get("H2MCPvsMass");
  if(!H2MCPvsMass){
    cout<<"No histogram found"<<endl;
    return 0;
  }

  ///Efficiency map used for MC
  TH2F H2PvsMMC("H2PvsMMC","MC",
	      H2MCPvsMass->GetXaxis()->GetNbins(),
	      H2MCPvsMass->GetXaxis()->GetXmin(),
	      H2MCPvsMass->GetXaxis()->GetXmax(),
	      H2MCPvsMass->GetYaxis()->GetNbins(),
	      H2MCPvsMass->GetYaxis()->GetXmin(),
	      H2MCPvsMass->GetYaxis()->GetXmax());
  ReducedNtuple->Draw("(dstpimdm-dstpidmres):dstpipstar>>H2PvsMMC");
  Compute2DEfficiencyFit(&H2PvsMMC,H2MCPvsMass);
  

  ///loop to fill reco histo with pid weights 
  TH2F H2PvsMData("H2PvsMData","Data",
	      H2MCPvsMass->GetXaxis()->GetNbins(),
	      H2MCPvsMass->GetXaxis()->GetXmin(),
	      H2MCPvsMass->GetXaxis()->GetXmax(),
	      H2MCPvsMass->GetYaxis()->GetNbins(),
	      H2MCPvsMass->GetYaxis()->GetXmin(),
	      H2MCPvsMass->GetYaxis()->GetXmax());
  H2PvsMData.Sumw2();  
  Int_t evid=0;
  while(ReducedNtuple->GetEntry(evid,0)){
    evid++;
    H2PvsMData.Fill(dstpipstar,dstpimdm,dstpipidw);
  }  
  Compute2DEfficiencyFit(&H2PvsMData,H2MCPvsMass);
  //show pid weights
  Canvas.Clear();
  TH1F Hpidw("Hpidw","",60,.7,1.3);
  Hpidw.GetXaxis()->SetTitle("data efficiency / MC efficiency");
  ReducedNtuple->Draw("dstpipidw>>Hpidw");
  Canvas.Print(filename);

  Canvas.Clear();
  TH1F Hpidwe("Hpidwe","",100,0,.1);
  Hpidwe.GetXaxis()->SetTitle("error on (data efficiency / MC efficiency)");
  ReducedNtuple->Draw("dstpipidwe>>Hpidwe");
  Canvas.Print(filename);

  return 1;   
}


Int_t DstPiAnalysis::Compute2DEfficiencyFit(TH2F*H2Reco,TH2F*H2Gen){
  

  //Show the efficiency shapes
  TH1F htcorr("htcorr","htcorr",H2Gen->GetYaxis()->GetNbins(),
	      H2Gen->GetYaxis()->GetXmin(),
	      H2Gen->GetYaxis()->GetXmax());
  htcorr.Sumw2();
  TH1F ht("ht","ht",H2Gen->GetYaxis()->GetNbins(),
	  H2Gen->GetYaxis()->GetXmin(),
	  H2Gen->GetYaxis()->GetXmax());

  TF1 fitf("fitf","[0]+[1]*x",2.1,3.1);
  fitf.SetParameters(1,.01);
  TH1*htcorraddcl[4];  
  //Float_t totalN=H2Reco->Integral();
  Int_t cid=1;
  Canvas.Clear();
  Canvas.Divide(2,2); 
  for(Int_t pb=1;pb<=4;pb++){      
    Canvas.cd(cid);    
    for(Int_t y=1;y<=ht.GetNbinsX();y++){
      htcorr.SetBinContent(y,H2Reco->GetBinContent(pb,y));	                       
      htcorr.SetBinError(y,H2Reco->GetBinError(pb,y));
      ht.SetBinContent(y,H2Gen->GetBinContent(pb,y));
      ht.SetBinError(y,H2Gen->GetBinError(pb,y));	
    }
    htcorraddcl[cid-1]=(TH1*)htcorr.Clone();
    htcorraddcl[cid-1]->SetName(TString(htcorr.GetName())+(long)cid);
    htcorraddcl[cid-1]->Divide(&ht);
    htcorraddcl[cid-1]->Fit(&fitf);      
    
    sprintf(textlabel,"p*=[%.1f,%.1f], slope=%.2f+-%.2f",H2Reco->GetXaxis()->GetBinCenter(pb)-H2Reco->GetXaxis()->GetBinWidth(1)/2,H2Reco->GetXaxis()->GetBinCenter(pb+1)-H2Reco->GetXaxis()->GetBinWidth(1)/2,fitf.GetParameter(1),fitf.GetParError(1));
    htcorraddcl[cid-1]->SetTitle(textlabel);
    htcorraddcl[cid-1]->GetYaxis()->SetRangeUser(0,htcorraddcl[cid-1]->GetMaximum()*1.2);
    htcorraddcl[cid-1]->SetStats(0);
    htcorraddcl[cid-1]->GetYaxis()->SetTitle("Efficiency");
    htcorraddcl[cid-1]->GetXaxis()->SetTitle(DstarPiXLabel);
    htcorraddcl[cid-1]->Draw("pe");
    cid++;
  }    

  Canvas.Print(filename);


  //compare y projection of 3-D rec mass to 3D projection of gen mass
  TH1*hgenmass=H2Gen->ProjectionY("hgenmass",1,H2Gen->GetNbinsX(),"");  
  TH1*hrecmass=H2Reco->ProjectionY("hrecmass",1,H2Gen->GetNbinsX(),"");   
  hrecmass->SetTitle("");
  hrecmass->SetStats(0);
  hrecmass->GetYaxis()->SetTitle("Efficiency");
  hrecmass->GetXaxis()->SetTitle(DstarPiXLabel);
  delete Plot1DEfficiency(hgenmass,hrecmass,1);


  TH1*hgenpstar=H2Gen->ProjectionX("hgenpstar",1,H2Gen->GetNbinsY(),"");  
  TH1*hrecpstar=H2Reco->ProjectionX("hrecpstar",1,H2Gen->GetNbinsY(),"");   
  hrecpstar->SetTitle("");
  hrecpstar->SetStats(0);
  hrecpstar->GetYaxis()->SetTitle("Efficiency");
  hrecpstar->GetXaxis()->SetTitle("p*(D*#pi)");
  delete Plot1DEfficiency(hgenpstar,hrecpstar,1);
  //---------------------------------------

 

  Float_t maxefferr=.1;  
  //calculate the efficiency
  TH2F*H2Eff=(TH2F*)H2Reco->Clone();
  H2Eff->Sumw2();
  H2Eff->Divide(H2Gen);
  //H2Eff->SetTitle("H2DEfficiency"); 
  //clean out the large errors
  for(Int_t i=1;i<=H2Eff->GetXaxis()->GetNbins();i++)
    for(Int_t j=1;j<=H2Eff->GetYaxis()->GetNbins();j++)
      if(H2Eff->GetBinError(i,j)/H2Eff->GetBinContent(i,j)>maxefferr){
	H2Eff->SetBinError(i,j,0);
	H2Eff->SetBinContent(i,j,0);
      }
 

  Canvas.Clear();
  H2Eff->GetYaxis()->SetTitle(DstarPiXLabel);
  H2Eff->GetXaxis()->SetTitle("p* GeV/c");
  H2Eff->GetZaxis()->SetTitle("Efficiency");
  H2Eff->SetStats(0);
  H2Eff->SetTitle(""); 
  H2Eff->Draw("colz"); 
  Canvas.Print(filename);
  //plot the relative error in the efficiency
  TH2F* H2EffErr;   
  H2EffErr=(TH2F*)H2Eff->Clone();    
  Fill2DRelativeEffErr(H2Eff,H2EffErr);
  Canvas.Clear();
  H2EffErr->SetStats(0);
  H2EffErr->SetTitle("");    
  H2EffErr->GetZaxis()->SetTitle("Efficiency Error/ Efficiency");
  H2EffErr->Draw("colz");   
  Canvas.Update();
  Canvas.Print(filename);
  delete H2EffErr;


 
  //--------------------------------
  //make interpolation in p*
  //--------------------------------
  Canvas.Clear();
  TH2F* H2EffInterX;
  TH1F HEffChiX;
  H2EffInterX=Interpolate2DEfficiencyFitX(H2Eff,&HEffChiX);
  H2EffInterX->GetYaxis()->SetTitle(DstarPiXLabel);
  H2EffInterX->GetXaxis()->SetTitle("p* GeV/c");
  H2EffInterX->GetZaxis()->SetTitle("Efficiency");
  H2EffInterX->SetStats(0);  
  H2EffInterX->SetTitle("");
  H2EffInterX->Draw("colz");  
  Canvas.Print(filename); 
  H2EffErr=(TH2F*)H2EffInterX->Clone();   			     
  Fill2DRelativeEffErr(H2EffInterX,H2EffErr);    
  Canvas.Clear();
  H2EffErr->SetStats(0);
  H2EffErr->SetTitle("");
  H2EffErr->GetZaxis()->SetTitle("Efficiency Error/ Efficiency");
  H2EffErr->Draw("colz");    
  Canvas.Print(filename); 
  Canvas.Clear();
  HEffChiX.SetStats(0);
  HEffChiX.SetTitle("");
  HEffChiX.GetXaxis()->SetTitle("m(D*#pi) (GeV/c)");
  HEffChiX.GetYaxis()->SetTitle("#chi^{2}/NDF");
  HEffChiX.Draw("p");
  Canvas.Print(filename); 


    
  //-----------------------------------
  //make interpolation in mass
  //----------------------------------
  Canvas.Clear();
  TH2F* H2EffInterY;
  TH1F HEffChiY;
  H2EffInterY=Interpolate2DEfficiencyFitY(H2EffInterX,&HEffChiY);   
  H2EffInterY->GetYaxis()->SetTitle(DstarPiXLabel);
  H2EffInterY->GetXaxis()->SetTitle("p* GeV/c");
  H2EffInterY->GetZaxis()->SetTitle("Efficiency");     
  H2EffInterY->SetStats(0);
  H2EffInterY->SetTitle("");
  H2EffInterY->Draw("colz"); 
  Canvas.Print(filename); 
  H2EffErr=(TH2F*)H2EffInterY->Clone();   		
  Fill2DRelativeEffErr(H2EffInterY,H2EffErr);
  Canvas.Clear(); 
  H2EffErr->SetStats(0);
  H2EffErr->SetTitle("");
  H2EffErr->GetZaxis()->SetTitle("Efficiency Error/ Efficiency");
  H2EffErr->Draw("colz");    
  Canvas.Print(filename);
  Canvas.Clear();
  HEffChiY.SetStats(0);
  HEffChiY.SetTitle("");
  HEffChiY.GetXaxis()->SetTitle("p*   (GeV/c)");
  HEffChiY.GetYaxis()->SetTitle("#chi^{2}/NDF");
  HEffChiY.Draw("p");
  Canvas.Print(filename); 


  H2EffInterY->SetName(TString("H2Efficiency")+H2Reco->GetTitle());
  H2EffInterY->Write();  
  delete H2EffInterX; 
  delete H2EffInterY;


  return 1;   
}




//methods for using the Efficiency:
Int_t DstPiAnalysis::OpenEfficiency(){
  
  ///these are the the 3D efficiency maps: Eff vs. (p*,cos*,cos_D*)
  if(_D0Mode==1)EffFile=new TFile("DstarCharged/DstarPi/MC/D2420andD2460/Kpi/ResolutionEff.root","read");
  if(_D0Mode==2)EffFile=new TFile("DstarCharged/DstarPi/MC/D2420andD2460/K3pi/ResolutionEff.root","read");
  if(!EffFile){cout<<"No Efficiency file"<<endl;return 0;}
  if(EffFile->IsZombie()){cout<<"No Efficiency file"<<endl;return 0;}
  if(_MCorDATA==0)H2Efficiency=(TH2F*)EffFile->Get("H2EfficiencyMC");
  if(_MCorDATA==1)H2Efficiency=(TH2F*)EffFile->Get("H2EfficiencyData");//includes pid weight correction
  if(!H2Efficiency){cout<<"No Efficiency Histogram"<<endl;return 0;}
  gROOT->cd();


  //shape factors for angular distributions
  TF1 FHel("FHel","[0]+[1]*x",-1,1);
  FHel.SetParameters(helioffset,helislope);
  FHelEff=new TF1("FHelEff","([0]+[1]*x)/[2]",-1,1);
  FHelEff->SetParameters(FHel.GetParameter(0),FHel.GetParameter(1),FHel.Eval(0));//note: efficiency is relative to midpoint
  //
  Float_t cosmin=-1.,cosmax=.5; 
  TF1 FCosDst("FCosDst","[0]+[1]*x",cosmin,cosmax);
  FCosDst.SetParameters(cosdstoffset,cosdstslope);  
  FCosDstEff=new TF1("FCosDstEff","([0]+[1]*x)/[2]",cosmin,cosmax);
  FCosDstEff->SetParameters(FCosDst.GetParameter(0),FCosDst.GetParameter(1),FCosDst.Eval((cosmax+cosmin)/2));
  
  //
  effpstarstep=H2Efficiency->GetXaxis()->GetBinWidth(1);
  effpstarmin=H2Efficiency->GetXaxis()->GetXmin();

  effmassstep=H2Efficiency->GetYaxis()->GetBinWidth(1);
  effmassmin=H2Efficiency->GetYaxis()->GetXmin();

  return 1;
}
Float_t DstPiAnalysis::GetEfficiency(){
  //return H3Efficiency->GetBinContent(1+(int)((dstpipstar-effpstarmin)/effpstarstep),
  //1+(int)((dstpicosstar-effcosstarmin)/effcosstarstep),
  // 1+(int)((dstarcostheta-effcosdstarmin)/effcosdstarstep))*FHelEff->Eval(dstarhelicity)*FMassEff->Eval(DstarPDGMass + dstpideltam);

//   return H3Efficiency->GetBinContent(1+(int)((dstpipstar-effpstarmin)/effpstarstep),
// 				     1+(int)((dstpimdm-effmassmin)/effmassstep),
// 				     1+(int)((dstarcostheta-effcosdstarmin)/effcosdstarstep));

  return H2Efficiency->GetBinContent(1+(int)((dstpipstar-effpstarmin)/effpstarstep),1+(int)((dstpimdm-effmassmin)/effmassstep))*FHelEff->Eval(dstarhelicity)*FCosDstEff->Eval(dstarcostheta);
  
}
Float_t DstPiAnalysis::GetEfficiencyNoShape(){
  //return H3Efficiency->GetBinContent(1+(int)((dstpipstar-effpstarmin)/effpstarstep),
  //1+(int)((dstpicosstar-effcosstarmin)/effcosstarstep),
  //  1+(int)((dstarcostheta-effcosdstarmin)/effcosdstarstep));
  
//   return H3Efficiency->GetBinContent(1+(int)((dstpipstar-effpstarmin)/effpstarstep),
// 				     1+(int)((dstpimdm-effmassmin)/effmassstep),
// 				     1+(int)((dstarcostheta-effcosdstarmin)/effcosdstarstep));

  return H2Efficiency->GetBinContent(1+(int)((dstpipstar-effpstarmin)/effpstarstep),1+(int)((dstpimdm-effmassmin)/effmassstep));
}
Float_t DstPiAnalysis::GetEfficiencyErr(){
//   return H3Efficiency->GetBinError(1+(int)((dstpipstar-effpstarmin)/effpstarstep),
// 				   1+(int)((dstpicosstar-effcosstarmin)/effcosstarstep),
// 				   1+(int)((dstarcostheta-effcosdstarmin)/effcosdstarstep));

//   return H3Efficiency->GetBinError(1+(int)((dstpipstar-effpstarmin)/effpstarstep),
// 				   1+(int)((dstpimdm-effmassmin)/effmassstep),
// 				   1+(int)((dstarcostheta-effcosdstarmin)/effcosdstarstep));

  return H2Efficiency->GetBinError(1+(int)((dstpipstar-effpstarmin)/effpstarstep),1+(int)((dstpimdm-effmassmin)/effmassstep));

}
void DstPiAnalysis::CloseEfficiency(){
  EffFile->Close();
  delete EffFile;
  
  delete FHelEff;
  delete FCosDstEff;
}



Int_t DstPiAnalysis::EffAfterEffCorr2(){
  cout<<"Going to check the efficiency  shape in mass and dstarhelicity after the 3-D efficiency correction  "<<endl;
  if(!_TruthMatch){
    cout<<"Error: This operation is only MC."<<endl;
    return 0;
  }
 
  if(!OpenReducedFile())return 0;

  //ps file
  filename=_OutputDir+"/EffAfter2DEffCorr.ps";
  Canvas.Print(filename+"[");

  //create a Tfile where the efficiency parameters will be saved.
  TFile LegendreWeightedHistos(_OutputDir+"/LegendreWeightedHistos.root","read");    
  gROOT->cd();


  //make the pstar ratio
  HMCXp3CM=(TH1F*)ReducedFile->Get("HMCXp3CM");
  if(!HMCXp3CM)return 0;  
  TH1F*HPstarCorr=(TH1F*)LegendreWeightedHistos.Get("HPstarNoShape");
  if(!HPstarCorr)return 0;
  Canvas.Clear();
  HMCXp3CM->GetYaxis()->SetRangeUser(0,1.1*HMCXp3CM->GetMaximum());
  HMCXp3CM->Draw("pe");
  HPstarCorr->Draw("pesame");
  Canvas.Print(filename);
  HPstarCorr->GetYaxis()->SetTitle("Efficiency");
  HPstarCorr->GetXaxis()->SetTitle("p*   (GeV/c^2)");
  delete Plot1DEfficiency(HMCXp3CM,HPstarCorr,1);
  


  //---------------------------------------------------------------
  //make the mass ratio
  //--------------------------------------------------------------
  HMCXMass=(TH1F*)ReducedFile->Get("HMCXMass");
  if(!HMCXMass)return 0;  
  HMCXMass->Sumw2();
  HMCXMass=(TH1F*)CreateRebinnedDstPiHisto(HMCXMass,40,2.1,3.1);
  if(!HMCXMass)return 0;  
  TH1F*HDMCorr=(TH1F*)LegendreWeightedHistos.Get("HDstPiMNoShape");
  if(!HDMCorr)return 0;
  HDMCorr->Sumw2();
  HDMCorr=(TH1F*)CreateRebinnedDstPiHisto(HDMCorr,40,2.1,3.1);
  if(!HDMCorr)return 0;
  HDMCorr->GetYaxis()->SetTitle("Efficiency");
  HDMCorr->GetXaxis()->SetTitle("m(D*#pi) (GeV/c^2)");
  delete Plot1DEfficiency(HMCXMass,HDMCorr,1);
  
  
  // bin by bin in p* ratio
  H2MCPvsMass=(TH2F*)ReducedFile->Get("H2MCPvsMass");
  if(!H2MCPvsMass)return 0;
  TH2F* H2PvsM=(TH2F*)LegendreWeightedHistos.Get("H2PvsM");
  if(!H2PvsM)return 0;
  TH1F htcorr("htcorr","htcorr",H2MCPvsMass->GetYaxis()->GetNbins(),
	      H2MCPvsMass->GetYaxis()->GetXmin(),
	      H2MCPvsMass->GetYaxis()->GetXmax());
  htcorr.Sumw2();
  TH1F ht("ht","ht",H2MCPvsMass->GetYaxis()->GetNbins(),
	  H2MCPvsMass->GetYaxis()->GetXmin(),
	  H2MCPvsMass->GetYaxis()->GetXmax());

 
  TF1 fitf("fitf","[0]+[1]*x",2.1,3.1);
  fitf.SetParameters(1,.01);
  TH1*htcorraddcl[4];
  Int_t cid=1;
  Canvas.Clear();
  Canvas.Divide(2,2); 
  for(Int_t pb=1;pb<=4;pb++){      
    Canvas.cd(cid);
    
    for(Int_t y=1;y<=ht.GetNbinsX();y++){
      htcorr.SetBinContent(y,H2PvsM->GetBinContent(pb,y));	                       
      htcorr.SetBinError(y,H2PvsM->GetBinError(pb,y));
      ht.SetBinContent(y,H2MCPvsMass->GetBinContent(pb,y));
      ht.SetBinError(y,H2MCPvsMass->GetBinError(pb,y));	
    }
    htcorraddcl[cid-1]=(TH1*)htcorr.Clone();
    htcorraddcl[cid-1]->SetName(TString(htcorr.GetName())+(long)cid);
    htcorraddcl[cid-1]->Divide(&ht);
    htcorraddcl[cid-1]->Fit(&fitf);      
    sprintf(textlabel,"slope=%.2f+-%.2f, r=%.3f",fitf.GetParameter(1),fitf.GetParError(1),htcorr.Integral()/ht.Integral());
    htcorraddcl[cid-1]->SetTitle(textlabel);
    htcorraddcl[cid-1]->GetYaxis()->SetRangeUser(0,htcorraddcl[cid-1]->GetMaximum()*1.2);
    htcorraddcl[cid-1]->Draw("pe");
    cid++;
  }    
  
  Canvas.Print(filename);
//   for(Int_t pb=1;pb<=4;pb++){
//     delete htcorraddcl[cid-1];      
//     cid++;
//   }

  //compare y projection of 3-D rec mass to 3D projection of gen mass
  TH1*hgenmass=H2MCPvsMass->ProjectionY("hgenmass",1,H2MCPvsMass->GetNbinsX(),"");  
  TH1*hrecmass=H2PvsM->ProjectionY("hrecmass",1,H2MCPvsMass->GetNbinsX(),""); 
  hrecmass->SetTitle("Cut rec / Cut gen");
  hrecmass->GetYaxis()->SetTitle("Efficiency");
  hrecmass->GetXaxis()->SetTitle("m(D*#pi) (GeV/c^2)");
  delete Plot1DEfficiency(hgenmass,hrecmass,1);

  //
  hrecmass->SetTitle("Cut rec / total gen");  
  Canvas.Clear();
  HMCXMass->Draw("pe");
  hrecmass->Draw("pesame");
  Canvas.Print(filename);
  hrecmass->GetYaxis()->SetTitle("Efficiency");
  hrecmass->GetXaxis()->SetTitle("m(D*#pi) (GeV/c^2)");
  delete Plot1DEfficiency(HMCXMass,hrecmass,1);

  //compare the y projection of the Cut gen mass to the total gen  mass
  hgenmass->SetTitle("Cut gen / total gen");  
  Canvas.Clear();
  HMCXMass->Draw("pe");
  hgenmass->Draw("pesame");
  Canvas.Print(filename);
  hgenmass->GetYaxis()->SetTitle("Efficiency");
  hgenmass->GetXaxis()->SetTitle("m(D*#pi) (GeV/c^2)");
  delete Plot1DEfficiency(HMCXMass,hgenmass,1);
  
  //get correction factors for the p* cuts: H2MCPvsMass has binnig=(4,3,5,40,2.1,3.1);
  TH1*hgenmassp[4];
  for(Int_t i=0;i<4;i++){
    hgenmassp[i]=H2MCPvsMass->ProjectionY(TString("hgenmassp")+(long)i,i+1,i+1,"");
    delete Plot1DEfficiency(HMCXMass,hgenmassp[i],1);
  }

  //fully corrected
  HMCXMass=(TH1F*)ReducedFile->Get("HMCXMass");
  if(!HMCXMass)return 0;  
  //HMCXMass->Sumw2();
  HMCXMass=(TH1F*)CreateRebinnedDstPiHisto(HMCXMass,300,2.2,2.8);
  if(!HMCXMass)return 0;  
  TH1F*HDstPiMFine=(TH1F*)LegendreWeightedHistos.Get("HDstPiMFineNoRes");
  if(!HDstPiMFine)return 0;
  //HDstPiMFine->Sumw2();
  HDstPiMFine=(TH1F*)CreateRebinnedDstPiHisto(HDstPiMFine,300,2.2,2.8);
  if(!HDstPiMFine)return 0;
  HDstPiMFine->SetTitle("Fully Corr/ Total Gen");
  HDstPiMFine->GetYaxis()->SetTitle("Efficiency");
  HDstPiMFine->GetXaxis()->SetTitle("m(D*#pi) (GeV/c^2)");
  delete Plot1DEfficiency(HMCXMass,HDstPiMFine,1);

  //-------------------------------------------------------
  //make the helicity ratio
  //-----------------------------------------------------
  HMCDstarHelicity=(TH1F*)ReducedFile->Get("HMCDstarHelicity");
  if(!HMCDstarHelicity)return 0;  
  TH1F*HHelCorr=(TH1F*)LegendreWeightedHistos.Get("HDstarHelicityNoShape");
  if(!HHelCorr)return 0;
  Canvas.Clear();
  HMCDstarHelicity->GetYaxis()->SetRangeUser(0,1.1*HMCDstarHelicity->GetMaximum());
  HMCDstarHelicity->Draw("pe"); HHelCorr->Draw("pesame");
  Canvas.Print(filename);
  HHelCorr->GetYaxis()->SetTitle("Efficiency");
  HHelCorr->GetXaxis()->SetTitle("cos(#theta_{H})");
  delete Plot1DEfficiency(HMCDstarHelicity,HHelCorr,1);

  //fully corrected
  TH1F*HDstarHelicity=(TH1F*)LegendreWeightedHistos.Get("HDstarHelicity");
  if(!HDstarHelicity)return 0;
  Canvas.Clear();
  HMCDstarHelicity->GetYaxis()->SetRangeUser(0,1.1*HMCDstarHelicity->GetMaximum());
  HMCDstarHelicity->Draw("pe");
  HDstarHelicity->Draw("pesame");
  Canvas.Print(filename);
  HDstarHelicity->GetYaxis()->SetTitle("Efficiency");
  HDstarHelicity->GetXaxis()->SetTitle("cos(#theta_{H})");
  delete Plot1DEfficiency(HMCDstarHelicity,HDstarHelicity,1); 

  //-----------------------------------------
  //make the HMCDstarAngle ratio
  //-----------------------------------------
  HMCDstarAngle=(TH1F*)ReducedFile->Get("HMCDstarAngle");
  if(!HMCDstarAngle)return 0;  
  TH1F*HDstarAngleNoShape=(TH1F*)LegendreWeightedHistos.Get("HDstarAngleNoShape");
  if(!HDstarAngleNoShape)return 0;
  Canvas.Clear();
  HMCDstarAngle->GetYaxis()->SetRangeUser(0,1.1*HMCDstarAngle->GetMaximum());
  HMCDstarAngle->Draw("pe");
  HDstarAngleNoShape->Draw("pesame");
  Canvas.Print(filename);
  HDstarAngleNoShape->GetYaxis()->SetTitle("Efficiency");
  HDstarAngleNoShape->GetXaxis()->SetTitle("cos(#theta_{D*})");
  delete Plot1DEfficiency(HMCDstarAngle,HDstarAngleNoShape,1);  
  
  //fully corrected
  TH1F*HDstarAngle=(TH1F*)LegendreWeightedHistos.Get("HDstarAngle");
  if(!HDstarAngle)return 0;
  Canvas.Clear();
  HMCDstarAngle->GetYaxis()->SetRangeUser(0,1.1*HMCDstarAngle->GetMaximum());
  HMCDstarAngle->Draw("pe");
  HDstarAngle->Draw("pesame");
  Canvas.Print(filename);
  HDstarAngle->GetYaxis()->SetTitle("Efficiency");
  HDstarAngle->GetXaxis()->SetTitle("cos(#theta_{D*})");
  delete Plot1DEfficiency(HMCDstarAngle,HDstarAngle,1);  

  Canvas.Print(filename+"]");

  LegendreWeightedHistos.Close();
  if(!CloseReducedFile())return 0;
  
  return 1;

}
/*
////----------Old trials------------------------------------------------------------------------

Int_t DstPiAnalysis::Draw3DEfficiency(){
  cout<<"Computing 3-D Efficiency"<<endl;
    
  H3MCPvsThetaVsDstAngle=(TH3F*)ReducedFile->Get("H3MCPvsThetaVsDstAngle");
  if(!H3MCPvsThetaVsDstAngle){
    cout<<"No histogram found"<<endl;
    return 0;
  }
  
  
  TH3F H3PvsThetaVsDstAngle("H3PvsThetaVsDstAngle","H3PvsThetaVsDstAngle",
			    H3MCPvsThetaVsDstAngle->GetXaxis()->GetNbins(),
			    H3MCPvsThetaVsDstAngle->GetXaxis()->GetXmin(),
			    H3MCPvsThetaVsDstAngle->GetXaxis()->GetXmax(),
			    H3MCPvsThetaVsDstAngle->GetYaxis()->GetNbins(),
			    H3MCPvsThetaVsDstAngle->GetYaxis()->GetXmin(),
			    H3MCPvsThetaVsDstAngle->GetYaxis()->GetXmax(),
			    H3MCPvsThetaVsDstAngle->GetZaxis()->GetNbins(),
			    H3MCPvsThetaVsDstAngle->GetZaxis()->GetXmin(),
			    H3MCPvsThetaVsDstAngle->GetZaxis()->GetXmax());  
  ReducedNtuple->Draw("dstarcostheta:dstpicosstar:dstpipstar>>H3PvsThetaVsDstAngle");


  //calculate the efficiency
  TH3*H3Eff=(TH3*)H3PvsThetaVsDstAngle.Clone();
  H3Eff->Sumw2();
  H3Eff->Divide(H3MCPvsThetaVsDstAngle);
  H3Eff->SetTitle("H3DEfficiency");

  //----------------
  //Now look at the dstarcostheta slices
  ///------------------------
  Canvas.Clear();
  Canvas.Divide(2,2);
  TH2F* H2Eff[4];
  for(Int_t z=1;z<=H3Eff->GetNbinsZ();z++){
    Canvas.cd(z);
    H2Eff[z-1]=new TH2F(TString("H2Eff")+(long)z,TString("H2Eff")+(long)z,
			H3MCPvsThetaVsDstAngle->GetXaxis()->GetNbins(),
			H3MCPvsThetaVsDstAngle->GetXaxis()->GetXmin(),
			H3MCPvsThetaVsDstAngle->GetXaxis()->GetXmax(),
			H3MCPvsThetaVsDstAngle->GetYaxis()->GetNbins(),
			H3MCPvsThetaVsDstAngle->GetYaxis()->GetXmin(),
			H3MCPvsThetaVsDstAngle->GetYaxis()->GetXmax());
    
    for(Int_t x=1;x<=H3Eff->GetNbinsX();x++){
      for(Int_t y=1;y<=H3Eff->GetNbinsY();y++){
	H2Eff[z-1]->SetBinContent(x,y,H3Eff->GetBinContent(x,y,z));    
	H2Eff[z-1]->SetBinError(x,y,H3Eff->GetBinError(x,y,z));      
      }
    }        
    H2Eff[z-1]->SetStats(0);
    H2Eff[z-1]->Draw("colz");    
  }     
  Canvas.Print(filename);


  //plot the relative error in the efficiency
  Canvas.Clear();
  Canvas.Divide(2,2);
  TH2F* H2EffErr[4];
  for(Int_t z=1;z<=H3Eff->GetNbinsZ();z++){
    Canvas.cd(z);
    H2EffErr[z-1]=(TH2F*)H2Eff[z-1]->Clone();    
 
    Fill2DRelativeEffErr(H2Eff[z-1],H2EffErr[z-1]);

    H2EffErr[z-1]->SetTitle(TString(H2Eff[z-1]->GetTitle())+"Err");    
    H2EffErr[z-1]->SetStats(0);    
    H2EffErr[z-1]->Draw("colz");    
  } 
  Canvas.Print(filename);

  for(Int_t z=1;z<=H3Eff->GetNbinsZ();z++){
    delete H2EffErr[z-1];
  }


  //fix the last bin in the eff
  Canvas.Clear();
  Canvas.Divide(2,2);
  for(Int_t z=1;z<=H3Eff->GetNbinsZ();z++){
    Canvas.cd(z);
    FixPstarEff(H2Eff[z-1]);
    H2Eff[z-1]->SetStats(0);    
    H2Eff[z-1]->Draw("colz");    
  }     
  Canvas.Print(filename);
  Canvas.Clear();
  Canvas.Divide(2,2);  
  for(Int_t z=1;z<=H3Eff->GetNbinsZ();z++){
    Canvas.cd(z);
    H2EffErr[z-1]=(TH2F*)H2Eff[z-1]->Clone();    
    Fill2DRelativeEffErr(H2Eff[z-1],H2EffErr[z-1]);
     
    H2EffErr[z-1]->SetStats(0);
    H2EffErr[z-1]->Draw("colz");    
  } 
  Canvas.Print(filename);
  //clean
  for(Int_t z=1;z<=H3Eff->GetNbinsZ();z++){
    //delete H2Eff[z-1];
    delete H2EffErr[z-1];
  }


  //make interpolation in p*
  Canvas.Clear();
  Canvas.Divide(2,2);  
  TH2F* H2EffInterX[4];
  for(Int_t z=1;z<=H3Eff->GetNbinsZ();z++){
    Canvas.cd(z);
    H2EffInterX[z-1]=Interpolate2DEfficiencyX(H2Eff[z-1]);
    H2EffInterX[z-1]->SetStats(0);
    H2EffInterX[z-1]->Draw("colz"); 
  }
  Canvas.Print(filename); 
  Canvas.Clear();
  Canvas.Divide(2,2);  
  for(Int_t z=1;z<=H3Eff->GetNbinsZ();z++){
    Canvas.cd(z); 
    H2EffErr[z-1]=(TH2F*)H2EffInterX[z-1]->Clone();   			     
    Fill2DRelativeEffErr(H2EffInterX[z-1],H2EffErr[z-1]);
    
    H2EffErr[z-1]->SetStats(0);
    H2EffErr[z-1]->Draw("colz");    
  } 
  Canvas.Print(filename); 



  //make interpolation in cos*
  Canvas.Clear();
  Canvas.Divide(2,2);  
  TH2F* H2EffInterY[4];
  for(Int_t z=1;z<=H3Eff->GetNbinsZ();z++){
    Canvas.cd(z);
    H2EffInterY[z-1]=Interpolate2DEfficiencyY(H2EffInterX[z-1]);
    
    //fix the bins below p*=3GeV because they acquired an eff above
    for(Int_t x=1;x<=H2EffInterY[z-1]->GetNbinsX();x++)
      for(Int_t y=1;y<=H2EffInterY[z-1]->GetNbinsY();y++)
	if(H2EffInterY[z-1]->GetXaxis()->GetBinCenter(x)<3.){	
	  H2EffInterY[z-1]->SetBinContent(x,y,0);
	  H2EffInterY[z-1]->SetBinError(x,y,0);
	}
    
    H2EffInterY[z-1]->SetStats(0);
    H2EffInterY[z-1]->Draw("colz"); 
  }
  Canvas.Print(filename); 
  Canvas.Clear();
  Canvas.Divide(2,2);  
  for(Int_t z=1;z<=H3Eff->GetNbinsZ();z++){
    Canvas.cd(z); 
    H2EffErr[z-1]=(TH2F*)H2EffInterY[z-1]->Clone();   		
    Fill2DRelativeEffErr(H2EffInterY[z-1],H2EffErr[z-1]);
	         
    H2EffErr[z-1]->SetStats(0);
    H2EffErr[z-1]->Draw("colz");    
  } 
  Canvas.Print(filename); 


  ///create the 3D interpolation and show one x-y bin 
  TH3F* H3DInterpolation=Interpolate2DEfficiencyZ(H2EffInterY);
  TH1D* hzproj=H3DInterpolation->ProjectionZ();
  for(Int_t z=1;z<=H3DInterpolation->GetNbinsZ();z++){
    hzproj->SetBinContent(z,H3DInterpolation->GetBinContent(75,20,z));
    hzproj->SetBinError(z,H3DInterpolation->GetBinError(75,20,z));
  }
  Canvas.Clear();
  hzproj->GetYaxis()->SetRangeUser(0,.5);
  hzproj->SetStats(0);
  hzproj->Draw();
  Canvas.Print(filename); 
  
  H3DInterpolation->SetName("H3Efficiency");
  H3DInterpolation->Write();


  delete H3DInterpolation;
  delete hzproj;
    
  //clean
  for(Int_t z=1;z<=H3Eff->GetNbinsZ();z++){
    delete H2Eff[z-1];
    delete H2EffErr[z-1];
    delete H2EffInterX[z-1];
    delete H2EffInterY[z-1];
  }

  return 1; 
}




Int_t DstPiAnalysis::EffAfterEffCorr(){
  cout<<"Going to check the efficiency  shape in mass and dstarhelicity after the 3-D efficiency correction  "<<endl;
  if(!_TruthMatch){
    cout<<"Error: This operation is only MC."<<endl;
    return 0;
  }
 
  if(!OpenReducedFile())return 0;

  //ps file
  filename=_OutputDir+"/EffAfter3DEffCorr.ps";
  Canvas.Print(filename+"[");

  //create a Tfile where the efficiency parameters will be saved.
  TFile LegendreWeightedHistos(_OutputDir+"/LegendreWeightedHistos.root","read");    
  gROOT->cd();


  //make the pstar ratio
  HMCXp3CM=(TH1F*)ReducedFile->Get("HMCXp3CM");
  if(!HMCXp3CM)return 0;  
  TH1F*HPstarCorr=(TH1F*)LegendreWeightedHistos.Get("HPstarNoShape");
  if(!HPstarCorr)return 0;

  Canvas.Clear();
  HMCXp3CM->GetYaxis()->SetRangeUser(0,1.1*HMCXp3CM->GetMaximum());
  HMCXp3CM->Draw("pe");
  HPstarCorr->Draw("pesame");
  Canvas.Print(filename);

  TH1*h=Plot1DEfficiency(HMCXp3CM,HPstarCorr,1);
  delete h;
  //---------------------------------------------------------------

  //make the mass ratio
  HMCXMass=(TH1F*)ReducedFile->Get("HMCXMass");
  if(!HMCXMass)return 0;  
  HMCXMass->Sumw2();
  HMCXMass=(TH1F*)CreateRebinnedDstPiHisto(HMCXMass,40,2.1,3.1);
  TH1F*HDMCorr=(TH1F*)LegendreWeightedHistos.Get("HDstPiMNoShape");
  if(!HDMCorr)return 0;
  HDMCorr->Sumw2();
  HDMCorr=(TH1F*)CreateRebinnedDstPiHisto(HDMCorr,40,2.1,3.1);
  h=Plot1DEfficiency(HMCXMass,HDMCorr,1);
  delete h;
  
  
  //
  H3MCPvsMassVsDstAngle=(TH3F*)ReducedFile->Get("H3MCPvsMassVsDstAngle");
  TH3F* H3PvsMVsDstA=(TH3F*)LegendreWeightedHistos.Get("H3PvsMVsDstA");
  TH1F htcorr("htcorr","htcorr",H3MCPvsMassVsDstAngle->GetYaxis()->GetNbins(),
	      H3MCPvsMassVsDstAngle->GetYaxis()->GetXmin(),
	      H3MCPvsMassVsDstAngle->GetYaxis()->GetXmax());
  htcorr.Sumw2();
  TH1F ht("ht","ht",H3MCPvsMassVsDstAngle->GetYaxis()->GetNbins(),
	  H3MCPvsMassVsDstAngle->GetYaxis()->GetXmin(),
	  H3MCPvsMassVsDstAngle->GetYaxis()->GetXmax());

 
  TF1 fitf("fitf","[0]+[1]*x",2.1,3.1);
  fitf.SetParameters(1,.01);
  TH1*htcorraddcl[12];
  //Float_t totalN=H3PvsMVsDstA->Integral();
  Int_t cid=1;
  Canvas.Clear();
  Canvas.Divide(4,3); 
  for(Int_t pc=1;pc<=3;pc++){
    for(Int_t pb=1;pb<=4;pb++){      
      Canvas.cd(cid);

      for(Int_t y=1;y<=ht.GetNbinsX();y++){
	htcorr.SetBinContent(y,H3PvsMVsDstA->GetBinContent(pb,y,pc));	                       
	htcorr.SetBinError(y,H3PvsMVsDstA->GetBinError(pb,y,pc));
	ht.SetBinContent(y,H3MCPvsMassVsDstAngle->GetBinContent(pb,y,pc));
	ht.SetBinError(y,H3MCPvsMassVsDstAngle->GetBinError(pb,y,pc));	
      }
      htcorraddcl[cid-1]=(TH1*)htcorr.Clone();
      htcorraddcl[cid-1]->SetName(TString(htcorr.GetName())+(long)cid);
      htcorraddcl[cid-1]->Divide(&ht);
      htcorraddcl[cid-1]->Fit(&fitf);      
      sprintf(textlabel,"slope=%.2f+-%.2f, r=%.3f",fitf.GetParameter(1),fitf.GetParError(1),htcorr.Integral()/ht.Integral());
      htcorraddcl[cid-1]->SetTitle(textlabel);
      htcorraddcl[cid-1]->GetYaxis()->SetRangeUser(0,htcorraddcl[cid-1]->GetMaximum()*1.2);
      htcorraddcl[cid-1]->Draw("pe");
      cid++;
    }    
  }
  Canvas.Print(filename);
 //  for(Int_t pc=1;pc<=3;pc++)
//     for(Int_t pb=1;pb<=4;pb++){
//       delete htcorraddcl[cid-1];      
//       cid++;
//     }

  //compare y projection of 3-D rec mass to 3D projection of gen mass
  TH1*hgenmass=H3MCPvsMassVsDstAngle->Project3D("y");  
  TH1*hrecmass=H3PvsMVsDstA->Project3D("y"); 
  hrecmass->SetTitle("3-D rec / 3D gen");
  delete Plot1DEfficiency(hgenmass,hrecmass,1);

  //
  hrecmass->SetTitle("3-D rec / total gen");  
  delete Plot1DEfficiency(HMCXMass,hrecmass,1);

  //compare the y projection of the 3-D gen mass to the total gen  mass
  hgenmass->SetTitle("3-D gen / total gen");  
  delete Plot1DEfficiency(HMCXMass,hgenmass,1);

  //
  TH1*hgenpstar=H3MCPvsMassVsDstAngle->Project3D("x");  
  TH1*hrecpstar=H3PvsMVsDstA->Project3D("x"); 
  hrecpstar->SetTitle("3-D rec / 3D gen p*");
  delete Plot1DEfficiency(hgenpstar,hrecpstar,1);

  //
  TH1*hgencos=H3MCPvsMassVsDstAngle->Project3D("z");  
  TH1*hreccos=H3PvsMVsDstA->Project3D("z"); 
  hreccos->SetTitle("3-D rec / 3D gen cosD*");
  delete Plot1DEfficiency(hgencos,hreccos,1);



  //check mass distribution for different cosD*
  TH1F hmcoslow("hmcoslow","hmcoslow",H3MCPvsMassVsDstAngle->GetYaxis()->GetNbins(),
		H3MCPvsMassVsDstAngle->GetYaxis()->GetXmin(),
		H3MCPvsMassVsDstAngle->GetYaxis()->GetXmax());
  for(Int_t y=1;y<=H3MCPvsMassVsDstAngle->GetNbinsY();y++){
    for(Int_t x=1;x<=H3MCPvsMassVsDstAngle->GetNbinsX();x++){
      hmcoslow.AddBinContent(y,H3MCPvsMassVsDstAngle->GetBinContent(x,y,1));	
      hmcoslow.AddBinContent(y,H3MCPvsMassVsDstAngle->GetBinContent(x,y,2));			     
    }
  }
  TH1F hmcoshigh("hmcoshigh","hmcoshigh",H3MCPvsMassVsDstAngle->GetYaxis()->GetNbins(),
		H3MCPvsMassVsDstAngle->GetYaxis()->GetXmin(),
		H3MCPvsMassVsDstAngle->GetYaxis()->GetXmax());
  for(Int_t y=1;y<=H3MCPvsMassVsDstAngle->GetNbinsY();y++){
    for(Int_t x=1;x<=H3MCPvsMassVsDstAngle->GetNbinsX();x++){
      hmcoshigh.AddBinContent(y,H3MCPvsMassVsDstAngle->GetBinContent(x,y,3));	
      hmcoshigh.AddBinContent(y,H3MCPvsMassVsDstAngle->GetBinContent(x,y,4));			     
    }
  }
  hmcoshigh.SetTitle("high cosD* / low cosD*");
  delete Plot1DEfficiency(&hmcoslow,&hmcoshigh,1); 

  ///////////////--------------------------


  //make the HMCDstarAngle ratio
  HMCDstarAngle=(TH1F*)ReducedFile->Get("HMCDstarAngle");
  if(!HMCDstarAngle)return 0;  
  TH1F*HDstarAngleCorr=(TH1F*)LegendreWeightedHistos.Get("HDstarAngleNoShape");
  if(!HDstarAngleCorr)return 0;

  Canvas.Clear();
  HMCDstarAngle->GetYaxis()->SetRangeUser(0,1.1*HMCDstarAngle->GetMaximum());
  HMCDstarAngle->Draw("pe");
  HDstarAngleCorr->Draw("pesame");
  Canvas.Print(filename);

  h=Plot1DEfficiency(HMCDstarAngle,HDstarAngleCorr,1);
  delete h;  
  

  //make the helicity ratio
  HMCDstarHelicity=(TH1F*)ReducedFile->Get("HMCDstarHelicity");
  if(!HMCDstarHelicity)return 0;  
  TH1F*HHelCorr=(TH1F*)LegendreWeightedHistos.Get("HDstarHelicityNoShape");
  if(!HHelCorr)return 0;
  Canvas.Clear();
  HMCDstarHelicity->GetYaxis()->SetRangeUser(0,1.1*HMCDstarHelicity->GetMaximum());
  HMCDstarHelicity->Draw("pe");
  HHelCorr->Draw("pesame");
  Canvas.Print(filename);

  h=Plot1DEfficiency(HMCDstarHelicity,HHelCorr,1);
  delete h;


  //
  HMCXcosthCM=(TH1F*)ReducedFile->Get("HMCXcosthCM");
  if(!HMCXcosthCM)return 0;  
  TH1F*HCosstarCorr=(TH1F*)LegendreWeightedHistos.Get("HDstPiCosstarNoShape");
  if(!HCosstarCorr)return 0;

  Canvas.Clear();
  HMCXcosthCM->GetYaxis()->SetRangeUser(0,1.1*HMCXcosthCM->GetMaximum());
  HMCXcosthCM->Draw("pe");
  HCosstarCorr->Draw("pesame");
  Canvas.Print(filename);

  //h=Plot1DEfficiency(HMCXcosthCM,HCosstarCorr,1);
  //delete h;



  Canvas.Print(filename+"]");

  LegendreWeightedHistos.Close();
  if(!CloseReducedFile())return 0;
  
  return 1;

}



Int_t DstPiAnalysis::Draw3DEfficiencyFit(){
  cout<<"Computing 3-D Efficiency pstar vs mass vs dstarangle"<<endl;
    
  H3MCPvsMassVsDstAngle=(TH3F*)ReducedFile->Get("H3MCPvsMassVsDstAngle");
  if(!H3MCPvsMassVsDstAngle){
    cout<<"No histogram found"<<endl;
    return 0;
  }


  TH3F H3PvsMVsDstA("H3PvsMVsDstA","H3PvsMVsDstA",
			      H3MCPvsMassVsDstAngle->GetXaxis()->GetNbins(),
			      H3MCPvsMassVsDstAngle->GetXaxis()->GetXmin(),
			      H3MCPvsMassVsDstAngle->GetXaxis()->GetXmax(),
			      H3MCPvsMassVsDstAngle->GetYaxis()->GetNbins(),
			      H3MCPvsMassVsDstAngle->GetYaxis()->GetXmin(),
			      H3MCPvsMassVsDstAngle->GetYaxis()->GetXmax(),
			      H3MCPvsMassVsDstAngle->GetZaxis()->GetNbins(),
			      H3MCPvsMassVsDstAngle->GetZaxis()->GetXmin(),
			      H3MCPvsMassVsDstAngle->GetZaxis()->GetXmax());  
  ReducedNtuple->Draw("dstarcostheta:(dstpimdm-dstpidmres):dstpipstar>>H3PvsMVsDstA");


  //Show the efficiency shapes
  TH1F htcorr("htcorr","htcorr",H3MCPvsMassVsDstAngle->GetYaxis()->GetNbins(),
	      H3MCPvsMassVsDstAngle->GetYaxis()->GetXmin(),
	      H3MCPvsMassVsDstAngle->GetYaxis()->GetXmax());
  htcorr.Sumw2();
  TH1F ht("ht","ht",H3MCPvsMassVsDstAngle->GetYaxis()->GetNbins(),
	  H3MCPvsMassVsDstAngle->GetYaxis()->GetXmin(),
	  H3MCPvsMassVsDstAngle->GetYaxis()->GetXmax());

  TF1 fitf("fitf","[0]+[1]*x",2.1,3.1);
  fitf.SetParameters(1,.01);
  TH1*htcorraddcl[12];  
  //Float_t totalN=H3PvsMVsDstA.Integral();
  Int_t cid=1;
  Canvas.Clear();
  Canvas.Divide(4,3); 
  for(Int_t pc=1;pc<=3;pc++){
    for(Int_t pb=1;pb<=4;pb++){      
      Canvas.cd(cid);

      for(Int_t y=1;y<=ht.GetNbinsX();y++){
	htcorr.SetBinContent(y,H3PvsMVsDstA.GetBinContent(pb,y,pc));	                       
	htcorr.SetBinError(y,H3PvsMVsDstA.GetBinError(pb,y,pc));
	ht.SetBinContent(y,H3MCPvsMassVsDstAngle->GetBinContent(pb,y,pc));
	ht.SetBinError(y,H3MCPvsMassVsDstAngle->GetBinError(pb,y,pc));	
      }
      htcorraddcl[cid-1]=(TH1*)htcorr.Clone();
      htcorraddcl[cid-1]->SetName(TString(htcorr.GetName())+(long)cid);
      htcorraddcl[cid-1]->Divide(&ht);
      htcorraddcl[cid-1]->Fit(&fitf);      
      sprintf(textlabel,"slope=%.2f+-%.2f, r=%.3f",fitf.GetParameter(1),fitf.GetParError(1),htcorr.Integral()/ht.Integral());
      htcorraddcl[cid-1]->SetTitle(textlabel);
      htcorraddcl[cid-1]->GetYaxis()->SetRangeUser(0,htcorraddcl[cid-1]->GetMaximum()*1.2);
      htcorraddcl[cid-1]->Draw("pe");
      cid++;
    }    
  }
  Canvas.Print(filename);
//   cid=1;
//   for(Int_t pc=1;pc<=3;pc++)
//     for(Int_t pb=1;pb<=4;pb++){
//       delete htcorraddcl[cid-1];      
//       cid++;
//     }

  //compare y projection of 3-D rec mass to 3D projection of gen mass
  TH1*hgenmass=H3MCPvsMassVsDstAngle->Project3D("y");  
  TH1*hrecmass=H3PvsMVsDstA.Project3D("y"); 
  hrecmass->SetTitle("3-D rec / 3D gen");
  delete Plot1DEfficiency(hgenmass,hrecmass,1);

  //
  TH1*hgenpstar=H3MCPvsMassVsDstAngle->Project3D("x");  
  TH1*hrecpstar=H3PvsMVsDstA.Project3D("x"); 
  hrecpstar->SetTitle("3-D rec / 3D gen");
  delete Plot1DEfficiency(hgenpstar,hrecpstar,1);

  //
  TH1*hgencos=H3MCPvsMassVsDstAngle->Project3D("z");  
  TH1*hreccos=H3PvsMVsDstA.Project3D("z"); 
  hreccos->SetTitle("3-D rec / 3D gen");
  delete Plot1DEfficiency(hgencos,hreccos,1);
  //---------------------------------------



  Float_t maxefferr=.1;  
  //calculate the efficiency
  TH3*H3Eff=(TH3*)H3PvsMVsDstA.Clone();
  H3Eff->Sumw2();
  H3Eff->Divide(H3MCPvsMassVsDstAngle);
  H3Eff->SetTitle("H3DEfficiency2"); 
  //clean out the large errors
  for(Int_t i=1;i<=H3Eff->GetXaxis()->GetNbins();i++)
    for(Int_t j=1;j<=H3Eff->GetYaxis()->GetNbins();j++)
      for(Int_t k=1;k<=H3Eff->GetZaxis()->GetNbins();k++)
	if(H3Eff->GetBinError(i,j,k)/H3Eff->GetBinContent(i,j,k)>maxefferr){
	  H3Eff->SetBinError(i,j,k,0);
	  H3Eff->SetBinContent(i,j,k,0);
	}
 



  //----------------
  //Now look at the dstarcostheta  2-D slices
  ///------------------------
  Canvas.Clear();
  Canvas.Divide(2,2);
  TH2F* H2Eff[4];
  for(Int_t z=0;z<H3Eff->GetZaxis()->GetNbins();z++){
    Canvas.cd(z+1);
    H2Eff[z]=new TH2F(TString("H2Eff")+(long)z,TString("H2Eff")+(long)z,
			H3MCPvsMassVsDstAngle->GetXaxis()->GetNbins(),
			H3MCPvsMassVsDstAngle->GetXaxis()->GetXmin(),H3MCPvsMassVsDstAngle->GetXaxis()->GetXmax(),
			H3MCPvsMassVsDstAngle->GetYaxis()->GetNbins(),
			H3MCPvsMassVsDstAngle->GetYaxis()->GetXmin(),H3MCPvsMassVsDstAngle->GetYaxis()->GetXmax());
    
    for(Int_t x=1;x<=H3Eff->GetNbinsX();x++){
      for(Int_t y=1;y<=H3Eff->GetNbinsY();y++){
	H2Eff[z]->SetBinContent(x,y,H3Eff->GetBinContent(x,y,z+1));    
	H2Eff[z]->SetBinError(x,y,H3Eff->GetBinError(x,y,z+1));      
      }
    }        
    H2Eff[z]->SetStats(0);
    //H2Eff[z]->GetZaxis()->SetRangeUser(.1,.3);
    H2Eff[z]->Draw("colz");    
  }     
  Canvas.Print(filename);


  //plot the relative error in the efficiency
  Canvas.Clear();
  Canvas.Divide(2,2);
  TH2F* H2EffErr[4];
  for(Int_t z=0;z<H3Eff->GetZaxis()->GetNbins();z++){
    Canvas.cd(z+1);
    H2EffErr[z]=(TH2F*)H2Eff[z]->Clone();    

    Fill2DRelativeEffErr(H2Eff[z],H2EffErr[z]);

    H2EffErr[z]->SetStats(0);
    //H2EffErr[z]->GetZaxis()->SetRangeUser(.0,maxefferr+.01);    
    H2EffErr[z]->SetTitle(TString(H2Eff[z]->GetTitle())+"Err");    
    H2EffErr[z]->Draw("colz");    
  } 
  Canvas.Print(filename);

  for(Int_t z=0;z<H3Eff->GetZaxis()->GetNbins();z++){
    delete H2EffErr[z];
  }

 
    
  //--------------------------------
  //make interpolation in p*
  //--------------------------------
  Canvas.Clear();
  Canvas.Divide(2,2);  
  TH2F* H2EffInterX[4];
  TH1F HEffChiX[4];
  for(Int_t z=0;z<H3Eff->GetZaxis()->GetNbins();z++){
    Canvas.cd(z+1);
    H2EffInterX[z]=Interpolate2DEfficiencyFitX(H2Eff[z],&HEffChiX[z]);
    H2EffInterX[z]->SetStats(0);
    //H2EffInterX[z]->GetZaxis()->SetRangeUser(.0,.4);
    H2EffInterX[z]->Draw("colz"); 
  }
  Canvas.Print(filename); 
  Canvas.Clear();
  Canvas.Divide(2,2);  
  for(Int_t z=0;z<H3Eff->GetZaxis()->GetNbins();z++){
    Canvas.cd(z+1); 
    H2EffErr[z]=(TH2F*)H2EffInterX[z]->Clone();   			     
    Fill2DRelativeEffErr(H2EffInterX[z],H2EffErr[z]);    
    H2EffErr[z]->SetStats(0);
    //H2EffErr[z]->GetZaxis()->SetRangeUser(.0,maxefferr+.1);
    H2EffErr[z]->Draw("colz");    
  } 
  Canvas.Print(filename); 
  Canvas.Clear();
  Canvas.Divide(2,2);  
  for(Int_t z=0;z<H3Eff->GetZaxis()->GetNbins();z++){
    Canvas.cd(z+1);
    //HEffChiX[z].GetYaxis()->SetRangeUser(0,2);
    HEffChiX[z].SetStats(0);
    HEffChiX[z].Draw("p");
  }
  Canvas.Print(filename); 



  //-----------------------------------
  //make interpolation in mass
  //----------------------------------
  Canvas.Clear();
  Canvas.Divide(2,2);  
  TH2F* H2EffInterY[4];
  TH1F HEffChiY[4];
  for(Int_t z=0;z<H3Eff->GetZaxis()->GetNbins();z++){
    Canvas.cd(z+1);
    H2EffInterY[z]=Interpolate2DEfficiencyFitY(H2EffInterX[z],&HEffChiY[z]);        
    H2EffInterY[z]->SetStats(0);
    //H2EffInterY[z]->GetZaxis()->SetRangeUser(.0,.4);
    H2EffInterY[z]->Draw("colz"); 
  }
  Canvas.Print(filename); 
  Canvas.Clear();
  Canvas.Divide(2,2);  
  for(Int_t z=0;z<H3Eff->GetZaxis()->GetNbins();z++){
    Canvas.cd(z+1); 
    H2EffErr[z]=(TH2F*)H2EffInterY[z]->Clone();   		
    Fill2DRelativeEffErr(H2EffInterY[z],H2EffErr[z]);
	         
    H2EffErr[z]->SetStats(0);
    //H2EffErr[z]->GetZaxis()->SetRangeUser(.0,.03);
    H2EffErr[z]->Draw("colz");    
  } 
  Canvas.Print(filename); 
  Canvas.Clear();
  Canvas.Divide(2,2);  
  for(Int_t z=0;z<H3Eff->GetZaxis()->GetNbins();z++){
    Canvas.cd(z+1);
    //HEffChiY[z].GetYaxis()->SetRangeUser(0,2);
    HEffChiY[z].SetStats(0);
    HEffChiY[z].Draw("p");
  }
  Canvas.Print(filename); 


  //---------------------------------------------------
  ///create the 3D interpolation and show one x-y bin 
  //--------------------------------------------------
  TH2F HEffChiZ;
  //TH3F* H3DInterpolation=Interpolate2DEfficiencyFitZ(H2EffInterY,&HEffChiZ);
  TH3F* H3DInterpolation=Interpolate2DEfficiencyZ(H2EffInterY);
  TH1D* hzproj=H3DInterpolation->ProjectionZ();
  for(Int_t bz=1;bz<=H3DInterpolation->GetNbinsZ();bz++){
    hzproj->SetBinContent(bz,H3DInterpolation->GetBinContent(5,150,bz));
    hzproj->SetBinError(bz,H3DInterpolation->GetBinError(5,150,bz));
  }
  Canvas.Clear();
  hzproj->GetYaxis()->SetRangeUser(0,.3);
  hzproj->SetStats(0);
  hzproj->Draw();
  Canvas.Print(filename); 
  
  Canvas.Clear();
  HEffChiZ.Draw("colz");
  Canvas.Print(filename); 

  H3DInterpolation->SetName("H3Efficiency2");
  H3DInterpolation->Write();
  


  delete H3DInterpolation;
  delete hzproj;
    
  //clean
  for(Int_t z=0;z<H3Eff->GetZaxis()->GetNbins();z++){
    delete H2Eff[z];
    delete H2EffErr[z];
    delete H2EffInterX[z];
    delete H2EffInterY[z];
  }
  
  return 1; 
}




void FixPstarEff(TH2F* h2){
  
  //for each bin in y extrapolate into the last x bin
  TF1 fx("fx","[0]+[1]*x",h2->GetXaxis()->GetXmin(),h2->GetXaxis()->GetXmax());
  Float_t x0,y0,x1,y1,slope,yint;
  Int_t nxbins=h2->GetXaxis()->GetNbins();
  for(Int_t y=1;y<=h2->GetNbinsY();y++){
    x0=h2->GetXaxis()->GetBinCenter(nxbins-2);
    y0=h2->GetBinContent(nxbins-2,y);
    x1=h2->GetXaxis()->GetBinCenter(nxbins-1);
    y1=h2->GetBinContent(nxbins-1,y);
    slope=(y1-y0)/(x1-x0);
    yint=y0-slope*x0;
    
    fx.SetParameters(yint,slope);
    h2->SetBinContent(nxbins,y,fx.Eval(h2->GetXaxis()->GetBinCenter(nxbins)));
    h2->SetBinError(nxbins,y,h2->GetBinError(nxbins-1,y));

  }

}

TH2F* DstPiAnalysis::Interpolate2DEfficiencyX(TH2F* h2){
  if(!h2){
    cout<<"No histo"<<endl;
    return 0;
  }
   
  TString name=TString(h2->GetName())+"InterpolatedX";
  

  Int_t nbinsx=h2->GetNbinsX();
  Int_t nbinsy=h2->GetNbinsY();  
  TH2F* H2Interpolation=new TH2F(name,name,
				 100,h2->GetXaxis()->GetXmin(),h2->GetXaxis()->GetXmax(),
				 nbinsy,h2->GetYaxis()->GetXmin(),h2->GetYaxis()->GetXmax());
  TF1* fxinter=NULL;
  Float_t fxmin[5];
  Float_t fxmax[5];
  Float_t fx0[5];
  Float_t fx1[5];
  Float_t fy0[5];
  Float_t fy1[5];
  Float_t fslope[5];
  Float_t fyint[5];
  for(Int_t by=1;by<=nbinsy;by++){
    //for each bin in Y create a piece-wise function for interpolation in X
    fxmin[0]=3;
    fxmax[0]=h2->GetXaxis()->GetBinCenter(nbinsx-3);
    fx0[0]=h2->GetXaxis()->GetBinCenter(nbinsx-3);
    fx1[0]=h2->GetXaxis()->GetBinCenter(nbinsx-2);
    fy0[0]=h2->GetBinContent(nbinsx-3,by);
    fy1[0]=h2->GetBinContent(nbinsx-2,by);
    fslope[0]=(fy1[0]-fy0[0])/(fx1[0]-fx0[0]);
    fyint[0]=fy0[0]-fslope[0]*fx0[0];     
    //do the inner parts
    for(Int_t bx=1;bx<=3;bx++){
      fxmin[bx]=h2->GetXaxis()->GetBinCenter(nbinsx-3+bx-1);
      fxmax[bx]=h2->GetXaxis()->GetBinCenter(nbinsx-3+bx);
      fx0[bx]=fxmin[bx];
      fx1[bx]=fxmax[bx];
      fy0[bx]=h2->GetBinContent(nbinsx-3+bx-1,by);
      fy1[bx]=h2->GetBinContent(nbinsx-3+bx,by);
      fslope[bx]=(fy1[bx]-fy0[bx])/(fx1[bx]-fx0[bx]);
      fyint[bx]=fy0[bx]-fslope[bx]*fx0[bx];  
    }        
    //do the end    
    fxmin[4]=h2->GetXaxis()->GetBinCenter(nbinsx);
    fxmax[4]=h2->GetXaxis()->GetXmax();;
    fx0[4]=h2->GetXaxis()->GetBinCenter(nbinsx-1);
    fx1[4]=h2->GetXaxis()->GetBinCenter(nbinsx);
    fy0[4]=h2->GetBinContent(nbinsx-1,by);
    fy1[4]=h2->GetBinContent(nbinsx,by);
    fslope[4]=(fy1[4]-fy0[4])/(fx1[4]-fx0[4]);
    fyint[4]=fy0[4]-fslope[4]*fx0[4];     
    
    sprintf(textlabel,"(%.3f<=x&&x<%.3f)*(%.3f+%.3f*x)",fxmin[0],fxmax[0],fyint[0],fslope[0]);
    TString fform=textlabel;
    for(Int_t bx=1;bx<=4;bx++){
      sprintf(textlabel,"+(%.3f<=x&&x<%.3f)*(%.3f+%.3f*x)",fxmin[bx],fxmax[bx],fyint[bx],fslope[bx]);
      fform=fform+TString(textlabel);
    }    
    fxinter=new TF1((const char*)(TString("fxinter")+(long)by),(const char*)fform,h2->GetXaxis()->GetXmin(),h2->GetXaxis()->GetXmax());

    for(Int_t bx=1;bx<=H2Interpolation->GetNbinsX();bx++){ 
      //use function fo this mass bin for interpolation                
      H2Interpolation->SetBinContent(bx,by,fxinter->Eval(H2Interpolation->GetXaxis()->GetBinCenter(bx)));
      H2Interpolation->SetBinError(bx,by,h2->GetBinError(((int)(bx-1)*nbinsx)/H2Interpolation->GetNbinsX()+1,by));
      //cout<<bx<<" "<<(bx-1)*h2->GetNbinsX()/H2Interpolation->GetNbinsX()+1<<" "<<nbinsx<<endl;
    }
      
    delete fxinter;    
  }
 
  
  return H2Interpolation;
}


TH2F* DstPiAnalysis::Interpolate2DEfficiencyY(TH2F* h2){
  if(!h2){
    cout<<"No histo"<<endl;
    return 0;
  }
   
  TString name=TString(h2->GetName())+"InterpolatedY";
  
  Int_t nbinsx=h2->GetNbinsX();
  Int_t nbinsy=h2->GetNbinsY();  
  TH2F* H2Interpolation=new TH2F(name,name,
				 nbinsx,h2->GetXaxis()->GetXmin(),h2->GetXaxis()->GetXmax(),
				 40,h2->GetYaxis()->GetXmin(),h2->GetYaxis()->GetXmax());
  TF1* fxinter=NULL;
  Float_t fxmin[11];
  Float_t fxmax[11];
  Float_t fx0[11];
  Float_t fx1[11];
  Float_t fy0[11];
  Float_t fy1[11];
  Float_t fslope[11];
  Float_t fyint[11];
  for(Int_t bx=1;bx<=nbinsx;bx++){
    //for each bin in X create a piece-wise function for interpolation in X
    fxmin[0]=h2->GetYaxis()->GetXmin();
    fxmax[0]=h2->GetYaxis()->GetBinCenter(1);
    fx0[0]=h2->GetYaxis()->GetBinCenter(1);
    fx1[0]=h2->GetYaxis()->GetBinCenter(2);
    fy0[0]=h2->GetBinContent(bx,1);
    fy1[0]=h2->GetBinContent(bx,2);
    fslope[0]=(fy1[0]-fy0[0])/(fx1[0]-fx0[0]);
    fyint[0]=fy0[0]-fslope[0]*fx0[0];     
    //do the inner parts
    for(Int_t by=1;by<nbinsy;by++){
      fxmin[by]=h2->GetYaxis()->GetBinCenter(by);
      fxmax[by]=h2->GetYaxis()->GetBinCenter(by+1);
      fx0[by]=fxmin[by];
      fx1[by]=fxmax[by];
      fy0[by]=h2->GetBinContent(bx,by);
      fy1[by]=h2->GetBinContent(bx,by+1);
      fslope[by]=(fy1[by]-fy0[by])/(fx1[by]-fx0[by]);
      fyint[by]=fy0[by]-fslope[by]*fx0[by];  
    }        
    //do the end    
    fxmin[10]=h2->GetYaxis()->GetBinCenter(nbinsy);
    fxmax[10]=h2->GetYaxis()->GetXmax();
    fx0[10]=h2->GetYaxis()->GetBinCenter(nbinsy-1);
    fx1[10]=h2->GetYaxis()->GetBinCenter(nbinsy);
    fy0[10]=h2->GetBinContent(bx,nbinsy-1);
    fy1[10]=h2->GetBinContent(bx,nbinsy);
    fslope[10]=(fy1[10]-fy0[10])/(fx1[10]-fx0[10]);
    fyint[10]=fy0[10]-fslope[10]*fx0[10];     
    
    //create the function
    sprintf(textlabel,"(%.3f<=x&&x<%.3f)*(%.3f+%.3f*x)",fxmin[0],fxmax[0],fyint[0],fslope[0]);
    TString fform=textlabel;
    for(Int_t by=1;by<=nbinsy;by++){
      sprintf(textlabel,"+(%.3f<=x&&x<%.3f)*(%.3f+%.3f*x)",fxmin[by],fxmax[by],fyint[by],fslope[by]);
      fform=fform+TString(textlabel);
    }    
    fxinter=new TF1((const char*)(TString("fxinter")+(long)bx),(const char*)fform,h2->GetYaxis()->GetXmin(),h2->GetYaxis()->GetXmax());

    //fill the new histo
    for(Int_t by=1;by<=H2Interpolation->GetNbinsY();by++){ 
      //use function fo this mass bin for interpolation                
      H2Interpolation->SetBinContent(bx,by,fxinter->Eval(H2Interpolation->GetYaxis()->GetBinCenter(by)));
      H2Interpolation->SetBinError(bx,by,h2->GetBinError(bx,((int)(by-1)*nbinsy)/H2Interpolation->GetNbinsY()+1));
    }
      
    delete fxinter;    
  }
  
  
  return H2Interpolation;
}


TH3F* DstPiAnalysis::Interpolate2DEfficiencyZ(TH2F* h2[4]){
  if(!h2[0]){
    cout<<"No histo"<<endl;
    return 0;
  }
   
  TString name=TString(h2[0]->GetName())+"InterpolatedZ";
  
  //using the above 1-D ratios construct a 2-D extrapolated ratio, the x direction will binned into NAngleBins bins
  Int_t nbinsx=h2[0]->GetNbinsX();
  Int_t nbinsy=h2[0]->GetNbinsY();   
  TH3F* H3Interpolation=new TH3F(name,name,
				 nbinsx,h2[0]->GetXaxis()->GetXmin(),h2[0]->GetXaxis()->GetXmax(),
				 nbinsy,h2[0]->GetYaxis()->GetXmin(),h2[0]->GetYaxis()->GetXmax(),
				 40,-1.001,1.001);
  TF1* fxinter=NULL;
  Float_t fxmin[4];
  Float_t fxmax[4];
  Float_t fx0[4];
  Float_t fx1[4];
  Float_t fy0[4];
  Float_t fy1[4];
  Float_t fslope[4];
  Float_t fyint[4];

  Float_t zbinsize=.5;
  Float_t zmin=-1;

  for(Int_t by=1;by<=nbinsy;by++){
    for(Int_t bx=1;bx<=nbinsx;bx++){ 
      fxmin[0]=-1;
      fxmax[0]=-.75;
      fx0[0]=-.75;
      fx1[0]=-.25;
      fy0[0]=h2[0]->GetBinContent(bx,by);
      fy1[0]=h2[1]->GetBinContent(bx,by);
      fslope[0]=(fy1[0]-fy0[0])/(fx1[0]-fx0[0]);
      fyint[0]=fy0[0]-fslope[0]*fx0[0];     
      //do the inner parts
      for(Int_t bz=1;bz<=2;bz++){
	fxmin[bz]=zmin+zbinsize/2+(bz-1)*zbinsize;
	fxmax[bz]=zmin+zbinsize/2+(bz)*zbinsize;
	fx0[bz]=fxmin[bz];
	fx1[bz]=fxmax[bz];
	fy0[bz]=h2[bz-1]->GetBinContent(bx,by);
	fy1[bz]=h2[bz]->GetBinContent(bx,by);
	fslope[bz]=(fy1[bz]-fy0[bz])/(fx1[bz]-fx0[bz]);
	fyint[bz]=fy0[bz]-fslope[bz]*fx0[bz];  
      }        
      //do the end    
      fxmin[3]=.25;
      fxmax[3]=1;
      fx0[3]=-.25;
      fx1[3]=.25;
      fy0[3]=h2[1]->GetBinContent(bx,by);
      fy1[3]=h2[2]->GetBinContent(bx,by);
      fslope[3]=(fy1[3]-fy0[3])/(fx1[3]-fx0[3]);
      fyint[3]=fy0[3]-fslope[3]*fx0[3];     
    
      sprintf(textlabel,"(%.3f<=x&&x<%.3f)*(%.3f+%.3f*x)",fxmin[0],fxmax[0],fyint[0],fslope[0]);
      TString fform=textlabel;
      for(Int_t bz=1;bz<=3;bz++){
	sprintf(textlabel,"+(%.3f<=x&&x<%.3f)*(%.3f+%.3f*x)",fxmin[bz],fxmax[bz],fyint[bz],fslope[bz]);
	fform=fform+TString(textlabel);
      }    
      fxinter=new TF1((const char*)(TString("fxinter")+(long)bx+(long)by),
		      (const char*)fform,h2[0]->GetZaxis()->GetXmin(),h2[0]->GetZaxis()->GetXmax());


      for(Int_t bz=1;bz<=H3Interpolation->GetNbinsZ();bz++){              
	H3Interpolation->SetBinContent(bx,by,bz,fxinter->Eval(H3Interpolation->GetZaxis()->GetBinCenter(bz)));	
	H3Interpolation->SetBinError(bx,by,bz,h2[1]->GetBinError(bx,by));//h2[(bz-1)*4/H3Interpolation->GetNbinsZ()]->GetBinError(bx,by));	
      }
      
      delete fxinter;    
    }
  }
  
  return H3Interpolation;
}


TH3F* DstPiAnalysis::Interpolate2DEfficiencyFitZ(TH2F* h2[4],TH2F* HEffChiZ){
  if(!h2[0]){
    cout<<"No histo"<<endl;
    return 0;
  }
   
  TString name=TString(h2[0]->GetName())+"InterZ";
  
  //using the above 1-D ratios construct a 2-D extrapolated ratio, the x direction will binned into NAngleBins bins
  Int_t nbinsx=h2[0]->GetNbinsX();
  Int_t nbinsy=h2[0]->GetNbinsY();   
  TH3F* H3Interpolation=new TH3F(name,name,
				 nbinsx,h2[0]->GetXaxis()->GetXmin(),h2[0]->GetXaxis()->GetXmax(),
				 nbinsy,h2[0]->GetYaxis()->GetXmin(),h2[0]->GetYaxis()->GetXmax(),
				 40,-1.001,1.001);

  // 
  (*HEffChiZ).SetNameTitle(name+"chi",name+"chi");
  (*HEffChiZ).SetBins(nbinsx,h2[0]->GetXaxis()->GetXmin(),h2[0]->GetXaxis()->GetXmax(),
		      nbinsy,h2[0]->GetYaxis()->GetXmin(),h2[0]->GetYaxis()->GetXmax());
  
  
  //there are 40 bins in 2.1-->3.1: use 5 bins 2.375-->2.500 => bins 12->16
  TH1F HSlice("HSlice","HSlice",4,-1,1);  
  TF1 FSlice("FSlice","[0]+[1]*(x-[2])",-1,1);
  FSlice.SetParameters(.5,.01,-.5);

  Float_t eff;
  Float_t error;
  Float_t pars[3];
  Float_t chiperdf;
  Int_t ngood=0;
  //for each bin in x-y plane fit in z
  for(Int_t by=1;by<=nbinsy;by++){
    for(Int_t bx=1;bx<=nbinsx;bx++){ 
    
      ngood=0;
      for(Int_t bz=1;bz<=3;bz++){
	if(h2[bz-1]->GetBinContent(bx,by)>0){
	  ngood++;
	  HSlice.SetBinContent(bz,h2[bz-1]->GetBinContent(bx,by));
	  HSlice.SetBinError(bz,h2[bz-1]->GetBinError(bx,by));	 
	}else{
	  HSlice.SetBinContent(bz,0);
	  HSlice.SetBinError(bz,0);
	}          
      }        
      
      FSlice.FixParameter(2,HSlice.GetMean());
      if(ngood<3||HSlice.Fit(&FSlice,"QCF","",-1,1)!=0){
	if(ngood>=3)cout<<"*********fit failed: bin "<<bx<<" "<<by<<endl; 
	FSlice.SetParameter(0,0);
	FSlice.SetParameter(1,0);
	FSlice.SetParError(0,0);
	FSlice.SetParError(1,0);
	FSlice.SetChisquare(0);
	chiperdf=0;
      }else 
	chiperdf=FSlice.GetChisquare()/FSlice.GetNDF();
      (*HEffChiZ).SetBinContent(bx,by,chiperdf);

      pars[0]=FSlice.GetParameter(0);
      pars[1]=FSlice.GetParameter(1);
      pars[2]=FSlice.GetParameter(2);

      for(Int_t bz=1;bz<=H3Interpolation->GetNbinsZ();bz++){              
	FSlice.SetParameters(pars[0],pars[1],pars[2]);
	eff=FSlice.Eval(H3Interpolation->GetZaxis()->GetBinCenter(bz));
	H3Interpolation->SetBinContent(bx,by,bz,eff);	
   
	error=sqrt(FSlice.GetParError(0)*FSlice.GetParError(0)
		   +(H3Interpolation->GetZaxis()->GetBinCenter(bz)-FSlice.GetParameter(2))
		   *(H3Interpolation->GetZaxis()->GetBinCenter(bz)-FSlice.GetParameter(2))
		   *FSlice.GetParError(1)
		   *FSlice.GetParError(1));

	H3Interpolation->SetBinError(bx,by,bz,error);
      }
      
    }
  }
  
  return H3Interpolation;
}


Int_t DstPiAnalysis::CreateEffCorrectedHistos(){
 

  if(!OpenReducedNtuple())return 0;
  
  if(!OpenEfficiency()){cout<<"Could not open efficiecy file"<<endl;return 0;}
    
  Int_t ndstarhelbins=40;
  Int_t nmassbins=200;
  TH2F H2DstPiMVsDstarHel("H2DstPiMVsDstarHel","H2DstPiMVsDstarHel",nmassbins,2.1,3.1,ndstarhelbins,-1.0,1.0);


  Float_t mass;Int_t massbin;Float_t minmass=2.1;Float_t massstep=.005;
  Int_t dstarhelbin;Float_t mindstarhel=-1;Float_t dstarhelstep=.05;

  Float_t binadd; 
  Int_t e=0;
  while(ReducedNtuple->GetEntry(e)){
    e++;
    if(e%50000==0)cout<<e<<" cands done"<<endl;

    mass = DstarPDGMass + dstpideltam;
    massbin = (int)((mass-minmass)/massstep)+1;   
    if(massbin>nmassbins)massbin=nmassbins+1;
    if(massbin<0)massbin=0;

    dstarhelbin = (int)((dstarhelicity-mindstarhel)/dstarhelstep)+1;   
    if(dstarhelbin>ndstarhelbins)dstarhelbin=ndstarhelbins+1;
    if(dstarhelbin<0)dstarhelbin=0;

    
    binadd=0;
    if(GetEfficiency()>0)
      binadd=1./GetEfficiency();
    H2DstPiMVsDstarHel.SetBinContent(massbin,dstarhelbin,H2DstPiMVsDstarHel.GetBinContent(massbin,dstarhelbin)+binadd);
       
  }

  TFile EffCorrHistos(_OutputDir+"/EffCorrHistos.root","recreate");  
  H2DstPiMVsDstarHel.Write();
  EffCorrHistos.Close();

  if(CloseReducedNtuple()!=1)return 0;
  CloseEfficiency();
  
  return 1;
}
*/
