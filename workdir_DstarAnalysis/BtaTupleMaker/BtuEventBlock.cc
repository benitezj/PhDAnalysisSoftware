//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: BtuEventBlock.cc,v 1.5 2006/11/28 21:37:57 desilva Exp $
//
// Description:
//      Class for filling event block info.
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
//--------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

#include "BtaTupleMaker/BtuEventBlock.hh"

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
#include "ErrLogger/ErrLog.hh"
#include "Beta/EventInfo.hh"  
#include "AbsEvent/AbsEvent.hh"
#include "BbrStdUtils/Tokenize.hh"
#include "BbrStdUtils/String.hh"
#include "HepTuple/Tuple.h"
#include "AbsEventTag/AbsEventTag.hh"
#include "AbsEvent/AbsEventID.hh"
#include "EidData/EidEventTriplet.hh"
#include "OdfCommon/odfTime.hh"
#include "BbrGeom/BbrPointErr.hh"
#include "HepTuple/HTValOrderedVector.h"
#include "BtaTupleMaker/BtuUtil.hh"

// Added by Sasha Telnov to get the CondAlias into the EventID block
#include "EidData/EidCondKeyTriplet.hh"
#include "AbsEnv/AbsEnv.hh"
#include "GenEnv/GenEnv.hh"
#include "BtaEnv/BtaEnv.hh"

using namespace babar::String;

//----------------
// Constructors --
//----------------
BtuEventBlock::BtuEventBlock() : _initialized(false) {
}

//----------------
// Destructors --
//----------------
BtuEventBlock::~BtuEventBlock() {
}

void BtuEventBlock::init(std::string cont, std::string boolTags, 
			 std::string intTags, std::string floatTags,
			 std::string eID ) {

  if ( _initialized ) return;

  _usrEventBlock.clear();

  _eventID= eID;

  // List allowed content keywords here. Initialize them as -1.
  _contMap["EventID"]= -1;
  _contMap["CMp4"]= -1;
  _contMap["BeamSopt"]= -1;
  _contMap["UsrData"]= -1;

  std::string token;
  Tokenize tokens1(cont);
  while ( ! ( token = tokens1()).empty() ) {
    // special treatment for keyword "UsrData". Require it be UsrData(myBlock)
    std::string::size_type upos= token.find("UsrData");
    if ( upos!= string::npos ) {
      if ( token.find_first_of("(")== string::npos ||
	   token.find_last_of(")")== string::npos )
	ErrMsg(fatal) << "The keyword UsrData should be followed by "
		      << "the UsrBlock name in parentheses, e.g., "
		      << "UsrData(block1,block2)" << endmsg;
      std::string::size_type p1pos= token.find_first_of("(");
      std::string usrBlock= token.substr(p1pos);
      token= token.substr(0,p1pos);
      BtuUtil::chopEnds(usrBlock," (\t\n)");

      // usrBlock can be a list of block names separated by a comma
      Tokenize btokens(usrBlock);
      std::string bt;
      while ( ! (bt= btokens(",")).empty()) {
	  _usrEventBlock.push_back(bt);
      }
    }

    if ( ! _contMap[token]== -1 )
      ErrMsg(fatal) << " Undefined content: " << token
		    << ". Check EventBlock configuration." << endmsg;
    _contMap[token]= 1;
  }

  // tag data names
  Tokenize tokens2(boolTags);
  while ( ! ( token = tokens2()).empty() ) _boolTags.push_back(token);
  Tokenize tokens3(intTags);
  while ( ! ( token = tokens3()).empty() ) _intTags.push_back(token);
  Tokenize tokens4(floatTags);
  while ( ! ( token = tokens4()).empty() ) _floatTags.push_back(token);

  _initialized= true;

}

void BtuEventBlock::fillTupleColumns(HepTuple *tuple, AbsEvent *anEvent,
				     EventInfo* eventInfo) {

  // Fill requested contents
  if ( _contMap["EventID"] >0 )  fillEventID(tuple,anEvent);
  if ( _contMap["CMp4"] >0 )     fillCMp4(tuple,eventInfo);
  if ( _contMap["BeamSpot"] >0 ) fillBeamSpot(tuple,eventInfo);
  if ( _contMap["PrimVtx"] >0 )  fillPrimVtx(tuple,eventInfo);
  if ( _contMap["UsrData"] >0 )  fillUsrData(tuple, anEvent);


  // Fill event tags
  AbsEventTag *taginfo= Ifd<AbsEventTag>::get(anEvent);
  if ( ! taginfo ) 
    ErrMsg(fatal) << "Can't get AbsEventTag from this event." << endmsg;
  // --> bool
  for (int i=0; i< _boolTags.size(); i++) {
    std::string tn= _boolTags[i];
    bool b= false;
    taginfo->getBool(b, tn.c_str());
    tuple->column(tn.c_str(),b);
  }
  // --> int
  for (int i=0; i< _intTags.size(); i++) {
    std::string tn= _intTags[i];
    int b= -999;
    taginfo->getInt(b, tn.c_str());
    tuple->column(tn.c_str(),b);
  }
  // --> float
  for (int i=0; i< _floatTags.size(); i++) {
    std::string tn= _floatTags[i];
    float b= -9999.;
    taginfo->getFloat(b, tn.c_str());
    tuple->column(tn.c_str(),b);
  }
}

// ===>>> EventID
void BtuEventBlock::fillEventID(HepTuple *tuple, AbsEvent *anEvent) {
  int runNumber=0;
  AbsEventID *eventID = Ifd<AbsEventID>::get(anEvent, _eventID.c_str());
  if ( eventID ) {
    odfTime timeStamp = eventID->eventIdTriplet().timeStamp();
    tuple->column("runNumber",runNumber=(int)eventID->run());
    tuple->column("platform",(int)eventID->eventIdTriplet().platform());
    tuple->column("partition",(int)eventID->eventIdTriplet().partitionMask());
    tuple->column("upperID",(int)timeStamp.binary().upper);
    tuple->column("lowerID",(int)timeStamp.binary().lower);
    tuple->column("majorID",(int)eventID->majorID());
  } else {
    ErrMsg(warning) << " Can't find AbsEventID for " << _eventID << endmsg;
    tuple->column("runNumber",(int)-9999);
    tuple->column("platform",(int)-9999);
    tuple->column("partition",(int)-9999);
    tuple->column("upperID",(int)-9999);
    tuple->column("lowerID",(int)-9999);
    tuple->column("majorID",(int)-9999);
  }

// Added by Sasha Telnov to get the YYYYMM CondAlias to the EventID block
  int date=-9999;
  const EidCondKeyTriplet* triplet = gblEnv->getGen()->primaryCondKey();
  if (triplet==NULL) {
    ErrMsg(warning) << " No Event time ! "<< endmsg;
  } else {
    BdbTime bTime = triplet->key();
    struct tm myTime;
    bTime.tm(&myTime,BdbTime::Local);
    int y = myTime.tm_year + 1900;
    if (runNumber>200000) { y+=25; }
    int m = myTime.tm_mon + 1;
    date = y*100 + m;
  }
  tuple->column("date",(int)date);
}

// ===>>> CM four-momentum
void BtuEventBlock::fillCMp4(HepTuple *t, EventInfo* info) {
  HepLorentzVector p4Tot=
    info->electronBeam().p4() + info->positronBeam().p4() ;
  t->column("eePx", (float)p4Tot.px());
  t->column("eePy", (float)p4Tot.py());
  t->column("eePz", (float)p4Tot.pz());
  t->column("eeE", (float)p4Tot.e());
}

// ===>>> BeamSpot
void BtuEventBlock::fillBeamSpot(HepTuple *t, EventInfo* info) {
  const BbrPointErr beamSpot = info->beamSpot();    
  t->column("beamSX",(float)beamSpot.x());
  t->column("beamSY",(float)beamSpot.y());
  t->column("beamSZ",(float)beamSpot.z());
  t->column("beamSCovXX",(float)beamSpot.covMatrix()[0][0]);
  t->column("beamSCovYY",(float)beamSpot.covMatrix()[1][1]);
  t->column("beamSCovZZ",(float)beamSpot.covMatrix()[2][2]);
  t->column("beamSCovXZ",(float)beamSpot.covMatrix()[0][2]);
}

// ===>>> Primary Vertex
void BtuEventBlock::fillPrimVtx(HepTuple *t, EventInfo* info) {
  const BbrPointErr primVtxPoint = info->primaryVertex();
  t->column("primVtxX",(float)primVtxPoint.x());
  t->column("primVtxY",(float)primVtxPoint.y());
  t->column("primVtxZ",(float)primVtxPoint.z());
  t->column("primVtxCovXX",(float)primVtxPoint.covMatrix()[0][0]);
  t->column("primVtxCovYY",(float)primVtxPoint.covMatrix()[1][1]);
  t->column("primVtxCovZZ",(float)primVtxPoint.covMatrix()[2][2]);
  t->column("primVtxCovXY",(float)primVtxPoint.covMatrix()[0][1]);
  t->column("primVtxCovXZ",(float)primVtxPoint.covMatrix()[0][2]);
  t->column("primVtxCovYZ",(float)primVtxPoint.covMatrix()[1][2]);
  const BtaAbsVertex* primVtx=info->primaryVtx();
  if (primVtx!=0) {
    t->column("primVtxChi2",(float) primVtx->chiSquared());
    t->column("primVtxNDof",(int) primVtx->nDof());
  } else {
    t->column("primVtxChi2",(float)-999.);
    t->column("primVtxNDof",(int)-1);
  }
}

// ===>>> User Data
void BtuEventBlock::fillUsrData(HepTuple *t, AbsEvent *anEvent) {
  
  UsrType<int> type_int;
  UsrType<float> type_float;
  UsrType<double> type_double;

  // Loop over UsrEventBlock names
  for ( int j=0; j< _usrEventBlock.size(); j++) {
 
    // Get UsrData
    IfdStrKey theStrKey(_usrEventBlock[j]);
    UsrEventBlock *eventBlock=
      UsrIfd<UsrEventBlock>::get( anEvent, theStrKey);
    
    if ( ! eventBlock )
      ErrMsg(fatal) << "no UsrEventBlock named " << _usrEventBlock[j]
		    << " is found. " << endmsg;
    
    UsrBlockBase::const_iterator it= eventBlock->begin();
    while ( it!= eventBlock->end()) {
      std::string vname= it->first.name();
      
      if ( it->second == type_int ) {
	UsrVariable<int> x(vname);
	if ( ! eventBlock->get(x) ) errorGetVar(vname,_usrEventBlock[j]);
	t->column(vname.c_str(),x);
      } else if ( it->second == type_float ) {
	UsrVariable<float> x(vname);
	if ( ! eventBlock->get(x) ) errorGetVar(vname,_usrEventBlock[j]);
	t->column(vname.c_str(),x);
      } else if ( it->second == type_double ) {
	UsrVariable<double> x(vname);
	if ( ! eventBlock->get(x) ) errorGetVar(vname,_usrEventBlock[j]);
	t->column(vname.c_str(),x);
      }
      
      it++;
    }
  }
}


void BtuEventBlock::errorGetVar(std::string vname, std::string block) {
  ErrMsg(fatal) << "Error getting " << vname << " from UsrEventBlock "
		<< block << endmsg ;
}
