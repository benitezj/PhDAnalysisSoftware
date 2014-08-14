
#ifndef DRecoDsToHadronic_HH
#define DRecoDsToHadronic_HH 1

//----------------------
// Base Class Headers --
//----------------------
#include "TagModules/TagFilterModule.hh"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

#include "AbsParm/AbsParmIfdStrKey.hh"
#include "BetaMiniUser/NamedScalers.hh"
#include "Framework/AbsParmDouble.hh"
#include "Framework/AbsParmBool.hh"
#include "UsrData/UsrData.hh"
class AbsEvent;

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class DRecoDsToHadronic : public TagFilterModule {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  DRecoDsToHadronic( const char* const N, const char* const D );
  
  // Destructor
  virtual ~DRecoDsToHadronic( );
  
  // Operations
  
  virtual AppResult           beginJob( AbsEvent* anEvent );
  virtual AppResult           event   ( AbsEvent* anEvent );
  virtual AppResult           endJob  ( AbsEvent* anEvent );
   
protected:

  AbsParmDouble FitProbCut;
  AbsParmIfdStrKey DsHadronicList;
  AbsParmDouble DsHadronicMassCutLo;
  AbsParmDouble DsHadronicMassCutHi;
  AbsParmDouble DsHadronicPstarCut;
  AbsParmDouble DeltaMCut;
  AbsParmDouble DeltaMMean;
  AbsParmBool RequireNoOverlap;
private:
  NamedScalers _scalers; 
  UsrCandBlock UsrBlock;
  AbsParmIfdStrKey UsrBlockName;
  UsrVariable<float> DsPx;
  UsrVariable<float> DsPy;
  UsrVariable<float> DsPz;
  UsrVariable<float> DsE;
  UsrVariable<float> DsCharge;
  UsrVariable<float> GamPx;
  UsrVariable<float> GamPy;
  UsrVariable<float> GamPz;

};

#endif
