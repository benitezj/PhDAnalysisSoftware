
#include "../FUNCTIONS/DeterminePstarBins.C"
#include "../FUNCTIONS/DetermineFWHM.C"
#include "JResiduals.h"
#include <TPaveStats.h>

Int_t Ds1Analysis::FitDMResolution(){

  cout<<"Going to determine the Ds1 DM resolution"<<endl;
  
  if(!TruthMatch){
    cout<<"Resolution cannot be obtained from data"<<endl;
    return 0;
  }
  
  if(!OpenReducedNtuple()){
    cout<<"Failed to open the file"<<endl;
    return 0;
  }

  if(ReducedNtuple==NULL){
    cout<<"No ntuple"<<endl;
    return 0;
  }

  TCanvas Canvas("Canvas","Canvas");
  TString fname=_OutputDir+"/"+"Ds1DMResFit"+(long)_WhichCuts+".ps";
  Canvas.Print(fname+"[");



  TString resultsname=_OutputDir+"/"+"Ds1pDMResPars"+(long)_WhichCuts+".txt";
  FILE* results=fopen((const char*)resultsname,"w");
  cout<<"Resolution parameters File:"<<resultsname<<"  has been created."<<endl;
   

  text.SetTextSize(.03);
  
    
  Float_t fitrange=.0045;
  Float_t histrange=.006;
  Int_t ndmresbins=75;//75
  
  TCut totalcut=database.GetCut(1,100);


  TH1F HDs1DMRes("HDs1DMRes","Ds1 Resolution",ndmresbins,-histrange,histrange);
  char ytitle[100];
  sprintf(ytitle,"Entries/%.2fMeV",1000*2*histrange/ndmresbins);
  HDs1DMRes.SetXTitle("#Delta m_{rec}-#Delta m_{gen} [GeV]");
  HDs1DMRes.SetYTitle(ytitle);


  ReducedNtuple->Draw("ds1pdmres>>HDs1DMRes",totalcut);
  TF1*func=FitHistogramDs1Resolution(&HDs1DMRes,-fitrange,fitrange); 

  Float_t avgreswidth;
  Float_t avgresmean;
  Float_t maximumy;
  DetermineFWHM(func,&avgreswidth,&avgresmean,&maximumy);
  cout<<"Avg. FWHM: "<<avgreswidth<<" "<<avgresmean<<endl;

  Canvas.Clear();
  HDs1DMRes.Draw("p");
  func->Draw("same");
  sprintf(textlabel,"FWHM=%.6f",avgreswidth);
  text.DrawText(HDs1DMRes.GetXaxis()->GetXmin()+.05*(HDs1DMRes.GetXaxis()->GetXmax()-HDs1DMRes.GetXaxis()->GetXmin()),.1*HDs1DMRes.GetMaximum(),textlabel);
  Canvas.Print(fname);

  JResiduals fitresiduals(&HDs1DMRes,func);
  TH1F* ResidualHisto=fitresiduals.GetResidualHisto();
  Canvas.Clear();  
  ResidualHisto->Draw("pe");
  cutline.SetLineColor(1);
  cutline.DrawLine(-fitrange,0,fitrange,0);
  Canvas.Print(fname);  


  //fprintf(results,"avgnorm1 avgmean avgsigma1 avgnorm2 avgsigma2 avgnorm3 avgsigma3 ...\n");
  fprintf(results,"%.3e %.3e %.3e %.3e %.3e %.3e %.3e\n"
	  ,func->GetParameter(0)
	  ,func->GetParameter(1)
	  ,func->GetParameter(2)
	  ,func->GetParameter(3)
	  ,func->GetParameter(4)
	  ,func->GetParameter(5)
	  ,func->GetParameter(6)
	  );

  //Int_t Npbins=Npbins;
  Float_t Ds1ppstarMin=2.75;
  Float_t Ds1ppstarMax=4.75; 
  //Float_t deltapstar=(Ds1ppstarMax-Ds1ppstarMin)/Npbins;
  TH1F HDs1ppstar("HDs1ppstar","Ds1 p*",120*1000,Ds1ppstarMin,Ds1ppstarMax);
  ReducedNtuple->Draw("ds1ppstar>>HDs1ppstar",totalcut);
  Canvas.Clear();
  HDs1ppstar.Draw();
  Canvas.Print(fname);


  TH1F HDs1ppstar1("HDs1ppstar1","Ds1 p*",Npbins,Ds1ppstarMin,Ds1ppstarMax);
  HDs1ppstar1.SetXTitle("p* [GeV]");
  ReducedNtuple->Draw("ds1ppstar>>HDs1ppstar1",totalcut);
  Canvas.Clear();
  HDs1ppstar1.SetFillStyle(1);
  HDs1ppstar1.GetYaxis()->SetRangeUser(0,1.1*HDs1ppstar1.GetMaximum());
  HDs1ppstar1.Draw("bar");
  Canvas.Print(fname);


  Float_t pstarlowedge[Npbins+1];
  Float_t pstarbinavg[Npbins];
  DeterminePstarBins(&HDs1ppstar,pstarlowedge,pstarbinavg,Npbins);
  cout<<"The variable length p* bins"<<endl;
  cout<<"bin xlow xhigh xavg"<<endl;
  for(Int_t pb=0;pb<Npbins;pb++){
    cout<<pb<<" "<<(float)pstarlowedge[pb]<<" "<<(float)pstarlowedge[pb+1]<<" "<<pstarbinavg[pb]<<endl;
  }
  TH1F HDs1ppstar2("HDs1ppstar2","Ds1 p* New Binning",Npbins,pstarlowedge);
  HDs1ppstar2.SetXTitle("p* [GeV]");
  ReducedNtuple->Draw("ds1ppstar>>HDs1ppstar2",totalcut);
  Canvas.Clear();
  HDs1ppstar2.SetFillStyle(1);
  HDs1ppstar2.GetYaxis()->SetRangeUser(0,1.1*HDs1ppstar2.GetMaximum());
  HDs1ppstar2.Draw("bar");
  Canvas.Print(fname);

  //pstar dependent resolution
  
  TCut cutpstar[Npbins];
  for(Int_t pb=0;pb<Npbins;pb++){
    sprintf(textlabel,"%.4f<ds1ppstar&&ds1ppstar<%.4f",pstarlowedge[pb],pstarlowedge[pb+1]);
    cutpstar[pb]=textlabel;
  }


  TH1F HDs1DMResVsP[Npbins]; 
  TH1F HDs1DMResVsPSmallRange[Npbins];

  
  Float_t ds1reswidth[Npbins];
  Float_t ds1resmean[Npbins];
  Float_t resmin=.0005;
  Float_t resmax=.002;

  TGraphErrors GResVsp;  
  Int_t ngoodpoints=0;
  TGraphErrors GMeanVsp;  
  TGraph GRMS;
  TGraph GMean;
  
  fprintf(results,"%d\n",Npbins);
  //fprintf(results,"bin p mean norm1 sigma1 norm2 sigma2 norm3 sigma3 ...\n");
  
 

  for(Int_t pb=0;pb<Npbins;pb++){
    HDs1DMResVsP[pb]=TH1F(TString("HDs1DMResVsP")+(long)pb,TString("Ds1DMRes p*=")+(long)pb,ndmresbins,-histrange,histrange);
    HDs1DMResVsPSmallRange[pb]=TH1F(TString("HDs1DMResVsPSmallRange")+(long)pb,TString("Ds1DMRes p*=")+(long)pb,ndmresbins/2,-histrange/2,histrange/2); 
    char ytitle[100];
    sprintf(ytitle,"Entries/%.2fMeV",1000*2*histrange/ndmresbins);
    HDs1DMResVsP[pb].SetXTitle("#Delta m_{rec}-#Delta m_{gen} [GeV]");
    HDs1DMResVsP[pb].SetYTitle(ytitle);
    HDs1DMResVsP[pb].SetMarkerSize(.8);
    //HDs1DMResVsP[pb].Sumw2();//has to be called before filling, only matters if weight!=1
    
    ReducedNtuple->Draw(TString("ds1pdmres>>HDs1DMResVsP")+(long)pb,totalcut+cutpstar[pb]);
    ReducedNtuple->Draw(TString("ds1pdmres>>HDs1DMResVsPSmallRange")+(long)pb,totalcut+cutpstar[pb]);
    

    //HDs1DMResVsP[pb].Write();
    func=FitHistogramDs1Resolution(&HDs1DMResVsP[pb],-fitrange,fitrange);  
    if(func==NULL)continue;
  
    DetermineFWHM(func,&ds1reswidth[pb],&ds1resmean[pb],&maximumy);

    //ds1reswidth[pb]=func->GetParameter(2);
    //ds1resmean[pb]=func->GetParameter(1);
    cout<<pb<<" "<<pstarbinavg[pb]<<" "<<ds1reswidth[pb]<<" "<<ds1resmean[pb]<<endl;

    
    Canvas.Clear();
    HDs1DMResVsP[pb].Draw("pe");

    sprintf(textlabel,"FWHM=%.1f",ds1reswidth[pb]*1000);
    text.DrawText(HDs1DMResVsP[pb].GetXaxis()->GetXmin()+.05*(HDs1DMResVsP[pb].GetXaxis()->GetXmax()-HDs1DMResVsP[pb].GetXaxis()->GetXmin()),.1*HDs1DMResVsP[pb].GetMaximum(),textlabel);

    sprintf(textlabel,"p*=%.3f",pstarbinavg[pb]);
    text.DrawText(HDs1DMResVsP[pb].GetXaxis()->GetXmin()+.05*(HDs1DMResVsP[pb].GetXaxis()->GetXmax()-HDs1DMResVsP[pb].GetXaxis()->GetXmin()),.2*HDs1DMResVsP[pb].GetMaximum(),textlabel);

    Canvas.Print(fname);  
    
    JResiduals fitresiduals(&HDs1DMResVsP[pb],func);
    TH1F* ResidualHisto=fitresiduals.GetResidualHisto();
    Canvas.Clear();  
    ResidualHisto->Draw("pe");
    cutline.SetLineColor(1);
    cutline.DrawLine(-fitrange,0,fitrange,0);
    Canvas.Print(fname);  

    if(resmin<ds1reswidth[pb]&&ds1reswidth[pb]<resmax){
      GResVsp.SetPoint(ngoodpoints,pstarbinavg[pb],ds1reswidth[pb]*1000);
      //GResVsp.SetPointError(ngoodpoints,0,func->GetParError(2)*1000);
      GResVsp.SetPointError(ngoodpoints,0,.04*ds1reswidth[pb]*1000);
      GMeanVsp.SetPoint(ngoodpoints,pstarbinavg[pb],ds1resmean[pb]*1000);
      //GMeanVsp.SetPointError(ngoodpoints,0,func->GetParError(1)*1000);
      GRMS.SetPoint(ngoodpoints,pstarbinavg[pb],HDs1DMResVsPSmallRange[pb].GetRMS()*1000*sqrt(2.*log(2.)));
      GMean.SetPoint(ngoodpoints,pstarbinavg[pb],HDs1DMResVsPSmallRange[pb].GetMean()*1000);
      ngoodpoints++;
    }else cout<<"Bad resolution at "<<pb<<endl;
    
    fprintf(results,"%d %.2f %.3e %.3e %.3e %.3e %.3e %.3e %.3e\n"
	    ,pb
	    ,pstarbinavg[pb]
	    ,func->GetParameter(0)
	    ,func->GetParameter(1)
	    ,func->GetParameter(2)
	    ,func->GetParameter(3)
	    ,func->GetParameter(4)
	    ,func->GetParameter(5)
	    ,func->GetParameter(6)
	    );

  }
  
  fclose(results);
  cout<<"Parameters file closed."<<endl;


  if(!Close()){
    cout<<"Failed to close file"<<endl;
    return 0;
  }

  if(ngoodpoints==0){
    cout<<"Bad resolutions."<<endl;
    return 0;
  }

 
  GResVsp.SetMinimum(0.);
  GResVsp.SetMaximum(1.5);
  GResVsp.GetYaxis()->SetTitle("FWHM (MeV)");
  GResVsp.GetXaxis()->SetTitle("Ds1 p*  (GeV) ");
  GResVsp.Fit("pol1","Q");
  
 
  Canvas.Clear();
  GResVsp.Draw("ape");
  Canvas.Update();
  TPaveStats*stats=(TPaveStats*)GResVsp.GetListOfFunctions()->FindObject("stats");
  if(!stats)cout<<"GResVsp no stats: "<<endl;
  if(stats)stats->SetY1NDC(.25);
  if(stats)stats->SetY2NDC(.4);
  GResVsp.Draw("ape");
  // GRMS.Draw("lpsame");
  cutline.SetLineColor(4);
  if(_Mode=="D0ToKPi")cutline.DrawLine(pstarbinavg[0],1.13,pstarbinavg[Npbins-1],1.025);//torstens
  if(_Mode=="D0ToK3Pi")cutline.DrawLine(pstarbinavg[0],1.05,pstarbinavg[Npbins-1],.975);
  //cutline.SetLineColor(3);
  //cutline.DrawLine(pstarbinavg[0],avgreswidth*1000,pstarbinavg[Npbins-1],avgreswidth*1000);
  Canvas.Print(fname);  



  Canvas.Clear();
  GMeanVsp.SetMinimum(-.1);
  GMeanVsp.SetMaximum(.1);
  GMeanVsp.GetYaxis()->SetTitle("Fit Mean [MeV]");
  GMeanVsp.GetXaxis()->SetTitle("Ds1 p* [GeV]");
  GMeanVsp.Fit("pol1","QW");
  GMeanVsp.Draw("ape");
  Canvas.Update();
  stats=(TPaveStats*)GMeanVsp.GetListOfFunctions()->FindObject("stats");
  if(!stats)cout<<"GMeanVsp no stats: "<<endl;
  if(stats)stats->SetY1NDC(.1);
  if(stats)stats->SetY2NDC(.3);
  GMeanVsp.Draw("ape");
  GMean.Draw("lpsame");
  //cutline.SetLineColor(3);
  //cutline.DrawLine(pstarbinavg[0],avgresmean*1000,pstarbinavg[Npbins-1],avgresmean*1000);
  Canvas.Print(fname);  
  
  
  Canvas.Print(fname+"]");
  
  

  return 1;
}

