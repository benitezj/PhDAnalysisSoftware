
void Load(){ 
   //Auxiliary Classes
  gROOT->ProcessLine(TString(".L RebinHisto.C+")); 
  gROOT->ProcessLine(TString(".L JResiduals.C+"));
  gROOT->ProcessLine(TString(".L ParameterRead.C+"));

  gROOT->ProcessLine(TString(".L RooNLLVar1D.C+"));
  gROOT->ProcessLine(TString(".L Roo2BodyMassPhaseSpace.C+")); 
  gROOT->ProcessLine(TString(".L RooRelBreitWigner.C+"));
  gROOT->ProcessLine(TString(".L RooNRBreitWigner.C+"));
  gROOT->ProcessLine(TString(".L RooSwaveBreitWigner.C+"));
  gROOT->ProcessLine(TString(".L RooDwaveBreitWigner.C+"));
  gROOT->ProcessLine(TString(".L RooLwaveBreitWigner.C+"));
  gROOT->ProcessLine(TString(".L RooPolynomialFixEdgeNew.C+"));
  gROOT->ProcessLine(TString(".L RooConvolutionPdf.C+"));
  gROOT->ProcessLine(TString(".L RooChi2VarIntegral.C+"));
  gROOT->ProcessLine(TString(".L RooGenericPdfN.C+"));
  gROOT->ProcessLine(TString(".L RooProdPdfN.C+"));
  gROOT->ProcessLine(TString(".L RooAddPdfN.C+"));
  gROOT->ProcessLine(TString(".L RooPolyExpPdf.C+"));
  gROOT->ProcessLine(TString(".L RooExpPolyPolyPdf.C+"));
  gROOT->ProcessLine(TString(".L RooExpPiecePolyPdf.C+"));
  
  gROOT->ProcessLine(TString(".L MassFitter.C+"));
 
}



