#include "../../Globals.h"
#include "../../MACROS/TreeChain.C"
#include "../DstPiPi/JDataBase2640.h"
#include "../../FUNCTIONS/MoveStatsBox.h"
#include "../../FUNCTIONS/Fits.h"
#include "../../FUNCTIONS/SetHisto.h"

void validateNewSkimK3pi(Int_t whichdata){
  Float_t corrfactor=1;
  TString fileold;
  TString filenew;
  TString filenewoff;
  if(whichdata==1){//Run1 R22 compare with R18
    fileold="LogBookPlots/DstarCh_SkimValid/DATARun1Dstar_K3pi_OldSkim";
    filenew="DstarCharged/DstarPiPi/SkimValid/Dstar/DATARun1Dstar/K3Pi";
    filenewoff="DstarCharged/DstarPiPi/SkimValid/Dstar/DATARun1OffDstar/K3Pi";
    corrfactor=1;
  }
  if(whichdata==2){//Run3 R22 compare with R18
    fileold="LogBookPlots/DstarCh_SkimValid/DATARun3Dstar_K3pi";
    filenew="DstarCharged/DstarPiPi/SkimValid/Dstar/DATARun3On";
    corrfactor=1/.906;//accounts for OffPeak (and crashed jobs) data not included in new skim
  }
  if(whichdata==3){//Run3 R18(newSkim) compare with R18(oldSkim)
    fileold="LogBookPlots/DstarCh_SkimValid/DATARun3Dstar_K3pi";
    filenew="DstarCharged/DstarPiPi/SkimValid/Dstar/DATARun3R18";
    corrfactor=1/.934;
  }

 
  TChain treeOld("ntp1");
  if(whichdata==1)
    TreeChain(&treeOld,1,4,fileold);
  if(whichdata==2||whichdata==3)
    TreeChain(&treeOld,1,10,fileold);
 

  TChain treeNew("ntp1");
  if(whichdata==1){
    TreeChain(&treeNew,1,24,filenew);
    TreeChain(&treeNew,1,2,filenewoff);
  }
  if(whichdata==2)
    TreeChain(&treeNew,1,32,filenew);//49
  if(whichdata==3)
    TreeChain(&treeNew,1,35,filenew);

  Int_t DstarIdx;
  Int_t nDstar;
  Float_t DstarMass[100];
  Int_t Dstard1Idx[100];
  Int_t D0Idx;
  Float_t D0Mass[100];
  Int_t eventid=0;
  Int_t runNumber;
  treeNew.SetBranchStatus("*",0);
  treeNew.SetBranchAddress("runNumber",&runNumber);
  treeNew.SetBranchStatus("runNumber",1);
  treeNew.SetBranchAddress("nDstar",&nDstar);
  treeNew.SetBranchStatus("nDstar",1);
  treeNew.SetBranchAddress("DstarMass",DstarMass);
  treeNew.SetBranchStatus("DstarMass",1);
  treeNew.SetBranchAddress("Dstard1Idx",Dstard1Idx);
  treeNew.SetBranchStatus("Dstard1Idx",1);
  treeNew.SetBranchAddress("D0Mass",D0Mass);
  treeNew.SetBranchStatus("D0Mass",1);
  
 
  TH1F HDstarDMNew("HDstarDMNew","D* deltaM New",60,.139,.154);
  TH1F HrunNumberNew("HrunNumberNew","runNumber New",8000,9500.5,17500.5);
  if(whichdata==1)HrunNumberNew.SetBins(8000,9500.5,17500.5);
  if(whichdata==2||whichdata==3)HrunNumberNew.SetBins(8000,32000.5,40000.5);
  while(treeNew.GetEntry(eventid,0)>0){   
    eventid++;    
    if(eventid%5000==0)cout<<eventid<<endl;               
    for(DstarIdx=0;DstarIdx<nDstar;DstarIdx++){
      D0Idx=Dstard1Idx[DstarIdx];            
      if(.140<DstarMass[DstarIdx]-D0Mass[D0Idx]&&DstarMass[DstarIdx]-D0Mass[D0Idx]<.153){
	HDstarDMNew.Fill(DstarMass[DstarIdx]-D0Mass[D0Idx]); 
	HrunNumberNew.Fill(runNumber);     
      }
    }
  }
  //Correct the new skim
  HDstarDMNew.Scale(corrfactor);

  treeOld.SetBranchStatus("*",0);
  treeOld.SetBranchAddress("runNumber",&runNumber);
  treeOld.SetBranchStatus("runNumber",1);
  treeOld.SetBranchAddress("nDstar",&nDstar);
  treeOld.SetBranchStatus("nDstar",1);
  treeOld.SetBranchAddress("DstarMass",DstarMass);
  treeOld.SetBranchStatus("DstarMass",1);
  treeOld.SetBranchAddress("Dstard1Idx",Dstard1Idx);
  treeOld.SetBranchStatus("Dstard1Idx",1);
  treeOld.SetBranchAddress("D0Mass",D0Mass);
  treeOld.SetBranchStatus("D0Mass",1);
 
  TH1F HDstarDMOld("HDstarDMOld","D* deltaM Old",60,.139,.154);
  TH1F HrunNumberOld("HrunNumberOld","runNumber Old",8000,9500.5,17500.5);
  if(whichdata==1)HrunNumberOld.SetBins(8000,9500.5,17500.5);
  if(whichdata==2||whichdata==3)HrunNumberOld.SetBins(8000,32000.5,40000.5);
  eventid=0;
  while(treeOld.GetEntry(eventid,0)>0){   
    eventid++;    
    if(eventid%5000==0)cout<<eventid<<endl;               
    for(DstarIdx=0;DstarIdx<nDstar;DstarIdx++){
      D0Idx=Dstard1Idx[DstarIdx];            
      if(.140<DstarMass[DstarIdx]-D0Mass[D0Idx]&&DstarMass[DstarIdx]-D0Mass[D0Idx]<.153){
	HDstarDMOld.Fill(DstarMass[DstarIdx]-D0Mass[D0Idx]);
	HrunNumberOld.Fill(runNumber);           
      }
    }
  }



  TCanvas Canvas("Canvas","Canvas");
  TString psfile;
  if(whichdata==1)psfile="SkimValidK3PiRun1.ps";
  if(whichdata==2)psfile="SkimValidK3PiRun3.ps";
  if(whichdata==3)psfile="SkimValidK3PiRun3R18.ps";
  Canvas.Print(psfile+"[");

  HDstarDMOld.Draw();
  Canvas.Update();
  HDstarDMNew.Draw();
  Canvas.Update();
  MoveStatsBox(&HDstarDMOld,-1,1);   
  ((TPaveStats*)(HDstarDMOld.FindObject("stats")))->SetTextColor(2);

  if(HDstarDMNew.Integral()>HDstarDMOld.Integral()){
    Canvas.Clear();
    HDstarDMNew.SetFillColor(1);
    HDstarDMNew.Draw();
    Canvas.Update();
    MoveStatsBox(&HDstarDMNew,1,1);  
    HDstarDMOld.SetLineColor(2);
    HDstarDMOld.SetFillColor(2);
    HDstarDMOld.Draw("same");
    Canvas.Print(psfile);
  }
  if(HDstarDMNew.Integral()<HDstarDMOld.Integral()){
    Canvas.Clear();
    HDstarDMOld.SetLineColor(2);
    HDstarDMOld.SetFillColor(2);
    HDstarDMOld.Draw();
    HDstarDMNew.SetFillColor(1);
    HDstarDMNew.Draw("same");
    Canvas.Update();
    MoveStatsBox(&HDstarDMNew,1,1);  
    Canvas.Print(psfile);
  }
  
  Canvas.Clear();
  HDstarDMNew.Divide(&HDstarDMOld);
  HDstarDMNew.SetStats(0);
  HDstarDMNew.Draw("p");
  Canvas.Print(psfile);

  HrunNumberOld.Draw();
  Canvas.Update(); 
  MoveStatsBox(&HrunNumberOld,-1,1);  
  ((TPaveStats*)(HrunNumberOld.FindObject("stats")))->SetTextColor(2);

  Canvas.Clear();
  HrunNumberNew.SetFillColor(1);
  HrunNumberNew.Draw();
  Canvas.Update();
  MoveStatsBox(&HrunNumberNew,-1,0);
  HrunNumberOld.SetLineColor(2);
  HrunNumberOld.SetFillColor(2);
  HrunNumberOld.Draw("same");   
  Canvas.Print(psfile);

  Canvas.Print(psfile+"]");
  return;

}


void validateNewSkimKpi(){
  
  TString fileold="LogBookPlots/DstarCh_SkimValid/DATARun1Dstar_OldSkim";
  TString filenew="DstarCharged/DstarPiPi/SkimValid/Dstar/DATARun1Dstar";
  TString filenewoff="DstarCharged/DstarPiPi/SkimValid/Dstar/DATARun1OffDstar";
  
 
  TChain treeOld("ntp1");
  TreeChain(&treeOld,1,4,fileold);

  TChain treeNew("ntp1");
  TreeChain(&treeNew,1,24,filenew);
  TreeChain(&treeNew,1,2,filenewoff);

  Int_t DstarIdx;
  Int_t nDstar;
  Float_t DstarMass[100];
  Int_t Dstard1Idx[100];
  Int_t D0Idx;
  Float_t D0Mass[100];
  Int_t eventid=0;
  Int_t runNumber;
  treeNew.SetBranchStatus("*",0);
  treeNew.SetBranchAddress("runNumber",&runNumber);
  treeNew.SetBranchStatus("runNumber",1);
  treeNew.SetBranchAddress("nDstar",&nDstar);
  treeNew.SetBranchStatus("nDstar",1);
  treeNew.SetBranchAddress("DstarMass",DstarMass);
  treeNew.SetBranchStatus("DstarMass",1);
  treeNew.SetBranchAddress("Dstard1Idx",Dstard1Idx);
  treeNew.SetBranchStatus("Dstard1Idx",1);
  treeNew.SetBranchAddress("D0Mass",D0Mass);
  treeNew.SetBranchStatus("D0Mass",1);
  
 
  TH1F HDstarDMNew("HDstarDMNew","D* deltaM New",60,.139,.154);
  TH1F HrunNumberNew("HrunNumberNew","runNumber New",8000,9500,17500);
  while(treeNew.GetEntry(eventid,0)>0){   
    eventid++;    
    if(eventid%5000==0)cout<<eventid<<endl;               
    for(DstarIdx=0;DstarIdx<nDstar;DstarIdx++){
      D0Idx=Dstard1Idx[DstarIdx];            
      if(.140<DstarMass[DstarIdx]-D0Mass[D0Idx]&&DstarMass[DstarIdx]-D0Mass[D0Idx]<.153){
	HDstarDMNew.Fill(DstarMass[DstarIdx]-D0Mass[D0Idx]); 
	HrunNumberNew.Fill(runNumber);     
      }
    }
  }



  treeOld.SetBranchStatus("*",0);
  treeOld.SetBranchAddress("runNumber",&runNumber);
  treeOld.SetBranchStatus("runNumber",1);
  treeOld.SetBranchAddress("nDstar",&nDstar);
  treeOld.SetBranchStatus("nDstar",1);
  treeOld.SetBranchAddress("DstarMass",DstarMass);
  treeOld.SetBranchStatus("DstarMass",1);
  treeOld.SetBranchAddress("Dstard1Idx",Dstard1Idx);
  treeOld.SetBranchStatus("Dstard1Idx",1);
  treeOld.SetBranchAddress("D0Mass",D0Mass);
  treeOld.SetBranchStatus("D0Mass",1);
 
  TH1F HDstarDMOld("HDstarDMOld","D* deltaM Old",60,.139,.154);
  TH1F HrunNumberOld("HrunNumberOld","runNumber Old",8000,9500,17500);
  eventid=0;
  while(treeOld.GetEntry(eventid,0)>0){   
    eventid++;    
    if(eventid%5000==0)cout<<eventid<<endl;               
    for(DstarIdx=0;DstarIdx<nDstar;DstarIdx++){
      D0Idx=Dstard1Idx[DstarIdx];            
      if(.140<DstarMass[DstarIdx]-D0Mass[D0Idx]&&DstarMass[DstarIdx]-D0Mass[D0Idx]<.153){
	HDstarDMOld.Fill(DstarMass[DstarIdx]-D0Mass[D0Idx]);
	HrunNumberOld.Fill(runNumber);           
      }
    }
  }



  TCanvas Canvas("Canvas","Canvas");
  TString psfile="SkimValid.ps";
  Canvas.Print(psfile+"[");

  HDstarDMNew.Draw();
  Canvas.Update();
  MoveStatsBox(&HDstarDMNew,1,1);   

  Canvas.Clear();
  HDstarDMOld.SetLineColor(2);
  HDstarDMOld.SetFillColor(2);
  HDstarDMOld.Draw();
  Canvas.Update();
  MoveStatsBox(&HDstarDMOld,-1,1);
  HDstarDMNew.SetLineColor(1);
  HDstarDMNew.SetFillColor(1);
  HDstarDMNew.Draw("same");
  Canvas.Print(psfile);

  Canvas.Clear();
  HDstarDMNew.Divide(&HDstarDMOld);
  HDstarDMNew.SetStats(0);
  HDstarDMNew.Draw("p");
  Canvas.Print(psfile);


  HrunNumberNew.Draw();
  Canvas.Update(); 
  MoveStatsBox(&HrunNumberNew,-1,0);  

  Canvas.Clear();
  HrunNumberOld.SetLineColor(2);
  HrunNumberOld.SetFillColor(2);
  HrunNumberOld.Draw();   
  Canvas.Update();
  MoveStatsBox(&HrunNumberNew,-1,1);
  HrunNumberNew.SetLineColor(1);
  HrunNumberNew.SetFillColor(1);
  HrunNumberNew.Draw("same");
  Canvas.Print(psfile);

  Canvas.Print(psfile+"]");
  return;

}

///////////////////////////////

void validateSkimDstar(Int_t mode){

  
  TString fileold;
  TString filenew;
  TString filenewoff;
  if(mode==1){
    fileold="LogBookPlots/DstarCh_SkimValid/DATARun1Dstar_OldSkim";
    filenew="DstarCharged/DstarPiPi/DATARun1Dstar";
    filenewoff="DstarCharged/DstarPiPi/DATARun1OffDstar";
  }
  if(mode==2){
    fileold="LogBookPlots/DstarCh_SkimValid/DATARun1Dstar_K3pi_OldSkim";
    filenew="DstarCharged/DstarPiPi/DATARun1Dstar/K3Pi";
    filenewoff="DstarCharged/DstarPiPi/DATARun1OffDstar/K3Pi";
  }  
 
  TChain treeOld("ntp1");
  TreeChain(&treeOld,1,4,fileold);

  TChain treeNew("ntp1");
  TreeChain(&treeNew,1,24,filenew);
  TreeChain(&treeNew,1,2,filenewoff);


  TCanvas Canvas("Canvas","Canvas");
  TString psfile;
  if(mode==1)psfile="SkimValidDst.ps";
  if(mode==2)psfile="SkimValidDstK3Pi.ps";
  Canvas.Print(psfile+"[");
 
  TH1F HDstarDMNew("HDstarDMNew","D* deltaM New",60,.14,.155);
  TH1F HDstarDMNewBkg("HDstarDMNewBkg","D* deltaM",60,.14,.155); 
  treeNew.Draw("DstarMass-D0Mass>>HDstarDMNew");
  treeNew.Draw("DstarMass-D0Mass>>HDstarDMNewBkg","DstarMass-D0Mass<.142||.151<DstarMass-D0Mass");
 
  Canvas.Clear(); 
  TF1 fDstar("fDstar","[0]+[1]*x+[2]*x**2+[3]*x**3+[4]*x**4+[5]*x**5",.14,.155);
  HDstarDMNewBkg.Fit("fDstar");
  HDstarDMNew.GetYaxis()->SetRangeUser(0,HDstarDMNew.GetMaximum()*1.1);
  HDstarDMNew.Draw("p");
  fDstar.Draw("same");
  HDstarDMNewBkg.Draw("same");
  Canvas.Print(psfile);
  Canvas.Clear();
  TH1F*HDstarDMNewSub=(TH1F*)HDstarDMNew.Clone();
  delete HDstarDMNewSub->FindObject("stats");
  HDstarDMNewBkg.Clear();
  HDstarDMNewBkg.Reset();
  HDstarDMNewBkg.FillRandom("fDstar",(int)(fDstar.Integral(.14,.155)/HDstarDMNewBkg.GetBinWidth(1)));
  HDstarDMNewSub->Add(&HDstarDMNewBkg,-1); 
  HDstarDMNewSub->GetYaxis()->SetRangeUser(0,HDstarDMNewSub->GetMaximum()*1.1);
  HDstarDMNewSub->Draw("p");  
  Canvas.Print(psfile);



  
  TH1F HD0MassNew("HD0MassNew","D0 Mass New ",56,1.79,1.93);
  TH1F HD0MassNewBkg("HD0MassNewBkg","D0 Mass",56,1.79,1.93);
  treeNew.Draw("D0Mass>>HD0MassNew");
  treeNew.Draw("D0Mass>>HD0MassNewBkg","D0Mass<1.82||1.91<D0Mass");

  Canvas.Clear();
  TF1 fD0("fD0","[0]+[1]*x+[2]*x**2+[3]*x**3",1.79,1.93);
  HD0MassNewBkg.Fit("fD0");
  HD0MassNew.GetYaxis()->SetRangeUser(0,HD0MassNew.GetMaximum()*1.1);
  HD0MassNew.Draw("p");  
  fD0.Draw("same");
  HD0MassNewBkg.Draw("same");
  Canvas.Print(psfile);

  Canvas.Clear();
  TH1F*HD0MassNewSub=(TH1F*)HD0MassNew.Clone();
  delete HD0MassNewSub->FindObject("stats");
  HD0MassNewBkg.Clear();
  HD0MassNewBkg.Reset();
  HD0MassNewBkg.FillRandom("fD0",(int)(fD0.Integral(1.79,1.93)/HD0MassNewBkg.GetBinWidth(1)));
  HD0MassNewSub->Add(&HD0MassNewBkg,-1); 
  HD0MassNewSub->GetYaxis()->SetRangeUser(0,HD0MassNewSub->GetMaximum()*1.1);
  HD0MassNewSub->Draw("p");  
  Canvas.Print(psfile);


  //////////////////////////////
  //Fill the old skim plots
  ///////////////////////////////



  TH1F HDstarDMOld("HDstarDMOld","D* deltaM Old",60,.14,.155);
  TH1F HDstarDMOldBkg("HDstarDMOldBkg","D* deltaM",60,.14,.155); 
  treeOld.Draw("DstarMass-D0Mass>>HDstarDMOld");
  treeOld.Draw("DstarMass-D0Mass>>HDstarDMOldBkg","DstarMass-D0Mass<.142||.151<DstarMass-D0Mass");

  Canvas.Clear(); 
  HDstarDMOldBkg.Fit("fDstar");
  HDstarDMOld.GetYaxis()->SetRangeUser(0,HDstarDMOld.GetMaximum()*1.1);
  HDstarDMOld.Draw("p");
  fDstar.Draw("same");
  HDstarDMOldBkg.Draw("same");
  Canvas.Print(psfile);
  Canvas.Clear();
  TH1F*HDstarDMOldSub=(TH1F*)HDstarDMOld.Clone();
  delete HDstarDMOldSub->FindObject("stats");
  HDstarDMOldBkg.Clear();
  HDstarDMOldBkg.Reset();
  HDstarDMOldBkg.FillRandom("fDstar",(int)(fDstar.Integral(.14,.155)/HDstarDMOldBkg.GetBinWidth(1)));
  HDstarDMOldSub->Add(&HDstarDMOldBkg,-1); 
  HDstarDMOldSub->GetYaxis()->SetRangeUser(0,HDstarDMOldSub->GetMaximum()*1.1);
  HDstarDMOldSub->Draw("p");  
  Canvas.Print(psfile);
  


  
  TH1F HD0MassOld("HD0MassOld","D0 Mass Old",56,1.79,1.93);
  TH1F HD0MassOldBkg("HD0MassOldBkg","D0 Mass",56,1.79,1.93);
  treeOld.Draw("D0Mass>>HD0MassOld");
  treeOld.Draw("D0Mass>>HD0MassOldBkg","D0Mass<1.82||1.91<D0Mass");

  Canvas.Clear();
  HD0MassOldBkg.Fit("fD0");
  HD0MassOld.GetYaxis()->SetRangeUser(0,HD0MassOld.GetMaximum()*1.1);
  HD0MassOld.Draw("p");  
  fD0.Draw("same");
  HD0MassOldBkg.Draw("same");
  Canvas.Print(psfile);

  Canvas.Clear();
  TH1F*HD0MassOldSub=(TH1F*)HD0MassOld.Clone();
  delete HD0MassOldSub->FindObject("stats");
  HD0MassOldBkg.Clear();
  HD0MassOldBkg.Reset();
  HD0MassOldBkg.FillRandom("fD0",(int)(fD0.Integral(1.79,1.93)/HD0MassOldBkg.GetBinWidth(1)));
  HD0MassOldSub->Add(&HD0MassOldBkg,-1); 
  HD0MassOldSub->GetYaxis()->SetRangeUser(0,HD0MassOldSub->GetMaximum()*1.1);
  HD0MassOldSub->Draw("p");  
  Canvas.Print(psfile);
 

  Canvas.Clear();
  HD0MassNewSub->Draw("p");  
  HD0MassOldSub->Draw("same");  
  Canvas.Print(psfile);
  Canvas.Clear();
  //HD0MassNewSub->Sumw2();
  HD0MassNewSub->Add(HD0MassOldSub,-1);
  HD0MassNewSub->Divide(HD0MassOldSub);
  HD0MassNewSub->GetYaxis()->SetRangeUser(-.1,.1);
  HD0MassNewSub->Draw("p");
  Canvas.Print(psfile);


  Canvas.Clear();
  HDstarDMNewSub->Draw("p");  
  HDstarDMOldSub->Draw("same");  
  Canvas.Print(psfile);
  Canvas.Clear();
  //HDstarDMNewSub->Sumw2();
  HDstarDMNewSub->Add(HDstarDMOldSub,-1);
  HDstarDMNewSub->Divide(HDstarDMOldSub);
  HDstarDMNewSub->GetYaxis()->SetRangeUser(-.1,.1);
  HDstarDMNewSub->Draw("p");
  Canvas.Print(psfile);
  
  Canvas.Print(psfile+"]");

  delete HDstarDMNewSub;
  delete HD0MassNewSub;
  delete HDstarDMOldSub;
  delete HD0MassOldSub;

  return ;
}



void validateSkimDstarVsD0Cuts(Bool_t FullData){

  //-------------------------------------------------------
  //Dstars were reconstructed with the following cuts
  //------------------------------------------------------
  //---: prob(D*)>1e-4
  //---: 1.76<mass(D0)<1.96  : SkimCuts are at 1.8:1.93
  //---: .138<deltaM(D*)<.160
  //---: pstar(D*)>1.5


  //build the chain
  TString Dir="DstarCharged/Dstar";  
  TChain tree("ntp1");
  TreeChain(&tree,1,3,Dir+"/DATARun1Off");
  if(FullData){
    TreeChain(&tree,1,19,Dir+"/DATARun1On"); 
    
    TreeChain(&tree,1,62,Dir+"/DATARun2On");
    TreeChain(&tree,1,7,Dir+"/DATARun2Off");
    TreeChain(&tree,1,33,Dir+"/DATARun3On");
    TreeChain(&tree,1,3,Dir+"/DATARun3Off");
    TreeChain(&tree,1,99,Dir+"/DATARun4On");
    TreeChain(&tree,1,10,Dir+"/DATARun4Off");
    TreeChain(&tree,1,131,Dir+"/DATARun5On");
    TreeChain(&tree,1,14,Dir+"/DATARun5Off");
  }



  tree.SetBranchStatus("*",0);
  tree.SetBranchStatus("DstarMass",1);
  tree.SetBranchStatus("D0Mass",1);
  tree.SetBranchStatus("D0p3CM",1);
  tree.SetBranchStatus("D0d3Idx",1);
  tree.SetBranchStatus("D0d4Idx",1);
  tree.SetBranchStatus("DstarcosthCM",1);
  tree.SetBranchStatus("Dstarp3CM",1);

  TCut SignalCut=".1444<DstarMass-D0Mass&&DstarMass-D0Mass<.1464";
  TCut KpiCut="D0d3Idx==-1&&D0d4Idx==-1";
  TCut K3piCut="D0d3Idx>=0&&D0d4Idx>=0";
  TCut D0KpipCut="D0p3CM>2.4";
  TCut D0K3pipCut="D0p3CM>2.2";
  TCut DstpCut="Dstarp3CM>1.5";
    

  
  Float_t binsize=.00001;
  Float_t histmin=.138;
  Float_t histmax=.155;
  Int_t nhistbins=(int)((histmax-histmin)/binsize);


  TFile File("DstarCharged/Dstar/DstarD0MassPlots.root","recreate");

  TH1F HDstarDMNoD0Cuts("HDstarDMNoD0Cuts","D* deltaM",nhistbins,histmin,histmax);
  tree.Draw("DstarMass-D0Mass>>HDstarDMNoD0Cuts",KpiCut+DstpCut);
  HDstarDMNoD0Cuts.Write(); 

  TH1F HDstarDMD0pstarCut("HDstarDMD0pstarCut","D* deltaM",nhistbins,histmin,histmax);
  tree.Draw("DstarMass-D0Mass>>HDstarDMD0pstarCut",KpiCut+DstpCut+D0KpipCut); 
  HDstarDMD0pstarCut.Write();

  TH1F HDstarDMNoD0CutsK3pi("HDstarDMNoD0CutsK3pi","D* deltaM",nhistbins,histmin,histmax);
  tree.Draw("DstarMass-D0Mass>>HDstarDMNoD0CutsK3pi",K3piCut+DstpCut);
  HDstarDMNoD0CutsK3pi.Write();

  TH1F HDstarDMD0pstarCutK3pi("HDstarDMD0pstarCutK3pi","D* deltaM",nhistbins,histmin,histmax);
  tree.Draw("DstarMass-D0Mass>>HDstarDMD0pstarCutK3pi",K3piCut+DstpCut+D0K3pipCut);
  HDstarDMD0pstarCutK3pi.Write();




//   TH1F HDstpCM("HDstpCM","HDstpCM",500,0,5);
//   tree.Draw("Dstarp3CM>>HDstpCM",KpiCut+SignalCut);
//   TH1F HDstpCMD0pCut("HDstpCMD0pCut","HDstpCM",500,0,5);
//   tree.Draw("Dstarp3CM>>HDstpCMD0pCut",KpiCut+SignalCut+D0KpipCut);
//   HDstpCMD0pCut.SetLineColor(3);
//   TH1F HD0pCM("HD0pCM","HD0pCM",500,0,5);
//   tree.Draw("D0p3CM>>HD0pCM",KpiCut+SignalCut);
//   //HDstpCM.SetLineColor(2);
//   //HDstpCM.SetLineStyle(2);



//   TH1F HDstCos("H2DstCos","H2DstCos",200,-1.0001,1.0001);
//   tree.Draw("DstarcosthCM>>H2DstCos",KpiCut+DstpCut+SignalCut+D0KpipCut);
//   TH1F HDstCosNopCut("H2DstCosNopCut","H2DstCosNopCut",200,-1.0001,1.0001);
//   tree.Draw("DstarcosthCM>>H2DstCosNopCut",KpiCut+DstpCut+SignalCut);


//   //2D-distributions
//   TH2F H2DstCosVspCM("H2DstCosVspCM","H2DstCosVspCM",500,0,5,200,-1.0001,1.0001);
//   tree.Draw("DstarcosthCM:Dstarp3CM>>H2DstCosVspCM",KpiCut+DstpCut+SignalCut+D0KpipCut);

//   TH2F H2DstCosVspCMNopCut("H2DstCosVspCMNopCut","H2DstCosVspCMNopCut",500,0,5,200,-1.0001,1.0001);
//   tree.Draw("DstarcosthCM:Dstarp3CM>>H2DstCosVspCMNopCut",KpiCut+DstpCut+SignalCut);



  File.ls();
  File.Close();


  return;
}



void validateSkimDstarVsD0CutsFit(){

  TStyle* gStyle=gROOT->GetStyle("myStyle");
  Int_t optstat=gStyle->GetOptStat();
  gStyle->SetOptStat("i");


  TCanvas Canvas("Canvas","Canvas");
  TString psfile="DstarCharged/Dstar/CountDstarsKpi.ps";
  Canvas.Print(psfile+"[");


  Float_t binsize=.00001;
  Float_t histmin=.138;
  Float_t histmax=.155;
  Int_t nhistbins=(int)((histmax-histmin)/binsize);


  //Fit Function for the D* deltaM spectrum
//   Float_t fitmin=.1395;
//   Float_t fitmax=.155;
//   TF1 Signal;
//   TF1 Bkg;
//   TF1* FitFunc;



  TFile File("DstarCharged/Dstar/DstarD0MassPlots.root","read");
  TH1F*HDstarDMNoD0Cuts=(TH1F*)File.Get("HDstarDMNoD0Cuts");
  //FitFunc=FitHistogramDstarDM(HDstarDMNoD0Cuts,fitmin,fitmax,&Signal,&Bkg); 
  //cout<<"Kpi Yield No D0 p* Cut= "<<Signal.Integral(fitmin,fitmax)/binsize<<endl;
  Canvas.Clear();
  SetHistoXYLabels(HDstarDMNoD0Cuts,"m(K#pi#pi)-m(K#pi)","GeV/c^{2}");
  HDstarDMNoD0Cuts->Draw("pe");
  //Bkg.Draw("same");
  Canvas.Print(psfile);  
  //TF1*BkgClone=(TF1*)Bkg.Clone();



  TH1F*HDstarDMD0pstarCut=(TH1F*)File.Get("HDstarDMD0pstarCut");  
  //FitFunc=FitHistogramDstarDM(HDstarDMD0pstarCut,fitmin,fitmax,&Signal,&Bkg); 
  //cout<<"Kpi Yield With D0 p* Cut= "<<Signal.Integral(fitmin,fitmax)/binsize<<endl; 
  Canvas.Clear();
  SetHistoXYLabels(HDstarDMD0pstarCut,"m(K#pi#pi)-m(K#pi)","GeV/c^{2}");
  HDstarDMD0pstarCut->Draw("pe");
  //Bkg.Draw("same");
  Canvas.Print(psfile);

  //comparison
  Canvas.Clear();
  HDstarDMNoD0Cuts->Draw("pe");
  //BkgClone->Draw("same");
  HDstarDMD0pstarCut->Draw("pesame");
  //Bkg.Draw("same"); 
  Canvas.Print(psfile);
  TH1F HDstarRatio("HDstarRatio","D* deltaM",nhistbins,histmin,histmax);
  Canvas.Clear();
  HDstarRatio.Divide(HDstarDMNoD0Cuts,HDstarDMD0pstarCut,1,1,"");
  HDstarRatio.GetYaxis()->SetRangeUser(1,6);
  HDstarRatio.Draw("p");
  Canvas.Print(psfile);
  

  TH1F*HDstarDMNoD0CutsK3pi=(TH1F*)File.Get("HDstarDMNoD0CutsK3pi");
  //FitFunc=FitHistogramDstarDM(HDstarDMNoD0CutsK3pi,fitmin,fitmax,&Signal,&Bkg); 
  //cout<<"K3pi Yield No D0 p* Cut= "<<Signal.Integral(fitmin,fitmax)/binsize<<endl;
  Canvas.Clear();
  SetHistoXYLabels(HDstarDMNoD0CutsK3pi,"m(K#pi#pi#pi#pi)-m(K#pi#pi#pi)","GeV/c^{2}");
  HDstarDMNoD0CutsK3pi->Draw("pe");
  //Bkg.Draw("same");
  Canvas.Print(psfile);  
  //BkgClone=(TF1*)Bkg.Clone();


  TH1F*HDstarDMD0pstarCutK3pi=(TH1F*)File.Get("HDstarDMD0pstarCutK3pi");
  //FitFunc=FitHistogramDstarDM(HDstarDMD0pstarCutK3pi,fitmin,fitmax,&Signal,&Bkg);   
  //cout<<"K3pi Yield With D0 p* Cut= "<<Signal.Integral(fitmin,fitmax)/binsize<<endl;
  Canvas.Clear();
  SetHistoXYLabels(HDstarDMD0pstarCutK3pi,"m(K#pi#pi#pi#pi)-m(K#pi#pi#pi)","GeV/c^{2}");
  HDstarDMD0pstarCutK3pi->Draw("pe");
  //Bkg.Draw("same");
  Canvas.Print(psfile);

  Canvas.Clear();
  HDstarDMNoD0CutsK3pi->Draw("pe");
  //BkgClone->Draw("same");  
  HDstarDMD0pstarCutK3pi->Draw("pesame");
  //Bkg.Draw("same");  
  Canvas.Print(psfile);
  TH1F HDstarRatioK3pi("HDstarRatioK3pi","D* deltaM",nhistbins,histmin,histmax);
  Canvas.Clear();
  HDstarRatioK3pi.Divide(HDstarDMNoD0CutsK3pi,HDstarDMD0pstarCutK3pi,1,1,"");
  HDstarRatioK3pi.GetYaxis()->SetRangeUser(1,6);
  HDstarRatioK3pi.Draw("p");
  Canvas.Print(psfile);

  
  
//   ///make plot of D* angular distribution
  
//   //D0 and D*  p*
//   Float_t pfitmin=1.75;
//   Float_t pfitmax=4.6;
//   TF1 FDpccb("FDpccb",(const char*)("(pow(abs(x-5),.5))*([0]*exp(-.5*(x-[1])**2/[2]**2))"),0,pfitmax);
//   FDpccb.SetLineColor(3); 
//   FDpccb.SetLineStyle(2);
//   TF1 FDp("FDp",(const char*)(FDpccb.GetExpFormula()+"+([3]*exp(-(x-[4])/[5]))"),pfitmin,pfitmax);
//   FDp.SetLineColor(4);
//   FDp.SetParLimits(0,1,1e6);
//   FDp.SetParLimits(1,2.8,4.0); 
//   FDp.SetParLimits(2,.5,1.5);
//   FDp.SetParLimits(3,1,1e6);
//   FDp.SetParLimits(4,-5,5);
//   //FDp.SetParLimits(5,.1,2);  
//   FDp.SetParLimits(5,.1,5);



//   //HDstpCM.Fit("FDp","","",pfitmin,pfitmax);
//   //FDpccb.SetParameters(FDp.GetParameter(0),FDp.GetParameter(1),FDp.GetParameter(2));
//   Canvas.Clear();
//   HDstpCM.Draw();
//   Canvas.Update();
//   MoveStatsBox(&HDstpCM,1,1);
//   HDstpCMD0pCut.Draw("same");
//   //FDpccb.Draw("same");
//   Canvas.Print(psfile);

//   //HD0pCM.Fit("FDp","","",pfitmin,pfitmax);  
//   //FDpccb.SetParameters(FDp.GetParameter(0),FDp.GetParameter(1),FDp.GetParameter(2));
//   Canvas.Clear();
//   HD0pCM.Draw();
//   Canvas.Update();
//   MoveStatsBox(&HD0pCM,1,1);
//   //FDpccb.Draw("same");
//   Canvas.Print(psfile);

 
//   Canvas.Clear();
//   HDstCosNopCut.Draw();
//   HDstCos.Draw("same");
//   Canvas.Print(psfile);
   
//   Canvas.Clear();
//   Canvas.Divide(1,2);
//   Canvas.cd(1);
//   H2DstCosVspCM.Draw("colz");
//   Canvas.cd(2);
//   H2DstCosVspCMNopCut.Draw("colz");
//   Canvas.Print(psfile);
  

  File.Close();

  Canvas.Print(psfile+"]");

  gStyle->SetOptStat(optstat);//restore Stats


}






/////////////D0 cannot be compared because there are different cuts at the D* level

void validateSkimD0(Int_t mode){
  
  TString fileold;
  TString filenew;
  TString filenewoff;
  if(mode==1){
    fileold="LogBookPlots/DstarCh_SkimValid/DATARun1D0_OldSkim";
    filenew="DstarCharged/DstarPiPi/DATARun1D0";
    filenewoff="DstarCharged/DstarPiPi/DATARun1OffD0";
  }
  if(mode==2){
    fileold="LogBookPlots/DstarCh_SkimValid/DATARun1D0_K3pi_OldSkim";
    filenew="DstarCharged/DstarPiPi/DATARun1D0/K3pi";
    filenewoff="DstarCharged/DstarPiPi/DATARun1OffD0/K3pi";
  }  
 
  TChain treeOld("ntp1");
  TreeChain(&treeOld,1,4,fileold);

  TChain treeNew("ntp1");
  TreeChain(&treeNew,1,24,filenew);
  TreeChain(&treeNew,1,2,filenewoff);


  TCanvas Canvas("Canvas","Canvas");
  TString psfile;
  if(mode==1)psfile="SkimValidD0.ps";
  if(mode==2)psfile="SkimValidD0K3Pi.ps";
  Canvas.Print(psfile+"[");

  
  TH1F HD0MassNew("HD0MassNew","D0 Mass New ",56,1.79,1.93);
  TH1F HD0MassNewBkg("HD0MassNewBkg","D0 Mass",56,1.79,1.93);
  treeNew.Draw("D0Mass>>HD0MassNew");
  treeNew.Draw("D0Mass>>HD0MassNewBkg","D0Mass<1.82||1.91<D0Mass");

  Canvas.Clear();
  TF1 fD0("fD0","[0]+[1]*x+[2]*x**2+[3]*x**3",1.79,1.93);
  HD0MassNewBkg.Fit("fD0");
  HD0MassNew.GetYaxis()->SetRangeUser(0,HD0MassNew.GetMaximum()*1.1);
  HD0MassNew.Draw("p");  
  fD0.Draw("same");
  HD0MassNewBkg.Draw("same");
  Canvas.Print(psfile);

  Canvas.Clear();
  TH1F*HD0MassNewSub=(TH1F*)HD0MassNew.Clone();
  delete HD0MassNewSub->FindObject("stats");
  HD0MassNewBkg.Clear();
  HD0MassNewBkg.Reset();
  HD0MassNewBkg.FillRandom("fD0",(int)(fD0.Integral(1.79,1.93)/HD0MassNewBkg.GetBinWidth(1)));
  HD0MassNewSub->Add(&HD0MassNewBkg,-1); 
  HD0MassNewSub->GetYaxis()->SetRangeUser(0,HD0MassNewSub->GetMaximum()*1.1);
  HD0MassNewSub->Draw("p");  
  Canvas.Print(psfile);


  //////////////////////////////
  //Fill the old skim plots
  ///////////////////////////////
  
  
  TH1F HD0MassOld("HD0MassOld","D0 Mass Old",56,1.79,1.93);
  TH1F HD0MassOldBkg("HD0MassOldBkg","D0 Mass",56,1.79,1.93);
  treeOld.Draw("D0Mass>>HD0MassOld");
  treeOld.Draw("D0Mass>>HD0MassOldBkg","D0Mass<1.82||1.91<D0Mass");

  Canvas.Clear();
  HD0MassOldBkg.Fit("fD0");
  HD0MassOld.GetYaxis()->SetRangeUser(0,HD0MassOld.GetMaximum()*1.1);
  HD0MassOld.Draw("p");  
  fD0.Draw("same");
  HD0MassOldBkg.Draw("same");
  Canvas.Print(psfile);

  Canvas.Clear();
  TH1F*HD0MassOldSub=(TH1F*)HD0MassOld.Clone();
  delete HD0MassOldSub->FindObject("stats");
  HD0MassOldBkg.Clear();
  HD0MassOldBkg.Reset();
  HD0MassOldBkg.FillRandom("fD0",(int)(fD0.Integral(1.79,1.93)/HD0MassOldBkg.GetBinWidth(1)));
  HD0MassOldSub->Add(&HD0MassOldBkg,-1); 
  HD0MassOldSub->GetYaxis()->SetRangeUser(0,HD0MassOldSub->GetMaximum()*1.1);
  HD0MassOldSub->Draw("p");  
  Canvas.Print(psfile);
  
  
  Canvas.Clear();
  HD0MassNewSub->Draw("p");  
  HD0MassOldSub->Draw("same");  
  Canvas.Print(psfile);
  Canvas.Clear();
  //HD0MassNewSub->Sumw2();
  HD0MassNewSub->Add(HD0MassOldSub,-1);
  HD0MassNewSub->Divide(HD0MassOldSub);
  HD0MassNewSub->GetYaxis()->SetRangeUser(-.1,.1);
  HD0MassNewSub->Draw("p");
  Canvas.Print(psfile);

  Canvas.Print(psfile+"]");
  
  delete HD0MassNewSub;
  delete HD0MassOldSub;
//   delete &HD0MassNewBkg;
//   delete &HD0MassOldBkg;

  return ;
}

