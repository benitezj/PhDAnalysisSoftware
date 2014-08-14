//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: BtoGammaGammaFilter.cc,v 1.3 2006/11/23 19:05:01 desilva Exp $
//
//--------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

#include "FilterTools/BtoGammaGammaFilter.hh"

#include "ErrLogger/ErrLog.hh"
#include "Beta/BtaCandidate.hh"
#include "AbsEvent/AbsEvent.hh"
#include "AbsEvent/getTmpAList.hh"
#include "AbsParm/AbsParmIfdStrKey.hh"

#include "CLHEP/Alist/AList.h"
#include "CLHEP/String/Strings.h"

BtoGammaGammaFilter::BtoGammaGammaFilter( const char* const theName, 
			      const char* const theDescription )
  : TagFilterModule( theName, theDescription )
{
}

BtoGammaGammaFilter::~BtoGammaGammaFilter()
{
}

AppResult
BtoGammaGammaFilter::beginJob( AbsEvent* anEvent )
{
  ErrMsg(routine)<<"begin filter Job"<<endmsg; 
  _nEventsRead = 0;
  _nEventsPassed = 0;
  return AppResult::OK;
}

AppResult
BtoGammaGammaFilter::event( AbsEvent* anEvent )
{

  // Keep a count of how many events have been processed 
  _nEventsRead++;

  // Declare B to gamma gamma list
  HepAList< BtaCandidate > *BtoGammaGammaList;

  // Get lists of muons and charged D's
  static const IfdStrKey keyBtoGammaGamma("BtoGammaGamma");
  getTmpAList( anEvent, BtoGammaGammaList, keyBtoGammaGamma);

  if( BtoGammaGammaList->length() > 0 ) {
    _nEventsPassed++;
    setPassed( true );
  } else {
    setPassed( false );
  }
  
  return AppResult::OK;
}

AppResult
BtoGammaGammaFilter::endJob( AbsEvent* anEvent )
{
  // Print out some statistics at the end
  std::cout << "BtoGammaGammaFilter:" << std::endl
	    << "Read " << _nEventsRead << std::endl
	    << "Passed " << _nEventsPassed << std::endl;
  return AppResult::OK;
}

