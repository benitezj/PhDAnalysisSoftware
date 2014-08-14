#include "JCutEllipse.h"

ClassImp(JCutEllipse);
JCutEllipse::JCutEllipse():
  JCut(),
  _discquantityX(NULL),
  _highX(0),
  _lowX(0),
  _discquantityY(NULL),
  _highY(0),
  _lowY(0)
{}

JCutEllipse::JCutEllipse(TString cutname,Float_t* discquantityX,Float_t lowX,Float_t highX,Float_t* discquantityY,Float_t lowY,Float_t highY,Bool_t InsideOrOutside):
  JCut(cutname,1,InsideOrOutside)
{
  Init(cutname,discquantityX,lowX,highX,discquantityY,lowY,highY,InsideOrOutside);
}

JCutEllipse::~JCutEllipse(){ 
}

void  JCutEllipse::Init(TString cutname,Float_t* discquantityX,Float_t lowX,Float_t highX,Float_t* discquantityY,Float_t lowY,Float_t highY,Bool_t InsideOrOutside){  

  if(discquantityX==NULL || discquantityY==NULL || lowX>highX || lowY>highY){
    cout<<"Error JCutEllipse::  cannot create cut:  "<<cutname<<" address:"<<discquantityX<<" "<<discquantityY<<endl;
  }
  else {
    CutName=cutname;
    CutType=1;
    _InsideOrOutside=InsideOrOutside;
    _discquantityX=discquantityX;
    _highX=highX;
    _lowX=lowX;
    _discquantityY=discquantityY;
    _highY=highY;
    _lowY=lowY;
    Rx=(_highX-_lowX)/2;
    Ry=(_highY-_lowY)/2;  
    Cx=(_highX+_lowX)/2;
    Cy=(_highY+_lowY)/2;   
    ellipse.SetR1(Rx);
    ellipse.SetR2(Ry);
    ellipse.SetX1(Cx);
    ellipse.SetY1(Cy);
    

    cout<<"New JCutEllipse:: "<<cutname<<" address:"
	<<discquantityX<<" "<<_lowX<<" "<<_highX<<" "
	<<discquantityY<<" "<<_lowY<<" "<<_highY<<" "
	<<_InsideOrOutside<<endl;
  }
}


Bool_t JCutEllipse::EvalCut(){
  if(_discquantityX==NULL || _discquantityY==NULL)
    return kFALSE;

  Bool_t pass=kTRUE;
  
  ////need to determine if the data point is inside the oval
  //equation for oval perimeter: (x/Rx)^2+(y/Ry)^2=1 where Rx and Ry are the radii in x and y dimension
  //if LHS<=1 then data point is inside

     
  //for inside cut
  if(_InsideOrOutside){
    if(pow((*_discquantityX-Cx)/Rx,2)+pow((*_discquantityY-Cy)/Ry,2)<=1)
      pass=kTRUE;     
    else pass=kFALSE;
  }

  //for outside cut
  if(!_InsideOrOutside){
    if(pow((*_discquantityX-Cx)/Rx,2)+pow((*_discquantityY-Cy)/Ry,2)>1)
      pass=kTRUE;     
    else pass=kFALSE;
  }

  return pass;
}

void JCutEllipse::DrawCut(){

  ellipse.Draw();
  
}
