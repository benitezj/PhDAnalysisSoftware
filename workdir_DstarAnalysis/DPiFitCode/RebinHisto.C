#include "RebinHisto.h"

TH1* RebinHistoX(TH1* oldhisto,Int_t nbins,Float_t min,Float_t max){

  if(!oldhisto){cout<<" no histo"<<endl; return 0;}


  Float_t newbinsize=(max-min)/nbins;

  Float_t oldxmin=oldhisto->GetXaxis()->GetXmin();
  Float_t oldxmax=oldhisto->GetXaxis()->GetXmax();
  Int_t oldnbins=oldhisto->GetXaxis()->GetNbins();
  Float_t oldbinwidth=(oldxmax-oldxmin)/oldnbins;
  cout<<"Histo Info: "<<oldnbins<<" "<<oldxmin<<" "<<oldxmax<<" "<<oldbinwidth<<endl;
  cout<<"New Binning: "<<nbins<<" "<<min<<" "<<max<<" "<<newbinsize<<endl;
 
  //same binning
  if(oldxmin==min &&
     oldxmax==max &&
     oldnbins==nbins){return (TH1*)oldhisto->Clone();}

  //check ranges are within limits
  if(min>max){
    cout<<"min>max not good"<<endl;
    return NULL;
  }
  if((min+1e-6)<oldxmin){
    cout<<"min<histomin not good"<<endl;
    return NULL;
  }    
  if((max-1e-6)>oldxmax){
    cout<<"max>histomax not good"<<endl;
    return NULL;
  }  

  //check that newbinsize is in multiples of oldbinsize
  if( floor(newbinsize/oldbinwidth-.0001)!=floor(newbinsize/oldbinwidth+.0001)-1){
    cout<<"newbinsize is not good"<<endl;
    return NULL;
  }
  
  Int_t ntmpbins=(int)((max-min)/oldbinwidth + .5);		        
  
  //first create a new histo with new range but same bin size
  TH1* newhisto=new TH1F(TString(oldhisto->GetName())+"rebin",TString(oldhisto->GetTitle()),ntmpbins,min,max);//need retain title 
    
  Int_t bn=1;
  for(Int_t b=1;b<=oldnbins;b++){
    if(fabs(oldhisto->GetXaxis()->GetBinCenter(b)-newhisto->GetXaxis()->GetBinCenter(bn))<oldbinwidth/100 && bn<=ntmpbins){
      newhisto->SetBinContent(bn,oldhisto->GetBinContent(b));
      if(oldhisto->GetBinError(b)<1)newhisto->SetBinError(bn,1);
      else newhisto->SetBinError(bn,oldhisto->GetBinError(b));
      bn++;
    }    
  }
  if((bn-1)!=ntmpbins){
    cout<<"There was a problem in rebinning, bin numbers dont match.  "<<bn-1<<"!="<<ntmpbins<<endl;
    cout<<"bin centers may be out of sync."<<endl;
    return NULL;
  }
  
  newhisto->Sumw2(); 
  Int_t group=ntmpbins/nbins;   
  //now group bins
  TH1* rebinhisto=(TH1*)newhisto->Rebin(group,TString(oldhisto->GetName())+"rebinned"); 
  
  ///copy axes labels
  rebinhisto->SetTitle(oldhisto->GetTitle());
  //rebinhisto->GetYaxis()->SetTitle(oldhisto->GetYaxis()->GetTitle());//different binning
  rebinhisto->GetXaxis()->SetTitle(oldhisto->GetXaxis()->GetTitle());
  
  delete newhisto;
  return rebinhisto;
}


TH2F* RebinHistoXY(TH2* oldhisto,Int_t nbins,Float_t min,Float_t max,Int_t prevBinning){
  if(!oldhisto)return NULL;
  cout<<"Creating rebinned histo of "<<oldhisto->GetName()<<endl;
  
  Float_t newbinsize=(max-min)/nbins;
 
  Float_t oldxmin=oldhisto->GetXaxis()->GetXmin();
  Float_t oldxmax=oldhisto->GetXaxis()->GetXmax();
  Int_t oldnbins=oldhisto->GetXaxis()->GetNbins();
  Float_t oldbinwidth=(oldxmax-oldxmin)/oldnbins;

  if( floor(1000*newbinsize+.001)!=floor(1000*newbinsize+.999)){
    cout<<"newbinsize is not good"<<endl;
    return NULL;
  }
  if( floor(1000*min+.001)!=floor(1000*min+.999)){
    cout<<"min is not good"<<endl;
    return NULL;
  }
  if( floor(1000*max+.001)!=floor(1000*max+.999)){
    cout<<"max is not good"<<endl;
    return NULL;
  }  
  if(min>max){
    cout<<"min>max not good"<<endl;
    return NULL;
  }
  if(min<oldxmin){
    cout<<"min<histomin not good"<<endl;
    return NULL;
  }    
  if(max>oldxmax+.0001){
    cout<<"max>histomax not good"<<endl;
    return NULL;
  }


  Int_t ntmpbins=((int)floor(1000*(max-min)  +.5))/prevBinning;
  Int_t newybins=oldhisto->GetYaxis()->GetNbins();
  Float_t newymin=oldhisto->GetYaxis()->GetXmin();
  Float_t newymax=oldhisto->GetYaxis()->GetXmax();
  TH2F* newhisto=new TH2F(TString(oldhisto->GetName())+"rebin",TString(oldhisto->GetName())+"rebin",ntmpbins,min,max,newybins,newymin,newymax);


  //first create a new histo with new range but same binning 
  Int_t newbx=1;
  for(Int_t by=1;by<=oldhisto->GetNbinsY();by++){
    newbx=1;
    for(Int_t bx=1;bx<=oldnbins;bx++){
      if(fabs(oldhisto->GetXaxis()->GetBinCenter(bx)-newhisto->GetXaxis()->GetBinCenter(newbx))
	 <oldbinwidth/100
	 && newbx<=ntmpbins){
	newhisto->SetBinContent(newbx,by,oldhisto->GetBinContent(bx,by));
	newhisto->SetBinError(newbx,by,oldhisto->GetBinError(bx,by));
	newbx++;
	
      }    
    }
    
  }

  if((newbx-1)!=ntmpbins){
    cout<<"There was a problem in rebinning, bin numbers dont match."<<endl;
    return NULL;
  }

  
  //now group bins
  Int_t group=ntmpbins/nbins;
  TH2F* rebinhisto=(TH2F*)newhisto->Rebin2D(group,1,TString(oldhisto->GetName())+"rebinned"); 
  
 
  delete newhisto;
  return rebinhisto;
}



