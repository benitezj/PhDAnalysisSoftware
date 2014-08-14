

/////needed by resolution fits
#include "../FUNCTIONS/DeterminePstarBins.C"
#include "../FUNCTIONS/DetermineFWHM.C"
#include "JResiduals.h"
///////////////////////////////
#include <TPaveStats.h>

Int_t Dst2640Analysis::FitDMResolution(){
  
  
  TCanvas Canvas("Canvas","Canvas");
  TString fname=_OutputDir+"/"+"Dst2640DMResFit"+(long)_WhichCuts+".ps";
  Canvas.Print(fname+"[");
  TLine line;
  

  cout<<"Going to determine the Dst2640 DM resolution"<<endl;
  
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






  TString resultsname=_OutputDir+"/"+"Dst2640DMResPars"+(long)_WhichCuts+".txt";
  FILE* results=fopen((const char*)resultsname,"w");
  cout<<"Resolution parameters File:"<<resultsname<<"  has been created."<<endl;
   


    
  Float_t fitrange=.015;
  Float_t histrange=.02;
  Int_t ndmresbins=90;//75
  
  TCut totalcut=database.GetCut(1,100);


  TH1F HDst2640DMRes("HDst2640DMRes","Dst2640 Resolution",ndmresbins,-histrange,histrange);
  char ytitle[100];
  sprintf(ytitle,"Entries/%.2fMeV",1000*2*histrange/ndmresbins);
  HDst2640DMRes.SetXTitle("#Delta m_{rec}-#Delta m_{gen} [GeV]");
  HDst2640DMRes.SetYTitle(ytitle);


 
  ReducedNtuple->Draw("dst2640dmres>>HDst2640DMRes",totalcut);
  TF1*func=FitHistogramDs1Resolution(&HDst2640DMRes,-fitrange,fitrange); 


  Float_t avgreswidth;
  Float_t avgresmean;
  Float_t maximumy;
  DetermineFWHM(func,&avgreswidth,&avgresmean,&maximumy);
  cout<<"Avg. FWHM: "<<avgreswidth<<" "<<avgresmean<<endl;


  Canvas.Clear();
  HDst2640DMRes.Draw("p");
  func->Draw("same");
  sprintf(textlabel,"FWHM=%.6f",avgreswidth);
  text.DrawText(HDst2640DMRes.GetXaxis()->GetXmin()+.05*(HDst2640DMRes.GetXaxis()->GetXmax()-HDst2640DMRes.GetXaxis()->GetXmin()),.1*HDst2640DMRes.GetMaximum(),textlabel);
  Canvas.Print(fname);

  JResiduals fitresiduals(&HDst2640DMRes,func);
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
  Float_t Dst2640pstarMin=2.4;
  Float_t Dst2640pstarMax=4.75; 
  //Float_t deltapstar=(Dst2640pstarMax-Dst2640pstarMin)/Npbins;
  TH1F HDst2640pstar("HDst2640pstar","Dst2640 p*",120*1000,Dst2640pstarMin,Dst2640pstarMax);
  ReducedNtuple->Draw("dst2640pstar>>HDst2640pstar",totalcut);
  Canvas.Clear();
  HDst2640pstar.Draw();
  Canvas.Print(fname);


  TH1F HDst2640pstar1("HDst2640pstar1","Dst2640 p*",Npbins,Dst2640pstarMin,Dst2640pstarMax);
  HDst2640pstar1.SetXTitle("p* [GeV]");
  ReducedNtuple->Draw("dst2640pstar>>HDst2640pstar1",totalcut);
  Canvas.Clear();
  HDst2640pstar1.SetFillStyle(1);
  HDst2640pstar1.GetYaxis()->SetRangeUser(0,1.1*HDst2640pstar1.GetMaximum());
  HDst2640pstar1.Draw("bar");
  Canvas.Print(fname);


  Float_t pstarlowedge[Npbins+1];
  Float_t pstarbinavg[Npbins];
  DeterminePstarBins(&HDst2640pstar,pstarlowedge,pstarbinavg,Npbins);
  cout<<"The variable length p* bins"<<endl;
  cout<<"bin xlow xhigh xavg"<<endl;
  for(Int_t pb=0;pb<Npbins;pb++){
    cout<<pb<<" "<<(float)pstarlowedge[pb]<<" "<<(float)pstarlowedge[pb+1]<<" "<<pstarbinavg[pb]<<endl;
  }
  TH1F HDst2640pstar2("HDst2640pstar2","Dst2640 p* New Binning",Npbins,pstarlowedge);
  HDst2640pstar2.SetXTitle("p* [GeV]");
  ReducedNtuple->Draw("dst2640pstar>>HDst2640pstar2",totalcut);
  Canvas.Clear();
  HDst2640pstar2.SetFillStyle(1);
  HDst2640pstar2.GetYaxis()->SetRangeUser(0,1.1*HDst2640pstar2.GetMaximum());
  HDst2640pstar2.Draw("bar");
  Canvas.Print(fname);

  //pstar dependent resolution
  
  TCut cutpstar[Npbins];
  char cutpstarstring[Npbins][1000];
  for(Int_t pb=0;pb<Npbins;pb++){
    sprintf(cutpstarstring[pb],"%.4f<dst2640pstar&&dst2640pstar<%.4f",pstarlowedge[pb],pstarlowedge[pb+1]);
    cutpstar[pb]=cutpstarstring[pb];
  }


  TH1F HDst2640DMResVsP[Npbins]; 
 

  
  Float_t dmreswidth[Npbins];
  Float_t dmresmean[Npbins];
  Float_t resmin=.0005;
  Float_t resmax=.01;

  TGraphErrors GResVsp;  
  Int_t ngoodpoints=0;
  TGraphErrors GMeanVsp;  
  TGraph GRMS;
  TGraph GMean;
  
  fprintf(results,"%d\n",Npbins);
  //fprintf(results,"bin p mean norm1 sigma1 norm2 sigma2 norm3 sigma3 ...\n");
  
  
  for(Int_t pb=0;pb<Npbins;pb++){
    HDst2640DMResVsP[pb]=TH1F(TString("HDst2640DMResVsP")+(long)pb,TString("Dst2640DMRes p*=")+(long)pb,ndmresbins,-histrange,histrange); 
    char ytitle[100];
    sprintf(ytitle,"Entries/%.2fMeV",1000*2*histrange/ndmresbins);
    HDst2640DMResVsP[pb].SetXTitle("#Delta m_{rec}-#Delta m_{gen} [GeV]");
    HDst2640DMResVsP[pb].SetYTitle(ytitle);
    HDst2640DMResVsP[pb].SetMarkerSize(.8);
    //HDst2640DMResVsP[pb].Sumw2();//has to be called before filling, only matters if weight!=1
    
    ReducedNtuple->Draw(TString("dst2640dmres>>HDst2640DMResVsP")+(long)pb,totalcut+cutpstar[pb]);
    
    //HDst2640DMResVsP[pb].Write();
    func=FitHistogramDs1Resolution(&HDst2640DMResVsP[pb],-fitrange,fitrange);  
    if(func==NULL)continue;
  
    DetermineFWHM(func,&dmreswidth[pb],&dmresmean[pb],&maximumy);

    //dmreswidth[pb]=func->GetParameter(2);
    //dmresmean[pb]=func->GetParameter(1);
    cout<<pb<<" "<<dmreswidth[pb]<<" "<<dmresmean[pb]<<endl;

    Canvas.Clear();
    HDst2640DMResVsP[pb].Draw("pe");
    sprintf(textlabel,"FWHM=%.1f",dmreswidth[pb]*1000);
    text.DrawText(HDst2640DMResVsP[pb].GetXaxis()->GetXmin()+.05*(HDst2640DMResVsP[pb].GetXaxis()->GetXmax()-HDst2640DMResVsP[pb].GetXaxis()->GetXmin()),.1*HDst2640DMResVsP[pb].GetMaximum(),textlabel);
    sprintf(textlabel,"p*=%.3f",pstarbinavg[pb]);
    text.DrawText(HDst2640DMResVsP[pb].GetXaxis()->GetXmin()+.05*(HDst2640DMResVsP[pb].GetXaxis()->GetXmax()-HDst2640DMResVsP[pb].GetXaxis()->GetXmin()),.2*HDst2640DMResVsP[pb].GetMaximum(),textlabel);
    Canvas.Print(fname);  
    
    JResiduals fitresiduals(&HDst2640DMResVsP[pb],func);
    TH1F* ResidualHisto=fitresiduals.GetResidualHisto();
    Canvas.Clear();  
    ResidualHisto->Draw("pe");
    cutline.SetLineColor(1);
    cutline.DrawLine(-fitrange,0,fitrange,0);
    Canvas.Print(fname);  

    if(resmin<dmreswidth[pb]&&dmreswidth[pb]<resmax){
      GResVsp.SetPoint(ngoodpoints,pstarbinavg[pb],dmreswidth[pb]*1000);
      //GResVsp.SetPointError(ngoodpoints,0,func->GetParError(2)*1000);
      GResVsp.SetPointError(ngoodpoints,0,.04*dmreswidth[pb]*1000);
      GMeanVsp.SetPoint(ngoodpoints,pstarbinavg[pb],dmresmean[pb]*1000);
      //GMeanVsp.SetPointError(ngoodpoints,0,func->GetParError(1)*1000);

      GRMS.SetPoint(ngoodpoints,pstarbinavg[pb],HDst2640DMResVsP[pb].GetRMS()*1000*sqrt(2.*log(2.)));
      GMean.SetPoint(ngoodpoints,pstarbinavg[pb],HDst2640DMResVsP[pb].GetMean()*1000);
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
 
  GResVsp.SetMinimum(0.);
  GResVsp.SetMaximum(6.);
  GResVsp.GetYaxis()->SetTitle("FWHM (MeV)");
  GResVsp.GetXaxis()->SetTitle("D*(2640) p* (GeV) ");
  GResVsp.Fit("pol1","Q");
  
  Canvas.Clear();
  GResVsp.Draw("ape");
  Canvas.Update();
  TPaveStats*stats=(TPaveStats*)GResVsp.GetListOfFunctions()->FindObject("stats");
  if(!stats)cout<<"GResVsp no stats: "<<endl;
  if(stats)stats->SetY1NDC(.3);
  if(stats)stats->SetY2NDC(.5);
  GResVsp.Draw("ape");
  //GRMS.Draw("lpsame");
  Canvas.Print(fname);  



  Canvas.Clear();
  GMeanVsp.SetMinimum(-1);
  GMeanVsp.SetMaximum(1);
  GMeanVsp.GetYaxis()->SetTitle("Fit Mean [MeV]");
  GMeanVsp.GetXaxis()->SetTitle("Dst2640 p* [GeV]");
  GMeanVsp.Fit("pol1","QW");
  GMeanVsp.Draw("ape");
  //GMean.Draw("lpsame");
  Canvas.Print(fname);  
  
  
  Canvas.Print(fname+"]");
  
  if(!Close()){
    cout<<"Failed to close file"<<endl;
    return 0;
  }

  if(ngoodpoints==0){
    cout<<"Bad resolutions."<<endl;
    return 0;
  }
  
  
 
  return 1;
}

