#include "../Globals.h"

void MergeD0Modes(TString dir){
  //merge the Reduced.root and ReducedClean.root ntuples for any dataset
  TFile fclean(dir+"/KpiAndK3pi/ReducedClean.root","RECREATE");
  TChain CleanChain("CleanNtuple","CleanNtuple");
  CleanChain.Add(dir+"/Kpi/ReducedClean.root");
  CleanChain.Add(dir+"/K3pi/ReducedClean.root");
  CleanChain.Write();
  fclean.ls();
  fclean.Close();


  TFile f(dir+"/KpiAndK3pi/Reduced.root","RECREATE");
  TChain Chain("Ntuple","Ntuple");
  Chain.Add(dir+"/Kpi/Reduced.root");
  Chain.Add(dir+"/K3pi/Reduced.root");
  Chain.Write();
  f.ls();
  f.Close();

}

#include <TH3F.h>
Int_t  MergeSigMC(Int_t Mode){
  ///merge the clean ntuples and Gen MC histos for Signal MC.


  TString mode;
  if(Mode==1)mode="Kpi";
  if(Mode==2)mode="K3pi";

  TFile fn(TString("DstarCharged/DstarPi/MC/D2420andD2460/")+mode+"/ReducedClean.root","recreate");  
  TChain c("CleanNtuple","CleanNtuple");
  c.Add(TString("DstarCharged/DstarPi/MC/D2420/")+mode+"/ReducedClean.root");
  c.Add(TString("DstarCharged/DstarPi/MC/D2460/")+mode+"/ReducedClean.root");
  c.Write();
  fn.ls();
  fn.Close();



  TFile f1(TString("DstarCharged/DstarPi/MC/D2420/")+mode+"/Reduced.root","read");
  TFile f2(TString("DstarCharged/DstarPi/MC/D2460/")+mode+"/Reduced.root","read");
  TFile f(TString("DstarCharged/DstarPi/MC/D2420andD2460/")+mode+"/Reduced.root","recreate");  


  TH1F* hdm1=(TH1F*)f1.Get("HMCXMass");
  if(!hdm1)return 0;
  TH1F* hdm2=(TH1F*)f2.Get("HMCXMass");
  if(!hdm2)return 0;
  hdm1=(TH1F*)hdm1->Clone();
  hdm1->Add(hdm2); 
  hdm1->Write();

  TH3F* h31=(TH3F*)f1.Get("H3MCPvsThetaVsDstAngle");
  if(!h31)return 0;
  TH3F* h32=(TH3F*)f2.Get("H3MCPvsThetaVsDstAngle");
  if(!h32)return 0;
  h31=(TH3F*)h31->Clone();
  h31->Add(h32); 
  h31->Write();   


  h31=(TH3F*)f1.Get("H3MCPvsMassVsDstAngle");
  if(!h31)return 0;
  h32=(TH3F*)f2.Get("H3MCPvsMassVsDstAngle");
  if(!h32)return 0;
  h31=(TH3F*)h31->Clone();
  h31->Add(h32); 
  h31->Write();   


  
  h31=(TH3F*)f1.Get("H3MCPvsMassVsHelicity");
  if(!h31)return 0;
  h32=(TH3F*)f2.Get("H3MCPvsMassVsHelicity");
  if(!h32)return 0;
  h31=(TH3F*)h31->Clone();
  h31->Add(h32); 
  h31->Write();  


  TH2F* h21=(TH2F*)f1.Get("H2MCPvsMass");
  if(!h21)return 0;
  TH2F* h22=(TH2F*)f2.Get("H2MCPvsMass");
  if(!h22)return 0;
  h21=(TH2F*)h21->Clone();
  h21->Add(h22); 
  h21->Write();   

  h21=(TH2F*)f1.Get("H2MCXCMPvsTheta");
  if(!h21)return 0;
  h22=(TH2F*)f2.Get("H2MCXCMPvsTheta");
  if(!h22)return 0;
  h21=(TH2F*)h21->Clone();
  h21->Add(h22); 
  h21->Write();   


  TH1F* hp1=(TH1F*)f1.Get("HMCXp3CM");
  if(!hp1)return 0;
  TH1F* hp2=(TH1F*)f2.Get("HMCXp3CM");
  if(!hp2)return 0;
  hp1=(TH1F*)hp1->Clone();
  hp1->Add(hp2); 
  hp1->Write();   

  TH1F* hc1=(TH1F*)f1.Get("HMCXcosthCM");
  if(!hc1)return 0;
  TH1F* hc2=(TH1F*)f2.Get("HMCXcosthCM");
  if(!hc2)return 0;
  hc1=(TH1F*)hc1->Clone();
  hc1->Add(hc2); 
  hc1->Write(); 

  TH1F* hhel1=(TH1F*)f1.Get("HMCDstarHelicity");
  if(!hhel1)return 0;
  TH1F* hhel2=(TH1F*)f2.Get("HMCDstarHelicity");
  if(!hhel2)return 0;
  hhel1=(TH1F*)hhel1->Clone();
  hhel1->Add(hhel2); 
  hhel1->Write();   

  TH1F* hdst1=(TH1F*)f1.Get("HMCDstarAngle");
  if(!hdst1)return 0;
  TH1F* hdst2=(TH1F*)f2.Get("HMCDstarAngle");
  if(!hdst2)return 0;
  hdst1=(TH1F*)hdst1->Clone();
  hdst1->Add(hdst2); 
  hdst1->Write();   




  f.ls();
  f.Close();
  f1.Close();
  f2.Close();


  cout<<"Succeded in merging the files"<<endl;

  return 1;
}
 

void MergeEmbededSignal(){
  //merge the ccbar generic with signal ccbar for "embeded" studies

  TFile f1("DstarCharged/DstarPi/MC/SP-1005/Kpi/LegendreWeightedHistos.root","read"); 
  f1.ls();  
  TFile f2("DstarCharged/DstarPi/MC/D2420/Kpi/LegendreWeightedHistos.root","read"); 
  f2.ls();
  TFile f3("DstarCharged/DstarPi/MC/D2460/Kpi/LegendreWeightedHistos.root","read"); 
  f3.ls();
  TFile f("DstarCharged/DstarPi/MC/SP-1005_SigEmbed/Kpi/LegendreWeightedHistos.root","recreate");  
  
  TH1F* h1=(TH1F*)f1.Get("HDstPiMFine");
  if(!h1)return;
  TH1F* h2=(TH1F*)f2.Get("HDstPiMFine");
  if(!h2)return;
  TH1F* h3=(TH1F*)f3.Get("HDstPiMFine");
  if(!h3)return;
  for(Int_t i=1;i<=h3->GetNbinsX();i++){
    //approx ratio D2460/D2420 from data
    Float_t sigratio=.5;
    h3->SetBinContent(i,h3->GetBinContent(i)*sigratio);
    h3->SetBinError(i,h3->GetBinError(i)*sqrt(sigratio));
  }

  //
  h1=(TH1F*)h1->Clone();

  //add the D2420
  //h1->Add(h2); 
  
  TF1 FD2420("FD2420","1/((x-[0])**2+(.5*[1])**2)",2.1,3.1);
  FD2420.SetParameters(2.422,.030);
  TH1F hD2420("hD2420","hD2420",h1->GetNbinsX(),h1->GetXaxis()->GetXmin(),h1->GetXaxis()->GetXmax());
  hD2420.FillRandom("FD2420",(int)(1.4*h2->Integral()/5.));//scale to get proper error bars 1/5~ efficiency, 1.4 gives corr sig/bkg
  hD2420.Scale(5);  
  h1->Add(&hD2420);




  //add the D2460
  //h1->Add(h3);

  TF1 FD2460("FD2460","1/((x-[0])**2+(.5*[1])**2)",2.1,3.1);
  FD2460.SetParameters(2.459,.045);
  TH1F hD2460("hD2460","hD2460",h1->GetNbinsX(),h1->GetXaxis()->GetXmin(),h1->GetXaxis()->GetXmax());
  hD2460.FillRandom("FD2460",(int)(1.4*h3->Integral()/5.));//scale to get proper error bars 1/5~ efficiency
  hD2460.Scale(5);  
  h1->Add(&hD2460);

 

  //add extra bump
  TF1 Fbump("Fbump","1/((x-[0])**2+(.5*[1])**2)",2.1,3.1);
  Fbump.SetParameters(2.620,.150);
  TH1F hbump("hbump","hbump",h1->GetNbinsX(),h1->GetXaxis()->GetXmin(),h1->GetXaxis()->GetXmax());
  hbump.FillRandom("Fbump",(int)(h2->Integral()/5.));//scale to get proper error bars
  hbump.Scale(5);  
  h1->Add(&hbump);


  ///scale down to data
//   for(Int_t i=1;i<=h1->GetNbinsX();i++){//this scaling is not right one needs to stop the split ntuple 
//     Float_t datamcratio=.57*(9./14.);//lumi * (approx scale factor from data)
//     h1->SetBinContent(i,h1->GetBinContent(i)*datamcratio);
//     h1->SetBinError(i,h1->GetBinError(i)*sqrt(datamcratio));
//   }

  
  h1->Write();   
  
  f.ls();
  f.Close();
  f1.Close();
  f2.Close();


  cout<<"Succeded in merging the files"<<endl;

}




// void MergeLegendreMassProjs(TString dir){


//   TFile f1(dir+"/Kpi/LegendreMassProjs.root","read");
//   TFile f2(dir+"/K3pi/LegendreMassProjs.root","read");
//   TFile f(dir+"/KpiAndK3pi/LegendreMassProjs.root","recreate");  
  
//   TH1F* h1=(TH1F*)f1.Get("H2DstPiMVsDstarHel_LegendreProj2");
//   if(!h1)return;
//   TH1F* h2=(TH1F*)f2.Get("H2DstPiMVsDstarHel_LegendreProj2");
//   if(!h2)return;
//   h1=(TH1F*)h1->Clone();
//   h1->Add(h2); 
//   h1->Write();   
  

//   f.Close();
//   f1.Close();
//   f2.Close();


//   cout<<"Succeded in merging the files"<<endl;

// }

// void MergeLegendreEvents(TString dir){


//   TFile f1(dir+"/Kpi/LegendreWeightedHistos.root","read");
//   f1.ls();
//   TFile f2(dir+"/K3pi/LegendreWeightedHistos.root","read");
//   f2.ls();
//   TFile f(dir+"/KpiAndK3pi/LegendreWeightedHistos.root","recreate");  
  
//   //The total mass distribution
//   TH1F* h1=(TH1F*)f1.Get("HDstPiM");
//   if(!h1)return;
//   TH1F* h2=(TH1F*)f2.Get("HDstPiM");
//   if(!h2)return;
//   h1=(TH1F*)h1->Clone();
//   h1->Add(h2); 
//   h1->Write();   
//   delete h1;
//   cout<<"Done with total mass"<<endl;


//   //The total mass distribution
//   h1=(TH1F*)f1.Get("HDstPiMFine");
//   if(!h1)return;
//   h2=(TH1F*)f2.Get("HDstPiMFine");
//   if(!h2)return;
//   h1=(TH1F*)h1->Clone();
//   h1->Add(h2); 
//   h1->Write();   
//   delete h1;
//   cout<<"Done with total mass"<<endl;



//   //Mass vs helicity
//   TH2F* h21=(TH2F*)f1.Get("H2DstPiMVsDstarHel");
//   if(!h21)return;
//   TH2F* h22=(TH2F*)f2.Get("H2DstPiMVsDstarHel");
//   if(!h22)return;
//   h21=(TH2F*)h21->Clone();
//   h21->Add(h22); 
//   h21->Write();   
//   delete h21;
//   cout<<"Done with mass vs helicity"<<endl;


//   //Mass vs helicity
//   h21=(TH2F*)f1.Get("H2DstPiMVsDstarHelFine");
//   if(!h21)return;
//   h22=(TH2F*)f2.Get("H2DstPiMVsDstarHelFine");
//   if(!h22)return;
//   h21=(TH2F*)h21->Clone();
//   h21->Add(h22); 
//   h21->Write();   
//   delete h21;
//   cout<<"Done with mass vs helicity"<<endl;


//   ///the 2nd lengendre projections
//   for(Int_t i=0;i<5;i++){
//     h1=(TH1F*)f1.Get(TString("HDstPiMLegendre")+(long)i);
//     if(!h1)return;
//     h2=(TH1F*)f2.Get(TString("HDstPiMLegendre")+(long)i);
//     if(!h2)return;
//     h1=(TH1F*)h1->Clone();
//     h1->Add(h2); 
//     h1->Write();   
//     delete h1;
//   }

//   cout<<"Done with legendre projections"<<endl;

//   //merge the yield vs helicity
//   for(Int_t i=0;i<10;i++){
//     h1=(TH1F*)f1.Get(TString("HDstPiMVsHelicity")+(long)i);
//     h2=(TH1F*)f2.Get(TString("HDstPiMVsHelicity")+(long)i);
//     if(!h1||!h2)return;
//     h1=(TH1F*)h1->Clone();
//     h1->Add(h2); 
//     h1->Write();   
//     delete h1; 
//   }
//   cout<<"Done with yield vs helicity "<<endl;

//   //folded
//   for(Int_t i=0;i<5;i++){
//     h1=(TH1F*)f1.Get(TString("HDstPiMVsHelicityFolded")+(long)i);
//     h2=(TH1F*)f2.Get(TString("HDstPiMVsHelicityFolded")+(long)i);
//     if(!h1||!h2)return;
//     h1=(TH1F*)h1->Clone();
//     h1->Add(h2); 
//     h1->Write();   
//     delete h1; 
//   }
//   cout<<"Done with yield vs helicity folded"<<endl; 


//   //merge the yield vs helicity fine binning
//   for(Int_t i=0;i<20;i++){    
//     h1=(TH1F*)f1.Get(TString("HDstPiMVsHelicityFine")+(long)i+";1");
//     h2=(TH1F*)f2.Get(TString("HDstPiMVsHelicityFine")+(long)i+";1");
//     if(!h1||!h2)return;
//     h1=(TH1F*)h1->Clone();
//     h1->Add(h2); 
//     h1->Write();   
//     delete h1; 
//   }
//   cout<<"Done with yield vs helicity fine "<<endl;
  


//   //merge the yield vs helicity fine binning
//   for(Int_t i=0;i<10;i++){    
//     h1=(TH1F*)f1.Get(TString("HDstPiMVsHelicityFineFolded")+(long)i+";1");
//     h2=(TH1F*)f2.Get(TString("HDstPiMVsHelicityFineFolded")+(long)i+";1");
//     if(!h1||!h2)return;
//     h1=(TH1F*)h1->Clone();
//     h1->Add(h2); 
//     h1->Write();   
//     delete h1; 
//   }
//   cout<<"Done with yield vs helicity fine folded"<<endl;
  


  
//   cout<<" The following histos were written ---------------------"<<endl<<endl<<endl;
//   f.ls();
//   f.Close();
//   f1.Close();
//   f2.Close();

  
//   cout<<"Succeded in merging the files"<<endl;

// }
