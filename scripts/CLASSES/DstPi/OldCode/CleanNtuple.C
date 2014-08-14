{

DstPiAnalysis*a=NULL;
//a=analyzeDstPi(0,0);
//a->CleanNtuple();
//delete a;
gROOT->Clear();
gROOT->Reset();

//a=analyzeDstPi(1,0);
//a->CleanNtuple();
//delete a;
gROOT->Clear();
gROOT->Reset();

a=analyzeDstPi(2,0);
a->CleanNtuple();
delete a;
gROOT->Clear();
gROOT->Reset();

a=analyzeDstPi(3,0);
a->CleanNtuple();
delete a;
gROOT->Clear();
gROOT->Reset();

a=analyzeDstPi(4,0);
a->CleanNtuple();
delete a;
gROOT->Clear();
gROOT->Reset();

a=analyzeDstPi(5,0);
a->CleanNtuple();
delete a;
gROOT->Clear();
gROOT->Reset();

a=analyzeDstPi(6,0);
a->CleanNtuple();
delete a;
gROOT->Clear();
gROOT->Reset();

a=analyzeDstPi(7,0);
a->CleanNtuple();
delete a;
gROOT->Clear();
gROOT->Reset();

a=analyzeDstPi(13,0);
a->CleanNtuple();
delete a;
gROOT->Clear();
gROOT->Reset();

a=analyzeDstPi(14,0);
a->CleanNtuple();
delete a;
gROOT->Clear();
gROOT->Reset();

}
