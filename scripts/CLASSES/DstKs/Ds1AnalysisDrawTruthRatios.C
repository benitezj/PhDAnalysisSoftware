
void Ds1Analysis::DrawTruthRatios(TCanvas* Canvas,TString filename){
  cout<<"Drawing the truth ratios"<<endl;

  Float_t KTruthRatio=0;
  Float_t PiTruthRatio=0;
  Float_t D0TruthRatio=0;
  Float_t SlowPiTruthRatio=0;
  Float_t DstarTruthRatio=0;
  Float_t Pi1TruthRatio=0;
  Float_t Pi2TruthRatio=0;
  Float_t KsTruthRatio=0;
  Float_t Ds1pTruthRatio=0;


  if(ReducedNtuple==NULL){
    cout<<"Ntuple is NULL, cannot do truth match ratios."<<endl;
    return;
  }
  TH1F Htemp("Htemp","HTemp",1,0,100);
  TH1F Htemptrue("Htemptrue","Htemptrue",1,0,100);
  TCut truecut;
 
  sprintf(textlabel,"%.5f<ds1pdeltam2&&ds1pdeltam2<%.5f",database.GetDs1pDeltaMSignalMin(),database.GetDs1pDeltaMSignalMax());  
  TCut signalcut=textlabel;
  

  ReducedNtuple->Draw("kp3>>Htemp",database.GetCut(2,100)+signalcut);
  truecut="kmctrue==1";
  ReducedNtuple->Draw("kp3>>Htemptrue",database.GetCut(2,100)+truecut+signalcut);
  if(Htemp.Integral()>0)
    KTruthRatio=Htemptrue.Integral()/Htemp.Integral();
  
  ReducedNtuple->Draw("pip3>>Htemp",database.GetCut(2,100)+signalcut);
  truecut="pimctrue==1";
  ReducedNtuple->Draw("pip3>>Htemptrue",database.GetCut(2,100)+truecut+signalcut);
  if(Htemp.Integral()>0)
    PiTruthRatio=Htemptrue.Integral()/Htemp.Integral();

  ReducedNtuple->Draw("d0mass>>Htemp",database.GetCut(2,100)+signalcut);
  truecut="d0mctrue==1";
  ReducedNtuple->Draw("d0mass>>Htemptrue",database.GetCut(2,100)+truecut+signalcut);
  if(Htemp.Integral()>0)
    D0TruthRatio=Htemptrue.Integral()/Htemp.Integral();
 
  ReducedNtuple->Draw("slowpip3>>Htemp",database.GetCut(2,100)+signalcut);
  truecut="slowpimctrue==1";
  ReducedNtuple->Draw("slowpip3>>Htemptrue",database.GetCut(2,100)+truecut+signalcut);
  if(Htemp.Integral()>0)
    SlowPiTruthRatio=Htemptrue.Integral()/Htemp.Integral();

  ReducedNtuple->Draw("dstarmass>>Htemp",database.GetCut(2,100)+signalcut);
  truecut="dstarmctrue==1";
  ReducedNtuple->Draw("dstarmass>>Htemptrue",database.GetCut(2,100)+truecut+signalcut);
  if(Htemp.Integral()>0)
    DstarTruthRatio=Htemptrue.Integral()/Htemp.Integral();
 
  ReducedNtuple->Draw("pi1p3>>Htemp",database.GetCut(2,100)+signalcut);
  truecut="pi1mctrue==1";
  ReducedNtuple->Draw("pi1p3>>Htemptrue",database.GetCut(2,100)+truecut+signalcut);
  if(Htemp.Integral()>0)
    Pi1TruthRatio=Htemptrue.Integral()/Htemp.Integral();

  ReducedNtuple->Draw("pi2p3>>Htemp",database.GetCut(2,100)+signalcut);
  truecut="pi2mctrue==1";
  ReducedNtuple->Draw("pi2p3>>Htemptrue",database.GetCut(2,100)+truecut+signalcut);
  if(Htemp.Integral()>0)
    Pi2TruthRatio=Htemptrue.Integral()/Htemp.Integral();

  ReducedNtuple->Draw("ksmass>>Htemp",database.GetCut(2,100)+signalcut);
  truecut="ksmctrue==1";
  ReducedNtuple->Draw("ksmass>>Htemptrue",database.GetCut(2,100)+truecut+signalcut);
  if(Htemp.Integral()>0)
    KsTruthRatio=Htemptrue.Integral()/Htemp.Integral();
 
  ReducedNtuple->Draw("ds1pmass>>Htemp",database.GetCut(2,100)+signalcut);
  truecut="ds1pmctrue==1";
  ReducedNtuple->Draw("ds1pmass>>Htemptrue",database.GetCut(2,100)+truecut+signalcut);
  if(Htemp.Integral()>0)
    Ds1pTruthRatio=Htemptrue.Integral()/Htemp.Integral();
 

  TH1F HTruthRatioVsParticle;
  SetHistoXY(&HTruthRatioVsParticle,"Truth Match Ratios",10,.5,10.5,"Particle Type","TruthMatched/Reconstructed");
  HTruthRatioVsParticle.SetBinContent(1,KTruthRatio);
  HTruthRatioVsParticle.SetBinContent(2,PiTruthRatio);
  HTruthRatioVsParticle.SetBinContent(3,D0TruthRatio);
  HTruthRatioVsParticle.SetBinContent(4,SlowPiTruthRatio);
  HTruthRatioVsParticle.SetBinContent(5,DstarTruthRatio); 
  HTruthRatioVsParticle.SetBinContent(6,Pi1TruthRatio);   
  HTruthRatioVsParticle.SetBinContent(7,Pi2TruthRatio);   
  HTruthRatioVsParticle.SetBinContent(8,KsTruthRatio); 
  HTruthRatioVsParticle.SetBinContent(9,Ds1pTruthRatio); 
  HTruthRatioVsParticle.GetYaxis()->SetRangeUser(0,1);
  HTruthRatioVsParticle.SetStats(kFALSE);
  HTruthRatioVsParticle.SetBarWidth(.9);
  

  text.SetTextSize(.03);

  Canvas->Clear();
  HTruthRatioVsParticle.Draw("b");
  text.DrawText(1-.2,.1,"K");
  text.DrawText(2-.2,.1,"Pi");
  text.DrawText(3-.2,.1,"D0");
  text.DrawText(4-.4,.1,"SlowPi");
  text.DrawText(5-.2,.1,"D*");
  text.DrawText(6-.2,.1,"Pi1");
  text.DrawText(7-.2,.1,"Pi2");
  text.DrawText(8-.2,.1,"Ks");
  text.DrawText(9-.2,.1,"Ds1");
  text.DrawText(1-.35,KTruthRatio*.9,TString("")+long(100*KTruthRatio)+"."+long(1000*KTruthRatio)%10+"%");
  text.DrawText(2-.35,PiTruthRatio*.9,TString("")+long(100*PiTruthRatio)+"."+long(1000*PiTruthRatio)%10+"%");
  text.DrawText(3-.35,D0TruthRatio*.9,TString("")+long(100*D0TruthRatio)+"."+long(1000*D0TruthRatio)%10+"%");
  text.DrawText(4-.35,SlowPiTruthRatio*.9,TString("")+long(100*SlowPiTruthRatio)+"."+long(1000*SlowPiTruthRatio)%10+"%");
  text.DrawText(5-.35,DstarTruthRatio*.9,TString("")+long(100*DstarTruthRatio)+"."+long(1000*DstarTruthRatio)%10+"%");
  text.DrawText(6-.35,Pi1TruthRatio*.9,TString("")+long(100*Pi1TruthRatio)+"."+long(1000*Pi1TruthRatio)%10+"%");
  text.DrawText(7-.35,Pi2TruthRatio*.9,TString("")+long(100*Pi2TruthRatio)+"."+long(1000*Pi2TruthRatio)%10+"%");
  text.DrawText(8-.35,KsTruthRatio*.9,TString("")+long(100*KsTruthRatio)+"."+long(1000*KsTruthRatio)%10+"%");
  text.DrawText(9-.35,Ds1pTruthRatio*.9,TString("")+long(100*Ds1pTruthRatio)+"."+long(1000*Ds1pTruthRatio)%10+"%");
  Canvas->Print(filename);

}
