//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: SkimPathNtupleModule.cc,v 1.4 2002/06/18 21:38:18 jtinslay Exp $
//
// Description: -- see header
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Yury Kolomensky        original author
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "FilterTools/SkimPathNtupleModule.hh"

//-------------
// C Headers --
//-------------

//---------------
// C++ Headers --
//---------------

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "HepTuple/TupleManager.h"
#include "HepTuple/Tuple.h"
#include "AbsEnv/AbsEnv.hh"
#include "GenEnv/GenEnv.hh"

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------
std::vector<std::string>* SkimPathNtupleModule::_instances  = 
   new std::vector<std::string>;
std::multiset<std::string>* SkimPathNtupleModule::_passed   = 
   new std::multiset<std::string>;

//----------------
// Constructors --
//----------------
SkimPathNtupleModule::SkimPathNtupleModule(const char* const theName, 
					   const char* const theDescription )
  : AppModule( theName, theDescription )
  , _dumpNtuple("dumpNtuple",this,false)
  , _ntuple(0)
{
  commands( )->append( &_dumpNtuple);
}


//--------------
// Destructor --
//--------------

SkimPathNtupleModule::~SkimPathNtupleModule() {}

//--------------
// Operations --
//--------------
AppResult 
SkimPathNtupleModule::beginJob( AbsEvent* anEvent ) { 
  if ( _dumpNtuple.value() ) {
    // create an ntuple
    
    HepTupleManager* manager = gblEnv->getGen()->ntupleManager();
    assert(manager != 0);

    _ntuple = manager->ntuple("skim path ntuple");

  } else {
    // append myself to the list of instances
    _instances->push_back( name() );
  }

  return AppResult::OK;
}

AppResult 
SkimPathNtupleModule::event( AbsEvent *ev ) {

  if ( _dumpNtuple.value() ) {
    // loop over instances and check them against the hash table
    
    for (size_t i=0;i<_instances->size();i++) {

      bool passed(false);
      if (_passed->find((*_instances)[i]) != _passed->end()) passed = true;

      _ntuple->column((*_instances)[i], passed);
    }
    _ntuple->dumpData();
    _passed->clear();
  } else {

    // append myself to the hash table
    _passed->insert( name() );
  }

  
  return AppResult::OK;
}

AppResult 
SkimPathNtupleModule::endJob( AbsEvent* anEvent ) {
  return AppResult::OK;
}

AppModule* 
SkimPathNtupleModule::clone(const char* cloneName)
{
  return new SkimPathNtupleModule(cloneName, "cloned SkimPathNtupleModule");
}
