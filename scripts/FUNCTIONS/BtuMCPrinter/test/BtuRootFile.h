//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Mar 15 09:22:00 2007 by ROOT version 4.04/02b
// from TTree ntp1/Upsilon -> B_tag B_sig candidates using B_tag->Dlnu
// found on file: output/BToTauNu/BToDlnu-BpToKstarpNuNu_Generic-F2KBug/BToDlnu-BpToKstarpNuNu_Generic-F2KBug_1.root
//////////////////////////////////////////////////////////

#ifndef BtuRootFile_h
#define BtuRootFile_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

class BtuRootFile {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leave types
   Int_t           runNumber;
   Int_t           platform;
   Int_t           partition;
   Int_t           upperID;
   Int_t           lowerID;
   Int_t           majorID;
   Int_t           date;
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
   Float_t         EventWeight;
   Float_t         MMiss;
   Float_t         EMiss;
   Float_t         PMiss;
   Float_t         CosThMiss;
   Float_t         PhiMiss;
   Float_t         EMissCM;
   Float_t         PMissCM;
   Float_t         CosThMissCM;
   Float_t         PhiMissCM;
   Float_t         NetCharge;
   Float_t         NetChargeGTL;
   Float_t         NetChargeVisibleE;
   Int_t           nTracks;
   Int_t           nGoodTrkLoose;
   Float_t         R2All;
   Float_t         R2;
   Int_t           mcLen;
   Int_t           mcLund[57];   //[mcLen]
   Int_t           mothIdx[57];   //[mcLen]
   Int_t           dauLen[57];   //[mcLen]
   Int_t           dauIdx[57];   //[mcLen]
   Float_t         mcmass[57];   //[mcLen]
   Float_t         mcp3CM[57];   //[mcLen]
   Float_t         mccosthCM[57];   //[mcLen]
   Float_t         mcphiCM[57];   //[mcLen]
   Float_t         mcp3[57];   //[mcLen]
   Float_t         mccosth[57];   //[mcLen]
   Float_t         mcphi[57];   //[mcLen]
   Float_t         mcVtxx[57];   //[mcLen]
   Float_t         mcVtxy[57];   //[mcLen]
   Float_t         mcVtxz[57];   //[mcLen]
   Int_t           nY;
   Float_t         YChi2[1];   //[nY]
   Float_t         YMass[1];   //[nY]
   Float_t         YMassErr[1];   //[nY]
   Float_t         YSigBCosBY[1];   //[nY]
   Float_t         YSigBCosThetaThrust[1];   //[nY]
   Float_t         YSigBCosThetaThrustDL[1];   //[nY]
   Float_t         YSigBEExtraAll[1];   //[nY]
   Float_t         YSigBEExtraAll30[1];   //[nY]
   Float_t         YSigBEExtraNeutral[1];   //[nY]
   Float_t         YSigBEExtraNeutral30[1];   //[nY]
   Float_t         YSigBLgndrMom1[1];   //[nY]
   Float_t         YSigBLgndrMom2[1];   //[nY]
   Float_t         YSigBLgndrMom3[1];   //[nY]
   Float_t         YSigBLgndrMomDL1[1];   //[nY]
   Float_t         YSigBLgndrMomDL2[1];   //[nY]
   Float_t         YSigBLgndrMomDL3[1];   //[nY]
   Float_t         YSigBLgndrMomThrust1[1];   //[nY]
   Float_t         YSigBLgndrMomThrust2[1];   //[nY]
   Float_t         YSigBLgndrMomThrust3[1];   //[nY]
   Float_t         YSigBMLL[1];   //[nY]
   Float_t         YSigBMin2InvMass[1];   //[nY]
   Float_t         YSigBMin3InvMass[1];   //[nY]
   Float_t         YSigBPMissTheta[1];   //[nY]
   Float_t         YSigBPMissThetaCM[1];   //[nY]
   Float_t         YSigBTagBDeltaX[1];   //[nY]
   Float_t         YSigBTagBDeltaXErr[1];   //[nY]
   Float_t         YSigBTagBDeltaY[1];   //[nY]
   Float_t         YSigBTagBDeltaYErr[1];   //[nY]
   Float_t         YSigBTagBDeltaZ[1];   //[nY]
   Float_t         YSigBTagBDeltaZErr[1];   //[nY]
   Float_t         YTagBCLPi0Mass[1];   //[nY]
   Float_t         YTagBCosBY[1];   //[nY]
   Float_t         YTagBCosBYFlip[1];   //[nY]
   Float_t         YTagBCosBYPhotonAdd[1];   //[nY]
   Float_t         YTagBCosDstarl[1];   //[nY]
   Float_t         YTagBDeltaDMass[1];   //[nY]
   Float_t         YTagBDeltaM[1];   //[nY]
   Float_t         YTagBThetaDSoftDau[1];   //[nY]
   Float_t         YTagBVtxProb[1];   //[nY]
   Float_t         YVtxx[1];   //[nY]
   Float_t         YVtxy[1];   //[nY]
   Float_t         YVtxz[1];   //[nY]
   Float_t         Ycosth[1];   //[nY]
   Float_t         YcosthCM[1];   //[nY]
   Float_t         Yp3[1];   //[nY]
   Float_t         Yp3CM[1];   //[nY]
   Float_t         Yphi[1];   //[nY]
   Float_t         YphiCM[1];   //[nY]
   Int_t           YLund[1];   //[nY]
   Int_t           YMCIdx[1];   //[nY]
   Int_t           YSigBIsRecod[1];   //[nY]
   Int_t           YSigBMatchedTauMode[1];   //[nY]
   Int_t           YSigBNExtraEmcKL[1];   //[nY]
   Int_t           YSigBNExtraIfrKL[1];   //[nY]
   Int_t           YSigBNExtraNeutrals[1];   //[nY]
   Int_t           YSigBNExtraPi0[1];   //[nY]
   Int_t           YSigBNExtraTracks[1];   //[nY]
   Int_t           YSigBNExtraTracksVisibleE[1];   //[nY]
   Int_t           YSigBTruthMatchUp[1];   //[nY]
   Int_t           YTagBDDecMode[1];   //[nY]
   Int_t           YTagBLeptonID[1];   //[nY]
   Int_t           YTagBSoftDaufromDstarID[1];   //[nY]
   Int_t           YTagBTruthMatchUp[1];   //[nY]
   Int_t           YVtxStatus[1];   //[nY]
   Int_t           YnDaus[1];   //[nY]
   Int_t           YnDof[1];   //[nY]
   Int_t           Yd1Lund[1];   //[nY]
   Int_t           Yd1Idx[1];   //[nY]
   Int_t           Yd2Lund[1];   //[nY]
   Int_t           Yd2Idx[1];   //[nY]
   Int_t           nB;
   Float_t         BChi2[2];   //[nB]
   Float_t         BMass[2];   //[nB]
   Float_t         BMassErr[2];   //[nB]
   Float_t         BVtxx[2];   //[nB]
   Float_t         BVtxy[2];   //[nB]
   Float_t         BVtxz[2];   //[nB]
   Float_t         Bcosth[2];   //[nB]
   Float_t         BcosthCM[2];   //[nB]
   Float_t         Bp3[2];   //[nB]
   Float_t         Bp3CM[2];   //[nB]
   Float_t         Bphi[2];   //[nB]
   Float_t         BphiCM[2];   //[nB]
   Int_t           BLund[2];   //[nB]
   Int_t           BMCIdx[2];   //[nB]
   Int_t           BVtxStatus[2];   //[nB]
   Int_t           BnDaus[2];   //[nB]
   Int_t           BnDof[2];   //[nB]
   Int_t           Bd1Lund[2];   //[nB]
   Int_t           Bd1Idx[2];   //[nB]
   Int_t           Bd2Lund[2];   //[nB]
   Int_t           Bd2Idx[2];   //[nB]
   Int_t           nD;
   Float_t         DChi2[2];   //[nD]
   Float_t         DMass[2];   //[nD]
   Float_t         DMassErr[2];   //[nD]
   Float_t         DVtxx[2];   //[nD]
   Float_t         DVtxy[2];   //[nD]
   Float_t         DVtxz[2];   //[nD]
   Float_t         Dcosth[2];   //[nD]
   Float_t         DcosthCM[2];   //[nD]
   Float_t         Dp3[2];   //[nD]
   Float_t         Dp3CM[2];   //[nD]
   Float_t         Dphi[2];   //[nD]
   Float_t         DphiCM[2];   //[nD]
   Int_t           DLund[2];   //[nD]
   Int_t           DMCIdx[2];   //[nD]
   Int_t           DVtxStatus[2];   //[nD]
   Int_t           DnDaus[2];   //[nD]
   Int_t           DnDof[2];   //[nD]
   Int_t           Dd1Lund[2];   //[nD]
   Int_t           Dd1Idx[2];   //[nD]
   Int_t           Dd2Lund[2];   //[nD]
   Int_t           Dd2Idx[2];   //[nD]
   Int_t           Dd3Lund[2];   //[nD]
   Int_t           Dd3Idx[2];   //[nD]
   Int_t           Dd4Lund[2];   //[nD]
   Int_t           Dd4Idx[2];   //[nD]
   Int_t           nh;
   Float_t         hChi2[10];   //[nh]
   Float_t         hDoca[10];   //[nh]
   Float_t         hDocaErr[10];   //[nh]
   Float_t         hDocaXY_xy[10];   //[nh]
   Float_t         hDocaXY_xyErr[10];   //[nh]
   Float_t         hDocaXY_z[10];   //[nh]
   Float_t         hDocaXY_zErr[10];   //[nh]
   Float_t         hDoca_z[10];   //[nh]
   Float_t         hDoca_zErr[10];   //[nh]
   Float_t         hKLHTight_w[10];   //[nh]
   Float_t         hKLHTight_werr[10];   //[nh]
   Float_t         hMass[10];   //[nh]
   Float_t         hMassErr[10];   //[nh]
   Float_t         hPocaXY_cxx[10];   //[nh]
   Float_t         hPocaXY_cxy[10];   //[nh]
   Float_t         hPocaXY_cyy[10];   //[nh]
   Float_t         hPocaXY_cyz[10];   //[nh]
   Float_t         hPocaXY_czx[10];   //[nh]
   Float_t         hPocaXY_czz[10];   //[nh]
   Float_t         hPocaXY_x[10];   //[nh]
   Float_t         hPocaXY_xErr[10];   //[nh]
   Float_t         hPocaXY_y[10];   //[nh]
   Float_t         hPocaXY_yErr[10];   //[nh]
   Float_t         hPocaXY_z[10];   //[nh]
   Float_t         hPocaXY_zErr[10];   //[nh]
   Float_t         hPoca_cxx[10];   //[nh]
   Float_t         hPoca_cxy[10];   //[nh]
   Float_t         hPoca_cyy[10];   //[nh]
   Float_t         hPoca_cyz[10];   //[nh]
   Float_t         hPoca_czx[10];   //[nh]
   Float_t         hPoca_czz[10];   //[nh]
   Float_t         hPoca_x[10];   //[nh]
   Float_t         hPoca_xErr[10];   //[nh]
   Float_t         hPoca_y[10];   //[nh]
   Float_t         hPoca_yErr[10];   //[nh]
   Float_t         hPoca_z[10];   //[nh]
   Float_t         hPoca_zErr[10];   //[nh]
   Float_t         hVtxx[10];   //[nh]
   Float_t         hVtxy[10];   //[nh]
   Float_t         hVtxz[10];   //[nh]
   Float_t         hcosth[10];   //[nh]
   Float_t         hcosthCM[10];   //[nh]
   Float_t         hp3[10];   //[nh]
   Float_t         hp3CM[10];   //[nh]
   Float_t         hphi[10];   //[nh]
   Float_t         hphiCM[10];   //[nh]
   Float_t         hpiLHTight_w[10];   //[nh]
   Float_t         hpiLHTight_werr[10];   //[nh]
   Int_t           hKLHTight_stat[10];   //[nh]
   Int_t           hLund[10];   //[nh]
   Int_t           hMCIdx[10];   //[nh]
   Int_t           hVtxStatus[10];   //[nh]
   Int_t           hnDaus[10];   //[nh]
   Int_t           hnDof[10];   //[nh]
   Int_t           hpiLHTight_stat[10];   //[nh]
   Int_t           hd1Lund[10];   //[nh]
   Int_t           hd1Idx[10];   //[nh]
   Int_t           hd2Lund[10];   //[nh]
   Int_t           hd2Idx[10];   //[nh]
   Int_t           hTrkIdx[10];   //[nh]
   Int_t           nl;
   Float_t         lChi2[2];   //[nl]
   Float_t         lDoca[2];   //[nl]
   Float_t         lDocaErr[2];   //[nl]
   Float_t         lDocaXY_xy[2];   //[nl]
   Float_t         lDocaXY_xyErr[2];   //[nl]
   Float_t         lDocaXY_z[2];   //[nl]
   Float_t         lDocaXY_zErr[2];   //[nl]
   Float_t         lDoca_z[2];   //[nl]
   Float_t         lDoca_zErr[2];   //[nl]
   Float_t         lMass[2];   //[nl]
   Float_t         lMassErr[2];   //[nl]
   Float_t         lPidLHElectrons_w[2];   //[nl]
   Float_t         lPidLHElectrons_werr[2];   //[nl]
   Float_t         lPocaXY_cxx[2];   //[nl]
   Float_t         lPocaXY_cxy[2];   //[nl]
   Float_t         lPocaXY_cyy[2];   //[nl]
   Float_t         lPocaXY_cyz[2];   //[nl]
   Float_t         lPocaXY_czx[2];   //[nl]
   Float_t         lPocaXY_czz[2];   //[nl]
   Float_t         lPocaXY_x[2];   //[nl]
   Float_t         lPocaXY_xErr[2];   //[nl]
   Float_t         lPocaXY_y[2];   //[nl]
   Float_t         lPocaXY_yErr[2];   //[nl]
   Float_t         lPocaXY_z[2];   //[nl]
   Float_t         lPocaXY_zErr[2];   //[nl]
   Float_t         lPoca_cxx[2];   //[nl]
   Float_t         lPoca_cxy[2];   //[nl]
   Float_t         lPoca_cyy[2];   //[nl]
   Float_t         lPoca_cyz[2];   //[nl]
   Float_t         lPoca_czx[2];   //[nl]
   Float_t         lPoca_czz[2];   //[nl]
   Float_t         lPoca_x[2];   //[nl]
   Float_t         lPoca_xErr[2];   //[nl]
   Float_t         lPoca_y[2];   //[nl]
   Float_t         lPoca_yErr[2];   //[nl]
   Float_t         lPoca_z[2];   //[nl]
   Float_t         lPoca_zErr[2];   //[nl]
   Float_t         lVtxx[2];   //[nl]
   Float_t         lVtxy[2];   //[nl]
   Float_t         lVtxz[2];   //[nl]
   Float_t         lcosth[2];   //[nl]
   Float_t         lcosthCM[2];   //[nl]
   Float_t         lmuNNLoose_w[2];   //[nl]
   Float_t         lmuNNLoose_werr[2];   //[nl]
   Float_t         lmuNNTight_w[2];   //[nl]
   Float_t         lmuNNTight_werr[2];   //[nl]
   Float_t         lmuNNVeryTight_w[2];   //[nl]
   Float_t         lmuNNVeryTight_werr[2];   //[nl]
   Float_t         lp3[2];   //[nl]
   Float_t         lp3CM[2];   //[nl]
   Float_t         lphi[2];   //[nl]
   Float_t         lphiCM[2];   //[nl]
   Int_t           lLund[2];   //[nl]
   Int_t           lMCIdx[2];   //[nl]
   Int_t           lPidLHElectrons_stat[2];   //[nl]
   Int_t           lVtxStatus[2];   //[nl]
   Int_t           lmuNNLoose_stat[2];   //[nl]
   Int_t           lmuNNTight_stat[2];   //[nl]
   Int_t           lmuNNVeryTight_stat[2];   //[nl]
   Int_t           lnDaus[2];   //[nl]
   Int_t           lnDof[2];   //[nl]
   Int_t           ld1Lund[2];   //[nl]
   Int_t           ld1Idx[2];   //[nl]
   Int_t           ld2Lund[2];   //[nl]
   Int_t           ld2Idx[2];   //[nl]
   Int_t           lTrkIdx[2];   //[nl]
   Int_t           ngamma;
   Float_t         gammaMass[4];   //[ngamma]
   Float_t         gammaMassErr[4];   //[ngamma]
   Float_t         gammacosth[4];   //[ngamma]
   Float_t         gammacosthCM[4];   //[ngamma]
   Float_t         gammap3[4];   //[ngamma]
   Float_t         gammap3CM[4];   //[ngamma]
   Float_t         gammaphi[4];   //[ngamma]
   Float_t         gammaphiCM[4];   //[ngamma]
   Int_t           gammaLund[4];   //[ngamma]
   Int_t           gammaMCIdx[4];   //[ngamma]
   Int_t           gammanDaus[4];   //[ngamma]
   Int_t           neextragamma;
   Float_t         eextragammaMass[17];   //[neextragamma]
   Float_t         eextragammaMassErr[17];   //[neextragamma]
   Float_t         eextragammaPhotonTime[17];   //[neextragamma]
   Float_t         eextragammacosth[17];   //[neextragamma]
   Float_t         eextragammacosthCM[17];   //[neextragamma]
   Float_t         eextragammap3[17];   //[neextragamma]
   Float_t         eextragammap3CM[17];   //[neextragamma]
   Float_t         eextragammaphi[17];   //[neextragamma]
   Float_t         eextragammaphiCM[17];   //[neextragamma]
   Int_t           eextragammaLund[17];   //[neextragamma]
   Int_t           eextragammaMCIdx[17];   //[neextragamma]
   Int_t           eextragammanDaus[17];   //[neextragamma]
   Int_t           ntagbgamma;
   Float_t         tagbgammaMass[1];   //[ntagbgamma]
   Float_t         tagbgammaMassErr[1];   //[ntagbgamma]
   Float_t         tagbgammaPhotonTime[1];   //[ntagbgamma]
   Float_t         tagbgammacosth[1];   //[ntagbgamma]
   Float_t         tagbgammacosthCM[1];   //[ntagbgamma]
   Float_t         tagbgammap3[1];   //[ntagbgamma]
   Float_t         tagbgammap3CM[1];   //[ntagbgamma]
   Float_t         tagbgammaphi[1];   //[ntagbgamma]
   Float_t         tagbgammaphiCM[1];   //[ntagbgamma]
   Int_t           tagbgammaLund[1];   //[ntagbgamma]
   Int_t           tagbgammaMCIdx[1];   //[ntagbgamma]
   Int_t           tagbgammanDaus[1];   //[ntagbgamma]
   Int_t           nTRK;
   Int_t           TRKLund[8];   //[nTRK]
   Int_t           TRKnHitXY[8];   //[nTRK]
   Int_t           TRKnHitZ[8];   //[nTRK]
   Int_t           TRKnSvtXY[8];   //[nTRK]
   Int_t           TRKnSvtZ[8];   //[nTRK]
   UChar_t         TRKsvtViewXY1[8];   //[nTRK]
   UChar_t         TRKsvtViewXY2[8];   //[nTRK]
   UChar_t         TRKsvtViewXY3[8];   //[nTRK]
   UChar_t         TRKsvtViewXY4[8];   //[nTRK]
   UChar_t         TRKsvtViewXY5[8];   //[nTRK]
   UChar_t         TRKsvtViewZ1[8];   //[nTRK]
   UChar_t         TRKsvtViewZ2[8];   //[nTRK]
   UChar_t         TRKsvtViewZ3[8];   //[nTRK]
   UChar_t         TRKsvtViewZ4[8];   //[nTRK]
   UChar_t         TRKsvtViewZ5[8];   //[nTRK]
   Int_t           TRKnDchXY[8];   //[nTRK]
   Int_t           TRKnDchZ[8];   //[nTRK]
   Int_t           TRKdchFirst[8];   //[nTRK]
   Int_t           TRKdchLast[8];   //[nTRK]
   Float_t         TRKEff[8];   //[nTRK]
   Float_t         TRKEfferr[8];   //[nTRK]
   Float_t         TRKchEff[8];   //[nTRK]
   Float_t         TRKchEfferr[8];   //[nTRK]
   Int_t           KSelectorsMap[8];   //[nTRK]
   Int_t           eSelectorsMap[8];   //[nTRK]
   Int_t           muSelectorsMap[8];   //[nTRK]
   Int_t           piSelectorsMap[8];   //[nTRK]
   Int_t           pSelectorsMap[8];   //[nTRK]
   Float_t         TRKDrcTh[8];   //[nTRK]
   Float_t         TRKDrcThErr[8];   //[nTRK]
   Float_t         TRKDrcDeltaMom[8];   //[nTRK]
   Int_t           TRKDrcNph[8];   //[nTRK]
   Int_t           TRKDrcNbgph[8];   //[nTRK]
   Float_t         TRKdedxdch[8];   //[nTRK]
   Int_t           TRKdedxdchsamp[8];   //[nTRK]
   Float_t         TRKDchDeltaMom[8];   //[nTRK]
   Float_t         gammaLat[4];   //[ngamma]
   Float_t         gammaA42[4];   //[ngamma]
   Float_t         gammaECal[4];   //[ngamma]
   Float_t         gammaErawCal[4];   //[ngamma]
   Float_t         gammaCentx[4];   //[ngamma]
   Float_t         gammaCenty[4];   //[ngamma]
   Float_t         gammaCentz[4];   //[ngamma]
   Int_t           gammanCrys[4];   //[ngamma]

   // List of branches
   TBranch        *b_runNumber;   //!
   TBranch        *b_platform;   //!
   TBranch        *b_partition;   //!
   TBranch        *b_upperID;   //!
   TBranch        *b_lowerID;   //!
   TBranch        *b_majorID;   //!
   TBranch        *b_date;   //!
   TBranch        *b_eePx;   //!
   TBranch        *b_eePy;   //!
   TBranch        *b_eePz;   //!
   TBranch        *b_eeE;   //!
   TBranch        *b_beamSX;   //!
   TBranch        *b_beamSY;   //!
   TBranch        *b_beamSZ;   //!
   TBranch        *b_beamSCovXX;   //!
   TBranch        *b_beamSCovYY;   //!
   TBranch        *b_beamSCovZZ;   //!
   TBranch        *b_beamSCovXZ;   //!
   TBranch        *b_EventWeight;   //!
   TBranch        *b_MMiss;   //!
   TBranch        *b_EMiss;   //!
   TBranch        *b_PMiss;   //!
   TBranch        *b_CosThMiss;   //!
   TBranch        *b_PhiMiss;   //!
   TBranch        *b_EMissCM;   //!
   TBranch        *b_PMissCM;   //!
   TBranch        *b_CosThMissCM;   //!
   TBranch        *b_PhiMissCM;   //!
   TBranch        *b_NetCharge;   //!
   TBranch        *b_NetChargeGTL;   //!
   TBranch        *b_NetChargeVisibleE;   //!
   TBranch        *b_nTracks;   //!
   TBranch        *b_nGoodTrkLoose;   //!
   TBranch        *b_R2All;   //!
   TBranch        *b_R2;   //!
   TBranch        *b_mcLen;   //!
   TBranch        *b_mcLund;   //!
   TBranch        *b_mothIdx;   //!
   TBranch        *b_dauLen;   //!
   TBranch        *b_dauIdx;   //!
   TBranch        *b_mcmass;   //!
   TBranch        *b_mcp3CM;   //!
   TBranch        *b_mccosthCM;   //!
   TBranch        *b_mcphiCM;   //!
   TBranch        *b_mcp3;   //!
   TBranch        *b_mccosth;   //!
   TBranch        *b_mcphi;   //!
   TBranch        *b_mcVtxx;   //!
   TBranch        *b_mcVtxy;   //!
   TBranch        *b_mcVtxz;   //!
   TBranch        *b_nY;   //!
   TBranch        *b_YChi2;   //!
   TBranch        *b_YMass;   //!
   TBranch        *b_YMassErr;   //!
   TBranch        *b_YSigBCosBY;   //!
   TBranch        *b_YSigBCosThetaThrust;   //!
   TBranch        *b_YSigBCosThetaThrustDL;   //!
   TBranch        *b_YSigBEExtraAll;   //!
   TBranch        *b_YSigBEExtraAll30;   //!
   TBranch        *b_YSigBEExtraNeutral;   //!
   TBranch        *b_YSigBEExtraNeutral30;   //!
   TBranch        *b_YSigBLgndrMom1;   //!
   TBranch        *b_YSigBLgndrMom2;   //!
   TBranch        *b_YSigBLgndrMom3;   //!
   TBranch        *b_YSigBLgndrMomDL1;   //!
   TBranch        *b_YSigBLgndrMomDL2;   //!
   TBranch        *b_YSigBLgndrMomDL3;   //!
   TBranch        *b_YSigBLgndrMomThrust1;   //!
   TBranch        *b_YSigBLgndrMomThrust2;   //!
   TBranch        *b_YSigBLgndrMomThrust3;   //!
   TBranch        *b_YSigBMLL;   //!
   TBranch        *b_YSigBMin2InvMass;   //!
   TBranch        *b_YSigBMin3InvMass;   //!
   TBranch        *b_YSigBPMissTheta;   //!
   TBranch        *b_YSigBPMissThetaCM;   //!
   TBranch        *b_YSigBTagBDeltaX;   //!
   TBranch        *b_YSigBTagBDeltaXErr;   //!
   TBranch        *b_YSigBTagBDeltaY;   //!
   TBranch        *b_YSigBTagBDeltaYErr;   //!
   TBranch        *b_YSigBTagBDeltaZ;   //!
   TBranch        *b_YSigBTagBDeltaZErr;   //!
   TBranch        *b_YTagBCLPi0Mass;   //!
   TBranch        *b_YTagBCosBY;   //!
   TBranch        *b_YTagBCosBYFlip;   //!
   TBranch        *b_YTagBCosBYPhotonAdd;   //!
   TBranch        *b_YTagBCosDstarl;   //!
   TBranch        *b_YTagBDeltaDMass;   //!
   TBranch        *b_YTagBDeltaM;   //!
   TBranch        *b_YTagBThetaDSoftDau;   //!
   TBranch        *b_YTagBVtxProb;   //!
   TBranch        *b_YVtxx;   //!
   TBranch        *b_YVtxy;   //!
   TBranch        *b_YVtxz;   //!
   TBranch        *b_Ycosth;   //!
   TBranch        *b_YcosthCM;   //!
   TBranch        *b_Yp3;   //!
   TBranch        *b_Yp3CM;   //!
   TBranch        *b_Yphi;   //!
   TBranch        *b_YphiCM;   //!
   TBranch        *b_YLund;   //!
   TBranch        *b_YMCIdx;   //!
   TBranch        *b_YSigBIsRecod;   //!
   TBranch        *b_YSigBMatchedTauMode;   //!
   TBranch        *b_YSigBNExtraEmcKL;   //!
   TBranch        *b_YSigBNExtraIfrKL;   //!
   TBranch        *b_YSigBNExtraNeutrals;   //!
   TBranch        *b_YSigBNExtraPi0;   //!
   TBranch        *b_YSigBNExtraTracks;   //!
   TBranch        *b_YSigBNExtraTracksVisibleE;   //!
   TBranch        *b_YSigBTruthMatchUp;   //!
   TBranch        *b_YTagBDDecMode;   //!
   TBranch        *b_YTagBLeptonID;   //!
   TBranch        *b_YTagBSoftDaufromDstarID;   //!
   TBranch        *b_YTagBTruthMatchUp;   //!
   TBranch        *b_YVtxStatus;   //!
   TBranch        *b_YnDaus;   //!
   TBranch        *b_YnDof;   //!
   TBranch        *b_Yd1Lund;   //!
   TBranch        *b_Yd1Idx;   //!
   TBranch        *b_Yd2Lund;   //!
   TBranch        *b_Yd2Idx;   //!
   TBranch        *b_nB;   //!
   TBranch        *b_BChi2;   //!
   TBranch        *b_BMass;   //!
   TBranch        *b_BMassErr;   //!
   TBranch        *b_BVtxx;   //!
   TBranch        *b_BVtxy;   //!
   TBranch        *b_BVtxz;   //!
   TBranch        *b_Bcosth;   //!
   TBranch        *b_BcosthCM;   //!
   TBranch        *b_Bp3;   //!
   TBranch        *b_Bp3CM;   //!
   TBranch        *b_Bphi;   //!
   TBranch        *b_BphiCM;   //!
   TBranch        *b_BLund;   //!
   TBranch        *b_BMCIdx;   //!
   TBranch        *b_BVtxStatus;   //!
   TBranch        *b_BnDaus;   //!
   TBranch        *b_BnDof;   //!
   TBranch        *b_Bd1Lund;   //!
   TBranch        *b_Bd1Idx;   //!
   TBranch        *b_Bd2Lund;   //!
   TBranch        *b_Bd2Idx;   //!
   TBranch        *b_nD;   //!
   TBranch        *b_DChi2;   //!
   TBranch        *b_DMass;   //!
   TBranch        *b_DMassErr;   //!
   TBranch        *b_DVtxx;   //!
   TBranch        *b_DVtxy;   //!
   TBranch        *b_DVtxz;   //!
   TBranch        *b_Dcosth;   //!
   TBranch        *b_DcosthCM;   //!
   TBranch        *b_Dp3;   //!
   TBranch        *b_Dp3CM;   //!
   TBranch        *b_Dphi;   //!
   TBranch        *b_DphiCM;   //!
   TBranch        *b_DLund;   //!
   TBranch        *b_DMCIdx;   //!
   TBranch        *b_DVtxStatus;   //!
   TBranch        *b_DnDaus;   //!
   TBranch        *b_DnDof;   //!
   TBranch        *b_Dd1Lund;   //!
   TBranch        *b_Dd1Idx;   //!
   TBranch        *b_Dd2Lund;   //!
   TBranch        *b_Dd2Idx;   //!
   TBranch        *b_Dd3Lund;   //!
   TBranch        *b_Dd3Idx;   //!
   TBranch        *b_Dd4Lund;   //!
   TBranch        *b_Dd4Idx;   //!
   TBranch        *b_nh;   //!
   TBranch        *b_hChi2;   //!
   TBranch        *b_hDoca;   //!
   TBranch        *b_hDocaErr;   //!
   TBranch        *b_hDocaXY_xy;   //!
   TBranch        *b_hDocaXY_xyErr;   //!
   TBranch        *b_hDocaXY_z;   //!
   TBranch        *b_hDocaXY_zErr;   //!
   TBranch        *b_hDoca_z;   //!
   TBranch        *b_hDoca_zErr;   //!
   TBranch        *b_hKLHTight_w;   //!
   TBranch        *b_hKLHTight_werr;   //!
   TBranch        *b_hMass;   //!
   TBranch        *b_hMassErr;   //!
   TBranch        *b_hPocaXY_cxx;   //!
   TBranch        *b_hPocaXY_cxy;   //!
   TBranch        *b_hPocaXY_cyy;   //!
   TBranch        *b_hPocaXY_cyz;   //!
   TBranch        *b_hPocaXY_czx;   //!
   TBranch        *b_hPocaXY_czz;   //!
   TBranch        *b_hPocaXY_x;   //!
   TBranch        *b_hPocaXY_xErr;   //!
   TBranch        *b_hPocaXY_y;   //!
   TBranch        *b_hPocaXY_yErr;   //!
   TBranch        *b_hPocaXY_z;   //!
   TBranch        *b_hPocaXY_zErr;   //!
   TBranch        *b_hPoca_cxx;   //!
   TBranch        *b_hPoca_cxy;   //!
   TBranch        *b_hPoca_cyy;   //!
   TBranch        *b_hPoca_cyz;   //!
   TBranch        *b_hPoca_czx;   //!
   TBranch        *b_hPoca_czz;   //!
   TBranch        *b_hPoca_x;   //!
   TBranch        *b_hPoca_xErr;   //!
   TBranch        *b_hPoca_y;   //!
   TBranch        *b_hPoca_yErr;   //!
   TBranch        *b_hPoca_z;   //!
   TBranch        *b_hPoca_zErr;   //!
   TBranch        *b_hVtxx;   //!
   TBranch        *b_hVtxy;   //!
   TBranch        *b_hVtxz;   //!
   TBranch        *b_hcosth;   //!
   TBranch        *b_hcosthCM;   //!
   TBranch        *b_hp3;   //!
   TBranch        *b_hp3CM;   //!
   TBranch        *b_hphi;   //!
   TBranch        *b_hphiCM;   //!
   TBranch        *b_hpiLHTight_w;   //!
   TBranch        *b_hpiLHTight_werr;   //!
   TBranch        *b_hKLHTight_stat;   //!
   TBranch        *b_hLund;   //!
   TBranch        *b_hMCIdx;   //!
   TBranch        *b_hVtxStatus;   //!
   TBranch        *b_hnDaus;   //!
   TBranch        *b_hnDof;   //!
   TBranch        *b_hpiLHTight_stat;   //!
   TBranch        *b_hd1Lund;   //!
   TBranch        *b_hd1Idx;   //!
   TBranch        *b_hd2Lund;   //!
   TBranch        *b_hd2Idx;   //!
   TBranch        *b_hTrkIdx;   //!
   TBranch        *b_nl;   //!
   TBranch        *b_lChi2;   //!
   TBranch        *b_lDoca;   //!
   TBranch        *b_lDocaErr;   //!
   TBranch        *b_lDocaXY_xy;   //!
   TBranch        *b_lDocaXY_xyErr;   //!
   TBranch        *b_lDocaXY_z;   //!
   TBranch        *b_lDocaXY_zErr;   //!
   TBranch        *b_lDoca_z;   //!
   TBranch        *b_lDoca_zErr;   //!
   TBranch        *b_lMass;   //!
   TBranch        *b_lMassErr;   //!
   TBranch        *b_lPidLHElectrons_w;   //!
   TBranch        *b_lPidLHElectrons_werr;   //!
   TBranch        *b_lPocaXY_cxx;   //!
   TBranch        *b_lPocaXY_cxy;   //!
   TBranch        *b_lPocaXY_cyy;   //!
   TBranch        *b_lPocaXY_cyz;   //!
   TBranch        *b_lPocaXY_czx;   //!
   TBranch        *b_lPocaXY_czz;   //!
   TBranch        *b_lPocaXY_x;   //!
   TBranch        *b_lPocaXY_xErr;   //!
   TBranch        *b_lPocaXY_y;   //!
   TBranch        *b_lPocaXY_yErr;   //!
   TBranch        *b_lPocaXY_z;   //!
   TBranch        *b_lPocaXY_zErr;   //!
   TBranch        *b_lPoca_cxx;   //!
   TBranch        *b_lPoca_cxy;   //!
   TBranch        *b_lPoca_cyy;   //!
   TBranch        *b_lPoca_cyz;   //!
   TBranch        *b_lPoca_czx;   //!
   TBranch        *b_lPoca_czz;   //!
   TBranch        *b_lPoca_x;   //!
   TBranch        *b_lPoca_xErr;   //!
   TBranch        *b_lPoca_y;   //!
   TBranch        *b_lPoca_yErr;   //!
   TBranch        *b_lPoca_z;   //!
   TBranch        *b_lPoca_zErr;   //!
   TBranch        *b_lVtxx;   //!
   TBranch        *b_lVtxy;   //!
   TBranch        *b_lVtxz;   //!
   TBranch        *b_lcosth;   //!
   TBranch        *b_lcosthCM;   //!
   TBranch        *b_lmuNNLoose_w;   //!
   TBranch        *b_lmuNNLoose_werr;   //!
   TBranch        *b_lmuNNTight_w;   //!
   TBranch        *b_lmuNNTight_werr;   //!
   TBranch        *b_lmuNNVeryTight_w;   //!
   TBranch        *b_lmuNNVeryTight_werr;   //!
   TBranch        *b_lp3;   //!
   TBranch        *b_lp3CM;   //!
   TBranch        *b_lphi;   //!
   TBranch        *b_lphiCM;   //!
   TBranch        *b_lLund;   //!
   TBranch        *b_lMCIdx;   //!
   TBranch        *b_lPidLHElectrons_stat;   //!
   TBranch        *b_lVtxStatus;   //!
   TBranch        *b_lmuNNLoose_stat;   //!
   TBranch        *b_lmuNNTight_stat;   //!
   TBranch        *b_lmuNNVeryTight_stat;   //!
   TBranch        *b_lnDaus;   //!
   TBranch        *b_lnDof;   //!
   TBranch        *b_ld1Lund;   //!
   TBranch        *b_ld1Idx;   //!
   TBranch        *b_ld2Lund;   //!
   TBranch        *b_ld2Idx;   //!
   TBranch        *b_lTrkIdx;   //!
   TBranch        *b_ngamma;   //!
   TBranch        *b_gammaMass;   //!
   TBranch        *b_gammaMassErr;   //!
   TBranch        *b_gammacosth;   //!
   TBranch        *b_gammacosthCM;   //!
   TBranch        *b_gammap3;   //!
   TBranch        *b_gammap3CM;   //!
   TBranch        *b_gammaphi;   //!
   TBranch        *b_gammaphiCM;   //!
   TBranch        *b_gammaLund;   //!
   TBranch        *b_gammaMCIdx;   //!
   TBranch        *b_gammanDaus;   //!
   TBranch        *b_neextragamma;   //!
   TBranch        *b_eextragammaMass;   //!
   TBranch        *b_eextragammaMassErr;   //!
   TBranch        *b_eextragammaPhotonTime;   //!
   TBranch        *b_eextragammacosth;   //!
   TBranch        *b_eextragammacosthCM;   //!
   TBranch        *b_eextragammap3;   //!
   TBranch        *b_eextragammap3CM;   //!
   TBranch        *b_eextragammaphi;   //!
   TBranch        *b_eextragammaphiCM;   //!
   TBranch        *b_eextragammaLund;   //!
   TBranch        *b_eextragammaMCIdx;   //!
   TBranch        *b_eextragammanDaus;   //!
   TBranch        *b_ntagbgamma;   //!
   TBranch        *b_tagbgammaMass;   //!
   TBranch        *b_tagbgammaMassErr;   //!
   TBranch        *b_tagbgammaPhotonTime;   //!
   TBranch        *b_tagbgammacosth;   //!
   TBranch        *b_tagbgammacosthCM;   //!
   TBranch        *b_tagbgammap3;   //!
   TBranch        *b_tagbgammap3CM;   //!
   TBranch        *b_tagbgammaphi;   //!
   TBranch        *b_tagbgammaphiCM;   //!
   TBranch        *b_tagbgammaLund;   //!
   TBranch        *b_tagbgammaMCIdx;   //!
   TBranch        *b_tagbgammanDaus;   //!
   TBranch        *b_nTRK;   //!
   TBranch        *b_TRKLund;   //!
   TBranch        *b_TRKnHitXY;   //!
   TBranch        *b_TRKnHitZ;   //!
   TBranch        *b_TRKnSvtXY;   //!
   TBranch        *b_TRKnSvtZ;   //!
   TBranch        *b_TRKsvtViewXY1;   //!
   TBranch        *b_TRKsvtViewXY2;   //!
   TBranch        *b_TRKsvtViewXY3;   //!
   TBranch        *b_TRKsvtViewXY4;   //!
   TBranch        *b_TRKsvtViewXY5;   //!
   TBranch        *b_TRKsvtViewZ1;   //!
   TBranch        *b_TRKsvtViewZ2;   //!
   TBranch        *b_TRKsvtViewZ3;   //!
   TBranch        *b_TRKsvtViewZ4;   //!
   TBranch        *b_TRKsvtViewZ5;   //!
   TBranch        *b_TRKnDchXY;   //!
   TBranch        *b_TRKnDchZ;   //!
   TBranch        *b_TRKdchFirst;   //!
   TBranch        *b_TRKdchLast;   //!
   TBranch        *b_TRKEff;   //!
   TBranch        *b_TRKEfferr;   //!
   TBranch        *b_TRKchEff;   //!
   TBranch        *b_TRKchEfferr;   //!
   TBranch        *b_KSelectorsMap;   //!
   TBranch        *b_eSelectorsMap;   //!
   TBranch        *b_muSelectorsMap;   //!
   TBranch        *b_piSelectorsMap;   //!
   TBranch        *b_pSelectorsMap;   //!
   TBranch        *b_TRKDrcTh;   //!
   TBranch        *b_TRKDrcThErr;   //!
   TBranch        *b_TRKDrcDeltaMom;   //!
   TBranch        *b_TRKDrcNph;   //!
   TBranch        *b_TRKDrcNbgph;   //!
   TBranch        *b_TRKdedxdch;   //!
   TBranch        *b_TRKdedxdchsamp;   //!
   TBranch        *b_TRKDchDeltaMom;   //!
   TBranch        *b_gammaLat;   //!
   TBranch        *b_gammaA42;   //!
   TBranch        *b_gammaECal;   //!
   TBranch        *b_gammaErawCal;   //!
   TBranch        *b_gammaCentx;   //!
   TBranch        *b_gammaCenty;   //!
   TBranch        *b_gammaCentz;   //!
   TBranch        *b_gammanCrys;   //!

   BtuRootFile(TTree *tree=0);
   virtual ~BtuRootFile();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef BtuRootFile_cxx
BtuRootFile::BtuRootFile(TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("output/BToTauNu/BToDlnu-BpToKstarpNuNu_Generic-F2KBug/BToDlnu-BpToKstarpNuNu_Generic-F2KBug_1.root");
      if (!f) {
         f = new TFile("output/BToTauNu/BToDlnu-BpToKstarpNuNu_Generic-F2KBug/BToDlnu-BpToKstarpNuNu_Generic-F2KBug_1.root");
      }
      tree = (TTree*)gDirectory->Get("ntp1");

   }
   Init(tree);
}

BtuRootFile::~BtuRootFile()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t BtuRootFile::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t BtuRootFile::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->IsA() != TChain::Class()) return centry;
   TChain *chain = (TChain*)fChain;
   if (chain->GetTreeNumber() != fCurrent) {
      fCurrent = chain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void BtuRootFile::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses of the tree
   // will be set. It is normaly not necessary to make changes to the
   // generated code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running with PROOF.

   // Set branch addresses
   if (tree == 0) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("runNumber",&runNumber);
   fChain->SetBranchAddress("platform",&platform);
   fChain->SetBranchAddress("partition",&partition);
   fChain->SetBranchAddress("upperID",&upperID);
   fChain->SetBranchAddress("lowerID",&lowerID);
   fChain->SetBranchAddress("majorID",&majorID);
   fChain->SetBranchAddress("date",&date);
   fChain->SetBranchAddress("eePx",&eePx);
   fChain->SetBranchAddress("eePy",&eePy);
   fChain->SetBranchAddress("eePz",&eePz);
   fChain->SetBranchAddress("eeE",&eeE);
   fChain->SetBranchAddress("beamSX",&beamSX);
   fChain->SetBranchAddress("beamSY",&beamSY);
   fChain->SetBranchAddress("beamSZ",&beamSZ);
   fChain->SetBranchAddress("beamSCovXX",&beamSCovXX);
   fChain->SetBranchAddress("beamSCovYY",&beamSCovYY);
   fChain->SetBranchAddress("beamSCovZZ",&beamSCovZZ);
   fChain->SetBranchAddress("beamSCovXZ",&beamSCovXZ);
   fChain->SetBranchAddress("EventWeight",&EventWeight);
   fChain->SetBranchAddress("MMiss",&MMiss);
   fChain->SetBranchAddress("EMiss",&EMiss);
   fChain->SetBranchAddress("PMiss",&PMiss);
   fChain->SetBranchAddress("CosThMiss",&CosThMiss);
   fChain->SetBranchAddress("PhiMiss",&PhiMiss);
   fChain->SetBranchAddress("EMissCM",&EMissCM);
   fChain->SetBranchAddress("PMissCM",&PMissCM);
   fChain->SetBranchAddress("CosThMissCM",&CosThMissCM);
   fChain->SetBranchAddress("PhiMissCM",&PhiMissCM);
   fChain->SetBranchAddress("NetCharge",&NetCharge);
   fChain->SetBranchAddress("NetChargeGTL",&NetChargeGTL);
   fChain->SetBranchAddress("NetChargeVisibleE",&NetChargeVisibleE);
   fChain->SetBranchAddress("nTracks",&nTracks);
   fChain->SetBranchAddress("nGoodTrkLoose",&nGoodTrkLoose);
   fChain->SetBranchAddress("R2All",&R2All);
   fChain->SetBranchAddress("R2",&R2);
   fChain->SetBranchAddress("mcLen",&mcLen);
   fChain->SetBranchAddress("mcLund",mcLund);
   fChain->SetBranchAddress("mothIdx",mothIdx);
   fChain->SetBranchAddress("dauLen",dauLen);
   fChain->SetBranchAddress("dauIdx",dauIdx);
   fChain->SetBranchAddress("mcmass",mcmass);
   fChain->SetBranchAddress("mcp3CM",mcp3CM);
   fChain->SetBranchAddress("mccosthCM",mccosthCM);
   fChain->SetBranchAddress("mcphiCM",mcphiCM);
   fChain->SetBranchAddress("mcp3",mcp3);
   fChain->SetBranchAddress("mccosth",mccosth);
   fChain->SetBranchAddress("mcphi",mcphi);
   fChain->SetBranchAddress("mcVtxx",mcVtxx);
   fChain->SetBranchAddress("mcVtxy",mcVtxy);
   fChain->SetBranchAddress("mcVtxz",mcVtxz);
   fChain->SetBranchAddress("nY",&nY);
   fChain->SetBranchAddress("YChi2",YChi2);
   fChain->SetBranchAddress("YMass",YMass);
   fChain->SetBranchAddress("YMassErr",YMassErr);
   fChain->SetBranchAddress("YSigBCosBY",YSigBCosBY);
   fChain->SetBranchAddress("YSigBCosThetaThrust",YSigBCosThetaThrust);
   fChain->SetBranchAddress("YSigBCosThetaThrustDL",YSigBCosThetaThrustDL);
   fChain->SetBranchAddress("YSigBEExtraAll",YSigBEExtraAll);
   fChain->SetBranchAddress("YSigBEExtraAll30",YSigBEExtraAll30);
   fChain->SetBranchAddress("YSigBEExtraNeutral",YSigBEExtraNeutral);
   fChain->SetBranchAddress("YSigBEExtraNeutral30",YSigBEExtraNeutral30);
   fChain->SetBranchAddress("YSigBLgndrMom1",YSigBLgndrMom1);
   fChain->SetBranchAddress("YSigBLgndrMom2",YSigBLgndrMom2);
   fChain->SetBranchAddress("YSigBLgndrMom3",YSigBLgndrMom3);
   fChain->SetBranchAddress("YSigBLgndrMomDL1",YSigBLgndrMomDL1);
   fChain->SetBranchAddress("YSigBLgndrMomDL2",YSigBLgndrMomDL2);
   fChain->SetBranchAddress("YSigBLgndrMomDL3",YSigBLgndrMomDL3);
   fChain->SetBranchAddress("YSigBLgndrMomThrust1",YSigBLgndrMomThrust1);
   fChain->SetBranchAddress("YSigBLgndrMomThrust2",YSigBLgndrMomThrust2);
   fChain->SetBranchAddress("YSigBLgndrMomThrust3",YSigBLgndrMomThrust3);
   fChain->SetBranchAddress("YSigBMLL",YSigBMLL);
   fChain->SetBranchAddress("YSigBMin2InvMass",YSigBMin2InvMass);
   fChain->SetBranchAddress("YSigBMin3InvMass",YSigBMin3InvMass);
   fChain->SetBranchAddress("YSigBPMissTheta",YSigBPMissTheta);
   fChain->SetBranchAddress("YSigBPMissThetaCM",YSigBPMissThetaCM);
   fChain->SetBranchAddress("YSigBTagBDeltaX",YSigBTagBDeltaX);
   fChain->SetBranchAddress("YSigBTagBDeltaXErr",YSigBTagBDeltaXErr);
   fChain->SetBranchAddress("YSigBTagBDeltaY",YSigBTagBDeltaY);
   fChain->SetBranchAddress("YSigBTagBDeltaYErr",YSigBTagBDeltaYErr);
   fChain->SetBranchAddress("YSigBTagBDeltaZ",YSigBTagBDeltaZ);
   fChain->SetBranchAddress("YSigBTagBDeltaZErr",YSigBTagBDeltaZErr);
   fChain->SetBranchAddress("YTagBCLPi0Mass",YTagBCLPi0Mass);
   fChain->SetBranchAddress("YTagBCosBY",YTagBCosBY);
   fChain->SetBranchAddress("YTagBCosBYFlip",YTagBCosBYFlip);
   fChain->SetBranchAddress("YTagBCosBYPhotonAdd",YTagBCosBYPhotonAdd);
   fChain->SetBranchAddress("YTagBCosDstarl",YTagBCosDstarl);
   fChain->SetBranchAddress("YTagBDeltaDMass",YTagBDeltaDMass);
   fChain->SetBranchAddress("YTagBDeltaM",YTagBDeltaM);
   fChain->SetBranchAddress("YTagBThetaDSoftDau",YTagBThetaDSoftDau);
   fChain->SetBranchAddress("YTagBVtxProb",YTagBVtxProb);
   fChain->SetBranchAddress("YVtxx",YVtxx);
   fChain->SetBranchAddress("YVtxy",YVtxy);
   fChain->SetBranchAddress("YVtxz",YVtxz);
   fChain->SetBranchAddress("Ycosth",Ycosth);
   fChain->SetBranchAddress("YcosthCM",YcosthCM);
   fChain->SetBranchAddress("Yp3",Yp3);
   fChain->SetBranchAddress("Yp3CM",Yp3CM);
   fChain->SetBranchAddress("Yphi",Yphi);
   fChain->SetBranchAddress("YphiCM",YphiCM);
   fChain->SetBranchAddress("YLund",YLund);
   fChain->SetBranchAddress("YMCIdx",YMCIdx);
   fChain->SetBranchAddress("YSigBIsRecod",YSigBIsRecod);
   fChain->SetBranchAddress("YSigBMatchedTauMode",YSigBMatchedTauMode);
   fChain->SetBranchAddress("YSigBNExtraEmcKL",YSigBNExtraEmcKL);
   fChain->SetBranchAddress("YSigBNExtraIfrKL",YSigBNExtraIfrKL);
   fChain->SetBranchAddress("YSigBNExtraNeutrals",YSigBNExtraNeutrals);
   fChain->SetBranchAddress("YSigBNExtraPi0",YSigBNExtraPi0);
   fChain->SetBranchAddress("YSigBNExtraTracks",YSigBNExtraTracks);
   fChain->SetBranchAddress("YSigBNExtraTracksVisibleE",YSigBNExtraTracksVisibleE);
   fChain->SetBranchAddress("YSigBTruthMatchUp",YSigBTruthMatchUp);
   fChain->SetBranchAddress("YTagBDDecMode",YTagBDDecMode);
   fChain->SetBranchAddress("YTagBLeptonID",YTagBLeptonID);
   fChain->SetBranchAddress("YTagBSoftDaufromDstarID",YTagBSoftDaufromDstarID);
   fChain->SetBranchAddress("YTagBTruthMatchUp",YTagBTruthMatchUp);
   fChain->SetBranchAddress("YVtxStatus",YVtxStatus);
   fChain->SetBranchAddress("YnDaus",YnDaus);
   fChain->SetBranchAddress("YnDof",YnDof);
   fChain->SetBranchAddress("Yd1Lund",Yd1Lund);
   fChain->SetBranchAddress("Yd1Idx",Yd1Idx);
   fChain->SetBranchAddress("Yd2Lund",Yd2Lund);
   fChain->SetBranchAddress("Yd2Idx",Yd2Idx);
   fChain->SetBranchAddress("nB",&nB);
   fChain->SetBranchAddress("BChi2",BChi2);
   fChain->SetBranchAddress("BMass",BMass);
   fChain->SetBranchAddress("BMassErr",BMassErr);
   fChain->SetBranchAddress("BVtxx",BVtxx);
   fChain->SetBranchAddress("BVtxy",BVtxy);
   fChain->SetBranchAddress("BVtxz",BVtxz);
   fChain->SetBranchAddress("Bcosth",Bcosth);
   fChain->SetBranchAddress("BcosthCM",BcosthCM);
   fChain->SetBranchAddress("Bp3",Bp3);
   fChain->SetBranchAddress("Bp3CM",Bp3CM);
   fChain->SetBranchAddress("Bphi",Bphi);
   fChain->SetBranchAddress("BphiCM",BphiCM);
   fChain->SetBranchAddress("BLund",BLund);
   fChain->SetBranchAddress("BMCIdx",BMCIdx);
   fChain->SetBranchAddress("BVtxStatus",BVtxStatus);
   fChain->SetBranchAddress("BnDaus",BnDaus);
   fChain->SetBranchAddress("BnDof",BnDof);
   fChain->SetBranchAddress("Bd1Lund",Bd1Lund);
   fChain->SetBranchAddress("Bd1Idx",Bd1Idx);
   fChain->SetBranchAddress("Bd2Lund",Bd2Lund);
   fChain->SetBranchAddress("Bd2Idx",Bd2Idx);
   fChain->SetBranchAddress("nD",&nD);
   fChain->SetBranchAddress("DChi2",DChi2);
   fChain->SetBranchAddress("DMass",DMass);
   fChain->SetBranchAddress("DMassErr",DMassErr);
   fChain->SetBranchAddress("DVtxx",DVtxx);
   fChain->SetBranchAddress("DVtxy",DVtxy);
   fChain->SetBranchAddress("DVtxz",DVtxz);
   fChain->SetBranchAddress("Dcosth",Dcosth);
   fChain->SetBranchAddress("DcosthCM",DcosthCM);
   fChain->SetBranchAddress("Dp3",Dp3);
   fChain->SetBranchAddress("Dp3CM",Dp3CM);
   fChain->SetBranchAddress("Dphi",Dphi);
   fChain->SetBranchAddress("DphiCM",DphiCM);
   fChain->SetBranchAddress("DLund",DLund);
   fChain->SetBranchAddress("DMCIdx",DMCIdx);
   fChain->SetBranchAddress("DVtxStatus",DVtxStatus);
   fChain->SetBranchAddress("DnDaus",DnDaus);
   fChain->SetBranchAddress("DnDof",DnDof);
   fChain->SetBranchAddress("Dd1Lund",Dd1Lund);
   fChain->SetBranchAddress("Dd1Idx",Dd1Idx);
   fChain->SetBranchAddress("Dd2Lund",Dd2Lund);
   fChain->SetBranchAddress("Dd2Idx",Dd2Idx);
   fChain->SetBranchAddress("Dd3Lund",Dd3Lund);
   fChain->SetBranchAddress("Dd3Idx",Dd3Idx);
   fChain->SetBranchAddress("Dd4Lund",Dd4Lund);
   fChain->SetBranchAddress("Dd4Idx",Dd4Idx);
   fChain->SetBranchAddress("nh",&nh);
   fChain->SetBranchAddress("hChi2",hChi2);
   fChain->SetBranchAddress("hDoca",hDoca);
   fChain->SetBranchAddress("hDocaErr",hDocaErr);
   fChain->SetBranchAddress("hDocaXY_xy",hDocaXY_xy);
   fChain->SetBranchAddress("hDocaXY_xyErr",hDocaXY_xyErr);
   fChain->SetBranchAddress("hDocaXY_z",hDocaXY_z);
   fChain->SetBranchAddress("hDocaXY_zErr",hDocaXY_zErr);
   fChain->SetBranchAddress("hDoca_z",hDoca_z);
   fChain->SetBranchAddress("hDoca_zErr",hDoca_zErr);
   fChain->SetBranchAddress("hKLHTight_w",hKLHTight_w);
   fChain->SetBranchAddress("hKLHTight_werr",hKLHTight_werr);
   fChain->SetBranchAddress("hMass",hMass);
   fChain->SetBranchAddress("hMassErr",hMassErr);
   fChain->SetBranchAddress("hPocaXY_cxx",hPocaXY_cxx);
   fChain->SetBranchAddress("hPocaXY_cxy",hPocaXY_cxy);
   fChain->SetBranchAddress("hPocaXY_cyy",hPocaXY_cyy);
   fChain->SetBranchAddress("hPocaXY_cyz",hPocaXY_cyz);
   fChain->SetBranchAddress("hPocaXY_czx",hPocaXY_czx);
   fChain->SetBranchAddress("hPocaXY_czz",hPocaXY_czz);
   fChain->SetBranchAddress("hPocaXY_x",hPocaXY_x);
   fChain->SetBranchAddress("hPocaXY_xErr",hPocaXY_xErr);
   fChain->SetBranchAddress("hPocaXY_y",hPocaXY_y);
   fChain->SetBranchAddress("hPocaXY_yErr",hPocaXY_yErr);
   fChain->SetBranchAddress("hPocaXY_z",hPocaXY_z);
   fChain->SetBranchAddress("hPocaXY_zErr",hPocaXY_zErr);
   fChain->SetBranchAddress("hPoca_cxx",hPoca_cxx);
   fChain->SetBranchAddress("hPoca_cxy",hPoca_cxy);
   fChain->SetBranchAddress("hPoca_cyy",hPoca_cyy);
   fChain->SetBranchAddress("hPoca_cyz",hPoca_cyz);
   fChain->SetBranchAddress("hPoca_czx",hPoca_czx);
   fChain->SetBranchAddress("hPoca_czz",hPoca_czz);
   fChain->SetBranchAddress("hPoca_x",hPoca_x);
   fChain->SetBranchAddress("hPoca_xErr",hPoca_xErr);
   fChain->SetBranchAddress("hPoca_y",hPoca_y);
   fChain->SetBranchAddress("hPoca_yErr",hPoca_yErr);
   fChain->SetBranchAddress("hPoca_z",hPoca_z);
   fChain->SetBranchAddress("hPoca_zErr",hPoca_zErr);
   fChain->SetBranchAddress("hVtxx",hVtxx);
   fChain->SetBranchAddress("hVtxy",hVtxy);
   fChain->SetBranchAddress("hVtxz",hVtxz);
   fChain->SetBranchAddress("hcosth",hcosth);
   fChain->SetBranchAddress("hcosthCM",hcosthCM);
   fChain->SetBranchAddress("hp3",hp3);
   fChain->SetBranchAddress("hp3CM",hp3CM);
   fChain->SetBranchAddress("hphi",hphi);
   fChain->SetBranchAddress("hphiCM",hphiCM);
   fChain->SetBranchAddress("hpiLHTight_w",hpiLHTight_w);
   fChain->SetBranchAddress("hpiLHTight_werr",hpiLHTight_werr);
   fChain->SetBranchAddress("hKLHTight_stat",hKLHTight_stat);
   fChain->SetBranchAddress("hLund",hLund);
   fChain->SetBranchAddress("hMCIdx",hMCIdx);
   fChain->SetBranchAddress("hVtxStatus",hVtxStatus);
   fChain->SetBranchAddress("hnDaus",hnDaus);
   fChain->SetBranchAddress("hnDof",hnDof);
   fChain->SetBranchAddress("hpiLHTight_stat",hpiLHTight_stat);
   fChain->SetBranchAddress("hd1Lund",hd1Lund);
   fChain->SetBranchAddress("hd1Idx",hd1Idx);
   fChain->SetBranchAddress("hd2Lund",hd2Lund);
   fChain->SetBranchAddress("hd2Idx",hd2Idx);
   fChain->SetBranchAddress("hTrkIdx",hTrkIdx);
   fChain->SetBranchAddress("nl",&nl);
   fChain->SetBranchAddress("lChi2",lChi2);
   fChain->SetBranchAddress("lDoca",lDoca);
   fChain->SetBranchAddress("lDocaErr",lDocaErr);
   fChain->SetBranchAddress("lDocaXY_xy",lDocaXY_xy);
   fChain->SetBranchAddress("lDocaXY_xyErr",lDocaXY_xyErr);
   fChain->SetBranchAddress("lDocaXY_z",lDocaXY_z);
   fChain->SetBranchAddress("lDocaXY_zErr",lDocaXY_zErr);
   fChain->SetBranchAddress("lDoca_z",lDoca_z);
   fChain->SetBranchAddress("lDoca_zErr",lDoca_zErr);
   fChain->SetBranchAddress("lMass",lMass);
   fChain->SetBranchAddress("lMassErr",lMassErr);
   fChain->SetBranchAddress("lPidLHElectrons_w",lPidLHElectrons_w);
   fChain->SetBranchAddress("lPidLHElectrons_werr",lPidLHElectrons_werr);
   fChain->SetBranchAddress("lPocaXY_cxx",lPocaXY_cxx);
   fChain->SetBranchAddress("lPocaXY_cxy",lPocaXY_cxy);
   fChain->SetBranchAddress("lPocaXY_cyy",lPocaXY_cyy);
   fChain->SetBranchAddress("lPocaXY_cyz",lPocaXY_cyz);
   fChain->SetBranchAddress("lPocaXY_czx",lPocaXY_czx);
   fChain->SetBranchAddress("lPocaXY_czz",lPocaXY_czz);
   fChain->SetBranchAddress("lPocaXY_x",lPocaXY_x);
   fChain->SetBranchAddress("lPocaXY_xErr",lPocaXY_xErr);
   fChain->SetBranchAddress("lPocaXY_y",lPocaXY_y);
   fChain->SetBranchAddress("lPocaXY_yErr",lPocaXY_yErr);
   fChain->SetBranchAddress("lPocaXY_z",lPocaXY_z);
   fChain->SetBranchAddress("lPocaXY_zErr",lPocaXY_zErr);
   fChain->SetBranchAddress("lPoca_cxx",lPoca_cxx);
   fChain->SetBranchAddress("lPoca_cxy",lPoca_cxy);
   fChain->SetBranchAddress("lPoca_cyy",lPoca_cyy);
   fChain->SetBranchAddress("lPoca_cyz",lPoca_cyz);
   fChain->SetBranchAddress("lPoca_czx",lPoca_czx);
   fChain->SetBranchAddress("lPoca_czz",lPoca_czz);
   fChain->SetBranchAddress("lPoca_x",lPoca_x);
   fChain->SetBranchAddress("lPoca_xErr",lPoca_xErr);
   fChain->SetBranchAddress("lPoca_y",lPoca_y);
   fChain->SetBranchAddress("lPoca_yErr",lPoca_yErr);
   fChain->SetBranchAddress("lPoca_z",lPoca_z);
   fChain->SetBranchAddress("lPoca_zErr",lPoca_zErr);
   fChain->SetBranchAddress("lVtxx",lVtxx);
   fChain->SetBranchAddress("lVtxy",lVtxy);
   fChain->SetBranchAddress("lVtxz",lVtxz);
   fChain->SetBranchAddress("lcosth",lcosth);
   fChain->SetBranchAddress("lcosthCM",lcosthCM);
   fChain->SetBranchAddress("lmuNNLoose_w",lmuNNLoose_w);
   fChain->SetBranchAddress("lmuNNLoose_werr",lmuNNLoose_werr);
   fChain->SetBranchAddress("lmuNNTight_w",lmuNNTight_w);
   fChain->SetBranchAddress("lmuNNTight_werr",lmuNNTight_werr);
   fChain->SetBranchAddress("lmuNNVeryTight_w",lmuNNVeryTight_w);
   fChain->SetBranchAddress("lmuNNVeryTight_werr",lmuNNVeryTight_werr);
   fChain->SetBranchAddress("lp3",lp3);
   fChain->SetBranchAddress("lp3CM",lp3CM);
   fChain->SetBranchAddress("lphi",lphi);
   fChain->SetBranchAddress("lphiCM",lphiCM);
   fChain->SetBranchAddress("lLund",lLund);
   fChain->SetBranchAddress("lMCIdx",lMCIdx);
   fChain->SetBranchAddress("lPidLHElectrons_stat",lPidLHElectrons_stat);
   fChain->SetBranchAddress("lVtxStatus",lVtxStatus);
   fChain->SetBranchAddress("lmuNNLoose_stat",lmuNNLoose_stat);
   fChain->SetBranchAddress("lmuNNTight_stat",lmuNNTight_stat);
   fChain->SetBranchAddress("lmuNNVeryTight_stat",lmuNNVeryTight_stat);
   fChain->SetBranchAddress("lnDaus",lnDaus);
   fChain->SetBranchAddress("lnDof",lnDof);
   fChain->SetBranchAddress("ld1Lund",ld1Lund);
   fChain->SetBranchAddress("ld1Idx",ld1Idx);
   fChain->SetBranchAddress("ld2Lund",ld2Lund);
   fChain->SetBranchAddress("ld2Idx",ld2Idx);
   fChain->SetBranchAddress("lTrkIdx",lTrkIdx);
   fChain->SetBranchAddress("ngamma",&ngamma);
   fChain->SetBranchAddress("gammaMass",gammaMass);
   fChain->SetBranchAddress("gammaMassErr",gammaMassErr);
   fChain->SetBranchAddress("gammacosth",gammacosth);
   fChain->SetBranchAddress("gammacosthCM",gammacosthCM);
   fChain->SetBranchAddress("gammap3",gammap3);
   fChain->SetBranchAddress("gammap3CM",gammap3CM);
   fChain->SetBranchAddress("gammaphi",gammaphi);
   fChain->SetBranchAddress("gammaphiCM",gammaphiCM);
   fChain->SetBranchAddress("gammaLund",gammaLund);
   fChain->SetBranchAddress("gammaMCIdx",gammaMCIdx);
   fChain->SetBranchAddress("gammanDaus",gammanDaus);
   fChain->SetBranchAddress("neextragamma",&neextragamma);
   fChain->SetBranchAddress("eextragammaMass",eextragammaMass);
   fChain->SetBranchAddress("eextragammaMassErr",eextragammaMassErr);
   fChain->SetBranchAddress("eextragammaPhotonTime",eextragammaPhotonTime);
   fChain->SetBranchAddress("eextragammacosth",eextragammacosth);
   fChain->SetBranchAddress("eextragammacosthCM",eextragammacosthCM);
   fChain->SetBranchAddress("eextragammap3",eextragammap3);
   fChain->SetBranchAddress("eextragammap3CM",eextragammap3CM);
   fChain->SetBranchAddress("eextragammaphi",eextragammaphi);
   fChain->SetBranchAddress("eextragammaphiCM",eextragammaphiCM);
   fChain->SetBranchAddress("eextragammaLund",eextragammaLund);
   fChain->SetBranchAddress("eextragammaMCIdx",eextragammaMCIdx);
   fChain->SetBranchAddress("eextragammanDaus",eextragammanDaus);
   fChain->SetBranchAddress("ntagbgamma",&ntagbgamma);
   fChain->SetBranchAddress("tagbgammaMass",tagbgammaMass);
   fChain->SetBranchAddress("tagbgammaMassErr",tagbgammaMassErr);
   fChain->SetBranchAddress("tagbgammaPhotonTime",tagbgammaPhotonTime);
   fChain->SetBranchAddress("tagbgammacosth",tagbgammacosth);
   fChain->SetBranchAddress("tagbgammacosthCM",tagbgammacosthCM);
   fChain->SetBranchAddress("tagbgammap3",tagbgammap3);
   fChain->SetBranchAddress("tagbgammap3CM",tagbgammap3CM);
   fChain->SetBranchAddress("tagbgammaphi",tagbgammaphi);
   fChain->SetBranchAddress("tagbgammaphiCM",tagbgammaphiCM);
   fChain->SetBranchAddress("tagbgammaLund",tagbgammaLund);
   fChain->SetBranchAddress("tagbgammaMCIdx",tagbgammaMCIdx);
   fChain->SetBranchAddress("tagbgammanDaus",tagbgammanDaus);
   fChain->SetBranchAddress("nTRK",&nTRK);
   fChain->SetBranchAddress("TRKLund",TRKLund);
   fChain->SetBranchAddress("TRKnHitXY",TRKnHitXY);
   fChain->SetBranchAddress("TRKnHitZ",TRKnHitZ);
   fChain->SetBranchAddress("TRKnSvtXY",TRKnSvtXY);
   fChain->SetBranchAddress("TRKnSvtZ",TRKnSvtZ);
   fChain->SetBranchAddress("TRKsvtViewXY1",TRKsvtViewXY1);
   fChain->SetBranchAddress("TRKsvtViewXY2",TRKsvtViewXY2);
   fChain->SetBranchAddress("TRKsvtViewXY3",TRKsvtViewXY3);
   fChain->SetBranchAddress("TRKsvtViewXY4",TRKsvtViewXY4);
   fChain->SetBranchAddress("TRKsvtViewXY5",TRKsvtViewXY5);
   fChain->SetBranchAddress("TRKsvtViewZ1",TRKsvtViewZ1);
   fChain->SetBranchAddress("TRKsvtViewZ2",TRKsvtViewZ2);
   fChain->SetBranchAddress("TRKsvtViewZ3",TRKsvtViewZ3);
   fChain->SetBranchAddress("TRKsvtViewZ4",TRKsvtViewZ4);
   fChain->SetBranchAddress("TRKsvtViewZ5",TRKsvtViewZ5);
   fChain->SetBranchAddress("TRKnDchXY",TRKnDchXY);
   fChain->SetBranchAddress("TRKnDchZ",TRKnDchZ);
   fChain->SetBranchAddress("TRKdchFirst",TRKdchFirst);
   fChain->SetBranchAddress("TRKdchLast",TRKdchLast);
   fChain->SetBranchAddress("TRKEff",TRKEff);
   fChain->SetBranchAddress("TRKEfferr",TRKEfferr);
   fChain->SetBranchAddress("TRKchEff",TRKchEff);
   fChain->SetBranchAddress("TRKchEfferr",TRKchEfferr);
   fChain->SetBranchAddress("KSelectorsMap",KSelectorsMap);
   fChain->SetBranchAddress("eSelectorsMap",eSelectorsMap);
   fChain->SetBranchAddress("muSelectorsMap",muSelectorsMap);
   fChain->SetBranchAddress("piSelectorsMap",piSelectorsMap);
   fChain->SetBranchAddress("pSelectorsMap",pSelectorsMap);
   fChain->SetBranchAddress("TRKDrcTh",TRKDrcTh);
   fChain->SetBranchAddress("TRKDrcThErr",TRKDrcThErr);
   fChain->SetBranchAddress("TRKDrcDeltaMom",TRKDrcDeltaMom);
   fChain->SetBranchAddress("TRKDrcNph",TRKDrcNph);
   fChain->SetBranchAddress("TRKDrcNbgph",TRKDrcNbgph);
   fChain->SetBranchAddress("TRKdedxdch",TRKdedxdch);
   fChain->SetBranchAddress("TRKdedxdchsamp",TRKdedxdchsamp);
   fChain->SetBranchAddress("TRKDchDeltaMom",TRKDchDeltaMom);
   fChain->SetBranchAddress("gammaLat",gammaLat);
   fChain->SetBranchAddress("gammaA42",gammaA42);
   fChain->SetBranchAddress("gammaECal",gammaECal);
   fChain->SetBranchAddress("gammaErawCal",gammaErawCal);
   fChain->SetBranchAddress("gammaCentx",gammaCentx);
   fChain->SetBranchAddress("gammaCenty",gammaCenty);
   fChain->SetBranchAddress("gammaCentz",gammaCentz);
   fChain->SetBranchAddress("gammanCrys",gammanCrys);
   Notify();
}

Bool_t BtuRootFile::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. Typically here the branch pointers
   // will be retrieved. It is normaly not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed.

   // Get branch pointers
   b_runNumber = fChain->GetBranch("runNumber");
   b_platform = fChain->GetBranch("platform");
   b_partition = fChain->GetBranch("partition");
   b_upperID = fChain->GetBranch("upperID");
   b_lowerID = fChain->GetBranch("lowerID");
   b_majorID = fChain->GetBranch("majorID");
   b_date = fChain->GetBranch("date");
   b_eePx = fChain->GetBranch("eePx");
   b_eePy = fChain->GetBranch("eePy");
   b_eePz = fChain->GetBranch("eePz");
   b_eeE = fChain->GetBranch("eeE");
   b_beamSX = fChain->GetBranch("beamSX");
   b_beamSY = fChain->GetBranch("beamSY");
   b_beamSZ = fChain->GetBranch("beamSZ");
   b_beamSCovXX = fChain->GetBranch("beamSCovXX");
   b_beamSCovYY = fChain->GetBranch("beamSCovYY");
   b_beamSCovZZ = fChain->GetBranch("beamSCovZZ");
   b_beamSCovXZ = fChain->GetBranch("beamSCovXZ");
   b_EventWeight = fChain->GetBranch("EventWeight");
   b_MMiss = fChain->GetBranch("MMiss");
   b_EMiss = fChain->GetBranch("EMiss");
   b_PMiss = fChain->GetBranch("PMiss");
   b_CosThMiss = fChain->GetBranch("CosThMiss");
   b_PhiMiss = fChain->GetBranch("PhiMiss");
   b_EMissCM = fChain->GetBranch("EMissCM");
   b_PMissCM = fChain->GetBranch("PMissCM");
   b_CosThMissCM = fChain->GetBranch("CosThMissCM");
   b_PhiMissCM = fChain->GetBranch("PhiMissCM");
   b_NetCharge = fChain->GetBranch("NetCharge");
   b_NetChargeGTL = fChain->GetBranch("NetChargeGTL");
   b_NetChargeVisibleE = fChain->GetBranch("NetChargeVisibleE");
   b_nTracks = fChain->GetBranch("nTracks");
   b_nGoodTrkLoose = fChain->GetBranch("nGoodTrkLoose");
   b_R2All = fChain->GetBranch("R2All");
   b_R2 = fChain->GetBranch("R2");
   b_mcLen = fChain->GetBranch("mcLen");
   b_mcLund = fChain->GetBranch("mcLund");
   b_mothIdx = fChain->GetBranch("mothIdx");
   b_dauLen = fChain->GetBranch("dauLen");
   b_dauIdx = fChain->GetBranch("dauIdx");
   b_mcmass = fChain->GetBranch("mcmass");
   b_mcp3CM = fChain->GetBranch("mcp3CM");
   b_mccosthCM = fChain->GetBranch("mccosthCM");
   b_mcphiCM = fChain->GetBranch("mcphiCM");
   b_mcp3 = fChain->GetBranch("mcp3");
   b_mccosth = fChain->GetBranch("mccosth");
   b_mcphi = fChain->GetBranch("mcphi");
   b_mcVtxx = fChain->GetBranch("mcVtxx");
   b_mcVtxy = fChain->GetBranch("mcVtxy");
   b_mcVtxz = fChain->GetBranch("mcVtxz");
   b_nY = fChain->GetBranch("nY");
   b_YChi2 = fChain->GetBranch("YChi2");
   b_YMass = fChain->GetBranch("YMass");
   b_YMassErr = fChain->GetBranch("YMassErr");
   b_YSigBCosBY = fChain->GetBranch("YSigBCosBY");
   b_YSigBCosThetaThrust = fChain->GetBranch("YSigBCosThetaThrust");
   b_YSigBCosThetaThrustDL = fChain->GetBranch("YSigBCosThetaThrustDL");
   b_YSigBEExtraAll = fChain->GetBranch("YSigBEExtraAll");
   b_YSigBEExtraAll30 = fChain->GetBranch("YSigBEExtraAll30");
   b_YSigBEExtraNeutral = fChain->GetBranch("YSigBEExtraNeutral");
   b_YSigBEExtraNeutral30 = fChain->GetBranch("YSigBEExtraNeutral30");
   b_YSigBLgndrMom1 = fChain->GetBranch("YSigBLgndrMom1");
   b_YSigBLgndrMom2 = fChain->GetBranch("YSigBLgndrMom2");
   b_YSigBLgndrMom3 = fChain->GetBranch("YSigBLgndrMom3");
   b_YSigBLgndrMomDL1 = fChain->GetBranch("YSigBLgndrMomDL1");
   b_YSigBLgndrMomDL2 = fChain->GetBranch("YSigBLgndrMomDL2");
   b_YSigBLgndrMomDL3 = fChain->GetBranch("YSigBLgndrMomDL3");
   b_YSigBLgndrMomThrust1 = fChain->GetBranch("YSigBLgndrMomThrust1");
   b_YSigBLgndrMomThrust2 = fChain->GetBranch("YSigBLgndrMomThrust2");
   b_YSigBLgndrMomThrust3 = fChain->GetBranch("YSigBLgndrMomThrust3");
   b_YSigBMLL = fChain->GetBranch("YSigBMLL");
   b_YSigBMin2InvMass = fChain->GetBranch("YSigBMin2InvMass");
   b_YSigBMin3InvMass = fChain->GetBranch("YSigBMin3InvMass");
   b_YSigBPMissTheta = fChain->GetBranch("YSigBPMissTheta");
   b_YSigBPMissThetaCM = fChain->GetBranch("YSigBPMissThetaCM");
   b_YSigBTagBDeltaX = fChain->GetBranch("YSigBTagBDeltaX");
   b_YSigBTagBDeltaXErr = fChain->GetBranch("YSigBTagBDeltaXErr");
   b_YSigBTagBDeltaY = fChain->GetBranch("YSigBTagBDeltaY");
   b_YSigBTagBDeltaYErr = fChain->GetBranch("YSigBTagBDeltaYErr");
   b_YSigBTagBDeltaZ = fChain->GetBranch("YSigBTagBDeltaZ");
   b_YSigBTagBDeltaZErr = fChain->GetBranch("YSigBTagBDeltaZErr");
   b_YTagBCLPi0Mass = fChain->GetBranch("YTagBCLPi0Mass");
   b_YTagBCosBY = fChain->GetBranch("YTagBCosBY");
   b_YTagBCosBYFlip = fChain->GetBranch("YTagBCosBYFlip");
   b_YTagBCosBYPhotonAdd = fChain->GetBranch("YTagBCosBYPhotonAdd");
   b_YTagBCosDstarl = fChain->GetBranch("YTagBCosDstarl");
   b_YTagBDeltaDMass = fChain->GetBranch("YTagBDeltaDMass");
   b_YTagBDeltaM = fChain->GetBranch("YTagBDeltaM");
   b_YTagBThetaDSoftDau = fChain->GetBranch("YTagBThetaDSoftDau");
   b_YTagBVtxProb = fChain->GetBranch("YTagBVtxProb");
   b_YVtxx = fChain->GetBranch("YVtxx");
   b_YVtxy = fChain->GetBranch("YVtxy");
   b_YVtxz = fChain->GetBranch("YVtxz");
   b_Ycosth = fChain->GetBranch("Ycosth");
   b_YcosthCM = fChain->GetBranch("YcosthCM");
   b_Yp3 = fChain->GetBranch("Yp3");
   b_Yp3CM = fChain->GetBranch("Yp3CM");
   b_Yphi = fChain->GetBranch("Yphi");
   b_YphiCM = fChain->GetBranch("YphiCM");
   b_YLund = fChain->GetBranch("YLund");
   b_YMCIdx = fChain->GetBranch("YMCIdx");
   b_YSigBIsRecod = fChain->GetBranch("YSigBIsRecod");
   b_YSigBMatchedTauMode = fChain->GetBranch("YSigBMatchedTauMode");
   b_YSigBNExtraEmcKL = fChain->GetBranch("YSigBNExtraEmcKL");
   b_YSigBNExtraIfrKL = fChain->GetBranch("YSigBNExtraIfrKL");
   b_YSigBNExtraNeutrals = fChain->GetBranch("YSigBNExtraNeutrals");
   b_YSigBNExtraPi0 = fChain->GetBranch("YSigBNExtraPi0");
   b_YSigBNExtraTracks = fChain->GetBranch("YSigBNExtraTracks");
   b_YSigBNExtraTracksVisibleE = fChain->GetBranch("YSigBNExtraTracksVisibleE");
   b_YSigBTruthMatchUp = fChain->GetBranch("YSigBTruthMatchUp");
   b_YTagBDDecMode = fChain->GetBranch("YTagBDDecMode");
   b_YTagBLeptonID = fChain->GetBranch("YTagBLeptonID");
   b_YTagBSoftDaufromDstarID = fChain->GetBranch("YTagBSoftDaufromDstarID");
   b_YTagBTruthMatchUp = fChain->GetBranch("YTagBTruthMatchUp");
   b_YVtxStatus = fChain->GetBranch("YVtxStatus");
   b_YnDaus = fChain->GetBranch("YnDaus");
   b_YnDof = fChain->GetBranch("YnDof");
   b_Yd1Lund = fChain->GetBranch("Yd1Lund");
   b_Yd1Idx = fChain->GetBranch("Yd1Idx");
   b_Yd2Lund = fChain->GetBranch("Yd2Lund");
   b_Yd2Idx = fChain->GetBranch("Yd2Idx");
   b_nB = fChain->GetBranch("nB");
   b_BChi2 = fChain->GetBranch("BChi2");
   b_BMass = fChain->GetBranch("BMass");
   b_BMassErr = fChain->GetBranch("BMassErr");
   b_BVtxx = fChain->GetBranch("BVtxx");
   b_BVtxy = fChain->GetBranch("BVtxy");
   b_BVtxz = fChain->GetBranch("BVtxz");
   b_Bcosth = fChain->GetBranch("Bcosth");
   b_BcosthCM = fChain->GetBranch("BcosthCM");
   b_Bp3 = fChain->GetBranch("Bp3");
   b_Bp3CM = fChain->GetBranch("Bp3CM");
   b_Bphi = fChain->GetBranch("Bphi");
   b_BphiCM = fChain->GetBranch("BphiCM");
   b_BLund = fChain->GetBranch("BLund");
   b_BMCIdx = fChain->GetBranch("BMCIdx");
   b_BVtxStatus = fChain->GetBranch("BVtxStatus");
   b_BnDaus = fChain->GetBranch("BnDaus");
   b_BnDof = fChain->GetBranch("BnDof");
   b_Bd1Lund = fChain->GetBranch("Bd1Lund");
   b_Bd1Idx = fChain->GetBranch("Bd1Idx");
   b_Bd2Lund = fChain->GetBranch("Bd2Lund");
   b_Bd2Idx = fChain->GetBranch("Bd2Idx");
   b_nD = fChain->GetBranch("nD");
   b_DChi2 = fChain->GetBranch("DChi2");
   b_DMass = fChain->GetBranch("DMass");
   b_DMassErr = fChain->GetBranch("DMassErr");
   b_DVtxx = fChain->GetBranch("DVtxx");
   b_DVtxy = fChain->GetBranch("DVtxy");
   b_DVtxz = fChain->GetBranch("DVtxz");
   b_Dcosth = fChain->GetBranch("Dcosth");
   b_DcosthCM = fChain->GetBranch("DcosthCM");
   b_Dp3 = fChain->GetBranch("Dp3");
   b_Dp3CM = fChain->GetBranch("Dp3CM");
   b_Dphi = fChain->GetBranch("Dphi");
   b_DphiCM = fChain->GetBranch("DphiCM");
   b_DLund = fChain->GetBranch("DLund");
   b_DMCIdx = fChain->GetBranch("DMCIdx");
   b_DVtxStatus = fChain->GetBranch("DVtxStatus");
   b_DnDaus = fChain->GetBranch("DnDaus");
   b_DnDof = fChain->GetBranch("DnDof");
   b_Dd1Lund = fChain->GetBranch("Dd1Lund");
   b_Dd1Idx = fChain->GetBranch("Dd1Idx");
   b_Dd2Lund = fChain->GetBranch("Dd2Lund");
   b_Dd2Idx = fChain->GetBranch("Dd2Idx");
   b_Dd3Lund = fChain->GetBranch("Dd3Lund");
   b_Dd3Idx = fChain->GetBranch("Dd3Idx");
   b_Dd4Lund = fChain->GetBranch("Dd4Lund");
   b_Dd4Idx = fChain->GetBranch("Dd4Idx");
   b_nh = fChain->GetBranch("nh");
   b_hChi2 = fChain->GetBranch("hChi2");
   b_hDoca = fChain->GetBranch("hDoca");
   b_hDocaErr = fChain->GetBranch("hDocaErr");
   b_hDocaXY_xy = fChain->GetBranch("hDocaXY_xy");
   b_hDocaXY_xyErr = fChain->GetBranch("hDocaXY_xyErr");
   b_hDocaXY_z = fChain->GetBranch("hDocaXY_z");
   b_hDocaXY_zErr = fChain->GetBranch("hDocaXY_zErr");
   b_hDoca_z = fChain->GetBranch("hDoca_z");
   b_hDoca_zErr = fChain->GetBranch("hDoca_zErr");
   b_hKLHTight_w = fChain->GetBranch("hKLHTight_w");
   b_hKLHTight_werr = fChain->GetBranch("hKLHTight_werr");
   b_hMass = fChain->GetBranch("hMass");
   b_hMassErr = fChain->GetBranch("hMassErr");
   b_hPocaXY_cxx = fChain->GetBranch("hPocaXY_cxx");
   b_hPocaXY_cxy = fChain->GetBranch("hPocaXY_cxy");
   b_hPocaXY_cyy = fChain->GetBranch("hPocaXY_cyy");
   b_hPocaXY_cyz = fChain->GetBranch("hPocaXY_cyz");
   b_hPocaXY_czx = fChain->GetBranch("hPocaXY_czx");
   b_hPocaXY_czz = fChain->GetBranch("hPocaXY_czz");
   b_hPocaXY_x = fChain->GetBranch("hPocaXY_x");
   b_hPocaXY_xErr = fChain->GetBranch("hPocaXY_xErr");
   b_hPocaXY_y = fChain->GetBranch("hPocaXY_y");
   b_hPocaXY_yErr = fChain->GetBranch("hPocaXY_yErr");
   b_hPocaXY_z = fChain->GetBranch("hPocaXY_z");
   b_hPocaXY_zErr = fChain->GetBranch("hPocaXY_zErr");
   b_hPoca_cxx = fChain->GetBranch("hPoca_cxx");
   b_hPoca_cxy = fChain->GetBranch("hPoca_cxy");
   b_hPoca_cyy = fChain->GetBranch("hPoca_cyy");
   b_hPoca_cyz = fChain->GetBranch("hPoca_cyz");
   b_hPoca_czx = fChain->GetBranch("hPoca_czx");
   b_hPoca_czz = fChain->GetBranch("hPoca_czz");
   b_hPoca_x = fChain->GetBranch("hPoca_x");
   b_hPoca_xErr = fChain->GetBranch("hPoca_xErr");
   b_hPoca_y = fChain->GetBranch("hPoca_y");
   b_hPoca_yErr = fChain->GetBranch("hPoca_yErr");
   b_hPoca_z = fChain->GetBranch("hPoca_z");
   b_hPoca_zErr = fChain->GetBranch("hPoca_zErr");
   b_hVtxx = fChain->GetBranch("hVtxx");
   b_hVtxy = fChain->GetBranch("hVtxy");
   b_hVtxz = fChain->GetBranch("hVtxz");
   b_hcosth = fChain->GetBranch("hcosth");
   b_hcosthCM = fChain->GetBranch("hcosthCM");
   b_hp3 = fChain->GetBranch("hp3");
   b_hp3CM = fChain->GetBranch("hp3CM");
   b_hphi = fChain->GetBranch("hphi");
   b_hphiCM = fChain->GetBranch("hphiCM");
   b_hpiLHTight_w = fChain->GetBranch("hpiLHTight_w");
   b_hpiLHTight_werr = fChain->GetBranch("hpiLHTight_werr");
   b_hKLHTight_stat = fChain->GetBranch("hKLHTight_stat");
   b_hLund = fChain->GetBranch("hLund");
   b_hMCIdx = fChain->GetBranch("hMCIdx");
   b_hVtxStatus = fChain->GetBranch("hVtxStatus");
   b_hnDaus = fChain->GetBranch("hnDaus");
   b_hnDof = fChain->GetBranch("hnDof");
   b_hpiLHTight_stat = fChain->GetBranch("hpiLHTight_stat");
   b_hd1Lund = fChain->GetBranch("hd1Lund");
   b_hd1Idx = fChain->GetBranch("hd1Idx");
   b_hd2Lund = fChain->GetBranch("hd2Lund");
   b_hd2Idx = fChain->GetBranch("hd2Idx");
   b_hTrkIdx = fChain->GetBranch("hTrkIdx");
   b_nl = fChain->GetBranch("nl");
   b_lChi2 = fChain->GetBranch("lChi2");
   b_lDoca = fChain->GetBranch("lDoca");
   b_lDocaErr = fChain->GetBranch("lDocaErr");
   b_lDocaXY_xy = fChain->GetBranch("lDocaXY_xy");
   b_lDocaXY_xyErr = fChain->GetBranch("lDocaXY_xyErr");
   b_lDocaXY_z = fChain->GetBranch("lDocaXY_z");
   b_lDocaXY_zErr = fChain->GetBranch("lDocaXY_zErr");
   b_lDoca_z = fChain->GetBranch("lDoca_z");
   b_lDoca_zErr = fChain->GetBranch("lDoca_zErr");
   b_lMass = fChain->GetBranch("lMass");
   b_lMassErr = fChain->GetBranch("lMassErr");
   b_lPidLHElectrons_w = fChain->GetBranch("lPidLHElectrons_w");
   b_lPidLHElectrons_werr = fChain->GetBranch("lPidLHElectrons_werr");
   b_lPocaXY_cxx = fChain->GetBranch("lPocaXY_cxx");
   b_lPocaXY_cxy = fChain->GetBranch("lPocaXY_cxy");
   b_lPocaXY_cyy = fChain->GetBranch("lPocaXY_cyy");
   b_lPocaXY_cyz = fChain->GetBranch("lPocaXY_cyz");
   b_lPocaXY_czx = fChain->GetBranch("lPocaXY_czx");
   b_lPocaXY_czz = fChain->GetBranch("lPocaXY_czz");
   b_lPocaXY_x = fChain->GetBranch("lPocaXY_x");
   b_lPocaXY_xErr = fChain->GetBranch("lPocaXY_xErr");
   b_lPocaXY_y = fChain->GetBranch("lPocaXY_y");
   b_lPocaXY_yErr = fChain->GetBranch("lPocaXY_yErr");
   b_lPocaXY_z = fChain->GetBranch("lPocaXY_z");
   b_lPocaXY_zErr = fChain->GetBranch("lPocaXY_zErr");
   b_lPoca_cxx = fChain->GetBranch("lPoca_cxx");
   b_lPoca_cxy = fChain->GetBranch("lPoca_cxy");
   b_lPoca_cyy = fChain->GetBranch("lPoca_cyy");
   b_lPoca_cyz = fChain->GetBranch("lPoca_cyz");
   b_lPoca_czx = fChain->GetBranch("lPoca_czx");
   b_lPoca_czz = fChain->GetBranch("lPoca_czz");
   b_lPoca_x = fChain->GetBranch("lPoca_x");
   b_lPoca_xErr = fChain->GetBranch("lPoca_xErr");
   b_lPoca_y = fChain->GetBranch("lPoca_y");
   b_lPoca_yErr = fChain->GetBranch("lPoca_yErr");
   b_lPoca_z = fChain->GetBranch("lPoca_z");
   b_lPoca_zErr = fChain->GetBranch("lPoca_zErr");
   b_lVtxx = fChain->GetBranch("lVtxx");
   b_lVtxy = fChain->GetBranch("lVtxy");
   b_lVtxz = fChain->GetBranch("lVtxz");
   b_lcosth = fChain->GetBranch("lcosth");
   b_lcosthCM = fChain->GetBranch("lcosthCM");
   b_lmuNNLoose_w = fChain->GetBranch("lmuNNLoose_w");
   b_lmuNNLoose_werr = fChain->GetBranch("lmuNNLoose_werr");
   b_lmuNNTight_w = fChain->GetBranch("lmuNNTight_w");
   b_lmuNNTight_werr = fChain->GetBranch("lmuNNTight_werr");
   b_lmuNNVeryTight_w = fChain->GetBranch("lmuNNVeryTight_w");
   b_lmuNNVeryTight_werr = fChain->GetBranch("lmuNNVeryTight_werr");
   b_lp3 = fChain->GetBranch("lp3");
   b_lp3CM = fChain->GetBranch("lp3CM");
   b_lphi = fChain->GetBranch("lphi");
   b_lphiCM = fChain->GetBranch("lphiCM");
   b_lLund = fChain->GetBranch("lLund");
   b_lMCIdx = fChain->GetBranch("lMCIdx");
   b_lPidLHElectrons_stat = fChain->GetBranch("lPidLHElectrons_stat");
   b_lVtxStatus = fChain->GetBranch("lVtxStatus");
   b_lmuNNLoose_stat = fChain->GetBranch("lmuNNLoose_stat");
   b_lmuNNTight_stat = fChain->GetBranch("lmuNNTight_stat");
   b_lmuNNVeryTight_stat = fChain->GetBranch("lmuNNVeryTight_stat");
   b_lnDaus = fChain->GetBranch("lnDaus");
   b_lnDof = fChain->GetBranch("lnDof");
   b_ld1Lund = fChain->GetBranch("ld1Lund");
   b_ld1Idx = fChain->GetBranch("ld1Idx");
   b_ld2Lund = fChain->GetBranch("ld2Lund");
   b_ld2Idx = fChain->GetBranch("ld2Idx");
   b_lTrkIdx = fChain->GetBranch("lTrkIdx");
   b_ngamma = fChain->GetBranch("ngamma");
   b_gammaMass = fChain->GetBranch("gammaMass");
   b_gammaMassErr = fChain->GetBranch("gammaMassErr");
   b_gammacosth = fChain->GetBranch("gammacosth");
   b_gammacosthCM = fChain->GetBranch("gammacosthCM");
   b_gammap3 = fChain->GetBranch("gammap3");
   b_gammap3CM = fChain->GetBranch("gammap3CM");
   b_gammaphi = fChain->GetBranch("gammaphi");
   b_gammaphiCM = fChain->GetBranch("gammaphiCM");
   b_gammaLund = fChain->GetBranch("gammaLund");
   b_gammaMCIdx = fChain->GetBranch("gammaMCIdx");
   b_gammanDaus = fChain->GetBranch("gammanDaus");
   b_neextragamma = fChain->GetBranch("neextragamma");
   b_eextragammaMass = fChain->GetBranch("eextragammaMass");
   b_eextragammaMassErr = fChain->GetBranch("eextragammaMassErr");
   b_eextragammaPhotonTime = fChain->GetBranch("eextragammaPhotonTime");
   b_eextragammacosth = fChain->GetBranch("eextragammacosth");
   b_eextragammacosthCM = fChain->GetBranch("eextragammacosthCM");
   b_eextragammap3 = fChain->GetBranch("eextragammap3");
   b_eextragammap3CM = fChain->GetBranch("eextragammap3CM");
   b_eextragammaphi = fChain->GetBranch("eextragammaphi");
   b_eextragammaphiCM = fChain->GetBranch("eextragammaphiCM");
   b_eextragammaLund = fChain->GetBranch("eextragammaLund");
   b_eextragammaMCIdx = fChain->GetBranch("eextragammaMCIdx");
   b_eextragammanDaus = fChain->GetBranch("eextragammanDaus");
   b_ntagbgamma = fChain->GetBranch("ntagbgamma");
   b_tagbgammaMass = fChain->GetBranch("tagbgammaMass");
   b_tagbgammaMassErr = fChain->GetBranch("tagbgammaMassErr");
   b_tagbgammaPhotonTime = fChain->GetBranch("tagbgammaPhotonTime");
   b_tagbgammacosth = fChain->GetBranch("tagbgammacosth");
   b_tagbgammacosthCM = fChain->GetBranch("tagbgammacosthCM");
   b_tagbgammap3 = fChain->GetBranch("tagbgammap3");
   b_tagbgammap3CM = fChain->GetBranch("tagbgammap3CM");
   b_tagbgammaphi = fChain->GetBranch("tagbgammaphi");
   b_tagbgammaphiCM = fChain->GetBranch("tagbgammaphiCM");
   b_tagbgammaLund = fChain->GetBranch("tagbgammaLund");
   b_tagbgammaMCIdx = fChain->GetBranch("tagbgammaMCIdx");
   b_tagbgammanDaus = fChain->GetBranch("tagbgammanDaus");
   b_nTRK = fChain->GetBranch("nTRK");
   b_TRKLund = fChain->GetBranch("TRKLund");
   b_TRKnHitXY = fChain->GetBranch("TRKnHitXY");
   b_TRKnHitZ = fChain->GetBranch("TRKnHitZ");
   b_TRKnSvtXY = fChain->GetBranch("TRKnSvtXY");
   b_TRKnSvtZ = fChain->GetBranch("TRKnSvtZ");
   b_TRKsvtViewXY1 = fChain->GetBranch("TRKsvtViewXY1");
   b_TRKsvtViewXY2 = fChain->GetBranch("TRKsvtViewXY2");
   b_TRKsvtViewXY3 = fChain->GetBranch("TRKsvtViewXY3");
   b_TRKsvtViewXY4 = fChain->GetBranch("TRKsvtViewXY4");
   b_TRKsvtViewXY5 = fChain->GetBranch("TRKsvtViewXY5");
   b_TRKsvtViewZ1 = fChain->GetBranch("TRKsvtViewZ1");
   b_TRKsvtViewZ2 = fChain->GetBranch("TRKsvtViewZ2");
   b_TRKsvtViewZ3 = fChain->GetBranch("TRKsvtViewZ3");
   b_TRKsvtViewZ4 = fChain->GetBranch("TRKsvtViewZ4");
   b_TRKsvtViewZ5 = fChain->GetBranch("TRKsvtViewZ5");
   b_TRKnDchXY = fChain->GetBranch("TRKnDchXY");
   b_TRKnDchZ = fChain->GetBranch("TRKnDchZ");
   b_TRKdchFirst = fChain->GetBranch("TRKdchFirst");
   b_TRKdchLast = fChain->GetBranch("TRKdchLast");
   b_TRKEff = fChain->GetBranch("TRKEff");
   b_TRKEfferr = fChain->GetBranch("TRKEfferr");
   b_TRKchEff = fChain->GetBranch("TRKchEff");
   b_TRKchEfferr = fChain->GetBranch("TRKchEfferr");
   b_KSelectorsMap = fChain->GetBranch("KSelectorsMap");
   b_eSelectorsMap = fChain->GetBranch("eSelectorsMap");
   b_muSelectorsMap = fChain->GetBranch("muSelectorsMap");
   b_piSelectorsMap = fChain->GetBranch("piSelectorsMap");
   b_pSelectorsMap = fChain->GetBranch("pSelectorsMap");
   b_TRKDrcTh = fChain->GetBranch("TRKDrcTh");
   b_TRKDrcThErr = fChain->GetBranch("TRKDrcThErr");
   b_TRKDrcDeltaMom = fChain->GetBranch("TRKDrcDeltaMom");
   b_TRKDrcNph = fChain->GetBranch("TRKDrcNph");
   b_TRKDrcNbgph = fChain->GetBranch("TRKDrcNbgph");
   b_TRKdedxdch = fChain->GetBranch("TRKdedxdch");
   b_TRKdedxdchsamp = fChain->GetBranch("TRKdedxdchsamp");
   b_TRKDchDeltaMom = fChain->GetBranch("TRKDchDeltaMom");
   b_gammaLat = fChain->GetBranch("gammaLat");
   b_gammaA42 = fChain->GetBranch("gammaA42");
   b_gammaECal = fChain->GetBranch("gammaECal");
   b_gammaErawCal = fChain->GetBranch("gammaErawCal");
   b_gammaCentx = fChain->GetBranch("gammaCentx");
   b_gammaCenty = fChain->GetBranch("gammaCenty");
   b_gammaCentz = fChain->GetBranch("gammaCentz");
   b_gammanCrys = fChain->GetBranch("gammanCrys");

   return kTRUE;
}

void BtuRootFile::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t BtuRootFile::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef BtuRootFile_cxx
