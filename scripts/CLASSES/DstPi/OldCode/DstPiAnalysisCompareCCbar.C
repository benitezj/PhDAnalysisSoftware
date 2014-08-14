#include "DstPiAnalysis.h"

#include <TF2.h>

#define NEWFITMIN 2.160
#define NEWFITMAX 3.160
#define NNEWFITBINS 200


TH1F* HDstarPi[8];
TH1F* HDstarPiCCbar[8];
TH1F* HDATACCbarRatio[8];
TH1F* HDATACCbarRatioInter[8];

Int_t GroupY=5;
Int_t GroupX=1;
Int_t NAngleBins=40/GroupX;

Int_t massbincheck=20;
 
Int_t DstPiAnalysis::CompareCCbar(){
  //***************************************************
  //The plan of this macro is the folllowing:
  //1: use the wrong sign distributions to create a corrective ratio for the right sign distributions
  //2: apply the ratio to the right sign ccbar and then subtract that from the right sign data

  cout<<"Going to compare the data to the ccbar"<<endl;
  //Use the same histograms that are fitted

  cout<<"Lumi check: ccbar="<<lumiccbar<<" data="<<lumidata<<" scale="<<scalefactorccbar<<endl;
  
  //ps file
  filename=_OutputDir+"/CCbarComparison.ps";
  Canvas.Print(filename+"[");

  ///print the cuts into the file
  database.PrintCutList(&Canvas,filename);  
    

  //----------------------------
  //Do the Wrong Sign
  ///---------------------------  
  //-------------------------------------------------------- 
  //Get the CCbar histos :: Important to open this file before thet data file to save ratio histos in data file 
  //--------------------------------------------------------
  cout<<"Getting the ccbar histograms"<<endl;
  TString wsfile;
  if(_D0Mode==1)wsfile="DstarCharged/DstarPi/MC/SP-1005/Kpi/HistosForWSFit.root";
  if(_D0Mode==2)wsfile="DstarCharged/DstarPi/MC/SP-1005/K3pi/HistosForWSFit.root";  
  if(_D0Mode==3)wsfile="DstarCharged/DstarPi/MC/SP-1005/KpiAndK3pi/HistosForWSFit.root";  
  TFile HistFileCCbar(wsfile,"Update");
  gROOT->cd();

  //-------------------------------------------------------- 
  //Get the DATA histos 
  //--------------------------------------------------------
  cout<<"Getting the data histograms"<<endl;
  TFile HistFile(_OutputDir+"/HistosForWSFit.root","Update");
  //HistFile.ls();
  gROOT->cd();

  //plot the D*
  TH1F* HDstar=(TH1F*)HistFile.Get("RawHistos/HDstar");
  if(!HDstar)return 0;
  SetHistoXYLabels(HDstar,"m(K^{+}#pi^{-}#pi^{+})-m(K^{+}#pi^{-})  (GeV/c^{2})","GeV");
  
  TH1F* HDstarCCbar=(TH1F*)HistFileCCbar.Get("RawHistos/HDstar");
  if(!HDstarCCbar)return 0;
  //scale down
  HDstarCCbar->Scale(HDstar->Integral()/HDstarCCbar->Integral());
 
  Canvas.Clear();
  HDstar->Draw("pe");
  HDstarCCbar->Draw("same");
  Canvas.Print(filename);
  cout<<"Dstar peak saved"<<endl;

  
  ///cos theta dependent distributions
  if(!PlotMassVsDstarAngleCut(&HistFile,"RawHistos/HDstarPiSlice",&HistFileCCbar,"RawHistos/HDstarPiSlice",&Canvas,filename,0,2))return 0;
  cout<<"Done with SaveSliceCuts"<<endl;

 
  //make ratios using the WS distributions
  if(!CreateDstarAngleCorrection(&HistFile,&HistFileCCbar))return 0;   
  cout<<"Done with CreateDstarAngleCorrection "<<endl;

  
  //show the ratios
  if(!ShowRatios(&HistFile,&Canvas,filename))return 0; 


  //----------------------------
  //Do the Right Sign
  ///-----------------------------
  //Get the DATA histos 
  cout<<"Getting the data histograms"<<endl;
  TFile HistFileRS(_OutputDir+"/HistosForFit.root","read");
  //HistFileRS.ls();
    
  cout<<"Getting the ccbar histograms"<<endl;
  TString rsfile;
  if(_D0Mode==1)rsfile="DstarCharged/DstarPi/MC/SP-1005/Kpi/HistosForFit.root";
  if(_D0Mode==2)rsfile="DstarCharged/DstarPi/MC/SP-1005/K3pi/HistosForFit.root";  
  if(_D0Mode==3)rsfile="DstarCharged/DstarPi/MC/SP-1005/KpiAndK3pi/HistosForFit.root"; 
  TFile HistFileRSCCbar(rsfile,"update");
  //  HistFileRSCCbar.ls();  
  gROOT->cd();

  //show ccbar and data, ccbar normalized to by luminosity
  if(!PlotMassVsDstarAngleCut(&HistFileRS,"RawHistos/HDstarPiSlice",&HistFileRSCCbar,"RawHistos/HDstarPiSlice",&Canvas,filename,0,2))return 0;
  cout<<"Done with Slices scale lumi"<<endl;

  //show ccbar and data, ccbar normalized in first bins
  if(!PlotMassVsDstarAngleCut(&HistFileRS,"RawHistos/HDstarPiSlice",&HistFileRSCCbar,"RawHistos/HDstarPiSlice",&Canvas,filename,0,1))return 0;
  cout<<"Done with Slices scale first bins"<<endl;

 
  //------------------------------
  //make corrected ccbar plots to ccbar 2-D map
  //----------------------------
  if(!ApplyDstarAngleCorrection(&HistFile,&HistFileRSCCbar))return 0;    
  cout<<"Done with ApplyDstarAngleCorrection"<<endl;

  //show ccbar corrected and data, ccbar normalized in first bins
  if(!PlotMassVsDstarAngleCut(&HistFileRS,"RawHistos/HDstarPiSlice",&HistFileRSCCbar,"CorrHistos/HDstarPiSlice",&Canvas,filename,0,1))
    return 0;
  cout<<"Done with Corrected Slices scale first bins"<<endl;

    
  //plot the corrected data
  if(!PlotCorrectedData(&HistFileRS,&HistFileRSCCbar,&Canvas,filename))return 0;

  HistFile.Close();
  HistFileCCbar.Close();

  HistFileRS.Close();
  HistFileRSCCbar.Close();

  Canvas.Print(filename+"]");
  return 1;

}

Int_t DstPiAnalysis::PlotCorrectedData(TFile* DataFile,TFile* MCFile,TCanvas* Canvas,TString filename){
  //------------------------------------------
  //apply 2-D correction from interpolation
  //------------------------------------------
  //show the data
  TH2* h2map=(TH2*)DataFile->Get("RawHistos/H2DstarPi");
  if(!h2map)return 0;
  //first rebin data
  h2map=h2map->Rebin2D(GroupX,GroupY,"H2DstarPi_Rebin"); 
  TH2F H2MapRaw("H2MapRaw","H2MapRaw",NAngleBins,-1.001,1.001,NNEWFITBINS,NEWFITMIN,NEWFITMAX);//
  for(Int_t bx=1;bx<=NAngleBins;bx++){
    for(Int_t by=1;by<=NNEWFITBINS;by++){      
      H2MapRaw.SetBinContent(bx,by,h2map->GetBinContent(bx,(160/GroupY)+by));      
    }
  }
  Canvas->Clear(); Canvas->SetLogz(1); H2MapRaw.Draw("colz");Canvas->Print(filename);
  delete h2map;


  //show the raw ccbar
  TH2* h2cmapraw=(TH2*)MCFile->Get("RawHistos/H2DstarPi");   
  h2cmapraw=h2cmapraw->Rebin2D(GroupX,GroupY,"H2DstarPiCCbar_Rebin");
  TH2F H2CMapRaw("H2CMapRaw","H2CMapRaw",NAngleBins,-1.001,1.001,NNEWFITBINS,NEWFITMIN,NEWFITMAX);//
  for(Int_t bx=1;bx<=NAngleBins;bx++){
    for(Int_t by=1;by<=NNEWFITBINS;by++){      
      H2CMapRaw.SetBinContent(bx,by,h2cmapraw->GetBinContent(bx,(160/GroupY)+by));      
    }
  }  
  Canvas->Clear(); Canvas->SetLogz(1); H2CMapRaw.Draw("colz");Canvas->Print(filename);
  delete h2cmapraw;

  //show the corrected ccbar
  TH2* h2cmap=(TH2*)MCFile->Get("CorrHistos/H2Map");  
  h2cmap->SetTitle("H2Map_Corrected");
  Canvas->Clear(); Canvas->SetLogz(1); h2cmap->Draw("colz");Canvas->Print(filename);


  //make the 2-D subtraction of the CCbar from the DATA
  TH2*h2mapsig=(TH2*)H2MapRaw.Clone();
  h2mapsig->Sumw2();
  Scale2DMass(&H2MapRaw,h2cmap,Canvas,filename); 
  h2mapsig->Add(h2cmap,-1);
  h2mapsig->SetTitle("Data After Subtraction Interpolated");
  Canvas->Clear(); Canvas->SetLogz(1); h2mapsig->Draw("colz");Canvas->Print(filename);
  PlotXSlices(h2mapsig,Canvas,filename);

  //correct the tails
  Canvas->SetLogz(0);
  if(!FitMassTails(h2mapsig,Canvas,filename))return 0;
  h2mapsig->SetTitle("Data After Subtraction Interpolated and Plane Fit");
  Canvas->Clear();Canvas->SetLogz(1);h2mapsig->Draw("colz");Canvas->Print(filename);  
  PlotXSlices(h2mapsig,Canvas,filename);

  //show angle distribution before effciency correction
  TH1F*h2mapsigX=(TH1F*)h2mapsig->ProjectionX();
  Canvas->Clear();h2mapsigX->GetYaxis()->SetRangeUser(-100,1.15*h2mapsigX->GetMaximum());h2mapsigX->Draw("pe");Canvas->Print(filename); 
  delete h2mapsigX;

  //Apply efficiency vs dstacostheta
  TFile FAngleEff("DstarCharged/DstarPi/MC/D2420/Kpi/ResolutionEff.root","read");
  gROOT->cd();
  TH1F* Hangleeff=(TH1F*)FAngleEff.Get("HDstarAngle")->Clone();
  FAngleEff.Close();
  Canvas->Clear();Hangleeff->Draw("pe");Canvas->Print(filename);
  Int_t index=0;
  Float_t eff=0;
  for(Int_t bx=1;bx<=h2mapsig->GetNbinsX();bx++){
    index=(bx-1)*Hangleeff->GetNbinsX()/h2mapsig->GetNbinsX() + 1;      
    eff=Hangleeff->GetBinContent(index);
    for(Int_t by=1;by<=h2mapsig->GetNbinsY();by++){
      h2mapsig->SetBinContent(bx,by,h2mapsig->GetBinContent(bx,by)/eff);
      h2mapsig->SetBinError(bx,by,h2mapsig->GetBinError(bx,by)/eff);
    }
  }
  h2mapsig->SetTitle("Data After Subtraction Interpolated and Plane Fit and EffCorr");
  Canvas->Clear(); Canvas->SetLogz(1); h2mapsig->Draw("colz");Canvas->Print(filename);
  delete Hangleeff;  
  PlotXSlices(h2mapsig,Canvas,filename);


  //make a projection of the clean signal on to the Mass axis
  TH1F*h2mapsigM=(TH1F*)h2mapsig->ProjectionY();
  Canvas->Clear();
  h2mapsigM->GetYaxis()->SetRangeUser(-.1*h2mapsigM->GetMaximum(),1.15*h2mapsigM->GetMaximum());
  h2mapsigM->Draw("pe");
  cutline.DrawLine(NEWFITMIN,0,NEWFITMAX,0);
  Canvas->Print(filename); 
 
  //make a projection of the clean signal onto the angle axis
  h2mapsigX=(TH1F*)h2mapsig->ProjectionX();
  Canvas->Clear();h2mapsigX->GetYaxis()->SetRangeUser(-100,1.15*h2mapsigX->GetMaximum());h2mapsigX->Draw("pe");Canvas->Print(filename); 
 
  //Cut the dstarcostheta above .5 then make a projection of the clean signal on to the Mass axis
  for(Int_t bx=1;bx<=h2mapsig->GetNbinsX();bx++){
    for(Int_t by=1;by<=h2mapsig->GetNbinsY();by++){
      if(h2mapsig->GetXaxis()->GetBinCenter(bx)>.5){
	h2mapsig->SetBinContent(bx,by,0);
	h2mapsig->SetBinError(bx,by,0);
      }
    }
  }  

  TH1F*h2mapsigMCut=(TH1F*)h2mapsig->ProjectionY();
  Canvas->Clear();
  h2mapsigMCut->GetYaxis()->SetRangeUser(-.1*h2mapsigMCut->GetBinContent(h2mapsigMCut->GetMaximumBin()),1.1*h2mapsigMCut->GetBinContent(h2mapsigMCut->GetMaximumBin()));
  h2mapsigMCut->Draw("pe");
  cutline.DrawLine(NEWFITMIN,0,NEWFITMAX,0);
  Canvas->Print(filename); 


  delete h2mapsigM;
  delete h2mapsigX;
  delete h2mapsig;
  
  return 1;
}


Int_t DstPiAnalysis::ShowRatios(TFile* File,TCanvas*C,TString filename){
  cout<<"Going to show the ratios"<<endl;

  //show the ratios
  C->Clear();
  C->Divide(2,4);  
  for(Int_t c=7;c>=0;c--){
    C->cd(c+1);

    TString hname=TString("Ratios/HDstarPiSlice")+(long)c+"_Ratio";
    HDATACCbarRatioInter[c]=(TH1F*)File->Get(hname);
    if(!HDATACCbarRatioInter[c])return 0;  
    HDATACCbarRatioInter[c]->GetYaxis()->SetRangeUser(0,3);
    HDATACCbarRatioInter[c]->Draw("pe");

    hname=TString("Ratios/HDstarPiSlice")+(long)c+"_CoarseRatio";
    HDstarPi[c]=(TH1F*)File->Get(hname);
    if(!HDstarPi[c])return 0;    
    HDstarPi[c]->SetMarkerColor(2);
    HDstarPi[c]->SetLineColor(2);
    HDstarPi[c]->Draw("pesame");


    cutline.DrawLine(NEWFITMIN,1,NEWFITMAX,1);
  }
  C->Print(filename);

  
  //show the ratio map 
  TH2* h2mapratio=(TH2*)File->Get("Ratios/H2RatioMap");
  if(!h2mapratio)return 0;
  C->Clear();h2mapratio->Draw("colz");C->Print(filename);


  //Also for one mass bin show the interpolation in X 
  TH1F hratiointerVsx("hratiointerVsx","hratiointerVsx",NAngleBins,-1.001,1.001);
  for(Int_t x=1;x<=NAngleBins;x++){
    hratiointerVsx.SetBinContent(x,h2mapratio->GetBinContent(x,massbincheck));
    hratiointerVsx.SetBinError(x,h2mapratio->GetBinError(x,massbincheck));
  }
  TH1F hratiovsx("hratiovsx","hratiovsx",8,-1.001,1.001);
  for(Int_t x=1;x<=8;x++){
    hratiovsx.SetBinContent(x,HDATACCbarRatioInter[8-x]->GetBinContent(massbincheck));
    hratiovsx.SetBinError(x,HDATACCbarRatioInter[8-x]->GetBinError(massbincheck));
  }

  C->Clear();   
  hratiointerVsx.Draw("pe");
  hratiovsx.SetMarkerColor(2);
  hratiovsx.SetLineColor(2);
  hratiovsx.Draw("pesame");
  C->Print(filename); 
  
  return 1;
}

Int_t DstPiAnalysis::PlotXSlices(TH2*h2,TCanvas*C,TString filename){
  
  //make bins in angle project each onto the Mass axis
  C->Clear();
  C->Divide(2,4);  
  for(Int_t bx=0;bx<8;bx++){    
    C->cd(bx+1);
    HDstarPi[bx]=(TH1F*)h2->ProjectionY(TString("_py")+(long)bx,(8-bx-1)*(40/(8*GroupX)),(8-bx)*(40/(8*GroupX)),"");
    HDstarPi[bx]->SetMarkerSize(.02);
    HDstarPi[bx]->Draw("pe"); 
    cutline.DrawLine(NEWFITMIN,0,NEWFITMAX,0);
  }
  C->Print(filename); 
  for(Int_t bx=0;bx<8;bx++)
    delete HDstarPi[bx];

  return 1;
}

Int_t Create1DHistosFromTailFits(TF1* Ftail[8],TH1F* HTail[8]){
  for(Int_t i=0;i<8;i++){
    HTail[i]=new TH1F(TString(Ftail[i]->GetName())+"hist",TString(Ftail[i]->GetName())+"hist",NNEWFITBINS,NEWFITMIN,NEWFITMAX);
    
    for(Int_t m=1;m<=HTail[i]->GetNbinsX();m++){
      HTail[i]->SetBinContent(m,Ftail[i]->Eval(HTail[i]->GetBinCenter(m))/(40./(8.*GroupX)));
      HTail[i]->SetBinError(m,0);//assign error to 0
    }
    
  }
  
  return 1;
}

Int_t DstPiAnalysis::FitMassTails(TH2*h2,TCanvas*C,TString filename){

  //clean the signal  region
  TH2*h2clone=(TH2*)h2->Clone();
  for(Int_t bx=1;bx<=h2clone->GetNbinsX();bx++){
    for(Int_t by=1;by<=h2clone->GetNbinsY();by++){
      if(2.25<h2clone->GetYaxis()->GetBinCenter(by)&&h2clone->GetYaxis()->GetBinCenter(by)<3.05){
	h2clone->SetBinContent(bx,by,0);
	h2clone->SetBinError(bx,by,0);
      }
    }
  }

  TF1* Ftail[8];
  C->Clear();
  C->Divide(2,4);
  for(Int_t bx=0;bx<8;bx++){    
    C->cd(bx+1);
    Ftail[bx]=new TF1(TString("Ftail")+(long)bx,"[0]+[1]*x+[2]*x**2",NEWFITMIN,NEWFITMAX);
    HDstarPi[bx]=(TH1F*)h2clone->ProjectionY(TString("_py")+(long)bx,(8-bx-1)*(40/(8*GroupX)),(8-bx)*(40/(8*GroupX)),"");
    HDstarPi[bx]->Fit(TString("Ftail")+(long)bx);  
    HDstarPi[bx]->Draw("pe");
    cutline.DrawLine(NEWFITMIN,0,NEWFITMAX,0);
  }
  C->Print(filename);
  
  //from functions create histos 
  TH1F* HTail[8];
  Create1DHistosFromTailFits(Ftail,HTail);
  //from histos create 2-D map
  TH2F*h2flatbkg=ExtrapolateInAngle(HTail);
  h2flatbkg->SetTitle("Tails Fit");
  C->Clear();    
  h2flatbkg->Draw("colz");
  C->Print(filename);

  h2->Add(h2flatbkg,-1); 
  
  for(Int_t bx=0;bx<8;bx++){
    delete HDstarPi[bx];
    delete Ftail[bx];
  }

  delete h2clone;
  delete h2flatbkg;

  return 1;
}

Int_t DstPiAnalysis::Scale2DMass(TH2*h2,TH2*h2c,TCanvas* Canvas,TString filename){
  TH2*h2clone=(TH2*)h2->Clone();
  TH2*h2cclone=(TH2*)h2c->Clone();
  
  //clean both histos in the high mass
  for(Int_t bx=1;bx<=h2->GetNbinsX();bx++){
    for(Int_t by=1;by<=h2->GetNbinsY();by++){
      if(2.25<h2clone->GetYaxis()->GetBinCenter(by)/*&&h2clone->GetYaxis()->GetBinCenter(by)<3.1*/){
	h2clone->SetBinContent(bx,by,0);
	h2cclone->SetBinContent(bx,by,0);
      }
    }
  }
   
  //project onto the x-axis and get x-dependent scaling  
  TH1F*h2X=(TH1F*)h2clone->ProjectionX();
  TH1F*h2cX=(TH1F*)h2cclone->ProjectionX();
  //before making ratio do a rebin to increase stats
  TH1F*h2Xr=(TH1F*)h2X->Rebin(40/(8*GroupX),"h2XRebin",0);
  TH1F*h2cXr=(TH1F*)h2cX->Rebin(40/(8*GroupX),"h2cXRebin",0);  
  if(!h2Xr||!h2cXr)return 0;
  h2Xr->Divide(h2cXr);

  //create a fine binning histo of the angle dependent scaling factor
  TH1F*h2Xrfine=ExtrapolateRatio(h2Xr,5);
  if(!h2Xrfine)return 0;
  

  //scale the c-cbar 2-D mass
  for(Int_t bx=1;bx<=h2->GetNbinsX();bx++){
    for(Int_t by=1;by<=h2->GetNbinsY();by++){
      h2c->SetBinContent(bx,by,h2c->GetBinContent(bx,by)*h2Xrfine->GetBinContent(bx));
    }
  }
  
  //make a plot
  Canvas->Clear();
  h2Xrfine->GetYaxis()->SetRangeUser(0,1);
  h2Xrfine->Draw("pe");
  h2Xr->SetMarkerSize(.07);
  h2Xr->SetMarkerColor(2);
  h2Xr->SetLineColor(2);  
  h2Xr->Draw("pesame");
  cutline.DrawLine(-1,scalefactorccbar,1,scalefactorccbar);
  Canvas->Print(filename);


  delete h2clone;
  delete h2cclone;
  delete h2X;
  delete h2cX;
  delete h2Xr;
  delete h2Xrfine ;
  delete h2cXr;

  return 1;
}


Int_t DstPiAnalysis::PlotMassVsDstarAngleCut(TFile* DataFile,TString plotname,TFile* MCFile,TString mcplotname,TCanvas* Canvas,TString filename,Bool_t accumulate,Int_t whichscale){

  ///--------------------
  //get the slices and add them up
  //----------------------
  Double_t scaleint=0;
  Canvas->Clear();
  Canvas->Divide(2,4);  
  for(Int_t c=7;c>=0;c--){//start backwards because slice 1 is at .75->1
    Canvas->cd(c+1);
    TString hname=plotname+(long)c;
    TString mchname=mcplotname+(long)c;
    //cout<<hname<<endl;
    HDstarPi[c]=(TH1F*)DataFile->Get(hname);
    if(!HDstarPi[c])return 0;    
    HDstarPi[c]=(TH1F*)CreateRebinnedDstPiHisto(HDstarPi[c],NNEWFITBINS,NEWFITMIN,NEWFITMAX);
    if(!HDstarPi[c])return 0;
    SetHistoXYLabels(HDstarPi[c],DstarPiXLabel,"GeV"); 
    HDstarPi[c]->Sumw2();   
    scaleint=HDstarPi[c]->Integral(1,20);
    if(c<7&&accumulate)HDstarPi[c]->Add(HDstarPi[c+1],1); //add to previous
    

    //Get the ccbar histos 
    HDstarPiCCbar[c]=(TH1F*)MCFile->Get(mchname);
    if(!HDstarPiCCbar[c])return 0;
    if(mcplotname!="CorrHistos/HDstarPiSlice")HDstarPiCCbar[c]=(TH1F*)CreateRebinnedDstPiHisto(HDstarPiCCbar[c],NNEWFITBINS,NEWFITMIN,NEWFITMAX);
    if(!HDstarPiCCbar[c])return 0;
    HDstarPiCCbar[c]->Sumw2();
    //subtract residual signal
    TH1F*HCCbarCorrHisto=(TH1F*)MCFile->Get("RawHistos/HCCbarCorrHisto");
    if(HCCbarCorrHisto)
      if(HCCbarCorrHisto->GetName()=="HCCbarCorrHisto")
	HDstarPiCCbar[c]->Add(HCCbarCorrHisto,-1);
    
 
    //scale down
    if(whichscale==1)HDstarPiCCbar[c]->Scale(scaleint/HDstarPiCCbar[c]->Integral(1,20));
    if(whichscale==2)HDstarPiCCbar[c]->Scale(scalefactorccbar);    
    if(c<7&&accumulate)HDstarPiCCbar[c]->Add(HDstarPiCCbar[c+1],1);//add the previous one
 
    //comparison plot 
    HDstarPi[c]->SetMarkerSize(.06);  
    HDstarPi[c]->GetYaxis()->SetLabelSize(.08);
    HDstarPi[c]->GetYaxis()->SetNdivisions(5);  
    HDstarPi[c]->GetYaxis()->SetRangeUser(0,1.15*HDstarPi[c]->GetMaximum());
    HDstarPi[c]->Draw("pe");
    HDstarPiCCbar[c]->SetMarkerSize(.06);
    HDstarPiCCbar[c]->SetMarkerColor(4);
    HDstarPiCCbar[c]->SetLineColor(4);
    HDstarPiCCbar[c]->Draw("pesame");
  }
  Canvas->Print(filename);


  //difference plot
  Canvas->Clear();
  Canvas->Divide(2,4); 
  for(Int_t c=0;c<8;c++){ 
    Canvas->cd(c+1);
    HDstarPi[c]->Add(HDstarPiCCbar[c],-1);   
    HDstarPi[c]->Draw("pe");
    cutline.DrawLine(NEWFITMIN-.01,0,NEWFITMAX,0);    
  }
  Canvas->Print(filename);


  for(Int_t c=0;c<8;c++){ 
    delete HDstarPi[c];
    delete HDstarPiCCbar[c];
  }
  


  return 1;
}


Int_t DstPiAnalysis::CreateDstarAngleCorrection(TFile* DataFile,TFile* MCFile){
  cout<<"Making ratio of Wrong Sign DATA/CCBar and writing to datfile"<<endl;
  //need to create mass dependent ratios of the data/ccbar   
  //use the same root file to save the ratios
  DataFile->SetWritable(1);
  DataFile->rmdir("Ratios");
  DataFile->mkdir("Ratios");
  DataFile->cd("Ratios");
  
  //DataFile->ls();
  //return 0;
  
  for(Int_t c=0;c<8;c++){
    TString hname=TString("HDstarPiSlice")+(long)c;
    HDstarPi[c]=(TH1F*)DataFile->Get(TString("RawHistos/")+hname);
    if(!HDstarPi[c])return 0;
    HDstarPi[c]=(TH1F*)CreateRebinnedDstPiHisto(HDstarPi[c],NNEWFITBINS/10,NEWFITMIN,NEWFITMAX);
    if(!HDstarPi[c])return 0;
    HDstarPi[c]->Sumw2();

    //Get the ccbar histos 
    HDstarPiCCbar[c]=(TH1F*)MCFile->Get(TString("RawHistos/")+hname);
    if(!HDstarPiCCbar[c])return 0;
    HDstarPiCCbar[c]=(TH1F*)CreateRebinnedDstPiHisto(HDstarPiCCbar[c],NNEWFITBINS/10,NEWFITMIN,NEWFITMAX);  
    if(!HDstarPiCCbar[c])return 0;
    HDstarPiCCbar[c]->Sumw2();
  
    //scale down
    //cout<<" first bins="<<HDstarPi[c]->Integral(1,20)/HDstarPiCCbar[c]->Integral(1,20)<<"  lumi="<<scalefactorccbar<<endl;
    HDstarPiCCbar[c]->Scale(scalefactorccbar);
   
    //create the ratio histogram    
    HDstarPi[c]->Divide(HDstarPiCCbar[c]);
    HDstarPi[c]->SetName(hname+"_CoarseRatio");
    HDstarPi[c]->Write();
    HDstarPi[c]=ExtrapolateRatio(HDstarPi[c],10);
    if(!HDstarPi[c])return 0;
    HDstarPi[c]->SetName(hname+"_Ratio");
    HDstarPi[c]->Write();

  }

  ///
  TH2F* H2RatioMap=ExtrapolateInAngle(HDstarPi);
  H2RatioMap->Write();

  DataFile->Write();
  DataFile->ls();  
  DataFile->SetWritable(0);
  gROOT->cd();

//   for(Int_t c=0;c<8;c++)
//     delete HDstarPi[c];
  

  delete H2RatioMap;

  return 1;
}

TH2F* DstPiAnalysis::ExtrapolateInAngle(TH1F* H1D[8]){  
  if(!H1D){
    cout<<"No 1D histos for AngleExtraPolation"<<endl;
    return 0;
  }
  
  //TString name=TString(H1D[0]->GetName())+"H2ExtrapolationMap";
  TString name="H2RatioMap";
  
  //using the above 1-D ratios construct a 2-D extrapolated ratio, the x direction will binned into NAngleBins bins
  TH2F* H2ExtrapolationMap=new TH2F(name,name,NAngleBins,-1.001,1.001,NNEWFITBINS,NEWFITMIN,NEWFITMAX);
  TF1* fxinter=NULL;
  Float_t fxmin[9];
  Float_t fxmax[9];
  Float_t fx0[9];
  Float_t fx1[9];
  Float_t fy0[9];
  Float_t fy1[9];
  Float_t fslope[9];
  Float_t fyint[9];
  for(Int_t by=1;by<=NNEWFITBINS;by++){
    //for bin in Y create a piece-wise function for interpolation in X
    fxmin[0]=-1;
    fxmax[0]=-1+.25/2.;
    fx0[0]=-1+.25/2.;
    fx1[0]=-1+.25/2.+.25;
    fy0[0]=H1D[7]->GetBinContent(by);
    fy1[0]=H1D[6]->GetBinContent(by);
    fslope[0]=(fy1[0]-fy0[0])/(fx1[0]-fx0[0]);
    fyint[0]=fy0[0]-fslope[0]*fx0[0];     
    //do the inner parts
    for(Int_t bx=1;bx<8;bx++){
      fxmin[bx]=-1+.25/2+.25*(bx-1);
      fxmax[bx]=-1+.25/2+.25*bx;
      fx0[bx]=-1+.25/2.+.25*(bx-1);
      fx1[bx]=-1+.25/2.+.25*bx;
      fy0[bx]=H1D[8-bx]->GetBinContent(by);
      fy1[bx]=H1D[8-bx-1]->GetBinContent(by);
      fslope[bx]=(fy1[bx]-fy0[bx])/(fx1[bx]-fx0[bx]);
      fyint[bx]=fy0[bx]-fslope[bx]*fx0[bx];  
    }        
    //do the end    
    fxmin[8]=1.-.25/2.;
    fxmax[8]=1.;
    fx0[8]=1-.25/2.;
    fx1[8]=1-.25/2.-.25;
    fy0[8]=H1D[0]->GetBinContent(by);
    fy1[8]=H1D[1]->GetBinContent(by);
    fslope[8]=(fy1[8]-fy0[8])/(fx1[8]-fx0[8]);
    fyint[8]=fy0[8]-fslope[8]*fx0[8];     
    
    sprintf(textlabel,"(%.3f<x&&x<%.3f)*(%.3f+%.3f*x)",fxmin[0],fxmax[0],fyint[0],fslope[0]);
    TString fform=textlabel;
    for(Int_t bx=1;bx<9;bx++){
      sprintf(textlabel,"+(%.3f<x&&x<%.3f)*(%.3f+%.3f*x)",fxmin[bx],fxmax[bx],fyint[bx],fslope[bx]);
      fform=fform+TString(textlabel);
    }    
    fxinter=new TF1((const char*)(TString("fxinter")+(long)by),(const char*)fform,-1,1);

    for(Int_t bx=1;bx<=NAngleBins;bx++){ 
      //use function fo this mass bin for interpolation                
      H2ExtrapolationMap->SetBinContent(bx,by,fxinter->Eval(H2ExtrapolationMap->GetXaxis()->GetBinCenter(bx)));
      H2ExtrapolationMap->SetBinError(bx,by,H1D[7-((bx-1)*8/(NAngleBins))]->GetBinError(by));
    }
      
    delete fxinter;    
  }



  return H2ExtrapolationMap;
}



TH1F* DstPiAnalysis::ExtrapolateRatio(TH1F*h,Int_t split){
  //This is the extrapolation of the coarse ratios in mass 
  if(!h||split<1||h->GetNbinsX()>100)return 0;
  TH1F* newh=new TH1F(TString(h->GetName())+"_Extrapolated",TString(h->GetName())+"_Extrapolated",h->GetNbinsX()*split,h->GetXaxis()->GetXmin(),h->GetXaxis()->GetXmax());


  Float_t binsize=h->GetBinWidth(1);
  Float_t lowxedge=h->GetXaxis()->GetXmin();
  Float_t fxmin[100];
  Float_t fxmax[100];
  Float_t fx0[100];
  Float_t fx1[100];
  Float_t fy0[100];
  Float_t fy1[100];
  Float_t fslope[100];
  Float_t fyint[100];
  
  //do the inner parts
  for(Int_t bx=0;bx<h->GetNbinsX()-1;bx++){
    fx0[bx]=lowxedge+binsize/2.+binsize*bx;
    fx1[bx]=lowxedge+binsize/2.+binsize*(bx+1);
    fxmin[bx]=fx0[bx];
    fxmax[bx]=fx1[bx];
    fy0[bx]=h->GetBinContent(bx+1);
    fy1[bx]=h->GetBinContent(bx+2);
    fslope[bx]=(fy1[bx]-fy0[bx])/(fx1[bx]-fx0[bx]);
    fyint[bx]=fy0[bx]-fslope[bx]*fx0[bx];  
  }       
  //change range on beginning piece
  fxmin[0]=h->GetXaxis()->GetXmin();
  //change range on ending piece
  fxmax[h->GetNbinsX()-2]=h->GetXaxis()->GetXmax();
    
  //create a piece-wise formula
  sprintf(textlabel,"(%.3f<x&&x<%.3f)*(%.3f+%.3f*x)",fxmin[0],fxmax[0],fyint[0],fslope[0]);
  TString fform=textlabel;
  for(Int_t bx=1;bx<h->GetNbinsX()-1;bx++){
    sprintf(textlabel,"+(%.3f<x&&x<%.3f)*(%.3f+%.3f*x)",fxmin[bx],fxmax[bx],fyint[bx],fslope[bx]);
    fform=fform+TString(textlabel);
  }    
  TF1* fxinter=new TF1((const char*)(TString(h->GetName())+"fxinter"),(const char*)fform,-1,1);

  for(Int_t bx=1;bx<=newh->GetNbinsX();bx++){ 
    newh->SetBinContent(bx,fxinter->Eval(newh->GetBinCenter(bx)));
    //newh->SetBinError(bx,h->GetBinError(bx));
    newh->SetBinError(bx,0);
    //cout<<bx<<" "<<newh->GetBinCenter(bx)<<" "<<fxinter->Eval(newh->GetBinCenter(bx))<<" "<<h->GetBinError(bx)<<endl;
  }

  delete fxinter;   
    
  return newh;
}


TH1F* DstPiAnalysis::AverageBins(TH1F*h,Int_t group){
  if(!h||group<1)return 0;
  TH1F* hclone=(TH1F*)h->Clone();
  Float_t add=0;

  for(Int_t b=0;b<h->GetNbinsX()/group;b++){
    add=0;
    for(Int_t g=1;g<=group;g++)
      add+=h->GetBinContent(b*group+g);

    for(Int_t g=1;g<=group;g++){
      hclone->SetBinContent(b*group+g,add);
      hclone->SetBinError(b*group+g,sqrt(hclone->GetBinContent(b*group+g)));
    }    
  }

  return hclone;
}



Int_t DstPiAnalysis::ApplyDstarAngleCorrection(TFile* DataFile,TFile* MCFile){
  cout<<"apply the correction to right sign ccbar monte carlo"<<endl;
  //MCFile->ls();
  MCFile->cd();
  MCFile->SetWritable(1);
  MCFile->rmdir("CorrHistos");
  MCFile->mkdir("CorrHistos");
  MCFile->cd("CorrHistos");
  
  //use the same ccbar root file to save the ratios
  for(Int_t c=0;c<8;c++){
    TString hname=TString("HDstarPiSlice")+(long)c;
    //Get the correction from the wrong sign data file
    HDATACCbarRatio[c]=(TH1F*)DataFile->Get(TString("Ratios/")+hname+"_Ratio");
    if(!HDATACCbarRatio[c]){cout<<"No "<< TString("Ratios/")+hname+"_Ratio"<<endl;return 0;}//it is already rebinned

    //Get the ccbar histos from the Right sign ccbar
    HDstarPiCCbar[c]=(TH1F*)MCFile->Get(TString("RawHistos/")+hname);
    if(!HDstarPiCCbar[c]){cout<<"No "<<TString("RawHistos/")+hname<<endl;return 0;}
    HDstarPiCCbar[c]=(TH1F*)CreateRebinnedDstPiHisto(HDstarPiCCbar[c],NNEWFITBINS,NEWFITMIN,NEWFITMAX);
    if(!HDstarPiCCbar[c]){return 0;}
    //subtract residual signal
    TH1F*HCCbarCorrHisto=(TH1F*)MCFile->Get("RawHistos/HCCbarCorrHisto");
    if(!HCCbarCorrHisto){cout<<"No RawHistos/HCCbarCorrHisto"<<endl;return 0;}
    HDstarPiCCbar[c]->Add(HCCbarCorrHisto,-1);
         
    HDstarPiCCbar[c]->Multiply(HDATACCbarRatio[c]);
    HDstarPiCCbar[c]->SetName(hname);
    HDstarPiCCbar[c]->Write();
    delete HDstarPiCCbar[c];
  }
  
  ///2-D Correction using map
  TH2* h2cmap=(TH2*)MCFile->Get("RawHistos/H2DstarPi");
  if(!h2cmap){cout<<"No RawHistos/H2DstarPi"<<endl;return 0;}

  //get the residual ccbar signal and subtract
  TH2*H2Residual=(TH2*)MCFile->Get("RawHistos/H2DCCbarCorrHisto");
  if(!H2Residual){cout<<"No H2Residual"<<endl;return 0;}
  h2cmap->Add(H2Residual,-1);

  //now rebin
  h2cmap=h2cmap->Rebin2D(GroupX,GroupY,"H2DstarPi_Rebin");
  TH2F H2RawCCbarMassVsAngle("H2RawCCbarMassVsAngle","H2RawCCbarMassVsAngle",NAngleBins,-1.001,1.001,NNEWFITBINS,NEWFITMIN,NEWFITMAX);//  
  for(Int_t bx=1;bx<=NAngleBins;bx++){
    for(Int_t by=1;by<=NNEWFITBINS;by++){      
      H2RawCCbarMassVsAngle.SetBinContent(bx,by,h2cmap->GetBinContent(bx,(160/GroupY)+by));//shift the start of copy in y
      H2RawCCbarMassVsAngle.SetBinError(bx,by,h2cmap->GetBinError(bx,(160/GroupY)+by));      
    }
  }
  delete h2cmap;

  //get the 2D ratio
  TH2*H2RatioMap=(TH2*)DataFile->Get("Ratios/H2RatioMap");
  if(!H2RatioMap){cout<<"No Ratios/H2RatioMap"<<endl;return 0;}
  H2RawCCbarMassVsAngle.Sumw2();
  H2RawCCbarMassVsAngle.Multiply(H2RatioMap);
  H2RawCCbarMassVsAngle.SetName("H2Map");
  H2RawCCbarMassVsAngle.Write();
    
  MCFile->Write();
  MCFile->SetWritable(0);  
  gROOT->cd();
  
  //MCFile->ls();
  return 1;
}



//   //create slices of the ratio from the 2-D ratio map to check the interpolation in mass and angle
//   TH1F* h2mapratioslice[8];
//   Float_t cadd=0;
//   Float_t cadderr=0;
//   Float_t nadded=0;
//   Float_t xbin=0;
//   Float_t anglemin=0;
//   Float_t anglemax=0;
  
//   Canvas.Clear();
//   Canvas.Divide(2,4);
//   for(Int_t c=7;c>=0;c--){
//     Canvas.cd(c+1);
//     h2mapratioslice[c]=new TH1F(TString("h2mapratioslice")+(long)c,TString("h2mapratioslice")+(long)c,NNEWFITBINS,NEWFITMIN,NEWFITMAX);
//     anglemin=1-.25*(c+1); 
//     anglemax=1-.25*c;

//     for(Int_t i=1;i<=NNEWFITBINS;i++){     
//       //for the ith mass bin make an average over the angle bins corresponding to c
//       cadd=0;
//       nadded=0;
//       cadderr=0;
//       for(Int_t x=1;x<=h2mapratio->GetNbinsX();x++){
// 	xbin=h2mapratio->GetXaxis()->GetBinCenter(x);
// 	if(anglemin<xbin&&xbin<anglemax){cadd+=h2mapratio->GetBinContent(x,i);cadderr+=h2mapratio->GetBinError(x,i);nadded++;}
//       }
//       h2mapratioslice[c]->SetBinContent(i,cadd/nadded);	
//       h2mapratioslice[c]->SetBinError(i,cadderr/nadded);
//     }
        
//     h2mapratioslice[c]->GetYaxis()->SetRangeUser(0,3);
//     h2mapratioslice[c]->Draw("pe");
//     cutline.DrawLine(NEWFITMIN,1,NEWFITMAX,1);    
//   }
//   Canvas.Print(filename);

  
//   for(Int_t c=7;c>=0;c--)
//     delete h2mapratioslice[c];
    



  /*
  //Apply 2-D correction working only in 2-D no interpolation, not precise enough
  //------------Wrong sign
  TH2* h2=(TH2*)HistFile.Get("H2DstarPi");
  if(!h2)return 0;
  h2->SetTitle("Wrong Sign Data");
  h2=h2->Rebin2D(GroupX,GroupY,"H2DstarPiCCbar_Rebin");
  Canvas.Clear();h2->Draw("colz");Canvas.Print(filename);
 

  TH2* h2c=(TH2*)HistFileCCbar.Get("H2DstarPi");
  if(!h2c)return 0;
  h2c->SetTitle("Wrong Sign c-cbar");
  h2c=h2c->Rebin2D(GroupX,GroupY,"H2DstarPiCCbar_Rebin");
  Canvas.Clear();h2c->Draw("colz");Canvas.Print(filename);


  TH2* h2ratio=(TH2*)HistFile.Get("H2DstarPi_Ratio");
  if(!h2ratio)return 0;
  h2ratio->SetTitle("Correction Factor Data/c-cbar");
  Canvas.Clear();h2ratio->Draw("colz");Canvas.Print(filename);
  

  //------------Right sign
  TH2* h2rsc=(TH2*)HistFileRSCCbar.Get("H2DstarPi");
  if(!h2rsc)return 0;
  h2rsc=h2rsc->Rebin2D(GroupX,GroupY,"H2DstarPiCCbar_Rebin");
  h2rsc->SetTitle("c-cbar Before Correction");
  Canvas.Clear();h2rsc->Draw("colz");Canvas.Print(filename); 


  TH2* h2rscorr=(TH2*)HistFileRSCCbar.Get("H2DstarPi_Corrected");
  if(!h2rscorr)return 0;
  h2rscorr->SetTitle("c-cbar After Correction");
  Canvas.Clear();h2rscorr->Draw("colz");Canvas.Print(filename);  
 

  TH2* h2rsraw=(TH2*)DataFile->Get("H2DstarPi");
  if(!h2rsraw)return 0;
  h2rsraw=h2rsraw->Rebin2D(GroupX,GroupY,"H2DstarPi_Rebin");
  h2rsraw->SetTitle("Data Before Subtraction");
  Canvas.Clear();h2rsraw->Draw("colz");Canvas.Print(filename);


  //-----subtract the ccbar
  TH2*h2rssig=(TH2*)h2rsraw->Clone();
  //h2rscorr->Scale(h2rsraw->Integral(1,h2rsraw->GetNbinsX(),1,60)/h2rscorr->Integral(1,h2rscorr->GetNbinsX(),1,60));
  Scale2DMass(h2rsraw,h2rscorr); 
  h2rssig->Add(h2rscorr,-1);
  h2rssig->SetTitle("Data After Subtraction");
  Canvas.Clear();h2rssig->Draw("colz");Canvas.Print(filename);
  TH1*h2rssigM=h2rssig->ProjectionY();
  Canvas.Clear();h2rssigM->Draw("pe");Canvas.Print(filename);
  

  delete h2;
  delete h2c; 
  delete h2rsc;
  delete h2rsraw;
  delete h2rssigM;

  ///Do the 2-D correction:
  TH2*h2=(TH2*)DataFile->Get("H2DstarPi");
  if(!h2)return 0;
  h2=h2->Rebin2D(GroupX,GroupY,"H2DstarPi_Rebin");
  h2->Write(); 
  TH2*h2c=(TH2*)MCFile->Get("H2DstarPi");
  if(!h2c)return 0;
  h2c=h2c->Rebin2D(GroupX,GroupY,"H2DstarPi_Rebin");
  h2c->Write();
  TH2* h2ratio=(TH2*)h2c->Clone();
  h2ratio->Sumw2();
  h2ratio->Divide(h2);
  h2ratio->SetName("H2DstarPi_Ratio");
  h2ratio->Write();
  delete h2ratio;
  delete h2c;
  delete h2;


//   //Apply 2-D correction
//   TH2* h2=(TH2*)DataFile->Get("H2DstarPi_Ratio");
//   if(!h2)return 0;
//   TH2* h2c=(TH2*)MCFile->Get("H2DstarPi");
//   h2c=h2c->Rebin2D(GroupX,GroupY,"H2DstarPi_Rebin");
//   if(!h2c)return 0;
//   TH2* h2corr=(TH2*)h2c->Clone();
//   h2corr->SetName("H2DstarPi_Corrected");
//   h2corr->Multiply(h2);
//   h2corr->Write();    
//   delete h2c;
//   delete h2corr;

  
  */


