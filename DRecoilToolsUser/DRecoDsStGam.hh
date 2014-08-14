
#ifndef DRecoDsStGam_HH
#define DRecoDsStGam_HH 1

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
 
class DRecoDsStGam : public TagFilterModule {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  DRecoDsStGam( const char* const N, const char* const D );
  
  // Destructor
  virtual ~DRecoDsStGam( );
  
  // Operations
  
  virtual AppResult           beginJob( AbsEvent* anEvent );
  virtual AppResult           event   ( AbsEvent* anEvent );
  virtual AppResult           endJob  ( AbsEvent* anEvent );
  
protected:

  AbsParmIfdStrKey GamList;
  AbsParmDouble GamEnergyCutLo; 
  AbsParmDouble GamEnergyCutHi; 
  AbsParmDouble GamAngleCut;
  AbsParmIfdStrKey Pi0VetoList;
  AbsParmIfdStrKey EtaVetoList;
  
  AbsParmDouble GamLabAngleCut;
  AbsParmDouble GamLatCutLo; 
  AbsParmDouble GamLatCutHi;   
  AbsParmDouble GamBumpDistCut; 
  AbsParmDouble GamTrkDistCut; 

private:
  NamedScalers _scalers; 
};


#endif
