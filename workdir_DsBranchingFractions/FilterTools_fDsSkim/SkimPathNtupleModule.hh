//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: SkimPathNtupleModule.hh,v 1.5 2004/08/16 01:57:09 bartoldu Exp $
//
// Description:
//      Stores an ntuple to study correlations between streams

// Author List:
//      Yury Kolomensky              Origical author
//------------------------------------------------------------------------

#ifndef SKIMPATHNTUPLEMODULE_HH
#define SKIMPATHNTUPLEMODULE_HH

//----------------------
// Base Class Headers --
//----------------------
#include "Framework/AppModule.hh"
#include <string>
#include <vector>
#include <set>

class HepTuple;

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "Framework/AbsParmBool.hh"

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class SkimPathNtupleModule : public AppModule {

//--------------------
// Instance Members --
//--------------------

public:

    // Constructors
    SkimPathNtupleModule( const char* const theName, 
			  const char* const theDescription );

    // Destructor
    virtual ~SkimPathNtupleModule( );

    virtual AppResult beginJob( AbsEvent* anEvent );

    virtual AppResult event( AbsEvent *ev );

    virtual AppResult endJob( AbsEvent* anEvent );

    virtual AppModule* clone(const char* cloneName);

private:
  AbsParmBool         _dumpNtuple;  

  HepTuple*           _ntuple;

private:
  // static data members that allows instances to communicate. A bit wierd...
  static std::vector<std::string>*   _instances;
  static std::multiset<std::string>*       _passed;

};

#endif 
