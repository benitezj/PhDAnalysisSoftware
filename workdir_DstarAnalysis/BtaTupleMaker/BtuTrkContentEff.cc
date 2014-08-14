//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: BtuTrkContentEff.cc,v 1.2 2004/11/10 01:02:50 fwinkl Exp $
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
//--------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

#include "BtaTupleMaker/BtuTrkContentEff.hh"
//-------------
// C Headers --
//-------------
#include <assert.h>

//---------------
// C++ Headers --
//---------------
#include <stdio.h>
#include <math.h>

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
#include "EffTable/EffTable.hh"

using namespace babar::String;

//----------------
// Constructors --
//----------------
BtuTrkContentEff::BtuTrkContentEff() : BtuContentBase() {
}

BtuTrkContentEff::BtuTrkContentEff(std::string block, std::string nprefix, 
				   int maxCands, std::string args) 
  : BtuContentBase(block, nprefix, maxCands) {
  // -----------------------------------------------------------
  // args can be a list of the following separated by a comma
  //    ave
  //    charge
  //    <a file name contains tracking efficiency>
  //------------------------------------------------------------

  Tokenize tokens(args);
  std::string token;
  while ( ! (token = tokens(",")).empty() )
    _tables.push_back(token);
}

//----------------
// Destructors --
//----------------
BtuTrkContentEff::~BtuTrkContentEff() {
}


void BtuTrkContentEff::fillNtpColumns(HepTuple *tuple, AbsEvent *anEvent, 
				      HepAList<BtaCandidate>* trklist) {

  // find out the "multiplicity" of this event
  HepAList<BtaCandidate>* gtvl= 
    Ifd< HepAList< BtaCandidate > >::get(anEvent, "GoodTracksVeryLoose");
  if (! gtvl)
    ErrMsg(fatal) << "Can't get GoodTracksVeryLoose" << endmsg;
  int mult= gtvl->length();

  std::string app="";
  for ( int i=0; i< _tables.size(); i++) {
    EffTable *tpos(0), *tneg(0);
    if ( _tables[i]== "ave" ) {
      static const IfdStrKey avetk("/physicstools/trk/svt_tables/average");
      static const IfdStrKey alltk("/physicstools/trk/svt_tables/all");
      tpos= getEffTable(avetk);
      if ( ! tpos ) tpos=  getEffTable(alltk);
      if ( ! tpos ) ErrMsg(fatal) << "Can't get either EffTable :\n"
				  << avetk << "  or\n" << alltk << endmsg;
      tneg= tpos;
      app= "";  // no appendix in var name
    } else if ( _tables[i]== "charge" ) {
      static const IfdStrKey postk("/physicstools/trk/svt_tables/pos");
      static const IfdStrKey negtk("/physicstools/trk/svt_tables/neg");    
      tpos= getEffTable(postk);
      if ( ! tpos ) ErrMsg(fatal) << "Can't get EffTable " << postk << endmsg;
      tneg= getEffTable(negtk);
      if ( ! tneg ) ErrMsg(fatal) << "Can't get EffTable " << negtk << endmsg;
      app= "ch";
    } else {
      tpos= getEffTable(_tables[i].c_str());
      tneg= tpos;
      char buf[4];
      sprintf(buf,"%d",i);
      app= buf;
    }      

    HTValOrderedVector<float> eff_v,err_v;

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
      std::vector<double> vect(4,0);
      vect[0]= cand->pt();
      vect[1]= cand->p3().theta() * Constants::radToDegrees;
      vect[2]= cand->p3().phi() * Constants::radToDegrees;
      vect[3]= mult;
      double eff(0), err(0);
      bool ok(false);
      if ( cand->charge() > 0 ) 
	ok= tpos->get_efficiency_and_error(eff,err,vect);
      else if ( cand->charge() < 0 ) 
	ok= tneg->get_efficiency_and_error(eff,err,vect);
      else 
	ErrMsg(error) << "Track list has a neutral charged candidate "
		      << cand << endmsg;
      if ( ! ok ) {
	//ErrMsg(error) << "Error getting tracking eff " << endmsg;
	eff=1;  err= -1;
      }
      eff_v.append((float)eff);
      err_v.append((float)err);
    }

    std::string vname, nCname;
    nCname= _nprefix+_block;

    vname= _block+ app + "Eff";
    tuple->column(vname.c_str(), eff_v, nCname.c_str(),0,_block.c_str());
    vname= _block+ app + "Efferr";
    tuple->column(vname.c_str(), err_v, nCname.c_str(),0,_block.c_str());

  }

}

EffTable* BtuTrkContentEff::getEffTable(const char* key) {
  EffTable *table= Ifd<EffTable>::get( gblPEnv,key);
  return table;
}

EffTable* BtuTrkContentEff::getEffTable(const IfdStrKey& key) {
  EffTable *table= Ifd<EffTable>::get( gblPEnv,key);
  return table;
}

