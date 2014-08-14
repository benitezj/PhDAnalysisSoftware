//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: DstD0ToKsHpHmSkim.hh,v 1.3 2004/09/26 19:09:59 lockman Exp $
//
// Description:
//      module to select Dstar+->pi+D0, D0->K0s had1 had2 + cc 
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Bill Lockman                    Original Author
//
// Copyright Information:
//      Copyright (C) 2004              UC Santa Cruz
//
//------------------------------------------------------------------------

#ifndef _DstD0ToKsHpHmSkim_
#define _DstD0ToKsHpHmSkim_ 1

//----------------------
// Base Class Headers --
//----------------------
#include "CLHEP/Alist/AList.h"

#include "Beta/BtaCandidate.hh"
#include "AbsParm/AbsParmIfdStrKey.hh"
#include "TagModules/TagFilterModule.hh"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

class AbsEvent;
class EventInfo;

//		---------------------
// 		-- Class Interface --
//		---------------------
 

class DstD0ToKsHpHmSkim : public TagFilterModule {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  DstD0ToKsHpHmSkim ( const char* const theName, 
		      const char* const theDescription );

  // Destructor
  virtual ~DstD0ToKsHpHmSkim( );
  
  // Operations
  virtual AppResult           beginJob( AbsEvent* anEvent );
  virtual AppResult           event   ( AbsEvent* anEvent );
  virtual AppResult           endJob  ( AbsEvent* anEvent );

protected:
  AbsParmIfdStrKey _outputListName1;
  AbsParmIfdStrKey _outputListName2;
  AbsParmIfdStrKey _ksListName;
  AbsParmIfdStrKey _slowPionListName;
  AbsParmIfdStrKey _D0ToKsPiPiListName;
  AbsParmIfdStrKey _D0ToKsKKListName;
  AbsParmIfdStrKey _DstPD0ToKsPiPiListName;
  AbsParmIfdStrKey _DstMD0ToKsPiPiListName;
  AbsParmIfdStrKey _DstPD0ToKsKKListName;
  AbsParmIfdStrKey _DstMD0ToKsKKListName;

private:

  HepAList<BtaCandidate>* fetchList(AbsEvent* anEvent, 
				    const AbsParmIfdStrKey& lname);

  long _nEventsRead;
  const unsigned _comboSize;
  long* _combo1;
  long* _combo2;

};

#endif




