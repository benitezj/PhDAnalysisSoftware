//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Apr 12 20:24:03 2007 by ROOT version 4.04/02b
// from TTree ntp1/Ntuple
// found on file: Dst2640ToDstPiPi/D0ToKPi/SP8881/NtupleFile.root
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
   Int_t           mcLen;
   Int_t           mcLund[1000];   //[mcLen]
   Int_t           mothIdx[1000];   //[mcLen]
   Int_t           dauLen[1000];   //[mcLen]
   Int_t           dauIdx[1000];   //[mcLen]
   Float_t         mcmass[1000];   //[mcLen]
   Float_t         mcp3CM[1000];   //[mcLen]
   Float_t         mccosthCM[1000];   //[mcLen]
   Float_t         mcphiCM[1000];   //[mcLen]
   Float_t         mcp3[1000];   //[mcLen]
   Float_t         mccosth[1000];   //[mcLen]
   Float_t         mcphi[1000];   //[mcLen]
   Float_t         mcVtxx[1000];   //[mcLen]
   Float_t         mcVtxy[1000];   //[mcLen]
   Float_t         mcVtxz[1000];   //[mcLen]
   Int_t           nDst2640;
   Float_t         Dst2640Chi2[1000];   //[nDst2640]
   Float_t         Dst2640Doca[1000];   //[nDst2640]
   Float_t         Dst2640DocaErr[1000];   //[nDst2640]
   Float_t         Dst2640Doca_z[1000];   //[nDst2640]
   Float_t         Dst2640Doca_zErr[1000];   //[nDst2640]
   Float_t         Dst2640Mass[1000];   //[nDst2640]
   Float_t         Dst2640MassErr[1000];   //[nDst2640]
   Float_t         Dst2640Poca_x[1000];   //[nDst2640]
   Float_t         Dst2640Poca_y[1000];   //[nDst2640]
   Float_t         Dst2640Poca_z[1000];   //[nDst2640]
   Float_t         Dst2640Vtxx[1000];   //[nDst2640]
   Float_t         Dst2640Vtxy[1000];   //[nDst2640]
   Float_t         Dst2640Vtxz[1000];   //[nDst2640]
   Float_t         Dst2640costh[1000];   //[nDst2640]
   Float_t         Dst2640costhCM[1000];   //[nDst2640]
   Float_t         Dst2640p3[1000];   //[nDst2640]
   Float_t         Dst2640p3CM[1000];   //[nDst2640]
   Float_t         Dst2640phi[1000];   //[nDst2640]
   Float_t         Dst2640phiCM[1000];   //[nDst2640]
   Int_t           Dst2640Lund[1000];   //[nDst2640]
   Int_t           Dst2640MCIdx[1000];   //[nDst2640]
   Int_t           Dst2640VtxStatus[1000];   //[nDst2640]
   Int_t           Dst2640nDof[1000];   //[nDst2640]
   Int_t           Dst2640d1Lund[1000];   //[nDst2640]
   Int_t           Dst2640d1Idx[1000];   //[nDst2640]
   Int_t           Dst2640d2Lund[1000];   //[nDst2640]
   Int_t           Dst2640d2Idx[1000];   //[nDst2640]
   Int_t           Dst2640d3Lund[1000];   //[nDst2640]
   Int_t           Dst2640d3Idx[1000];   //[nDst2640]
   Int_t           nDstar;
   Float_t         DstarChi2[1000];   //[nDstar]
   Float_t         DstarDoca[1000];   //[nDstar]
   Float_t         DstarDocaErr[1000];   //[nDstar]
   Float_t         DstarDoca_z[1000];   //[nDstar]
   Float_t         DstarDoca_zErr[1000];   //[nDstar]
   Float_t         DstarMass[1000];   //[nDstar]
   Float_t         DstarMassErr[1000];   //[nDstar]
   Float_t         DstarPoca_x[1000];   //[nDstar]
   Float_t         DstarPoca_y[1000];   //[nDstar]
   Float_t         DstarPoca_z[1000];   //[nDstar]
   Float_t         DstarVtxx[1000];   //[nDstar]
   Float_t         DstarVtxy[1000];   //[nDstar]
   Float_t         DstarVtxz[1000];   //[nDstar]
   Float_t         Dstarcosth[1000];   //[nDstar]
   Float_t         DstarcosthCM[1000];   //[nDstar]
   Float_t         Dstarp3[1000];   //[nDstar]
   Float_t         Dstarp3CM[1000];   //[nDstar]
   Float_t         Dstarphi[1000];   //[nDstar]
   Float_t         DstarphiCM[1000];   //[nDstar]
   Int_t           DstarLund[1000];   //[nDstar]
   Int_t           DstarMCIdx[1000];   //[nDstar]
   Int_t           DstarVtxStatus[1000];   //[nDstar]
   Int_t           DstarnDof[1000];   //[nDstar]
   Int_t           Dstard1Lund[1000];   //[nDstar]
   Int_t           Dstard1Idx[1000];   //[nDstar]
   Int_t           Dstard2Lund[1000];   //[nDstar]
   Int_t           Dstard2Idx[1000];   //[nDstar]
   Int_t           nD0;
   Float_t         D0Chi2[1000];   //[nD0]
   Float_t         D0Doca[1000];   //[nD0]
   Float_t         D0DocaErr[1000];   //[nD0]
   Float_t         D0Doca_z[1000];   //[nD0]
   Float_t         D0Doca_zErr[1000];   //[nD0]
   Float_t         D0FlightBSCosA[1000];   //[nD0]
   Float_t         D0FlightBSErr[1000];   //[nD0]
   Float_t         D0FlightBSLen[1000];   //[nD0]
   Float_t         D0FlightCosA[1000];   //[nD0]
   Float_t         D0FlightErr[1000];   //[nD0]
   Float_t         D0FlightLen[1000];   //[nD0]
   Float_t         D0Mass[1000];   //[nD0]
   Float_t         D0MassErr[1000];   //[nD0]
   Float_t         D0Poca_x[1000];   //[nD0]
   Float_t         D0Poca_y[1000];   //[nD0]
   Float_t         D0Poca_z[1000];   //[nD0]
   Float_t         D0Vtxx[1000];   //[nD0]
   Float_t         D0Vtxy[1000];   //[nD0]
   Float_t         D0Vtxz[1000];   //[nD0]
   Float_t         D0costh[1000];   //[nD0]
   Float_t         D0costhCM[1000];   //[nD0]
   Float_t         D0p3[1000];   //[nD0]
   Float_t         D0p3CM[1000];   //[nD0]
   Float_t         D0phi[1000];   //[nD0]
   Float_t         D0phiCM[1000];   //[nD0]
   Int_t           D0Lund[1000];   //[nD0]
   Int_t           D0MCIdx[1000];   //[nD0]
   Int_t           D0VtxStatus[1000];   //[nD0]
   Int_t           D0nDof[1000];   //[nD0]
   Int_t           D0d1Lund[1000];   //[nD0]
   Int_t           D0d1Idx[1000];   //[nD0]
   Int_t           D0d2Lund[1000];   //[nD0]
   Int_t           D0d2Idx[1000];   //[nD0]
   Int_t           nPi;
   Float_t         PiDoca[1000];   //[nPi]
   Float_t         PiDocaErr[1000];   //[nPi]
   Float_t         PiDoca_z[1000];   //[nPi]
   Float_t         PiDoca_zErr[1000];   //[nPi]
   Float_t         PiPoca_x[1000];   //[nPi]
   Float_t         PiPoca_y[1000];   //[nPi]
   Float_t         PiPoca_z[1000];   //[nPi]
   Float_t         Picosth[1000];   //[nPi]
   Float_t         PicosthCM[1000];   //[nPi]
   Float_t         Pip3[1000];   //[nPi]
   Float_t         Pip3CM[1000];   //[nPi]
   Float_t         Piphi[1000];   //[nPi]
   Float_t         PiphiCM[1000];   //[nPi]
   Int_t           PiLund[1000];   //[nPi]
   Int_t           PiMCIdx[1000];   //[nPi]
   Int_t           PiTrkIdx[1000];   //[nPi]
   Int_t           nK;
   Float_t         KDoca[1000];   //[nK]
   Float_t         KDocaErr[1000];   //[nK]
   Float_t         KDoca_z[1000];   //[nK]
   Float_t         KDoca_zErr[1000];   //[nK]
   Float_t         KPoca_x[1000];   //[nK]
   Float_t         KPoca_y[1000];   //[nK]
   Float_t         KPoca_z[1000];   //[nK]
   Float_t         Kcosth[1000];   //[nK]
   Float_t         KcosthCM[1000];   //[nK]
   Float_t         Kp3[1000];   //[nK]
   Float_t         Kp3CM[1000];   //[nK]
   Float_t         Kphi[1000];   //[nK]
   Float_t         KphiCM[1000];   //[nK]
   Int_t           KLund[1000];   //[nK]
   Int_t           KMCIdx[1000];   //[nK]
   Int_t           KTrkIdx[1000];   //[nK]
   Int_t           nTRK;
   Int_t           TRKLund[1000];   //[nTRK]
   Int_t           TRKnSvt[1000];   //[nTRK]
   Int_t           TRKnDch[1000];   //[nTRK]
   Int_t           TRKnHit[1000];   //[nTRK]
   Int_t           piSelectorsMap[1000];   //[nTRK]

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
   TBranch        *b_nDst2640;   //!
   TBranch        *b_Dst2640Chi2;   //!
   TBranch        *b_Dst2640Doca;   //!
   TBranch        *b_Dst2640DocaErr;   //!
   TBranch        *b_Dst2640Doca_z;   //!
   TBranch        *b_Dst2640Doca_zErr;   //!
   TBranch        *b_Dst2640Mass;   //!
   TBranch        *b_Dst2640MassErr;   //!
   TBranch        *b_Dst2640Poca_x;   //!
   TBranch        *b_Dst2640Poca_y;   //!
   TBranch        *b_Dst2640Poca_z;   //!
   TBranch        *b_Dst2640Vtxx;   //!
   TBranch        *b_Dst2640Vtxy;   //!
   TBranch        *b_Dst2640Vtxz;   //!
   TBranch        *b_Dst2640costh;   //!
   TBranch        *b_Dst2640costhCM;   //!
   TBranch        *b_Dst2640p3;   //!
   TBranch        *b_Dst2640p3CM;   //!
   TBranch        *b_Dst2640phi;   //!
   TBranch        *b_Dst2640phiCM;   //!
   TBranch        *b_Dst2640Lund;   //!
   TBranch        *b_Dst2640MCIdx;   //!
   TBranch        *b_Dst2640VtxStatus;   //!
   TBranch        *b_Dst2640nDof;   //!
   TBranch        *b_Dst2640d1Lund;   //!
   TBranch        *b_Dst2640d1Idx;   //!
   TBranch        *b_Dst2640d2Lund;   //!
   TBranch        *b_Dst2640d2Idx;   //!
   TBranch        *b_Dst2640d3Lund;   //!
   TBranch        *b_Dst2640d3Idx;   //!
   TBranch        *b_nDstar;   //!
   TBranch        *b_DstarChi2;   //!
   TBranch        *b_DstarDoca;   //!
   TBranch        *b_DstarDocaErr;   //!
   TBranch        *b_DstarDoca_z;   //!
   TBranch        *b_DstarDoca_zErr;   //!
   TBranch        *b_DstarMass;   //!
   TBranch        *b_DstarMassErr;   //!
   TBranch        *b_DstarPoca_x;   //!
   TBranch        *b_DstarPoca_y;   //!
   TBranch        *b_DstarPoca_z;   //!
   TBranch        *b_DstarVtxx;   //!
   TBranch        *b_DstarVtxy;   //!
   TBranch        *b_DstarVtxz;   //!
   TBranch        *b_Dstarcosth;   //!
   TBranch        *b_DstarcosthCM;   //!
   TBranch        *b_Dstarp3;   //!
   TBranch        *b_Dstarp3CM;   //!
   TBranch        *b_Dstarphi;   //!
   TBranch        *b_DstarphiCM;   //!
   TBranch        *b_DstarLund;   //!
   TBranch        *b_DstarMCIdx;   //!
   TBranch        *b_DstarVtxStatus;   //!
   TBranch        *b_DstarnDof;   //!
   TBranch        *b_Dstard1Lund;   //!
   TBranch        *b_Dstard1Idx;   //!
   TBranch        *b_Dstard2Lund;   //!
   TBranch        *b_Dstard2Idx;   //!
   TBranch        *b_nD0;   //!
   TBranch        *b_D0Chi2;   //!
   TBranch        *b_D0Doca;   //!
   TBranch        *b_D0DocaErr;   //!
   TBranch        *b_D0Doca_z;   //!
   TBranch        *b_D0Doca_zErr;   //!
   TBranch        *b_D0FlightBSCosA;   //!
   TBranch        *b_D0FlightBSErr;   //!
   TBranch        *b_D0FlightBSLen;   //!
   TBranch        *b_D0FlightCosA;   //!
   TBranch        *b_D0FlightErr;   //!
   TBranch        *b_D0FlightLen;   //!
   TBranch        *b_D0Mass;   //!
   TBranch        *b_D0MassErr;   //!
   TBranch        *b_D0Poca_x;   //!
   TBranch        *b_D0Poca_y;   //!
   TBranch        *b_D0Poca_z;   //!
   TBranch        *b_D0Vtxx;   //!
   TBranch        *b_D0Vtxy;   //!
   TBranch        *b_D0Vtxz;   //!
   TBranch        *b_D0costh;   //!
   TBranch        *b_D0costhCM;   //!
   TBranch        *b_D0p3;   //!
   TBranch        *b_D0p3CM;   //!
   TBranch        *b_D0phi;   //!
   TBranch        *b_D0phiCM;   //!
   TBranch        *b_D0Lund;   //!
   TBranch        *b_D0MCIdx;   //!
   TBranch        *b_D0VtxStatus;   //!
   TBranch        *b_D0nDof;   //!
   TBranch        *b_D0d1Lund;   //!
   TBranch        *b_D0d1Idx;   //!
   TBranch        *b_D0d2Lund;   //!
   TBranch        *b_D0d2Idx;   //!
   TBranch        *b_nPi;   //!
   TBranch        *b_PiDoca;   //!
   TBranch        *b_PiDocaErr;   //!
   TBranch        *b_PiDoca_z;   //!
   TBranch        *b_PiDoca_zErr;   //!
   TBranch        *b_PiPoca_x;   //!
   TBranch        *b_PiPoca_y;   //!
   TBranch        *b_PiPoca_z;   //!
   TBranch        *b_Picosth;   //!
   TBranch        *b_PicosthCM;   //!
   TBranch        *b_Pip3;   //!
   TBranch        *b_Pip3CM;   //!
   TBranch        *b_Piphi;   //!
   TBranch        *b_PiphiCM;   //!
   TBranch        *b_PiLund;   //!
   TBranch        *b_PiMCIdx;   //!
   TBranch        *b_PiTrkIdx;   //!
   TBranch        *b_nK;   //!
   TBranch        *b_KDoca;   //!
   TBranch        *b_KDocaErr;   //!
   TBranch        *b_KDoca_z;   //!
   TBranch        *b_KDoca_zErr;   //!
   TBranch        *b_KPoca_x;   //!
   TBranch        *b_KPoca_y;   //!
   TBranch        *b_KPoca_z;   //!
   TBranch        *b_Kcosth;   //!
   TBranch        *b_KcosthCM;   //!
   TBranch        *b_Kp3;   //!
   TBranch        *b_Kp3CM;   //!
   TBranch        *b_Kphi;   //!
   TBranch        *b_KphiCM;   //!
   TBranch        *b_KLund;   //!
   TBranch        *b_KMCIdx;   //!
   TBranch        *b_KTrkIdx;   //!
   TBranch        *b_nTRK;   //!
   TBranch        *b_TRKLund;   //!
   TBranch        *b_TRKnSvt;   //!
   TBranch        *b_TRKnDch;   //!
   TBranch        *b_TRKnHit;   //!
   TBranch        *b_piSelectorsMap;   //!

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
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("Dst2640ToDstPiPi/D0ToKPi/SP8881/NtupleFile.root");
      if (!f) {
         f = new TFile("Dst2640ToDstPiPi/D0ToKPi/SP8881/NtupleFile.root");
      }
      tree = (TTree*)gDirectory->Get("ntp1");

   }
   Init(tree);
}

BtuRootFile::~BtuRootFile()
{
   if (!fChain) return;
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
   fChain->SetBranchAddress("nDst2640",&nDst2640);
   fChain->SetBranchAddress("Dst2640Chi2",Dst2640Chi2);
   fChain->SetBranchAddress("Dst2640Doca",Dst2640Doca);
   fChain->SetBranchAddress("Dst2640DocaErr",Dst2640DocaErr);
   fChain->SetBranchAddress("Dst2640Doca_z",Dst2640Doca_z);
   fChain->SetBranchAddress("Dst2640Doca_zErr",Dst2640Doca_zErr);
   fChain->SetBranchAddress("Dst2640Mass",Dst2640Mass);
   fChain->SetBranchAddress("Dst2640MassErr",Dst2640MassErr);
   fChain->SetBranchAddress("Dst2640Poca_x",Dst2640Poca_x);
   fChain->SetBranchAddress("Dst2640Poca_y",Dst2640Poca_y);
   fChain->SetBranchAddress("Dst2640Poca_z",Dst2640Poca_z);
   fChain->SetBranchAddress("Dst2640Vtxx",Dst2640Vtxx);
   fChain->SetBranchAddress("Dst2640Vtxy",Dst2640Vtxy);
   fChain->SetBranchAddress("Dst2640Vtxz",Dst2640Vtxz);
   fChain->SetBranchAddress("Dst2640costh",Dst2640costh);
   fChain->SetBranchAddress("Dst2640costhCM",Dst2640costhCM);
   fChain->SetBranchAddress("Dst2640p3",Dst2640p3);
   fChain->SetBranchAddress("Dst2640p3CM",Dst2640p3CM);
   fChain->SetBranchAddress("Dst2640phi",Dst2640phi);
   fChain->SetBranchAddress("Dst2640phiCM",Dst2640phiCM);
   fChain->SetBranchAddress("Dst2640Lund",Dst2640Lund);
   fChain->SetBranchAddress("Dst2640MCIdx",Dst2640MCIdx);
   fChain->SetBranchAddress("Dst2640VtxStatus",Dst2640VtxStatus);
   fChain->SetBranchAddress("Dst2640nDof",Dst2640nDof);
   fChain->SetBranchAddress("Dst2640d1Lund",Dst2640d1Lund);
   fChain->SetBranchAddress("Dst2640d1Idx",Dst2640d1Idx);
   fChain->SetBranchAddress("Dst2640d2Lund",Dst2640d2Lund);
   fChain->SetBranchAddress("Dst2640d2Idx",Dst2640d2Idx);
   fChain->SetBranchAddress("Dst2640d3Lund",Dst2640d3Lund);
   fChain->SetBranchAddress("Dst2640d3Idx",Dst2640d3Idx);
   fChain->SetBranchAddress("nDstar",&nDstar);
   fChain->SetBranchAddress("DstarChi2",DstarChi2);
   fChain->SetBranchAddress("DstarDoca",DstarDoca);
   fChain->SetBranchAddress("DstarDocaErr",DstarDocaErr);
   fChain->SetBranchAddress("DstarDoca_z",DstarDoca_z);
   fChain->SetBranchAddress("DstarDoca_zErr",DstarDoca_zErr);
   fChain->SetBranchAddress("DstarMass",DstarMass);
   fChain->SetBranchAddress("DstarMassErr",DstarMassErr);
   fChain->SetBranchAddress("DstarPoca_x",DstarPoca_x);
   fChain->SetBranchAddress("DstarPoca_y",DstarPoca_y);
   fChain->SetBranchAddress("DstarPoca_z",DstarPoca_z);
   fChain->SetBranchAddress("DstarVtxx",DstarVtxx);
   fChain->SetBranchAddress("DstarVtxy",DstarVtxy);
   fChain->SetBranchAddress("DstarVtxz",DstarVtxz);
   fChain->SetBranchAddress("Dstarcosth",Dstarcosth);
   fChain->SetBranchAddress("DstarcosthCM",DstarcosthCM);
   fChain->SetBranchAddress("Dstarp3",Dstarp3);
   fChain->SetBranchAddress("Dstarp3CM",Dstarp3CM);
   fChain->SetBranchAddress("Dstarphi",Dstarphi);
   fChain->SetBranchAddress("DstarphiCM",DstarphiCM);
   fChain->SetBranchAddress("DstarLund",DstarLund);
   fChain->SetBranchAddress("DstarMCIdx",DstarMCIdx);
   fChain->SetBranchAddress("DstarVtxStatus",DstarVtxStatus);
   fChain->SetBranchAddress("DstarnDof",DstarnDof);
   fChain->SetBranchAddress("Dstard1Lund",Dstard1Lund);
   fChain->SetBranchAddress("Dstard1Idx",Dstard1Idx);
   fChain->SetBranchAddress("Dstard2Lund",Dstard2Lund);
   fChain->SetBranchAddress("Dstard2Idx",Dstard2Idx);
   fChain->SetBranchAddress("nD0",&nD0);
   fChain->SetBranchAddress("D0Chi2",D0Chi2);
   fChain->SetBranchAddress("D0Doca",D0Doca);
   fChain->SetBranchAddress("D0DocaErr",D0DocaErr);
   fChain->SetBranchAddress("D0Doca_z",D0Doca_z);
   fChain->SetBranchAddress("D0Doca_zErr",D0Doca_zErr);
   fChain->SetBranchAddress("D0FlightBSCosA",D0FlightBSCosA);
   fChain->SetBranchAddress("D0FlightBSErr",D0FlightBSErr);
   fChain->SetBranchAddress("D0FlightBSLen",D0FlightBSLen);
   fChain->SetBranchAddress("D0FlightCosA",D0FlightCosA);
   fChain->SetBranchAddress("D0FlightErr",D0FlightErr);
   fChain->SetBranchAddress("D0FlightLen",D0FlightLen);
   fChain->SetBranchAddress("D0Mass",D0Mass);
   fChain->SetBranchAddress("D0MassErr",D0MassErr);
   fChain->SetBranchAddress("D0Poca_x",D0Poca_x);
   fChain->SetBranchAddress("D0Poca_y",D0Poca_y);
   fChain->SetBranchAddress("D0Poca_z",D0Poca_z);
   fChain->SetBranchAddress("D0Vtxx",D0Vtxx);
   fChain->SetBranchAddress("D0Vtxy",D0Vtxy);
   fChain->SetBranchAddress("D0Vtxz",D0Vtxz);
   fChain->SetBranchAddress("D0costh",D0costh);
   fChain->SetBranchAddress("D0costhCM",D0costhCM);
   fChain->SetBranchAddress("D0p3",D0p3);
   fChain->SetBranchAddress("D0p3CM",D0p3CM);
   fChain->SetBranchAddress("D0phi",D0phi);
   fChain->SetBranchAddress("D0phiCM",D0phiCM);
   fChain->SetBranchAddress("D0Lund",D0Lund);
   fChain->SetBranchAddress("D0MCIdx",D0MCIdx);
   fChain->SetBranchAddress("D0VtxStatus",D0VtxStatus);
   fChain->SetBranchAddress("D0nDof",D0nDof);
   fChain->SetBranchAddress("D0d1Lund",D0d1Lund);
   fChain->SetBranchAddress("D0d1Idx",D0d1Idx);
   fChain->SetBranchAddress("D0d2Lund",D0d2Lund);
   fChain->SetBranchAddress("D0d2Idx",D0d2Idx);
   fChain->SetBranchAddress("nPi",&nPi);
   fChain->SetBranchAddress("PiDoca",PiDoca);
   fChain->SetBranchAddress("PiDocaErr",PiDocaErr);
   fChain->SetBranchAddress("PiDoca_z",PiDoca_z);
   fChain->SetBranchAddress("PiDoca_zErr",PiDoca_zErr);
   fChain->SetBranchAddress("PiPoca_x",PiPoca_x);
   fChain->SetBranchAddress("PiPoca_y",PiPoca_y);
   fChain->SetBranchAddress("PiPoca_z",PiPoca_z);
   fChain->SetBranchAddress("Picosth",Picosth);
   fChain->SetBranchAddress("PicosthCM",PicosthCM);
   fChain->SetBranchAddress("Pip3",Pip3);
   fChain->SetBranchAddress("Pip3CM",Pip3CM);
   fChain->SetBranchAddress("Piphi",Piphi);
   fChain->SetBranchAddress("PiphiCM",PiphiCM);
   fChain->SetBranchAddress("PiLund",PiLund);
   fChain->SetBranchAddress("PiMCIdx",PiMCIdx);
   fChain->SetBranchAddress("PiTrkIdx",PiTrkIdx);
   fChain->SetBranchAddress("nK",&nK);
   fChain->SetBranchAddress("KDoca",KDoca);
   fChain->SetBranchAddress("KDocaErr",KDocaErr);
   fChain->SetBranchAddress("KDoca_z",KDoca_z);
   fChain->SetBranchAddress("KDoca_zErr",KDoca_zErr);
   fChain->SetBranchAddress("KPoca_x",KPoca_x);
   fChain->SetBranchAddress("KPoca_y",KPoca_y);
   fChain->SetBranchAddress("KPoca_z",KPoca_z);
   fChain->SetBranchAddress("Kcosth",Kcosth);
   fChain->SetBranchAddress("KcosthCM",KcosthCM);
   fChain->SetBranchAddress("Kp3",Kp3);
   fChain->SetBranchAddress("Kp3CM",Kp3CM);
   fChain->SetBranchAddress("Kphi",Kphi);
   fChain->SetBranchAddress("KphiCM",KphiCM);
   fChain->SetBranchAddress("KLund",KLund);
   fChain->SetBranchAddress("KMCIdx",KMCIdx);
   fChain->SetBranchAddress("KTrkIdx",KTrkIdx);
   fChain->SetBranchAddress("nTRK",&nTRK);
   fChain->SetBranchAddress("TRKLund",TRKLund);
   fChain->SetBranchAddress("TRKnSvt",TRKnSvt);
   fChain->SetBranchAddress("TRKnDch",TRKnDch);
   fChain->SetBranchAddress("TRKnHit",TRKnHit);
   fChain->SetBranchAddress("piSelectorsMap",piSelectorsMap);
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
   b_nDst2640 = fChain->GetBranch("nDst2640");
   b_Dst2640Chi2 = fChain->GetBranch("Dst2640Chi2");
   b_Dst2640Doca = fChain->GetBranch("Dst2640Doca");
   b_Dst2640DocaErr = fChain->GetBranch("Dst2640DocaErr");
   b_Dst2640Doca_z = fChain->GetBranch("Dst2640Doca_z");
   b_Dst2640Doca_zErr = fChain->GetBranch("Dst2640Doca_zErr");
   b_Dst2640Mass = fChain->GetBranch("Dst2640Mass");
   b_Dst2640MassErr = fChain->GetBranch("Dst2640MassErr");
   b_Dst2640Poca_x = fChain->GetBranch("Dst2640Poca_x");
   b_Dst2640Poca_y = fChain->GetBranch("Dst2640Poca_y");
   b_Dst2640Poca_z = fChain->GetBranch("Dst2640Poca_z");
   b_Dst2640Vtxx = fChain->GetBranch("Dst2640Vtxx");
   b_Dst2640Vtxy = fChain->GetBranch("Dst2640Vtxy");
   b_Dst2640Vtxz = fChain->GetBranch("Dst2640Vtxz");
   b_Dst2640costh = fChain->GetBranch("Dst2640costh");
   b_Dst2640costhCM = fChain->GetBranch("Dst2640costhCM");
   b_Dst2640p3 = fChain->GetBranch("Dst2640p3");
   b_Dst2640p3CM = fChain->GetBranch("Dst2640p3CM");
   b_Dst2640phi = fChain->GetBranch("Dst2640phi");
   b_Dst2640phiCM = fChain->GetBranch("Dst2640phiCM");
   b_Dst2640Lund = fChain->GetBranch("Dst2640Lund");
   b_Dst2640MCIdx = fChain->GetBranch("Dst2640MCIdx");
   b_Dst2640VtxStatus = fChain->GetBranch("Dst2640VtxStatus");
   b_Dst2640nDof = fChain->GetBranch("Dst2640nDof");
   b_Dst2640d1Lund = fChain->GetBranch("Dst2640d1Lund");
   b_Dst2640d1Idx = fChain->GetBranch("Dst2640d1Idx");
   b_Dst2640d2Lund = fChain->GetBranch("Dst2640d2Lund");
   b_Dst2640d2Idx = fChain->GetBranch("Dst2640d2Idx");
   b_Dst2640d3Lund = fChain->GetBranch("Dst2640d3Lund");
   b_Dst2640d3Idx = fChain->GetBranch("Dst2640d3Idx");
   b_nDstar = fChain->GetBranch("nDstar");
   b_DstarChi2 = fChain->GetBranch("DstarChi2");
   b_DstarDoca = fChain->GetBranch("DstarDoca");
   b_DstarDocaErr = fChain->GetBranch("DstarDocaErr");
   b_DstarDoca_z = fChain->GetBranch("DstarDoca_z");
   b_DstarDoca_zErr = fChain->GetBranch("DstarDoca_zErr");
   b_DstarMass = fChain->GetBranch("DstarMass");
   b_DstarMassErr = fChain->GetBranch("DstarMassErr");
   b_DstarPoca_x = fChain->GetBranch("DstarPoca_x");
   b_DstarPoca_y = fChain->GetBranch("DstarPoca_y");
   b_DstarPoca_z = fChain->GetBranch("DstarPoca_z");
   b_DstarVtxx = fChain->GetBranch("DstarVtxx");
   b_DstarVtxy = fChain->GetBranch("DstarVtxy");
   b_DstarVtxz = fChain->GetBranch("DstarVtxz");
   b_Dstarcosth = fChain->GetBranch("Dstarcosth");
   b_DstarcosthCM = fChain->GetBranch("DstarcosthCM");
   b_Dstarp3 = fChain->GetBranch("Dstarp3");
   b_Dstarp3CM = fChain->GetBranch("Dstarp3CM");
   b_Dstarphi = fChain->GetBranch("Dstarphi");
   b_DstarphiCM = fChain->GetBranch("DstarphiCM");
   b_DstarLund = fChain->GetBranch("DstarLund");
   b_DstarMCIdx = fChain->GetBranch("DstarMCIdx");
   b_DstarVtxStatus = fChain->GetBranch("DstarVtxStatus");
   b_DstarnDof = fChain->GetBranch("DstarnDof");
   b_Dstard1Lund = fChain->GetBranch("Dstard1Lund");
   b_Dstard1Idx = fChain->GetBranch("Dstard1Idx");
   b_Dstard2Lund = fChain->GetBranch("Dstard2Lund");
   b_Dstard2Idx = fChain->GetBranch("Dstard2Idx");
   b_nD0 = fChain->GetBranch("nD0");
   b_D0Chi2 = fChain->GetBranch("D0Chi2");
   b_D0Doca = fChain->GetBranch("D0Doca");
   b_D0DocaErr = fChain->GetBranch("D0DocaErr");
   b_D0Doca_z = fChain->GetBranch("D0Doca_z");
   b_D0Doca_zErr = fChain->GetBranch("D0Doca_zErr");
   b_D0FlightBSCosA = fChain->GetBranch("D0FlightBSCosA");
   b_D0FlightBSErr = fChain->GetBranch("D0FlightBSErr");
   b_D0FlightBSLen = fChain->GetBranch("D0FlightBSLen");
   b_D0FlightCosA = fChain->GetBranch("D0FlightCosA");
   b_D0FlightErr = fChain->GetBranch("D0FlightErr");
   b_D0FlightLen = fChain->GetBranch("D0FlightLen");
   b_D0Mass = fChain->GetBranch("D0Mass");
   b_D0MassErr = fChain->GetBranch("D0MassErr");
   b_D0Poca_x = fChain->GetBranch("D0Poca_x");
   b_D0Poca_y = fChain->GetBranch("D0Poca_y");
   b_D0Poca_z = fChain->GetBranch("D0Poca_z");
   b_D0Vtxx = fChain->GetBranch("D0Vtxx");
   b_D0Vtxy = fChain->GetBranch("D0Vtxy");
   b_D0Vtxz = fChain->GetBranch("D0Vtxz");
   b_D0costh = fChain->GetBranch("D0costh");
   b_D0costhCM = fChain->GetBranch("D0costhCM");
   b_D0p3 = fChain->GetBranch("D0p3");
   b_D0p3CM = fChain->GetBranch("D0p3CM");
   b_D0phi = fChain->GetBranch("D0phi");
   b_D0phiCM = fChain->GetBranch("D0phiCM");
   b_D0Lund = fChain->GetBranch("D0Lund");
   b_D0MCIdx = fChain->GetBranch("D0MCIdx");
   b_D0VtxStatus = fChain->GetBranch("D0VtxStatus");
   b_D0nDof = fChain->GetBranch("D0nDof");
   b_D0d1Lund = fChain->GetBranch("D0d1Lund");
   b_D0d1Idx = fChain->GetBranch("D0d1Idx");
   b_D0d2Lund = fChain->GetBranch("D0d2Lund");
   b_D0d2Idx = fChain->GetBranch("D0d2Idx");
   b_nPi = fChain->GetBranch("nPi");
   b_PiDoca = fChain->GetBranch("PiDoca");
   b_PiDocaErr = fChain->GetBranch("PiDocaErr");
   b_PiDoca_z = fChain->GetBranch("PiDoca_z");
   b_PiDoca_zErr = fChain->GetBranch("PiDoca_zErr");
   b_PiPoca_x = fChain->GetBranch("PiPoca_x");
   b_PiPoca_y = fChain->GetBranch("PiPoca_y");
   b_PiPoca_z = fChain->GetBranch("PiPoca_z");
   b_Picosth = fChain->GetBranch("Picosth");
   b_PicosthCM = fChain->GetBranch("PicosthCM");
   b_Pip3 = fChain->GetBranch("Pip3");
   b_Pip3CM = fChain->GetBranch("Pip3CM");
   b_Piphi = fChain->GetBranch("Piphi");
   b_PiphiCM = fChain->GetBranch("PiphiCM");
   b_PiLund = fChain->GetBranch("PiLund");
   b_PiMCIdx = fChain->GetBranch("PiMCIdx");
   b_PiTrkIdx = fChain->GetBranch("PiTrkIdx");
   b_nK = fChain->GetBranch("nK");
   b_KDoca = fChain->GetBranch("KDoca");
   b_KDocaErr = fChain->GetBranch("KDocaErr");
   b_KDoca_z = fChain->GetBranch("KDoca_z");
   b_KDoca_zErr = fChain->GetBranch("KDoca_zErr");
   b_KPoca_x = fChain->GetBranch("KPoca_x");
   b_KPoca_y = fChain->GetBranch("KPoca_y");
   b_KPoca_z = fChain->GetBranch("KPoca_z");
   b_Kcosth = fChain->GetBranch("Kcosth");
   b_KcosthCM = fChain->GetBranch("KcosthCM");
   b_Kp3 = fChain->GetBranch("Kp3");
   b_Kp3CM = fChain->GetBranch("Kp3CM");
   b_Kphi = fChain->GetBranch("Kphi");
   b_KphiCM = fChain->GetBranch("KphiCM");
   b_KLund = fChain->GetBranch("KLund");
   b_KMCIdx = fChain->GetBranch("KMCIdx");
   b_KTrkIdx = fChain->GetBranch("KTrkIdx");
   b_nTRK = fChain->GetBranch("nTRK");
   b_TRKLund = fChain->GetBranch("TRKLund");
   b_TRKnSvt = fChain->GetBranch("TRKnSvt");
   b_TRKnDch = fChain->GetBranch("TRKnDch");
   b_TRKnHit = fChain->GetBranch("TRKnHit");
   b_piSelectorsMap = fChain->GetBranch("piSelectorsMap");

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
