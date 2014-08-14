//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: DstD0ToKsHpHmSkim.cc,v 1.3 2004/10/05 05:31:23 lockman Exp $
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

#include "BaBar/BaBar.hh"
#include "FilterTools/DstD0ToKsHpHmSkim.hh"

///////////////
#include "CLHEP/Alist/AList.h"
#include "CLHEP/Alist/AIterator.h"
#include "CLHEP/Matrix/Vector.h"
#include "CLHEP/Vector/LorentzVector.h"
#include "AbsEnv/AbsEnv.hh"
#include "AbsEvent/AbsEvent.hh"
#include "AbsEvent/getTmpAList.hh"
#include "ErrLogger/ErrLog.hh"
#include "AbsEventTag/AbsEventTag.hh"
#include "Beta/EventInfo.hh"
#include "ProxyDict/IfdStrKey.hh"

DstD0ToKsHpHmSkim::DstD0ToKsHpHmSkim( const char* const theName, 
			      const char* const theDescription )
  : TagFilterModule( theName, theDescription )
  , _outputListName1("outputList1", this, "DstarD0ToKShortPiPi")
  , _outputListName2("outputList2", this, "DstarD0ToKShortKK")
  , _ksListName("ksList", this, "DstD0ToKsHpHmKsDefault")
  , _slowPionListName("slowPionList", this, "DstD0ToKsHpHmSlowPions")
  , _D0ToKsPiPiListName("D0ToKsPiPiList", this, "D0ToKShortPiPi")
  , _D0ToKsKKListName("D0ToKsKKList", this, "D0ToKShortKK")
  , _DstPD0ToKsPiPiListName("DstPD0ToKsPiPiList", this, "DstarPlusD0ToKShortPiPi")
  , _DstMD0ToKsPiPiListName("DstMD0ToKsPiPiList", this, "DstarMinusD0ToKShortPiPi")
  , _DstPD0ToKsKKListName("DstPD0ToKsKKList", this, "DstarPlusD0ToKShortKK")
  , _DstMD0ToKsKKListName("DstMD0ToKsKKList", this, "DstarMinusD0ToKShortKK")

  , _nEventsRead(0)
  , _comboSize(5)
  , _combo1(0)
  , _combo2(0)
{
  // final lists
  commands()->append(&_outputListName1);
  commands()->append(&_outputListName2);

  // intermediate list
  commands()->append(&_ksListName);
  commands()->append(&_slowPionListName);
  commands()->append(&_D0ToKsPiPiListName);
  commands()->append(&_D0ToKsKKListName);
  commands()->append(&_DstPD0ToKsPiPiListName);
  commands()->append(&_DstMD0ToKsPiPiListName);
  commands()->append(&_DstPD0ToKsKKListName);
  commands()->append(&_DstMD0ToKsKKListName);

  _combo1 = new long[_comboSize];
  _combo2 = new long[_comboSize];

}

DstD0ToKsHpHmSkim::~DstD0ToKsHpHmSkim()
{
  delete [] _combo1;
  delete [] _combo2;
}

AppResult
DstD0ToKsHpHmSkim::beginJob( AbsEvent* anEvent )
{
  ErrMsg(routine)<<"begin filter Job"<<endmsg; 
  for (int i = 0; i < _comboSize; i++) _combo1[i] = 0;
  for (int i = 0; i < _comboSize; i++) _combo2[i] = 0;
  return AppResult::OK;
}

AppResult
DstD0ToKsHpHmSkim::event( AbsEvent* anEvent )
{
  _nEventsRead++;
  
  // fetch the output lists
  //
  HepAList<BtaCandidate>* outputList1 = fetchList(anEvent, _outputListName1);
  HepAList<BtaCandidate>* outputList2 = fetchList(anEvent, _outputListName2);

  if (!outputList1) ErrMsg(fatal) << "No output list " 
				  << _outputListName1.valueString().c_str()
				  << " found in event" << endmsg;

  if (!outputList2) ErrMsg(fatal) << "No output list " 
				  << _outputListName2.valueString().c_str()
				  << " found in event" << endmsg;

  bool accept = 
    (outputList1 != 0 && outputList1->length() > 0) ||
    (outputList2 != 0 && outputList2->length() > 0);

  {
    int PerEvent = outputList1->length();
    int maxs = _comboSize - 1;
    if (PerEvent > maxs ) PerEvent = maxs;
    _combo1[PerEvent]++;
  }

  {
    int PerEvent = outputList2->length();
    int maxs = _comboSize - 1;
    if (PerEvent > maxs ) PerEvent = maxs;
    _combo2[PerEvent]++;
  }

  setPassed(accept);
  return AppResult::OK;
}

AppResult
DstD0ToKsHpHmSkim::endJob( AbsEvent* anEvent )
{
  ErrMsg(routine) << endmsg;
  ErrMsg(routine) << " +++++++++++++++++++++++++++++++++++++++++" << endmsg;
  ErrMsg(routine) << " +++ No. events read:   " << _nEventsRead << endmsg;
  for (int i = 0; i < _comboSize; i++) 
    ErrMsg(routine) << " +++ Events with " << i << " " 
		    << _outputListName1.valueString() << " combinations: "
		    << _combo1[i] << endmsg;
  for (int i = 0; i < _comboSize; i++) 
    ErrMsg(routine) << " +++ Events with " << i << " " 
		    << _outputListName2.valueString() << " combinations: "
		    << _combo2[i] << endmsg;
  ErrMsg(routine) << " +++++++++++++++++++++++++++++++++++++++++" << endmsg;
  return AppResult::OK;
}

HepAList<BtaCandidate>* 
DstD0ToKsHpHmSkim::fetchList(AbsEvent* anEvent, const AbsParmIfdStrKey& k)
{
  return Ifd< HepAList<BtaCandidate> >::get(anEvent, k.value());
}
