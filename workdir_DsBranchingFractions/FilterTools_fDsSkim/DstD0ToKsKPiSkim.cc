//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: DstD0ToKsKPiSkim.cc,v 1.1 2008/11/26 22:33:25 dubrovin Exp $
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

#include "BaBar/BaBar.hh"
#include "FilterTools/DstD0ToKsKPiSkim.hh"

///////////////
#include "CLHEP/Alist/AList.h"
#include "CLHEP/Alist/AIterator.h"
#include "CLHEP/Matrix/Vector.h"
#include "CLHEP/Vector/LorentzVector.h"
#include "AbsEnv/AbsEnv.hh"
#include "AbsEvent/AbsEvent.hh"
#include "AbsEvent/getTmpAList.hh"
#include "ErrLogger/ErrLog.hh"
#include "AbsEventTag/AbsEventTag.hh"
#include "Beta/EventInfo.hh"
#include "ProxyDict/IfdStrKey.hh"

#include <iostream>
#include <math.h>

using std::endl;
using std::cout;
using std::ostream;

//------------------------------------------------------------------------------

DstD0ToKsKPiSkim::DstD0ToKsKPiSkim( const char* const theName, 
			            const char* const theDescription )
  : TagFilterModule( theName, theDescription )
  , _outputListName1("outputList1", this, "DstarD0ToKShortKPi")
  , _nEventsRead(0) 
  , _nEventsPassed(0) 
{
  // final list(s)
  commands()->append(&_outputListName1);
  // intermediate list
  //commands()->append(&_ksListName);
  //commands()->append(&_slowPionListName);
  //commands()->append(&_D0ToKsKPiListName);
}

//------------------------------------------------------------------------------

DstD0ToKsKPiSkim::~DstD0ToKsKPiSkim()
{
}

//------------------------------------------------------------------------------

AppResult
DstD0ToKsKPiSkim::beginJob( AbsEvent* anEvent )
{
  ErrMsg(routine)<<" Begin job"<<endmsg; 
  return AppResult::OK;
}

//------------------------------------------------------------------------------

AppResult
DstD0ToKsKPiSkim::event( AbsEvent* anEvent )
{
  _nEventsRead++;
  
  BtaCandList* outputList1 = Ifd< BtaCandList >::get(anEvent, _outputListName1.value());

  if (!outputList1) ErrMsg(fatal) << "No output list " 
				  << _outputListName1.valueString().c_str()
				  << " found in event" << endmsg;

  bool accept = (outputList1 != 0 && outputList1->length() > 0);

  if  (accept)
  {
     _nEventsPassed++;

     //-----------------------
     // Loop over the list

     // BtaCandidate* cand(0);
     // BtaCandIter    iterList(*outputList1);
     // while ( cand = iterList() )
     // {
     //    printDecayChain( cand ); cout << endl;
     // }
  }

  setPassed(accept);
  return AppResult::OK;
}

//------------------------------------------------------------------------------

AppResult
DstD0ToKsKPiSkim::endJob( AbsEvent* anEvent )
{
  ErrMsg(routine) << " End of job: # events read: " << _nEventsRead  
                  << " passed: "                    << _nEventsPassed << endmsg;
  return AppResult::OK;
}

//------------------------------------------------------------------------------

void 
DstD0ToKsKPiSkim::printDecayChain( BtaCandidate* cand )
{
  if( cand->nDaughters() < 2 ) return;
      // Print the decay chain: the parent and all nearest daughters
     cout << " " << cand->pdtEntry()->name() << " -> ";

     BtaCandIter candDaugIter( cand->daughterIterator() );
     while( BtaCandidate* daug = candDaugIter() )
       {
          cout << daug->pdtEntry()->name() << " ";
       }
          cout << "; ";

     // Loop over daughters and and print recursively their chains

                                 candDaugIter.rewind();
     while( BtaCandidate* daug = candDaugIter() )
       {
          if(daug->nDaughters() > 1) printDecayChain( daug );
       }
}

//------------------------------------------------------------------------------

