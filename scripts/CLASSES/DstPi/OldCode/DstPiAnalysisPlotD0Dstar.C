#include "DstPiAnalysis.h"


Int_t DstPiAnalysis::MakeD0DstarMass(){
 
  if(OpenReducedNtuple(1)!=1) return 0;

  TFile File(_OutputDir+"/D0DstarMassPlots.root","recreate");


  //TH1F HD0Mass("HD0Mass","",database.GetD0MassRangeNbins(),database.GetD0MassRangeMin(),database.GetD0MassRangeMax());
  TH1F HD0Mass("HD0Mass","",200,1.75,1.95);
  ReducedNtuple->Draw("d0mass>>HD0Mass",database.GetCut(1,4)+database.GetCut(6,100));
  HD0Mass.Write();

  TH1F HDstarMass("HDstarMass","",database.GetDstarDeltaMRangeNbins(),database.GetDstarDeltaMRangeMin(),database.GetDstarDeltaMRangeMax());
  ReducedNtuple->Draw("dstardeltam>>HDstarMass",database.GetCut(1,3)+database.GetCut(5,100));
  HDstarMass.Write();

  //for fit
  TH1F HDstarMassFine("HDstarMassFine","",database.GetDstarDeltaMRangeNbins()*10,database.GetDstarDeltaMRangeMin(),database.GetDstarDeltaMRangeMax());
  ReducedNtuple->Draw("dstardeltam>>HDstarMassFine",database.GetCut(1,3)+database.GetCut(5,100)+TCut("dstpibestcand==1"));
  HDstarMassFine.Write();


  TH1F HCosDstar("HCosDstar","",200,-1,1);
  ReducedNtuple->Draw("dstarcostheta>>HCosDstar",database.GetCut(1,7));
  HCosDstar.Write();


//   TH1F Hpstar("Hpstar","",500,0,5);
//   ReducedNtuple->Draw("dstpipstar>>Hpstar",database.GetCut(1,5)+database.GetCut(7,100));
//   Hpstar.Write();

  TH1F Hpstar("Hpstar","",400,2.1,3.3);
  ReducedNtuple->Draw("dstpimdm>>Hpstar",database.GetCut(1,5)+database.GetCut(7,100)+TCut("dstpipstar<3"));
  Hpstar.Write();


  TH1F HDstPiMass("HDstPiMass","",400,2.1,3.3);
  ReducedNtuple->Draw("dstpimdm>>HDstPiMass",database.GetCut(1,100));
  HDstPiMass.Write();
  
  File.ls();
  File.Close();
 
  if(CloseReducedNtuple()!=1) return 0;

  return 1;
}

Int_t DstPiAnalysis::PlotD0DstarMass(){
 
  filename=_OutputDir+"/D0DstarMassPlots.ps";
  Canvas.Print(filename+"[");

  TFile File(_OutputDir+"/D0DstarMassPlots.root","read");
  
  cutline.SetLineColor(2);

  TH1F* HD0Mass=(TH1F*)File.Get("HD0Mass"); 
  SetHistoXYLabels(HD0Mass,"m(K#pi)","GeV/c^{2}");
  if(_D0Mode==2)SetHistoXYLabels(HD0Mass,"m(K#pi#pi#pi)","GeV/c^{2}");
  HD0Mass->SetTitle("");
  Canvas.Clear();
  HD0Mass->Draw("pe");
  cutline.DrawLine(database.GetD0MassMin(),0,database.GetD0MassMin(),HD0Mass->GetMaximum());
  cutline.DrawLine(database.GetD0MassMax(),0,database.GetD0MassMax(),HD0Mass->GetMaximum());  
  Canvas.Print(filename);

  TH1F* HDstarMass=(TH1F*)File.Get("HDstarMass");
  SetHistoXYLabels(HDstarMass,"m(D^{0}#pi)-m(D^{0})","GeV/c^{2}");
  HDstarMass->SetTitle("");
  Canvas.Clear();
  HDstarMass->Draw("pe");
  cutline.DrawLine(database.GetDstarDeltaMMin(),0,database.GetDstarDeltaMMin(),HDstarMass->GetMaximum());
  cutline.DrawLine(database.GetDstarDeltaMMax(),0,database.GetDstarDeltaMMax(),HDstarMass->GetMaximum());  
  Canvas.Print(filename);

  TH1F* HCosDstar=(TH1F*)File.Get("HCosDstar");
  SetHistoXYLabels(HCosDstar,"cos(#theta_{D*})","");
  HCosDstar->SetTitle("");
  HCosDstar->SetStats(0);
  HCosDstar->SetFillColor(4);
  Canvas.Clear();
  HCosDstar->Draw();
  cutline.DrawLine(.5,0,.5,HCosDstar->GetMaximum());  
  Canvas.Print(filename);

  TH1F* Hpstar=(TH1F*)File.Get("Hpstar");
  SetHistoXYLabels(Hpstar,"m(D*#pi)","GeV/c^{2}");
  Hpstar->SetTitle("");
  Hpstar->SetStats(0);
  Canvas.Clear();
  Hpstar->Draw();
  Canvas.Print(filename);


  TH1F* HDstPiMass=(TH1F*)File.Get("HDstPiMass");
  SetHistoXYLabels(HDstPiMass,"m(D*#pi)","GeV/c^{2}");
  HDstPiMass->SetTitle("");
  Canvas.Clear();
  HDstPiMass->Draw("pe");
  Canvas.Print(filename);


  if(OpenReducedNtuple(2)!=1) return 0;  
  TH1F HCosDstarPi("HCosDstarPi","",200,-1,1);
  SetHistoXYLabels(&HCosDstarPi,"cos(#alpha)","");
  HCosDstarPi.SetStats(0);
  Canvas.Clear();
  ReducedNtuple->Draw("pi1cosine>>HCosDstarPi","","pe");  
  Canvas.Print(filename);

  TH1F HMultiplicity("HMultiplicity","",50,.5,50.5);
  HMultiplicity.SetStats(0);
  HMultiplicity.GetXaxis()->SetTitle("Candidates/event");
  HMultiplicity.GetYaxis()->SetTitle("events");
  Int_t currIdx=0;
  Int_t e=0;
  while(ReducedNtuple->GetEntry(e,0)){
    if(dstpiIdx!=currIdx){
      HMultiplicity.Fill(dstpinX);
      currIdx=dstpiIdx;
    }
    e++;
  }
  Canvas.Clear();
  HMultiplicity.GetYaxis()->SetRangeUser(1,HMultiplicity.GetMaximum());
  HMultiplicity.Draw();
  Canvas.SetLogy(1);
  Canvas.Print(filename);
  Canvas.SetLogy(0);

  if(CloseReducedNtuple()!=1) return 0;
  

  Canvas.Print(filename+"]");  

  File.Close();

  return 1;
}


Int_t DstPiAnalysis::MakeRSWithNotTruthedandWS(){
  if(_TruthMatch)return 0;

  TFile file("RawDstPiMassPlots.root","recreate");
  if(OpenReducedNtuple(2)!=1) return 0;
  TH1F HDstPiMass("HDstPiMass","",400,2.1,3.3);
  ReducedNtuple->Draw("dstpimdm>>HDstPiMass");
  if(CloseReducedNtuple()!=1) return 0;

  file.cd();
  HDstPiMass.Write();

  //now get the not truthed ccbar
  if(_MCorDATA==1){
    TString fname;
    if(_D0Mode==1)fname="DstarCharged/DstarPi/MC/SP-1005/Kpi/ReducedClean.root";
    if(_D0Mode==2)fname="DstarCharged/DstarPi/MC/SP-1005/K3pi/ReducedClean.root";
    TFile FNotTruthed(fname,"read");
    FNotTruthed.ls();
    gROOT->cd();
    TTree*tree=(TTree*)FNotTruthed.Get("CleanNtuple");
    TH1F HDstPiMassNotTruthed("HDstPiMassNotTruthed","",400,2.1,3.3);
    tree->Draw("dstpimdm>>HDstPiMassNotTruthed");
    FNotTruthed.Close();
    
    file.cd();
    HDstPiMassNotTruthed.Write();
 
    //
    //   TH1F HDstPiMassWSCCbar("HDstPiMassWSCCbar","",400,2.1,3.3);
    //   
    //     if(_D0Mode==1)fname="DstarCharged/DstarPi/MC/SP-1005/Kpi/Reduced.root";
    //     if(_D0Mode==2)fname="DstarCharged/DstarPi/MC/SP-1005/K3pi/Reduced.root";
    //     TFile FCCBarWS(fname,"read");
    //     FCCBarWS.ls();
    //     gROOT->cd();
    //     TTree*tree=(TTree*)FCCBarWS.Get("Ntuple"); 
    //     tree->Draw("dstpimdm>>HDstPiMassWSCCbar",database.GetCut(2,100)+TCut("abs(dstpicharge)==2"));//ccbar and DATA have same cuts
    //     FCCBarWS.Close();
    //     file.cd();
    //     HDstPiMassWSCCbar.Write();
  }
  

  //Now get the wrong sign data
  if(OpenReducedNtuple(1)!=1) return 0;
  TH1F HDstPiMassWS("HDstPiMassWS","",400,2.1,3.3);
  ReducedNtuple->Draw("dstpimdm>>HDstPiMassWS",database.GetCut(2,100)+TCut("abs(dstpicharge)==2"));
  if(CloseReducedNtuple()!=1) return 0;
  
  
  file.cd();
  HDstPiMassWS.Write();
 
 

  file.Close();


  return 1;
}




Int_t DstPiAnalysis::PlotRSWithNotTruthedandWS(){
  if(_TruthMatch)return 0;
  

  filename=_OutputDir+"/DstPiMassPlots.ps";
  Canvas.Print(filename+"["); 

  TFile file("RawDstPiMassPlots.root","read");
  gROOT->cd();


  //right sign
  TH1F*HDstPiMass=(TH1F*)file.Get("HDstPiMass");
  SetHistoXYLabels(HDstPiMass,"m(D*#pi)","GeV/c^{2}");
  HDstPiMass->SetStats(0);
  Canvas.Clear();
  HDstPiMass->Draw("pe");
  Canvas.Print(filename);


  //wrong sign
  TH1F*HDstPiMassWS=(TH1F*)file.Get("HDstPiMassWS");
  SetHistoXYLabels(HDstPiMassWS,"m(D*#pi)","GeV/c^{2}");
  HDstPiMassWS->SetStats(0);
  Canvas.Clear();
  HDstPiMassWS->Draw("pe");
  Canvas.Print(filename);

  //rs and ws 
  Canvas.Clear();
  HDstPiMass->Draw("pe");
  HDstPiMassWS->Draw("pesame");
  Canvas.Print(filename); 

  /*
  if(_MCorDATA==1){
    //right sign not truth-matched
    TH1F*HDstPiMassNotTruthed=(TH1F*)file.Get("HDstPiMassNotTruthed");
    SetHistoXYLabels(HDstPiMassNotTruthed,"m(D*#pi)","GeV/c^{2}");
    HDstPiMassNotTruthed->SetStats(0);
    Canvas.Clear();
    HDstPiMassNotTruthed->Draw("pe");
    Canvas.Print(filename);
  

    //ccbar wrong sign  
    TH1F*HDstPiMassWSCCbar=NULL;
    
    HDstPiMassWSCCbar=(TH1F*)file.Get("HDstPiMassWSCCbar");
    SetHistoXYLabels(HDstPiMassWSCCbar,"m(D*#pi)","GeV/c^{2}");
    HDstPiMassWSCCbar->SetStats(0);
    Canvas.Clear();
    HDstPiMassWSCCbar->Draw("pe");
    Canvas.Print(filename);

  



 
    ///scale down the ccbar background
    TH1F*HDstPiMassNotTruthedLumiScale=NULL;
    if(_MCorDATA==1){
      ///scale by lumi
      HDstPiMassNotTruthedLumiScale=(TH1F*)HDstPiMassNotTruthed->Clone();
      HDstPiMassNotTruthedLumiScale->Scale(scalefactorccbar); 
      
      ///scale more by sidebands
      Float_t scaleccb=(HDstPiMass->Integral(1,33)+HDstPiMass->Integral(400-33,400))/(HDstPiMassNotTruthed->Integral(1,33)+HDstPiMassNotTruthed->Integral(400-33,400));//below 2.2, and above 3.2
      //Float_t scaleccb=HDstPiMassWS->Integral()/HDstPiMassWSCCbar->Integral();
      HDstPiMassNotTruthed->Scale(scaleccb);    
    }

 
  }
 

 
  //rs and scaled ws 
  Float_t scalews=(HDstPiMass->Integral(1,33)+HDstPiMass->Integral(400-33,400))/(HDstPiMassWS->Integral(1,33)+HDstPiMassWS->Integral(400-33,400));//below 2.2, and above 3.2
  TH1F*HWSScaled=(TH1F*)HDstPiMassWS->Clone();
  HWSScaled->Scale(scalews);
  Canvas.Clear();
  HDstPiMass->Draw("pe");
  HWSScaled->Draw("pesame");
  Canvas.Print(filename); 
  delete HWSScaled;
  
  ///plot all
  Canvas.Clear();
  HDstPiMass->Draw("pe");
  HDstPiMassNotTruthed->Draw("pesame");
  //HDstPiMassWS->Draw("pesame");
  Canvas.Print(filename); 

  if(_MCorDATA==1){//for data  also compare to lumi scaled ccbar   
    Canvas.Clear();
    HDstPiMass->Draw("pe");
    HDstPiMassNotTruthedLumiScale->Draw("pesame");
    Canvas.Print(filename); 
  }

  ///compare wrong signs
  if(_MCorDATA==1){
    HDstPiMassWSCCbar->Sumw2();
    HDstPiMassWSCCbar->Scale(HDstPiMassWS->Integral()/HDstPiMassWSCCbar->Integral());
    Canvas.Clear();
    HDstPiMassWS->Draw("pe");
    HDstPiMassWSCCbar->Draw("lsame");
    Canvas.Print(filename); 
  }else {
    HDstPiMassWS->Sumw2();
    HDstPiMassWS->Scale(HDstPiMassNotTruthed->Integral()/HDstPiMassWS->Integral());
    Canvas.Clear();
    HDstPiMassWS->Draw("pe");
    HDstPiMassNotTruthed->Draw("same");
    Canvas.Print(filename);  
  }
 
  if(_MCorDATA==1){
    HDstPiMassWS->Sumw2();
    HDstPiMassWS->Add(HDstPiMassWSCCbar,-1);
    Canvas.Clear();
    HDstPiMassWS->Draw("pe");
    Canvas.Print(filename);     
  }else {
    HDstPiMassNotTruthed->Sumw2();
    HDstPiMassNotTruthed->Add(HDstPiMassWS,-1);
    Canvas.Clear();
    HDstPiMassNotTruthed->Draw("pe");
    Canvas.Print(filename);  
  }
  
  //background subtracted
  HDstPiMass->Sumw2();
  HDstPiMass->Add(HDstPiMassNotTruthed,-1);
  Canvas.Clear();
  HDstPiMass->Draw("pe");
  cutline.DrawLine(2.1,0,3.3,0);
  Canvas.Print(filename); 
    
  */


  Canvas.Print(filename+"]");

  file.Close();
  return 1;
}



Int_t DstPiAnalysis::FitDstarMass(){

  filename=_OutputDir+"/DstarMassFit.ps";
  Canvas.Print(filename+"[");

  TFile File(_OutputDir+"/D0DstarMassPlots.root","read");
  
  TH1F* HDstarMass=(TH1F*)File.Get("HDstarMassFine");
  if(!HDstarMass)return 0;
  SetHistoXYLabels(HDstarMass,"m(D^{0}#pi)-m(D^{0})","GeV/c^{2}");
  HDstarMass->SetTitle("");
 
  TF1 sig;
  TF1 bkg;
  if(_TruthMatch)FitHistogramDstarDM(HDstarMass,.141,.152,&sig,0);
  else FitHistogramDstarDM(HDstarMass,.141,.152,&sig,&bkg);
  Canvas.Clear();
  HDstarMass->Draw("pe");
  bkg.Draw("same");
  text.SetTextSize(.04);
  sprintf(textlabel,"mean=%.9f +- %.9f",sig.GetParameter(1),sig.GetParError(1));
  text.DrawTextNDC(.03,.02,textlabel);
  Canvas.SetLogy(1);
  Canvas.Print(filename);

  Canvas.Print(filename+"]");  

  File.Close();

  return 1;
}



Int_t DstPiAnalysis::FitD0Mass(){

  filename=_OutputDir+"/D0MassFit.ps";
  Canvas.Print(filename+"[");

  TFile File(_OutputDir+"/D0DstarMassPlots.root","read");
  gROOT->cd();

  TH1F* HD0Mass=(TH1F*)File.Get("HD0Mass");
  if(!HD0Mass)return 0;
  SetHistoXYLabels(HD0Mass,"D^{0} candidate mass ","GeV/c^{2}");
  HD0Mass->SetTitle("");
 

  Canvas.Clear();
  HD0Mass->Draw("pe");
  Canvas.Print(filename);  

  MassFitter d0massfit(&mass);
  if(!d0massfit.Initialize(HD0Mass,_OutputDir+"/InitRooFitParsDMass.txt"))return 0;
  if(d0massfit.Fit()!=0)return 0;
 //  RooAbsPdf*bkgraw=d0massfit.GetBkgPdf();
//   mass.setRange("sigregion",DPlusPDGMass-mmax,DPlusPDGMass+mmax);
//   RooAbsReal* integralraw= bkgraw->createIntegral(mass,"sigregion");
//   cout<<"SigYield="<<d0massfit.GetSigYield()<<" totbkg="<<d0massfit.GetBkgYield()<<" bkgundersig"<<integralraw->getVal()<<endl;
  
  Canvas.Clear();
  RooPlot*fitplot=d0massfit.GetFitPlot();
  fitplot->Draw();
  Canvas.Print(filename);  


  Canvas.Print(filename+"]");  

  File.Close();

  return 1;
}





Int_t DstPiAnalysis::DrawMassVsHelicity2D(Int_t nbins,Float_t massmin,Float_t massmax,TCut cut){


  if(OpenReducedNtuple(2)!=1) return 0;
  TH2F* H2=new TH2F("H2","",nbins,massmin,massmax,20,-1,1);
  H2->GetYaxis()->SetTitle("cos(#theta_{H})");
  H2->GetXaxis()->SetTitle("m(D*#pi)");
  H2->SetStats(0);
  ReducedNtuple->Draw("dstarhelicity:dstpimdm>>H2",cut);
  if(CloseReducedNtuple()!=1) return 0;
  cout<<H2<<endl;
  Canvas.Clear();
  H2->Draw("colz");
  //Canvas.Print("H2MassVsHelicity.ps");
  
  
  return 1;
}
