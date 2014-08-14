#include <TH1F.h>
#include <TChain.h>
#include <TLorentzVector.h>
#include <TVector3.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TF1.h>
#include "../tmp/Linux26SL5_i386_gcc412/DRecoilToolsUser/NtupleCode/TreeChain.hh"
#include "../tmp/Linux26SL5_i386_gcc412/DRecoilToolsUser/NtupleCode/DRecoResiduals.hh"
#include "../tmp/Linux26SL5_i386_gcc412/DRecoilToolsUser/NtupleCode/DRecoHistPdf.hh"
#include "../tmp/Linux26SL5_i386_gcc412/DRecoilToolsUser/NtupleCode/DRecoHistPdfChi2Var.hh"
#include "../tmp/Linux26SL5_i386_gcc412/DRecoilToolsUser/NtupleCode/DRecoAddPdf.hh"
#include "../tmp/Linux26SL5_i386_gcc412/DRecoilToolsUser/NtupleCode/DRecoProdPdf.hh"
#include "../tmp/Linux26SL5_i386_gcc412/DRecoilToolsUser/NtupleCode/DRecoSimulChi2Var.hh"
#include <RooAddPdf.h>
#include <RooFormulaVar.h>
#include <TList.h>
#include <RooPlot.h>
#include <RooMinuit.h>
#include <RooRealVar.h>
#include <RooDataHist.h>
#include <RooGenericPdf.h>
#include "RooGlobalFunc.h"
#include "RooCmdArg.h"
#include <RooChebychev.h>
#include <RooNumConvPdf.h>
using namespace RooFit;

void CreateDsToKKpiNoDenom(){


  Int_t           runNumber;
  Int_t           platform;
  Int_t           partition;
  Int_t           upperID;
  Int_t           lowerID;
  Int_t           majorID;
  Int_t           configKey;
  Int_t           date;
  Int_t           ddate;
  Float_t         eePx;
  Float_t         eePy;
  Float_t         eePz;
  Float_t         eeE;
  Float_t         beamSX;
  Float_t         beamSY;
  Float_t         beamSZ;
  Float_t         beamSCovXX;
  Float_t         beamSCovYY;
  Float_t         beamSCovZZ;
  Float_t         beamSCovXZ;
  Float_t         primVtxX;
  Float_t         primVtxY;
  Float_t         primVtxZ;
  Float_t         primVtxCovXX;
  Float_t         primVtxCovYY;
  Float_t         primVtxCovZZ;
  Float_t         primVtxCovXY;
  Float_t         primVtxCovXZ;
  Float_t         primVtxCovYZ;
  Float_t         primVtxChi2;
  Int_t           primVtxNDof;
  Int_t           mcLen;
  Int_t           mcLund[800];   //[mcLen]
  Int_t           mothIdx[800];   //[mcLen]
  Int_t           dauLen[800];   //[mcLen]
  Int_t           dauIdx[800];   //[mcLen]
  Float_t         mcmass[800];   //[mcLen]
  Float_t         mcp3CM[800];   //[mcLen]
  Float_t         mccosthCM[800];   //[mcLen]
  Float_t         mcphiCM[800];   //[mcLen]
  Float_t         mcenergyCM[800];   //[mcLen]
  Float_t         mcp3[800];   //[mcLen]
  Float_t         mccosth[800];   //[mcLen]
  Float_t         mcphi[800];   //[mcLen]
  Float_t         mcenergy[800];   //[mcLen]
  Float_t         mcVtxx[800];   //[mcLen]
  Float_t         mcVtxy[800];   //[mcLen]
  Float_t         mcVtxz[800];   //[mcLen]
  Int_t           nDsSt;
  Float_t         DsStMass[400];   //[nDsSt]
  Float_t         DsStMassErr[400];   //[nDsSt]
  Float_t         DsStcosth[400];   //[nDsSt]
  Float_t         DsStcosthCM[400];   //[nDsSt]
  Float_t         DsStenergy[400];   //[nDsSt]
  Float_t         DsStenergyCM[400];   //[nDsSt]
  Float_t         DsStp3[400];   //[nDsSt]
  Float_t         DsStp3CM[400];   //[nDsSt]
  Float_t         DsStphi[400];   //[nDsSt]
  Float_t         DsStphiCM[400];   //[nDsSt]
  Int_t           DsStLund[400];   //[nDsSt]
  Int_t           DsStMCIdx[400];   //[nDsSt]
  Int_t           DsStd1Lund[400];   //[nDsSt]
  Int_t           DsStd1Idx[400];   //[nDsSt]
  Int_t           DsStd2Lund[400];   //[nDsSt]
  Int_t           DsStd2Idx[400];   //[nDsSt]
  Int_t           nDs;
  Float_t         DsChi2[400];   //[nDs]
  Float_t         DsMass[400];   //[nDs]
  Float_t         DsMassErr[400];   //[nDs]
  Float_t         DsVtxx[400];   //[nDs]
  Float_t         DsVtxy[400];   //[nDs]
  Float_t         DsVtxz[400];   //[nDs]
  Float_t         Dscosth[400];   //[nDs]
  Float_t         DscosthCM[400];   //[nDs]
  Float_t         Dsenergy[400];   //[nDs]
  Float_t         DsenergyCM[400];   //[nDs]
  Float_t         Dsp3[400];   //[nDs]
  Float_t         Dsp3CM[400];   //[nDs]
  Float_t         Dsphi[400];   //[nDs]
  Float_t         DsphiCM[400];   //[nDs]
  Int_t           DsLund[400];   //[nDs]
  Int_t           DsMCIdx[400];   //[nDs]
  Int_t           DsVtxStatus[400];   //[nDs]
  Int_t           DsnDof[400];   //[nDs]
  Int_t           Dsd1Lund[400];   //[nDs]
  Int_t           Dsd1Idx[400];   //[nDs]
  Int_t           Dsd2Lund[400];   //[nDs]
  Int_t           Dsd2Idx[400];   //[nDs]
  Int_t           Dsd3Lund[400];   //[nDs]
  Int_t           Dsd3Idx[400];   //[nDs]
  Int_t           nK;
  Float_t         Kcosth[800];   //[nK]
  Float_t         KcosthCM[800];   //[nK]
  Float_t         Kenergy[800];   //[nK]
  Float_t         KenergyCM[800];   //[nK]
  Float_t         Kp3[800];   //[nK]
  Float_t         Kp3CM[800];   //[nK]
  Float_t         Kphi[800];   //[nK]
  Float_t         KphiCM[800];   //[nK]
  Int_t           KLund[800];   //[nK]
  Int_t           KMCIdx[800];   //[nK]
  Int_t           KTrkIdx[800];   //[nK]
  Int_t           nPi;
  Float_t         Picosth[400];   //[nPi]
  Float_t         PicosthCM[400];   //[nPi]
  Float_t         Pienergy[400];   //[nPi]
  Float_t         PienergyCM[400];   //[nPi]
  Float_t         Pip3[400];   //[nPi]
  Float_t         Pip3CM[400];   //[nPi]
  Float_t         Piphi[400];   //[nPi]
  Float_t         PiphiCM[400];   //[nPi]
  Int_t           PiLund[400];   //[nPi]
  Int_t           PiMCIdx[400];   //[nPi]
  Int_t           PiTrkIdx[400];   //[nPi]
  Int_t           nGam;
  Float_t         Gamcosth[400];   //[nGam]
  Float_t         GamcosthCM[400];   //[nGam]
  Float_t         Gamenergy[400];   //[nGam]
  Float_t         GamenergyCM[400];   //[nGam]
  Float_t         Gamp3[400];   //[nGam]
  Float_t         Gamp3CM[400];   //[nGam]
  Float_t         Gamphi[400];   //[nGam]
  Float_t         GamphiCM[400];   //[nGam]
  Int_t           GamLund[400];   //[nGam]
  Int_t           GamMCIdx[400];   //[nGam]
  Int_t           nTRK;
  Int_t           TRKLund[500];   //[nTRK]
  Int_t           piSelectorsMap[500];   //[nTRK]
  Int_t           KSelectorsMap[500];   //[nTRK]
  Float_t         GamLat[400];   //[nGam]
  Float_t         GamA42[400];   //[nGam]
  Float_t         GamECal[400];   //[nGam]
  Float_t         GamErawCal[400];   //[nGam]
  Float_t         GamCentx[400];   //[nGam]
  Float_t         GamCenty[400];   //[nGam]
  Float_t         GamCentz[400];   //[nGam]
  Int_t           GamnCrys[400];   //[nGam]
  Float_t         GamClosestBumpDist[400];   //[nGam]
  Float_t         GamClosestTrkDist[400];   //[nGam]
  Int_t           GamClosestTrkIdx[400];   //[nGam]


  TH1F HDsMassMCRaw("HDsMassMCRaw","",80,1.89,2.05);
  TH1F HDsMassDataRaw("HDsMassDataRaw","",80,1.89,2.05);

  TH1F HDsMassMCRawCut("HDsMassMCRawCut","",80,1.89,2.05);
  TH1F HDsMassDataRawCut("HDsMassDataRawCut","",80,1.89,2.05);

  TH1F HDsStMassMC("HDsStMassMC","",128,1.970,2.226);
  TH1F HDsStMassMCSig("HDsStMassMCSig","",128,1.970,2.226);
  TH1F HDsStMassMCBkg("HDsStMassMCBkg","",128,1.970,2.226);
  TH1F HDsStMassData("HDsStMassData","",128,1.970,2.226);

  TH1F HDsStMassMCDalitz0("HDsStMassMCDalitz0","",128,1.970,2.226);
  TH1F HDsStMassMCBkgDalitz0("HDsStMassMCBkgDalitz0","",128,1.970,2.226);
  TH1F HDsStMassDataDalitz0("HDsStMassDataDalitz0","",128,1.970,2.226);
  TH1F HDsStMassMCDalitz1("HDsStMassMCDalitz1","",128,1.970,2.226);
  TH1F HDsStMassMCBkgDalitz1("HDsStMassMCBkgDalitz1","",128,1.970,2.226);
  TH1F HDsStMassDataDalitz1("HDsStMassDataDalitz1","",128,1.970,2.226);
  TH1F HDsStMassMCDalitz2("HDsStMassMCDalitz2","",128,1.970,2.226);
  TH1F HDsStMassMCBkgDalitz2("HDsStMassMCBkgDalitz2","",128,1.970,2.226);
  TH1F HDsStMassDataDalitz2("HDsStMassDataDalitz2","",128,1.970,2.226);


  TH1F HDsStDeltaMMCRaw("HDsStDeltaMMCRaw","",200,.04,.24);
  TH1F HDsStDeltaMDataRaw("HDsStDeltaMDataRaw","",200,.04,.24);

  TH1F HDsStDeltaMMCRawCut("HDsStDeltaMMCRawCut","",200,.04,.24);
  TH1F HDsStDeltaMDataRawCut("HDsStDeltaMDataRawCut","",200,.04,.24);

  TH1F HDsMassMC("HDsMassMC","",80,1.89,2.05);
  TH1F HDsMassMCSig("HDsMassMCSig","",80,1.89,2.05);
  TH1F HDsMassMCBkg("HDsMassMCBkg","",80,1.89,2.05);
  TH1F HDsMassData("HDsMassData","",80,1.89,2.05);

  //for Dalitz weights
  TH1F HDsMassMCDalitz0("HDsMassMCDalitz0","",80,1.89,2.05);
  TH1F HDsMassMCBkgDalitz0("HDsMassMCBkgDalitz0","",80,1.89,2.05);
  TH1F HDsMassDataDalitz0("HDsMassDataDalitz0","",80,1.89,2.05);
  TH1F HDsMassMCDalitz1("HDsMassMCDalitz1","",80,1.89,2.05);
  TH1F HDsMassMCBkgDalitz1("HDsMassMCBkgDalitz1","",80,1.89,2.05);
  TH1F HDsMassDataDalitz1("HDsMassDataDalitz1","",80,1.89,2.05);
  TH1F HDsMassMCDalitz2("HDsMassMCDalitz2","",80,1.89,2.05);
  TH1F HDsMassMCBkgDalitz2("HDsMassMCBkgDalitz2","",80,1.89,2.05);
  TH1F HDsMassDataDalitz2("HDsMassDataDalitz2","",80,1.89,2.05);
  
  //for Dalitz resolution systematic
  TH1F HDsMassMCDalitzRes0("HDsMassMCDalitzRes0","",80,1.89,2.05);
  TH1F HDsMassMCDalitzRes1("HDsMassMCDalitzRes1","",80,1.89,2.05);
  TH1F HDsMassMCDalitzRes2("HDsMassMCDalitzRes2","",80,1.89,2.05);


  //for pid systematic
  TH1F HDsMassMCNoPid("HDsMassMCNoPid","",80,1.89,2.05);
  TH1F HDsMassMCNoPidBkg("HDsMassMCNoPidBkg","",80,1.89,2.05);
  TH1F HDsMassDataNoPid("HDsMassDataNoPid","",80,1.89,2.05);

  TH1F HDsMassMCNoPidSigSide("HDsMassMCNoPidSigSide","",80,1.89,2.05);
  TH1F HDsMassDataNoPidSigSide("HDsMassDataNoPidSigSide","",80,1.89,2.05);
  TH1F HDsMassMCNoPidLoSide("HDsMassMCNoPidLoSide","",80,1.89,2.05);
  TH1F HDsMassDataNoPidLoSide("HDsMassDataNoPidLoSide","",80,1.89,2.05);
  TH1F HDsMassMCNoPidHiSide("HDsMassMCNoPidHiSide","",80,1.89,2.05);
  TH1F HDsMassDataNoPidHiSide("HDsMassDataNoPidHiSide","",80,1.89,2.05);

  TH1F HDsMassMCSigSide("HDsMassMCSigSide","",80,1.89,2.05);
  TH1F HDsMassDataSigSide("HDsMassDataSigSide","",80,1.89,2.05);
  TH1F HDsMassMCLoSide("HDsMassMCLoSide","",80,1.89,2.05);
  TH1F HDsMassDataLoSide("HDsMassDataLoSide","",80,1.89,2.05);
  TH1F HDsMassMCHiSide("HDsMassMCHiSide","",80,1.89,2.05);
  TH1F HDsMassDataHiSide("HDsMassDataHiSide","",80,1.89,2.05);


  //prob(chi2)
  TH1F HDsVtxChi2MC("HDsVtxChi2MC","",100,-5,0.0001);
  TH1F HDsVtxChi2MCSig("HDsVtxChi2MCSig","",100,-5,0.0001);
  TH1F HDsVtxChi2MCBkg("HDsVtxChi2MCBkg","",100,-5,0.0001);
  TH1F HDsVtxChi2Data("HDsVtxChi2Data","",100,-5,0.0001);

  TH1F HDsVtxChi2MCLoSide("HDsVtxChi2MCLoSide","",100,-5,0.0001); 
  TH1F HDsVtxChi2DataLoSide("HDsVtxChi2DataLoSide","",100,-5,0.0001);

  TH1F HDsVtxChi2MCHiSide("HDsVtxChi2MCHiSide","",100,-5,0.0001); 
  TH1F HDsVtxChi2DataHiSide("HDsVtxChi2DataHiSide","",100,-5,0.0001);


  ////
  TH1F HD0KKDeltaMMC("HD0KKDeltaMMC","",150,.130,.160);
  TH1F HD0KKDeltaMData("HD0KKDeltaMData","",150,.130,.160);
  TH1F HKKMassMC("HKKMassMC","",200,1.,2.);
  TH1F HKKMassData("HKKMassData","",200,1.,2.);
  TH1F HKpiMassMC("HKpiMassMC","",200,.7,1.7);
  TH1F HKpiMassData("HKpiMassData","",200,.7,1.7);


 
  TChain* tmc=new TChain("ntp1");
  //TreeChain(tmc,1,90,"DsTolnu/KKpi/NoDenom/SP1005/Ntuple");
  TreeChain(tmc,1,900,"DsTolnu/KKpi/NoDenom/SP1005/Ntuple");

  tmc->SetBranchAddress("mcLen", &mcLen);
  tmc->SetBranchAddress("mcLund", mcLund);
  tmc->SetBranchAddress("mothIdx", mothIdx);
  tmc->SetBranchAddress("dauLen", dauLen);
  tmc->SetBranchAddress("dauIdx", dauIdx);
  tmc->SetBranchAddress("mcmass", mcmass);
  tmc->SetBranchAddress("mcp3CM", mcp3CM);
  tmc->SetBranchAddress("mccosthCM", mccosthCM);
  tmc->SetBranchAddress("mcphiCM", mcphiCM);
  tmc->SetBranchAddress("mcenergyCM", mcenergyCM);
  tmc->SetBranchAddress("mcp3", mcp3);
  tmc->SetBranchAddress("mccosth", mccosth);
  tmc->SetBranchAddress("mcphi", mcphi);
  tmc->SetBranchAddress("mcenergy", mcenergy);
  tmc->SetBranchAddress("mcVtxx", mcVtxx);
  tmc->SetBranchAddress("mcVtxy", mcVtxy);
  tmc->SetBranchAddress("mcVtxz", mcVtxz);
  tmc->SetBranchAddress("nDsSt", &nDsSt);
  tmc->SetBranchAddress("DsStMass", DsStMass);
  tmc->SetBranchAddress("DsStMassErr", DsStMassErr);
  tmc->SetBranchAddress("DsStcosth", DsStcosth);
  tmc->SetBranchAddress("DsStcosthCM", DsStcosthCM);
  tmc->SetBranchAddress("DsStenergy", DsStenergy);
  tmc->SetBranchAddress("DsStenergyCM", DsStenergyCM);
  tmc->SetBranchAddress("DsStp3", DsStp3);
  tmc->SetBranchAddress("DsStp3CM", DsStp3CM);
  tmc->SetBranchAddress("DsStphi", DsStphi);
  tmc->SetBranchAddress("DsStphiCM", DsStphiCM);
  tmc->SetBranchAddress("DsStLund", DsStLund);
  tmc->SetBranchAddress("DsStMCIdx", DsStMCIdx);
  tmc->SetBranchAddress("DsStd1Lund", DsStd1Lund);
  tmc->SetBranchAddress("DsStd1Idx", DsStd1Idx);
  tmc->SetBranchAddress("DsStd2Lund", DsStd2Lund);
  tmc->SetBranchAddress("DsStd2Idx", DsStd2Idx);
  tmc->SetBranchAddress("nDs", &nDs);
  tmc->SetBranchAddress("DsChi2", DsChi2);
  tmc->SetBranchAddress("DsMass", DsMass);
  tmc->SetBranchAddress("DsMassErr", DsMassErr);
  tmc->SetBranchAddress("DsVtxx", DsVtxx);
  tmc->SetBranchAddress("DsVtxy", DsVtxy);
  tmc->SetBranchAddress("DsVtxz", DsVtxz);
  tmc->SetBranchAddress("Dscosth", Dscosth);
  tmc->SetBranchAddress("DscosthCM", DscosthCM);
  tmc->SetBranchAddress("Dsenergy", Dsenergy);
  tmc->SetBranchAddress("DsenergyCM", DsenergyCM);
  tmc->SetBranchAddress("Dsp3", Dsp3);
  tmc->SetBranchAddress("Dsp3CM", Dsp3CM);
  tmc->SetBranchAddress("Dsphi", Dsphi);
  tmc->SetBranchAddress("DsphiCM", DsphiCM);
  tmc->SetBranchAddress("DsLund", DsLund);
  tmc->SetBranchAddress("DsMCIdx", DsMCIdx);
  tmc->SetBranchAddress("DsVtxStatus", DsVtxStatus);
  tmc->SetBranchAddress("DsnDof", DsnDof);
  tmc->SetBranchAddress("Dsd1Lund", Dsd1Lund);
  tmc->SetBranchAddress("Dsd1Idx", Dsd1Idx);
  tmc->SetBranchAddress("Dsd2Lund", Dsd2Lund);
  tmc->SetBranchAddress("Dsd2Idx", Dsd2Idx);
  tmc->SetBranchAddress("Dsd3Lund", Dsd3Lund);
  tmc->SetBranchAddress("Dsd3Idx", Dsd3Idx);
  tmc->SetBranchAddress("nK", &nK);
  tmc->SetBranchAddress("Kcosth", Kcosth);
  tmc->SetBranchAddress("KcosthCM", KcosthCM);
  tmc->SetBranchAddress("Kenergy", Kenergy);
  tmc->SetBranchAddress("KenergyCM", KenergyCM);
  tmc->SetBranchAddress("Kp3", Kp3);
  tmc->SetBranchAddress("Kp3CM", Kp3CM);
  tmc->SetBranchAddress("Kphi", Kphi);
  tmc->SetBranchAddress("KphiCM", KphiCM);
  tmc->SetBranchAddress("KLund", KLund);
  tmc->SetBranchAddress("KMCIdx", KMCIdx);
  tmc->SetBranchAddress("KTrkIdx", KTrkIdx);
  tmc->SetBranchAddress("nPi", &nPi);
  tmc->SetBranchAddress("Picosth", Picosth);
  tmc->SetBranchAddress("PicosthCM", PicosthCM);
  tmc->SetBranchAddress("Pienergy", Pienergy);
  tmc->SetBranchAddress("PienergyCM", PienergyCM);
  tmc->SetBranchAddress("Pip3", Pip3);
  tmc->SetBranchAddress("Pip3CM", Pip3CM);
  tmc->SetBranchAddress("Piphi", Piphi);
  tmc->SetBranchAddress("PiphiCM", PiphiCM);
  tmc->SetBranchAddress("PiLund", PiLund);
  tmc->SetBranchAddress("PiMCIdx", PiMCIdx);
  tmc->SetBranchAddress("PiTrkIdx", PiTrkIdx);
  tmc->SetBranchAddress("nTRK", &nTRK);
  tmc->SetBranchAddress("TRKLund", TRKLund);
  tmc->SetBranchAddress("piSelectorsMap", piSelectorsMap);
  tmc->SetBranchAddress("KSelectorsMap", KSelectorsMap);
  tmc->SetBranchAddress("nGam", &nGam);
  tmc->SetBranchAddress("Gamcosth", Gamcosth);
  tmc->SetBranchAddress("GamcosthCM", GamcosthCM);
  tmc->SetBranchAddress("Gamenergy", Gamenergy);
  tmc->SetBranchAddress("GamenergyCM", GamenergyCM);
  tmc->SetBranchAddress("Gamp3", Gamp3);
  tmc->SetBranchAddress("Gamp3CM", Gamp3CM);
  tmc->SetBranchAddress("Gamphi", Gamphi);
  tmc->SetBranchAddress("GamphiCM", GamphiCM);
  tmc->SetBranchAddress("GamLund", GamLund);
  tmc->SetBranchAddress("GamMCIdx", GamMCIdx);
  tmc->SetBranchAddress("GamLat", GamLat);
  tmc->SetBranchAddress("GamA42", GamA42);
  tmc->SetBranchAddress("GamECal", GamECal);
  tmc->SetBranchAddress("GamErawCal", GamErawCal);
  tmc->SetBranchAddress("GamCentx", GamCentx);
  tmc->SetBranchAddress("GamCenty", GamCenty);
  tmc->SetBranchAddress("GamCentz", GamCentz);
  tmc->SetBranchAddress("GamnCrys", GamnCrys);
  tmc->SetBranchAddress("GamClosestBumpDist", GamClosestBumpDist);
  tmc->SetBranchAddress("GamClosestTrkDist", GamClosestTrkDist);
  tmc->SetBranchAddress("GamClosestTrkIdx", GamClosestTrkIdx);

  Int_t evtid=1;
  cout<<"Starting MC loop for "<<tmc->GetEntries()<<" events"<<endl;
  while(tmc->GetEntry(evtid++)){
    if(evtid%1000==0)cout<<evtid<<" mc events done"<<endl;
    
    for(Int_t DsStIdx=0;DsStIdx<nDsSt;DsStIdx++){
      Int_t DsIdx=DsStd1Idx[DsStIdx];
      Int_t GamIdx=DsStd2Idx[DsStIdx];
      Int_t K1Idx=Dsd1Idx[DsIdx];      
      Int_t K2Idx=Dsd2Idx[DsIdx];
      Int_t PiIdx=Dsd3Idx[DsIdx];

      TVector3 DsStVect(DsStp3CM[DsStIdx]*sin(acos(DsStcosthCM[DsStIdx]))*cos(DsStphiCM[DsStIdx]),
			DsStp3CM[DsStIdx]*sin(acos(DsStcosthCM[DsStIdx]))*sin(DsStphiCM[DsStIdx]),
			DsStp3CM[DsStIdx]*DsStcosthCM[DsStIdx]);
      TLorentzVector DsStp4(DsStVect,DsStenergyCM[DsStIdx]);

      TVector3 DsVect(Dsp3CM[DsIdx]*sin(acos(DscosthCM[DsIdx]))*cos(DsphiCM[DsIdx]),
		      Dsp3CM[DsIdx]*sin(acos(DscosthCM[DsIdx]))*sin(DsphiCM[DsIdx]),
		      Dsp3CM[DsIdx]*DscosthCM[DsIdx]);
      TLorentzVector Dsp4(DsVect,DsenergyCM[DsIdx]);
      
      TVector3 GamVect(Gamp3CM[GamIdx]*sin(acos(GamcosthCM[GamIdx]))*cos(GamphiCM[GamIdx]),
		       Gamp3CM[GamIdx]*sin(acos(GamcosthCM[GamIdx]))*sin(GamphiCM[GamIdx]),
		       Gamp3CM[GamIdx]*GamcosthCM[GamIdx]);
      TLorentzVector Gamp4(GamVect,GamenergyCM[GamIdx]);

      TVector3 K1Vect(Kp3CM[K1Idx]*sin(acos(KcosthCM[K1Idx]))*cos(KphiCM[K1Idx]),
		      Kp3CM[K1Idx]*sin(acos(KcosthCM[K1Idx]))*sin(KphiCM[K1Idx]),
		      Kp3CM[K1Idx]*KcosthCM[K1Idx]);
      TLorentzVector K1p4(K1Vect,KenergyCM[K1Idx]);
      
      TVector3 K2Vect(Kp3CM[K2Idx]*sin(acos(KcosthCM[K2Idx]))*cos(KphiCM[K2Idx]),
		      Kp3CM[K2Idx]*sin(acos(KcosthCM[K2Idx]))*sin(KphiCM[K2Idx]),
		      Kp3CM[K2Idx]*KcosthCM[K2Idx]);
      TLorentzVector K2p4(K2Vect,KenergyCM[K2Idx]);

      TVector3 PiVect(Pip3CM[PiIdx]*sin(acos(PicosthCM[PiIdx]))*cos(PiphiCM[PiIdx]),
		      Pip3CM[PiIdx]*sin(acos(PicosthCM[PiIdx]))*sin(PiphiCM[PiIdx]),
		      Pip3CM[PiIdx]*PicosthCM[PiIdx]);
      TLorentzVector Pip4(PiVect,PienergyCM[PiIdx]);

      //basic cuts      
      if(Gamp4.Vect().Unit()*DsStp4.Vect().Unit()<0)continue;

      if(GamClosestBumpDist[GamIdx]<15||GamClosestTrkDist[GamIdx]<15)continue;

      Int_t K1Pid=0;
      for(Int_t i=19;i<=23;i++)//NEW PID; 19=notpion,20=veryloose,21=loose,22=tight,23=verytight
        if((KSelectorsMap[KTrkIdx[K1Idx]] & (1<<i)) == (1<<i))
          K1Pid++;//if passes selector increase
      Int_t K2Pid=0;
      for(Int_t i=19;i<=23;i++)//NEW PID; 19=notpion,20=veryloose,21=loose,22=tight,23=verytight
        if((KSelectorsMap[KTrkIdx[K2Idx]] & (1<<i)) == (1<<i))
          K2Pid++;//if passes selector increase  

      ///cut D*+-->D0(KK)pi+
      HKKMassMC.Fill((K1p4+K2p4).Mag());
      if(Dsd1Lund[DsIdx]*Dsd3Lund[DsIdx]<0)HKpiMassMC.Fill((K1p4+Pip4).Mag());
      if(Dsd2Lund[DsIdx]*Dsd3Lund[DsIdx]<0)HKpiMassMC.Fill((K2p4+Pip4).Mag());
      HD0KKDeltaMMC.Fill(DsMass[DsIdx]-(K1p4+K2p4).Mag());
  

      ///truth matched quantities
      Bool_t trueDs=1;
      Int_t K1mcidx=KMCIdx[Dsd1Idx[DsIdx]];
      Int_t K2mcidx=KMCIdx[Dsd2Idx[DsIdx]];
      Int_t Pimcidx=PiMCIdx[Dsd3Idx[DsIdx]];
      if(K1mcidx<0||K2mcidx<0||Pimcidx<0)trueDs=0;//tracks were generated
      if(trueDs)//Kaons and pi is true
	if(abs(mcLund[KMCIdx[K1Idx]])!=321 
	   || abs(mcLund[KMCIdx[K2Idx]])!=321 
	   || abs(mcLund[PiMCIdx[PiIdx]])!=211)trueDs=0;
      Int_t trueDsIdx=-1;
      if(trueDs){//Kaon has Ds mother
	if(abs(mcLund[mothIdx[KMCIdx[K1Idx]]])==431)trueDsIdx=mothIdx[KMCIdx[K1Idx]];
	if(abs(mcLund[mothIdx[mothIdx[KMCIdx[K1Idx]]]])==431)trueDsIdx=mothIdx[mothIdx[KMCIdx[K1Idx]]];
      }
      //tracks have same mother
      if(trueDsIdx<0
	 ||(mothIdx[KMCIdx[K2Idx]]!=trueDsIdx && mothIdx[mothIdx[KMCIdx[K2Idx]]]!=trueDsIdx )
	 ||(mothIdx[PiMCIdx[PiIdx]]!=trueDsIdx && mothIdx[mothIdx[PiMCIdx[PiIdx]]]!=trueDsIdx ) ) trueDs=0;
      
      Bool_t trueDsSt=1;
      if(!trueDs||GamMCIdx[DsStd2Idx[DsStIdx]]<0)trueDsSt=0;//gam was generated and Ds is true
      if(trueDsSt)
	if(abs(mcLund[GamMCIdx[DsStd2Idx[DsStIdx]]])!=22//gam is true photon
	   ||abs(mcLund[mothIdx[GamMCIdx[DsStd2Idx[DsStIdx]]]])!=433//gam has Ds* mother
	   ||mothIdx[GamMCIdx[DsStd2Idx[DsStIdx]]]!=mothIdx[trueDsIdx]) trueDsSt=0; //gam has same mother as Ds

      
      //PID Syst: apply pid to one K to clean bkg
      if(K2Pid>=2&&fabs(DsMass[DsIdx]-(K1p4+K2p4).Mag()-.1454)>.005)
	if(.120<(DsStMass[DsStIdx]-DsMass[DsIdx])&&(DsStMass[DsStIdx]-DsMass[DsIdx])<.160){
	  HDsMassMCNoPid.Fill(DsMass[DsIdx]);
	  if(!trueDs)HDsMassMCNoPidBkg.Fill(DsMass[DsIdx]);
	  if(1.93<DsMass[DsIdx]&&DsMass[DsIdx]<2.00)HDsMassMCNoPidSigSide.Fill(DsMass[DsIdx]);
	  if(1.93-.004-.035<DsMass[DsIdx]&&DsMass[DsIdx]<1.93-.004)HDsMassMCNoPidLoSide.Fill(DsMass[DsIdx]);	
	  if(2.00+.004<DsMass[DsIdx]&&DsMass[DsIdx]<2.00+.004+.035)HDsMassMCNoPidHiSide.Fill(DsMass[DsIdx]);
	}
      
     
      //full cuts
      if(K1Pid<2||K2Pid<2)continue;
      if(fabs(DsMass[DsIdx]-(K1p4+K2p4).Mag()-.1454)<.005) continue;

     
      
      HDsMassMCRaw.Fill(DsMass[DsIdx]);
      if(fabs(DsMass[DsIdx]-1.970)<.02){
	HDsMassMCRawCut.Fill(DsMass[DsIdx]);
	HDsStMassMC.Fill(DsStMass[DsStIdx]);
	HDsStDeltaMMCRaw.Fill(DsStMass[DsStIdx]-DsMass[DsIdx]);
	
	if(trueDsSt)HDsStMassMCSig.Fill(DsStMass[DsStIdx]);
	if(!trueDsSt)HDsStMassMCBkg.Fill(DsStMass[DsStIdx]);
	
	//Dalitz regions
	if(.95<(K1p4+K2p4).Mag2()&&(K1p4+K2p4).Mag2()<1.15)
	  HDsStMassMCDalitz0.Fill(DsStMass[DsStIdx]);//Phi region
	else if((Dsd1Lund[DsIdx]*Dsd3Lund[DsIdx]<0&&.6<(K1p4+Pip4).Mag2()&&(K1p4+Pip4).Mag2()<1.) //K1 pi : K* region
		|| (Dsd2Lund[DsIdx]*Dsd3Lund[DsIdx]<0&&.6<(K2p4+Pip4).Mag2()&&(K2p4+Pip4).Mag2()<1.)) //K2 pi : K* region
	  HDsStMassMCDalitz1.Fill(DsStMass[DsStIdx]);
	else  HDsStMassMCDalitz2.Fill(DsStMass[DsStIdx]);
	
	if(!trueDsSt){//bkg Dalitz regions
	  if(.95<(K1p4+K2p4).Mag2()&&(K1p4+K2p4).Mag2()<1.15)
	    HDsStMassMCBkgDalitz0.Fill(DsStMass[DsStIdx]);//Phi region
	  else if((Dsd1Lund[DsIdx]*Dsd3Lund[DsIdx]<0&&.6<(K1p4+Pip4).Mag2()&&(K1p4+Pip4).Mag2()<1.) //K1 pi : K* region
		  || (Dsd2Lund[DsIdx]*Dsd3Lund[DsIdx]<0&&.6<(K2p4+Pip4).Mag2()&&(K2p4+Pip4).Mag2()<1.)) //K2 pi : K* region
	    HDsStMassMCBkgDalitz1.Fill(DsStMass[DsStIdx]);
	  else  HDsStMassMCBkgDalitz2.Fill(DsStMass[DsStIdx]);
	}
	
      }
      
      

      //apply deltaM cut
      if(.120<(DsStMass[DsStIdx]-DsMass[DsIdx])&&(DsStMass[DsStIdx]-DsMass[DsIdx])<.160){
	if(fabs(DsMass[DsIdx]-(1.970))<.02)HDsStDeltaMMCRawCut.Fill(DsStMass[DsStIdx]-DsMass[DsIdx]);
	
	HDsMassMC.Fill(DsMass[DsIdx]);
	if(trueDs)HDsMassMCSig.Fill(DsMass[DsIdx]);
	if(!trueDs)HDsMassMCBkg.Fill(DsMass[DsIdx]);
	
	if(1.93<DsMass[DsIdx]&&DsMass[DsIdx]<2.00)HDsMassMCSigSide.Fill(DsMass[DsIdx]);
	if(1.93-.004-.035<DsMass[DsIdx]&&DsMass[DsIdx]<1.93-.004)HDsMassMCLoSide.Fill(DsMass[DsIdx]);	
	if(2.00+.004<DsMass[DsIdx]&&DsMass[DsIdx]<2.00+.004+.035)HDsMassMCHiSide.Fill(DsMass[DsIdx]);	

	if(1.93<DsMass[DsIdx]&&DsMass[DsIdx]<2.00){
	  HDsVtxChi2MC.Fill(log10(TMath::Prob(DsChi2[DsIdx],DsnDof[DsIdx])));
	  if(trueDs)HDsVtxChi2MCSig.Fill(log10(TMath::Prob(DsChi2[DsIdx],DsnDof[DsIdx])));
	  if(!trueDs)HDsVtxChi2MCBkg.Fill(log10(TMath::Prob(DsChi2[DsIdx],DsnDof[DsIdx])));
	}
	
	if(1.93-.004-.035<DsMass[DsIdx]&&DsMass[DsIdx]<1.93-.004)
	  HDsVtxChi2MCLoSide.Fill(log10(TMath::Prob(DsChi2[DsIdx],DsnDof[DsIdx])));
	if(2.00+.004<DsMass[DsIdx]&&DsMass[DsIdx]<2.00+.004+.035)
	  HDsVtxChi2MCHiSide.Fill(log10(TMath::Prob(DsChi2[DsIdx],DsnDof[DsIdx])));      
	
	
	//Dalitz regions
	if(.95<(K1p4+K2p4).Mag2()&&(K1p4+K2p4).Mag2()<1.15)
	  HDsMassMCDalitz0.Fill(DsMass[DsIdx]);//Phi region
	else if((Dsd1Lund[DsIdx]*Dsd3Lund[DsIdx]<0&&.6<(K1p4+Pip4).Mag2()&&(K1p4+Pip4).Mag2()<1.) //K1 pi : K* region
		|| (Dsd2Lund[DsIdx]*Dsd3Lund[DsIdx]<0&&.6<(K2p4+Pip4).Mag2()&&(K2p4+Pip4).Mag2()<1.)) //K2 pi : K* region
	  HDsMassMCDalitz1.Fill(DsMass[DsIdx]);
	else  HDsMassMCDalitz2.Fill(DsMass[DsIdx]);
	
	if(!trueDs){//bkg Dalitz regions
	  if(.95<(K1p4+K2p4).Mag2()&&(K1p4+K2p4).Mag2()<1.15)
	    HDsMassMCBkgDalitz0.Fill(DsMass[DsIdx]);//Phi region
	  else if((Dsd1Lund[DsIdx]*Dsd3Lund[DsIdx]<0&&.6<(K1p4+Pip4).Mag2()&&(K1p4+Pip4).Mag2()<1.) //K1 pi : K* region
		  || (Dsd2Lund[DsIdx]*Dsd3Lund[DsIdx]<0&&.6<(K2p4+Pip4).Mag2()&&(K2p4+Pip4).Mag2()<1.)) //K2 pi : K* region
	    HDsMassMCBkgDalitz1.Fill(DsMass[DsIdx]);
	  else  HDsMassMCBkgDalitz2.Fill(DsMass[DsIdx]);
	}

	//Dalitz regions for Dalitz resolution Systematic --> Use generated 4-vectors
	if(trueDs){
	  TVector3 K1VectTrue(mcp3CM[K1mcidx]*sin(acos(mccosthCM[K1mcidx]))*cos(mcphiCM[K1mcidx]),
			      mcp3CM[K1mcidx]*sin(acos(mccosthCM[K1mcidx]))*sin(mcphiCM[K1mcidx]),
			      mcp3CM[K1mcidx]*mccosthCM[K1mcidx]);
	  TLorentzVector K1p4True(K1VectTrue,mcenergyCM[K1mcidx]);
      
	  TVector3 K2VectTrue(mcp3CM[K2mcidx]*sin(acos(mccosthCM[K2mcidx]))*cos(mcphiCM[K2mcidx]),
			      mcp3CM[K2mcidx]*sin(acos(mccosthCM[K2mcidx]))*sin(mcphiCM[K2mcidx]),
			      mcp3CM[K2mcidx]*mccosthCM[K2mcidx]);
	  TLorentzVector K2p4True(K2VectTrue,mcenergyCM[K2mcidx]);

	  TVector3 PiVectTrue(mcp3CM[Pimcidx]*sin(acos(mccosthCM[Pimcidx]))*cos(mcphiCM[Pimcidx]),
			      mcp3CM[Pimcidx]*sin(acos(mccosthCM[Pimcidx]))*sin(mcphiCM[Pimcidx]),
			      mcp3CM[Pimcidx]*mccosthCM[Pimcidx]);
	  TLorentzVector Pip4True(PiVectTrue,mcenergyCM[Pimcidx]);

	  
	  if(.95<(K1p4True+K2p4True).Mag2()&&(K1p4True+K2p4True).Mag2()<1.15)
	    HDsMassMCDalitzRes0.Fill(DsMass[DsIdx]);//Phi region
	  else if((Dsd1Lund[DsIdx]*Dsd3Lund[DsIdx]<0&&.6<(K1p4True+Pip4True).Mag2()&&(K1p4True+Pip4True).Mag2()<1.) //K1 pi : K* region
		  || (Dsd2Lund[DsIdx]*Dsd3Lund[DsIdx]<0&&.6<(K2p4True+Pip4True).Mag2()&&(K2p4True+Pip4True).Mag2()<1.)) //K2 pi : K* region
	    HDsMassMCDalitzRes1.Fill(DsMass[DsIdx]);
	  else  HDsMassMCDalitzRes2.Fill(DsMass[DsIdx]);
	}


      }
    }   
  }

  delete tmc;
  //--------------------------------------------------------------------------------------------------
  //Loop over Data events to fill histograms
  //--------------------------------------------------------------------------------------------------
  TChain* tdata=new TChain("ntp1");
  //TreeChain(tdata,1,510,"DsTolnu/KKpi/NoDenom/Data/Ntuple");
  TreeChain(tdata,1,5100,"DsTolnu/KKpi/NoDenom/Data/Ntuple");
  
  tdata->SetBranchAddress("nDsSt", &nDsSt);
  tdata->SetBranchAddress("DsStMass", DsStMass);
  tdata->SetBranchAddress("DsStMassErr", DsStMassErr);
  tdata->SetBranchAddress("DsStcosth", DsStcosth);
  tdata->SetBranchAddress("DsStcosthCM", DsStcosthCM);
  tdata->SetBranchAddress("DsStenergy", DsStenergy);
  tdata->SetBranchAddress("DsStenergyCM", DsStenergyCM);
  tdata->SetBranchAddress("DsStp3", DsStp3);
  tdata->SetBranchAddress("DsStp3CM", DsStp3CM);
  tdata->SetBranchAddress("DsStphi", DsStphi);
  tdata->SetBranchAddress("DsStphiCM", DsStphiCM);
  tdata->SetBranchAddress("DsStLund", DsStLund);
  tdata->SetBranchAddress("DsStMCIdx", DsStMCIdx);
  tdata->SetBranchAddress("DsStd1Lund", DsStd1Lund);
  tdata->SetBranchAddress("DsStd1Idx", DsStd1Idx);
  tdata->SetBranchAddress("DsStd2Lund", DsStd2Lund);
  tdata->SetBranchAddress("DsStd2Idx", DsStd2Idx);
  tdata->SetBranchAddress("nDs", &nDs);
  tdata->SetBranchAddress("DsChi2", DsChi2);
  tdata->SetBranchAddress("DsMass", DsMass);
  tdata->SetBranchAddress("DsMassErr", DsMassErr);
  tdata->SetBranchAddress("DsVtxx", DsVtxx);
  tdata->SetBranchAddress("DsVtxy", DsVtxy);
  tdata->SetBranchAddress("DsVtxz", DsVtxz);
  tdata->SetBranchAddress("Dscosth", Dscosth);
  tdata->SetBranchAddress("DscosthCM", DscosthCM);
  tdata->SetBranchAddress("Dsenergy", Dsenergy);
  tdata->SetBranchAddress("DsenergyCM", DsenergyCM);
  tdata->SetBranchAddress("Dsp3", Dsp3);
  tdata->SetBranchAddress("Dsp3CM", Dsp3CM);
  tdata->SetBranchAddress("Dsphi", Dsphi);
  tdata->SetBranchAddress("DsphiCM", DsphiCM);
  tdata->SetBranchAddress("DsLund", DsLund);
  tdata->SetBranchAddress("DsMCIdx", DsMCIdx);
  tdata->SetBranchAddress("DsVtxStatus", DsVtxStatus);
  tdata->SetBranchAddress("DsnDof", DsnDof);
  tdata->SetBranchAddress("Dsd1Lund", Dsd1Lund);
  tdata->SetBranchAddress("Dsd1Idx", Dsd1Idx);
  tdata->SetBranchAddress("Dsd2Lund", Dsd2Lund);
  tdata->SetBranchAddress("Dsd2Idx", Dsd2Idx);
  tdata->SetBranchAddress("Dsd3Lund", Dsd3Lund);
  tdata->SetBranchAddress("Dsd3Idx", Dsd3Idx);
  tdata->SetBranchAddress("nK", &nK);
  tdata->SetBranchAddress("Kcosth", Kcosth);
  tdata->SetBranchAddress("KcosthCM", KcosthCM);
  tdata->SetBranchAddress("Kenergy", Kenergy);
  tdata->SetBranchAddress("KenergyCM", KenergyCM);
  tdata->SetBranchAddress("Kp3", Kp3);
  tdata->SetBranchAddress("Kp3CM", Kp3CM);
  tdata->SetBranchAddress("Kphi", Kphi);
  tdata->SetBranchAddress("KphiCM", KphiCM);
  tdata->SetBranchAddress("KLund", KLund);
  tdata->SetBranchAddress("KMCIdx", KMCIdx);
  tdata->SetBranchAddress("KTrkIdx", KTrkIdx);
  tdata->SetBranchAddress("nPi", &nPi);
  tdata->SetBranchAddress("Picosth", Picosth);
  tdata->SetBranchAddress("PicosthCM", PicosthCM);
  tdata->SetBranchAddress("Pienergy", Pienergy);
  tdata->SetBranchAddress("PienergyCM", PienergyCM);
  tdata->SetBranchAddress("Pip3", Pip3);
  tdata->SetBranchAddress("Pip3CM", Pip3CM);
  tdata->SetBranchAddress("Piphi", Piphi);
  tdata->SetBranchAddress("PiphiCM", PiphiCM);
  tdata->SetBranchAddress("PiLund", PiLund);
  tdata->SetBranchAddress("PiMCIdx", PiMCIdx);
  tdata->SetBranchAddress("PiTrkIdx", PiTrkIdx);
  tdata->SetBranchAddress("nTRK", &nTRK);
  tdata->SetBranchAddress("TRKLund", TRKLund);
  tdata->SetBranchAddress("piSelectorsMap", piSelectorsMap);
  tdata->SetBranchAddress("KSelectorsMap", KSelectorsMap);
  tdata->SetBranchAddress("nGam", &nGam);
  tdata->SetBranchAddress("Gamcosth", Gamcosth);
  tdata->SetBranchAddress("GamcosthCM", GamcosthCM);
  tdata->SetBranchAddress("Gamenergy", Gamenergy);
  tdata->SetBranchAddress("GamenergyCM", GamenergyCM);
  tdata->SetBranchAddress("Gamp3", Gamp3);
  tdata->SetBranchAddress("Gamp3CM", Gamp3CM);
  tdata->SetBranchAddress("Gamphi", Gamphi);
  tdata->SetBranchAddress("GamphiCM", GamphiCM);
  tdata->SetBranchAddress("GamLund", GamLund);
  tdata->SetBranchAddress("GamMCIdx", GamMCIdx);
  tdata->SetBranchAddress("GamLat", GamLat);
  tdata->SetBranchAddress("GamA42", GamA42);
  tdata->SetBranchAddress("GamECal", GamECal);
  tdata->SetBranchAddress("GamErawCal", GamErawCal);
  tdata->SetBranchAddress("GamCentx", GamCentx);
  tdata->SetBranchAddress("GamCenty", GamCenty);
  tdata->SetBranchAddress("GamCentz", GamCentz);
  tdata->SetBranchAddress("GamnCrys", GamnCrys);
  tdata->SetBranchAddress("GamClosestBumpDist", GamClosestBumpDist);
  tdata->SetBranchAddress("GamClosestTrkDist", GamClosestTrkDist);
  tdata->SetBranchAddress("GamClosestTrkIdx", GamClosestTrkIdx);


  evtid=1;
  cout<<"Starting Data loop for "<<tdata->GetEntries()<<" events"<<endl;
  while(tdata->GetEntry(evtid++)){
    if(evtid%1000==0)cout<<evtid<<" data events done"<<endl;
  
    
    for(Int_t DsStIdx=0;DsStIdx<nDsSt;DsStIdx++){
      Int_t DsIdx=DsStd1Idx[DsStIdx];
      Int_t GamIdx=DsStd2Idx[DsStIdx];
      Int_t K1Idx=Dsd1Idx[DsIdx];      
      Int_t K2Idx=Dsd2Idx[DsIdx];
      Int_t PiIdx=Dsd3Idx[DsIdx];


      TVector3 DsStVect(DsStp3CM[DsStIdx]*sin(acos(DsStcosthCM[DsStIdx]))*cos(DsStphiCM[DsStIdx]),
			DsStp3CM[DsStIdx]*sin(acos(DsStcosthCM[DsStIdx]))*sin(DsStphiCM[DsStIdx]),
			DsStp3CM[DsStIdx]*DsStcosthCM[DsStIdx]);
      TLorentzVector DsStp4(DsStVect,DsStenergyCM[DsStIdx]);

      TVector3 DsVect(Dsp3CM[DsIdx]*sin(acos(DscosthCM[DsIdx]))*cos(DsphiCM[DsIdx]),
		      Dsp3CM[DsIdx]*sin(acos(DscosthCM[DsIdx]))*sin(DsphiCM[DsIdx]),
		      Dsp3CM[DsIdx]*DscosthCM[DsIdx]);
      TLorentzVector Dsp4(DsVect,DsenergyCM[DsIdx]);
      
      TVector3 GamVect(Gamp3CM[GamIdx]*sin(acos(GamcosthCM[GamIdx]))*cos(GamphiCM[GamIdx]),
		       Gamp3CM[GamIdx]*sin(acos(GamcosthCM[GamIdx]))*sin(GamphiCM[GamIdx]),
		       Gamp3CM[GamIdx]*GamcosthCM[GamIdx]);
      TLorentzVector Gamp4(GamVect,GamenergyCM[GamIdx]);

      TVector3 K1Vect(Kp3CM[K1Idx]*sin(acos(KcosthCM[K1Idx]))*cos(KphiCM[K1Idx]),
		      Kp3CM[K1Idx]*sin(acos(KcosthCM[K1Idx]))*sin(KphiCM[K1Idx]),
		      Kp3CM[K1Idx]*KcosthCM[K1Idx]);
      TLorentzVector K1p4(K1Vect,KenergyCM[K1Idx]);
      
      TVector3 K2Vect(Kp3CM[K2Idx]*sin(acos(KcosthCM[K2Idx]))*cos(KphiCM[K2Idx]),
		      Kp3CM[K2Idx]*sin(acos(KcosthCM[K2Idx]))*sin(KphiCM[K2Idx]),
		      Kp3CM[K2Idx]*KcosthCM[K2Idx]);
      TLorentzVector K2p4(K2Vect,KenergyCM[K2Idx]);

      TVector3 PiVect(Pip3CM[PiIdx]*sin(acos(PicosthCM[PiIdx]))*cos(PiphiCM[PiIdx]),
		      Pip3CM[PiIdx]*sin(acos(PicosthCM[PiIdx]))*sin(PiphiCM[PiIdx]),
		      Pip3CM[PiIdx]*PicosthCM[PiIdx]);
      TLorentzVector Pip4(PiVect,PienergyCM[PiIdx]);

 
      //basic cuts
      if(Gamp4.Vect().Unit()*DsStp4.Vect().Unit()<0)continue;
 
      if(GamClosestBumpDist[GamIdx]<15||GamClosestTrkDist[GamIdx]<15)continue;
      
      Int_t K1Pid=0;
      for(Int_t i=19;i<=23;i++)//NEW PID; 19=notpion,20=veryloose,21=loose,22=tight,23=verytight
        if((KSelectorsMap[KTrkIdx[K1Idx]] & (1<<i)) == (1<<i))
          K1Pid++;//if passes selector increase
      Int_t K2Pid=0;
      for(Int_t i=19;i<=23;i++)//NEW PID; 19=notpion,20=veryloose,21=loose,22=tight,23=verytight
        if((KSelectorsMap[KTrkIdx[K2Idx]] & (1<<i)) == (1<<i))
          K2Pid++;//if passes selector increase
       
      //cut D*+-->D0(KK)pi+
      HKKMassData.Fill((K1p4+K2p4).Mag());
      if(Dsd1Lund[DsIdx]*Dsd3Lund[DsIdx]<0)HKpiMassData.Fill((K1p4+Pip4).Mag());
      if(Dsd2Lund[DsIdx]*Dsd3Lund[DsIdx]<0)HKpiMassData.Fill((K2p4+Pip4).Mag());
      HD0KKDeltaMData.Fill(DsMass[DsIdx]-(K1p4+K2p4).Mag());

      //PID Sys: apply pid to one K to clean bkg
      if(K2Pid>=2&&fabs(DsMass[DsIdx]-(K1p4+K2p4).Mag()-.1454)>.005)
	if(.120<(DsStMass[DsStIdx]-DsMass[DsIdx])&&(DsStMass[DsStIdx]-DsMass[DsIdx])<.160){
	  HDsMassDataNoPid.Fill(DsMass[DsIdx]); 
	  if(1.93<DsMass[DsIdx]&&DsMass[DsIdx]<2.00)HDsMassDataNoPidSigSide.Fill(DsMass[DsIdx]);
	  if(1.93-.004-.035<DsMass[DsIdx]&&DsMass[DsIdx]<1.93-.004)HDsMassDataNoPidLoSide.Fill(DsMass[DsIdx]);	
	  if(2.00+.004<DsMass[DsIdx]&&DsMass[DsIdx]<2.00+.004+.035)HDsMassDataNoPidHiSide.Fill(DsMass[DsIdx]);
	}


      if(K1Pid<2||K2Pid<2)continue;
      if(fabs(DsMass[DsIdx]-(K1p4+K2p4).Mag()-.1454)<.005) continue;


      HDsMassDataRaw.Fill(DsMass[DsIdx]);
      if(fabs(DsMass[DsIdx]-(1.970))<.02){
	HDsMassDataRawCut.Fill(DsMass[DsIdx]);
	HDsStMassData.Fill(DsStMass[DsStIdx]);
	HDsStDeltaMDataRaw.Fill(DsStMass[DsStIdx]-DsMass[DsIdx]);

	//Dalitz regions
	if(.95<(K1p4+K2p4).Mag2()&&(K1p4+K2p4).Mag2()<1.15)
	  HDsStMassDataDalitz0.Fill(DsStMass[DsStIdx]);//Phi region
	else if((Dsd1Lund[DsIdx]*Dsd3Lund[DsIdx]<0&&.6<(K1p4+Pip4).Mag2()&&(K1p4+Pip4).Mag2()<1.)
		|| (Dsd2Lund[DsIdx]*Dsd3Lund[DsIdx]<0&&.6<(K2p4+Pip4).Mag2()&&(K2p4+Pip4).Mag2()<1.))
	  HDsStMassDataDalitz1.Fill(DsStMass[DsStIdx]);//K2 pi : K* region
	else  HDsStMassDataDalitz2.Fill(DsStMass[DsStIdx]);
	
      }

      if(.120<(DsStMass[DsStIdx]-DsMass[DsIdx])&&(DsStMass[DsStIdx]-DsMass[DsIdx])<.160){
	if(fabs(DsMass[DsIdx]-(1.970))<.02)HDsStDeltaMDataRawCut.Fill(DsStMass[DsStIdx]-DsMass[DsIdx]);
	
	HDsMassData.Fill(DsMass[DsIdx]);
	if(1.93<DsMass[DsIdx]&&DsMass[DsIdx]<2.00)HDsMassDataSigSide.Fill(DsMass[DsIdx]);	
	if(1.93-.004-.035<DsMass[DsIdx]&&DsMass[DsIdx]<1.93-.004)HDsMassDataLoSide.Fill(DsMass[DsIdx]);	
	if(2.00+.004<DsMass[DsIdx]&&DsMass[DsIdx]<2.00+.004+.035)HDsMassDataHiSide.Fill(DsMass[DsIdx]);	

	if(1.93<DsMass[DsIdx]&&DsMass[DsIdx]<2.00)
	  HDsVtxChi2Data.Fill(log10(TMath::Prob(DsChi2[DsIdx],DsnDof[DsIdx])));
	if(1.93-.004-.035<DsMass[DsIdx]&&DsMass[DsIdx]<1.93-.004)
	  HDsVtxChi2DataLoSide.Fill(log10(TMath::Prob(DsChi2[DsIdx],DsnDof[DsIdx])));
	if(2.00+.004<DsMass[DsIdx]&&DsMass[DsIdx]<2.00+.004+.035)
	  HDsVtxChi2DataHiSide.Fill(log10(TMath::Prob(DsChi2[DsIdx],DsnDof[DsIdx])));

	//Dalitz regions
	if(.95<(K1p4+K2p4).Mag2()&&(K1p4+K2p4).Mag2()<1.15)
	  HDsMassDataDalitz0.Fill(DsMass[DsIdx]);//Phi region
	else if((Dsd1Lund[DsIdx]*Dsd3Lund[DsIdx]<0&&.6<(K1p4+Pip4).Mag2()&&(K1p4+Pip4).Mag2()<1.)
		|| (Dsd2Lund[DsIdx]*Dsd3Lund[DsIdx]<0&&.6<(K2p4+Pip4).Mag2()&&(K2p4+Pip4).Mag2()<1.))
	  HDsMassDataDalitz1.Fill(DsMass[DsIdx]);//K2 pi : K* region
	else  HDsMassDataDalitz2.Fill(DsMass[DsIdx]);

      }

    }
   
  }

  delete tdata;
  

  ///////////////////////////////////////////////////////////////
  TFile HistsFile("CreateDsToKKpiNoDenom_New.root","recreate");
  HDsMassMCRaw.Write();  
  HDsMassDataRaw.Write(); 
  HDsMassMCRawCut.Write();  
  HDsMassDataRawCut.Write(); 

  HDsStMassMC.Write(); 
  HDsStMassData.Write();

  HDsStMassMCSig.Write();
  HDsStMassMCBkg.Write(); 

  HDsStMassMCDalitz0.Write();
  HDsStMassMCDalitz1.Write();
  HDsStMassMCDalitz2.Write();
  HDsStMassMCBkgDalitz0.Write();
  HDsStMassMCBkgDalitz1.Write();
  HDsStMassMCBkgDalitz2.Write();
  HDsStMassDataDalitz0.Write();
  HDsStMassDataDalitz1.Write();
  HDsStMassDataDalitz2.Write();


  HDsStDeltaMMCRaw.Write(); 
  HDsStDeltaMDataRaw.Write(); 
  HDsStDeltaMMCRawCut.Write(); 
  HDsStDeltaMDataRawCut.Write(); 

  HDsMassMC.Write();  
  HDsMassData.Write();

  HDsMassMCDalitz0.Write();
  HDsMassMCDalitz1.Write();
  HDsMassMCDalitz2.Write();
  HDsMassMCBkgDalitz0.Write();
  HDsMassMCBkgDalitz1.Write();
  HDsMassMCBkgDalitz2.Write();
  HDsMassDataDalitz0.Write();
  HDsMassDataDalitz1.Write();
  HDsMassDataDalitz2.Write();

  HDsMassMCDalitzRes0.Write();
  HDsMassMCDalitzRes1.Write();
  HDsMassMCDalitzRes2.Write();



  HDsMassMCSig.Write();
  HDsMassMCBkg.Write();

  HDsMassMCNoPid.Write();
  HDsMassMCNoPidBkg.Write();
  HDsMassDataNoPid.Write();

  HDsMassDataSigSide.Write();
  HDsMassDataLoSide.Write();
  HDsMassDataHiSide.Write();
  HDsMassDataNoPidSigSide.Write();
  HDsMassDataNoPidLoSide.Write();
  HDsMassDataNoPidHiSide.Write();

  HDsMassMCSigSide.Write();
  HDsMassMCLoSide.Write();
  HDsMassMCHiSide.Write();
  HDsMassMCNoPidSigSide.Write();
  HDsMassMCNoPidLoSide.Write();
  HDsMassMCNoPidHiSide.Write();


  HDsVtxChi2MC.Write();
  HDsVtxChi2Data.Write();
  HDsVtxChi2MCLoSide.Write();
  HDsVtxChi2DataLoSide.Write();
  HDsVtxChi2MCHiSide.Write();
  HDsVtxChi2DataHiSide.Write();

  HDsVtxChi2MCSig.Write();
  HDsVtxChi2MCBkg.Write();

  HKKMassMC.Write();  
  HKKMassData.Write();
  HKpiMassMC.Write();  
  HKpiMassData.Write();  
  HD0KKDeltaMMC.Write();
  HD0KKDeltaMData.Write();

  HistsFile.ls();
  HistsFile.Close();
}

void PlotDsToKKpiNoDenom(){
  
  TFile HistsFile("DsTolnu/KKpi/NoDenom/CreateDsToKKpiNoDenom.root","read"); 
  gROOT->cd();

  TCanvas C;
  C.Print("DsTolnu/KKpi/NoDenom/analyzeDsToKKpiNoDenom.ps[");
  
  
  TF1 FDsSt("FDsSt","[0]+[1]*x+[2]*(exp(-.5*(x-[3])**2/[4]**2)/(sqrt(2*3.14159)*[4])+[5]*exp(-.5*(x-[6])**2/[7]**2)/(sqrt(2*3.14159)*[7]))",2.0,2.2);
  FDsSt.SetParLimits(0,0,100000);
  FDsSt.SetParLimits(1,-1000,1000);
  FDsSt.SetParLimits(2,0,100000);
  FDsSt.SetParLimits(3,2.09,2.13);
  FDsSt.SetParLimits(4,.001,.02);
  FDsSt.SetParLimits(5,.001,1);
  FDsSt.SetParLimits(6,2.09,2.13);
  FDsSt.SetParLimits(7,.001,.02);


  //-----------------------------
  //show the Ds Mass without Ds* requirement
  //---------------------------- 
//   TH1F*HDsMassDataRaw=(TH1F*)HistsFile.Get("HDsMassDataRaw");
//   TH1F*HDsMassMCRaw=(TH1F*)HistsFile.Get("HDsMassMCRaw");
//   C.Clear();
//   C.Divide(1,2);
//   C.cd(1);
//   HDsMassDataRaw->GetYaxis()->SetRangeUser(0,HDsMassDataRaw->GetMaximum());
//   HDsMassDataRaw->GetXaxis()->SetTitle("m(KK#pi)  (GeV/c^{2})");
//   HDsMassDataRaw->Draw("pe");
//   C.cd(2);
//   HDsMassMCRaw->GetYaxis()->SetRangeUser(0,HDsMassMCRaw->GetMaximum());
//   HDsMassMCRaw->GetXaxis()->SetTitle("m(KK#pi)  (GeV/c^{2})");
//   HDsMassMCRaw->Draw("");
//   C.Print("DsTolnu/KKpi/NoDenom/analyzeDsToKKpiNoDenom.ps");

  //-----------------------------
  //show the Ds* Mass with Ds cut
  //---------------------------- 
  TH1F*HDsStMassData=(TH1F*)HistsFile.Get("HDsStMassData");
  TH1F*HDsStMassMC=(TH1F*)HistsFile.Get("HDsStMassMC");
  TH1F*HDsStMassMCBkg=(TH1F*)HistsFile.Get("HDsStMassMCBkg");
  TH1F*HDsStMassMCSig=(TH1F*)HistsFile.Get("HDsStMassMCSig");
  C.Clear();
  C.Divide(1,2);
  C.cd(1);
  HDsStMassData->GetYaxis()->SetRangeUser(0,HDsStMassData->GetMaximum());
  HDsStMassData->GetXaxis()->SetTitle("m(KK#pi#gamma)  (GeV/c^{2})");
  HDsStMassData->GetYaxis()->SetTitle("Entries/2 MeV");
  HDsStMassData->Draw("pe");
  C.cd(2);
  HDsStMassMC->GetYaxis()->SetRangeUser(0,HDsStMassMC->GetMaximum());
  HDsStMassMC->GetXaxis()->SetTitle("m(KK#pi#gamma)  (GeV/c^{2})");
  HDsStMassMC->GetYaxis()->SetTitle("Entries/2 MeV");
  HDsStMassMC->Draw("pe");
  HDsStMassMCBkg->Draw("same");
  C.Print("DsTolnu/KKpi/NoDenom/analyzeDsToKKpiNoDenom.ps");


  cout<<endl<<endl<<endl;
  cout<<"--------------------------------Fit Model systematic--------------------"<<endl;
  /////////////Fit the Ds* mass 
  RooRealVar dsstmass("dsstmass","m(KK#pi#gamma)",HDsStMassMCSig->GetXaxis()->GetXmin(),HDsStMassMCSig->GetXaxis()->GetXmax());
  dsstmass.setBins(HDsStMassMCSig->GetXaxis()->GetNbins());
  dsstmass.setUnit("GeV/c^{2}");
  RooArgSet dsstmasset(dsstmass);
  RooPlot* dsstplot;    
  RooDataHist DsStMassMCSigData("DsStMassMCSigData","",RooArgList(dsstmass),HDsStMassMCSig);
  RooDataHist DsStMassMCBkgData("DsStMassMCBkgData","",RooArgList(dsstmass),HDsStMassMCBkg);
  RooDataHist DsStMassData("DsStMassData","",RooArgList(dsstmass),HDsStMassData);
  DRecoHistPdf DsStSigPdf("DsStSigPdf","",dsstmasset,DsStMassMCSigData);
  DRecoHistPdf DsStBkgPdf("DsStBkgPdf","",dsstmasset,DsStMassMCBkgData);
  RooRealVar yield("yield","yield",.01,.5);
  DRecoAddPdf DsStPdf("DsStPdf","",dsstmasset,DsStSigPdf,DsStBkgPdf,yield);
  
  C.Clear();
  TH1F*HDsStSigPdf=(TH1F*)DsStSigPdf.createHistogram("HDsStSigPdf",dsstmass);
  HDsStSigPdf->Scale(1./HDsStSigPdf->Integral());
  TH1F*HDsStBkgPdf=(TH1F*)DsStBkgPdf.createHistogram("HDsStBkgPdf",dsstmass);
  HDsStBkgPdf->Scale(1./HDsStBkgPdf->Integral());
  HDsStSigPdf->SetLineColor(4);
  HDsStSigPdf->SetTitle("");
  HDsStSigPdf->Draw("hist");
  HDsStBkgPdf->SetLineColor(4);
  HDsStBkgPdf->Draw("hist same");
  C.Print("DsTolnu/KKpi/NoDenom/analyzeDsToKKpiNoDenom.ps");

  DRecoHistPdfChi2Var chi2("chi2","chi2",DsStPdf,DsStMassData);
  RooMinuit minuit(chi2);
  minuit.migrad();
  dsstplot=dsstmass.frame();
  dsstplot->addTH1(HDsStMassData,"pe");
  TH1F*HDsStMassDataFit=(TH1F*)DsStPdf.createHistogram("HDsStMassDataFit",dsstmass);
  HDsStMassDataFit->Scale(HDsStMassData->Integral()/HDsStMassDataFit->Integral());
  HDsStMassDataFit->SetLineColor(4);
  dsstplot->addTH1(HDsStMassDataFit,"hist same");
  DRecoResiduals DsStDataResid("DsStDataResid",HDsStMassData,HDsStMassDataFit);
  dsstplot->SetTitle("");
  C.Clear();
  DsStDataResid.MakeFitPlotWithResiduals(&C,dsstplot);  
  C.Print("DsTolnu/KKpi/NoDenom/analyzeDsToKKpiNoDenom.ps");
  //delete dsstplot;

  ///-------------------refit with a linear background----------------------------
  //First rebin all histos
  TH1F HDsStMassDataRebin("HDsStMassDataRebin","",128-35,1.970+35*.002,2.226);
  TH1F HDsStMassMCRebin("HDsStMassMCRebin","",128-35,1.970+35*.002,2.226);
  TH1F HDsStMassMCSigRebin("HDsStMassMCSigRebin","",128-35,1.970+35*.002,2.226);
  TH1F HDsStMassMCBkgRebin("HDsStMassMCBkgRebin","",128-35,1.970+35*.002,2.226);
  for(Int_t i=1;i<=(128-35);i++){
    HDsStMassDataRebin.SetBinContent(i,HDsStMassData->GetBinContent(i+35));
    HDsStMassDataRebin.SetBinError(i,HDsStMassData->GetBinError(i+35));
    HDsStMassMCRebin.SetBinContent(i,HDsStMassMC->GetBinContent(i+35));
    HDsStMassMCRebin.SetBinError(i,HDsStMassMC->GetBinError(i+35));
    HDsStMassMCSigRebin.SetBinContent(i,HDsStMassMCSig->GetBinContent(i+35));
    HDsStMassMCSigRebin.SetBinError(i,HDsStMassMCSig->GetBinError(i+35));
    HDsStMassMCBkgRebin.SetBinContent(i,HDsStMassMCBkg->GetBinContent(i+35));
    HDsStMassMCBkgRebin.SetBinError(i,HDsStMassMCBkg->GetBinError(i+35));
  }
//   ///first fit the MC bkg to show it is linear
//   C.Clear();
//   HDsStMassMCBkgRebin.Fit("pol1");
//   HDsStMassMCBkgRebin.GetYaxis()->SetRangeUser(0,HDsStMassMCBkgRebin.GetMaximum()*1.4);
//   HDsStMassMCBkgRebin.Draw("pe");
//   C.Print("DsTolnu/KKpi/NoDenom/analyzeDsToKKpiNoDenom.ps");

  //now fit the Data 
  RooRealVar dsstmassRebin("dsstmassRebin","m(KK#pi#gamma)",HDsStMassMCSigRebin.GetXaxis()->GetXmin(),HDsStMassMCSigRebin.GetXaxis()->GetXmax());
  dsstmassRebin.setBins(HDsStMassMCSigRebin.GetXaxis()->GetNbins());
  dsstmassRebin.setUnit("GeV/c^{2}");
  RooArgSet dsstmassetRebin(dsstmassRebin);

  RooDataHist DsStMassMCSigDataLin("DsStMassMCSigDataLin","DsStMassMCSigDataLin",RooArgList(dsstmassRebin),&HDsStMassMCSigRebin);
  RooDataHist DsStMassDataLin("DsStMassDataLin","DsStMassDataLin",RooArgList(dsstmassRebin),&HDsStMassDataRebin);
  RooRealVar p1("p1","p1",-1,1);
  RooGenericPdf LinearPdf("LinearPdf","LinearPdf","1+p1*(dsstmassRebin-2.04)/(2.226-2.04)",RooArgList(p1,dsstmassRebin));
  DRecoHistPdf DsStSigPdfLin("DsStSigPdfLin","DsStSigPdfLin",dsstmassetRebin,DsStMassMCSigDataLin);
  DRecoAddPdf DsStPdfLin("DsStPdfLin","DsStPdfLin",dsstmassetRebin,DsStSigPdfLin,LinearPdf,yield); 

  DRecoHistPdfChi2Var chi2Lin("chi2Lin","chi2Lin",DsStPdfLin,DsStMassDataLin);
  RooMinuit minuitLin(chi2Lin);
  minuitLin.migrad();
  dsstplot=dsstmassRebin.frame();
  dsstplot->addTH1(&HDsStMassDataRebin,"pe");
  TH1F*HDsStMassDataFitLin=(TH1F*)DsStPdfLin.createHistogram("HDsStMassDataFitLin",dsstmassRebin);
  HDsStMassDataFitLin->Scale(HDsStMassDataRebin.Integral()/HDsStMassDataFitLin->Integral());
  HDsStMassDataFitLin->SetLineColor(4);
  dsstplot->addTH1(HDsStMassDataFitLin,"hist same");
  DRecoResiduals DsStDataResidLin("DsStDataResidLin",&HDsStMassDataRebin,HDsStMassDataFitLin);
  dsstplot->SetTitle("");
  C.Clear();
  DsStDataResidLin.MakeFitPlotWithResiduals(&C,dsstplot);  
  C.Print("DsTolnu/KKpi/NoDenom/analyzeDsToKKpiNoDenom.ps");
  //delete dsstplot;
  cout<<"Data fit sig yield= "<< yield.getVal()*HDsStMassDataRebin.Integral()
      <<" +- "<< yield.getError()*HDsStMassDataRebin.Integral()<<endl;
//   ///Estimate a systematic difference between True signal yield and fitted yield
//   Float_t Diff=0.;
//   Float_t TotWeight=0.;
//   //for(Int_t i=11;i<=55;i++){//weight down by the statistical errors
//   for(Int_t i=1;i<=HDsStMassDataRebin.GetXaxis()->GetNbins();i++){//weight down by the statistical errors
//     Diff+=fabs(HDsStMassDataRebin.GetBinContent(i)-HDsStMassDataFitLin->GetBinContent(i))/HDsStMassDataRebin.GetBinError(i);
//     TotWeight+=1./HDsStMassDataRebin.GetBinError(i);
//   }
//   cout<<" Difference between Fit and Data Ds* yield = "<< Diff/(TotWeight*yield.getVal()*HDsStMassDataRebin.Integral()) <<endl;
  

  
//   //refit Data using MC Bkg in linear range
//   RooDataHist DsStMassMCBkgLin("DsStMassMCBkgLin","DsStMassMCBkgLin",RooArgList(dsstmassRebin),&HDsStMassMCBkgRebin);
//   DRecoHistPdf DsStBkgPdfLin("DsStBkgPdfLin","DsStBkgPdfLin",dsstmassetRebin,DsStMassMCBkgLin);
//   DRecoAddPdf DsStPdfLinMCBkg("DsStPdfLinMCBkg","DsStPdfLinMCBkg",dsstmassetRebin,DsStSigPdfLin,DsStBkgPdfLin,yield); 
//   DRecoHistPdfChi2Var chi2LinMCBkg("chi2LinMCBkg","chi2LinMCBkg",DsStPdfLinMCBkg,DsStMassDataLin);
//   RooMinuit minuitLinMCBkg(chi2LinMCBkg);
//   minuitLinMCBkg.migrad();
//   dsstplot=dsstmassRebin.frame();
//   dsstplot->addTH1(&HDsStMassDataRebin,"pe");
//   TH1F*HDsStMassDataFitLinMCBkg=(TH1F*)DsStPdfLinMCBkg.createHistogram("HDsStMassDataFitLinMCBkg",dsstmassRebin);
//   HDsStMassDataFitLinMCBkg->Scale(HDsStMassDataRebin.Integral()/HDsStMassDataFitLinMCBkg->Integral());
//   HDsStMassDataFitLinMCBkg->SetLineColor(4);
//   dsstplot->addTH1(HDsStMassDataFitLinMCBkg,"hist same");
//   DRecoResiduals DsStDataResidLinMCBkg("DsStDataResidLinMCBkg",&HDsStMassDataRebin,HDsStMassDataFitLinMCBkg);
//   dsstplot->SetTitle("");
//   C.Clear();
//   DsStDataResidLinMCBkg.MakeFitPlotWithResiduals(&C,dsstplot);  
//   C.Print("DsTolnu/KKpi/NoDenom/analyzeDsToKKpiNoDenom.ps");
//   //delete dsstplot;
//   cout<<"Data fit sig yield (MC Bkg)= "<< yield.getVal()*HDsStMassDataRebin.Integral()
//       <<" +- "<< yield.getError()*HDsStMassDataRebin.Integral()<<endl;

  //refit the Data using 2nd order poly
  RooRealVar c1("c1","c1",-10,10);
  RooRealVar c2("c2","c2",-10,10);
  //RooGenericPdf NonLinearPdf("NonLinearPdf","NonLinearPdf","2+c1*(dsstmassRebin-2.04)/(2.226-2.04)+c2*((dsstmassRebin-2.04)/(2.226-2.04))**2",RooArgList(c1,c2,dsstmassRebin));
  //RooGenericPdf NonLinearPdf("NonLinearPdf","NonLinearPdf","(1-c1/2-c2/3)/(2.226-2.04)+c1*(dsstmassRebin-2.04)/(2.226-2.04)+c2*((dsstmassRebin-2.04)/(2.226-2.04))**2",RooArgList(c1,c2,dsstmassRebin));
  RooChebychev NonLinearPdf("NonLinearPdf","NonLinearPdf",dsstmassRebin,RooArgList(c1,c2));
  DRecoAddPdf DsStPdfNonLin("DsStPdfNonLin","DsStPdfNonLin",dsstmassetRebin,DsStSigPdfLin,NonLinearPdf,yield); 
  DRecoHistPdfChi2Var chi2NonLin("chi2NonLin","chi2NonLin",DsStPdfNonLin,DsStMassDataLin);
  RooMinuit minuitNonLin(chi2NonLin);
  minuitNonLin.migrad();
  dsstplot=dsstmassRebin.frame();
  dsstplot->addTH1(&HDsStMassDataRebin,"pe");
  TH1F*HDsStMassDataFitNonLin=(TH1F*)DsStPdfNonLin.createHistogram("HDsStMassDataFitNonLin",dsstmassRebin);
  HDsStMassDataFitNonLin->Scale(HDsStMassDataRebin.Integral()/HDsStMassDataFitNonLin->Integral());
  HDsStMassDataFitNonLin->SetLineColor(4);
  dsstplot->addTH1(HDsStMassDataFitNonLin,"hist same");
  DRecoResiduals DsStDataResidNonLin("DsStDataResidNonLin",&HDsStMassDataRebin,HDsStMassDataFitNonLin);
  dsstplot->SetTitle("");
  C.Clear();
  DsStDataResidNonLin.MakeFitPlotWithResiduals(&C,dsstplot);  
  C.Print("DsTolnu/KKpi/NoDenom/analyzeDsToKKpiNoDenom.ps");
  //delete dsstplot;
  cout<<"Data fit sig yield (NonLinear)= "<< yield.getVal()*HDsStMassDataRebin.Integral()
      <<" +- "<< yield.getError()*HDsStMassDataRebin.Integral()<<endl;

  
//   ///Fit the Data using 2 Gauss plus linear
//   RooRealVar g1("g1","g1",2.09,2.13);
//   RooRealVar g2("g2","g2",.004,.04);
//   RooRealVar g3("g3","g3",.01,1);
//   RooRealVar g4("g4","g4",2.09,2.13);
//   RooRealVar g5("g5","g5",.005,.04);
//   RooGenericPdf GaussPdf("GaussPdf","GaussPdf","(1-g3)*exp(-.5*(dsstmassRebin-g1)**2/g2**2)/(sqrt(2*3.14159)*g2)+g3*exp(-.5*(dsstmassRebin-g4)**2/g5**2)/(sqrt(2*3.14159)*g5)",RooArgList(dsstmassRebin,g1,g2,g3,g4,g5));
//   DRecoAddPdf DsStPdf2GaussLin("DsStPdf2GaussLin","DsStPdf2GaussLin",dsstmassetRebin,GaussPdf,LinearPdf,yield); 
//   DRecoHistPdfChi2Var chi22GaussLin("chi22GaussLin","chi22GaussLin",DsStPdf2GaussLin,DsStMassDataLin);
//   RooMinuit minuit2GaussLin(chi22GaussLin);
//   minuit2GaussLin.migrad();
//   dsstplot=dsstmassRebin.frame();
//   dsstplot->addTH1(&HDsStMassDataRebin,"pe");
//   TH1F*HDsStMassDataFit2GaussLin=(TH1F*)DsStPdf2GaussLin.createHistogram("HDsStMassDataFit2GaussLin",dsstmassRebin);
//   HDsStMassDataFit2GaussLin->Scale(HDsStMassDataRebin.Integral()/HDsStMassDataFit2GaussLin->Integral());
//   HDsStMassDataFit2GaussLin->SetLineColor(4);
//   dsstplot->addTH1(HDsStMassDataFit2GaussLin,"hist same");
//   DRecoResiduals DsStDataResid2GaussLin("DsStDataResid2GaussLin",&HDsStMassDataRebin,HDsStMassDataFit2GaussLin);
//   dsstplot->SetTitle("");
//   C.Clear();
//   DsStDataResid2GaussLin.MakeFitPlotWithResiduals(&C,dsstplot);  
//   C.Print("DsTolnu/KKpi/NoDenom/analyzeDsToKKpiNoDenom.ps");
//   //delete dsstplot;
//   cout<<"Data fit sig yield (2GaussSignal+Linear)= "<< yield.getVal()*HDsStMassDataRebin.Integral()
//       <<" +- "<< yield.getError()*HDsStMassDataRebin.Integral()<<endl;

  ///Fit the Data using Signal MC  shifted by .5MeV
  RooRealVar res2("res2","res2",.001);
  RooGenericPdf ResPdf("ResPdf","ResPdf","exp(-.5*(dsstmassRebin-.0005)**2/res2**2)/(sqrt(2*3.14159)*res2)",RooArgList(dsstmassRebin,res2));
  RooNumConvPdf DsStSigPdfConv("DsStSigPdfConv","DsStSigPdfConv",dsstmassRebin,ResPdf,DsStSigPdfLin);
  DRecoAddPdf DsStPdfConvLin("DsStPdfConvLin","DsStPdfConvLin",dsstmassetRebin,DsStSigPdfConv,LinearPdf,yield); 
  DRecoHistPdfChi2Var chi2ConvLin("chi2ConvLin","chi2ConvLin",DsStPdfConvLin,DsStMassDataLin);
  RooMinuit minuitConvLin(chi2ConvLin);
  minuitConvLin.migrad();
  dsstplot=dsstmassRebin.frame();
  dsstplot->addTH1(&HDsStMassDataRebin,"pe");
  TH1F*HDsStMassDataFitConvLin=(TH1F*)DsStPdfConvLin.createHistogram("HDsStMassDataFitConvLin",dsstmassRebin);
  HDsStMassDataFitConvLin->Scale(HDsStMassDataRebin.Integral()/HDsStMassDataFitConvLin->Integral());
  HDsStMassDataFitConvLin->SetLineColor(4);
  dsstplot->addTH1(HDsStMassDataFitConvLin,"hist same");
  DRecoResiduals DsStDataResidConvLin("DsStDataResidConvLin",&HDsStMassDataRebin,HDsStMassDataFitConvLin);
  dsstplot->SetTitle("");
  C.Clear();
  DsStDataResidConvLin.MakeFitPlotWithResiduals(&C,dsstplot);  
  C.Print("DsTolnu/KKpi/NoDenom/analyzeDsToKKpiNoDenom.ps");
  //delete dsstplot;
  cout<<"Data fit sig yield (MCConv+Linear)= "<< yield.getVal()*HDsStMassDataRebin.Integral()
      <<" +- "<< yield.getError()*HDsStMassDataRebin.Integral()<<endl;


  //fit the MC with MC signal and Linear Bkg
  RooDataHist DsStMassMCLin("DsStMassMCLin","DsStMassMCLin",RooArgList(dsstmassRebin),&HDsStMassMCRebin);
  DRecoHistPdfChi2Var chi2LinMC("chi2LinMC","chi2LinMC",DsStPdfLin,DsStMassMCLin);
  RooMinuit minuitLinMC(chi2LinMC);
  minuitLinMC.migrad();
  dsstplot=dsstmassRebin.frame();
  dsstplot->addTH1(&HDsStMassMCRebin,"pe");
  TH1F*HDsStMassMCFitLin=(TH1F*)DsStPdfLin.createHistogram("HDsStMassMCFitLin",dsstmassRebin);
  HDsStMassMCFitLin->Scale(HDsStMassMCRebin.Integral()/HDsStMassMCFitLin->Integral());
  HDsStMassMCFitLin->SetLineColor(4);
  dsstplot->addTH1(HDsStMassMCFitLin,"hist same");
  DRecoResiduals DsStMCResidLin("DsStMCResidLin",&HDsStMassMCRebin,HDsStMassMCFitLin);
  dsstplot->SetTitle("");
  C.Clear();
  DsStMCResidLin.MakeFitPlotWithResiduals(&C,dsstplot);  
  C.Print("DsTolnu/KKpi/NoDenom/analyzeDsToKKpiNoDenom.ps");
  cout<<" MC fit sig yield= "<< yield.getVal()*HDsStMassMCRebin.Integral()
      <<" +- "<< yield.getError()*HDsStMassMCRebin.Integral()
      <<"   ----> True yield= "<<HDsStMassMCSigRebin.Integral()<<endl;
//   Float_t DiffMC=0.;
//   Float_t TotWeightMC=0.;
//   //for(Int_t i=11;i<=55;i++){//weight down by the statistical errors
//   for(Int_t i=1;i<=HDsStMassMCRebin.GetXaxis()->GetNbins();i++){//weight down by the statistical errors
//     DiffMC+=fabs(HDsStMassMCRebin.GetBinContent(i)-HDsStMassMCFitLin->GetBinContent(i))/HDsStMassMCRebin.GetBinError(i);
//     TotWeightMC+=1./HDsStMassMCRebin.GetBinError(i);
//   }
//  cout<<" Difference between Fit and MC Ds* yield = "<< DiffMC/(TotWeightMC*yield.getVal()*HDsStMassMCRebin.Integral()) <<endl;

  //fit the MC with 2Gauss and Linear Bkg
//   DRecoHistPdfChi2Var chi2GaussLinMC("chi2GaussLinMC","chi2GaussLinMC",DsStPdf2GaussLin,DsStMassMCLin);//DsStPdf2GaussNonLin 
//   RooMinuit minuitGaussLinMC(chi2GaussLinMC);
//   minuitGaussLinMC.migrad();
//   dsstplot=dsstmassRebin.frame();
//   dsstplot->addTH1(&HDsStMassMCRebin,"pe");
//   TH1F*HDsStMassMCFitGaussLin=(TH1F*)DsStPdf2GaussLin.createHistogram("HDsStMassMCFitGaussLin",dsstmassRebin);
//   HDsStMassMCFitGaussLin->Scale(HDsStMassMCRebin.Integral()/HDsStMassMCFitGaussLin->Integral());
//   HDsStMassMCFitGaussLin->SetLineColor(4);
//   dsstplot->addTH1(HDsStMassMCFitGaussLin,"hist same");
//   DRecoResiduals DsStMCResidGaussLin("DsStMCResidGaussLin",&HDsStMassMCRebin,HDsStMassMCFitGaussLin);
//   dsstplot->SetTitle("");
//   C.Clear();
//   DsStMCResidGaussLin.MakeFitPlotWithResiduals(&C,dsstplot);  
//   C.Print("DsTolnu/KKpi/NoDenom/analyzeDsToKKpiNoDenom.ps");
//   cout<<" MC fit sig yield (2Gauss+Linear)= "<< yield.getVal()*HDsStMassMCRebin.Integral()
//       <<" +- "<< yield.getError()*HDsStMassMCRebin.Integral()
//       <<"   ----> True yield= "<<HDsStMassMCSigRebin.Integral()<<endl;


  cout<<endl<<endl<<endl;
  cout<<"--------------------------------Ds Mass cut systematic--------------------"<<endl;
  ///-------------------------------------------------------------
  //Show Ds* DeltaM with Ds cut
  //-----------------------------------------------------------
  TH1F*HDsStDeltaMDataRaw=(TH1F*)HistsFile.Get("HDsStDeltaMDataRaw");
  TH1F*HDsStDeltaMMCRaw=(TH1F*)HistsFile.Get("HDsStDeltaMMCRaw");
  TH1F*HDsStDeltaMDataRawCut=(TH1F*)HistsFile.Get("HDsStDeltaMDataRawCut");
  TH1F*HDsStDeltaMMCRawCut=(TH1F*)HistsFile.Get("HDsStDeltaMMCRawCut");
  C.Clear();
  C.Divide(1,2);
  C.cd(1);
  HDsStDeltaMDataRaw->GetYaxis()->SetRangeUser(0,HDsStDeltaMDataRaw->GetMaximum());
  HDsStDeltaMDataRaw->GetXaxis()->SetTitle("m(KK#pi#gamma)-m(KK#pi)  (GeV/c^{2})");
  HDsStDeltaMDataRaw->Draw("pe");
  HDsStDeltaMDataRawCut->Draw("same");
  C.cd(2);
  HDsStDeltaMMCRaw->GetYaxis()->SetRangeUser(0,HDsStDeltaMMCRaw->GetMaximum());
  HDsStDeltaMMCRaw->GetXaxis()->SetTitle("m(KK#pi#gamma)-m(KK#pi)  (GeV/c^{2})");
  HDsStDeltaMMCRaw->Draw("");
  HDsStDeltaMMCRawCut->Draw("same");
  C.Print("DsTolnu/KKpi/NoDenom/analyzeDsToKKpiNoDenom.ps");

  ///----------------------------------------------------------
  //Study the shape of the Ds mass peak
  //-----------------------------------------------------------
  TH1F*HDsMassData=(TH1F*)HistsFile.Get("HDsMassData");
  HDsMassData->GetXaxis()->SetTitle("m(KK#pi)  (GeV/c^{2})");
  HDsMassData->GetYaxis()->SetTitle("Entries/2 MeV");
  TH1F*HDsMassMC=(TH1F*)HistsFile.Get("HDsMassMC");
  HDsMassMC->GetXaxis()->SetTitle("m(KK#pi)  (GeV/c^{2})");
  HDsMassMC->GetYaxis()->SetTitle("Entries/2 MeV");
  TH1F*HDsMassMCBkg=(TH1F*)HistsFile.Get("HDsMassMCBkg");
  TF1 FDs("FDs","[0]+[1]*x+([2]*.002)*((1-[5])*exp(-.5*(x-[3])**2/[4]**2)/(sqrt(2*3.14159)*[4])+[5]*exp(-.5*(x-[6])**2/[7]**2)/(sqrt(2*3.14159)*[7]))",1.9,2.04);
  TF1 FDsSig("FDsSig","([2]*.002)*((1-[5])*exp(-.5*(x-[3])**2/[4]**2)/(sqrt(2*3.14159)*[4])+[5]*exp(-.5*(x-[6])**2/[7]**2)/(sqrt(2*3.14159)*[7]))",1.9,2.04);
  FDs.SetParLimits(0,-100000,100000);
  FDs.SetParLimits(1,-10000,10000);
  FDs.SetParLimits(2,0,100000);
  FDs.SetParLimits(3,1.95,1.99);
  FDs.SetParLimits(4,.001,.02);
  FDs.SetParLimits(5,.001,.5);
  FDs.SetParLimits(6,1.95,1.99);
  FDs.SetParLimits(7,.001,.02);
  FDs.SetParName(0,"y0");
  FDs.SetParName(1,"slope");
  FDs.SetParName(2,"N");
  FDs.SetParName(3,"m1");
  FDs.SetParName(4,"s1");
  FDs.SetParName(5,"r");
  FDs.SetParName(6,"m2");
  FDs.SetParName(7,"s2");


  RooRealVar dsmass("dsmass","m(KK#pi)",HDsMassData->GetXaxis()->GetXmin(),HDsMassData->GetXaxis()->GetXmax());
  dsmass.setBins(HDsMassData->GetXaxis()->GetNbins());
  dsmass.setUnit("GeV/c^{2}");
  RooArgSet dsmassset(dsmass);
  C.Clear();
  TH1F*HDsMassDataCl=(TH1F*)(HDsMassData->Clone("HDsMassDataCl"));
  HDsMassDataCl->Fit(&FDs,"QILL","",1.89,2.05);
  HDsMassDataCl->GetYaxis()->SetRangeUser(0,HDsMassDataCl->GetMaximum());
  TH1F* HDsMassDataFit=(TH1F*)HDsMassDataCl->Clone("HDsMassDataFit");
  for(Int_t i=1;i<=HDsMassDataFit->GetXaxis()->GetNbins();i++){
    HDsMassDataFit->SetBinContent(i,FDs.Eval( HDsMassDataFit->GetBinCenter(i)));
    HDsMassDataFit->SetBinError(i,0);
  }
  DRecoResiduals DsDataResid("DsDataResid",HDsMassDataCl,HDsMassDataFit);
  RooPlot*plot=dsmass.frame();
  plot->SetTitle("");
  plot->addTH1(HDsMassDataCl,"pe");
  DsDataResid.MakeFitPlotWithResiduals(&C,plot);  
  C.Print("DsTolnu/KKpi/NoDenom/analyzeDsToKKpiNoDenom.ps");
  delete plot;
  FDsSig.SetParameter(2,FDs.GetParameter(2));
  FDsSig.SetParameter(3,FDs.GetParameter(3));
  FDsSig.SetParameter(4,FDs.GetParameter(4));
  FDsSig.SetParameter(5,FDs.GetParameter(5));
  FDsSig.SetParameter(6,FDs.GetParameter(6));
  FDsSig.SetParameter(7,FDs.GetParameter(7));
  cout<<"Data Integral in cut region ="<< 1.- FDsSig.Integral(1.93,2.00)/FDsSig.Integral(1.87,2.07)<<endl;
  Float_t SignalYieldData=FDsSig.GetParameter(2);
  Float_t SignalYieldDataEr=FDs.GetParError(2);
  cout<<"Total Data yield= "<< FDs.GetParameter(2)<<" +-"<<FDs.GetParError(2)<<endl;
  Float_t SigDataPars[8]={0,0,FDs.GetParameter(2),FDs.GetParameter(3),FDs.GetParameter(4),FDs.GetParameter(5),FDs.GetParameter(6),FDs.GetParameter(7)};


  C.Clear();
  TH1F*HDsMassMCCl=(TH1F*)(HDsMassMC->Clone("HDsMassMCCl"));
  HDsMassMCCl->Fit(&FDs,"QILL","",1.89,2.05);
  HDsMassMCCl->GetYaxis()->SetRangeUser(0,HDsMassMCCl->GetMaximum());
  TH1F* HDsMassMCFit=(TH1F*)HDsMassMCCl->Clone("HDsMassMCFit");
  for(Int_t i=1;i<=HDsMassMCFit->GetXaxis()->GetNbins();i++){
    HDsMassMCFit->SetBinContent(i,FDs.Eval( HDsMassMCFit->GetBinCenter(i)));
    HDsMassMCFit->SetBinError(i,0);
  }
  DRecoResiduals DsMCResid("DsMCResid",HDsMassMCCl,HDsMassMCFit);
  RooPlot*mcplot=dsmass.frame();
  mcplot->SetTitle("");
  mcplot->addTH1(HDsMassMCCl,"pe");
  DsDataResid.MakeFitPlotWithResiduals(&C,mcplot);  
  C.cd(1);
  HDsMassMCBkg->Draw("same");
  C.Print("DsTolnu/KKpi/NoDenom/analyzeDsToKKpiNoDenom.ps");
  delete mcplot;
  FDsSig.SetParameter(2,FDs.GetParameter(2));
  FDsSig.SetParameter(3,FDs.GetParameter(3));
  FDsSig.SetParameter(4,FDs.GetParameter(4));
  FDsSig.SetParameter(5,FDs.GetParameter(5));
  FDsSig.SetParameter(6,FDs.GetParameter(6));
  FDsSig.SetParameter(7,FDs.GetParameter(7));
  cout<<"MC Integral in cut region ="<< 1.- FDsSig.Integral(1.93,2.00)/FDsSig.Integral(1.87,2.07)<<endl;
  Float_t SignalYieldMC=FDsSig.GetParameter(2);
  Float_t SignalYieldMCEr=FDs.GetParError(2);
  cout<<"Total MC yield= "<< FDs.GetParameter(2)<<" +-"<<FDs.GetParError(2)<<endl;
  Float_t SigMCPars[8]={0,0,FDs.GetParameter(2),FDs.GetParameter(3),FDs.GetParameter(4),FDs.GetParameter(5),FDs.GetParameter(6),FDs.GetParameter(7)};
  
//   //fit the background MC 
//   C.Clear();
//   HDsMassMCBkg->Fit("pol1");
//   HDsMassMCBkg->GetYaxis()->SetRangeUser(0,HDsMassMCBkg->GetMaximum()*1.6);
//   HDsMassMCBkg->Draw("pe");
//   C.Print("DsTolnu/KKpi/NoDenom/analyzeDsToKKpiNoDenom.ps");

//   //fit the Signal MC
//   TH1F*HDsMassMCSig=(TH1F*)HistsFile.Get("HDsMassMCSig");
//   C.Clear();
//   HDsMassMCSig->Fit(&FDs);
//   C.Print("DsTolnu/KKpi/NoDenom/analyzeDsToKKpiNoDenom.ps");
  

  cout<<endl<<endl<<endl<<endl<<"Vertex Prob Study-----------------------------------------------------"<<endl;
  ///----------------------------------------------------------
  //Study the shape of vertex prob
  //-----------------------------------------------------------
  TH1F*HDsVtxChi2Data=(TH1F*)HistsFile.Get("HDsVtxChi2Data");
  TH1F*HDsVtxChi2MC=(TH1F*)HistsFile.Get("HDsVtxChi2MC");
  TH1F*HDsVtxChi2DataLoSide=(TH1F*)HistsFile.Get("HDsVtxChi2DataLoSide");
  TH1F*HDsVtxChi2MCLoSide=(TH1F*)HistsFile.Get("HDsVtxChi2MCLoSide");
  TH1F*HDsVtxChi2DataHiSide=(TH1F*)HistsFile.Get("HDsVtxChi2DataHiSide");
  TH1F*HDsVtxChi2MCHiSide=(TH1F*)HistsFile.Get("HDsVtxChi2MCHiSide");
  //compare lo and hi Data
  C.Clear();
  delete HDsVtxChi2DataHiSide->FindObject("stats");
  HDsVtxChi2DataHiSide->GetYaxis()->SetRangeUser(0,HDsVtxChi2DataHiSide->GetMaximum());
  HDsVtxChi2DataHiSide->GetXaxis()->SetTitle("log(prob(#chi^{2}))");
  HDsVtxChi2DataHiSide->GetYaxis()->SetTitle("Entries/.05");
  HDsVtxChi2DataHiSide->Draw("pe");
  HDsVtxChi2DataLoSide->Draw("same");
  C.Print("DsTolnu/KKpi/NoDenom/analyzeDsToKKpiNoDenom.ps");
  
  //subtract bkg from Data then compare with Signal MC
  HDsVtxChi2DataHiSide->Add(HDsVtxChi2DataLoSide);
  HDsVtxChi2Data->Add(HDsVtxChi2DataHiSide,-1);
  C.Clear();
  delete HDsVtxChi2Data->FindObject("stats");
  HDsVtxChi2Data->GetYaxis()->SetRangeUser(0,HDsVtxChi2Data->GetMaximum()*1.3);
  HDsVtxChi2Data->GetXaxis()->SetTitle("log(prob(#chi^{2}))");
  HDsVtxChi2Data->GetYaxis()->SetTitle("Entries/.05");
  HDsVtxChi2Data->Draw("pe");
  TH1F*HDsVtxChi2MCSig=(TH1F*)HistsFile.Get("HDsVtxChi2MCSig");
  HDsVtxChi2MCSig->Scale(HDsVtxChi2Data->Integral()/HDsVtxChi2MCSig->Integral());
  HDsVtxChi2MCSig->Draw("same");
  C.Print("DsTolnu/KKpi/NoDenom/analyzeDsToKKpiNoDenom.ps");

  //show ratio
  HDsVtxChi2Data->Sumw2();
  HDsVtxChi2Data->Divide(HDsVtxChi2MCSig);
  C.Clear();
  delete HDsVtxChi2Data->FindObject("stats");
  HDsVtxChi2Data->SetStats(0);
  HDsVtxChi2Data->GetYaxis()->SetRangeUser(0,5);
  HDsVtxChi2Data->GetYaxis()->SetTitle("Data/MC");
  HDsVtxChi2Data->Draw("pe");
  HDsVtxChi2Data->Fit("pol1","QILL","",-5,0);
  C.Print("DsTolnu/KKpi/NoDenom/analyzeDsToKKpiNoDenom.ps");

  ///////////////-----------------------------------------------
  //Study the K pid efficiency
  //---------------------------------------------------------
  cout<<endl<<endl<<endl<<endl<<"PID Study-----------------------------------------------------"<<endl;
  TH1F*HDsMassDataNoPid=(TH1F*)HistsFile.Get("HDsMassDataNoPid");
  HDsMassDataNoPid->GetXaxis()->SetTitle("m(KK#pi)  (GeV/c^{2})");
  HDsMassDataNoPid->GetYaxis()->SetTitle("Entries/2 MeV");
  TH1F*HDsMassMCNoPid=(TH1F*)HistsFile.Get("HDsMassMCNoPid");
  HDsMassMCNoPid->GetXaxis()->SetTitle("m(KK#pi)  (GeV/c^{2})");
  HDsMassMCNoPid->GetYaxis()->SetTitle("Entries/2 MeV");
  TH1F*HDsMassMCNoPidBkg=(TH1F*)HistsFile.Get("HDsMassMCNoPidBkg");
 
//   C.Clear();
//   FDs.FixParameter(3,SigDataPars[3]);
//   FDs.FixParameter(4,SigDataPars[4]);
//   FDs.FixParameter(5,SigDataPars[5]);
//   FDs.FixParameter(6,SigDataPars[6]);
//   FDs.FixParameter(7,SigDataPars[7]);
//   TH1F*HDsMassDataNoPidCl=(TH1F*)(HDsMassDataNoPid->Clone("HDsMassDataNoPidCl"));
//   HDsMassDataNoPidCl->Fit(&FDs,"QILL","",1.89,2.05);
//   HDsMassDataNoPidCl->GetYaxis()->SetRangeUser(0,HDsMassDataNoPidCl->GetMaximum());
//   TH1F* HDsMassDataNoPidFit=(TH1F*)HDsMassDataNoPidCl->Clone("HDsMassDataNoPidFit");
//   for(Int_t i=1;i<=HDsMassDataNoPidFit->GetXaxis()->GetNbins();i++){
//     HDsMassDataNoPidFit->SetBinContent(i,FDs.Eval( HDsMassDataNoPidFit->GetBinCenter(i)));
//     HDsMassDataNoPidFit->SetBinError(i,0);
//   }
//   DRecoResiduals DsDataResidPid("DsDataResidPid",HDsMassDataNoPidCl,HDsMassDataNoPidFit);
//   plot=dsmass.frame();
//   plot->SetTitle("");
//   plot->addTH1(HDsMassDataNoPidCl,"pe");
//   DsDataResidPid.MakeFitPlotWithResiduals(&C,plot);  
//   C.Print("DsTolnu/KKpi/NoDenom/analyzeDsToKKpiNoDenom.ps");
//   delete plot;
//   cout<<"Data No Pid Yield ="<<FDs.GetParameter(2)<<" +-"<<FDs.GetParError(2)<<endl;
//   cout<<"Data PID Efficiency ="<<SignalYieldData/FDs.GetParameter(2)
//       <<"  +-"<<(SignalYieldData/FDs.GetParameter(2))*sqrt(pow(SignalYieldDataEr/SignalYieldData,2)+pow(FDs.GetParError(2)/FDs.GetParameter(2),2))<<endl;

//   C.Clear();
//   FDs.FixParameter(3,SigMCPars[3]);
//   FDs.FixParameter(4,SigMCPars[4]);
//   FDs.FixParameter(5,SigMCPars[5]);
//   FDs.FixParameter(6,SigMCPars[6]);
//   FDs.FixParameter(7,SigMCPars[7]);
//   TH1F*HDsMassMCNoPidCl=(TH1F*)(HDsMassMCNoPid->Clone("HDsMassMCNoPidCl"));
//   HDsMassMCNoPidCl->Fit(&FDs,"QILL","",1.89,2.05);
//   HDsMassMCNoPidCl->GetYaxis()->SetRangeUser(0,HDsMassMCNoPidCl->GetMaximum());
//   TH1F* HDsMassMCNoPidFit=(TH1F*)HDsMassMCNoPidCl->Clone("HDsMassMCNoPidFit");
//   for(Int_t i=1;i<=HDsMassMCNoPidFit->GetXaxis()->GetNbins();i++){
//     HDsMassMCNoPidFit->SetBinContent(i,FDs.Eval( HDsMassMCNoPidFit->GetBinCenter(i)));
//     HDsMassMCNoPidFit->SetBinError(i,0);
//   }
//   DRecoResiduals DsMCResidPid("DsMCResidPid",HDsMassMCNoPidCl,HDsMassMCNoPidFit);
//   mcplot=dsmass.frame();
//   mcplot->SetTitle("");
//   mcplot->addTH1(HDsMassMCNoPidCl,"pe");
//   DsDataResidPid.MakeFitPlotWithResiduals(&C,mcplot);  
//   C.Print("DsTolnu/KKpi/NoDenom/analyzeDsToKKpiNoDenom.ps");
//   delete mcplot;
//   cout<<"MC No Pid Yield ="<<FDs.GetParameter(2)<<" +-"<<FDs.GetParError(2)<<endl;
//   cout<<"MC PID efficiency ="<<SignalYieldMC/FDs.GetParameter(2)
//       <<"  +-"<<(SignalYieldMC/FDs.GetParameter(2))*sqrt(pow(SignalYieldMCEr/SignalYieldMC,2)+pow(FDs.GetParError(2)/FDs.GetParameter(2),2))<<endl;



//   cout<<endl<<endl;
//   //--------------------------------------------------------------------------------------------
//   ///Above method gives a large error on the yield because fit function has many parameters
//   //Below compute signal yields by using sideband subtraction
//   C.Clear();
//   delete HDsMassDataNoPid->FindObject("stats");
//   delete HDsMassDataNoPid->FindObject("FDs");
//   delete HDsMassData->FindObject("stats");
//   delete HDsMassData->FindObject("FDs");
//   HDsMassDataNoPid->SetStats(0);
//   HDsMassDataNoPid->GetYaxis()->SetRangeUser(0.,HDsMassDataNoPid->GetMaximum());
//   HDsMassDataNoPid->Draw("pe");  
//   HDsMassData->Draw("pe same");

//   TH1F*HDsMassDataNoPidSigSide=(TH1F*)HistsFile.Get("HDsMassDataNoPidSigSide");
//   HDsMassDataNoPidSigSide->SetFillColor(2);HDsMassDataNoPidSigSide->SetLineColor(0);
//   HDsMassDataNoPidSigSide->Draw("same");
//   TH1F*HDsMassDataNoPidLoSide=(TH1F*)HistsFile.Get("HDsMassDataNoPidLoSide");
//   HDsMassDataNoPidLoSide->SetFillColor(3);HDsMassDataNoPidLoSide->SetLineColor(0);
//   HDsMassDataNoPidLoSide->Draw("same");
//   TH1F*HDsMassDataNoPidHiSide=(TH1F*)HistsFile.Get("HDsMassDataNoPidHiSide");
//   HDsMassDataNoPidHiSide->SetFillColor(3);HDsMassDataNoPidHiSide->SetLineColor(0);
//   HDsMassDataNoPidHiSide->Draw("same");

//   TH1F*HDsMassDataSigSide=(TH1F*)HistsFile.Get("HDsMassDataSigSide");
//   HDsMassDataSigSide->SetFillColor(2);HDsMassDataSigSide->SetLineColor(0);
//   HDsMassDataSigSide->Draw("same");
//   TH1F*HDsMassDataLoSide=(TH1F*)HistsFile.Get("HDsMassDataLoSide");
//   HDsMassDataLoSide->SetFillColor(3);HDsMassDataLoSide->SetLineColor(0);
//   HDsMassDataLoSide->Draw("same");
//   TH1F*HDsMassDataHiSide=(TH1F*)HistsFile.Get("HDsMassDataHiSide");
//   HDsMassDataHiSide->SetFillColor(3);HDsMassDataHiSide->SetLineColor(0);
//   HDsMassDataHiSide->Draw("same");

//   HDsMassDataNoPid->Draw("pe same");  
//   HDsMassData->Draw("pe same");
// //   TLine cutline;
// //   cutline.SetLineColor(4);
// //   cutline.DrawLine(1.89,0,1.89,HDsMassDataNoPid->GetMaximum());  
// //   cutline.DrawLine(1.89+.036,0,1.89+.036,HDsMassDataNoPid->GetMaximum());
// //   cutline.DrawLine(2.004,0,2.004,HDsMassDataNoPid->GetMaximum());  
// //   cutline.DrawLine(2.004+.036,0,2.004+.036,HDsMassDataNoPid->GetMaximum());
// //   cutline.SetLineColor(2);
// //   cutline.DrawLine(1.93,0,1.93,HDsMassDataNoPid->GetMaximum());  
// //   cutline.DrawLine(2.00,0,2.00,HDsMassDataNoPid->GetMaximum());
//   C.Print("DsTolnu/KKpi/NoDenom/analyzeDsToKKpiNoDenom.ps");
//   //compute yield in sig region
//   Float_t DataSigRegion=HDsMassDataSigSide->Integral(); 
//   Float_t DataLoRegion=HDsMassDataLoSide->Integral();
//   Float_t DataHiRegion=HDsMassDataHiSide->Integral();
//   Float_t DataSigYield=DataSigRegion-(DataLoRegion+DataHiRegion);
//   Float_t DataSigYieldEr=sqrt(DataSigRegion+(DataLoRegion+DataHiRegion));
//   cout<<"Data Yields : "<<DataLoRegion<<" "<<DataSigRegion<<" "<<DataHiRegion<<endl;
//   Float_t DataSigRegionNoPid=HDsMassDataNoPidSigSide->Integral(); 
//   Float_t DataLoRegionNoPid=HDsMassDataNoPidLoSide->Integral();
//   Float_t DataHiRegionNoPid=HDsMassDataNoPidHiSide->Integral();
//   Float_t DataSigYieldNoPid=DataSigRegionNoPid-(DataLoRegionNoPid+DataHiRegionNoPid);
//   Float_t DataSigYieldNoPidEr=sqrt(DataSigRegionNoPid+(DataLoRegionNoPid+DataHiRegionNoPid));
//   cout<<"Data Yields No Pid: "<<DataLoRegionNoPid<<" "<<DataSigRegionNoPid<<" "<<DataHiRegionNoPid<<endl;
//   cout<<"Data Yields : "<< DataSigYield<<" +-"<<DataSigYieldEr<<" -->NoPid: "<<DataSigYieldNoPid<<" +-"<<DataSigYieldNoPidEr<<endl;
//   cout<<"Data PID efficiency ="<<DataSigYield/DataSigYieldNoPid
//       <<"  +-"<<(DataSigYield/DataSigYieldNoPid)*sqrt(pow(DataSigYieldEr/DataSigYield,2)+pow(DataSigYieldNoPidEr/DataSigYieldNoPid,2))<<endl;

  

//   C.Clear();
//   delete HDsMassMCNoPid->FindObject("stats");
//   delete HDsMassMCNoPid->FindObject("FDs");
//   delete HDsMassMC->FindObject("stats");
//   delete HDsMassMC->FindObject("FDs");
//   HDsMassMCNoPid->SetStats(0);
//   HDsMassMCNoPid->GetYaxis()->SetRangeUser(0.,HDsMassMCNoPid->GetMaximum());
//   HDsMassMCNoPid->Draw("pe");
//   HDsMassMCNoPidBkg->Draw("same");  
//   HDsMassMC->Draw("pe same");
//   HDsMassMCBkg->Draw("same");

//   TH1F*HDsMassMCNoPidSigSide=(TH1F*)HistsFile.Get("HDsMassMCNoPidSigSide");
//   HDsMassMCNoPidSigSide->SetFillColor(2);HDsMassMCNoPidSigSide->SetLineColor(0);
//   HDsMassMCNoPidSigSide->Draw("same");
//   TH1F*HDsMassMCNoPidLoSide=(TH1F*)HistsFile.Get("HDsMassMCNoPidLoSide");
//   HDsMassMCNoPidLoSide->SetFillColor(3);HDsMassMCNoPidLoSide->SetLineColor(0);
//   HDsMassMCNoPidLoSide->Draw("same");
//   TH1F*HDsMassMCNoPidHiSide=(TH1F*)HistsFile.Get("HDsMassMCNoPidHiSide");
//   HDsMassMCNoPidHiSide->SetFillColor(3);HDsMassMCNoPidHiSide->SetLineColor(0);
//   HDsMassMCNoPidHiSide->Draw("same");

//   TH1F*HDsMassMCSigSide=(TH1F*)HistsFile.Get("HDsMassMCSigSide");
//   HDsMassMCSigSide->SetFillColor(2);HDsMassMCSigSide->SetLineColor(0);
//   HDsMassMCSigSide->Draw("same");
//   TH1F*HDsMassMCLoSide=(TH1F*)HistsFile.Get("HDsMassMCLoSide");
//   HDsMassMCLoSide->SetFillColor(3);HDsMassMCLoSide->SetLineColor(0);
//   HDsMassMCLoSide->Draw("same");
//   TH1F*HDsMassMCHiSide=(TH1F*)HistsFile.Get("HDsMassMCHiSide");
//   HDsMassMCHiSide->SetFillColor(3);HDsMassMCHiSide->SetLineColor(0);
//   HDsMassMCHiSide->Draw("same");

//   HDsMassMCNoPid->Draw("pe same");
//   HDsMassMCNoPidBkg->Draw("same");  
//   HDsMassMC->Draw("pe same");
//   HDsMassMCBkg->Draw("same");

//   C.Print("DsTolnu/KKpi/NoDenom/analyzeDsToKKpiNoDenom.ps");
//   Float_t MCSigRegion=HDsMassMCSigSide->Integral(); 
//   Float_t MCLoRegion=HDsMassMCLoSide->Integral();
//   Float_t MCHiRegion=HDsMassMCHiSide->Integral();
//   Float_t MCSigRegionNoPid=HDsMassMCNoPidSigSide->Integral(); 
//   Float_t MCLoRegionNoPid=HDsMassMCNoPidLoSide->Integral();
//   Float_t MCHiRegionNoPid=HDsMassMCNoPidHiSide->Integral();
//   Float_t MCSigYield=MCSigRegion-(MCLoRegion+MCHiRegion);
//   Float_t MCSigYieldEr=sqrt(MCSigRegion+(MCLoRegion+MCHiRegion));
//   Float_t MCSigYieldNoPid=MCSigRegionNoPid-(MCLoRegionNoPid+MCHiRegionNoPid);
//   Float_t MCSigYieldNoPidEr=sqrt(MCSigRegionNoPid+(MCLoRegionNoPid+MCHiRegionNoPid));
//   cout<<"MC Yields : "<< MCSigYield<<" +-"<<MCSigYieldEr<<" -->NoPid: "<<MCSigYieldNoPid<<" +-"<<MCSigYieldNoPidEr<<endl;
//   cout<<"MC PID efficiency ="<<MCSigYield/MCSigYieldNoPid
//       <<"  +-"<<(MCSigYield/MCSigYieldNoPid)*sqrt(pow(MCSigYieldEr/MCSigYield,2)+pow(MCSigYieldNoPidEr/MCSigYieldNoPid,2))<<endl;

//   TH1F*HDsMassMCSig=(TH1F*)HistsFile.Get("HDsMassMCSig");
//   cout<<"True MC Sig yield "<<HDsMassMCSig->Integral(21,55)<<" --> No Pid "<<HDsMassMCNoPid->Integral(21,55)-HDsMassMCNoPidBkg->Integral(21,55)<<endl;
//   cout<<"True MC PID efficiency = "<<HDsMassMCSig->Integral(21,55)/(HDsMassMCNoPid->Integral(21,55)-HDsMassMCNoPidBkg->Integral(21,55))<<" +-"<<sqrt(HDsMassMCSig->Integral(21,55))/HDsMassMCSig->Integral(21,55)<<endl;
  


  cout<<endl<<endl;
  ////---------------------------------------------------------------------------------------
  ///perform a roofit where you float the efficiency and fix the shape parameters 
  TH1F*HDsMassMCNoPidClCl=(TH1F*)(HDsMassMCNoPid->Clone("HDsMassMCNoPidClCl"));
  RooDataHist DsMassMCSetNoPid("DsMassMCSetNoPid","DsMassMCSetNoPid",RooArgList(dsmass),HDsMassMCNoPidClCl);
  TH1F*HDsMassMCClCl=(TH1F*)(HDsMassMC->Clone("HDsMassMCClCl"));
  RooDataHist DsMassMCSet("DsMassMCSet","DsMassMCSet",RooArgList(dsmass),HDsMassMCClCl);

  RooRealVar mean1("mean1","mean1",SigMCPars[3]);
  RooRealVar sigma1("sigma1","sigma1",SigMCPars[4]);
  RooRealVar ratio("ratio","ratio",SigMCPars[5]);
  RooRealVar mean2("mean2","mean2",SigMCPars[6]);
  RooRealVar sigma2("sigma2","sigma2",SigMCPars[7]);
  RooGenericPdf DsMassSigPdf("DsMassSigPdf","DsMassSigPdf","(1-ratio)*exp(-.5*(dsmass-mean1)**2/sigma1**2)/(sqrt(2*3.14159)*sigma1)+ratio*exp(-.5*(dsmass-mean2)**2/sigma2**2)/(sqrt(2*3.14159)*sigma2)",RooArgList(dsmass,mean1,sigma1,ratio,mean2,sigma2));


  //----------Fit the MC
  //define pdf for denominator yield
  RooRealVar slope("slope","slope",-1,1);
  RooGenericPdf LinearPdfMC("LinearPdfMC","LinearPdfMC","1+slope*(dsmass-1.89)/(2.05-1.89)",RooArgList(slope,dsmass));
  RooRealVar DsYield("DsYield","DsYield",.01,.7);
  RooAddPdf TotDsPdf("TotDsPdf","TotDsPdf",DsMassSigPdf,LinearPdfMC,DsYield);
  //define pdf for numerator yield
  RooRealVar slopeEff("slopeEff","slopeEff",-1,1);
  RooGenericPdf LinearPdfMCEff("LinearPdfMCEff","LinearPdfMCEff","1+slopeEff*(dsmass-1.89)/(2.05-1.89)",RooArgList(slopeEff,dsmass));
  RooRealVar eff("eff","eff",.8,1.1);  
  RooRealVar IntegFracMC("IntegFracMC","IntegFracMC",HDsMassMCClCl->Integral()/HDsMassMCNoPidClCl->Integral());
  RooFormulaVar DsYieldEff("DsYieldEff","DsYieldEff","DsYield*eff/IntegFracMC",RooArgSet(DsYield,eff,IntegFracMC));
  RooAddPdf TotDsPdfEff("TotDsPdfEff","TotDsPdfEff",DsMassSigPdf,LinearPdfMCEff,DsYieldEff);




  TList pdfs;
  pdfs.Add(&TotDsPdf);
  pdfs.Add(&TotDsPdfEff);
  TList hists;
  hists.Add(HDsMassMCNoPidClCl);
  hists.Add(HDsMassMCClCl);
  TList vars;
  vars.Add(&dsmass);
  vars.Add(&dsmass);

  RooGenericPdf dummypdf("dummypdf","dummypdf","dsmass+DsYield+slope+slopeEff+eff",RooArgSet(dsmass,DsYield,slope,slopeEff,eff));//  
  DRecoSimulChi2Var chi2MC("chi2MC","chi2MC",dummypdf,DsMassMCSet,&hists,&pdfs,&vars,0);  
  RooMinuit minuitMC(chi2MC);
  minuitMC.migrad();
  RooPlot* mcpideffplot=dsmass.frame();
  DsMassMCSetNoPid.plotOn(mcpideffplot);
  TotDsPdf.plotOn(mcpideffplot);
  DsMassMCSet.plotOn(mcpideffplot);
  TotDsPdfEff.plotOn(mcpideffplot,Normalization(IntegFracMC.getVal()));
  C.Clear();
  mcpideffplot->SetTitle("");
  mcpideffplot->Draw();
  C.Print("DsTolnu/KKpi/NoDenom/analyzeDsToKKpiNoDenom.ps");
  delete mcpideffplot;
  cout<<" RooFit MC efficiency "<<eff.getVal()<<"  +- "<<eff.getError()<<endl;



  ///Fit the Data 
  TH1F*HDsMassDataNoPidClCl=(TH1F*)(HDsMassDataNoPid->Clone("HDsMassDataNoPidClCl"));
  RooDataHist DsMassDataSetNoPid("DsMassDataSetNoPid","DsMassDataSetNoPid",RooArgList(dsmass),HDsMassDataNoPidClCl);
  TH1F*HDsMassDataClCl=(TH1F*)(HDsMassData->Clone("HDsMassDataClCl"));
  RooDataHist DsMassDataSet("DsMassDataSet","DsMassDataSet",RooArgList(dsmass),HDsMassDataClCl);
  
  RooRealVar IntegFracData("IntegFracData","IntegFracData",HDsMassDataClCl->Integral()/HDsMassDataNoPidClCl->Integral());
  RooFormulaVar DsYieldEffData("DsYieldEffData","DsYieldEffData","DsYield*eff/IntegFracData",RooArgSet(DsYield,eff,IntegFracData));
  RooAddPdf TotDsPdfEffData("TotDsPdfEffData","TotDsPdfEffData",DsMassSigPdf,LinearPdfMCEff,DsYieldEffData);

  TList pdfsdata;
  pdfsdata.Add(&TotDsPdf);
  pdfsdata.Add(&TotDsPdfEffData);
  TList histsdata;
  histsdata.Add(HDsMassDataNoPidClCl);
  histsdata.Add(HDsMassDataClCl);

  mean1.setVal(SigDataPars[3]);
  sigma1.setVal(SigDataPars[4]);
  ratio.setVal(SigDataPars[5]);
  mean2.setVal(SigDataPars[6]);
  sigma2.setVal(SigDataPars[7]);
  

  DRecoSimulChi2Var chi2Data("chi2Data","chi2Data",dummypdf,DsMassDataSet,&histsdata,&pdfsdata,&vars,0);  
  RooMinuit minuitData(chi2Data);
  minuitData.migrad();
  RooPlot* datapideffplot=dsmass.frame();
  DsMassDataSetNoPid.plotOn(datapideffplot);
  TotDsPdf.plotOn(datapideffplot);
  DsMassDataSet.plotOn(datapideffplot);
  TotDsPdfEffData.plotOn(datapideffplot,Normalization(IntegFracData.getVal()));
  C.Clear();
  datapideffplot->SetTitle("");
  datapideffplot->Draw();
  C.Print("DsTolnu/KKpi/NoDenom/analyzeDsToKKpiNoDenom.ps");
  delete datapideffplot;
  cout<<" RooFit Data efficiency "<<eff.getVal()<<"  +- "<<eff.getError()<<endl;




  cout<<endl<<endl;
  ////---------------------------------------------------------------------------------------
  cout<<"------------------------- Dalitz dependence "<<endl;
  //FDs.ReleaseParameter(3);  FDs.ReleaseParameter(4);  FDs.ReleaseParameter(5);  FDs.ReleaseParameter(6);  FDs.ReleaseParameter(7);
  FDs.FixParameter(3,SigDataPars[3]);
  FDs.FixParameter(4,SigDataPars[4]);
  FDs.FixParameter(5,SigDataPars[5]);
  FDs.FixParameter(6,SigDataPars[6]);
  FDs.FixParameter(7,SigDataPars[7]);
  Float_t dalitzweights[3];Float_t dalitzweightsTot=0.;
  Float_t dalitzweightser[3];
  TH1F*HDsMassDataDalitz[3];
  for(Int_t d=0;d<3;d++){
    HDsMassDataDalitz[d]=(TH1F*)HistsFile.Get(TString("HDsMassDataDalitz")+(long)d);
    C.Clear();
    HDsMassDataDalitz[d]->GetXaxis()->SetTitle("m(KK#pi)");
    HDsMassDataDalitz[d]->Fit(&FDs,"QILL","pe",1.89,2.05);
    HDsMassDataDalitz[d]->GetYaxis()->SetRangeUser(0,HDsMassDataDalitz[d]->GetMaximum()*1.1);
    cout<<"Data Dalitz Yield "<<d<<" = "<<FDs.GetParameter(2)<<" +-"<<FDs.GetParError(2)<<endl;
    dalitzweights[d]=FDs.GetParameter(2);
    dalitzweightser[d]=FDs.GetParError(2);
    dalitzweightsTot+=dalitzweights[d];
    C.Print("DsTolnu/KKpi/NoDenom/analyzeDsToKKpiNoDenom.ps");
  }

  //Fit MC 
  FDs.FixParameter(3,SigMCPars[3]);
  FDs.FixParameter(4,SigMCPars[4]);
  FDs.FixParameter(5,SigMCPars[5]);
  FDs.FixParameter(6,SigMCPars[6]);
  FDs.FixParameter(7,SigMCPars[7]);
  Float_t mcdalitzweights[3];Float_t mcdalitzweightsTot=0.;
  Float_t mcdalitzweightser[3];
  TH1F*HDsMassMCDalitz[3];
  TH1F*HDsMassMCDalitzBkg[3];
  TH1F*HDsMassMCDalitzRes[3];Float_t mcdalitzweightsTotRes=0.;
  TH1F*HDsMassMCDalitzTrue[3];Float_t mcdalitzweightsTotTrue=0.;
  for(Int_t d=0;d<3;d++){
    HDsMassMCDalitz[d]=(TH1F*)HistsFile.Get(TString("HDsMassMCDalitz")+(long)d);
    HDsMassMCDalitzBkg[d]=(TH1F*)HistsFile.Get(TString("HDsMassMCBkgDalitz")+(long)d);
    C.Clear();
    HDsMassMCDalitz[d]->GetXaxis()->SetTitle("m(KK#pi) (GeV/c^{2})");
    HDsMassMCDalitz[d]->GetYaxis()->SetTitle("Entries/2 MeV");
    HDsMassMCDalitz[d]->Fit(&FDs,"QILL","pe",1.89,2.05);
    HDsMassMCDalitz[d]->GetYaxis()->SetRangeUser(0,HDsMassMCDalitz[d]->GetMaximum()*1.1);
    cout<<"MC Dalitz Yield "<<d<<" = "<<FDs.GetParameter(2)<<" +-"<<FDs.GetParError(2)<<endl;
    mcdalitzweights[d]=FDs.GetParameter(2);
    mcdalitzweightser[d]=FDs.GetParError(2);
    mcdalitzweightsTot+=mcdalitzweights[d];
    C.Print("DsTolnu/KKpi/NoDenom/analyzeDsToKKpiNoDenom.ps");

    //create the true histo with reco vectors by subtraction
    HDsMassMCDalitzTrue[d]=(TH1F*)HDsMassMCDalitz[d]->Clone(TString("HDsMassMCDalitzCl")+(long)d);
    HDsMassMCDalitzTrue[d]->Add(HDsMassMCDalitzBkg[d],-1);
    mcdalitzweightsTotTrue+=HDsMassMCDalitzTrue[d]->Integral();
    
    //get weights without resolution effects
    HDsMassMCDalitzRes[d]=(TH1F*)HistsFile.Get(TString("HDsMassMCDalitzRes")+(long)d);
    mcdalitzweightsTotRes+=HDsMassMCDalitzRes[d]->Integral();
  }  




  TFile FitResults("DsTolnu/KKpi/NoDenom/FitResults.root","recreate"); 

  //set the weights
  for(Int_t d=0;d<3;d++){
    HDsMassDataDalitz[d]->SetBinContent(0,dalitzweights[d]/dalitzweightsTot);
    HDsMassDataDalitz[d]->SetBinError(0,sqrt(3.)*(dalitzweightser[d]/dalitzweights[d])*dalitzweights[d]/dalitzweightsTot);
    HDsMassDataDalitz[d]->Write();

    HDsMassMCDalitz[d]->SetBinContent(0,mcdalitzweights[d]/mcdalitzweightsTot);
    HDsMassMCDalitz[d]->SetBinError(0,sqrt(3.)*(mcdalitzweightser[d]/mcdalitzweights[d])*mcdalitzweights[d]/mcdalitzweightsTot);
    HDsMassMCDalitz[d]->Write();
    
    cout<<setprecision(4)<<fixed<<endl;
    cout<<" Dalitz Region "<<d
	<<" & "<<HDsMassMCDalitzRes[d]->Integral()/mcdalitzweightsTotRes<<" $\\pm$ "<<sqrt(HDsMassMCDalitzRes[d]->Integral())/mcdalitzweightsTotRes
	<<" & "<<HDsMassMCDalitzTrue[d]->Integral()/mcdalitzweightsTotTrue<<" $\\pm$ "<<sqrt(HDsMassMCDalitzTrue[d]->Integral())/mcdalitzweightsTotTrue
	<<" & "<<HDsMassMCDalitz[d]->GetBinContent(0)<<" $\\pm$ "<<HDsMassMCDalitz[d]->GetBinError(0)
	<<" & "<<HDsMassDataDalitz[d]->GetBinContent(0)<<" $\\pm$ "<<HDsMassDataDalitz[d]->GetBinError(0)
	<<" \\\\"<<endl;
  }

  FitResults.ls();
  FitResults.Close();
  

  

  C.Print("DsTolnu/KKpi/NoDenom/analyzeDsToKKpiNoDenom.ps]");
}
