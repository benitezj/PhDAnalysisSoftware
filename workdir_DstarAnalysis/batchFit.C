{

gROOT->ProcessLine(".x SOFTWARE/RooLogon.C");
gROOT->ProcessLine(".x SOFTWARE/Load.C");
DCPiAnalysis* a=analyzeDCPi(1,0);
a->FitMass("HDCPi");

}
