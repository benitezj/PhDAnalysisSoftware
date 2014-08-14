#include "DstPiAnalysis.h"
#include "../../FUNCTIONS/Legendre.h"
#define NPROJS 6

#include <TArrayD.h>
Int_t DstPiAnalysis::WeightEvents(){
 
  
  if(!OpenReducedNtuple())return 0;
  SetReducedNtupleBranches();
  if(!OpenEfficiency()){cout<<"Could not open efficiecy file"<<endl;return 0;}


  //
  TH1F HDstPi("HDstPi","HDstPi",2000,2.1,4.1);
 
  //
  TH1F HDstPiEff("HDstPiEff","HDstPiEff",2000,2.1,4.1);
  HDstPiEff.Sumw2();
    
  //mass cut abs(heli)>.75
  TH1F HDstPiMD2420Helicity("HDstPiMD2420Helicity","HDstPiMD2420Helicity",2000,2.1,4.1);
  HDstPiMD2420Helicity.Sumw2();

  //mass cut abs(heli)<.5
  TH1F HDstPiMD2460Helicity("HDstPiMD2460Helicity","HDstPiMD2460Helicity",2000,2.1,4.1);
  HDstPiMD2460Helicity.Sumw2();


  //mass vs helicity .2 slices
  TH1F* HDstPiMVsHelicity[10];
  for(Int_t i=0;i<10;i++){
    HDstPiMVsHelicity[i]=new TH1F(TString("HDstPiMVsHelicity")+(long)i,"HDstPiMVsHelicity",2000,2.1,4.1);
    HDstPiMVsHelicity[i]->Sumw2();
  }  

  //mass vs helicity .1 slices
  TH1F* HDstPiMVsHelicityFine[20];
  for(Int_t i=0;i<20;i++){
    HDstPiMVsHelicityFine[i]=new TH1F(TString("HDstPiMVsHelicityFine")+(long)i,"HDstPiMVsHelicityFine",2000,2.1,4.1);
    HDstPiMVsHelicityFine[i]->Sumw2();
  }  


  //mass vs helicity .2 slices cut -.5 cosD*
  TH1F* HDstPiMVsHelicityN5Cos[10];
  for(Int_t i=0;i<10;i++){
    HDstPiMVsHelicityN5Cos[i]=new TH1F(TString("HDstPiMVsHelicityN5Cos")+(long)i,"HDstPiMVsHelicityN5Cos",2000,2.1,4.1);
    HDstPiMVsHelicityN5Cos[i]->Sumw2();
  }  


  //mass vs helicity .2 slices cut -.9 cosD*
  TH1F* HDstPiMVsHelicityN9Cos[10];
  for(Int_t i=0;i<10;i++){
    HDstPiMVsHelicityN9Cos[i]=new TH1F(TString("HDstPiMVsHelicityN9Cos")+(long)i,"HDstPiMVsHelicityN9Cos",2000,2.1,4.1);
    HDstPiMVsHelicityN9Cos[i]->Sumw2();
  }  


  //mass vs helicity in slices of .25, for dependent mass widths 
  TH1F* HDstPiMVsHelicityMW[8];
  for(Int_t i=0;i<8;i++){
    HDstPiMVsHelicityMW[i]=new TH1F(TString("HDstPiMVsHelicityMW")+(long)i,"HDstPiMVsHelicityMW",2000,2.1,4.1);
    HDstPiMVsHelicityMW[i]->Sumw2();
  }


  //mass vs helicity in slices of .25, for dependent mass widths 
  TH1F* HDstPiMVsCoarseAbsHelicity[4];
  for(Int_t i=0;i<4;i++){
    HDstPiMVsCoarseAbsHelicity[i]=new TH1F(TString("HDstPiMVsCoarseAbsHelicity")+(long)i,"HDstPiMVsCoarseAbsHelicity",2000,2.1,4.1);
    HDstPiMVsCoarseAbsHelicity[i]->Sumw2();
  }




  //efficiency corrected mass vs p* coarse
  TH1F* HDstPiMVsPstar[4];
  for(Int_t i=0;i<4;i++){
    HDstPiMVsPstar[i]=new TH1F(TString("HDstPiMVsPstar")+(long)i,"HDstPiMVsPstar",2000,2.1,4.1);
    HDstPiMVsPstar[i]->Sumw2();
  } 
  TH1* HDstPiMVsPstarNoEff[4];//for p* dependent mass-width systematics with no helicity cut
  for(Int_t i=0;i<4;i++)
    HDstPiMVsPstarNoEff[i]=new TH1F(TString("HDstPiMVsPstarNoEff")+(long)i,"HDstPiMVsPstarNoEff",2000,2.1,4.1); 

  TH1* HDstPiMVsPstarD2420[4];//for p* systematics for MC this should not be weighted due to low stats
  for(Int_t i=0;i<4;i++){
    HDstPiMVsPstarD2420[i]=new TH1F(TString("HDstPiMVsPstarD2420")+(long)i,"HDstPiMVsPstarD2420",2000,2.1,4.1); 
    HDstPiMVsPstarD2420[i]->Sumw2();
  }
  TH1* HDstPiMVsPstarD2460[4];//for p* systematics
  for(Int_t i=0;i<4;i++){
    HDstPiMVsPstarD2460[i]=new TH1F(TString("HDstPiMVsPstarD2460")+(long)i,"HDstPiMVsPstarD2460",2000,2.1,4.1); 
    HDstPiMVsPstarD2460[i]->Sumw2();
  }


  //efficiency corrected mass vs p* for crossections  
  TH1F* HDstPiMVsPstarFine[10];
  for(Int_t i=0;i<10;i++){
    HDstPiMVsPstarFine[i]=new TH1F(TString("HDstPiMVsPstarFine")+(long)i,"HDstPiMVsPstarFine",2000,2.1,4.1);
    HDstPiMVsPstarFine[i]->Sumw2();
  }   
  TH1F* HDstPiMVsEnergyFine[30];
  for(Int_t i=0;i<30;i++){
    HDstPiMVsEnergyFine[i]=new TH1F(TString("HDstPiMVsEnergyFine")+(long)i,"HDstPiMVsEnergyFine",2000,2.1,4.1);
    HDstPiMVsEnergyFine[i]->Sumw2();
  }   
  

  //efficiency corrected mass vs cos Dstar  
  TH1F* HDstPiMVsCosDstar[20];
  for(Int_t i=0;i<20;i++){
    HDstPiMVsCosDstar[i]=new TH1F(TString("HDstPiMVsCosDstar")+(long)i,"HDstPiMVsCosDstar",2000,2.1,4.1);
    HDstPiMVsCosDstar[i]->Sumw2();
  }


  //efficiency corrected mass vs cosD* in slices of .25, for dependent mass widths 
  TH1F* HDstPiMVsCosDstarMW[8];
  for(Int_t i=0;i<8;i++){
    HDstPiMVsCosDstarMW[i]=new TH1F(TString("HDstPiMVsCosDstarMW")+(long)i,"HDstPiMVsCosDstarMW",2000,2.1,4.1);
    HDstPiMVsCosDstarMW[i]->Sumw2();
  }

  //efficiency corrected mass vs cosD* in slices of .25, for dependent mass widths , for D2420 cut helicity>.75
  TH1F* HDstPiMVsCosDstarD2420MW[8];
  for(Int_t i=0;i<8;i++){
    HDstPiMVsCosDstarD2420MW[i]=new TH1F(TString("HDstPiMVsCosDstarD2420MW")+(long)i,"HDstPiMVsCosDstarD2420MW",2000,2.1,4.1);
    HDstPiMVsCosDstarD2420MW[i]->Sumw2();
  }
  //efficiency corrected mass vs cosD* in slices of .25, for dependent mass widths , for D2420 cut helicity<.5
  TH1F* HDstPiMVsCosDstarD2460MW[8];
  for(Int_t i=0;i<8;i++){
    HDstPiMVsCosDstarD2460MW[i]=new TH1F(TString("HDstPiMVsCosDstarD2460MW")+(long)i,"HDstPiMVsCosDstarD2460MW",2000,2.1,4.1);
    HDstPiMVsCosDstarD2460MW[i]->Sumw2();
  }




  //efficiency corrected mass vs phi*
  TH1F* HDstPiMVsPhi[4];
  for(Int_t i=0;i<4;i++){
    HDstPiMVsPhi[i]=new TH1F(TString("HDstPiMVsPhi")+(long)i,"HDstPiMVsPhi",2000,2.1,4.1);
    HDstPiMVsPhi[i]->Sumw2();
  } 
  TH1F* HDstPiMVsPhiNoEff[4];
  for(Int_t i=0;i<4;i++){
    HDstPiMVsPhiNoEff[i]=new TH1F(TString("HDstPiMVsPhiNoEff")+(long)i,"HDstPiMVsPhiNoEff",2000,2.1,4.1);
    HDstPiMVsPhiNoEff[i]->Sumw2();
  } 
  TH1* HDstPiMVsPhiD2420[4];//for  systematics
  for(Int_t i=0;i<4;i++){
    HDstPiMVsPhiD2420[i]=new TH1F(TString("HDstPiMVsPhiD2420")+(long)i,"HDstPiMVsPhiD2420",2000,2.1,4.1);
    HDstPiMVsPhiD2420[i]->Sumw2();
  }
  TH1* HDstPiMVsPhiD2460[4];//for   systematics 
  for(Int_t i=0;i<4;i++){
    HDstPiMVsPhiD2460[i]=new TH1F(TString("HDstPiMVsPhiD2460")+(long)i,"HDstPiMVsPhiD2460",2000,2.1,4.1); 
    HDstPiMVsPhiD2460[i]->Sumw2();
  }




  //efficiency corrected mass vs cos*
  TH1F* HDstPiMVsCos[4];
  for(Int_t i=0;i<4;i++){
    HDstPiMVsCos[i]=new TH1F(TString("HDstPiMVsCos")+(long)i,"HDstPiMVsCos",2000,2.1,4.1);
    HDstPiMVsCos[i]->Sumw2();
  }
  TH1F* HDstPiMVsCosNoEff[4];
  for(Int_t i=0;i<4;i++){
    HDstPiMVsCosNoEff[i]=new TH1F(TString("HDstPiMVsCosNoEff")+(long)i,"HDstPiMVsCosNoEff",2000,2.1,4.1);
    HDstPiMVsCosNoEff[i]->Sumw2();
  } 
  TH1* HDstPiMVsCosD2420[4];//for  systematics  
  for(Int_t i=0;i<4;i++){
    HDstPiMVsCosD2420[i]=new TH1F(TString("HDstPiMVsCosD2420")+(long)i,"HDstPiMVsCosD2420",2000,2.1,4.1); 
    HDstPiMVsCosD2420[i]->Sumw2();
  }
  TH1* HDstPiMVsCosD2460[4];//for  systematics 
  for(Int_t i=0;i<4;i++){
    HDstPiMVsCosD2460[i]=new TH1F(TString("HDstPiMVsCosD2460")+(long)i,"HDstPiMVsCosD2460",2000,2.1,4.1); 
    HDstPiMVsCosD2460[i]->Sumw2();
  }





  //Legendre projections
  TH1F* HDstPiMLegendre[NPROJS];
  for(Int_t i=0;i<NPROJS;i++){
    HDstPiMLegendre[i]=new TH1F(TString("HDstPiMLegendre")+(long)i,"HDstPiMLegendre",2000,2.1,4.1);
    HDstPiMLegendre[i]->Sumw2();
  }  
  //slice P2 in cosD* 
  TH1F* HDstPiMP2VsCosDstar[8];
  for(Int_t i=0;i<8;i++){
    HDstPiMP2VsCosDstar[i]=new TH1F(TString("HDstPiMP2VsCosDstar")+(long)i,"HDstPiMP2VsCosDstar",2000,2.1,4.1);
    HDstPiMP2VsCosDstar[i]->Sumw2();
  }  



  //2D Helicity vs mass
  TH2F H2DstPiMVsDstarHel("H2DstPiMVsDstarHel","H2DstPiMVsDstarHel",2000,2.1,4.1,20,-1.0,1.0);
  H2DstPiMVsDstarHel.Sumw2();
  TH2F H2DstPiMVsDstarHelNoEff("H2DstPiMVsDstarHelNoEff","H2DstPiMVsDstarHelNoEff",2000,2.1,4.1,20,-1.0,1.0);
  H2DstPiMVsDstarHelNoEff.Sumw2();

  TH2F* H2DstPiMVsHelCosDstar[4];
  for(Int_t i=0;i<4;i++){
    H2DstPiMVsHelCosDstar[i]=new TH2F(TString("H2DstPiMVsHelCosDstar")+(long)i,TString("H2DstPiMVsHelCosDstar")+(long)i,2000,2.1,4.1,20,-1.0,1.0);
    H2DstPiMVsHelCosDstar[i]->Sumw2();
  }  



  ///------------------------------------------
  ////efficiencies after eff corrections
  //----------------------------
  TH3F H3PvsMVsDstA; 

  TH2F H2PvsM;   

  TH1F HDstarHelicityNoShape;

  TH1F HDstarHelicity;
 
  TH1F HDstPiMNoShape;

  TH1F HDstPiDM;

  TH1F HPstarNoShape;

  TH1F HDstarAngleNoShape;

  TH1F HDstarAngle;
  
  TH1F HDstPiCosstarNoShape;
    
  //fully efficiency corrected mass
  TH1F HDstPiMFineNoRes;

  //Generated mass vs p*   
  TH1* HMCMVsPstar[4];

  if(_TruthMatch){
   
    if(!OpenReducedFile())return 0;

    HMCDstarHelicity=(TH1F*)ReducedFile->Get("HMCDstarHelicity");
    if(!HMCDstarHelicity)return 0;
  
    HMCXMass=(TH1F*)ReducedFile->Get("HMCXMass");
    if(!HMCXMass)return 0;  
  
    HMCXp3CM=(TH1F*)ReducedFile->Get("HMCXp3CM");
    if(!HMCXp3CM)return 0;

    HMCXcosthCM=(TH1F*)ReducedFile->Get("HMCXcosthCM");
    if(!HMCXcosthCM)return 0;

    HMCDstarAngle=(TH1F*)ReducedFile->Get("HMCDstarAngle");
    if(!HMCDstarAngle)return 0;

    H3MCPvsMassVsDstAngle=(TH3F*)ReducedFile->Get("H3MCPvsMassVsDstAngle");                                                    
    if(!H3MCPvsMassVsDstAngle){
      cout<<"No histogram found"<<endl;
      return 0;
    }
    
    H2MCPvsMass=(TH2F*)ReducedFile->Get("H2MCPvsMass");                                                    
    if(!H2MCPvsMass){
      cout<<"No histogram found"<<endl;
      return 0;
    }

    H2MCPvsMassFine=(TH2F*)ReducedFile->Get("H2MCPvsMassFine");                                                    
    if(!H2MCPvsMass){
      cout<<"No histogram found"<<endl;
      return 0;
    }


    //----------------------------
    H3PvsMVsDstA.SetNameTitle("H3PvsMVsDstA","H3PvsMVsDstA");
    H3PvsMVsDstA.SetBins(H3MCPvsMassVsDstAngle->GetXaxis()->GetNbins(),
		      H3MCPvsMassVsDstAngle->GetXaxis()->GetXmin(),
		      H3MCPvsMassVsDstAngle->GetXaxis()->GetXmax(),
		      H3MCPvsMassVsDstAngle->GetYaxis()->GetNbins(),
		      H3MCPvsMassVsDstAngle->GetYaxis()->GetXmin(),
		      H3MCPvsMassVsDstAngle->GetYaxis()->GetXmax(),
		      H3MCPvsMassVsDstAngle->GetZaxis()->GetNbins(),
		      H3MCPvsMassVsDstAngle->GetZaxis()->GetXmin(),
		      H3MCPvsMassVsDstAngle->GetZaxis()->GetXmax()); 
    H3PvsMVsDstA.Sumw2();


    //----------------------------
    H2PvsM.SetNameTitle("H2PvsM","H2PvsM");
    H2PvsM.SetBins(H2MCPvsMass->GetXaxis()->GetNbins(),
		H2MCPvsMass->GetXaxis()->GetXmin(),
		H2MCPvsMass->GetXaxis()->GetXmax(),
		H2MCPvsMass->GetYaxis()->GetNbins(),
		H2MCPvsMass->GetYaxis()->GetXmin(),
		H2MCPvsMass->GetYaxis()->GetXmax());   

    H2PvsM.Sumw2();


    //
    HDstarHelicityNoShape.SetNameTitle("HDstarHelicityNoShape","HDstarHelicityNoShape");
    HDstarHelicityNoShape.SetBins(HMCDstarHelicity->GetXaxis()->GetNbins(),HMCDstarHelicity->GetXaxis()->GetXmin(),HMCDstarHelicity->GetXaxis()->GetXmax());
    HDstarHelicityNoShape.Sumw2();
    
    
    HDstarHelicity.SetNameTitle("HDstarHelicity","HDstarHelicity");
    HDstarHelicity.SetBins(HMCDstarHelicity->GetXaxis()->GetNbins(),HMCDstarHelicity->GetXaxis()->GetXmin(),HMCDstarHelicity->GetXaxis()->GetXmax());
    HDstarHelicity.Sumw2();
    
    //efficiency corrected mass
    HDstPiMNoShape.SetNameTitle("HDstPiMNoShape","HDstPiMNoShape");
    HDstPiMNoShape.SetBins(HMCXMass->GetXaxis()->GetNbins(),HMCXMass->GetXaxis()->GetXmin(),HMCXMass->GetXaxis()->GetXmax());
    HDstPiMNoShape.Sumw2();
    
    //fully efficiency corrected delta mass
    HDstPiDM.SetNameTitle("HDstPiDM","HDstPiDM");
    HDstPiDM.SetBins(HMCXMass->GetXaxis()->GetNbins(),HMCXMass->GetXaxis()->GetXmin(),HMCXMass->GetXaxis()->GetXmax());
    HDstPiDM.Sumw2();
    
    HPstarNoShape.SetNameTitle("HPstarNoShape","HPstarNoShape");
    HPstarNoShape.SetBins(HMCXp3CM->GetXaxis()->GetNbins(),HMCXp3CM->GetXaxis()->GetXmin(),HMCXp3CM->GetXaxis()->GetXmax());
    HPstarNoShape.Sumw2();
    
    HDstarAngleNoShape.SetNameTitle("HDstarAngleNoShape","HDstarAngleNoShape");
    HDstarAngleNoShape.SetBins(HMCDstarAngle->GetXaxis()->GetNbins(),HMCDstarAngle->GetXaxis()->GetXmin(),HMCDstarAngle->GetXaxis()->GetXmax());
    HDstarAngleNoShape.Sumw2();

    HDstarAngle.SetNameTitle("HDstarAngle","HDstarAngle");
    HDstarAngle.SetBins(HMCDstarAngle->GetXaxis()->GetNbins(),HMCDstarAngle->GetXaxis()->GetXmin(),HMCDstarAngle->GetXaxis()->GetXmax());
    HDstarAngle.Sumw2();
    
    HDstPiCosstarNoShape.SetNameTitle("HDstPiCosstarNoShape","HDstPiCosstarNoShape");
    HDstPiCosstarNoShape.SetBins(HMCXcosthCM->GetXaxis()->GetNbins(),HMCXcosthCM->GetXaxis()->GetXmin(),HMCXcosthCM->GetXaxis()->GetXmax());
    HDstPiCosstarNoShape.Sumw2();
    
    //efficiency corrected mass
    HDstPiMFineNoRes.SetNameTitle("HDstPiMFineNoRes","HDstPiMFineNoRes");
    HDstPiMFineNoRes.SetBins(2000,2.1,4.1);
    HDstPiMFineNoRes.Sumw2();
   
   
    //Generated mass vs p*   
    for(Int_t i=0;i<4;i++){
      HMCMVsPstar[i]=H2MCPvsMassFine->ProjectionY(TString("HMCMVsPstar")+(long)i,i+1,i+1,"");
      if(!HMCMVsPstar[i]){cout<<"bad projection of HMCMVsPstar"<<endl; return 0;}
    } 


    if(!CloseReducedFile())return 0;
  
  }


  ///
  Float_t eff;
  Float_t effnoshape;
  Int_t e=0;
  while(ReducedNtuple->GetEntry(e)){
    e++;
    if(e%50000==0)cout<<e<<" cands done"<<endl;


    //for checking eff shapes after 3-D eff correction
    if(_TruthMatch){
      effnoshape=GetEfficiencyNoShape();
      if(effnoshape>0){//note resolution is removed so should not attemp to fit these 
	HDstarHelicityNoShape.Fill(dstarhelicity,1./effnoshape);
	HDstPiMNoShape.Fill(dstpimdm-dstpidmres,1./effnoshape);
	HPstarNoShape.Fill(dstpipstar,1./effnoshape);
	HDstarAngleNoShape.Fill(dstarcostheta,1./effnoshape);
	HDstPiCosstarNoShape.Fill(dstpicosstar,1./effnoshape);
	H3PvsMVsDstA.Fill(dstpipstar,dstpimdm-dstpidmres,dstarcostheta,1./effnoshape);
	H2PvsM.Fill(dstpipstar,dstpimdm-dstpidmres,1./effnoshape);
      }

    }
    

    HDstPi.Fill(dstpimdm);   
    H2DstPiMVsDstarHelNoEff.Fill(dstpimdm,dstarhelicity);
    ///
    HDstPiDM.Fill(dstpideltam);
    if(fabs(dstarhelicity)>.75)
      HDstPiMD2420Helicity.Fill(dstpimdm);
    if(fabs(dstarhelicity)<.5)
      HDstPiMD2460Helicity.Fill(dstpimdm);

    //for bumps
    for(Int_t i=0;i<10;i++){
      if((-1+.2*i)<dstarhelicity&&dstarhelicity<(-1+.2*(i+1))){
	if(dstarcostheta<-.5)
	  HDstPiMVsHelicityN5Cos[i]->Fill(dstpimdm);
	
	if(dstarcostheta<-.9)
	  HDstPiMVsHelicityN9Cos[i]->Fill(dstpimdm);
	
      }
    }

    for(Int_t i=0;i<4;i++){
      if(.25*i<fabs(dstarhelicity)&&fabs(dstarhelicity)<.25*(i+1))
	HDstPiMVsCoarseAbsHelicity[i]->Fill(dstpimdm);
    }


    //
    for(Int_t i=0;i<10;i++){
      if((-1+.2*i)<dstarhelicity&&dstarhelicity<(-1+.2*(i+1))){
	HDstPiMVsHelicity[i]->Fill(dstpimdm);
      }
    }
    for(Int_t i=0;i<20;i++){
      if((-1+.1*i)<dstarhelicity&&dstarhelicity<(-1+.1*(i+1)))
	HDstPiMVsHelicityFine[i]->Fill(dstpimdm);
    }
    for(Int_t i=0;i<8;i++)
      if((-1+.25*i)<dstarhelicity&&dstarhelicity<(-1.+.25*(i+1)))
	HDstPiMVsHelicityMW[i]->Fill(dstpimdm);



    //p* dependence
    for(Int_t i=0;i<4;i++)
      if((3+.5*i)<dstpipstar&&dstpipstar<(3+.5*(i+1)))
	HDstPiMVsPstar[i]->Fill(dstpimdm); 
    for(Int_t i=0;i<10;i++)
      if((3+.2*i)<dstpipstar&&dstpipstar<(3+.2*(i+1)))
	HDstPiMVsPstarFine[i]->Fill(dstpimdm);
    Float_t energyfraction=sqrt(dstpimass*dstpimass+dstpipstar*dstpipstar)/(OnPeakEnergy/2.);
    for(Int_t i=0;i<30;i++)
      if((.5+.02*i)<energyfraction&&energyfraction<(.5+.02*(i+1)))
	HDstPiMVsEnergyFine[i]->Fill(dstpimdm);

    //cos D*
    for(Int_t i=0;i<20;i++)
      if((-1+.1*i)<dstarcostheta&&dstarcostheta<(-1.+.1*(i+1)))
	HDstPiMVsCosDstar[i]->Fill(dstpimdm);
    for(Int_t i=0;i<8;i++)
      if((-1+.25*i)<dstarcostheta&&dstarcostheta<(-1.+.25*(i+1)))
	HDstPiMVsCosDstarMW[i]->Fill(dstpimdm);
    for(Int_t i=0;i<4;i++){
      if((-1+.5*i)<dstarcostheta&&dstarcostheta<(-1.+.5*(i+1)))
	H2DstPiMVsHelCosDstar[i]->Fill(dstpimdm,dstarhelicity);
    }
    for(Int_t i=0;i<8;i++)
      if((-1+.25*i)<dstarcostheta&&dstarcostheta<(-1.+.25*(i+1))&&fabs(dstarhelicity)>.75)
	HDstPiMVsCosDstarD2420MW[i]->Fill(dstpimdm);
    for(Int_t i=0;i<8;i++)
      if((-1+.25*i)<dstarcostheta&&dstarcostheta<(-1.+.25*(i+1))&&fabs(dstarhelicity)<.5)
	HDstPiMVsCosDstarD2460MW[i]->Fill(dstpimdm);

        
    //phi dependence
    for(Int_t i=0;i<4;i++)
      if((-mypi+i*mypi/2)<dstpiphistar&&dstpiphistar<(-mypi+(i+1)*mypi/2))
	HDstPiMVsPhi[i]->Fill(dstpimdm);



    //cos* dependence
    for(Int_t i=0;i<4;i++)
      if((-1+.5*i)<dstpicosstar&&dstpicosstar<(-1.+(i+1)*.5))
	HDstPiMVsCos[i]->Fill(dstpimdm);



    // p* systematics
    for(Int_t i=0;i<4;i++)
      if((3+.5*i)<dstpipstar&&dstpipstar<(3+.5*(i+1)))
	HDstPiMVsPstarNoEff[i]->Fill(dstpimdm);
    if(fabs(dstarhelicity)>.75)
      for(Int_t i=0;i<4;i++)
	if((3+.5*i)<dstpipstar&&dstpipstar<(3+.5*(i+1)))
	  HDstPiMVsPstarD2420[i]->Fill(dstpimdm);
    if(fabs(dstarhelicity)<.5)
      for(Int_t i=0;i<4;i++)
	if((3+.5*i)<dstpipstar&&dstpipstar<(3+.5*(i+1)))
	  HDstPiMVsPstarD2460[i]->Fill(dstpimdm);
    
    //phi
    for(Int_t i=0;i<4;i++)
      if((-mypi+i*mypi/2)<dstpiphistar&&dstpiphistar<(-mypi+(i+1)*mypi/2))
	HDstPiMVsPhiNoEff[i]->Fill(dstpimdm);
    if(fabs(dstarhelicity)>.75)
      for(Int_t i=0;i<4;i++)
	if((-mypi+i*mypi/2)<dstpiphistar&&dstpiphistar<(-mypi+(i+1)*mypi/2))
	  HDstPiMVsPhiD2420[i]->Fill(dstpimdm);
    if(fabs(dstarhelicity)<.5)
      for(Int_t i=0;i<4;i++)
	if((-mypi+i*mypi/2)<dstpiphistar&&dstpiphistar<(-mypi+(i+1)*mypi/2))
	  HDstPiMVsPhiD2460[i]->Fill(dstpimdm);

    //cos(theta*)
    for(Int_t i=0;i<4;i++)
      if((-1+.5*i)<dstpicosstar&&dstpicosstar<(-1.+(i+1)*.5))
	HDstPiMVsCosNoEff[i]->Fill(dstpimdm);
    if(fabs(dstarhelicity)>.75)
      for(Int_t i=0;i<4;i++)
	if((-1+.5*i)<dstpicosstar&&dstpicosstar<(-1.+(i+1)*.5))
	  HDstPiMVsCosD2420[i]->Fill(dstpimdm);
    if(fabs(dstarhelicity)<.5)
      for(Int_t i=0;i<4;i++)
	if((-1+.5*i)<dstpicosstar&&dstpicosstar<(-1.+(i+1)*.5))
	  HDstPiMVsCosD2460[i]->Fill(dstpimdm);
   


    //
    eff=GetEfficiency();
    if(eff<=0)continue;

    HDstPiEff.Fill(dstpimdm,1./eff);  

    //angles
    HDstarAngle.Fill(dstarcostheta,1./eff);
    HDstarHelicity.Fill(dstarhelicity,1./eff);

    HDstPiMFineNoRes.Fill(dstpimdm-dstpidmres,1./eff);


    ///
    H2DstPiMVsDstarHel.Fill(dstpimdm,dstarhelicity,1./eff);
 
    for(Int_t i=0;i<NPROJS;i++){
      HDstPiMLegendre[i]->Fill(dstpimdm,((2.*i+1.)/2.)*legendre(i,dstarhelicity)/eff);
    }
    for(Int_t i=0;i<8;i++)//weight by P2
      if((-1+.25*i)<dstarcostheta&&dstarcostheta<(-1.+.25*(i+1)))
	HDstPiMP2VsCosDstar[i]->Fill(dstpimdm,((2.*2+1.)/2.)*legendre(2,dstarhelicity)/eff);



  }
  if(CloseReducedNtuple()!=1)return 0;
  CloseEfficiency();


  //   //add error from the efficiency;
  //   TArrayD* sw2=HDstPiMFine.GetSumw2();
  //   for(Int_t i=1;i<=HDstPiMFine.GetNbinsX();i++){
  //     (*sw2)[i]=(*sw2)[i]+pow(.0*HDstPiMFine.GetBinContent(i),2);    
  //     if(i>300&&i<350)cout<<" "<<HDstPiMFine.GetBinError(i)/HDstPiMFine.GetBinContent(i)<<endl;        
  //   }


  TFile HistosForFit(_OutputDir+"/HistosForFit.root","recreate");  

  if(_TruthMatch){
    HDstarHelicityNoShape.Write();
    HDstPiMNoShape.Write();
    HPstarNoShape.Write();
    HDstarAngleNoShape.Write();
    HDstPiCosstarNoShape.Write();
    H3PvsMVsDstA.Write();
    H2PvsM.Write();
    HDstPiMFineNoRes.Write();

    for(Int_t i=0;i<4;i++)      
      HMCMVsPstar[i]->Write();
    
  }

  //
  HDstPi.Write();
  HDstPiEff.Write();
  HDstPiMD2420Helicity.Write();
  HDstPiMD2460Helicity.Write();
  

  //
  HDstarHelicity.Write();
  HDstarAngle.Write();
  HDstPiDM.Write();

  for(Int_t i=0;i<10;i++)
    HDstPiMVsHelicityN5Cos[i]->Write();
  for(Int_t i=0;i<10;i++)
    HDstPiMVsHelicityN9Cos[i]->Write();
  for(Int_t i=0;i<10;i++)
    HDstPiMVsHelicity[i]->Write();
  for(Int_t i=0;i<20;i++)
    HDstPiMVsHelicityFine[i]->Write();
  for(Int_t i=0;i<8;i++)
    HDstPiMVsHelicityMW[i]->Write();

  for(Int_t i=0;i<4;i++)
    HDstPiMVsCoarseAbsHelicity[i]->Write();

  for(Int_t i=0;i<4;i++)
    HDstPiMVsPstar[i]->Write();
  for(Int_t i=0;i<4;i++)
    HDstPiMVsPstarNoEff[i]->Write();
  for(Int_t i=0;i<4;i++)
    HDstPiMVsPstarD2420[i]->Write();
  for(Int_t i=0;i<4;i++)
    HDstPiMVsPstarD2460[i]->Write();
  for(Int_t i=0;i<10;i++)
    HDstPiMVsPstarFine[i]->Write();
  for(Int_t i=0;i<30;i++)
    HDstPiMVsEnergyFine[i]->Write();


  for(Int_t i=0;i<20;i++)
    HDstPiMVsCosDstar[i]->Write();
  for(Int_t i=0;i<8;i++)
    HDstPiMVsCosDstarMW[i]->Write();
  for(Int_t i=0;i<4;i++)
    H2DstPiMVsHelCosDstar[i]->Write();
  for(Int_t i=0;i<8;i++)
    HDstPiMVsCosDstarD2420MW[i]->Write();
  for(Int_t i=0;i<8;i++)
    HDstPiMVsCosDstarD2460MW[i]->Write();



  for(Int_t i=0;i<4;i++)
    HDstPiMVsPhi[i]->Write();
  for(Int_t i=0;i<4;i++)
    HDstPiMVsPhiNoEff[i]->Write();
  for(Int_t i=0;i<4;i++)
    HDstPiMVsPhiD2420[i]->Write();
  for(Int_t i=0;i<4;i++)
    HDstPiMVsPhiD2460[i]->Write();

  for(Int_t i=0;i<4;i++)
    HDstPiMVsCos[i]->Write();
  for(Int_t i=0;i<4;i++)
    HDstPiMVsCosNoEff[i]->Write();
  for(Int_t i=0;i<4;i++)
    HDstPiMVsCosD2420[i]->Write();
  for(Int_t i=0;i<4;i++)
    HDstPiMVsCosD2460[i]->Write();

  H2DstPiMVsDstarHel.Write();
  H2DstPiMVsDstarHelNoEff.Write();


  for(Int_t i=0;i<NPROJS;i++){
    HDstPiMLegendre[i]->Write();
  }
  for(Int_t i=0;i<8;i++)
    HDstPiMP2VsCosDstar[i]->Write();


  HistosForFit.ls();
  HistosForFit.Close();


  //-------------
  //clean up
  
  for(Int_t i=0;i<NPROJS;i++)
    delete HDstPiMLegendre[i];
  for(Int_t i=0;i<8;i++)
    delete HDstPiMP2VsCosDstar[i];
  
  for(Int_t i=0;i<10;i++)
    delete HDstPiMVsHelicityN5Cos[i];
  for(Int_t i=0;i<10;i++)
    delete HDstPiMVsHelicityN9Cos[i];
  for(Int_t i=0;i<10;i++)
    delete HDstPiMVsHelicity[i];  
  for(Int_t i=0;i<20;i++)
    delete HDstPiMVsHelicityFine[i];
  for(Int_t i=0;i<8;i++)
    delete HDstPiMVsHelicityMW[i];
 
  for(Int_t i=0;i<4;i++)
    delete HDstPiMVsCoarseAbsHelicity[i];

  for(Int_t i=0;i<4;i++)
    delete HDstPiMVsPstar[i];
  for(Int_t i=0;i<4;i++)
    delete HDstPiMVsPstarNoEff[i];
  for(Int_t i=0;i<4;i++)
    delete HDstPiMVsPstarD2420[i];
  for(Int_t i=0;i<4;i++)
    delete HDstPiMVsPstarD2460[i];
  for(Int_t i=0;i<10;i++)
    delete HDstPiMVsPstarFine[i];
  for(Int_t i=0;i<30;i++)
    delete HDstPiMVsEnergyFine[i];

  for(Int_t i=0;i<20;i++)
    delete HDstPiMVsCosDstar[i];
  for(Int_t i=0;i<8;i++)
    delete HDstPiMVsCosDstarMW[i];
  for(Int_t i=0;i<4;i++)
    delete H2DstPiMVsHelCosDstar[i];
  for(Int_t i=0;i<8;i++)
    delete HDstPiMVsCosDstarD2420MW[i];
  for(Int_t i=0;i<8;i++)
    delete HDstPiMVsCosDstarD2460MW[i];

  for(Int_t i=0;i<4;i++)
    delete HDstPiMVsPhi[i];
  for(Int_t i=0;i<4;i++)
    delete HDstPiMVsPhiNoEff[i];
  for(Int_t i=0;i<4;i++)
    delete HDstPiMVsPhiD2420[i];
  for(Int_t i=0;i<4;i++)
    delete HDstPiMVsPhiD2460[i];

  for(Int_t i=0;i<4;i++)
    delete HDstPiMVsCos[i];
  for(Int_t i=0;i<4;i++)
    delete HDstPiMVsCosNoEff[i];
  for(Int_t i=0;i<4;i++)
    delete HDstPiMVsCosD2420[i];
  for(Int_t i=0;i<4;i++)
    delete HDstPiMVsCosD2460[i];
  
 
  return 1;
}

/*
Int_t DstPiAnalysis::WeightK3piBkgEvents(Int_t WhichSideBand){
  if(_D0Mode!=2){
    cout<<" This operation only for K3pi mode"<<endl;
    return 0;
  }
  
  
  if(!OpenReducedNtuple(1))return 0;
  
  //create a clean copy
  TTree* Ntuple=ReducedNtuple->CopyTree(database.GetCut(1,3)+database.GetCut(5,100));//dont include D* deltaM cut

  if(!OpenEfficiency()){cout<<"Could not open efficiecy file"<<endl;return 0;}
  
  //efficiency corrected mass
  TH1F HDstPiM("HDstPiM","HDstPiM",200,2.1,3.1);

  //efficiency corrected D* peak
  TH1F HDstarDeltaM("HDstarDeltaM","HDstarDeltaM",300,.130,.160);

  //efficiency corrected mass vs helicity
  TH1F* HDstPiMVsHelicity[10];
  for(Int_t i=0;i<10;i++){
    HDstPiMVsHelicity[i]=new TH1F(TString("HDstPiMVsHelicity")+(long)i,"HDstPiMVsHelicity",200,2.1,3.1);
    HDstPiMVsHelicity[i]->Sumw2();
  }  

  //efficiency corrected mass vs helicity finner binning
  TH1F* HDstPiMVsHelicityFine[20];
  for(Int_t i=0;i<20;i++){
    HDstPiMVsHelicityFine[i]=new TH1F(TString("HDstPiMVsHelicityFine")+(long)i,"HDstPiMVsHelicityFine",200,2.1,3.1);
    HDstPiMVsHelicityFine[i]->Sumw2();
  }  

  //
  TH2F H2DstPiMVsDstarHel("H2DstPiMVsDstarHel","H2DstPiMVsDstarHel",200,2.1,3.1,40,-1.0,1.0);
  H2DstPiMVsDstarHel.Sumw2();

  //
  TH1F* HDstPiMLegendre[NPROJS];
  for(Int_t i=0;i<NPROJS;i++){
    HDstPiMLegendre[i]=new TH1F(TString("HDstPiMLegendre")+(long)i,"HDstPiMLegendre",200,2.1,3.1);
    HDstPiMLegendre[i]->Sumw2();
  }  

  
  //define the cuts on the D* sidebands
  Float_t dmstep=database.GetDstarDeltaMMax()-database.GetDstarDeltaMMin();
  
  ///
  Float_t eff;
  

  Int_t e=0;
  while(Ntuple->GetEntry(e)){
    e++;
    if(e%50000==0)cout<<e<<" cands done"<<endl;


    //
    eff=GetEfficiency();
    if(eff<=0)continue;

    //
    HDstarDeltaM.Fill(dstardeltam,1./eff);   


    //apply cut on D* sidebands
    if(WhichSideBand==1)
      if(!((database.GetDstarDeltaMMin()-2*dmstep)<dstardeltam&&dstardeltam<(database.GetDstarDeltaMMin()-dmstep))) continue;
    if(WhichSideBand==2)
      if(!((database.GetDstarDeltaMMax()+dmstep)<dstardeltam&&dstardeltam<(database.GetDstarDeltaMMax()+2*dmstep))) continue;



    ///
    HDstPiM.Fill(dstpimdm,1./eff);

    //
    for(Int_t i=0;i<10;i++){
      if((-1+.2*i)<dstarhelicity&&dstarhelicity<(-1+.2*(i+1)))
	HDstPiMVsHelicity[i]->Fill(dstpimdm,1./eff);
    }

    //
    for(Int_t i=0;i<20;i++){
      if((-1+.1*i)<dstarhelicity&&dstarhelicity<(-1+.1*(i+1)))
	HDstPiMVsHelicityFine[i]->Fill(dstpimdm,1./eff);
    }


    ///
    H2DstPiMVsDstarHel.Fill(dstpimdm,dstarhelicity,1./eff);
 
    for(Int_t i=0;i<NPROJS;i++){
      HDstPiMLegendre[i]->Fill(dstpimdm,((2.*i+1.)/2.)*legendre(i,dstarhelicity)/eff);
    }
    
  }


  TFile* OutFile=NULL;  
  if(WhichSideBand==1)OutFile=new TFile(_OutputDir+"/LegendreWeightedEventsLowBkg.root","recreate");  
  if(WhichSideBand==2)OutFile=new TFile(_OutputDir+"/LegendreWeightedEventsHighBkg.root","recreate");  
  
  HDstarDeltaM.Write();
  HDstPiM.Write();
  for(Int_t i=0;i<10;i++)
    HDstPiMVsHelicity[i]->Write();

  for(Int_t i=0;i<20;i++)
    HDstPiMVsHelicityFine[i]->Write();

  H2DstPiMVsDstarHel.Write();
  for(Int_t i=0;i<NPROJS;i++){
    HDstPiMLegendre[i]->Write();
  }

  OutFile->ls();
  OutFile->Close();

  if(CloseReducedNtuple()!=1)return 0;
  CloseEfficiency();


  for(Int_t i=0;i<10;i++)
    delete HDstPiMVsHelicity[i];

  for(Int_t i=0;i<20;i++)
    delete HDstPiMVsHelicityFine[i];

  for(Int_t i=0;i<NPROJS;i++)
    delete HDstPiMLegendre[i];


  return 1;
}



Int_t DstPiAnalysis::PlotLegendreWeightedEvents(){
  filename=_OutputDir+"/LegendreWeightedEvents.ps";
  Canvas.Print(filename+"[");
  
  TFile* File=NULL;
  File=new TFile(_OutputDir+"/HistosForFit.root","read");
  if(!File)return 0;
  if(File->IsZombie())return 0;
  gROOT->cd();
    
 
  TH2* H2DstPiMVsDstarHel=(TH2*)File->Get("H2DstPiMVsDstarHel");
  if(!H2DstPiMVsDstarHel){cout<<"No H2DstPiMVsDstarHel"<<endl;return 0;}

  Canvas.Clear();
  H2DstPiMVsDstarHel->Draw("colz");
  Canvas.Print(filename);


  TH1D* HMass=H2DstPiMVsDstarHel->ProjectionX();
  HMass->SetName("HMass");
  HMass->GetYaxis()->SetRangeUser(0,1.1*HMass->GetMaximum());
  Canvas.Clear();
  HMass->Draw();
  Canvas.Print(filename);
  delete HMass;


  TH1D* HDstarHel=H2DstPiMVsDstarHel->ProjectionY();
  HDstarHel->SetName("HDstarHel");
  Canvas.Clear();
  HDstarHel->GetYaxis()->SetRangeUser(0,1.1*HDstarHel->GetMaximum());
  HDstarHel->Draw();
  Canvas.Print(filename);
  delete HDstarHel;

  ///save the projections
  Canvas.Clear();
  Canvas.Divide((int)sqrt((float)NPROJS)+1,(int)sqrt((float)NPROJS));
  text.SetTextSize(.1);
  text.SetTextColor(2);

  ///now create the Legendre projections
  TH1* HDstPiMVsDstarHelLegProj[NPROJS];

  for(Int_t p=0;p<NPROJS;p++){
    Canvas.cd(p+1);

    HDstPiMVsDstarHelLegProj[p]=(TH1*)File->Get(TString("HDstPiMLegendre")+(long)p);
    if(!HDstPiMVsDstarHelLegProj[p])return 0;   
 
    HDstPiMVsDstarHelLegProj[p]->Draw("pe");
    text.DrawTextNDC(.5,.7,TString("l=")+(long)p);
    cutline.DrawLine(HDstPiMVsDstarHelLegProj[p]->GetXaxis()->GetXmin(),0,HDstPiMVsDstarHelLegProj[p]->GetXaxis()->GetXmax(),0);
    
  }
  Canvas.Print(filename);  


  Canvas.Print(filename+"]");
  
  File->Close();
  delete File;

  return 1;
}

*/








