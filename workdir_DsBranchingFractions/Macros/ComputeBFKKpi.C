#define NXT 7

void ComputeBFKKpi(Int_t _DataType){
  TFile FDsFit("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/DenominatorFitResult_MC.root","read");//Denominator
  TFile FNumeratorFit("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResultsKKpi/KKpiFitResult_MC.root","read");//Numerator  
  TFile FDenominatorEff("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/DenominatorEff.root","r");//Ds efficiency
  TFile FNumeratorEff("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResultsKKpi/KKpiEffNumerator.root","r");//KKpi efficiency
  TFile FDalitzWeight("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResultsKKpi/DalitzWeights.root","r");//Dalitz weights contains both MC and Data


  //------------------------------
  //Numerator efficiencies
  //------------------------------  
  if(FNumeratorEff.IsZombie()){cout<<"Bad FNumeratorEff File"<<endl; return 0;} 
  if(FDenominatorEff.IsZombie()){cout<<"Bad FDenominatorEff File"<<endl; return 0;}

  ifstream DsProducedFile; 
  DsProducedFile.open("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/DsProduced.txt");
  ifstream KKpiProducedFile; 
  KKpiProducedFile.open("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResultsKKpi/KKpiProduced.txt");

  Float_t Eff[NXT];
  Float_t EffErr[NXT];
  Float_t EffDs[NXT];
  Float_t EffDsErr[NXT];
  Float_t EffKKpi[NXT];
  Float_t EffKKpiErr[NXT];
  Float_t TotKKpiProd=24439880+24409072;//49439345.93-->does not remove DamageFilter;
  Float_t KKpiProd[NXT];
  Float_t KKpiRec[NXT];
  Float_t TotDsProd=0;
  Float_t DsProd[NXT];
  Float_t DsRec[NXT];


  //Add up all the Gen Ds 
  for(Int_t j=0;j<NXT;j++){
    Int_t indexcheckn;
    Float_t NProdDs=0.;
    DsProducedFile>>indexcheckn>>NProdDs;   
    if(indexcheckn!=j){cout<<" NXT index does not match for Ds produced   j="<<j<<endl;exit(0);}
    DsProd[j]=NProdDs;
    TotDsProd+=NProdDs;

    TH2F*HDsEff=(TH2F*)FDenominatorEff.Get(TString("HDsMassTrue2D_nXT")+(long)j);
    if(!HDsEff){cout<<"No HDsEff"<<j<<endl;return 0;}
    DsRec[j]=HDsEff->Integral();
    
  }

  //determine nXT gen weights
  Float_t GenNXTWeights[NXT];
  for(Int_t j=0;j<NXT;j++){
    GenNXTWeights[j]=DsProd[j]/TotDsProd;
    //cout<<j<<" DsGenW="<<GenNXTWeights[j]<<endl;
  }

     
  //KKpi signal events
  for(Int_t j=0;j<NXT;j++){ 
    KKpiProd[j]=TotKKpiProd*GenNXTWeights[j];
    
    TH1F*HEff=(TH1F*)FNumeratorEff.Get(TString("HKKpiGamMassSignal_nXT")+(long)j);
    //if(!HEff){cout<<"No HEff"<<d<<" "<<j<<endl;return 0;}
    KKpiRec[j]=HEff->Integral();
  }
  
    
  //compute the efficiency
  //cout<<endl;
  for(Int_t j=0;j<NXT;j++){
    EffKKpi[j]=KKpiRec[j]/KKpiProd[j];
    if(_DataType==1){EffKKpi[j]=EffKKpi[j]*1.01493; cout<<"MC dalitz correction applied j="<<j<<endl;}
    if(_DataType==2){EffKKpi[j]=EffKKpi[j]*1.01955; cout<<"Data dalitz correction applied j="<<j<<endl;}
    EffKKpiErr[j]=sqrt(KKpiRec[j])/KKpiProd[j];  

    EffDs[j]=DsRec[j]/DsProd[j];
    EffDsErr[j]=sqrt(DsRec[j])/DsProd[j]; 

    Eff[j]=EffKKpi[j]/EffDs[j];
    EffErr[j]=Eff[j]*sqrt(pow(sqrt(KKpiRec[j])/KKpiRec[j],2)+pow(sqrt(DsRec[j])/DsRec[j],2));  

  }
  //cout<<endl;


  //------------------------------
  ///Ds yield 
  //------------------------------ 
  TFile FDenominatorTrueYield("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResults/DenominatorEff.root","r");

  //------------------------------
  //Numerator yield
  //------------------------------
  TFile FNumeratorTrueYield("DsTolnu/KKpi/SP1005/HistosForFit.root","r");

  Float_t NRecoDs[NXT];
  Float_t NRecoNumerator[NXT];
  TH1F HDs("HDs","",NXT,-.5,NXT-.5);
  HDs.GetXaxis()->SetTitle("n_{X}^{T}");
  HDs.GetYaxis()->SetTitle("reco. events");
  TH1F HKKpi("HKKpi","",NXT,-.5,NXT-.5);
  HKKpi.GetXaxis()->SetTitle("n_{X}^{T}");
  HKKpi.GetYaxis()->SetTitle("reco. events");
  TH1F HCorrDs("HCorrDs","",NXT,-.5,NXT-.5);
  HCorrDs.GetXaxis()->SetTitle("n_{X}^{T}");
  HCorrDs.GetYaxis()->SetTitle("corr. reco. events");
  TH1F HCorrKKpi("HCorrKKpi","",NXT,-.5,NXT-.5);
  HCorrKKpi.GetXaxis()->SetTitle("n_{X}^{T}");
  HCorrKKpi.GetYaxis()->SetTitle("corr. reco. events");
  TH1F HBFEff("HBFEff","",NXT,-.5,NXT-.5);
  HBFEff.GetXaxis()->SetTitle("n_{X}^{T}");
  HBFEff.GetYaxis()->SetTitle("efficiency");
  TH1F HBF("HBF","",NXT,-.5,NXT-.5);
  HBF.GetXaxis()->SetTitle("n_{X}^{T}");
  HBF.GetYaxis()->SetTitle("B(D_{s}#rightarrow KK#pi)");

  for(Int_t j=0;j<NXT;j++){
    NRecoDs[j]=((TH1F*)FDenominatorTrueYield.Get(TString("HDsMassTrue2D_nXT")+(long)j))->Integral();
    NRecoNumerator[j]=((TH1F*)FNumeratorTrueYield.Get(TString("HKKpiGamMassSignal_nXT")+(long)j))->Integral();
    
    Float_t BF=(NRecoNumerator[j]/EffKKpi[j])/(NRecoDs[j]/EffDs[j]);
    Float_t BFErr=BF*sqrt(pow(sqrt(NRecoNumerator[j])/NRecoNumerator[j],2)
			  +pow(sqrt(NRecoDs[j])/NRecoDs[j],2)
			  +pow(EffErr[j]/Eff[j],2));
    printf("%d  %.3e  %.3e  %.3e  %.4f +- %.4f\n",j,NRecoDs[j],NRecoNumerator[j],Eff[j],BF,BFErr);
    //cout<<j<<" Ds="<<NRecoDs[j]<<"  KKpi="<<NRecoNumerator[j]<<" eff="<<Eff[j]<<"   BF="<<BF<<" +- "<<BFErr<<endl;

    HDs.SetBinContent(j+1,NRecoDs[j]);
    HDs.SetBinError(j+1,sqrt(NRecoDs[j]));
    HKKpi.SetBinContent(j+1,NRecoNumerator[j]);
    HKKpi.SetBinError(j+1,sqrt(NRecoNumerator[j]));
    HCorrDs.SetBinContent(j+1,NRecoDs[j]/EffDs[j]);
    HCorrDs.SetBinError(j+1,(NRecoDs[j]/EffDs[j])*sqrt(pow(sqrt(NRecoDs[j])/NRecoDs[j],2)+pow(EffDsErr[j]/EffDs[j],2)));
    HCorrKKpi.SetBinContent(j+1,NRecoNumerator[j]/EffKKpi[j]);
    HCorrKKpi.SetBinError(j+1,(NRecoNumerator[j]/EffKKpi[j])*sqrt(pow(sqrt(NRecoNumerator[j])/NRecoNumerator[j],2)+pow(EffKKpiErr[j]/EffKKpi[j],2)));
    HBFEff.SetBinContent(j+1,Eff[j]);
    HBFEff.SetBinError(j+1,EffErr[j]);
    HBF.SetBinContent(j+1,BF);
    HBF.SetBinError(j+1,BFErr);

  }


  TCanvas C("C");
  C.Print("ComputeBFKKpi.ps[");
  
  C.Clear();
  HDs.GetYaxis()->SetRangeUser(0, HDs.GetMaximum()*1.2);
  HDs.Draw();
  C.Print("ComputeBFKKpi.ps");

  C.Clear();
  HKKpi.GetYaxis()->SetRangeUser(0, HKKpi.GetMaximum()*1.2);
  HKKpi.Draw();
  C.Print("ComputeBFKKpi.ps");

  C.Clear();
  HCorrDs.GetYaxis()->SetRangeUser(0, HCorrDs.GetMaximum()*1.2);
  HCorrDs.Draw();
  C.Print("ComputeBFKKpi.ps");

  C.Clear();
  HCorrKKpi.GetYaxis()->SetRangeUser(0, HCorrKKpi.GetMaximum()*1.2);
  HCorrKKpi.Draw();
  C.Print("ComputeBFKKpi.ps");

  C.Clear();
  HBFEff.GetYaxis()->SetRangeUser(0, HBFEff.GetMaximum()*1.2);
  HBFEff.Draw();
  C.Print("ComputeBFKKpi.ps");

  C.Clear();
  HBF.Fit("pol0");
  HBF.GetYaxis()->SetRangeUser(0, HBF.GetMaximum()*1.4);
  HBF.Draw();
  C.Print("ComputeBFKKpi.ps");
  
  C.Print("ComputeBFKKpi.ps]");
}



void ComputeDalitzCorrection(Int_t _DataType){
  
  
  TFile FNumeratorEff("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResultsKKpi/KKpiEffNumerator.root","r");//KKpi efficiency
  TFile FDalitzWeight("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResultsKKpi/DalitzWeights.root","r");//Dalitz weights contains both MC and Data
  if(FNumeratorEff.IsZombie()){cout<<"Bad FNumeratorEff File"<<endl; return ;} 
  if(FDalitzWeight.IsZombie()){cout<<"Bad FDalitzWeight File"<<endl; return ;}

  ifstream KKpiProducedFile; 
  KKpiProducedFile.open("/afs/slac.stanford.edu/g/babar/www/Physics/Analysis/AWG/Charm/fDs2009/Denominator/NominalFitResultsKKpi/KKpiProduced.txt");


  Float_t TotKKpiProd=0;
  Float_t TotKKpiRec=0;
  Float_t KKpiProd[3];
  Float_t KKpiRec[3];     
  for(Int_t j=0;j<NXT;j++){
    for(Int_t d=0;d<3;d++){ 
      Int_t jcheck;
      Int_t dcheck;
      Float_t nKKpi;
      KKpiProducedFile>>jcheck>>dcheck>>nKKpi;
      //cout<<j<<" "<<d<<endl;
      if(jcheck!=j||dcheck!=d+1){
	cout<<"no match j="<<j<<":"<<jcheck<<" d="<<d<<":"<<dcheck<<" for file KKpiProducedFile"<<endl;
	return;
      }
      KKpiProd[d]+=nKKpi;
      TotKKpiProd+=nKKpi;
    }
  } 

  for(Int_t d=0;d<3;d++){ 
    TH1F*HEff=(TH1F*)FNumeratorEff.Get(TString("HKKpiGamMassSignal_Dalitz")+(long)d);
    if(!HEff){cout<<"No HEff"<<d<<endl;return;}
    KKpiRec[d]=HEff->Integral();
    TotKKpiRec+=HEff->Integral();
  }
  
  //Determine Total efficiency from KKpi Signal MC 
  Float_t TotEff=TotKKpiRec/TotKKpiProd;
  cout<<"Total efficiency "<<TotEff<<endl;

  //determine efficiency for each dalitz region
  Float_t Eff[3];
  Float_t EffErr[3];
  for(Int_t d=0;d<3;d++){ 
    Eff[d]=KKpiRec[d]/KKpiProd[d];
    EffErr[d]=sqrt(KKpiRec[d])/KKpiProd[d];
    cout<<d<<"  "<<Eff[d]<<"+-"<<EffErr[d]<<endl;
  }


  //determine weighted efficiency 
  Float_t dalitzweight[3];
  Float_t totDalitzweight=0.;
  for(Int_t d=0;d<3;d++){
    TH1F*HDalitzWeight=NULL;
    if(_DataType==1)HDalitzWeight=(TH1F*)FDalitzWeight.Get(TString("HDsMassMCDalitz")+(long)d);
    if(_DataType==2)HDalitzWeight=(TH1F*)FDalitzWeight.Get(TString("HDsMassDataDalitz")+(long)d);
    if(!HDalitzWeight)cout<<"Bad Dalitz Histo"<<d<<endl;
    dalitzweight[d]=HDalitzWeight->GetBinContent(0);//ignore weight errors
    totDalitzweight+= dalitzweight[d];
    cout<<" Dalitz weight "<<d<<" ="<<dalitzweight[d]<<endl;
  }
  if(fabs(totDalitzweight-1.)>1e-5){cout<<" Dalitz weights do not add up to 1."<<endl; return 0;}
  Float_t TotEffWeighted=0;  
  for(Int_t d=0;d<3;d++){ 
    TotEffWeighted+= dalitzweight[d]* Eff[d];
  }
  cout<<"Weighted efficiency = "<<TotEffWeighted<<endl;

  //determine correction factor: total/weighted 
  cout<<" correction factor Weighted/Total="<<TotEffWeighted/TotEff<<endl;


}
