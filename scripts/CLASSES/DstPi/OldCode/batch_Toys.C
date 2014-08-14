{
gROOT->ProcessLine(".x SOFTWARE/RooLogon.C");
gROOT->ProcessLine(".x SOFTWARE/Load.C");
DstPiAnalysis* a=analyzeDstPi(0,0);
a->GenerateToySigOnBkg(1,200,0);
}
