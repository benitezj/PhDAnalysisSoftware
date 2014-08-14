//##########################################################################
//RooLogon for DRecoilToolsUser
//##########################################################################

{

  gROOT->Reset();

  //------------
  //RooFit
  //------------
  gSystem->Load("libTreePlayer");//needed by libDRecoilToolsUser
  gSystem->Load("libPhysics");  
  gSystem->Load("libRooFit");
  gSystem->Load("libDRecoilToolsUser");
  using namespace RooFit ;
 

  TStyle *myStyle= new TStyle("myStyle","myStyle");
  
  // use plain black on white colors
  myStyle->SetFrameBorderMode(0);
  myStyle->SetCanvasBorderMode(0);
  myStyle->SetPadBorderMode(0);
  myStyle->SetPadColor(0);
  myStyle->SetCanvasColor(0);
  //myStyle->SetTitleColor(0);
  myStyle->SetTitleFillColor(0);

  // set the paper & margin sizes
  myStyle->SetPaperSize(35,30);//8.5"x 11.75" = (21.6,29.6)
  myStyle->SetPadTopMargin(.16);//.15
  myStyle->SetPadRightMargin(.21);//.12 
  myStyle->SetPadBottomMargin(.15);//.16
  myStyle->SetPadLeftMargin(0.12);//.114

  // use large Times-Roman fonts
  myStyle->SetTitleBorderSize(0);
  myStyle->SetTitleFont(132,"xyz");  // set the all 3 axes title font
  myStyle->SetTitleFont(132," ");    // set the pad title font
  myStyle->SetTitleSize(0.055,"xyz"); // set the 3 axes title size
  myStyle->SetTitleSize(0.08," ");   // set the pad title size
  myStyle->SetTitleOffset(1,"Y");
  myStyle->SetTitleOffset(1,"X");
  myStyle->SetLabelFont(132,"xyz");
  myStyle->SetLabelSize(0.042,"xyz");
  myStyle->SetTextFont(132);
  myStyle->SetTextSize(0.03);
 

  // use bold lines and markers
  myStyle->SetMarkerStyle(8);
  myStyle->SetMarkerSize(.8);// use smaller markers in a histogram with many bins
  myStyle->SetHistLineWidth(1.85);
  myStyle->SetLineStyleString(2,"[12 12]"); // postscript dashes


  //..Get rid of X error 
  myStyle->SetErrorX(0.001);

  // do not display any of the standard histogram decorations
  myStyle->SetOptTitle(1);
  
  //put tick marks on top and RHS of plots
  myStyle->SetPadTickX(1);
  myStyle->SetPadTickY(1);
  //myStyle->SetNdivisions(10,"y");    
   
  

  myStyle->SetNumberContours(50);  
  myStyle->SetPalette(1,0);
  
  
  myStyle->SetOptFit(1);
  myStyle->SetFuncColor(2);
  myStyle->SetFuncWidth(2);

  //  myStyle->SetOptStat(1111111);  
  myStyle->SetOptStat("mri");  
  myStyle->SetStatFormat("8.6g"); 
  myStyle->SetStatStyle(0);
  myStyle->SetStatColor(0);
  myStyle->SetStatBorderSize(0);
  myStyle->SetStatFont(2);
  myStyle->SetStatFontSize(.035);
  myStyle->SetStatX(.78);
  myStyle->SetStatY(.82); 
  myStyle->SetStatW(.15);
  myStyle->SetStatH(.2);

  myStyle->SetHistFillColor(0);
  //myStyle->SetHistFillStyle(0);//this affects bar histograms 
  myStyle->SetHistLineWidth(2); 
  myStyle->SetHistLineStyle(1); 
  myStyle->SetHistLineColor(1); 
  
  myStyle->SetHatchesSpacing(3.0);
  myStyle->SetHatchesLineWidth(.5);


  myStyle->SetFrameFillStyle(4001);
  myStyle->SetFrameFillColor(0);
  myStyle->SetFillStyle(4001);
 
  myStyle->SetCanvasDefW(1100/1.5);
  myStyle->SetCanvasDefH(800/1.5);
  
 
  gROOT->SetStyle("myStyle");
 
}
