//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: BtuTupleMaker.cc,v 1.12 2006/03/29 19:59:40 chcheng Exp $
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
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

#include "BtaTupleMaker/BtuTupleMaker.hh"

//-------------
// C Headers --
//-------------
#include <assert.h>

//---------------
// C++ Headers --
//---------------
#include <math.h>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "AbsEnv/AbsEnv.hh"
#include "AbsEvent/AbsEvent.hh"
#include "BaBar/Constants.hh"
#include "Beta/EventInfo.hh"
#include "CLHEP/Alist/AIterator.h"
#include "ErrLogger/ErrLog.hh"
#include "GenEnv/GenEnv.hh"
#include "HepTuple/TupleManager.h"
#include "HepTuple/Tuple.h"
#include "ProxyDict/IfdStrKey.hh"
#include "ProxyDict/Ifd.hh"
#include "PDT/Pdt.hh"
#include "PDT/PdtEntry.hh"
#include "HepTuple/HTValOrderedVector.h"
#include "BbrStdUtils/Tokenize.hh"
#include "BbrStdUtils/String.hh"
#include "BtaTupleMaker/BtuCandBlock.hh"
#include "BtaTupleMaker/BtuEventBlock.hh"
#include "BtaTupleMaker/BtuMCBlock.hh"
#include "BtaTupleMaker/BtuTrkContentFiller.hh"
#include "BtaTupleMaker/BtuGamContentFiller.hh"
#include "BtaTupleMaker/BtuUtil.hh"
#include "BetaCoreTools/BtaPrintTree.hh"

using namespace babar::String;

//----------------
// Constructors --
//----------------
BtuTupleMaker::BtuTupleMaker(const char* const theName,
			     const char* const theDescription )
  : AppModule( theName, theDescription )
    , _eventInfoList("eventInfoList", this, "Default" )
    , _eventID("eventID", this, "AbsEventID")
    , _ntupleName("ntupleName", this, "myNtuple")
    , _listToDump("listToDump", this, "NULL")
    , _mcListName("mcList", this, "MCTruth")
    , _truthAssociator("truthAssociator", this, "GHit")
    , _fillMC("fillMC", this, true )
    , _checkClones("checkClones", this, true )
    , _baseTrackList("baseTrackList", this, "ChargedTracks")
    , _eventBlockContents("eventBlockContents", this, "EventID")
    , _mcBlockContents("mcBlockContents", this, "")
    , _eventTagsBool("eventTagsBool", this, "")
    , _eventTagsInt("eventTagsInt", this, "")
    , _eventTagsFloat("eventTagsFloat", this, "")
    , _ntpBlockConfigs("ntpBlockConfigs", this)
    , _ntpBlockToTrk("ntpBlockToTrk", this, "")
    , _ntpBlockContents("ntpBlockContents", this)
    , _ntpAuxListContents("ntpAuxListContents", this)
    , _fillAllCandsInList("fillAllCandsInList", this)
    , _trkExtraContents("trkExtraContents",this)
    , _gamExtraContents("gamExtraContents",this)
    , _trkMaxCands("trkMaxCands", this, 50 )
    , _trkBlockName("trkBlockName", this, "TRK" )
    , _wantATrkBlock("wantATrkBlock", this, false)
    , _writeEveryEvent("writeEveryEvent", this, false)
    , _partialDecayTreeOK("partialDecayTreeOK", this, false)
    , _checkCloneBlocks("checkCloneBlocks", this, "")
    , _nevt(0), _mcList(0), _truthMap(0)
    , _tuple(0)
    , _btuevent(0),_btumc(0),_btuTrkCont(0),_btuGamCont(0)
{
  commands()->append( &_eventInfoList );
  commands()->append( &_eventID);
  commands()->append( &_ntupleName);
  commands()->append( &_listToDump );
  commands()->append( &_mcListName);
  commands()->append( &_truthAssociator);
  commands()->append( &_fillMC);
  commands()->append( &_checkClones);
  commands()->append( &_baseTrackList );
  commands()->append( &_eventBlockContents);
  commands()->append( &_mcBlockContents);
  commands()->append( &_eventTagsBool);
  commands()->append( &_eventTagsInt);
  commands()->append( &_eventTagsFloat);
  commands()->append( &_ntpBlockConfigs );
  commands()->append( &_ntpBlockToTrk );
  commands()->append( &_ntpBlockContents );
  commands()->append( &_ntpAuxListContents);
  commands()->append( &_fillAllCandsInList );
  commands()->append( &_trkExtraContents );
  commands()->append( &_gamExtraContents );
  commands()->append( &_trkMaxCands );
  commands()->append( &_trkBlockName );
  commands()->append( &_wantATrkBlock);
  commands()->append( &_writeEveryEvent);
  commands()->append( &_partialDecayTreeOK);
  commands()->append( &_checkCloneBlocks);
}

//--------------
// Destructor --
//--------------

BtuTupleMaker::~BtuTupleMaker( )
{
  destroyNtupleLists();
  if (_btuevent) delete _btuevent;
  if (_btumc) delete _btumc;
  if (_btuTrkCont) delete _btuTrkCont;
  if (_btuGamCont) delete _btuGamCont;

}

//--------------
// Operations --
//--------------
AppResult
BtuTupleMaker::beginJob( AbsEvent *anEvent)
{
  ErrMsg(debugging) << name() << ": beginJob" << endmsg;
  return AppResult::OK;
}

AppResult
BtuTupleMaker::endJob( AbsEvent *anEvent)
{
  ErrMsg(debugging) << name() << ": endJob" << endmsg;
  return AppResult::OK;
}

AppResult
BtuTupleMaker::event( AbsEvent *anEvent )
{
  _nevt++;

  // Get event info
  HepAList<EventInfo>* eventInfoList= 
    Ifd< HepAList<EventInfo> >::get(anEvent, _eventInfoList.value() );
  EventInfo* eventInfo = eventInfoList->first();
  assert(eventInfo);
  
  // Get the track list
  _trkList= locateList(anEvent, _baseTrackList.valueString());
  
  // Get Truth info
  if ( _fillMC.value() ) {
    _truthMap= Ifd< BtaMcAssoc >::get( anEvent, _truthAssociator.value() );
    _mcList = BtuUtil::rearrangeMcList( Ifd< HepAList<BtaCandidate> >::get(anEvent, _mcListName.value()), false );

    if ( ! _truthMap && _nevt <= 10 ) 
      ErrMsg(warning) << "Truth map " << _truthAssociator.value()
		      << " does not exist! Make sure the key is correct, or "
		      << " make sure you are running MC collection, or"
		      << " set fillMC to false."
		      << " Otherwise you'll get empty MC block."
		      << endmsg;
    if ( ! _mcList && _nevt <= 10 ) 
      ErrMsg(warning) << "MC list " << _mcListName.value()
		      << " does not exist! Make sure the key is correct, or "
		      << " make sure you are running MC collection, or"
		      << " set fillMC to false."
		      << " Otherwise you'll get empty MC block."
		      << endmsg;
  }

  // ===>>> Book an ntuple and configure it at the first event.
  if ( _nevt==1 ) {
    ErrMsg(routine) << name() << ": Create an ntuple named " 
		    << _ntupleName.value() << endmsg;
    HepTupleManager* manager = gblEnv->getGen()->ntupleManager();
    assert(manager != 0);
    _tuple= manager->ntuple( _ntupleName.value() );

    ErrMsg(routine) << name() << ": configuring the ntuple" << endmsg;

    configureTupleBlock();

    // Initialize Event block
    _btuevent= new BtuEventBlock();
    _btuevent->init(_eventBlockContents.value(), _eventTagsBool.value(),
		    _eventTagsInt.value(), _eventTagsFloat.value(),
		    _eventID.value());
    // Initialize MC block
    _btumc= new BtuMCBlock();
    _btumc->init(_mcBlockContents.value());

    // Initialize Candidate blocks
    configureCandBlock(anEvent);
    
    // Extra TRK info
    _btuTrkCont= new BtuTrkContentFiller();
    _btuTrkCont->init(_trkBlockName.value(),"n", _nCandsMap[_trkBlockName.value()], _trkExtraContents);

    // Extra Gamma info
    if ( _gammaBlockName != "" ) {
      _btuGamCont= new BtuGamContentFiller();
      _btuGamCont->init(_gammaBlockName,"n", _nCandsMap[_gammaBlockName], _gamExtraContents);
    }
  } // end if _nevt==1


  // Get a list to dump to the ntuple
  HepAList<BtaCandidate>* thelist= locateList(anEvent, _listToDump.valueString());

  if ( !_writeEveryEvent.value() && thelist->length()==0 ) 
    return AppResult::OK;

  // If a certain block is requested to be filled with a certain list,
  // copy the candidates over to the temp list.
  std::map<std::string,std::string>::const_iterator siter= _allCandsLists.begin();
  while ( siter!= _allCandsLists.end()) {
    if ( _ntplistMap[siter->first] ) {
      HepAList<BtaCandidate>* tlist= locateList(anEvent, siter->second);
      HepAListIterator<BtaCandidate> tt(*tlist);
      BtaCandidate *cc;
      while ( (cc= tt()) ) _ntplistMap[siter->first]->append(cc);
    } else {
      ErrMsg(fatal) << "There is no block called " << siter->first
		    << " It is likely a configuration error." << endmsg;
    }
    siter++;
  }
  
  // Distribute the candidates to temporary lists for ntuple blocks
  //  (It allows different particle types in the list-to-dump. This process
  //   puts one particle type to one temporary list.)
  HepAListIterator<BtaCandidate> iter(*thelist);
  BtaCandidate *cand;
  while ( (cand= iter()) ) idxAndAppendCandToList(cand);


  //===>>> Start to fill ntuple <<<===

  // Fill Event block
  _btuevent->fillTupleColumns(_tuple, anEvent, eventInfo);

  // Fill MC block
  if ( _fillMC.value() )
    _btumc->fillTupleColumns(_tuple, _mcList, eventInfo);

  // Fill in ntuple block by block (from top level)
  std::vector<std::string>::const_iterator it= _orderedBlockNames.begin();
  while ( it!= _orderedBlockNames.end() ) {
    std::string block= *it;
    fillNtpCommonVars(anEvent, eventInfo, block);
    it++;
  }

  // Fill extra trk info 
  // (it will do nothing if _ntplistMap[_trkBlockName.value()]==0 )
  _btuTrkCont->fillNtpColumns(_tuple,anEvent,_ntplistMap[_trkBlockName.value()]);
 
  // Fill extra gamma info
  // (it will do nothing if _ntplistMap[_gammaBlockName]==0 )
  _btuGamCont->fillNtpColumns(_tuple,anEvent,_ntplistMap[_gammaBlockName]);

  // Dump data  
  _tuple->dumpData();

  // Clear the content of temporary lists
  clearNtupleLists();

  return AppResult::OK;
}


//---------------------------------------------------------------------------
void BtuTupleMaker::configureTupleBlock() {

  int maxMaxDaus(0);
  // build pdtname -> block name map
  for ( int i=0; i< _ntpBlockConfigs.value().size(); i++ ) {
    std::string config= _ntpBlockConfigs.value()[i];
    Tokenize lineToken(config);
    std::string pdtname, block, smaxDau, smaxCand;
    if ( (pdtname=  lineToken()).empty()) ntpBlockConfigErr(config);
    if ( (block=    lineToken()).empty()) ntpBlockConfigErr(config);
    if ( (smaxDau=  lineToken()).empty()) ntpBlockConfigErr(config);
    if ( (smaxCand= lineToken()).empty()) ntpBlockConfigErr(config);
    
    if (! Pdt::lookup(pdtname.c_str()) ) 
      ErrMsg(fatal) << "\"" << pdtname << "\":"
		    << " no such particle in the pdt table." << endmsg;

    if ( block== _trkBlockName.value() )
      ErrMsg(fatal) << "block name " << _trkBlockName.value() 
		    << " is reserved for charged tracks. "
		    << "Check your ntpBlockConfigs setting."  << endmsg;

    if ( _blockMap[Pdt::lookup(pdtname.c_str())] == "") {
      _blockMap[Pdt::lookup(pdtname.c_str())] = block;

      if ( _orderedBlockNames.size()== 0 ||
	   _orderedBlockNames[_orderedBlockNames.size()-1]!= block )
	_orderedBlockNames.push_back(block);
    } else 
      ErrMsg(warning) << pdtname << " or its charge conjugate has been used. "
		      << "Ignore second attemp." << endmsg;
    // add charge conjugate as well
    if ( _blockMap[Pdt::lookup(pdtname.c_str())->conjugate()] == "") 
      _blockMap[Pdt::lookup(pdtname.c_str())->conjugate()] = block;

    int maxDau, maxCand;
    sscanf(smaxDau.c_str(),"%d",&maxDau);
    sscanf(smaxCand.c_str(),"%d",&maxCand);
    if (! _nDaughtersMap[block] ) _nDaughtersMap[block] = maxDau;
    if (! _nCandsMap[block] ) _nCandsMap[block] = maxCand;    
    
    maxMaxDaus= (maxDau > maxMaxDaus) ? maxDau : maxMaxDaus;
  }
  
  bool needATrkBlock= false;
  // block name -> whether to link to TRK block
  Tokenize lineToken(_ntpBlockToTrk.value());
  std::string block;
  while ( !(block= lineToken()).empty() ) {
    if ( !_nCandsMap[block] ) 
      ErrMsg(fatal) << "The block name " << block << " in ntpBlockToTrk "
		    << " is not defined in ntpBlockConfigs." << endmsg;
    _linkToTrkMap[block]= true;
    needATrkBlock= true;
  }

  // remember gamma block name
  _gammaBlockName= _blockMap[Pdt::lookup("gamma")];

  checkBlockOrder();

  // Append a trk block
  if ( needATrkBlock || _wantATrkBlock.value() ) {
    _orderedBlockNames.push_back(_trkBlockName.value());
    _nDaughtersMap[_trkBlockName.value()]= 0;
    _nCandsMap[_trkBlockName.value()]= _trkMaxCands.value();
  }

  // Parse _fillAllCandsList and create a map:   block -> listname
  for ( int i=0; i< _fillAllCandsInList.value().size(); i++) {
    Tokenize tokens(_fillAllCandsInList.value()[i]);
    std::string bb,ll,junk;
    if ( (bb= tokens()).empty() ) 
      ErrMsg(fatal) << "Error fillAllCandsInList format -- "
		    << _fillAllCandsInList.value()[i] << endmsg;
    if ( (ll= tokens()).empty() ) 
      ErrMsg(fatal) << "Error fillAllCandsInList format -- "
		    << _fillAllCandsInList.value()[i] << endmsg;
    if ( !(junk= tokens()).empty() ) 
      ErrMsg(fatal) << "Error fillAllCandsInList format -- "
		    << _fillAllCandsInList.value()[i] << endmsg;

    _allCandsLists[bb]=ll;
  }


  // Create a HepAList per block
  for ( int i=0; i< _orderedBlockNames.size(); i++ ) {
    std::string block= _orderedBlockNames[i];
    HepAList<BtaCandidate> *tmplist= new HepAList<BtaCandidate>();
    _ntplistMap[block]= tmplist;
    _ntplists.push_back(tmplist);
  }

  // Allocate HTValOrderedVector for daughter lund id and index in list
  for ( int i=0; i< maxMaxDaus; i++ ) {
    _dxLund.push_back( new HTValOrderedVector<int>());
    _dxIdx.push_back( new HTValOrderedVector<int>());
  }

  // block name -> whether to check clones
  Tokenize lineToken2(_checkCloneBlocks.value());
  while ( !(block= lineToken2()).empty() ) {
    if ( !_nCandsMap[block] ) 
      ErrMsg(fatal) << "The block name " << block << " in checkCloneBlocks"
		    << " is not defined in ntpBlockConfigs." << endmsg;
    _toCheckClones[block]= true;
  }

}

//---------------------------------------------------------------------------
void BtuTupleMaker::configureCandBlock(AbsEvent *anEvent) {

  // Parse candidate block content key words
  std::map<std::string, std::string> candContMap;
  std::map<std::string, bool> configedBlock;
  for ( int i=0; i< _ntpBlockContents.value().size(); i++) {
    Tokenize lineToken( _ntpBlockContents.value()[i]);
    std::string block= lineToken(":");   BtuUtil::chopEnds(block);
    std::string keys= lineToken(":");    BtuUtil::chopEnds(keys);
    if ( block=="" ) 
      ErrMsg(fatal) << "ntpBlockContent must start with a block name "
		    << "followed by a \":\" . " << endmsg;
    
    if (configedBlock[block] )
      ErrMsg(fatal) << "ntpBlockContent for " << block << " has been set. "
                    << "Don't repeat." << endmsg;

    bool hasBlock=false;
    for ( int j=0; j< _orderedBlockNames.size(); j++) {
      if ( block== _orderedBlockNames[j] ) {
        hasBlock= true;
        break;
      }
    }
    if ( ! hasBlock )
      ErrMsg(fatal) << "Don't know about the block " << block
                    << " that you set in ntpBlockContent. "
                    << "It has to be set in ntpBlockConfigs." << endmsg;

    candContMap[block]= keys;
    configedBlock[block]= true;
  }

  // Parse candidate block auxiliary content key words
  std::map<std::string, std::vector< std::string > > auxContKeys, auxTags;
  std::map<std::string, bool> configedAux;
  for ( int i=0; i< _ntpAuxListContents.value().size(); i++) {
    Tokenize lineToken( _ntpAuxListContents.value()[i]);
    std::string block= lineToken(":");       BtuUtil::chopEnds(block);
    std::string listname= lineToken(":");    BtuUtil::chopEnds(listname);
    std::string tag= lineToken(":");         BtuUtil::chopEnds(tag);
    std::string keys= lineToken(":");        BtuUtil::chopEnds(keys);
    std::string blocklist= block+"-"+listname;

    if ( block=="" || listname=="" || tag=="")
      ErrMsg(fatal) << "configuration error with line \n"
                    << _ntpAuxListContents.value()[i]
                    << "\nYou need 4 fields separated by \":\"" << endmsg;    
    if (configedAux[blocklist] )
      ErrMsg(fatal) << "ntpAuxListContent for " << blocklist 
		    << " has been set. Do not repeat." << endmsg;
    
    bool hasBlock=false;
    for ( int j=0; j< _orderedBlockNames.size(); j++) {
      if ( block== _orderedBlockNames[j] ) {
        hasBlock= true;
        break;
      }
    }
    if ( ! hasBlock )
      ErrMsg(fatal) << "Don't know about the block " << block
                    << " that you set in ntpAuxListContent. "
                    << "It has to be set in ntpBlockConfigs." << endmsg;
    
    _ntpAuxlistMap[block].push_back(listname);
    auxContKeys[block].push_back(keys);
    auxTags[block].push_back(tag);
    
    configedAux[blocklist]= true;
  }
  
  
  for (int i=0; i< _orderedBlockNames.size(); i++) {
    std::string block= _orderedBlockNames[i];
    _btucands[block]= new BtuCandBlock(block);
    _btucands[block]->init(anEvent,candContMap[block],auxTags[block],auxContKeys[block]);
  }
  

}



//---------------------------------------------------------------------------
void BtuTupleMaker::fillNtpCommonVars(AbsEvent *anEvent, EventInfo *eventInfo,
				      std::string block, std::string nprefix) {
  
  // Create HTVectors for this block
  //  _btucand->createHTVectors(anEvent, block);


  HepAList<BtaCandidate> *alist= _ntplistMap[block];
  int maxDaus= _nDaughtersMap[block];
  int maxCands= _nCandsMap[block];


  HTValOrderedVector<int> trkIdx;  // special index to trk block
  bool fillTrkIdx= _linkToTrkMap[block];
  BtaCandidate *cand;
  HepAListIterator<BtaCandidate> iter(*alist);

  
  // Auxiliary lists for this block
  std::vector< HepAList<BtaCandidate>* > auxlists;
  std::vector< std::string >::const_iterator auxiter=_ntpAuxlistMap[block].begin();
  while ( auxiter!= _ntpAuxlistMap[block].end() ) {
    auxlists.push_back(locateList(anEvent,*auxiter));
    auxiter++;
  }


  int counter=0;
  iter.rewind();
  // Loop over candidates in this list and fill the ntuple for this block
  while ( (cand= iter()) ) {
    if ( counter >= maxCands ) {
      ErrMsg(warning) << "Number of candidates ("<< alist->length() 
                      <<") for ntuple block " 
	              << block << " exceeds the limit " << maxCands << endmsg;
      break;
    }
    counter++;
    
    // Find a clone from auxiliary list if provided
    std::vector<BtaCandidate*> candclones;
    if ( auxlists.size() > 0 ) {
      std::vector< HepAList<BtaCandidate>* >::const_iterator aLiter= auxlists.begin();
      int kk=0;
      BtaCandidate *auxc=0;
      while ( aLiter!= auxlists.end() ) {
	auxc=0;
	auxc= BtuUtil::findClone(*aLiter,cand);
	if ( !auxc ) ErrMsg(warning) << "Can't find a clone from list "
				     << (_ntpAuxlistMap[block])[kk] << " for candidate "
				     << cand << " in block " << block << endmsg;
	candclones.push_back(auxc);
	aLiter++;
      }
    }
    // Fill candidate block HTVectors
    _btucands[block]->fillHTVectors(anEvent,eventInfo,cand,candclones,_mcList,_truthMap);

    if ( maxDaus < cand->nDaughters() ) {
      BtaPrintTree printtree;
      ErrMsg(fatal) << "The number of daughters of candidate "
		    << cand->pdtEntry()->name() << " (" << cand << ")"
		    << " is larger than the configured max nDaughters "
		    << maxDaus << " in block " << block 
		    << printtree.print(*cand) << endmsg;
    }

    // fill daughter indices
    BtaCandidate *dau;
    HepAListIterator<BtaCandidate> diter= cand->daughterIterator();
    for ( int i=0; i< maxDaus; i++) {
      HTValOrderedVector<int> *dauLund= _dxLund[i];
      HTValOrderedVector<int> *dauIdx=  _dxIdx[i];
      dau= diter();
      if ( dau ) {
	dauLund->append(dau->pdtEntry()->lundId());
	dauIdx->append( idxAndAppendCandToList(dau) );
      } else {
	dauLund->append(0);
	dauIdx->append(-1);
      }
    }

    // Fill index pointing to trk block if qualified
    if ( fillTrkIdx )
      trkIdx.append(idxAndAppendCandToList(cand,_trkBlockName.value(),_trkList, false));

  } // end looping candidates

  std::string vname;
  std::string np= nprefix;
  std::string nCname= np+block;  
  // Block length
  _tuple->column(nCname.c_str(), counter, 0, block.c_str(), HTRange<int>(0,maxCands));

  // Block content
  _btucands[block]->fillTupleColumns(_tuple, nCname);

  // ---> daughter lund id and daughter list index
  for ( int i=0; i< maxDaus; i++) {
    char buf[1];
    sprintf(buf,"%d",i+1);
    vname= block + "d"+ std::string(buf)+ "Lund";
    _tuple->column(vname.c_str(), *_dxLund[i], nCname.c_str(),0,block.c_str());
    _dxLund[i]->clear();

    vname= block + "d"+ std::string(buf)+ "Idx";
    _tuple->column(vname.c_str(), *_dxIdx[i], nCname.c_str(),0,block.c_str());
    _dxIdx[i]->clear();
  }

  // index to TRK block
  if (fillTrkIdx) {
    vname= block+ "TrkIdx";
    _tuple->column(vname.c_str(), trkIdx, nCname.c_str(),0,block.c_str());
  }

}


//---------------------------------------------------------------------------
// Get the index of a given candidate from a HepAList, which is among a set
//  lists that were already created in configureTupleBlock(). This function
//  will select a correct list based on the candidate's pdtEntry (or the 
//  block names if given). The mapping is stored in _blockMap.
//  If the clone of the candidate is not already in the list, this candidate
//  will be appended into that list.
//
// If a composite candidate has a daughter with the same type (e.g. brem-
//  covered e -> e gamma ), the index will point to back to the same block,
//  and if the daughter is not already in the list, it will be appended
//  and will be filled to the ntuple transparently.
//
// If the auxiliary list (auxlist) is given, instead of appending this 
//  candidate to the corresponding HepAList, this function will find a clone
//  of this candidate from the auxlist and append that candidate from the 
//  auxlist to corresponding HepAList. If the clone of the given candidate 
//  cannot be found in the auxiliary list, it will just return -1.
int BtuTupleMaker::idxAndAppendCandToList(BtaCandidate *cand, 
					  std::string bl,
					  HepAList<BtaCandidate> *auxlist,
					  bool checkType) {  
  // Find the correct list: 
  //    if bl is provided, find the corresponding list from _ntplistMap
  //        using block name, 
  //    otherwise use the candidate pdtEntry to fine the block name.
  std::string block=  bl == "" ?  _blockMap[cand->pdtEntry()] : bl;
  HepAList<BtaCandidate>* alist= _ntplistMap[block];

  bool checkClonesForThisBlock= _checkClones.value() || _toCheckClones[block];

  if ( ! alist ) {
    if ( _partialDecayTreeOK.value()) return -1;

    if ( bl == "" ) 
      ErrMsg(fatal) << "The ntuple block for The candidate " 
		    << cand->pdtEntry()->name()
		    << " in the decay tree is not defined. "
		    << "It is likely a configuration error. " << endmsg;
    else 
      ErrMsg(fatal) << "The ntuple block \"" << block << "\" is not defined."
		    << "It is likely a configuration error. " << endmsg;
    return -1;
  }

  // Search for a clone from the list itself. If found, return the index.
  HepAListIterator<BtaCandidate> iter(*alist);
  BtaCandidate *ac(0);
  int index= -1;
  bool found=false;

  if ( checkClonesForThisBlock ) {
    while ( (ac= iter()) ) {
      index++;
      if ( cand->isCloneOf(*ac, checkType) ) {
	found= true;
	break;
      }
    }
  } else {
    // ====  always append the new candidate, without check isClone() 
    index= alist->length()-1;
  }

  // If not found:  
  //     if auxlist is not provided, append the cand to alist,
  //     otherwise, return -1.
  if (! found) {
    if ( ! auxlist ) alist->append(cand);
    else {
      HepAListIterator<BtaCandidate> xiter(*auxlist);
      bool found2(false);
      while ( (ac= xiter()) ) {
	if ( cand->isCloneOf(*ac, checkType) ) {
	  alist->append(ac);
	  found2= true;
	  break;
	}
      }
      if ( !found2 ) {
	return -1;
      }
    }
    index++;
    assert(index+1 == alist->length() ) ;
  }

  return index;
}

//--------------------------------------------------------------------------
// Get a list from the event  
HepAList<BtaCandidate>*
BtuTupleMaker::locateList(AbsEvent* anEvent, const std::string& listKey)
{
  HepAList<BtaCandidate>* list = 
    Ifd<HepAList< BtaCandidate > >::get(anEvent, listKey.c_str());
  if (list == 0)
    ErrMsg(fatal) << "Could not locate list " << listKey.c_str()
		  << ". This is likely a configuration error." << endmsg;
  return list;
}
//--------------------------------------------------------------------------
// Remove the content BtaCandidates from temporary lists
void BtuTupleMaker::clearNtupleLists() {
  std::vector< HepAList<BtaCandidate>* >::const_iterator it= _ntplists.begin();
  while ( it != _ntplists.end() ) {
    (*it)->removeAll();
    it++;
  }
}
//--------------------------------------------------------------------------
// Delete temporary lists
void BtuTupleMaker::destroyNtupleLists() {
  std::vector<HepAList<BtaCandidate>* >::const_iterator it1= _ntplists.begin();
  while ( it1 != _ntplists.end() ) {
    if ( *it1 ) delete *it1;
    it1++;
  }

  std::map<std::string, BtuCandBlock* >::const_iterator it2= _btucands.begin();
  while ( it2 != _btucands.end() ) {
    if ( it2->second ) delete it2->second;
    it2++;
  }

  std::vector< HTValOrderedVector<int>* >::const_iterator it3= _dxLund.begin();
  while ( it3 != _dxLund.end() ) {
    if ( *it3 ) delete *it3;
    it3++;
  }

  std::vector< HTValOrderedVector<int>* >::const_iterator it4= _dxIdx.begin();
  while ( it4 != _dxIdx.end() ) {
    if ( *it4 ) delete *it4;
    it4++;
  }

}
//--------------------------------------------------------------------------
// Print an error message
void BtuTupleMaker::ntpBlockConfigErr(std::string config) {
  ErrMsg(error) << "Configuration error in \"ntpBlockConfigs set "
		<< config << "\"\n" 
		<< "It should be like \"pdtName blockName maxNdaughters "
		<< "maxNCandidates\", where the last two fields are numbers. "
		<< endmsg;
}
//--------------------------------------------------------------------------
// Make sure the specified blocks are in correct order
void BtuTupleMaker::checkBlockOrder() {
  // Only check uniqueness for now
  // Need to order it by pdt mass
  for (int i=0; i< _orderedBlockNames.size(); i++)
    for (int j=0; j<i; j++)
      if ( _orderedBlockNames[i]== _orderedBlockNames[j])
	ErrMsg(fatal) << "There is a configuration error on setting "
		      << "ntpBlockConfigs. "
		      << "Make sure the block names are in sorted in order."
		      << endmsg;
}
