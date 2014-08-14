#include "DstPiAnalysis.h"
#include "../RooNLLVar1D.h"
#include "RooMinuit.h"


Int_t DstPiAnalysis::GenerateToySigOnBkg(Bool_t generate,Int_t ntoys,Int_t fileid){
  if(_TruthMatch){cout<<" this test only for generic and data"<<endl;return 0;}

  Int_t nbins=120;
  Float_t fitmin=2.26;
  Float_t fitmax=2.62;
  Float_t gensyld=3.9136e+04;
  Float_t genmean=2.422;
  Float_t genwidth=.019;
  Float_t gensyld2=1.9895e+04;
  Float_t genmean2=2.459;
  Float_t genwidth2=.030;
  Float_t gensyld3=0;
  Float_t genmean3=2.610;
  Float_t genwidth3=.1;
  Float_t genbyld=5.7126e+05;
  //----------------------
  //from not truthed ccbar fit with ps*exp(pol4)
  Float_t c1=-8.3805e+01;
  Float_t c2=4.2185e+01 ;
  Float_t c3=-8.8243e+00;
  Float_t c4=6.8929e-01;
  //---------------
  Int_t Nevents=637000;
  if(_MCorDATA==1){
    nbins=120;
    fitmin=2.26;
    fitmax=2.62;
    genmean=2.422;
    genwidth=.030;
    genmean2=2.460;
    genwidth2=.040;
    //----------------------
    gensyld=54664;//from Kpi data fit with DWBW+DWBW+pol4==> Range:2.260--2.620
    gensyld2=19086;//from Kpi data fit with DWBW+DWBW+pol4==> Range:2.260--2.620
    Nevents=362583;//from histo integral Range:2.260--2.620
    //Nevents=507280;//from histo integral Range:2.2--3.1
    //----------------------
    gensyld3=25000;
    genmean3=2.610;
    genwidth3=.15;
    if(fileid/3==0) genwidth3=genwidth3*.7;
    if(fileid/3==2) genwidth3=genwidth3*1.3;
    if(fileid%3==0) gensyld3=gensyld3*.7;
    if(fileid%3==2) gensyld3=gensyld3*1.3;

    genbyld=Nevents-gensyld-gensyld2-gensyld3;
  }


  RooRealVar mass("mass","mass",fitmin,fitmax);//original fine binning for histo
  mass.setBins(nbins);  
  
  RooRealVar dau1m("dau1m","dau1m",DstarPDGMass);
  RooRealVar dau2m("dau2m","dau2m",PiPDGMass);
  Roo2BodyMassPhaseSpace TwoBodyPS("TwoBodyPS","TwoBodyPS",mass,dau1m,dau2m);
  RooRealVar radius("radius","radius",3);//5GeV~1fm 3GeV~.6f

  RooRealVar mean("mean","mean",2.40,2.44);
  RooRealVar width("width","width",.001,.05);
  RooAbsPdf* bw=new RooBreitWigner("bw","bw",mass,mean,width);
  if(_MCorDATA==1)bw=new RooDwaveBreitWigner("bw","bw",&mass,mean,width,&TwoBodyPS,radius);  
  RooRealVar syld("syld","syld",1,1e7);  

 
  RooRealVar mean2("mean2","mean2",2.44,2.48);
  RooRealVar width2("width2","width2",.001,.06);
  RooAbsPdf* bw2=new RooBreitWigner("bw2","bw2",mass,mean2,width2);  
  if(_MCorDATA==1)bw2=new RooDwaveBreitWigner("bw2","bw2",&mass,mean2,width2,&TwoBodyPS,radius);
  RooRealVar syld2("syld2","syld2",1,1e7);  


  RooRealVar mean3("mean3","mean3",genmean3);
  RooRealVar width3("width3","width3",genwidth3);
  RooBreitWigner bump("bump","bump",mass,mean3,width3);  
  RooRealVar syld3("syld3","syld3",1,1e7);  
  
  
  RooRealVar coef1("coef1","coef1",-1e10,1e10);
  RooRealVar coef2("coef2","coef2",-1e10,1e10);
  RooRealVar coef3("coef3","coef3",-1e10,1e10);
  RooRealVar coef4("coef4","coef4",-1e10,1e10);
  RooRealVar coef5("coef5","coef5",-1e10,1e10);
  RooRealVar coef6("coef6","coef6",-1e10,1e10);
  RooRealVar coef7("coef7","coef7",-1e10,1e10);
  RooRealVar coef8("coef8","coef8",-1e10,1e10);
  RooRealVar coef9("coef9","coef9",-1e10,1e10);
  RooRealVar coef10("coef10","coef10",-1e10,1e10);
  RooArgList coeflist;
  coeflist.add(mass);  
  ////-----phasespace*exp(pol4)---- as background shape from not truthed ccbar
  coeflist.add(coef1);
  coeflist.add(coef2);
  coeflist.add(coef3);
  coeflist.add(coef4);
  RooGenericPdf expdecay("expdecay",GetBackgroundFormula(1),coeflist);
  RooProdPdf bkg("bkg","bkg",TwoBodyPS,expdecay);  
  RooRealVar byld("byld","byld",0,1e7);
  
  ////--add sig plus bkg
  RooArgList pdflist;
  pdflist.add(*bw);
  pdflist.add(*bw2);
  pdflist.add(bkg);
  if(_MCorDATA==1)pdflist.add(bump);

  RooArgList yieldlist;
  yieldlist.add(syld);
  yieldlist.add(syld2);
  yieldlist.add(byld);
  if(_MCorDATA==1)yieldlist.add(syld3); 
  RooAddPdf totalpdf("totalpdf","totalpdf",pdflist,yieldlist);
 
  if(generate){
    TFile file(_OutputDir+"/ToySigOnBkg"+(long)fileid+".root","recreate");
    mean.setVal(genmean);
    width.setVal(genwidth);
    syld.setVal(gensyld);
  
    mean2.setVal(genmean2);
    width2.setVal(genwidth2);
    syld2.setVal(gensyld2);

    mean3.setVal(genmean3);
    width3.setVal(genwidth3);
    syld3.setVal(gensyld3);

    coef1.setVal(c1);
    coef2.setVal(c2);
    coef3.setVal(c3);
    coef4.setVal(c4);
    byld.setVal(genbyld);    
  
    RooPlot*pl=mass.frame();
    bw->plotOn(pl);
    bw2->plotOn(pl);
    bkg.plotOn(pl);
    if(_MCorDATA==1)bump.plotOn(pl);
    pl->SetName("pdfs");
    pl->Write();

    for(Int_t i=0;i<ntoys;i++){
      RooDataSet*bwdatset=totalpdf.generate(RooArgSet(mass),Nevents);
      RooDataHist bwdathist("bwhist","bwhist",mass,*bwdatset);  
      TH1*BwHisto=bwdathist.createHistogram("BwHisto",mass);
      BwHisto->SetBinError(1,sqrt(BwHisto->GetBinContent(1)));
      BwHisto->SetName(TString("BwHisto")+(long)i);
      BwHisto->Write();
    }
    file.ls();
    file.Close();    
  }
  


  ///Now fit all the toys
  TFile file(_OutputDir+"/ToySigOnBkg"+(long)fileid+".root","update");
  file.Delete("ToyResults;1");
    

  RooRealVar x0("x0","x0",0);x0.setConstant(1);
  RooRealVar x1("x1","x1",0);x1.setConstant(1);
  RooRealVar y0("y0","y0",0);y0.setConstant(1);
  RooRealVar y1("y1","y1",0);y1.setConstant(1);  
  RooPolynomialFixEdge bkgpol("bkgpol","bkgpol",mass,x0,y0,x1,y1,byld,coef3,coef4,coef5,coef6,coef7,coef8,coef9,coef10);

  ////--add sig plus bkg
  RooArgList yieldlistfit;
  yieldlistfit.add(syld);
  yieldlistfit.add(syld2);
  yieldlistfit.add(byld);
  RooArgList pdflistfit;
  pdflistfit.add(*bw);
  pdflistfit.add(*bw2);
  pdflistfit.add(bkgpol);
  RooAddPdf totalpdffit("totalpdf","totalpdf",pdflistfit,yieldlistfit);


  
  Float_t pullmean;
  Float_t pullwidth;
  Float_t pullmean2;
  Float_t pullwidth2; 
  Float_t chiperdf;
  Float_t pullmeaner;
  Float_t pullwidther;
  Float_t pullmean2er;
  Float_t pullwidth2er; 
  Int_t id=-1;
  Int_t status=0;
  TTree ToyResults("ToyResults","ToyResults",99);
  ToyResults.Branch("pullmean",&pullmean,"pullmean/F");
  ToyResults.Branch("pullwidth",&pullwidth,"pullwidth/F");
  ToyResults.Branch("pullmean2",&pullmean2,"pullmean2/F");
  ToyResults.Branch("pullwidth2",&pullwidth2,"pullwidth2/F");
  ToyResults.Branch("pullmeaner",&pullmeaner,"pullmeaner/F");
  ToyResults.Branch("pullwidther",&pullwidther,"pullwidther/F");
  ToyResults.Branch("pullmean2er",&pullmean2er,"pullmean2er/F");
  ToyResults.Branch("pullwidth2er",&pullwidth2er,"pullwidth2er/F");
  ToyResults.Branch("id",&id,"id/I");
  ToyResults.Branch("status",&status,"status/I");
  ToyResults.Branch("chiperdf",&chiperdf,"chiperdf/F");

  Int_t ntotal=0;
  Int_t ngood=0;
  TH1*BwHisto;
 
  while((BwHisto=(TH1*)file.Get(TString("BwHisto")+(long)(++id)))&&id<ntoys){
    ntotal++;
    //BwHisto=CreateRebinnedDstPiHisto(BwHisto,nbins,fitmin,fitmax);
    RooDataHist bwdathist("bwhist","bwhist",mass,BwHisto); 
  
    //init signals
    mean.setVal(2.42);
    width.setVal(.01);
    syld.setVal(10000);
    mean2.setVal(2.45);
    width2.setVal(.02);
    syld2.setVal(5000);
    
    //init background 
    x0.setVal(BwHisto->GetBinCenter(1));
    y0.setVal(BwHisto->GetBinContent(1)/BwHisto->GetBinWidth(1));
    x1.setVal(BwHisto->GetBinCenter(BwHisto->GetNbinsX()));
    y1.setVal(BwHisto->GetBinContent(BwHisto->GetNbinsX())/BwHisto->GetBinWidth(1));  
    coef3.setVal(0);coef3.setConstant(0);
    coef4.setVal(0);coef4.setConstant(0);
    coef5.setVal(0);coef5.setConstant(0);
    coef6.setVal(0);coef6.setConstant(0);
    coef7.setVal(0);coef7.setConstant(0);
    coef8.setVal(0);coef8.setConstant(0);
    coef9.setVal(0);coef9.setConstant(0);
    coef10.setVal(0);coef10.setConstant(0);
    if(_MCorDATA==0){//for ccbar use 3rd order pol for data use 4th order
      coef4.setConstant(1);
    }
    coef5.setConstant(1);
    coef6.setConstant(1);
    coef7.setConstant(1);
    coef8.setConstant(1);
    coef9.setConstant(1);
    coef10.setConstant(1);
    byld.setVal(40000);
    if(!bkgpol.InitParameters()){cout<<"Bkg initialized bad."<<endl; return 0;}

    //--------------Fit-------------
    RooChi2Var chi2("chi2","chi2",totalpdffit,bwdathist);
    RooMinuit minuit(chi2);
    minuit.migrad();  
    RooFitResult* res=minuit.save();
    
    if(res->status()!=0)minuit.migrad(); //try again
    res=minuit.save();

    const RooArgList pars=res->floatParsFinal();
    cout<<id<<" minNLL="<<res->minNll()/(BwHisto->GetXaxis()->GetNbins()-pars.getSize())<<"  edm="<<res->edm()
	<<" "<<mean.getVal()
	<<" "<<width.getVal()
	<<" "<<mean2.getVal()
	<<" "<<width2.getVal()
	<<endl;
   
    if(res->status()!=0){cout<<"fit "<<id<<" failed"<<endl;}
    else ngood++;
    
    status=res->status();
    pullmean=(mean.getVal()-genmean)*1000;
    pullwidth=(width.getVal()-genwidth)*1000;
    pullmean2=(mean2.getVal()-genmean2)*1000;
    pullwidth2=(width2.getVal()-genwidth2)*1000;
    pullmeaner=mean.getError()*1000;
    pullwidther=width.getError()*1000;
    pullmean2er=mean2.getError()*1000;
    pullwidth2er=width2.getError()*1000;
    chiperdf=res->minNll()/(BwHisto->GetXaxis()->GetNbins()-pars.getSize());
    ToyResults.Fill();


    
    RooPlot* pl=mass.frame();
    bwdathist.plotOn(pl);
    totalpdffit.plotOn(pl);      
    totalpdffit.plotOn(pl,Components(bkgpol),LineStyle(kDashed),LineColor(1)); 
    pl->SetTitle("");
    pl->SetName(TString("Plot")+(long)id);
    file.Delete(TString("Plot")+(long)id+";1");
    pl->Write();


    //delete BwHisto;
  }
  ToyResults.Write();


  file.Close();

  cout<<" Ngood/ Ntotal="<<ngood<<"/"<<ntotal<<endl;

  return 1;
}


Int_t DstPiAnalysis::PlotToySigOnBkg(Int_t fileid){
 
  filename=_OutputDir+"/ToySigOnBkg"+(long)fileid+".ps";
  Canvas.Print(filename+"[");
  

  //
  TH1F HMean("HMean","",20,-5,5);
  HMean.GetXaxis()->SetTitle("D(2420) Mass pull ");
  TH1F HWidth("HWidth","",20,-5,5);
  HWidth.GetXaxis()->SetTitle("D(2420) Width pull ");
  TH1F HMean2("HMean2","",20,-5,5);
  HMean2.GetXaxis()->SetTitle("D(2460) Mass pull  ");
  TH1F HWidth2("HWidth2","",20,-5,5);
  HWidth2.GetXaxis()->SetTitle("D(2460) Width pull  ");

  TH1F HChisq("HChisq","",30,0,3);
  HChisq.GetXaxis()->SetTitle(" #chi^{2}/DF");

  
  TFile file(_OutputDir+"/ToySigOnBkg"+(long)fileid+".root","read");
  TTree*Toy=(TTree*)file.Get("ToyResults");
  if(!Toy)return 0;
  gROOT->cd();

  cout<<Toy->GetEntries()<<" entries"<<endl;
  
  Toy->Draw("pullmean/pullmeaner>>HMean","status==0","pe");//,"status==0","pe"
  Toy->Draw("pullwidth/pullwidther>>HWidth","status==0","pe");
  Toy->Draw("pullmean2/pullmean2er>>HMean2","status==0","pe");
  Toy->Draw("pullwidth2/pullwidth2er>>HWidth2","status==0","pe");
  Toy->Draw("chiperdf>>HChisq");


  RooPlot*pl=NULL;
  Int_t p=-1;
  while((pl=(RooPlot*)file.Get(TString("Plot")+(long)(++p)))&&p<10){
    Canvas.Clear();
    pl->Draw();
    Canvas.Print(filename);
  }


 
      //    Canvas.Clear();   
//       pl->Draw();
//       text.SetTextSize(.03);
//       sprintf(textlabel,"M=%.2f+-%.2f   W=%.2f+-%.2f,  M2=%.2f+-%.2f   W2=%.2f+-%.2f",	
// 	      mean.getVal()*1000,mean.getError()*1000,
// 	      width.getVal()*1000,width.getError()*1000,
// 	      mean2.getVal()*1000,mean2.getError()*1000,
// 	      width2.getVal()*1000,width2.getError()*1000);
//       text.DrawTextNDC(.01,.01,textlabel);
//       Canvas.Print(filename);

  Canvas.Clear();
  //HMean.Draw("pe");
  HMean.Fit("gaus","QIL","pe");
  Canvas.Print(filename);

  Canvas.Clear();
  HWidth.Fit("gaus","QIL","pe");//.Draw("pe");
  Canvas.Print(filename);
  

  Canvas.Clear();
  HMean2.Fit("gaus","QIL","pe");//.Draw("pe");
  Canvas.Print(filename);

  Canvas.Clear();
  HWidth2.Fit("gaus","QIL","pe");//.Draw("pe");
  Canvas.Print(filename);
  
  Canvas.Clear();
  HChisq.Draw("pe");
  Canvas.Print(filename);
  

  pl=(RooPlot*)file.Get("pdfs;1");
  if(!pl)return 0;
  Canvas.Clear();
  pl->Draw();
  Canvas.Print(filename);

  Canvas.Print(filename+"]");

  //cout<<" Ngood/ Ntotal="<<ngood<<"/"<<ntotal<<endl;
  file.Close();
  return 1;
}




/*
Int_t DstPiAnalysis::GenerateToySmearing(){
   // Use a Breit-Wigner to generate a histogram then smear bin contents with resolution.
   // Use a Breit-Wigner and convolute with resolution for comparison.
  

  cout<<"Generating Toy Smearing"<<endl;



  TFile file("ToySmearing.root","recreate");
  
  
  mass.setRange(2.32,2.53);
  mass.setBins(70);
  resVar.setRange(-.01,.01);
  resVar.setBins(100);

  mass.setVal(2.430);
  TH1*ResHisto=respdf->createHistogram("ResHisto",resVar);
  ResHisto->SetName("ResHisto");
  ResHisto->Write();

  RooRealVar mean("mean","mean",2.422);
  RooRealVar width("width","width",.019);
  RooBreitWigner bw("bw","bw",mass,mean,width);  
  RooDataSet*bwdatset=bw.generate(RooArgSet(mass),100000);
  RooDataHist bwdathist("bwhist","bwhist",mass,*bwdatset);  
  TH1*BwHisto=bwdathist.createHistogram("BwHisto",mass);
  BwHisto->SetBinError(1,sqrt(BwHisto->GetBinContent(1)));
  BwHisto->SetName("BwHisto");
  BwHisto->Write();
    
  //smear histogram 
  TF1 fgaus1("faus1","exp(-.5*(x-[0])**2/[1]**2)/([1]*2.5066)",-.03,.03);
  TF1 fgaus2("faus2","exp(-.5*(x-[0])**2/[1]**2)/([1]*2.5066)",-.03,.03);  
  TF1 fsig("fsig","[1]*x-[0]",2.1,3.1);
  fsig.SetParameters(resoffset,resslope);
  Float_t center;
  Float_t center2;
  Float_t binw=BwHisto->GetBinWidth(1);
  TH1F BwHistoS("BwHistoS","BwHistoS",BwHisto->GetXaxis()->GetNbins(),BwHisto->GetXaxis()->GetXmin(),BwHisto->GetXaxis()->GetXmax());
  for(Int_t i=1;i<BwHisto->GetXaxis()->GetNbins();i++){
    center=BwHisto->GetBinCenter(i);
    fgaus1.SetParameters(resmean,fsig.Eval(center));
    fgaus2.SetParameters(resmean-resmeandelta,ressigmaratio*fsig.Eval(center));
    for(Int_t j=1;j<BwHisto->GetXaxis()->GetNbins();j++){
      center2=BwHistoS.GetBinCenter(j);
      BwHistoS.AddBinContent(j,(1.-resnormratio)*BwHisto->GetBinContent(i)*fgaus1.Eval(center2-center)*binw);
      BwHistoS.AddBinContent(j,resnormratio*BwHisto->GetBinContent(i)*fgaus2.Eval(center2-center)*binw);  
    }
  } 
  for(Int_t j=1;j<BwHisto->GetXaxis()->GetNbins();j++)
    BwHistoS.SetBinError(j,sqrt(BwHistoS.GetBinCenter(j))); 
  BwHistoS.Write();
  
  RooConvolutionPdf bwconv("bwconv","bwconv",&mass,&bw,&resVar,respdf,.03);  
  RooDataSet*bwconvdatset=bwconv.generate(RooArgSet(mass),100000);
  RooDataHist bwconvdathist("bwconvhist","bwconvhist",mass,*bwconvdatset);  
  TH1*BwconvHisto=bwconvdathist.createHistogram("BwconvHisto",mass);
  BwconvHisto->SetBinError(1,sqrt(BwconvHisto->GetBinContent(1)));
  BwconvHisto->SetName("BwconvHisto");
  BwconvHisto->Write();

  file.Close();
 
  return 1;
}


Int_t DstPiAnalysis::FitToySmearing(){
  cout<<"Fitting Toy Smearing"<<endl;
  
  filename="ToySmearing.ps";
  Canvas.Print(filename+"["); 
  
  TFile file("ToySmearing.root","read");
  TH1*ResHisto=(TH1*)file.Get("ResHisto");
  if(!ResHisto)return 0;
  Canvas.Clear();
  ResHisto->Draw("p");
  Canvas.Print(filename);  


  RooRealVar mean("mean","mean",2.4,2.6);
  RooRealVar width("width","width",.001,.1);
  text.SetTextSize(.04);
  RooPlot*pl=NULL;


  TH1*BwHisto=(TH1*)file.Get("BwHisto");
  if(!BwHisto)return 0; 
  RooDataHist bwdata("binneddata","binneddata",RooArgList(mass),BwHisto);
  RooBreitWigner bw("bw","bw",mass,mean,width);  
  bw.fitTo(bwdata,"m");
  pl=mass.frame();
  bwdata.plotOn(pl);
  bw.plotOn(pl);
  pl->SetTitle("");
  Canvas.Clear();  
  pl->Draw();
  sprintf(textlabel,"Mean=%.2f+-%.2f",mean.getVal()*1000,mean.getError()*1000);
  text.DrawTextNDC(.01,.06,textlabel);
  sprintf(textlabel,"Width=%.2f+-%.2f",width.getVal()*1000,width.getError()*1000);
  text.DrawTextNDC(.01,.01,textlabel);
  Canvas.Print(filename);  



  TH1*BwconvHisto=(TH1*)file.Get("BwconvHisto");
  if(!BwconvHisto)return 0;   
  RooDataHist bwconvdata("binneddata","binneddata",RooArgList(mass),BwconvHisto);
  //fit without convolution
  bw.fitTo(bwconvdata,"m");
  pl=mass.frame();
  bwconvdata.plotOn(pl);
  bw.plotOn(pl);
  pl->SetTitle("");
  Canvas.Clear();  
  pl->Draw();
  sprintf(textlabel,"Mean=%.2f+-%.2f",mean.getVal()*1000,mean.getError()*1000);
  text.DrawTextNDC(.01,.06,textlabel);
  sprintf(textlabel,"Width=%.2f+-%.2f",width.getVal()*1000,width.getError()*1000);
  text.DrawTextNDC(.01,.01,textlabel);
  Canvas.Print(filename);  
  //fit with convolution
  RooConvolutionPdf bwconv("bwconv","bwconv",&mass,&bw,&resVar,respdf,.03);  
  bwconv.fitTo(bwconvdata,"m");
  pl=mass.frame();
  bwconvdata.plotOn(pl);
  bwconv.plotOn(pl);
  pl->SetTitle("");
  Canvas.Clear();  
  pl->Draw();
  sprintf(textlabel,"Mean=%.2f+-%.2f",mean.getVal()*1000,mean.getError()*1000);
  text.DrawTextNDC(.01,.06,textlabel);
  sprintf(textlabel,"Width=%.2f+-%.2f",width.getVal()*1000,width.getError()*1000);
  text.DrawTextNDC(.01,.01,textlabel);
  Canvas.Print(filename);
  //plot difference between bw and smeared-bw
  Canvas.Clear();
  BwconvHisto->Sumw2();
  BwconvHisto->Add(BwHisto,-1);
  BwconvHisto->Draw("pe");
  cutline.DrawLine(2.32,0,2.53,0);
  Canvas.Print(filename);


  TH1*BwHistoS=(TH1*)file.Get("BwHistoS");
  if(!BwHistoS)return 0;   
  RooDataHist bwSdata("binneddata","binneddata",RooArgList(mass),BwHistoS);
  //fit without convolution
  bw.fitTo(bwSdata,"m");
  pl=mass.frame();
  bwSdata.plotOn(pl);
  bw.plotOn(pl);
  pl->SetTitle("");
  Canvas.Clear();  
  pl->Draw();
  sprintf(textlabel,"Mean=%.2f+-%.2f",mean.getVal()*1000,mean.getError()*1000);
  text.DrawTextNDC(.01,.06,textlabel);
  sprintf(textlabel,"Width=%.2f+-%.2f",width.getVal()*1000,width.getError()*1000);
  text.DrawTextNDC(.01,.01,textlabel);
  Canvas.Print(filename);  
  //fit with convolution
  bwconv.fitTo(bwSdata,"m");
  pl=mass.frame();
  bwSdata.plotOn(pl);
  bwconv.plotOn(pl);
  pl->SetTitle("");
  Canvas.Clear();  
  pl->Draw();
  sprintf(textlabel,"Mean=%.2f+-%.2f",mean.getVal()*1000,mean.getError()*1000);
  text.DrawTextNDC(.01,.06,textlabel);
  sprintf(textlabel,"Width=%.2f+-%.2f",width.getVal()*1000,width.getError()*1000);
  text.DrawTextNDC(.01,.01,textlabel);
  Canvas.Print(filename);
  //plot difference
  Canvas.Clear();
  BwHistoS->Sumw2();
  BwHistoS->Add(BwHisto,-1);
  BwHistoS->Draw("pe");
  cutline.DrawLine(2.32,0,2.53,0);
  Canvas.Print(filename);


  file.Close();

  Canvas.Print(filename+"]");     
  return 1;
}

 */
