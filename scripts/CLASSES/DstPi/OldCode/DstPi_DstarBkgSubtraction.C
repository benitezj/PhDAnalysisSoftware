#include "../Globals.h"
#include "../FUNCTIONS/Fits.h"
#include "../CLASSES/JDataBaseDstPi.h"
#include <TCut.h>
#include <TLine.h>
#include "../FUNCTIONS/CalculateIntegral.C"
#include "../CLASSES/JResiduals.h"

TH1* DstPi_DstarBkgSubtraction(Int_t nbins,Float_t min,Float_t max){

  JDataBaseDstPi database(0,0);
  char sometext[100];  
  Float_t dmstep=database.GetDstarDeltaMMax()-database.GetDstarDeltaMMin();
  sprintf(sometext,"%.5f<dstardeltam&&dstardeltam<%.5f",database.GetDstarDeltaMMin(),database.GetDstarDeltaMMax());
  TCut dstarcut0=TCut(sometext);
  sprintf(sometext,"%.5f<dstardeltam&&dstardeltam<%.5f",database.GetDstarDeltaMMax(),database.GetDstarDeltaMMax()+1*dmstep);
  TCut dstarcut1=TCut(sometext);
  sprintf(sometext,"%.5f<dstardeltam&&dstardeltam<%.5f",database.GetDstarDeltaMMax()+1*dmstep,database.GetDstarDeltaMMax()+2*dmstep);
  TCut dstarcut2=TCut(sometext);
  sprintf(sometext,"%.5f<dstardeltam&&dstardeltam<%.5f",database.GetDstarDeltaMMax()+2*dmstep,database.GetDstarDeltaMMax()+3*dmstep);
  TCut dstarcut3=TCut(sometext);
  sprintf(sometext,"%.5f<dstardeltam&&dstardeltam<%.5f",database.GetDstarDeltaMMax()+3*dmstep,database.GetDstarDeltaMMax()+4*dmstep);
  TCut dstarcut4=TCut(sometext);
  sprintf(sometext,"%.5f<dstardeltam&&dstardeltam<%.5f",database.GetDstarDeltaMMax()+4*dmstep,database.GetDstarDeltaMMax()+5*dmstep);
  TCut dstarcut5=TCut(sometext);
  sprintf(sometext,"%.5f<dstardeltam&&dstardeltam<%.5f",database.GetDstarDeltaMMax()+5*dmstep,database.GetDstarDeltaMMax()+6*dmstep);
  TCut dstarcut6=TCut(sometext);
  sprintf(sometext,"%.5f<dstardeltam&&dstardeltam<%.5f",database.GetDstarDeltaMMin()-dmstep,database.GetDstarDeltaMMin());
  TCut dstarcut7=TCut(sometext);
  sprintf(sometext,"%.5f<dstardeltam&&dstardeltam<%.5f",database.GetDstarDeltaMMin()-2*dmstep,database.GetDstarDeltaMMin()-dmstep);
  TCut dstarcut8=TCut(sometext);




  TFile f("DstarCharged/DstarPi/K3pi/Reduced.root");
  gROOT->cd();
  TTree* t0=(TTree*)f.Get("CleanNtuple");
  TTree* t=NULL;
  t=t0->CopyTree(database.GetCut(1,3)+database.GetCut(5,100));
  if(!t)return NULL;
  f.Close();

  TCanvas C("C","C");
  TString filename="DstarCharged/DstarPi/K3pi/DstarBkg.ps";
  C.Print(filename+"[");

  //plot the dstar mass 
  Int_t ndstarbins=200*4;
  Float_t dstarmin=.139;
  Float_t dstarmax=.159;

  TH1F hdstar8("hdstar8","hdstar8",ndstarbins,dstarmin,dstarmax);
  TH1F hdstar7("hdstar7","hdstar7",ndstarbins,dstarmin,dstarmax);
  TH1F hdstar("hdstar","hdstar",ndstarbins,dstarmin,dstarmax);
  TH1F hdstar0("hdstar0","hdstar0",ndstarbins,dstarmin,dstarmax);
  TH1F hdstar1("hdstar1","hdstar1",ndstarbins,dstarmin,dstarmax);
  TH1F hdstar2("hdstar2","hdstar2",ndstarbins,dstarmin,dstarmax);
  TH1F hdstar3("hdstar3","hdstar3",ndstarbins,dstarmin,dstarmax);
  TH1F hdstar4("hdstar4","hdstar4",ndstarbins,dstarmin,dstarmax);
  TH1F hdstar5("hdstar5","hdstar5",ndstarbins,dstarmin,dstarmax);
  TH1F hdstar6("hdstar6","hdstar6",ndstarbins,dstarmin,dstarmax);
  t->Draw("dstardeltam>>hdstar");
  t->Draw("dstardeltam>>hdstar0",dstarcut0);
  t->Draw("dstardeltam>>hdstar1",dstarcut1);
  t->Draw("dstardeltam>>hdstar2",dstarcut2);
  t->Draw("dstardeltam>>hdstar3",dstarcut3);
  t->Draw("dstardeltam>>hdstar4",dstarcut4);
  t->Draw("dstardeltam>>hdstar5",dstarcut5);
  t->Draw("dstardeltam>>hdstar6",dstarcut6);
  t->Draw("dstardeltam>>hdstar7",dstarcut7);
  t->Draw("dstardeltam>>hdstar8",dstarcut8);
 
 
  hdstar1.SetFillColor(1);
  hdstar2.SetFillColor(2);
  hdstar3.SetFillColor(3);
  hdstar4.SetFillColor(4);
  hdstar5.SetFillColor(6);
  hdstar6.SetFillColor(7);
  hdstar7.SetFillColor(8);
  hdstar8.SetFillColor(9);

  TLine line;
  line.SetLineColor(2);

  TF1 sig;
  TF1 bkg;
  TF1*fitF=FitHistogramDstarDM(&hdstar,.140,.155,&sig,&bkg); 
  if(!fitF) return NULL;
  bkg.SetLineColor(4);
  bkg.SetLineStyle(2); 
  Float_t scalef=bkg.Integral(database.GetDstarDeltaMMin(),database.GetDstarDeltaMMax())/hdstar.GetBinWidth(1);
  cout<<scalef<<" norm factor"<<endl;


 
  sig.SetLineColor(2);
  sig.SetLineStyle(3); 
  Float_t nsig=sig.Integral(database.GetDstarDeltaMMin(),database.GetDstarDeltaMMax())/hdstar.GetBinWidth(1);
  cout<<nsig<<" n signal distar"<<endl;

  C.Clear();
  hdstar.Draw();
  hdstar0.Draw("same");
  hdstar1.Draw("same");
  hdstar2.Draw("same");
  hdstar3.Draw("same");
  hdstar4.Draw("same");
  hdstar5.Draw("same");
  hdstar6.Draw("same");
  hdstar7.Draw("same");
  hdstar8.Draw("same");
  line.DrawLine(database.GetDstarDeltaM(),0,database.GetDstarDeltaM(),hdstar.GetMaximum());
  bkg.Draw("same");
  fitF->Draw("same");
  C.Print(filename);

  JResiduals dstarres(&hdstar,fitF);
  C.Clear();
  dstarres.GetResidualHisto()->DrawClone("pe");
  line.DrawLine(dstarmin,0,dstarmax,0);
  line.DrawLine(database.GetDstarDeltaM(),-dstarres.GetResidualHisto()->GetMaximum(),database.GetDstarDeltaM(),dstarres.GetResidualHisto()->GetMaximum());
  C.Print(filename);

//   Int_t nbins=database.GetDstPiDeltaMRangeNbins();
//   Float_t min=database.GetDstPiDeltaMRangeMin();
//   Float_t max=database.GetDstPiDeltaMRangeMax();
  
  TH1F h("h","h",nbins,min,max);
  t->Draw("dstpideltam>>h");
  TH1F h0("h0","h0",nbins,min,max);
  t->Draw("dstpideltam>>h0",dstarcut0);
  TH1F h1("h1","h1",nbins,min,max);
  t->Draw("dstpideltam>>h1",dstarcut1);
  TH1F h2("h2","h2",nbins,min,max); 
  t->Draw("dstpideltam>>h2",dstarcut2);
  TH1F h3("h3","h3",nbins,min,max); 
  t->Draw("dstpideltam>>h3",dstarcut3);
  TH1F h4("h4","h4",nbins,min,max); 
  t->Draw("dstpideltam>>h4",dstarcut4);
  TH1F h5("h5","h5",nbins,min,max); 
  t->Draw("dstpideltam>>h5",dstarcut5);
  TH1F h6("h6","h6",nbins,min,max); 
  t->Draw("dstpideltam>>h6",dstarcut6);
  TH1F h7("h7","h7",nbins,min,max); 
  t->Draw("dstpideltam>>h7",dstarcut7);
  TH1F h8("h8","h8",nbins,min,max); 
  t->Draw("dstpideltam>>h8",dstarcut8);

  h.SetLineColor(1); h.SetLineWidth(3);
  h0.SetLineColor(1); h0.SetLineWidth(3);
  h1.SetLineColor(1); h1.SetLineWidth(3);
  h2.SetLineColor(2); h2.SetLineWidth(3);
  h3.SetLineColor(3); h3.SetLineWidth(3);
  h4.SetLineColor(4); h4.SetLineWidth(3);
  h5.SetLineColor(6); h5.SetLineWidth(3);
  h6.SetLineColor(7); h6.SetLineWidth(3);
  h7.SetLineColor(8); h7.SetLineWidth(3);
  h8.SetLineColor(9); h8.SetLineWidth(3);

  C.Clear();
  h0.GetYaxis()->SetRangeUser(0,h0.GetMaximum());
  h0.Draw();
  h1.Draw("same");
  h2.Draw("same");
  h3.Draw("same");
  h4.Draw("same");
  h5.Draw("same");
  h6.Draw("same");
  h7.Draw("same");
  h8.Draw("same");
  C.Print(filename);  

  C.Clear();
  h1.GetYaxis()->SetRangeUser(0,h1.GetMaximum());
  h1.Draw();
  h2.Draw("same");
  h3.Draw("same");
  h4.Draw("same");
  h5.Draw("same");
  h6.Draw("same");
  h7.Draw("same");
  h8.Draw("same");
  C.Print(filename);

//   Float_t scalef2=CalculateIntegral(0,.35,&h6)+CalculateIntegral(.55,1.5,&h6);
//   //Float_t scalef2=1;
//   h1.Scale(scalef2/(CalculateIntegral(0,.35,&h1)+CalculateIntegral(.55,1.5,&h1)));
//   h2.Scale(scalef2/(CalculateIntegral(0,.35,&h2)+CalculateIntegral(.55,1.5,&h2)));
//   h3.Scale(scalef2/(CalculateIntegral(0,.35,&h3)+CalculateIntegral(.55,1.5,&h3)));
//   h4.Scale(scalef2/(CalculateIntegral(0,.35,&h4)+CalculateIntegral(.55,1.5,&h4)));
//   h5.Scale(scalef2/(CalculateIntegral(0,.35,&h5)+CalculateIntegral(.55,1.5,&h5)));
//   h6.Scale(scalef2/(CalculateIntegral(0,.35,&h6)+CalculateIntegral(.55,1.5,&h6)));
//   h7.Scale(scalef2/(CalculateIntegral(0,.35,&h7)+CalculateIntegral(.55,1.5,&h7)));
//   h8.Scale(scalef2/(CalculateIntegral(0,.35,&h8)+CalculateIntegral(.55,1.5,&h8))); 
  h1.SetNormFactor(scalef);
  h2.SetNormFactor(scalef);
  h3.SetNormFactor(scalef);
  h4.SetNormFactor(scalef);
  h5.SetNormFactor(scalef);
  h6.SetNormFactor(scalef);
  h7.SetNormFactor(scalef);
  h8.SetNormFactor(scalef);
 

  C.Clear();
  h1.GetYaxis()->SetRangeUser(0,h1.GetMaximum());
  h1.Draw();
  h2.Draw("same");
  h3.Draw("same");
  h4.Draw("same");
  h5.Draw("same");
  h6.Draw("same");
  h7.Draw("same");
  h8.Draw("same");
  C.Print(filename);


  TH1*h2cl=(TH1*)h2.Clone();
  h2cl->Add(&h8);
  //h2cl->Scale(.5);
  h2cl->SetNormFactor(scalef);
  h2cl->SetLineColor(1);
  C.Clear();
  h2.GetYaxis()->SetRangeUser(0,1.2*h2.GetMaximum());
  h2.Draw();
  h8.Draw("same");
  h2cl->Draw("same");
  C.Print(filename);

  C.Clear();
  h0.GetYaxis()->SetRangeUser(0,h0.GetMaximum());
  h0.Draw();
  h2cl->Draw("same");
  C.Print(filename);

 
  C.Print(filename+"]");

  delete t;  
  delete fitF;
  
  return h2cl;
  //return (TH1*)h4.Clone();
}



void SubtractK3piBkg(){

  JDataBaseDstPi database(0,0);
  TFile fk3pi("DstarCharged/DstarPi/K3pi/Reduced.root");
  TFile fkpi("DstarCharged/DstarPi/Kpi/Reduced.root");
  gROOT->cd();
  TTree* t0k3pi=(TTree*)fk3pi.Get("CleanNtuple");
  TTree* tk3pi=t0k3pi->CopyTree(database.GetCut(1,100));
  TTree* t0kpi=(TTree*)fkpi.Get("Ntuple");
  TTree* tkpi=t0kpi->CopyTree(database.GetCut(1,100));
  if(!tk3pi||!tkpi)return;

  
  TCanvas C("C","C");
  TString filename="DstarCharged/DstarPi/K3pi/DstarBkgSubtraction.ps";
  C.Print(filename+"[");

  TLine line;

  Int_t nbins=database.GetDstPiDeltaMRangeNbins();
  Float_t min=database.GetDstPiDeltaMRangeMin();
  Float_t max=database.GetDstPiDeltaMRangeMax();
  
  TH1F hkpi("hkpi","hkpi",nbins,min,max);
  tkpi->Draw("dstpideltam>>hkpi");
  hkpi.Sumw2();
  TH1F hk3pi("hk3pi","hk3pi",nbins,min,max);
  tk3pi->Draw("dstpideltam>>hk3pi");
  hk3pi.Sumw2();

  C.Clear();
  hk3pi.Draw();
  hkpi.Draw("same");
  C.Print(filename);

  ///now  compare the kpi and k3pi shapes
  TH1*hk3piNorm=(TH1*)hk3pi.Clone();
  hk3piNorm->Scale(hkpi.Integral()/hk3piNorm->Integral());

  C.Clear();
  hk3piNorm->Draw();
  hkpi.Draw("same");
  C.Print(filename);

  TH1*hdiff=(TH1*)hkpi.Clone();
  hdiff->Add((TH1*)hk3piNorm,-1.);
  hdiff->GetYaxis()->SetRangeUser(-1000,1000);
  C.Clear();
  hdiff->Draw();
  line.DrawLine(min,0,max,0);
  C.Print(filename);

  
  JResiduals diffres(&hkpi,hk3piNorm);
  C.Clear();
  TH1*hres=diffres.GetNormResidualHisto();
  hres->GetYaxis()->SetRangeUser(-10,10);
  hres->DrawClone("p");
  line.DrawLine(min,0,max,0);
  C.Print(filename);

  //-------------------------------------------------
  ///now subtract the dstar background and compare
  //-------------------------------------------------
  TH1* hk3piBkgSubNorm=(TH1*)hk3pi.Clone();
  TH1* hk3pidstarbkg=DstPi_DstarBkgSubtraction(nbins,min,max); 
//   TH1F hk3pidstarbkgbins("hk3pidstarbkgbins","hk3pidstarbkgbins",nbins,min,max);
//   hk3pidstarbkgbins.FillRandom(hk3pidstarbkg,(int)(hk3pidstarbkg->Integral()*2));
//   hk3pidstarbkgbins.Scale(.5);
//   C.Clear();
//   hk3pidstarbkg->Draw();
//   hk3pidstarbkgbins.Draw("same");
//   C.Print(filename);
  

  //subtract dstar bkg and compare
  hk3piBkgSubNorm->Add(hk3pidstarbkg,-1);
  C.Clear();
  hk3pi.SetTitle("k3pi after D* bkg sub");
  hk3pi.Draw();
  hk3piBkgSubNorm->Draw("same");
  hkpi.Draw("same");
  C.Print(filename);

  //scale k3pi down and compare
  hk3piBkgSubNorm->Scale(hkpi.Integral()/hk3piBkgSubNorm->Integral());
  C.Clear();
  hk3piBkgSubNorm->SetTitle("k3pi after D* bkg sub and scale");
  hk3piBkgSubNorm->Draw();
  hkpi.Draw("same");
  C.Print(filename);

  ///make the difierence 
  TH1*hdiffbkgsub=(TH1*)hkpi.Clone();
  hdiffbkgsub->Add((TH1*)hk3piBkgSubNorm,-1);
  hdiffbkgsub->GetYaxis()->SetRangeUser(-1000,1000);
  C.Clear();
  hdiffbkgsub->Draw();
  line.DrawLine(min,0,max,0);
  C.Print(filename);
  
  JResiduals diffbkgsubres(&hkpi,hk3piBkgSubNorm);
  C.Clear();
  TH1*hresbkgsub=diffbkgsubres.GetNormResidualHisto();
  hresbkgsub->GetYaxis()->SetRangeUser(-10,10);
  hresbkgsub->DrawClone("p");
  line.DrawLine(min,0,max,0);
  C.Print(filename);

 
  C.Print(filename+"]");



  delete tk3pi;
  delete tkpi;
  delete hk3piNorm;
  delete hdiff;
  delete hk3pidstarbkg;
  delete hdiffbkgsub;
}
