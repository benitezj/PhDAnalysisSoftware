
#include "SOFTWARE/Globals.h"

#define NnXTBins 7

//TString filename("DsTolnu/SP1005Sig/DsReco/Final/Ntuple.root");
TString filename("DsTolnu/SP1005Skim/DsReco/Final/Ntuple.root");

void MakenXRVsnXTNew(){
  
  TFile file(filename,"read");
  gROOT->cd();
  TTree*Ntuple=(TTree*)file.Get("Ntuple");
  cout<<Ntuple<<" "<<Ntuple->GetEntries()<<endl;

  TFile outfile("nXRVsnXT.root","recreate");
  

  TH1F HTotGen("HTotGen","",NnXTBins,-.5,NnXTBins-.5);
  HTotGen.GetXaxis()->SetTitle("n_{X}");
  Ntuple->Draw("fragnpartgen>>HTotGen","evthassignal&&1.82<dsmass&&dsmass<2.12"); 
  HTotGen.Write();
  TH1F HTotReco("HTotReco","",NnXTBins,-.5,NnXTBins-.5);
  HTotReco.GetXaxis()->SetTitle("n_{X}");
  Ntuple->Draw("fragndaus>>HTotReco","evthassignal&&1.82<dsmass&&dsmass<2.12");
  HTotReco.Write();


  TH1F* H[NnXTBins];
  for(Int_t i=0;i<NnXTBins;i++){
    H[i]=new TH1F(TString("H")+(long)i,"",NnXTBins,-.5,NnXTBins-.5);
    Int_t color=i+2;
    if(color>=8)color+=32; 
    H[i]->SetFillColor(color);
    H[i]->SetLineColor(0);
    H[i]->SetStats(0);
  }
  for(Int_t i=0;i<NnXTBins;i++){
    Ntuple->Draw(TString("fragnpartgen>>H")+(long)i,TString("evthassignal&&1.82<dsmass&&dsmass<2.12&&fragnpartgen==")+(long)i);
    H[i]->Write();
  }


 
  TH1F* hr[NnXTBins][NnXTBins];//i=gen,j=reco
  for(Int_t i=0;i<NnXTBins;i++)
    for(Int_t j=0;j<NnXTBins;j++){
      hr[i][j]=new TH1F(TString("hr")+(long)i+"_"+(long)j,"",NnXTBins,-.5,NnXTBins-.5);
      Int_t color=i+2;
      if(color>=8)color+=32;//skip 10 which is white
      hr[i][j]->SetFillColor(color);
      hr[i][j]->SetStats(0);
      hr[i][j]->SetLineColor(0);
    }
  for(Int_t i=0;i<NnXTBins;i++)
    for(Int_t j=0;j<NnXTBins;j++){
      Ntuple->Draw(TString("fragndaus>>hr")+(long)i+"_"+(long)j,TString("evthassignal&&1.82<dsmass&&dsmass<2.12&&fragnpartgen==")+(long)i+"&&fragndaus=="+(long)j);
      hr[i][j]->Write();
    }
  
  outfile.ls();
  outfile.Close();
}


void PlotnXRVsNXTNew(){
  

  TFile outfile("nXRVsnXT.root","read");
  gROOT->cd();


  TCanvas C("C");
  TString filename="BellenXRCrossFeed.ps";
  C.Print(filename+"[");

  TH1F* HTotGen=(TH1F*)outfile.Get("HTotGen");
  HTotGen->GetXaxis()->SetTitle("n_{X}");
  TH1F* HTotReco=(TH1F*)outfile.Get("HTotReco");
  HTotReco->GetXaxis()->SetTitle("n_{X}");

  HTotGen->GetYaxis()->SetRangeUser(0,HTotGen->GetMaximum()*1.6);
  C.Clear();
  HTotGen->Draw();
  HTotReco->Draw("pesame");
  C.Print(filename);



  TH1F* H[NnXTBins];
  for(Int_t i=0;i<NnXTBins;i++){
    H[i]=(TH1F*)outfile.Get(TString("H")+(long)i);
    Int_t color=i+2;
    if(color>=8)color+=32; 
    H[i]->SetFillColor(color);
    H[i]->SetLineColor(0);
    H[i]->SetStats(0);
  }

 
  TH1F* hr[NnXTBins][NnXTBins];//i=gen,j=reco
  for(Int_t i=0;i<NnXTBins;i++)
    for(Int_t j=0;j<NnXTBins;j++){
      hr[i][j]=(TH1F*)outfile.Get(TString("hr")+(long)i+"_"+(long)j);
      Int_t color=i+2;
      if(color>=8)color+=32;//skip 10 which is white
      hr[i][j]->SetFillColor(color);
      hr[i][j]->SetStats(0);
      hr[i][j]->SetLineColor(0);
    }

  //for each nXT add all nXT which have nXT below this one
  TH1* hrorderedAdded[NnXTBins][NnXTBins];
  for(Int_t i=0;i<NnXTBins;i++)
    for(Int_t j=0;j<NnXTBins;j++)
      hrorderedAdded[i][j]=(TH1*)hr[i][j]->Clone();
  for(Int_t j=0;j<NnXTBins;j++)
    for(Int_t i=0;i<NnXTBins;i++)
      for(Int_t k=0;k<NnXTBins;k++)
	if(k<i)hrorderedAdded[i][j]->Add(hr[k][j]);



  ///--------------------------------------------------
  //Make Belle style plot
  C.Clear();
  C.Divide(1,2);
  C.cd(1);
  HTotGen->Draw();
  for(Int_t i=0;i<NnXTBins;i++)
    H[i]->Draw("same");
  HTotGen->Draw("same");

  C.cd(2);
  HTotGen->Draw(); 
  for(Int_t j=0;j<NnXTBins;j++)
    for(Int_t i=NnXTBins-1;i>=0;i--)
      hrorderedAdded[i][j]->Draw("same");
  HTotGen->Draw("same");

  C.Print(filename);
  ///--------------------------------------------------



  C.Print(filename+"]");
}



//////////////
void PlotnXRVsNXT_Resonances(){
  
  TFile file(filename,"read");
  gROOT->cd();
  TTree*Ntuple=(TTree*)file.Get("Ntuple");
  cout<<Ntuple<<" "<<Ntuple->GetEntries()<<endl;

  TCanvas C("C");
  TString filename="BellenXRCrossFeed_Resonances.ps";
  C.Print(filename+"[");

 
  TH1F HTotGen("HTotGen","",NnXTBins,-.5,NnXTBins-.5);
  HTotGen.GetXaxis()->SetTitle("n_{X}");
  Ntuple->Draw("evtnXTgen>>HTotGen","evthassignal"); 
  TH1F HTotReco("HTotReco","",NnXTBins,-.5,NnXTBins-.5);
  HTotReco.GetXaxis()->SetTitle("n_{X}");
  Ntuple->Draw("fragndaus>>HTotReco","evthassignal");

  HTotGen.GetYaxis()->SetRangeUser(0,HTotGen.GetMaximum()*1.6);
  C.Clear();
  HTotGen.Draw();
  HTotReco.Draw("pesame");
  C.Print(filename);



  TH1F* H[NnXTBins];
  for(Int_t i=0;i<NnXTBins;i++){
    H[i]=new TH1F(TString("H")+(long)i,"",NnXTBins,-.5,NnXTBins-.5);
    Int_t color=i-1;
    if(color>=8)color+=32;//skip  white
    H[i]->SetFillColor(color); 
    H[i]->SetLineColor(0);
    H[i]->SetStats(0);
  }
  for(Int_t i=0;i<NnXTBins;i++)
    Ntuple->Draw(TString("evtnXTgen>>H")+(long)i,TString("evthassignal&&evtnXTgen==")+(long)i);


 
  TH1F* hr[NnXTBins][NnXTBins];//i=gen,j=reco
  for(Int_t i=0;i<NnXTBins;i++)
    for(Int_t j=0;j<NnXTBins;j++){
      hr[i][j]=new TH1F(TString("hr")+(long)i+"_"+(long)j,"",NnXTBins,-.5,NnXTBins-.5);
      Int_t color=i-1;
      if(color>=8)color+=32; 
      hr[i][j]->SetFillColor(color);
      hr[i][j]->SetStats(0);
      hr[i][j]->SetLineColor(0);
    }
  for(Int_t i=0;i<NnXTBins;i++)
    for(Int_t j=0;j<NnXTBins;j++)
      Ntuple->Draw(TString("fragndaus>>hr")+(long)i+"_"+(long)j,TString("evthassignal&&evtnXTgen==")+(long)i+"&&fragndaus=="+(long)j);

  //print out unordered numbers
  for(Int_t j=0;j<NnXTBins;j++){
    cout<<"  "<<j<<": ";
    for(Int_t i=0;i<NnXTBins;i++)
      cout<<" "<<hr[i][j]->Integral();
    cout<<"  "<<endl;
  }
  cout<<"  "<<endl;

  //for each nXR order nXT plots from smallest bin content to largest
  TH1F* hrordered[NnXTBins][NnXTBins];
  for(Int_t j=0;j<NnXTBins;j++)
    for(Int_t i=0;i<NnXTBins;i++){
      
      hrordered[i][j]=hr[i][j];//initialize
      
      for(Int_t k=0;k<NnXTBins;k++)//init to biggest (necessary for i!=0
	if(hr[k][j]->Integral()>=hrordered[i][j]->Integral())
	  hrordered[i][j]=hr[k][j];

      
      if(i==0){//for first plot just find smallest
	for(Int_t k=0;k<NnXTBins;k++)
	  if(hr[k][j]->Integral()<=hrordered[i][j]->Integral())
	    hrordered[i][j]=hr[k][j];
      }else {
	for(Int_t k=0;k<NnXTBins;k++)//find smallest one which is bigger than previous
	  if(hr[k][j]->Integral()<hrordered[i][j]->Integral()
	     &&hr[k][j]->Integral()>=hrordered[i-1][j]->Integral()
	     ){
	    Bool_t alreadyhere=0;//cannot be one of the ones already sorted
	    for(Int_t l=0;l<i;l++)
	      if(hr[k][j]==hrordered[l][j])alreadyhere=1;
	    if(!alreadyhere)
	      hrordered[i][j]=hr[k][j];
	  }
      }
      
    }

  //print out ordered numbers
  for(Int_t j=0;j<NnXTBins;j++){
    cout<<"  "<<j<<": ";
    for(Int_t i=0;i<NnXTBins;i++)
      cout<<" "<<hrordered[i][j]->Integral();
    cout<<"  "<<endl;
  }
  cout<<"  "<<endl;

  //now add all above this one
  TH1* hrorderedAdded[NnXTBins][NnXTBins];
  for(Int_t i=0;i<NnXTBins;i++)
    for(Int_t j=0;j<NnXTBins;j++)
      hrorderedAdded[i][j]=(TH1*)hrordered[i][j]->Clone();
  for(Int_t j=0;j<NnXTBins;j++)
    for(Int_t i=0;i<NnXTBins;i++)
      for(Int_t k=NnXTBins-1;k>i;k--)
	hrorderedAdded[i][j]->Add(hrordered[k][j]);
  
  //print out ordered numbers
  for(Int_t j=0;j<NnXTBins;j++){
    cout<<"  "<<j<<": ";
    for(Int_t i=0;i<NnXTBins;i++)
      cout<<" "<<hrorderedAdded[i][j]->Integral();
    cout<<"  "<<endl;
  }
  cout<<"  "<<endl;


  //crosscheck by ploting pieces on total reco 
  C.Clear();
  HTotReco.Draw();
  for(Int_t j=0;j<NnXTBins;j++)
    for(Int_t i=0;i<NnXTBins;i++)//smallest goes first becuase is now biggest      
      hrorderedAdded[i][j]->Draw("same");
  HTotReco.Draw("same");
  C.Print(filename);

  //Make Belle style plot
  C.Clear();
  C.Divide(1,2);
  C.cd(1);
  HTotGen.Draw();
  for(Int_t i=0;i<NnXTBins;i++)
    H[i]->Draw("same");
  HTotGen.Draw("same");

  C.cd(2);
  HTotGen.Draw(); 
  for(Int_t j=0;j<NnXTBins;j++)
    for(Int_t i=0;i<NnXTBins;i++)//smallest goes first becuase is now biggest      
      hrorderedAdded[i][j]->Draw("same");
  HTotGen.Draw("same");

  C.Print(filename);

  C.Print(filename+"]");
}



void PlotnXRVsNXT(){
  
  TFile file(filename,"read");
  gROOT->cd();
  TTree*Ntuple=(TTree*)file.Get("Ntuple");
  cout<<Ntuple<<" "<<Ntuple->GetEntries()<<endl;

  TCanvas C("C");
  TString filename="BellenXRCrossFeed.ps";
  C.Print(filename+"[");

  TH1F HTotGen("HTotGen","",NnXTBins,-.5,NnXTBins-.5);
  HTotGen.GetXaxis()->SetTitle("n_{X}");
  Ntuple->Draw("fragnpartgen>>HTotGen","evthassignal"); 
  TH1F HTotReco("HTotReco","",NnXTBins,-.5,NnXTBins-.5);
  HTotReco.GetXaxis()->SetTitle("n_{X}");
  Ntuple->Draw("fragndaus>>HTotReco","evthassignal");

  HTotGen.GetYaxis()->SetRangeUser(0,HTotGen.GetMaximum()*1.6);
  C.Clear();
  HTotGen.Draw();
  HTotReco.Draw("pesame");
  C.Print(filename);



  TH1F* H[NnXTBins];
  for(Int_t i=0;i<NnXTBins;i++){
    H[i]=new TH1F(TString("H")+(long)i,"",NnXTBins,-.5,NnXTBins-.5);
    Int_t color=i-1;
    if(color>=8)color+=32; 
    H[i]->SetFillColor(color);
    H[i]->SetLineColor(0);
    H[i]->SetStats(0);
  }
  for(Int_t i=0;i<NnXTBins;i++)
    Ntuple->Draw(TString("fragnpartgen>>H")+(long)i,TString("evthassignal&&fragnpartgen==")+(long)i);


 
  TH1F* hr[NnXTBins][NnXTBins];//i=gen,j=reco
  for(Int_t i=0;i<NnXTBins;i++)
    for(Int_t j=0;j<NnXTBins;j++){
      hr[i][j]=new TH1F(TString("hr")+(long)i+"_"+(long)j,"",NnXTBins,-.5,NnXTBins-.5);
      Int_t color=i-1;
      if(color>=8)color+=32;//skip 10 which is white
      hr[i][j]->SetFillColor(color);
      hr[i][j]->SetStats(0);
      hr[i][j]->SetLineColor(0);
    }
  for(Int_t i=0;i<NnXTBins;i++)
    for(Int_t j=0;j<NnXTBins;j++)
      Ntuple->Draw(TString("fragndaus>>hr")+(long)i+"_"+(long)j,TString("evthassignal&&fragnpartgen==")+(long)i+"&&fragndaus=="+(long)j);

  //print out unordered numbers
  for(Int_t j=0;j<NnXTBins;j++){
    cout<<"  "<<j<<": ";
    for(Int_t i=0;i<NnXTBins;i++)
      cout<<" "<<hr[i][j]->Integral();
    cout<<"  "<<endl;
  }
  cout<<"  "<<endl;

  
  //for each nXR order nXT plots from smallest bin content to largest
  TH1F* hrordered[NnXTBins][NnXTBins];
  for(Int_t j=0;j<NnXTBins;j++)
    for(Int_t i=0;i<NnXTBins;i++){
      
      hrordered[i][j]=hr[i][j];//initialize
      
      for(Int_t k=0;k<NnXTBins;k++)//init to biggest (necessary for i!=0
	if(hr[k][j]->Integral()>=hrordered[i][j]->Integral())
	  hrordered[i][j]=hr[k][j];

      
      if(i==0){//for first plot just find smallest
	for(Int_t k=0;k<NnXTBins;k++)
	  if(hr[k][j]->Integral()<=hrordered[i][j]->Integral())
	    hrordered[i][j]=hr[k][j];
      }else {
	for(Int_t k=0;k<NnXTBins;k++)//find smallest one which is bigger than previous
	  if(hr[k][j]->Integral()<hrordered[i][j]->Integral()
	     &&hr[k][j]->Integral()>=hrordered[i-1][j]->Integral()
	     ){
	    Bool_t alreadyhere=0;//cannot be one of the ones already sorted
	    for(Int_t l=0;l<i;l++)
	      if(hr[k][j]==hrordered[l][j])alreadyhere=1;
	    if(!alreadyhere)
	      hrordered[i][j]=hr[k][j];
	  }
      }
      
    }

  
  
  //print out ordered numbers
  for(Int_t j=0;j<NnXTBins;j++){
    cout<<"  "<<j<<": ";
    for(Int_t i=0;i<NnXTBins;i++)
      cout<<" "<<hrordered[i][j]->Integral();
    cout<<"  "<<endl;
  }
  cout<<"  "<<endl;

  //now add all above this one
  TH1* hrorderedAdded[NnXTBins][NnXTBins];
  for(Int_t i=0;i<NnXTBins;i++)
    for(Int_t j=0;j<NnXTBins;j++)
      hrorderedAdded[i][j]=(TH1*)hrordered[i][j]->Clone();
  for(Int_t j=0;j<NnXTBins;j++)
    for(Int_t i=0;i<NnXTBins;i++)
      for(Int_t k=NnXTBins-1;k>i;k--)
	hrorderedAdded[i][j]->Add(hrordered[k][j]);
  
  //print out ordered numbers
  for(Int_t j=0;j<NnXTBins;j++){
    cout<<"  "<<j<<": ";
    for(Int_t i=0;i<NnXTBins;i++)
      cout<<" "<<hrorderedAdded[i][j]->Integral();
    cout<<"  "<<endl;
  }
  cout<<"  "<<endl;


  //crosscheck by ploting pieces on total reco 
  C.Clear();
  HTotReco.Draw();
  for(Int_t j=0;j<NnXTBins;j++)
    for(Int_t i=0;i<NnXTBins;i++)//smallest goes first becuase is now biggest      
      hrorderedAdded[i][j]->Draw("same");
  HTotReco.Draw("same");
  C.Print(filename);

  //Make Belle style plot
  C.Clear();
  C.Divide(1,2);
  C.cd(1);
  HTotGen.Draw();
  for(Int_t i=0;i<NnXTBins;i++)
    H[i]->Draw("same");
  HTotGen.Draw("same");

  C.cd(2);
  HTotGen.Draw(); 
  for(Int_t j=0;j<NnXTBins;j++)
    for(Int_t i=0;i<NnXTBins;i++)//smallest goes first because is now biggest      
      hrorderedAdded[i][j]->Draw("same");
  HTotGen.Draw("same");

  C.Print(filename);

  C.Print(filename+"]");
}




