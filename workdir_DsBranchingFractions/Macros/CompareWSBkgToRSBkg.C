#include "../tmp/Linux26SL5_i386_gcc412/DRecoilToolsUser/NtupleCode/DRecoEvtAnalysis.hh"
#include "../tmp/Linux26SL5_i386_gcc412/DRecoilToolsUser/NtupleCode/Fits.hh"
#include <TH1F.h>
#include <TH2F.h>


#define NXR 4
void CompareTrueWSBkgandRSBkgvsnXR(){//creates ratio of True backgrounds for systematic

  TFile fileRS("DsTolnu/SP1005Skim/DsReco/Final/HistosForFit.root","read");
  fileRS.ls();
  TFile fileWS("DsTolnu/SP1005Skim/DsReco/FinalWS/HistosForFit.root","read"); 
  fileWS.ls();
  gROOT->cd();



  TH2F* H2RSBkg=(TH2F*)(fileRS.Get("HDsMass2DNoSig"));
  TH2F* H2WSBkg=(TH2F*)(fileWS.Get("HDsMass2DNoSig"));


  TH1D* HRS[NXR];
  TH1D* HWS[NXR];
  for(Int_t i=0;i<NXR;i++){
    HRS[i]=H2RSBkg->ProjectionY(TString("HDsMassNoSigRS_nXR")+(long)i,i+1,i+1);
    HWS[i]=H2WSBkg->ProjectionY(TString("HDsMassNoSigWS_nXR")+(long)i,i+1,i+1);
  }


//   TH1F* HRS[NXR];
//   TH1F* HWS[NXR];
//   for(Int_t i=1;i<NXR;i++){
//     HRS[i]=(TH1F*)(fileRS.Get(TString("HDsMassNoSig_nXR")+(long)i)->Clone());
//     HWS[i]=(TH1F*)(fileWS.Get(TString("HDsMassNoSig_nXR")+(long)i)->Clone());
//     if(!HRS[i]||!HWS[i]){cout<<" Not found histo"<<endl; return;}
//     cout<<HRS[i]->GetName()<<" "<<HWS[i]->GetName()<<endl;
//   }


  TH1F* HWSClone[NXR];
  TCanvas C("C");  
  C.Print("DsTolnu/SP1005Skim/DsReco/FinalWS/CompareTrueWSBkgandRSBkgvsnXR.ps[");
  C.Clear();
  C.Divide(2,2);
  for(Int_t i=1;i<NXR;i++){
    C.cd(i+1);
    HRS[i]->GetYaxis()->SetRangeUser(0,HRS[i]->GetBinContent(HRS[i]->GetMaximumBin())*1.2);
    HRS[i]->GetXaxis()->SetTitle("m(Ds)");
    HRS[i]->SetTitle("");
    HRS[i]->Draw("pe");

    HWS[i]->Scale(HRS[i]->Integral()/HWS[i]->Integral());
    HWSClone[i]=new TH1F(TString("HWSClone")+(long)i,"",
			 HWS[i]->GetXaxis()->GetNbins(),HWS[i]->GetXaxis()->GetXmin(),HWS[i]->GetXaxis()->GetXmax());
    for(Int_t r=1;r<=HWS[i]->GetXaxis()->GetNbins();r++)
      HWSClone[i]->SetBinContent(r,HWS[i]->GetBinContent(r));
    HWSClone[i]->SetLineColor(2);
    HWSClone[i]->Draw("same");
  
  }  
  C.Print("DsTolnu/SP1005Skim/DsReco/FinalWS/CompareTrueWSBkgandRSBkgvsnXR.ps");


  TLine line;
  //Make Ratio
  
  //first double the bin size to get better statistical error
  TH1F* HWSDouble[NXR];
  TH1F* HRSDouble[NXR];
  for(Int_t i=1;i<NXR;i++){
    HWSDouble[i]=(TH1F*)HWS[i]->Rebin(2,TString("HWSDouble")+(long)i);
    HRSDouble[i]=(TH1F*)HRS[i]->Rebin(2,TString("HRSDouble")+(long)i);    
  }
    
  TH1F* HRSRatioDouble[NXR];
  C.Clear();
  C.Divide(2,2);
  for(Int_t i=1;i<NXR;i++){
    C.cd(i+1);
    HRSRatioDouble[i]=(TH1F*)HRSDouble[i]->Clone(TString("HRSRatioDouble")+(long)i);
    HRSRatioDouble[i]->SetLineColor(2);
    HRSRatioDouble[i]->Sumw2();
    HRSRatioDouble[i]->Divide(HWSDouble[i]);
    HRSRatioDouble[i]->GetYaxis()->SetRangeUser(.5,1.5);
    HRSRatioDouble[i]->Draw("pe hist");
    line.DrawLine(HWS[i]->GetXaxis()->GetXmin(),1,HWS[i]->GetXaxis()->GetXmax(),1);
  }  
  C.Print("DsTolnu/SP1005Skim/DsReco/FinalWS/CompareTrueWSBkgandRSBkgvsnXR.ps");
  //change the bin size again to match the original binning
  TH1F* HRSRatio[NXR];
  for(Int_t i=1;i<NXR;i++){
     HRSRatio[i]=(TH1F*)HRS[i]->Clone(TString("HRSRatioDouble")+(long)i);
     if(i==1)
       for(Int_t b=1;b<=HRS[i]->GetXaxis()->GetNbins();b++){
	 HRSRatio[i]->SetBinContent(b,HRSRatioDouble[i]->GetBinContent((b-1)/2 + 1));
	 HRSRatio[i]->SetBinError(b,HRSRatioDouble[i]->GetBinError((b-1)/2 + 1));
       }
     else
       for(Int_t b=1;b<=HRS[i]->GetXaxis()->GetNbins();b++){
	 HRSRatio[i]->SetBinContent(b,1);
	 HRSRatio[i]->SetBinError(b,0);
       }
  }
  C.Clear();
  C.Divide(2,2);
  for(Int_t i=1;i<NXR;i++){
    C.cd(i+1);
    HRSRatio[i]->GetYaxis()->SetRangeUser(.5,1.5);
    HRSRatio[i]->Draw("pe hist");
    line.DrawLine(HWS[i]->GetXaxis()->GetXmin(),1,HWS[i]->GetXaxis()->GetXmax(),1);
  }
  C.Print("DsTolnu/SP1005Skim/DsReco/FinalWS/CompareTrueWSBkgandRSBkgvsnXR.ps");

  //save corrections
  TFile file("DsTolnu/SP1005Skim/DsReco/FinalWS/WSnXRBkgCorr.root","recreate");
  for(Int_t i=1;i<NXR;i++){
    //This is the correction that should multiply the RS background shape for each nXR
    HRSRatio[i]->SetName(TString("HWSBkgCorr")+(long)i);
    HRSRatio[i]->Write();
  }
  file.ls();
  file.Close();

  C.Print("DsTolnu/SP1005Skim/DsReco/FinalWS/CompareTrueWSBkgandRSBkgvsnXR.ps]");
}

void CreatenXR0Correction(){//creates correction factors for nXR=0

  TFile fileRS("DsTolnu/SP1005Skim/DsReco/Final/HistosForFit.root","read");
  fileRS.ls();
  TFile fileWS("DsTolnu/SP1005Skim/DsReco/FinalWS/HistosForFit.root","read"); 
  fileWS.ls();
  gROOT->cd();

  TH2F* H2RSBkg=(TH2F*)(fileRS.Get("HDsMass2DNoSig"));
  TH2F* H2WSBkg=(TH2F*)(fileWS.Get("HDsMass2DNoSig"));


  TH1D* HRS[NXR];
  TH1D* HWS[NXR];
  for(Int_t i=0;i<NXR;i++){
    HRS[i]=H2RSBkg->ProjectionY(TString("HDsMassNoSigRS_nXR")+(long)i,i+1,i+1);
    HWS[i]=H2WSBkg->ProjectionY(TString("HDsMassNoSigWS_nXR")+(long)i,i+1,i+1);
  }


  TCanvas C("C");  

  C.Print("DsTolnu/SP1005Skim/DsReco/FinalWS/WSnXR0Correction.ps[");

  //show the wrong sign backgrounds 
  C.Clear();
  HWS[1]->Scale(1./HWS[1]->Integral());
  HWS[1]->SetLineColor(1);
  HWS[1]->SetStats(0);
  HWS[1]->GetXaxis()->SetTitle("m(D_{s})  (GeV/c^{2})");
  HWS[1]->Draw("hist ");
  for(Int_t i=2;i<NXR;i++){
    HWS[i]->SetLineColor(i);
    //HWS[i]->SetMarkerColor(i);
    HWS[i]->Scale(1./HWS[i]->Integral());
    HWS[i]->Draw("same hist");
  }
  C.Print("DsTolnu/SP1005Skim/DsReco/FinalWS/WSnXR0Correction.ps");

  //construct the average of the Wrong Sign backgrounds to be used for the nXR=0 bkg model
  HWS[0]->Clear();
  HWS[0]->Reset();
  TH1F* HWS0Clone=(TH1F*)HWS[0]->Clone("HWS0Clone");
  HWS0Clone->Sumw2();
  for(Int_t i=0;i<NXR;i++)
    HWS0Clone->Add(HWS[i]);
  HWS0Clone->Scale(1./HWS0Clone->Integral());  
  HRS[0]->Sumw2();
  HRS[0]->Scale(1./HRS[0]->Integral());
  C.Clear();
  HRS[0]->SetStats(0);
  HRS[0]->GetXaxis()->SetTitle("m(D_{s})  (GeV/c^{2})");
  HRS[0]->Draw("hist");
  HWS0Clone->SetLineColor(2);
  HWS0Clone->SetMarkerColor(2);
  HWS0Clone->Draw("pe hist same");
  C.Print("DsTolnu/SP1005Skim/DsReco/FinalWS/WSnXR0Correction.ps");

  //construct correction ratio: should be a ratio to Multiply the average of the wrong sign backgrounds 
  TH1F* HRatio=(TH1F*)HRS[0]->Clone("HRatio");
  HRatio->Divide(HWS0Clone);
  for(Int_t b=1;b<=HWS0Clone->GetXaxis()->GetNbins();b++)
    if(HRatio->GetBinContent(b)<.01||HRatio->GetBinContent(b)>30.){
      HRatio->SetBinContent(b,1);
      HRatio->SetBinError(b,.001);
    }
  HRatio->SetStats(0);
  C.Clear();
  HRatio->GetYaxis()->SetRangeUser(0.,2);
  HRatio->GetXaxis()->SetTitle("m(D_{s})  (GeV/c^{2})");
  HRatio->Draw("pe");
  TLine line;
  line.DrawLine(HRatio->GetXaxis()->GetXmin(),1,HRatio->GetXaxis()->GetXmax(),1);
  C.Print("DsTolnu/SP1005Skim/DsReco/FinalWS/WSnXR0Correction.ps");


  //construct the average of the Wrong Sign backgrounds to be used for the nXR=1 bkg model
  TH1F* HWS1Clone=(TH1F*)HWS[0]->Clone("HWS1Clone");//get an empty histogram
  HWS1Clone->Sumw2();
  for(Int_t i=0;i<NXR;i++)
    HWS1Clone->Add(HWS[i]);
  HWS1Clone->Scale(1./HWS1Clone->Integral());  
  C.Clear();
  HRS[1]->Sumw2();
  HRS[1]->Scale(1./HRS[1]->Integral());
  HRS[1]->SetStats(0);
  HRS[1]->GetXaxis()->SetTitle("m(D_{s})  (GeV/c^{2})");
  HRS[1]->Draw("hist");
  HWS1Clone->SetLineColor(2);
  HWS1Clone->SetMarkerColor(2);
  HWS1Clone->Draw("pe same hist");
  C.Print("DsTolnu/SP1005Skim/DsReco/FinalWS/WSnXR0Correction.ps");

  //construct correction ratio: should be a ratio to Multiply the average of the wrong sign backgrounds 
  TH1F* HRatio1=(TH1F*)HRS[1]->Clone("HRatio1");
  HRatio1->Divide(HWS1Clone);
  for(Int_t b=1;b<=HWS1Clone->GetXaxis()->GetNbins();b++)
    if(HRatio1->GetBinContent(b)<.01||HRatio1->GetBinContent(b)>30.){
      HRatio1->SetBinContent(b,1);
      HRatio1->SetBinError(b,.001);
    }
  HRatio1->SetStats(0);
  C.Clear();
  HRatio1->GetYaxis()->SetRangeUser(0.,2);
  HRatio1->GetXaxis()->SetTitle("m(D_{s})  (GeV/c^{2})");
  HRatio1->Draw("pe");
  line.DrawLine(HRatio->GetXaxis()->GetXmin(),1,HRatio->GetXaxis()->GetXmax(),1);
  C.Print("DsTolnu/SP1005Skim/DsReco/FinalWS/WSnXR0Correction.ps");


  C.Print("DsTolnu/SP1005Skim/DsReco/FinalWS/WSnXR0Correction.ps]");


  //save correction
  TFile file("DsTolnu/SP1005Skim/DsReco/FinalWS/WSnXR0Correction.root","recreate");
  HRatio->SetName("HnXR0Correction");
  HRatio->Write();
  HWS0Clone->SetName("HnXR0Avg");
  HWS0Clone->Write();
  HRatio1->SetName("HnXR1Correction");
  HRatio1->Write();
  HWS1Clone->SetName("HnXR1Avg");
  HWS1Clone->Write();
  file.ls();
  file.Close();

  fileRS.Close();
  fileWS.Close();
}

/////////////////////////////--------------------------------------------------


void CompareWSBkgRSBkg(){//shows compareson of total true backgrounds

  TFile fileRS("DsTolnu/SP1005Skim/DsReco/Final/HistosForFit.root");
  TFile fileWS("DsTolnu/SP1005Skim/DsReco/FinalWS/HistosForFit.root");
  gROOT->cd();

  TH1F* HRS=(TH1F*)fileRS.Get("HDsMassNoSig");
  TH1F* HWS=(TH1F*)fileWS.Get("HDsMassNoSig");
  TCanvas C("C");
  C.Print("WSToRSBkgComparison.ps[");
  
  C.Clear();
  HRS->GetYaxis()->SetRangeUser(0,HRS->GetBinContent(HRS->GetMaximumBin())*1.2);
  HRS->GetXaxis()->SetTitle("m(Ds)");
  HRS->SetTitle("");
  HRS->SetStats(0);
  HRS->Draw("pe");

  HWS->Scale(HRS->Integral()/HWS->Integral());
  HWS->SetLineColor(2);
  HWS->Draw("same hist");
  C.Print("WSToRSBkgComparison.ps");  

  //show the ratio WS/RS
  TH1F*HWSC=(TH1F*)HWS->Clone("HWSTotalBkgCorr");
  HWSC->Divide(HRS);
  C.Clear();
  HWSC->SetStats(0);
  HWSC->Draw("pe hist");
  TLine lin;
  lin.DrawLine(HWSC->GetXaxis()->GetXmin(),1,HWSC->GetXaxis()->GetXmax(),1);
  C.Print("WSToRSBkgComparison.ps");

  C.Print("WSToRSBkgComparison.ps]");

  //save correction defined as the ratio of the RS/WS !!
  TH1F*HWSCorr=(TH1F*)HRS->Clone("HWSTotalBkgCorr");
  HWSCorr->Divide(HWS);  
  TFile file("DsTolnu/SP1005Skim/DsReco/FinalWS/WSTotalBkgCorr.root","recreate");
  HWSCorr->SetName("HWSTotalBkgCorr");
  HWSCorr->Write();
  file.ls();
  file.Close();

}


// void CompareWSBkgRSFittedBkg(){

//   TFile fileRS("DsTolnu/SP1005Skim/DsReco/Final/HistosForFit.root");
//   fileRS.ls();
//   TFile fileWS("DsTolnu/SP1005Skim/DsReco/FinalWS/DsMassWSRooFitWeight.root","update"); 
//   fileWS.ls();
//   gROOT->cd();

//   TH1F* HRS=(TH1F*)fileRS.Get("HDsMassNoSig");
//   TH1F* HWS=(TH1F*)fileWS.Get("HDsMass");
//   if(!HRS||!HWS){cout<<" Not found histo"<<endl; return;}
//   TCanvas C("C");
//   C.Print("DsTolnu/SP1005Skim/DsReco/FinalWS/WSToRSBkgComparisonFitted.ps[");  

//   HRS->GetYaxis()->SetRangeUser(0,HRS->GetBinContent(HRS->GetMaximumBin())*1.2);
//   HRS->GetXaxis()->SetTitle("m(Ds)");
//   HRS->SetTitle("");
//   C.Clear();
//   HRS->Draw("pe");
//   HWS->Scale(HRS->Integral()/HWS->Integral());
//   TH1F HWSClone("HWSClone","",HWS->GetXaxis()->GetNbins(),HWS->GetXaxis()->GetXmin(),HWS->GetXaxis()->GetXmax());
//   for(Int_t i=1;i<=HWS->GetXaxis()->GetNbins();i++)
//     HWSClone.SetBinContent(i,HWS->GetBinContent(i));
//   HWSClone.SetLineColor(2);
//   HWSClone.Draw("same");
//   C.Print("DsTolnu/SP1005Skim/DsReco/FinalWS/WSToRSBkgComparisonFitted.ps");


//   TLine line;
//   //Make difference
//   TH1F HWSDiff(TString("HWSDiff"),"",HWS->GetXaxis()->GetNbins(),HWS->GetXaxis()->GetXmin(),HWS->GetXaxis()->GetXmax());
//   for(Int_t r=1;r<=HWS->GetXaxis()->GetNbins();r++)
//     HWSDiff.SetBinContent(r,HWS->GetBinContent(r));
//   HWSDiff.SetLineColor(2);
//   HWSDiff.Sumw2();
//   HWSDiff.Add(HRS,-1);
//   HWSDiff.Divide(HRS);
//   C.Clear();
//   HWSDiff.GetYaxis()->SetRangeUser(-.1,.1);
//   HWSDiff.Draw("pe");
//   line.DrawLine(HWS->GetXaxis()->GetXmin(),0,HWS->GetXaxis()->GetXmax(),0);
//   C.Print("DsTolnu/SP1005Skim/DsReco/FinalWS/WSToRSBkgComparisonFitted.ps");


//   //Make Ratio
//   TH1F HWSRatio(TString("HWSRatio"),"",HWS->GetXaxis()->GetNbins(),HWS->GetXaxis()->GetXmin(),HWS->GetXaxis()->GetXmax());
//   for(Int_t r=1;r<=HWS->GetXaxis()->GetNbins();r++)
//     HWSRatio.SetBinContent(r,HWS->GetBinContent(r));
//   HWSRatio.SetLineColor(2);
//   HWSRatio.Sumw2();  
//   HWSRatio.Divide(HRS);
//   C.Clear();
//   HWSRatio.GetYaxis()->SetRangeUser(.8,1.2);
//   HWSRatio.Draw("pe");
//   line.DrawLine(HWS->GetXaxis()->GetXmin(),1,HWS->GetXaxis()->GetXmax(),1);
//   C.Print("DsTolnu/SP1005Skim/DsReco/FinalWS/WSToRSBkgComparisonFitted.ps");

//   C.Print("DsTolnu/SP1005Skim/DsReco/FinalWS/WSToRSBkgComparisonFitted.ps]");

//   //correct the background shape
//   fileWS.cd();
//   fileWS.Delete("HDsMass_Corr;1");
//   HRS->SetName("HDsMass_Corr");  
//   HRS->Write();
//   fileWS.Close();

// }

// void CompareWSBkgRSFittedBkgvsnXR(){

//   TFile fileRS("DsTolnu/SP1005Skim/DsReco/Final/HistosForFit.root","read");
//   fileRS.ls();
//   TFile fileWS("DsTolnu/SP1005Skim/DsReco/FinalWS/DsMassWSRooFitvsnXR.root","read"); 
//   fileWS.ls();
//   gROOT->cd();

//   TH1F* HRS[NXR];
//   TH1F* HWS[NXR];
//   for(Int_t i=0;i<NXR;i++){
//     HRS[i]=(TH1F*)(fileRS.Get(TString("HDsMassNoSig_nXR")+(long)i)->Clone());
//     HWS[i]=(TH1F*)(fileWS.Get(TString("HDsMass_nXR")+(long)0)->Clone());
//     if(!HRS[i]||!HWS[i]){cout<<" Not found histo"<<endl; return;}
//     cout<<HRS[i]->GetName()<<" "<<HWS[i]->GetName()<<endl;
//   }


//   TH1F* HWSClone[NXR];
//   TCanvas C("C");  
//   C.Print("DsTolnu/SP1005Skim/DsReco/FinalWS/WSToRSBkgComparisonFittedvsnXR.ps[");
//   C.Clear();
//   C.Divide(2,2);
//   for(Int_t i=0;i<NXR;i++){
//     C.cd(i+1);
//     HRS[i]->GetYaxis()->SetRangeUser(0,HRS[i]->GetBinContent(HRS[i]->GetMaximumBin())*1.2);
//     HRS[i]->GetXaxis()->SetTitle("m(Ds)");
//     HRS[i]->SetTitle("");
//     HRS[i]->Draw("pe");

//     HWS[i]->Scale(HRS[i]->Integral()/HWS[i]->Integral());
//     HWSClone[i]=new TH1F(TString("HWSClone")+(long)i,"",
// 			 HWS[i]->GetXaxis()->GetNbins(),HWS[i]->GetXaxis()->GetXmin(),HWS[i]->GetXaxis()->GetXmax());
//     for(Int_t r=1;r<=HWS[i]->GetXaxis()->GetNbins();r++)
//       HWSClone[i]->SetBinContent(r,HWS[i]->GetBinContent(r));
//     HWSClone[i]->SetLineColor(2);
//     HWSClone[i]->Draw("same");
  
//   }  
//   C.Print("DsTolnu/SP1005Skim/DsReco/FinalWS/WSToRSBkgComparisonFittedvsnXR.ps");

//   TLine line;
//   //Make difference
//   TH1F* HRSClone[NXR];
//   C.Clear();
//   C.Divide(2,2);
//   for(Int_t i=0;i<NXR;i++){
//     C.cd(i+1);
//     HRSClone[i]=new TH1F(TString("HRSClone")+(long)i,"",
// 			 HWS[i]->GetXaxis()->GetNbins(),HWS[i]->GetXaxis()->GetXmin(),HWS[i]->GetXaxis()->GetXmax());
//     for(Int_t r=1;r<=HWS[i]->GetXaxis()->GetNbins();r++)
//       HRSClone[i]->SetBinContent(r,HRS[i]->GetBinContent(r));
//     HRSClone[i]->SetLineColor(2);
//     HRSClone[i]->Sumw2();
//     HRSClone[i]->Add(HWS[i],-1);
//     HRSClone[i]->GetYaxis()->SetRangeUser(-.1,.1);
//     HRSClone[i]->Divide(HRS[i]);
//     HRSClone[i]->Draw("pe");
//     line.DrawLine(HWS[i]->GetXaxis()->GetXmin(),0,HWS[i]->GetXaxis()->GetXmax(),0);
//   }  
//   C.Print("DsTolnu/SP1005Skim/DsReco/FinalWS/WSToRSBkgComparisonFittedvsnXR.ps");



//   C.Print("DsTolnu/SP1005Skim/DsReco/FinalWS/WSToRSBkgComparisonFittedvsnXR.ps]");
// }


// void CompareWSBkgRSFittedBkgvsnXRvsnXT(){

//   TFile fileRS("DsTolnu/SP1005Skim/DsReco/Final/HistosForFit.root","read");
//   fileRS.ls();
//   TFile fileWS("DsTolnu/SP1005Skim/DsReco/FinalWS/DsMassWSRooFitvsnXRvsnXT.root","read"); 
//   fileWS.ls();
//   gROOT->cd();

//   TH1F* HRS[NXR];
//   TH1F* HWS[NXR];
//   for(Int_t i=0;i<NXR;i++){
//     HRS[i]=(TH1F*)(fileRS.Get(TString("HDsMassNoSig_nXR")+(long)i)->Clone());
//     HWS[i]=(TH1F*)(fileWS.Get(TString("HDsMass_nXR")+(long)0)->Clone());
//     if(!HRS[i]||!HWS[i]){cout<<" Not found histo"<<endl; return;}
//     cout<<HRS[i]->GetName()<<" "<<HWS[i]->GetName()<<endl;
//   }


 

//   TH1F* HWSClone[NXR];
//   TCanvas C("C");  
//   C.Print("DsTolnu/SP1005Skim/DsReco/FinalWS/WSToRSBkgComparisonFittedvsnXRvsnXT.ps[");
//   C.Clear();
//   C.Divide(2,2);
//   for(Int_t i=0;i<NXR;i++){
//     C.cd(i+1);
//     HRS[i]->GetYaxis()->SetRangeUser(0,HRS[i]->GetBinContent(HRS[i]->GetMaximumBin())*1.2);
//     HRS[i]->GetXaxis()->SetTitle("m(Ds)");
//     HRS[i]->SetTitle("");
//     HRS[i]->Draw("pe");

//     HWS[i]->Scale(HRS[i]->Integral()/HWS[i]->Integral());
//     HWSClone[i]=new TH1F(TString("HWSClone")+(long)i,"",
// 			 HWS[i]->GetXaxis()->GetNbins(),HWS[i]->GetXaxis()->GetXmin(),HWS[i]->GetXaxis()->GetXmax());
//     for(Int_t r=1;r<=HWS[i]->GetXaxis()->GetNbins();r++)
//       HWSClone[i]->SetBinContent(r,HWS[i]->GetBinContent(r));
//     HWSClone[i]->SetLineColor(2);
//     HWSClone[i]->Draw("same");
  
//   }  
//   C.Print("DsTolnu/SP1005Skim/DsReco/FinalWS/WSToRSBkgComparisonFittedvsnXRvsnXT.ps");


//   TLine line;
//   //Make difference
//   TH1F* HRSClone[NXR];

//   C.Clear();
//   C.Divide(2,2);
//   for(Int_t i=0;i<NXR;i++){
//     C.cd(i+1);
//     HRSClone[i]=new TH1F(TString("HRSClone")+(long)i,"",
// 			 HWS[i]->GetXaxis()->GetNbins(),HWS[i]->GetXaxis()->GetXmin(),HWS[i]->GetXaxis()->GetXmax());
//     for(Int_t r=1;r<=HWS[i]->GetXaxis()->GetNbins();r++)
//       HRSClone[i]->SetBinContent(r,HRS[i]->GetBinContent(r));
//     HRSClone[i]->SetLineColor(2);
//     HRSClone[i]->Sumw2();
//     HRSClone[i]->Add(HWS[i],-1);
//     HRSClone[i]->GetYaxis()->SetRangeUser(-.1,.1);
//     HRSClone[i]->Divide(HRS[i]);
//     HRSClone[i]->Draw("pe");
//     line.DrawLine(HWS[i]->GetXaxis()->GetXmin(),0,HWS[i]->GetXaxis()->GetXmax(),0);
//   }  
//   C.Print("DsTolnu/SP1005Skim/DsReco/FinalWS/WSToRSBkgComparisonFittedvsnXRvsnXT.ps");


//   C.Print("DsTolnu/SP1005Skim/DsReco/FinalWS/WSToRSBkgComparisonFittedvsnXRvsnXT.ps]");

// }
