
#ifndef DRecoCleanUp_HH
#define DRecoCleanUp_HH 1

//----------------------
// Base Class Headers --
//----------------------
#include "TagModules/TagFilterModule.hh"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

#include "BetaMiniUser/NamedScalers.hh"
class AbsEvent;

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class DRecoCleanUp : public TagFilterModule {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  DRecoCleanUp( const char* const N, const char* const D );
  
  // Destructor
  virtual ~DRecoCleanUp( );
  
  // Operations
  
  virtual AppResult           beginJob( AbsEvent* anEvent );
  virtual AppResult           event   ( AbsEvent* anEvent );
  virtual AppResult           endJob  ( AbsEvent* anEvent );
  
protected:


private:
  NamedScalers _scalers; 
};

#endif
