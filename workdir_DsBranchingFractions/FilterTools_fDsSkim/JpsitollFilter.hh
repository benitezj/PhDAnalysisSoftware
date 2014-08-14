//------------------------------------------------------------------------
// File and Version Information:
//      $Id: JpsitollFilter.hh,v 1.1 2005/11/04 07:25:50 fwinkl Exp $
//
// Description:
//      Class JpsitollFilter
//
//      A tag filter module for the inclusive J/psi skims
//      Filters on:
//              (0 <= R2 <= R2max) ||  BGFMultiHadron
//
// Author List:
//      Frank Winklmeier
//
// Copyright Information:
//      Copyright (C) 2005     Colorado State University
//
//------------------------------------------------------------------------

#ifndef JpsitollFilter_hh
#define JpsitollFilter_hh

#include "TagModules/TagFilterModule.hh"

#include "Framework/AbsParmDouble.hh"



class JpsitollFilter : public TagFilterModule {

public:
  JpsitollFilter( const char * const name = "JpsitollFilter",
				  const char * const descr = "Filter for Jpsitoll skims" );
  ~JpsitollFilter() {};

  virtual AppResult event( AbsEvent *event );
  virtual AppResult beginJob( AbsEvent *event );
  virtual AppResult endJob( AbsEvent* anEvent );


private:
  AbsParmDouble _R2max;

  int _nEvents;
  int _nPassed;
};

#endif
