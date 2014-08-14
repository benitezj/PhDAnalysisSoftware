//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: BtuMCBlock.cc,v 1.3 2005/06/24 01:44:39 jreiss Exp $
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

#include "BtaTupleMaker/BtuMCBlock.hh"

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
#include "BbrGeom/BbrPointErr.hh"
#include "CLHEP/Alist/AIterator.h"
#include "Beta/BtaCandidate.hh"
#include "HepTuple/HTValOrderedVector.h"
#include "BtaTupleMaker/BtuUtil.hh"
#include "Beta/BtaAbsVertex.hh"
#include "BetaCoreTools/BtaBooster.hh"

using namespace babar::String;

//----------------
// Constructors --
//----------------
BtuMCBlock::BtuMCBlock() : _initialized(false), _mclen("mcLen"), _mcblock("MC")
{
}

//----------------
// Destructors --
//----------------
BtuMCBlock::~BtuMCBlock() {
}

void BtuMCBlock::init(std::string cont ) {

  if ( _initialized ) return;

  // List allowed content keywords here. Initialize them as -1.
  _contMap["Mass"]= -1;
  _contMap["CMMomentum"]= -1;
  _contMap["Momentum"]= -1;
  _contMap["Vertex"]= -1;


  std::string token;
  Tokenize tokens1(cont);
  while ( ! ( token = tokens1()).empty() ) {
    if ( ! _contMap[token]== -1 )
      ErrMsg(fatal) << " Undefined content: " << token
		    << ". Check MCBlock configuration." << endmsg;
    _contMap[token]= 1;
  }

  _initialized= true;

}

void BtuMCBlock::fillTupleColumns(HepTuple *tuple,
				  HepAList<BtaCandidate> *mcList,
				  EventInfo* eventInfo) {

  fillBasic(tuple,mcList);
  // Fill requested contents
  if ( _contMap["Mass"] >0 )       fillMass(tuple,mcList);
  if ( _contMap["CMMomentum"] >0 ) fillCMMomentum(tuple,mcList,eventInfo);
  if ( _contMap["Momentum"] >0 )   fillMomentum(tuple,mcList);
  if ( _contMap["Vertex"] >0 )     fillVertex(tuple,mcList);

}

// ===>>> Basic info (lund id and mother index)
void BtuMCBlock::fillBasic(HepTuple *t, HepAList<BtaCandidate> *mcList){
  HTValOrderedVector<int> mcPdgID, mcMothIdx, mcNDaughters, mcDaughterIdx;
  if ( mcList ) {
    HepAListIterator<BtaCandidate> iter(*mcList);
    BtaCandidate *mc;
    while ( (mc= iter()) ) {
      mcPdgID.append((int)mc->pdtEntry()->pdgId());
      mcNDaughters.append(mc->nDaughters());
      HepAListIterator<BtaCandidate> iterD = mc->daughterIterator();
      BtaCandidate* daughter = iterD();
      mcDaughterIdx.append(BtuUtil::getIndex(daughter,mcList));
      if ( mc->theMother()) {
    mcMothIdx.append(mcList->index(BtuUtil::matchUid(mcList,mc->theMother())));
      } else {
    mcMothIdx.append(-1);
      }
    }
  }

  int nmc= mcList ? mcList->length() : 0;
  t->column(_mclen.c_str(), nmc,0, _mcblock.c_str(),HTRange<int>(0,200));
  t->column("mcLund", mcPdgID, _mclen.c_str(),0,_mcblock.c_str());
  t->column("mothIdx", mcMothIdx, _mclen.c_str(),0,_mcblock.c_str());
  t->column("dauLen", mcNDaughters, _mclen.c_str(),0,_mcblock.c_str());
  t->column("dauIdx", mcDaughterIdx, _mclen.c_str(),0,_mcblock.c_str());
}


// ===>>> Mass
void BtuMCBlock::fillMass(HepTuple *t, HepAList<BtaCandidate> *mcList){
  HTValOrderedVector<float> mass;
  if ( mcList ) {
    HepAListIterator<BtaCandidate> iter(*mcList);
    BtaCandidate *mc;
    while ( (mc= iter()) ) {
      mass.append(mc->mass());
    }
  }
  t->column("mcmass", mass, _mclen.c_str(),0,_mcblock.c_str());
}

// ===>>> Momentum 
void BtuMCBlock::fillMomentum(HepTuple *t, HepAList<BtaCandidate> *mcList){
  HTValOrderedVector<float> mcp3,mccosth, mcphi;
  if ( mcList ) {
    HepAListIterator<BtaCandidate> iter(*mcList);
    BtaCandidate *mc;
    while ( (mc= iter()) ) {
      mcp3.append(mc->p4().rho());
      mccosth.append(mc->p4().cosTheta());
      mcphi.append(mc->p4().phi());
    }
  }
  t->column("mcp3", mcp3, _mclen.c_str(),0,_mcblock.c_str());
  t->column("mccosth", mccosth, _mclen.c_str(),0,_mcblock.c_str());
  t->column("mcphi", mcphi, _mclen.c_str(),0,_mcblock.c_str());
}

// ===>>> CM Momentum 
void BtuMCBlock::fillCMMomentum(HepTuple *t, HepAList<BtaCandidate> *mcList,
				EventInfo* eventInfo){
  HTValOrderedVector<float> mcp3,mccosth, mcphi;
  if ( mcList ) {
    BtaBooster booster(eventInfo->cmFrame());

    HepAListIterator<BtaCandidate> iter(*mcList);
    BtaCandidate *mc;
    while ( (mc= iter()) ) {
      BtaCandidate tmp(booster.boostTo(*mc));
      mcp3.append(tmp.p4().rho());
      mccosth.append(tmp.p4().cosTheta());
      mcphi.append(tmp.p4().phi());
    }
  }
  t->column("mcp3CM", mcp3, _mclen.c_str(),0,_mcblock.c_str());
  t->column("mccosthCM", mccosth, _mclen.c_str(),0,_mcblock.c_str());
  t->column("mcphiCM", mcphi, _mclen.c_str(),0,_mcblock.c_str());
}

// ===>>> Vertex
void BtuMCBlock::fillVertex(HepTuple *t, HepAList<BtaCandidate> *mcList){
  HTValOrderedVector<float> vtxx, vtxy, vtxz;
  if ( mcList ) {
    HepAListIterator<BtaCandidate> iter(*mcList);
    BtaCandidate *mc;
    while ( (mc= iter()) ) {
      BtaAbsVertex *vtx= mc->decayVtx();
      vtxx.append(vtx ? vtx->point().x() : -999.);
      vtxy.append(vtx ? vtx->point().y() : -999.);
      vtxz.append(vtx ? vtx->point().z() : -999.);
    }
  }

  t->column("mcVtxx", vtxx, _mclen.c_str(),0,_mcblock.c_str());
  t->column("mcVtxy", vtxy, _mclen.c_str(),0,_mcblock.c_str());
  t->column("mcVtxz", vtxz, _mclen.c_str(),0,_mcblock.c_str());

}
