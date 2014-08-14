
void Load(TString Opt="+"){ 
  gROOT->ProcessLine(TString(".L DsInclusiveAnalysisInit.C")+Opt);
}

