//------------------------------------------------------------------------
// File and Version Information:
//      $Id: FilterByList.hh,v 1.3 2006/11/28 21:42:54 desilva Exp $
//
// Description:
//      Class FilterByList
//
//      Filters on non-empty candidates lists
//      The logic is the same as in TagFilterByName
//
// Author List:
//      Frank Winklmeier   (adapted from TagModules/TagFilterByName)
//
// Copyright Information:
//      Copyright (C) 2005     Colorado State University
//
//------------------------------------------------------------------------
#ifndef FILTERBYLIST_hh
#define FILTERBYLIST_hh


//----------------------
// Base Class Headers --
//----------------------
#include "Framework/AppFilterModule.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "Framework/AbsParmGeneral.hh"
#include "Framework/AbsParmBool.hh"
#include "AbsParm/AbsParmVector.hh"

class FilterByList : public AppFilterModule {

public:
  FilterByList (const char* const name = "FilterByList",
                const char* const descr = "Filter on candidate lists" );
  ~FilterByList();

  virtual AppResult event( AbsEvent *event );
  virtual AppResult beginJob( AbsEvent *event );
  virtual AppResult endJob( AbsEvent* anEvent );
  virtual void help( int argc, char** argv );

  virtual AppModule* clone(const char* cloneName);

private:
  // Data members

  AbsParmVector<std::string> _orList;
  AbsParmVector<std::string> _andList;
  AbsParmBool  _assertIfMissing;
  AbsParmBool  _flipSign;
  AbsParmGeneral<int> _prescale;

  int _nEvents;
  int _nPassed;
  int _nPrescaled;
};

#endif
