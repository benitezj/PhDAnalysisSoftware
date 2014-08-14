#ifndef DCTO3PIPROMPT_HH
#define DCTO3PIPROMPT_HH

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

class DcTo3PiPromptSkim : public TagFilterModule {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  DcTo3PiPromptSkim ( const char* const theName, const char* const theDescription );

  // Destructor
  virtual ~DcTo3PiPromptSkim( );

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
