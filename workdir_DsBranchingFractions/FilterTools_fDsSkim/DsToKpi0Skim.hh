#ifndef DSTOKPI0SKIM_HH
#define DSTOKPI0SKIM_HH 

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

class HepHistogram;
class HepTuple;
class AbsEvent;
class BtaMcAssoc;
class EventInfo;
class BtaBooster;

//		---------------------
// 		-- Class Interface --
//		---------------------
 

class DsToKpi0Skim : public TagFilterModule {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  DsToKpi0Skim ( const char* const theName, const char* const theDescription );

  // Destructor
  virtual ~DsToKpi0Skim( );
  
  // Operations
  virtual AppResult           beginJob( AbsEvent* anEvent );
  virtual AppResult           event   ( AbsEvent* anEvent );
  virtual AppResult           endJob  ( AbsEvent* anEvent );

protected:

private:
  // Parameters
  AbsParmIfdStrKey _DsToKpi0ListName;
  AbsParmIfdStrKey _DsTopipi0ListName;
  AbsParmIfdStrKey _DsToKsKListName;
  AbsParmIfdStrKey _DsToKspiListName;

  // A list locator
  HepAList<BtaCandidate>* locateList(AbsEvent* anEvent, const std::string& s);

  // Some counters for the skim
  int _nEventsRead;
  int _nEventsPassed;

};

#endif
