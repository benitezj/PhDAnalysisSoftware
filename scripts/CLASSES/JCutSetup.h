#ifndef JCutSetup_hh
#define JCutSetup_hh
#include "../Globals.h"
#include "../AnalysisGlobals.h"
#include "../FUNCTIONS/SetHisto.h"
#include "JCut.h"
#include "JCutList.h"


class JCutSetup:public TObject{
  ClassDef(JCutSetup,1);

 public:
  JCutSetup();
  JCutSetup(Float_t* mainquantity,TString title,Int_t nbins,Float_t low,Float_t high,TString xtitle,TString ytitle);
  ~JCutSetup();
  
  void Init(Float_t* mainquantity,TString title,Int_t nbins,Float_t low,Float_t high,TString xtitle,TString ytitle);
  //Cuts will be applied in order of id
  void InitInt(Int_t* mainquantity,TString title,Int_t nbins,Float_t low,Float_t high,TString xtitle,TString ytitle);
  void AddCut(JCut* cut);
  void FillPlots();
  void DrawPlots(Int_t idmin,Int_t idmax,TString opt);//cutid:<0=all,0=original,0<cutid=after cutid  IntoRootFile: 0=write, 1=write and draw, 2=draw
  void DrawRatios(Int_t idmin,Int_t idmax,TH1F* href,TString opt); 
  void DrawBackgrounds(Int_t idmin,Int_t idmax,TString opt);  
  TH1F* GetPlot(Int_t id);
  TH1F* GetPlotByName(TString cutname);
  TH1F* GetRatio(Int_t id);
  TH1F* GetBackground(Int_t id);
  TH1F* GetBackgroundByName(TString cutname);
  Int_t GetNCuts(){return CutCounter;}
  JCut* GetCut(Int_t cutnumber){return Cut[cutnumber];}
  void AddCutsByList(JCutList* cutlist);
  void RemoveCut(JCut* cutpointer);

 private:
  void CreateHistos(Int_t id);
  void DeleteHistos(Int_t id);
  
  //histogram which stores main quantity after each cut is applied
  TH1F* HMainQuantity[100];
  Float_t* _mainquantity;
  Int_t* _mainquantityint;
  Int_t floatorint;

  TString _title; 
  Int_t _nbins;
  Float_t _low;
  Float_t _high;
  TString _xtitle;
  TString _ytitle; 

  Int_t CutCounter;
  JCut* Cut[100];
  
  //Efficiency ratios
  Bool_t MadeRatios;
  TH1F* HMainQuantityRatio[100];
  //Backgrounds
  TH1F* HMainQuantityBackground[100];
  


};
  

#endif

