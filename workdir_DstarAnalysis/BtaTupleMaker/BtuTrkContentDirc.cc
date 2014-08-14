//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: BtuTrkContentDirc.cc,v 1.4 2006/06/07 02:02:27 avtelnov Exp $
//
// Description:
//      Class for filling TRK tracking efficiency information.
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
// History:
//      2006/05/17: Add DCH dE/dx information - Sasha Telnov (Princeton)
//
//
//--------------------------------------------------------------------------
#include "BaBar/BaBar.hh"
 
#include "BtaTupleMaker/BtuTrkContentDirc.hh"
//-------------
// C Headers --
//-------------
#include <assert.h>
 
//---------------
// C++ Headers --
//---------------
 
//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "AbsEvent/AbsEvent.hh"
#include "AbsEnv/AbsEnv.hh"
#include "ProxyDict/IfdStrKey.hh"
#include "ErrLogger/ErrLog.hh"
#include "Beta/BtaCandidate.hh"
#include "CLHEP/Alist/AIterator.h"
#include "HepTuple/HTValOrderedVector.h"
#include "HepTuple/Tuple.h"
#include "BbrStdUtils/Tokenize.hh"
#include "BaBar/Constants.hh"

#include "BetaMicroAdapter/BtaMicroAdapter.hh"
#include "BetaMicroAdapter/BtaPidQual.hh"
#include "DrcPidUtils/DrcConst.hh"
#include "TrkBase/TrkRecoTrk.hh"

using namespace babar::String;
 
//----------------
// Constructors --
//----------------
BtuTrkContentDirc::BtuTrkContentDirc() : BtuContentBase() {
}
 
BtuTrkContentDirc::BtuTrkContentDirc(std::string block, std::string nprefix,
				     int maxCands, std::string args)
  : BtuContentBase(block, nprefix, maxCands) {
  // -----------------------------------------------------------
  // args is a list of particle type, i.e. [ pi, K, p, e, mu ]
  //------------------------------------------------------------
 
  Tokenize tokens(args);
  std::string token;
  
  while ( ! (token = tokens(",")).empty() ) {
    if ( token== "pi" )  _types.push_back(PdtPid::pion);
    else if ( token== "K" ) _types.push_back(PdtPid::kaon);
    else if ( token== "e" ) _types.push_back(PdtPid::electron);
    else if ( token== "mu" ) _types.push_back(PdtPid::muon);
    else if ( token== "p" ) _types.push_back(PdtPid::proton);
    else ErrMsg(fatal) << "Unknown particle type: " << token << ". "
		       << "Must be pi, K, p, mu, or e." << endmsg;
    _typeNames.push_back(token);
  }
}

//----------------
// Destructors --
//----------------
BtuTrkContentDirc::~BtuTrkContentDirc() {
}


void BtuTrkContentDirc::fillNtpColumns(HepTuple *tuple, AbsEvent *anEvent,
				       HepAList<BtaCandidate>* trklist) {
  
  std::string vname, nCname;
  nCname= _nprefix+_block;
  HTValOrderedVector<float> drcTh_v, drcThErr_v, drcDeltaMom_v, dedxdch_v, dchDeltaMom_v;
  HTValOrderedVector<int>   drcNph_v, drcNbgph_v, dedxdchsamp_v;
  BtaCandidate *trk;
  HepAListIterator<BtaCandidate> iter(*trklist);
  int counter(0);
  while ( (trk= iter()) ) {
    if ( counter>= _maxCands) {
      ErrMsg(warning) << "Number of candidates ("<< trklist->length()
		      <<") for ntuple block "
		      << _block << " exceeds the limit " << _maxCands << endmsg;
      break;
    }
    counter++;
    const BtaPidQual *pidQual= trk->getMicroAdapter()->getPidQual();
    if ( pidQual ) {
      drcTh_v.append(pidQual->thetaC());
      drcThErr_v.append(pidQual->thetaCErr() );
      drcDeltaMom_v.append(pidQual->deltaDrcMomentum());
      drcNph_v.append(pidQual->ringNPhot());
      drcNbgph_v.append(pidQual->ringNBkgd());
      dedxdch_v.append(pidQual->dEdXDch());
      dedxdchsamp_v.append(pidQual->nSamplesDeDxDch());
      dchDeltaMom_v.append(pidQual->deltaDchMomentum());
    } else {
      drcTh_v.append(-999);
      drcThErr_v.append(-999);
      drcDeltaMom_v.append(-999);
      drcNph_v.append(-1);
      drcNbgph_v.append(-999);
      dedxdch_v.append(-999);
      dedxdchsamp_v.append(-999);
      dchDeltaMom_v.append(-999);
    }
  }
  vname= _block+ "DrcTh";
  tuple->column(vname.c_str(), drcTh_v, nCname.c_str(),0,_block.c_str());
  vname= _block+ "DrcThErr";
  tuple->column(vname.c_str(), drcThErr_v, nCname.c_str(),0,_block.c_str());
  vname= _block+ "DrcDeltaMom";
  tuple->column(vname.c_str(), drcDeltaMom_v, nCname.c_str(),0,_block.c_str());
  vname= _block+ "DrcNph";
  tuple->column(vname.c_str(), drcNph_v, nCname.c_str(),0,_block.c_str());
  vname= _block+ "DrcNbgph";
  tuple->column(vname.c_str(), drcNbgph_v, nCname.c_str(),0,_block.c_str());
  vname= _block+ "dedxdch";
  tuple->column(vname.c_str(), dedxdch_v, nCname.c_str(),0,_block.c_str());
  vname= _block+ "dedxdchsamp";
  tuple->column(vname.c_str(), dedxdchsamp_v, nCname.c_str(),0,_block.c_str());
  vname= _block+ "DchDeltaMom";
  tuple->column(vname.c_str(), dchDeltaMom_v, nCname.c_str(),0,_block.c_str());
  
  for ( int i=0; i< _types.size(); i++ ) {
    HTValOrderedVector<float> drcThExp_v;
    HTValOrderedVector<int>   drcNphExp_v;
    PdtPid::PidType part_type= _types[i];
    std::string partName= _typeNames[i];

    BtaCandidate *trk;
    HepAListIterator<BtaCandidate> iter2(*trklist);
    counter=0;;
    while ( (trk= iter2()) ) {
      if ( counter>= _maxCands) {
	ErrMsg(warning) << "Number of candidates ("<< trklist->length()
			<<") for ntuple block "
			<< _block << " exceeds the limit " << _maxCands << endmsg;
	break;
      }
      counter++;

      const BtaPidQual *pidQual= trk->getMicroAdapter()->getPidQual();
      if ( pidQual ) {
	const TrkRecoTrk *trkReco= trk->recoTrk();
	double momentum;
	if ( ! trkReco ) {
	  ErrMsg(trace) << "Can't get recoTrk(). Use BtaCandidate::p()" 
			<< endmsg;
	  momentum = trk->p();
	} else if ( ! trkReco->fitResult(part_type) ) {
	  ErrMsg(trace) << "Can't get recoTrk()->fitResult() with particle "
			<< "type " << partName << " Use BtaCandidate::p()"
			<< endmsg;
	  momentum = trk->p();
	} else {
	  momentum= trkReco->fitResult(part_type)->momentum().mag();
	}

	drcThExp_v.append(DrcConst::thetaC(momentum, part_type));
	drcNphExp_v.append(pidQual->ringNExPhot(part_type));
      } else {
	drcThExp_v.append(-999);
	drcNphExp_v.append(-1);
      }
    } // looping trk list

    vname= _block+ "DrcThExp_" + partName;
    tuple->column(vname.c_str(), drcThExp_v, nCname.c_str(),0,_block.c_str());
    vname= _block+ "DrcNphExp_" + partName;
    tuple->column(vname.c_str(), drcNphExp_v, nCname.c_str(),0,_block.c_str());

  } // looping particle types

}
