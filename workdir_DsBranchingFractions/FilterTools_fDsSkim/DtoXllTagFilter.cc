//--------------------------------------------------------------------------
// File and Version Information:
//
// Description:
//      Class DtoXllTagFilter - tag filtering for D0->ll analysis
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Ajit Mohapatra                Original author
//      Chunhui Chen
// Modification:
// Chunhui Chen 3/2/2004: remove some error message output
//
// Copyright Information:
//      Copyright (C) 2004                UW-Madison
//
//------------------------------------------------------------------------
#include"BaBar/BaBar.hh"
#include <iomanip>
//-----------------------
// This Class's Header --
//-----------------------            
#include "FilterTools/DtoXllTagFilter.hh"
//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "TagData/TagTransient.hh"
#include "TagData/TagTransientBoolIter.hh"
#include "TagData/TagTransientIntIter.hh"
#include "TagData/TagTransientFloatIter.hh"

#include "AbsEnv/AbsEnv.hh"
#include "GenEnv/GenEnv.hh"
#include "HepTuple/TupleManager.h"
#include "HepTuple/Histogram.h"

#include "ErrLogger/ErrLog.hh"

#include "HepTuple/Tuple.h"
using std::cout;
using std::endl;

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------
 
//----------------
// Constructors --
//----------------                                   


DtoXllTagFilter::DtoXllTagFilter( const char* const theName, 
			const char* const theDescription )
  : TagFilterModule( theName, theDescription ) 
  , _makeNtuple("makeNtuple", this, false)
{ 
   commands()->append( & _makeNtuple );
}
		
//--------------
// Destructor --
//--------------
DtoXllTagFilter::~DtoXllTagFilter() 
{ 
}


AppResult DtoXllTagFilter::beginJob( AbsEvent*  anEvent ) {

  if ( _verbose.value() ) cout << name() << ": begin job" << endl;

  _nReadEvents = 0;
  _nPassed = 0;

  HepTupleManager* manager = gblEnv->getGen()->ntupleManager();
  

  if (_makeNtuple.value()) {
     ErrMsg(routine) << "DtoXllTagFilter: will book an ntuple " << endmsg ;
     _tagNtuple = manager->ntuple("D0ll tag ntuple") ;            
  }

  return AppResult::OK;
}

//--------------
// Operations --
//--------------
              
AppResult DtoXllTagFilter::event( AbsEvent* anEvent ) {

  _nReadEvents++;

  if (_verbose.value()){
    ErrMsg(routine) << "DtoXllTagFilter: processing " 
		    << _nReadEvents << endmsg;
  }

  if (_verbose.value()&&(_nReadEvents%1000==0)) {
    ErrMsg(routine) << " DtoXllTagFilter:event   " 
	 << _nReadEvents << endmsg;
  }

  bool status;
  bool passed=false;

  // Build the tag accessor using the base class
  TagFilterModule::event( anEvent );
  
  if ( ! tag() ) {
    ErrMsg(error) << " DtoXllTagFilter::event *** Error! "
		  << ": No tag! " << endmsg;
    setPassed(false);
    return AppResult::OK;
  }

  bool llMassCut;

  status = tag()->getBool(llMassCut,"eLHmuTightMassCut");

  if (!status){
    //    ErrMsg(error) << " DtoXllTagFilter::tagbit missing:" << endmsg;
    //    if (!tag()->getBool(llMassCut,"eLHmuTightMassCut")){
    //  ErrMsg(error) <<"eLHmuTightMassCut"<< endmsg;
    //}

    setPassed(false);
    return AppResult::OK;
  }

  
  // First require that L3 triggered event (for MC primarily)
  passed = llMassCut;

  setPassed( passed );  

  if (passed) _nPassed++;

  // If Ntuple filling requested
  if (_makeNtuple.value()) {

    _tagNtuple->column("llMassCut",(int)llMassCut);
    _tagNtuple->dumpData();
 
  } // ntuple.
                                                                                                 

  return AppResult::OK;

}

AppResult DtoXllTagFilter::endJob( AbsEvent* anEvent ) {

  ErrMsg(routine) << name()  << " end job: " << _nPassed << " out of " 
       << _nReadEvents << " passed" << endmsg;
  
  return AppResult::OK;
}




