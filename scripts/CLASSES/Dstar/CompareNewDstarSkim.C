#include "../Globals.h"
#include "TreeChain.C"
#include "../CLASSES/JDataBase2640.h"
#include "../FUNCTIONS/MoveStatsBox.h"

void compareNewSkimDstPiPi(Int_t mode){

  TString fileold;
  TString filenew;  
  fileold="$localntupledir/Dst2640ToDstPiPi/D0ToKPi/Dst2640Reduced.root";
  filenew="DstarCharged/DstarPiPi/Dst2640Reduced.root";
  TFile FOld(fileold,"r");
  TFile FNew(filenew,"r");
  gROOT->cd();
  TTree* treeOld=(TTree*)FOld.Get("Dst2640Ntuple");
  TTree* treeNew=(TTree*)FNew.Get("Dst2640Ntuple");
  if(!treeOld||!treeNew){cout<<"No Ntuples."<<" old="<<treeOld<<" new="<<treeNew<<endl;return;}

  TString Mode;
  if(mode==1)Mode="D0ToKPi";
  if(mode==2)Mode="D0ToK3Pi";

  JDataBase2640 database(0,Mode,0);
   

  TCanvas Canvas("Canvas","Canvas");
  TString psfile;
  if(mode==1)psfile="KPi.ps";
  if(mode==2)psfile="K3Pi.ps";
  Canvas.Print(psfile+"[");

 
  TH1F HDstPiPiDMOld("HDstPiPiDMOld","D*#pi#pi deltaM",20,.2,1.6);
  TH1F HDstPiPiDMNew("HDstPiPiDMNew","D*#pi#pi deltaM",20,.2,1.6);
  Canvas.Clear(); 
  treeNew->Draw("dst2640deltam>>HDstPiPiDMNew",database.GetCut(1,100),"p");
  treeOld->Draw("dst2640deltam>>HDstPiPiDMOld",database.GetCut(1,100),"same");
  Canvas.Print(psfile); 
  HDstPiPiDMNew.Add(&HDstPiPiDMOld,-1);
  Canvas.Clear();
  HDstPiPiDMNew.Draw("p");
  Canvas.Print(psfile);
  HDstPiPiDMNew.Divide(&HDstPiPiDMOld);
  Canvas.Clear();
  HDstPiPiDMNew.Draw("p");
  Canvas.Print(psfile);

  Canvas.Print(psfile+"]");

  return;
}

