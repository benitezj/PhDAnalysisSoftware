#include "DstPiAnalysis.h"
#include "../../FUNCTIONS/Fits.h"

TH1* DstPiAnalysis::K3piDstarBkg(Int_t nbins,Float_t min,Float_t max){
  cout<<"Going to calculate the D* background"<<endl;
 
  //define the cuts on the D* sidebands
  Float_t dmstep=database.GetDstarDeltaMMax()-database.GetDstarDeltaMMin();
  sprintf(textlabel,"%.5f<dstardeltam&&dstardeltam<%.5f",database.GetDstarDeltaMMax()+1*dmstep,database.GetDstarDeltaMMax()+2*dmstep);
  TCut highdstarcut=TCut(textlabel);
  sprintf(textlabel,"%.5f<dstardeltam&&dstardeltam<%.5f",database.GetDstarDeltaMMin()-2*dmstep,database.GetDstarDeltaMMin()-dmstep);
  TCut lowdstarcut=TCut(textlabel);

  //------------------------------------------
  //Open the K3pi root file extract the Ntuple and make a clean copy
  //-------------------------------
  TFile f(_OutputDir+"/../K3pi/Reduced.root");
  if(f.IsZombie()){cout<<"bad file name"<<endl;return 0;}
  gROOT->cd();
  TTree* t0=(TTree*)f.Get("Ntuple");
  if(!t0){cout<<"bad tree"<<endl;return 0;}
  TTree* t=NULL;
  t=t0->CopyTree(database.GetCut(1,3)+database.GetCut(5,100));//dont include D* deltaM cut
  f.Close();
  if(!t)return NULL;
  cout<<" The new tree has "<<t->GetEntries()<<" entries"<<endl;



  //------------------------
  t->SetBranchStatus("*",0);
  t->SetBranchStatus("dstardeltam",1);
  t->SetBranchStatus("dstpideltam",1);
  
  Int_t ndstarbins=database.GetDstarDeltaMRangeNbins()*4;
  Float_t dstarmin=database.GetDstarDeltaMRangeMin();
  Float_t dstarmax=database.GetDstarDeltaMRangeMax();
 
  TH1F hdstar("hdstar","hdstar",ndstarbins,dstarmin,dstarmax);
  t->Draw("dstardeltam>>hdstar");
  //return (TH1F*)hdstar.Clone();

  TF1 sig;
  TF1 bkg;
  TF1* fitF=FitHistogramDstarDM(&hdstar,.140,dstarmax,&sig,&bkg); 
  if(!fitF) return NULL;
  Float_t scalef=bkg.Integral(database.GetDstarDeltaMMin(),database.GetDstarDeltaMMax())/hdstar.GetBinWidth(1);
  cout<<scalef<<" K3pi D* bkg integral"<<endl;
  
  
  TH1F HBkgLow("HBkgLow","HBkgLow",nbins,min,max); 
  t->Draw("dstpideltam+2.01>>HBkgLow",lowdstarcut);
  HBkgLow.SetNormFactor(scalef);
 
  TH1F HBkgHigh("HBkgHigh","HBkgHigh",nbins,min,max); 
  t->Draw("dstpideltam+2.01>>HBkgHigh",highdstarcut);
  HBkgHigh.SetNormFactor(scalef);

  TH1*HFinalBkg=(TH1*)HBkgLow.Clone();
  HFinalBkg->Add(&HBkgHigh);
  HFinalBkg->SetNormFactor(scalef);


  delete t;  
  delete fitF;

  return HFinalBkg;
}


TH1F* DstPiAnalysis::K3piDstarBkgEffWeighted(Int_t nbins,Float_t min,Float_t max){
  
  TFile* File1=new TFile(_OutputDir+"/../K3pi/LegendreWeightedEventsLowBkg.root","read");  
  gROOT->cd();
  TFile* File2=new TFile(_OutputDir+"/../K3pi/LegendreWeightedEventsHighBkg.root","read");
  gROOT->cd();  

  TH1F* H1=(TH1F*)File1->Get("HDstPiM");
  if(!H1)return 0;
  H1=CleanAndRebinHisto(H1,nbins,min,max);
  if(!H1)return 0;

  TH1F* H2=(TH1F*)File2->Get("HDstPiM");
  if(!H2)return 0;
  H2=CleanAndRebinHisto(H2,nbins,min,max);
  if(!H2)return 0;
  
  //average the 2
  H2->Add(H1);
  H2->Scale(.5);


  //determine quantity under D* peak
  TH1F* hdstar=(TH1F*)File1->Get("HDstarDeltaM;1");
  if(!hdstar){cout<<" no Dstar Histo"<<endl;return 0;}
  TF1 sig;
  TF1 bkg;
  TF1* fitF=FitHistogramDstarDM(hdstar,.140,.155,&sig,&bkg); 
  if(!fitF) return NULL;
  cout<<"Fit chisquare "<<fitF->GetChisquare()/fitF->GetNDF()<<endl;
  delete fitF;
  Float_t scalef=bkg.Integral(database.GetDstarDeltaMMin(),database.GetDstarDeltaMMax())/hdstar->GetBinWidth(1);
  cout<<scalef<<" K3pi D* bkg integral"<<endl;
  

  H2->SetNormFactor(scalef);  

  
  delete H1;
  delete File1;
  delete File2;
  
  
  return H2;
}
