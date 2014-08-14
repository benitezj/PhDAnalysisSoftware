//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: BtuTrkContentHots.cc,v 1.3 2006/08/07 21:38:27 mallen Exp $
//
// Description:
//      Class for filling Hit-on-track information.
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

#include "BtaTupleMaker/BtuTrkContentHots.hh"
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
#include "AbsEvent/AbsEvent.hh"
#include "AbsEnv/AbsEnv.hh"
#include "AbsPid/PidInfoSummary.hh"
#include "AbsPid/AbsPidInfo.hh"
#include "DchPid/DchPidInfo.hh"
#include "ProxyDict/IfdStrKey.hh"
#include "ErrLogger/ErrLog.hh"
#include "Beta/BtaCandidate.hh"
#include "CLHEP/Alist/AIterator.h"
#include "HepTuple/HTValOrderedVector.h"
#include "HepTuple/Tuple.h"
#include "BbrStdUtils/Tokenize.hh"
#include "BaBar/Constants.hh"
#include "BetaMicroAdapter/BtaPidQual.hh"
#include "BetaMicroAdapter/BtaCalQual.hh"
#include "TrkBase/TrkRecoTrk.hh"
#include "TrkBase/TrkHotList.hh"
#include "TrkBase/TrkEnums.hh"

using namespace babar::String;

//----------------
// Constructors --
//----------------
BtuTrkContentHots::BtuTrkContentHots() : BtuContentBase() {
}

BtuTrkContentHots::BtuTrkContentHots(std::string block, std::string nprefix, 
				   int maxCands, std::string args) 
  : BtuContentBase(block, nprefix, maxCands),
    _detailSVT(false), _detailDCH(false), _detailEMC(false)  {
  // -----------------------------------------------------------
  // args can be a list of the following separated by a comma
  //   detailSVT
  //   detailDCH
  //   detailEMC
  // or nothing
  //------------------------------------------------------------

  Tokenize tokens(args);
  std::string token;
  while ( ! (token = tokens(",")).empty() ) {
    if ( token == "detailSVT" ) _detailSVT= true;
    else if ( token == "detailDCH" ) _detailDCH= true;
    else if ( token == "detailEMC" ) _detailEMC= true;    
    else
      ErrMsg(fatal) << "Unknown argument " << token << endmsg;
  }
}

//----------------
// Destructors --
//----------------
BtuTrkContentHots::~BtuTrkContentHots() {
}


void BtuTrkContentHots::fillNtpColumns(HepTuple *tuple, AbsEvent *anEvent, 
				       HepAList<BtaCandidate>* trklist) {

  std::string vname, nCname;
  nCname= _nprefix+_block;

  HTValOrderedVector<int> nDch,nSvt,nHit;
  HTValOrderedVector<int> nHitXY, nHitZ;
  HTValOrderedVector<int> nSvtXY, nSvtZ;
  HTValOrderedVector<bool> svtViewXY1, svtViewXY2, svtViewXY3, svtViewXY4, svtViewXY5;
  HTValOrderedVector<bool> svtViewZ1, svtViewZ2, svtViewZ3, svtViewZ4, svtViewZ5;
  HTValOrderedVector<int> nDchXY, nDchZ, dchfirst, dchlast;
  HTValOrderedVector<float> EMCcentX, EMCcentY, EMCcentZ, EMCecal, EMCeraw, EMClat;

  BtaCandidate *cand;
  HepAListIterator<BtaCandidate> iter(*trklist);
  int counter(0);
  while ( (cand= iter()) ) {
    if ( counter>= _maxCands) {
      ErrMsg(warning) << "Number of candidates ("<< trklist->length() 
	              <<") for ntuple block "
                      << _block << " exceeds the limit " << _maxCands << endmsg;
      break;
    }
    counter++;
   
   
    const BtaCalQual *cQual = cand->getMicroAdapter()->getCalQual();   
    const TrkRecoTrk *recoTrk= cand->recoTrk();
    const TrkHotList *hots= recoTrk->hots();


    if( _detailEMC ) {
      if(cQual){
	EMCcentX.append(cQual->centroid().x());
	EMCcentY.append(cQual->centroid().y());
	EMCcentZ.append(cQual->centroid().z());
	
	EMCecal.append(cQual->ecalEnergy());
	EMCeraw.append(cQual->rawEnergy());
	EMClat.append(cQual->lateralMoment());
      }
      else{
	EMCcentX.append(-999.9);
	EMCcentY.append(-999.9);
	EMCcentZ.append(-999.9);
	
	EMCecal.append(-999.9);
	EMCeraw.append(-999.9);
	EMClat.append(-999.9);
      }
    }
    
    if ( _detailSVT || _detailDCH ) {
      nHitXY.append(hots->nHit(TrkEnums::xyView));
      nHitZ.append(hots->nHit(TrkEnums::zView));
      
      if ( _detailSVT ) {
	nSvtXY.append(hots->nSvt(TrkEnums::xyView));
	nSvtZ.append(hots->nSvt(TrkEnums::zView));
	svtViewXY1.append(hots->hasSvtView(TrkEnums::xyView, 1));
	svtViewXY2.append(hots->hasSvtView(TrkEnums::xyView, 2));
	svtViewXY3.append(hots->hasSvtView(TrkEnums::xyView, 3));
	svtViewXY4.append(hots->hasSvtView(TrkEnums::xyView, 4));
	svtViewXY5.append(hots->hasSvtView(TrkEnums::xyView, 5));
	svtViewZ1.append(hots->hasSvtView(TrkEnums::zView, 1));
	svtViewZ2.append(hots->hasSvtView(TrkEnums::zView, 2));
	svtViewZ3.append(hots->hasSvtView(TrkEnums::zView, 3));
	svtViewZ4.append(hots->hasSvtView(TrkEnums::zView, 4));
	svtViewZ5.append(hots->hasSvtView(TrkEnums::zView, 5));
      }
      if ( _detailDCH ) {
	nDchXY.append(hots->nDch(TrkEnums::xyView));
	nDchZ.append(hots->nDch(TrkEnums::zView));
	dchfirst.append(hots->firstDchLayer());
        dchlast.append(hots->lastDchLayer());
      }
    } else {
      nDch.append(hots->nDch());
      nSvt.append(hots->nSvt());
      nHit.append(hots->nHit());
    }
  }
  
  if ( _detailEMC ) {
    vname= _block + "EMCcentX";
    tuple->column(vname.c_str(), EMCcentX, nCname.c_str(),0,_block.c_str());
    vname= _block + "EMCcentY";
    tuple->column(vname.c_str(), EMCcentY, nCname.c_str(),0,_block.c_str());
    vname= _block + "EMCcentZ";
    tuple->column(vname.c_str(), EMCcentZ, nCname.c_str(),0,_block.c_str());
    
    vname= _block + "EMCecal";
    tuple->column(vname.c_str(), EMCecal, nCname.c_str(),0,_block.c_str());
    vname= _block + "EMCeraw";
    tuple->column(vname.c_str(), EMCeraw, nCname.c_str(),0,_block.c_str());
    vname= _block + "EMClat";
    tuple->column(vname.c_str(), EMClat, nCname.c_str(),0,_block.c_str());
  }
  



  if ( _detailSVT || _detailDCH ) {
    vname= _block + "nHitXY";
    tuple->column(vname.c_str(), nHitXY, nCname.c_str(),0,_block.c_str());
    vname= _block + "nHitZ";
    tuple->column(vname.c_str(), nHitZ, nCname.c_str(),0,_block.c_str());

    if ( _detailSVT ) {
      vname= _block + "nSvtXY";
      tuple->column(vname.c_str(), nSvtXY, nCname.c_str(),0,_block.c_str());
      vname= _block + "nSvtZ";
      tuple->column(vname.c_str(), nSvtZ, nCname.c_str(),0,_block.c_str());

      vname= _block + "svtViewXY1";
      tuple->column(vname.c_str(), svtViewXY1, nCname.c_str(),0,_block.c_str());
      vname= _block + "svtViewXY2";
      tuple->column(vname.c_str(), svtViewXY2, nCname.c_str(),0,_block.c_str());
      vname= _block + "svtViewXY3";
      tuple->column(vname.c_str(), svtViewXY3, nCname.c_str(),0,_block.c_str());
      vname= _block + "svtViewXY4";
      tuple->column(vname.c_str(), svtViewXY4, nCname.c_str(),0,_block.c_str());
      vname= _block + "svtViewXY5";
      tuple->column(vname.c_str(), svtViewXY5, nCname.c_str(),0,_block.c_str());    
      vname= _block + "svtViewZ1";
      tuple->column(vname.c_str(), svtViewZ1, nCname.c_str(),0,_block.c_str());
      vname= _block + "svtViewZ2";
      tuple->column(vname.c_str(), svtViewZ2, nCname.c_str(),0,_block.c_str());
      vname= _block + "svtViewZ3";
      tuple->column(vname.c_str(), svtViewZ3, nCname.c_str(),0,_block.c_str());
      vname= _block + "svtViewZ4";
      tuple->column(vname.c_str(), svtViewZ4, nCname.c_str(),0,_block.c_str());
      vname= _block + "svtViewZ5";
      tuple->column(vname.c_str(), svtViewZ5, nCname.c_str(),0,_block.c_str());

    }

    if ( _detailDCH ) {
      vname= _block + "nDchXY";
      tuple->column(vname.c_str(), nDchXY, nCname.c_str(),0,_block.c_str());
      vname= _block + "nDchZ";
      tuple->column(vname.c_str(), nDchZ, nCname.c_str(),0,_block.c_str());  
      vname= _block + "dchFirst";
      tuple->column(vname.c_str(), dchfirst, nCname.c_str(),0,_block.c_str());
      vname= _block + "dchLast";
      tuple->column(vname.c_str(), dchlast, nCname.c_str(),0,_block.c_str());
    }
    
  } else {
    vname= _block + "nSvt";
    tuple->column(vname.c_str(), nSvt, nCname.c_str(),0,_block.c_str());
    vname= _block + "nDch";
    tuple->column(vname.c_str(), nDch, nCname.c_str(),0,_block.c_str());
    vname= _block + "nHit";
    tuple->column(vname.c_str(), nHit, nCname.c_str(),0,_block.c_str());
  }
}

