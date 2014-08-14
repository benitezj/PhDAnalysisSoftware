
#ifndef DRecoTagKaon_HH
#define DRecoTagKaon_HH 1

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
 
class DRecoTagKaon : public TagFilterModule {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  DRecoTagKaon( const char* const N, const char* const D );
  
  // Destructor
  virtual ~DRecoTagKaon( );
  
  // Operations
  
  virtual AppResult           beginJob( AbsEvent* anEvent );
  virtual AppResult           event   ( AbsEvent* anEvent );
  virtual AppResult           endJob  ( AbsEvent* anEvent );
  
protected:

  AbsParmIfdStrKey KList;
  AbsParmIfdStrKey K0List;
  AbsParmDouble KaonPstarCutLo; 
  AbsParmDouble KaonPstarCutHi;

  AbsParmIfdStrKey ProtonList;

private:
  NamedScalers _scalers; 
};


#endif
