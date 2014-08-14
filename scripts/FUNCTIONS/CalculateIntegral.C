#include "../Globals.h"
Float_t CalculateIntegral(Float_t xmin,Float_t xmax,TH1* h){
  if(!h||xmax<xmin){
    cout<<"CalculateIntegral:: Warning: bad inputs h="<<h->GetName()<<endl;
    return 0;
  }
  //find the bin boudaries
  Int_t minbin=1;  
  Int_t ntotbins=h->GetNbinsX();
  Int_t maxbin=ntotbins;
  for(Int_t b=1;b<=ntotbins;b++){
    if(h->GetBinLowEdge(b)<=xmin && xmin<(h->GetBinLowEdge(b)+h->GetBinWidth(b)))
      minbin=b;
    
    if(h->GetBinLowEdge(b)<xmax && xmax<=(h->GetBinLowEdge(b)+h->GetBinWidth(b)))
      maxbin=b;

    //cout<<minbin<<" "<<maxbin<<" "<<h->GetBinLowEdge(b)<<" "<<xmin<<"  "<<xmax<<endl;
  }

//   if(minbin==-1||maxbin==-1){
//     cout<<"CalculateIntegral:: Warning xmin or xmax not found in histogram range h="<<h->GetName()<<endl;
//     return 0;
//   }
  
  return h->Integral(minbin,maxbin);
}
