



void ENuUpperLimit(){

  Float_t range =8.0;
  TF1 FLike("FLike","exp(-.5*(x*4.7362-[0])**2/[1]**2)",-range,range);
  FLike.SetParameter(0,6.09);
  //FLike.SetParameter(1,sqrt(2.8*2.8+4.3*4.3));
  FLike.SetParameter(1,5.19);
  //FLike.SetParameter(2,1);
  

  TH1F HLikeInt("HLikeInt","",1000,0,range);

  Int_t nsteps=1000;
  Float_t step=range/nsteps;
  HLikeInt.SetBinContent(0,0.);//
  for(Int_t i=1;i<=nsteps;i++){
    HLikeInt.SetBinContent(i,HLikeInt.GetBinContent(i-1)+FLike.Eval(((i-1)+0.5)*step)*step);    
  }
  HLikeInt.Scale(1./HLikeInt.GetBinContent(nsteps));
  Float_t limit=0.;
  for(Int_t i=1;i<=nsteps;i++)
    if( HLikeInt.GetBinContent(i) < 0.9) limit=HLikeInt.GetBinCenter(i);       
  


  TCanvas C("C","",600,500);

  HLikeInt.SetStats(0);
  HLikeInt.SetLineStyle(2);
  HLikeInt.GetXaxis()->SetTitle("B  (#times 10^{-4})");
  HLikeInt.Draw("l");
  FLike.Draw("same");
  TLine LLimit;
  LLimit.DrawLine(limit,0,limit,1);
  C.Print("ENuLimit_Likelihood.ps");

  cout<<"limi="<<limit<<endl;

}
