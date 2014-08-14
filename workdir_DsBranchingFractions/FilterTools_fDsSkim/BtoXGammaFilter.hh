//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: BtoXGammaFilter.hh,v 1.8 2006/10/18 23:04:49 bartoldu Exp $
//
// Description:
//      Skim for the fully inclusive b -> s gamma analysis.
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Rainer Bartoldus
//      Philip Bechtle
//
// Copyright Information:
//      Copyright (C) 2006  Stanford Linear Accelerator Center
//
//--------------------------------------------------------------------------
#ifndef BTOXGAMMAFILTER_HH
#define BTOXGAMMAFILTER_HH

//---------------
// C++ Headers --
//---------------

//----------------------
// Base Class Headers --
//----------------------
#include "TagModules/TagFilterModule.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "Framework/AbsParmDouble.hh"
#include "Framework/AbsParmGeneral.hh"
#include "AbsParm/AbsParmIfdStrKey.hh"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

//              ---------------------
//              -- Class Interface --
//              ---------------------

class BtoXGammaFilter : public TagFilterModule
{
public:

  // Constructors
  BtoXGammaFilter( const char* const theName, 
              const char* const theDescription );

  // Destructor
  virtual ~BtoXGammaFilter();

  // Methods
  virtual AppModule* clone( const char *cloneName );

  virtual AppResult beginJob( AbsEvent* anEvent );
  virtual AppResult    event( AbsEvent* anEvent );
  virtual AppResult   endJob( AbsEvent* anEvent );

private:

  // Commands list
  AbsParmIfdStrKey    _eventInfoList;
  AbsParmIfdStrKey    _btaPhotonList;       
  AbsParmIfdStrKey    _btaTrackList;        

  AbsParmGeneral<int> _nTrackMinCut;          
  AbsParmDouble       _heGammaELow;         
  AbsParmDouble       _heGammaEHigh;        
  AbsParmDouble       _cosThetaGammaLabLow; 
  AbsParmDouble       _cosThetaGammaLabHigh;
  AbsParmDouble       _r2StarMaxCut;        
  AbsParmDouble       _pTrackMaxCut;        

  // event counters
  unsigned _nEvents;
  unsigned _nPassed;
};

#endif 
