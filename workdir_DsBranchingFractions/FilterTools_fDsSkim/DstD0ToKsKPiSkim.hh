//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: DstD0ToKsKPiSkim.hh,v 1.1 2008/11/26 22:33:25 dubrovin Exp $
//
// Description:
//      module to select D*+ -> D0 pi+, D0->K0s K Pi + cc 
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Mikhail Dubrovin                Original Author
//
// Copyright Information:
//      Copyright (C) 2008              University of Cincinnati
//
//------------------------------------------------------------------------

#ifndef _DstD0ToKsKPiSkim_HH
#define _DstD0ToKsKPiSkim_HH

//----------------------
// Base Class Headers --
//----------------------
#include "CLHEP/Alist/AList.h"

#include "Beta/BtaCandidate.hh"
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
 

class DstD0ToKsKPiSkim : public TagFilterModule {

// Typedefs
typedef HepAList<BtaCandidate>         BtaCandList;
typedef HepAListIterator<BtaCandidate> BtaCandIter;

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  DstD0ToKsKPiSkim ( const char* const theName, 
		     const char* const theDescription );

  // Destructor
  virtual ~DstD0ToKsKPiSkim( );
  
  // Operations
  virtual AppResult           beginJob( AbsEvent* anEvent );
  virtual AppResult           event   ( AbsEvent* anEvent );
  virtual AppResult           endJob  ( AbsEvent* anEvent );

protected:
  AbsParmIfdStrKey _outputListName1;
  //AbsParmIfdStrKey _outputListName2;
  //AbsParmIfdStrKey _ksListName;
  //AbsParmIfdStrKey _slowPionListName;
  //AbsParmIfdStrKey _D0ToKsKPiListName;

private:

    void printDecayChain( BtaCandidate* cand );

    long _nEventsRead;
    long _nEventsPassed;
};

#endif




