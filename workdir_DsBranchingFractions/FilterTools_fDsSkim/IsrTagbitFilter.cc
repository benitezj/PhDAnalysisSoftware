//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: IsrTagbitFilter.cc,v 1.6 2006/11/23 19:05:02 desilva Exp $
//
// Description:
//	Class IsrTagbitFilter
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// History (add to end):
//      Joe Izen   Oct, 2001 - creation
//
// Author List:
//	Joe Izen    	                Original Author
//
// Copyright Information:
//	Copyright (C) 2001		University of Texas at Dallas
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "FilterTools/IsrTagbitFilter.hh"

//-------------
// C Headers --
//-------------
//extern "C" {
//}

//---------------
// C++ Headers --
//---------------

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "AbsEventTag/AbsEventTag.hh"
#include "AbsEnv/AbsEnv.hh"
#include "AbsEvent/AbsEvent.hh"
#include "AbsEvent/AbsEventID.hh"
#include "GenEnv/GenEnv.hh"
#include "ErrLogger/ErrLog.hh"
#include "Beta/EventInfo.hh"

#include "OdfCommon/odfTime.hh"
#include "EidData/EidEventTriplet.hh"
using std::cout;
using std::endl;

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------
IsrTagbitFilter::IsrTagbitFilter( const char* const theName, 
		      const char* const theDescription )
  : TagFilterModule( theName, theDescription )
     , _enableGammaISR(     "enableGammaISR",      this,      false)
     , _enableNoGammaISR(   "enableNoGammaISR",    this,      false)
     , _enableLooseGammaISR("enableLooseGammaISR", this,      false)
     , _enableRadBhabha(    "enableRadBhabha",     this,      false)
     , _nPassedNG(0)
     , _nPassedG(0)
     , _nPassedLG(0)
     , _nPassedGLG(0)
     , _nPassedRBB(0)
     , _nPassedISR(0)
     , _nPrescaledISR(0)
     , _nPrescaledRBB(0)
     , _nPassed(0)
     , _evtNum(0)
     , _nthisrun(0)
     , _thisrun(0)
     , _lastrun(-999)
{
  //  commands()->append(& _eventInfoList);
  commands()->append(& _enableGammaISR);
  commands()->append(& _enableNoGammaISR);
  commands()->append(& _enableLooseGammaISR);
  commands()->append(& _enableRadBhabha);

}

//--------------
// Destructor --
//--------------
IsrTagbitFilter::~IsrTagbitFilter()
{
}

//-------------
// Modifiers --
//-------------
AppResult
IsrTagbitFilter::beginJob( AbsEvent* anEvent )
{
  if ( _verbose.value() ) cout << name() << ": Begin Job." << endl;
  return AppResult::OK;
}

AppResult
IsrTagbitFilter::event( AbsEvent* anEvent )
{
  _evtNum++;
  
  if ( _verbose.value() ) {
    static const IfdStrKey keyAbsEventID("AbsEventID");
    AbsEventID* eventID = Ifd<AbsEventID>::get(anEvent, keyAbsEventID );
    //If the run number has changed redo the boost
    _thisrun = (int)eventID->run();
    if (_lastrun != _thisrun) {
      _lastrun = _thisrun;
      _nthisrun = 0;
    }//if (_lastrun != _thisrun)
    _nthisrun++;
    
    if ((_evtNum%1000)==0 
	|| (_evtNum<11                      )
	|| (_evtNum<101 && (_evtNum%10)==0  )
	|| (_evtNum<1001 && (_evtNum%100)==0 )
	|| (_evtNum<10001 && (_evtNum%1000)==0 )
	|| (_nthisrun==1)  ){
      odfTime timeStamp = eventID->eventIdTriplet().timeStamp();
      cout << name() << "::event processing event number " << _evtNum 
	   << "  Run " << _thisrun
	   << "  evID: " << (int)timeStamp.binary().upper << (int)timeStamp.binary().lower
	   << "  Events this run " << _nthisrun << endl;
    }//if ((_evtNum%1000)==0 
  }//if ( _verbose.value() ) 
  

  // Build the tag accessor using the base class
  TagFilterModule::event( anEvent );

  bool status(true);     //make sure tag bits are healthy
  bool passedG(false);    //if true, this event is an ISR candidate with a Gamma
  bool passedNG(false);   //if true, this event is an ISR candidate with NoGamma
  bool passedLG(false);   //if true, this event is an ISR candidate with a GammaLoose
  bool passedGLG(false);  //if true, this event is an ISR candidate with a GammaLoose and/pr a Gamma
  bool passedISR(false);  //if true, this event is an ISR candidate with any of the above
  bool passedRBB(false);  //if true, this event is a Rad Bhabha candidate
  bool passed(false);     //if true, filter passes the event

  if ( 0 != tag( ) ) {

    if (_enableNoGammaISR.value())      status &= tag( )->getBool( passedNG, "ISRNoGamma"   );
    if (_enableGammaISR.value())        status &= tag( )->getBool( passedG,  "ISRGamma"     );
    if (_enableLooseGammaISR.value())   status &= tag( )->getBool( passedLG, "ISRLooseGamma");
    if (_enableRadBhabha.value())       status &= tag( )->getBool( passedRBB,"ISRRadBhabha" );
    if (!status) {
      ErrMsg(fatal) 
	<< " tag()-getBool unable to find ISRNoGamma,ISRGamma,ISRLooseGamma, and/or ISRRhadbha tagbits."
	<< endmsg;       
    }

    if ( passedNG) {
      _nPassedNG++;
      passedISR = true;
    }//if ( passedNG) 
    
    if ( passedG) {
      _nPassedG++;
      passedISR = true;
      if ( passedLG) {
	passedGLG = true;
	_nPassedGLG++;
      }
    }//if ( passedG) 
    
    if ( passedLG) {
      _nPassedLG++;
      passedISR = true;
    }//if ( passedLG) 
    
    if (passedISR){
      _nPassedISR++;
      passed = true; 
    }
    
    
    if ( passedRBB) {
      _nPassedRBB++;
      passed = true; 
    }//if ( passedRBB) 
    
  }//if ( 0 != tag( ) )
  else{
      ErrMsg(fatal) 
	<< " tag() returns 0"
	<< endmsg;
  }//0 == tag()
  
  
  if (passed) _nPassed++;
  setPassed( passed );

//   if ( _verbose.value() ){
//     char i('f'); if (passedNG ) i = 't';
//     char j('f'); if (passedG  ) j = 't';
//     char k('f'); if (passedLG ) k = 't';
//     char n('f'); if (passedGLG) n = 't';
//     char l('f'); if (passedRBB) l = 't';
//     char m('f'); if (passed   ) m = 't';
//     cout << name() 
//  	 << ": event "        << _evtNum 
//  	 << "  NG G LG GLG RBB Event:"
// 	 << " " << i 
// 	 << " " << j
// 	 << " " << k
// 	 << " " << n
//  	 << " " << l
//  	 << " " << m
//  	 << "   Sums:"
//  	 << " " << _nPassedNG
//  	 << " " << _nPassedG
//  	 << " " << _nPassedLG
//  	 << " " << _nPassedGLG
//  	 << " " << _nPassedRBB
//  	 << "  Passed after prescale: " << _nPassed
//  	 << endl;
//   }
  
  return AppResult::OK;
}

AppResult
IsrTagbitFilter::endJob( AbsEvent* anEvent )
{
  ErrMsg(routine)   << "\t endJob summary: " << endmsg;
  ErrMsg(routine)   << "\t Events processed           : " << _evtNum        << endmsg;
  if (_enableNoGammaISR.value())
    ErrMsg(routine) << "\t Events passed    NoGammaISR: " << _nPassedNG     << endmsg;
  if (_enableGammaISR.value())
    ErrMsg(routine) << "\t Events passed      GammaISR: " << _nPassedG      << endmsg;
  if (_enableLooseGammaISR.value())
    ErrMsg(routine) << "\t Events passed LooseGammaISR: " << _nPassedLG     << endmsg;
  if (_enableLooseGammaISR.value() && _enableGammaISR.value() )
    ErrMsg(routine) << "\t Events passed G and LG  ISR: " << _nPassedGLG    << endmsg;
  if (_enableLooseGammaISR.value() || _enableGammaISR.value()|| _enableNoGammaISR.value()  ){
    ErrMsg(routine) << "\t Events passed       Any ISR: " << _nPassedISR    << endmsg;
    ErrMsg(routine)	<< "\t Events prescaled    Any ISR: " << _nPrescaledISR << endmsg;
    ErrMsg(routine) << "\t ==================================="             << endmsg;
  }
  if (_enableRadBhabha.value()){
    ErrMsg(routine) << "\t Events passed    Rad Bhabha: " << _nPassedRBB    << endmsg;
    ErrMsg(routine) << "\t Events prescaled Rad BhaBha: " << _nPrescaledRBB << endmsg;
    ErrMsg(routine) << "\t ==================================="             << endmsg;
  }
  ErrMsg(routine)   << "\t Events passed         Total: " << _nPassed       << endmsg;
  
  return AppResult::OK;
}


void
IsrTagbitFilter::help(int argc, char** argv)
{
  ErrMsg(routine) << "This clonable module filters for intial state radiation tagbits.\n"
		  << "\t There are four bits which may be turned on independently.  Result is or'ed.\n"
		  << "\t These bits set by EventTagTools/TagIsr.cc.\n"
		  << "\t enableGammaISR      set f\n"
		  << "\t enableLooseGammaISR set f\n"
		  << "\t enableNoGammaISR    set f\n"
		  << "\t enableRadBhabha     set f\n"
		  << endmsg;  
}

// clone
//
AppModule* 
IsrTagbitFilter::clone(const char* cloneName)
{
  return new IsrTagbitFilter(cloneName, "clone of IsrTagbitFilter");
}
