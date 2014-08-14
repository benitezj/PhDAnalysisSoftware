
void Dst2640Analysis::DrawTruthRatios(TCanvas* Canvas,TString filename){
  cout<<"Drawing the truth ratios"<<endl;

  Float_t KTruthRatio=0;
  Float_t PiTruthRatio=0;
  Float_t D0Pi2TruthRatio=0;
  Float_t D0Pi3TruthRatio=0;
  Float_t D0TruthRatio=0;
  Float_t SlowPiTruthRatio=0;
  Float_t DstarTruthRatio=0;
  Float_t Pi1TruthRatio=0;
  Float_t Pi2TruthRatio=0;
  Float_t Dst2640TruthRatio=0;


  if(ReducedNtuple==NULL){
    cout<<"Ntuple is NULL, cannot do truth match ratios."<<endl;
    return;
  }
  TH1F Htemp("Htemp","HTemp",1,-1,100);
  TH1F Htemptrue("Htemptrue","Htemptrue",1,-1,100);
  TCut truecut;
  
  sprintf(textlabel,"%.5f<dst2640deltam&&dst2640deltam<%.5f",database.GetDst2640DeltaMSignalMin(),database.GetDst2640DeltaMSignalMax());  
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

  if(_Mode=="D0ToK3Pi"){
    ReducedNtuple->Draw("d0pi2lh>>Htemp",database.GetCut(2,100)+signalcut);
    truecut="d0pi2mctrue==1";
    ReducedNtuple->Draw("d0pi2lh>>Htemptrue",database.GetCut(2,100)+truecut+signalcut);
    if(Htemp.Integral()>0)
      D0Pi2TruthRatio=Htemptrue.Integral()/Htemp.Integral();
    
    ReducedNtuple->Draw("d0pi3lh>>Htemp",database.GetCut(2,100)+signalcut);
    truecut="d0pi3mctrue==1";
    ReducedNtuple->Draw("d0pi3lh>>Htemptrue",database.GetCut(2,100)+truecut+signalcut);
    if(Htemp.Integral()>0)
      D0Pi3TruthRatio=Htemptrue.Integral()/Htemp.Integral();
  }
  
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
  
  ReducedNtuple->Draw("dst2640deltam>>Htemp",database.GetCut(2,100)+signalcut);
  truecut="dst2640mctrue==1"; 
  ReducedNtuple->Draw("dst2640deltam>>Htemptrue",database.GetCut(2,100)+truecut+signalcut);
  if(Htemp.Integral()>0)
    Dst2640TruthRatio=Htemptrue.Integral()/Htemp.Integral();



  TH1F HTruthRatioVsParticle;
  SetHistoXY(&HTruthRatioVsParticle,"Truth Match Ratios",10,.5,10.5,"Particle Type","TruthMatched/Reconstructed");
  HTruthRatioVsParticle.SetBinContent(1,KTruthRatio);
  HTruthRatioVsParticle.SetBinContent(2,PiTruthRatio);
  HTruthRatioVsParticle.SetBinContent(3,D0Pi2TruthRatio);
  HTruthRatioVsParticle.SetBinContent(4,D0Pi3TruthRatio);
  HTruthRatioVsParticle.SetBinContent(5,D0TruthRatio);
  HTruthRatioVsParticle.SetBinContent(6,SlowPiTruthRatio);
  HTruthRatioVsParticle.SetBinContent(7,DstarTruthRatio); 
  HTruthRatioVsParticle.SetBinContent(8,Pi1TruthRatio);   
  HTruthRatioVsParticle.SetBinContent(9,Pi2TruthRatio);   
  HTruthRatioVsParticle.SetBinContent(10,Dst2640TruthRatio); 
  HTruthRatioVsParticle.GetYaxis()->SetRangeUser(0,1); 
  HTruthRatioVsParticle.SetBarWidth(.9);
  

  text.SetTextSize(.03);
  
  Canvas->Clear();
  HTruthRatioVsParticle.SetFillStyle(1);
  HTruthRatioVsParticle.Draw("b");
  HTruthRatioVsParticle.SetStats(kFALSE);
  HTruthRatioVsParticle.Draw("b");
  text.DrawText(1-.2,.1,"K");
  text.DrawText(2-.2,.1,"Pi");
  text.DrawText(3-.2,.1,"D0Pi2");
  text.DrawText(4-.2,.1,"D0Pi3");
  text.DrawText(5-.2,.1,"D0");
  text.DrawText(6-.4,.1,"SlowPi");
  text.DrawText(7-.2,.1,"D*");
  text.DrawText(8-.2,.1,"Pi1");
  text.DrawText(9-.2,.1,"Pi2");
  text.DrawText(10-.2,.1,"Dst2640");
  text.DrawText(1-.35,KTruthRatio*.9,TString("")+long(100*KTruthRatio)+"."+long(1000*KTruthRatio)%10+"%");
  text.DrawText(2-.35,PiTruthRatio*.9,TString("")+long(100*PiTruthRatio)+"."+long(1000*PiTruthRatio)%10+"%");
  text.DrawText(3-.35,D0Pi2TruthRatio*.9,TString("")+long(100*D0Pi2TruthRatio)+"."+long(1000*D0Pi2TruthRatio)%10+"%");
  text.DrawText(4-.35,D0Pi3TruthRatio*.9,TString("")+long(100*D0Pi3TruthRatio)+"."+long(1000*D0Pi2TruthRatio)%10+"%");
  text.DrawText(5-.35,D0TruthRatio*.9,TString("")+long(100*D0TruthRatio)+"."+long(1000*D0TruthRatio)%10+"%");
  text.DrawText(6-.35,SlowPiTruthRatio*.9,TString("")+long(100*SlowPiTruthRatio)+"."+long(1000*SlowPiTruthRatio)%10+"%");
  text.DrawText(7-.35,DstarTruthRatio*.9,TString("")+long(100*DstarTruthRatio)+"."+long(1000*DstarTruthRatio)%10+"%");
  text.DrawText(8-.35,Pi1TruthRatio*.9,TString("")+long(100*Pi1TruthRatio)+"."+long(1000*Pi1TruthRatio)%10+"%");
  text.DrawText(9-.35,Pi2TruthRatio*.9,TString("")+long(100*Pi2TruthRatio)+"."+long(1000*Pi2TruthRatio)%10+"%");
  text.DrawText(10-.35,Dst2640TruthRatio*.9,TString("")+long(100*Dst2640TruthRatio)+"."+long(1000*Dst2640TruthRatio)%10+"%");
  Canvas->Print(filename);



 
  TH1F HDst2640NoTrue("HDst2640NoTrue","D*(2640) deltaM",database.GetDst2640DeltaMNbins(),database.GetDst2640DeltaMMin(),database.GetDst2640DeltaMMax());
  TH1F HDst2640True("HDst2640True","D*(2640) deltaM",database.GetDst2640DeltaMNbins(),database.GetDst2640DeltaMMin(),database.GetDst2640DeltaMMax());
  TH1F HDst2640TrueBkg("HDst2640TrueBkg","D*(2640) deltaM",database.GetDst2640DeltaMNbins(),database.GetDst2640DeltaMMin(),database.GetDst2640DeltaMMax());

  ReducedNtuple->Draw("dst2640deltam>>HDst2640NoTrue",database.GetCut(2,100));
  truecut="dst2640mctrue==1"; 
  ReducedNtuple->Draw("dst2640deltam>>HDst2640True",database.GetCut(2,100)+truecut);
  TCut truebkgcut="dst2640mctrue!=1"; 
  ReducedNtuple->Draw("dst2640deltam>>HDst2640TrueBkg",database.GetCut(2,100)+truebkgcut);
  
  Canvas->Clear(); 
  HDst2640NoTrue.Draw();
  //HDst2640True.Draw("same");
  HDst2640TrueBkg.SetFillColor(1); 
  HDst2640TrueBkg.Draw("bsame");
  Canvas->Print(filename);
  

}

