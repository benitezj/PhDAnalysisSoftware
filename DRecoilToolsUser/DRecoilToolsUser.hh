//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: DRecoilToolsUser.hh,v 1.2 2009/02/09 19:29:58 aidanrc Exp $
//
// Description:
//	Class MyAnalysis - the barest outline of a Beta
//      Analysis, suitable for simple filling-in
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Bob Jacobsen                    Original author
//
// Copyright Information:
//	Copyright (C) 1997		Lawrence Berkeley Laboratory
//
//------------------------------------------------------------------------
#ifndef DRECOILTOOLSUSER_HH
#define DRECOILTOOLSUSER_HH

//----------------------
// Base Class Headers --
//----------------------
#include "Framework/AppModule.hh"

//------------------------------------
// Collaborating Class Header files --
//------------------------------------
#include "AbsParm/AbsParmIfdStrKey.hh"
#include "Beta/BtaCandidate.hh"
#include "BetaCoreTools/BtaMcAssoc.hh"
#include "BetaMC/BtaMcAssocGHit.hh"
#include "CLHEP/Alist/AList.h"
#include "ErrLogger/ErrLog.hh"
#include "ProxyDict/IfdKey.hh"

#include "UsrData/UsrData.hh"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class DRecoilToolsUser : public AppModule {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  DRecoilToolsUser( const char* const theName,
	 const char* const theDescription );
  
  // Destructor
  virtual ~DRecoilToolsUser( );
  
  // Operations
  
  virtual AppResult           beginJob( AbsEvent* anEvent );
  virtual AppResult           event   ( AbsEvent* anEvent );
  virtual AppResult           endJob  ( AbsEvent* anEvent );
  
  
protected:
  
  // The following are sample parameters
  AbsParmIfdStrKey _eventInfoList;
  AbsParmIfdStrKey _outputList;

  AbsParmIfdStrKey _pion_list;
  AbsParmIfdStrKey _kaon_list;
  AbsParmIfdStrKey _proton_list;
  AbsParmIfdStrKey _muon_list;
  AbsParmIfdStrKey _electron_list;
  AbsParmIfdStrKey _gamma_list;
  AbsParmIfdStrKey _pi0_list ;
  AbsParmIfdStrKey _ks_list ;

  AbsParmIfdStrKey _tag1_list;
  AbsParmIfdStrKey _tag2_list;
  AbsParmIfdStrKey _tag3_list;
  AbsParmIfdStrKey _tag4_list;
  AbsParmIfdStrKey _tag5_list;
  AbsParmIfdStrKey _tag6_list;
  AbsParmIfdStrKey _tag7_list;
  AbsParmIfdStrKey _tag8_list;
  AbsParmIfdStrKey _tag9_list;

  AbsParmIfdStrKey _tag10_list;
  AbsParmIfdStrKey _tag11_list;
  AbsParmIfdStrKey _tag12_list;
  AbsParmIfdStrKey _tag13_list;
  AbsParmIfdStrKey _tag14_list;
  AbsParmIfdStrKey _tag15_list;
  AbsParmIfdStrKey _tag16_list;
  AbsParmIfdStrKey _tag17_list;
//AbsParmIfdStrKey _tag18_list;
  AbsParmIfdStrKey _tag19_list;
  
  AbsParmIfdStrKey _tag20_list;
  AbsParmIfdStrKey _tag21_list;
  AbsParmIfdStrKey _tag22_list;
  AbsParmIfdStrKey _tag23_list;
  AbsParmIfdStrKey _tag24_list;
  AbsParmIfdStrKey _tag25_list;
  AbsParmIfdStrKey _tag26_list;
  AbsParmIfdStrKey _tag27_list;
  AbsParmIfdStrKey _tag28_list;

  AbsParmIfdStrKey _double_tag_list;

  HepLorentzVector p4MissCM_pion(AbsEvent*);
  HepLorentzVector p4Miss_pion(AbsEvent*);
  HepLorentzVector p4MissCM_cascade(AbsEvent*, int, int) ;
  float CosThMiss(AbsEvent*) ;
  HepLorentzVector CMBoost(AbsEvent*) ;
  BbrLorentzVectorErr Upsp4(AbsEvent*) ;
  float CM_dot_product(AbsEvent*, BtaCandidate*, BtaCandidate*) ;
  void show_p4(BtaCandidate*) ;
  void show_p4(HepLorentzVector) ;

private:

  HepAList<BtaCandidate>* locateList(AbsEvent*, const std::string& s);
  bool is_a_tag(int);
  HepAList<BtaCandidate>* get_tag_list(AbsEvent*, int ) ;

  int _n_events ;
  int _signal_muon ;
  int _signal_electron ;
  int _signal_pion ;
  int _signal_rho ;
  int _signal_gamma ;
  int _signal_pi0 ;

  UsrCandBlock  _D_block ;
  UsrCandBlock  _Ups_block ;
  UsrEventBlock _event_block ;

  UsrVariable<int>   _tag_mode ;
  UsrVariable<int>   _RS ;
  UsrVariable<int>   _WS ;
  UsrVariable<int>   _FH ;
  UsrVariable<int>   _BH ;
  UsrVariable<int>   _FH_RS ;
  UsrVariable<int>   _FH_WS ;
  UsrVariable<int>   _BH_RS ;
  UsrVariable<int>   _BH_WS ;
  UsrVariable<int>   _is_a_tag ;
  UsrVariable<int>   _is_DT ;

  UsrVariable<int>   _n_tag1 ;
  UsrVariable<int>   _n_tag2 ;
  UsrVariable<int>   _n_tag3 ;
  UsrVariable<int>   _n_tag4 ;
  UsrVariable<int>   _n_tag5 ;
  UsrVariable<int>   _n_tag6 ;
  UsrVariable<int>   _n_tag7 ;
  UsrVariable<int>   _n_tag8 ;
  UsrVariable<int>   _n_tag9 ;
  UsrVariable<int>   _n_tag10 ;
  UsrVariable<int>   _n_tag11 ;
  UsrVariable<int>   _n_tag12 ;
  UsrVariable<int>   _n_tag13 ;
  UsrVariable<int>   _n_tag14 ;
  UsrVariable<int>   _n_tag15 ;
  UsrVariable<int>   _n_tag16 ;
  UsrVariable<int>   _n_tag17 ;
  //  UsrVariable<int>   _n_tag18 ;
  UsrVariable<int>   _n_tag19 ;
  UsrVariable<int>   _n_tag20 ;
  UsrVariable<int>   _n_tag21 ;
  UsrVariable<int>   _n_tag22 ;
  UsrVariable<int>   _n_tag23 ;
  UsrVariable<int>   _n_tag24 ;
  UsrVariable<int>   _n_tag25 ;
  UsrVariable<int>   _n_tag26 ;
  UsrVariable<int>   _n_tag27 ;
  UsrVariable<int>   _n_tag28 ;
  UsrVariable<int>   _n_RS ;
  UsrVariable<int>   _n_WS ;
  UsrVariable<int>   _n_FH ;
  UsrVariable<int>   _n_BH ;
  UsrVariable<int>   _n_FH_RS ;
  UsrVariable<int>   _n_FH_WS ;
  UsrVariable<int>   _n_BH_RS ;
  UsrVariable<int>   _n_BH_WS ;
  UsrVariable<int>   _is_sig ;
  UsrVariable<int>   _is_fitted ;
  UsrVariable<int>   _n_signal_gamma ;
  UsrVariable<int>   _visible_charge ;

  UsrVariable<float> _mMiss_pion ;
  UsrVariable<float> _p3MissCM_pion ;
  UsrVariable<float> _pxMissCM_pion ;
  UsrVariable<float> _pyMissCM_pion ;
  UsrVariable<float> _pzMissCM_pion ;
  UsrVariable<float> _EMissCM_pion ;

  UsrVariable<float> _mMiss_cascade ;
  UsrVariable<float> _p3MissCM_cascade ;
  UsrVariable<float> _pxMissCM_cascade ;
  UsrVariable<float> _pyMissCM_cascade ;
  UsrVariable<float> _pzMissCM_cascade ;
  UsrVariable<float> _EMissCM_cascade ;

  UsrVariable<float> _CosThMiss_pion ;

  UsrVariable<float> _tag_pxCM ;
  UsrVariable<float> _tag_pyCM ;
  UsrVariable<float> _tag_pzCM ;
  UsrVariable<float> _tag_energyCM;
  UsrVariable<float> _tag_mass ;
  UsrVariable<float> _tag_charge ;

  UsrVariable<float> _K_pxCM ;
  UsrVariable<float> _K_pyCM ;
  UsrVariable<float> _K_pzCM ;
  UsrVariable<float> _K_energyCM ;
  UsrVariable<float> _K_mass ;
  UsrVariable<float> _K_charge ;

  UsrVariable<float> _X_pxCM ;
  UsrVariable<float> _X_pyCM ;
  UsrVariable<float> _X_pzCM ;
  UsrVariable<float> _X_energyCM ;
  UsrVariable<float> _X_mass ;
  UsrVariable<float> _X_charge ;

  UsrVariable<float> _Dsstarpre_pxCM ;
  UsrVariable<float> _Dsstarpre_pyCM ;
  UsrVariable<float> _Dsstarpre_pzCM ;
  UsrVariable<float> _Dsstarpre_energyCM ;
  UsrVariable<float> _Dsstarpre_mass ;
  UsrVariable<float> _Dsstarpre_charge ;

  UsrVariable<float> _Dsstarpost_pxCM ;
  UsrVariable<float> _Dsstarpost_pyCM ;
  UsrVariable<float> _Dsstarpost_pzCM ;
  UsrVariable<float> _Dsstarpost_energyCM ;
  UsrVariable<float> _Dsstarpost_mass ;
  UsrVariable<float> _Dsstarpost_charge ;

  UsrVariable<float> _Ds_pxCM ;
  UsrVariable<float> _Ds_pyCM ;
  UsrVariable<float> _Ds_pzCM ;
  UsrVariable<float> _Ds_energyCM ;
  UsrVariable<float> _Ds_mass ;
  UsrVariable<float> _Ds_charge ;
  UsrVariable<float> _Ds_premass ;

  UsrVariable<float> _mu_pxCM ;
  UsrVariable<float> _mu_pyCM ;
  UsrVariable<float> _mu_pzCM ;
  UsrVariable<float> _mu_energyCM ;
  UsrVariable<float> _mu_charge ;

  UsrVariable<float> _nu_pxCM ;
  UsrVariable<float> _nu_pyCM ;
  UsrVariable<float> _nu_pzCM ;
  UsrVariable<float> _nu_energyCM ;
  UsrVariable<float> _nu_mass ;
  UsrVariable<float> _nu_deltap3 ;

  UsrVariable<float> _gamma_pxCM ;
  UsrVariable<float> _gamma_pyCM ;
  UsrVariable<float> _gamma_pzCM ;
  UsrVariable<float> _gamma_energyCM ;

  UsrVariable<float> _Ds_testmass ;
  UsrVariable<float> _nu_testmass_a ;
  UsrVariable<float> _nu_testmass_b ;
  UsrVariable<int>   _truthmatched ;

  BtaMcAssoc *truthMap;
  AbsParmIfdStrKey _btaTruthList;
  HepAList<BtaCandidate> *mcList;
};

#endif
