#define NRESPOINTS 12
#define RESMIN 2.3
#define RESMAX 2.6

Int_t DCPiAnalysis::MakeResolution(){
  if(!_TruthMatch)return 0;
  
  if(OpenReducedNtuple()!=1) return 0;

  TFile file(_OutputDir+"/Resolution.root","recreate");

  TH1F HResTotal("HResTotal","",100,-.05,.05);
  ReducedNtuple->Draw("xdmres>>HResTotal");
  HResTotal.Write(); 

  Float_t mstep=(RESMAX-RESMIN)/NRESPOINTS;

  for(Int_t i=0;i<NRESPOINTS;i++){
    TH1F HRes(TString("HRes")+(long)i,TString("HRes")+(long)i,100,-.05,.05);
    sprintf(textlabel,"%.6f<xmdm-xdmres&&xmdm-xdmres<%.6f",RESMIN+i*mstep,RESMIN+(i+1)*mstep);
    ReducedNtuple->Draw(TString("xdmres>>HRes")+(long)i,textlabel);
    HRes.Write(); 
  }
  

  file.ls();
  file.Close();

  return 1;

}

Int_t DCPiAnalysis::FitResolutionTotal(){
  if(!_TruthMatch)return 0;
  
  TFile file(_OutputDir+"/Resolution.root","read");
  file.ls();

  TH1F* HResTotal=(TH1F*)file.Get("HResTotal"); 
  
  filename=_OutputDir+"/ResolutionTotal.ps";
  Canvas.Print(filename+"["); 
  Canvas.Clear();
  HResTotal->Draw("pe");
  Canvas.Print(filename);
  
  RooDataHist dataset("dataset","dataset",resVar,HResTotal); 

//use the offset as the sigma variable
  Float_t offsetmin=resoffset.getMin();
  Float_t offsetmax=resoffset.getMax();  
  Float_t offsetval=resoffset.getVal();  
  resoffset.setRange(.0001,.02);
  resoffset.setVal(.002);
  resoffset.setConstant(0);
  Float_t slopeval=resslope.getVal();
  resslope.setVal(0);
  mass.setConstant(1);

  resmean.setConstant(0);
  resnormratio2.setConstant(0);
  resmean2.setConstant(0);
  ressigmaratio2.setConstant(0);
  resnormratio3.setConstant(0);
  resmean3.setConstant(0);
  ressigmaratio3.setConstant(0);


  ///Total Fit
  respdf->fitTo(dataset,"m");
  RooPlot* plot=resVar.frame();
  dataset.plotOn(plot);
  respdf->plotOn(plot);
  Canvas.Clear();
  plot->Draw();
  Canvas.Print(filename);
  Canvas.Clear();
  Canvas.SetLogy(1);
  plot->Draw();
  Canvas.Print(filename);
  delete plot;
  
  
  
  Canvas.Print(filename+"]"); 

  resmean.setConstant(1);
  resoffset.setRange(offsetmin,offsetmax);
  resoffset.setVal(offsetval);
  resoffset.setConstant(1);
  resslope.setVal(slopeval);
  resslope.setConstant(1);
  resnormratio2.setConstant(1);
  resmean2.setConstant(1);
  ressigmaratio2.setConstant(1);
  resnormratio3.setConstant(1);
  resmean3.setConstant(1);
  ressigmaratio3.setConstant(1);
  mass.setConstant(0);


  return 1;
}



Int_t DCPiAnalysis::FitResolution(){
  if(!_TruthMatch)return 0;
  
  TFile file(_OutputDir+"/Resolution.root","read");
  file.ls();

  filename=_OutputDir+"/Resolution.ps";
  Canvas.Print(filename+"["); 

  //use the offset as the sigma variable
  Float_t offsetmin=resoffset.getMin();
  Float_t offsetmax=resoffset.getMax();  
  Float_t offsetval=resoffset.getVal();  
  resoffset.setRange(.0001,.02);
  resoffset.setVal(.002);
  resoffset.setConstant(0);
  Float_t slopeval=resslope.getVal();
  resslope.setVal(0);
  mass.setConstant(1);

  //Fit vs mass
  TH1F HSigma("HSigma","",NRESPOINTS,RESMIN,RESMAX);
  HSigma.GetYaxis()->SetTitle("#sigma (MeV)");
  HSigma.GetXaxis()->SetTitle("m(D#pi) (GeV)");
  HSigma.SetStats(0);
  TH1F HFWHM("HFWHM","",NRESPOINTS,RESMIN,RESMAX);
  HFWHM.GetYaxis()->SetTitle("HWHM (MeV)");
  HFWHM.GetXaxis()->SetTitle("m(D#pi) (GeV)");
  HFWHM.SetStats(0);
  Canvas.Clear();
  Int_t nx=2;Int_t ny=4;
  Canvas.Divide(nx,ny);
  Int_t ngood=0; 
  text.SetTextSize(.2);
  RooPlot* plots[10];
  for(Int_t i=0;i<NRESPOINTS;i++){
 
    TH1F* HRes=(TH1F*)file.Get(TString("HRes")+(long)i);
    if(!HRes){cout<<" no histo"<<endl;return 0;}
    if(HRes->Integral()<1500)continue;
    ngood++;
    if(ngood>nx*ny){cout<<" Ran out of Canvas pads"<<endl;continue;}

    Canvas.cd(ngood);
    Canvas.GetPad(ngood)->SetLogy(1);
    Canvas.GetPad(ngood)->SetTopMargin(.09);
    Canvas.GetPad(ngood)->SetBottomMargin(.2);
    Canvas.GetPad(ngood)->SetLeftMargin(.12);
    Canvas.GetPad(ngood)->SetRightMargin(.12);

    RooDataHist dataset("dataset","dataset",resVar,HRes); 
    respdf->fitTo(dataset,"m");
    plots[ngood-1]=resVar.frame();
    dataset.plotOn(plots[ngood-1]);
    respdf->plotOn(plots[ngood-1]);
   
    plots[ngood-1]->SetTitle("");
    plots[ngood-1]->GetXaxis()->SetTitle("m_{rec}-m_{gen}  (GeV)");
    //plots[ngood-1]->GetXaxis()->SetTitle("");
    plots[ngood-1]->GetXaxis()->SetTitleSize(.1);
    plots[ngood-1]->GetYaxis()->SetTitleSize(.1);
    plots[ngood-1]->GetYaxis()->SetTitleOffset(.5);
    plots[ngood-1]->GetXaxis()->SetLabelSize(.1);
    plots[ngood-1]->GetYaxis()->SetLabelSize(.1);
    plots[ngood-1]->GetYaxis()->SetNdivisions(5);
    plots[ngood-1]->GetXaxis()->SetNdivisions(5);

    plots[ngood-1]->Draw();
    text.DrawTextNDC(.2,.7,TString("")+(long)ngood);

    
    Float_t fwhm;
    Float_t mean;
    Float_t xofymax;
    DetermineFWHM(respdf,&resVar,&fwhm,&mean,&xofymax);
    HFWHM.SetBinContent(i,1000*fwhm/2);
    HSigma.SetBinContent(i,1000*resoffset.getVal());
    HSigma.SetBinError(i,1000*resoffset.getError());

   
    cout<<"Done with point "<<ngood<<" histo:"<<HRes->GetName()<<endl;
    
  }
  Canvas.Print(filename);
  Canvas.SetLogy(0);
 //  for(Int_t i=0;i<10;i++)
//     if(plots[i]) delete plots[i];


  file.Close();
  if(ngood<1)return 0;


  Canvas.Clear();
  HSigma.Fit("pol1");
  HSigma.Draw("pe");
  Canvas.Print(filename);  

  Canvas.Clear();
  HFWHM.Draw("p");
  Canvas.Print(filename);    
  
  Canvas.Print(filename+"]"); 

  //return parameters to initial
  resmean.setConstant(1);
  resoffset.setRange(offsetmin,offsetmax);
  resoffset.setVal(offsetval);
  resoffset.setConstant(1);
  resslope.setVal(slopeval);
  resslope.setConstant(1);
  mass.setConstant(0);


  return 1;
}
