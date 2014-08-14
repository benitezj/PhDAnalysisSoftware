//-----------------------------------------------------------------------------------//
//                                                                                   //
//   Filter module for the B -> pi/pi0/eta/eta'/rho/rho0/omega/gamma l nu analyses   //
//                                                                                   //
//     Sylvie Brunet  2003 Universite de Montreal (BaBar)                            //
//     David Cote     2003 Universite de Montreal                                    //
//                                                                                   //
//   Documentation: BAD 740                                                          //
//                                                                                   //
//-----------------------------------------------------------------------------------//

#ifndef XSLBTOXULNUFILTER_HH
#define XSLBTOXULNUFILTER_HH

//----------------------
// Base Class Headers --
//----------------------
#include "TagModules/TagFilterModule.hh"

#include "AbsParm/AbsParmIfdStrKey.hh"
#include "Framework/AbsParmBool.hh"
#include "Framework/AbsParmDouble.hh"

#include "UsrData/UsrVariable.hh"
#include "UsrData/UsrEventBlock.hh"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

class HepHistogram;
class HepTuple;


//Necessaire d'inclure BtaCandidate pour pouvoir avoir des objets de la classe
//tels que HepLorentzVector, HepPoint, etc.
#include "Beta/BtaCandidate.hh"
class AbsEvent;

class BtaMcAssoc;
class EventInfo;
class BtaBooster;


//		---------------------
// 		-- Class Interface --
//		---------------------
 

class XSLBtoXulnuFilter : public TagFilterModule {

//--------------------
// Instance Members --
//--------------------

public:

    // Constructors
    XSLBtoXulnuFilter( const char* const theName, const char* const theDescription );

    // Destructor
    virtual ~XSLBtoXulnuFilter( );

    // Operations
    virtual AppResult           beginJob( AbsEvent* anEvent );
    virtual AppResult           event   ( AbsEvent* anEvent );
    virtual AppResult           endJob  ( AbsEvent* anEvent );
      
  
protected:
  AbsParmIfdStrKey  _eventInfoList;
  AbsParmIfdStrKey  _InputLeptonList;
  AbsParmIfdStrKey  _InputPiList;
  AbsParmIfdStrKey  _InputPi0List;
  AbsParmIfdStrKey  _InputEtaList;
  AbsParmIfdStrKey  _InputEtapList;
  AbsParmIfdStrKey  _InputRho0List;
  AbsParmIfdStrKey  _InputRhoCList;
  AbsParmIfdStrKey  _InputOmegaList;
  AbsParmIfdStrKey  _InputGammaList;
  AbsParmIfdStrKey  _OutputYList;
  AbsParmIfdStrKey  _eventData;

  HepAList<BtaCandidate> NewLepList;
  HepAList<BtaCandidate> NewPiList;
  HepAList<BtaCandidate> NewPi0List;
  HepAList<BtaCandidate> NewEtaList;
  HepAList<BtaCandidate> NewEtapList;
  HepAList<BtaCandidate> NewOmegaList;
  HepAList<BtaCandidate> NewRho0List;
  HepAList<BtaCandidate> NewRhoCList;
  HepAList<BtaCandidate> NewGammaList;

  HepAList<BtaCandidate>* _BasicTrkList;
  HepAList<BtaCandidate>* _BasicBumpList;
  HepAList<BtaCandidate>* _piLooseLH;


  double _mB0;
  double _mBp;

private:

  void MakeHadronsAndLeptonsList( AbsEvent* anEvent );
  double cosTT( BtaCandidate* Y );
  double cosBY(double Ebeam, double mB, double mY, double pY);

  bool SurvivedTrigger( AbsEvent* anEvent );
  bool SurvivedISH( AbsEvent* anEvent );
  bool SurvivedR2All( AbsEvent* anEvent );
  bool pionPID( BtaCandidate* candid );
  virtual AppResult QuitEvent();
  virtual AppModule* clone(const char* cloneName);


  AbsParmDouble    _refBeamEnergy;

  //UsrEventBlock
  UsrEventBlock _eventBlock;
  UsrVariable<int> _decayMode;

  //evt cuts
  AbsParmDouble    _r2allMAX;
  AbsParmDouble    _ishMIN;
  AbsParmDouble    _isnhMIN;
  AbsParmDouble    _trigMIN;

  //trk building cuts
  AbsParmDouble  _pLABmin_electron;
  AbsParmDouble  _pLABmin_muon;
  AbsParmDouble  _thetaLABmin_electron;
  AbsParmDouble  _thetaLABmin_muon;
  AbsParmDouble  _thetaLABmax_electron;
  AbsParmDouble  _thetaLABmax_muon;
  AbsParmDouble  _pLABmax_pi0;
  AbsParmDouble  _pLABmax_eta;
  AbsParmDouble  _pLABmax_etap;
  AbsParmDouble  _pLABmax_rhoC;
  AbsParmDouble  _pLABmax_rho0;
  AbsParmDouble  _pLABmax_omega;
  AbsParmDouble  _pLABmax_gamma;
  AbsParmDouble  _massmin_rhoC;
  AbsParmDouble  _massmax_rhoC;
  AbsParmDouble  _massmin_rho0;
  AbsParmDouble  _massmax_rho0;

  //Y selection
  AbsParmDouble  _cosBYmin;
  AbsParmDouble  _cosBYmax;
  AbsParmDouble  _cosBYLNGmin;
  AbsParmDouble  _cosBYLNGmax;
  AbsParmDouble  _pStar2Dslope_pi;
  AbsParmDouble  _pStar2Dslope_pi0;
  AbsParmDouble  _pStar2Dslope_eta;
  AbsParmDouble  _pStar2Dslope_etap;
  AbsParmDouble  _pStar2Dslope_rhoC;
  AbsParmDouble  _pStar2Dslope_rho0;
  AbsParmDouble  _pStar2Dslope_omega;
  AbsParmDouble  _pStar2Dsum_eta;
  AbsParmDouble  _pStar2Dsum_pi0;
  AbsParmDouble  _pStar2Dsum_pi;
  AbsParmDouble  _pStar2Dsum_etap;
  AbsParmDouble  _pStar2Dsum_rhoC;
  AbsParmDouble  _pStar2Dsum_rho0;
  AbsParmDouble  _pStar2Dsum_omega;
  AbsParmDouble  _pXuUpsMin_pi;
  AbsParmDouble  _pXuUpsMin_pi0;
  AbsParmDouble  _pXuUpsMin_eta;
  AbsParmDouble  _pXuUpsMin_etap;
  AbsParmDouble  _pXuUpsMin_rho0;
  AbsParmDouble  _pXuUpsMin_rhoC;
  AbsParmDouble  _pXuUpsMin_omega;
  AbsParmDouble  _pLepUpsMin_pi;
  AbsParmDouble  _pLepUpsMin_pi0;
  AbsParmDouble  _pLepUpsMin_eta;
  AbsParmDouble  _pLepUpsMin_etap;
  AbsParmDouble  _pLepUpsMin_rho0;
  AbsParmDouble  _pLepUpsMin_rhoC;
  AbsParmDouble  _pLepUpsMin_omega;
  AbsParmDouble  _nTrkMin;
  AbsParmDouble  _cosTTMax;

  //bool
  AbsParmBool _analyzeChargedPi;
  AbsParmBool _analyzeNeutralPi;
  AbsParmBool _analyzeEta;
  AbsParmBool _analyzeEtap;
  AbsParmBool _analyzeChargedRho;
  AbsParmBool _analyzeNeutralRho;
  AbsParmBool _analyzeOmega;
  AbsParmBool _analyzeGamma;
  AbsParmBool _MyVerbose;
  AbsParmBool _doPionPidYourself;

  BtaBooster* _CMS;  //created and deleted in event (after SurvivedEventCuts() )
  EventInfo* _eventInfo;

  //For Verbose stuff...
  float  _TotalNbEvents;
  float  _NbAfterTrigger;
  float  _NbAfterISHCuts;
  float  _NbAfterR2AllCuts;
  float  _NbAfterOneLeptonTight;
  float  _NbAfterEventCuts;
  float  _NbAllModesAfterCoupleCuts;
  float  _NbPiAfterCoupleCuts;
  float  _NbPiAfterLeptonSelCuts;
  float  _NbPiAfterTrkNb;
  float  _NbPiAfterCosBY;
  float  _NbPiAfterpStar2D;
  float  _NbPiAfterCosTT;
  float  _NbPi0AfterCoupleCuts;
  float  _NbPi0AfterTrkNb;
  float  _NbPi0AfterLeptonSelCuts;
  float  _NbPi0AfterCosBY;
  float  _NbPi0AfterpStar2D;
  float  _NbPi0AfterCosTT;
  float  _NbEtaAfterCoupleCuts;
  float  _NbEtaAfterLeptonSelCuts;
  float  _NbEtaAfterCosBY;
  float  _NbEtaAfterpStar2D;
  float  _NbEtaAfterCosTT;
  float  _NbEtapAfterCoupleCuts;
  float  _NbEtapAfterLeptonSelCuts;
  float  _NbEtapAfterCosBY;
  float  _NbEtapAfterpStar2D;
  float  _NbEtapAfterCosTT;
  float  _NbRhoCAfterCoupleCuts;
  float  _NbRhoCAfterLeptonSelCuts;
  float  _NbRhoCAfterCosBY;
  float  _NbRhoCAfterpStar2D;
  float  _NbRhoCAfterCosTT;
  float  _NbRho0AfterCoupleCuts;
  float  _NbRho0AfterLeptonSelCuts;
  float  _NbRho0AfterCosBY;
  float  _NbRho0AfterpStar2D;
  float  _NbRho0AfterCosTT;
  float  _NbOmeAfterCoupleCuts;
  float  _NbOmeAfterLeptonSelCuts;
  float  _NbOmeAfterCosBY;
  float  _NbOmeAfterpStar2D;
  float  _NbOmeAfterCosTT;
  float  _NbGammaAfterCoupleCuts;
  float  _NbGammaAfterLeptonSelCuts;
  float  _NbGammaAfterCosBY;
  float  _NbGammaAfterCosTT;
  float _NbCoupleAllModesAfterAllCuts;
  float _NbCouplePiAfterAllCuts;
  float _NbCouplePi0AfterAllCuts;
  float _NbCoupleEtaAfterAllCuts;
  float _NbCoupleEtapAfterAllCuts;
  float _NbCoupleRhoCAfterAllCuts;
  float _NbCoupleRho0AfterAllCuts;
  float _NbCoupleOmegaAfterAllCuts;
  float _NbCoupleGammaAfterAllCuts;
};




#endif







