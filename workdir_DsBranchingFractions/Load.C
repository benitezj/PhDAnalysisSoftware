
void Load(TString Opt="+"){ 
  //gROOT->ProcessLine(TString(".L Macros/TagAnalysisInit.C")+Opt);
  gROOT->ProcessLine(TString(".L Macros/DsInclusiveAnalysisInit.C")+Opt);
  gROOT->ProcessLine(TString(".L Macros/DsToKKpiAnalysisInit.C")+Opt);
}

