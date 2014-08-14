
#ifndef DRecoFragPions_HH
#define DRecoFragPions_HH 1

//----------------------
// Base Class Headers --
//----------------------
#include "TagModules/TagFilterModule.hh"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

#include "AbsParm/AbsParmIfdStrKey.hh"
#include "BetaMiniUser/NamedScalers.hh"
#include "Framework/AbsParmDouble.hh"
#include "VtxBase/VtxParmAlgId.hh"
#include "VtxBase/VtxParmAlgMode.hh"
#include "Framework/AbsParmBool.hh"
#include "CLHEP/Alist/AList.h"

class AbsEvent;

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class DRecoFragPions : public TagFilterModule {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  DRecoFragPions( const char* const N, const char* const D );
  
  // Destructor
  virtual ~DRecoFragPions( );
  
  // Operations
  
  virtual AppResult           beginJob( AbsEvent* anEvent );
  virtual AppResult           event   ( AbsEvent* anEvent );
  virtual AppResult           endJob  ( AbsEvent* anEvent );
  
protected:

  AbsParmIfdStrKey PiList;
  AbsParmDouble PiPCutLo;   
  AbsParmDouble PiPCutHi;
  AbsParmIfdStrKey PiKsVetoList;
  AbsParmIfdStrKey PiKVetoList;
  AbsParmDouble PiMaxNumber;

  AbsParmIfdStrKey Pi0List;
  AbsParmDouble Pi0GamEnergyCutLo;   
  AbsParmDouble Pi0GamEnergyCutHi;
  AbsParmDouble Pi0MassCutLo;
  AbsParmDouble Pi0MassCutHi;
  AbsParmBool   Pi0MassConstrain;
  AbsParmDouble Pi0MaxNumber;
  
  AbsParmIfdStrKey GammaList;
  AbsParmDouble GammaEnergyCutLo;   
  AbsParmDouble GammaEnergyCutHi; 
  AbsParmIfdStrKey GammaPi0VetoList;
  AbsParmIfdStrKey GammaEtaVetoList;
  AbsParmDouble GammaMaxNumber;

  AbsParmDouble XChargeCut; 

private:

  void CreateXCandidates(HepAList< HepAList< BtaCandidate > >* XCandidates, HepAList< BtaCandidate >* PiList, HepAList< BtaCandidate >* Pi0List, HepAList< BtaCandidate >* GammaList);
  void CreateXPi0Lists(HepAList< HepAList< BtaCandidate > >* XCandidates, HepAList< BtaCandidate >* PiList, HepAList< BtaCandidate >* Pi0List, HepAList< BtaCandidate >* GammaList);
  void CreateXGammaLists(HepAList< HepAList< BtaCandidate > >* XCandidates, HepAList< BtaCandidate >* PiPi0List, HepAList< BtaCandidate >* GammaList);
  
  NamedScalers _scalers;   
};


#endif
