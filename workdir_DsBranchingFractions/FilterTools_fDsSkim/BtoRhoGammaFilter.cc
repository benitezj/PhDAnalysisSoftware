//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: BtoRhoGammaFilter.cc,v 1.12 2006/11/28 21:42:53 desilva Exp $
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
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "FilterTools/BtoRhoGammaFilter.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "AbsEnv/AbsEnv.hh"
#include "AbsEvent/getTmpAList.hh"
#include "Beta/BtaCandidate.hh"
#include "Beta/EventInfo.hh"
#include "BetaCoreTools/BtaBooster.hh"
#include "BetaCoreTools/BtaBVariables.hh"
#include "BetaCoreTools/BtaFWMoments.hh"
#include "BetaCoreTools/BtaLgndrMomFlow.hh"
#include "BetaCoreTools/BtaSphericity.hh"
#include "BetaCoreTools/BtaThrust.hh"
#include "BetaMicroAdapter/BtaCalQual.hh"
#include "CLHEP/Alist/AList.h"
#include "GenEnv/GenEnv.hh"
#include "ErrLogger/ErrLog.hh"
#include "PDT/Pdt.hh"
#include "TagData/TagTransient.hh"
#include "TagData/TagTransientBoolIter.hh"
#include "TagData/TagTransientIntIter.hh"
#include "TagData/TagTransientFloatIter.hh"
#include "UsrTools/UsrPhotonVars.hh"


//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------
BtoRhoGammaFilter::BtoRhoGammaFilter( const char* const theName, 
			const char* const theDescription )
  : TagFilterModule( theName, theDescription )
  , _eventInfoList( "eventInfoList", this, "Default" )
  , _btaTrackList( "TrackList", this, "ChargedTracks" )
  , _btaNeutralList( "NeutralList", this, "CalorNeutral" )
  , _bLists( "BLists", this )
  , _bToXGammaExclCandBlock( "UsrCandBlock", this, "BToXGammaExclCandBlock" )
  , bMes( "mES" )
  , bDeltaE( "deltaE" )
  , gamBestPi0( "gamBestPi0" )
  , gamBestPi030( "gamBestPi030" )
  , gamBestPi050( "gamBestPi050" )
  , gamBestEta250( "gamBestEta250" )
  , gamBestEta280( "gamBestEta280" )
  , gamDistNe( "gamDistNe" )
  , gamDistNe50( "gamDistNe50" )
  , gamDistCh( "gamDistCh" )
  , gamDistCh50( "gamDistCh50" )
  , roeThr( "roeThr" )
  , roeThrGamTheta( "roeThrGamTheta" )
  , roeSph( "roeSph" )
  , roeThrL1( "roeThrL1" )
  , roeThrL2( "roeThrL2" )
  , roeThrL3( "roeThrL3" )
  , roeGamL1( "roeGamL1" )
  , roeGamL2( "roeGamL2" )
  , roeGamL3( "roeGamL3" )
  , recoilR2prime( "recoilR2prime" )
{
  commands()->append( &_eventInfoList );
  commands()->append( &_btaTrackList );
  commands()->append( &_btaNeutralList );
  commands()->append( &_bLists );
  commands()->append( &_bToXGammaExclCandBlock );

  // Add data members to the _BToXGammaExclCandBlock
  _BToXGammaExclCandBlock.addVariable( bMes );
  _BToXGammaExclCandBlock.addVariable( bDeltaE );
  _BToXGammaExclCandBlock.addVariable( gamBestPi0 );
  _BToXGammaExclCandBlock.addVariable( gamBestPi030 );
  _BToXGammaExclCandBlock.addVariable( gamBestPi050 );
  _BToXGammaExclCandBlock.addVariable( gamBestEta250 );
  _BToXGammaExclCandBlock.addVariable( gamBestEta280 );
  _BToXGammaExclCandBlock.addVariable( gamDistNe );
  _BToXGammaExclCandBlock.addVariable( gamDistNe50 );
  _BToXGammaExclCandBlock.addVariable( gamDistCh );
  _BToXGammaExclCandBlock.addVariable( gamDistCh50 );
  _BToXGammaExclCandBlock.addVariable( roeThr );
  _BToXGammaExclCandBlock.addVariable( roeThrGamTheta );
  _BToXGammaExclCandBlock.addVariable( roeSph );
  _BToXGammaExclCandBlock.addVariable( roeThrL1 );
  _BToXGammaExclCandBlock.addVariable( roeThrL2 );
  _BToXGammaExclCandBlock.addVariable( roeThrL3 );
  _BToXGammaExclCandBlock.addVariable( roeGamL1 );
  _BToXGammaExclCandBlock.addVariable( roeGamL2 );
  _BToXGammaExclCandBlock.addVariable( roeGamL3 );
  _BToXGammaExclCandBlock.addVariable( recoilR2prime );
}
		

//--------------
// Destructor --
//--------------
BtoRhoGammaFilter::~BtoRhoGammaFilter()
{
}

//-------------
// Methods   --
//-------------

AppResult BtoRhoGammaFilter::beginJob( AbsEvent*  anEvent )
{
  ErrMsg(trace) << name() << ": begin job" << endmsg;

  // initialize event counters
  _nReadEvents = 0;
  _nPassed = 0;

  // Set up the counters for each list
  const std::vector<std::string> &bLists  = _bLists.value();
  std::vector<std::string>::const_iterator bListsIter;
  for( bListsIter = bLists.begin(); bListsIter != bLists.end(); bListsIter++ )
  {
    _nBCands.push_back( 0 );
  }

  return AppResult::OK;
}


AppResult BtoRhoGammaFilter::event( AbsEvent *anEvent )
{
  _nReadEvents++;

  UsrIfd<UsrCandBlock>::put( anEvent, &_BToXGammaExclCandBlock, _bToXGammaExclCandBlock.value() );

  bool passed = false;

  int nCandsThisEv = 0;
  int nUsrThisEv = 0;

  // Grab the lists from the event.
  HepAList<BtaCandidate> *TrackList;
  HepAList<BtaCandidate> *NeutralList;

  HepAList<EventInfo> *infoList = NULL;
  getTmpAList( anEvent, infoList, _eventInfoList.value() );
  EventInfo *eventInfo = infoList->first();

  getTmpAList( anEvent, TrackList, _btaTrackList.value() );
  getTmpAList( anEvent, NeutralList, _btaNeutralList.value() );

  // Get the list of reconstructed B candidates
  const std::vector<std::string> &bLists  = _bLists.value();
  std::vector<std::string>::const_iterator bListsIter;

  int i;
  for( bListsIter = bLists.begin(), i = 0; bListsIter != bLists.end(); bListsIter++, i++ )
  {
    HepAList<BtaCandidate> *BtoXGammaList = 0;
    IfdStrKey theStrKey(*bListsIter);
    getTmpAList( anEvent, BtoXGammaList, theStrKey);

    int nUsrData = fillUsrCandBlock( eventInfo, TrackList, NeutralList,
							BtoXGammaList );
   
    if( BtoXGammaList->length() ) 
      _nBCands[i]++;

    ErrMsg(debugging) << name() << ": List " << bListsIter->c_str()
			<< ": Cand : " << BtoXGammaList->length()
			<< ": Usr : " << nUsrData << endmsg;

    nCandsThisEv += BtoXGammaList->length();
    nUsrThisEv += nUsrData;
  }

  if( nCandsThisEv )
    passed = true;
 
  if( passed )
  {
    _nPassed++;
    ErrMsg(debugging) << name() << ": event # " << _nReadEvents 
			<< " passed with " << nCandsThisEv << " B cand with "
			<< nUsrThisEv << " UsrCand entries." << endmsg;
  }
  else
  {
    ErrMsg(debugging) << name() << ": event # " << _nReadEvents 
			<< " rejected." << endmsg;
  }

  setPassed( passed );

  return AppResult::OK;
}

AppResult BtoRhoGammaFilter::endJob( AbsEvent* anEvent )
{

  ErrMsg(debugging) << name()  << " reporting candidate counts:" << endmsg;
  const std::vector<std::string> &bLists  = _bLists.value();
  std::vector<std::string>::const_iterator bListsIter;

  int i;
  for( bListsIter = bLists.begin(), i = 0; bListsIter != bLists.end(); bListsIter++, i++ )
  {
    ErrMsg(debugging) << "\t" << bListsIter->c_str() << " : " 
    			<< _nBCands[i] << " Ev with cand." <<   endmsg;
  }

  ErrMsg(trace) << name()  << " end job: " << _nPassed << " out of " 
		  << _nReadEvents << " passed" << endmsg;
  
  return AppResult::OK;
}


// Puts the candidate-associated user data in the UsrCandBlock
int BtoRhoGammaFilter::fillUsrCandBlock( EventInfo *eventInfo, 
	HepAList<BtaCandidate> *trackList, HepAList<BtaCandidate> *neutralList,
	HepAList<BtaCandidate> *BList )
{
  int nUsrCands = 0;

  // Will need a booster
  HepLorentzVector initialP4 = eventInfo->cmFrame();
  BtaBooster cmBooster( initialP4 );

  // And the primary vertex
  BbrPointErr primaryVtx;
  const BtaAbsVertex *theVtx = eventInfo->primaryVtx();
  if( theVtx )
    primaryVtx = theVtx->point();
  else
    primaryVtx = eventInfo->beamSpot();


  // Loop over B candidates in list.
  BtaCandidate *BCand;
  HepAListIterator<BtaCandidate> BIter( *BList );
  while( (BCand = BIter()) )
  {
    // If necessary, calculate m_ES and delta_E and store them in the Usr block
    BtaBVariables theBVars(BCand->p4WCov());

    bMes = theBVars.m_ES();
    bDeltaE = theBVars.deltaE();

    // Find the primary B-daughter photon
    BtaCandidate *bDaug, *bGamma = 0;
    HepAListIterator<BtaCandidate> bDaugIter = BCand->daughterIterator();
    while( (bDaug = bDaugIter()) && !bGamma )
    {
      if( bDaug->pdtEntry()->pdgId() == PdtPdg::gamma )
        bGamma = bDaug;
    }

    // Boost the photon to the CM frame:
    BtaCandidate cmGamma = cmBooster.boostTo( *bGamma );

    // Add the photon variables to the Usr block.
    UsrPhotonVars photonVars( bGamma, neutralList, trackList );
    photonVars.setPrimaryVtx( primaryVtx );
    gamBestPi0 = photonVars.m_pi0();
    gamBestPi030 = photonVars.m_pi030();
    gamBestPi050 = photonVars.m_pi050();
    gamBestEta250 = photonVars.m_eta250();
    gamBestEta280 = photonVars.m_eta280();
    gamDistNe = photonVars.distNeutral();
    gamDistNe50 = photonVars.distNeutral50();
    gamDistCh = photonVars.distCharged();
    gamDistCh50 = photonVars.distCharged50();


    // Make the ROE lists
    HepAList<BtaCandidate> roeList;
    HepAList<BtaCandidate> allSansGamma;	// Photon recoil system

    HepAList<BtaCandidate> allCandList(*trackList);
    allCandList.append(*neutralList);

    BtaCandidate *btaTmp;
    HepAListIterator<BtaCandidate> allIter(allCandList);
    while( (btaTmp = allIter()) )
    {
      if( !BCand->overlaps( *btaTmp ) )
	roeList.append( *btaTmp );

      if( !bGamma->overlaps( *btaTmp ) )
	allSansGamma.append( *btaTmp );
    }


    // Calculate the ROE values to be stored.

    // Calculate R2', begin with a booster to the photon recoil frame
    BtaCandidate recoilFrame( eventInfo->cmFrame() - bGamma->p4(), 0 );
    BtaBooster recoilBooster( &recoilFrame );

    BtaFWMoments recoilFWAll;		// FW moment calculator class
    recoilFWAll.boostAndCompute( allSansGamma, &recoilBooster );

    // The Usr variable for R2'
    recoilR2prime = recoilFWAll.R(2);

    //  thrust and sphericity of ROE
    BtaThrust thrROE( roeList, *eventInfo, BtaThrust::BTAllParticles,
						 false, BtaThrust::BTFitter ); 

    BtaSphericity sphROE( roeList, *eventInfo, false );

    Hep3Vector thrAxis   = thrROE.thrust_axis();

    // The Usr variables for ROE thrust & sphericity:
    roeThr = thrROE.thrust(); 
    roeSph = sphROE.sph();
    roeThrGamTheta = cmGamma.p3().angle(thrAxis);

    // 18 energy cones.
    // Leave commented out unless reinstated in the analysis.
    // BtaCones theCones( &roeList, gamCand, eventInfo, 18 );

    // Longitudinal Legendre moments.
    BtaLgndrMomFlow lgndrCalculator( 4 );

    // moments relative to lab frame thrust axis:
    lgndrCalculator.boostAndCompute( roeList, thrAxis, &cmBooster );
    const HepVector &thrMoments = lgndrCalculator.LgndrMomentsP1();
    roeThrL1 = thrMoments[1] / thrMoments[0];
    roeThrL2 = thrMoments[2] / thrMoments[0];
    roeThrL3 = thrMoments[3] / thrMoments[0];

    // moments relative to cm frame photon
    lgndrCalculator.boostAndCompute( roeList, cmGamma.p3(), &cmBooster );
    const HepVector &gamMoments = lgndrCalculator.LgndrMomentsP1();
    roeGamL1 = gamMoments[1] / gamMoments[0];
    roeGamL2 = gamMoments[2] / gamMoments[0];
    roeGamL3 = gamMoments[3] / gamMoments[0];


    // Add the ROE info to the Usr block.  Need to add some error checking.
    bool usrWrote = _BToXGammaExclCandBlock.put( *BCand, bMes );
    usrWrote = usrWrote && _BToXGammaExclCandBlock.put( *BCand, bDeltaE );
    usrWrote = usrWrote && _BToXGammaExclCandBlock.put( *BCand, gamBestPi0 );
    usrWrote = usrWrote && _BToXGammaExclCandBlock.put( *BCand, gamBestPi030 );
    usrWrote = usrWrote && _BToXGammaExclCandBlock.put( *BCand, gamBestPi050 );
    usrWrote = usrWrote && _BToXGammaExclCandBlock.put( *BCand, gamBestEta250 );
    usrWrote = usrWrote && _BToXGammaExclCandBlock.put( *BCand, gamBestEta280 );
    usrWrote = usrWrote && _BToXGammaExclCandBlock.put( *BCand, gamDistNe );
    usrWrote = usrWrote && _BToXGammaExclCandBlock.put( *BCand, gamDistNe50 );
    usrWrote = usrWrote && _BToXGammaExclCandBlock.put( *BCand, gamDistCh );
    usrWrote = usrWrote && _BToXGammaExclCandBlock.put( *BCand, gamDistCh50 );
    usrWrote = usrWrote && _BToXGammaExclCandBlock.put( *BCand, roeThr );
    usrWrote = usrWrote && _BToXGammaExclCandBlock.put( *BCand, roeThrGamTheta );
    usrWrote = usrWrote && _BToXGammaExclCandBlock.put( *BCand, roeSph );
    usrWrote = usrWrote && _BToXGammaExclCandBlock.put( *BCand, roeThrL1 );
    usrWrote = usrWrote && _BToXGammaExclCandBlock.put( *BCand, roeThrL2 );
    usrWrote = usrWrote && _BToXGammaExclCandBlock.put( *BCand, roeThrL3 );
    usrWrote = usrWrote && _BToXGammaExclCandBlock.put( *BCand, roeGamL1 );
    usrWrote = usrWrote && _BToXGammaExclCandBlock.put( *BCand, roeGamL2 );
    usrWrote = usrWrote && _BToXGammaExclCandBlock.put( *BCand, roeGamL3 );
    usrWrote = usrWrote && _BToXGammaExclCandBlock.put( *BCand, recoilR2prime );

    if( usrWrote )
      nUsrCands++;
  }

  return nUsrCands;
}

