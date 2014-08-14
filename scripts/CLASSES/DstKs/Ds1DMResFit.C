#include "../Globals.h"
#include "../FUNCTIONS/Fits.h"
#include <TLine.h>
#include <TGraphErrors.h>
#include <TCut.h>
#include <TArrayF.h>
#include "../CLASSES/JDataBase.h"
#include <TPaveStats.h>
#define Npbins 3

void DeterminePstarBins(TH1F* HDs1ppstar,Float_t* pstarlimit,Float_t* pstarbinavg);
void DetermineFWHM(TF1*F,Float_t* fwhm,Float_t* mean);
void fitDs1DMRes(TString rootfile){

  TFile*file=new TFile(rootfile,"read");
  TTree* tree=(TTree*)file->Get("Ds1pNtuple");
  if(tree!=NULL)cout<<"Ntuple: "<<rootfile<<" extracted with "<<tree->GetEntries()<<" entries"<<endl;
  tree->SetDirectory(0);
  file->Close();
 

  TCanvas Canvas("Canvas","Canvas");
  TString fname="Ds1DMResFit.ps";
  Canvas.Print(fname+"[");
  TLine line;

  Float_t fitrange=.0035;
  Float_t histrange=.006;
  //Float_t resmax=.0015;
  //Float_t resmin=.0008;
  Int_t ndmresbins=90;//75

  JDataBase database;
  database.SetCuts(1);
  Int_t ncuts=10;
  char cutstring[10][1000];
  TCut cut[10];


  TH1F HDs1DMRes("HDs1DMRes","Ds1 Resolution",ndmresbins,-histrange,histrange);
  char ytitle[100];
  sprintf(ytitle,"Entries/%.2fMeV",1000*2*histrange/ndmresbins);
  HDs1DMRes.SetXTitle("#Delta m_{rec}-#Delta m_{gen} [GeV]");
  HDs1DMRes.SetYTitle(ytitle);

  ///define the cuts
  sprintf(cutstring[0],"ds1pmctrue==1");
  sprintf(cutstring[1],"d0pstar>%.1f",database.GetD0pstarMin());
  sprintf(cutstring[2],"%.3f<dstardeltam&&dstardeltam<%.3f",database.GetDstarDeltaMMin(),database.GetDstarDeltaMMax());
  sprintf(cutstring[3],"klh>=%d",database.GetKLHBit());
  sprintf(cutstring[4],"pilh>=%d",database.GetPiLHBit());
  sprintf(cutstring[5],"d0vtxprob>%.4f",database.GetD0MinProbab());
  sprintf(cutstring[6],"abs(ksmass-%.4f)<%.3f",K0PDGMass,database.GetKsMassWindow());
  sprintf(cutstring[7],"kscosine>%.5f",database.GetKsCosineMin());
  sprintf(cutstring[8],"abs(d0mass-%.4f)<%.3f",D0PDGMass,database.GetD0MassWindow());
  sprintf(cutstring[9],"%.3f<ds1pdeltam2&&ds1pdeltam2<%.3f",database.GetDs1pDeltaMMin(),database.GetDs1pDeltaMMax());
 
 

  TCut totalcut;
  for(Int_t cutid=0;cutid<ncuts;cutid++){
    cut[cutid]=cutstring[cutid];
    cut[cutid].Print();
    totalcut+=cut[cutid];
  }
  
 
 
  Canvas.Clear();
  //HDs1DMRes.Sumw2();//has to be called before filling
  tree->Draw("ds1pdmres>>HDs1DMRes",totalcut);
  //TF1*func=FitHistogramBreitWigner(&HDs1DMRes,-fitrange,fitrange);
  TF1*func=FitHistogramDs1Resolution(&HDs1DMRes,-fitrange,fitrange); 
  HDs1DMRes.Draw("p");
  func->Draw("same");
  Canvas.Print(fname);

  //Int_t Npbins=Npbins;
  Float_t Ds1ppstarMin=2.75;
  Float_t Ds1ppstarMax=4.75; 
  //Float_t deltapstar=(Ds1ppstarMax-Ds1ppstarMin)/Npbins;
  TH1F HDs1ppstar("HDs1ppstar","Ds1 p*",120*1000,Ds1ppstarMin,Ds1ppstarMax);
  tree->Draw("ds1ppstar>>HDs1ppstar",totalcut);
  Canvas.Clear();
  HDs1ppstar.Draw();
  Canvas.Print(fname);


  TH1F HDs1ppstar1("HDs1ppstar1","Ds1 p*",Npbins,Ds1ppstarMin,Ds1ppstarMax);
  HDs1ppstar1.SetXTitle("p* [GeV]");
  tree->Draw("ds1ppstar>>HDs1ppstar1",totalcut);
  Canvas.Clear();
  HDs1ppstar1.Draw("bar");
  Canvas.Print(fname);

  
  Float_t pstarlowedge[Npbins+1];
  Float_t pstarbinavg[Npbins];
  DeterminePstarBins(&HDs1ppstar,pstarlowedge,pstarbinavg);
  for(Int_t pb=0;pb<Npbins;pb++){
    cout<<pb<<" "<<pstarbinavg[pb]<<" "<<(float)pstarlowedge[pb]<<" "<<(float)pstarlowedge[pb+1]<<endl;
  }
  TH1F HDs1ppstar2("HDs1ppstar2","Ds1 p* New Binning",Npbins,pstarlowedge);
  HDs1ppstar2.SetXTitle("p* [GeV]");
  tree->Draw("ds1ppstar>>HDs1ppstar2",totalcut);
  Canvas.Clear();
  HDs1ppstar2.GetYaxis()->SetRangeUser(0,1.1*HDs1ppstar2.GetBinContent(HDs1ppstar2.GetMaximumBin()));
  HDs1ppstar2.Draw("bar");
  Canvas.Print(fname);

  //pstar dependent resolution
  
  TCut cutpstar[Npbins];
  char cutpstarstring[Npbins][1000];
  for(Int_t pb=0;pb<Npbins;pb++){
    sprintf(cutpstarstring[pb],"%.4f<ds1ppstar&&ds1ppstar<%.4f",pstarlowedge[pb],pstarlowedge[pb+1]);
    cutpstar[pb]=cutpstarstring[pb];
  }


  TH1F HDs1DMResVsP[Npbins]; 
 

  
  Float_t ds1reswidth[Npbins];
  Float_t ds1resmean[Npbins];
  
  TGraphErrors GResVsp;  
  Int_t ngoodpoints=0;
  TGraphErrors GMeanVsp;  
  TGraph GRMS;
  TGraph GMean;
  
  
  for(Int_t pb=0;pb<Npbins;pb++){
    HDs1DMResVsP[pb]=TH1F(TString("HDs1DMResVsP")+(long)pb,TString("Ds1DMRes p*=")+(long)pb,ndmresbins,-histrange,histrange); 
    char ytitle[100];
    sprintf(ytitle,"Entries/%.2fMeV",1000*2*histrange/ndmresbins);
    HDs1DMResVsP[pb].SetXTitle("#Delta m_{rec}-#Delta m_{gen} [GeV]");
    HDs1DMResVsP[pb].SetYTitle(ytitle);
    HDs1DMResVsP[pb].SetMarkerSize(.8);
    //HDs1DMResVsP[pb].Sumw2();//has to be called before filling, only matters if weight!=1
    
    tree->Draw(TString("ds1pdmres>>HDs1DMResVsP")+(long)pb,totalcut+cutpstar[pb]);
     
    //func=FitHistogramBreitWigner(&HDs1DMResVsP[pb],-fitrange,fitrange);    
    //func=FitHistogram2gausFixRange(&HDs1DMResVsP[pb],-fitrange,fitrange);  
    func=FitHistogramDs1Resolution(&HDs1DMResVsP[pb],-fitrange,fitrange);  
    if(func==NULL)continue;
    
    DetermineFWHM(func,&ds1reswidth[pb],&ds1resmean[pb]);

    //ds1reswidth[pb]=func->GetParameter(2);
    //ds1resmean[pb]=func->GetParameter(1);
    cout<<pb<<" "<<ds1reswidth[pb]<<" "<<ds1resmean[pb]<<endl;

    Canvas.Clear();
    HDs1DMResVsP[pb].Draw("pe");
    Canvas.Print(fname);  
    
    //if(resmin<ds1reswidth[pb]&&ds1reswidth[pb]<resmax){
    GResVsp.SetPoint(ngoodpoints,pstarbinavg[pb],ds1reswidth[pb]*1000);
    //GResVsp.SetPointError(ngoodpoints,0,func->GetParError(2)*1000);
    GResVsp.SetPointError(ngoodpoints,0,ds1reswidth[pb]*1000/20);
    GMeanVsp.SetPoint(ngoodpoints,pstarbinavg[pb],ds1resmean[pb]*1000);
    //GMeanVsp.SetPointError(ngoodpoints,0,func->GetParError(1)*1000);
    GRMS.SetPoint(ngoodpoints,pstarbinavg[pb],HDs1DMResVsP[pb].GetRMS()*1000);
    GMean.SetPoint(ngoodpoints,pstarbinavg[pb],HDs1DMResVsP[pb].GetMean()*1000);
    ngoodpoints++;
    //}else cout<<"Bad resolution at "<<pb<<endl;
    
  }

  // if(ngoodpoints==0) return; 

 
  //GResVsp.SetMinimum(0.95);
  //GResVsp.SetMaximum(1.27);
  GResVsp.SetMinimum(0.);
  GResVsp.SetMaximum(1.5);
  GResVsp.GetYaxis()->SetTitle("FWHM [MeV]");
  GResVsp.GetXaxis()->SetTitle("Ds1 p*[GeV] ");
  GResVsp.Fit("pol1","Q");
  
//   GRMS.Fit("pol1","Q");
//   GRMS.Draw("ap");
//   TList*list=GRMS.GetListOfFunctions();
//   if(list!=NULL){
//     TPaveStats* stats=(TPaveStats*)list->FindObject("stasts");  
//     if(stats!=NULL){stats->SetY1NDC(.2); stats->SetY2NDC(.4); }
//     else cout<<"stats not found"<<endl;
//   }else cout<<"list not found"<<endl;
 
  Canvas.Clear();
  GResVsp.Draw("ape");
  GRMS.Draw("lpsame");
  line.SetLineColor(4);
  line.DrawLine(pstarbinavg[0],1.2,pstarbinavg[Npbins-1],1);
  Canvas.Print(fname);  



  Canvas.Clear();
  GMeanVsp.SetMinimum(-.1);
  GMeanVsp.SetMaximum(.1);
  GMeanVsp.GetYaxis()->SetTitle("Fit Mean [MeV]");
  GMeanVsp.GetXaxis()->SetTitle("Ds1 p* [GeV]");
  GMeanVsp.Fit("pol1","QW");
  GMeanVsp.Draw("ape");
  GMean.Draw("lpsame");
  Canvas.Print(fname);  
  



  Canvas.Print(fname+"]");
  file->Close();
  delete file;
  return; 
}

void DetermineFWHM(TF1* F,Float_t* fwhm,Float_t* mean){
  Int_t nbins=10000;
  Float_t xmaximum=F->GetXmin();
  Float_t xlow=F->GetXmin();
  Float_t xhigh=F->GetXmax();  
  Float_t xstep=(xhigh-xlow)/nbins;
  Float_t xmean=F->GetXmin();
  Float_t xfwhm=0;
  Float_t ymaximum=0;

  Float_t fvalue=0;
  Float_t x=0;
  
  Float_t tolerance=1;
  
  Float_t weightsum=0;
  Int_t npoints=0;
  //make first sweep to determine the mean and maximum
  for(Int_t b=1;b<nbins;b++){
    x=xlow+b*xstep;
    fvalue=F->Eval(x);

    //compute mean
    xmean+=x*fvalue;
    npoints++;
    weightsum+=fvalue;

    //find maximumn
    if(fvalue > F->Eval(xlow+(b-1)*xstep)){
      xmaximum=x;
      ymaximum=fvalue;
      tolerance=2*fvalue/nbins;
    }
    
    //now determine the fwhm
    if(ymaximum>0 && fabs(fvalue-ymaximum/2.)<tolerance)
      xfwhm=x;
    
    //cout<<xmaximum<<" "<<ymaximum<<endl;
  }
  
  //compute mean
  xmean=xmean/weightsum;
  
  if(xfwhm==0||ymaximum==0)cout<<"No FWHM found"<<endl;


  *fwhm=2*(xfwhm-xmaximum);
  *mean=xmean;

  return;
}


void DeterminePstarBins(TH1F* HDs1ppstar,Float_t* pstarlowedge,Float_t* pstarbinavg){


  //Int_t Npbins=HDs1ppstar->GetNbinsX();
  Int_t NEntriesPerBin=(int)HDs1ppstar->GetEntries()/Npbins;

  //TArrayF* pstarlowedge=new TArrayF(Npbins+1);
  pstarlowedge[0]=(float)HDs1ppstar->GetXaxis()->GetXmin();
  Int_t integral=0;
  Int_t bin=1;
  Float_t binwidth=HDs1ppstar->GetBinWidth(1);
  //Int_t integral=0;
  for(Int_t i=1;i<=Npbins;i++){
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
    pstarlowedge[Npbins]=(float)(HDs1ppstar->GetXaxis()->GetXmax());
    //cout<<Npbins<<" "<<pstarlowedge[Npbins]<<endl;
  }
    

  return;
}
