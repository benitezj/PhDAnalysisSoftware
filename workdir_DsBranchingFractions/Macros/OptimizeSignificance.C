{


for(Int_t i=0;i<10;i++){
  TCut cut=TString("dspstar>3.")+(long)(i);
  Float_t b=Ntuple->GetEntries(cut+TCut(".05<abs(dsmass-1.9693)&&abs(dsmass-1.9693)<.075"));
  Float_t splusb=Ntuple->GetEntries(cut+TCut("abs(dsmass-1.9693)<.025"));
  printf("pstar=%.1f SPlusB=%.1f  B=%.1f Significance=%.1f\n",3.+((float)i)/10.,splusb,b,(splusb-b)/sqrt(splusb));
}

//Results from last time
// pstar=3.0 SPlusB=427350.0  B=344565.0 Significance=126.6
// pstar=3.1 SPlusB=374959.0  B=297292.0 Significance=126.8
// pstar=3.2 SPlusB=325997.0  B=253698.0 Significance=126.6
// pstar=3.3 SPlusB=280921.0  B=214083.0 Significance=126.1
// pstar=3.4 SPlusB=239392.0  B=178406.0 Significance=124.6
// pstar=3.5 SPlusB=201908.0  B=146506.0 Significance=123.3
// pstar=3.6 SPlusB=167637.0  B=118725.0 Significance=119.5
// pstar=3.7 SPlusB=136982.0  B=94337.0 Significance=115.2
// pstar=3.8 SPlusB=109546.0  B=73435.0 Significance=109.1
// pstar=3.9 SPlusB=85630.0  B=55869.0 Significance=101.7

}
