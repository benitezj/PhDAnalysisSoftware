//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: BtuMyAnalysis.hh,v 1.3 2006/10/06 16:55:25 desilva Exp $
//
// Description:
//	Class MyAnalysis - the barest outline of a Beta
//      Analysis, suitable for simple filling-in
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Bob Jacobsen                    Original author
//
// Copyright Information:
//	Copyright (C) 1997		Lawrence Berkeley Laboratory
//
//------------------------------------------------------------------------
#ifndef MYANALYSIS_HH
#define MYANALYSIS_HH

//----------------------
// Base Class Headers --
//----------------------
#include "Framework/AppModule.hh"

//------------------------------------
// Collaborating Class Header files --
//------------------------------------
#include "AbsParm/AbsParmIfdStrKey.hh"
#include "Beta/BtaCandidate.hh"
#include "CLHEP/Alist/AList.h"
#include "ErrLogger/ErrLog.hh"
#include "ProxyDict/IfdKey.hh"

#include "UsrData/UsrData.hh"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class BtuMyAnalysis : public AppModule {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  BtuMyAnalysis( const char* const theName, 
		 const char* const theDescription );
  
  // Destructor
  virtual ~BtuMyAnalysis( );
  
  // Operations
  
  virtual AppResult           beginJob( AbsEvent* anEvent );
  virtual AppResult           event   ( AbsEvent* anEvent );
  virtual AppResult           endJob  ( AbsEvent* anEvent );
  
  
protected:
  
  // The following are sample parameters
  AbsParmIfdStrKey _eventInfoList;
  AbsParmIfdStrKey _inputList;
  AbsParmIfdStrKey _outputList;
  AbsParmIfdStrKey _evtShapeTracks, _evtShapeNeutrals;

private:

  HepAList<BtaCandidate>* locateList(AbsEvent* anEvent, const std::string& s);

  UsrCandBlock  _myCandBlock;
  UsrCandBlock  _myD0CandBlock;
  UsrEventBlock _myEventBlock;

  UsrVariable<float> _cosT, _newR2;
  UsrVariable<float> _usrD0mass;

  int _nevt;
};

#endif
