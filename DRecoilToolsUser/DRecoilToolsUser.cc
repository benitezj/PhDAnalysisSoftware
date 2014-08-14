//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: DRecoilToolsUser.cc,v 1.2 2009/02/09 19:29:57 aidanrc Exp $
//
// Description:
//	Class MyAnalysis - a bare outline of an analysis which should
//      run on either the mini or micro, suitable for filling in with 
//      additional code.
//
//      The "event" entry point is where you should add code to 
//      process event data; define histograms & ntuples in "begin"
//
//      You might want to rename this file and class (module)
//      before making any local modifications.  
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Bob Jacobsen                    Original Author
//
// Copyright Information:
//	Copyright (C) 1994, 1995, 1996, 1997		Lawrence Berkeley Laboratory
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "DRecoilToolsUser/DRecoilToolsUser.hh"

//-------------
// C Headers --
//-------------
#include <assert.h>

//---------------
// C++ Headers --
//---------------
#include <math.h>
#include <iostream>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "AbsEnv/AbsEnv.hh"

#include "AbsEvent/AbsEvent.hh"
#include "AbsEvent/getTmpAList.hh"

#include "Beta/EventInfo.hh"

#include "BaBar/Constants.hh"

#include "Beta/BtaCandidate.hh"
#include "BetaCoreTools/BtaFoxWolfMom.hh"
#include "BetaCoreTools/BtaPrintTree.hh"
#include "BetaCoreTools/BtaMcAssoc.hh"
#include "BetaCoreTools/BtaOpMakeTree.hh"
#include "BetaCoreTools/BtaThrust.hh"
#include "BetaCoreTools/BtaTreeNavigator.hh"

#include "BetaMicroAdapter/BtaMicroAdapter.hh"
#include "BetaMicroAdapter/BtaCalQual.hh"
#include "BetaMicroAdapter/BtaPidQual.hh"
#include "BetaMicroAdapter/BtaTrkQual.hh"
#include "BetaMicroAdapter/BtaPidInfo.hh"
#include "BetaMicroAdapter/BtaIfrQual.hh"

#include "BetaCoreTools/BtaMcAssoc.hh"
#include "BtaTupleMaker/BtuUtil.hh"

#include "CLHEP/Alist/AList.h"
#include "CLHEP/Alist/AIterator.h"

#include "ErrLogger/ErrLog.hh"

#include "GenEnv/GenEnv.hh"

#include "ProxyDict/Ifd.hh"
#include "ProxyDict/IfdHepAListProxy.hh"
#include "ProxyDict/IfdStrKey.hh"

#include "PDT/Pdt.hh"
#include "PDT/PdtEntry.hh"
#include "PDT/PdtLund.hh"

#include "VtxFitter/VtxFitterOper.hh"

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

using std::cout;
using std::cerr;
using std::endl;

//----------------
// Constructors --
//----------------

// in general, a module constructor should not do much.  The beginJob
// member function is a better place to put initialization
//
// This ctor initializes the three sample list-name parameters
DRecoilToolsUser::DRecoilToolsUser( const char* const theName, 
	      const char* const theDescription )
  : AppModule( theName, theDescription )
  , _eventInfoList("eventInfoList", this, "Default")
  , _outputList(   "outputList",    this, "NULL")
  , _pion_list(    "pion_list",     this, "NULL")
  , _kaon_list(    "kaon_list",     this, "NULL")
  , _proton_list(  "proton_list",   this, "NULL")
  , _muon_list(    "muon_list",     this, "NULL")
  , _electron_list("electron_list", this, "NULL")
  , _gamma_list(   "gamma_list",    this, "NULL")
  , _pi0_list(     "pi0_list",      this, "NULL")
  , _ks_list(      "ks_list",       this, "NULL")
  
  , _tag1_list(    "tag1_list",     this, "NULL")
  , _tag2_list(    "tag2_list",     this, "NULL")
  , _tag3_list(    "tag3_list",     this, "NULL")
  , _tag4_list(    "tag4_list",     this, "NULL")
  , _tag5_list(    "tag5_list",     this, "NULL")
  , _tag6_list(    "tag6_list",     this, "NULL")
  , _tag7_list(    "tag7_list",     this, "NULL")
  , _tag8_list(    "tag8_list",     this, "NULL")
  , _tag9_list(    "tag9_list",     this, "NULL")
  , _tag10_list(   "tag10_list",    this, "NULL")
  , _tag11_list(   "tag11_list",    this, "NULL")
  , _tag12_list(   "tag12_list",    this, "NULL")
  , _tag13_list(   "tag13_list",    this, "NULL")
  , _tag14_list(   "tag14_list",    this, "NULL")
  , _tag15_list(   "tag15_list",    this, "NULL")
  , _tag16_list(   "tag16_list",    this, "NULL")
  , _tag17_list(   "tag17_list",    this, "NULL")
  //, _tag18_list(   "tag18_list",    this, "NULL")
  , _tag19_list(   "tag19_list",    this, "NULL")
  , _tag20_list(   "tag20_list",    this, "NULL")
  , _tag21_list(   "tag21_list",    this, "NULL")
  , _tag22_list(   "tag22_list",    this, "NULL")
  , _tag23_list(   "tag23_list",    this, "NULL")
  , _tag24_list(   "tag24_list",    this, "NULL")
  , _tag25_list(   "tag25_list",    this, "NULL")
  , _tag26_list(   "tag26_list",    this, "NULL")
  , _tag27_list(   "tag27_list",    this, "NULL")
  , _tag28_list(   "tag28_list",    this, "NULL")
  
  , _double_tag_list( "double_tag_list", this, "NULL")
  
  , _n_events(0)
  , _signal_muon(1)
  , _signal_electron(0)
  , _signal_pion(0)
  , _signal_rho(0)
  , _signal_gamma(1)
  , _signal_pi0(0)
  
  , _tag_mode("tag_mode")
  // Information about right sign/wrong sign (RS/WS) tag candidates as well as forward/backward hemisphere (FH/BH)
  // Note that a tag candidate can be, say, RS and BH without being BH_RS
  , _RS("RS")
  , _WS("WS")
  , _FH("FH")
  , _BH("BH")
  , _FH_RS("FH_RS")
  , _FH_WS("FH_WS")
  , _BH_RS("BH_RS")
  , _BH_WS("BH_WS")
  , _is_a_tag("is_a_tag")
  , _is_DT("is_DT")
  , _n_tag1("n_tag1")
  , _n_tag2("n_tag2")
  , _n_tag3("n_tag3")
  , _n_tag4("n_tag4")
  , _n_tag5("n_tag5")
  , _n_tag6("n_tag6")
  , _n_tag7("n_tag7")
  , _n_tag8("n_tag8")
  , _n_tag9("n_tag9")
  , _n_tag10("n_tag10")
  , _n_tag11("n_tag11")
  , _n_tag12("n_tag12")
  , _n_tag13("n_tag13")
  , _n_tag14("n_tag14")
  , _n_tag15("n_tag15")
  , _n_tag16("n_tag16")
  , _n_tag17("n_tag17")
  //, _n_tag18("n_tag18")
  , _n_tag19("n_tag19")
  , _n_tag20("n_tag20")
  , _n_tag21("n_tag21")
  , _n_tag22("n_tag22")
  , _n_tag23("n_tag23")
  , _n_tag24("n_tag24")
  , _n_tag25("n_tag25")
  , _n_tag26("n_tag26")
  , _n_tag27("n_tag27")
  , _n_tag28("n_tag28")
  
  , _n_RS("n_RS")
  , _n_WS("n_WS")
  , _n_FH("n_FH")
  , _n_BH("n_BH")
  , _n_FH_RS("n_FH_RS")
  , _n_FH_WS("n_FH_WS")
  , _n_BH_RS("n_BH_RS")
  , _n_BH_WS("n_BH_WS")
  
  , _is_sig("is_sig")
  , _is_fitted("is_fitted")
  
  , _n_signal_gamma("n_signal_gamma")
  , _visible_charge("visible_charge")
  
  , _mMiss_pion("mMiss_pion")
  , _p3MissCM_pion("p3MissCM_pion")
  , _pxMissCM_pion("pxMissCM_pion")
  , _pyMissCM_pion("pyMissCM_pion")
  , _pzMissCM_pion("pzMissCM_pion")
  , _EMissCM_pion("EMissCM_pion")
  
  , _mMiss_cascade("mMiss_cascade")
  , _p3MissCM_cascade("p3MissCM_cascade")
  , _pxMissCM_cascade("pxMissCM_cascade")
  , _pyMissCM_cascade("pyMissCM_cascade")
  , _pzMissCM_cascade("pzMissCM_cascade")
  , _EMissCM_cascade("EMissCM_cascade") 
  , _CosThMiss_pion("CosThMiss_pion")
  
  , _tag_pxCM("tagpxCM")
  , _tag_pyCM("tagpyCM")
  , _tag_pzCM("tagpzCM")
  , _tag_energyCM("tagenergyCM")
  , _tag_mass("tagmass")
  , _tag_charge("tagcharge")
  
  , _K_pxCM("KpxCM")
  , _K_pyCM("KpyCM")
  , _K_pzCM("KpzCM")
  , _K_energyCM("KenergyCM")
  , _K_mass("Kmass")
  , _K_charge("Kcharge")
  
  , _X_pxCM("XpxCM")
  , _X_pyCM("XpyCM")
  , _X_pzCM("XpzCM")
  , _X_energyCM("XenergyCM")
  , _X_mass("Xmass")
  , _X_charge("Xcharge")
  
  , _Dsstarpre_pxCM("DsstarprepxCM")
  , _Dsstarpre_pyCM("DsstarprepyCM")
  , _Dsstarpre_pzCM("DsstarprepzCM")
  , _Dsstarpre_energyCM("DsstarpreenergyCM")
  , _Dsstarpre_mass("Dsstarpremass")
  , _Dsstarpre_charge("Dsstarprecharge")
  
  , _Dsstarpost_pxCM("DsstarpostpxCM")
  , _Dsstarpost_pyCM("DsstarpostpyCM")
  , _Dsstarpost_pzCM("DsstarpostpzCM")
  , _Dsstarpost_energyCM("DsstarpostenergyCM")
  , _Dsstarpost_mass("Dsstarpostmass")
  , _Dsstarpost_charge("Dsstarpostcharge")
  
  , _Ds_pxCM("DspxCM")
  , _Ds_pyCM("DspyCM")
  , _Ds_pzCM("DspzCM")
  , _Ds_energyCM("DsenergyCM")
  , _Ds_mass("Dsmass")
  , _Ds_charge("Dscharge")
  , _Ds_premass("Dspremass")
  
  , _mu_pxCM("mupxCM")
  , _mu_pyCM("mupyCM")
  , _mu_pzCM("mupzCM")
  , _mu_energyCM("muenergyCM")
  , _mu_charge("mucharge")
  
  , _nu_pxCM("nupxCM")
  , _nu_pyCM("nupyCM")
  , _nu_pzCM("nupzCM")
  , _nu_energyCM("nuenergyCM")
  , _nu_mass("numass")
  , _nu_deltap3("nudeltap3")
  
  , _gamma_pxCM("gammapxCM")
  , _gamma_pyCM("gammapyCM")
  , _gamma_pzCM("gammapzCM")
  , _gamma_energyCM("gammaenergyCM")

  , _Ds_testmass("Dstestmass")
  , _nu_testmass_a("nutestmass_unconstrained")
  , _nu_testmass_b("nutestmass_constrained")
  , _truthmatched("truthmatched")
  
  , _btaTruthList("truthCandidates", this, "MCTruth")

{
  commands()->append(&_eventInfoList);
  commands()->append(&_outputList);
  
  commands()->append(&_pion_list) ;
  commands()->append(&_kaon_list) ;
  commands()->append(&_proton_list) ;
  commands()->append(&_muon_list) ;
  commands()->append(&_electron_list) ;
  commands()->append(&_gamma_list) ;
  commands()->append(&_pi0_list) ;
  commands()->append(&_ks_list) ;
  
  commands()->append(&_tag1_list) ;
  commands()->append(&_tag2_list) ;
  commands()->append(&_tag3_list) ;
  commands()->append(&_tag4_list) ;
  commands()->append(&_tag5_list) ;
  commands()->append(&_tag6_list) ;
  commands()->append(&_tag7_list) ;
  commands()->append(&_tag8_list) ;
  commands()->append(&_tag9_list) ;
  commands()->append(&_tag10_list) ;
  commands()->append(&_tag11_list) ;
  commands()->append(&_tag12_list) ;
  commands()->append(&_tag13_list) ;
  commands()->append(&_tag14_list) ;
  commands()->append(&_tag15_list) ;
  commands()->append(&_tag16_list) ;
  commands()->append(&_tag17_list) ;
  //commands()->append(&_tag18_list) ;
  commands()->append(&_tag19_list) ;
  commands()->append(&_tag20_list) ;
  commands()->append(&_tag21_list) ;
  commands()->append(&_tag22_list) ;
  commands()->append(&_tag23_list) ;
  commands()->append(&_tag24_list) ;
  commands()->append(&_tag25_list) ;
  commands()->append(&_tag26_list) ;
  commands()->append(&_tag27_list) ;
  commands()->append(&_tag28_list) ;
  
  commands()->append(&_double_tag_list) ;

  commands()->append(&_btaTruthList) ;
  
  _event_block.addVariable(_n_tag1);
  _event_block.addVariable(_n_tag2);
  _event_block.addVariable(_n_tag3);
  _event_block.addVariable(_n_tag4);
  _event_block.addVariable(_n_tag5);
  _event_block.addVariable(_n_tag6);
  _event_block.addVariable(_n_tag7);
  _event_block.addVariable(_n_tag8);
  _event_block.addVariable(_n_tag9);
  _event_block.addVariable(_n_tag10);
  _event_block.addVariable(_n_tag11);
  _event_block.addVariable(_n_tag12);
  _event_block.addVariable(_n_tag13);
  _event_block.addVariable(_n_tag14);
  _event_block.addVariable(_n_tag15);
  _event_block.addVariable(_n_tag16);
  _event_block.addVariable(_n_tag17);
  //_event_block.addVariable(_n_tag18);
  _event_block.addVariable(_n_tag19);
  _event_block.addVariable(_n_tag20);
  _event_block.addVariable(_n_tag21);
  _event_block.addVariable(_n_tag22);
  _event_block.addVariable(_n_tag23);
  _event_block.addVariable(_n_tag24);
  _event_block.addVariable(_n_tag25);
  _event_block.addVariable(_n_tag26);
  _event_block.addVariable(_n_tag27);
  _event_block.addVariable(_n_tag28);
  
  _event_block.addVariable(_n_RS);
  _event_block.addVariable(_n_WS);
  _event_block.addVariable(_n_FH);
  _event_block.addVariable(_n_BH);
  _event_block.addVariable(_n_FH_RS);
  _event_block.addVariable(_n_FH_WS);
  _event_block.addVariable(_n_BH_RS);
  _event_block.addVariable(_n_BH_WS);
  
  _event_block.addVariable(_n_signal_gamma);
  _event_block.addVariable(_visible_charge);
  
  _event_block.addVariable(_mMiss_pion);
  _event_block.addVariable(_p3MissCM_pion);
  _event_block.addVariable(_pxMissCM_pion);
  _event_block.addVariable(_pyMissCM_pion);
  _event_block.addVariable(_pzMissCM_pion);
  _event_block.addVariable(_EMissCM_pion);
  
  _event_block.addVariable(_mMiss_cascade);
  _event_block.addVariable(_p3MissCM_cascade);
  _event_block.addVariable(_pxMissCM_cascade);
  _event_block.addVariable(_pyMissCM_cascade);
  _event_block.addVariable(_pzMissCM_cascade);
  _event_block.addVariable(_EMissCM_cascade);
  
  _event_block.addVariable(_CosThMiss_pion);
  
  _D_block.addVariable(_tag_mode) ;
  _D_block.addVariable(_RS) ;
  _D_block.addVariable(_WS) ;
  _D_block.addVariable(_FH) ;
  _D_block.addVariable(_BH) ;
  _D_block.addVariable(_FH_RS) ;
  _D_block.addVariable(_FH_WS) ;
  _D_block.addVariable(_BH_RS) ;
  _D_block.addVariable(_BH_WS) ;
  _D_block.addVariable(_is_a_tag) ;
  _D_block.addVariable(_is_DT) ;
  _D_block.addVariable(_is_sig) ;
  _D_block.addVariable(_is_fitted) ;
  
  _Ups_block.addVariable(_tag_pxCM) ;
  _Ups_block.addVariable(_tag_pyCM) ;
  _Ups_block.addVariable(_tag_pzCM) ;
  _Ups_block.addVariable(_tag_energyCM) ;
  _Ups_block.addVariable(_tag_mass) ;
  _Ups_block.addVariable(_tag_charge) ;

  _Ups_block.addVariable(_Dsstarpre_pxCM) ;
  _Ups_block.addVariable(_Dsstarpre_pyCM) ;
  _Ups_block.addVariable(_Dsstarpre_pzCM) ;
  _Ups_block.addVariable(_Dsstarpre_energyCM) ;
  _Ups_block.addVariable(_Dsstarpre_mass) ;
  _Ups_block.addVariable(_Dsstarpre_charge) ;
  
  _Ups_block.addVariable(_Dsstarpost_pxCM) ;
  _Ups_block.addVariable(_Dsstarpost_pyCM) ;
  _Ups_block.addVariable(_Dsstarpost_pzCM) ;
  _Ups_block.addVariable(_Dsstarpost_energyCM) ;
  _Ups_block.addVariable(_Dsstarpost_mass) ;
  _Ups_block.addVariable(_Dsstarpost_charge) ;
  
  _Ups_block.addVariable(_gamma_pxCM) ;
  _Ups_block.addVariable(_gamma_pyCM) ;
  _Ups_block.addVariable(_gamma_pzCM) ;
  _Ups_block.addVariable(_gamma_energyCM) ;
  
  _Ups_block.addVariable(_Ds_pxCM) ;
  _Ups_block.addVariable(_Ds_pyCM) ;
  _Ups_block.addVariable(_Ds_pzCM) ;
  _Ups_block.addVariable(_Ds_energyCM) ;
  _Ups_block.addVariable(_Ds_mass) ;
  _Ups_block.addVariable(_Ds_charge) ;
  _Ups_block.addVariable(_Ds_premass) ;
  
  _Ups_block.addVariable(_K_pxCM) ;
  _Ups_block.addVariable(_K_pyCM) ;
  _Ups_block.addVariable(_K_pzCM) ;
  _Ups_block.addVariable(_K_energyCM) ;
  _Ups_block.addVariable(_K_mass) ;
  _Ups_block.addVariable(_K_charge) ;
  
  _Ups_block.addVariable(_X_pxCM) ;
  _Ups_block.addVariable(_X_pyCM) ;
  _Ups_block.addVariable(_X_pzCM) ;
  _Ups_block.addVariable(_X_energyCM) ;
  _Ups_block.addVariable(_X_mass) ;
  _Ups_block.addVariable(_X_charge) ;
  
  _Ups_block.addVariable(_mu_pxCM) ;
  _Ups_block.addVariable(_mu_pyCM) ;
  _Ups_block.addVariable(_mu_pzCM) ;
  _Ups_block.addVariable(_mu_energyCM) ;
  _Ups_block.addVariable(_mu_charge) ;
  
  _Ups_block.addVariable(_nu_pxCM) ;
  _Ups_block.addVariable(_nu_pyCM) ;
  _Ups_block.addVariable(_nu_pzCM) ;
  _Ups_block.addVariable(_nu_energyCM) ;
  _Ups_block.addVariable(_nu_mass) ;
  _Ups_block.addVariable(_nu_deltap3) ;

  _Ups_block.addVariable(_Ds_testmass) ;
  _Ups_block.addVariable(_nu_testmass_a) ;
  _Ups_block.addVariable(_nu_testmass_b) ;
  _Ups_block.addVariable(_truthmatched) ;
  
}

//--------------
// Destructor --
//--------------

DRecoilToolsUser::~DRecoilToolsUser( )
{
}

//--------------
// Operations --
//--------------

// The begin(AppJob*) member function is run before any events are
// processed.  In this analysis, it opens the output histogram file
// and then books a number of histograms and a ntuple.

AppResult
DRecoilToolsUser::endJob( AbsEvent* anEvent )
{
  return AppResult::OK;
}

AppResult
DRecoilToolsUser::beginJob( AbsEvent* anEvent )
{
  return AppResult::OK;
}

AppResult
DRecoilToolsUser::event( AbsEvent* anEvent )
{
  // Identify four signal candidates: e, mu, pi, rho (pi, rho for D->tau nu)
  if(_signal_muon==0 && _signal_electron==0 && _signal_pion==0 && _signal_rho==0) ErrMsg(fatal) << "No signal modes specified" << endmsg ;
  _n_events++ ;

  // Define some constants
  const float Ds_mass_PDG = 1.968 ;
  const float mu_mass_PDG = 0.10565837 ;

  if(_n_events==1)
  {
    cout << "Using the following lists:" << endl ;
    cout << "Output:   " << _outputList.valueString()    << endl ;
    cout << "pion:     " << _pion_list.valueString()     << endl ;
    cout << "kaon:     " << _kaon_list.valueString()     << endl ;
    cout << "proton:   " << _proton_list.valueString()   << endl ;
    cout << "muon:     " << _muon_list.valueString()     << endl ;
    cout << "electron: " << _electron_list.valueString() << endl ;
    cout << "gamma:    " << _gamma_list.valueString()    << endl ;
    cout << "pi0:      " << _pi0_list.valueString()      << endl ;
    cout << "ks:       " << _ks_list.valueString()       << endl ;
  }
  
  HepAList<EventInfo>* infoList = Ifd< HepAList< EventInfo > >::get(anEvent, _eventInfoList.value());
  if(infoList == 0) ErrMsg(fatal) << "eventInfoList not in event" << endmsg;
  EventInfo* eventInfo = infoList->first();
  if(eventInfo == 0) ErrMsg(fatal) << "eventInfo not in event" << endmsg;
  HepLorentzVector CMp4 = eventInfo->cmFrame() ;

  // Put UsrData blocks into the event
  static const IfdStrKey event_data_key("event_data"); 
  if(!UsrIfd<UsrEventBlock>::put(anEvent,&_event_block, event_data_key))    ErrMsg(error) << "Can't put event_data "   << endmsg;
  static const IfdStrKey D_data_key("D_data");
  if(!UsrIfd<UsrCandBlock>::put(anEvent,&_D_block, D_data_key))             ErrMsg(error) << "Can't put D_data "       << endmsg;
  static const IfdStrKey Ups_data_key("Ups_data");
  if(!UsrIfd<UsrCandBlock>::put(anEvent,&_Ups_block, Ups_data_key)) ErrMsg(error) << "Can't put Ups_data " << endmsg;

  // Get total visible charge for the event
  HepAList<BtaCandidate>* charged_tracks = locateList(anEvent, "ChargedTracks") ;
  HepAListIterator<BtaCandidate> charged_tracks_iter(*charged_tracks) ;
  double charge(0) ; // Should be double as BtaCandidate->charge() is double
  while(BtaCandidate* charged_track = charged_tracks_iter())
  {
    charge = charge + charged_track->charge() ;
  }
  int visible_charge = (int)charge ; // Need to cast as int here rather than the next line
  _visible_charge = visible_charge ;

  // Crazy voodoo to get the outputList to work properly
  HepAList<BtaCandidate>* outputList = new HepAList<BtaCandidate>;
  IfdHepAListProxy<BtaCandidate>* outproxy = new IfdHepAListProxy<BtaCandidate>(outputList);
  bool can_place_proxy = false ;
  if(outproxy==0)
  {
    delete outputList;
    outputList=0;
    ErrMsg(fatal) << "Can't create a IfdHepAListProxy<BtaCandidate>" << endmsg;
  }
  else
  {
    can_place_proxy = Ifd<HepAList<BtaCandidate> > :: put (anEvent, outproxy, _outputList.value());
    if(!can_place_proxy)
    {
      delete outproxy;
      outproxy = 0;
      ErrMsg(fatal) << "The key for output list \"" << _outputList.value() << "\" may already be taken. Choose another name." << endmsg;
    }
  }

  // Print the truth tree
  if(false)
  {
    BtaPrintTree printTree;
    truthMap = Ifd< BtaMcAssoc >::get(anEvent, "GHit");
    if(truthMap)
    {
      getTmpAList(anEvent, mcList, _btaTruthList.value());
      HepAListIterator<BtaCandidate> reco2Iter(*mcList);
      while(BtaCandidate* reco = reco2Iter())
      {
	cout<<printTree.print(*reco);
      }
    }
  }
  // _is_sig and _is_fitted are only ever used for fitted signal
  _is_sig = 1 ;
  _is_fitted = 1 ;

  // Start requiring signal candidates.  This should be done BEFORE the tag list is populated
  // (Otherwise the tag list will get populated and upon return AppResult::OK BtaTupleMaker will dump the tag lists and ROE
  HepAList<BtaCandidate>* signal_track_list(0) ;
  int n_muons = 0 ;

  // input_signal_track_list is used to grab a PID list  signal_track_list is the resulting list of BtaCandidates after selection
  HepAList<BtaCandidate>* input_signal_track_list(0) ;
  signal_track_list = new HepAList<BtaCandidate> ;
  if(_signal_muon)
  {
    input_signal_track_list = locateList(anEvent, _muon_list.valueString());
    HepAListIterator<BtaCandidate> input_signal_track_muon_iter(*input_signal_track_list) ;
    while(BtaCandidate* input_signal_track_cand = input_signal_track_muon_iter())
    {
      // Place cuts here
      // First check the SVT/DCH quality of the track
      const BtaTrkQual* TrkQual = input_signal_track_cand->getMicroAdapter()->getTrkQual() ;
      int n_SVTHits = TrkQual->nSvtHits() ;
      int n_DCHHits = TrkQual->nDchHits() ;
      int SVT_pattern = TrkQual->SvtPattern() ;
      int n_z_hits = 0 ;
      int hit[10] = { 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 } ;
      int power_of_2 = 2 ;
      for(int i=0 ; i<10 ; i++)
      {
	if(SVT_pattern%power_of_2) hit[i] = 1 ;
	SVT_pattern = SVT_pattern-SVT_pattern%power_of_2 ;
	power_of_2 = power_of_2*2 ;
      }
      for(int i=5 ; i<10 ; i++)
      {
	if(hit[i]) n_z_hits++ ;
      }
      if(n_SVTHits<4 ) continue ;
      if(n_z_hits<2) continue ;
      if(n_DCHHits<12) continue ;
      signal_track_list->append(input_signal_track_cand) ;
      n_muons++ ;
    }
  }
  
  if(_signal_electron)
  {
    input_signal_track_list = locateList(anEvent, _electron_list.valueString());
    HepAListIterator<BtaCandidate> input_signal_track_electron_iter(*input_signal_track_list) ;
    while(BtaCandidate* input_signal_track_cand = input_signal_track_electron_iter())
    {
      // Place cuts here
      signal_track_list->append(input_signal_track_cand) ;
    }
  }
  
  if(_signal_pion)
  {
    input_signal_track_list = locateList(anEvent, _pion_list.valueString());
    HepAListIterator<BtaCandidate> input_signal_track_pion_iter(*input_signal_track_list) ;
    while(BtaCandidate* input_signal_track_cand = input_signal_track_pion_iter())
    {
      // Place cuts here
      signal_track_list->append(input_signal_track_cand) ;
    }
  }
           
  if(_signal_rho)
  {
    // Need pi and pi0 lists for this
    // Speak to Graham about these reconstructions!
  }
  
  // Jump out of events if the signal candidate list is empty
  if(signal_track_list->length()==0)
  {
    ErrMsg(warning) << "The signal list is empty.  I'm not going to bother filling outputList" << endmsg ;
    return AppResult::OK ;
  }

  // Check to see if there are any photons with ECM>100MeV and on the same side as at least one signal track
  // If not bail out of the event
  HepAList<BtaCandidate>* gamma_list = locateList(anEvent, _gamma_list.valueString()) ;

  HepAListIterator<BtaCandidate> signal_track_iter(*signal_track_list) ;
  HepAListIterator<BtaCandidate> gamma_iter(*gamma_list) ;
  HepAList<BtaCandidate>* signal_gamma_list = new HepAList<BtaCandidate> ;
  while(BtaCandidate* gamma_cand = gamma_iter())
  {
    HepLorentzVector gamma_p4 = gamma_cand->p4() ;
    HepLorentzVector gamma_p4CM  = gamma_p4.boost(-CMp4.boostVector()) ;
    if(gamma_p4CM.e()<0.1) continue ;
    /* Relaxed this cut.  Not sure it makes physical sense really...
    bool same_side = false ;
    while(BtaCandidate* signal_cand = signal_track_iter())
    {
      if(CM_dot_product(anEvent, signal_cand, gamma_cand)>0)
      {
	same_side = true ;
      }
    }
    if(same_side==true) signal_gamma_list->append(gamma_cand) ;
    */
    signal_gamma_list->append(gamma_cand) ;
  }
  signal_track_iter.rewind() ;
  _n_signal_gamma = signal_gamma_list->length() ;

  if(signal_gamma_list->length()*signal_track_list->length()==0) return AppResult::OK ;

  // Taken from Jose and edited
  BtaMcAssoc* _truthMap = Ifd< BtaMcAssoc >::get( anEvent,"GHit") ;  // What is GHit?
  HepAList<BtaCandidate>* _mcList = BtuUtil::rearrangeMcList( Ifd< HepAList<BtaCandidate> >::get(anEvent,"MCTruth" )) ;
  
  BtaCandidate* true_Dsstar = NULL ;
  BtaCandidate*     true_Ds = NULL ;
  BtaCandidate*  true_gamma = NULL ;
  BtaCandidate*     true_mu = NULL ;
  BtaCandidate*     true_nu = NULL ;
  
  //Find the Ds*
  bool found_truth = false ;
  if(_truthMap && _mcList)
  {
    //truth-match if this is MC
    HepAListIterator<BtaCandidate> mc_iter(*_mcList) ;
    while(BtaCandidate* Dsstar = mc_iter())
    {
      //if(found_truth==true) continue ;
      if(abs(Dsstar->pdtEntry()->lundId())==abs(Pdt::lookup(PdtPdg::D_s_star_plus)->lundId()))
      {
	// If we get this far we have a Ds*
	// Must decay to Ds gamma
	HepAListIterator<BtaCandidate> Dsstar_daughter_iter = Dsstar->daughterIterator() ;
	bool has_gamma = false ;
	bool has_pi0   = false ;
	bool has_Ds    = false ;
	bool has_other = false ;
	int number_of_Dsstar_daughters = 0 ;
	while(BtaCandidate* Dsstar_daughter = Dsstar_daughter_iter())
	{
	  if(abs(Dsstar_daughter->pdtEntry()->lundId())==abs(Pdt::lookup(PdtPdg::gamma)->lundId())) has_gamma = true ;
	  if(abs(Dsstar_daughter->pdtEntry()->lundId())==abs(Pdt::lookup(PdtPdg::pi0)->lundId()))     has_pi0 = true ;
	  if(abs(Dsstar_daughter->pdtEntry()->lundId())==abs(Pdt::lookup(PdtPdg::D_s_plus)->lundId())) has_Ds = true ;
	  number_of_Dsstar_daughters++ ;
	}
	cout << "Number of Dsstar daughters: " << number_of_Dsstar_daughters << endl ;
	Dsstar_daughter_iter.rewind() ;
	if(number_of_Dsstar_daughters==2)
	{
	  if(has_Ds)
	  {
	    if(has_gamma==true || has_pi0==true)
	    {
	      has_other = false ;
	    }
	    else
	    {
	      has_other = true ;
	    }
	  }
	  else
	  {
	    has_other = true ;
	  }
	}
	else
	{
	  continue ;
	}
	if(has_other==false)
	{
	  while(BtaCandidate* Ds = Dsstar_daughter_iter())
	  {
	    if(found_truth==true) continue ;
	    if(abs(Ds->pdtEntry()->lundId())==abs(Pdt::lookup(PdtPdg::D_s_plus)->lundId()))
	    {
	      // Investigate the Ds
	      HepAListIterator<BtaCandidate> Ds_daughter_iter = Ds->daughterIterator() ;
	      bool has_mu = false ;
	      bool has_nu = false ;
	      bool has_extra_gamma = false ;
	      int number_of_Ds_daughters = 0 ;
	      while(BtaCandidate* Ds_daughter = Ds_daughter_iter())
	      {
		if(abs(Ds_daughter->pdtEntry()->lundId())==abs(Pdt::lookup(PdtPdg::mu_plus)->lundId()))
		{
		  has_mu = true ;
		}
		if(abs(Ds_daughter->pdtEntry()->lundId())==abs(Pdt::lookup(PdtPdg::nu_mu)->lundId()))
		{
		  has_nu = true ;
		}
		if(abs(Ds_daughter->pdtEntry()->lundId())==abs(Pdt::lookup(PdtPdg::gamma)->lundId()))
		{
		  has_extra_gamma = true ;
		}
		number_of_Ds_daughters++ ;
	      }
	      Ds_daughter_iter.rewind() ;
	      cout << "Number of Ds daughters: " << number_of_Ds_daughters << endl ;
	      if(number_of_Ds_daughters==2 && has_mu==true && has_nu==true)
	      {
		// We have signal!  Just fill all the relevant candidates and continue
		true_Dsstar = Dsstar ;
		true_Ds = Ds ;
		while(BtaCandidate* Dsstar_daughter = Dsstar_daughter_iter())
		{
		  if(abs(Dsstar_daughter->pdtEntry()->lundId())==abs(Pdt::lookup(PdtPdg::gamma)->lundId())) true_gamma = Dsstar_daughter ;
		}
		Dsstar_daughter_iter.rewind() ;
		while(BtaCandidate* Ds_daughter = Ds_daughter_iter())
		{
		  if(abs(Ds_daughter->pdtEntry()->lundId())==abs(Pdt::lookup(PdtPdg::mu_plus)->lundId())) true_mu = Ds_daughter ;
		  if(abs(Ds_daughter->pdtEntry()->lundId())==abs(Pdt::lookup(PdtPdg::nu_mu)->lundId()))   true_nu = Ds_daughter ;
		}
		Ds_daughter_iter.rewind() ;
		found_truth = true ;
	      }
	      
	      if(!found_truth) continue ;
	      // cout << "Dsstar: " << true_Dsstar->uid() << endl ;
	      // cout << "    Ds: " << true_Ds->uid() << endl ;
	      // cout << "    mu: " << true_mu->uid() << endl ;
	      // cout << " gamma: " << true_gamma->uid() << endl ;
	    }
	  }
	}
      }
    }  
  }

  if(found_truth==false)
  {
    cout << "Could not truth match the event" << endl ;
    return AppResult::OK ;
  }

  const int number_of_tags = 29 ; // 0 and 18 are empty
  int n_tags[number_of_tags] ;
  _n_RS = 0 ;
  _n_WS = 0 ;
  _n_FH = 0 ;
  _n_BH = 0 ;
  _n_FH_RS = 0 ;
  _n_FH_WS = 0 ;
  _n_BH_RS = 0 ;
  _n_BH_WS = 0 ;
  _is_DT = 0 ;
  for(int i=1 ; i<number_of_tags ; i++)
  {
    if(!is_a_tag(i)) continue ;
    // Loop over tag lists
    n_tags[i] = 0 ;
    _tag_mode = i ;
    HepAList<BtaCandidate>* tag_list = 0 ;
    tag_list = get_tag_list(anEvent, i) ;
    
    HepAListIterator<BtaCandidate> tag_cand_iter(*tag_list) ;
    while(BtaCandidate* tag_cand = tag_cand_iter())
    {
      _RS = 0 ;
      _WS = 0 ;
      _FH = 0 ;
      _BH = 0 ;
      _FH_RS = 0 ;
      _FH_WS = 0 ;
      _BH_RS = 0 ;
      _BH_WS = 0 ;
      _is_a_tag = 1 ;

      int tag_flavor = 0 ;
      // Determine the flavor of the tag candidate
      int lund_id = tag_cand->pdtEntry()->lundId() ;
      if(lund_id==421)   tag_flavor = 1 ;
      if(lund_id==411)   tag_flavor = 1 ;
      if(lund_id==431)   tag_flavor = 1 ;
      if(lund_id==413)   tag_flavor = 1 ;
      if(lund_id==4122)  tag_flavor = 1 ;
    
      if(lund_id==-421)  tag_flavor = -1 ;
      if(lund_id==-411)  tag_flavor = -1 ;
      if(lund_id==-431)  tag_flavor = -1 ;
      if(lund_id==-413)  tag_flavor = -1 ;
      if(lund_id==-4122) tag_flavor = -1 ;

      bool get_kaon = true ;
      if(lund_id==431)   get_kaon = false ;
      if(lund_id==-431)  get_kaon = false ;

      while(BtaCandidate* signal_cand = signal_track_iter())
      {
	// NB: Flavor of the signal candidate is the charge of the signal candidate
	// Right sign tag candidates are where tag_flavor + signal_flavor = 0
	if(signal_cand->overlaps(*tag_cand)) continue ;
	int signal_flavor = (int)signal_cand->charge() ;
	float dot_product = CM_dot_product(anEvent, tag_cand, signal_cand) ;
	if(dot_product>0)
	{
	  _n_FH = _n_FH+1 ;
	  _FH = 1 ;
	  if(signal_flavor+tag_flavor==0)
	  {
	    _n_FH_RS = _n_FH_RS+1 ;
	    _FH_RS = 1 ;
	  }
	  else
	  {
	    _n_FH_WS = _n_FH_WS+1 ;
	    _FH_WS = 1 ;
	  }
	}
	else
	{
	  _n_BH = _n_BH+1 ;
	  _BH = 1 ;
	  if(signal_flavor+tag_flavor==0)
	  {
	    _n_BH_RS = _n_BH_RS+1 ;
	    _BH_RS = 1 ;
	  }
	  else
	  {
	    _BH_WS = _BH_WS+1 ;
	    _BH_WS = 1 ;
	  }
	}
	if(signal_flavor+tag_flavor==0)
	{
	  _n_RS = _n_RS+1 ;
	  _RS = 1 ;
	}
	else
	{
	  _n_WS = _n_WS+1 ;
	  _WS = 1 ;
	}
      }

      // put usr variable into usr_D_block
      BtaCandidate* tag_cand_to_append = new BtaCandidate(*tag_cand) ;
      if (!_D_block.put(*tag_cand_to_append,_tag_mode)) ErrMsg(error) << "Can't put " << _tag_mode.name() << " into UsrCandBlock D_block" << endmsg;
      if (!_D_block.put(*tag_cand_to_append,_RS))       ErrMsg(error) << "Can't put " << _RS.name()       << " into UsrCandBlock D_block" << endmsg;
      if (!_D_block.put(*tag_cand_to_append,_WS))       ErrMsg(error) << "Can't put " << _WS.name()       << " into UsrCandBlock D_block" << endmsg;
      if (!_D_block.put(*tag_cand_to_append,_FH))       ErrMsg(error) << "Can't put " << _FH.name()       << " into UsrCandBlock D_block" << endmsg;
      if (!_D_block.put(*tag_cand_to_append,_BH))       ErrMsg(error) << "Can't put " << _BH.name()       << " into UsrCandBlock D_block" << endmsg;
      if (!_D_block.put(*tag_cand_to_append,_FH_RS))    ErrMsg(error) << "Can't put " << _FH_RS.name()    << " into UsrCandBlock D_block" << endmsg;
      if (!_D_block.put(*tag_cand_to_append,_FH_WS))    ErrMsg(error) << "Can't put " << _FH_WS.name()    << " into UsrCandBlock D_block" << endmsg;
      if (!_D_block.put(*tag_cand_to_append,_BH_RS))    ErrMsg(error) << "Can't put " << _BH_RS.name()    << " into UsrCandBlock D_block" << endmsg;
      if (!_D_block.put(*tag_cand_to_append,_BH_WS))    ErrMsg(error) << "Can't put " << _BH_WS.name()    << " into UsrCandBlock D_block" << endmsg;
      if (!_D_block.put(*tag_cand_to_append,_is_a_tag)) ErrMsg(error) << "Can't put " << _is_a_tag.name() << " into UsrCandBlock D_block" << endmsg;
      if (!_D_block.put(*tag_cand_to_append,_is_DT))    ErrMsg(error) << "Can't put " << _is_DT.name()    << " into UsrCandBlock D_block" << endmsg;
      if (!_D_block.put(*tag_cand_to_append,_is_sig))   ErrMsg(error) << "Can't put " << _is_sig.name()   << " into UsrCandBlock D_block" << endmsg;
      if (!_D_block.put(*tag_cand_to_append,_is_fitted))ErrMsg(error) << "Can't put " << _is_fitted.name()<< " into UsrCandBlock D_block" << endmsg;
      outputList->append(tag_cand_to_append) ;
      //cout << "tag_cand_to_append: " << tag_cand_to_append << endl ;

      n_tags[i]++ ;
      
      HepAListIterator<BtaCandidate> signal_gamma_iter(*signal_gamma_list) ;
      while(BtaCandidate* gamma = signal_gamma_iter())
      {
	if(gamma->overlaps(*tag_cand))
	{
	  cout << "gamma overlaps tag" << endl ;
	  continue ;
	}
	
	// Check truth of photon
	BtaCandidate* MC_gamma = _truthMap->mcFromReco(gamma) ; 
	if(!MC_gamma) continue ;
	if(MC_gamma->uid()!=true_gamma->uid())
	{
	  cout << "Photon does not truthmatch" << endl ;
	  //continue ;
	}
	
	HepAListIterator<BtaCandidate> signal_muon_iter(*signal_track_list) ;
	while(BtaCandidate* mu = signal_muon_iter())
	{

	  int truthmatched = 1 ;

	  if(mu->overlaps(*tag_cand))
	  {
	    cout << "Tag overlaps signal" << endl ;
	    continue ;
	  }
	  if(fabs(mu->charge()+tag_flavor)>1)
	  {
	    cout << "Tag flavor doesn't match signal charge" << endl ;
	    continue ;
	  }



   	  // Check truth of mu
	  BtaCandidate* MC_mu = _truthMap->mcFromReco(mu) ; 
	  if(!MC_mu) continue ;
	  if(MC_mu->uid()!=true_mu->uid())
	  {
	    cout << "Muon does not truthmatch" << endl ;
	    //continue ;
	    truthmatched = 0 ;
	  }
	  // Check truth of gamma
	  if(MC_gamma->uid()!=true_gamma->uid())
	  {
	    cout << "Photon does not truthmatch" << endl ;
	    //continue ;
	    truthmatched = 0 ;
	  }

	  _truthmatched = truthmatched ;

	  BtaCandidate K = NULL ;
	  HepAList<BtaCandidate> flavor_k_list ;
	  HepAListIterator<BtaCandidate> flavor_k_iter(flavor_k_list) ;  
	  if(get_kaon)
	  {
	    // Look for flavor balancing kaon.

	    HepAList<BtaCandidate>* kaon_list = locateList(anEvent, _kaon_list.valueString()) ;
	    HepAListIterator<BtaCandidate> kaon_iter(*kaon_list) ;
	    while(BtaCandidate* kaon_cand = kaon_iter())
	    {
	      bool add_kaon = true ;
	      if(kaon_cand->overlaps(*tag_cand)) add_kaon = false ;
	      if(kaon_cand->overlaps(*mu)) add_kaon = false ;
	      if(add_kaon) flavor_k_list.append(kaon_cand) ;
	    }
	    kaon_iter.rewind() ;
	    HepAList<BtaCandidate>* kshort_list = locateList(anEvent, _ks_list.valueString()) ;
	    HepAListIterator<BtaCandidate> kshort_iter(*kshort_list) ;
	    while(BtaCandidate* kshort_cand = kshort_iter())
	    {
	      bool add_kaon = true ;
	      while(BtaCandidate* flavor_k_cand = flavor_k_iter())
	      {
		if(kshort_cand->overlaps(*flavor_k_cand)) add_kaon = false ;
	      }
	      flavor_k_iter.rewind() ;
	      if(kshort_cand->overlaps(*tag_cand)) add_kaon = false ;
	      if(kshort_cand->overlaps(*mu))       add_kaon = false ;
	      if(add_kaon) flavor_k_list.append(kshort_cand) ;
	    }
	    kshort_iter.rewind() ;
	  
	    int number_of_flavor_k = flavor_k_list.length() ;
	    if(number_of_flavor_k!=1) continue ;
	    while(BtaCandidate* flavor_k_cand = flavor_k_iter())
	    {
	      K = *flavor_k_cand ;
	    }
	    if(fabs(K.charge())>0.5)
	    {
	      if(fabs(mu->charge()+K.charge())>1)
	      {
		cout << "K charge doesn't match signal charge" << endl ;
		continue ;
	      }
	      if(fabs(tag_cand->charge()+K.charge())<1.5)
	      {
		cout << "K charge doesn't match tag flavor" << endl ;
		continue ;
	      }
	    }
	  }
	        
	  // Construct X
	  // Take pionList and pi0List, exclude everything from the signal, tag and kaons
      
	  HepLorentzVector Xp4 ;
	  Xp4.setX(0) ;
	  Xp4.setY(0) ;
	  Xp4.setZ(0) ;
	  Xp4.setT(0) ;
      
	  HepAList<BtaCandidate> Xpi_list ;
	  HepAListIterator<BtaCandidate> Xpi_iter(Xpi_list) ;
	  HepAList<BtaCandidate>* pion_list = locateList(anEvent, _pion_list.valueString()) ;
	  HepAListIterator<BtaCandidate> pion_iter(*pion_list) ;
	  while(BtaCandidate* pion_cand = pion_iter())
	  {
	    bool add_track = true ;
	    HepAListIterator<BtaCandidate> daughter_iter = tag_cand->daughterIterator() ;
	    if(pion_cand->overlaps(*tag_cand)) add_track = false ;
	    if(pion_cand->overlaps(*mu))       add_track = false ;
	    if(K!=NULL)
	    {
	      while(BtaCandidate* flavor_k_cand = flavor_k_iter()){ if(pion_cand->overlaps(*flavor_k_cand)) add_track = false ; }
	    }
	    if(add_track==true) Xpi_list.append(pion_cand) ;
	  }
	  
	  HepAList<BtaCandidate> Xpi0_list ;
	  HepAListIterator<BtaCandidate> Xpi0_iter(Xpi0_list) ;
	  HepAList<BtaCandidate>* pi0_list = locateList(anEvent, _pi0_list.valueString()) ;
	  HepAListIterator<BtaCandidate> pi0_iter(*pi0_list) ;
	  while(BtaCandidate* pi0_cand = pi0_iter())
	  {
	    bool add_pi0 = true ;
	    if(pi0_cand->overlaps(*tag_cand)) add_pi0 = false ;
	    if(pi0_cand->overlaps(*gamma))    add_pi0 = false ;
	    while(BtaCandidate* Xpi0_cand = Xpi0_iter()){ if(Xpi0_cand->overlaps(*pi0_cand)) add_pi0 = false ; }
	    Xpi0_iter.rewind() ;
	    if(add_pi0==true) Xpi0_list.append(pi0_cand) ;
	  }
	  pi0_iter.rewind() ;

	  HepAList<BtaCandidate>* gamma_list = locateList(anEvent, _gamma_list.valueString()) ;
	  HepAList<BtaCandidate> extra_gamma_list ;
	  HepAListIterator<BtaCandidate> gamma_iter(*gamma_list) ;
	  while(BtaCandidate* gamma_cand = gamma_iter())
	  {
	    bool add_gamma = true ;
	    if(gamma_cand->overlaps(*tag_cand)) add_gamma = false ;
	    if(gamma_cand->overlaps(*gamma))    add_gamma = false ;
	    while(BtaCandidate* Xpi0_cand = Xpi0_iter())
	    {
	      if(gamma_cand->overlaps(*Xpi0_cand)) add_gamma = false ;
	    }
	    Xpi0_iter.rewind() ;
	    if(add_gamma) extra_gamma_list.append(gamma_cand) ;
	  }
	  float max_extra_gamma_energy = 0 ;
	  float total_extra_gamma_energy = 0 ;
	  int number_of_extra_gammas = 0 ;
	  HepAListIterator<BtaCandidate> extra_gamma_iter(extra_gamma_list) ;
	  while(BtaCandidate* extra_gamma = extra_gamma_iter())
	  {
	    float gamma_energy = extra_gamma->p4().e() ;
	    total_extra_gamma_energy += gamma_energy ;
	    if(gamma_energy>max_extra_gamma_energy) max_extra_gamma_energy = gamma_energy ;
	    number_of_extra_gammas++ ;
	  }
	  extra_gamma_iter.rewind() ;
	  while(BtaCandidate* extra_gamma = extra_gamma_iter())
	  {
	    float gamma_energy = extra_gamma->p4().e() ;
	    if(gamma_energy==max_extra_gamma_energy) Xpi0_list.append(extra_gamma) ;
	  }
	  if(number_of_extra_gammas*(total_extra_gamma_energy-max_extra_gamma_energy)>1)
	  {
	    continue ;
	  }
	  double X_charge = 0 ;
	  while(BtaCandidate* Xpi_cand = Xpi_iter())
	  {
	    Xp4 += Xpi_cand->p4() ;
	    X_charge += Xpi_cand->charge() ;
      	  }
	  while(BtaCandidate* Xpi0_cand = Xpi0_iter())
	  {
	    Xp4 += Xpi0_cand->p4() ;
      	  }
	  BtaCandidate X = NULL ;
	  if(Xp4.e()>0)
	  {
	    BtaCandidate X_temp(Xp4, X_charge, 0) ;
	    X = X_temp ;
	    X.setType(Pdt::lookup(PdtPdg::eta)) ;
	  }

	  BtaCandidate tag = *tag_cand ;
	  
	  // Time to do vertex fits
	  // Specify which vertexer to use
	  VtxAbsAlgorithm::algId algorithm = VtxAbsAlgorithm::TreeFitter ;
	  
	  // Make something to build the decay trees
	  BtaOpMakeTree tree_maker ;
	  
	  BbrLorentzVectorErr eep4 = eventInfo->electronBeam().fitParams().p4WErr() + eventInfo->positronBeam().fitParams().p4WErr() ;
	  
	  // Create the Ds*
	  HepLorentzVector Dsstarp4 ;
	  if(X!=NULL)
	  {
	    if(K!=NULL)
	    {
	      Dsstarp4 = eep4 - tag.p4() - K.p4() - X.p4() ;
	    }
	    else
	    {
	      Dsstarp4 = eep4 - tag.p4() - X.p4() ;
	    }
	  }
	  else
	  {
	    if(K!=NULL)
	    {
	      Dsstarp4 = eep4 - tag.p4() - K.p4() ;
	    }
	    else
	    {
	      Dsstarp4 = eep4 - tag.p4() ;
	    }
	  }
	  
	  double K_charge = 0 ;
	  if(K!=NULL) K_charge = K.charge() ;
	  if(X!=NULL) X_charge = X.charge() ;
	  double Dsstar_charge = 0 - tag.charge() - K_charge - X_charge ;

	  if(Dsstar_charge>1 || Dsstar_charge<-1 || Dsstar_charge==0)
	  {
	    cout << "Ds* charge is not +/-1" << endl ;
	    continue ;
	  }
	  
	  cout << "Ds* " ; show_p4(Dsstarp4) ;
	  BtaCandidate Dsstar_1(Dsstarp4) ;
	  if(Dsstar_charge>0) Dsstar_1.setType(Pdt::lookup(PdtPdg::D_s_star_plus)) ;
	  if(Dsstar_charge<0) Dsstar_1.setType(Pdt::lookup(PdtPdg::D_s_star_minus)) ;
	  setMassConstraint(Dsstar_1) ;
      	  BtaCandidate tag_1 = tag ;

	  BtaCandidate K_1 ;
	  if(K!=NULL)
	  {
	    K_1 = K ;
	  }

	  BtaCandidate X_1 ;
	  if(X!=NULL)
	  {
	    X_1 = X ;
	  }

	 
	  
	  BtaCandidate* Ups_1 ;
	  if(X!=NULL)
	  {
	    if(K!=NULL)
	    {
	      Ups_1 = tree_maker.create(tag_1, K_1, X_1, Dsstar_1) ;
	    }
	    else
            {
	      Ups_1 = tree_maker.create(tag_1, X_1, Dsstar_1) ;
	    }
	  }
	  else
	  {
	    if(K!=NULL)
	    {
	      Ups_1 = tree_maker.create(tag_1, K_1, Dsstar_1) ;
	    }
	    else
            {
	      Ups_1 = tree_maker.create(tag_1, Dsstar_1) ;
	    }
	  }
	  Ups_1->setType(Pdt::lookup(PdtPdg::Upsilon_4S)) ;
	  cout << "ups: " ; show_p4(Ups_1->p4()) ;
	 
	  // Set constraints
	  setMassConstraint(tag_1) ;
	  setOriginConstraint(tag_1, eventInfo) ;
	  if(K!=NULL) setMassConstraint(K_1) ;
	  if(K!=NULL) setOriginConstraint(K_1, eventInfo) ;
	  if(X!=NULL) setOriginConstraint(X_1, eventInfo) ;
	  setOriginConstraint(Dsstar_1, eventInfo) ;
	  setMassConstraint(*Ups_1) ;
	  setBeamConstraint(*Ups_1,eventInfo) ;
	  setEnergyConstraint(*Ups_1,eventInfo) ;

	  // Show p4 before fit
	  cout << "          === p4 before first fit: === " << endl ;
	  cout << "ups  " ; show_p4(Ups_1->p4()) ;
	  cout << "tag  " ; show_p4(tag_1.p4()) ;
	  if(K!=NULL){ cout << "  K " ; show_p4(K_1.p4()) ; }
	  if(X!=NULL){ cout << "  X " ; show_p4(X_1.p4()) ; }
	  cout << "Ds* " ; show_p4(Dsstarp4) ;
	  
	  // Create fitter
	  VtxFitterOper Ups_fitter_1(*Ups_1, algorithm) ;
	  Ups_fitter_1.invalidateFit() ;
	  Ups_fitter_1.fitAll() ;
	  
	  BtaCandidate fitted_Ups_1 = Ups_fitter_1.getFitted(*Ups_1) ;
	  HepLorentzVector fitted_Ups_1_p4 = fitted_Ups_1.p4() ;
	  if(fitted_Ups_1.decayVtx()->status())
	  {
	    cout << "First fit failed with code " << fitted_Ups_1.decayVtx()->status() << endl ;
	    continue ;
	  }

	  // Boost info
	  HepLorentzVector CM_boost = CMBoost(anEvent) ;
	  float boost_e = CM_boost.e() ;
	  float boost_x = CM_boost.x()/boost_e ;
	  float boost_y = CM_boost.y()/boost_e ;
	  float boost_z = CM_boost.z()/boost_e ;

	  BtaCandidate fitted_tag_1 = Ups_fitter_1.getFitted(tag_1) ;
	  HepLorentzVector fitted_tag_1_p4 = fitted_tag_1.p4() ;

	  BtaCandidate fitted_K_1 = Ups_fitter_1.getFitted(K_1) ;
	  HepLorentzVector fitted_K_1_p4 = fitted_K_1.p4() ;

	  BtaCandidate fitted_X_1 = Ups_fitter_1.getFitted(X_1) ;
	  HepLorentzVector fitted_X_1_p4 = fitted_X_1.p4() ;

	  BtaCandidate fitted_Dsstar_1 = Ups_fitter_1.getFitted(Dsstar_1) ;
	  HepLorentzVector fitted_Dsstar_1_p4 = fitted_Dsstar_1.p4() ;
	  HepLorentzVector fitted_Dsstar_1_p4CM = fitted_Dsstar_1_p4.boost(boost_x, boost_y, boost_z) ;
	  _Dsstarpre_pxCM     = fitted_Dsstar_1_p4CM.x() ;
	  _Dsstarpre_pyCM     = fitted_Dsstar_1_p4CM.y() ;
	  _Dsstarpre_pzCM     = fitted_Dsstar_1_p4CM.z() ;
	  _Dsstarpre_energyCM = fitted_Dsstar_1_p4CM.e() ;
	  _Dsstarpre_mass     = fitted_Dsstar_1_p4CM.m() ;
	  _Dsstarpre_charge   = Dsstar_charge ;
	 
	  Dsstarp4 = fitted_Dsstar_1_p4 ;

	  // Show p4 after fit
	  cout << "          === p4 after first fit: === " << endl ;
	  cout << "ups " ; show_p4(fitted_Ups_1_p4) ;
	  cout << "tag " ; show_p4(fitted_tag_1_p4) ;
	  if(K!=NULL){ cout << "  K " ; show_p4(fitted_K_1_p4) ; }
	  if(X!=NULL){ cout << "  X " ; show_p4(fitted_X_1_p4) ; }
	  cout << "Ds* " ; show_p4(fitted_Dsstar_1_p4) ;

	  if(Dsstarp4.m()>2.612 || Dsstarp4.m()<1.612)
	  {
	    cout << "Ds* mass too far from nominal value" << endl ;
	    continue ;
	  }
	  


	  // Do test fit of Ds+* -> Ds gamma
	  BtaCandidate Ds_test(Dsstarp4-gamma->p4()) ;
	  if(Dsstar_charge>0) Ds_test.setType(Pdt::lookup(PdtPdg::D_s_plus)) ;
	  if(Dsstar_charge<0) Ds_test.setType(Pdt::lookup(PdtPdg::D_s_minus)) ;
	  setMassConstraint(Ds_test) ;
	  BtaCandidate gamma_test = *gamma ;
	  BtaCandidate Dsstar_test = tree_maker.combine(Ds_test, gamma_test) ;
	  if(Dsstar_charge>0) Dsstar_test.setType(Pdt::lookup(PdtPdg::D_s_star_plus)) ;
	  if(Dsstar_charge<0) Dsstar_test.setType(Pdt::lookup(PdtPdg::D_s_star_minus)) ;
	  setMassConstraint(Dsstar_test) ;
	   // Create fitter
	  VtxFitterOper test_fitter(Dsstar_test, VtxAbsAlgorithm::Add4 ) ;
	  test_fitter.invalidateFit() ;
	  test_fitter.fitAll() ;
	  BtaCandidate Ds_test_post = test_fitter.getFitted(Ds_test) ;
	  cout << "Test Ds: " ; show_p4(Ds_test_post.p4()) ;
	  _Ds_testmass = Ds_test_post.p4().m() ;

	  // Do test fit of Ds+ -> mu nu
	  // Two fits, one with nu mass constraint on nu and one without
	  BtaCandidate nu_test_2a(Ds_test_post.p4()-mu->p4()) ;
	  nu_test_2a.setType(Pdt::lookup(PdtPdg::nu_mu)) ;
	  BtaCandidate nu_test_2b = nu_test_2a ;
	  setMassConstraint(nu_test_2b) ;
	  BtaCandidate mu_test_2a = *mu ;
	  BtaCandidate Ds_test_2a = tree_maker.combine(mu_test_2a, nu_test_2a) ;
	  if(Dsstar_charge>0) Ds_test_2a.setType(Pdt::lookup(PdtPdg::D_s_plus)) ;
	  if(Dsstar_charge<0) Ds_test_2a.setType(Pdt::lookup(PdtPdg::D_s_minus)) ;
	  setMassConstraint(Ds_test_2a) ;
	  // Create fitters
	  VtxFitterOper test_fitter_2_a(Ds_test_2a, VtxAbsAlgorithm::Add4 ) ;
	  test_fitter_2_a.invalidateFit() ;
	  test_fitter_2_a.fitAll() ;
	  BtaCandidate nu_test_2a_post = test_fitter_2_a.getFitted(nu_test_2b) ;
	  cout << "Unconstrained test nu: " ; show_p4(nu_test_2a_post.p4()) ;
	  _nu_testmass_a = nu_test_2a_post.p4().m() ;

	  BtaCandidate mu_test_2b = *mu ;
	  BtaCandidate Ds_test_2b = tree_maker.combine(mu_test_2b, nu_test_2b) ;
	  if(Dsstar_charge>0) Ds_test_2b.setType(Pdt::lookup(PdtPdg::D_s_plus)) ;
	  if(Dsstar_charge<0) Ds_test_2b.setType(Pdt::lookup(PdtPdg::D_s_minus)) ;
	  setMassConstraint(Ds_test_2b) ;
	  VtxFitterOper test_fitter_2_b(Ds_test_2b, VtxAbsAlgorithm::Add4 ) ;
	  test_fitter_2_b.invalidateFit() ;
	  test_fitter_2_b.fitAll() ;
	  BtaCandidate nu_test_2b_post = test_fitter_2_b.getFitted(nu_test_2b) ;
	  cout << "Mass constrained test nu: " ; show_p4(nu_test_2b_post.p4()) ;
	  _nu_testmass_b = nu_test_2b_post.p4().m() ;



	  bool perform_intermediate_fit = true ;
	  BtaCandidate tag_2 ;
	  BtaCandidate K_2 ;
	  BtaCandidate X_2 ;
	  BtaCandidate gamma_2 ;
	  BtaCandidate Ds_2 ;
	  BtaCandidate* Dsstar_2 = NULL ;
	  BtaCandidate* Ups_2 ;
	  BtaCandidate fitted_Ups_2 ;
	  BtaCandidate fitted_tag_2 ;
	  BtaCandidate fitted_Dsstar_2 ;
	  BtaCandidate fitted_Ds_2 ;
	  BtaCandidate fitted_gamma_2 ;
	  if(perform_intermediate_fit)
	  {
	    tag_2 = Ups_fitter_1.getFitted(tag_1) ;
	    setMassConstraint(tag_2) ;
	    setOriginConstraint(tag_2, eventInfo) ;

	    if(K!=NULL)
	    {
	      K_2 = Ups_fitter_1.getFitted(K_1) ;
	      setMassConstraint(K_2) ;
	      setOriginConstraint(K_2, eventInfo) ;
	    }
	    
	    if(X!=NULL)
	    {
	      X_2 = Ups_fitter_1.getFitted(X_1) ;
	      setOriginConstraint(X_2, eventInfo) ;
	    }
	    
	    gamma_2 = *gamma ;
	    setMassConstraint(gamma_2) ;
	  
	    HepLorentzVector Dsp4 = fitted_Dsstar_1.p4()-gamma_2.p4() ;
	    Ds_2 = BtaCandidate(Dsp4) ;
	    if(Dsstar_charge>0) Ds_2.setType(Pdt::lookup(PdtPdg::D_s_plus)) ;
	    if(Dsstar_charge<0) Ds_2.setType(Pdt::lookup(PdtPdg::D_s_minus)) ;
	    //setMassConstraint(Ds_2) ;
	    
	    Dsstar_2 = tree_maker.create(Ds_2, gamma_2) ;
	    if(Dsstar_charge>0) Dsstar_2->setType(Pdt::lookup(PdtPdg::D_s_star_plus)) ;
	    if(Dsstar_charge<0) Dsstar_2->setType(Pdt::lookup(PdtPdg::D_s_star_minus)) ;
	    setMassConstraint(*Dsstar_2) ;
	    setOriginConstraint(*Dsstar_2, eventInfo) ;
     	    setOriginConstraint(Ds_2, Dsstar_2) ;
	    
	    if(X!=NULL)
	    {
	      if(K!=NULL)
	      {
		Ups_2 = tree_maker.create(tag_2, K_2, X_2, *Dsstar_2) ;
	      }
	      else
	      {
		Ups_2 = tree_maker.create(tag_2, X_2, *Dsstar_2) ;
	      }
	      
	    }
	    else
	    {
	      if(K!=NULL)
	      {
		Ups_2 = tree_maker.create(tag_2, K_2, *Dsstar_2) ;
	      }
	      else
	      {
		Ups_2 = tree_maker.create(tag_2, *Dsstar_2) ;
	      }
	    }
	    Ups_2->setType(Pdt::lookup(PdtPdg::Upsilon_4S)) ;
	    setMassConstraint(*Ups_2) ;
	    setBeamConstraint(*Ups_2,eventInfo) ;
	    setEnergyConstraint(*Ups_2,eventInfo) ;
	  
	    // Show p4 before second fit
	    cout << "          === p4 before second fit: === " << endl ;
	    cout << "ups " ; show_p4(Ups_2->p4()) ;
	    cout << "tag " ; show_p4(tag_2.p4()) ;
	    if(X!=NULL){ cout << "  X " ; show_p4(X_2.p4()) ; }
	    if(K!=NULL){ cout << "  K " ; show_p4(K_2.p4()) ; }
	    cout << "Ds* " ; show_p4(Dsstar_2->p4()) ;
	    cout << " Ds " ; show_p4(Ds_2.p4()) ;
	    cout << "  g " ; show_p4(gamma_2.p4()) ;
	  
	    // Create fitter
	    VtxFitterOper Ups_fitter_2(*Ups_2, algorithm) ;
	    Ups_fitter_2.invalidateFit() ;
	    Ups_fitter_2.fitAll() ;
	    
	    fitted_Ups_2 = Ups_fitter_2.getFitted(*Ups_2) ;
	    if(fitted_Ups_2.decayVtx()->status())
	    {
	      cout << "Second fit failed with code " << fitted_Ups_2.decayVtx()->status() << endl ; 
	      //continue ;
	    }
	    cout << "Fit prob chi2: " << fitted_Ups_2.decayVtx()->chiSquared() << endl ;
	    HepLorentzVector fitted_Ups_2_p4 = fitted_Ups_2.p4() ;
	    
	    BtaCandidate fitted_tag_2      = Ups_fitter_2.getFitted(tag_2) ;
	    BtaCandidate fitted_Dsstar_2   = Ups_fitter_2.getFitted(*Dsstar_2) ;
	    BtaCandidate fitted_Ds_2       = Ups_fitter_2.getFitted(Ds_2) ;
	    BtaCandidate fitted_gamma_2    = Ups_fitter_2.getFitted(gamma_2) ;
	    
	    HepLorentzVector fitted_tag_2_p4    = fitted_tag_2.p4() ;
	    HepLorentzVector fitted_Dsstar_2_p4 = fitted_Dsstar_2.p4() ;
	    HepLorentzVector fitted_Ds_2_p4     = fitted_Ds_2.p4() ;
	    HepLorentzVector fitted_gamma_2_p4  = fitted_gamma_2.p4() ;
	    
	    _Ds_premass = fitted_Ds_2_p4.m() ;
	    
	    HepLorentzVector fitted_K_2_p4 ;
	    BtaCandidate fitted_K_2 ;
	    fitted_K_2_p4.setX(0) ;
	    fitted_K_2_p4.setY(0) ;
	    fitted_K_2_p4.setZ(0) ;
	    fitted_K_2_p4.setT(0) ;
	    _K_charge = 0 ;
	    if(K!=NULL)
	    {
	      fitted_K_2 = Ups_fitter_2.getFitted(K_2) ;
	      fitted_K_2_p4 = fitted_K_2.p4() ;
	      _K_charge = (float)fitted_K_2.charge() ;
	    }
	    
	    HepLorentzVector fitted_X_2_p4 ;
	    BtaCandidate fitted_X_2 ;
	    fitted_X_2_p4.setX(0) ;
	    fitted_X_2_p4.setY(0) ;
	    fitted_X_2_p4.setZ(0) ;
	    fitted_X_2_p4.setT(0) ;
	    _X_charge = 0 ;
	    if(X!=NULL)
	    {
	      fitted_X_2 = Ups_fitter_2.getFitted(X_2) ;
	      fitted_X_2_p4 = fitted_X_2.p4() ;
	      _X_charge = (float)fitted_X_2.charge() ;
	    }
	    
	    // Show p4 after fit
	    cout << "          === p4 after second fit: === " << endl ;
	    cout << "ups " ; show_p4(fitted_Ups_2_p4) ;
	    cout << "tag " ; show_p4(fitted_tag_2_p4) ;
	    if(X!=NULL){ cout << "  X " ; show_p4(fitted_X_2_p4) ; }
	    if(K!=NULL){ cout << "  K " ; show_p4(fitted_K_2_p4) ; }
	    cout << "Ds* " ; show_p4(fitted_Dsstar_2_p4) ;
	    cout << " Ds " ; show_p4(fitted_Ds_2_p4) ;
	    cout << "  g " ; show_p4(fitted_gamma_2_p4) ;
	  }
	  else
	  {
	     fitted_Ups_2 = fitted_Ups_2 ;
	     fitted_tag_2 = fitted_tag_1 ;
	     fitted_Dsstar_2 = fitted_Dsstar_2 ;
	     fitted_Ds_2 = fitted_Ds_2 ;
	     fitted_gamma_2 = *gamma ;
	  } 
	  
	  
	  
	  
	  
	  //Dsp4 = fitted_Dsstar_2_p4-fitted_gamma_2_p4 ;
	  HepLorentzVector Dsp4 = fitted_Dsstar_1_p4-gamma->p4() ;
	  HepLorentzVector mup4 = mu->p4() ;

	  // Get the raw nup4
	  HepLorentzVector nup4_raw = Dsp4-mup4 ;
	  HepLorentzVector nup4 = nup4_raw ;

	  // Now do the momentum correction
	  // Boost into the rest frame of the muon, then the on-shell solutions form a spherical surface
	  Hep3Vector mu_boost = mu->p4().boostVector() ;
	  HepLorentzVector nup4_boosted = nup4_raw ;
	  nup4_boosted.boost(-mu_boost) ;
	  double nup4_rho = nup4_boosted.rho() ;
	  if(nup4_rho==0)
	  {
	    cout << "nu has zero p3" << endl ;
	    continue ;
	  }
	  // Work this out from kinematics
	  float referencep3 = (Ds_mass_PDG*Ds_mass_PDG-mu_mass_PDG*mu_mass_PDG)/(2*mu_mass_PDG) ;
	  nup4_boosted.setX(referencep3*nup4_boosted.x()/nup4_rho) ;
	  nup4_boosted.setY(referencep3*nup4_boosted.y()/nup4_rho) ;
	  nup4_boosted.setZ(referencep3*nup4_boosted.z()/nup4_rho) ;
	  nup4_boosted.setT(referencep3) ;
	  HepLorentzVector nup4_boosted_back = nup4_boosted ;
	  nup4_boosted_back.boost(mu_boost) ;
	  float nu_deltap3 = 0 ;
	  nu_deltap3 = (nup4_raw.x()-nup4_boosted_back.x())*(nup4_raw.x()-nup4_boosted_back.x()) + 
	    (nup4_raw.y()-nup4_boosted_back.y())*(nup4_raw.y()-nup4_boosted_back.y()) + 
	    (nup4_raw.z()-nup4_boosted_back.z())*(nup4_raw.z()-nup4_boosted_back.z()) ;
	  if(nu_deltap3<0)
	  {
	    cout << "nup3^2 is less than zero.  (How did that happen?!)" << endl ;
	    continue ;
	  }
	  nu_deltap3 = sqrt(nu_deltap3) ;
	  cout << "nu deltap3: " << nu_deltap3 << endl ;
	  nup4 = nup4_boosted_back ;


	  // Ignore boosting correction for now
	  nup4 = nup4_raw ;
	  
	  BtaCandidate   tag_3 = fitted_tag_1 ;
	  setMassConstraint(tag_3) ;
	  setBeamConstraint(tag_3, eventInfo) ;

	  BtaCandidate K_3 ;
	  if(K!=NULL)
	  {
	    K_3 = fitted_K_1 ;
	    setMassConstraint(K_3) ;
	    setOriginConstraint(K_3, eventInfo) ;
	  }
	  
	  BtaCandidate X_3 ;
	  if(X!=NULL)
	  {
	    X_3 = fitted_X_1 ;
	    setOriginConstraint(X_3, eventInfo) ;
	  }

	  BtaCandidate    mu_3 = *mu ;
	  setMassConstraint(mu_3) ;

	  BtaCandidate nu_3(nup4, Pdt::lookup(PdtPdg::nu_mu)) ;
	  show_p4(nup4) ;

	  BtaCandidate gamma_3 = *gamma ;
	  setMassConstraint(gamma_3) ;
	  
	  BtaCandidate* Ds_3 = tree_maker.create(mu_3, nu_3) ;
	  if(Dsstar_charge>0) Ds_3->setType(Pdt::lookup(PdtPdg::D_s_plus)) ;
	  if(Dsstar_charge<0) Ds_3->setType(Pdt::lookup(PdtPdg::D_s_minus)) ;
	  setMassConstraint(*Ds_3) ;

	  BtaCandidate* Dsstar_3 = tree_maker.create(*Ds_3, gamma_3) ;
	  if(Dsstar_charge>0) Dsstar_3->setType(Pdt::lookup(PdtPdg::D_s_star_plus)) ;
	  if(Dsstar_charge<0) Dsstar_3->setType(Pdt::lookup(PdtPdg::D_s_star_minus)) ;
	  setMassConstraint(*Dsstar_3) ;
	  setOriginConstraint(*Dsstar_3, eventInfo) ;

	  setBeamConstraint(*Ds_3, Dsstar_3) ;
	  setBeamConstraint(nu_3,  Ds_3) ;
	  setBeamConstraint(mu_3,  Ds_3) ;

	  BtaCandidate* Ups_3 ;
	  if(X!=NULL)
	  {
	    if(K!=NULL)
	    {
	      Ups_3 = tree_maker.create(tag_3, K_3, X_3, *Dsstar_3) ;
	    }
	    else
	    {
	       Ups_3 = tree_maker.create(tag_3, X_3, *Dsstar_3) ;
	    }
	   
	  }
	  else
	  {
	    if(K!=NULL)
	    {
	      Ups_3 = tree_maker.create(tag_3, K_3, *Dsstar_3) ;
	    }
	    else
	    {
	      Ups_3 = tree_maker.create(tag_3, *Dsstar_3) ;
	    }
	  }
	  Ups_3->setType(Pdt::lookup(PdtPdg::Upsilon_4S)) ;
	  setMassConstraint(*Ups_3) ;
	  setBeamConstraint(*Ups_3,eventInfo) ;
	  setEnergyConstraint(*Ups_3,eventInfo) ;

	  // Show p4 before second fit
	  cout << "          === p4 before third fit: === " << endl ;
	  cout << "ups " ; show_p4(Ups_3->p4()) ;
	  cout << "tag " ; show_p4(tag_3.p4()) ;
	  if(X!=NULL){ cout << "  X " ; show_p4(X_3.p4()) ; }
	  if(K!=NULL){ cout << "  K " ; show_p4(K_3.p4()) ; }
	  cout << "Ds* " ; show_p4(Dsstar_3->p4()) ;
	  cout << " Ds " ; show_p4(Ds_3->p4()) ;
	  cout << "  g " ; show_p4(gamma_3.p4()) ;
	  cout << " mu " ; show_p4(mu_3.p4()) ;
	  cout << " nu " ; show_p4(nu_3.p4()) ;
	  
	  // Create fitter
	  VtxFitterOper Ups_fitter_3(*Ups_3, algorithm) ;
	  Ups_fitter_3.invalidateFit() ;
	  Ups_fitter_3.fitAll() ;
	  
	  // Need a temp object here, as we need a pointer for UsrData
	  BtaCandidate fitted_Ups_3_temp = Ups_fitter_3.getFitted(*Ups_3) ;
	  if(fitted_Ups_3_temp.decayVtx()->status())
	  {
	    cout << "Third fit failed with code " << fitted_Ups_3_temp.decayVtx()->status() << endl ; 
	    //continue ;
	  }
	  cout << "Fit prob chi2: " << fitted_Ups_3_temp.decayVtx()->chiSquared() << endl ;
	  HepLorentzVector fitted_Ups_3_p4 = fitted_Ups_3_temp.p4() ;
	  BtaCandidate* fitted_Ups_3 = new BtaCandidate(fitted_Ups_3_p4,Pdt::lookup(PdtPdg::Upsilon_4S)) ;

	  BtaCandidate fitted_tag_3      = Ups_fitter_3.getFitted(tag_3) ;	  
	  BtaCandidate fitted_Dsstar_3   = Ups_fitter_3.getFitted(*Dsstar_3) ;
	  BtaCandidate fitted_Ds_3       = Ups_fitter_3.getFitted(*Ds_3) ;
	  BtaCandidate fitted_gamma_3    = Ups_fitter_3.getFitted(gamma_3) ;
	  BtaCandidate fitted_mu_3       = Ups_fitter_3.getFitted(mu_3) ;
	  BtaCandidate fitted_nu_3       = Ups_fitter_3.getFitted(nu_3) ;
	  
	  HepLorentzVector fitted_tag_3_p4    = fitted_tag_3.p4() ;
	  HepLorentzVector fitted_Dsstar_3_p4 = fitted_Dsstar_3.p4() ;
	  HepLorentzVector fitted_Ds_3_p4     = fitted_Ds_3.p4() ;
	  HepLorentzVector fitted_gamma_3_p4  = fitted_gamma_3.p4() ;
	  HepLorentzVector fitted_mu_3_p4     = fitted_mu_3.p4() ;
	  HepLorentzVector fitted_nu_3_p4     = fitted_nu_3.p4() ;

	  HepLorentzVector fitted_K_3_p4 ;
	  fitted_K_3_p4.setX(0) ;
	  fitted_K_3_p4.setY(0) ;
	  fitted_K_3_p4.setZ(0) ;
	  fitted_K_3_p4.setT(0) ;
	  _K_charge = 0 ;
	  if(K!=NULL)
	  {
	    BtaCandidate fitted_K_3 = Ups_fitter_3.getFitted(K_3) ;
	    fitted_K_3_p4 = fitted_K_3.p4() ;
	    _K_charge = (float)fitted_K_3.charge() ;
	  }

	  HepLorentzVector fitted_X_3_p4 ;
	  fitted_X_3_p4.setX(0) ;
	  fitted_X_3_p4.setY(0) ;
	  fitted_X_3_p4.setZ(0) ;
	  fitted_X_3_p4.setT(0) ;
	  _X_charge = 0 ;
	  if(X!=NULL)
	  {
	    BtaCandidate fitted_X_3 = Ups_fitter_3.getFitted(X_3) ;
	    fitted_X_3_p4 = fitted_X_3.p4() ;
	    _X_charge = (float)fitted_X_3.charge() ;
	  }
	  
	  // Show p4 after fit
	  cout << "          === p4 after third fit: === " << endl ;
	  cout << "ups " ; show_p4(fitted_Ups_3_p4) ;
	  cout << "tag " ; show_p4(fitted_tag_3_p4) ;
	  if(X!=NULL){ cout << "  X " ; show_p4(fitted_X_3_p4) ; }
	  if(K!=NULL){ cout << "  K " ; show_p4(fitted_K_3_p4) ; }
	  cout << "Ds* " ; show_p4(fitted_Dsstar_3_p4) ;
	  cout << " Ds " ; show_p4(fitted_Ds_3_p4) ;
	  cout << "  g " ; show_p4(fitted_gamma_3_p4) ;
	  cout << " mu " ; show_p4(fitted_mu_3_p4) ;
	  cout << " nu " ; show_p4(fitted_nu_3_p4) ;

	  // No need to check for K and X here- the p4 already exist
	  HepLorentzVector fitted_tag_3_p4CM    =    fitted_tag_3_p4.boost(boost_x, boost_y, boost_z) ;
	  HepLorentzVector fitted_K_3_p4CM      =      fitted_K_3_p4.boost(boost_x, boost_y, boost_z) ;
	  HepLorentzVector fitted_X_3_p4CM      =      fitted_X_3_p4.boost(boost_x, boost_y, boost_z) ;
	  HepLorentzVector fitted_Dsstar_3_p4CM = fitted_Dsstar_3_p4.boost(boost_x, boost_y, boost_z) ;
	  HepLorentzVector fitted_Ds_3_p4CM     =     fitted_Ds_3_p4.boost(boost_x, boost_y, boost_z) ;
	  HepLorentzVector fitted_gamma_3_p4CM  =  fitted_gamma_3_p4.boost(boost_x, boost_y, boost_z) ;
	  HepLorentzVector fitted_mu_3_p4CM     =     fitted_mu_3_p4.boost(boost_x, boost_y, boost_z) ;
	  HepLorentzVector fitted_nu_3_p4CM     =     fitted_nu_3_p4.boost(boost_x, boost_y, boost_z) ;

	  _tag_pxCM            = (float)fitted_tag_3_p4CM.x() ;
	  _tag_pyCM            = (float)fitted_tag_3_p4CM.y() ;
	  _tag_pzCM            = (float)fitted_tag_3_p4CM.z() ;
	  _tag_energyCM        = (float)fitted_tag_3_p4CM.e() ;
	  _tag_mass            = (float)fitted_tag_3_p4CM.m() ;

	  _K_pxCM              = (float)fitted_K_3_p4CM.x() ;
	  _K_pyCM              = (float)fitted_K_3_p4CM.y() ;
	  _K_pzCM              = (float)fitted_K_3_p4CM.z() ;
	  _K_energyCM          = (float)fitted_K_3_p4CM.e() ;
	  _K_mass              = (float)fitted_K_3_p4CM.m() ;

	  _X_pxCM              = (float)fitted_X_3_p4CM.x() ;
	  _X_pyCM              = (float)fitted_X_3_p4CM.y() ;
	  _X_pzCM              = (float)fitted_X_3_p4CM.z() ;
	  _X_energyCM          = (float)fitted_X_3_p4CM.e() ;
	  _X_mass              = (float)fitted_X_3_p4CM.m() ;

	  _Dsstarpost_pxCM     = (float)fitted_Dsstar_3_p4CM.x() ;
	  _Dsstarpost_pyCM     = (float)fitted_Dsstar_3_p4CM.y() ;
	  _Dsstarpost_pzCM     = (float)fitted_Dsstar_3_p4CM.z() ;
	  _Dsstarpost_energyCM = (float)fitted_Dsstar_3_p4CM.e() ;
	  _Dsstarpost_mass     = (float)fitted_Dsstar_3_p4CM.m() ;
	  _Dsstarpost_charge   = Dsstar_charge ;

	  _gamma_pxCM          = (float)fitted_gamma_3_p4CM.x() ;
	  _gamma_pyCM          = (float)fitted_gamma_3_p4CM.y() ;
	  _gamma_pzCM          = (float)fitted_gamma_3_p4CM.z() ;
	  _gamma_energyCM      = (float)fitted_gamma_3_p4CM.e() ;

	  _Ds_pxCM             = (float)fitted_Ds_3_p4CM.x() ;
	  _Ds_pyCM             = (float)fitted_Ds_3_p4CM.y() ;
	  _Ds_pzCM             = (float)fitted_Ds_3_p4CM.z() ;
	  _Ds_energyCM         = (float)fitted_Ds_3_p4CM.e() ;
	  _Ds_mass             = (float)fitted_Ds_3_p4CM.m() ;
	  _Ds_charge           = Dsstar_charge ;

	  _mu_pxCM             = (float)fitted_mu_3_p4CM.x() ;
	  _mu_pyCM             = (float)fitted_mu_3_p4CM.y() ;
	  _mu_pzCM             = (float)fitted_mu_3_p4CM.z() ;
	  _mu_energyCM         = (float)fitted_mu_3_p4CM.e() ;
	  _mu_charge           = Dsstar_charge ;

	  _nu_pxCM             = (float)fitted_nu_3_p4CM.x() ;
	  _nu_pyCM             = (float)fitted_nu_3_p4CM.y() ;
	  _nu_pzCM             = (float)fitted_nu_3_p4CM.z() ;
	  _nu_energyCM         = (float)fitted_nu_3_p4CM.e() ;
	  _nu_mass             = (float)fitted_nu_3_p4CM.m() ;

	  _nu_deltap3          = nu_deltap3 ;

	  if(!_Ups_block.put(*fitted_Ups_3,_tag_pxCM))           ErrMsg(error) <<"Can't put "<< _tag_pxCM.name()           <<" into Ups_block"<<endmsg ;
	  if(!_Ups_block.put(*fitted_Ups_3,_tag_pyCM))           ErrMsg(error) <<"Can't put "<< _tag_pyCM.name()           <<" into Ups_block"<<endmsg ;
	  if(!_Ups_block.put(*fitted_Ups_3,_tag_pzCM))           ErrMsg(error) <<"Can't put "<< _tag_pzCM.name()           <<" into Ups_block"<<endmsg ;
	  if(!_Ups_block.put(*fitted_Ups_3,_tag_energyCM))       ErrMsg(error) <<"Can't put "<< _tag_energyCM.name()       <<" into Ups_block"<<endmsg ;
	  if(!_Ups_block.put(*fitted_Ups_3,_tag_mass))           ErrMsg(error) <<"Can't put "<< _tag_mass.name()           <<" into Ups_block"<<endmsg ;
	  if(!_Ups_block.put(*fitted_Ups_3,_tag_charge))         ErrMsg(error) <<"Can't put "<< _tag_charge.name()         <<" into Ups_block"<<endmsg ;

	  if(!_Ups_block.put(*fitted_Ups_3,_K_pxCM))             ErrMsg(error) <<"Can't put "<< _K_pxCM.name()             <<" into Ups_block"<<endmsg ;
	  if(!_Ups_block.put(*fitted_Ups_3,_K_pyCM))             ErrMsg(error) <<"Can't put "<< _K_pyCM.name()             <<" into Ups_block"<<endmsg ;
	  if(!_Ups_block.put(*fitted_Ups_3,_K_pzCM))             ErrMsg(error) <<"Can't put "<< _K_pzCM.name()             <<" into Ups_block"<<endmsg ;
	  if(!_Ups_block.put(*fitted_Ups_3,_K_energyCM))         ErrMsg(error) <<"Can't put "<< _K_energyCM.name()         <<" into Ups_block"<<endmsg ;
	  if(!_Ups_block.put(*fitted_Ups_3,_K_mass))             ErrMsg(error) <<"Can't put "<< _K_mass.name()             <<" into Ups_block"<<endmsg ;
	  if(!_Ups_block.put(*fitted_Ups_3,_K_charge))           ErrMsg(error) <<"Can't put "<< _K_charge.name()           <<" into Ups_block"<<endmsg ;

	  if(!_Ups_block.put(*fitted_Ups_3,_X_pxCM))             ErrMsg(error) <<"Can't put "<< _X_pxCM.name()             <<" into Ups_block"<<endmsg ;
	  if(!_Ups_block.put(*fitted_Ups_3,_X_pyCM))             ErrMsg(error) <<"Can't put "<< _X_pyCM.name()             <<" into Ups_block"<<endmsg ;
	  if(!_Ups_block.put(*fitted_Ups_3,_X_pzCM))             ErrMsg(error) <<"Can't put "<< _X_pzCM.name()             <<" into Ups_block"<<endmsg ;
	  if(!_Ups_block.put(*fitted_Ups_3,_X_energyCM))         ErrMsg(error) <<"Can't put "<< _X_energyCM.name()         <<" into Ups_block"<<endmsg ;
	  if(!_Ups_block.put(*fitted_Ups_3,_X_mass))             ErrMsg(error) <<"Can't put "<< _X_mass.name()             <<" into Ups_block"<<endmsg ;
	  if(!_Ups_block.put(*fitted_Ups_3,_X_charge))           ErrMsg(error) <<"Can't put "<< _X_charge.name()           <<" into Ups_block"<<endmsg ;

	  if(!_Ups_block.put(*fitted_Ups_3,_Dsstarpre_pxCM))     ErrMsg(error) <<"Can't put "<< _Dsstarpre_pxCM.name()     <<" into Ups_block"<<endmsg ;
	  if(!_Ups_block.put(*fitted_Ups_3,_Dsstarpre_pyCM))     ErrMsg(error) <<"Can't put "<< _Dsstarpre_pyCM.name()     <<" into Ups_block"<<endmsg ;
	  if(!_Ups_block.put(*fitted_Ups_3,_Dsstarpre_pzCM))     ErrMsg(error) <<"Can't put "<< _Dsstarpre_pzCM.name()     <<" into Ups_block"<<endmsg ;
	  if(!_Ups_block.put(*fitted_Ups_3,_Dsstarpre_energyCM)) ErrMsg(error) <<"Can't put "<< _Dsstarpre_energyCM.name() <<" into Ups_block"<<endmsg ;
	  if(!_Ups_block.put(*fitted_Ups_3,_Dsstarpre_mass))     ErrMsg(error) <<"Can't put "<< _Dsstarpre_mass.name()     <<" into Ups_block"<<endmsg ;
	  if(!_Ups_block.put(*fitted_Ups_3,_Dsstarpre_charge))   ErrMsg(error) <<"Can't put "<< _Dsstarpre_charge.name()   <<" into Ups_block"<<endmsg ;

	  if(!_Ups_block.put(*fitted_Ups_3,_Dsstarpost_pxCM))    ErrMsg(error) <<"Can't put "<< _Dsstarpost_pxCM.name()    <<" into Ups_block"<<endmsg ;
	  if(!_Ups_block.put(*fitted_Ups_3,_Dsstarpost_pyCM))    ErrMsg(error) <<"Can't put "<< _Dsstarpost_pyCM.name()    <<" into Ups_block"<<endmsg ;
	  if(!_Ups_block.put(*fitted_Ups_3,_Dsstarpost_pzCM))    ErrMsg(error) <<"Can't put "<< _Dsstarpost_pzCM.name()    <<" into Ups_block"<<endmsg ;
	  if(!_Ups_block.put(*fitted_Ups_3,_Dsstarpost_energyCM))ErrMsg(error) <<"Can't put "<< _Dsstarpost_energyCM.name()<<" into Ups_block"<<endmsg ;
	  if(!_Ups_block.put(*fitted_Ups_3,_Dsstarpost_mass))    ErrMsg(error) <<"Can't put "<< _Dsstarpost_mass.name()    <<" into Ups_block"<<endmsg ;
	  if(!_Ups_block.put(*fitted_Ups_3,_Dsstarpost_charge))  ErrMsg(error) <<"Can't put "<< _Dsstarpost_charge.name()  <<" into Ups_block"<<endmsg ;

	  if(!_Ups_block.put(*fitted_Ups_3,_gamma_pxCM))         ErrMsg(error) <<"Can't put "<< _gamma_pxCM.name()         <<" into Ups_block"<<endmsg ;
	  if(!_Ups_block.put(*fitted_Ups_3,_gamma_pyCM))         ErrMsg(error) <<"Can't put "<< _gamma_pyCM.name()         <<" into Ups_block"<<endmsg ;
	  if(!_Ups_block.put(*fitted_Ups_3,_gamma_pzCM))         ErrMsg(error) <<"Can't put "<< _gamma_pzCM.name()         <<" into Ups_block"<<endmsg ;
	  if(!_Ups_block.put(*fitted_Ups_3,_gamma_energyCM))     ErrMsg(error) <<"Can't put "<< _gamma_energyCM.name()     <<" into Ups_block"<<endmsg ;

	  if(!_Ups_block.put(*fitted_Ups_3,_Ds_pxCM))            ErrMsg(error) <<"Can't put "<< _Ds_pxCM.name()            <<" into Ups_block"<<endmsg ;
	  if(!_Ups_block.put(*fitted_Ups_3,_Ds_pyCM))            ErrMsg(error) <<"Can't put "<< _Ds_pyCM.name()            <<" into Ups_block"<<endmsg ;
	  if(!_Ups_block.put(*fitted_Ups_3,_Ds_pzCM))            ErrMsg(error) <<"Can't put "<< _Ds_pzCM.name()            <<" into Ups_block"<<endmsg ;
	  if(!_Ups_block.put(*fitted_Ups_3,_Ds_energyCM))        ErrMsg(error) <<"Can't put "<< _Ds_energyCM.name()        <<" into Ups_block"<<endmsg ;
	  if(!_Ups_block.put(*fitted_Ups_3,_Ds_mass))            ErrMsg(error) <<"Can't put "<< _Ds_mass.name()            <<" into Ups_block"<<endmsg ;
	  if(!_Ups_block.put(*fitted_Ups_3,_Ds_charge))          ErrMsg(error) <<"Can't put "<< _Ds_charge.name()          <<" into Ups_block"<<endmsg ;
	  if(!_Ups_block.put(*fitted_Ups_3,_Ds_premass))         ErrMsg(error) <<"Can't put "<< _Ds_premass.name()         <<" into Ups_block"<<endmsg ;
	  
	  if(!_Ups_block.put(*fitted_Ups_3,_mu_pxCM))            ErrMsg(error) <<"Can't put "<< _mu_pxCM.name()            <<" into Ups_block"<<endmsg ;
	  if(!_Ups_block.put(*fitted_Ups_3,_mu_pyCM))            ErrMsg(error) <<"Can't put "<< _mu_pyCM.name()            <<" into Ups_block"<<endmsg ;
	  if(!_Ups_block.put(*fitted_Ups_3,_mu_pzCM))            ErrMsg(error) <<"Can't put "<< _mu_pzCM.name()            <<" into Ups_block"<<endmsg ;
	  if(!_Ups_block.put(*fitted_Ups_3,_mu_energyCM))        ErrMsg(error) <<"Can't put "<< _mu_energyCM.name()        <<" into Ups_block"<<endmsg ;
	  if(!_Ups_block.put(*fitted_Ups_3,_mu_charge))          ErrMsg(error) <<"Can't put "<< _mu_charge.name()          <<" into Ups_block"<<endmsg ;

	  if(!_Ups_block.put(*fitted_Ups_3,_nu_pxCM))            ErrMsg(error) <<"Can't put "<< _nu_pxCM.name()            << " into Ups_block"<<endmsg ;
	  if(!_Ups_block.put(*fitted_Ups_3,_nu_pyCM))            ErrMsg(error) <<"Can't put "<< _nu_pyCM.name()            << " into Ups_block"<<endmsg ;
	  if(!_Ups_block.put(*fitted_Ups_3,_nu_pzCM))            ErrMsg(error) <<"Can't put "<< _nu_pzCM.name()            << " into Ups_block"<<endmsg ;
	  if(!_Ups_block.put(*fitted_Ups_3,_nu_energyCM))        ErrMsg(error) <<"Can't put "<< _nu_energyCM.name()        << " into Ups_block"<<endmsg ;
	  if(!_Ups_block.put(*fitted_Ups_3,_nu_mass))            ErrMsg(error) <<"Can't put "<< _nu_mass.name()            << " into Ups_block"<<endmsg ;
	  if(!_Ups_block.put(*fitted_Ups_3,_nu_deltap3))         ErrMsg(error) <<"Can't put "<< _nu_deltap3.name()         << " into Ups_block"<<endmsg ;

	  if(!_Ups_block.put(*fitted_Ups_3,_Ds_testmass))        ErrMsg(error) <<"Can't put "<< _Ds_testmass.name()        << " into Ups_block"<<endmsg ;
	  if(!_Ups_block.put(*fitted_Ups_3,_nu_testmass_a))      ErrMsg(error) <<"Can't put "<< _nu_testmass_a.name()      << " into Ups_block"<<endmsg ;
	  if(!_Ups_block.put(*fitted_Ups_3,_nu_testmass_b))      ErrMsg(error) <<"Can't put "<< _nu_testmass_b.name()      << " into Ups_block"<<endmsg ;
	  if(!_Ups_block.put(*fitted_Ups_3,_truthmatched))       ErrMsg(error) <<"Can't put "<< _truthmatched.name()       << " into Ups_block"<<endmsg ;

	  outputList->append(fitted_Ups_3) ;

	  delete Ups_1 ;
	  delete Ups_3 ;
	} // mu
      } // gamma
    } // tag
  } // tag list
  _n_tag1  = n_tags[1] ;
  _n_tag2  = n_tags[2] ;
  _n_tag3  = n_tags[3] ;
  _n_tag4  = n_tags[4] ;
  _n_tag5  = n_tags[5] ;
  _n_tag6  = n_tags[6] ;
  _n_tag7  = n_tags[7] ;
  _n_tag8  = n_tags[8] ;
  _n_tag9  = n_tags[9] ;
  _n_tag10 = n_tags[10] ;
  _n_tag11 = n_tags[11] ;
  _n_tag12 = n_tags[12] ;
  _n_tag13 = n_tags[13] ;
  _n_tag14 = n_tags[14] ;
  _n_tag15 = n_tags[15] ;
  _n_tag16 = n_tags[16] ;
  _n_tag17 = n_tags[17] ;
  //_n_tag18 = n_tags[18] ;
  _n_tag19 = n_tags[19] ;
  _n_tag20 = n_tags[20] ;
  _n_tag21 = n_tags[21] ;
  _n_tag22 = n_tags[22] ;
  _n_tag23 = n_tags[23] ;
  _n_tag24 = n_tags[24] ;
  _n_tag25 = n_tags[25] ;
  _n_tag26 = n_tags[26] ;
  _n_tag27 = n_tags[27] ;
  _n_tag28 = n_tags[28] ;
      
  // put into usr_event_block
  if(!_event_block.put(_n_tag1 )) ErrMsg(error) << "Can't put " << _n_tag1.name()  << " into event_data" << endmsg;
  if(!_event_block.put(_n_tag2 )) ErrMsg(error) << "Can't put " << _n_tag2.name()  << " into event_data" << endmsg;
  if(!_event_block.put(_n_tag3 )) ErrMsg(error) << "Can't put " << _n_tag3.name()  << " into event_data" << endmsg;
  if(!_event_block.put(_n_tag4 )) ErrMsg(error) << "Can't put " << _n_tag4.name()  << " into event_data" << endmsg;
  if(!_event_block.put(_n_tag5 )) ErrMsg(error) << "Can't put " << _n_tag5.name()  << " into event_data" << endmsg;
  if(!_event_block.put(_n_tag6 )) ErrMsg(error) << "Can't put " << _n_tag6.name()  << " into event_data" << endmsg;
  if(!_event_block.put(_n_tag7 )) ErrMsg(error) << "Can't put " << _n_tag7.name()  << " into event_data" << endmsg;
  if(!_event_block.put(_n_tag8 )) ErrMsg(error) << "Can't put " << _n_tag8.name()  << " into event_data" << endmsg;
  if(!_event_block.put(_n_tag9 )) ErrMsg(error) << "Can't put " << _n_tag9.name()  << " into event_data" << endmsg;
  
  if(!_event_block.put(_n_tag10)) ErrMsg(error) << "Can't put " << _n_tag10.name() << " into event_data" << endmsg;
  if(!_event_block.put(_n_tag11)) ErrMsg(error) << "Can't put " << _n_tag11.name() << " into event_data" << endmsg;
  if(!_event_block.put(_n_tag12)) ErrMsg(error) << "Can't put " << _n_tag12.name() << " into event_data" << endmsg;
  if(!_event_block.put(_n_tag13)) ErrMsg(error) << "Can't put " << _n_tag13.name() << " into event_data" << endmsg;
  if(!_event_block.put(_n_tag14)) ErrMsg(error) << "Can't put " << _n_tag14.name() << " into event_data" << endmsg;
  if(!_event_block.put(_n_tag15)) ErrMsg(error) << "Can't put " << _n_tag15.name() << " into event_data" << endmsg;
  if(!_event_block.put(_n_tag16)) ErrMsg(error) << "Can't put " << _n_tag16.name() << " into event_data" << endmsg;
  if(!_event_block.put(_n_tag17)) ErrMsg(error) << "Can't put " << _n_tag17.name() << " into event_data" << endmsg;
  //if(!_event_block.put(_n_tag18)) ErrMsg(error) << "Can't put " << _n_tag18.name() << " into event_data" << endmsg;
  if(!_event_block.put(_n_tag19)) ErrMsg(error) << "Can't put " << _n_tag19.name() << " into event_data" << endmsg;
  
  if(!_event_block.put(_n_tag20)) ErrMsg(error) << "Can't put " << _n_tag20.name() << " into event_data" << endmsg;
  if(!_event_block.put(_n_tag21)) ErrMsg(error) << "Can't put " << _n_tag21.name() << " into event_data" << endmsg;
  if(!_event_block.put(_n_tag22)) ErrMsg(error) << "Can't put " << _n_tag22.name() << " into event_data" << endmsg;
  if(!_event_block.put(_n_tag23)) ErrMsg(error) << "Can't put " << _n_tag23.name() << " into event_data" << endmsg;
  if(!_event_block.put(_n_tag24)) ErrMsg(error) << "Can't put " << _n_tag24.name() << " into event_data" << endmsg;
  if(!_event_block.put(_n_tag25)) ErrMsg(error) << "Can't put " << _n_tag25.name() << " into event_data" << endmsg;
  if(!_event_block.put(_n_tag26)) ErrMsg(error) << "Can't put " << _n_tag26.name() << " into event_data" << endmsg;
  if(!_event_block.put(_n_tag27)) ErrMsg(error) << "Can't put " << _n_tag27.name() << " into event_data" << endmsg;
  if(!_event_block.put(_n_tag28)) ErrMsg(error) << "Can't put " << _n_tag28.name() << " into event_data" << endmsg;

  if(!_event_block.put(_n_RS))    ErrMsg(error) << "Can't put " << _n_RS.name()    << " into event_data" << endmsg;
  if(!_event_block.put(_n_WS))    ErrMsg(error) << "Can't put " << _n_WS.name()    << " into event_data" << endmsg;
  if(!_event_block.put(_n_FH))    ErrMsg(error) << "Can't put " << _n_FH.name()    << " into event_data" << endmsg;
  if(!_event_block.put(_n_BH))    ErrMsg(error) << "Can't put " << _n_BH.name()    << " into event_data" << endmsg;
  if(!_event_block.put(_n_FH_RS)) ErrMsg(error) << "Can't put " << _n_FH_RS.name() << " into event_data" << endmsg;
  if(!_event_block.put(_n_FH_WS)) ErrMsg(error) << "Can't put " << _n_FH_WS.name() << " into event_data" << endmsg;
  if(!_event_block.put(_n_BH_RS)) ErrMsg(error) << "Can't put " << _n_BH_RS.name() << " into event_data" << endmsg;
  if(!_event_block.put(_n_BH_WS)) ErrMsg(error) << "Can't put " << _n_BH_WS.name() << " into event_data" << endmsg;
  if(!_event_block.put(_n_signal_gamma)) ErrMsg(error) << "Can't put " << _n_signal_gamma.name() << " into event_data" << endmsg;
  if(!_event_block.put(_visible_charge)) ErrMsg(error) << "Can't put " << _visible_charge.name() << " into event_data" << endmsg;

  // Start to look at double tag samples
  // Reconstruct a tag on the "signal" side which does not overlap with any real tag and remove the CM momentum cut on the double tag candidate
  HepAListIterator<BtaCandidate> outputList_iter(*outputList) ;
  
  HepAList<BtaCandidate>* double_tag_list = locateList(anEvent, _double_tag_list.valueString());
  HepAListIterator<BtaCandidate> double_tag_iter(*double_tag_list) ;
  while(BtaCandidate* DT_cand = double_tag_iter())
  {
    BtaCandidate* DT_cand_to_append = new BtaCandidate(*DT_cand) ;

    bool append_to_outputList = true ;
    bool other_side = false ;
    while(BtaCandidate* tag_check = outputList_iter())
    {
      if(DT_cand->overlaps(*tag_check)) append_to_outputList = false ;
      float dot_product = CM_dot_product(anEvent, DT_cand, tag_check) ;
      if(dot_product<0) other_side = true ;
    }
    if(append_to_outputList && other_side && false)
    {
      // put usr variable into usr_D_block
      _RS        = -999 ;
      _WS        = -999 ;
      _FH        = -999 ;
      _BH        = -999 ;
      _FH_RS     = -999 ;
      _FH_WS     = -999 ;
      _BH_RS     = -999 ;
      _BH_WS     = -999 ;
      _is_a_tag  = 0 ;
      _is_DT     = 1 ;
      _is_sig    = 1 ;
      _is_fitted = 1 ;
      
      if(!_D_block.put(*DT_cand_to_append,_tag_mode)) ErrMsg(error) << "Can't put " << _tag_mode.name() << " into D_block" << endmsg;
      if(!_D_block.put(*DT_cand_to_append,_RS))       ErrMsg(error) << "Can't put " << _RS.name()       << " into D_block" << endmsg;
      if(!_D_block.put(*DT_cand_to_append,_WS))       ErrMsg(error) << "Can't put " << _WS.name()       << " into D_block" << endmsg;
      if(!_D_block.put(*DT_cand_to_append,_FH))       ErrMsg(error) << "Can't put " << _FH.name()       << " into D_block" << endmsg;
      if(!_D_block.put(*DT_cand_to_append,_BH))       ErrMsg(error) << "Can't put " << _BH.name()       << " into D_block" << endmsg;
      if(!_D_block.put(*DT_cand_to_append,_FH_RS))    ErrMsg(error) << "Can't put " << _FH_RS.name()    << " into D_block" << endmsg;
      if(!_D_block.put(*DT_cand_to_append,_FH_WS))    ErrMsg(error) << "Can't put " << _FH_WS.name()    << " into D_block" << endmsg;
      if(!_D_block.put(*DT_cand_to_append,_BH_RS))    ErrMsg(error) << "Can't put " << _BH_RS.name()    << " into D_block" << endmsg;
      if(!_D_block.put(*DT_cand_to_append,_BH_WS))    ErrMsg(error) << "Can't put " << _BH_WS.name()    << " into D_block" << endmsg;
      if(!_D_block.put(*DT_cand_to_append,_is_a_tag)) ErrMsg(error) << "Can't put " << _is_a_tag.name() << " into D_block" << endmsg;
      if(!_D_block.put(*DT_cand_to_append,_is_DT))    ErrMsg(error) << "Can't put " << _is_DT.name()    << " into D_block" << endmsg;
      if(!_D_block.put(*DT_cand_to_append,_is_sig))   ErrMsg(error) << "Can't put " << _is_sig.name()   << " into D_block" << endmsg;
      if(!_D_block.put(*DT_cand_to_append,_is_fitted))ErrMsg(error) << "Can't put " << _is_fitted.name()<< " into D_block" << endmsg;
      
      outputList->append(DT_cand_to_append) ;
    }

  }

  // Get missing mass, energy and momentum
  HepLorentzVector p4MissCM = p4MissCM_pion(anEvent) ;
  // Make sure these get cast as floats!
  float mMiss_pion    = p4MissCM.m() ;
  float p3MissCM_pion = p4MissCM.rho() ;
  float pxMissCM_pion = p4MissCM.x() ;
  float pyMissCM_pion = p4MissCM.y() ;
  float pzMissCM_pion = p4MissCM.z() ;
  float EMissCM_pion  = p4MissCM.e() ;
  _mMiss_pion = mMiss_pion ;
  _p3MissCM_pion = p3MissCM_pion ;
  _pxMissCM_pion = pxMissCM_pion ;
  _pyMissCM_pion = pyMissCM_pion ;
  _pzMissCM_pion = pzMissCM_pion ;
  _EMissCM_pion  = EMissCM_pion ;

  //cout << "MMiss, PMissCM, EMissCM: " << MMiss_pion << " " << PMissCM_pion << " " << EMissCM_pion << endl ;
  if(!_event_block.put(_mMiss_pion)) ErrMsg(error)    << "Can't put " << _mMiss_pion.name()    << " into event_data" << endmsg;
  if(!_event_block.put(_p3MissCM_pion)) ErrMsg(error) << "Can't put " << _p3MissCM_pion.name() << " into event_data" << endmsg;
  if(!_event_block.put(_pxMissCM_pion)) ErrMsg(error) << "Can't put " << _pxMissCM_pion.name() << " into event_data" << endmsg;
  if(!_event_block.put(_pyMissCM_pion)) ErrMsg(error) << "Can't put " << _pyMissCM_pion.name() << " into event_data" << endmsg;
  if(!_event_block.put(_pzMissCM_pion)) ErrMsg(error) << "Can't put " << _pzMissCM_pion.name() << " into event_data" << endmsg;
  if(!_event_block.put(_EMissCM_pion)) ErrMsg(error)  << "Can't put " << _EMissCM_pion.name()  << " into event_data" << endmsg;

  // Do the same but with cascading PID
  p4MissCM = p4MissCM_cascade(anEvent, 0, 0) ;
  float mMiss_cascade    = p4MissCM.m() ;
  float p3MissCM_cascade = p4MissCM.rho() ;
  float pxMissCM_cascade = p4MissCM.x() ;
  float pyMissCM_cascade = p4MissCM.y() ;
  float pzMissCM_cascade = p4MissCM.z() ;
  float EMissCM_cascade  = p4MissCM.e() ;
  _mMiss_cascade = mMiss_cascade ;
  _p3MissCM_cascade = p3MissCM_cascade ;
  _pxMissCM_cascade = pxMissCM_cascade ;
  _pyMissCM_cascade = pyMissCM_cascade ;
  _pzMissCM_cascade = pzMissCM_cascade ;
  _EMissCM_cascade  = EMissCM_cascade ;

  if(!_event_block.put(_mMiss_cascade))    ErrMsg(error) << "Can't put " << _mMiss_cascade.name()    << " into event_data" << endmsg;
  if(!_event_block.put(_p3MissCM_cascade)) ErrMsg(error) << "Can't put " << _p3MissCM_cascade.name() << " into event_data" << endmsg;
  if(!_event_block.put(_pxMissCM_cascade)) ErrMsg(error) << "Can't put " << _pxMissCM_cascade.name() << " into event_data" << endmsg;
  if(!_event_block.put(_pyMissCM_cascade)) ErrMsg(error) << "Can't put " << _pyMissCM_cascade.name() << " into event_data" << endmsg;
  if(!_event_block.put(_pzMissCM_cascade)) ErrMsg(error) << "Can't put " << _pzMissCM_cascade.name() << " into event_data" << endmsg;
  if(!_event_block.put(_EMissCM_cascade))  ErrMsg(error) << "Can't put " << _EMissCM_cascade.name()  << " into event_data" << endmsg;

  float cosThMiss = CosThMiss(anEvent) ;
  _CosThMiss_pion = cosThMiss ;
  if(!_event_block.put(_CosThMiss_pion)) ErrMsg(error) << "Can't put " << _CosThMiss_pion.name() << " into event_data" << endmsg;

  return AppResult::OK;
}
  
HepAList< BtaCandidate >* DRecoilToolsUser::locateList(AbsEvent* anEvent, const std::string& listKey)
{
  HepAList< BtaCandidate >* list = Ifd< HepAList< BtaCandidate > >::get(anEvent, listKey.c_str());
  if (list == 0) ErrMsg(fatal) << "Could not locate list " << listKey.c_str() << ". This is likely to be a configuration error." << endmsg;
  return list;
}

HepAList<BtaCandidate>* DRecoilToolsUser::get_tag_list(AbsEvent* anEvent, int i)
{
  HepAList< BtaCandidate>* tag_list = 0 ;
  if(i==1)  tag_list = locateList(anEvent, _tag1_list.valueString());
  if(i==2)  tag_list = locateList(anEvent, _tag2_list.valueString());
  if(i==3)  tag_list = locateList(anEvent, _tag3_list.valueString());
  if(i==4)  tag_list = locateList(anEvent, _tag4_list.valueString());
  if(i==5)  tag_list = locateList(anEvent, _tag5_list.valueString());
  if(i==6)  tag_list = locateList(anEvent, _tag6_list.valueString());
  if(i==7)  tag_list = locateList(anEvent, _tag7_list.valueString());
  if(i==8)  tag_list = locateList(anEvent, _tag8_list.valueString());
  if(i==9)  tag_list = locateList(anEvent, _tag9_list.valueString());
  if(i==10) tag_list = locateList(anEvent, _tag10_list.valueString());
  if(i==11) tag_list = locateList(anEvent, _tag11_list.valueString());
  if(i==12) tag_list = locateList(anEvent, _tag12_list.valueString());
  if(i==13) tag_list = locateList(anEvent, _tag13_list.valueString());
  if(i==14) tag_list = locateList(anEvent, _tag14_list.valueString());
  if(i==15) tag_list = locateList(anEvent, _tag15_list.valueString());
  if(i==16) tag_list = locateList(anEvent, _tag16_list.valueString());
  if(i==17) tag_list = locateList(anEvent, _tag17_list.valueString());
  //if(i==18) tag_list = locateList(anEvent, _tag18_list.valueString());
  if(i==19) tag_list = locateList(anEvent, _tag19_list.valueString());
  if(i==20) tag_list = locateList(anEvent, _tag20_list.valueString());
  if(i==21) tag_list = locateList(anEvent, _tag21_list.valueString());
  if(i==22) tag_list = locateList(anEvent, _tag22_list.valueString());
  if(i==23) tag_list = locateList(anEvent, _tag23_list.valueString());
  if(i==24) tag_list = locateList(anEvent, _tag24_list.valueString());
  if(i==25) tag_list = locateList(anEvent, _tag25_list.valueString());
  if(i==26) tag_list = locateList(anEvent, _tag26_list.valueString());
  if(i==27) tag_list = locateList(anEvent, _tag27_list.valueString());
  if(i==28) tag_list = locateList(anEvent, _tag28_list.valueString());
  
  return tag_list ;
}

bool DRecoilToolsUser::is_a_tag(int i)
{
  if(i==1 ) return true  ;
  if(i==2 ) return false ;
  if(i==3 ) return true  ;
  if(i==4 ) return true  ;
  if(i==5 ) return true  ;
  if(i==6 ) return true  ;
  if(i==7 ) return true  ;
  if(i==8 ) return true  ;
  if(i==9 ) return true  ;
  if(i==10) return false ;
  if(i==11) return false ;
  if(i==12) return true  ;
  if(i==13) return false ;
  if(i==14) return false ;
  if(i==15) return true  ;
  if(i==16) return true  ;
  if(i==17) return false ;
  if(i==18) return false ;
  if(i==19) return true  ;
  if(i==20) return true  ;
  if(i==21) return true  ;
  if(i==22) return false ;
  if(i==23) return false ;
  if(i==24) return false ;
  if(i==25) return false ;
  if(i==26) return false ;
  if(i==27) return false ;
  if(i==28) return false ;
  if(i==29) return false ;
  return false ;
}

float DRecoilToolsUser::CM_dot_product(AbsEvent* anEvent, BtaCandidate* c1, BtaCandidate* c2)
{
  // Take two BtaCandidates in the lab frame and find their dot product in the CM frame
  if(c1->p()==0) return -999 ;
  if(c2->p()==0) return -999 ;

  // Get CM boost information
  HepLorentzVector CMp4 = CMBoost(anEvent) ;

  HepLorentzVector c1_p4 = c1->p4() ;
  HepLorentzVector c1_p4CM = c1_p4.boost(-CMp4.boostVector()) ;
  HepLorentzVector c2_p4 = c2->p4() ;
  HepLorentzVector c2_p4CM = c2_p4.boost(-CMp4.boostVector()) ;
  Hep3Vector c1_p3CM = c1_p4CM.vect() ;
  Hep3Vector c2_p3CM = c2_p4CM.vect() ;
  float dot_product = c1_p3CM.dot(c2_p3CM) ;

  return dot_product ;
}

HepLorentzVector DRecoilToolsUser::p4MissCM_pion(AbsEvent* anEvent)
{
  HepLorentzVector p4Miss = p4Miss_pion(anEvent) ;
  // Get CM boost information
  HepLorentzVector eep4 = Upsp4(anEvent) ;
  HepLorentzVector p4MissCM = p4Miss.boost(-eep4.boostVector()) ;
  return p4MissCM ;
}

HepLorentzVector DRecoilToolsUser::p4Miss_pion(AbsEvent* anEvent)
{
  // Assume every track has a pion mass.  Really easy.
  // In fact I stole bits of it from Shane

  HepLorentzVector tracks_p4 = 0 ;
  HepAList<BtaCandidate> *tracks_list = locateList(anEvent,"ChargedTracks" ) ;
  HepAListIterator<BtaCandidate> tracks_iter(*tracks_list);
  while(BtaCandidate* tracks_cand = tracks_iter())
  {
    tracks_p4 += tracks_cand->p4();
  }
  
  HepLorentzVector neutrals_p4 = 0;
  HepAList<BtaCandidate> *neutrals_list = locateList(anEvent,"CalorNeutral" ) ;
  HepAListIterator<BtaCandidate> neutrals_iter(*neutrals_list);
  while(BtaCandidate* neutrals_cand = neutrals_iter())
  {
    neutrals_p4 += neutrals_cand->p4();
  }
  HepLorentzVector eep4 = Upsp4(anEvent) ;
  HepLorentzVector p4Miss = 0 ;
  p4Miss = eep4 - tracks_p4 - neutrals_p4 ;

  return p4Miss ;
}

float DRecoilToolsUser::CosThMiss(AbsEvent* anEvent)
{
  // Need to get CosThMiss in the lab frame in order to study events that lose particles down the beampipe
  HepLorentzVector p4Miss = p4Miss_pion(anEvent) ;
  return p4Miss.cosTheta() ;
}

HepLorentzVector DRecoilToolsUser::p4MissCM_cascade(AbsEvent* anEvent, int SVT, int DCH)
{
  // Get CM boost information
  HepLorentzVector eep4 = Upsp4(anEvent) ;

  // Loop through ChargedTracks and cascade the PID
  HepLorentzVector tracks_p4 = 0 ;

  int current_uid = -1 ;
  HepAList<BtaCandidate> *ChargedTracks_list = locateList(anEvent,"ChargedTracks" ) ;
  HepAListIterator<BtaCandidate> ChargedTracks_iter(*ChargedTracks_list) ;

  HepAList<BtaCandidate> *muon_list     = locateList(anEvent, _muon_list.valueString()) ;
  HepAList<BtaCandidate> *electron_list = locateList(anEvent, _electron_list.valueString()) ;
  HepAList<BtaCandidate> *proton_list   = locateList(anEvent, _proton_list.valueString()) ;
  HepAList<BtaCandidate> *kaon_list     = locateList(anEvent, _kaon_list.valueString()) ;

  HepAListIterator<BtaCandidate> muon_iter(*muon_list) ;
  HepAListIterator<BtaCandidate> electron_iter(*electron_list) ;
  HepAListIterator<BtaCandidate> proton_iter(*proton_list) ;
  HepAListIterator<BtaCandidate> kaon_iter(*kaon_list) ;

  bool is_a_kaon = false ;
  bool is_a_muon = false ;
  bool is_an_electron = false ;
  bool is_a_proton = false ;
  int n_pass = 0 ;

  while(BtaCandidate* ChargedTracks_cand = ChargedTracks_iter())
  {
    // First check the SVT/DCH quality of the track
    const BtaTrkQual* TrkQual = ChargedTracks_cand->getMicroAdapter()->getTrkQual() ;
    int n_SVTHits = TrkQual->nSvtHits() ;
    int n_DCHHits = TrkQual->nDchHits() ;

    // These are arbitrary requirements.  Would be nice to do a study on these.
    int SVT_threshold = SVT ;
    int DCH_threshold = DCH ;
    if(n_SVTHits<SVT_threshold) continue ;
    if(n_DCHHits<DCH_threshold) continue ;

    // Must take the 4-vector to get the momentum corrections for the given mass.
    // Assume it's a pion and overwrite track_p4 if it's identified as something else
    HepLorentzVector track_p4 = ChargedTracks_cand->p4() ;
    current_uid = ChargedTracks_cand->uid() ;
    is_a_kaon = false ;
    is_a_muon = false ;
    is_an_electron = false ;
    is_a_proton = false ;

    n_pass = 0 ;

    while(BtaCandidate* cand = kaon_iter())
    {
      if(cand->uid()==current_uid)
      {
	is_a_kaon = true ;
	n_pass++ ;
      }
    }
    kaon_iter.rewind() ;
    
    while(BtaCandidate* cand = proton_iter())
    {
      if(cand->uid()==current_uid)
      {
	is_a_proton = true ;
	n_pass++ ;
      }
    }
    proton_iter.rewind() ;

    while(BtaCandidate* cand = electron_iter())
    {
      if(cand->uid()==current_uid)
      {
	is_an_electron = true ;
	n_pass++ ;
      }
    }
    electron_iter.rewind() ;

    while(BtaCandidate* cand = muon_iter())
    {
      if(cand->uid()==current_uid)
      {
	is_a_muon = true ;
	n_pass++ ;
      }
    }
    muon_iter.rewind() ;

    if(n_pass==0)
    {
      track_p4 = ChargedTracks_cand->p4() ;
    }
    else
    {
      if(n_pass==1)
      {
	if(is_a_kaon)
	{
	  while(BtaCandidate* cand = kaon_iter())
	  {
	    if(cand->uid()==current_uid) track_p4 = cand->p4() ;
	  }
	  kaon_iter.rewind() ;
	}
	if(is_a_muon)
	{
	  while(BtaCandidate* cand = muon_iter())
	  {
	    if(cand->uid()==current_uid) track_p4 = cand->p4() ;
	  }
	  muon_iter.rewind() ;
	}
	if(is_an_electron)
	{
	  while(BtaCandidate* cand = electron_iter())
	  {
	    if(cand->uid()==current_uid) track_p4 = cand->p4() ;
	  }
	  electron_iter.rewind() ;
	}
	if(is_a_proton)
	{
	  while(BtaCandidate* cand = proton_iter())
	  {
	    if(cand->uid()==current_uid) track_p4 = cand->p4() ;
	  }
	  proton_iter.rewind() ;
	}
      }
      else
      {
	// Use Owen's choice of cascade
	if(is_a_kaon && is_a_muon)
	{
	  while(BtaCandidate* cand = kaon_iter())
	  {
	    if(cand->uid()==current_uid) track_p4 = cand->p4() ;
	  }
	  kaon_iter.rewind() ;
	}
	else
	{
	  if(is_an_electron && is_a_kaon)
	  {
	    //Crazy _kSel stuff should go here
	    while(BtaCandidate* cand = kaon_iter())
	    {
	      if(cand->uid()==current_uid) track_p4 = cand->p4() ;
	    }
	    kaon_iter.rewind() ;
	  }
	  else
	  {
	    if(is_a_proton)
	    {
	      // Don't trust the proton PID!
	      if(is_an_electron)
	      {
		while(BtaCandidate* cand = electron_iter())
		{
		  if(cand->uid()==current_uid) track_p4 = cand->p4() ;
		}
		electron_iter.rewind() ;
	      }
	      else
	      {
		if(is_a_muon)
		{
		  while(BtaCandidate* cand = muon_iter())
		  {
		    if(cand->uid()==current_uid) track_p4 = cand->p4() ;
		  }
		  muon_iter.rewind() ;
		}
		else // Must be a kaon
		{
		  while(BtaCandidate* cand = kaon_iter())
		  {
		    if(cand->uid()==current_uid) track_p4 = cand->p4() ;
		  }
		  kaon_iter.rewind() ;
		}
	      }
	    }
	  }
	}
      }
    }
    is_a_kaon = false ;
    is_a_muon = false ;
    is_an_electron = false ;
    is_a_proton = false ;
    
    tracks_p4 += track_p4 ;
  }

  HepLorentzVector neutrals_p4 = 0;
  HepAList<BtaCandidate> *neutrals_list = locateList(anEvent,"CalorNeutral" ) ;
  HepAListIterator<BtaCandidate> neutrals_iter(*neutrals_list);
  while(BtaCandidate* neutrals_cand = neutrals_iter())
  {
    neutrals_p4 += neutrals_cand->p4();
  }
  
  HepLorentzVector p4Miss = 0 ;
  p4Miss = eep4 - tracks_p4 - neutrals_p4 ;
  HepLorentzVector p4MissCM = p4Miss.boost(-eep4.boostVector()) ;

  return p4MissCM ;
}

HepLorentzVector DRecoilToolsUser::CMBoost(AbsEvent* anEvent)
{
  HepAList<EventInfo>* infoList = Ifd< HepAList< EventInfo > >::get(anEvent, _eventInfoList.value());
  if(infoList==0) ErrMsg(fatal) << "eventInfoList not in event" << endmsg;
  EventInfo* eventInfo = infoList->first();
  if(eventInfo==0) ErrMsg(fatal) << "eventInfo not in event" << endmsg;
  HepLorentzVector CMp4 = eventInfo->cmFrame() ;
  return CMp4 ;
}

BbrLorentzVectorErr DRecoilToolsUser::Upsp4(AbsEvent* anEvent)
{
  HepAList<EventInfo>* infoList = Ifd< HepAList< EventInfo > >::get(anEvent, _eventInfoList.value());
  if(infoList==0) ErrMsg(fatal) << "eventInfoList not in event" << endmsg;
  EventInfo* eventInfo = infoList->first();
  if(eventInfo==0) ErrMsg(fatal) << "eventInfo not in event" << endmsg;
  BbrLorentzVectorErr Upsp4 = eventInfo->electronBeam().fitParams().p4WErr() + eventInfo->positronBeam().fitParams().p4WErr() ;
  return Upsp4 ;
}

void DRecoilToolsUser::show_p4(BtaCandidate* cand)
{
  HepLorentzVector p4 = cand->p4() ;
  show_p4(p4) ;
}

void DRecoilToolsUser::show_p4(HepLorentzVector p4)
{
  cout << "m, p3, p4: " ;
  cout.width(8) ;
  cout <<p4.m() ;
  cout << " , " ;
  cout.width(8) ;
  cout << p4.rho() ;
  cout << " (" ;
  cout.width(8) ;
  cout << p4.e() ;
  cout << " ; " ;
  cout.width(8) ;
  cout << p4.px() ;
  cout << " , " ;
  cout.width(8) ;
  cout << p4.py() ;
  cout << " , " ;
  cout.width(8) ;
  cout << p4.pz() ;
  cout << ")" ;
  cout << endl ;
}
