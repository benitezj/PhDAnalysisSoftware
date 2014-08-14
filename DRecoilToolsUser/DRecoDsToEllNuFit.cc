#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "DRecoDsToEllNuFit.hh"

//-------------
// C Headers --
//-------------
#include <assert.h>

//---------------
// C++ Headers --
//---------------
#include <iostream>
#include <math.h>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "AbsEvent/AbsEvent.hh"
#include "Beta/EventInfo.hh"
#include "Beta/BtaCandidate.hh"
#include "BetaCoreTools/BtaOpMakeTree.hh"
//#include "CharmTagTools/ChmTagToolsSequence.hh"
//#include "CharmTagTools/ChmSlowPionSelector.hh"
#include "CLHEP/Alist/AList.h"
#include "CLHEP/Alist/AIterator.h"
#include "ErrLogger/ErrLog.hh"
#include "PDT/Pdt.hh"
#include "ProxyDict/IfdStrKey.hh"
#include "ProxyDict/IfdKey.hh"
#include "ProxyDict/Ifd.hh"
#include "ProxyDict/IfdHepAListProxy.hh"

#include "TrkBase/TrkAbsFit.hh"
#include "VtxBase/BtaAbsVertexer.hh"
#include "VtxFitter/VtxFitterOper.hh"
#include "VtxBase/VtxAbsAlgorithm.hh"
#include "Beta/BtaAbsVertex.hh"
#include "ProbTools/probab.hh"
#include "BetaCoreTools/BtaBooster.hh"

#include "BetaMicroAdapter/BtaMicroAdapter.hh"
#include "BetaMicroAdapter/BtaCalQual.hh"
#include "BetaMicroAdapter/BtaPidQual.hh"
#include "BetaMicroAdapter/BtaTrkQual.hh"
#include "BetaMicroAdapter/BtaPidInfo.hh"
#include "BetaMicroAdapter/BtaIfrQual.hh"

#include "TrkBase/TrkRecoTrk.hh"

#include "BetaCoreTools/BtaMcAssoc.hh"
#include "BtaTupleMaker/BtuUtil.hh"

#include "BetaCoreTools/BtaPrintTree.hh"

#include "BbrGeom/BbrVectorErr.hh"

#include "BetaCoreTools/BtaThrustFitter.hh"
#include "BetaCoreTools/BtaFoxWolfMom.hh"

using std::cout ;
using std::endl ;
using std::ostream ;

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//----------------
// Constructors --
//----------------
DRecoDsToEllNuFit::DRecoDsToEllNuFit( const char* const theName,const char* const theDescription )
  :   TagFilterModule (  theName, theDescription                           )
  ,        FitProbCut ( "FitProbCut"         , this , 0.                   )
  ,          muonList ( "muonList"           , this , "muNNTight"          )
  ,      electronList ( "electronList"       , this , "eKMVeryLoose"       )
  ,         trackList ( "trackList"          , this , "ChargedTracks"      )
  ,       chargedList ( "chargedList"        , this , "ChargedTracks"      )
  ,       neutralList ( "neutralList"        , this , "CalorNeutral"       )
  ,   ExGamEnergyCutHi( "ExGamEnergyCutHi"   , this , 0.1                  )
  ,          _scalers ( "DRecoDsToEllNuFit"                                )
  , DRecoUsrBlockName ( "DRecoUsrBlockEllNu" , this , "DRecoUsrBlockEllNu" )
  
  ,    TagUncMass ( "TagUncMass"       , 0.  )
  ,   DsStUncMass ( "DsStUncMass"      , 0.  )
  ,     DsUncMass ( "DsUncMass"        , 0.  )
  ,        DsMass ( "DsMass"           , 0.  )
  
  ,        qualMu ( "qualMu"           , 0   )
  ,         qualE ( "qualE"            , 0   )
  
  ,    MyGamMCIdx ( "MyGamMCIdx"       , -1  )
  ,    MyTagMCIdx ( "MyTagMCIdx"       , -1  )
  ,      MyKMCIdx ( "MyKMCIdx"         , -1  )
  ,     MyMuMCIdx ( "MyMuMCIdx"        , -1  )
  ,      MyEMCIdx ( "MyEMCIdx"         , -1  )
  
  ,  mm2corrected ( "mm2corrected"     , 0.  )
  ,       Deltap3 ( "Deltap3"          , 0.  )
  ,      DsToMuNu ( "DsToMuNu"         , 0   )
  ,       DsToENu ( "DsToENu"          , 0   )
  
  ,      ProbChi2 ( "DsToEllNuProbChi2", 0   )
  ,          Chi2 ( "DsToEllNuChi2"    , 0   )
  ,          ndof ( "DsToEllNundof"    , 0   )
  
  ,     NuMassRaw ( "NuMassRaw"        , 0   )
  ,    SignalMode ( "SignalMode"       , 0   )
  
  ,       NupxRaw ( "NupxRaw"          , 0   )
  ,       NupyRaw ( "NupyRaw"          , 0   )
  ,       NupzRaw ( "NupzRaw"          , 0   )
  ,   NuenergyRaw ( "NuenergyRaw"      , 0   )
  
  ,       MupxRaw ( "MupxRaw"          , 0   )
  ,       MupyRaw ( "MupyRaw"          , 0   )
  ,       MupzRaw ( "MupzRaw"          , 0   )
  ,   MuenergyRaw ( "MuenergyRaw"      , 0   )
  
  ,      NupxCorr ( "NupxCorr"         , 0   )
  ,      NupyCorr ( "NupyCorr"         , 0   )
  ,      NupzCorr ( "NupzCorr"         , 0   )
  ,  NuenergyCorr ( "NuenergyCorr"     , 0   )
  
  ,       ThrustX ( "ThrustX"          , -99 )
  ,       ThrustY ( "ThrustY"          , -99 )
  ,       ThrustZ ( "ThrustZ"          , -99 )
  ,     ThrustMag ( "ThrustMag"        , -99 )
  
  ,            R0 ( "R0"               , -99 )
  ,            R1 ( "R1"               , -99 )
  ,            R2 ( "R2"               , -99 )
  ,            R3 ( "R3"               , -99 )
  ,            R4 ( "R4"               , -99 )
  
  , EExtraNeutral ( "EExtraNeutral"    , -99 )
  , NExtraNeutral ( "NExtraNeutral"    , -99 )
  
  ,    PMissCosTh ( "PMissCosTh"       , -99 )
  ,      PMissPhi ( "PMissPhi"         , -99 )
  ,         PMiss ( "PMiss"            , -99 )
  ,         EMiss ( "EMiss"            , -99 )
  
{
  commands()->append( &FitProbCut       ) ;
  commands()->append( &muonList         ) ; 
  commands()->append( &electronList     ) ;
  commands()->append( &trackList        ) ;
  commands()->append( &chargedList      ) ;
  commands()->append( &neutralList      ) ;
  commands()->append( &ExGamEnergyCutHi ) ;

  DRecoUsrBlock.addVariable( TagUncMass    ) ;
  DRecoUsrBlock.addVariable( DsStUncMass   ) ;
  DRecoUsrBlock.addVariable( DsUncMass     ) ;
  DRecoUsrBlock.addVariable( DsMass        ) ;
					   
  DRecoUsrBlock.addVariable( qualE         ) ;
  DRecoUsrBlock.addVariable( qualMu        ) ;
					   
  DRecoUsrBlock.addVariable( MyGamMCIdx    ) ;
  DRecoUsrBlock.addVariable( MyTagMCIdx    ) ;
  DRecoUsrBlock.addVariable( MyKMCIdx      ) ;
  DRecoUsrBlock.addVariable( MyMuMCIdx     ) ;
  DRecoUsrBlock.addVariable( MyEMCIdx      ) ;
					   
  DRecoUsrBlock.addVariable( mm2corrected  ) ;
  DRecoUsrBlock.addVariable( Deltap3       ) ;
  DRecoUsrBlock.addVariable( DsToMuNu      ) ;
  DRecoUsrBlock.addVariable( DsToENu       ) ;
					   
  DRecoUsrBlock.addVariable( ProbChi2      ) ;
  DRecoUsrBlock.addVariable( Chi2          ) ;
  DRecoUsrBlock.addVariable( ndof          ) ;
					   
  DRecoUsrBlock.addVariable( NuMassRaw     ) ;
  DRecoUsrBlock.addVariable( SignalMode    ) ;
					   
  DRecoUsrBlock.addVariable( NupxRaw       ) ;
  DRecoUsrBlock.addVariable( NupyRaw       ) ;
  DRecoUsrBlock.addVariable( NupzRaw       ) ;
  DRecoUsrBlock.addVariable( NuenergyRaw   ) ;
					  
  DRecoUsrBlock.addVariable( MupxRaw       ) ;
  DRecoUsrBlock.addVariable( MupyRaw       ) ;
  DRecoUsrBlock.addVariable( MupzRaw       ) ;
  DRecoUsrBlock.addVariable( MuenergyRaw   ) ;
					  
  DRecoUsrBlock.addVariable( NupxCorr      ) ;
  DRecoUsrBlock.addVariable( NupyCorr      ) ;
  DRecoUsrBlock.addVariable( NupzCorr      ) ;
  DRecoUsrBlock.addVariable( NuenergyCorr  ) ;
					  
  DRecoUsrBlock.addVariable( ThrustX       ) ;
  DRecoUsrBlock.addVariable( ThrustY       ) ;
  DRecoUsrBlock.addVariable( ThrustZ       ) ;
  DRecoUsrBlock.addVariable( ThrustMag     ) ;
					  
  DRecoUsrBlock.addVariable( R0            ) ;
  DRecoUsrBlock.addVariable( R1            ) ;
  DRecoUsrBlock.addVariable( R2            ) ;
  DRecoUsrBlock.addVariable( R3            ) ;
  DRecoUsrBlock.addVariable( R4            ) ;
					   
  DRecoUsrBlock.addVariable( EExtraNeutral ) ;
  DRecoUsrBlock.addVariable( NExtraNeutral ) ;
					   
  DRecoUsrBlock.addVariable( PMissCosTh    ) ;
  DRecoUsrBlock.addVariable( PMissPhi      ) ;
  DRecoUsrBlock.addVariable( PMiss         ) ;
  DRecoUsrBlock.addVariable( EMiss         ) ;
}

//--------------
// Destructor --
//--------------
DRecoDsToEllNuFit::~DRecoDsToEllNuFit()
{}

//--------------
// Operations --
//--------------
AppResult DRecoDsToEllNuFit::beginJob( AbsEvent* anEvent )
{ 
  return AppResult::OK;
}

AppResult DRecoDsToEllNuFit::event( AbsEvent* anEvent )
{
  //cout << "Testing 0" << endl ;
  _scalers.sum("#  events ") ;
  
  // Create new UsrBlock
  UsrIfd<UsrCandBlock>::put(anEvent,&DRecoUsrBlock,DRecoUsrBlockName.value()) ;
  
  // This block comes from previous module
  UsrCandBlock* DRecoUsrBlockIn = Ifd<UsrCandBlock>::get(anEvent,"DRecoUsrBlock") ;
  
  // Create evt list and add to event
  HepAList<BtaCandidate>* output_list = new HepAList<BtaCandidate> ;
  if(output_list==0) ErrMsg(fatal) << "Failed to create an output_list list" << endmsg ;
  
  // Create proxy to dump Ups_out list
  IfdHepAListProxy<BtaCandidate>* proxy = new IfdHepAListProxy<BtaCandidate>(output_list) ;
  if(proxy == 0) ErrMsg(fatal) << "Failed to create output_list list proxy" << endmsg ;
  if(!Ifd<HepAList<BtaCandidate> >::put(anEvent, proxy,"DsToEllNuCandidateList"))
    ErrMsg(fatal) << "failed to add DsToEllNuCandidateList list to event using proxy" << endmsg ; 
  
  // Retreive the event candidates
  HepAList< BtaCandidate >* DRecoEvtCandidateList = Ifd<HepAList<BtaCandidate > >::get(anEvent,"DRecoCandidateList") ;
  if(DRecoEvtCandidateList==0)  ErrMsg(fatal) <<"DRecoCandidateList not found" << endmsg ;
  
  // Pointer to the event summary info object
  HepAList< EventInfo >* eventInfoListPtr =  Ifd<HepAList< EventInfo > >::get(anEvent,"Default") ;
  if(eventInfoListPtr==0) ErrMsg(fatal) << "Could not locate event info list in the event. This is likely to be a configuration error." << endmsg ;
  EventInfo* eventInfo = eventInfoListPtr->first() ;
  if(eventInfo==0) ErrMsg(fatal) << "Nothing in eventInfoList...aborting" << endmsg ;
  
  // The beam spot below has been smeared in MC and fixed error=10mu data/MC
  const BbrPointErr beamSpot = eventInfoListPtr->first()->beamSpot() ;
  
  // Get the MC Truth info
  BtaMcAssoc* _truthMap = Ifd< BtaMcAssoc >::get( anEvent,"GHit") ;
  HepAList<BtaCandidate>* _mcList(0) ;
  if(_truthMap) _mcList = BtuUtil::rearrangeMcList( Ifd< HepAList<BtaCandidate> >::get(anEvent,"MCTruth" )) ;    
  
  // Find the Ds and get its ell for later matching  
  BtaCandidate* GenMu = NULL ;
  DsToMuNu = 0 ;
  if(_truthMap && _mcList)
  { // Truthmatch if this is MC
    HepAListIterator<BtaCandidate> mc_iterator(*_mcList) ;
    while(BtaCandidate* candidate = mc_iterator())
    {
      // Look for Ds
      BtaCandidate* GenMuTemp = NULL ;
      if(abs(candidate->pdtEntry()->lundId())==abs(Pdt::lookup( PdtPdg::D_s_plus)->lundId()))
      {
	GenMu = NULL ;
	// Must decay to mu nu
	int number_of_Ds_daughters = 0 ;
	HepAListIterator<BtaCandidate> Ds_daughter_iterator = candidate->daughterIterator() ;
	while(BtaCandidate* Ds_daughter = Ds_daughter_iterator())
	{
	  number_of_Ds_daughters++ ;
	  if(abs(Ds_daughter->pdtEntry()->lundId())==abs(Pdt::lookup( PdtPdg::mu_plus)->lundId()))
	  {
	    GenMuTemp = Ds_daughter ;
	    DsToMuNu = 1 ;
	  }
	}
	if(number_of_Ds_daughters==2) GenMu = GenMuTemp ;
      }
    }
    mc_iterator.rewind() ;

    // Print truth information for entire event
    if(false)
    {
       while(BtaCandidate* candidate = mc_iterator())
       {
	 cout << (candidate->uid()-10000) << "=" << candidate->pdtEntry()->lundId() <<endl ;
	 HepAListIterator<BtaCandidate> daughter_iterator = candidate->daughterIterator() ;
	 while(BtaCandidate* daughter = daughter_iterator())
	 {
	   cout << "  daughter: uid=" << (daughter->uid()-10000) << " lund=" << daughter->pdtEntry()->lundId() << endl ;
	 }
       }
    }
  }
  
  BtaCandidate* GenE = NULL ;
  DsToENu = 0 ;
  if(_truthMap && _mcList)
  { // Truthmatch if this is MC
    HepAListIterator<BtaCandidate> mc_iterator(*_mcList) ;
    while(BtaCandidate* candidate = mc_iterator())
    {
      BtaCandidate* GenETemp = NULL ;
      // Look for a Ds
      if(abs(candidate->pdtEntry()->lundId())==abs(Pdt::lookup( PdtPdg::D_s_plus)->lundId()))
      {
	GenE = NULL ;
	// Must decay to e nu
	int number_of_Ds_daughters = 0 ;
	HepAListIterator<BtaCandidate> Ds_daughter_iterator = candidate->daughterIterator() ;
	while(BtaCandidate* Ds_daughter = Ds_daughter_iterator())
	{
	  number_of_Ds_daughters++ ;
	  if(abs(Ds_daughter->pdtEntry()->lundId())==abs(Pdt::lookup( PdtPdg::e_plus)->lundId()))
	  {
	    GenETemp = Ds_daughter ;
	    DsToENu = 1 ;
	  }
	}
	if(number_of_Ds_daughters==2) GenE = GenETemp ;
      }
    }
  }
  
 // Get signal lists
  const std::string&     muonListKey = muonList.valueString() ;
  const std::string& electronListKey = electronList.valueString() ;
  const std::string&    trackListKey = trackList.valueString() ;
  HepAList<BtaCandidate>*     muon_list = Ifd<HepAList<BtaCandidate> >::get(anEvent, muonListKey.c_str()) ;
  HepAList<BtaCandidate>* electron_list = Ifd<HepAList<BtaCandidate> >::get(anEvent, electronListKey.c_str()) ;
  HepAList<BtaCandidate>*    track_list = Ifd<HepAList<BtaCandidate> >::get(anEvent, trackListKey.c_str()) ;
  if(muon_list==0)     ErrMsg(fatal) << "Could not locate the list " << muonList.valueString()     << ".  This is likely a configuration error." << endmsg ;
  if(electron_list==0) ErrMsg(fatal) << "Could not locate the list " << electronList.valueString() << ".  This is likely a configuration error." << endmsg ;
  if(track_list==0)    ErrMsg(fatal) << "Could not locate the list " << trackList.valueString()    << ".  This is likely a configuration error." << endmsg ;
  HepAListIterator<BtaCandidate>     muon_iterator(*muon_list) ;
  HepAListIterator<BtaCandidate> electron_iterator(*electron_list) ;
  HepAListIterator<BtaCandidate>    track_iterator(*track_list) ;
  
  HepAList<BtaCandidate>* signal_lists[4] ;
  signal_lists[0] = muon_list ;
  signal_lists[1] = electron_list ;
  signal_lists[2] = track_list ;
  signal_lists[3] = track_list ;

  // Get list of EvtCandidates from previous module
  HepAListIterator<BtaCandidate> Ups_iterator(*DRecoEvtCandidateList) ;
  int n_cands    = 0 ;
  int n_mu_cands = 0 ;
  int n_e_cands  = 0 ;
  while(BtaCandidate* Ups_in = Ups_iterator())
  { // Loop over EvtCandidates
    _scalers.sum("initial lists") ;	 
    
    BtaCandidate* recCharm1 = NULL ;
    BtaCandidate* recCharm2 = NULL ;
    BtaCandidate*  recKaon1 = NULL ;
    BtaCandidate*  recKaon2 = NULL ;
    BtaCandidate*    recPK1 = NULL ;
    BtaCandidate*    recPK2 = NULL ;
    BtaCandidate*  recfrag1 = NULL ;
    BtaCandidate*  recfrag2 = NULL ;
    BtaCandidate*    recgam = NULL ;
    BtaCandidate*   recDsSt = NULL ;
    BtaCandidate*     recDs = NULL ;
    HepAListIterator<BtaCandidate> Ups_daughter_iterator = Ups_in->daughterIterator() ; 

    while(BtaCandidate* Ups_daughter = Ups_daughter_iterator())
    {
      switch( abs(Ups_daughter->pdtEntry()->lundId()))
      {
      case 411:     //D+
      case 421:     //D0
      case 413:     //D+*
      case 423:     //D0*
      case 4122:    //LambdaC
      case 431:     //Ds
	recCharm1 = Ups_daughter;
	recCharm2 = Ups_daughter;
	break;
	
      case 321:    //K+
      case 310:    //Ks
	recKaon1 = Ups_daughter;
	recKaon2 = Ups_daughter;
	break;
	
      case 433:   //Ds*
	recDsSt = Ups_daughter;
	break;
	
      case 5132:  //Chib
      case 5232:  //Chib0
	recPK1 = Ups_daughter;
	recPK2 = Ups_daughter;
	break;
	
      default:     //pions
	recfrag1 = Ups_daughter;
	recfrag2 = Ups_daughter;
	break;
      }
    }

    HepAListIterator<BtaCandidate> DsSt_daughter_iterator = recDsSt->daughterIterator() ;
    while(BtaCandidate* DsSt_daughter = DsSt_daughter_iterator())
    {
      if( abs(DsSt_daughter->pdtEntry()->lundId())==abs(Pdt::lookup( PdtPdg::gamma    )->lundId())) recgam = DsSt_daughter ;
      if( abs(DsSt_daughter->pdtEntry()->lundId())==abs(Pdt::lookup( PdtPdg::D_s_plus )->lundId()))  recDs = DsSt_daughter  ;
    }
    
    if(recCharm1) cout << "recCharm lund: " << recCharm1->pdtEntry()->lundId() << endl ;
    if(recKaon1 ) cout << " recKaon lund: " << recKaon1->pdtEntry()->lundId()  << endl ;
    if(recfrag1 ) cout << " recfrag lund: " << recfrag1->pdtEntry()->lundId()  << endl ;
    if(recDsSt  ) cout << " recDsSt lund: " << recDsSt->pdtEntry()->lundId()   << endl ;
    if(recDs    ) cout << "   recDs lund: " <<   recDs->pdtEntry()->lundId()   << endl ;
    if(recgam   ) cout << "  recgam lund: " <<  recgam->pdtEntry()->lundId()   << endl ;
    if(recPK1   ) cout << "      PK lund: " <<  recPK1->pdtEntry()->lundId()   << endl ;
    
    int CharmLund = 0;
    if(recCharm1)
    {
      CharmLund = (recCharm1->pdtEntry()->lundId());
      switch(abs(CharmLund)){
      case 411 :  //D plus
      case 421 :  //D0
      case 413 :  //D*+  
      case 423 :  //D*0   
	if(!recKaon1) ErrMsg(fatal) << "Cannot find required kaon aborting" << endmsg;
	break;
      case 4122 :
	if(!recPK1)  ErrMsg(fatal) << "Cannot find required Lambda aborting" << endmsg;
	break;
      case 431 :
	break;
      default:
	ErrMsg(fatal) <<  "Charm tag not identified; CharmLund = " << CharmLund << endmsg;
      }
    }
    else ErrMsg(fatal) << "Cannot find charm tag aborting" << endmsg;
    
    float dsMass = recDs->p4().mag() ;
    DsMass = dsMass ;

    for(int signal_mode=2 ; signal_mode<4 ; signal_mode++)
    { // Loop over signal modes
      // signal_mode = 0 and signal_mode = 1 are deprecated (MuNNTight, EKMVeryLoose)

      HepAListIterator<BtaCandidate> iter(*signal_lists[signal_mode]) ;
      iter.rewind() ;
      while(BtaCandidate *recEll_temp = iter())
      { // Loop over leptons
	if(output_list->length()>=300) continue ;
	if(recEll_temp->overlaps(*Ups_in)) continue ;
	_scalers.sum("no lepton overlap") ;
	//if(recEll->charge()*recDs->charge()<0) continue ;

	BtaCandidate* recEll = new BtaCandidate(*recEll_temp) ;
	if(recEll->charge()>0 && signal_mode==2) recEll->setType(Pdt::lookup(PdtLund::mu_plus )) ;
	if(recEll->charge()<0 && signal_mode==2) recEll->setType(Pdt::lookup(PdtLund::mu_minus)) ;
	if(recEll->charge()>0 && signal_mode==3) recEll->setType(Pdt::lookup(PdtLund::e_plus  )) ;
	if(recEll->charge()<0 && signal_mode==3) recEll->setType(Pdt::lookup(PdtLund::e_minus )) ;

	// Compute missing 4-vector
	BbrLorentzVectorErr p4ups = eventInfo->electronBeam().fitParams().p4WErr() + eventInfo->positronBeam().fitParams().p4WErr() ;
	HepLorentzVector nup4miss = recDs->p4() - recEll->p4() ;
	
	NupxRaw     = nup4miss.px() ;
	NupyRaw     = nup4miss.py() ;
	NupzRaw     = nup4miss.pz() ;
	NuenergyRaw = nup4miss.e() ;
	NuMassRaw   = nup4miss.m() ;
	
	MupxRaw     = recEll->p4().px() ;
	MupyRaw     = recEll->p4().py() ;
	MupzRaw     = recEll->p4().pz() ;
	MuenergyRaw = recEll->p4().e() ;
	
	// Correction to nup4 (on-shell solution)
	HepLorentzVector nup4Corr = nup4Corrected(nup4miss, recEll->p4(), signal_mode) ;
	float Deltap3Squared = 
	    (nup4Corr.x()-nup4miss.x())*(nup4Corr.x()-nup4miss.x()) 
	  + (nup4Corr.y()-nup4miss.y())*(nup4Corr.y()-nup4miss.y())
	  + (nup4Corr.z()-nup4miss.z())*(nup4Corr.z()-nup4miss.z()) ;
	Deltap3 = sqrt(Deltap3Squared) ;
	
	NupxCorr = nup4Corr.px() ;
	NupyCorr = nup4Corr.py() ;
	NupzCorr = nup4Corr.pz() ;
	NuenergyCorr = nup4Corr.e() ;

	//-----------------------------------------------
	// Now create the Ups with on-shell nu
	//-----------------------------------------------
	BtaCandidate candNuCorr(nup4Corr) ;
	if(signal_mode==0) candNuCorr.setType(Pdt::lookup(PdtLund::nu_mu)) ;
	if(signal_mode==1) candNuCorr.setType(Pdt::lookup(PdtLund::nu_e )) ;
	if(signal_mode==2) candNuCorr.setType(Pdt::lookup(PdtLund::nu_mu)) ;
	if(signal_mode==3) candNuCorr.setType(Pdt::lookup(PdtLund::nu_e )) ;
	
	BtaCandidate tagDCorr = *recCharm1 ;
	BtaCandidate KaonCorr = *recKaon1 ;
	BtaCandidate  gamCorr = *recgam ;
	BtaCandidate fragCorr ;
	if(recfrag1!=NULL) fragCorr = *recfrag1 ;
	BtaCandidate   ellCorr = *recEll ;
	
	BtaOpMakeTree combDsCorr ;      
	BtaCandidate candDsCorr = combDsCorr.combine(candNuCorr, ellCorr) ;
	candDsCorr.setType(recDs->pdtEntry()) ;
	setMassConstraint(candDsCorr) ;
	
	BtaOpMakeTree combDsStCorr ;
	BtaCandidate candDsStCorr = combDsStCorr.combine(candDsCorr, gamCorr) ;
	candDsStCorr.setType(recDsSt->pdtEntry()) ;
	setMassConstraint(candDsStCorr) ;
	
	BtaOpMakeTree combUpsCorr ;
	BtaCandidate candUpsCorr ;

	switch(abs(CharmLund))
	{
	case  411 : //D+
	case  421 : //D0
	case  413 : //D+*
	case  423 : //D0*
	  if(recfrag1!=NULL) { candUpsCorr = combUpsCorr.combine(*recCharm1, *recKaon1, candDsStCorr, fragCorr) ; }
	  else { candUpsCorr = combUpsCorr.combine(*recCharm1, *recKaon1, candDsStCorr) ; }
	  break;
	case 4122 :
	  if(recfrag1!=NULL) { candUpsCorr = combUpsCorr.combine(*recCharm1, *recPK1, candDsStCorr, fragCorr) ; }
	  else { candUpsCorr = combUpsCorr.combine(*recCharm1, *recPK1, candDsStCorr) ; }
	  break;
	case 431 :
	  if(recfrag1!=NULL) { candUpsCorr = combUpsCorr.combine(*recCharm1, candDsStCorr, fragCorr) ; }
	  else { candUpsCorr = combUpsCorr.combine(*recCharm1, candDsStCorr) ; }
	  break;
	default:
	  ErrMsg(fatal) <<  "Charm tag not identified; CharmLund = " << CharmLund << endmsg;
	}
	candUpsCorr.setType(Pdt::lookup(PdtLund::Upsilon_4S)) ;
	
	//----------------------------------
	// Fit the event
	//----------------------------------
	setBeamConstraint(candUpsCorr, eventInfo) ;
	BbrVectorErr eMinusMom(eventInfo->electronBeam().p3WCov()) ;
	BbrVectorErr  ePlusMom(eventInfo->positronBeam().p3WCov()) ;
	eMinusMom.setCovMatrix(eMinusMom.covMatrix()*2.5) ;
     	ePlusMom.setCovMatrix(ePlusMom.covMatrix()*2.5) ;
	setEnergyConstraint(candUpsCorr, eMinusMom, ePlusMom, 0.5) ;
	
	// Fit the upsilon
	VtxFitterOper fitterCorr(candUpsCorr,VtxAbsAlgorithm::TreeFitter) ;
	fitterCorr.invalidateFit() ;
	fitterCorr.fitAll() ;
	
	// Extract 
	BtaCandidate fitted_nuCorr   = fitterCorr.getFitted(candNuCorr) ;
	HepLorentzVector fitted_nuCorr_p4 = fitted_nuCorr.p4() ;
	mm2corrected = fitted_nuCorr_p4.e()*fitted_nuCorr_p4.e() - fitted_nuCorr_p4.rho()*fitted_nuCorr_p4.rho() ;
 
	//-----------------------------------------------
	// Now create the Ups
	//-----------------------------------------------
	BtaCandidate candNu(nup4miss) ;
	if(signal_mode==0) candNu.setType(Pdt::lookup(PdtLund::nu_mu)) ;
	if(signal_mode==1) candNu.setType(Pdt::lookup(PdtLund::nu_e )) ;
	if(signal_mode==2) candNu.setType(Pdt::lookup(PdtLund::nu_mu)) ;
	if(signal_mode==3) candNu.setType(Pdt::lookup(PdtLund::nu_e )) ;
	
	BtaOpMakeTree combDs ;      
	BtaCandidate candDs = combDs.combine(candNu, *recEll) ;
	candDs.setType(recDs->pdtEntry())  ;
	setMassConstraint(candDs) ;
	
	BtaOpMakeTree combDsSt ;
	BtaCandidate candDsSt = combDsSt.combine(candDs, *recgam) ;
	candDsSt.setType(recDsSt->pdtEntry())  ;
	setMassConstraint(candDsSt) ;
	
	BtaOpMakeTree combUps ;
	BtaCandidate candUps ;
	switch(abs(CharmLund))
	{
	case  411 : //D+
	case  421 : //D0
	case  413 : //D+*
	case  423 : //D0*
	  if(recfrag1!=NULL) { candUps = combUps.combine(*recCharm1, *recKaon1, candDsSt, *recfrag1) ; }
	  else { candUps = combUps.combine(*recCharm1, *recKaon1, candDsSt) ; }
	  break;
	case 4122 :
	  if(recfrag1!=NULL) { candUps = combUps.combine(*recCharm1, *recPK1, candDsSt, *recfrag1) ; }
	  else { candUps = combUps.combine(*recCharm1, *recPK1, candDsSt) ; }
	  break;
	case 431 :
	  if(recfrag1!=NULL) { candUps = combUps.combine(*recCharm1, candDsSt, *recfrag1) ; }
	  else { candUps = combUps.combine(*recCharm1, candDsSt) ; }
	  break;
	default:
	  ErrMsg(fatal) <<  "Charm tag not identified; CharmLund = " << CharmLund << endmsg;
	}
	candUps.setType(Pdt::lookup(PdtLund::Upsilon_4S)) ;
	
	//----------------------------------
	// Fit the event
	//----------------------------------
	setBeamConstraint(candUps, eventInfo) ;
	eMinusMom.setCovMatrix(eMinusMom.covMatrix()*2.5 ) ;
	ePlusMom.setCovMatrix(ePlusMom.covMatrix()*2.5) ;
	setEnergyConstraint(candUps, eMinusMom, ePlusMom, 0.5) ;
		  
	// Fit the upsilon
	VtxFitterOper fitter(candUps,VtxAbsAlgorithm::TreeFitter) ;
	fitter.invalidateFit() ;
	fitter.fitAll() ;
	
	// Extract 
	candUps = fitter.getFittedTree() ;
	
	if((candUps.decayVtx())->status()!=BtaAbsVertex::Success) continue ;
	_scalers.sum("fit-success") ;
	
	int nDof = (int)candUps.decayVtx()->nDof() ;
	float chiSquared =  candUps.decayVtx()->chiSquared() ;
	if(probab(nDof, chiSquared) < FitProbCut.value()) continue ;
	ProbChi2 = probab(nDof, chiSquared) ;
	Chi2 = chiSquared ;
	ndof = nDof ;
	_scalers.sum("fit-prob") ;
	
	MyMuMCIdx = -1 ;
	MyEMCIdx  = -1 ;
	if(signal_mode==0 || signal_mode==2)
        { // Second truthmatch
	  if(_truthMap && _mcList)
	  {
	    if(GenMu)
	    {
	      // If No mu is generated all event is fake
	      BtaCandidate* recMutrue = NULL ;
	      if( abs ( recEll->pdtEntry()->lundId() )==abs(Pdt::lookup( PdtPdg::mu_plus)->lundId())) recMutrue=_truthMap->mcFromReco(recEll) ;
	      if(recMutrue)
	      {
		MyMuMCIdx = _mcList->index(BtuUtil::matchUid(_mcList,recMutrue)) ;
		_scalers.sum("# truth-matched muons") ; 
	      }
	    }
	  }
	  
	  //----------------------------------
	  // Quality of muon
	  //----------------------------------
	  qualMu = 1 ;
	  const BtaTrkQual* TrkQual = recEll->getMicroAdapter()->getTrkQual() ;
	  int n_SVTHits   = TrkQual->nSvtHits() ;
	  int n_DCHHits   = TrkQual->nDchHits() ;
	  int SVT_pattern = TrkQual->SvtPattern() ;
	  int n_SVTzHits  = 0 ;
	  int power_of_2  = 2 ;
	  for(int i=0 ; i<10 ; i++)
	  { // Quality
	    if(SVT_pattern%power_of_2 && i>4) n_SVTzHits++ ;
	    SVT_pattern = SVT_pattern - SVT_pattern%power_of_2 ;
	    power_of_2 = power_of_2*2 ;
	  }
	  if(n_SVTHits<4)  qualMu = 0 ;
	  if(n_SVTzHits<2) qualMu = 0 ;
	  if(n_DCHHits<12) qualMu = 0 ;
	}
	if(signal_mode==1 || signal_mode==3)
        { // Second truthmatch
	  if(_truthMap && _mcList)
	  {
	    if(GenE)
	    {
	      // Look for e
	      BtaCandidate* recEtrue = NULL ;
	      if(abs(recEll->pdtEntry()->lundId())==abs(Pdt::lookup(PdtPdg::e_plus)->lundId())) recEtrue=_truthMap->mcFromReco(recEll) ;
	      if(recEtrue)
	      {
		MyEMCIdx = _mcList->index(BtuUtil::matchUid(_mcList,recEtrue)) ;
		_scalers.sum("# truth-matched electrons") ; 
	      }
	    }
	  }
	  //----------------------------------
	  // Quality of electron
	  // nb this doesn't make sense if we want to search for signal
	  // This is for looking at backgrounds for Ds->mu nu, so treat it the same as a muon for now
	  // Change these quality cuts when looking for Ds->e nu signal
	  //----------------------------------
	  qualE = 1 ;
	  const BtaTrkQual* TrkQual = recEll->getMicroAdapter()->getTrkQual() ;
	  int n_SVTHits   = TrkQual->nSvtHits() ;
	  int n_DCHHits   = TrkQual->nDchHits() ;
	  int SVT_pattern = TrkQual->SvtPattern() ;
	  int n_SVTzHits  = 0 ;
	  int power_of_2  = 2 ;
	  for(int i=0 ; i<10 ; i++)
          { // Quality
	    if(SVT_pattern%power_of_2 && i>4) n_SVTzHits++ ;
	    SVT_pattern = SVT_pattern - SVT_pattern%power_of_2 ;
	    power_of_2 = power_of_2*2 ;
	  }
	  if(n_SVTHits<4 ) qualE = 0 ;
	  if(n_SVTzHits<2) qualE = 0 ;
	  if(n_DCHHits<12) qualE = 0 ;
	}

	//----------------------------------
	// Get the thrust, Fox-Wolfram moments, visible charge
	//----------------------------------	
	const std::string& chargedListKey = chargedList.valueString() ;
	const std::string& neutralListKey = neutralList.valueString() ;
	HepAList<BtaCandidate>* charged_list = Ifd<HepAList<BtaCandidate> >::get(anEvent, chargedListKey.c_str()) ;
	HepAList<BtaCandidate>* neutral_list = Ifd<HepAList<BtaCandidate> >::get(anEvent, neutralListKey.c_str()) ;
	if(charged_list==0) ErrMsg(fatal) << "Could not locate the list " << chargedList.valueString() << ". This is likely a configuration error." << endmsg ;
	if(neutral_list==0) ErrMsg(fatal) << "Could not locate the list " << neutralList.valueString() << ". This is likely a configuration error." << endmsg ;
	HepAListIterator<BtaCandidate> charged_iterator(*charged_list) ;
	HepAListIterator<BtaCandidate> neutral_iterator(*neutral_list) ;
	
	HepAList<BtaCandidate> all_list ;
	float extra_energy_neutral = 0 ;
	BbrLorentzVectorErr eep4 = eventInfo->electronBeam().fitParams().p4WErr() + eventInfo->positronBeam().fitParams().p4WErr() ;
	float PMiss_x    = eep4.x() ;
	float PMiss_y    = eep4.y() ;
	float PMiss_z    = eep4.z() ;
	float EMiss_temp = eep4.e() ;
	int number_of_extra_tracks = 0 ;
	while(BtaCandidate* charged = charged_iterator())
	{
	  PMiss_x    = PMiss_x    - charged->p4().x() ;
	  PMiss_y    = PMiss_y    - charged->p4().y() ;
	  PMiss_z    = PMiss_z    - charged->p4().z() ;
	  EMiss_temp = EMiss_temp - charged->p4().e() ;
	  if( !charged->overlaps(candUps) )
	  {
	    number_of_extra_tracks++ ;
	  }
	  all_list.append(charged) ;  
	}
	if(number_of_extra_tracks>0) continue ;
	charged_iterator.rewind() ;
	int n_extra_neutral = 0 ;
	while(BtaCandidate* neutral = neutral_iterator())
	{
	  PMiss_x    = PMiss_x    - neutral->p4().x() ;
	  PMiss_y    = PMiss_y    - neutral->p4().y() ;
	  PMiss_z    = PMiss_z    - neutral->p4().z() ;
	  EMiss_temp = EMiss_temp - neutral->p4().e() ;
	  if( !neutral->overlaps(candUps) && neutral->energy()>=0.03 )
	  {
	    extra_energy_neutral += neutral->energy() ;
	    n_extra_neutral++ ;
	  }
	  all_list.append(neutral) ;
	}
	neutral_iterator.rewind() ;
	EExtraNeutral = extra_energy_neutral ;
	NExtraNeutral = n_extra_neutral ;

	// Thrust
	BtaThrustFitter thruster ;
	thruster.compute(all_list) ;
	ThrustX = thruster.thrustAxis().x() ;
	ThrustY = thruster.thrustAxis().y() ;
	ThrustZ = thruster.thrustAxis().z() ;
	ThrustMag = thruster.thrust() ;

	// Fox-Wolfram moments
	BtaFoxWolfMom foxWolf(&all_list, eventInfo, 4) ;
	R0 = foxWolf.FWzero() ;
	R1 = foxWolf.FWnorm(1) ;
	R2 = foxWolf.FWnorm(2) ;
	R3 = foxWolf.FWnorm(3) ;
	R4 = foxWolf.FWnorm(4) ;

	//----------------------------------
	// Fill UsrData
	//----------------------------------
	DRecoUsrBlockIn->get(*Ups_in,  TagUncMass  ) ;
	DRecoUsrBlockIn->get(*Ups_in,  DsStUncMass ) ;
	DRecoUsrBlockIn->get(*Ups_in,  DsUncMass   ) ;
	DRecoUsrBlockIn->get(*Ups_in,  MyGamMCIdx  ) ;
	DRecoUsrBlockIn->get(*Ups_in,  MyKMCIdx    ) ;
	DRecoUsrBlockIn->get(*Ups_in,  MyTagMCIdx  ) ;

	PMiss_x = PMiss_x ;
	PMiss_y = PMiss_y ;
	PMiss_z = PMiss_z ;
	EMiss   = EMiss ;
	float PMiss_temp = sqrt(PMiss_x*PMiss_x + PMiss_y*PMiss_y + PMiss_z*PMiss_z) ;
	PMissCosTh = PMiss_z/PMiss_temp ;
	PMissPhi   = atan2(PMiss_y, PMiss_x) ;
	PMiss      = PMiss_temp ;
	EMiss      = EMiss_temp ;

	SignalMode = signal_mode ;
	if(!DRecoUsrBlock.put(candUps, TagUncMass     )) cout << "Error saving TagUncMass UsrData"    << endl ;
	if(!DRecoUsrBlock.put(candUps, DsStUncMass    )) cout << "Error saving DsStUncMass UsrData"   << endl ;
	if(!DRecoUsrBlock.put(candUps, DsUncMass      )) cout << "Error saving DsUncMass UsrData"     << endl ;
	if(!DRecoUsrBlock.put(candUps, DsMass         )) cout << "Error saving DsMass UsrData"        << endl ;
	if(!DRecoUsrBlock.put(candUps, qualMu         )) cout << "Error saving qualMu UsrData"        << endl ;
	if(!DRecoUsrBlock.put(candUps, qualE          )) cout << "Error saving qualE UsrData"         << endl ;
 	if(!DRecoUsrBlock.put(candUps, MyGamMCIdx     )) cout << "Error saving MyMuMCIdx UsrData"     << endl ;
	if(!DRecoUsrBlock.put(candUps, MyKMCIdx       )) cout << "Error saving MyKMCIdx UsrData"      << endl ;
	if(!DRecoUsrBlock.put(candUps, MyTagMCIdx     )) cout << "Error saving MyTagMCIdx UsrData"    << endl ;
	if(!DRecoUsrBlock.put(candUps, MyMuMCIdx      )) cout << "Error saving MyMuMCIdx UsrData"     << endl ;
	if(!DRecoUsrBlock.put(candUps, MyEMCIdx       )) cout << "Error saving MyEMCIdx UsrData"      << endl ;
	if(!DRecoUsrBlock.put(candUps, mm2corrected   )) cout << "Error saving mm2corrected UsrData"  << endl ;
	if(!DRecoUsrBlock.put(candUps, Deltap3        )) cout << "Error saving Deltap3 UsrData"       << endl ;
	if(!DRecoUsrBlock.put(candUps, DsToMuNu       )) cout << "Error saving DsToMuNu UsrData"      << endl ;
	if(!DRecoUsrBlock.put(candUps, DsToENu        )) cout << "Error saving DsToENu UsrData"       << endl ;
	if(!DRecoUsrBlock.put(candUps, ProbChi2       )) cout << "Error saving ProbChi2 UsrData"      << endl ;
	if(!DRecoUsrBlock.put(candUps, Chi2           )) cout << "Error saving Chi2 UsrData"          << endl ;
	if(!DRecoUsrBlock.put(candUps, ndof           )) cout << "Error saving ndof UsrData"          << endl ;
	if(!DRecoUsrBlock.put(candUps, NuMassRaw      )) cout << "Error saving NuMassRaw UsrData"     << endl ;
	if(!DRecoUsrBlock.put(candUps, SignalMode     )) cout << "Error saving SignalMode UsrData"    << endl ;
	if(!DRecoUsrBlock.put(candUps, NupxRaw        )) cout << "Error saving NupxRaw UsrData"       << endl ;
	if(!DRecoUsrBlock.put(candUps, NupyRaw        )) cout << "Error saving NupyRaw UsrData"       << endl ;
	if(!DRecoUsrBlock.put(candUps, NupzRaw        )) cout << "Error saving NupzRaw UsrData"       << endl ;
	if(!DRecoUsrBlock.put(candUps, NuenergyRaw    )) cout << "Error saving NuenergyRaw UsrData"   << endl ;
	if(!DRecoUsrBlock.put(candUps, MupxRaw        )) cout << "Error saving MupxRaw UsrData"       << endl ;
	if(!DRecoUsrBlock.put(candUps, MupyRaw        )) cout << "Error saving MupyRaw UsrData"       << endl ;
	if(!DRecoUsrBlock.put(candUps, MupzRaw        )) cout << "Error saving MupzRaw UsrData"       << endl ;
	if(!DRecoUsrBlock.put(candUps, MuenergyRaw    )) cout << "Error saving MuenergyRaw UsrData"   << endl ;
	if(!DRecoUsrBlock.put(candUps, NupxCorr       )) cout << "Error saving NupxCorr UsrData"      << endl ;
	if(!DRecoUsrBlock.put(candUps, NupyCorr       )) cout << "Error saving NupyCorr UsrData"      << endl ;
	if(!DRecoUsrBlock.put(candUps, NupzCorr       )) cout << "Error saving NupzCorr UsrData"      << endl ;
	if(!DRecoUsrBlock.put(candUps, NuenergyCorr   )) cout << "Error saving NuenergyCorr UsrData"  << endl ;
	if(!DRecoUsrBlock.put(candUps, ThrustX        )) cout << "Error saving thrust_x UsrData"      << endl ;
	if(!DRecoUsrBlock.put(candUps, ThrustY        )) cout << "Error saving thrust_y UsrData"      << endl ;
	if(!DRecoUsrBlock.put(candUps, ThrustZ        )) cout << "Error saving thrust_z UsrData"      << endl ;
	if(!DRecoUsrBlock.put(candUps, ThrustMag      )) cout << "Error saving thrust_mag UsrData"    << endl ;
	if(!DRecoUsrBlock.put(candUps, R0             )) cout << "Error saving R0 UsrData"            << endl ;
	if(!DRecoUsrBlock.put(candUps, R1             )) cout << "Error saving R1 UsrData"            << endl ;
	if(!DRecoUsrBlock.put(candUps, R2             )) cout << "Error saving R2 UsrData"            << endl ;
	if(!DRecoUsrBlock.put(candUps, R3             )) cout << "Error saving R3 UsrData"            << endl ;
	if(!DRecoUsrBlock.put(candUps, R4             )) cout << "Error saving R4 UsrData"            << endl ;
	if(!DRecoUsrBlock.put(candUps, EExtraNeutral  )) cout << "Error saving EExtraNeutral UsrData" << endl ;
	if(!DRecoUsrBlock.put(candUps, NExtraNeutral  )) cout << "Error saving NExtraNeutral UsrData" << endl ;
	if(!DRecoUsrBlock.put(candUps, PMissCosTh     )) cout << "Error saving PMissCosTh UsrData"    << endl ;
	if(!DRecoUsrBlock.put(candUps, PMissPhi       )) cout << "Error saving PMissPhi UsrData"      << endl ;
	if(!DRecoUsrBlock.put(candUps, PMiss          )) cout << "Error saving PMiss UsrData"         << endl ;
	if(!DRecoUsrBlock.put(candUps, EMiss          )) cout << "Error saving EMiss UsrData"         << endl ;

	if(signal_mode==0 || signal_mode==2) n_mu_cands++ ;
	if(signal_mode==1 || signal_mode==3) n_e_cands++ ;
	n_cands++ ;
	output_list->append(new BtaCandidate(candUps)) ;
       	_scalers.sum("final lists") ;
      } // ell iterator
    } // signal mode iterator
  }
  return AppResult::OK ;
}

AppResult DRecoDsToEllNuFit::endJob( AbsEvent* anEvent )
{
  // endJob
  ostream& o=cout ;
  _scalers.print(o) ;
  o << endl ;

  return AppResult::OK ;
}

HepLorentzVector DRecoDsToEllNuFit::nup4Corrected(HepLorentzVector nup4, HepLorentzVector Ellp4, int signal_mode)
{ 
  // nup3 correction
  // Steve's numerical method re-hashed
  const double mDs_PDG = Pdt::lookup(PdtLund::D_s_plus)->mass() ;
  const double mMu_PDG =  Pdt::lookup(PdtLund::mu_plus)->mass() ;
  const double  mE_PDG =   Pdt::lookup(PdtLund::e_plus)->mass() ;

  double mEll = 0 ;
  if(signal_mode==0) mEll = mMu_PDG ;
  if(signal_mode==1) mEll = mE_PDG ;
  if(signal_mode==2) mEll = mMu_PDG ;
  if(signal_mode==3) mEll = mE_PDG ;
  if(mEll==0) ErrMsg(fatal) << "Cannot determine signal mode.  The mass of the lepton must be known for nup4 correction" << endmsg ;
  double M2 = (mDs_PDG*mDs_PDG-mEll*mEll)*0.5 ;
  double E_Ell = Ellp4.e() ;
  double p_Ell = Ellp4.rho() ;

  // Determine the distance of closest approach of the missing momentun recoiling against the DKXgamma system.
  // On-shell solutions fall on an ellipse defined by
  //
  // cos(psi) = [ E_ell - (m_Ds^2 - m_ell^2)/(2p_nu) ]/p_ell
  //                         or
  // p_nu = [ m_Ds^2 - m_ell^2 ]/2*( E_ell - p_ell*cos(psi) )
  //
  // Rotate about psi until a minimum solution is found

  const double pi = 3.1519265 ;
  int number_of_steps = 1000 ;
  double psi_step_size = 2*pi/number_of_steps ;
  double closest_distance = 1.e6 ;
  double closest_psi = -2 ;
  double second_closest_distance = 1.e6 ;
  double second_closest_psi = -2 ;

  Hep3Vector nup3 = nup4.vect() ;
  Hep3Vector Ellp3 = Ellp4.vect() ;
  Hep3Vector rotation_axis = nup3.unit().cross(Ellp3.unit()) ;

  Hep3Vector closest_p3(0,0,0) ;

  for(int i=0 ; i<number_of_steps ; i++)
  {
    double angle = i*psi_step_size ;
    Hep3Vector rotated_nup3 = nup3 ;
    rotated_nup3.rotate(angle, rotation_axis.unit()) ;
    double psi = Ellp3.angle(rotated_nup3) ;
    double p_nu_mag = M2/(E_Ell-p_Ell*cos(psi)) ;
    rotated_nup3.setMag(p_nu_mag) ;
    Hep3Vector difference = rotated_nup3 - nup3 ;
    double distance = difference.mag() ;

    if(distance<closest_distance)
    {
      second_closest_distance = closest_distance ;
      second_closest_psi = closest_psi ;
      closest_distance = distance ;
      closest_psi = psi ;
      closest_p3 = rotated_nup3 ;
    }
    else
    {
      if(distance<second_closest_distance)
      {
	second_closest_distance = distance ;
	second_closest_psi = psi ;
      }
    }
  }

  double lower_distance = 1.e6 ;
  double lower_psi = 1.e6 ;
  double upper_distance = 1.e6 ;
  double upper_psi = 1.e6 ;
  if(closest_psi<second_closest_psi)
  {
    lower_distance = closest_distance ;
    lower_psi = closest_psi ;
    upper_distance = second_closest_distance ;
    upper_psi = second_closest_psi ;
  }
  else
  {
    upper_distance = closest_distance ;
    upper_psi = closest_psi ;
    lower_distance = second_closest_distance ;
    lower_psi = second_closest_psi ;
  }

  HepLorentzVector nup4_new(closest_p3, closest_p3.mag()) ;
  return nup4_new ;
}


