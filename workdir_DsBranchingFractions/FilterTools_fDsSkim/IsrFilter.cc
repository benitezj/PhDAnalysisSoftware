
//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: IsrFilter.cc,v 1.14 2006/11/23 19:05:02 desilva Exp $
//
// Description:
//	Class IsrFilter
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// History (add to end):
//      Joe Izen  ~Sep, 2000 - creation
//      Joe Izen   Oct, 2001 - bug fix to IsrNoGamma, add IsrLooseGamma, IsrRadBhabha
//
// Author List:
//	Joe Izen    	                Original Author
//
// Copyright Information:
//	Copyright (C) 2001		University of Texas at Dallas
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "FilterTools/IsrFilter.hh"

//-------------
// C Headers --
//-------------
extern "C" {
}

//---------------
// C++ Headers --
//---------------

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "CLHEP/Alist/AList.h"
#include "CLHEP/Alist/AIterator.h"
#include "AbsEventTag/AbsEventTag.hh"
#include "AbsEnv/AbsEnv.hh"
#include "AbsEvent/AbsEvent.hh"
#include "AbsEvent/AbsEventID.hh"
#include "GenEnv/GenEnv.hh"
#include "ErrLogger/ErrLog.hh"
#include "HepTuple/Tuple.h"
#include "HepTuple/TupleManager.h"
#include "Beta/EventInfo.hh"
#include "BetaCoreTools/BtaBooster.hh"
#include "CLHEP/ThreeVector.h"
#include "CLHEP/LorentzVector.h"

#include "OdfCommon/odfTime.hh"
#include "EidData/EidEventTriplet.hh"

#include "PepEnv/PepEnv.hh"
#include "PepEnvData/PepBeams.hh"
using std::cout;
using std::endl;

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------
IsrFilter::IsrFilter( const char* const theName, 
		      const char* const theDescription )
  : TagFilterModule( theName, theDescription )
     //     , _eventInfoList("eventInfoList",      this,  "Default") //from conditionsDB - for cmFrame
     , _doNtuple(           "doNtuple",            this,      false) //tcl on/off switch for hists
     , _enableGammaISR(     "enableGammaISR",      this,      false)
     , _enableNoGammaISR(   "enableNoGammaISR",    this,      false)
     , _enableLooseGammaISR("enableLooseGammaISR", this,      false)
     , _enableRadBhabha(    "enableRadBhabha",     this,      false)
     , _useRealBoost(       "useRealBoost",        this,       true)//NG
     , _nTracksMin(         "nTracksMin",          this,          3)//G NG
     , _nTracksMinL(        "nTracksMinL",         this,          2)//LG RBB
     , _eGamMin(            "eGamMin",             this,         2.8)//G NG
     , _eGamMinL(           "eGamMinL",            this,         2.)//LG RBB
     , _nTracksMax(         "nTracksMax",          this,         99)//NG
     , _pMissCosPlusMin(    "pMissCosPlusMin",     this,       0.70)//NG
     , _pMissCosMinusMax(   "pMissCosMinusMax",    this,      -0.80)//NG
     , _massTotalMax(       "massTotalMax",        this,         6.)//NG
     , _eTotalMax(          "eTotalMax",           this,        14.)//LG RBB
     , _eoverp1Max(         "eoverp1Max",          this,        0.9)//LG RBB
     , _eoverp2Max(         "eoverp2Max",          this,        0.9)//LG RBB
     , _labPx(              "labPx",               this,  -0.118806)//NG
     , _labPy(              "labPy",               this,-0.00118391)//NG
     , _labPz(              "labPz",               this,    5.85775)//NG
     , _labE (              "labE",                this,    12.0923)//NG
     , _prescaleISR(        "prescaleISR",         this,          1)//G NG LG
     , _prescaleRBB(        "prescaleRBB",         this,         20)//RBB
     , _rotBoostLabtoCM(0)
     , _oldLabFourMomGblEnv(0)
     , _nPassedNG(0)
     , _nPassedG(0)
     , _nPassedLG(0)
     , _nPassedGLG(0)
     , _nPassedRBB(0)
     , _nPassedISR(0)
     , _nPrescaledISR(0)
     , _nPrescaledRBB(0)
     , _nPassed(0)
     , _evtNum(0)
     , _nthisrun(0)
     , _thisrun(0)
     , _lastrun(-999)
     , _warn(0)
{
  //  commands()->append(& _eventInfoList);
  commands()->append(& _doNtuple);
  commands()->append(& _enableGammaISR);
  commands()->append(& _enableNoGammaISR);
  commands()->append(& _enableLooseGammaISR);
  commands()->append(& _enableRadBhabha);
  commands()->append(& _useRealBoost);
  commands()->append(& _nTracksMin);
  commands()->append(& _nTracksMinL);
  commands()->append(& _eGamMin);
  commands()->append(& _eGamMinL);
  commands()->append(& _nTracksMax);
  commands()->append(& _pMissCosPlusMin);
  commands()->append(& _pMissCosMinusMax);
  commands()->append(& _massTotalMax);
  commands()->append(& _eTotalMax);
  commands()->append(& _eoverp1Max);
  commands()->append(& _eoverp2Max);
  commands()->append(& _labPx);
  commands()->append(& _labPy);
  commands()->append(& _labPz);
  commands()->append(& _labE);
  commands()->append(& _eGamMinL);
  commands()->append(& _prescaleISR);
  commands()->append(& _prescaleRBB);

  //allocate memory for _rotBoostLabtoCM to use
  _rotBoostLabtoCM = new HepLorentzRotation( );
}

//--------------
// Destructor --
//--------------
IsrFilter::~IsrFilter()
{
  delete _rotBoostLabtoCM;
}

//-------------
// Methods   --
//-------------

    
//-------------
// Operators --
//-------------
    
//-------------
// Selectors --
//-------------
    
//-------------
// Modifiers --
//-------------
AppResult
IsrFilter::beginJob( AbsEvent* anEvent )
{
  if ( _verbose.value() ) cout << name() << ": Begin Job." << endl;
  
  if (_doNtuple.value()) {
    HepTupleManager* manager = gblEnv->getGen()->ntupleManager();
    assert(manager != 0);
    
    _IsrNtuple = manager->ntuple("ISR Ntuple");   
  }
  
  //set up to boost from lab to cm with default values
  HepLorentzVector labFourMom(_labPx.value(), _labPy.value(), _labPz.value(), _labE.value());

  //Get the boost to the CM frame. 
  BtaCandidate theCM(labFourMom , 0 );
  BtaBooster theCmBooster( &theCM );
  
  //extract the rotation/boost from the btaBooster so we can apply
  //the boost directly to HepLorentzVectors.
  *_rotBoostLabtoCM = theCmBooster.rotateAndBoost() ;

  HepLorentzVector cmFourMom( *_rotBoostLabtoCM * labFourMom);
  if (_enableNoGammaISR.value()){
    if (!_useRealBoost.value()) cout << name()
				     << "::beginJob:  Using hardwired values of CM system for NoGammaISR filtering:" << endl 
				     << "\t labFourMom=" << labFourMom << endl
				     << "\t cmFourMom =" << cmFourMom
				     << endl;
    else cout << name()
	      << "::beginJob:  Use PepII GblEnv for CM system when  NoGammaISR filtering:" << endl;
  }
  return AppResult::OK;
}

AppResult
IsrFilter::event( AbsEvent* anEvent )
{
  _evtNum++;

  static const IfdStrKey keyAbsEventID("AbsEventID");  
  AbsEventID* eventID = Ifd<AbsEventID>::get(anEvent, keyAbsEventID );
  
  //If the run number has changed redo the boost
  _thisrun = (int)eventID->run();
  if (_lastrun != _thisrun) {
    _lastrun = _thisrun;
    _nthisrun = 0;
  }//if (_lastrun != _thisrun) 
  
  //get the current CMS energy from the pep environment 
  const PepBeams *_theBeams;
  if (_enableNoGammaISR.value()) {
    if (_useRealBoost.value()) {
      
      _theBeams = gblEnv->getPep()->pepBeams(); 
      
      if( _theBeams == 0){
	cout << name()
	     << ":  Did not find the current beam parameters in gblEnv for run "<< _thisrun << endl
	     << "\t Possibly a configuration error.  If _enableNoGammaISR and _useRealBoost are true" << endl
	     << "\t check that the PepII gblEnv database is loaded before " << name() << "in the path." << endl
	     << "\t Alternatively, consider using a default boost for faster performance by setting useRealBoost false." << endl
	     << name() <<":  Asserting!"
	     << endl; 
	assert(0);
      }
      
      //Hep3Vector labTocmBoostVector( -_theBeams->boostCMtoLab() );//use labTocmBoostVectorfor boosting from lab to cm Frame
      HepLorentzVector labFourMomGblEnv(_theBeams->total4Momentum());
      if (_oldLabFourMomGblEnv != labFourMomGblEnv){
	//Get the boost to the CM frame. 
	BtaCandidate theGblEnvCM(labFourMomGblEnv , 0 );
	BtaBooster theGblEnvCmBooster( &theGblEnvCM );
	
	//extract the rotation/boost from the btaBooster so we can apply
	//the boost directly to HepLorentzVectors.
	*_rotBoostLabtoCM = theGblEnvCmBooster.rotateAndBoost() ;

	if ( _verbose.value() ) 
	  cout << name()
	       << ": Loading New CM Info  Run "               << _thisrun 
	       << ", events this run: "  << _nthisrun 
	       << ":  labFourMom = " << labFourMomGblEnv 
	       << endl;
	
	_oldLabFourMomGblEnv = labFourMomGblEnv;
      }//if (_oldLabFourMomGblEnv != labFourMomGblEnv)
      
    }//if (_useRealBoost.value()) 
  }//if (_enableNoGammaISR.value()) 
  
    //}//if (_lastrun != _thisrun) 
  
  _nthisrun++;
  
  if ( _verbose.value() ) {
    if ((_evtNum%1000)==0 
	|| (_evtNum<11                      )
	|| (_evtNum<101 && (_evtNum%10)==0  )
	|| (_evtNum<1001 && (_evtNum%100)==0 )
	|| (_evtNum<10001 && (_evtNum%1000)==0 )
	|| (_nthisrun==1)  ){
      
      odfTime timeStamp = eventID->eventIdTriplet().timeStamp();
      cout << name() << "::event processing event number " << _evtNum 
	   << "  Run " << _thisrun
	   << "  evID: " << (int)timeStamp.binary().upper << (int)timeStamp.binary().lower
	   << "  Events this run " << _nthisrun << endl;
    }
  }
 
  bool bgfmultihadron(false);
  int nTracks(0), nGoodTrkLoose(0), totChargeLoose(0);

  float eTotal(0.f),    eNeutralMag(0.f), eNeutralCosTh(0.f), eNeutralPhi(0.f);
  float pTotalMag(0.f), pTotalCosTh(0.f), pTotalPhi(0.f),   pTotalMass(0.f);
  float e1Mag(0.f),     p1Mag(0.f),       p2Mag(0.f);
  float p1EmcCandEnergy(0.f), p2EmcCandEnergy(0.f);

  HepLorentzVector QtotalLab, QtotalChargedLab, QtotalNeutralLab; 
  HepLorentzVector QtotalCm,  QtotalChargedCm,  QtotalNeutralCm;  


  // Build the tag accessor using the base class
  TagFilterModule::event( anEvent );
  bool status(false);     //make sure tag bits are healthy
  bool passedG(false);    //if true, this event is an ISR candidate with a Gamma
  bool passedNG(false);   //if true, this event is an ISR candidate with NoGamma
  bool passedLG(false);   //if true, this event is an ISR candidate with a GammaLoose
  bool passedGLG(false);  //if true, this event is an ISR candidate with a GammaLoose and/pr a Gamma
  bool passedISR(false);  //if true, this event is an ISR candidate with any of the above
  bool passedRBB(false);  //if true, this event is a Rad Bhabha candidate
  bool passed(false);     //if true, filter passes the event

  if ( 0 != tag( ) ) {

    //tag words used by all Filters
    status  = tag( )->getInt  ( nGoodTrkLoose,   "nGoodTrkLoose" );
    if ( status ){ 

      //charm filters first
      if (_enableNoGammaISR.value() || _doNtuple.value() || _enableGammaISR.value() ){ 
	
	if ( nGoodTrkLoose >= _nTracksMin.value() ) {
	  
	  status &= tag( )->getFloat( e1Mag,       "e1Mag");              //CM quantity  see EventTagTools/TagStoreCalorInfo.cc
	  
	  if (status) {
	    if (e1Mag < _eGamMin.value()){ //no ISR gamma 
	      
	      //if there is no high energy photon, consider checking the NoGammaISR criteria.
	      //Since ntuple will require the same quantities, proceed if n-tuple is requested too 
	      
	      if (_enableNoGammaISR.value() || _doNtuple.value() ){           //Skip if not looking for NoGammaISR and not making ntuples
		HepLorentzVector rest1;       //Do this here to avoid a scope problem
		//		float mrest;                  //Do this here to avoid a scope problem
		//		float erest;                  //Do this here to avoid a scope problem
		//		float e1Cos = -1.2;           //Do this here to avoid a scope problem
		//Build a HepLorentzVector for the highest energy photons in CM frame
		HepLorentzVector cm_e1(0);    //Do this here to avoid a scope problem
		//Everything minus highest energy gamma
		HepLorentzVector cm_rest(0);  //Do this here to avoid a scope problem
		
		status &= tag( )->getFloat( pTotalMag,   "pTotalMag");        //Lab quantity  see EventTagTools/TagStoreTrackInfo.cc
		status &= tag( )->getFloat( pTotalCosTh, "pTotalCosTh");      //Lab quantity  see EventTagTools/TagStoreTrackInfo.cc
		status &= tag( )->getFloat( pTotalPhi,   "pTotalPhi");        //Lab quantity  see EventTagTools/TagStoreTrackInfo.cc
		status &= tag( )->getFloat( pTotalMass,  "pTotalMass");       //Lab quantity  see EventTagTools/TagStoreTrackInfo.cc
		
		status &= tag( )->getFloat( eTotal,         "eTotal" );       //Lab quantity  see EventTagTools/TagStoreCalorInfo.cc
		status &= tag( )->getFloat( eNeutralMag,    "eNeutralMag" );  //Lab quantity  see EventTagTools/TagStoreCalorInfo.cc 
		status &= tag( )->getFloat( eNeutralCosTh,  "eNeutralCosTh" );//Lab quantity  see EventTagTools/TagStoreCalorInfo.cc   
		status &= tag( )->getFloat( eNeutralPhi,    "eNeutralPhi" );  //Lab quantity  see EventTagTools/TagStoreCalorInfo.cc      
		
		
		if (_doNtuple.value()) { //only bother if need for ntuple
		  status &= tag( )->getInt(   totChargeLoose, "totChargeLoose");
		  status &= tag( )->getBool(  bgfmultihadron, "BGFMultiHadron" );
		}// if (_doNtuple.value()) 
		
		if ( status ){ 
		  float pTotalE( sqrt (pTotalMag*pTotalMag + pTotalMass*pTotalMass) );
		  
		  //Build a HepLorentzVector for total charged particle sum in lab frame
		  //Note that these are the charged particles in the TaggingList with mass assignments there
		  // 	QtotalChargedLab.setPx(pow( (1.-pTotalCosTh*pTotalCosTh), 0.5)*pTotalMag); //px = SinTh*pTotalMag
		  QtotalChargedLab.setPx(sqrt(1.-pTotalCosTh*pTotalCosTh) * pTotalMag);      //px = SinTh*pTotalMag
		  QtotalChargedLab.setPy(			       0.);                        //py dummy for now
		  QtotalChargedLab.setPz(			       pTotalCosTh*pTotalMag);     //pz
		  QtotalChargedLab.setE(			       pTotalE);                   //E
		  
		  QtotalChargedLab.setPhi(pTotalPhi); // this is how we set px and py 
		  
// 		  if ( _verbose.value() ) cout << name()
// 					       << "::Event:QtotalChargedLab=" << QtotalChargedLab
// 					       << endl;
		  
		  
		  //Build a HepLorentzVector for total neutral particle sum in lab frame
		  // eTotal is formed from both TaggingList and CalorNeutral
		  // It is the sum of the E component of the 4mom for charged (TaggingList)+
		  // The sum of the calorimeter energy for neutrals (CalorNeutral).  
		  // is computed in EventTagTools/TagStoreCalorInfo.cc
		  QtotalNeutralLab.setPx(pow( (1.-eNeutralCosTh*eNeutralCosTh), 0.5)*eNeutralMag);  //px = SinTh*eNeutralMag
		  QtotalNeutralLab.setPy(			       0.);                               //py dummy for now
		  QtotalNeutralLab.setPz(			       eNeutralCosTh*eNeutralMag);        //pz
		  QtotalNeutralLab.setE(			       eTotal-pTotalE);                   //E Subtract off portion from charged tracks.
		  QtotalNeutralLab.setPhi(eNeutralPhi); // this is how we set px and py
		  
		  //Add the charged sum and the neutral sum.
		  QtotalLab = QtotalChargedLab + QtotalNeutralLab ;
		  
		  if (QtotalLab.e() > 0.) QtotalCm = *_rotBoostLabtoCM * QtotalChargedLab;
		  else QtotalCm = QtotalLab;       //protect against an empty event
		  
		  if (_doNtuple.value()){
		    if (QtotalChargedLab.e() > 0.) QtotalChargedCm = *_rotBoostLabtoCM * QtotalChargedLab;
		    else QtotalChargedCm = QtotalChargedLab;       //protect against a pure neutral event
		    
		    if (QtotalNeutralLab.e() > 0.) QtotalNeutralCm = *_rotBoostLabtoCM * QtotalNeutralLab;
		    else QtotalNeutralCm = QtotalNeutralLab;       //protect against a pure charged event
		  }//if (_doNtuple.value())
		  
		  
// 		  if ( _verbose.value() ) {
// 		    cout << name() << "::Event:QtotalNeutralLab=" << QtotalNeutralLab << endl;
// 		    cout << name() << "::Event:QtotalChargedLab=" << QtotalChargedLab << endl;
// 		    cout << name() << "::Event:QtotalLab=       " << QtotalLab        << endl;
// 		    if (_doNtuple.value()){
// 		      cout << name() << "::Event:QtotalChargedCm=" << QtotalChargedCm << endl;
// 		      cout << name() << "::Event:QtotalNeutralCm=" << QtotalNeutralCm << endl;
// 		    }
// 		    cout << name() << "::Event:QtotalCm=        " << QtotalCm         << endl;
// 		  }//if ( _verbose.value() )
		  
		  
		  
		  
		  if (_enableNoGammaISR.value()){ 
		    
		    if( nGoodTrkLoose <= _nTracksMax.value() ){  //charged mult
		      
		      if (QtotalCm.m() <= _massTotalMax.value()){
			if (QtotalCm.cosTheta() <= _pMissCosMinusMax.value() ||//pmis is forward/backward
			    QtotalCm.cosTheta() >= _pMissCosPlusMin.value() ){
			  //		  if ( _verbose.value() ) cout << name() << ": ISR noGamma candidate found" << endl;

			  passedNG = true;	
			  
			  //	if ( _verbose.value() ){ 
			  //	  int ipassed(0);    if (passedNG)ipassed=1;
			  //	  int ibgfmultihadron(0);    if (bgfmultihadron)ibgfmultihadron=1;
			  //	  cout << name() 
			  //	       << "::Event: passed=" << ipassed
			  //	       << " bgfmh=" << ibgfmultihadron
			  //	       << " CosTheta: " 
			  //	       << totalCm.p3().cosTheta() << " should not be within "
			  //	       << _pMissCosiMinusMax.value() << " to "
			  //	       << _pMissCosPlusMin.value() << "   " 
			  //	       << " nGoodTrkLoose: "
			  //	       << _nTracksMin.value() << " <= "
			  //	       << nGoodTrkLoose  << " <= "
			  //	       << _nTracksMax.value() 
			  //	       << endl;
			  //	}
			  
			  
			  
			  
			}//if (QtotalCm.cosTheta() <= _pMissCosMinusMax.value() ||//pmis is forward/backward
		      }//if (QtotalCm.m() <= _massTotalMax.value()){
		    }//if( nGoodTrkLoose <= _nTracksMax.value() ){  //charged mult

// 		    if ( _verbose.value() ) cout << name() 
// 						 << " e1Mag="  << e1Mag 
// 						 << " nGTL="   << nGoodTrkLoose 
// 						 << " CmMass=" << QtotalCm.m() 
// 						 << " cosTh="  << QtotalCm.cosTheta()
// 						 << endl;
		    
		    
		    
		    
		  }//if (_enableNoGammaISR.value()) 
		  
		  //fill ntuple
		  if (_doNtuple.value()) {
		    _IsrNtuple->column("passedNG",       passedNG);
		    _IsrNtuple->column("bgfmultihadron", bgfmultihadron);
		    _IsrNtuple->column("ntracks",        nTracks);
		    _IsrNtuple->column("ngoodtrkloose",  nGoodTrkLoose);
		    _IsrNtuple->column("totchargeloose", totChargeLoose);
		    
		    _IsrNtuple->column("e1mag",       e1Mag);
		    
		    _IsrNtuple->column("ptotalmag",   pTotalMag);
		    _IsrNtuple->column("ptotalcosth", pTotalCosTh);
		    _IsrNtuple->column("ptotalphi",   pTotalPhi);
		    
		    _IsrNtuple->column("ptotalenergy",(float)QtotalChargedLab.e() );
		    _IsrNtuple->column("ptotalmass",  pTotalMass);
		    
		    _IsrNtuple->column("etotal",        eTotal);
		    _IsrNtuple->column("eneutralmag",   eNeutralMag);
		    _IsrNtuple->column("eneutracosth",  eNeutralCosTh);
		    _IsrNtuple->column("eneutralphi",   eNeutralPhi);
		    
		    _IsrNtuple->column("eneutralenergy",(float)QtotalNeutralLab.e() );
		    _IsrNtuple->column("eneutralmass",  (float)QtotalNeutralLab.m() );
		    
		    _IsrNtuple->column("totalchargedcmmag",   (float)QtotalChargedCm.rho()      );
		    _IsrNtuple->column("totalchargedcmcosth", (float)QtotalChargedCm.cosTheta() );
		    _IsrNtuple->column("totalchargedcmphi",   (float)QtotalChargedCm.phi()      );
		    _IsrNtuple->column("totalchargedcmenergy",(float)QtotalChargedCm.e()        );
		    _IsrNtuple->column("totalchargedcmmass",  (float)QtotalChargedCm.m()        );
		    
		    _IsrNtuple->column("totalneutralcmmag",   (float)QtotalNeutralCm.rho()      );
		    _IsrNtuple->column("totalneutralcmcosth", (float)QtotalNeutralCm.cosTheta() );
		    _IsrNtuple->column("totalneutralcmphi",   (float)QtotalNeutralCm.phi()      );
		    _IsrNtuple->column("totalneutralcmenergy",(float)QtotalNeutralCm.e()        );
		    _IsrNtuple->column("totalneutralcmmass",  (float)QtotalNeutralCm.m()        );
		    
		    _IsrNtuple->column("totalcmmag",   (float)QtotalCm.rho()      );
		    _IsrNtuple->column("totalcmcosth", (float)QtotalCm.cosTheta() );
		    _IsrNtuple->column("totalcmphi",   (float)QtotalCm.phi()      );
		    _IsrNtuple->column("totalcmenergy",(float)QtotalCm.e()        );
		    _IsrNtuple->column("totalcmmass",  (float)QtotalCm.m()        );
		    
		    _IsrNtuple->dumpData(); 
		  }//if (_doNtuple.value()) 
		  
		  
		}//if ( status )
	      }//if (_enableNoGammaISR.value() || _doNtuple.value() ){  Skip if not looking for NoGammaISR and not making ntuples
	    }//if (e1Mag < _eGamMin.value()){ //no ISR gamma 
	    
	    else { //ISR gamma spotted
	      if (_enableGammaISR.value()){ 
		//could add radBhabha rejection here someday
		passedG = true;
// 		if ( _verbose.value() ) cout << name()<< ": ISR gamma candidate found"  
// 					     << " e1Mag="  << e1Mag 
// 					     << " nGTL="   << nGoodTrkLoose 
// 					     << endl;
	      }//if (_enableGammaISR.value())
	      
	    }//else { //ISR gamma spotted
	    
	    
	  }// if (status)    from e1Mag
	  
	}//if ( nGoodTrkLoose >= _nTracksMin.value() ) 
      }//if (_enableNoGammaISR.value() || _doNtuple.value() || _enableGammaISR.value() )

      if (_enableLooseGammaISR.value() || _enableRadBhabha.value()){ 
	if ( nGoodTrkLoose >= _nTracksMinL.value() ) {
	  
	  status &= tag( )->getFloat( e1Mag,       "e1Mag");              //CM quantity  see EventTagTools/TagStoreCalorInfo.cc

	  if (status) {
	    if (e1Mag > _eGamMinL.value()){ //ISR gamma
	      
	      bool LumiGaGaEvent(false);
	      status &= tag()->getBool(LumiGaGaEvent,"LumiGaGaEvent");
	      //	      status &= tag()->getBool(LumiGaGaEvent,"BGFGammaGamma");
	      if (!status) {
		_warn++;
		if (_warn<31){
		  cout << name()
		       << ":  Did not find the ** LumiGaGaEvent ** tagbit for run "<< _thisrun << endl
		       << "\t Possibly a configuration error when _enableLooseGammaISR or _enableRadBhabhaISR are true." << endl
		       << "\t Old versions of OPR and some SP3 sets did not make this bit.  To remedy, place" << endl
		       << "\t EmcLumiSequence in your path before IsrFilter.  This message will only repeat a few times."
		       << endl; 
		}
	      }
	      else  {
		if (!LumiGaGaEvent){//   Skip all gamma gamma events 
		  
		  status &= tag( )->getFloat( eTotal,         "eTotal" );       //Lab quantity  see EventTagTools/TagStoreCalorInfo.cc
		  if (status) {
		    if (eTotal < _eTotalMax.value()){
		      
		      //passed all tests but Radiative Bhabha.  Now test for that
		      if (nGoodTrkLoose == 2) {
			bool LumiBaBaEvent(false);
			status &= tag()->getBool(LumiBaBaEvent,"LumiBaBaEvent");
			//			status &= tag()->getBool(LumiBaBaEvent,"DigiFRadiativeBhabha");
			if (!status) {
			  _warn++;
			  if (_warn<31){
			    cout << name()
				 << ":  Did not find the ** LumiBaBaEvent ** tagbit for run "<< _thisrun << endl
				 << "\t Possibly a configuration error when _enableLooseGammaISR or _enableRadBhabhaISR are true." << endl
				 << "\t Old versions of OPR and some SP3 sets did not make this bit.  To remedy, place" << endl
				 << "\t EmcLumiSequence in your path before IsrFilter.  This message will only repeat a few times"
				 << endl; 
			  }
			}
			else {
			  passedRBB = LumiBaBaEvent;
			  if (!LumiBaBaEvent){
			    
			    //test e/p for highest two tracks		    
			    status &= tag()->getFloat(p1Mag,"p1Mag");
			    status &= tag()->getFloat(p1EmcCandEnergy,"p1EmcCandEnergy");
			    if (status) {
			      if (p1Mag > 0.f){
				passedRBB = (p1EmcCandEnergy/p1Mag >_eoverp1Max.value());
			      }//if (p1Mag > 0.f){
			    }//if (status) from p1Mag,p1EmcCandEnergy			    

			    if (!passedRBB){
			      status &= tag()->getFloat(p2Mag,"p2Mag");
			      status &= tag()->getFloat(p2EmcCandEnergy,"p2EmcCandEnergy");
			      if (status) {
				if (p2Mag > 0.f){
				  passedRBB = (p2EmcCandEnergy/p2Mag >_eoverp2Max.value());
				}//if (p2Mag > 0.f){
			      }//if (status) from p2Mag,p2EmcCandEnergy
			    }//if (!passedRBB)
			  }//if (!LumiBaBaEvent){
			}//if (status)  from LumiBaBa
		      }//if (nGoodTrkLoose == 2) 
		      
		      //set passed vars here
		      if (_enableLooseGammaISR.value()) passedLG = !passedRBB;
		      if (!_enableRadBhabha.value()) passedRBB = false;

		    }//if (eTotal < eTotalMax)
		  }//if (status)  from eTotal
		}//if (!LumiGaGaEvent)
	      }//if (status)  from LumiGaGa
	    }//if (e1Mag > _eGamMinL.value()){ //ISR gamma
	    
	  }//if (status)  from e1Mag
	  
	}//if ( nGoodTrkLoose >= _nTracksMinL.value() )
	
      }//if (_enableLooseGammaISR.value() || _enableRadBhabha.value())
	      

      
    }//    if ( status ) from GoodTrkLoose
    if (!status) {
      _warn++;
      if (_warn<21){
	cout << name()
	     << ":  Some of the tag info in run "<< _thisrun << " was not available." << endl
	     << "\t Possibly a configuration error. For old data sets, early versions of OPR " << endl
	     << "\t may not have produced all the bits required for IsrFilter." << endl
	     << "\t This message will repeat a few times."
	     << endl; 
      }
    }
    
  }//  if ( 0 != tag( ) ) 
  
  if ( passedNG) {
    _nPassedNG++;
    passedISR = true;
  }//if ( passedNG) 
  
  if ( passedG) {
    _nPassedG++;
    passedISR = true;
    if ( passedLG) {
      passedGLG = true;
      _nPassedGLG++;
    }
  }//if ( passedG) 
  
  if ( passedLG) {
    _nPassedLG++;
    passedISR = true;
  }//if ( passedLG) 
  
  if (passedISR){
    _nPassedISR++;

    if ( _nPassedISR%_prescaleISR.value() == 0 ) {    // prescale
      _nPrescaledISR++;
      passed = true; 
    } 
    else 
      if ( _verbose.value() )
	cout << name() << " failed a good GammaISR/LooseGammaISR/NoGammaISR due to prescaling."<<endl;
  }
  
  
  if ( passedRBB) {
    _nPassedRBB++;

    if ( _nPassedRBB%_prescaleRBB.value() == 0 ) {
      _nPrescaledRBB++;
      passed = true; 
    } 
  }//if ( passedRBB) 
  
  if (passed) _nPassed++;
  setPassed( passed );
  
//   if ( _verbose.value() ){
//     char i('f'); if (passedNG ) i = 't';
//     char j('f'); if (passedG  ) j = 't';
//     char k('f'); if (passedLG ) k = 't';
//     char n('f'); if (passedGLG) n = 't';
//     char l('f'); if (passedRBB) l = 't';
//     char m('f'); if (passed   ) m = 't';
//     cout << name() 
//  	 << ": event "        << _evtNum 
//  	 << "  NG G LG GLG RBB Event:"
// 	 << " " << i 
// 	 << " " << j
// 	 << " " << k
// 	 << " " << n
//  	 << " " << l
//  	 << " " << m
//  	 << "   Sums:"
//  	 << " " << _nPassedNG
//  	 << " " << _nPassedG
//  	 << " " << _nPassedLG
//  	 << " " << _nPassedGLG
//  	 << " " << _nPassedRBB
//  	 << "  Passed after prescale: " << _nPassed
//  	 << endl;
//   }
  
  return AppResult::OK;
}

AppResult
IsrFilter::endJob( AbsEvent* anEvent )
{
  ErrMsg(routine)   << "\t endJob summary: " << endmsg;
  ErrMsg(routine)   << "\t Events processed           : " << _evtNum        << endmsg;
  if (_enableNoGammaISR.value())
    ErrMsg(routine) << "\t Events passed    NoGammaISR: " << _nPassedNG     << endmsg;
  if (_enableGammaISR.value())
    ErrMsg(routine) << "\t Events passed      GammaISR: " << _nPassedG      << endmsg;
  if (_enableLooseGammaISR.value())
    ErrMsg(routine) << "\t Events passed LooseGammaISR: " << _nPassedLG     << endmsg;
  if (_enableLooseGammaISR.value() && _enableGammaISR.value() )
    ErrMsg(routine) << "\t Events passed G and LG  ISR: " << _nPassedGLG    << endmsg;
  if (_enableLooseGammaISR.value() || _enableGammaISR.value()|| _enableNoGammaISR.value()  ){
    ErrMsg(routine) << "\t Events passed       Any ISR: " << _nPassedISR    << endmsg;
    ErrMsg(routine)	<< "\t Events prescaled    Any ISR: " << _nPrescaledISR << endmsg;
    ErrMsg(routine) << "\t ==================================="             << endmsg;
  }
  if (_enableRadBhabha.value()){
    ErrMsg(routine) << "\t Events passed    Rad Bhabha: " << _nPassedRBB    << endmsg;
    ErrMsg(routine) << "\t Events prescaled Rad BhaBha: " << _nPrescaledRBB << endmsg;
    ErrMsg(routine) << "\t ==================================="             << endmsg;
  }
  ErrMsg(routine)   << "\t Events passed         Total: " << _nPassed       << endmsg;
  
  return AppResult::OK;
}


void
IsrFilter::help(int argc, char** argv)
{
  cout << "This module filters for intial state radiation candidates"
       << "There are four sets of criteria which are or'ed and may be"
       << "turned on independently:\n"
       << "   enableGammaISR      set t\n"
       << "   enableLooseammaISR  set t\n"
       << "   enableNoGammaISR    set t\n"
       << "   enableRadBhabha     set t\n\n"
       << "The enableGammaISR criteria requires a gamma be found.\n"
       << "It is designed for an ISR charm meson analysis. Parameters are:\n"
       << "   nTracksMin          set    3  (min number of nGoodTrkLoose)"
       << "   eGamMin             set    3. (min value of e1Mag, i.e. highest energy gamma, CM Frame)"
       << "The enableLooseGammaISR criteria requires a gamma be found.\n\n"
       << "It is designed for an ISR light meson analysis. Parameters are:\n\n"
       << "   nTracksMinL         set    2  (min number of nGoodTrkLoose)"
       << "   eGamMinL            set    2. (min value of e1Mag, i.e. highest energy gamma, CM Frame)"
       << "   eTotalMax           set   14. (total visibe energy, Lab frame)"
       << "   eoverp1Max          set  0.9  (max EMC energy/Momentum, Lab frame for 1st highest momentum track)"
       << "   eoverp2Max          set  0.9  (max EMC energy/Momentum, Lab frame for 2nd highest momentum track)"
       << "The enableNoGammaISR criteria finds candidates with the ISR gamma lost down the beampipe:\n\n"
       << "   nTracksMin          set    3  (min number of nGoodTrkLoose)"
       << "   nTracksMax          set   99  (max number of nGoodTrkLoose)"
       << "   eGamMin             set    3. (min value of e1Mag. Here, all photons must be less energetic)"
       << "   massTotalMax        set    6. (Visible mass. ISR charm near threshold is typ 3.7 - 4.3 GeV with small, high tail)"
       << "   pMissCosPlusMin     set  0.7  (Cos of pMiss greater than this value.  Or with pMissCosMinusMax)"
       << "   pMissCosMinusMax    set -0.8  (Cos of pMiss less than this value)\n\n"
       << "   doNtuple            set    f  (makes an Ntuple.  Turn off for production)\n\n"
       << "The enableRadBhabha selects and prescales radiative Bhabha's rejected by enableLooseGammaIsr:\n\n"
       << "   prescaleRBB         set   20  (Prescale factor) for Radiative Bhabhas"
       << "General controls are:\n\n"
       << "   prescale         set       1 (Prescale factor for GammaISR, NoGammaISR, LooseGammaISR)"
       << endl;  
}

//		-----------------------------------------------
// 		-- Static Data & Function Member Definitions --
//		-----------------------------------------------

//		-------------------------------------------
// 		-- Protected Function Member Definitions --
//		-------------------------------------------

//		-----------------------------------------
// 		-- Private Function Member Definitions --
//		-----------------------------------------

//		-----------------------------------
// 		-- Internal Function Definitions --
//		-----------------------------------
//

// clone
//
AppModule* 
IsrFilter::clone(const char* cloneName)
{
  return new IsrFilter(cloneName, "clone of IsrFilter");
}
