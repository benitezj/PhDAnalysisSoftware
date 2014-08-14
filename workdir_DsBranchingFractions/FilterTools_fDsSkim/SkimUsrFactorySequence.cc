//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: SkimUsrFactorySequence.cc,v 1.7 2006/07/08 04:39:54 cklae Exp $
//
// Description:
//	Sequence SkimUsrFactorySequence
//
//      This sequence registers user selection and user calculators.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Frank Winklmeier
//
// Copyright Information:
//	Copyright (C) 2005	  Colorado State University
//
//--------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "FilterTools/SkimUsrFactorySequence.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "Framework/AppUserBuild.hh"
#include "SimpleComposition/SmpSelectCand.hh"
#include "SimpleComposition/SmpSelectionFactory.hh"
#include "UsrTools/UsrVarFactory.hh"
#include "UsrTools/UsrCalcCand.hh"
#include "UsrTools/UsrCalcEvent.hh"

//-----------------------------------------
// include files for user filter modules --
//-----------------------------------------
#include "FilterTools/BToXsllCalcMHadsll.hh"
#include "FilterTools/BToXsllCalcLepVtx.hh"
#include "FilterTools/BToXsllCalcMll.hh"
#include "FilterTools/BToXsllCalcSignal.hh"
#include "FilterTools/BToXsllCalcLep1Doca.hh"
#include "FilterTools/BToXsllCalcLep2Doca.hh"
#include "FilterTools/BToXsllCalcBVars.hh"
#include "FilterTools/BToXsllCalcGammaConv.hh"

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//----------------
// Constructors --
//----------------

AppSequence* SkimUsrFactorySequence(AppUserBuild* forWho)
{

  //----------------------------------------------
  // Registration of SimpleComposition selections
  //----------------------------------------------

  SmpSelectionFactory::instance().registerSelection
    ("mHad", SmpSelectCand<BToXsllCalcMHadsll>::createObject);

  //SmpSelectionFactory::instance().registerSelection
  //("isGammaConv", SmpSelectCand<BToXsllCalcGammaConv>::createObject);
  

  //----------------------------------
  // Registration of user calculators
  //----------------------------------

  UsrVarFactory::addCalcVar
    ("mHad", UsrCalcCand<BToXsllCalcMHadsll, float>::instance);

  UsrVarFactory::addCalcVar
    ("isGammaConv", UsrCalcCand<BToXsllCalcGammaConv, int>::instance);
  
  UsrVarFactory::addCalcVar
    ("mll", UsrCalcCand<BToXsllCalcMll, float>::instance);
  
  UsrVarFactory::addCalcVar
    ("lepVtxProb", UsrCalcCand<BToXsllCalcLepVtx, double>::instance);
  
  UsrVarFactory::addCalcVar
    ("lep1Doca", UsrCalcCand<BToXsllCalcLep1Doca, Hep3Vector>::instance);
  
  UsrVarFactory::addCalcVar
    ("lep2Doca", UsrCalcCand<BToXsllCalcLep2Doca, Hep3Vector>::instance);

  UsrVarFactory::addCalcVar
    ("bVars", UsrCalcCand<BToXsllCalcBVars, HepLorentzVector>::instance);
  
  UsrVarFactory::addCalcVar
    ("isSignalish", UsrCalcEvent<BToXsllCalcSignal, int>::instance);
  
  return 0;
}
