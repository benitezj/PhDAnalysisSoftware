#ifndef DmixD0ToKPiPi0Skim_hh
#define DmixD0ToKPiPi0Skim_hh

// ********************************************************************
// CVS $Id: DmixD0ToKPiPi0Skim.hh,v 1.4 2004/09/15 05:21:13 mwilson Exp $
//
// Software developed for the BaBar Detector at the SLAC B-Factory
//
// Author: Michael Wilson, UC Santa Cruz
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


class DmixD0ToKPiPi0Skim : public TagFilterModule {
public:

  DmixD0ToKPiPi0Skim( const char* const theName,
                      const char* const theDescription );

  virtual ~DmixD0ToKPiPi0Skim() { }

  virtual AppResult           beginJob( AbsEvent* anEvent );
  virtual AppResult           event   ( AbsEvent* anEvent );
  virtual AppResult           endJob  ( AbsEvent* anEvent );


protected:

  // **********************************************
  // variables that are availabe to the Framework
  // **********************************************

  AbsParmIfdStrKey     prescaledDstarListName;
  AbsParmIfdStrKey     inputDstarListName;
  AbsParmIfdStrKey     scaledDstarListName;
  AbsParmIfdStrKey     finalDstarListName;

  AbsParmGeneral<int>  prescaleFactor;


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
  int nCandsSeen;

};

#endif
