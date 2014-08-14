//--------------------------------------------------------------------------
//
// Description:
//
//	Class DsToPhiPiFilterModule - module for the filtering of Ds -> phi + pi mode
//
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Sergey Ganzhur                    Original author
//
// Copyright Information:
//	Copyright (C) 1999		 CEA - Centre d'Etude de Saclay
//
//------------------------------------------------------------------------

#ifndef DSTOPHIPIFILTERMODULE_HH
#define DSTOPHIPIFILTERMODULE_HH

//----------------------
// Base Class Headers --
//----------------------
#include "Framework/AppFilterModule.hh"
#include "Framework/AbsParmBool.hh"
#include "AbsParm/AbsParmIfdStrKey.hh"
#include "Framework/AbsParmGeneral.hh"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
class HepTupleManager;
class HepHistogram;
class HepTuple;

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class DsToPhiPiFilterModule : public AppFilterModule {

//--------------------
// Instance Members --
//--------------------

public:

// Constructors
    DsToPhiPiFilterModule( const char* const theName, const char* const theDescription );

// Destructor
    virtual ~DsToPhiPiFilterModule( );


// Operations

    virtual AppResult           beginJob( AbsEvent* anEvent );
    virtual AppResult           event( AbsEvent* anEvent );
    virtual AppResult           endJob  ( AbsEvent* anEvent );
    
protected:

    AbsParmIfdStrKey _eventInfoList;
    AbsParmIfdStrKey _btaCandList;


    AbsParmGeneral<double> _phiMassCut;         // phi mass cut |M_real - M_rec| < _phiMassCut
    AbsParmGeneral<double> _massCutLower;       // lower limit mass for K-K-pi mass
    AbsParmGeneral<double> _massCutUpper;       // upper limit mass for K-K-pi mass
    AbsParmGeneral<double> _pCut;               // Ds momentum in Ups(4S) rest frame  cut P* > _pCut


    AbsParmBool _checkPiTightKaon;              // do the checking pion id (must not be tight kaon)

// define "local" variables to store from event to event
// see the "begin" method for a discussion of these

    HepHistogram*    _mPhiHisto;
    HepHistogram*    _mPhiHisto2;
    HepHistogram*    _mDsHisto;

// event counter
    int _nReadEvents ; 
    int _nPassed;
 

};

#endif

