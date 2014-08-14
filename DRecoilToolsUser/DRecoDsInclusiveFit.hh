
#ifndef DRecoDsInclusiveFit_HH
#define DRecoDsInclusiveFit_HH 1

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
 
class DRecoDsInclusiveFit : public TagFilterModule {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  DRecoDsInclusiveFit( const char* const N, const char* const D );
  
  // Destructor
  virtual ~DRecoDsInclusiveFit( );
  
  // Operations
  
  virtual AppResult           beginJob( AbsEvent* anEvent );
  virtual AppResult           event   ( AbsEvent* anEvent );
  virtual AppResult           endJob  ( AbsEvent* anEvent );
  
protected:

  AbsParmDouble DsStMassCutLo;
  AbsParmDouble DsStMassCutHi;
  AbsParmBool FitEvt;
  AbsParmDouble FitProbCut;
  AbsParmDouble DsMomentumCut;
  AbsParmBool CorrDsSt;
  AbsParmBool MassConstrainTag;
  AbsParmBool MassConstrainDsSt;

private:
  NamedScalers _scalers; 
  UsrCandBlock DRecoUsrBlock;
  AbsParmIfdStrKey DRecoUsrBlockName;
  UsrVariable<float> TagUncVtxProb;
  UsrVariable<float> TagUncMass;
  UsrVariable<float> DsStUncMass;
  UsrVariable<float> DsUncMass;
  UsrVariable<int> MyGamMCIdx;
  UsrVariable<int> MyTagMCIdx;  
  UsrVariable<int> MyKMCIdx;

  UsrVariable<int> MyXd1MCIdx;
  UsrVariable<int> MyXd2MCIdx;
  UsrVariable<int> MyXd3MCIdx;
  UsrVariable<int> MyXd4MCIdx;
  UsrVariable<int> MyXd5MCIdx;
  UsrVariable<int> MyXd6MCIdx;
  UsrVariable<int> MyXd7MCIdx;
  UsrVariable<int> MyXd8MCIdx;
  UsrVariable<int> MyXd9MCIdx;
  UsrVariable<int> MyXd10MCIdx;
  UsrVariable<float> MyXPi0Mass;

  UsrVariable<float> DsStPxRaw;
  UsrVariable<float> DsStPyRaw;
  UsrVariable<float> DsStPzRaw;
  UsrVariable<float> DsStERaw;
  UsrVariable<float> DsStPxCorr;
  UsrVariable<float> DsStPyCorr;
  UsrVariable<float> DsStPzCorr;
  UsrVariable<float> DsStECorr;
  UsrVariable<float> DsMassUnFit;
  HepLorentzVector DsStp4Corrected(HepLorentzVector DsStp4) ;
};

#endif
