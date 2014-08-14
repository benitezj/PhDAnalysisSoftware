#define NDalitzPstarBins 11

void Dst2640Analysis::DrawDalitzPlots(TCanvas* Canvas,TString filename){
  cout<<"Drawing Dalitz plots"<<endl;

  if(ReducedNtuple==NULL){
    cout<<"Ntuple is NULL"<<endl;
    return;
  }
  if(!(H2MCDst2640DalitzPlot=(TH2F*)OutPutFile->Get("H2MCDst2640DalitzPlot"))){
    cout<<"No saved histogramH2MCDst2640DalitzPlot "<<endl;return;}
  H2MCDst2640DalitzPlot->SetDirectory(0);
  

  TH2F* H2Dalitz[NDalitzPstarBins];
  
  Float_t pstarmin=2.4;
  Float_t pstarmax=4.6;
  Float_t deltapstar=(pstarmax-pstarmin)/NDalitzPstarBins;
 
  TCut pstarcut;
  TString plotname;
  TString plottitle;

  Canvas->Clear(); 
  H2MCDst2640DalitzPlot->Draw("colz");
  Canvas->Print(filename);
  
  Canvas->Clear(); 
  plotname=TString("H2DalitzAllPstar");
  plottitle=plotname;
  TH2F H2DalitzAllpstar(plotname,plottitle,NDalitzPlotBins,DalitzMin,DalitzMax,NDalitzPlotBins,DalitzMin,DalitzMax);
  H2DalitzAllpstar.GetXaxis()->SetTitle(H2MCDst2640DalitzPlot->GetXaxis()->GetTitle());
  H2DalitzAllpstar.GetYaxis()->SetTitle(H2MCDst2640DalitzPlot->GetYaxis()->GetTitle());
  ReducedNtuple->Draw(TString("msqdstarpiplus:msqdstarpiminus>>")+plotname,database.GetCut(1,100),"colz");
  Canvas->Print(filename);


  Canvas->Clear(); 
  Canvas->Divide(3,4);
  

  for(Int_t pbin=0;pbin<NDalitzPstarBins;pbin++){      
    Canvas->cd(pbin+1);
    
    sprintf(textlabel,"%.5f<dst2640pstar&&dst2640pstar<%.5f",pstarmin+pbin*deltapstar,pstarmin+(pbin+1)*deltapstar);    
    pstarcut=textlabel;
    
    plotname=TString("H2Dalitz")+(long)pbin;
    
    sprintf(textlabel,"p* range = [%.1f,%.1f]",pstarmin+pbin*deltapstar,pstarmin+(pbin+1)*deltapstar);
    plottitle=textlabel;

    H2Dalitz[pbin]=new TH2F(plotname,plottitle,NDalitzPlotBins/4,DalitzMin,DalitzMax,NDalitzPlotBins/4,DalitzMin,DalitzMax);
    //    H2Dalitz[pbin]->SetTitleSize(.16);
    if(_Mode=="D0ToKPi")H2Dalitz[pbin]->GetZaxis()->SetRangeUser(0,400);
    if(_Mode=="D0ToK3Pi")H2Dalitz[pbin]->GetZaxis()->SetRangeUser(0,350);
    H2Dalitz[pbin]->GetZaxis()->SetNdivisions(4);
    H2Dalitz[pbin]->GetZaxis()->SetLabelSize(.08);
    H2Dalitz[pbin]->GetYaxis()->SetNdivisions(4);
    H2Dalitz[pbin]->GetYaxis()->SetLabelSize(.08);
    H2Dalitz[pbin]->GetXaxis()->SetNdivisions(4);
    H2Dalitz[pbin]->GetXaxis()->SetLabelSize(.08);
    H2Dalitz[pbin]->GetXaxis()->SetTitle(H2MCDst2640DalitzPlot->GetXaxis()->GetTitle());
    H2Dalitz[pbin]->GetYaxis()->SetTitle(H2MCDst2640DalitzPlot->GetYaxis()->GetTitle());
    

    ReducedNtuple->Draw(TString("msqdstarpiplus:msqdstarpiminus>>")+plotname,database.GetCut(1,100)+pstarcut,"colz");

  }
  
  Canvas->Print(filename);
  
  for(Int_t pbin=0;pbin<NDalitzPstarBins;pbin++)
    delete H2Dalitz[pbin];


}

