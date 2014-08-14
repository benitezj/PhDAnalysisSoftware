#define NMassPstarBins 12

Int_t  Dst2640Analysis::DrawMassPlots(TCanvas* Canvas,TString filename,Int_t WhichMassRange,Int_t SubtractBkg){
  cout<<"Drawing Mass plots"<<endl;
  cout<<"As of 9/29/2007 this action is obsolute. There are better ways to plot the mass spectrum."<<endl;
  return 1;


  if(ReducedNtuple==NULL){
    cout<<"Ntuple is NULL"<<endl;
    return 0;
  }

  if(!OpenBkgNtuple()){
    cout<<"Failed to open the Bkg file"<<endl;
    return 0;
  }  
  

  ///
  sprintf(textlabel,"%.5f<dst2640pstar&&dst2640pstar<%.5f",3.8,10.);    
  DstPiPipstarcut=textlabel;


  TH1F* HMass[NMassPstarBins];
  
  Float_t pstarmin=2.4;
  Float_t pstarmax=4.8;
  Float_t deltapstar=(pstarmax-pstarmin)/NMassPstarBins;
 

  TString plotname;
  TString plottitle;
  

  Float_t massrangemin=0;
  Float_t massrangemax=0;
  Int_t nmassrangebins=0;
  if(WhichMassRange==1){
    massrangemin=2.6;
    massrangemax=2.7;
    nmassrangebins=20;
  }
  if(WhichMassRange==0){
    massrangemin=database.GetDst2640DeltaMMin()+DstarPDGMass;
    massrangemax=database.GetDst2640DeltaMMax()+DstarPDGMass;
    nmassrangebins=database.GetDst2640DeltaMNbins();
  }
  
    


  TH1F Hccbarbkg("Hccbarbkg","ccbar background",nmassrangebins,massrangemin,massrangemax);
  if(SubtractBkg==1){//get the background plot   
    ccbartree->Draw("dst2640deltam+2.010>>Hccbarbkg",database.GetCut(1,100)+DstPiPipstarcut);
    Hccbarbkg.Scale(scalefactorccbar);
    cout<<"Bkg Integral= "<<Hccbarbkg.Integral()<<endl;
  }
  TH1F* HMassccbar[NMassPstarBins];
  TH1F HAllBkgWeighted("HAllBkgWeighted","HAllBkgWeighted",nmassrangebins,massrangemin,massrangemax);


  
  Canvas->Clear();//Draw the data with a high p*(D*pipi)  
  plotname="HMassAllpstar";
  plottitle="Allpstar";
  TH1F HMassAllpstar(plotname,plottitle,nmassrangebins,massrangemin,massrangemax);
  HMassAllpstar.GetYaxis()->SetTitle(DstarPiPiYLabel);
  HMassAllpstar.GetXaxis()->SetTitle(DstarPiPiXLabel);
  HMassAllpstar.SetStats(0);
  HMassAllpstar.SetMarkerSize(.8);
  HMassAllpstar.SetLineWidth(3);
  ReducedNtuple->Draw(TString("dst2640deltam+2.010>>")+plotname,database.GetCut(1,100)+DstPiPipstarcut,"");
  HMassAllpstar.GetYaxis()->SetRangeUser(0,HMassAllpstar.GetMaximum()*1.05);

  HMassAllpstar.Draw("pe");
  cutline.DrawLine(database.GetDst2640DeltaMSignalMin()+DstarPDGMass,0,
		   database.GetDst2640DeltaMSignalMin()+DstarPDGMass,HMassAllpstar.GetMaximum());
  cutline.DrawLine(database.GetDst2640DeltaMSignalMax()+DstarPDGMass,0,
		   database.GetDst2640DeltaMSignalMax()+DstarPDGMass,HMassAllpstar.GetMaximum());
  if(SubtractBkg==1){
    Hccbarbkg.Draw("same");
  }
  Canvas->Print(filename);



  

  Canvas->Clear(); 
  Canvas->Divide(3,4);


  Float_t binpstarmin=0;
  Float_t binpstarmax=0;
  TCut binpstarcut;

  for(Int_t pbin=0;pbin<NMassPstarBins;pbin++){      
    Canvas->cd(pbin+1);
    binpstarmin=pstarmin+pbin*deltapstar;
    binpstarmax=pstarmin+(pbin+1)*deltapstar;

    sprintf(textlabel,"%.5f<dst2640pstar&&dst2640pstar<%.5f",binpstarmin,binpstarmax);    
    binpstarcut=textlabel;
    
    plotname=TString("HMass")+(long)pbin;    
    sprintf(textlabel,"p* range = [%.1f,%.1f]",binpstarmin,binpstarmax);
    plottitle=textlabel;

    HMass[pbin]=new TH1F(plotname,plottitle,nmassrangebins,massrangemin,massrangemax);

    HMass[pbin]->GetYaxis()->SetNdivisions(5);
    HMass[pbin]->GetYaxis()->SetLabelSize(.08);
    HMass[pbin]->GetXaxis()->SetNdivisions(5);
    HMass[pbin]->GetXaxis()->SetLabelSize(.08);
    HMass[pbin]->SetMarkerSize(.8);
    HMass[pbin]->SetLineWidth(3);
    HMass[pbin]->SetStats(0);
    HMass[pbin]->GetYaxis()->SetTitle(DstarPiPiYLabel);
    HMass[pbin]->GetXaxis()->SetTitle(DstarPiPiXLabel);//"m(D*#pi#pi) - m(D*) + m_{D*}  (GeV/c^{2})");

    
    if(SubtractBkg==1){
      HMassccbar[pbin]=new TH1F(plotname+"ccbar",plottitle,nmassrangebins,massrangemin,massrangemax);
      ccbartree->Draw("dst2640deltam+2.010>>"+plotname+"ccbar",database.GetCut(1,100)+binpstarcut);
      HMassccbar[pbin]->Scale(scalefactorccbar);
    }

    ReducedNtuple->Draw(TString("dst2640deltam+2.010>>")+plotname,database.GetCut(1,100)+binpstarcut,"pe");
    HMass[pbin]->GetYaxis()->SetRangeUser(HMass[pbin]->GetMinimum(),HMass[pbin]->GetMaximum()*1.05);
    HMass[pbin]->Draw("pe");
    cutline.DrawLine(database.GetDst2640DeltaMSignalMin()+DstarPDGMass,HMass[pbin]->GetMinimum(),
		     database.GetDst2640DeltaMSignalMin()+DstarPDGMass,HMass[pbin]->GetMaximum());
    cutline.DrawLine(database.GetDst2640DeltaMSignalMax()+DstarPDGMass,HMass[pbin]->GetMinimum(),
		     database.GetDst2640DeltaMSignalMax()+DstarPDGMass,HMass[pbin]->GetMaximum());

    if(SubtractBkg==1){
      HMassccbar[pbin]->Draw("same");
      cout<<pbin<<" "<<HMassccbar[pbin]->Integral()<<" "<<HMass[pbin]->Integral()<<endl;
    }
    
  } 
  Canvas->Print(filename);
  
  if(SubtractBkg==1){
    //now add all the backgrounds p* weighted. 
    for(Int_t pbin=7;pbin<NMassPstarBins;pbin++){//only add the bins that fall in the p*(D*pipi) cut
      HMassccbar[pbin]->Scale(HMass[pbin]->Integral()/HMassccbar[pbin]->Integral()); 
      HAllBkgWeighted.Add(HMassccbar[pbin]);
      cout<<pbin<<" "<<HMassccbar[pbin]->Integral()<<" "<<HMass[pbin]->Integral()<<endl;
    }
    
    Canvas->Clear(); 
    HMassAllpstar.Draw("pe");
    HAllBkgWeighted.Draw("same");
    Canvas->Print(filename);
  }


  for(Int_t pbin=0;pbin<NMassPstarBins;pbin++){
    delete HMass[pbin];
    if(SubtractBkg==1)
      delete HMassccbar[pbin];      
  }


  
  if(!CloseBkgNtuple()){
    cout<<"Failed to close the Bkg file"<<endl;
    return 0;
  }

  return 1;
}

