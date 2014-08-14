#ifndef JCutEllipse_hh
#define JCutEllipse_hh
#include "JCut.h"
#include "../Globals.h"
#include <TEllipse.h>

class JCutEllipse:public JCut{
  ClassDef(JCutEllipse,1);

 public:
  JCutEllipse();
  JCutEllipse(TString cutname,Float_t* discquantityX,Float_t lowX,Float_t highX,Float_t* discquantityY,Float_t lowY,Float_t highY,Bool_t InsideOrOutside);
  ~JCutEllipse();

  void Init(TString cutname,Float_t* discquantityX,Float_t lowX,Float_t highX,Float_t* discquantityX,Float_t lowY,Float_t highY,Bool_t InsideOrOutside);
  Bool_t EvalCut();
  void DrawCut();

 private: 
 
  Float_t* _discquantityX;
  Float_t _highX;
  Float_t _lowX;
  Float_t* _discquantityY;
  Float_t _highY;
  Float_t _lowY;

  Float_t Rx;
  Float_t Ry;
  Float_t Cx;
  Float_t Cy;

  TEllipse ellipse;
};
  

#endif

