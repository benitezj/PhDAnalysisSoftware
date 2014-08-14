#include "DstPiAnalysis.h"
#include "../JResiduals.h"

#define PSFACTOR "((1/(2*(x+2.01)**2))*sqrt(((x+2.01)*(x+2.01) - 4.618)*((x+2.01)*(x+2.01)-3.500)))"
#define QFACTOR "((1/(2*(x+2.01)))*sqrt(((x+2.01)*(x+2.01) - 4.618)*((x+2.01)*(x+2.01)-3.500)))"

Int_t DstPiAnalysis::FitMassPeak(Bool_t fit1,Bool_t fit2,Bool_t fit3,Bool_t fit4,Bool_t fit5,Bool_t fit6){
  cout<<"Going to fit the D*Pi Mass Peak"<<endl;

  //
  TString directory=_OutputDir;

  //ps file
  TString filename=directory+"/DstPiFit.ps";
  TCanvas Canvas(filename,filename);
  Canvas.Print(filename+"[");

  //
  TLine normline;  
  

  //apply some cuts 
  TCut rscut="abs(dstpicharge)==1";
  TCut wscut="abs(dstpicharge)==2";
  TCut TotalCut=database.GetCut(2,100);

  ///Open the clean reduced Ntuple
  OpenReducedNtuple(1);
 
  //make a copy of the tree with some cuts
  TTree* tree=ReducedNtuple->CopyTree(TotalCut);
  
  ///ranges//With cuts 
  Float_t binsize=.005;
  Float_t rangemin=.1;
  Float_t rangemax=1.5;
  Int_t nbins=(int)((rangemax-rangemin)/binsize);  
  Float_t fitmin=database.GetDstPiDeltaMRangeMin();
  Float_t fitmax=database.GetDstPiDeltaMRangeMax();
  Float_t fitcutmin=.35;//.16
  Float_t fitcutmax=1.1;//1.5

  Float_t fitmin2=database.GetDstPiDeltaMMin();//.27
  Float_t fitmax2=database.GetDstPiDeltaMMax();//.62
  Int_t nbins2=(int)((fitmax2-fitmin2)/binsize);    

  TString Xlabel="m(D*#pi) - m(D*)    (GeV/c^{2})";
  TString Ylabel="Entries/5 MeV";
  
  cout<<"signal fit range: "<<fitmin2<<" "<<fitmax2<<endl;
  cout<<"Full Fit Range: "<<fitmin<<" "<<fitmax<<endl;
  
  //the q factor for the threshold
  //(1/(2*M))*sqrt((M^2 - (mDstar+mpi)^2)*(M^2 - (mDstar-mpi)^2))
  TString phasefactor=TString(PSFACTOR);
  TString qfactor=TString(QFACTOR);  


  ///print the cuts into the file
  database.PrintCutList(&Canvas,filename);

  //----------
  //Plot the D* deltaM
  //----------
  TH1F HDstar("HDstar","HDstar",100,.14,.15);
  HDstar.GetXaxis()->SetTitle("#DeltaM_{D*}");
  HDstar.GetXaxis()->SetTitle("Entries /.1 MeV");
  tree->Draw("dstardeltam>>HDstar",TotalCut+rscut);
  Canvas.Clear();
  HDstar.Draw("pe");
  Canvas.Print(filename);
  

  ////--------------Get the RS for the full range
  TH1F HDstarPi("HDstarPi","HDstarPi",nbins,rangemin,rangemax);
  HDstarPi.GetXaxis()->SetTitle(Xlabel);
  HDstarPi.GetYaxis()->SetTitle(Ylabel);   
  tree->Draw("dstpideltam>>HDstarPi",TotalCut+rscut);
  HDstarPi.Sumw2();
  Canvas.Clear();
  HDstarPi.Draw("pe");
  Canvas.Print(filename);

  
  //------------------
  //Make a simple fit on the 2420 nad 2460 only
  //------------------
  TH1F HDstarPi2("HDstarPi2","HDstarPi2",nbins2,fitmin2,fitmax2); 
  HDstarPi2.GetXaxis()->SetTitle(Xlabel);
  HDstarPi2.GetYaxis()->SetTitle(Ylabel); 
  tree->Draw("dstpideltam>>HDstarPi2",TotalCut+rscut);

  //Note parameter [1] is reserved for Dst2420 mean, [2] for Dst2420 Width, [4] for Dst2460 Mean,[5] for Dst2460 Width

  //NonRel-BW No phase space
  TString NonRelBW2420="[0]/((x-[1])**2 + (.5*[2])**2)";
  TString NonRelBW2460="[3]/((x-[4])**2 + (.5*[5])**2)"; 

  //Rel-BW No phase space
  TString RelBW2420="[0]*x/((x*x-[1]*[1])**2 + ([1]*[2])**2)";
  TString RelBW2460="[3]*x/((x*x-[4]*[4])**2 + ([4]*[5])**2)"; 

  //formula for relativistic BW times  2-body phase space
  TString RelBWPS2420=phasefactor+"*"+"[0]*x/((x*x-[1]*[1])**2 + ([1]*[2])**2)";
  TString RelBWPS2460=phasefactor+"*"+"[3]*x/((x*x-[4]*[4])**2 + ([4]*[5])**2)";
  
  //Proper line shapes 
  //--------------------D(2420)
  TString phasefactor2420="((1/(2*([1]+2.01)**2))*sqrt((([1]+2.01)*([1]+2.01) - 4.618)*(([1]+2.01)*([1]+2.01)-3.500)))";
  TString Width2420="([2]*"+phasefactor+"/"+phasefactor2420+")";
  TString Signal2420=phasefactor+"*"+"[0]*( x/( (x*x-[1]*[1])**2 + ([1]*"+Width2420+")**2 ))";
  Float_t Integral2420=26.577;

  //test a spin-2 line shape 
  TString qfactor2420="((1/(2*([1]+2.01)))*sqrt((([1]+2.01)*([1]+2.01) - 4.618)*(([1]+2.01)*([1]+2.01)-3.500)))"; 
  TString Radius2420="5";//5 GeV ~ .6fm
  TString BWfactor2420="(9+3*("+qfactor+"*"+Radius2420+")**2+("+qfactor+"*"+Radius2420+")**4)";
  TString BWfactor02420="(9+3*("+qfactor2420+"*"+Radius2420+")**2+("+qfactor2420+"*"+Radius2420+")**4)";
  //TString Width2420="([2]*("+phasefactor+"/"+phasefactor2420+")*(("+qfactor+"/"+qfactor2420+")**4)*("+BWfactor02420+"/"+BWfactor2420+")"+")";
  //TString Signal2420=phasefactor+"*"+"[0]*( (x*("+qfactor+"/"+qfactor2420+")**4)*("+BWfactor02420+"/"+BWfactor2420+")"+"/( (x*x-[1]*[1])**2 + ([1]*"+Width2420+")**2))"; 
  //Float_t Integral2420=33.6081;


  //-------------------D*(2460)
  TString phasefactor2460="((1/(2*([4]+2.01)**2))*sqrt((([4]+2.01)*([4]+2.01) - 4.618)*(([4]+2.01)*([4]+2.01)-3.500)))";
  TString qfactor2460="((1/(2*([4]+2.01)))*sqrt((([4]+2.01)*([4]+2.01) - 4.618)*(([4]+2.01)*([4]+2.01)-3.500)))";
  TString Radius2460="3";//5 GeV ~ .6fm
  TString BWfactor2460="(9+3*("+qfactor+"*"+Radius2460+")**2+("+qfactor+"*"+Radius2460+")**4)";
  TString BWfactor02460="(9+3*("+qfactor2460+"*"+Radius2460+")**2+("+qfactor2460+"*"+Radius2460+")**4)";
  //spin-1
  //TString Width2460="([5]*"+phasefactor+"/"+phasefactor2460+")";
  //TString Signal2460=phasefactor+"*"+"[3]*( x/( (x*x-[4]*[4])**2 + ([4]*"+Width2460+")**2 ))";
  //spin-2
  TString Width2460="([5]*("+phasefactor+"/"+phasefactor2460+")*(("+qfactor+"/"+qfactor2460+")**4)*("+BWfactor02460+"/"+BWfactor2460+")"+")";
  TString Signal2460=phasefactor+"*"+"[3]*( (x*("+qfactor+"/"+qfactor2460+")**4)*("+BWfactor02460+"/"+BWfactor2460+")"+"/( (x*x-[4]*[4])**2 + ([4]*"+Width2460+")**2))";


  ///lets make plots of these shapes
  //phase space factors
  Canvas.Clear();
  Canvas.Divide(1,2);
  Canvas.cd(1);
  TF1 fps("fps",phasefactor,fitmin2,fitmax2);
  fps.Draw();
  Canvas.cd(2);
  TF1 fq("fq",qfactor,fitmin2,fitmax2);
  fq.Draw();
  Canvas.Print(filename);

  Canvas.Clear();
  Canvas.Divide(2,3);  
  Canvas.cd(1);
  TF1 fpsps02420("fpsps02420",phasefactor+"/"+phasefactor2420,fitmin2,fitmax2);
  fpsps02420.SetParameters(0,.412);
  cout<<fpsps02420.Eval(.412)<<endl;
  fpsps02420.Draw(); 
  Canvas.cd(3);
  TF1 fqq02420("fqq02420",qfactor+"/"+qfactor2420,fitmin2,fitmax2);
  fqq02420.SetParameters(0,.412);
  cout<<fqq02420.Eval(.412)<<endl;
  fqq02420.Draw();  
  Canvas.cd(5);
  TF1 fBWBW02420("fBWBW02420",BWfactor02420+"/"+BWfactor2420,fitmin2,fitmax2);
  fBWBW02420.SetParameters(0,.412);
  cout<<fBWBW02420.Eval(.412)<<endl;
  fBWBW02420.Draw();  

  Canvas.cd(2);
  TF1 fpsps02460("fpsps02460",phasefactor+"/"+phasefactor2460,fitmin2,fitmax2);
  fpsps02460.SetParameters(0,0,0,0,.450);
  cout<<fpsps02460.Eval(.450)<<endl;
  fpsps02460.Draw();
  Canvas.cd(4);
  TF1 fqq02460("fqq02460",qfactor+"/"+qfactor2460,fitmin2,fitmax2);
  fqq02460.SetParameters(0,0,0,0,.450);
  cout<<fqq02460.Eval(.450)<<endl;
  fqq02460.Draw();  
  Canvas.cd(6);
  TF1 fBWBW02460("fBWBW02460",BWfactor02460+"/"+BWfactor2460,fitmin2,fitmax2);
  fBWBW02460.SetParameters(0,0,0,0,.450);
  cout<<fBWBW02460.Eval(.450)<<endl;
  fBWBW02460.Draw();  

  Canvas.Print(filename);

  ////D(2420) line shape
  Canvas.Clear();
  //non-rel BW
  TF1 fnrbw2420("fnrbw2420",NonRelBW2420,fitmin2,fitmax2);  
  fnrbw2420.SetParameters(1./299.815,.412,.0204);
  cout<<fnrbw2420.Integral(fitmin2,fitmax2)<<endl;
  fnrbw2420.SetLineColor(1);
  fnrbw2420.SetNpx(10000);
  fnrbw2420.Draw();
  //rel BW
  TF1 frbw2420("frbw2420",RelBW2420,fitmin2,fitmax2);  
  frbw2420.SetParameters(1./181.484,.412,.0204);
  cout<<frbw2420.Integral(fitmin2,fitmax2)<<endl;
  frbw2420.SetLineColor(2);
  frbw2420.SetLineStyle(2);
  frbw2420.SetNpx(10000);
  frbw2420.Draw("same");
  //rel BW times phase space
  TF1 frbwps2420("frbw2420",RelBWPS2420,fitmin2,fitmax2);
  frbwps2420.SetParameters(1./26.5781,.412,.0204);
  cout<<frbwps2420.Integral(fitmin2,fitmax2)<<endl;
  frbwps2420.SetLineColor(3);
  frbwps2420.SetLineStyle(3);
  frbwps2420.SetNpx(10000);
  frbwps2420.Draw("same");
  //proper line shape
  TF1 fsig2420("fsig2420",Signal2420,fitmin2,fitmax2);
  fsig2420.SetParameters(1./Integral2420,.412,.0204);
  cout<<fsig2420.Integral(fitmin2,fitmax2)<<endl;
  fsig2420.SetLineColor(4);
  fsig2420.SetLineStyle(4);
  fsig2420.SetNpx(10000);
  fsig2420.Draw("same");
  Canvas.Print(filename);
  
  //  Canvas.Print(filename+"]");
  //   return 1;

  ///D*(2460) line shape
  Canvas.Clear();
  //rel BW times phase space
  TF1 frbwps2460("frbw2460",RelBWPS2460,fitmin2,fitmax2);
  frbwps2460.SetParameters(0,0,0,1./12.36227,.450,.043);
  cout<<frbwps2460.Integral(fitmin2,fitmax2+.5)<<endl;
  frbwps2460.SetLineColor(3);
  frbwps2460.SetLineStyle(3);
  frbwps2460.SetNpx(10000);
  frbwps2460.Draw();
  //proper line shape
  TF1 fsig2460("fsig2460",Signal2460,fitmin2,fitmax2);
  fsig2460.SetParameters(0,0,0,1./14.376813,.450,.043);
  cout<<fsig2460.Integral(fitmin2,fitmax2+.5)<<endl;
  fsig2460.SetLineColor(4);
  fsig2460.SetLineStyle(4);
  fsig2460.SetNpx(10000);
  fsig2460.Draw("same");
  Canvas.Print(filename);

  //the background
  TString Signal1f;
  TString Signal2f;
  Signal1f=NonRelBW2420;
  Signal2f=NonRelBW2460;
  Signal1f=RelBW2420;
  Signal2f=RelBW2460;  
  Signal1f=RelBWPS2420;
  Signal2f=RelBWPS2460;  
  Signal1f=Signal2420;
  Signal2f=Signal2460;  

  TString Bkgf=phasefactor+"*"+"([6]+[7]*x+[8]*x**2+[9]*x**3+[10]*x**4+[11]*x**5+[12]*x**6+[13]*x**7+[14]*x**8+[15]*x**9)";
  TF1 FRS2("FRS2",Signal1f+"+"+Signal2f+"+"+Bkgf,fitmin2,fitmax2); 
  FRS2.SetParNames("N_{1}","#mu_{1}","#Gamma_{1}","N_{2}","#mu_{2}","#Gamma_{2}");
  FRS2.SetLineColor(4);
  FRS2.SetLineWidth(2);
  FRS2.SetNpx(10000);
  FRS2.SetParLimits(0,.01,100); 
  FRS2.SetParLimits(1,.402,.422);    
  FRS2.SetParLimits(2,.020,.040); 
  FRS2.SetParLimits(3,.01,50); 
  FRS2.SetParLimits(4,.440,.460);
  FRS2.SetParLimits(5,.030,.060); 

//   FRS2.SetParLimits(6,.5e1,20e7);    
//   FRS2.SetParLimits(7,-20e5,-.1e5);     
//   FRS2.SetParLimits(8,-10e8,-.1e1); 
//   FRS2.SetParLimits(9,.5e5,12e5); 
//   FRS2.SetParLimits(10,1e5,20e5);      
//   FRS2.SetParLimits(11,.1e5,10e5);    
//   FRS2.SetParLimits(12,-10e6,-.1e6);     
//   FRS2.SetParLimits(13,-20e5,-4e5); 
//   FRS2.SetParLimits(14,.1e6,5e6); 
//   FRS2.SetParLimits(15,2e5,15e5);

//   FRS2.FixParameter(0,19.8);
//   FRS2.FixParameter(1,.412);//note that the charged D_1 and D*_2 have different masses and widths
//   FRS2.FixParameter(2,.0107);//32
//   FRS2.FixParameter(3,10);
//   FRS2.FixParameter(4,.450);
//   FRS2.FixParameter(5,.015);//46

  // FRS2.FixParameter(12,0);
  //FRS2.FixParameter(13,0);
  FRS2.FixParameter(14,0);
  FRS2.FixParameter(15,0);

 
  if(fit1){
    Int_t fitstatus=HDstarPi2.Fit("FRS2","","",fitmin2,fitmax2);
    if(fitstatus!=0){
      //FRS2.SetParLimits(6,.1e4,40e4); 
      FRS2.SetParameter(1,2*FRS2.GetParameter(1));
      fitstatus=HDstarPi2.Fit("FRS2","I","pe",fitmin2,fitmax2);
    }
    if(fitstatus!=0){
      //FRS2.SetParLimits(7,-40e5,-.05e4); 
      FRS2.SetParameter(3,2*FRS2.GetParameter(3));
      fitstatus=HDstarPi2.Fit("FRS2","I","pe",fitmin2,fitmax2);
    }
    if(fitstatus!=0)cout<<"-------------------Fit did not converge----------------------"<<endl;
  }
  //the signals
  TF1 fsig1("fsig1",Signal1f,fitmin,fitmax);
  fsig1.SetParameters(FRS2.GetParameter(0),FRS2.GetParameter(1),FRS2.GetParameter(2));
  fsig1.SetLineColor(4);
  fsig1.SetNpx(10000);  
  TF1 fsig2("fsig2",Signal2f,fitmin,fitmax);
  fsig2.SetParameters(0,0,0,FRS2.GetParameter(3),FRS2.GetParameter(4),FRS2.GetParameter(5));
  fsig2.SetLineColor(4);
  fsig2.SetNpx(10000);
  //the background
  TF1 FRS2bkg("FRS2bkg",phasefactor+"*([0]+[1]*x+[2]*x**2+[3]*x**3+[4]*x**4+[5]*x**5+[6]*x**6+[7]*x**7+[8]*x**8+[9]*x**9)",fitmin2,fitmax2); 
  FRS2bkg.SetParameters(FRS2.GetParameter(6),
			FRS2.GetParameter(7),
			FRS2.GetParameter(8),
			FRS2.GetParameter(9),
			FRS2.GetParameter(10),
			FRS2.GetParameter(11),
			FRS2.GetParameter(12),
			FRS2.GetParameter(13),
			FRS2.GetParameter(14),
			FRS2.GetParameter(15));
  FRS2bkg.SetLineColor(2);
  FRS2bkg.SetLineWidth(2);
  FRS2bkg.SetLineStyle(2);
  
  Canvas.Clear();
  HDstarPi2.Draw();
  Canvas.Update();
  Canvas.Clear();
  //HDstarPi2.SetStats(0);
  HDstarPi2.GetYaxis()->SetRangeUser(0,HDstarPi2.GetMaximum()*1.1);
  HDstarPi2.Draw("pe");
  TPaveStats*st=(TPaveStats*)HDstarPi2.FindObject("stats");
  if(st){
    st->SetX1NDC(.6);
    st->SetX2NDC(.79);
    st->SetY1NDC(.35);
    st->SetY2NDC(.82);
  }else cout<<"no stats"<<endl;
  FRS2.Draw("same");
  FRS2bkg.Draw("same");
  fsig1.Draw("same");
  fsig2.Draw("same");
  Canvas.Update();
  Canvas.Print(filename);

  JResiduals RS2Residuals(&HDstarPi2,&FRS2);
  TH1F*HRS2res=RS2Residuals.GetResidualHisto();
  HRS2res->GetYaxis()->SetRangeUser(-1000,1000);
  Canvas.Clear();
  HRS2res->Draw();
  normline.SetLineWidth(3);
  normline.SetLineColor(4);
  normline.DrawLine(fitmin2,0,fitmax2,0);
  Canvas.Print(filename);
  

  if(fit2){
    TGraphErrors GchiVsPolDeg;
    GchiVsPolDeg.SetMarkerSize(2);
  
    Float_t chiperdf=0;
    Int_t ngoodpoints=0;
    Canvas.Clear();
    Canvas.Divide(2,5);
    for(Int_t c=1;c<=10;c++){
      Canvas.cd(c);
      chiperdf=0;
      chiperdf=FitHistogram2BWPlusPolN(&HDstarPi2,fitmin2,fitmax2,c+2,phasefactor);
      if(chiperdf>0){
	GchiVsPolDeg.SetPoint(ngoodpoints,c+2,chiperdf);
	GchiVsPolDeg.SetPointError(ngoodpoints,0,0);
	ngoodpoints++;
      }    
    } 
    Canvas.Print(filename);
    
    GchiVsPolDeg.Draw("ape");
    GchiVsPolDeg.GetXaxis()->SetTitle("Order of Polynomial");
    GchiVsPolDeg.GetYaxis()->SetTitle("#chi^{2}/ndf");
    GchiVsPolDeg.GetXaxis()->SetRangeUser(0,3);
    Canvas.Clear();
    GchiVsPolDeg.Draw("ape");
    Canvas.Print(filename);
  }





  ///-----------------------
  ///Get the Wrong Sign Plot it under the Right-Sign and then fit it
  //------------------------
  TH1F HDstarPiWS("HDstarPiWS","HDstarPiWS",nbins,rangemin,rangemax);
  HDstarPiWS.GetXaxis()->SetTitle(Xlabel);
  HDstarPiWS.GetYaxis()->SetTitle(Ylabel);   
  tree->Draw("dstpideltam>>HDstarPiWS",TotalCut+wscut);
  for(Int_t b=1;b<nbins;b++)
    if(HDstarPiWS.GetBinCenter(b)<.148)HDstarPiWS.SetBinContent(b,0);

  
  TH1F*HDstarPiSub=(TH1F*)HDstarPi.Clone("HDstarPiSub");
  //HDstarPiSub->Add(&HDstarPi,1);
  for(Int_t b=1;b<nbins;b++)//subtract the region of the D* peak in the Wrong sign
    if(HDstarPiSub->GetBinCenter(b)<.148)
      HDstarPiSub->SetBinContent(b,0);
  TH1F*HDstarPiWSScale=(TH1F*)HDstarPiWS.Clone("HDstarPiWSScale");
  HDstarPiWSScale->Scale(HDstarPiSub->Integral(1,20)/HDstarPiWSScale->Integral(1,20));
  Canvas.Clear();
  HDstarPiSub->Draw("pe");
  HDstarPiWSScale->Draw("same");
  Canvas.Print(filename); 

  //make a plot where we subtract the WS from the RS
  HDstarPiSub->Add(HDstarPiWSScale,-1);
  Canvas.Clear();
  HDstarPiSub->Draw("pe");
  normline.DrawLine(rangemin,0,rangemax,0);
  Canvas.Print(filename);
  Canvas.Clear();
  HDstarPiSub->Draw("pe");
  Canvas.SetLogy(1);
  Canvas.Print(filename);
  Canvas.SetLogy(0);

  delete HDstarPiWSScale;
  delete HDstarPiSub;

  TString BkgWS2=phasefactor+"*exp([0]*x)*([1]*exp(-.5*(x-[2])**2/[3]**2) + [4]*exp(-.5*(x-[5])**2/[6]**2)+ [7]*exp(-.5*(x-[8])**2/[9]**2))";

  TF1 FuncWS("FuncWS",BkgWS2,fitmin,fitmax); 
  //
  FuncWS.SetLineColor(4);
  FuncWS.SetLineWidth(2);
  FuncWS.SetNpx(10000);
  FuncWS.SetParLimits(0,-5,-.001);
  FuncWS.SetParLimits(1,1e1,1e7);  
  FuncWS.SetParLimits(2,.001,2);  
  FuncWS.SetParLimits(3,.1,5);  
  FuncWS.SetParLimits(4,1e1,1e7);    
  FuncWS.SetParLimits(5,.0001,3.);  
  FuncWS.SetParLimits(6,.1,3);    
  FuncWS.SetParLimits(7,1e1,1e7);    
  FuncWS.SetParLimits(8,.00001,4);  
  FuncWS.SetParLimits(9,.1,5);  
    
  TH1F* HDstarPiWSClone=(TH1F*)HDstarPiWS.Clone("HDstarPiWSClone");
  if(fit3)HDstarPiWS.Fit("FuncWS","","pe",fitmin,fitmax);
  Canvas.Clear();
  HDstarPiWS.Draw("pe");
  FuncWS.Draw("same");
  Canvas.Print(filename);

  JResiduals WSResiduals(&HDstarPiWS,&FuncWS);
  TH1F*HWSres=WSResiduals.GetResidualHisto();
  HWSres->GetYaxis()->SetRangeUser(-500,500);
  Canvas.Clear();
  HWSres->Draw();
  normline.SetLineWidth(3);
  normline.SetLineColor(4);
  normline.DrawLine(fitmin,0,fitmax,0);
  Canvas.Print(filename);

   
  /*
  ///--------------Fit the full range of RS by adding a third guass for the extra bump
  //TString Signal3f=phasefactor+"*"+"[16]*x/((x*x-[17]*[17])**2 + ([17]*[18])**2)";
  //TString Signal3f=phasefactor+"*"+"[16]*exp(-.5*(x-[17])**2/[18]**2)";
  TString Signal3f=phasefactor+"*"+"([16]*exp(-.5*(x-[17])**2/[18]**2)+[19]*exp(-.5*(x-[17])**2/[20]**2))";

  TString BkgWS=phasefactor+"*exp([6]*x)*([7]*exp(-.5*(x-[8])**2/[9]**2) + [10]*exp(-.5*(x-[11])**2/[12]**2)+ [13]*exp(-.5*(x-[14])**2/[15]**2))";

  TF1 FRS3("FRS3",Signal1f+"+"+Signal2f+"+"+BkgWS+"+"+Signal3f,rangemin,rangemax); 
  FRS3.SetParNames("N_{1}","#mu_{1}","#Gamma_{1}","N_{2}","#mu_{2}","#Gamma_{2}");
  FRS3.SetLineColor(4);
  FRS3.SetLineWidth(2);
  FRS3.SetNpx(10000);

  FRS3.SetParLimits(0,.9*FRS2.GetParameter(0),1.1*FRS2.GetParameter(0));
  FRS3.SetParLimits(1,.9*FRS2.GetParameter(1),1.1*FRS2.GetParameter(1));
  FRS3.SetParLimits(2,.9*FRS2.GetParameter(2),1.1*FRS2.GetParameter(2));

  FRS3.SetParLimits(3,.9*FRS2.GetParameter(3),1.1*FRS2.GetParameter(3));
  FRS3.SetParLimits(4,.9*FRS2.GetParameter(4),1.1*FRS2.GetParameter(4));
  FRS3.SetParLimits(5,.9*FRS2.GetParameter(5),1.1*FRS2.GetParameter(5));  

  FRS3.SetParLimits(6,1.9*FuncWS.GetParameter(0),.6*FuncWS.GetParameter(0));//negative
  FRS3.SetParLimits(7,.6*FuncWS.GetParameter(1),1.4*FuncWS.GetParameter(1));
  //FRS3.SetParLimits(8,.6*FuncWS.GetParameter(2),1.4*FuncWS.GetParameter(2));
  FRS3.FixParameter(8,FuncWS.GetParameter(2));
  //FRS3.SetParLimits(9,.6*FuncWS.GetParameter(3),1.4*FuncWS.GetParameter(3));
  FRS3.FixParameter(9,FuncWS.GetParameter(3));
  FRS3.SetParLimits(10,.3*FuncWS.GetParameter(4),1.4*FuncWS.GetParameter(4));
  //FRS3.SetParLimits(11,.6*FuncWS.GetParameter(5),1.4*FuncWS.GetParameter(5));
  FRS3.FixParameter(11,FuncWS.GetParameter(5));
  //FRS3.SetParLimits(12,.6*FuncWS.GetParameter(6),1.4*FuncWS.GetParameter(6));
  FRS3.FixParameter(12,FuncWS.GetParameter(6));
  FRS3.SetParLimits(13,.6*FuncWS.GetParameter(7),1.4*FuncWS.GetParameter(7));
  //FRS3.SetParLimits(14,.6*FuncWS.GetParameter(8),1.4*FuncWS.GetParameter(8));
  FRS3.FixParameter(14,FuncWS.GetParameter(8));
  //FRS3.SetParLimits(15,.6*FuncWS.GetParameter(9),1.4*FuncWS.GetParameter(9));
  FRS3.FixParameter(15,FuncWS.GetParameter(9));

  FRS3.SetParLimits(16,.00001,1); 
  FRS3.SetParLimits(17,.600,.700);
  FRS3.SetParLimits(18,.01,5); 
  FRS3.SetParLimits(19,.001,2e6); 
  FRS3.SetParLimits(20,.01,6);


  if(fit3){
    Int_t fitstatus=HDstarPi2.Fit("FRS3","","",rangemin,rangemax);
    if(fitstatus!=0){
      FRS3.SetParameter(1,2*FRS3.GetParameter(1));
      fitstatus=HDstarPi2.Fit("FRS3","","",rangemin,rangemax);
    }
    if(fitstatus!=0){
      FRS3.SetParameter(3,2*FRS3.GetParameter(3));
      fitstatus=HDstarPi2.Fit("FRS3","","",rangemin,rangemax);
    }
    if(fitstatus!=0)cout<<"-------------------Fit did not converge----------------------"<<endl;
  }
  
  Canvas.Clear();
  HDstarPi.Draw("pe");
  FRS3.Draw("same");
  Canvas.Print(filename); 
  */

  //Now cut the bump region and fit again to show that the outer regions define the background 
  //first make a clone of the function 
  TF1* FuncWSClone=(TF1*)FuncWS.Clone("FuncWSClone");
  FuncWSClone->SetLineColor(2);
  for(Int_t b=1;b<nbins;b++)
    if(fitcutmin<HDstarPiWSClone->GetBinCenter(b)&&HDstarPiWSClone->GetBinCenter(b)<fitcutmax){
      HDstarPiWSClone->SetBinContent(b,0);
      HDstarPiWSClone->SetBinError(b,0);
    }
  
  FuncWSClone->SetParLimits(0,1.9*FuncWS.GetParameter(0),.1*FuncWS.GetParameter(0));//negative
  FuncWSClone->SetParLimits(1,.1*FuncWS.GetParameter(1),1.9*FuncWS.GetParameter(1));
  FuncWSClone->SetParLimits(2,1.1*FuncWS.GetParameter(2),.9*FuncWS.GetParameter(2));
  FuncWSClone->SetParLimits(3,1.1*FuncWS.GetParameter(3),.9*FuncWS.GetParameter(3));
  FuncWSClone->SetParLimits(4,.1*FuncWS.GetParameter(4),1.9*FuncWS.GetParameter(4));
  FuncWSClone->SetParLimits(5,1.1*FuncWS.GetParameter(5),.9*FuncWS.GetParameter(5));
  FuncWSClone->SetParLimits(6,1.1*FuncWS.GetParameter(6),.9*FuncWS.GetParameter(6));
  FuncWSClone->SetParLimits(7,.1*FuncWS.GetParameter(7),1.9*FuncWS.GetParameter(7));
  FuncWSClone->SetParLimits(8,1.1*FuncWS.GetParameter(8),.9*FuncWS.GetParameter(8));
  FuncWSClone->SetParLimits(9,1.1*FuncWS.GetParameter(9),.9*FuncWS.GetParameter(9));
  delete HDstarPiWS.FindObject("stats");
  if(fit4)HDstarPiWSClone->Fit("FuncWSClone","","pe",fitmin,fitmax);
  Canvas.Clear();
  HDstarPiWSClone->Draw("pe");
  FuncWSClone->Draw("same");
  Canvas.Print(filename);
  JResiduals WSCutResiduals(HDstarPiWSClone,FuncWSClone);
  TH1F*HWSCutres=WSCutResiduals.GetResidualHisto();
  HWSCutres->GetYaxis()->SetRangeUser(-500,500);
  Canvas.Clear();
  HWSCutres->Draw();
  normline.SetLineWidth(3);
  normline.SetLineColor(4);
  normline.DrawLine(fitmin,0,fitmax,0);
  Canvas.Print(filename);

  Canvas.Clear();
  FuncWSClone->Draw();
  FuncWS.Draw("same");
  Canvas.Print(filename);

  delete HDstarPiWSClone;
  delete FuncWSClone;

  FuncWS.FixParameter(2,FuncWS.GetParameter(2));
  FuncWS.FixParameter(3,FuncWS.GetParameter(3));
  FuncWS.FixParameter(5,FuncWS.GetParameter(5));
  FuncWS.FixParameter(6,FuncWS.GetParameter(6));
  FuncWS.FixParameter(8,FuncWS.GetParameter(8));
  FuncWS.FixParameter(9,FuncWS.GetParameter(9));

  //------------------
  //Get the D*pi deltaM Right sign subtract the 2420 and 2460 then fit the background
  //------------------
 
  //create a histogram and fill it the D signals
  TF1 FDSignals("FDSignals",Signal1f+"+"+Signal2f,rangemin,rangemax);
  FDSignals.SetParameter(0,FRS2.GetParameter(0));
  FDSignals.SetParameter(1,FRS2.GetParameter(1));
  FDSignals.SetParameter(2,FRS2.GetParameter(2));
  FDSignals.SetParameter(3,FRS2.GetParameter(3));
  FDSignals.SetParameter(4,FRS2.GetParameter(4));
  FDSignals.SetParameter(5,FRS2.GetParameter(5));
  //determine the number of singal particles
  Float_t fintegral=FDSignals.Integral(rangemin,rangemax);
  Int_t NDs=(int)(fintegral/binsize);
  cout<<"The number of D_1 and D_2 ="<<NDs<<endl;
  
  TH1F HDSignals("HDSignals","HDSignals",nbins,rangemin,rangemax);
  HDSignals.GetXaxis()->SetTitle(Xlabel);
  HDSignals.GetYaxis()->SetTitle(Ylabel); 
  HDSignals.FillRandom("FDSignals",NDs);
  
  //now subtract the signals from the full spectrum
  HDstarPi.Sumw2();//this makes the fit not ignore the 0's.
  TH1F*HDstarPiNoSignals=(TH1F*)HDstarPi.Clone("HDstarPiNoSignals");
  HDstarPiNoSignals->Add(&HDSignals,-1);

  Canvas.Clear();
  HDstarPi.GetYaxis()->SetRangeUser(0,HDstarPi.GetMaximum()*1.1);
  HDstarPi.Draw("pe");
  Canvas.Print(filename);

  Canvas.Clear();
  HDstarPiNoSignals->GetYaxis()->SetRangeUser(0,HDstarPiNoSignals->GetMaximum()*1.1); 
  HDstarPiNoSignals->Draw("pe");
  Canvas.Print(filename);

  //------------------------------------
  //Take the signal-subtracted spectrum cut the bumps out and then fit the background
  //-------------------------------------  
  TH1F*HDstarPiCut=(TH1F*)HDstarPiNoSignals->Clone();
  //remove the bumps
  for(Int_t b=1;b<nbins;b++)
    if(fitcutmin<HDstarPiCut->GetBinCenter(b)&&HDstarPiCut->GetBinCenter(b)<fitcutmax){
      HDstarPiCut->SetBinContent(b,0);
      HDstarPiCut->SetBinError(b,0);
    }
  
  delete HDstarPiCut->FindObject("stats");
  if(fit5)HDstarPiCut->Fit("FuncWS","","pe",fitmin,fitmax);//note the parameters start from the parameters of the WS fit
  
  Canvas.Clear();
  HDstarPiCut->Draw("pe");
  Canvas.Print(filename);

  JResiduals BkgResiduals(HDstarPiCut,&FuncWS);
  TH1F*HBkgres=BkgResiduals.GetResidualHisto();
  HBkgres->GetYaxis()->SetRangeUser(-500,500);
  Canvas.Clear();
  HBkgres->Draw();
  normline.SetLineWidth(3);
  normline.SetLineColor(4);
  normline.DrawLine(fitmin,0,fitmax,0);
  Canvas.Print(filename);
  
  Canvas.Clear();
  HDstarPiNoSignals->Draw("pe");
  FuncWS.Draw("same");
  Canvas.Print(filename);



  //-----------------------------------
  //Subtract the combinatorial background
  //----------------------------------
  Float_t Fbkgintegral=FuncWS.Integral(rangemin,rangemax);
  Int_t Nbkg=(int)(Fbkgintegral/binsize);
  //create a histogram and fill it with background
  TH1F HBackground("HBackground","HBackground",nbins,rangemin,rangemax);
  HBackground.GetXaxis()->SetTitle(Xlabel);
  HBackground.GetYaxis()->SetTitle(Ylabel); 
  HBackground.FillRandom("FuncWS",Nbkg);

  Canvas.Clear();
  HDstarPiNoSignals->Draw("pe");
  HBackground.Draw("same");
  Canvas.Print(filename);
  
  delete HDstarPiNoSignals->FindObject("stats");
  TH1F*HDstarPiNoSignalsNoBkg=(TH1F*)HDstarPiNoSignals->Clone();
  HDstarPiNoSignalsNoBkg->Sumw2();
  HDstarPiNoSignalsNoBkg->Add(&HBackground,-1);
  
  Canvas.Clear();
  HDstarPiNoSignalsNoBkg->Draw("pe");
  normline.SetLineColor(1);
  normline.DrawLine(rangemin,0,rangemax,0);
  Canvas.Print(filename);
  
  //do a rebining then fit 
  TF1 FBumps("FBumps","([0]*exp(-.5*(x-[1])**2/[2]**2)+[3]*exp(-.5*(x-[4])**2/[5]**2)+[6]*exp(-.5*(x-[7])**2/[8]**2))",fitmin,fitmax);
  FBumps.SetParNames("N_{1}","#mu_{1}","#sigma_{1}","N_{2}","#mu_{2}","#sigma_{2}","N_{3}","#mu_{3}","#sigma_{3}");
  FBumps.SetLineColor(4);
  FBumps.SetLineWidth(2);
  FBumps.SetNpx(10000);

  FBumps.SetParLimits(0,0,1e6); 
  FBumps.SetParLimits(1,.55,.65);    
  FBumps.SetParLimits(2,.01,.5); 
  FBumps.SetParLimits(3,0,1e6); 
  FBumps.SetParLimits(4,.7,1);    
  FBumps.SetParLimits(5,.01,.2); 
  FBumps.SetParLimits(6,0,1e6); 
  FBumps.SetParLimits(7,.7,1);    
  FBumps.SetParLimits(8,.01,.5); 

  delete HDstarPiNoSignalsNoBkg->FindObject("stats");
  TH1F*HBumpRebin=(TH1F*)HDstarPiNoSignalsNoBkg->Rebin(4,"HBumpRebin");
  HBumpRebin->GetYaxis()->SetTitle("Entries/20 MeV");
  if(fit6)HBumpRebin->Fit("FBumps","","pe",fitmin,fitmax);

  Canvas.Clear();  
  HBumpRebin->Draw("pe");
  normline.DrawLine(rangemin,0,rangemax,0);
  Canvas.Print(filename);


  JResiduals BumpResiduals(HBumpRebin,&FBumps);
  TH1F*HBumpres=BumpResiduals.GetResidualHisto();
  HBumpres->GetYaxis()->SetRangeUser(-1000,1000);
  Canvas.Clear();
  HBumpres->Draw();
  normline.SetLineWidth(3);
  normline.SetLineColor(2);
  normline.DrawLine(rangemin,0,rangemax,0);
  Canvas.Print(filename);

  Canvas.Print(filename+"]");



  delete HDstarPiNoSignals;
  delete HDstarPiCut;
  delete HDstarPiNoSignalsNoBkg;
  delete HBumpRebin;
  delete tree;
  CloseReducedNtuple();



  //Canvas.Print(filename+"]");
  //return 1;


  return 1;

}


/*
  ///-------------------------------------------
  ///now create a histogram of the right sign and then cut out the signal region to fit the background
  //--------------------------------------------
  TH1F*HDstarPiCut=(TH1F*)HDstarPi.Clone();
  //remove the peaks
  for(Int_t b=1;b<nbins;b++)
    if(fitcutmin<HDstarPiCut->GetBinCenter(b)&&HDstarPiCut->GetBinCenter(b)<fitcutmax)HDstarPiCut->SetBinContent(b,0);

  TF1* FuncRSCut=new TF1("FuncRSCut",phasefactor+"*(exp([0]*x)*([1]*exp(-.5*(x-[2])**2/[3]**2) + [4]*exp(-.5*(x-[5])**2/[6]**2) + [7]*exp(-.5*(x-[8])**2/[9]**2)) + [10]*x/(pow((x*x-[11]*[11])/([11]*[12]),2) + 1) + [13]*x/(pow((x*x-[14]*[14])/([14]*[15]),2) + 1) +(.4<x&&x<.58)*([16] + [17]*x + [18]*x**2+ [19]*x**4))",fitmin,fitmax); 
  //(.35<x&&x<.6)*([16] + [17]*x + [18]*x**2 + [19]*x**3 + [20]*x**4 + [21]*x**5 )
  //exp(-.5*(x-[16])**2/[17]**2)*([18] + [19]*x + [20]*x**2 + [21]*x**3 )
  //+ exp(-.5*(x-[16])**2/[17]**2)*([18] + [19]*x + [20]*x**2 + [21]*x**3 + [22]*x**2 + [23]*x**3 )

  //set limits on the paramaters
  FuncRSCut->SetLineColor(4);
  FuncRSCut->SetLineWidth(2);
  FuncRSCut->SetNpx(10000);
  FuncWS->SetParLimits(0,-5,-1);
  FuncWS->SetParLimits(1,1e3,1e7);  
  FuncWS->SetParLimits(2,.001,2);  
  FuncWS->SetParLimits(3,.1,5);  
  FuncWS->SetParLimits(4,1e2,1e6);    
  FuncWS->SetParLimits(5,-10.,-3.);  
  FuncWS->SetParLimits(6,.1,3);    
  FuncWS->SetParLimits(7,1e2,1e6);    
  FuncWS->SetParLimits(8,.0001,4);  
  FuncWS->SetParLimits(9,.1,5);   
  FuncRSCut->SetParLimits(10,0,1e6);//signals 
  FuncRSCut->SetParLimits(11,.4,.42);    
  FuncRSCut->SetParLimits(12,.01,.1); 
  FuncRSCut->SetParLimits(13,0,1e6); 
  FuncRSCut->SetParLimits(14,.44,.46);    
  FuncRSCut->SetParLimits(15,.01,.1); 
  FuncRSCut->SetParLimits(16,-1e9,1e9);  
  FuncRSCut->SetParLimits(17,-1e9,1e9);    
  FuncRSCut->SetParLimits(18,-1e9,1e9);   
  FuncRSCut->SetParLimits(19,-1e9,1e9);  
  FuncRSCut->SetParLimits(20,-1e9,1e9);    
  FuncRSCut->SetParLimits(21,-1e9,1e9);   
  FuncRSCut->SetParLimits(22,-1e9,1e9);    
  FuncRSCut->SetParLimits(23,-1e9,1e9);
  ////initialize some parameters
  FuncRSCut->SetParameter(0,FuncWS->GetParameter(0));
  FuncRSCut->SetParameter(1,FuncWS->GetParameter(1));
  FuncRSCut->SetParameter(2,FuncWS->GetParameter(2));
  FuncRSCut->SetParameter(3,FuncWS->GetParameter(3));
  FuncRSCut->SetParameter(4,FuncWS->GetParameter(4));
  FuncRSCut->SetParameter(5,FuncWS->GetParameter(5));
  FuncRSCut->SetParameter(6,FuncWS->GetParameter(6));
  FuncRSCut->SetParameter(7,FuncWS->GetParameter(7));
  FuncRSCut->SetParameter(8,FuncWS->GetParameter(8));
  FuncRSCut->SetParameter(9,FuncWS->GetParameter(9));
  ///fix some parameters
  //  FuncRSCut->FixParameter(0,FuncWS->GetParameter(0));//exp decay
//   FuncRSCut->FixParameter(2,FuncWS->GetParameter(2));//gaus1 mean
//   FuncRSCut->FixParameter(3,FuncWS->GetParameter(3));//gaus1 sigma
//   FuncRSCut->FixParameter(5,FuncWS->GetParameter(5));//gaus2 mean
//   FuncRSCut->FixParameter(6,FuncWS->GetParameter(6));//gaus2 sigma
//   FuncRSCut->FixParameter(8,FuncWS->GetParameter(8));//gaus3 mean
//   FuncRSCut->FixParameter(9,FuncWS->GetParameter(9));//gaus3 sigma
  //FuncRSCut->FixParameter(11,.4117);//note that the charged Ds have different masses and widths
  //FuncRSCut->FixParameter(12,.0204);  
  //FuncRSCut->FixParameter(14,.4511);
  //FuncRSCut->FixParameter(15,.043);  
  //FuncRSCut->FixParameter(16,.65);  
  //FuncRSCut->FixParameter(17,.55);
  //FuncRSCut->FixParameter(18,.025);  
  //FuncRSCut->FixParameter(20,.75);
  //FuncRSCut->FixParameter(21,.03);  
  
  if(fitRSCut)HDstarPiCut->Fit(FuncRSCut,"","pe",fitmin,fitmax);

  TPaveStats*stats1=(TPaveStats*)HDstarPiCut->FindObject("stats");
  if(!stats1){cout<<"not stats box"<<endl;return;}
  stats1->SetX1NDC(.49);
  stats1->SetX2NDC(.79);
  stats1->SetY1NDC(.22);
  stats1->SetY2NDC(.82);
  Canvas.Clear();
  HDstarPiCut->Draw("pe");
  FuncRSCut->Draw("same");
  //stats1->Draw("same");
  Canvas.Print(filename);

  JResiduals RSCutResiduals(HDstarPiCut,FuncRSCut);
  TH1F*HRSCutres=RSCutResiduals.GetResidualHisto();
  HRSCutres->GetYaxis()->SetRangeUser(-1000,1000);
  Canvas.Clear();
  HRSCutres->Draw();
  normline.SetLineWidth(3);
  normline.SetLineColor(4);
  normline.DrawLine(fitmin,0,fitmax,0);
  Canvas.Print(filename);

  Canvas.Clear();
  HDstarPi.SetStats(0);
  HDstarPi.Draw("pe");
  FuncRSCut->SetRange(fitmin,fitcutmin);
  FuncRSCut->Draw("same");
  TF1* FuncRSCutNew=(TF1*) FuncRSCut->Clone();
  FuncRSCutNew->SetRange(fitcutmax,fitmax);
  FuncRSCutNew->Draw("same");
  stats1->Draw("same");
  Canvas.Print(filename);
  
  ///zoom in to see the fit at the signals
  Canvas.Clear();
  HDstarPi.GetXaxis()->SetRangeUser(minzoom,maxzoom);
  HDstarPi.SetStats(0);
  HDstarPi.Draw("pe");
  FuncRSCut->Draw("same");
  //Draw the backgrounds
  //  TF1 FBkg("FBkg",
  Canvas.Print(filename);
*/



  /*
  //---------------------------------------------------------
  //normalize the WS to the RS 
  //---------------------------------------------------------
  Float_t sfactor=HDstarPi.Integral(minnormbin,maxnormbin)/HDstarPiWS.Integral(minnormbin,maxnormbin);
  cout<<"Scale Factor"<<sfactor<<endl;
  TH1F* HDstarPiWSScale=(TH1F*)HDstarPiWS.Clone();

  if(HDstarPiWSScale==NULL){cout<<"clone not created"<<endl;return;}

  HDstarPiWSScale->Scale(sfactor);
  for(Int_t b=0;b<nbins;b++){
    HDstarPiWSScale->SetBinError(b,sfactor*HDstarPiWS.GetBinError(b));
  }
  
  //now Subtract the WS from the RS
  TH1F HDstarPiBkgSub("HDstarPiBkgSub","HDstarPiBkgSub",nbins,rangemin,rangemax);  
  HDstarPiBkgSub.Add(&HDstarPi);
  HDstarPiBkgSub.Add(HDstarPiWSScale,-1);
  for(Int_t b=0;b<nbins;b++){
    HDstarPiBkgSub.SetBinError(b,sqrt(pow(HDstarPiWSScale->GetBinError(b),2)+pow(HDstarPi.GetBinError(b),2)));
  } 


  Canvas.Clear();
  HDstarPi.Draw("pe");
  HDstarPiWSScale->Draw("pesame");
  normline.DrawLine(minnorm,0,minnorm,HDstarPiWSScale->GetMaximum());
  normline.DrawLine(maxnorm,0,maxnorm,HDstarPiWSScale->GetMaximum());
  Canvas.Print(filename);
  

  Canvas.Clear();
  HDstarPiBkgSub.GetYaxis()->SetRangeUser(0,1.1*HDstarPiBkgSub.GetMaximum());
  HDstarPiBkgSub.Draw("pe");
  Canvas.Print(filename);

  ///--------------------
  //now lets fit the subtracted distribution
  //---------------
  TF1* Func=new TF1("Func",phasefactor+"*([0]*x/(pow((x*x-[1]*[1])/([1]*[2]),2) + 1) + [3]*x/(pow((x*x-[4]*[4])/([4]*[5]),2) + 1) + [6]*x/(pow((x*x-[7]*[7])/([7]*[8]),2) + 1) + [9]*x/(pow((x*x-[10]*[10])/([10]*[11]),2) + 1) + ([12]+[13]*x+[14]*x**2+[15]*x**3+[16]*x**4+[17]*x**5+[18]*x**6) )",fitmin,fitmax);

  Func->SetParName(0,"N_{1}");
  Func->SetParName(1,"#mu_{1}");
  Func->SetParName(2,"#Gamma_{1}");
  Func->SetParName(3,"N_{2}");
  Func->SetParName(4,"#mu_{2}");
  Func->SetParName(5,"#Gamma_{2}");
  Func->SetParName(6,"N_{3}");
  Func->SetParName(7,"#mu_{3}");
  Func->SetParName(8,"#Gamma_{3}");
  Func->SetParName(9,"N_{4}");
  Func->SetParName(10,"#mu_{4}");
  Func->SetParName(11,"#Gamma_{4}");
  Func->SetLineColor(2);
  Func->SetLineWidth(2);
  Func->SetNpx(10000);

  Func->SetParLimits(0,1e3,1e6);
  Func->SetParLimits(1,.403,.423);
  Func->SetParLimits(2,.02,.03);
  Func->SetParLimits(3,1e3,1e6);
  Func->SetParLimits(4,.439,.459);
  Func->SetParLimits(5,.025,.1);
  Func->SetParLimits(6,1e3,1e8);
  Func->SetParLimits(7,.5,.7);
  Func->SetParLimits(8,.05,.5);
  Func->SetParLimits(9,1e1,1e8);
  Func->SetParLimits(10,.7,.9);
  Func->SetParLimits(11,.05,1);
  Func->SetParLimits(12,-1e6,1e6);//polynomial
  Func->SetParLimits(13,-1e6,1e6);
  Func->SetParLimits(14,-1e6,1e6);
  Func->SetParLimits(15,-1e6,1e6);
  Func->SetParLimits(16,-1e6,1e6);
  Func->SetParLimits(17,-1e6,1e6);
  Func->SetParLimits(18,-1e6,1e6);


  if(fitSignal)HDstarPiBkgSub.Fit(Func,"LI","pe",fitmin,fitmax);
  

  ///The individual signals
  TF1 fsig1("fsig1",phasefactor+"*[0]*x/(pow((x*x-[1]*[1])/([1]*[2]),2) + 1)",fitmin,fitmax);
  fsig1.SetParameters(Func->GetParameter(0),Func->GetParameter(1),Func->GetParameter(2));
  fsig1.SetLineColor(4);
  fsig1.SetNpx(10000);
  TF1 fsig2("fsig2",phasefactor+"*[0]*x/(pow((x*x-[1]*[1])/([1]*[2]),2) + 1)",fitmin,fitmax);
  fsig2.SetParameters(Func->GetParameter(3),Func->GetParameter(4),Func->GetParameter(5));
  fsig2.SetLineColor(4);
  fsig2.SetNpx(10000);
  TF1 fsig3("fsig3",phasefactor+"*[0]*x/(pow((x*x-[1]*[1])/([1]*[2]),2) + 1)",fitmin,fitmax);
  fsig3.SetParameters(Func->GetParameter(6),Func->GetParameter(7),Func->GetParameter(8));
  fsig3.SetLineColor(4);
  fsig3.SetNpx(10000);
  TF1 fsig4("fsig4",phasefactor+"*[0]*x/(pow((x*x-[1]*[1])/([1]*[2]),2) + 1)",fitmin,fitmax);
  fsig4.SetParameters(Func->GetParameter(9),Func->GetParameter(10),Func->GetParameter(11));
  fsig4.SetLineColor(4);
  fsig4.SetNpx(10000);
  TF1 fbkg("fbkg",phasefactor+"*([0]+[1]*x+[2]*x**2+[3]*x**3+[4]*x**4+[5]*x**5+[6]*x**6)",fitmin,fitmax);
  fbkg.SetParameters(Func->GetParameter(12),Func->GetParameter(13),Func->GetParameter(14),Func->GetParameter(15),Func->GetParameter(16),Func->GetParameter(17),Func->GetParameter(18));
  fbkg.SetLineColor(1);
  fbkg.SetNpx(10000);

  //full background
  TF1 fbkgfull("fbkgfull",phasefactor+"*([0]*x/(pow((x*x-[1]*[1])/([1]*[2]),2) + 1)+[3]*x/(pow((x*x-[4]*[4])/([4]*[5]),2) + 1) + ([6]+[7]*x+[8]*x**2+[9]*x**3+[10]*x**4+[11]*x**5+[12]*x**6))",fitmin,fitmax);
  fbkgfull.SetParameter(0,Func->GetParameter(6));
  fbkgfull.SetParameter(1,Func->GetParameter(7));
  fbkgfull.SetParameter(2,Func->GetParameter(8));
  fbkgfull.SetParameter(3,Func->GetParameter(9));
  fbkgfull.SetParameter(4,Func->GetParameter(10));
  fbkgfull.SetParameter(5,Func->GetParameter(11));
  fbkgfull.SetParameter(6,Func->GetParameter(12));
  fbkgfull.SetParameter(7,Func->GetParameter(13));
  fbkgfull.SetParameter(8,Func->GetParameter(14));
  fbkgfull.SetParameter(9,Func->GetParameter(15));
  fbkgfull.SetParameter(10,Func->GetParameter(16));
  fbkgfull.SetParameter(11,Func->GetParameter(17));
  fbkgfull.SetParameter(12,Func->GetParameter(18));
  fbkgfull.SetLineColor(1);
  fbkgfull.SetNpx(10000);
  

  Canvas.Clear();
  HDstarPiBkgSub.GetYaxis()->SetRangeUser(-1000,1.1*HDstarPiBkgSub.GetMaximum());
  HDstarPiBkgSub.Draw("pe");
  Func->Draw("same");
  TPaveStats*stats=(TPaveStats*)HDstarPiBkgSub.FindObject("stats");
  if(!stats){cout<<"not stats box"<<endl;return;}
  stats->SetX1NDC(.49);
  stats->SetX2NDC(.79);
  stats->SetY1NDC(.22);
  stats->SetY2NDC(.82);
  normline.DrawLine(rangemin,0,rangemax,0);
  Canvas.Update();
  Canvas.Print(filename);


  //Zoom into fit region 
  TH1F HDstarPiBkgSubZoom("HDstarPiBkgSubZoom","HDstarPiBkgSubZoom",maxbinzoom-minbinzoom,minzoom,maxzoom);   
  for(Int_t b=0;b<(maxbinzoom-minbinzoom);b++){
    HDstarPiBkgSubZoom.SetBinContent(b,HDstarPiBkgSub.GetBinContent(b+minbinzoom));
    HDstarPiBkgSubZoom.SetBinError(b,HDstarPiBkgSub.GetBinError(b+minbinzoom));
  }
  Canvas.Clear();
  HDstarPiBkgSubZoom.SetStats(0);
  HDstarPiBkgSubZom.GetYaxis()->SetRangeUser(-1000,1.1*HDstarPiBkgSubZoom.GetMaximum());
  HDstarPiBkgSubZoom.Draw("pe");
  Func->Draw("same");
  stats->Draw("same");
  fbkg.Draw("same");
  fsig1.Draw("same");
  fsig2.Draw("same");
  fsig3.Draw("same");
  fsig4.Draw("same"); 
  normline.DrawLine(minzoom,0,maxzoom,0);
  Canvas.Print(filename);
  
  */
