#include "../Globals.h"
#include "../CLASSES/JDataBaseDstPi.h"
void checkmassdep(){
  TFile f("DstarCharged/DstarPi/MC/D2420/Kpi/Reduced.root");
  gROOT->cd();
  TTree*t=(TTree*)f.Get("Ntuple");
  cout<<t<<endl;
  TCut cut1="-1<dstarcostheta&&dstarcostheta<-.5";
  TCut cut2="0<dstarcostheta&&dstarcostheta<.5";
  TCut lund="abs(dstpimclund)==10423";
  JDataBaseDstPi d(0,0);
  TH1F h1("h1","h1",45,.2,.65);
  TH1F h2("h2","h2",45,.2,.65);
  t->Draw("dstpideltam>>h1",d.GetCut(1,6)+cut1+lund);
  t->Draw("dstpideltam>>h2",d.GetCut(1,6)+cut2+lund);
  
  TCanvas c;
  c.Clear();
  h1.Draw();
  h2.Draw("same");
  c.Print("test1.gif");

  Float_t r=0;
  TH1F h3("h3","h3",45,.2,.65);
  for(Int_t i=1;i<=45;i++){
    if(h2.GetBinContent(i)>50){
      r=h1.GetBinContent(i)/h2.GetBinContent(i);
      h3.SetBinContent(i,r);
      h3.SetBinError(i,r*sqrt(2*h1.GetBinContent(i))/h2.GetBinContent(i));
    }
  }
  c.Clear();
  h3.Draw("pe");
  c.Print("test2.gif");
  

}
  
