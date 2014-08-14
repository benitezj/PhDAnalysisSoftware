//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: BtuMyAnalysis.cc,v 1.2 2006/11/27 18:08:26 desilva Exp $
//
// Description:
//	Class MyAnalysis - a bare outline of an analysis which should
//      run on either the mini or micro, suitable for filling in with 
//      additional code.
//
//      The "event" entry point is where you should add code to 
//      process event data; define histograms & ntuples in "begin"
//
//      You might want to rename this file and class (module)
//      before making any local modifications.  
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Bob Jacobsen                    Original Author
//
// Copyright Information:
//	Copyright (C) 1994, 1995, 1996, 1997		Lawrence Berkeley Laboratory
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "BtaTupleMaker/BtuMyAnalysis.hh"

//-------------
// C Headers --
//-------------
#include <assert.h>

//---------------
// C++ Headers --
//---------------
#include <math.h>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "AbsEnv/AbsEnv.hh"
#include "AbsEvent/AbsEvent.hh"
#include "Beta/EventInfo.hh"
#include "BaBar/Constants.hh"
#include "BetaCoreTools/BtaPrintTree.hh"
#include "BetaCoreTools/BtaMcAssoc.hh"
#include "CLHEP/Alist/AIterator.h"
#include "ErrLogger/ErrLog.hh"
#include "GenEnv/GenEnv.hh"
#include "ProxyDict/IfdHepAListProxy.hh"
#include "ProxyDict/IfdStrKey.hh"
#include "ProxyDict/Ifd.hh"
#include "PDT/Pdt.hh"
#include "PDT/PdtEntry.hh"
#include "BetaCoreTools/BtaFoxWolfMom.hh"  
#include "BetaCoreTools/BtaThrust.hh"
#include "BetaCoreTools/BtaTreeNavigator.hh"

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//----------------
// Constructors --
//----------------

// in general, a module constructor should not do much.  The beginJob
// member function is a better place to put initialization
//
// This ctor initializes the three sample list-name parameters
BtuMyAnalysis::BtuMyAnalysis( const char* const theName, 
			      const char* const theDescription )
  : AppModule( theName, theDescription )
    , _eventInfoList( "eventInfoList", this, "Default" )
    , _inputList( "inputList",  this, "NULL")
    , _outputList( "outputList",  this, "NULL")
    , _evtShapeTracks("evtShapeTracks", this, "ChargedTracksAcc")
    , _evtShapeNeutrals("evtShapeNeutrals", this, "GoodNeutralLooseAcc")
    , _cosT("cosT"), _newR2("newR2"), _usrD0mass("usrMass")
    , _nevt(0)
{
  commands()->append( &_eventInfoList );
  commands()->append( &_inputList );
  commands()->append( &_outputList );
  commands()->append(& _evtShapeTracks);
  commands()->append(& _evtShapeNeutrals);
  _myEventBlock.addVariable(_newR2);
  _myCandBlock.addVariable(_cosT);
  _myD0CandBlock.addVariable(_usrD0mass);
}

//--------------
// Destructor --
//--------------

BtuMyAnalysis::~BtuMyAnalysis( )
{
}

//--------------
// Operations --
//--------------

// The begin(AppJob*) member function is run before any events are
// processed.  In this analysis, it opens the output histogram file
// and then books a number of histograms and a ntuple.

AppResult
BtuMyAnalysis::endJob( AbsEvent* anEvent )
{
  return AppResult::OK;
}

AppResult
BtuMyAnalysis::beginJob( AbsEvent* anEvent )
{
  
  return AppResult::OK;

}

AppResult
BtuMyAnalysis::event( AbsEvent* anEvent )
{
  _nevt++;
  
  HepAList< EventInfo >* infoList = 
    Ifd<HepAList< EventInfo > >::get(anEvent, _eventInfoList.value() );
  if (infoList == 0)
    ErrMsg(fatal) << "eventInfoList not in event" << endmsg;

  EventInfo* eventInfo = infoList->first();

  // Put UsrData blocks into the event
  static const IfdStrKey myEventDataKey("myEventData");
  if (! UsrIfd<UsrEventBlock>::put(anEvent,&_myEventBlock, myEventDataKey))
    ErrMsg(trace) << "Can't put myEventData " << endmsg;
  
  static const IfdStrKey myCandDataKey("myCandData");
  if (! UsrIfd<UsrCandBlock>::put(anEvent,&_myCandBlock, myCandDataKey))
    ErrMsg(error) << "Can't put myCandData " << endmsg;
  
  static const IfdStrKey myD0CandDataKey("myD0CandData");
  if (! UsrIfd<UsrCandBlock>::put(anEvent,&_myD0CandBlock, myD0CandDataKey))
    ErrMsg(error) << "Can't put myD0CandData " << endmsg;
  

  // Get the input list
  HepAList<BtaCandidate>* inputList = 
    locateList(anEvent, _inputList.valueString());

  // Get the lists for calculating R2
  HepAList<BtaCandidate> *evtShapeTracks=
    locateList(anEvent, _evtShapeTracks.valueString());
  HepAList<BtaCandidate> *evtShapeNeutrals=
    locateList(anEvent, _evtShapeNeutrals.valueString());
  HepAList<BtaCandidate> evtShapeList= *evtShapeTracks;
  evtShapeList+= *evtShapeNeutrals;

  BtaFoxWolfMom foxWol(&evtShapeList, eventInfo);
  // Assign a value to a usr variable
  _newR2= foxWol.FWnorm(2);
  // Put this usr data to event block
  if ( ! _myEventBlock.put(_newR2) )
    ErrMsg(error) << "Can't put usr variable " << _newR2.name() 
		  << " into UsrEventBlock myEventData" << endmsg;
  

  // Create an output list and put it back to the event
  HepAList<BtaCandidate>* outputList= new HepAList<BtaCandidate>;
  IfdHepAListProxy<BtaCandidate>* outproxy= new IfdHepAListProxy<BtaCandidate>(outputList);
  if ( outproxy == 0 ) {
    delete outputList;
    outputList=0;
    ErrMsg(fatal) << "Can't create a IfdHepAListProxy<BtaCandidate>" << endmsg;
  } else {
    bool ok= Ifd<HepAList<BtaCandidate> > :: put (anEvent, outproxy, _outputList.value());
    if ( ! ok ) {
      delete outproxy;
      outproxy = 0;
      ErrMsg(fatal) << "The key for output list \"" << _outputList.value()
		    << "\" may already be taken. Choose another name."
		    << endmsg;
    }
  }

  // Loop over input list
  HepAListIterator<BtaCandidate> iter(*inputList);
  while (BtaCandidate* cand = iter()) {

    // ---> calculate cos thrust
    // Create a B candidate final state list
    HepAList<BtaCandidate> bcandList;
    BtaTreeNavigator navigator(*cand);
    std::list<BtaCandidate*>::iterator finalIter =  
      navigator.getFinalCands().begin();
    while ( finalIter != navigator.getFinalCands().end() ) {
      bcandList += *finalIter;
      finalIter++;
    }
    // Create a list for the rest of the event
    HepAList<BtaCandidate> btagList;
    HepAListIterator<BtaCandidate> iter(evtShapeList);
    while ( BtaCandidate *sh= iter() )
      if ( ! cand->overlaps(*sh) ) btagList+= sh;
    
    // Thurst of the B
    BtaThrust thrusterB(bcandList, *eventInfo, BtaThrust::BTAllParticles);
    Hep3Vector thrAxBCand(thrusterB.thrust_axis());
    // Thrust of the rest of the event
    BtaThrust thrusterOther(btagList, *eventInfo, BtaThrust::BTAllParticles);
    Hep3Vector thrAxOtherCand(thrusterOther.thrust_axis());
    
    // Put cos thrust 
    if ( btagList.length() > 0 )
      _cosT= cos(thrAxOtherCand.angle( thrAxBCand ));
    else 
      _cosT= 0;
    
    // Make some selection to reject candidate here
    // if (...) continue;
    
    // Make a clone of passed event and append it to the output list
    BtaCandidate *nc= new BtaCandidate(*cand);
    outputList->append(nc);
    
    // put usr variable into usrCandBlock
    if ( ! _myCandBlock.put(*nc,_cosT) ) 
      ErrMsg(error) << "Can't put usr variable " << _cosT.name() 
		    << " into UsrCandBlock myCandData" << endmsg;

    HepAListIterator<BtaCandidate> diter= nc->daughterIterator();
    BtaCandidate *dd= diter();
    _usrD0mass= dd->mass();
    if ( ! _myD0CandBlock.put(*dd,_usrD0mass) ) 
      if ( ErrLogging(trace) )
	ErrMsg(trace) << "Can't put usr variable " << _usrD0mass.name() 
		      << " into UsrCandBlock myD0CandData" << endmsg;
 
  }

  return AppResult::OK;
}
  
HepAList<BtaCandidate>*
BtuMyAnalysis::locateList(AbsEvent* anEvent, const std::string& listKey)
{
  HepAList<BtaCandidate>* list = 
    Ifd<HepAList< BtaCandidate > >::get(anEvent, listKey.c_str());
  if (list == 0)
    ErrMsg(fatal) << "Could not locate list " << listKey.c_str()
		  << ". This is likely to be a configuration error." << endmsg;
  
  return list;
}
