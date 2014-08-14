#include "../Globals.h"
#include "../MACROS/DstPiAnalysisControl.C"

#define Nfits 10

void runDstPiFits(Int_t which){

  TString dir="DstarCharged/DstarPi/Kpi";
  TString files[100];

  //No resolution, no efficiency
  if(which==1)
    for(Int_t f=0;f<Nfits;f++){
      files[f]=dir+"/config_"+(long)f+".txt";    
      analyzeDstPi(files[f]);    
    }
  
  //yes resolution, no efficiency
  if(which==2)
    for(Int_t f=0;f<Nfits;f++){
      files[20+f]=dir+"/config_"+(long)(20+f)+".txt";    
      analyzeDstPi(files[20+f]);    
    }

  //yes resolution, yes efficiency
  if(which==3)
    for(Int_t f=0;f<Nfits;f++){
      files[40+f]=dir+"/config_"+(long)(40+f)+".txt";    
      analyzeDstPi(files[40+f]);    
    }


}
