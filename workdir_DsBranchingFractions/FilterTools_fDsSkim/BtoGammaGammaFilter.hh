#ifndef BTOGAMMAGAMMAFILTER_HH
#define BTOGAMMAGAMMAFILTER_HH 

//----------------------
// Base Class Headers --
//----------------------

#include "TagModules/TagFilterModule.hh"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

class AbsEvent;

//		---------------------
// 		-- Class Interface --
//		---------------------
 

class BtoGammaGammaFilter : public TagFilterModule {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  BtoGammaGammaFilter( const char* const theName, const char* const theDescription );

  // Destructor
  virtual ~BtoGammaGammaFilter( );
  
  // Operations
  virtual AppResult           beginJob( AbsEvent* anEvent );
  virtual AppResult           event   ( AbsEvent* anEvent );
  virtual AppResult           endJob  ( AbsEvent* anEvent );

private:
  int _nEventsRead;
  int _nEventsPassed;
};

#endif
