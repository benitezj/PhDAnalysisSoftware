#include "DstPiAnalysis.h"
 
Int_t DstPiAnalysis::DstarHelicity(){ 
  cout<<"Going to plot final state masses"<<endl;
  //Use the same histograms that are fitted

  //ps file
  filename=_OutputDir+"/DstarHelicity.ps";
  Canvas.Print(filename+"[");
  
  ///print the cuts into the file
  database.PrintCutList(&Canvas,filename);  

  if(!OpenReducedNtuple(2)) return 0;
  

  //plot the sideband
  TH2F HDstarHelSB("HDstarHelSB","HDstarHelSB",20,2.25,2.35,20,-1.001,1.001);  
  ReducedNtuple->Draw("dstarhelicity:dstpideltam+2.01>>HDstarHelSB","2.25<dstpideltam+2.01&&dstpideltam+2.01<2.35");

  Canvas.Clear();
  Canvas.Divide(2,2);
  Canvas.cd(1);
  HDstarHelSB.Draw("colz");
  
  TH1*hYSB=HDstarHelSB.ProjectionY();
  hYSB->GetYaxis()->SetRangeUser(0,hYSB->GetMaximum());
  Canvas.cd(2);
  hYSB->Draw();   

  TH1*hXSB=HDstarHelSB.ProjectionX();
  hXSB->GetYaxis()->SetRangeUser(0,hXSB->GetMaximum());
  Canvas.cd(3);
  hXSB->Draw();   
  Canvas.Print(filename);


  ///signal region
  TH2F HDstarHel("HDstarHel","HDstarHel",40,2.35,2.55,20,-1.001,1.001);  
  ReducedNtuple->Draw("dstarhelicity:dstpideltam+2.01>>HDstarHel","2.35<dstpideltam+2.01&&dstpideltam+2.01<2.55");

  Canvas.Clear();
  Canvas.Divide(2,2);
  Canvas.cd(1);
  HDstarHel.Draw("colz");
 
  TH1*hY=HDstarHel.ProjectionY();
  hY->GetYaxis()->SetRangeUser(0,hY->GetMaximum());
  Canvas.cd(2);
  hY->Draw("pe");   

  TH1*hX=HDstarHel.ProjectionX();
  hX->GetYaxis()->SetRangeUser(0,hX->GetMaximum());
  Canvas.cd(3);
  hX->Draw("pe");   
  Canvas.Print(filename);

  Canvas.Clear();
  HDstarHel.Draw("colz");
  Canvas.Print(filename);
  Canvas.Clear();
  hY->SetFillColor(1);
  hY->SetFillStyle(3144);
  hY->Draw("pe"); 
  Canvas.Print(filename);
  Canvas.Clear();
  hX->SetFillColor(1);
  hX->SetFillStyle(3144);
  hX->Draw("pe");  
  Canvas.Print(filename);
  



  ///D2420 region
  TH2F HDstarHelD2420("HDstarHelD2420","HDstarHelD2420",8,2.420-.030,2.420+.01,20,-1.001,1.001);  
  ReducedNtuple->Draw("dstarhelicity:dstpideltam+2.01>>HDstarHelD2420","2.420-.03<dstpideltam+2.01&&dstpideltam+2.01<2.420+.01");

  Canvas.Clear();
  Canvas.Divide(2,2);
  Canvas.cd(1);
  HDstarHelD2420.Draw("colz");
  
  TH1*hY2420=HDstarHelD2420.ProjectionY();
  hY2420->GetYaxis()->SetRangeUser(0,hY2420->GetMaximum());
  Canvas.cd(2);
  hY2420->Draw();   
  
  TH1*hX2420=HDstarHelD2420.ProjectionX();
  hX2420->GetYaxis()->SetRangeUser(0,hX2420->GetMaximum());
  Canvas.cd(3); 
  hX2420->Draw();   
  Canvas.Print(filename);


  ///D2460 region
  TH2F HDstarHelD2460("HDstarHelD2460","HDstarHelD2460",6,2.460-.01,2.460+.02,20,-1.001,1.001);  
  ReducedNtuple->Draw("dstarhelicity:dstpideltam+2.01>>HDstarHelD2460","2.460-.01<dstpideltam+2.01&&dstpideltam+2.01<2.460+.02");

  Canvas.Clear();
  Canvas.Divide(2,2);
  Canvas.cd(1);
  HDstarHelD2460.Draw("colz");
  
  TH1*hY2460=HDstarHelD2460.ProjectionY();
  hY2460->GetYaxis()->SetRangeUser(0,hY2460->GetMaximum());
  Canvas.cd(2);
  hY2460->Draw();   
  
  TH1*hX2460=HDstarHelD2460.ProjectionX();
  hX2460->GetYaxis()->SetRangeUser(0,hX2460->GetMaximum());
  Canvas.cd(3); 
  hX2460->Draw();   
  Canvas.Print(filename);



  Canvas.Print(filename+"]");
  if(CloseReducedNtuple()!=1) return 0;


  return 1;

}



Int_t DstPiAnalysis::DstarHelicityEffCorr(){ 
  cout<<"Going to plot eff corr helicity"<<endl;
 
  //ps file
  filename=_OutputDir+"/DstarHelicityEffCorr.ps";
  Canvas.Print(filename+"[");
  
  TFile* File=new TFile(_OutputDir+"/LegendreWeightedHistos.root","read");
  gROOT->cd();
  TH2F* H2DstPiMVsDstarHel=(TH2F*)File->Get("H2DstPiMVsDstarHelFine");
  if(!H2DstPiMVsDstarHel){cout<<"No H2DstPiMVsDstarHel"<<endl;return 0;}
  H2DstPiMVsDstarHel=CreateRebinnedDstPiHisto2D(H2DstPiMVsDstarHel,100,2.35,2.55,1);  
  if(!H2DstPiMVsDstarHel){cout<<"No H2DstPiMVsDstarHel"<<endl;return 0;}
 
  TH1*hY=H2DstPiMVsDstarHel->ProjectionY();
  hY->GetYaxis()->SetRangeUser(0,1.1*hY->GetMaximum());

  TH1*hX=H2DstPiMVsDstarHel->ProjectionX();
  hX->GetYaxis()->SetRangeUser(0,1.1*hX->GetMaximum());

  Canvas.Clear();
  Canvas.SetLogz(1);
  H2DstPiMVsDstarHel->SetStats(0);
  H2DstPiMVsDstarHel->GetYaxis()->SetTitle("cos(#theta_{H})");
  H2DstPiMVsDstarHel->GetXaxis()->SetTitle(DstarPiXLabel);
  H2DstPiMVsDstarHel->Draw("colz");
  Canvas.Print(filename);
  Canvas.SetLogz(0);
  Canvas.Print(filename);
  Canvas.Clear();
  hY->GetYaxis()->SetTitle("Entries/.05");
  hY->GetXaxis()->SetTitle("cos(#theta_{H})");
  hY->SetStats(0);
  hY->SetFillColor(1);
  hY->SetFillStyle(3144);
  hY->Draw("pe"); 
  Canvas.Print(filename);
  Canvas.Clear();
  hX->GetXaxis()->SetTitle(DstarPiXLabel);
  hX->GetYaxis()->SetTitle(DstarPiYLabel);
  hX->SetStats(0);
  hX->SetFillColor(1);
  hX->SetFillStyle(3144);
  hX->Draw("pe");  
  Canvas.Print(filename);

  Canvas.Print(filename+"]");
  
  delete File;
  delete hX;
  delete hY;

  return 1;
}
