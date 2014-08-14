//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: Jpsi_Psi2sToChChGammaFilter.cc,v 1.1 2007/05/25 23:38:59 echenard Exp $
//
// Description:
//	An example module that filters on the transient tag.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Paul Harrison and Theresa Champion - based on Original template
//	by Charles Young
//
// Copyright Information:
//	Copyright (C) 1998      SLAC
//	Copyright (C) 2000      Hedgehog concepts :-)
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "FilterTools/Jpsi_Psi2sToChChGammaFilter.hh"

//-------------
// C Headers --
//-------------
#include <assert.h>
extern "C" {
}

//---------------
// C++ Headers --
//---------------
#include "AbsEvent/AbsEvent.hh"
#include "AbsEvent/getTmpAList.hh"
#include "CLHEP/Alist/AList.h"
#include "CLHEP/Alist/AIterator.h"
#include "CLHEP/Alist/ConstAList.h"
#include "Beta/BtaCandidate.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

#include "AbsEnv/AbsEnv.hh"
#include "GenEnv/GenEnv.hh"
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
Jpsi_Psi2sToChChGammaFilter::Jpsi_Psi2sToChChGammaFilter( const char* const theName, 
			const char* const theDescription )
  : TagFilterModule( theName, theDescription )
  , _EOrig("EOrig",this, 6.)
  , _ESlope("ESlope",this, -1.)
  , _ThetaMin("ThetaMin",this, 1.)
{
  commands()->append(&_EOrig);
  commands()->append(&_ESlope);
  commands()->append(&_ThetaMin);
}
		

//--------------
// Destructor --
//--------------
Jpsi_Psi2sToChChGammaFilter::~Jpsi_Psi2sToChChGammaFilter()
{
}

//-------------
// Methods   --
//-------------

AppResult
Jpsi_Psi2sToChChGammaFilter::beginJob( AbsEvent*  anEvent )
{
       return AppResult::OK;
}


AppResult 
Jpsi_Psi2sToChChGammaFilter::event( AbsEvent* anEvent )
{

  
 //LOAD J/PSI ORIGINAL LIST
  HepAList<BtaCandidate> *Jpsi_piList = 
   Ifd<HepAList< BtaCandidate > >::get(anEvent,"JpsiToPiPiGamma");
  HepAList<BtaCandidate> *Jpsi_kList = 
   Ifd<HepAList< BtaCandidate > >::get(anEvent,"JpsiToKKGamma");
  HepAList<BtaCandidate> *Jpsi_pList = 
   Ifd<HepAList< BtaCandidate > >::get(anEvent,"JpsiToPPGamma");


 //LOAD PSI(2S) ORIGINAL LIST
  HepAList<BtaCandidate> *Psi2s_piList = 
   Ifd<HepAList< BtaCandidate > >::get(anEvent,"Psi2sToPiPiGamma");
  HepAList<BtaCandidate> *Psi2s_kList = 
   Ifd<HepAList< BtaCandidate > >::get(anEvent,"Psi2sToKKGamma");
  HepAList<BtaCandidate> *Psi2s_pList = 
   Ifd<HepAList< BtaCandidate > >::get(anEvent,"Psi2sToPPGamma");


 //FIRST, CHECK IF WE HAVE ANY KK OR PP CANDIDATE IN THE JPSI AND PSI2S LISTS
  if (Jpsi_kList->length()>0 || Psi2s_kList->length()>0){setPassed(true);return AppResult::OK;}
  if (Jpsi_pList->length()>0 || Psi2s_pList->length()>0){setPassed(true);return AppResult::OK;}


 //CHECK IF WE HAVE GOOD CANDIDATE IN THE JPSI PI LIST
  HepAListIterator<BtaCandidate> iterJpsipi( *Jpsi_piList );  
   while (BtaCandidate* cand = iterJpsipi())
   {
     double ValCut=_EOrig.value()+_ESlope.value()*cand->p3().theta(); 
     if (cand->energy() > ValCut || cand->p3().theta()> _ThetaMin.value()) 
     {
       setPassed(true);
       return AppResult::OK;
     }
   }
  

 //THEN, CHECK IF WE HAVE GOOD CANDIDATE IN THE PSI2S PI LIST
  HepAListIterator<BtaCandidate> iterPsi2s( *Psi2s_piList );  
   while (BtaCandidate* cand = iterPsi2s())
   {
     double ValCut=_EOrig.value()+_ESlope.value()*cand->p3().theta(); 
     if (cand->energy() > ValCut || cand->p3().theta()> _ThetaMin.value()) 
     {
       setPassed(true);
       return AppResult::OK;
     }
   }



 //WE HAVE NOTHING, SNIFF, RETURN FALSE
  setPassed( false );
  return AppResult::OK;

}



AppResult
Jpsi_Psi2sToChChGammaFilter::endJob( AbsEvent* anEvent ) {
  return AppResult::OK;
}


