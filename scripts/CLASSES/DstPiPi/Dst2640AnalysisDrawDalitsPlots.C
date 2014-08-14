#define NDalitzPstarBins 11

void Dst2640Analysis::DrawDalitzPlots(TCanvas* Canvas,TString filename){
  cout<<"Drawing Dalitz plots"<<endl;

  if(ReducedNtuple==NULL){
    cout<<"Ntuple is NULL"<<endl;
    return;
  }
  TH2F* H2Dalitz[NDalitzPstarBins];
  
  Float_t pstarmin=2.4;
  Float_t pstarmax=2.6;
  Float_t deltapstar=(pstarmax-pstarmin)/NDalitPstarBins;
 
  TCut pstarcut;
  TString plotname;

  Canvas->Clear(); 
  Canvas->Divide(3,4);
  

  for(Int_t pbin=0;pbin<NDalitzPstarBins;pbin++){      
    Canvas->cd(pbin+1);
    
    sprintf(textlabel,"%.5f<dst2640pstar&&dst2640pstar<%.5f",pstarmin+pbin*deltapstar,pstarmin+(pbin+1)*deltapstar);    
    pstarcut=textlabel;
    
    plotname=TString("H2Dalitz")+(long)pbin;
    
    H2Dalitz[pbin]=new TH2F(plotname,plotname,20,4.5,6.5,20,4.5,6.5);
    
    
    ReducedNtuple->Draw(TString("msqdstarpiplus:msqdstarpiminus>>")+plotname,database.GetCut(1,100)+pstarcut,"colz");

  }
  
  Canvas->Print(filename);
  

}

