
#ifndef DRecoDstarTag_HH
#define DRecoDstarTag_HH 1

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

class AbsEvent;

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class DRecoDstarTag : public TagFilterModule {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  DRecoDstarTag( const char* const N, const char* const D );
  
  // Destructor
  virtual ~DRecoDstarTag( );
  
  // Operations
  
  virtual AppResult           beginJob( AbsEvent* anEvent );
  virtual AppResult           event   ( AbsEvent* anEvent );
  virtual AppResult           endJob  ( AbsEvent* anEvent );
  
protected:

  AbsParmIfdStrKey PiList;
  AbsParmIfdStrKey Pi0List;
  AbsParmIfdStrKey GammaList;

  AbsParmDouble D0PiDeltaMMean; 
  AbsParmDouble D0Pi0DeltaMMean; 
  AbsParmDouble D0GamDeltaMMean; 
  AbsParmDouble DPlusPi0DeltaMMean; 

  AbsParmDouble D0PiDeltaMCut; 
  AbsParmDouble D0Pi0DeltaMCut; 
  AbsParmDouble D0GamDeltaMCut; 
  AbsParmDouble DPlusPi0DeltaMCut; 

  AbsParmDouble Pi0GamEnergyCut; 
  AbsParmDouble GamEnergyCut; 
  AbsParmIfdStrKey GammaPi0VetoList;
  AbsParmIfdStrKey GammaEtaVetoList;

  AbsParmDouble VtxProbCut;


private:
  NamedScalers _scalers; 
};


#endif
