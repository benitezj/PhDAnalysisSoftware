#define NCT 10

void RecoEffTable(){

  TString cutname[NCT];
  Float_t nInit=49017235.37;
  Float_t nEvtPass[NCT];
  for(Int_t c=0;c<NCT;c++)nEvtPass[c]=0; 
  nEvtPass[0]=3638723; cutname[0]="$D$ reconstruction";
  nEvtPass[1]=3579849; cutname[1]="$K$ reconstruction";
  nEvtPass[2]=2946463; cutname[2]="$\\gamma$ reconstruction";
  nEvtPass[3]=2687801; cutname[3]="Right-Sign requirement";
  nEvtPass[4]=123983; cutname[4]="$m(D_s^*), p^*(D_s)$ and event fit prob. cuts";
  nEvtPass[5]=26482; cutname[5]="$KK\\pi$ reconstruction";
 

  for(Int_t c=0;c<NCT;c++){
    //Int_t c=0;
    //while(nEvtPass[c]>0){
    if(nEvtPass[c]>0){
      cout<<precision(4)<<fixed<<cutname[c]<<" & "<<100*nEvtPass[c]/nInit<<" \\\\"<<endl;    
    }
    //c++;
  }

}
