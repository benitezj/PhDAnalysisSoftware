#include "../Globals.h"
#include "../AnalysisGlobals.h"
#include "../FUNCTIONS/SetHisto.h"
#include "TreeChain.C"
#include <TVector3.h>
#include <TLorentzVector.h>
#include "../FUNCTIONS/Project2DHistoLegendre.h"




void MakeDstarTrkPlots(Int_t WhichTrk){



  TString NtupleDir[8]={"DstarCharged/DstarE/DATARun5On",
			"DstarCharged/DstarMu/DATARun5On",
			"DstarCharged/DstarPi/DATARun5On",
			"DstarCharged/DstarK/DATARun5On",
			"DstarCharged/DstarP/DATARun5On",
			"DstarCharged/DstarGam/DATARun5On",
			"DstarCharged/DstarPi0/DATARun5On",
			"DstarCharged/DstarEta/DATARun5On",
  };
  
  TString Dir=NtupleDir[WhichTrk-1];


  
  //build the chain
  //TreeChain(&tree,1,131,Dir);
  TChain tree("ntp1");
  TreeChain(&tree,1,131,Dir);


  Float_t DstarMass[100];
  Int_t Dstard1Idx[100];
  Float_t D0Mass[100];
  Float_t D0p3CM[100];  
  Float_t D0costhCM[100];  
  Float_t D0phiCM[100];
  Float_t XMass[100];
  Float_t Xp3CM[100];
  Int_t Xd1Idx[100];
  Int_t Xd2Idx[100];
  Int_t nX;
  Float_t XcosthCM[100];  
  Float_t XphiCM[100];  
  Float_t Dstarp3CM[100];  
  Float_t DstarcosthCM[100];  
  Float_t DstarphiCM[100];
  Float_t Pip3CM[100]; 
  Float_t PicosthCM[100]; 
  Float_t PiphiCM[100];
  Float_t Kp3CM[100]; 
  Float_t KcosthCM[100]; 
  Float_t KphiCM[100]; 
  Float_t Pi0p3CM[100]; 
  Float_t Pi0costhCM[100]; 
  Float_t Pi0phiCM[100]; 
  Float_t Pi0p3[100]; 

  Float_t Trkp3CM[100];
  Float_t Trkp3[100]; 
  Float_t TrkcosthCM[100]; 
  Float_t TrkphiCM[100]; 
  Int_t TrkIdx;
  Float_t TrkMass=0;
  //apply tight cuts
  Float_t Trkp3Cut=.2;
  Float_t DstarCosthetaCut=.0;
  Float_t pstarCut=4.;
  //extra particle mass
  if(WhichTrk==1){TrkMass=ElectronPDGMass;}
  if(WhichTrk==2){TrkMass=MuonPDGMass;}
  if(WhichTrk==3){TrkMass=PiPDGMass;}
  if(WhichTrk==4){TrkMass=KPDGMass;}
  if(WhichTrk==5){TrkMass=ProtonPDGMass;}
  if(WhichTrk==6){TrkMass=0;}
  if(WhichTrk==7){TrkMass=Pi0PDGMass;}
  if(WhichTrk==8){TrkMass=EtaPDGMass;}
  


  tree.SetBranchStatus("*",0);
  tree.SetBranchStatus("nX",1);
  tree.SetBranchAddress("nX",&nX);
  tree.SetBranchStatus("Xd1Idx",1);
  tree.SetBranchAddress("Xd1Idx",Xd1Idx);
  tree.SetBranchStatus("Xd2Idx",1);
  tree.SetBranchAddress("Xd2Idx",Xd2Idx);
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
  tree.SetBranchStatus("D0Mass",1);
  tree.SetBranchAddress("D0Mass",D0Mass);  
  tree.SetBranchStatus("D0p3CM",1);
  tree.SetBranchAddress("D0p3CM",D0p3CM);
  tree.SetBranchStatus("D0costhCM",1);
  tree.SetBranchAddress("D0costhCM",D0costhCM);
  tree.SetBranchStatus("D0phiCM",1);
  tree.SetBranchAddress("D0phiCM",D0phiCM);
  tree.SetBranchStatus("Kp3CM",1);
  tree.SetBranchAddress("Kp3CM",Kp3CM);
  tree.SetBranchStatus("KcosthCM",1);
  tree.SetBranchAddress("KcosthCM",KcosthCM);
  tree.SetBranchStatus("KphiCM",1);
  tree.SetBranchAddress("KphiCM",KphiCM);

  if(WhichTrk==1||WhichTrk==2||WhichTrk==5||WhichTrk==6||WhichTrk==8){//electron,muon,proton //3,4 are the pion and Kaon
    tree.SetBranchStatus("Trkp3CM",1);
    tree.SetBranchAddress("Trkp3CM",Trkp3CM);
    tree.SetBranchStatus("Trkp3",1);
    tree.SetBranchAddress("Trkp3",Trkp3);
    tree.SetBranchStatus("TrkcosthCM",1);
    tree.SetBranchAddress("TrkcosthCM",TrkcosthCM);
    tree.SetBranchStatus("TrkphiCM",1);
    tree.SetBranchAddress("TrkphiCM",TrkphiCM);
  
  }
  if(WhichTrk==7){

    tree.SetBranchStatus("Pi0p3CM",1);
    tree.SetBranchAddress("Pi0p3CM",Pi0p3CM);
    tree.SetBranchStatus("Pi0p3",1);
    tree.SetBranchAddress("Pi0p3",Pi0p3);
    tree.SetBranchStatus("Pi0costhCM",1);
    tree.SetBranchAddress("Pi0costhCM",Pi0costhCM);
    tree.SetBranchStatus("Pi0phiCM",1);
    tree.SetBranchAddress("Pi0phiCM",Pi0phiCM);

  }


  TFile SavedPlots(Dir+"/SavedHistos.root","recreate");  

  TH1F HXMass("HXMass","HXMass",150,2.,3.5);
  TH2F HXMassVsDstarHel("HXMassVsDstarHel","HXMassVsDstarHel",150,2.,3.5,40,-1.001,1.001);
  TH1F HDstarMass("HDstarMass","HDstarMass",300,.130,.160);  
  TH1F HDstarAngle("HDstarAngle","HDstarAngle",40,-1.001,1.001);  
  TH1F HXPstar("HXPstar","HXPstar",500,0,5);  
  TH1F HD0Mass("HD0Mass","HD0Mass",200,1.76,1.96);          

  Float_t dstardm;
  Float_t d0m;
  Float_t xpstar;
  Float_t xmass;
  Float_t dstarcostheta;
  Int_t DstarIdx;
  Int_t D0Idx;
  Float_t dstarhelicity;



  //loop
  Int_t evid=0;
  Int_t totentries=tree.GetEntries();
  while(tree.GetEntry(evid)>0){    
    if(evid%5000==0)cout<<evid<<"/"<<totentries<<" events done"<<endl;
    evid++; 
    
    if(nX>100)continue;
    
    for(Int_t XIdx=0;XIdx<nX;XIdx++){
      DstarIdx=Xd1Idx[XIdx];
      D0Idx=Dstard1Idx[DstarIdx];
      TrkIdx=Xd2Idx[XIdx];
      d0m=D0Mass[D0Idx];
      dstardm=DstarMass[DstarIdx]-D0Mass[D0Idx];
      xpstar=Xp3CM[XIdx];
      xmass=XMass[XIdx]-DstarMass[DstarIdx]+DstarPDGMass;
     
      ///These 3 come from different Ntuples
      if(WhichTrk==3){//The Pion
	Trkp3CM[TrkIdx]=Pip3CM[TrkIdx];
	TrkcosthCM[TrkIdx]=PicosthCM[TrkIdx];
	TrkphiCM[TrkIdx]=PiphiCM[TrkIdx];
      }
      if(WhichTrk==4){//The Kaon 
	Trkp3CM[TrkIdx]=Kp3CM[TrkIdx];
	TrkcosthCM[TrkIdx]=KcosthCM[TrkIdx];
	TrkphiCM[TrkIdx]=KphiCM[TrkIdx];
      }
      if(WhichTrk==7){//The pi0
	Trkp3CM[TrkIdx]=Pi0p3CM[TrkIdx];
	TrkcosthCM[TrkIdx]=Pi0costhCM[TrkIdx];
	TrkphiCM[TrkIdx]=Pi0phiCM[TrkIdx];
	Trkp3[TrkIdx]=Pi0p3[TrkIdx];
      }

      //-----------------------------------------------
      //Angle of the D* wrt to the X direction in the X frame
      TVector3 Xp3vec(Xp3CM[XIdx]*sin(acos(XcosthCM[XIdx]))*cos(XphiCM[XIdx]),
		      Xp3CM[XIdx]*sin(acos(XcosthCM[XIdx]))*sin(XphiCM[XIdx]),
		      Xp3CM[XIdx]*XcosthCM[XIdx]);
      
      Float_t XEnergy=sqrt(Xp3vec*Xp3vec + XMass[XIdx]*XMass[XIdx]);
      TVector3 XVelvec=(1/XEnergy)*Xp3vec;
      
      TVector3 Dstarp3vec(Dstarp3CM[DstarIdx]*sin(acos(DstarcosthCM[DstarIdx]))*cos(DstarphiCM[DstarIdx]),
			  Dstarp3CM[DstarIdx]*sin(acos(DstarcosthCM[DstarIdx]))*sin(DstarphiCM[DstarIdx]),
			  Dstarp3CM[DstarIdx]*DstarcosthCM[DstarIdx]); 
      
      Float_t DstarEnergy=sqrt(Dstarp3vec*Dstarp3vec + DstarMass[DstarIdx]*DstarMass[DstarIdx]);
      TVector3 DstarVelvec=(1/DstarEnergy)*Dstarp3vec;

      
      //boost the dstar momentum into rest frame of X
      TLorentzVector Dstarp4XRF(Dstarp3vec,sqrt(Dstarp3vec*Dstarp3vec+DstarMass[DstarIdx]*DstarMass[DstarIdx]));
      Dstarp4XRF.Boost(-XVelvec);
      TVector3 Dstarp3vecXRF=Dstarp4XRF.Vect();
      
      dstarcostheta=Dstarp3vecXRF.Unit()*Xp3vec.Unit();      
      //--------------------------------------------------------------------
 
      //---------------------------------------------------
      //DstarHelicity angle
      //-------------------------------------------------
      //angle of the D0 w.r.t the X in the D* frame
      
      TVector3 D0p3vec(D0p3CM[D0Idx]*sin(acos(D0costhCM[D0Idx]))*cos(D0phiCM[D0Idx]),
			  D0p3CM[D0Idx]*sin(acos(D0costhCM[D0Idx]))*sin(D0phiCM[D0Idx]),
			  D0p3CM[D0Idx]*D0costhCM[D0Idx]); 
      
      //boost the D0 momentum into rest frame of D*
      TLorentzVector D0p4DstRF(D0p3vec,sqrt(D0p3vec*D0p3vec+D0Mass[D0Idx]*D0Mass[D0Idx]));
      D0p4DstRF.Boost(-DstarVelvec);
      TVector3 D0p3vecDstarRF=D0p4DstRF.Vect();

      //boost the X momentum into rest frame of D*
      TLorentzVector Xp4DstRF(Xp3vec,sqrt(Xp3vec*Xp3vec+XMass[XIdx]*XMass[XIdx]));     
      Xp4DstRF.Boost(-DstarVelvec);
      TVector3 Xp3vecDstarRF=Xp4DstRF.Vect();
      
      dstarhelicity=D0p3vecDstarRF.Unit()*Xp3vecDstarRF.Unit();      
      //--------------------------------------------------------------------
      
      

      

 
      //Fill the histos
      if(fabs(dstardm-(DstarPDGMass-D0PDGMass+.00025))<.001
	 &&fabs(d0m-D0PDGMass)<.02
	 &&xpstar>pstarCut
	 &&dstarcostheta<DstarCosthetaCut
	 ){
	if((WhichTrk==6||WhichTrk==7||WhichTrk==8)&&Trkp3[TrkIdx]<Trkp3Cut)continue;//for gammas apply energy cut
	
	HXMass.Fill(xmass); 
	HDstarMass.Fill(dstardm);        
	HDstarAngle.Fill(dstarcostheta);
	HXPstar.Fill(xpstar);         
	HD0Mass.Fill(d0m); 
	HXMassVsDstarHel.Fill(xmass,dstarhelicity);
      }
    }
  }
  
  //save the histograms for later fitting

      
  HXMass.Write(); 
  HDstarMass.Write();  
  HDstarAngle.Write();
  HXPstar.Write();  
  HD0Mass.Write();  
  HXMassVsDstarHel.Write();
  SavedPlots.ls();//turns out this is quite important for the objects to be written.
  SavedPlots.Close();

  
  return;
}

void PlotDstarTrkPlots(Int_t WhichTrk){

  TString NtupleDir[8]={"DstarCharged/DstarE/DATARun5On",
			"DstarCharged/DstarMu/DATARun5On",
			"DstarCharged/DstarPi/DATARun5On",
			"DstarCharged/DstarK/DATARun5On",
			"DstarCharged/DstarP/DATARun5On",
			"DstarCharged/DstarGam/DATARun5On",
			"DstarCharged/DstarPi0/DATARun5On",
			"DstarCharged/DstarEta/DATARun5On",
  };


  TString Dir=NtupleDir[WhichTrk-1];


  TFile SavedPlots(Dir+"/SavedHistos.root","read");  
  if(SavedPlots.IsZombie()){cout<<"Not file"<<endl;return;}
  SavedPlots.ls();
  gROOT->cd();

  TCanvas Canvas("Canvas","Canvas");
  TString psfile=Dir+"/DstarTrk.ps";
  Canvas.Print(psfile+"[");


  TH1F* HXMass=(TH1F*)SavedPlots.Get("HXMass");
  if(!HXMass)return;
  SetHistoXYLabels(HXMass,"m(D*Y)-m(D*)+m_{D*}","GeV/c^{2}");
  Canvas.Clear();
  HXMass->Draw("pe"); 
  Canvas.Print(psfile);

  TH2F* HXMassVsDstarHel=(TH2F*)SavedPlots.Get("HXMassVsDstarHel");
  if(!HXMassVsDstarHel)return;
  Canvas.Clear();
  HXMassVsDstarHel->Draw("colz"); 
  Canvas.Print(psfile);

  TH1F* HDstarMass=(TH1F*)SavedPlots.Get("HDstarMass");
  if(!HDstarMass)return;
  Canvas.Clear();
  HDstarMass->Draw("pe"); 
  Canvas.Print(psfile);

  TH1F* HDstarAngle=(TH1F*)SavedPlots.Get("HDstarAngle");
  if(!HDstarAngle)return;
  Canvas.Clear();
  HDstarAngle->Draw("pe"); 
  Canvas.Print(psfile);

  TH1F* HXPstar=(TH1F*)SavedPlots.Get("HXPstar");
  if(!HXPstar)return;
  Canvas.Clear();
  HXPstar->Draw("pe"); 
  Canvas.Print(psfile);

  TH1F* HD0Mass=(TH1F*)SavedPlots.Get("HD0Mass");
  if(!HD0Mass)return;
  Canvas.Clear();
  HD0Mass->Draw("pe"); 
  Canvas.Print(psfile);


  TLine cutline;
  Canvas.Clear();
  Canvas.Divide(2,2);
  ///now create the Legendre projections
  TH1* HDstPiMVsDstarHelLegProj[4];
  for(Int_t p=0;p<4;p++){
    Canvas.cd(p+1);

    HDstPiMVsDstarHelLegProj[p]=project2DHistoLegendre(HXMassVsDstarHel,p);
    if(!HDstPiMVsDstarHelLegProj[p])return;   
    HDstPiMVsDstarHelLegProj[p]->Draw("pe");
    cutline.DrawLine(HDstPiMVsDstarHelLegProj[p]->GetXaxis()->GetXmin(),0,HDstPiMVsDstarHelLegProj[p]->GetXaxis()->GetXmax(),0);
    
  }
  Canvas.Print(psfile);  


  Canvas.Print(psfile+"]");

  SavedPlots.Close();
  

  return;
}




