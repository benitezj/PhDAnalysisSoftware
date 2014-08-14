


void BFRatioCrossChecks(){
 
  TFile FDPiMC("DCharged/DChargedPi/MC/SP-9015/KPiPi/Ntuple.root");
  TTree*TDPiMC=(TTree*)FDPiMC.Get("Ntuple");
 
  TFile FDPiData("DCharged/DChargedPi/KPiPi/Ntuple.root");
  TTree*TDPiData=(TTree*)FDPiData.Get("Ntuple");

  TFile FDstPiMC("DstarCharged/DstarPi/MC/Signal/D2460/Kpi/Ntuple.root");
  TTree*TDstPiMC=(TTree*)FDstPiMC.Get("Ntuple");

  TFile FDstPiData("DstarCharged/DstarPi/Data/Kpi/Ntuple.root");
  TTree*TDstPiData=(TTree*)FDstPiData.Get("Ntuple");


  
  TCanvas C("C");

  C.Print("BFRatioCrossChecks.ps[");
  
  ///plot the DPi MC Cuts
  TH1F HDpiMCDMass("HDpiMCDMass","",200,1.8,1.9);
  HDpiMCDMass.GetXaxis()->SetTitle("D^{+} Mass");
  TDPiMC->Draw("dmass>>HDpiMCDMass");
  TH1F HDpiMCFlight("HDpiMCFlight","",250,-5,20);
  HDpiMCFlight.GetXaxis()->SetTitle("D^{+} Flight Significance");
  TDPiMC->Draw("dflights>>HDpiMCFlight");
  TH1F HDpiMCPstar("HDpiMCPstar","",250,2.5,5);
  HDpiMCPstar.GetXaxis()->SetTitle("p*(D#pi)");
  TDPiMC->Draw("xpstar>>HDpiMCPstar");
  TH1F HDpiMCDCos("HDpiMCDCos","",200,-1,1);
  HDpiMCDCos.GetXaxis()->SetTitle("cos(#theta_{#pi})");
  TDPiMC->Draw("dcostheta>>HDpiMCDCos");

  //plot the DPi Data cuts
  TH1F HDpiDataDMass("HDpiDataDMass","",200,1.8,1.9);
  HDpiDataDMass.GetXaxis()->SetTitle("D^{+} Mass");
  TDPiData->Draw("dmass>>HDpiDataDMass");
  TH1F HDpiDataFlight("HDpiDataFlight","",250,-5,20);
  HDpiDataFlight.GetXaxis()->SetTitle("D^{+} Flight Significance");
  TDPiData->Draw("dflights>>HDpiDataFlight");
  TH1F HDpiDataPstar("HDpiDataPstar","",250,2.5,5);
  HDpiDataPstar.GetXaxis()->SetTitle("p*(D#pi)");
  TDPiData->Draw("xpstar>>HDpiDataPstar");
  TH1F HDpiDataDCos("HDpiDataDCos","",200,-1,1);
  HDpiDataDCos.GetXaxis()->SetTitle("cos(#theta_{#pi})");
  TDPiData->Draw("dcostheta>>HDpiDataDCos");
  
  C.Clear();
  C.Divide(2,2);
  C.cd(1);
  HDpiMCDMass.Draw();
  C.cd(2);
  HDpiDataDMass.Draw();
  C.Print("BFRatioCrossChecks.ps");

  C.Clear();
  C.Divide(2,2);
  C.cd(1);
  HDpiMCFlight.Draw();
  C.cd(2);
  HDpiDataFlight.Draw();
  C.Print("BFRatioCrossChecks.ps");

  C.Clear();
  C.Divide(2,2);
  C.cd(1);
  HDpiMCPstar.Draw();
  C.cd(2);
  HDpiDataPstar.Draw();
  C.Print("BFRatioCrossChecks.ps");

  C.Clear();
  C.Divide(2,2);
  C.cd(1);
  HDpiMCDCos.Draw();
  C.cd(2);
  HDpiDataDCos.Draw();
  C.Print("BFRatioCrossChecks.ps");
 



  //plot the D*pi MC cuts
  TH1F HDstpiMCDMass("HDstpiMCDMass","",200,1.8,1.9);
  HDstpiMCDMass.GetXaxis()->SetTitle("D^{0} Mass");
  TDstPiMC->Draw("d0mass>>HDstpiMCDMass");
  TH1F HDstpiMCDeltaM("HDstpiMCDeltaM","",100,.140,.15);
  HDstpiMCDeltaM.GetXaxis()->SetTitle("D* DeltaM");
  TDstPiMC->Draw("dstardeltam>>HDstpiMCDeltaM");
  TH1F HDstpiMCPstar("HDstpiMCPstar","",250,2.5,5);
  HDstpiMCPstar.GetXaxis()->SetTitle("p*(D*#pi)");
  TDstPiMC->Draw("dstpipstar>>HDstpiMCPstar");
  TH1F HDstpiMCDCos("HDstpiMCDCos","",200,-1,1);
  HDstpiMCDCos.GetXaxis()->SetTitle("cos(#theta_{#pi})");
  TDstPiMC->Draw("-dstarcostheta>>HDstpiMCDCos");

  //plot the D*pi Data cuts
  TH1F HDstpiDataDMass("HDstpiDataDMass","",200,1.8,1.9);
  HDstpiDataDMass.GetXaxis()->SetTitle("D^{0} Mass");
  TDstPiData->Draw("d0mass>>HDstpiDataDMass");
  TH1F HDstpiDataDeltaM("HDstpiDataDeltaM","",100,.140,.15);
  HDstpiDataDeltaM.GetXaxis()->SetTitle("D* DeltaM");
  TDstPiData->Draw("dstardeltam>>HDstpiDataDeltaM");
  TH1F HDstpiDataPstar("HDstpiDataPstar","",250,2.5,5);
  HDstpiDataPstar.GetXaxis()->SetTitle("p*(D*#pi)");
  TDstPiData->Draw("dstpipstar>>HDstpiDataPstar");
  TH1F HDstpiDataDCos("HDstpiDataDCos","",200,-1,1);
  HDstpiDataDCos.GetXaxis()->SetTitle("cos(#theta_{#pi})");
  TDstPiData->Draw("-dstarcostheta>>HDstpiDataDCos");


  C.Clear();
  C.Divide(2,2);
  C.cd(1);
  HDstpiMCDMass.Draw();
  C.cd(2);
  HDstpiDataDMass.Draw();
  C.Print("BFRatioCrossChecks.ps");

  C.Clear();
  C.Divide(2,2);
  C.cd(1);
  HDstpiMCDeltaM.Draw();
  C.cd(2);
  HDstpiDataDeltaM.Draw();
  C.Print("BFRatioCrossChecks.ps");

  C.Clear();
  C.Divide(2,2);
  C.cd(1);
  HDstpiMCPstar.Draw();
  C.cd(2);
  HDstpiDataPstar.Draw();
  C.Print("BFRatioCrossChecks.ps");

  C.Clear();
  C.Divide(2,2);
  C.cd(1);
  HDstpiMCDCos.Draw();
  C.cd(2);
  HDstpiDataDCos.Draw();
  C.Print("BFRatioCrossChecks.ps");
 

  //C.Print("BFRatioCrossChecks.ps]");
  //return;

  //plot the Dpi efficiency as a function of time
  TH1F HDPiMCReco("HDPiMCReco","",5,0.5,5.5);
  TH1F HDPiMCGen("HDPiMCGen","",5,0.5,5.5);
  Float_t NDPiGen[5]={150000,150000,150000,150000,102000};
  for(Int_t i=1;i<=5;i++){
    TFile FDPiNtp(TString("DCharged/DChargedPi/MC/SP-9015/Ntuple")+(long)i+".root");
    TTree*TDPiNtp=(TTree*)FDPiNtp.Get("ntp1");    
    HDPiMCReco.SetBinContent(i,TDPiNtp->GetEntries("nX>0"));
    HDPiMCGen.SetBinContent(i,NDPiGen[i-1]);
  }
  HDPiMCReco.Sumw2();
  HDPiMCReco.Divide(&HDPiMCGen);
  C.Clear();
  HDPiMCReco.SetStats(0);
  HDPiMCReco.GetYaxis()->SetTitle("Raw Efficiency");
  HDPiMCReco.GetXaxis()->SetTitle("MC Sample (Time)");
  HDPiMCReco.GetYaxis()->SetRangeUser(0,HDPiMCReco.GetMaximum()*1.2);
  HDPiMCReco.Draw("hist p");
  C.Print("BFRatioCrossChecks.ps");

  //plot the DPi yield vs run
  Float_t Lumi[6]={23,68,34.8,110.4,147.4,70.7};
  TH1F HLumi("HLumi","",6,0.5,6.5);
  HLumi.SetBinContent(1,Lumi[0]);
  HLumi.SetBinContent(2,Lumi[1]);
  HLumi.SetBinContent(3,Lumi[2]);
  HLumi.SetBinContent(4,Lumi[3]);
  HLumi.SetBinContent(5,Lumi[4]);
  HLumi.SetBinContent(6,Lumi[5]);
  TH1F HDPiDataVsRun("HDPiDataVsRun","",6,0.5,6.5);
  HDPiDataVsRun.GetYaxis()->SetTitle("Events Passed");
  HDPiDataVsRun.GetXaxis()->SetTitle("Run Number");
  HDPiDataVsRun.SetStats(0);
  HDPiDataVsRun.SetMarkerSize(2);
  HDPiDataVsRun.SetBinContent(1,TDPiData->GetEntries("0<runnumber/1000&&runnumber/1000<18"));
  HDPiDataVsRun.SetBinContent(2,TDPiData->GetEntries("18<runnumber/1000&&runnumber/1000<32"));
  HDPiDataVsRun.SetBinContent(3,TDPiData->GetEntries("32<runnumber/1000&&runnumber/1000<40"));
  HDPiDataVsRun.SetBinContent(4,TDPiData->GetEntries("40<runnumber/1000&&runnumber/1000<52"));
  HDPiDataVsRun.SetBinContent(5,TDPiData->GetEntries("52<runnumber/1000&&runnumber/1000<69"));
  HDPiDataVsRun.SetBinContent(6,TDPiData->GetEntries("69<runnumber/1000&&runnumber/1000<80"));
  C.Clear();
  HDPiDataVsRun.GetYaxis()->SetRangeUser(0,HDPiDataVsRun.GetMaximum()*1.2);
  HDPiDataVsRun.Draw("hist p");
  HLumi.Scale(HDPiDataVsRun.Integral()/HLumi.Integral());
  HLumi.Draw("hist same");
  C.Print("BFRatioCrossChecks.ps");



  
  //plot the Dpi efficiency as a function of time
  TH1F HDstPiMCReco("HDstPiMCReco","",8,0.5,8.5);
  TH1F HDstPiMCGen("HDstPiMCGen","",8,0.5,8.5);
  Float_t NDstPiGen[8]={100000,100000,100000,100000,100000,100000,100000,4000};
  for(Int_t i=1;i<=8;i++){
    TFile FDstPiNtp(TString("DstarCharged/DstarPi/MC/Signal/D2460/Kpi/Ntuples/Ntuple")+(long)i+".root");
    TTree*TDstPiNtp=(TTree*)FDstPiNtp.Get("ntp1");    
    HDstPiMCReco.SetBinContent(i,TDstPiNtp->GetEntries("nX>0"));
    HDstPiMCGen.SetBinContent(i,NDstPiGen[i-1]);
  }
  HDstPiMCReco.Sumw2();
  HDstPiMCReco.Divide(&HDstPiMCGen);
  C.Clear();
  HDstPiMCReco.SetStats(0);
  HDstPiMCReco.GetYaxis()->SetTitle("Raw Efficiency");
  HDstPiMCReco.GetXaxis()->SetTitle("MC Sample (Time)");
  HDstPiMCReco.GetYaxis()->SetRangeUser(0,HDstPiMCReco.GetMaximum()*1.2);
  HDstPiMCReco.Draw("hist p");
  C.Print("BFRatioCrossChecks.ps");


  //plot the D*Pi yield vs run
  TH1F HDstPiDataVsRun("HDstPiDataVsRun","",6,0.5,6.5);
  HDstPiDataVsRun.GetYaxis()->SetTitle("Events Passed");
  HDstPiDataVsRun.GetXaxis()->SetTitle("Run Number");
  HDstPiDataVsRun.SetStats(0);
  HDstPiDataVsRun.SetMarkerSize(2);
  HDstPiDataVsRun.SetBinContent(1,TDstPiData->GetEntries("0<runnumber/1000&&runnumber/1000<18"));
  HDstPiDataVsRun.SetBinContent(2,TDstPiData->GetEntries("18<runnumber/1000&&runnumber/1000<32"));
  HDstPiDataVsRun.SetBinContent(3,TDstPiData->GetEntries("32<runnumber/1000&&runnumber/1000<40"));
  HDstPiDataVsRun.SetBinContent(4,TDstPiData->GetEntries("40<runnumber/1000&&runnumber/1000<52"));
  HDstPiDataVsRun.SetBinContent(5,TDstPiData->GetEntries("52<runnumber/1000&&runnumber/1000<70.5"));
  HDstPiDataVsRun.SetBinContent(6,TDstPiData->GetEntries("70.5<runnumber/1000&&runnumber/1000<80"));
  C.Clear();
  HDstPiDataVsRun.GetYaxis()->SetRangeUser(0,HDstPiDataVsRun.GetMaximum()*1.2);
  HDstPiDataVsRun.Draw("hist p");
  HLumi.Scale(HDstPiDataVsRun.Integral()/HLumi.Integral());
  HLumi.Draw("hist same");
  C.Print("BFRatioCrossChecks.ps");


  C.Print("BFRatioCrossChecks.ps]");
}



void BFRatioCrossChecksD0piDstPiRatio(){
  
  Float_t D0PiYield=110.8;
  Float_t DstPiKpiYield=28.7;

  Float_t DCPiEff=0.142;
  Float_t DstPiKPiEff=0.175;
  Float_t DstPiK3PiEff=0.120;

  //derived from DCPi eff, correct for Flight cut and K angle cut in D0Pi then correct for extra track
  Float_t trackEff=sqrt(0.120/0.175);
  Float_t D0PiKangleEff=0.95;
  Float_t DCPiFlightEff=0.91;
  Float_t D0PiEff=(DCPiEff*D0PiKangleEff)/(DCPiFlightEff*trackEff);
  
  //effects ignored:
  //-DCPi: piLHLoose(0.98) on extra D+ pion --> negative change in ratio
  //-DCPi: primary pion piLHVeryTight(0.92)   D0Pi:  piLHLoose(0.98)  -->negative change
  //-D0Pi: D0pi0 and D0pi+ vetos --> positive change 
  //-D0Pi: Skim prob cut of .5% --> positive change

  printf("DCPiEff=%.3f\n DstPiKPiEff=%.3f\n DstPiK3PiEff=%.3f\n trackEff=%.3f\n",DCPiEff,DstPiKPiEff,DstPiK3PiEff,trackEff);
  printf("DCPiFlightEff=%.3f\n D0PiKangleEff=%.3f\n",DCPiFlightEff,D0PiKangleEff);
  printf("D0PiEff=%.3f\n",D0PiEff);
  printf(" B(D0Pi)/B(DstPi)=%.3f\n",(D0PiYield/D0PiEff)/(DstPiKpiYield/(DstPiKPiEff*0.677)));

}
