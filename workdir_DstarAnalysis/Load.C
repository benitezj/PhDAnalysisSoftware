
void Load(TString Opt="+"){ 
  
  //globals things
  gROOT->ProcessLine(".L SOFTWARE/Globals.h");
  gROOT->ProcessLine(".L SOFTWARE/AnalysisGlobals.h");

  //Simple Tools
  gROOT->ProcessLine(TString(".L SOFTWARE/FUNCTIONS/SetHisto.C")+Opt);
  gROOT->ProcessLine(TString(".L SOFTWARE/MACROS/TreeChain.C")+Opt);  
  //gROOT->ProcessLine(TString(".L SOFTWARE/FUNCTIONS/DeterminePstarBins.C")+Opt);
  gROOT->ProcessLine(TString(".L SOFTWARE/FUNCTIONS/DetermineFWHM.C")+Opt);
  gROOT->ProcessLine(TString(".L SOFTWARE/FUNCTIONS/MoveStatsBox.C")+Opt);
  //gROOT->ProcessLine(TString(".L SOFTWARE/FUNCTIONS/MakeDalitzContour.C")+Opt);
  //gROOT->ProcessLine(TString(".L SOFTWARE/FUNCTIONS/CalculateIntegral.C")+Opt);
  //gROOT->ProcessLine(TString(".L SOFTWARE/FUNCTIONS/Legendre.C")+Opt);
  //gROOT->ProcessLine(TString(".L SOFTWARE/FUNCTIONS/Project2DHistoLegendre.C")+Opt);
  gROOT->ProcessLine(TString(".L SOFTWARE/FUNCTIONS/RebinHisto.C")+Opt);
  //gROOT->ProcessLine(TString(".L SOFTWARE/FUNCTIONS/Compute2DEfficiency.C")+Opt);

  //Auxiliary Classes
  gROOT->ProcessLine(TString(".L SOFTWARE/CLASSES/JResiduals.C")+Opt);
  gROOT->ProcessLine(TString(".L SOFTWARE/CLASSES/ParameterRead.C")+Opt);

   
  //need to circle around a couple of times
  gROOT->ProcessLine(TString(".L SOFTWARE/CLASSES/RooNLLVar1D.C")+Opt);
  gROOT->ProcessLine(TString(".L SOFTWARE/CLASSES/Roo2BodyPhaseSpace.C")+Opt); 
  gROOT->ProcessLine(TString(".L SOFTWARE/CLASSES/Roo2BodyMassPhaseSpace.C")+Opt); 
  gROOT->ProcessLine(TString(".L SOFTWARE/CLASSES/RooRelBreitWigner.C")+Opt);
  gROOT->ProcessLine(TString(".L SOFTWARE/CLASSES/RooNRBreitWigner.C")+Opt);
  gROOT->ProcessLine(TString(".L SOFTWARE/CLASSES/RooSwaveBreitWigner.C")+Opt);
  gROOT->ProcessLine(TString(".L SOFTWARE/CLASSES/RooDwaveBreitWigner.C")+Opt);
  gROOT->ProcessLine(TString(".L SOFTWARE/CLASSES/RooLwaveBreitWigner.C")+Opt);
  gROOT->ProcessLine(TString(".L SOFTWARE/CLASSES/RooPolynomialFixEdge.C")+Opt);
  gROOT->ProcessLine(TString(".L SOFTWARE/CLASSES/RooPolynomialFixEdgeNew.C")+Opt);
  gROOT->ProcessLine(TString(".L SOFTWARE/CLASSES/RooConvolutionPdf.C")+Opt);
  gROOT->ProcessLine(TString(".L SOFTWARE/CLASSES/RooChi2VarIntegral.C")+Opt);
  gROOT->ProcessLine(TString(".L SOFTWARE/CLASSES/RooGenericPdfN.C")+Opt);
  gROOT->ProcessLine(TString(".L SOFTWARE/CLASSES/RooProdPdfN.C")+Opt);
  gROOT->ProcessLine(TString(".L SOFTWARE/CLASSES/RooAddPdfN.C")+Opt);
  gROOT->ProcessLine(TString(".L SOFTWARE/CLASSES/RooPolyExpPdf.C")+Opt);
  gROOT->ProcessLine(TString(".L SOFTWARE/CLASSES/RooExpPolyPolyPdf.C")+Opt);
  gROOT->ProcessLine(TString(".L SOFTWARE/CLASSES/RooExpPiecePolyPdf.C")+Opt);
  gROOT->ProcessLine(TString(".L SOFTWARE/CLASSES/RooResolutionPdf.C")+Opt);
  //gROOT->ProcessLine(TString(".L SOFTWARE/CLASSES/RooChi2VarSimultaneous.C")+Opt); 

  
  gROOT->ProcessLine(TString(".L SOFTWARE/CLASSES/MassFitter.C")+Opt);
  //gROOT->ProcessLine(TString(".L SOFTWARE/CLASSES/FragmentationFitter.C")+Opt);
  //gROOT->ProcessLine(TString(".L SOFTWARE/CLASSES/RooCutOptimizer.C")+Opt);
  //  gROOT->ProcessLine(TString(".L SOFTWARE/CLASSES/RooTagCutOptimizer.C")+Opt);

 
  //Tools that depend on the Classes
  gROOT->ProcessLine(TString(".L SOFTWARE/FUNCTIONS/Fits.C")+Opt);

  //Analysis
  //gROOT->ProcessLine(TString(".L SOFTWARE/CLASSES/AbsDataBase.C")+Opt);
  gROOT->ProcessLine(TString(".L SOFTWARE/CLASSES/AbsAnalysis.C")+Opt);
 

  ////DC Analysis
  //gROOT->ProcessLine(TString(".L SOFTWARE/CLASSES/DC/DataBaseDC.C")+Opt);
  gROOT->ProcessLine(TString(".L SOFTWARE/CLASSES/DC/DCAnalysis.C")+Opt);

  ////D0 Analysis
  //gROOT->ProcessLine(TString(".L SOFTWARE/CLASSES/D0/DataBaseD0.C")+Opt);
  gROOT->ProcessLine(TString(".L SOFTWARE/CLASSES/D0/D0Analysis.C")+Opt);

  ////Dstar Analysis
  gROOT->ProcessLine(TString(".L SOFTWARE/CLASSES/Dstar/DstAnalysis.C")+Opt);
  //  gROOT->ProcessLine(TString(".L SOFTWARE/CLASSES/Dstar0/Dst0Analysis.C")+Opt);

  ////DCPi Analysis
  //gROOT->ProcessLine(TString(".L SOFTWARE/CLASSES/DCPi/DataBaseDCPi.C")+Opt);
  gROOT->ProcessLine(TString(".L SOFTWARE/CLASSES/DCPi/DCPiAnalysis.C")+Opt);
  gROOT->ProcessLine(TString(".L SOFTWARE/CLASSES/DCPi/DCPiAnalysisControl.C")+Opt);

  ////D0Pi Analysis
  gROOT->ProcessLine(TString(".L SOFTWARE/CLASSES/D0Pi/D0PiAnalysis.C")+Opt);
  gROOT->ProcessLine(TString(".L SOFTWARE/CLASSES/D0Pi/D0PiAnalysisControl.C")+Opt);


  ////DstPi Analysis
  //gROOT->ProcessLine(TString(".L SOFTWARE/CLASSES/DstPi/JDataBaseDstPi.C")+Opt);
  gROOT->ProcessLine(TString(".L SOFTWARE/CLASSES/DstPi/DstPiAnalysis.C")+Opt);
  gROOT->ProcessLine(TString(".L SOFTWARE/CLASSES/DstPi/DstPiAnalysisControl.C")+Opt);
  //gROOT->ProcessLine(TString(".L SOFTWARE/CLASSES/DstPi/runDstPiFits.C")+Opt);
  //gROOT->ProcessLine(TString(".L SOFTWARE/CLASSES/DstPi/DstPiPlotFitParameters.C")+Opt);
  //gROOT->ProcessLine(TString(".L SOFTWARE/CLASSES/DstPi/YieldVsHelicity.C")+Opt);

  ////DstPi0 Analysis
  //gROOT->ProcessLine(TString(".L SOFTWARE/CLASSES/DstPi0/DstPi0Analysis.C")+Opt);
  //gROOT->ProcessLine(TString(".L SOFTWARE/CLASSES/DstPi0/DstPi0AnalysisControl.C")+Opt);


  ///////simultaneous fit to DCPi and DstPi
  //gROOT->ProcessLine(TString(".L SOFTWARE/MACROS/DstPi_DPi_SimultaneousFit.C")+Opt);

}



