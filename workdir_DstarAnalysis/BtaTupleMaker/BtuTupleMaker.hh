//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: BtuTupleMaker.hh,v 1.8 2006/10/06 16:55:25 desilva Exp $
//
// Description:
//      An utility module for physics analysis users to write out
//      an ntuple from a toplevel HepAList<BtaCandidate>.
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Chih-hsiang Cheng              Original Author
//
// Copyright Information:
//      Copyright (C) 2004      Lawrence Livermore National Laboratory
//
//---------------------------------------------------------------------------
#ifndef BTUTUPLEMAKER_HH
#define BTUTUPLEMAKER_HH

//----------------------
// Base Class Headers --
//----------------------
#include "Framework/AppModule.hh"

//------------------------------------
// Collaborating Class Header files --
//------------------------------------
#include "AbsParm/AbsParmIfdStrKey.hh"
#include "AbsParm/AbsParmVector.hh"
#include "Framework/AbsParmString.hh"
#include "Framework/AbsParmBool.hh"
#include "Framework/AbsParmGeneral.hh"

#include "PDT/PdtEntry.hh"
#include "Beta/BtaCandidate.hh"
#include "CLHEP/Alist/AList.h"
#include "ErrLogger/ErrLog.hh"
#include "ProxyDict/IfdKey.hh"
#include "PDT/Pdt.hh"
#include "BetaCoreTools/BtaMcAssoc.hh"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
class HepTuple;
template <class T> class HTValOrderedVector;
class BtuCandBlock;
class BtuEventBlock;
class BtuMCBlock;
class BtuTrkContentFiller;
class BtuGamContentFiller;
//		---------------------
// 		-- Class Interface --
//		---------------------
 
class BtuTupleMaker : public AppModule {

public:
  
  BtuTupleMaker( const char* const theName ,
		 const char* const theDescription );

  virtual ~BtuTupleMaker();

  virtual AppResult           beginJob( AbsEvent* anEvent );
  virtual AppResult           event   ( AbsEvent* anEvent );
  virtual AppResult           endJob  ( AbsEvent* anEvent );

  // clone
  virtual AppModule *clone( const char *cloneName ) {
    return new BtuTupleMaker( cloneName, "clone of BtuTupleMaker");
  }

protected:

private:

  AbsParmIfdStrKey _eventInfoList;
  AbsParmString    _eventID, _ntupleName;
  AbsParmIfdStrKey _listToDump;
  AbsParmIfdStrKey _mcListName, _truthAssociator;
  AbsParmBool _fillMC, _checkClones;
  AbsParmIfdStrKey _baseTrackList;
  AbsParmString _eventBlockContents, _mcBlockContents;
  AbsParmString _eventTagsBool, _eventTagsInt, _eventTagsFloat;
  AbsParmVector<std::string> _ntpBlockConfigs;
  AbsParmString _ntpBlockToTrk;
  AbsParmVector<std::string> _ntpBlockContents;
  AbsParmVector<std::string> _ntpAuxListContents;
  AbsParmVector<std::string> _fillAllCandsInList;
  AbsParmVector<std::string> _trkExtraContents, _gamExtraContents;
  AbsParmGeneral<int> _trkMaxCands;
  AbsParmString _trkBlockName;
  AbsParmBool _wantATrkBlock;
  AbsParmBool _writeEveryEvent, _partialDecayTreeOK;
  AbsParmString  _checkCloneBlocks;

  int _nevt;
  HepAList<BtaCandidate> *_mcList;
  BtaMcAssoc *_truthMap;
  HepTuple   *_tuple;
  std::map<std::string, BtuCandBlock* > _btucands;
  BtuEventBlock       *_btuevent;
  BtuMCBlock          *_btumc;
  BtuTrkContentFiller *_btuTrkCont;
  BtuGamContentFiller *_btuGamCont;

  std::vector< HTValOrderedVector<int>* > _dxLund; // daughter lund id
  std::vector< HTValOrderedVector<int>* > _dxIdx; // daughter index in its list

  std::string _gammaBlockName;

  // pdtEntry  ->  block name
  std::map<const PdtEntry*, std::string> _blockMap;

  // block name -> temporary HepAList for ntuple blocks
  std::map<std::string, HepAList<BtaCandidate>* > _ntplistMap;

  // block name -> auxiliary list names
  std::map<std::string, std::vector< std::string > > _ntpAuxlistMap;

  // block name -> max number of daughters
  std::map<std::string, int > _nDaughtersMap;

  // block name -> max number of candidates
  std::map<std::string, int > _nCandsMap;

  // block name -> whether to make a link to TRK block
  std::map<std::string, bool> _linkToTrkMap;

  // block name -> whether to check clones
  std::map<std::string, bool> _toCheckClones;

  std::vector<std::string> _orderedBlockNames;
  std::vector< HepAList<BtaCandidate>* > _ntplists;
  std::map<std::string, std::string> _allCandsLists;

  HepAList<BtaCandidate>* _trkList;


  HepAList<BtaCandidate>* locateList(AbsEvent* anEvent, const std::string& s);
  void configureTupleBlock();
  void configureCandBlock(AbsEvent *anEvent);
  void fillNtpCommonVars(AbsEvent *anEvent, EventInfo *info, std::string block,
			 std::string nprefix="n");
  int idxAndAppendCandToList(BtaCandidate* cand, std::string bl="", 
			     HepAList<BtaCandidate>* auxlist=0,
			     bool checkType= true);

  void ntpBlockConfigErr(std::string config);
  void checkBlockOrder();
  void clearNtupleLists();
  void destroyNtupleLists();

};

#endif
