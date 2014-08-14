
#ifndef DRecoCheckCharge_HH
#define DRecoCheckCharge_HH 1

//----------------------
// Base Class Headers --
//----------------------
#include "TagModules/TagFilterModule.hh"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

#include "Framework/AbsParmDouble.hh"
#include "Framework/AbsParmBool.hh"
#include "BetaMiniUser/NamedScalers.hh"

class AbsEvent;

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class DRecoCheckCharge : public TagFilterModule {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  DRecoCheckCharge( const char* const N, const char* const D );
  
  // Destructor
  virtual ~DRecoCheckCharge( );
  
  // Operations
  
  virtual AppResult           beginJob( AbsEvent* anEvent );
  virtual AppResult           event   ( AbsEvent* anEvent );
  virtual AppResult           endJob  ( AbsEvent* anEvent );
  
protected:
  AbsParmBool SelectWrongSign;

private:
  NamedScalers _scalers; 
};

#endif
