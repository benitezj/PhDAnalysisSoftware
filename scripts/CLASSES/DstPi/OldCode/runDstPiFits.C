#include "../Globals.h"
#include "../CLASSES/DstPiAnalysisControl.C"

void runDstPiFits(Int_t whichtype,Int_t whichone){
  

  TString dir="DstarCharged/DstarPi/Kpi";
  TString files[100];

  //No resolution, no efficiency
  if(whichtype==1)
    for(Int_t f=0;f<10;f++){
      files[f]=dir+"/config_"+(long)f+".txt";    
      if(whichone==f)analyzeDstPi(files[f]);    
    }
  
  //yes resolution, no efficiency
  if(whichtype==2)
    for(Int_t f=0;f<10;f++){
      files[20+f]=dir+"/config_"+(long)(20+f)+".txt";    
      if(whichone==f)analyzeDstPi(files[20+f]);    
    }

  //yes resolution, yes efficiency
  if(whichtype==3)
    for(Int_t f=0;f<10;f++){
      files[40+f]=dir+"/config_"+(long)(40+f)+".txt";    
      if(whichone==f)analyzeDstPi(files[40+f]);    
    }


}
