//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: BtuCandBlock.cc,v 1.20 2006/11/27 18:08:25 desilva Exp $
//
// Description:
//      Class for filling candidate blocks info.
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

#include "BtaTupleMaker/BtuCandBlock.hh"

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
#include "AbsEvent/AbsEvent.hh"
#include "Beta/EventInfo.hh"
#include "Beta/BtaCandidate.hh"
#include "BbrStdUtils/Tokenize.hh"
#include "BbrStdUtils/String.hh"
#include "HepTuple/Tuple.h"
#include "HepTuple/HTValOrderedVector.h"
#include "Beta/BtaAbsVertex.hh"
#include "CLHEP/Alist/ConstAIterator.h"
#include "BtaTupleMaker/BtuUtil.hh"
#include "BetaCoreTools/BtaBooster.hh"
#include "BetaPid/PidWeight.hh"

#include "BetaCoreTools/BtaDeltaTConverter.hh"
#include "BetaCoreTools/BtaExclusiveDecayTruthComp.hh"
#include "BetaMicroBase/BtaAttributes.hh"
#include "AbsEventTag/AbsEventTag.hh"
#include "VtxBase/VtxAbsAlgorithm.hh"
#include "VtxTreeFitter/VtkFitter.hh"
#include "difAlgebra/DifNumber.hh"

#include "BtaTupleMaker/BtuUtil.hh"

using namespace babar::String;
//----------------
// Constructors --
//----------------
BtuCandBlock::BtuCandBlock(std::string block) : 
  _block(block), _initialized(false), _contMap(0) {
}

//--------------
// Destructor --
//--------------
BtuCandBlock::~BtuCandBlock() {
  deleteHTVectors(_boolVars);
  deleteHTVectors(_intVars);
  deleteHTVectors(_floatVars);
}

void BtuCandBlock::init(AbsEvent *anEvent,std::string &keys, 
			std::vector<std::string> &auxTag, 
			std::vector<std::string> &auxkeys) {
  if ( _initialized ) return;

  // BlockName: key1 key2 ...  
  _keywordBit["MCIdx"]=      (1 << BtuCandBlock::MCIdx );
  _keywordBit["Mass"]=       (1 << BtuCandBlock::Mass );
  _keywordBit["Momentum"]=   (1 << BtuCandBlock::Momentum );
  _keywordBit["CMMomentum"]= (1 << BtuCandBlock::CMMomentum );
  _keywordBit["VtxChi2"]=    (1 << BtuCandBlock::VtxChi2 );
  _keywordBit["Vertex"]=     (1 << BtuCandBlock::Vertex );
  _keywordBit["VtxCov"]=     (1 << BtuCandBlock::VtxCov );
  _keywordBit["Doca"]=       (1 << BtuCandBlock::Doca );
  _keywordBit["DocaXY"]=     (1 << BtuCandBlock::DocaXY );
  _keywordBit["Poca"]=       (1 << BtuCandBlock::Poca );
  _keywordBit["PocaXY"]=     (1 << BtuCandBlock::PocaXY );
  _keywordBit["PocaErr"]=    (1 << BtuCandBlock::PocaErr );
  _keywordBit["PocaCov"]=    (1 << BtuCandBlock::PocaCov );
  _keywordBit["UsrData"]=    (1 << BtuCandBlock::UsrData );
  _keywordBit["PIDWeight"]=  (1 << BtuCandBlock::PIDWeight );
  _keywordBit["nDaughters"]= (1 << BtuCandBlock::nDaughters );
  _keywordBit["TagVtx"]=     (1 << BtuCandBlock::TagVtx );  
  _keywordBit["Flight"]=     (1 << BtuCandBlock::Flight );  
  _keywordBit["FlightBS"]=   (1 << BtuCandBlock::FlightBS );  
  _keywordBit["Dalitz"]=     (1 << BtuCandBlock::Dalitz );  
  _keywordBit["DalitzMC"]=   (1 << BtuCandBlock::DalitzMC );  
  _keywordBit["ThreeBodyAngles"]= (1 << BtuCandBlock::ThreeBodyAngles );  
  

  // Main configuration
  setKeywordBitmap(keys,_contMap, _kwordargs);

  setKeywordBitmap(auxkeys,_auxMap, _auxkwordargs);
  _auxTag= auxTag;

  // create HTVectors
  createHTVectors(anEvent);

}

void BtuCandBlock::createHTVectors(AbsEvent *anEvent) {
  // default variables
  addIntVar("Lund");

  createHTVectors(anEvent, _contMap, _kwordargs);
  createHTVectors(anEvent, _auxMap, _auxkwordargs,_auxTag);
}

void BtuCandBlock::createHTVectors(AbsEvent *anEvent, std::vector<bitmap_t>& bitmap,
				   std::vector<std::map<std::string, std::vector<std::string> > >& kargs,
				   std::vector<std::string> tags) {
  
  std::vector<bitmap_t>::const_iterator bititer= bitmap.begin();
  int kk=0;
  while ( bititer!= bitmap.end() ) {
    
    createHTVectors(anEvent,*bititer,kargs[kk],tags[kk]);
    bititer++;
    kk++;
  }
  
}

void BtuCandBlock::createHTVectors(AbsEvent *anEvent,bitmap_t bitmap, 
				   std::map<std::string,std::vector<std::string> >& kargs, 
				   std::string tag) {
  
  if ( bitmap&(0x1<<BtuCandBlock::MCIdx) ) {
    addIntVar(tag+"MCIdx");
  }
  if ( bitmap&(0x1<<BtuCandBlock::Mass) ) {
    addFloatVar(tag+"Mass");
    addFloatVar(tag+"MassErr");
  }
  if ( bitmap&(0x1<<BtuCandBlock::Momentum) ) {
    addFloatVar(tag+"p3");
    addFloatVar(tag+"costh");
    addFloatVar(tag+"phi");
  }
  if ( bitmap&(0x1<<BtuCandBlock::CMMomentum) ) {
    addFloatVar(tag+"p3CM");
    addFloatVar(tag+"costhCM");
    addFloatVar(tag+"phiCM");
  }
  if ( bitmap&(0x1<<BtuCandBlock::Vertex) ) {
    addFloatVar(tag+"Vtxx");
    addFloatVar(tag+"Vtxy");
    addFloatVar(tag+"Vtxz");
  }
  if ( bitmap&(0x1<<BtuCandBlock::VtxCov) ) {
    addFloatVar(tag+"Vtx_cxx");
    addFloatVar(tag+"Vtx_cyy");
    addFloatVar(tag+"Vtx_czz");
    addFloatVar(tag+"Vtx_cxy");
    addFloatVar(tag+"Vtx_cyz");
    addFloatVar(tag+"Vtx_czx");
  }
  if ( bitmap&(0x1<<BtuCandBlock::VtxChi2) ) {
    addIntVar(tag+"nDof");
    addFloatVar(tag+"Chi2");
    addIntVar(tag+"VtxStatus");
  }
  if ( bitmap&(0x1<<BtuCandBlock::Doca) ) {
    addFloatVar(tag+"Doca");
    addFloatVar(tag+"DocaErr");
    addFloatVar(tag+"Doca_z");
    addFloatVar(tag+"Doca_zErr");
  }
  if ( bitmap&(0x1<<BtuCandBlock::DocaXY) ) {
    addFloatVar(tag+"DocaXY_xy");
    addFloatVar(tag+"DocaXY_xyErr");
    addFloatVar(tag+"DocaXY_z");
    addFloatVar(tag+"DocaXY_zErr");
  }
  if ( bitmap&(0x1<<BtuCandBlock::Poca ) ) {
    addFloatVar(tag+"Poca_x");
    addFloatVar(tag+"Poca_y");
    addFloatVar(tag+"Poca_z");
    if ( bitmap&(0x1<<BtuCandBlock::PocaErr ) ) {
      addFloatVar(tag+"Poca_xErr");
      addFloatVar(tag+"Poca_yErr");
      addFloatVar(tag+"Poca_zErr");
    }
    if ( bitmap&(0x1<<BtuCandBlock::PocaCov ) ) {
      addFloatVar(tag+"Poca_cxx");
      addFloatVar(tag+"Poca_cyy");
      addFloatVar(tag+"Poca_czz");
      addFloatVar(tag+"Poca_cxy");
      addFloatVar(tag+"Poca_cyz");
      addFloatVar(tag+"Poca_czx");
    }
  }
  if ( bitmap&(0x1<<BtuCandBlock::PocaXY ) ) {
    addFloatVar(tag+"PocaXY_x");
    addFloatVar(tag+"PocaXY_y");
    addFloatVar(tag+"PocaXY_z");
    if ( bitmap&(0x1<<BtuCandBlock::PocaErr ) ) {
      addFloatVar(tag+"PocaXY_xErr");
      addFloatVar(tag+"PocaXY_yErr");
      addFloatVar(tag+"PocaXY_zErr");
    }
    if ( bitmap&(0x1<<BtuCandBlock::PocaCov ) ) {
      addFloatVar(tag+"PocaXY_cxx");
      addFloatVar(tag+"PocaXY_cyy");
      addFloatVar(tag+"PocaXY_czz");
      addFloatVar(tag+"PocaXY_cxy");
      addFloatVar(tag+"PocaXY_cyz");
      addFloatVar(tag+"PocaXY_czx");
    }
  }

  if ( bitmap&(0x1<<BtuCandBlock::UsrData) ) {
    createUsrHTVectors(anEvent,kargs["UsrData"],tag);
  }

  if ( bitmap&(0x1<<BtuCandBlock::PIDWeight) ) {
    createPidHTVectors(anEvent,kargs["PIDWeight"],tag);
  }

  if ( bitmap&(0x1<<BtuCandBlock::nDaughters) ) {
    addIntVar(tag+"nDaus");
  }

  if ( bitmap&(0x1<<BtuCandBlock::TagVtx) ) {
    addFloatVar(tag+"DeltaZ");
    addFloatVar(tag+"DeltaZProbChi2");
    addFloatVar(tag+"DeltaZErr");
    addFloatVar(tag+"DeltaT");
    addFloatVar(tag+"DeltaTErr");
    addIntVar(tag+"TagVtxNTUsed");
    addIntVar(tag+"TagVtxNCandUsed");
    addIntVar(tag+"TagVtxStatus");
    addIntVar(tag+"TagVtxNDof");
    addIntVar(tag+"Tag04Cat");
    addFloatVar(tag+"Tag04Tag");
    addFloatVar(tag+"DeltaZMC");
    addFloatVar(tag+"DeltaTMC");
    addFloatVar(tag+"SumTMC");
    addIntVar(tag+"TagFlavorMC");
    addIntVar(tag+"RecFlavorMC");
  }

  if ( bitmap&(0x1<<BtuCandBlock::Flight) ) {
    addFloatVar(tag+"FlightLen");
    addFloatVar(tag+"FlightErr");
    addFloatVar(tag+"FlightCosA");
  }

  if ( bitmap&(0x1<<BtuCandBlock::FlightBS) ) {
    addFloatVar(tag+"FlightBSLen");
    addFloatVar(tag+"FlightBSErr");
    addFloatVar(tag+"FlightBSCosA");
  }

  if ( bitmap&(0x1<<BtuCandBlock::Dalitz) ) {
    addFloatVar(tag+"Msq12");
    addFloatVar(tag+"Msq23");
    addFloatVar(tag+"Msq31");
  }
  if ( bitmap&(0x1<<BtuCandBlock::DalitzMC) ) {
    addFloatVar(tag+"Msq12MC");
    addFloatVar(tag+"Msq23MC");
    addFloatVar(tag+"Msq31MC");
  }
  if ( bitmap&(0x1<<BtuCandBlock::ThreeBodyAngles) ) {
    addFloatVar(tag+"CosN");
    addFloatVar(tag+"CosD");
  }

}

// --- public ---
void BtuCandBlock::fillHTVectors(AbsEvent *anEvent, EventInfo *eventInfo,
				 BtaCandidate *cand, 
				 std::vector<BtaCandidate*>& auxlist,
				 HepAList<BtaCandidate> *mclist,
				 BtaMcAssoc *truthMap) {
  
  // Abort if passed a null candidate
  if ( !cand ) ErrMsg(fatal) << "null BtaCandidate " << endmsg;
  _intVars["Lund"]->append(cand->pdtEntry()->lundId());

  fillHTVectors(anEvent,eventInfo,cand,_contMap,_kwordargs,mclist,truthMap);

  if ( auxlist.size() > 0 ) {
    std::vector<BtaCandidate*>::const_iterator iter= auxlist.begin();
    int kk=0;
    while ( iter!= auxlist.end() ) {
      BtaCandidate *auxc= *iter;
      // Abort if cand and auxc are not clones
      if ( (auxc) && !(cand->isCloneOf(*auxc))) 
	ErrMsg(fatal) << "auxc is not a clone of cand. " << endmsg;
      
      fillHTVectors(anEvent, eventInfo, auxc, _auxMap[kk],_auxkwordargs[kk],
		    mclist, truthMap, _auxTag[kk]);

      iter++;
      kk++;
    }
  }
}

// --- protected ---
void BtuCandBlock::fillHTVectors(AbsEvent *anEvent, EventInfo *eventInfo,
				 BtaCandidate *cand, bitmap_t bitmap,
				 std::map<std::string, std::vector<std::string> >& kargs,
				 HepAList<BtaCandidate> *mclist,
				 BtaMcAssoc *truthMap, std::string tag) {

  assert(eventInfo);
  if ( bitmap== 0 ) return;

  bool nocand= (0==cand);
  //  assert(cand);

  // ===>>> Matched MC index in mc list
  if ( bitmap&(0x1<<BtuCandBlock::MCIdx) ) {
    BtaCandidate *mcc(0);
    if ( cand && truthMap && mclist ) mcc= truthMap->mcFromReco(cand);
    _intVars[tag+"MCIdx"]->append((mcc && mclist) ? mclist->index(BtuUtil::matchUid(mclist,mcc)) : -1 );
	ErrMsg(trace) << ((mcc && mclist) ? mclist->index(BtuUtil::matchUid(mclist,mcc)) : -1) << endmsg;
  }
  
  // ===>>> Mass
  if ( bitmap&(0x1<<BtuCandBlock::Mass) ) {
    _floatVars[tag+"Mass"]->append(nocand ? -1 : cand->mass());
    _floatVars[tag+"MassErr"]->append(nocand ? -1 : cand->massErr());
  }

  // ===>>> Momentum
  if ( bitmap&(0x1<<BtuCandBlock::Momentum) ) {
    _floatVars[tag+"p3"]->append(nocand ? -1 : cand->p4().rho());
    _floatVars[tag+"costh"]->append(nocand ? -9 : cand->p4().cosTheta());
    _floatVars[tag+"phi"]->append(nocand ? -9 : cand->p4().phi());
  }

  // ===>>> CM Momentum
  if ( bitmap&(0x1<<BtuCandBlock::CMMomentum) ) {
    BtaBooster booster(eventInfo->cmFrame());
    BtaCandidate tmp(nocand ? 0 : booster.boostTo(*cand));
    _floatVars[tag+"p3CM"]->append(nocand ? -1 : tmp.p4().rho());
    _floatVars[tag+"costhCM"]->append(nocand ? -9 : tmp.p4().cosTheta());
    _floatVars[tag+"phiCM"]->append(nocand ? -9 : tmp.p4().phi());
  }

  // ===>>> Vertex location
  if ( bitmap&(0x1<<BtuCandBlock::Vertex) ) {
    BtaAbsVertex *vtx= nocand ? 0 : cand->decayVtx();
    _floatVars[tag+"Vtxx"]->append(vtx ? vtx->point().x() : -999.);
    _floatVars[tag+"Vtxy"]->append(vtx ? vtx->point().y() : -999.);
    _floatVars[tag+"Vtxz"]->append(vtx ? vtx->point().z() : -999.);
  }

  // ===>>> Vertex errors
  if ( bitmap&(0x1<<BtuCandBlock::VtxCov) ) {
    BtaAbsVertex *vtx= nocand ? 0 : cand->decayVtx();
    if ( vtx ) assert(vtx->covariance().num_row()== 3);
    _floatVars[tag+"Vtx_cxx"]->append(vtx ? vtx->covariance()[0][0] : -999.);
    _floatVars[tag+"Vtx_cyy"]->append(vtx ? vtx->covariance()[1][1] : -999.);
    _floatVars[tag+"Vtx_czz"]->append(vtx ? vtx->covariance()[2][2] : -999.);
    _floatVars[tag+"Vtx_cxy"]->append(vtx ? vtx->covariance()[0][1] : -999.);
    _floatVars[tag+"Vtx_cyz"]->append(vtx ? vtx->covariance()[1][2] : -999.);
    _floatVars[tag+"Vtx_czx"]->append(vtx ? vtx->covariance()[2][0] : -999.);
  }

  // ===>>> Vertexing Chi-squared
  if ( bitmap&(0x1<<BtuCandBlock::VtxChi2) ) {
    BtaAbsVertex *vtx= nocand ? 0 : cand->decayVtx();
    _intVars[tag+"nDof"]->append(vtx ? vtx->nDof() : -1 );
    _floatVars[tag+"Chi2"]->append(vtx ? vtx->chiSquared() : -1.0 );
    _intVars[tag+"VtxStatus"]->append(vtx ? vtx->status() : -1 );
  }

  // ===>>> DOCA and/or POCA  w.r.t beamspot
  if ( bitmap&(0x1<<BtuCandBlock::Doca) ||
       bitmap&(0x1<<BtuCandBlock::Poca) ) {
    const BbrPointErr beamSpot = eventInfo->beamSpot();
    const BtaAbsRecoObject* theReco(cand->recoObject());
    BbrPointErr pos(HepPoint(-9999.,-9999.,-9999.));
    if ( theReco ) pos= theReco->positionErr(beamSpot,BtaAbsRecoObject::XYZ);
    BbrVectorErr diff= pos- beamSpot;

    if ( bitmap&(0x1<<BtuCandBlock::Doca) ) {
      _floatVars[tag+"Doca"]->append((float)diff.mag());
      float cov= diff.covRTPMatrix()[BbrVectorErr::Rho][BbrVectorErr::Rho];
      _floatVars[tag+"DocaErr"]->append(cov<0 ? -1: sqrt(cov));
      _floatVars[tag+"Doca_z"]->append((float)diff.z());
      cov= diff.covMatrix()[Hep3Vector::Z][Hep3Vector::Z];
      _floatVars[tag+"Doca_zErr"]->append(cov<0 ? -1: sqrt(cov));
    }
    if ( bitmap&(0x1<<BtuCandBlock::Poca) ) {
      _floatVars[tag+"Poca_x"]->append(pos.x());
      _floatVars[tag+"Poca_y"]->append(pos.y());
      _floatVars[tag+"Poca_z"]->append(pos.z());
      if ( bitmap&(0x1<<BtuCandBlock::PocaErr ) ) {
	float cov = pos.covMatrix()[HepPoint::X][HepPoint::X];
	_floatVars[tag+"Poca_xErr"]->append(cov<0 ? -1: sqrt(cov));
	cov = pos.covMatrix()[HepPoint::Y][HepPoint::Y];
	_floatVars[tag+"Poca_yErr"]->append(cov<0 ? -1: sqrt(cov));
	cov = pos.covMatrix()[HepPoint::Z][HepPoint::Z];
	_floatVars[tag+"Poca_zErr"]->append(cov<0 ? -1: sqrt(cov));
      }
      if ( bitmap&(0x1<<BtuCandBlock::PocaCov ) ) {
	BbrError mat(pos.covMatrix());
	_floatVars[tag+"Poca_cxx"]->append(mat[HepPoint::X][HepPoint::X]);
	_floatVars[tag+"Poca_cyy"]->append(mat[HepPoint::Y][HepPoint::Y]);
	_floatVars[tag+"Poca_czz"]->append(mat[HepPoint::Z][HepPoint::Z]);
	_floatVars[tag+"Poca_cxy"]->append(mat[HepPoint::X][HepPoint::Y]);
	_floatVars[tag+"Poca_cyz"]->append(mat[HepPoint::Y][HepPoint::Z]);
	_floatVars[tag+"Poca_czx"]->append(mat[HepPoint::Z][HepPoint::X]);
      }
    }
  }

  // ===>>> DOCAXY and/or POCAXY w.r.t beamspot
  if ( bitmap&(0x1<<BtuCandBlock::DocaXY) ||
       bitmap&(0x1<<BtuCandBlock::PocaXY) ) {
    const BbrPointErr beamSpot = eventInfo->beamSpot();
    const BtaAbsRecoObject* theReco(cand->recoObject());
    BbrPointErr pos(HepPoint(-9999.,-9999.,-9999.));
    if ( theReco ) pos= theReco->positionErr(beamSpot,BtaAbsRecoObject::XY);
    BbrVectorErr diff= pos- beamSpot;

    if ( bitmap&(0x1<<BtuCandBlock::DocaXY) ) {
      _floatVars[tag+"DocaXY_xy"]->append((float)diff.perp());
      float cov= diff.covRZPMatrix()[BbrVectorErr::C_Rho][BbrVectorErr::C_Rho];
      _floatVars[tag+"DocaXY_xyErr"]->append(cov<0 ? -1: sqrt(cov));
      _floatVars[tag+"DocaXY_z"]->append((float)diff.z());
      cov= diff.covMatrix()[Hep3Vector::Z][Hep3Vector::Z];
      _floatVars[tag+"DocaXY_zErr"]->append(cov<0 ? -1: sqrt(cov));
    }
    if ( bitmap&(0x1<<BtuCandBlock::PocaXY) ) {
      _floatVars[tag+"PocaXY_x"]->append(pos.x());
      _floatVars[tag+"PocaXY_y"]->append(pos.y());
      _floatVars[tag+"PocaXY_z"]->append(pos.z());
      if ( bitmap&(0x1<<BtuCandBlock::PocaErr ) ) {
	float cov = pos.covMatrix()[HepPoint::X][HepPoint::X];
	_floatVars[tag+"PocaXY_xErr"]->append(cov<0 ? -1: sqrt(cov));
	cov = pos.covMatrix()[HepPoint::Y][HepPoint::Y];
	_floatVars[tag+"PocaXY_yErr"]->append(cov<0 ? -1: sqrt(cov));
	cov = pos.covMatrix()[HepPoint::Z][HepPoint::Z];
	_floatVars[tag+"PocaXY_zErr"]->append(cov<0 ? -1: sqrt(cov));
      }
      if ( bitmap&(0x1<<BtuCandBlock::PocaCov ) ) {
	BbrError mat(pos.covMatrix());
	_floatVars[tag+"PocaXY_cxx"]->append(mat[HepPoint::X][HepPoint::X]);
	_floatVars[tag+"PocaXY_cyy"]->append(mat[HepPoint::Y][HepPoint::Y]);
	_floatVars[tag+"PocaXY_czz"]->append(mat[HepPoint::Z][HepPoint::Z]);
	_floatVars[tag+"PocaXY_cxy"]->append(mat[HepPoint::X][HepPoint::Y]);
	_floatVars[tag+"PocaXY_cyz"]->append(mat[HepPoint::Y][HepPoint::Z]);
	_floatVars[tag+"PocaXY_czx"]->append(mat[HepPoint::Z][HepPoint::X]);
      }
    }
  }

  // ===>>> User Data
  if ( bitmap&(0x1<<BtuCandBlock::UsrData) ) {
    fillUsrHTVectors(anEvent,cand,kargs["UsrData"],tag);
  }

  // ===>>> PidWeight
  if ( bitmap&(0x1<<BtuCandBlock::PIDWeight) ) {
    fillPidHTVectors(anEvent,cand,kargs["PIDWeight"],tag);
  }

  // ===>>> nDaughters
  if ( bitmap&(0x1<<BtuCandBlock::nDaughters) ) {
    _intVars[tag+"nDaus"]->append(nocand ? -1 : cand->nDaughters());
  }

  // ===>>> Tag Vertex
  if ( bitmap&(0x1<<BtuCandBlock::TagVtx) ) {

    static const IfdStrKey defaultKey("Default");    
    BtaAttributes* attributes = Ifd<BtaAttributes>::get(anEvent,defaultKey) ;
    AbsEventTag *tagInfo= attributes->findY4SDaughter(cand);
    if ( !tagInfo ) {
      ErrMsg(fatal) << "Can't find an Upsilon(4S) that decays to this candidate " << cand->pdtEntry()->name() << ".\n Make sure you have BtsTaggingDispatch in your path and you use the same B list for tagging." << endmsg;
    }

    float deltaz, deltazCov, deltat, deltaterr;
    int ntagvtxtrk(0), tag04cat(0), ntagcandused(0), tagvtxndof(-1);
    float tag04tag, deltazprobchi2;
    int tagvtxstatus(BtaAbsVertex::Success);
    bool status(true);
    status &= tagInfo->getFloat(deltaz,"DeltaZ");
    status &= tagInfo->getFloat(deltazprobchi2,"DeltaZProbChi2");
    status &= tagInfo->getFloat(deltazCov,"CovDeltaZ");
    status &= tagInfo->getInt(ntagvtxtrk,"VtxNTUsed");
    status &= tagInfo->getInt(ntagcandused,"VtxNCandUsed");
    status &= tagInfo->getInt(tagvtxstatus,"VtxStatus");
    status &= tagInfo->getInt(tagvtxndof,"VtxNDof");
    status &= tagInfo->getInt(tag04cat,"BtgBTaggerTag04Cat");
    status &= tagInfo->getFloat(tag04tag,"BtgBTaggerTag04Tag");

    if ( tagvtxstatus!= BtaAbsVertex::Failed  && !status ) {
      ErrMsg(warning) << deltaz<< endmsg;
      ErrMsg(warning) << deltazprobchi2<< endmsg;
      ErrMsg(warning) << deltazCov<< endmsg;
      ErrMsg(warning) << ntagvtxtrk<< endmsg;
      ErrMsg(warning) << ntagcandused<< endmsg;
      ErrMsg(warning) << tagvtxstatus<< endmsg;
      ErrMsg(warning) << tagvtxndof<< endmsg;
      ErrMsg(warning) << tag04cat<< endmsg;
      ErrMsg(warning) << tag04tag<< endmsg;
      ErrMsg(fatal) << "Some or all of the tag info from BatAttributes cannot be retrieved!!" << endmsg;
    }

    BtaDeltaTConverter dzdt(deltaz,deltazCov);
    BbrDoubleErr deltaTfromZ = dzdt.deltaT(cand) ;
    deltat= deltaTfromZ.value();
    deltaterr= sqrt(deltaTfromZ.covariance());
   
    // MC
    float deltazmc(-99999.), deltatmc(-99999.), sumtmc(-99999.);
    int tagflavormc(0),recflavormc(0);
    if ( mclist ) {
      BtaExclusiveDecayTruthComp truthtool(cand,mclist);
      deltazmc= truthtool.deltaZ();
      deltatmc= truthtool.deltaT();
      sumtmc= truthtool.tTag() + truthtool.tReco();
      tagflavormc = truthtool.getTagFlavor() ;
      recflavormc = truthtool.getRecoFlavor() ;
    }

    _floatVars[tag+"DeltaZ"]->append(deltaz);
    _floatVars[tag+"DeltaZProbChi2"]->append(deltazprobchi2);
    _floatVars[tag+"DeltaZErr"]->append(sqrt(deltazCov));
    _floatVars[tag+"DeltaT"]->append(deltat);
    _floatVars[tag+"DeltaTErr"]->append(deltaterr);
    _intVars[tag+"TagVtxNTUsed"]->append(ntagvtxtrk);
    _intVars[tag+"TagVtxNCandUsed"]->append(ntagcandused);
    _intVars[tag+"TagVtxNDof"]->append(tagvtxndof);
    _intVars[tag+"TagVtxStatus"]->append(tagvtxstatus);
    _intVars[tag+"Tag04Cat"]->append(tag04cat);
    _floatVars[tag+"Tag04Tag"]->append(tag04tag);
    _floatVars[tag+"DeltaZMC"]->append(deltazmc);
    _floatVars[tag+"DeltaTMC"]->append(deltatmc * 1e12);
    _floatVars[tag+"SumTMC"]->append(sumtmc * 1e12);
    _intVars[tag+"TagFlavorMC"]->append(tagflavormc);
    _intVars[tag+"RecFlavorMC"]->append(recflavormc);

  }

  // ===>>> Decay length, significance and flight angle on the XY plane
  // using beam spot
  if ( bitmap&(0x1<<BtuCandBlock::FlightBS) ) {
    double decayLen(-1e6), decayErr(-1e6), cosAngle(-9);
    if ( nocand ) {
    } else {
      const BbrPointErr beamSpot = eventInfo->beamSpot();
      calcV0Flight(cand,beamSpot,decayLen,decayErr,cosAngle,true);
    }
    _floatVars[tag+"FlightBSLen"]->append(decayLen);
    _floatVars[tag+"FlightBSErr"]->append(decayErr);
    _floatVars[tag+"FlightBSCosA"]->append(cosAngle);
  }

  // ===>>> Decay length, significance and flight angle on the XY plane
  // using mother's decay vtx
  if ( bitmap&(0x1<<BtuCandBlock::Flight) ) {
    double decayLen(-1e6), decayErr(-1e6), cosAngle(-9);
    if ( nocand ) {
    } else {
      const BtaCandidate *mom= cand->theMother();
      if ( ! mom ) {
	// If the candidate has no mother, use beamspot.
	const BbrPointErr beamSpot = eventInfo->beamSpot();
	calcV0Flight(cand,beamSpot,decayLen,decayErr,cosAngle,true);
      } else if ( cand->decayVtx() && 
		  cand->decayVtx()->status()==BtaAbsVertex::Success && 
		  mom->decayVtx() &&
		  mom->decayVtx()->status()==BtaAbsVertex::Success ){
	// If the candidate and its mother have good vertexes, use them. 
	const BbrPointErr orig(mom->decayVtx()->point(), mom->decayVtx()->covariance());
	calcV0Flight(cand,orig,decayLen,decayErr,cosAngle,false);
      } else { // If the candidate and its mother don't have good vertexes, refit mother.
	BtaCandidate clone(*mom);
	clone.invalidateFit();
	vtxtreefit::Fitter treeFit(clone);
	treeFit.fit();
	BtaCandidate fittedM= treeFit.getFitted(*mom);
	BtaCandidate fittedD= treeFit.getFitted(*cand);
	if ( fittedD.decayVtx() && 
	     fittedD.decayVtx()->status()==BtaAbsVertex::Success &&
	     fittedM.decayVtx() && 
	     fittedM.decayVtx()->status()==BtaAbsVertex::Success) {
	  const BbrPointErr orig(fittedM.decayVtx()->point(), fittedM.decayVtx()->covariance());
	  calcV0Flight(&fittedD,orig,decayLen,decayErr,cosAngle,false);
	}
      }
    }
    _floatVars[tag+"FlightLen"]->append(decayLen);
    _floatVars[tag+"FlightErr"]->append(decayErr);
    _floatVars[tag+"FlightCosA"]->append(cosAngle);
  }

  // ===>>> Dalitz variables for 3-body decays
  if ( bitmap&(0x1<<BtuCandBlock::Dalitz) ) {
    float msq12(-1), msq23(-1), msq31(-1);
    if ( 0!= cand && cand->nDaughters() == 3 ) {
      HepAListIterator<BtaCandidate> diter= cand->daughterIterator();
      const BtaCandidate *d1= diter();
      const BtaCandidate *d2= diter();
      const BtaCandidate *d3= diter();
      
      msq12= (d1->p4()+d2->p4()).m2();
      msq23= (d2->p4()+d3->p4()).m2();
      msq31= (d3->p4()+d1->p4()).m2();
    }
    _floatVars[tag+"Msq12"]->append(msq12);
    _floatVars[tag+"Msq23"]->append(msq23);
    _floatVars[tag+"Msq31"]->append(msq31);
  }

  // ===>>> Dalitz variables for 3-body decays (MC truth)
  if ( bitmap&(0x1<<BtuCandBlock::DalitzMC) ) {
    float mcmsq12(-1), mcmsq23(-1), mcmsq31(-1);
    if ( 0!= cand && cand->nDaughters() == 3 && truthMap) {
      HepAListIterator<BtaCandidate> diter= cand->daughterIterator();
      const BtaCandidate *d1= diter();
      const BtaCandidate *d2= diter();
      const BtaCandidate *d3= diter();
      const BtaCandidate *mcd1  = truthMap->mcFromReco(d1);
      const BtaCandidate *mcd2  = truthMap->mcFromReco(d2);
      const BtaCandidate *mcd3  = truthMap->mcFromReco(d3);

      if (0!=mcd1 && 0!=mcd2 && 0!=mcd3) {
      mcmsq12=  (mcd1->p4()+mcd2->p4()).m2();
      mcmsq23=  (mcd2->p4()+mcd3->p4()).m2();
      mcmsq31=  (mcd3->p4()+mcd1->p4()).m2();
    }
    }
    _floatVars[tag+"Msq12MC"]->append(mcmsq12);
    _floatVars[tag+"Msq23MC"]->append(mcmsq23);
    _floatVars[tag+"Msq31MC"]->append(mcmsq31);
  }


  // Three-body decay angles:  V -> PPP
  //   CosN  : cosine of the angle between the parent particle direction
  //           and the normal of PPP plane in the PPP rest frame.
  //   CosD  : cosine of the angle between one P and the axis formed by the
  //           other P's in the di-P rest frame.
  if ( bitmap&(0x1<<BtuCandBlock::ThreeBodyAngles) ) {
    float costhN(-9), costhD(-9);
    if ( 0!=cand && cand->nDaughters() == 3 && cand->p4().isTimelike() ) {  // only works for three-body decay.
      HepAListIterator<BtaCandidate> diter= cand->daughterIterator();
      const BtaCandidate *d1= diter();
      const BtaCandidate *d2= diter();
      const BtaCandidate *d3= diter();
      
      // Booster to cand's rest frame
      BtaBooster booster(cand);

      // get p4's
      HepLorentzVector p4d1= booster.boostedP4(*d1,BtaBooster::To);
      HepLorentzVector p4d2= booster.boostedP4(*d2,BtaBooster::To);
      //HepLorentzVector p4d3= booster.boostedP4(*d3,BtaBooster::To);

      // find the parent particle
      BtaCandidate *parent= cand->theMother();
      if ( parent ) {
	// get parent's p4 at cand's rest frame
	HepLorentzVector p4moth= booster.boostedP4(*parent,BtaBooster::To);

	// find normal vector
	Hep3Vector normal= p4d1.vect().cross(p4d2.vect());
	costhN= cos(normal.angle(p4moth.vect()));
      }

      // get d1+d2 system
      HepLorentzVector p4d12= d1->p4()+d2->p4();
      BtaBooster booster12(p4d12);
      HepLorentzVector p4d1x= booster12.boostedP4(*d1,BtaBooster::To);
      HepLorentzVector p4d3x= booster12.boostedP4(*d3,BtaBooster::To);
      costhD= cos(p4d1x.vect().angle(p4d3x.vect()));
    }

    _floatVars[tag+"CosN"]->append(costhN);
    _floatVars[tag+"CosD"]->append(costhD);

  }




}

void BtuCandBlock::createUsrHTVectors(AbsEvent *anEvent, 
				      std::vector<std::string>& usrblks,
				      std::string tag) {

  UsrType<int> type_int;
  UsrType<float> type_float;
  UsrType<double> type_double;

  UsrType<char> type_char;
  UsrType<short> type_short;

  UsrType<Hep3Vector> type_H3Vector;
  
  // Loop over UsrCandBlock names
  for ( int j=0; j< usrblks.size(); j++) {
    
    // Get UsrData
    IfdStrKey theKey(usrblks[j].c_str());
    UsrCandBlock *candBlock=
      UsrIfd<UsrCandBlock>::get( anEvent, theKey);
    
    if ( ! candBlock )
      ErrMsg(fatal) << "No UsrCandBlock named " << usrblks[j]
		    << " is found. " << endmsg;
    
    UsrBlockBase::const_iterator it= candBlock->begin();
    while ( it!= candBlock->end()) {
      std::string vname= it->first.name();
      if ( it->second == type_int ) {
	addIntVar(tag+vname);
      } else if ( it->second == type_float ) {
	addFloatVar(tag+vname);
      } else if ( it->second == type_double ) {
	addFloatVar(tag+vname);     // Use float in ntuple to store double
      } else if ( it->second == type_short ) {
	addIntVar(tag+vname);
      } else if ( it->second == type_char ) {
	addIntVar(tag+vname);
      } else if ( it->second == type_H3Vector ) {
	addFloatVar(tag+vname+"0");
	addFloatVar(tag+vname+"1");
	addFloatVar(tag+vname+"2");
      } 
      it++;
    }
  }
}


void BtuCandBlock::createPidHTVectors(AbsEvent *anEvent,
				      std::vector<std::string>& selectors,
				      std::string tag) {

  for ( int j=0; j< selectors.size(); j++) {
    addFloatVar(tag+ selectors[j]+ "_w");
    addFloatVar(tag+ selectors[j]+ "_werr");
    addIntVar(tag+ selectors[j]+ "_stat");
  }

}

void BtuCandBlock::fillUsrHTVectors(AbsEvent *anEvent, BtaCandidate *cand, 
				    std::vector<std::string>& usrblks,
				    std::string tag) {
  UsrType<int>   type_int;
  UsrType<short> type_short;  
  UsrType<char>  type_char;
  UsrType<float> type_float;
  UsrType<double> type_double;
  UsrType<Hep3Vector> type_H3Vector;

  // Loop over UsrCandBlock names
  for ( int j=0; j< usrblks.size(); j++) {
    std::string ucblock= usrblks[j];
    // Get UsrData
    IfdStrKey theKey(ucblock.c_str());
    UsrCandBlock *candBlock=
      UsrIfd<UsrCandBlock>::get( anEvent, theKey);;
    
    if ( ! candBlock )
      ErrMsg(fatal) << "No UsrCandBlock named " << ucblock
		    << " is found. " << endmsg;
    
    UsrBlockBase::const_iterator it= candBlock->begin();
    while ( it!= candBlock->end()) {
      std::string vname= it->first.name();
      
      if ( it->second == type_int ) {
	UsrVariable<int> x(vname);
	if ( 0== cand ) x= -999;
	else if ( ! candBlock->get(*cand,x) ) x= -999;
	  //errorGetVar(vname,ucblock);
	_intVars[tag+vname]->append((int)x);
      } else if ( it->second == type_short ) {
	UsrVariable<short> x(vname);
	if ( 0== cand ) x= -999;
	else if ( ! candBlock->get(*cand,x) ) x= -999;
	  //errorGetVar(vname,ucblock);
	_intVars[tag+vname]->append((int)x);
      } else if ( it->second == type_char ) {
	UsrVariable<char> x(vname);
	if ( 0== cand ) x= -999;
	else if ( ! candBlock->get(*cand,x) ) x= -999;
	  //errorGetVar(vname,ucblock);
	_intVars[tag+vname]->append((int)x);
      } else if ( it->second == type_float ) {
	UsrVariable<float> x(vname);
	if ( 0== cand ) x= -999;
	else if ( ! candBlock->get(*cand,x) ) x=-999; 
	  //errorGetVar(vname,ucblock);
	_floatVars[tag+vname]->append((float)x);
      } else if ( it->second == type_double ) {
	UsrVariable<double> x(vname);
	if ( 0== cand ) x= -999;
	else if ( ! candBlock->get(*cand,x) ) x=-999;
	  //errorGetVar(vname,ucblock);
	_floatVars[tag+vname]->append((float)x); // Use float in ntuple to store double
      } else if ( it->second == type_H3Vector ) {
	UsrVariable<Hep3Vector> x(vname);
	Hep3Vector vec(-999.,-999.,-999.);
	if ( ( 0!= cand ) && (candBlock->get(*cand,x)) ) vec=x.value();
	  
	_floatVars[tag+vname+"0"]->append((float)vec[0]);
	_floatVars[tag+vname+"1"]->append((float)vec[1]);
	_floatVars[tag+vname+"2"]->append((float)vec[2]);
      }
      
      it++;
    }
  }
}

void BtuCandBlock::fillPidHTVectors(AbsEvent *anEvent, BtaCandidate *c, 
				    std::vector<std::string>& selectors, std::string tag) {
  
  for ( int j=0; j< selectors.size(); j++) {
    std::map<int,PidWeight> *aMap
      = Ifd<std::map<int,PidWeight> >::get(anEvent,selectors[j].c_str());

    float value(1), error(-1000);
    PidWeight::status_t status(PidWeight::unknown);
    if ( aMap ) {
      PidWeight w= (*aMap)[c->uid()];
      value= w._value;
      error= w._error;
      status= w._status;
    }
    if ( status == PidWeight::unknown ) {
      value= 1;
      error= -1000;
    }

    _floatVars[tag+ selectors[j]+ "_w"]->append(value);
    _floatVars[tag+ selectors[j]+ "_werr"]->append(error);
    _intVars[tag+ selectors[j]+ "_stat"]->append((int)status);
  }
}


void BtuCandBlock::fillTupleColumns(HepTuple *tuple, std::string& nCname) {

  std::string vname;

  std::map<std::string, HTValOrderedVector<float>* >::const_iterator fiter;
  std::map<std::string, HTValOrderedVector<int>* >::const_iterator iiter;
  std::map<std::string, HTValOrderedVector<bool>* >::const_iterator biter;

  // ---> float variables
  fiter= _floatVars.begin();
  while ( fiter != _floatVars.end() ) {
    vname= _block+ fiter->first;
    tuple->column(vname.c_str(), *(fiter->second), nCname.c_str(),0,_block.c_str());
    fiter->second->clear();
    fiter++;
  }
  // ---> int variables
  iiter= _intVars.begin();
  while ( iiter != _intVars.end() ) {
    vname= _block+ iiter->first;
    tuple->column(vname.c_str(), *(iiter->second), nCname.c_str(),0,_block.c_str());
    iiter->second->clear();
    iiter++;
  }
  // ---> bool variables
  biter= _boolVars.begin();
  while ( biter != _boolVars.end() ) {
    vname= _block+ biter->first;
    tuple->column(vname.c_str(), *(biter->second), nCname.c_str(),0,_block.c_str());
    biter->second->clear();
    biter++;
  }

}



//-------
void BtuCandBlock::setKeywordBitmap(std::vector<std::string> keys, 
				    std::vector<bitmap_t> &cm, 
				    std::vector<std::map<std::string, std::vector<std::string> > > &kargs) {

  std::vector<std::string>::const_iterator keysiter= keys.begin();
  while (keysiter!= keys.end() ) {
    bitmap_t themap=0;
    std::map<std::string, std::vector<std::string> > theargs;
    setKeywordBitmap(*keysiter, themap, theargs);
    cm.push_back(themap);
    kargs.push_back(theargs);
    keysiter++;
  }
  
}

//--------
void BtuCandBlock::setKeywordBitmap(std::string keys, bitmap_t &cm, 
				    std::map<std::string, std::vector<std::string> > &kargs){

  std::map<std::string, bool> oldkey;
  Tokenize lineToken(keys);
  
  std::string keyword="";
  while ( ! (keyword= lineToken()).empty()) {
    // find those with arguments, i.e.,  keyword(arg1,arg2...)
    if (keyword.find("(")!= string::npos && keyword.find(")")!= string::npos) {
      std::string::size_type p1pos= keyword.find_first_of("(");
      std::string args= keyword.substr(p1pos);
      keyword= keyword.substr(0,p1pos);
      BtuUtil::chopEnds(args," (\t\n)");
      Tokenize btokens(args);
      std::string bt;
      while ( ! (bt= btokens(",")).empty())
	kargs[keyword].push_back(bt);
    }

    if ( !_keywordBit[keyword] ) {
      ErrMsg(routine) << "The allowed keywords in ntpBlockContent are ";
      std::map<std::string, bitmap_t>::const_iterator kiter= _keywordBit.begin();
      while ( kiter!= _keywordBit.end() ) {
	ErrMsg(routine) << kiter->first << endmsg;
	kiter++;
      }
      ErrMsg(fatal) << "Undefined keyword \"" << keyword 
		    << "\" in ntpBlockContent setting" << endmsg;
    }
    if ( oldkey[keyword] ) {
      ErrMsg(warning) << "You are repeating keyword " << keyword
		      << endmsg;
      continue;
    }
    oldkey[keyword]= true;
    
    cm+= _keywordBit[keyword];
  }

}


void BtuCandBlock::addIntVar(std::string name) {
  if ( _intVars[name]!=0 ) ErrMsg(fatal) << "variable name conflict " 
					 << name << endmsg;
  _intVars[name]= new HTValOrderedVector<int>();
}
void BtuCandBlock::addFloatVar(std::string name) {
  if ( _floatVars[name]!=0 ) ErrMsg(fatal) << "variable name conflict " 
					   << name << endmsg;
  _floatVars[name]= new HTValOrderedVector<float>();
}
void BtuCandBlock::addBoolVar(std::string name) {
  if ( _boolVars[name]!=0 ) ErrMsg(fatal) << "variable name conflict " 
					  << name << endmsg;
  _boolVars[name]= new HTValOrderedVector<bool>();
}

void BtuCandBlock::errorGetVar(std::string vname, std::string block) {
  ErrMsg(fatal) << "Error getting " << vname << " from UsrCandBlock "
		<< block << endmsg ;
}

template <class T>
void BtuCandBlock::deleteHTVectors(std::map<std::string, HTValOrderedVector<T>*> &map) {
  typename std::map<std::string, HTValOrderedVector<T>* >::const_iterator it= map.begin();
  while ( it != map.end() ) {
    if ( it->second ) delete it->second;
    it++;
  }
}


// V0 flight information on XY plane
void BtuCandBlock::calcV0Flight(const BtaCandidate *cand, const BbrPointErr& orig, 
				double &flen, double &flerr, double &cosangle,
				bool fitIfFail ){

  BtaCandidate *vcand(0);

  if ( !cand->decayVtx() || cand->decayVtx()->status() != BtaAbsVertex::Success ) {
    if ( fitIfFail ) {
      BtaCandidate clone(*cand);
      clone.invalidateFit();
      vtxtreefit::Fitter treeFit(clone);
      setBeamConstraint(clone,orig);
      treeFit.fit();
      vcand= new BtaCandidate( treeFit.getFitted(*cand) );
      
      if ( !vcand->decayVtx() || 
	   vcand->decayVtx()->status() != BtaAbsVertex::Success ) return;
      
    } else {
      return;
    }
  }

  const BtaAbsVertex *vtx=  vcand ?  vcand->decayVtx() : cand->decayVtx();

  double vx= vtx->point().x();
  double vy= vtx->point().y();
  double px= vtx->p4().x();
  double py= vtx->p4().y();
  double dx= vx-orig.x();
  double dy= vy-orig.y();

  cosangle= (dx*px + dy*py)/(sqrt(dx*dx+dy*dy)*sqrt(px*px+py*py));

  DifNumber difVx(vx,1,6), difVy(vy,2,6);
  DifNumber difPx(px,3,6), difPy(py,4,6);
  DifNumber difBsx(orig.x(),5,6), difBsy(orig.y(),6,6);
  DifNumber difFlen= sqrt((difVx-difBsx)*(difVx-difBsx) + 
			  (difVy-difBsy)*(difVy-difBsy) );

  HepPoint pt(0,0,0);
  HepSymMatrix ctCov(6,0), xxCov(3),ppCov(3);
  HepMatrix xpCov(3,3);
  
  if ( vcand ) vcand->recoObject()->getAllCovs(pt,xxCov,ppCov,xpCov);
  else cand->recoObject()->getAllCovs(pt,xxCov,ppCov,xpCov);

  ctCov[0][0] = xxCov[0][0];
  ctCov[0][1] = xxCov[0][1];
  ctCov[1][1] = xxCov[1][1];
       
  ctCov[2][2] = ppCov[0][0];
  ctCov[2][3] = ppCov[0][1];
  ctCov[3][3] = ppCov[1][1];
	
  ctCov[0][2] = xpCov[0][0];
  ctCov[0][3] = xpCov[0][1];
  ctCov[1][2] = xpCov[1][0];
  ctCov[1][3] = xpCov[1][1];
	
  ctCov[4][4] = orig.covMatrix()[0][0];
  ctCov[5][5] = orig.covMatrix()[1][1];
	

  double corr= difFlen.correlation(difFlen,ctCov);
  if ( corr >= 0 ) flerr= difFlen.error(ctCov);

  flen= difFlen.number();

  if (vcand) delete vcand;
  return;
}
