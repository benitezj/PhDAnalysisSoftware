#ifndef DSPPBARFILTER_HH
#define DSPPBARFILTER_HH 

//----------------------
// Base Class Headers --
//----------------------
#include "CLHEP/Alist/AList.h"
#include "CLHEP/Matrix/Vector.h"
#include "CLHEP/Vector/LorentzVector.h"

#include "Beta/BtaCandidate.hh"
#include "Framework/AppFilterModule.hh"
#include "Framework/AbsParmBool.hh"
#include "Framework/AbsParmDouble.hh"
#include "Framework/AbsParmGeneral.hh"
#include "Framework/AbsParmString.hh"
#include "AbsParm/AbsParmIfdStrKey.hh"

#include "TagModules/TagFilterModule.hh"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

class AbsEvent;
class EventInfo;

//		---------------------
// 		-- Class Interface --
//		---------------------
 

class DsppbarFilter : public TagFilterModule {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
 DsppbarFilter ( const char* const theName, const char* const theDescription );

  // Destructor
  virtual ~DsppbarFilter( );
  
  // Operations
  virtual AppResult           beginJob( AbsEvent* anEvent );
  virtual AppResult           event   ( AbsEvent* anEvent );
  virtual AppResult           endJob  ( AbsEvent* anEvent );

private:
  int _nEventsRead;
  int _nEventsPassed;
};

#endif
