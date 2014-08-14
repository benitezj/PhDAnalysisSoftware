
#ifndef DRecoPrintTree_HH
#define DRecoPrintTree_HH 1

//----------------------
// Base Class Headers --
//----------------------
#include "TagModules/TagFilterModule.hh"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
#include "Beta/BtaCandidate.hh"
#include "BetaMiniUser/NamedScalers.hh"
#include "Framework/AbsParmBool.hh"
#include "BetaCoreTools/BtaPrintTree.hh"
class AbsEvent;

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class DRecoPrintTree : public TagFilterModule {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  DRecoPrintTree( const char* const N, const char* const D );
  
  // Destructor
  virtual ~DRecoPrintTree( );
  
  // Operations
  
  virtual AppResult           beginJob( AbsEvent* anEvent );
  virtual AppResult           event   ( AbsEvent* anEvent );
  virtual AppResult           endJob  ( AbsEvent* anEvent );


  AbsParmBool PrintMCTruth;
  AbsParmBool DefaultRecoPrint;
  
  
protected:


private:
  NamedScalers _scalers; 
  BtaPrintTree CandPrinter;

  void printCand(BtaCandidate* c);
};

#endif
