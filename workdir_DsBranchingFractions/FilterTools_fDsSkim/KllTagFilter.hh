//--------------------------------------------------------------------------
// File and Version Information:
//
// Description:
//      Class KllTagFilter - tag filtering for K(K*)ll analysis
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Natalia Kuznetsova                Original author
//
// Copyright Information:
//      Copyright (C) 2000                UCSB
//
//------------------------------------------------------------------------     
#ifndef KllTagFilter_HH
#define KllTagFilter_HH

#include "TagModules/TagFilterModule.hh"
#include "Framework/AbsParmGeneral.hh"

class HepTupleManager;
class HepHistogram;
class HepTuple ;

class KllTagFilter : public TagFilterModule {


public:

  KllTagFilter( const char* const theName, 
	      const char* const theDescription );

  virtual ~KllTagFilter();

  virtual AppResult beginJob( AbsEvent* anEvent );
  virtual AppResult event( AbsEvent* anEvent );
  virtual AppResult endJob( AbsEvent* anEvent );
  virtual AppModule* clone(const char* cloneName);

private:

  AbsParmGeneral<double> _electronPCut;
  AbsParmGeneral<double> _muonPCut;
  AbsParmGeneral<double> _R2AllCut;
  AbsParmGeneral<int> _NChargedTrackCut;
  AbsParmGeneral<int> _NLepCut;
  
  AbsParmBool  _makeNtuple ;
  AbsParmBool  _passAll ;
  AbsParmBool  _failAll ;

  int _nReadEvents;
  int _nPassed;

  HepTuple* _tagNtuple ;
  
  int _ntype[16] ;
   

};

#endif 
