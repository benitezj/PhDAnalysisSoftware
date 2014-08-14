#include "../Globals.h"
#include "../AnalysisGlobals.h"
#include "../FUNCTIONS/Fits.h"


////a function that convolutes the breight wigner of the Ds1 and Dstar and mayb Ks in order to determine the intrinsic width of the deltaM(Ds1)
void determineDs1DMPDF(){
  
  TCanvas Canvas("Canvas","Canvas");
  TString fname="DetermineDMPDF.ps";
  Canvas.Print(fname+"[");
 
  
  Int_t ndm1pdfbins=100;
  Float_t ds1width=.001;   
  Int_t nsigma=10;
  TF1 FBWDs1("FBWDs1","[0]*([2]/(2*3.14159))/((x-[1])**2+.25*([2]**2))",
	     Ds1pPDGMass-nsigma*ds1width,Ds1pPDGMass+nsigma*ds1width);  
  FBWDs1.SetParameters(1,Ds1pPDGMass,ds1width);
 
  TH1F HBWDs1("HBWDs1","Ds1 Distribution",ndm1pdfbins,Ds1pPDGMass-nsigma*ds1width,Ds1pPDGMass+nsigma*ds1width);
  HBWDs1.FillRandom("FBWDs1",100000);
  FitHistogramBreitWigner(&HBWDs1,Ds1pPDGMass-nsigma*ds1width,Ds1pPDGMass+nsigma*ds1width);
  Canvas.Clear();
  HBWDs1.Draw();
  Canvas.Print(fname);
 
  Float_t dstarwidth=9.6e-5;
  TF1 FBWDstar("FBWDstar","[0]*([2]/(2*3.14159))/((x-[1])**2+.25*([2]**2))",
	       DstarPDGMass-nsigma*dstarwidth,DstarPDGMass+nsigma*dstarwidth);  
  FBWDstar.SetParameters(1,DstarPDGMass,dstarwidth);
  
 
  TH1F HBWDstar("HBWDstar","Dstar Distribution",ndm1pdfbins,DstarPDGMass-nsigma*dstarwidth,DstarPDGMass+nsigma*dstarwidth);
  HBWDstar.FillRandom("FBWDstar",100000);
  FitHistogramBreitWigner(&HBWDstar,DstarPDGMass-nsigma*dstarwidth,DstarPDGMass+nsigma*dstarwidth);
  Canvas.Clear();
  HBWDstar.GetXaxis()->SetNdivisions(5);
  HBWDstar.Draw();
  Canvas.Print(fname);




  //The width of dm1=m(Ds1)-m(Dstar) is given by the following convolution
  //PDFDM1(x)=Integral( PDFDs1(y)*PDFDstar(y-x) ) dy from x to infinity  
  //Lets create a Histogram  to store the PDFDM1


  
  Float_t PDGDM1=Ds1pPDGMass-DstarPDGMass;
  TH1F HPDFDM1("HPDFDM1","DM1 distribution",ndm1pdfbins,PDGDM1-nsigma*ds1width,PDGDM1+nsigma*ds1width);
  Float_t dm1pdfvalue;

  Float_t dm1=0;
  Float_t dm1min=PDGDM1-nsigma*ds1width;
  Float_t dm1max=PDGDM1+nsigma*ds1width;
  Float_t dm1step=(dm1max-dm1min)/ndm1pdfbins;

  //lets fill the pdf
  for(Int_t bin=0;bin<ndm1pdfbins;bin++){
    dm1=dm1min+dm1step*bin;

    ///for each value we must perform an integral
    //define the range of integration
    Float_t mds1min=2.526;
    //actually the mininum is determined by the above bin however lets just ingnore that for the moment
    //mds1min=dm1
    Float_t mds1max=2.546;
    Float_t mds1step=.00001;    
    Int_t nds1bins=(int)((mds1max-mds1min)/mds1step);
    Float_t mds1=0;


    dm1pdfvalue=0;
    for(Int_t mds1bin=0;mds1bin<nds1bins;mds1bin++){
      mds1=mds1min+mds1step*mds1bin;
      
      //Now multiply the pdfs
      dm1pdfvalue+=FBWDs1.Eval(mds1)*FBWDstar.Eval(mds1-dm1)*mds1step;
      

    
    }

    
    HPDFDM1.SetBinContent(bin,dm1pdfvalue);

  }


 
  
  Canvas.Clear();
  FitHistogramBreitWigner(&HPDFDM1,PDGDM1-nsigma*ds1width/2,PDGDM1+nsigma*ds1width/2);
  HPDFDM1.Draw();
  Canvas.Print(fname);

  ////////////the following is not working yet

  Float_t kswidth=1e-9;
  TF1 FBWKs("FBWKs","[0]*([2]/(2*3.14159))/((x-[1])**2+.25*([2]**2))",
	    K0PDGMass-nsigma*kswidth,K0PDGMass+nsigma*kswidth);
  FBWKs.SetParameters(1,K0PDGMass,kswidth);


  TH1F HBWKs("HBWKs","Ks Distribution",ndm1pdfbins,K0PDGMass-nsigma*kswidth,K0PDGMass+nsigma*kswidth);
  HBWKs.FillRandom("FBWKs",100000);
  FitHistogramBreitWigner(&HBWKs,K0PDGMass-nsigma*kswidth,K0PDGMass+nsigma*kswidth);
  Canvas.Clear();
  HBWKs.GetXaxis()->SetNdivisions(5);
  HBWKs.Draw();
  Canvas.Print(fname);



  //The width of dm2=m(Ds1)-m(Dstar)-m(KS) is given by the following convolution
  //PDFDM1(z)=Integral( Integral( PDFDs1(y)*PDFDstar(y-x))dy * PDFKs(x-z) )dz 
  //the limits should be y= x -> inf  and z=y->inf
  //Lets create a Histogram  to store the PDFDM2


  Int_t ndm2pdfbins=ndm1pdfbins;
  Float_t PDGDM2=Ds1pPDGMass-DstarPDGMass-K0PDGMass;
  TH1F HPDFDM2("HPDFDM2","DM2 distribution",ndm2pdfbins,PDGDM2-nsigma*ds1width,PDGDM2+nsigma*ds1width);
  Float_t dm2pdfvalue;

  Float_t dm2=0;
  Float_t dm2min=PDGDM2-nsigma*ds1width/2;
  Float_t dm2max=PDGDM2+nsigma*ds1width/2;
  Float_t dm2step=(dm2max-dm2min)/ndm2pdfbins;

  //lets fill the pdf
  for(Int_t bin=0;bin<ndm2pdfbins;bin++){
    dm2=dm2min+dm2step*bin;
    dm2pdfvalue=0;
       
    ///for each value we must perform two integrals integral
   
    //do the dm1 integral
    Float_t mdm1min=.516;
    //actually the mininum is determined by the above bin however lets just ingnore that for the moment
    //mdm1min=dm1
    Float_t mdm1max=.536;
    Float_t mdm1step=.0001;    
    Int_t ndm1bins=(int)((mdm1max-mdm1min)/mdm1step);
    Float_t mdm1=0;   
    for(Int_t mdm1bin=0;mdm1bin<ndm1bins;mdm1bin++){
      mdm1=mdm1min+mdm1step*mdm1bin;    
      dm1pdfvalue=0;

      ///do the Ds1 integral
      //define the range of integration
      Float_t mds1min=2.526;
      //actually the mininum is determined by the above bin however lets just ingnore that for the moment
      //mds1min=dm2
      Float_t mds1max=2.546;
      Float_t mds1step=.0001;    
      Int_t nds1bins=(int)((mds1max-mds1min)/mds1step);
      Float_t mds1=0;      
      for(Int_t mds1bin=0;mds1bin<nds1bins;mds1bin++){
	mds1=mds1min+mds1step*mds1bin;      
	//Now multiply the pdfs
	dm1pdfvalue+=FBWDs1.Eval(mds1)*FBWDstar.Eval(mds1-mdm1)*mds1step;         
      }

     
      //Now multiply the pdfs
      dm2pdfvalue+=dm1pdfvalue*FBWKs.Eval(mdm1-dm2)*mdm1step;         
    }
    
    
    HPDFDM2.SetBinContent(bin,dm2pdfvalue);

  }


 
  Canvas.Clear();
  FitHistogramBreitWigner(&HPDFDM2,PDGDM2-nsigma*ds1width/2,PDGDM2+nsigma*ds1width/2);
  Canvas.Print(fname);




  Canvas.Print(fname+"]");
}
