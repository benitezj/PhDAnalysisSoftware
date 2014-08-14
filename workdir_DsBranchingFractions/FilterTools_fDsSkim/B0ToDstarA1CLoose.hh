//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: B0ToDstarA1CLoose.hh,v 1.1 2004/10/02 03:37:33 denardo Exp $
//
// Description:
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//
// Copyright Information:
//	Copyright (C) 2004      SLAC
//
//------------------------------------------------------------------------

#ifndef B0TODSTARA1CLOOSE_HH
#define B0TODSTARA1CLOOSE_HH

//-------------
// C Headers --
//-------------
extern "C" {
}

//----------------------
// Base Class Headers --
//----------------------
#include "TagModules/TagFilterModule.hh"
#include "Framework/AbsParmDouble.hh"
#include "AbsParm/AbsParmIfdStrKey.hh"
// #include "CLHEP/Alist/AList.h"
//		---------------------
// 		-- Class Interface --
//		---------------------

template<class T>
class HepAList;

template<class T>
class HepAListIterator;

class BtaCandidate;

class B0ToDstarA1CLoose : public TagFilterModule {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  B0ToDstarA1CLoose( const char* const theName, 
		     const char* const theDescription );

  // Destructor
  virtual ~B0ToDstarA1CLoose();

  // Modifiers
  virtual AppResult beginJob( AbsEvent* anEvent );
  virtual AppResult event( AbsEvent* anEvent );
  virtual AppResult endJob( AbsEvent* anEvent );
  virtual AppModule *clone( const char *cloneName );

private:
  typedef HepAList<BtaCandidate> BtaList;
  typedef HepAListIterator<BtaCandidate> BtaListIter;
  // Commands list
  AbsParmIfdStrKey _inputList;
  AbsParmDouble _maxR2;

};

#endif 
