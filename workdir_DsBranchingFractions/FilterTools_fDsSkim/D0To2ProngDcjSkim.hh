#ifndef D0To2ProngDcjSkim_hh
#define D0To2ProngDcjSkim_hh

// ********************************************************************
// Software developed for the BaBar Detector at the SLAC B-Factory
//
// ********************************************************************

#include "TagModules/TagFilterModule.hh"

#include "Framework/AbsParmBool.hh"
#include "Framework/AbsParmDouble.hh"
#include "Framework/AbsParmGeneral.hh"
#include "Framework/AbsParmString.hh"
#include "AbsParm/AbsParmIfdStrKey.hh"

#include "Beta/BtaCandidate.hh"
#include "CLHEP/Alist/AList.h"

class AbsEvent;
class EventInfo;


class D0To2ProngDcjSkim : public TagFilterModule {
public:

  D0To2ProngDcjSkim( const char* const theName,
                     const char* const theDescription );

  virtual ~D0To2ProngDcjSkim() { }

  virtual AppResult           beginJob( AbsEvent* anEvent );
  virtual AppResult           event   ( AbsEvent* anEvent );
  virtual AppResult           endJob  ( AbsEvent* anEvent );


protected:

  // **********************************************
  // variables that are availabe to the Framework
  // **********************************************

  AbsParmIfdStrKey  inputDstarListName;
  AbsParmIfdStrKey  inputD0UntaggedListName;


  // ***************************
  // class variables and methods
  // ***************************


  virtual AppResult Passed();
  virtual AppResult Failed();


  static EventInfo* FetchEventInfo( AbsEvent* anEvent );

  static HepAList<BtaCandidate>*
    FetchList( AbsEvent* anEvent, const AbsParmIfdStrKey& key );


private:

  // Some counters for the skim
  int nEventsRead;
  int nEventsPassed;

};

#endif
