//--------------------------------------------------------------------------
// File and Version Information:
//      TrkEffTau31Skim.hh
//
// Description:
//
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//     Ian Nugent
//
// Copyright Information:
//      Copyright (C) 2005                          University of Victoria
//
//------------------------------------------------------------------------


#ifndef MYADDLOWMULTIPLICITYSKIM_HH
#define MYADDLOWMULTIPLICITYSKIM_HH 1

//----------------------
// Base Class Headers --
//----------------------
//#include "Framework/AppFilterModule.hh"

#include "TagModules/TagFilterModule.hh"
//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

#include "AbsParm/AbsParmIfdStrKey.hh"
#include "Framework/AbsParmGeneral.hh"
#include "BetaCoreTools/BtaBooster.hh"
#include "BetaCoreTools/BtaThrust.hh"
	
//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class TrkEffTau31Skim : public TagFilterModule {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
    TrkEffTau31Skim( const char* const N, const char* const D );
  

  // Destructor
  virtual ~TrkEffTau31Skim( );
  
  // Operations
  virtual AppResult           beginJob( AbsEvent* anEvent );
  virtual AppResult           event   ( AbsEvent* anEvent );
  virtual AppResult           endJob  ( AbsEvent* anEvent );


protected:

  bool pass(AbsEvent* anEvent);   // actually checks if event passes

  // The following are sample parameters
  AbsParmIfdStrKey _eventInfoList;
  AbsParmIfdStrKey _CTList;
  AbsParmIfdStrKey _GTVLList;
  AbsParmIfdStrKey _GTLList;
  AbsParmIfdStrKey _pidTrkList;


private:
  int _nTrials; // keep track of statistics
  int _cutNChg;
  int _cutNCalNeut;
  int _cutNNeutHad;
  int _numberEvt;
  int _numRejectedEvt;
  int _numPassedEvt;
  int _numPassedChgEvt;
  int _numPassedCalNeutEvt;
  int _numPassedNeutHadEvt;

  //  BtaThrust::BetaFitter _fitter;

  bool Topology_Cut(AbsEvent* anEvent,EventInfo* eventInfo,
		    HepAList<BtaCandidate>* CandList);
  bool Charge_Cut(AbsEvent* anEvent,
		  HepAList<BtaCandidate>* CandList);

};

#endif
