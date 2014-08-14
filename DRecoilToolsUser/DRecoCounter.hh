
#ifndef DRecoCounter_HH
#define DRecoCounter_HH 1

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
class AbsEvent ;

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class DRecoCounter : public TagFilterModule {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  DRecoCounter( const char* const N, const char* const D ) ;
  
  // Clone
  virtual AppModule* clone(const char* cloneName) {
    return new DRecoCounter(cloneName, "Clone of DRecoCounter") ; }

  // Destructor
  virtual ~DRecoCounter( ) ;
  
  // Operations
  
  virtual AppResult           beginJob( AbsEvent* anEvent ) ;
  virtual AppResult           event   ( AbsEvent* anEvent ) ;
  virtual AppResult           endJob  ( AbsEvent* anEvent ) ;

  AbsParmDouble counterNumber ;
  
protected:

private:
  NamedScalers     _scalers          ;

} ;

#endif
