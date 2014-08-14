#include "../Globals.h"

void DeterminePstarBins(TH1F* HDs1ppstar,Float_t* pstarlowedge,Float_t* pstarbinavg,Int_t Nbins){


  //Int_t Npbins=HDs1ppstar->GetNbinsX();
  Int_t NEntriesPerBin=(int)HDs1ppstar->Integral()/Nbins;

  //TArrayF* pstarlowedge=new TArrayF(Nbins+1);
  pstarlowedge[0]=(float)HDs1ppstar->GetXaxis()->GetXmin();
  Int_t integral=0;
  Int_t bin=1;
  Float_t binwidth=HDs1ppstar->GetBinWidth(1);
  //Int_t integral=0;
  for(Int_t i=1;i<=Nbins;i++){
    integral=0;
    pstarbinavg[i-1]=0;
    while(integral<NEntriesPerBin&&bin<=HDs1ppstar->GetNbinsX()){
      integral+=(int)HDs1ppstar->GetBinContent(bin);
      //cout<<i<<" "<<bin<<" "<<integral<<endl;

      pstarbinavg[i-1]+=HDs1ppstar->GetBinCenter(bin)*HDs1ppstar->GetBinContent(bin);
      
      bin++;
    }
    if(integral>=NEntriesPerBin){
      pstarlowedge[i]=(float)pstarlowedge[0] + bin*binwidth;
      //cout<<i<<" "<<pstarlowedge[i]<<endl;
    }

    pstarbinavg[i-1]=pstarbinavg[i-1]/integral;
  } 
  if(integral<NEntriesPerBin){
    pstarlowedge[Nbins]=(float)(HDs1ppstar->GetXaxis()->GetXmax());
    //cout<<Nbins<<" "<<pstarlowedge[Nbins]<<endl;
  }
    

  return;
}
