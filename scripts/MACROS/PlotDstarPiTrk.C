#include "../Globals.h"
#include "../AnalysisGlobals.h"
#include "../FUNCTIONS/SetHisto.h"
#include "TreeChain.C"
#include <TVector3.h>
#include <TLorentzVector.h>




void MakeDstarPiTrkPlots(Int_t WhichTrk){
  
  TString NtupleDir[8]={"DstarCharged/DstarPiE/DATARun5On",
			"DstarCharged/DstarPiMu/DATARun5On",
			"DstarCharged/DstarPiPi/DATARun5On",
			"DstarCharged/DstarPiK/DATARun5On",
			"DstarCharged/DstarPiP/DATARun5On",
			"DstarCharged/DstarPiGam/DATARun5On",
			"DstarCharged/DstarPiPi0/DATARun5On",
			"DstarCharged/DstarPiEta/DATARun5On",
  };
  
  Int_t nevent=100000000;
  TString Dir=NtupleDir[WhichTrk-1];



 
  //build the chain
  //TreeChain(&tree,1,131,Dir);
  TChain tree("ntp1");
  TreeChain(&tree,1,131,Dir);


  Float_t DstarMass[100];
  Int_t Dstard1Idx[100];
  Float_t DstarLund[100];  
  Float_t D0Mass[100];
  Float_t XMass[100];
  Float_t Xp3CM[100];
  Int_t Xd1Idx[100];
  Int_t Xd2Idx[100];
  Int_t Xd3Idx[100];
  Int_t nX;
  Float_t XcosthCM[100];  
  Float_t XphiCM[100];  
  Float_t Dstarp3CM[100];  
  Float_t DstarcosthCM[100];  
  Float_t DstarphiCM[100];
  Float_t Pip3CM[100]; 
  Float_t PicosthCM[100]; 
  Float_t PiphiCM[100]; 
  Float_t PiLund[100];  
  Float_t Kp3CM[100]; 
  Float_t KcosthCM[100]; 
  Float_t KphiCM[100]; 
  Float_t KLund[100];  

  Float_t Trkp3CM[100]; 
  Float_t TrkcosthCM[100]; 
  Float_t TrkphiCM[100];
  Float_t TrkLund[100];  
  Int_t TrkIdx;
  Float_t TrkMass=0;
  if(WhichTrk==1)TrkMass=ElectronPDGMass;
  if(WhichTrk==2)TrkMass=MuonPDGMass;
  if(WhichTrk==3)TrkMass=PiPDGMass;
  if(WhichTrk==4)TrkMass=KPDGMass;
  if(WhichTrk==5)TrkMass=ProtonPDGMass;
  if(WhichTrk==6)TrkMass=0;
  if(WhichTrk==7)TrkMass=Pi0PDGMass;
  if(WhichTrk==8)TrkMass=EtaPDGMass;

  tree.SetBranchStatus("*",0);
  tree.SetBranchStatus("nX",1);
  tree.SetBranchAddress("nX",&nX);
  tree.SetBranchStatus("Xd1Idx",1);
  tree.SetBranchAddress("Xd1Idx",Xd1Idx);
  tree.SetBranchStatus("Xd2Idx",1);
  tree.SetBranchAddress("Xd2Idx",Xd2Idx);
  tree.SetBranchStatus("Xd3Idx",1);
  tree.SetBranchAddress("Xd3Idx",Xd3Idx);
  tree.SetBranchStatus("XMass",1);
  tree.SetBranchAddress("XMass",XMass);
  tree.SetBranchStatus("Xp3CM",1);
  tree.SetBranchAddress("Xp3CM",Xp3CM);
  tree.SetBranchStatus("XcosthCM",1);
  tree.SetBranchAddress("XcosthCM",XcosthCM);
  tree.SetBranchStatus("XphiCM",1);
  tree.SetBranchAddress("XphiCM",XphiCM);
  tree.SetBranchStatus("Dstarp3CM",1);
  tree.SetBranchAddress("Dstarp3CM",Dstarp3CM);
  tree.SetBranchStatus("DstarLund",1);
  tree.SetBranchAddress("DstarLund",DstarLund);
  tree.SetBranchStatus("DstarcosthCM",1);
  tree.SetBranchAddress("DstarcosthCM",DstarcosthCM);
  tree.SetBranchStatus("DstarphiCM",1);
  tree.SetBranchAddress("DstarphiCM",DstarphiCM);
  tree.SetBranchStatus("DstarMass",1);
  tree.SetBranchAddress("DstarMass",DstarMass);  
  tree.SetBranchStatus("Dstard1Idx",1);
  tree.SetBranchAddress("Dstard1Idx",Dstard1Idx);
  tree.SetBranchStatus("Pip3CM",1);
  tree.SetBranchAddress("Pip3CM",Pip3CM);
  tree.SetBranchStatus("PicosthCM",1);
  tree.SetBranchAddress("PicosthCM",PicosthCM);
  tree.SetBranchStatus("PiphiCM",1);
  tree.SetBranchAddress("PiphiCM",PiphiCM);
  tree.SetBranchStatus("PiLund",1);
  tree.SetBranchAddress("PiLund",PiLund);
  tree.SetBranchStatus("D0Mass",1);
  tree.SetBranchAddress("D0Mass",D0Mass);  
  tree.SetBranchStatus("Kp3CM",1);
  tree.SetBranchAddress("Kp3CM",Kp3CM);
  tree.SetBranchStatus("KcosthCM",1);
  tree.SetBranchAddress("KcosthCM",KcosthCM);
  tree.SetBranchStatus("KphiCM",1);
  tree.SetBranchAddress("KphiCM",KphiCM);
  tree.SetBranchStatus("KLund",1);
  tree.SetBranchAddress("KLund",KLund);

  if(WhichTrk==1||WhichTrk==2||WhichTrk==5||WhichTrk==6||WhichTrk==7||WhichTrk==8){//electron,muon,proton //3,4 are the pion and Kaon
    tree.SetBranchStatus("Trkp3CM",1);
    tree.SetBranchAddress("Trkp3CM",Trkp3CM);
    tree.SetBranchStatus("TrkcosthCM",1);
    tree.SetBranchAddress("TrkcosthCM",TrkcosthCM);
    tree.SetBranchStatus("TrkphiCM",1);
    tree.SetBranchAddress("TrkphiCM",TrkphiCM);
    tree.SetBranchStatus("TrkLund",1);
    tree.SetBranchAddress("TrkLund",TrkLund);
  
  }

  TFile SavedPlots(Dir+"/SavedHistos.root","recreate");  

  TH1F HXMass("HXMass","HXMass",600,2.,5.);
  TH1F HDstPiMass("HDstPiMass","HDstPiMass",200,2.,3.);  
  TH1F HDstarMass("HDstarMass","HDstarMass",300,.130,.160);  
  TH1F HDstarAngle("HDstarAngle","HDstarAngle",40,-1.001,1.001);  
  TH1F HXPstar("HXPstar","HXPstar",500,0,5);  
  TH1F HD0Mass("HD0Mass","HD0Mass",200,1.76,1.96);          
  TH1F HPi1TrkMass("HPi1TrkMass","HPi1TrkMass",200,0,2);  
  TH2F HPi1TrkMassVsXMass("HPi1TrkMassVsXMass","HPi1TrkMassVsXMass",600,2.,5.,200,0,2);  
  TH2F HPi1TrkMassVsDstarMass("HPi1TrkMassVsDstarMass","HPi1TrkMassVsDstarMass",300,.13,.16,200,0,2);  
  TH1F HDstarTrkMass("HDstarTrkMass","HDstarTrkMass",300,2.,5.);
  TH1F HXMassCutPiTrkMass("HXMassCutPiTrkMass","HXMassCutPiTrkMass",100,2.5,2.6); 
  TH1F HDstPiMassCutXMass("HDstPiMassCutXMass","HDstPiMassCutXMass",200,2.,3.);  

  Float_t dstpim;
  Float_t dstardm;
  Float_t d0m;
  Float_t xpstar;
  Float_t xmass;
  Float_t dstarcostheta;
  Int_t DstarIdx;
  Int_t D0Idx;
  Int_t Pi1Idx;
  
  Float_t pi1trkm;
  Float_t dsttrkm;
  
  //loop
  Int_t evid=0;
  Int_t totentries=tree.GetEntries();
  while(tree.GetEntry(evid)>0&&evid<nevent){    
    if(evid%5000==0)cout<<evid<<"/"<<totentries<<" events done"<<endl;
    evid++; 
    
    if(nX>100)continue;
    
    for(Int_t XIdx=0;XIdx<nX;XIdx++){
      DstarIdx=Xd1Idx[XIdx];
      D0Idx=Dstard1Idx[Xd1Idx[XIdx]];
      Pi1Idx=Xd2Idx[XIdx];
      TrkIdx=Xd3Idx[XIdx];
      d0m=D0Mass[D0Idx];
      dstardm=DstarMass[DstarIdx]-d0m;
      xpstar=Xp3CM[XIdx];
      xmass=XMass[XIdx]-d0m-dstardm+2.01;
      
      if(WhichTrk==3){//The Pion
	Trkp3CM[TrkIdx]=Pip3CM[TrkIdx];
	TrkcosthCM[TrkIdx]=PicosthCM[TrkIdx];
	TrkphiCM[TrkIdx]=PiphiCM[TrkIdx];
	TrkLund[TrkIdx]=PiLund[TrkIdx];
      }
      if(WhichTrk==4){//The Kaon 
	Trkp3CM[TrkIdx]=Kp3CM[TrkIdx];
	TrkcosthCM[TrkIdx]=KcosthCM[TrkIdx];
	TrkphiCM[TrkIdx]=KphiCM[TrkIdx];
	TrkLund[TrkIdx]=KLund[TrkIdx];
      }

      //-----------------------------------------------
      //Float_t DstPiAnalysis::ComputeDstarAngle(){
      //------------------------------------------------
      //Angle of the D* wrt to the X direction in the X frame
      TVector3 Xp3vec(Xp3CM[XIdx]*sin(acos(XcosthCM[XIdx]))*cos(XphiCM[XIdx]),
		      Xp3CM[XIdx]*sin(acos(XcosthCM[XIdx]))*sin(XphiCM[XIdx]),
		      Xp3CM[XIdx]*XcosthCM[XIdx]);
      
      Float_t XEnergy=sqrt(Xp3vec*Xp3vec + XMass[XIdx]*XMass[XIdx]);
      TVector3 XVelvec=(1/XEnergy)*Xp3vec;
      
      TVector3 Dstarp3vec(Dstarp3CM[DstarIdx]*sin(acos(DstarcosthCM[DstarIdx]))*cos(DstarphiCM[DstarIdx]),
			  Dstarp3CM[DstarIdx]*sin(acos(DstarcosthCM[DstarIdx]))*sin(DstarphiCM[DstarIdx]),
			  Dstarp3CM[DstarIdx]*DstarcosthCM[DstarIdx]); 
      
      TLorentzVector Dstarp4CM(Dstarp3vec,sqrt(Dstarp3vec*Dstarp3vec+DstarMass[DstarIdx]*DstarMass[DstarIdx]));
      TLorentzVector Dstarp4XCM(Dstarp3vec,sqrt(Dstarp3vec*Dstarp3vec+DstarMass[DstarIdx]*DstarMass[DstarIdx]));
      
      //boost the dstar momentum into rest frame of X
      Dstarp4XCM.Boost(-XVelvec);
      TVector3 DstarBoostedp3vec=Dstarp4XCM.Vect();
      
      //dot product of boosted dstar direction with X direction 
      //return 
      dstarcostheta=DstarBoostedp3vec.Unit()*Xp3vec.Unit();
      
      //}
      //--------------------------------------------------------------------
     
      ////---------------------------------
      ///Compute m(D*pi)
      ///--------------------------------------
      TVector3 Pi1p3vec(Pip3CM[Pi1Idx]*sin(acos(PicosthCM[Pi1Idx]))*cos(PiphiCM[Pi1Idx]),
			  Pip3CM[Pi1Idx]*sin(acos(PicosthCM[Pi1Idx]))*sin(PiphiCM[Pi1Idx]),
			  Pip3CM[Pi1Idx]*PicosthCM[Pi1Idx]); 
      
      TLorentzVector Pi1p4CM(Pi1p3vec,sqrt(Pi1p3vec*Pi1p3vec+PiPDGMass*PiPDGMass));
     
      dstpim=(Dstarp4CM+Pi1p4CM).Mag();
      //-----------------------------------------------
      
      ///---------------------------------
      ///Compute m(Pi1Trk)
      ///--------------------------------------
      TVector3 Trkp3vec(Trkp3CM[TrkIdx]*sin(acos(TrkcosthCM[TrkIdx]))*cos(TrkphiCM[TrkIdx]),
			  Trkp3CM[TrkIdx]*sin(acos(TrkcosthCM[TrkIdx]))*sin(TrkphiCM[TrkIdx]),
			  Trkp3CM[TrkIdx]*TrkcosthCM[TrkIdx]); 
      
      TLorentzVector Trkp4CM(Trkp3vec,sqrt(Trkp3vec*Trkp3vec+TrkMass*TrkMass));
       
      pi1trkm=(Pi1p4CM+Trkp4CM).Mag();
      
      ///-------------------------------
      //Compute m(D*Trk)
      ///-------------------------------
      dsttrkm=(Dstarp4CM+Trkp4CM).Mag()-d0m-dstardm+2.01;


 
      //Fill the histos
      if(fabs(dstardm-(DstarPDGMass-D0PDGMass+.00025))<.001
	 &&fabs(d0m-D0PDGMass)<.02
	 &&xpstar>3
	 &&dstarcostheta<.5
	 //&&PiLund[Pi1Idx]*DstarLund[DstarIdx]<0
	 //&&TrkLund[TrkIdx]*DstarLund[DstarIdx]>0
	 //&&TrkLund[TrkIdx]*PiLund[Pi1Idx]<0
	 ){

	HXMass.Fill(xmass); 
	HDstPiMass.Fill(dstpim);
	HDstarMass.Fill(dstardm);        
	HDstarAngle.Fill(dstarcostheta);
	HXPstar.Fill(xpstar);         
	HD0Mass.Fill(d0m); 
	HPi1TrkMass.Fill(pi1trkm);
	HPi1TrkMassVsXMass.Fill(xmass,pi1trkm);
	HPi1TrkMassVsDstarMass.Fill(dstardm,pi1trkm);
	HDstarTrkMass.Fill(dsttrkm);
	if(fabs(pi1trkm-K0PDGMass)<.01) HXMassCutPiTrkMass.Fill(xmass);
	if(fabs(xmass-2.535)<.002) HDstPiMassCutXMass.Fill(dstpim);
      }
    }
  }
  
  //save the histograms for later fitting

      
  HXMass.Write(); 
  HDstPiMass.Write();   
  HDstarMass.Write();  
  HDstarAngle.Write();
  HXPstar.Write();  
  HD0Mass.Write();  
  HPi1TrkMass.Write();
  HPi1TrkMassVsXMass.Write();
  HPi1TrkMassVsDstarMass.Write();
  HDstarTrkMass.Write();
  HXMassCutPiTrkMass.Write();
  HDstPiMassCutXMass.Write();
  SavedPlots.ls();//turns out this is quite important for the objects to be written.
  SavedPlots.Close();

  
  return;
}

void PlotDstarPiTrkPlots(Int_t WhichTrk){


  TString NtupleDir[8]={"DstarCharged/DstarPiE/DATARun5On",
			"DstarCharged/DstarPiMu/DATARun5On",
			"DstarCharged/DstarPiPi/DATARun5On",
			"DstarCharged/DstarPiK/DATARun5On",
			"DstarCharged/DstarPiP/DATARun5On",
			"DstarCharged/DstarPiGam/DATARun5On",
			"DstarCharged/DstarPiPi0/DATARun5On",
			"DstarCharged/DstarPiEta/DATARun5On",
  };

  TString Dir=NtupleDir[WhichTrk-1];



  TFile SavedPlots(Dir+"/SavedHistos.root","read");  
  if(SavedPlots.IsZombie()){cout<<"Not file"<<endl;return;}
  SavedPlots.ls();
  gROOT->cd();

  TCanvas Canvas("Canvas","Canvas");
  TString psfile=Dir+"/DstarPiTrk.ps";
  Canvas.Print(psfile+"[");


  TH1F* HXMass=(TH1F*)SavedPlots.Get("HXMass");
  SetHistoXYLabels(HXMass,"m(D*#piY)-m(D*)+m_{D*}","GeV/c^{2}");
  Canvas.Clear();
  HXMass->Draw("pe"); 
  Canvas.Print(psfile);

  TH1F* HDstarMass=(TH1F*)SavedPlots.Get("HDstarMass");
  Canvas.Clear();
  HDstarMass->Draw("pe"); 
  Canvas.Print(psfile);

  TH1F* HDstarAngle=(TH1F*)SavedPlots.Get("HDstarAngle");
  Canvas.Clear();
  HDstarAngle->Draw("pe"); 
  Canvas.Print(psfile);

  TH1F* HXPstar=(TH1F*)SavedPlots.Get("HXPstar");
  Canvas.Clear();
  HXPstar->Draw("pe"); 
  Canvas.Print(psfile);

  TH1F* HD0Mass=(TH1F*)SavedPlots.Get("HD0Mass");
  Canvas.Clear();
  HD0Mass->Draw("pe"); 
  Canvas.Print(psfile);


  TH1F* HDstPiMass=(TH1F*)SavedPlots.Get("HDstPiMass");
  SetHistoXYLabels(HDstPiMass,"m(D*#pi)-m(D*)+m_{D*}","GeV/c^{2}");
  Canvas.Clear();
  HDstPiMass->Draw("pe"); 
  Canvas.Print(psfile);

  TH1F* HPi1TrkMass=(TH1F*)SavedPlots.Get("HPi1TrkMass");
  SetHistoXYLabels(HPi1TrkMass,"m(#piY)","GeV/c^{2}");
  Canvas.Clear();
  HPi1TrkMass->Draw("pe"); 
  Canvas.Print(psfile);


  TH1F* HDstarTrkMass=(TH1F*)SavedPlots.Get("HDstarTrkMass");
  SetHistoXYLabels(HDstarTrkMass,"m(D*Y)","GeV/c^{2}");
  Canvas.Clear();
  HDstarTrkMass->Draw("pe"); 
  Canvas.Print(psfile);

 
  TH1F* HXMassCutPiTrkMass=(TH1F*)SavedPlots.Get("HXMassCutPiTrkMass");
  SetHistoXYLabels(HXMassCutPiTrkMass,"m(D*PiY)","GeV/c^{2}");
  Canvas.Clear();
  HXMassCutPiTrkMass->Draw("pe"); 
  Canvas.Print(psfile);

  
  TH1F* HDstPiMassCutXMass=(TH1F*)SavedPlots.Get("HDstPiMassCutXMass");
  SetHistoXYLabels(HDstPiMassCutXMass,"m(D*#pi)","GeV/c^{2}");
  Canvas.Clear();
  HDstPiMassCutXMass->Draw("pe"); 
  Canvas.Print(psfile);

  TH2F* HPi1TrkMassVsXMass=(TH2F*)SavedPlots.Get("HPi1TrkMassVsXMass");
  Canvas.Clear();
  HPi1TrkMassVsXMass->Draw("colz"); 
  Canvas.Print(psfile);

  TH2F* HPi1TrkMassVsDstarMass=(TH2F*)SavedPlots.Get("HPi1TrkMassVsDstarMass");
  Canvas.Clear();
  HPi1TrkMassVsDstarMass->Draw("colz"); 
  Canvas.Print(psfile);



  //some fits
  TF1 F("F","[0]/((x-[1])*(x-[1])+.25*[2]*[2]) + [3] + [4]*x +[5]*x**2",HPi1TrkMass->GetXaxis()->GetXmin(),HPi1TrkMass->GetXaxis()->GetXmax());
  F.SetParameters(1,1.,.1,1,2,3);
  Canvas.Clear();
  delete HPi1TrkMass->FindObject("stats");
  HPi1TrkMass->Fit(&F,"Q","Q",.3,1.6);
  HPi1TrkMass->Draw("pe"); 
  Canvas.Print(psfile);


  Canvas.Print(psfile+"]");

  SavedPlots.Close();
  

  return;
}




