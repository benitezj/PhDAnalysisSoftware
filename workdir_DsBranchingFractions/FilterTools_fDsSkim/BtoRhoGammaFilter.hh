//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: BtoRhoGammaFilter.hh,v 1.10 2006/11/28 21:42:54 desilva Exp $
//
// Description:
//	B -> rho gamma analysis filter
//
// Environment:
//
// Author List:
//
// Copyright Information:
//
//
//------------------------------------------------------------------------

#ifndef BTORHOGAMMAFILTER_HH
#define BTORHOGAMMAFILTER_HH

//----------------------
// Base Class Headers --
//---------------------
// #include "Framework/AppFilterModule.hh"
#include "TagModules/TagFilterModule.hh"

#include "AbsParm/AbsParmIfdStrKey.hh"
#include "AbsParm/AbsParmVector.hh"
#include "BbrGeom/BbrPointErr.hh"
#include "Beta/EventInfo.hh"
// #include "Framework/AbsParmBool.hh"
#include "Framework/AbsParmDouble.hh"
#include "Framework/AbsParmGeneral.hh"
#include "Framework/AbsParmString.hh"

// #include "CLHEP/Alist/AList.h"
// #include "CLHEP/Matrix/Vector.h"
// #include "CLHEP/Vector/LorentzVector.h"

#include "UsrData/UsrData.hh"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class BtoRhoGammaFilter : public TagFilterModule
{

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  BtoRhoGammaFilter( const char* const theName, 
		     const char* const theDescription );

  // Destructor
  virtual ~BtoRhoGammaFilter( );

  // Operations
  
  virtual AppResult beginJob( AbsEvent *anEvent );
  virtual AppResult event( AbsEvent *anEvent );
  virtual AppResult endJob( AbsEvent *anEvent );
    

private:

  AbsParmIfdStrKey _eventInfoList;
  AbsParmIfdStrKey _btaTrackList;
  AbsParmIfdStrKey _btaNeutralList;
  AbsParmVector<std::string> _bLists;
  AbsParmIfdStrKey _bToXGammaExclCandBlock;

protected:

  // private helper functions
  int fillUsrCandBlock( EventInfo *eventInfo,
			HepAList<BtaCandidate> *trackList, 
			HepAList<BtaCandidate> *neutralList,
			HepAList<BtaCandidate> *BList );

  // B candidate-associated user data variables
  // B0ToRho0GammaList:
  UsrCandBlock _BToXGammaExclCandBlock;
  UsrVariableFloat bMes;
  UsrVariableFloat bDeltaE;
  UsrVariableFloat gamBestPi0;		// Best pi0 mass formed with B-daughter
  UsrVariableFloat gamBestPi030;	//  photon.
  UsrVariableFloat gamBestPi050;
  UsrVariableFloat gamBestEta250;	// Best eta mass formed with B-daughter
  UsrVariableFloat gamBestEta280;	//  photon.
  UsrVariableFloat gamDistNe;		// Photon distance to nearest neutral
  UsrVariableFloat gamDistNe50;		//  bump.
  UsrVariableFloat gamDistCh;		// Photon distance to nearest charged
  UsrVariableFloat gamDistCh50;		//  bump.
  UsrVariableFloat roeThr;		// ROE thrust
  UsrVariableFloat roeThrGamTheta;	// ROE thrust angle wrt gamma dir
  UsrVariableFloat roeSph;		// ROE sphericity
  UsrVariableFloat roeThrL1;		// ROE Polynomial moments wrt ROE
  UsrVariableFloat roeThrL2;		//  thrust axis. 
  UsrVariableFloat roeThrL3;
  UsrVariableFloat roeGamL1;		// ROE Polynomial moments wrt photon
  UsrVariableFloat roeGamL2;		//  direction.
  UsrVariableFloat roeGamL3;
  UsrVariableFloat recoilR2prime;	// R2', R2 of photon recoil system.

  // event counters
  int _nReadEvents;
  int _nPassed;

  // Candidate counters
  std::vector<int> _nBCands;
};

#endif

