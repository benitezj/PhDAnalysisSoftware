#include "../RooTagCutOptimizer.h"

Int_t TagAnalysis::OptimizeCuts(){
  
  if(OpenReducedNtuple()!=1) return 0;
  ReducedNtuple->SetBranchStatus("*",0);
  ReducedNtuple->SetBranchStatus("tagmass",1);
  ReducedNtuple->SetBranchStatus("taglund",1);
  ReducedNtuple->SetBranchStatus("tagmode",1);
  ReducedNtuple->SetBranchStatus("tagpstar",1);
  ReducedNtuple->SetBranchStatus("taglogvtxprob",1);
  ReducedNtuple->SetBranchStatus("tagpid",1);
  
  TCut modecut="taglund==1&&tagmode==3";
  
  TTree*sigt=ReducedNtuple->CopyTree(TCut("abs(tagmass-1.8645)<.04")+modecut);
  TTree*bkgt=ReducedNtuple->CopyTree(TCut(".06<abs(tagmass-1.8645)&&abs(tagmass-1.8645)<.1")+modecut);//cannot change minimum, .06 is hard coded in RooTagCutOptimizer.
  
  ///Variables for Minuit Optimization 
  RooRealVar pmin("pmin","tagpstar>%6f",2.4,3.);pmin.setUnit("p*");pmin.setError(.1);pmin.setVal(2.4);//pmin.setConstant(1);
  RooRealVar mwin("mwin","abs(tagmass-1.8645)<%6f",.008,.030);mwin.setUnit("mass window");mwin.setError(.002);mwin.setVal(.03);//the name of this variable cannot be changed because of the way the signicance is defined inside  RooTagCutOptimizer
  RooRealVar probmin("probmin","taglogvtxprob>%6f",-3.,-1.);probmin.setUnit("vtx fit prob.");probmin.setError(.2);probmin.setVal(-3.);
  RooRealVar pidmin("pidmin","tagpid>=%6f",1.,5.);pidmin.setUnit("PID selector");pidmin.setVal(5.);pidmin.setError(1.);pidmin.setVal(1.);

  RooArgList cutvars;
  cutvars.add(pmin);
  cutvars.add(mwin);
  cutvars.add(probmin);
  cutvars.add(pidmin);


  //make plot with initial cuts and sideband definition
  Int_t nbins=220;
  Float_t xmin=1.75;Float_t xmax=1.97;  
  filename=_OutputDir+"/OptimizeCutsPlot.ps";
  Canvas.Print(filename+"[");
  TH1F HMass("HMass","",nbins,xmin,xmax);
  TH1F HMassSig("HMassSig","",nbins,xmin,xmax);HMassSig.SetFillColor(2);
  TH1F HMassSide("HMassSide","",nbins,xmin,xmax);HMassSide.SetFillColor(4);
  ReducedNtuple->Draw("tagmass>>HMass",modecut);
  sigt->Draw("tagmass>>HMassSig","","same");
  bkgt->Draw("tagmass>>HMassSide","","same");
  Canvas.Clear();
  HMass.GetYaxis()->SetRangeUser(0,HMass.GetMaximum());
  HMass.Draw();
  HMassSig.Draw("same");
  HMassSide.Draw("same");
  Canvas.Print(filename);
  
  //the dataset defines the variable (mass) to be excluded from the optimization, it has no other purpose
  mass.setRange(xmin,xmax);
  mass.setBins(nbins);
  TH1F Histo("Histo","",nbins,xmin,xmax);
  RooDataHist dataset("dataset","dataset",RooArgList(mass),&Histo); 
  //the pdf needs the mass variable to go with the dataset, cut variables need to be included here.
  RooArgSet pdfvars(cutvars);
  pdfvars.add(mass);
  RooGenericPdf pdf("pdf","mass+pmin+mwin+probmin+pidmin",pdfvars);
  RooTagCutOptimizer optimizer("optimizer","optimizer",pdf,dataset,sigt,bkgt,&cutvars,0);
  optimizer.SetSigType(1);

  //print the intial significance;
  cout<<"Initial Significance= "<<optimizer.GetSignificance()<<endl;//Note that this may not correspond to the plot above

  
  ///Minuit Optimization: Doesnt really work, is very unstable due its method
  //RooMinuit minuit(optimizer);
  //minuit.simplex();
  //minuit.simplex();//may need to call multiple times
  //minuit.simplex();
  //minuit.migrad();//migrad may not work because it uses derivatives and the surface may not be very smooth
  
  ///--------------------------------------------------
  //Perform a simple multidimensional grid search
  //----------------------------------------------------
  Int_t nvars=0;
  Int_t nsteps[10];
  Int_t ntotsteps=1;
  Int_t t[10];
  Float_t maxfcncut[10];
  TIterator*cutvarIter=cutvars.createIterator();
  while(RooRealVar* var=(RooRealVar*)cutvarIter->Next())
    nsteps[nvars++]=(int)((var->getMax()-var->getMin())/var->getError() +.5) + 1;
  for(Int_t i=0;i<nvars;i++){
    ntotsteps*=nsteps[i];
    t[i]=0;
    maxfcncut[i]=0.;
  }
        
  Float_t fcnmax=0.;
  for(Int_t s=1;s<=ntotsteps;s++){        
    
    Int_t j=0;cutvarIter->Reset();
    while(RooRealVar* var=(RooRealVar*)cutvarIter->Next())
      var->setVal(var->getMin()+t[j++]*var->getError());
    
    Float_t val=optimizer.GetSignificance();
    if(val>fcnmax){
      fcnmax=val; 
      Int_t j=0;cutvarIter->Reset();
      while(RooRealVar* var=(RooRealVar*)cutvarIter->Next())
	maxfcncut[j++]=var->getVal();
    }
    
    //    for(Int_t k=0;k<nvars;k++) cout<<t[k]<<" ";
    //     cout<<" fcn="<<fcnmax<<endl;

    //change the step indexes
    t[nvars-1]++;
    for(Int_t k=nvars-2;k>=0;k--){
      if(t[k+1]==nsteps[k+1])t[k]++;
    }   
    for(Int_t k=0;k<nvars;k++){
      if(t[k]==nsteps[k])t[k]=0;
    }         
    
  }
  Int_t j=0;cutvarIter->Reset();
  while(RooRealVar* var=(RooRealVar*)cutvarIter->Next()){
    var->setVal(maxfcncut[j++]);
    cout<<var->GetName()<<"= "<<var->getVal()<<endl;    
  }

  
  //compare plot with final cuts:
  TCut finalcut=modecut;
  char textl[100];cutvarIter->Reset();
  while(RooRealVar* var=(RooRealVar*)cutvarIter->Next()){
    sprintf(textl,var->GetTitle(),var->getVal());
    if(TString(var->GetName())!=TString("mwin"))finalcut+=TCut(textl); 
  }  
  Canvas.Clear();
  HMass.Draw();
  ReducedNtuple->Draw("tagmass",finalcut,"same");
  cutline.DrawLine(D0PDGMass-mwin.getVal(),0,D0PDGMass-mwin.getVal(),HMass.GetMaximum());
  cutline.DrawLine(D0PDGMass+mwin.getVal(),0,D0PDGMass+mwin.getVal(),HMass.GetMaximum());
  Canvas.Print(filename);

  
  //print the final significance;
  cout<<"Final Significance= "<<optimizer.GetSignificance()<<endl;



  //plot a scan 
  TGraph*pScan=optimizer.ScanVariable(&pmin,20);
  Canvas.Clear();
  pScan->Draw("ap");
  Canvas.Print(filename);

  TGraph*mScan=optimizer.ScanVariable(&mwin,20);
  Canvas.Clear();
  mScan->Draw("ap");
  Canvas.Print(filename);

  TGraph*probScan=optimizer.ScanVariable(&probmin,20);
  Canvas.Clear();
  probScan->Draw("ap");
  Canvas.Print(filename);


  TGraph*pidScan=optimizer.ScanVariable(&pidmin,20);
  Canvas.Clear();
  pidScan->Draw("ap");
  Canvas.Print(filename);

  Canvas.Print(filename+"]");

  delete sigt;delete bkgt;

  return 1;
  
}
