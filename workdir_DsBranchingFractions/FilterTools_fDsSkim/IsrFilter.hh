//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id:
//
// Description:
//	Class IsrFilter. 
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

#ifndef ISRFILTER_HH
#define ISRFILTER_HH

//-------------
// C Headers --
//-------------
extern "C" {
}

//---------------
// C++ Headers --
//---------------

//----------------------
// Base Class Headers --
//----------------------
#include "AbsParm/AbsParmIfdStrKey.hh"
#include "TagModules/TagFilterModule.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "Framework/AbsParmGeneral.hh"
#include "CLHEP/LorentzVector.h"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
class HepTupleManager;
class HepTuple;
//class HepLorentzVector;
#include "CLHEP/Vector/LorentzRotation.h"

//		---------------------
// 		-- Class Interface --
//		---------------------

class IsrFilter : public TagFilterModule {

//--------------------
// Declarations     --
//--------------------

  // Typedefs, consts, and enums

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  IsrFilter( const char* const theName, 
			    const char* const theDescription );

  // Copy Constructor

  // Destructor
  virtual ~IsrFilter();

  // Operators

  // Selectors (const)

  // Modifiers
  virtual AppResult beginJob( AbsEvent* anEvent );
  virtual AppResult event   ( AbsEvent* anEvent );

  virtual AppResult endJob  ( AbsEvent* anEvent );
  virtual void help(int argc, char** argv);
  virtual AppModule* clone(const char* cloneName);

protected:

  // Helper functions

private:
  // Data members
  //  AbsParmIfdStrKey       _eventInfoList;
  AbsParmBool            _doNtuple;
  AbsParmBool            _enableGammaISR;      //pass ISR candidates with visible gamma
  AbsParmBool            _enableNoGammaISR;    //pass ISR candidates with no visible gamma
  AbsParmBool            _enableLooseGammaISR; //pass ISR candidates with visible gamma for Noncharm ISR analysis
  AbsParmBool            _enableRadBhabha;     //pass Radiative Bhabhas rejected by enableLooseGammaISR

  AbsParmBool            _useRealBoost;     //when used as a tag filter, might not want to spend the time to 
                                            //read the boost in from the gblEnv DB
  

  AbsParmGeneral<int>    _nTracksMin;       //for GammaISR, NoGammaISR
  AbsParmGeneral<int>    _nTracksMinL;      //for LooseGammaISR
  AbsParmGeneral<double> _eGamMin;          //for GammaISR, NoGammaIsr  Min Energy for an ISR photon
  AbsParmGeneral<double> _eGamMinL;         //for LooseGammaISR         Min Energy for an ISR photon

  AbsParmGeneral<int>    _nTracksMax;       //for NoGammaISR  
  AbsParmGeneral<double> _pMissCosPlusMin;  //for NoGammaISR  limits for CosTh 
  AbsParmGeneral<double> _pMissCosMinusMax; //for NoGammaISR  limits for CosTh 
  AbsParmGeneral<double> _massTotalMax;     //for NoGammaISR  Maximum visible Mass

  AbsParmGeneral<double> _eTotalMax;        //for LooseGammaISR only
  AbsParmGeneral<double> _eoverp1Max;       //for LooseGammaISR only
  AbsParmGeneral<double> _eoverp2Max;       //for LooseGammaISR only

  //Lab frame doesn't change much.  Use a fixed value so as to not waste time
  //loading this info
  AbsParmGeneral<double> _labPx;            //Px of colliding beams in lab frame
  AbsParmGeneral<double> _labPy;            //Py of colliding beams in lab frame
  AbsParmGeneral<double> _labPz;            //Pz of colliding beams in lab frame
  AbsParmGeneral<double> _labE;             //E of colliding beams in lab frame

  AbsParmGeneral<int>    _prescaleISR;         //prescale factor for ISR events of any kind
  AbsParmGeneral<int>    _prescaleRBB;         //prescale Rad Bhabhas 

  HepTuple*              _IsrNtuple;        //fill an ntuple
  HepLorentzRotation*    _rotBoostLabtoCM;  //lab to CM boost matrix
  HepLorentzVector       _oldLabFourMomGblEnv;

  int                    _nPassedNG;
  int                    _nPassedG;
  int                    _nPassedLG;
  int                    _nPassedGLG;  //count G and LG 
  int                    _nPassedRBB;
  int                    _nPassedISR;  //count G and/or LG and/or NG 
  int                    _nPrescaledISR;  
  int                    _nPrescaledRBB;  
  int                    _nPassed;     //total passed after prescale taken into account
  int                    _evtNum;
  int                    _nthisrun;
  int                    _thisrun;
  int                    _lastrun;
  int                    _warn;
}; 

#endif 












