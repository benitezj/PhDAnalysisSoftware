#ifndef RBD0TOKMUNUSKIM_HH
#define RBD0TOKMUNUSKIM_HH

//----------------------
// Base Class Headers --
//----------------------
#include "TagModules/TagFilterModule.hh"
#include "Framework/AbsParmBool.hh"
#include "Framework/AbsParmDouble.hh"
#include "Framework/AbsParmGeneral.hh"
#include "Framework/AbsParmString.hh"
#include "AbsParm/AbsParmIfdStrKey.hh"
#include "Beta/BtaCandidate.hh"
#include "CLHEP/Alist/AList.h"

//    ---------------------
//    -- Class Interface --
//    ---------------------

class RbD0ToKmunuSkim : public TagFilterModule {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  RbD0ToKmunuSkim ( const char* const theName, const char* const theDescription );

  // Destructor
  virtual ~RbD0ToKmunuSkim( );

  // Operations
  virtual AppResult beginJob( AbsEvent* anEvent );
  virtual AppResult event   ( AbsEvent* anEvent );
  virtual AppResult endJob  ( AbsEvent* anEvent );

protected:
  AbsParmIfdStrKey _inputList;

private:
  int _nEventsRead;
  int _nEventsPassed;

};

#endif
