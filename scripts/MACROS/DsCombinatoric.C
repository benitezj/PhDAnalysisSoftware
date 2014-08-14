Int_t calculateDsCombinatoric(Int_t NParticles,Int_t nsigmax){
  
  if(NParticles<0||nsigmax<1)return 0;
  
  Int_t sum=0;
  for(Int_t i=0;i<=nsigmax;i++)//note that removing 0 particles also counts
    sum+=TMath::Binomial(NParticles,i);
  

  return sum;
}


//The number of combinations as a function of NParticles where we choose nsigmax=7 is the following:
// for(Int_t i=1;i<=10;i++)cout<<i<<" "<<calculateDsCombinatoric(i,7)<<" "<<log10(calculateDsCombinatoric(i,7))/(float)i<<endl;
// 1 2 0.30103
// 2 4 0.30103
// 3 8 0.30103
// 4 16 0.30103
// 5 32 0.30103
// 6 64 0.30103
// 7 128 0.30103
// 8 255 0.300818
// 9 502 0.300078
// 10 968 0.298588
///==> grows like a power law  C(N)~10^(.3N)=2^N
