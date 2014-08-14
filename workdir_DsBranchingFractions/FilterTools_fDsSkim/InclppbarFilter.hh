#ifndef INCLPPBARFILTER_HH
#define INCLPPBARFILTER_HH 

//----------------------
// Base Class Headers --
//----------------------
#include "TagModules/TagFilterModule.hh"
#include "CLHEP/Alist/AList.h"
#include "CLHEP/Matrix/Vector.h"
#include "CLHEP/Vector/LorentzVector.h"

#include "Beta/BtaCandidate.hh"
#include "AbsParm/AbsParmIfdStrKey.hh"
#include "Framework/AbsParmBool.hh"
#include "Framework/AbsParmDouble.hh"
#include "Framework/AbsParmGeneral.hh"
#include "Framework/AbsParmString.hh"
#include "Framework/AppFilterModule.hh"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
                                                                                                                  
class AbsEvent;
class EventInfo;
class PidSelector;

//		---------------------
// 		-- Class Interface --
//		---------------------
 

class InclppbarFilter : public TagFilterModule {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  InclppbarFilter ( const char* const theName, const char* const theDescription );

  // Destructor
  virtual ~InclppbarFilter( );
  
  // Operations
  virtual AppResult           beginJob( AbsEvent* anEvent );
  virtual AppResult           event   ( AbsEvent* anEvent );
  virtual AppResult           endJob  ( AbsEvent* anEvent );

private:
  int _nEventsRead;
  int _nEventsPassed;  
};

#endif
