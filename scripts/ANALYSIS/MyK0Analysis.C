#include "../Globals.h"
#include "../MACROS/MyK0Chain.C"
#include "../FUNCTIONS/Fits.h"
#include "../FUNCTIONS/SetHisto.h"
#include <TLine.h>

TH1F* AnalyzeK0(Int_t firstfile, Int_t lastfile,Int_t DATAorMC){

  Float_t K0massWindow=.03;
  Int_t NmassBins=62*4;
  Float_t K0PDGmass=.4976448;
  
  //define cuts: cuts depend on order applied
  Double_t finalmassCut=K0massWindow;
  Double_t finalmassTightCut=.01;
  Double_t K0Sep3dCut=.3;
  Double_t K0RadiusCut=.5;//3.5;//.2;
  Double_t K0ZcollCut=3;//12;//2.2;   
  Double_t K0ZcollMean=.062;
  Double_t K0SfL3dCut=.05;//.1;//choose positive signed lengths
  Double_t dchconsis1Cut=.001;
  Double_t dchconsis2Cut=.0001;
  Double_t drcconsis1Cut=1e-15;
  Double_t drcconsis2Cut=1e-15; 
  

  ///create mass peak histograms
  TH1F HMassPeakInitial;SetHisto(&HMassPeakInitial,"K_{S} Mass",NmassBins,-K0massWindow-.001,K0massWindow+.001,"#pi^{+}-#pi^{-} Mass - K_{S} PDG Mass (GeV)");
  HMassPeakInitial.SetLineColor(1);
  TH1F HMassPeakVertex;SetHisto(&HMassPeakVertex,"K_{S} Mass",NmassBins,-K0massWindow-.001,K0massWindow+.001,"#pi^{+}-#pi^{-} Mass - K_{S} PDG Mass (GeV)");
  HMassPeakVertex.SetLineColor(2);
  TH1F HMassPeakPiPiDist;SetHisto(&HMassPeakPiPiDist,"K_{S} Mass",NmassBins,-K0massWindow-.001,K0massWindow+.001,"#pi^{+}-#pi^{-} Mass - K_{S} PDG Mass (GeV)");
  HMassPeakPiPiDist.SetLineColor(3);
  TH1F HMassPeakRadius;SetHisto(&HMassPeakRadius,"K_{S} Mass",NmassBins,-K0massWindow-.001,K0massWindow+.001,"#pi^{+}-#pi^{-} Mass - K_{S} PDG Mass (GeV)");
  HMassPeakRadius.SetLineColor(4);
  TH1F HMassPeakZcoll;SetHisto(&HMassPeakZcoll,"K_{S} Mass",NmassBins,-K0massWindow-.001,K0massWindow+.001,"#pi^{+}-#pi^{-} Mass - K_{S} PDG Mass (GeV)");
  HMassPeakZcoll.SetLineColor(5);
  TH1F HMassPeakSfL3d;SetHisto(&HMassPeakSfL3d,"K_{S} Mass",NmassBins,-K0massWindow-.001,K0massWindow+.001,"#pi^{+}-#pi^{-} Mass - K_{S} PDG Mass (GeV)");
  HMassPeakSfL3d.SetLineColor(6);
  TH1F HMassPeakDCHPID;SetHisto(&HMassPeakDCHPID,"K_{S} Mass",NmassBins,-K0massWindow-.001,K0massWindow+.001,"#pi^{+}-#pi^{-} Mass - K_{S} PDG Mass (GeV)");
  HMassPeakDCHPID.SetLineColor(7);
  TH1F HMassPeakDRCPID;SetHisto(&HMassPeakDRCPID,"K_{S} Mass",NmassBins,-K0massWindow-.001,K0massWindow+.001,"#pi^{+}-#pi^{-} Mass - K_{S} PDG Mass (GeV)");
  HMassPeakDRCPID.SetLineColor(8);
  TH1F HMassPeakChisQ;SetHisto(&HMassPeakChisQ,"K_{S} Mass",NmassBins,-K0massWindow-.001,K0massWindow+.001,"#pi^{+}-#pi^{-} Mass - K_{S} PDG Mass (GeV)");
  HMassPeakChisQ.SetLineColor(10);
  TH1F HMassPeakFinal;SetHisto(&HMassPeakFinal,"K_{S} Mass",62*4,-K0massWindow-.001,K0massWindow+.001,"#pi^{+}-#pi^{-} Mass - K_{S} PDG Mass (GeV)");

  //Background Removed
  TH1F HBackgroundTotal;SetHisto(&HBackgroundTotal,"Background Total",62*4,-K0massWindow-.001,K0massWindow+.001,"Mass (GeV)");	
  HBackgroundTotal.SetLineColor(1);	  
  TH1F HBackgroundPiPiDist;SetHisto(&HBackgroundPiPiDist,"Background PiPiDist",NmassBins,-K0massWindow-.001,K0massWindow+.001,"#pi^{+}-#pi^{-} Mass - K_{S} PDG Mass (GeV)");
  HBackgroundPiPiDist.SetLineColor(3);
  TH1F HBackgroundRadius;SetHisto(&HBackgroundRadius,"Background Radius",NmassBins,-K0massWindow-.001,K0massWindow+.001,"#pi^{+}-#pi^{-} Mass - K_{S} PDG Mass (GeV)");
  HBackgroundRadius.SetLineColor(4);
  TH1F HBackgroundZcoll;SetHisto(&HBackgroundZcoll,"Background Z",NmassBins,-K0massWindow-.001,K0massWindow+.001,"#pi^{+}-#pi^{-} Mass - K_{S} PDG Mass (GeV)");
  HBackgroundZcoll.SetLineColor(5);
  TH1F HBackgroundSfL3d;SetHisto(&HBackgroundSfL3d,"Background SfL",NmassBins,-K0massWindow-.001,K0massWindow+.001,"#pi^{+}-#pi^{-} Mass - K_{S} PDG Mass (GeV)");
  HBackgroundSfL3d.SetLineColor(6);
  TH1F HBackgroundDCHPID;SetHisto(&HBackgroundDCHPID,"Background DCH PID",NmassBins,-K0massWindow-.001,K0massWindow+.001,"#pi^{+}-#pi^{-} Mass - K_{S} PDG Mass (GeV)");
  HBackgroundDCHPID.SetLineColor(7);
  TH1F HBackgroundDRCPID;SetHisto(&HBackgroundDRCPID,"Background DIRC PID",NmassBins,-K0massWindow-.001,K0massWindow+.001,"#pi^{+}-#pi^{-} Mass - K_{S} PDG Mass (GeV)");
  HBackgroundDRCPID.SetLineColor(8);
  TH1F HBackgroundChisQ;SetHisto(&HBackgroundChisQ,"Background Vertex #chi^{2}",NmassBins,-K0massWindow-.001,K0massWindow+.001,"#pi^{+}-#pi^{-}Mass - K_{S} PDG Mass (GeV)");
  HBackgroundChisQ.SetLineColor(10);


  //create properTime  histograms
  Float_t properTimeWindow=600;
  Int_t NproperTimeBins=100;
  TH1F HProperTime;SetHisto(&HProperTime,"K_{S}  Decay Time",NproperTimeBins,0,properTimeWindow,"Proper Decay Time (ps)");
  TH1F HProperTimeBkg1;SetHisto(&HProperTimeBkg1,"K_{S}  Proper Decay Time",NproperTimeBins,0,properTimeWindow,"Proper Decay Time (ps)");
  HProperTimeBkg1.SetLineColor(3);
  TH1F HProperTimeBkg2;SetHisto(&HProperTimeBkg2,"K_{S}  Proper Decay Time",NproperTimeBins,0,properTimeWindow,"Proper Decay Time (ps)");
  HProperTimeBkg2.SetLineColor(4);

  //create cut variable histograms
  TH1F HPiPiDist;SetHisto(&HPiPiDist,"#pi^{-}-#pi^{+} distance",100,0,10,"distance (cm)");
  TH1F HRadius;SetHisto(&HRadius,"Production Radius of K_{S} from Beam Spot ",100,0,10,"radius (cm)");
  TH1F HZcoll;SetHisto(&HZcoll,"Production Z of K_{S} from Beam Spot",100,-10,10,"z (cm)");
  TH1F HSfL3d;SetHisto(&HSfL3d,"Signed Flight Length of K_{S}",100,-10,10,"length (cm)");
  TH1F HDCHPID;SetHisto(&HDCHPID,"DCH Pion Consistency (one pion)",110,0,1.05,"probability");
  TH1F HDRCPID;SetHisto(&HDRCPID,"DRC Pion Consistency (one pion)",110,0,1.05,"probability");
  
  ///misc distributions
  TH1F HK0Momentum;SetHisto(&HK0Momentum,"K_{S} Momentum",100,0,5,"p (GeV)");
  TH1F HCosHel;SetHisto(&HCosHel,"K_{S} Helicity",120,-1.05,1.05,"cosine(helicity)");
  TH1F HCosHelBkg1;SetHisto(&HCosHelBkg1,"Background Helicity",120,-1.05,1.05,"cosine(helicity)");  
  HCosHelBkg1.SetLineColor(2);
  TH1F HCosHelBkg2;SetHisto(&HCosHelBkg2,"Background Helicity",120,-1.05,1.05,"cosine(helicity)");
  HCosHelBkg2.SetLineColor(3);
  TH2F H2MassVsCosHel;SetHisto2D(&H2MassVsCosHel,"K_{S} Mass vs. Helicity",
				 120,-1.05,1.05,"cosine(helicity)",
				 NmassBins,-finalmassTightCut,finalmassTightCut,
				 "#pi^{+}-#pi^{-}Mass - K_{S} PDG Mass (GeV)",
				 "Entries/");
 

  //Initialize variables
  Double_t initialmass=0.;
  Double_t finalmass=0.; 
  Double_t K0Sep3d=0.;
  Double_t K0Zcoll=0.;
  Double_t K0Radius=0.;
  Double_t K0SfL3d=0.;
  Double_t chisq=0.;
  Double_t probab=0.;
  Double_t dchconsis1=0.;
  Double_t dchconsis2=0.;
  Double_t drcconsis1=0.;
  Double_t drcconsis2=0.;
  Double_t p=0.;
  Double_t pt=0.; 
  Double_t coshel=0.;
  

  

  /*
  //tight cuts
  Double_t finalmassCut=K0massWindow;
  Double_t finalmassTightCut=.01;
  Double_t K0Sep3dCut=.2;
  Double_t K0RadiusCut=.3;//3.5;//.2;
  Double_t K0ZcollCut=1.5;//12;//2.2;   
  Double_t K0ZcollMean=.062;
  Double_t K0SfL3dCut=.2;//.1;//choose positive signed lengths
  Double_t dchconsis1Cut=.005;
  Double_t dchconsis2Cut=.005;
  Double_t drcconsis1Cut=1e-15;
  Double_t chisqCut=150.;//7.;
  Double_t probabCut=1e-25;
  */

  TChain* ntuple=MyK0Chain(firstfile,lastfile,DATAorMC);
  ntuple->SetBranchStatus("*",0);
  
  ntuple->SetBranchAddress("initialmass",&initialmass);
  ntuple->SetBranchStatus("initialmass",1);
  ntuple->SetBranchAddress("finalmass",&finalmass);
  ntuple->SetBranchStatus("finalmass",1);
  ntuple->SetBranchAddress("K0Sep3d",&K0Sep3d);
  ntuple->SetBranchStatus("K0Sep3d",1);
  ntuple->SetBranchAddress("K0Zcoll",&K0Zcoll);
  ntuple->SetBranchStatus("K0Zcoll",1);
  ntuple->SetBranchAddress("K0Radius",&K0Radius);
  ntuple->SetBranchStatus("K0Radius",1);
  ntuple->SetBranchAddress("K0SfL3d",&K0SfL3d);
  ntuple->SetBranchStatus("K0SfL3d",1);
  ntuple->SetBranchAddress("chisq",&chisq);
  ntuple->SetBranchStatus("chisq",1);
  ntuple->SetBranchAddress("probab",&probab);
  ntuple->SetBranchStatus("probab",1);
  ntuple->SetBranchAddress("dchconsis1",&dchconsis1);
  ntuple->SetBranchStatus("dchconsis1",1);
  ntuple->SetBranchAddress("dchconsis2",&dchconsis2);
  ntuple->SetBranchStatus("dchconsis2",1);
  ntuple->SetBranchAddress("drcconsis1",&drcconsis1);
  ntuple->SetBranchStatus("drcconsis1",1);
  ntuple->SetBranchAddress("drcconsis2",&drcconsis2);
  ntuple->SetBranchStatus("drcconsis2",1);
  ntuple->SetBranchAddress("p",&p);
  ntuple->SetBranchStatus("p",1);
  ntuple->SetBranchAddress("pt",&pt);
  ntuple->SetBranchStatus("pt",1);
  ntuple->SetBranchAddress("coshel",&coshel);
  ntuple->SetBranchStatus("coshel",1);


  Double_t speed=1;
  Double_t properTime2=0;
  Double_t gamma=0.;
  Bool_t IsBackground=1;

  //Start the event loop;
  Int_t eventid=0;
  while(ntuple->GetEntry(eventid,0)>0){   
    IsBackground=1;

    HMassPeakInitial.Fill(initialmass-K0PDGmass);    
    if(fabs(finalmass-K0PDGmass)<finalmassCut){        
      HMassPeakVertex.Fill(finalmass-K0PDGmass);
      if(K0Sep3d<K0Sep3dCut){
	HMassPeakPiPiDist.Fill(finalmass-K0PDGmass);	  	 
	if(K0Radius<K0RadiusCut){
	  HMassPeakRadius.Fill(finalmass-K0PDGmass);
	  if(fabs(K0Zcoll-K0ZcollMean)<K0ZcollCut){
	    HMassPeakZcoll.Fill(finalmass-K0PDGmass);
	    if(K0SfL3d>K0SfL3dCut){
	      HMassPeakSfL3d.Fill(finalmass-K0PDGmass);
	      //if(dchconsis1>dchconsis1Cut&&dchconsis2>dchconsis2Cut){
		HMassPeakDCHPID.Fill(finalmass-K0PDGmass);
		//if(drcconsis1>drcconsis1Cut&&drcconsis2>drcconsis2Cut){
		  HMassPeakDRCPID.Fill(finalmass-K0PDGmass);
				 
		  //
		  speed=myc*p/K0PDGmass;
		  properTime2=1e12*(K0SfL3d/100.)/speed;
		   
		  ///Alternate propertimes
		  /*
		  if(WhichProperTime==1){
		    speed=myc/sqrt(1 + (K0PDGmass*K0PDGmass)/(p*p));
		    gamma=1/sqrt(1+speed*speed/(myc*myc));
		    properTime2=1e12*((K0SfL3d/100.)/speed)/gamma;
		  }
		  if(WhichProperTime==2){
		    speed=myc/sqrt(1 + (K0PDGmass*K0PDGmass)/(p*p));
		    properTime2=1e12*(K0SfL3d/100.)/speed;
		  }
		  */

		  if(fabs(finalmass-K0PDGmass+finalmassTightCut*2)<finalmassTightCut){
		    HProperTimeBkg1.Fill(properTime2); 
		    HCosHelBkg1.Fill(coshel);
		  }
		  if(fabs(finalmass-K0PDGmass-finalmassTightCut*2)<finalmassTightCut){
		    HProperTimeBkg2.Fill(properTime2);
		    HCosHelBkg2.Fill(coshel);
		  }
		  
		  if(fabs(finalmass-K0PDGmass)<finalmassTightCut){   
		    HMassPeakFinal.Fill(finalmass-K0PDGmass);		      		      
		    HProperTime.Fill(properTime2);		      	   		    
		    HK0Momentum.Fill(p);
		    HCosHel.Fill(coshel);
		    H2MassVsCosHel.Fill(coshel,finalmass-K0PDGmass);
		  }

		  IsBackground=0;
		 
		  //}else HBackgroundDRCPID.Fill(finalmass-K0PDGmass);
		  //}else HBackgroundDCHPID.Fill(finalmass-K0PDGmass);
	    }else HBackgroundSfL3d.Fill(finalmass-K0PDGmass);
	  }else HBackgroundZcoll.Fill(finalmass-K0PDGmass);
	}else HBackgroundRadius.Fill(finalmass-K0PDGmass);
      }else HBackgroundPiPiDist.Fill(finalmass-K0PDGmass);

      if(IsBackground)HBackgroundTotal.Fill(finalmass-K0PDGmass);	
    }

    HPiPiDist.Fill(K0Sep3d);
    HRadius.Fill(K0Radius);
    HZcoll.Fill(K0Zcoll);
    HSfL3d.Fill(K0SfL3d);
    HDCHPID.Fill(dchconsis1);
    HDRCPID.Fill(drcconsis1);
  

    eventid++;
    if(eventid%30000==0)cout<<eventid<<" Events done."<<endl;        
  }


  TCanvas C("C","K0Analysis");
  TString filename;
  if(DATAorMC==1)filename="K0AnalysisDATA.ps";
  if(DATAorMC==2)filename="K0AnalysisMC.ps";
  C.Print(filename+"[");


  C.Clear();
  HMassPeakVertex.Draw();
  HMassPeakInitial.Draw("same");    
  C.Print(filename);
 

  ///save the discriminating variables
  TLine cutline;
  cutline.SetLineColor(2);
  C.SetLogy(1);
  C.Clear();
  HPiPiDist.Draw();
  cutline.DrawLine(K0Sep3dCut,0,K0Sep3dCut,HPiPiDist.GetMaximum());
  C.Print(filename);
  C.Clear();
  HRadius.Draw();
  cutline.DrawLine(K0RadiusCut,0,K0RadiusCut,HRadius.GetMaximum());
  C.Print(filename);
  C.Clear();
  HZcoll.Draw();
  cutline.DrawLine(K0ZcollMean+K0ZcollCut,0,K0ZcollMean+K0ZcollCut,HZcoll.GetMaximum());
  cutline.DrawLine(K0ZcollMean-K0ZcollCut,0,K0ZcollMean-K0ZcollCut,HZcoll.GetMaximum());
  C.Print(filename);
  C.Clear();
  C.SetLogy(0);
  HZcoll.Draw();
  cutline.DrawLine(K0ZcollMean+K0ZcollCut,0,K0ZcollMean+K0ZcollCut,HZcoll.GetMaximum());
  cutline.DrawLine(K0ZcollMean-K0ZcollCut,0,K0ZcollMean-K0ZcollCut,HZcoll.GetMaximum());
  C.Print(filename);
  C.SetLogy(1);
  C.Clear();
  HSfL3d.Draw();
  cutline.DrawLine(K0SfL3dCut,0,K0SfL3dCut,HSfL3d.GetMaximum());
  C.Print(filename);
  C.Clear();
  HDCHPID.Draw();
  cutline.DrawLine(dchconsis1Cut,0,dchconsis1Cut,HDCHPID.GetMaximum());
  C.Print(filename);
  C.Clear();
  HDRCPID.Draw();
  cutline.DrawLine(drcconsis1Cut,0,drcconsis1Cut,HDRCPID.GetMaximum());
  C.Print(filename);
  C.SetLogy(0);

  const Int_t nhistos=7;
  TH1F* HMassPeak[nhistos];
  HMassPeak[0]=&HMassPeakVertex;
  HMassPeak[1]=&HMassPeakPiPiDist;
  HMassPeak[2]=&HMassPeakRadius;
  HMassPeak[3]=&HMassPeakZcoll;
  HMassPeak[4]=&HMassPeakSfL3d;
  HMassPeak[5]=&HMassPeakDCHPID;
  HMassPeak[6]=&HMassPeakDRCPID;
 
  
  C.Clear();
  HMassPeakVertex.Draw();
  for(  Int_t i=0;i<nhistos;i++){
    HMassPeak[i]->Draw("same");
  }
  C.Print(filename);



  //backgrounds removed
  TH1F* HBackground[nhistos];
  HBackground[0]=&HBackgroundTotal;
  HBackground[1]=&HBackgroundPiPiDist;
  HBackground[2]=&HBackgroundRadius;
  HBackground[3]=&HBackgroundZcoll;
  HBackground[4]=&HBackgroundSfL3d;
  HBackground[5]=&HBackgroundDCHPID;
  HBackground[6]=&HBackgroundDRCPID;
  
	
  C.Clear();
  if(nhistos%2==0)C.Divide(2,nhistos/2);
  if(nhistos%2==1)C.Divide(2,nhistos/2 + 1);
  for(  Int_t i=0;i<nhistos;i++){
    C.cd(i+1);    
    HBackground[i]->Draw();
  }
  C.Print(filename);


  ///Proper Time 
  TH1F HProperTimeBkgAdded("HProperTimeBkgAdded","K_{S}  Proper Decay Time",NproperTimeBins,0,properTimeWindow);
  HProperTimeBkgAdded.SetLineColor(2);
  HProperTimeBkgAdded.Add(&HProperTimeBkg1,.5); 
  HProperTimeBkgAdded.Add(&HProperTimeBkg2,.5); 
 
  C.Clear();
  //HMassPeakFinal.Draw();
  HMassPeak[nhistos-1]->Draw();
  cutline.DrawLine(finalmassTightCut,0,finalmassTightCut,HMassPeak[nhistos-1]->GetMaximum());
  cutline.DrawLine(-finalmassTightCut,0,-finalmassTightCut,HMassPeak[nhistos-1]->GetMaximum());
  C.Print(filename);


  //helicity of Kshort
  C.Clear();
  HCosHel.Draw();
  HCosHelBkg1.Draw("same");
  HCosHelBkg2.Draw("same");  
  C.Print(filename); 
  C.Clear();
  H2MassVsCosHel.Draw("col");
  C.Print(filename);

  //misc plots
  C.Clear();
  HK0Momentum.Draw();
  C.Print(filename);   
 

  C.Clear();
  C.SetLogy(1);
  HProperTime.Draw(); 
  C.Print(filename);
  C.Clear(); 
  HProperTime.Draw();   
  HProperTimeBkgAdded.Draw("same"); 
  HProperTimeBkg1.Draw("same");
  HProperTimeBkg2.Draw("same");
  C.Print(filename);
  C.Clear();
  TH1F* HProperTimeSignal=(TH1F*)HProperTime.Clone();
  HProperTimeSignal->Add(&HProperTimeBkgAdded,-1);
  HProperTimeSignal->SetLineColor(3); 

  
  HProperTime.Draw();
  HProperTimeSignal->Draw("same");   
  HProperTimeBkgAdded.Draw("same");
  C.Print(filename);


  
  //fit the decay distribution with an exponential  
  Float_t minproperTime=10;
  Float_t maxproperTime=properTimeWindow;
  FitHistogramExponentialFixRange(HProperTimeSignal,minproperTime,maxproperTime);
  C.Clear();
  HProperTimeSignal->Draw();  
  C.Print(filename);
 

  //Scan the slope of the decay distribution
  Int_t nslopes=6;
  TGraph GSlopes;
  TF1* fslope[nslopes];
  Float_t deltaproperTime=(maxproperTime-minproperTime)/nslopes;
  for(Int_t s=0;s<nslopes;s++){
    fslope[s]=FitHistogramExponentialFixRange(HProperTimeSignal,minproperTime+s*deltaproperTime,minproperTime+(s+1)*deltaproperTime);
    GSlopes.SetPoint(s,minproperTime+(s+.5)*deltaproperTime,-1/fslope[s]->GetParameter(1));
    fslope[s]->SetLineColor(s%2+1);
    fslope[s]->SetLineWidth(2);
  }
  C.Clear();
  HProperTimeSignal->Draw();  
  for(Int_t s=0;s<nslopes;s++){
    fslope[s]->Draw("same");
  }
  C.Print(filename);
  C.SetLogy(0);  

  Float_t K0PDGLifetime=89.53;
  C.Clear();
  GSlopes.SetPoint(nslopes,0.,K0PDGLifetime);
  GSlopes.SetPoint(nslopes+1,0.,0.);
  GSlopes.GetXaxis()->SetTitle("Proper Decay Time (ps)");
  GSlopes.GetYaxis()->SetTitle("Lifetime (ps)");
  GSlopes.SetTitle("Lifetime vs. Proper Decay Time");
  GSlopes.Draw("ap");
  cutline.DrawLine(0,K0PDGLifetime,maxproperTime,K0PDGLifetime);
  C.Print(filename);   




  //efficiency of cuts
  TGraph GCutEff; 
  TF1* f[nhistos];
  for( Int_t i=0;i<nhistos;i++){
    f[i]=FitHistogram1gausPlusPolNFixRange(HMassPeak[i],-K0massWindow,K0massWindow,2);   
    GCutEff.SetPoint(i,(double)i,(f[i]->GetParameter(0)*fabs(f[i]->GetParameter(2)))/(f[0]->GetParameter(0)*fabs(f[0]->GetParameter(2))));    
  }
  GCutEff.SetPoint(nhistos,.0,.0);

  C.Clear();
  HMassPeakVertex.Draw();
  for(  Int_t i=0;i<nhistos;i++){
    HMassPeak[i]->Draw("same");
  }
  C.Print(filename);
  C.Clear();
  GCutEff.SetTitle("Efficiency of Cuts");
  GCutEff.GetXaxis()->SetTitle("Cut Id");
  GCutEff.Draw("AP");
  C.Print(filename);
  

  C.Print(filename+"]");  
  return (TH1F*)HProperTimeSignal; 
}




