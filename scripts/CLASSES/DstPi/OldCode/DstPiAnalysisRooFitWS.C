#include "DstPiAnalysis.h"

#define FITMIN 2.200
#define FITMAX 3.150
#define NFITBINS 190


///---------------------
//The variable to be fitted
//----------------------
RooRealVar roomass("roomass","roomass",FITMIN,FITMAX);
 

RooRealVar dau1m("dau1m","dau1m",DstarPDGMass);
RooRealVar dau2m("dau2m","dau2m",PiPDGMass);
Roo2BodyMassPhaseSpace TwoBodyPS("TwoBodyPS","TwoBodyPS",roomass,dau1m,dau2m);

///parameters
RooRealVar coef1("coef1","coef1",-100,100);
RooRealVar coef2("coef2","coef2",-100,100); 
RooRealVar coef3("coef3","coef3",-100,100);  
RooRealVar coef4("coef4","coef4",-100,100); 
RooRealVar coef5("coef5","coef5",-100,100);  
RooRealVar coef6("coef6","coef6",-100,100); 
RooRealVar coef7("coef7","coef7",-100,100);  
RooRealVar coef8("coef8","coef8",-100,100); 
RooRealVar coef9("coef9","coef9",-100,100);  
RooRealVar coef10("coef10","coef10",1,1e7); 
RooRealVar coef11("coef11","coef11",-100,100);  
RooRealVar coef12("coef12","coef12",-100,100); 
RooRealVar coef13("coef13","coef13",-100,100);  
RooRealVar coef14("coef14","coef14",-100,100); 

//yields 
RooRealVar Yield0("Yield0","Yield0",10000,1,1e6);
RooRealVar Yield1("Yield1","Yield1",1000,1,1e6);

//RooRealVar
  
RooAddPdf* RooFitWSPdf0(){
  
  coef1.setRange(1,20); 
  coef2.setRange(1,20);
  coef3.setRange(1,20);  
  coef4.setRange(1,20);
  coef5.setRange(1,20);
  coef6.setRange(1,20);

  

  RooGenericPdf* Pdf0=new RooGenericPdf("Pdf0","(roomass**coef1)*exp(-coef2*roomass-(coef3*roomass)**2)*(roomass<2.45)+(2.45**(coef1-coef4))*exp(-2.45*(coef2-coef5)-((coef3*2.45)**2-(coef6*2.45)**2))*(roomass**coef4)*exp(-(coef5*roomass)**1-(coef6*roomass)**2)*(roomass>2.45)",RooArgSet(roomass,coef10,coef1,coef2,coef3,coef4,coef5,coef6));   
  RooProdPdf* PSPdf0=new RooProdPdf("PSPdf0","PSPdf0",TwoBodyPS,*Pdf0);   
  
  
  RooGenericPdf* Pdf1=new RooGenericPdf("Pdf1","1",RooArgSet(roomass));
  Yield1.setVal(0);
  Yield1.setConstant(1);  


  RooAddPdf* totalpdf=new RooAddPdf("totalpdf","totalpdf",RooArgList(*PSPdf0,*Pdf1),RooArgList(Yield0,Yield1));
 

  return totalpdf;
}



RooAddPdf* RooFitWSPdf1(){

  RooGenericPdf* Pdf0=new RooGenericPdf("Pdf0","1+coef2*roomass**2+coef4*roomass**4+coef6*roomass**6+coef8*roomass**8+coef10*roomass**10+coef12*roomass**12+coef14*roomass**14",RooArgSet(roomass,coef2,coef4,coef6,coef8,coef10,coef12,coef14));//  
  RooProdPdf* PSPdf0=new RooProdPdf("PSPdf0","PSPdf0",TwoBodyPS,*Pdf0);   
    
  RooGenericPdf* Pdf1=new RooGenericPdf("Pdf1","roomass+coef3*roomass**3+coef5*roomass**5+coef7*roomass**7+coef9*roomass**9+coef11*roomass**11+coef13*roomass**13",RooArgSet(roomass,coef1,coef3,coef5,coef7,coef9,coef11,coef13));//  
  RooProdPdf* PSPdf1=new RooProdPdf("PSPdf1","PSPdf1",TwoBodyPS,*Pdf1);

  RooAddPdf* totalpdf=new RooAddPdf("totalpdf","totalpdf",RooArgList(*PSPdf0,*PSPdf1),RooArgList(Yield0,Yield1));
  
  return totalpdf;
}



RooAddPdf* RooFitWSPdf2(){
  
  coef1.setRange(.1,20);
  coef2.setRange(.1,20);
  coef3.setRange(.1,20);
  
  RooGenericPdf* Pdf0=new RooGenericPdf("Pdf0","(roomass**coef3)*exp(-(coef1*roomass)**1-(coef2*roomass)**2)",RooArgSet(roomass,coef1,coef2,coef3));
  RooProdPdf* PSPdf0=new RooProdPdf("PSPdf0","PSPdf0",TwoBodyPS,*Pdf0);   
    
  RooGenericPdf* Pdf1=new RooGenericPdf("Pdf1","1",RooArgSet(roomass));
  Yield1.setVal(0);
  Yield1.setConstant(1);

  RooAddPdf* totalpdf=new RooAddPdf("totalpdf","totalpdf",RooArgList(*PSPdf0,*Pdf1),RooArgList(Yield0,Yield1));
  
  return totalpdf;
}


RooAddPdf* RooFitWSPdf3(){

  coef1.setRange(.01,10);
  coef2.setRange(1,100);
  coef3.setRange(.01,10);
  coef4.setRange(1,100);

  RooGenericPdf* Pdf0=new RooGenericPdf("Pdf0","roomass*exp(-((roomass-coef3)/coef1)**2)",RooArgSet(roomass,coef1,coef3));//
  RooProdPdf* PSPdf0=new RooProdPdf("PSPdf0","PSPdf0",TwoBodyPS,*Pdf0);   
 
  RooGenericPdf* Pdf1=new RooGenericPdf("Pdf1","roomass*exp(-((roomass-coef4)/coef2)**2)",RooArgSet(roomass,coef2,coef4));//
  RooProdPdf* PSPdf1=new RooProdPdf("PSPdf1","PSPdf1",TwoBodyPS,*Pdf1);

  RooAddPdf* totalpdf=new RooAddPdf("totalpdf","totalpdf",RooArgList(*PSPdf0,*PSPdf1),RooArgList(Yield0,Yield1));
    
  return totalpdf;
}


RooAddPdf* RooFitWSPdf4(){
  
  coef1.setRange(.1,1);
  coef1.setVal(.5);
  coef1.setConstant(0);
  coef2.setRange(.001,20);
  coef2.setVal(10);
  coef3.setRange(.001,20);
  coef3.setVal(3);
  coef4.setRange(.001,20);
  coef4.setVal(2); 

  RooArgSet coeflist(roomass,dau1m,dau2m,coef1,coef2,coef3,coef4);
       
  RooGenericPdf* Pdf0=new RooGenericPdf("Pdf0","((roomass-dau1m-dau2m)**coef1)*exp(- coef2*roomass - coef3*roomass**2 - coef4*roomass**3 )",coeflist);
  RooProdPdf* PSPdf0=new RooProdPdf("PSPdf0","PSPdf0",TwoBodyPS,*Pdf0);   

  RooGenericPdf* Pdf1=new RooGenericPdf("Pdf1","1",coeflist);
  RooProdPdf* PSPdf1=new RooProdPdf("PSPdf1","PSPdf1",TwoBodyPS,*Pdf1);   
  Yield1.setVal(0);
  Yield1.setConstant(1);  

  RooAddPdf* totalpdf=new RooAddPdf("totalpdf","totalpdf",RooArgList(*PSPdf0,*PSPdf1),RooArgList(Yield0,Yield1));
  
  return totalpdf;
}


RooAddPdf* RooFitWSPdf5(){
  
  coef1.setRange(.001,100);
  RooArgSet coeflist(roomass,dau1m,dau2m,coef1);
  
  RooGenericPdf* Pdf0=new RooGenericPdf("Pdf0","sqrt((roomass/(dau1m+dau2m))**2 - 1)*exp(-coef1*roomass)",coeflist);

  RooGenericPdf* Pdf1=new RooGenericPdf("Pdf1","1",RooArgSet(roomass));
  Yield1.setVal(0);
  Yield1.setConstant(1);  

  RooAddPdf* totalpdf=new RooAddPdf("totalpdf","totalpdf",RooArgList(*Pdf0,*Pdf1),RooArgList(Yield0,Yield1));
  
  return totalpdf;
}

RooAddPdf* RooFitWSPdf6(){
  
  coef1.setRange(2.150,2.4);
  coef2.setRange(.01,10);  
  
  RooGenericPdf* Pdf0=new RooGenericPdf("Pdf0","sqrt((roomass/(dau1m+dau2m))**2 - 1)*exp(-((roomass-coef1)/coef2)**2)",RooArgSet(roomass,dau1m,dau2m,coef1,coef2));


  RooGenericPdf* Pdf1=new RooGenericPdf("Pdf1","1",RooArgSet(roomass));
  Yield1.setVal(0);
  Yield1.setConstant(1);  

  RooAddPdf* totalpdf=new RooAddPdf("totalpdf","totalpdf",RooArgList(*Pdf0,*Pdf1),RooArgList(Yield0,Yield1));
  
  return totalpdf;
}


RooAddPdf* RooFitWSPdf7(){
  
  coef1.setRange(-1,2);
  coef2.setRange(-1,2); 
  coef3.setRange(-1,2);
  coef4.setRange(-1,2);
  coef5.setRange(-1,2);
  coef6.setRange(-1,2);
  coef7.setRange(-1,2);
  coef8.setRange(-1,2);
  coef9.setRange(-1,2);  
  coef10.setRange(-1,2);
  coef11.setRange(-1,2);  

  RooArgSet coeflist(roomass,coef1,coef2,coef3,coef4,coef5,coef6); 
  //RooArgList coeflist(coef1,coef2,coef3,coef4,coef5,coef6,coef7);  
  coeflist.add(coef7);
//   coeflist.add(coef8);
//   coeflist.add(coef9);
//   coeflist.add(coef10);
//   coeflist.add(coef11);

  //sqrt((roomass/(dau1m+dau2m))**2 - 1)* //+coef7*roomass**7+coef8*roomass**8+coef9*roomass**9+coef10*roomass**10+coef11*roomass**11
  RooGenericPdf* Pdf0=new RooGenericPdf("Pdf0","1+coef1*roomass+coef2*roomass**2+coef3*roomass**3+coef4*roomass**4+coef5*roomass**5+coef6*roomass**6",coeflist);
  //RooChebychev* Pdf0=new RooChebychev("Pdf0","Pdf0",roomass,coeflist);
  //RooProdPdf* PSPdf0=new RooProdPdf("PSPdf0","PSPdf0",TwoBodyPS,*Pdf0);   




  RooGenericPdf* Pdf1=new RooGenericPdf("Pdf1","1",RooArgSet(roomass));
  Yield1.setVal(0);
  Yield1.setConstant(1);  

  RooAddPdf* totalpdf=new RooAddPdf("totalpdf","totalpdf",RooArgList(*Pdf0,*Pdf1),RooArgList(Yield0,Yield1));
  
  return totalpdf;
}





Int_t DstPiAnalysis::CreateHistosRooFitWS(){

  ///Open the reduced Ntuple
  if(OpenReducedNtuple(1)!=1) return 0;
    
  //apply some cuts 
  TCut wscut="abs(dstpicharge)==2";
  TCut TotalCut=database.GetCut(2,7)+wscut;

  //------------------------------
  //define the data to be fitted
  //------------------------------
  TFile Ftemp(_OutputDir+"FtempWSFit","recreate");//needed for memory
  TTree* ReducedNtupleCopy=ReducedNtuple->CopyTree(TotalCut);
  ReducedNtupleCopy->SetBranchStatus("*",0);
  if(CloseReducedNtuple()!=1) return 0;
  

  //----------
  //Plot the D* deltaM
  //----------
  TH1F HDstar("HDstar","HDstar",database.GetDstarDeltaMRangeNbins(),database.GetDstarDeltaMRangeMin(),database.GetDstarDeltaMRangeMax()); 
  ReducedNtupleCopy->SetBranchStatus("dstardeltam",1); 
  ReducedNtupleCopy->SetBranchStatus("dstarcostheta",1); 
  ReducedNtupleCopy->Draw("dstardeltam>>HDstar");
 
  TCut cut;
  TCut lowcut;
  TH1F* HDstarPi[8];
  TH1F* HDstarPiSlice[8];
  ////--------------Get plot for full range
  ReducedNtupleCopy->SetBranchStatus("dstpideltam",1); 
  for(Int_t c=0;c<8;c++){
    sprintf(textlabel,"dstarcostheta<%.2f",1.-c*.25);
    cut=TCut(textlabel);
    sprintf(textlabel,"dstarcostheta>%.2f",1.-(c+1)*.25);
    lowcut=TCut(textlabel);
    
    HDstarPi[c]=new TH1F(TString("HDstarPi")+(long)c,TString("HDstarPi")+(long)c,1200,2.050,3.250);    
    ReducedNtupleCopy->Draw(TString("dstpideltam+2.01>>HDstarPi")+(long)c,cut);   

    HDstarPiSlice[c]=new TH1F(TString("HDstarPiSlice")+(long)c,TString("HDstarPiSlice")+(long)c,1200,2.050,3.250);
    ReducedNtupleCopy->Draw(TString("dstpideltam+2.01>>HDstarPiSlice")+(long)c,cut+lowcut);    

  }

  //return 0;

  //2-D Map
  TH2F H2DstarPi("H2DstarPi","H2DstarPi",40,-1.001,1.001,1500,2.00,3.50);
  ReducedNtupleCopy->Draw("(dstpideltam+2.01):dstarcostheta>>H2DstarPi");
  

  TFile File(_OutputDir+"/HistosForWSFit.root","recreate");
  File.mkdir("RawHistos");
  File.cd("RawHistos");
  HDstar.Write();
  H2DstarPi.Write();
  for(Int_t c=0;c<8;c++){
    HDstarPi[c]->Write();
    HDstarPiSlice[c]->Write();
  }
  File.Write();
  File.ls();
  File.Close();


  for(Int_t c=0;c<8;c++){
    delete HDstarPi[c];
    delete HDstarPiSlice[c];
  }
  
  Ftemp.Close();

  return 1;

}

Int_t CutBins(TH1*h,Int_t firstbin,Int_t lastbin){
  for(Int_t b=firstbin;b<=lastbin;b++){
    h->SetBinContent(b,0);
    h->SetBinError(b,0);    
  }

  return 1;
}

TH1F* DstPiAnalysis::RooFitWSHisto(TH1* HDstarPi,TCanvas* Canvas,TString filename,Int_t whichpdf){

  
  //create the final binned data
  RooDataHist binneddata("binneddata","binneddata",RooArgList(roomass),HDstarPi);


  roomass.setBins(NFITBINS);  
  RooAddPdf* totalpdf=NULL;
  if(whichpdf==0)totalpdf=RooFitWSPdf0();
  if(whichpdf==1)totalpdf=RooFitWSPdf1();
  if(whichpdf==2)totalpdf=RooFitWSPdf2();
  if(whichpdf==3)totalpdf=RooFitWSPdf3();
  if(whichpdf==4)totalpdf=RooFitWSPdf4();
  if(whichpdf==5)totalpdf=RooFitWSPdf5();
  if(whichpdf==6)totalpdf=RooFitWSPdf6();
  if(whichpdf==7)totalpdf=RooFitWSPdf7();
  if(!totalpdf)return NULL;

  //
  totalpdf->fitTo(binneddata,"mt");//"mhert"
  
  //----------------------------
  //the Fit plot
  //----------------------------
  //   RooPlot* roomassframe=roomass.frame();
  //   Canvas->Clear(); 
  //   binneddata.plotOn(roomassframe);
  //   totalpdf->plotOn(roomassframe);
  //   roomassframe->Draw();
  //   Canvas->Print(filename);


  //----------------------------
  //now make the residual plot
  //----------------------------
  TH1* HFitDstPiDM=totalpdf->createHistogram(TString(HDstarPi->GetName())+"HFitDstPiDM",roomass);
  if(HFitDstPiDM==NULL){
    cout<<"No HDstPiDM  returned."<<endl;
    Canvas->Print(filename+"]");return NULL;
  }
  HFitDstPiDM->Scale(HDstarPi->Integral()); 

 
  JResiduals roofitresiduals("res",HDstarPi,HFitDstPiDM);
//   TH1F* ResidualHisto=roofitresiduals.GetResidualHisto();//returns ownership
//   Canvas->Clear();
//   ResidualHisto->Draw("pe");
//   cutline.SetLineColor(1);
//   cutline.DrawLine(FITMIN,0,FITMAX,0);
//   Canvas->Print(filename); 

  Canvas->Clear();
  roofitresiduals.MakeFitPlotWithResiduals(Canvas,0);
  Canvas->Print(filename); 
 
  TH1F* Hnormres=roofitresiduals.GetNormResidualHisto();
  delete HFitDstPiDM;
  delete totalpdf;
  return Hnormres;
}



Int_t DstPiAnalysis::RooFitWS(Bool_t CreateNewHistos,Int_t whichpdf){
  cout<<"Going to fit the D*Pi Wrong Sign distribution"<<endl;

  //ps file
  filename=_OutputDir+"/DstPiRooFitWS.ps";
  Canvas.Print(filename+"[");
    

  ///print the cuts into the file
  database.PrintCutList(&Canvas,filename);  


  //-------------------------------------------------------- 
  //Get the data
  //--------------------------------------------------------
  if(CreateNewHistos)
    if(!CreateHistosRooFitWS())return 0;

  TFile HistFile(_OutputDir+"/HistosForWSFit.root","read");
  Canvas.Clear();
  TH1* HDstar=(TH1*)HistFile.Get("HDstar");
  if(!HDstar)return 0;
  SetHistoXYLabels(HDstar,"m(K^{+}#pi^{-}#pi^{+})-m(K^{+}#pi^{-})  (GeV/c^{2})","GeV");
  HDstar->Draw("pe");
  Canvas.Print(filename);

  TH1* HDstarPi[8];
  TH1* HNormRes[8];
  for(Int_t c=0;c<8;c++){
    Canvas.Clear();
    HDstarPi[c]=(TH1*)HistFile.Get(TString("HDstarPi")+(long)c);
    if(!HDstarPi[c])return 0;
    HDstarPi[c]=CreateRebinnedDstPiHisto(HDstarPi[c],NFITBINS,FITMIN,FITMAX);
    if(!HDstarPi[c])return 0;
    SetHistoXYLabels(HDstarPi[c],DstarPiXLabel,"GeV");
    HNormRes[c]=RooFitWSHisto(HDstarPi[c],&Canvas,filename,whichpdf);
    if(!HNormRes[c])return 0;
  }

  
  Canvas.Clear();
  Canvas.Divide(2,4);
  cutline.SetLineColor(2);
  for(Int_t c=0;c<8;c++){
    Canvas.cd(c+1);
    HNormRes[c]->Draw("pe");
    cutline.DrawLine(FITMIN,2,FITMAX,2);
    cutline.DrawLine(FITMIN,-2,FITMAX,-2);
  }
  Canvas.Print(filename);


  Canvas.Clear();
  HDstarPi[0]->GetYaxis()->SetRangeUser(0,HDstarPi[0]->GetMaximum());
  HDstarPi[0]->Draw();
  for(Int_t c=1;c<8;c++){
    HDstarPi[c]->Draw("same");
  }
  Canvas.Print(filename);
  
 
  Canvas.Print(filename+"]");
  HistFile.Close();

  return 1;
}






