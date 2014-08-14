
#ifndef DRecoDsToKKpi_HH
#define DRecoDsToKKpi_HH 1

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
 
class DRecoDsToKKpi : public TagFilterModule {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  DRecoDsToKKpi( const char* const N, const char* const D );
  
  // Destructor
  virtual ~DRecoDsToKKpi( );
  
  // Operations  
  virtual AppResult           beginJob( AbsEvent* anEvent );
  virtual AppResult           event   ( AbsEvent* anEvent );
  virtual AppResult           endJob  ( AbsEvent* anEvent );
  
protected:

  AbsParmDouble DsMassCutLo;
  AbsParmDouble DsMassCutHi;
  AbsParmDouble DeltaMCutLo;
  AbsParmDouble DeltaMCutHi;
  AbsParmDouble FitProbCut;
  AbsParmDouble DsMomentumCut;
  AbsParmDouble DsStMassCutLo;
  AbsParmDouble DsStMassCutHi;
  AbsParmDouble NMaxExtraTrk;
  AbsParmDouble MaxExtraEnergy;
  
  AbsParmIfdStrKey KList;
  AbsParmIfdStrKey PiList;

  AbsParmIfdStrKey ExtraTrkList;
  AbsParmIfdStrKey ExtraNeutralList;

  AbsParmBool CutSelectedCands;
  
private:
  NamedScalers _scalers; 
  UsrCandBlock DRecoDsToKKpiUsrBlock;
  AbsParmIfdStrKey DRecoDsToKKpiUsrBlockName;
  UsrVariable<float> KKpiMass;//Ds mass
  UsrVariable<float> KKpiDeltaM;//m(Ds*)-m(Ds)
  UsrVariable<float> KKpiDsStMass;//m(Ds gamma)
  UsrVariable<float> KKpiPstar;//Ds p*
  UsrVariable<float> KKpiAngle;//Ds angle to Tag
  UsrVariable<float> KKMass;//m(K-K+)
  UsrVariable<float> KpiMass;//m(K-pi+)
  UsrVariable<int>   KKpiNExtraTrk;//number of extra tracks not overlapping with Upsilon or Ds
  UsrVariable<int>   KKpiNExtraNeutral;//number of extra photons not overlapping with Upsilon or Ds
  UsrVariable<float> KKpiExtraNeutralE;//extra neutral energy
  UsrVariable<int>   KKpiDsMCIdx;
  UsrVariable<int>   KKpiDsStMCIdx;

};

#endif
