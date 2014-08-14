//--------------------------------------------------------------------------
// File and Version Information:
//
// Description:
//      Class DtoXllTagFilter - tag filtering for D0->ll analysis
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Ajit Mohapatra                Original author
//
// Copyright Information:
//      Copyright (C) 2003                UW Madison
//
//------------------------------------------------------------------------     
#ifndef DtoXllTagFilter_HH
#define DtoXllTagFilter_HH

#include "TagModules/TagFilterModule.hh"
#include "Framework/AbsParmGeneral.hh"

class HepTupleManager;
class HepHistogram;
class HepTuple ;

class DtoXllTagFilter : public TagFilterModule {


public:

  DtoXllTagFilter( const char* const theName, 
	      const char* const theDescription );

  virtual ~DtoXllTagFilter();

  virtual AppResult beginJob( AbsEvent* anEvent );
  virtual AppResult event( AbsEvent* anEvent );
  virtual AppResult endJob( AbsEvent* anEvent );


private:

  AbsParmGeneral<bool>  _makeNtuple ;

  int _nReadEvents;
  int _nPassed;

  HepTuple* _tagNtuple ;

};

#endif 
