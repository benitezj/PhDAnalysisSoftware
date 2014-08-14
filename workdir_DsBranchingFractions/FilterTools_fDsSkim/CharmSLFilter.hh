//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: CharmSLFilter.hh,v 0. 2005/05/20 
//
// Description:
// Charm events with a semileptonic decay.  
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Orsay                        Original Author
//
// Copyright Information:
//	Copyright (C) 2002              UVIC
//
//------------------------------------------------------------------------
#ifndef CharmSLFilter_HH
#define CharmSLFilter_HH

//----------------------
// Base Class Headers --
//----------------------
#include "TagModules/TagFilterModule.hh"
#include "Framework/AppModule.hh"
#include "AbsParm/AbsParmIfdStrKey.hh"
#include "Framework/AbsParmGeneral.hh"
#include "Framework/AbsParmBool.hh"
#include "Framework/AbsParmDouble.hh"
#include "Framework/AbsParmString.hh"
#include "AbsParm/AbsParmVector.hh"
#include "BetaCoreTools/BtaBooster.hh"
#include "BetaCoreTools/BtaMcAssoc.hh"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
//
//		---------------------
// 		-- Class Interface --
//		---------------------
 
class CharmSLFilter : public  TagFilterModule{

//--------------------
// Instance Members --
//--------------------

public:

    // Constructors
    CharmSLFilter( const char* const theName, const char* const theDescription );

    // Destructor
    virtual ~CharmSLFilter( );

    // Operations

    virtual AppResult           beginJob( AbsEvent* anEvent );
    virtual AppResult           event   ( AbsEvent* anEvent );
    virtual AppResult           endJob  ( AbsEvent* anEvent );
    
protected:

  bool pass(AbsEvent* anEvent);   // actually checks if event passes

  // The following are sample parameters

  AbsParmIfdStrKey     _eventInfoList;
  AbsParmIfdStrKey     _chargedList;
  AbsParmIfdStrKey     _neutralList;
  AbsParmIfdStrKey     _electronList;

  AbsParmDouble _plepCut; 
  AbsParmDouble _MoppCut;
  AbsParmDouble _h2h0Cut;
  AbsParmDouble _MtagCut;

private:
  
  int _NEvents; // Events read 
  int _NPassed;
};

#endif
