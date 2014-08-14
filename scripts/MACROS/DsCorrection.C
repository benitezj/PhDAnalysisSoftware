
///Correct the raw Ds* 4-Vector such that it is on-shell
TGraph* findSolution(Float_t pDst,Float_t eDst){

  //Initial values    
  Float_t MDst=2.112;

  //This code finds the smallest solution to the following equation:
  ///A*x^4+B*x^3+C*x^2+D*x+E=0 
  Float_t A=9.;
  Float_t B=30*pDst;
  Float_t C=9.*MDst*MDst + 3.*pDst*pDst - eDst*eDst;
  Float_t D=12.*MDst*MDst*pDst + 20.pDst*pDst*pDst - 4*eDst*eDst*pDst;
  Float_t E=4*MDst*MDst*pDst*pDst + 4*pDst*pDst*pDst*pDst - 4.*eDst*eDst*pDst*pDst;
  

  //Now define range for w : (-T,T)
  Float_t min=-2;
  Float_t max=2.;
  Int_t Nsteps=1000;
  Float_t step=(max-min)/Nsteps;
  TGraph* G=new TGraph();
  Float_t w0=0.;
  Float_t fprev=0.;
  for(Int_t i=0;i<=Nsteps;i++){
    Float_t w=min+i*step;
    Float_t f=A*w*w*w*w+B*w*w*w+C*w*w+D*w+E;
    cout<<i<<" "<<w<<" "<<f<<endl;
    G->SetPoint(i,w,f);
    if(f*fprev<0.){w0=w-step/2.;break;}
    fprev=f;
  }
  G->SetPoint(i,w0,0.);
  cout<<"the solution is="<<w0<<endl;
  G->Draw("ap");
  return G;
}
