//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: BToDstarlnuXFilter.cc,v 1.9 2006/11/23 19:05:01 desilva Exp $
//
// Description:
//
// Framework module to select D* lnu events for the BToDlnu skim.
//
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Eugenio Paoloni & Gabriele Simi
//
// History (add to end):
//
// Copyleft Information:
//      Copyleft (C) 2004                      INFN Pisa and SLAC
//
//------------------------------------------------------------------------

#include "BaBar/BaBar.hh"
#include "FilterTools/BToDstarlnuXFilter.hh"
#include "ErrLogger/ErrLog.hh"
#include "CLHEP/Alist/AList.h"
#include "CLHEP/Alist/AIterator.h"
#include "CLHEP/Vector/LorentzVector.h"
#include "ProxyDict/IfdStrKey.hh"
#include "ProxyDict/IfdKey.hh"
#include "AbsEvent/getTmpAList.hh"      // Get from the "event" a given list 
                                        // of objects 
#include "Beta/BtaCandidate.hh"
#include "BetaMicroAdapter/BtaCalQual.hh"
#include "Beta/EventInfo.hh"
#include "PDT/Pdt.hh"
#include "BetaCoreTools/BtaOpMakeTree.hh"
#include "BetaCoreTools/BtaTreeNavigator.hh"
#include "VtxFitter/VtxFitterOper.hh"
#include "ProbTools/probab.hh"
#include "BetaTools/BtaSemilepCand.hh"

#include <assert.h>
#include <iomanip>
#include <iostream>


using std::cout;
using std::endl;
using std::setbase;

// Uncomment the next line to gather heap size information at run time
// #define GENE_MEMORY_LEACK_TEST

#ifdef GENE_MEMORY_LEACK_TEST
#include "RecoUtils/SizeCounter.hh"
#endif

BToDstarlnuXFilter::BToDstarlnuXFilter( const char* const name, 
					  const char* const description )
	: AppModule( name, description ),
	  _BList("BinputList",this,"BToDstarlnuAll"),
	  _refittedBList("BoutputList",this,"BToDstarlnuRefit"),
	  _eventData("eventBlock",this,"BToDlnuEventData"),
	  _candData("candBlock",this,"BToDlnuCandData"),
	  _eSelectorKey ( "eSelector", this,  "eSelectorsMap"),
	  _muSelectorKey("muSelector", this, "muSelectorsMap"),
	  _KSelectorKey ( "KSelector", this,  "KSelectorsMap"),
	  
	  _theB            (0),
	  _B_refitted      (0),

	  _B_decay_mode       ("B_decay_mode"),
	  _B_chi2_prob        ("B_chi2_prob"),
	  _B_vtx_status       ("B_vtx_status"),
	  _B_p4               ("B_p4"),

	  _l_p4               ("l_p4"),

	  _Dstar_mass_raw     ("Dstar_mass_raw"),
	  _Dstar_mass         ("Dstar_mass"),
	  _Dstar_p4           ("Dstar_p4"),

	  _D_mass_raw         ("D_mass_raw"),
	  _D_mass             ("D_mass"),
	  _D_chi2_prob        ("D_chi2_prob"),
	  _D_vtx_status       ("D_vtx_status"),
	  _D_p4               ("D_p4"),

	  _pis_p4             ("pis_p4"),

	  _cos_theta_BY       ("cos_theta_BY"),

	  _e_bitmask          ("e_bitmask"),
	  _mu_bitmask         ("mu_bitmask"),
	  _K_bitmask          ("K_bitmask"),

	  _put_cand_ok        (true)

{
	commands()->append( &_BList ); 
	commands()->append( &_refittedBList );
	commands()->append( &_eventData );
	commands()->append( &_candData );

	commands()->append( &_eSelectorKey);
	commands()->append( &_muSelectorKey);
	commands()->append( &_KSelectorKey);

	_candBlock.addVariable( _B_decay_mode );
	_candBlock.addVariable( _B_vtx_status );
	_candBlock.addVariable( _B_chi2_prob );
	_candBlock.addVariable( _B_p4 );
	
	_candBlock.addVariable( _l_p4 );

	_candBlock.addVariable( _Dstar_mass_raw );
	_candBlock.addVariable( _Dstar_mass );
	_candBlock.addVariable( _Dstar_p4 );

	_candBlock.addVariable( _D_mass_raw );
	_candBlock.addVariable( _D_mass );
	_candBlock.addVariable( _D_chi2_prob );
	_candBlock.addVariable( _D_vtx_status );
	_candBlock.addVariable( _D_p4 );

	_candBlock.addVariable( _pis_p4 );

	_candBlock.addVariable( _e_bitmask );
	_candBlock.addVariable( _mu_bitmask );
	_candBlock.addVariable( _K_bitmask );
	
	_candBlock.addVariable( _cos_theta_BY ); 
}

BToDstarlnuXFilter::~BToDstarlnuXFilter( )
{
}

AppResult BToDstarlnuXFilter::beginJob( AbsEvent* event )
{  
	return AppResult::OK; 
}

AppResult BToDstarlnuXFilter::endJob( AbsEvent* event ) 
{
	ErrMsg(routine) << "end job" << endmsg;
	return AppResult::OK;
}

AppResult BToDstarlnuXFilter::event( AbsEvent* event )
{

#ifdef GENE_MEMORY_LEACK_TEST	
	SizeCounter    Heap;
#endif

	ErrMsg( debugging )  << "Module " << name() << endmsg;

	_event = event;

	static const IfdStrKey defaultKey("Default");
	HepAList<EventInfo>* eventInfoList = 
		Ifd< HepAList<EventInfo> >::get( event, defaultKey );

	_eventInfo = eventInfoList->first();
	_p4CM = _eventInfo->cmFrame();
	_CM_frame = -(_p4CM.boostVector());

	_eSelectorsMap = 
		Ifd< std::map<BtaCandidate*, int> >::get(
			event,
			_eSelectorKey.value());
	_muSelectorsMap = 
		Ifd< std::map<BtaCandidate*, int> >::get(
			event,
			_muSelectorKey.value());
	_KSelectorsMap = 
		Ifd< std::map<BtaCandidate*, int> >::get(
			event,
			_KSelectorKey.value());

	if ( ! ( _eSelectorsMap && _muSelectorsMap && _KSelectorsMap) )
		ErrMsg( fatal ) << "Trouble with the PID bitmask" << endl 
				<< "Ele  :" << _eSelectorsMap  << endl
				<< "Muon :" << _muSelectorsMap << endl
				<< "Kaon :" << _KSelectorsMap  << endmsg;

	HepAList<BtaCandidate>* BList =
		Ifd< HepAList<BtaCandidate> >::get( event, _BList.value() );
	assert ( 0 != BList );


	HepAList<BtaCandidate>* out_BList;
	getTmpAList( event, out_BList, _refittedBList.value());	     
	assert ( 0 != out_BList );
	HepAListDeleteAll( * out_BList );
	
	ErrMsg( debugging ) << "\n\n Event (Writing User Data ) \n" 
			    << "storing UsrData block " 
			    << _eventData.value() 
			    << endmsg;

	UsrIfd<UsrEventBlock>::put( event, &_eventBlock, _eventData.value() );

	ErrMsg( debugging ) << "storing UsrData block " 
			    << _candData.value() << endmsg;

	
	UsrIfd<UsrCandBlock>::put( event, & _candBlock, _candData.value() );
  
  
	HepAListIterator<BtaCandidate> i( *BList );


	while( ( _theB = i() ) )
	{
		/***
		 * Set the data member to a safe initial state: All pointer = 0
		 */
		initialize();

		/**
		 *  Fill the pointers to the lepton, Dstar, pi_soft , D , Kaon
		 *  and the decay channel of the D
		 */
		if (! set_B_decay_mode() )
			continue;

		/****
		 * PID informations
		 */
		fill_PID();


		/***
		 *   Refit the D0 and the Dstar 
		 */
		refit_B();
		
		/***
		 *  Evaluate  Cos Theta BY
		 */

		cos_theta_BY();

		if( verbose() ){
			cout << "Refitted B:\n";
			BtaTreeNavigator::printTree(*_B_refitted);
		}
		out_BList->append( _B_refitted );

		_put_cand_ok &= _candBlock.put( *_B_refitted, _B_decay_mode);
		_put_cand_ok &= _candBlock.put( *_B_refitted, _B_vtx_status);
		_put_cand_ok &= _candBlock.put( *_B_refitted, _B_chi2_prob);
		_put_cand_ok &= _candBlock.put( *_B_refitted, _B_p4);

		_put_cand_ok &= _candBlock.put( *_B_refitted, _e_bitmask  );
		_put_cand_ok &= _candBlock.put( *_B_refitted, _mu_bitmask );
		_put_cand_ok &= _candBlock.put( *_B_refitted, _K_bitmask  );
		_put_cand_ok &= _candBlock.put( *_B_refitted, _l_p4);

		/** 
		 *   Vertex informations before the refit
		 */
		_D_mass_raw = _theD ? _theD->mass() : -1.;		
		_put_cand_ok &= _candBlock.put( *_B_refitted, _D_mass_raw);
		_Dstar_mass_raw = _Dstar->mass() ;
		_put_cand_ok &= _candBlock.put( *_B_refitted, _Dstar_mass_raw);

		_put_cand_ok &= _candBlock.put( *_B_refitted, _Dstar_mass);

		_put_cand_ok &= _candBlock.put( *_B_refitted, _Dstar_p4);

		_put_cand_ok &= _candBlock.put( *_B_refitted, _D_mass);
		_put_cand_ok &= _candBlock.put( *_B_refitted, _D_chi2_prob);
		_put_cand_ok &= _candBlock.put( *_B_refitted, _D_vtx_status);
		_put_cand_ok &= _candBlock.put( *_B_refitted, _D_p4);
		_put_cand_ok &= _candBlock.put( *_B_refitted, _pis_p4);

		_put_cand_ok &= _candBlock.put( *_B_refitted, _cos_theta_BY);

		
		if( ! _put_cand_ok)
			ErrMsg( error ) << "Event Cand Block: Put failed" 
					<< endl
					<< "B decay mode : " 
					<< setbase(16) << ((int) _B_decay_mode)
					<< endmsg;
	}
	
#ifdef GENE_MEMORY_LEACK_TEST
	cout << "Heap Evt Check:\t" << Heap.currentSize() 
	     << "\t" << Heap.deltaSize() << endl;
#endif

	return AppResult::OK;
}

void
BToDstarlnuXFilter::initialize(void)
{
	
	HepAListDeleteAll( _garbage );
	_put_cand_ok    = true;

	_B_decay_mode   = 0;
	_lepton         = 0;

	_pis            = 0;
	_theD              = 0;
	_theK              = 0;

	_B_refitted     = 0;
	_Dstar          = 0;
	
}

bool
BToDstarlnuXFilter::set_B_decay_mode( void )
{
	if( verbose() )
		BtaTreeNavigator::printTree(*_theB);

	HepAListIterator<BtaCandidate> daughterIterator = 
		_theB->daughterIterator();
	
	_Dstar  = daughterIterator(); // First  B Daughter
	_lepton = daughterIterator(); // Second B Daughter
	
	if( daughterIterator() )
		ErrMsg( error ) << "The candidate B: " << endl
				<< *_theB  << endl
				<< " has more than 2 daughters " 
				<< endmsg;
	
	if (! (_Dstar && _lepton) ){
		ErrMsg( error ) << "The candidate B: " <<endl
				<< *_theB << endl
				<< " has less than 2 daughters. " << endl 
				<< "Dstar : " << _Dstar << endl
				<< "Lept  : " << _lepton << endl
				<< endmsg;
		return false;
	}
	
	
	if ( _Dstar->nDaughters()!=2 )
		ErrMsg( error ) << "The candidate D*: " 
				<< *_Dstar 
				<< "does not have exactly 2 daughters " 
				<< endmsg;
	
	_B_decay_mode = _theB->charge() != 0 ? Charged_B : 0x0 ;
	_B_decay_mode = _lepton->charge() < 0 ?  
		( _B_decay_mode | lepton ):
		( _B_decay_mode & ~ lepton) ;
	
	switch ( _Dstar -> pdtEntry()->lundId() ){
		
	case PdtLund::D_star0     :
		_B_decay_mode = _B_decay_mode | neutral_D;
		_B_decay_mode = _B_decay_mode | quark_b ;
		_pis = _Dstar -> getDaughter(  PdtLund::pi0 );
		_theD   = _Dstar -> getDaughter(  PdtLund::D0 );
		break;
		
	case PdtLund::anti_D_star0:
		_B_decay_mode = _B_decay_mode | neutral_D;
		_B_decay_mode = _B_decay_mode & ~ quark_b;
		_pis = _Dstar -> getDaughter(  PdtLund::pi0 );
		_theD   = _Dstar -> getDaughter(  PdtLund::anti_D0 );
		break;
		
	case PdtLund::D_star_plus     :
		_B_decay_mode = _B_decay_mode | quark_b ;
		_pis = _Dstar -> getDaughter(  PdtLund::pi_plus );
		if( _pis ){
			_B_decay_mode = _B_decay_mode | neutral_D;
			_theD   = _Dstar -> getDaughter(  PdtLund::D0 );
		} else {
			_pis = _Dstar -> getDaughter(  PdtLund::pi0 );
			_B_decay_mode = _B_decay_mode & ~neutral_D;
			_theD   = _Dstar -> getDaughter(  PdtLund::D_plus );
		}
		break;
	case PdtLund::D_star_minus     :
		_B_decay_mode = _B_decay_mode & ~ quark_b ;
		_pis = _Dstar -> getDaughter(  PdtLund::pi_minus );
		if( _pis ){
			_B_decay_mode = _B_decay_mode | neutral_D;
			_theD   = _Dstar -> getDaughter(  PdtLund::anti_D0 );
		} else {
			_pis = _Dstar -> getDaughter(  PdtLund::pi0 );
			_B_decay_mode = _B_decay_mode & ~neutral_D;
			_theD   = _Dstar -> getDaughter(  PdtLund::D_minus );
		}
		break;
	default:
		ErrMsg ( fatal ) << (*_Dstar) << endl
				 << "The Dstar must have two daughters : a D0 and a pi  "
				 << endmsg;
	}
	
	switch ( _theD -> pdtEntry()->lundId() ){
		
	case PdtLund::D0      :
		_theK = _theD -> getDaughter( PdtLund::K_minus );

		switch ( _theD -> nDaughters() ){
		case 2:
			_B_decay_mode = _B_decay_mode  | K_pi ;
			break;
		case 3:
			if ( ! _theK ){
				_theK = _theD -> getDaughter( PdtLund::K_S0 );
				_B_decay_mode = _B_decay_mode  | KS_2pi ;
			} else
				_B_decay_mode = _B_decay_mode  | K_pi_pi0 ;
			break;
		case 4:
			_B_decay_mode = _B_decay_mode  | K_3pi ;
			break;
		default:
			ErrMsg( error ) << "The D candidate "  << endl
					<< * _theD << endl   
					<< " has a strange number of daughters"
					<< endmsg;
		}
		break;

	case PdtLund::anti_D0 :
		_theK = _theD -> getDaughter( PdtLund::K_plus );
			
		switch ( _theD -> nDaughters() ){
		case 2:
			_B_decay_mode = _B_decay_mode  | K_pi ;
			break;
		case 3:
			if ( ! _theK ){
				_theK = _theD -> getDaughter( PdtLund::K_S0 );
				_B_decay_mode = _B_decay_mode  | KS_2pi ;
			} else
				_B_decay_mode = _B_decay_mode  | K_pi_pi0 ;
			break;
		case 4:
			_B_decay_mode = _B_decay_mode  | K_3pi ;
			break;
		default:
			ErrMsg( error ) << "The D candidate "  << endl
					<< * _theD << endl   
					<< " has a strange number of daughters"
					<< endmsg;
		}
		break;
	case PdtLund::D_plus :
		
		switch ( _theD -> nDaughters() ){
		case 3:
			_theK = _theD -> getDaughter( PdtLund::K_minus );
			_B_decay_mode = _B_decay_mode  | K_2pi ;
			break;
		case 2:
			_theK = _theD -> getDaughter( PdtLund::K_S0 );
			_B_decay_mode = _B_decay_mode  | KS_pi ;
			break;
		default:
			ErrMsg ( error ) << "The D+ candidate " <<endl
					 << *_theD << endl
					 << "has a strange number of daughters" 
					 << endmsg;
		}
		break;
	case PdtLund::D_minus :
		switch ( _theD -> nDaughters() ){
		case 3:
			_theK = _theD -> getDaughter( PdtLund::K_plus );
			_B_decay_mode = _B_decay_mode  | K_2pi ;
			break;
		case 2:
			_theK = _theD -> getDaughter( PdtLund::K_S0 );
			_B_decay_mode = _B_decay_mode  | KS_pi ;
			break;
		default:
			ErrMsg ( error ) << "The D+ candidate " <<endl
					 << *_theD << endl
					 << "has a strange number of daughters" 
					 << endmsg;
		}
		break;
		
	default:
		ErrMsg ( error ) << "Strange to be there!!" << endmsg;
	}

	if( !(_lepton && _Dstar && _pis && _theD && _theK) )
		ErrMsg( error ) << "Not Found some daughter " << endl
				<< "Lept: " << _lepton << endl
				<< "D*  : " << _Dstar  << endl
				<< "Pi_s: " << _pis    << endl
				<< "D   : " << _theD      << endl
				<< "K   : " << _theK    << endmsg;

	return _lepton && _Dstar && _pis && _theD && _theK;
}

bool
BToDstarlnuXFilter::fill_PID( void )
{
	std::map< BtaCandidate*, int>::iterator  can_bitmask;

	static const IfdStrKey keyChargedTracks("ChargedTracks" );
	HepAList<BtaCandidate>* track_list =
		Ifd< HepAList<BtaCandidate> >::
		get(_event, keyChargedTracks);
	assert ( 0 != track_list );

	HepAListIterator<BtaCandidate> i( *track_list );
	const BtaCandidate * track;

	while( ( track = i() ) ){
		if( _lepton->isCloneOf( *track ) ){

			can_bitmask = _eSelectorsMap->find( (BtaCandidate *)track );
			if (can_bitmask != _eSelectorsMap->end() ) 
				_e_bitmask = can_bitmask->second;
			else
				ErrMsg( fatal ) 
					<< " Cannot find the candidate:" 
					<< endl
					<< _lepton << endl
					<< "in the electron bitmask" 
					<< endmsg ;
			
			can_bitmask = _muSelectorsMap->find( (BtaCandidate *)track );
			if (can_bitmask != _muSelectorsMap->end()) 
				_mu_bitmask = can_bitmask->second;
			else
				ErrMsg( fatal ) 
					<< " Cannot find the candidate:" 
					<< endl
					<< _lepton << endl
					<< "in the muon bitmask" << endmsg ;
		}
		    
		if( _theK->isCloneOf ( *track ) ) {
			can_bitmask = _KSelectorsMap->find((BtaCandidate *) track );
			if (can_bitmask != _KSelectorsMap->end()) 
				_K_bitmask = can_bitmask->second;
			else
				ErrMsg( fatal ) 
					<< " Cannot find the candidate:"
					<< endl
					<< _theK << endl
					<< "in the kaon bitmask" 
					<< endmsg ;
		}
	}   
	return true;
	
}


bool
BToDstarlnuXFilter::refit_B( void )
{

#ifdef GENE_MEMORY_LEACK_TEST
	SizeCounter    Heap;
#endif
	/*******
	 *
	 *  D with: 
	 *    - geometrical constraints
	 *    - Ks or pi0 constrained to pdf masses
	 *    
	 **/

	BtaCandidate * D_placeholder = make_D_tree();
	// Fit
	VtxFitterOper D_fitter ( *D_placeholder, VtxAbsAlgorithm::TreeFitter );
	D_fitter.fitAll();

	// Pick the result
	BtaCandidate * D_fitted;
	D_fitted = new BtaCandidate( D_fitter.getFitted( *D_placeholder ) );

	// Assign UserVariables
	_D_mass = D_fitted->mass();
	_D_chi2_prob = probab( D_fitted->decayVtx()->nDof(),
			       D_fitted->decayVtx()->chiSquared());
	_D_vtx_status = D_fitted->decayVtx()->status();

	_garbage.append( D_placeholder );
	_garbage.append( D_fitted    );

	/*****
	 *
	 *  Now the D*  with 
	 *    - the D mass constrained to the PDG value
	 *    - the soft pi0 mass constrained to the PDG value
	 *    - the D* and the lepton from a common vertex
	 *
	 */ 
	BtaOpMakeTree combiner;

	BtaCandidate * D_mass_constrained = make_D_tree();
	setMassConstraint ( *D_mass_constrained ); 	

	BtaCandidate * Soft_pion(0);
	switch ( _pis -> pdtEntry() -> lundId() ){
			// if the Soft pion is a pi0
		case PdtLund::pi0  :
		{
			HepAListIterator<BtaCandidate> tmpIterator( 
				_pis -> daughterIterator() );
			Soft_pion = 
				combiner.createFromList ( tmpIterator );
					
			// Lund Type 
			Soft_pion->setType( _pis->pdtEntry() );
			// constraints
			setGeoConstraint      ( *Soft_pion ); 
			setMomentumConstraint ( *Soft_pion ); 
			setMassConstraint     ( *Soft_pion ); 
			_garbage.append( Soft_pion );
		}
			break;
		default:
			Soft_pion = (BtaCandidate *)_pis;
		}


	// Dstar tree
	BtaCandidate * Dstar_placeholder = combiner.create( * D_mass_constrained, * Soft_pion );
	// set Dstar type
	Dstar_placeholder->setType ( _Dstar->pdtEntry() );

	// Dstar constraints
	setGeoConstraint      ( * Dstar_placeholder ); 
	setMomentumConstraint ( * Dstar_placeholder ); 

	// B tree
	BtaCandidate * B_placeholder = combiner.create( * Dstar_placeholder , * _lepton );
	// set B type
	B_placeholder->setType ( _theB->pdtEntry() );

	// B constraints
	setGeoConstraint      ( * B_placeholder );
	setMomentumConstraint ( * B_placeholder );

	// Fit All
	VtxFitterOper B_fitter ( * B_placeholder, VtxAbsAlgorithm::TreeFitter );
	B_fitter.fitAll();

	// Pick the results
	_B_refitted = new BtaCandidate(  B_fitter.getFittedTree() );

	HepAListIterator<BtaCandidate> Bdau = _B_refitted->daughterIterator();

  	BtaCandidate * Dstar_refitted = Bdau.next();
	BtaCandidate * l_refitted   = Bdau.next();

	HepAListIterator<BtaCandidate> DstarDau = Dstar_refitted->daughterIterator();
	BtaCandidate *  D_refitted = DstarDau.next();
	BtaCandidate * pis_refitted = DstarDau.next();	

	// assign User Data
	_B_p4         = _B_refitted  -> p4();
	_B_vtx_status = _B_refitted  -> decayVtx()->status();
	_B_chi2_prob  = probab( _B_refitted -> decayVtx()->nDof(),
				_B_refitted -> decayVtx()->chiSquared());

	_Dstar_mass      = Dstar_refitted -> mass();

	_Dstar_p4 =  Dstar_refitted -> p4();
	_l_p4     =  l_refitted     -> p4();
	_D_p4     =  D_refitted     -> p4();
	_pis_p4   =  pis_refitted   -> p4();

	if( verbose() ){
		cout << "\n\n\n\nB tree:";
		BtaTreeNavigator::printTree( * _B_refitted );

		cout << "\n\n\n\nDstar:";
		BtaTreeNavigator::printTree( * Dstar_refitted );

		cout << "\n\n\n\nLep";
		BtaTreeNavigator::printTree( * l_refitted );

		cout << "\n\n\n\nD";
		BtaTreeNavigator::printTree( * D_refitted );

		cout << "\n\n\n\n";
		BtaTreeNavigator::printTree( *  pis_refitted);

	}


	_garbage.append( D_mass_constrained );
	_garbage.append( Dstar_placeholder  );
	_garbage.append( B_placeholder      );

#ifdef GENE_MEMORY_LEACK_TEST
	cout << "Heap Check:\t" << Heap.currentSize() 
	     << "\t" << Heap.deltaSize() << endl;
#endif

	return true;	
}

BtaCandidate *
BToDstarlnuXFilter::make_D_tree( void )
{
	BtaOpMakeTree combiner;
	
	/*******
	 *
	 *  D with geometrical constraints
	 *
	 **/

	// Trees to constrain
	BtaCandidate *D_placeholder;
	BtaCandidate *D_composite_daughter( NULL ); // pi0 or Ks	
	HepAList<BtaCandidate> D_constrained_daughters;

	// Iterate over the unconstained daughters
	BtaCandidate * the_D_daughter;
	HepAListIterator<BtaCandidate> D_daughters=_theD->daughterIterator();
	while( ( the_D_daughter = (BtaCandidate *) D_daughters.next() ) )
	{
		switch ( the_D_daughter -> pdtEntry() -> lundId() ){
			// if they are composites applies mass constraints
		case PdtLund::K_S0 :
		case PdtLund::pi0  :
		{
			if ( ! the_D_daughter -> isComposite() ){			
				// Then we have a merged pi0
				D_constrained_daughters.append( the_D_daughter );				
				continue;
			}

			if( verbose() ){
				cout << "Composite D Daughter:\n" << endl;
				BtaTreeNavigator::printTree( * the_D_daughter);
			}

			HepAListIterator<BtaCandidate> tmpIterator( 
				the_D_daughter -> daughterIterator() );
			D_composite_daughter = 
				combiner.createFromList ( tmpIterator );
					
			// Lund Type 
			D_composite_daughter->setType( 
				the_D_daughter->pdtEntry() );
			// constraints
			setGeoConstraint      ( *D_composite_daughter ); 
			setMomentumConstraint ( *D_composite_daughter ); 
			setMassConstraint     ( *D_composite_daughter ); 
			D_constrained_daughters.append( D_composite_daughter );
			// Will be Cleaned-up at the next event: 
			_garbage.append( D_composite_daughter );

		}
			break;
		default:
			D_constrained_daughters.append( the_D_daughter );
		}
			
	}


	HepAListIterator<BtaCandidate> tmp_iterator( D_constrained_daughters );
	D_placeholder = combiner.createFromList ( tmp_iterator );
		
	// Lund Type 
	D_placeholder->setType ( _theD->pdtEntry() );

	// constraints
	setGeoConstraint      ( *D_placeholder ); 
	setMomentumConstraint ( *D_placeholder ); 


	return D_placeholder;
}

bool
BToDstarlnuXFilter::cos_theta_BY( void )
{
	static const double massB0_PDG = Pdt::lookup("B0")->mass();
	// "Effective" B energy used as Reference Point
	// to rescale cos Theta BY for off-peak data. 
	// 10.578210106 GeV is the mean Sqrt(s)  for Run1-4
	static const double energyBCMS_OffPeak_RP = 10.578210106 / 2;

	double energyB_CMS = _p4CM.m() / 2.;
	double massB0 = massB0_PDG;
	// check if it's off-peak data
	if (energyB_CMS < massB0) 
	  massB0 *= energyB_CMS /energyBCMS_OffPeak_RP;
	double massB0sqr = massB0 * massB0;
	double pB0= sqrt( energyB_CMS * energyB_CMS - massB0sqr);
 
	HepLorentzVector hadLepP4CMS= _B_refitted->p4();
	hadLepP4CMS.boost( _CM_frame ); 

	double hadLepMass  = hadLepP4CMS.m();
	double hadLepE     = hadLepP4CMS.e();
	double hadLepP     = hadLepP4CMS.rho();

 
	_cos_theta_BY =  - (massB0sqr + hadLepMass*hadLepMass - 
			   2 * energyB_CMS * hadLepE)/
               		   (2*pB0*hadLepP);

	return true;
   
}
