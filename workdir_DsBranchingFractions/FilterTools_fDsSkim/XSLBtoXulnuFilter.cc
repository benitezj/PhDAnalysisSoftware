//-----------------------------------------------------------------------------------//
//                                                                                   //
//   Filter module for the B -> pi/pi0/eta/eta'/rho/rho0/omega/gamma l nu analyses   //
//                                                                                   //
//     $Id: XSLBtoXulnuFilter.cc,v 1.32 2007/12/19 22:04:28 marchior Exp $
//
//     Sylvie Brunet  2003 Universite de Montreal                                    //
//     David Cote     2003 Universite de Montreal                                    //
//                                                                                   //
//   Documentation: BAD 740                                                          //
//                                                                                   //
//-----------------------------------------------------------------------------------//
#include "BaBar/BaBar.hh"


#include "FilterTools/XSLBtoXulnuFilter.hh"

///////////////
#include "CLHEP/Alist/AList.h"
#include "CLHEP/Alist/AIterator.h"
#include "ProxyDict/IfdHepAListProxy.hh"
#include "AbsEnv/AbsEnv.hh"
#include "AbsEvent/AbsEvent.hh"
#include "ErrLogger/ErrLog.hh"
#include "AbsEventTag/AbsEventTag.hh"
#include "BetaCoreTools/BtaOpMakeTree.hh"
#include "Beta/EventInfo.hh"
#include "BetaCoreTools/BtaBooster.hh"
#include "PDT/Pdt.hh"
#include "BetaCoreTools/BtaThrust.hh"

#include <iomanip>

using std::cout;
using std::endl;
using std::ostream;
using std::setprecision;


XSLBtoXulnuFilter::XSLBtoXulnuFilter( const char* const theName, 
				      const char* const theDescription )
  : TagFilterModule( theName, theDescription )

  //I/O lists
  , _eventInfoList("eventInfoList", this, "Default" )
  , _InputLeptonList("inputLeptonList",this,"eOrMuBremAndGTL") 
  , _InputPiList("inputPiList",this,"piLHLooseGTL")
  , _InputPi0List("inputPi0List",this,"pi0AllVeryLoose") 
  , _InputEtaList("inputEtaList",this,"etaAllSpecial3Pi")
  , _InputEtapList("inputEtapList",this,"etaPeppPID")  
  , _InputRho0List("inputRho0List",this,"rho0VeryLooseMassAndPid")
  , _InputRhoCList("inputRhoCList",this,"rhoCVeryLooseMassAndPid")  
  , _InputOmegaList("inputOmegaList",this,"omegaVeryLooseAndPid")
  , _InputGammaList("inputGammaList",this,"CalorNeutral")
  , _OutputYList("outputYList",this,"XSLBtoXulnuSkimmedYlist") 

  , _eventData("eventData",this,"XSLBtoXulnuEventData")
  , _mB0(5.2794), _mBp(5.2791) //this value is modified in event...
  , _refBeamEnergy("refBeamEnergy",this,5.2895)//mean run1-4 CM beam energy
  // see: http://babar-hn.slac.stanford.edu:5090/HyperNews/get/semi_lept_decays/177/2/1.html
  
  //UsrEventBlock
  , _decayMode("decayMode")
  
  //event parameters
  , _r2allMAX("r2allMAX",this,0.5)//no cut:>=1
  , _ishMIN("ishMIN",this,1)//no cut:0, cut:1
  , _isnhMIN("isnhMIN",this,1)//no cut:0, cut:1
  , _trigMIN("trigMIN",this,0)//no cut:0, cut:1
  //The one lepton tight requirement in the evt is fulfilled automatically by the taking tight electron and muon lists

  //list building parameters
  , _pLABmin_electron("pLABmin_electron",this,0.5)//no cut:0
  , _pLABmin_muon("pLABmin_muon",this,1)//no cut:0
  , _thetaLABmin_electron("thetaLABmin_electron",this,0.41)//no cut:0
  , _thetaLABmin_muon("thetaLABmin_muon",this,0.41)//no cut:0
  , _thetaLABmax_electron("thetaLABmax_electron",this, 2.54)//no cut: 3.14
  , _thetaLABmax_muon("thetaLABmax_muon",this, 2.54)//no cut: 3.14
  , _pLABmax_pi0("pLABmax_pi0",this,10)//no cut:100
  , _pLABmax_eta("pLABmax_eta",this,10)//no cut:100
  , _pLABmax_etap("pLABmax_etap",this,10)//no cut:100
  , _pLABmax_rhoC("pLABmax_rhoC",this,10)//no cut:100
  , _pLABmax_rho0("pLABmax_rho0",this,10)//no cut:100
  , _pLABmax_omega("pLABmax_omega",this,10)//no cut:100
  , _pLABmax_gamma("pLABmax_gamma",this,10)//no cut:100
  , _massmin_rhoC("massmin_rhoC",this,0.525)//no cut:0.3
  , _massmax_rhoC("massmax_rhoC",this,1.025)//no cut:1.3
  , _massmin_rho0("massmin_rho0",this,0.525)//no cut:0.3
  , _massmax_rho0("massmax_rho0",this,1.025)//no cut:1.3
  
  //Y selection
  //The cosBY cut is the same for all hadrons so we don't duplicate parameters
  //The cosBY cut is also the same for all leptons, since we're using BremRecovered electrons, so we don't duplicate parameters
  , _cosBYmin("cosBYmin",this,-1.5)//no cut:-5000
  , _cosBYmax("cosBYmax",this,1.2)//no cut:+1000
  , _cosBYLNGmin("cosBYLNGmin",this,-1.5)//no cut:-5000
  , _cosBYLNGmax("cosBYLNGmax",this,1.3)//no cut:+1000
  , _pStar2Dslope_pi("pStar2Dslope_pi",this,1)
  , _pStar2Dslope_pi0("pStar2Dslope_pi0",this,1)
  , _pStar2Dslope_eta("pStar2Dslope_eta",this,1)
  , _pStar2Dslope_etap("pStar2Dslope_etap",this,0.69)
  , _pStar2Dslope_rhoC("pStar2Dslope_rhoC",this,1)
  , _pStar2Dslope_rho0("pStar2Dslope_rho0",this,1)
  , _pStar2Dslope_omega("pStar2Dslope_omega",this,1)
  , _pStar2Dsum_eta("pStar2Dsum_eta",this,2.8) //no cut: 0
  , _pStar2Dsum_pi0("pStar2Dsum_pi0",this,2.8)//no cut: 0
  , _pStar2Dsum_pi("pStar2Dsum_pi",this,2.8)//no cut: 0
  , _pStar2Dsum_etap("pStar2Dsum_etap",this,2.4)//no cut: 0
  , _pStar2Dsum_rhoC("pStar2Dsum_rhoC",this,2.65)//no cut: 0
  , _pStar2Dsum_rho0("pStar2Dsum_rho0",this,2.65)//no cut: 0
  , _pStar2Dsum_omega("pStar2Dsum_omega",this,2.65)//no cut: 0
  , _pXuUpsMin_pi("pXuUpsMin_pi",this,1.3)//no cut:0
  , _pXuUpsMin_pi0("pXuUpsMin_pi0",this,1.3)//no cut:0
  , _pXuUpsMin_eta("pXuUpsMin_eta",this,1.3)//no cut:0
  , _pXuUpsMin_etap("pXuUpsMin_etap",this,1.65)//no cut:0
  , _pXuUpsMin_rho0("pXuUpsMin_rho0",this,1.3)//no cut:0
  , _pXuUpsMin_rhoC("pXuUpsMin_rhoC",this,1.3)//no cut:0
  , _pXuUpsMin_omega("pXuUpsMin_omega",this,1.3)//no cut:0
  , _pLepUpsMin_pi("pLepUpsMin_pi",this,2.2)//no cut:0
  , _pLepUpsMin_pi0("pLepUpsMin_pi0",this,2.2)//no cut:0
  , _pLepUpsMin_eta("pLepUpsMin_eta",this,2.1)//no cut:0
  , _pLepUpsMin_etap("pLepUpsMin_etap",this,2)//no cut:0
  , _pLepUpsMin_rho0("pLepUpsMin_rho0",this,2)//no cut:0
  , _pLepUpsMin_rhoC("pLepUpsMin_rhoC",this,2)//no cut:0
  , _pLepUpsMin_omega("pLepUpsMin_omega",this,2)//no cut:0
  , _nTrkMin("nTrkMin",this,4)//no cut:0
  , _cosTTMax("cosTTMax",this,0.9)//no cut:1000

  //modes to be studied
  , _analyzeChargedPi("AnalyzeChargedPi",this,true)
  , _analyzeNeutralPi("AnalyzeNeutralPi",this,true)
  , _analyzeEta("AnalyzeEta",this,true)
  , _analyzeEtap("AnalyzeEtap",this,true)
  , _analyzeChargedRho("AnalyzeChargedRho",this,true)
  , _analyzeNeutralRho("AnalyzeNeutralRho",this,true)
  , _analyzeOmega("AnalyzeOmega",this,true)
  , _analyzeGamma("AnalyzeGamma",this,true)

  //bool
  , _MyVerbose("MyVerbose",this,false)
  , _doPionPidYourself("doPionPidYourself",this,false)
{
  //UsrEventBlock
  commands()->append( &_eventData );
  _eventBlock.addVariable( _decayMode );

  //I/O lists
  commands()->append( & _eventInfoList);
  commands()->append( & _InputLeptonList);
  commands()->append( & _InputPiList);
  commands()->append( & _InputPi0List);
  commands()->append( & _InputEtaList);
  commands()->append( & _InputEtapList);
  commands()->append( & _InputRho0List);
  commands()->append( & _InputRhoCList);
  commands()->append( & _InputOmegaList);
  commands()->append( & _InputGammaList);
  commands()->append( & _OutputYList);

  commands()->append( & _refBeamEnergy );

  //evt cuts
  commands()->append( & _r2allMAX );
  commands()->append( & _ishMIN );
  commands()->append( & _isnhMIN );
  commands()->append( & _trigMIN );

  //trk building cuts
  commands()->append( & _pLABmin_electron );
  commands()->append( & _pLABmin_muon );
  commands()->append( & _thetaLABmin_electron );
  commands()->append( & _thetaLABmin_muon );
  commands()->append( & _thetaLABmax_electron );
  commands()->append( & _thetaLABmax_muon );
  commands()->append( & _pLABmax_pi0 );
  commands()->append( & _pLABmax_eta );
  commands()->append( & _pLABmax_etap );
  commands()->append( & _pLABmax_rhoC );
  commands()->append( & _pLABmax_rho0 );
  commands()->append( & _pLABmax_omega );
  commands()->append( & _pLABmax_gamma );
  commands()->append( & _massmin_rhoC );
  commands()->append( & _massmax_rhoC );
  commands()->append( & _massmin_rho0 );
  commands()->append( & _massmax_rho0 );

  //Y selection
  commands()->append( & _cosBYmin );
  commands()->append( & _cosBYmax );
  commands()->append( & _cosBYLNGmin );
  commands()->append( & _cosBYLNGmax );

  commands()->append( & _pStar2Dslope_pi );
  commands()->append( & _pStar2Dslope_pi0 );
  commands()->append( & _pStar2Dslope_eta );
  commands()->append( & _pStar2Dslope_etap );
  commands()->append( & _pStar2Dslope_rhoC );
  commands()->append( & _pStar2Dslope_rho0 );
  commands()->append( & _pStar2Dslope_omega );
  commands()->append( & _pStar2Dsum_eta );
  commands()->append( & _pStar2Dsum_pi0 );
  commands()->append( & _pStar2Dsum_pi );
  commands()->append( & _pStar2Dsum_etap );
  commands()->append( & _pStar2Dsum_rhoC );
  commands()->append( & _pStar2Dsum_rho0 );
  commands()->append( & _pStar2Dsum_omega );
  commands()->append( & _pXuUpsMin_pi );
  commands()->append( & _pXuUpsMin_pi0 );
  commands()->append( & _pXuUpsMin_eta );
  commands()->append( & _pXuUpsMin_etap );
  commands()->append( & _pXuUpsMin_rho0 );
  commands()->append( & _pXuUpsMin_rhoC );
  commands()->append( & _pXuUpsMin_omega );
  commands()->append( & _pLepUpsMin_pi );
  commands()->append( & _pLepUpsMin_pi0 );
  commands()->append( & _pLepUpsMin_eta );
  commands()->append( & _pLepUpsMin_etap );
  commands()->append( & _pLepUpsMin_rho0 );
  commands()->append( & _pLepUpsMin_rhoC );
  commands()->append( & _pLepUpsMin_omega );

  commands()->append( & _nTrkMin );
  commands()->append( & _cosTTMax );

  //bool
  commands()->append( & _analyzeChargedPi );
  commands()->append( & _analyzeNeutralPi );
  commands()->append( & _analyzeEta );
  commands()->append( & _analyzeEtap );
  commands()->append( & _analyzeChargedRho );
  commands()->append( & _analyzeNeutralRho );
  commands()->append( & _analyzeOmega );
  commands()->append( & _analyzeGamma );
  commands()->append( & _MyVerbose );
  commands()->append( & _doPionPidYourself );
}

XSLBtoXulnuFilter::~XSLBtoXulnuFilter()
{
}


AppResult
XSLBtoXulnuFilter::beginJob( AbsEvent* anEvent )
{
  ErrMsg(routine)<<"begin filter Job"<<endmsg; 

  _TotalNbEvents=0;
  _NbAfterTrigger=0;
  _NbAfterISHCuts=0;
  _NbAfterR2AllCuts=0;
  _NbAfterOneLeptonTight=0;
  _NbAfterEventCuts=0;
  _NbAllModesAfterCoupleCuts=0;
  _NbPiAfterCoupleCuts=0;
  _NbPiAfterLeptonSelCuts=0;
  _NbPiAfterTrkNb=0;
  _NbPiAfterCosBY=0;
  _NbPiAfterpStar2D=0;
  _NbPiAfterCosTT=0;
  _NbPi0AfterCoupleCuts=0;
  _NbPi0AfterLeptonSelCuts=0;
  _NbPi0AfterTrkNb=0;
  _NbPi0AfterCosBY=0;
  _NbPi0AfterpStar2D=0;
  _NbPi0AfterCosTT=0;
  _NbEtaAfterCoupleCuts=0;
  _NbEtaAfterLeptonSelCuts=0;
  _NbEtaAfterCosBY=0;
  _NbEtaAfterpStar2D=0;
  _NbEtaAfterCosTT=0;
  _NbEtapAfterCoupleCuts=0;
  _NbEtapAfterLeptonSelCuts=0;
  _NbEtapAfterCosBY=0;
  _NbEtapAfterpStar2D=0;
  _NbEtapAfterCosTT=0;
  _NbRhoCAfterCoupleCuts=0;
  _NbRhoCAfterLeptonSelCuts=0;
  _NbRhoCAfterCosBY=0;
  _NbRhoCAfterpStar2D=0;
  _NbRhoCAfterCosTT=0;
  _NbRho0AfterCoupleCuts=0;
  _NbRho0AfterLeptonSelCuts=0;
  _NbRho0AfterCosBY=0;
  _NbRho0AfterpStar2D=0;
  _NbRho0AfterCosTT=0;
  _NbOmeAfterCoupleCuts=0;
  _NbOmeAfterLeptonSelCuts=0;
  _NbOmeAfterCosBY=0;
  _NbOmeAfterpStar2D=0;
  _NbOmeAfterCosTT=0;
  _NbGammaAfterCoupleCuts=0;
  _NbGammaAfterLeptonSelCuts=0;
  _NbGammaAfterCosBY=0;
  _NbGammaAfterCosTT=0;
  _NbCoupleAllModesAfterAllCuts=0;
  _NbCouplePiAfterAllCuts=0;
  _NbCouplePi0AfterAllCuts=0;
  _NbCoupleEtaAfterAllCuts=0;
  _NbCoupleRhoCAfterAllCuts=0;
  _NbCoupleRho0AfterAllCuts=0;
  _NbCoupleOmegaAfterAllCuts=0;
  _NbCoupleGammaAfterAllCuts=0;


  return AppResult::OK;
}


AppResult
XSLBtoXulnuFilter::event( AbsEvent* anEvent )
{
  // Build the tag accessor using the base class
  TagFilterModule::event( anEvent );

  _TotalNbEvents+=1;

  //To set type of composite BtaCandidate
  static const PdtEntry* PdtB0 = Pdt::lookup(PdtLund::B0);
  static const PdtEntry* PdtAntiB0 = Pdt::lookup(PdtLund::anti_B0);
  static const PdtEntry* PdtBplus = Pdt::lookup(PdtLund::B_plus);
  static const PdtEntry* PdtBminus = Pdt::lookup(PdtLund::B_minus);
  _mB0=PdtB0->mass();
  _mBp=PdtBplus->mass();


  //Global event Cuts
  //The _CMS booster object is not created at this point so we cannot use QuitEvent()!
  //if (SurvivedTrigger( anEvent ))  _NbAfterTrigger+=1; 
  //else { setPassed(false); return AppResult::OK; } 
  if (SurvivedISH( anEvent ))  _NbAfterISHCuts+=1; 
  else { setPassed(false); return AppResult::OK; } 
  if(SurvivedR2All( anEvent ))  _NbAfterR2AllCuts+=1; 
  else { setPassed(false); return AppResult::OK; }


  // 4-Vector P(e+)+P(e-) CM Upsilon(4S)
  HepAList< EventInfo >* infoList = Ifd<HepAList< EventInfo > >::get(anEvent, _eventInfoList.value());
  if (infoList == 0){ ErrMsg(fatal) << "Could not locate event info list in XSLBtoXulnuFilter" << endmsg; }
  _eventInfo = infoList->first();  

  HepLorentzVector UpsP4 = _eventInfo->cmFrame(); //always take the real beam energy
  double eBeamCM = UpsP4.mag()/2.0; 
  
  //scaling factor for cosBY with OffPeak data
  //The adopted strategy is the following: 
  //  -a scale factor f=E_on/E_off is computed for OffPeak data only
  //  -this scale factor is used for cosBY and the p* 2D cut only
  //  -for cosBY, we correct Ebeam, mY and pY (thus EY). This is rigourously equivalent to correct mB by E_off/E_on and
  //   has the effect that cosBY gives the same numerical value for an Y with eBeam=E_on and an Y*E_on/E_off and eBeam=E_off.
  //  -for the p* 2D cut, it is unclear if this really the correct correction to do, however:
  //                *it is coherent with the cosBY correction
  //                *the correction is safe because it can never reject events that would be accepted without corrections.  
  double f=1.0;
  if( eBeamCM<_mBp ) {
    f=_refBeamEnergy.value()/eBeamCM;
  }


  //Create the Ups(4S) frame booster
  _CMS = new BtaBooster( UpsP4 );

  //Making lists
  MakeHadronsAndLeptonsList( anEvent );

  if(NewLepList.length()!=0) 
    { 
      _NbAfterEventCuts+=1;  
      _NbAfterOneLeptonTight+=1;    
    }
  else { setPassed(false); return QuitEvent(); }


  //TrkNb
  static const IfdStrKey ChgTrkKey("ChargedTracks");
  _BasicTrkList = Ifd<HepAList<BtaCandidate> >::get(anEvent,ChgTrkKey);
  if (_BasicTrkList == 0)  ErrMsg(fatal) << "XSLBtoXulnuFilter could not locate list ChargedTracks" << endmsg;
  int nTrk = _BasicTrkList->length();

  //Bumps
  static const IfdStrKey BumpKey("CalorNeutral");
  _BasicBumpList = Ifd<HepAList<BtaCandidate> >::get(anEvent,BumpKey);
  if (_BasicBumpList == 0)  ErrMsg(fatal) << "XSLBtoXulnuFilter could not locate list CalorNeutral" << endmsg;


  //////////////////////////////////////
  //  We now build the YList made out of all Y couples passing their mode-specific criteria
  /////////////////////////////////////
  BtaCandidate* lepton;
  BtaCandidate* hadron;

  HepAListIterator<BtaCandidate> iter_lep(NewLepList);  
  HepAListIterator<BtaCandidate> iter_pi(NewPiList);  
  HepAListIterator<BtaCandidate> iter_pi0(NewPi0List);  
  HepAListIterator<BtaCandidate> iter_eta(NewEtaList);  
  HepAListIterator<BtaCandidate> iter_etap(NewEtapList);  
  HepAListIterator<BtaCandidate> iter_rho0(NewRho0List);  
  HepAListIterator<BtaCandidate> iter_rhoC(NewRhoCList);  
  HepAListIterator<BtaCandidate> iter_ome(NewOmegaList);  
  HepAListIterator<BtaCandidate> iter_gam(NewGammaList);  

  //Create the output list
  //At the end, the length of the list will tell the module to either reject the event or accept the event and put the outYList in it
  HepAList< BtaCandidate >* outYList = new HepAList< BtaCandidate >;


  int piLevel=0,pi0Level=0,etaLevel=0,etapLevel=0,rhoCLevel=0,rho0Level=0,omeLevel=0,gamLevel=0;
  int piLevelWrongCharge=0,rhoCLevelWrongCharge=0;

  while ( 0 != ( lepton = iter_lep() ) ) {
   
    ///////////////////////////////////////////////////////////////////////    
    if( _analyzeChargedPi.value()==true ) {
      iter_pi.rewind();

      while ( 0 != ( hadron = iter_pi() ) ) {	
	
	//if( lepton->charge()!=hadron->charge() && !(hadron->overlaps(*lepton)) ) 
	if( !(hadron->overlaps(*lepton)) ) 
	  { 
	    if( piLevel==0 && lepton->charge()!=hadron->charge() )
	      { piLevel=1;  _NbPiAfterLeptonSelCuts+=1; }
	    else if( piLevelWrongCharge==0 && lepton->charge()==hadron->charge() )
	      { piLevelWrongCharge=1;  _NbPiAfterLeptonSelCuts+=1; }
	    
	    if(nTrk>=_nTrkMin.value()) 
	      {
		if( piLevel==1 && lepton->charge()!=hadron->charge() ) 
		  { piLevel=2;  _NbPiAfterTrkNb+=1; }
		else if( piLevelWrongCharge==1 && lepton->charge()==hadron->charge() ) 
		  { piLevelWrongCharge=2;  _NbPiAfterTrkNb+=1; }
		
		BtaCandidate boostedLep = _CMS->boostTo( *lepton );
		BtaCandidate boostedHad = _CMS->boostTo( *hadron );

		HepLorentzVector boostedP4Y = boostedLep.p4() + boostedHad.p4();
		HepLorentzVector P4Y = lepton->p4() + hadron->p4();
		
		double cBY = cosBY(eBeamCM*f,_mB0,P4Y.mag()*f,boostedP4Y.rho()*f);

		if( cBY>=_cosBYmin.value() &&  cBY<=_cosBYmax.value() ) 
		  {
		    if( piLevel==2 && lepton->charge()!=hadron->charge() )
		      { piLevel=3;  _NbPiAfterCosBY+=1; }
		    else if( piLevelWrongCharge==2 && lepton->charge()==hadron->charge() ) 
		      { piLevelWrongCharge=3;  _NbPiAfterCosBY+=1; }
		    
		    double pLep = f*boostedLep.p();
		    double pXu = f*boostedHad.p();
		    double sum = pLep + _pStar2Dslope_pi.value()*pXu;
		    
		    if( sum >= _pStar2Dsum_pi.value() || pXu>= _pXuUpsMin_pi.value() || pLep>= _pLepUpsMin_pi.value() )		
		      {
			if( piLevel==3 && lepton->charge()!=hadron->charge() ) 
			  { piLevel=4;  _NbPiAfterpStar2D+=1; }
			else if( piLevelWrongCharge==3 && lepton->charge()==hadron->charge() )
			  { piLevelWrongCharge=4;  _NbPiAfterpStar2D+=1; }
			
			BtaOpMakeTree comb;
			BtaCandidate* Y = comb.create(*hadron,*lepton);
			if(lepton->charge()==1) Y->setType(PdtB0);
			else if(lepton->charge()==-1) Y->setType(PdtAntiB0);
			else cout<<"Houston, we have a problem with the pi mode!"<<endl;
			
			double cTT = cosTT(Y);
			if( cTT< _cosTTMax.value() ) 
			  {
			    if( piLevel==4 && lepton->charge()!=hadron->charge() ) 
			      { piLevel=5;  _NbPiAfterCosTT+=1;  _NbPiAfterCoupleCuts+=1; }
			    else if( piLevelWrongCharge==4 && lepton->charge()==hadron->charge() ) 
			      { piLevelWrongCharge=5; _NbPiAfterCosTT+=1;  _NbPiAfterCoupleCuts+=1; }
			    
			    outYList->append( new BtaCandidate(*Y) );
			    _NbCouplePiAfterAllCuts+=1;
			  }//cosTT
			
			delete Y;
			
		      }//pStar2D
		  }//cosBY
	      }//trkNb
	  }//no overlap
       }//while ( 0 != ( hadron = iter_pi() ) )
    }//if(_chargedPi)
    

    ///////////////////////////////////////////////////////////////////////
    if( _analyzeNeutralPi.value()==true ) {
      iter_pi0.rewind();

      while ( 0 != ( hadron = iter_pi0() ) ) {
		
	// Check for overlap due to brem recovery
	if( !(hadron->overlaps(*lepton)) ) 
	  {
	    if( pi0Level==0 ) { pi0Level=1;  _NbPi0AfterLeptonSelCuts+=1; }
	    
	    if(nTrk>=_nTrkMin.value()) 
	      {
		if( pi0Level==1 ) { pi0Level=2;  _NbPi0AfterTrkNb+=1; }

		BtaCandidate boostedLep = _CMS->boostTo( *lepton );
		BtaCandidate boostedHad = _CMS->boostTo( *hadron );
		
		HepLorentzVector boostedP4Y = boostedLep.p4() + boostedHad.p4();
		HepLorentzVector P4Y = lepton->p4() + hadron->p4();
				
		double cBY = cosBY(eBeamCM*f,_mBp,P4Y.mag()*f,boostedP4Y.rho()*f);
		
		if( cBY>=_cosBYmin.value() &&  cBY<=_cosBYmax.value() ) 
		  {
		    if( pi0Level==2 ) { pi0Level=3;  _NbPi0AfterCosBY+=1; }
		    
		    double pLep = f*boostedLep.p();
		    double pXu = f*boostedHad.p();
		    double sum = pLep + _pStar2Dslope_pi0.value()*pXu;
		    
		    if( sum >= _pStar2Dsum_pi0.value() || pXu>= _pXuUpsMin_pi0.value() || pLep>= _pLepUpsMin_pi0.value() )		
		      {
			if( pi0Level==3 ) { pi0Level=4;  _NbPi0AfterpStar2D+=1; }
			
			BtaOpMakeTree comb;
			BtaCandidate* Y = comb.create(*hadron,*lepton);
			if(lepton->charge()==1) Y->setType(PdtBplus);
			else if(lepton->charge()==-1) Y->setType(PdtBminus);
			else cout<<"Houston, we have a problem with the pi0 mode!"<<endl;
			
			double cTT = cosTT(Y);
			if( cTT< _cosTTMax.value() ) 
			  {
			    if( pi0Level==4 ) { pi0Level=5;  _NbPi0AfterCosTT+=1;  _NbPi0AfterCoupleCuts+=1; }
			    outYList->append( new BtaCandidate(*Y) );
			    _NbCouplePi0AfterAllCuts+=1;
			  }//cosTT
			
			delete Y;
			
		      }//pStar2D
		  }//cosBY
	      }//nTrk
	  }//lepton selection
      }//while ( 0 != ( hadron = iter_pi0() ) )
    }//if(_analyzeNeutralPi)
    
    ///////////////////////////////////////////////////////////////////////////////
    if( _analyzeEta.value()==true ) {
      iter_eta.rewind();

      while ( 0 != ( hadron = iter_eta() ) ) {	
	
	//We don't want Eta's daughters to be the lepton!
	if( !(hadron->overlaps(*lepton)) )

	  {
	    if( etaLevel==0 ) { etaLevel=1;   _NbEtaAfterLeptonSelCuts+=1; }
	    
	    BtaCandidate boostedLep = _CMS->boostTo( *lepton );
	    BtaCandidate boostedHad = _CMS->boostTo( *hadron );
	    
	    HepLorentzVector boostedP4Y = boostedLep.p4() + boostedHad.p4();
	    HepLorentzVector P4Y = lepton->p4() + hadron->p4();
	    
	    double cBY = cosBY(eBeamCM*f,_mBp,P4Y.mag()*f,boostedP4Y.rho()*f);
	    
	    if( cBY>=_cosBYmin.value() &&  cBY<=_cosBYmax.value() ) 
	      {
		if( etaLevel==1 ) { etaLevel=2;  _NbEtaAfterCosBY+=1; }

		double pLep = f*boostedLep.p();
		double pXu = f*boostedHad.p();
		double sum = pLep + _pStar2Dslope_eta.value()*pXu;

		if( sum >= _pStar2Dsum_eta.value() || pXu>= _pXuUpsMin_eta.value() || pLep>= _pLepUpsMin_eta.value() )		
		  {
		    if( etaLevel==2 ) { etaLevel=3;  _NbEtaAfterpStar2D+=1; }
		    
			BtaOpMakeTree comb;
			BtaCandidate* Y = comb.create(*hadron,*lepton);
			if(lepton->charge()==1) Y->setType(PdtBplus);
			else if(lepton->charge()==-1) Y->setType(PdtBminus);
			else cout<<"Houston, we have a problem with the eta mode!"<<endl;
			
			double cTT = cosTT(Y);
			if( cTT< _cosTTMax.value() ) 
			  {
			    if( etaLevel==3 ) { etaLevel=4;  _NbEtaAfterCosTT+=1;  _NbEtaAfterCoupleCuts+=1; }
			    outYList->append( new BtaCandidate(*Y) );
			    _NbCoupleEtaAfterAllCuts+=1;
			  }//cosTT
			
			delete Y;

		  }//pStar2D
	      }//cosBY
	  }//if HadNotMixedToLepton
      }//while ( 0 != ( hadron = iter_eta() ) )
    }//if(_analyzeEta)


    ///////////////////////////////////////////////////////////////////////////////
    if( _analyzeEtap.value()==true ) {
      iter_etap.rewind();

      while ( 0 != ( hadron = iter_etap() ) ) {	
	
	//We don't want Etap's daughters to be the lepton!
	if( !(hadron->overlaps(*lepton)) )
	  {
	    if( etapLevel==0 ) { etapLevel=1;   _NbEtapAfterLeptonSelCuts+=1; }
	    
	    BtaCandidate boostedLep = _CMS->boostTo( *lepton );
	    BtaCandidate boostedHad = _CMS->boostTo( *hadron );
	    
	    HepLorentzVector boostedP4Y = boostedLep.p4() + boostedHad.p4();
	    HepLorentzVector P4Y = lepton->p4() + hadron->p4();
	    	    
	    double cBY = cosBY(eBeamCM*f,_mBp,P4Y.mag()*f,boostedP4Y.rho()*f);

	    if( cBY>=_cosBYmin.value() &&  cBY<=_cosBYmax.value() ) 
	      {
		if( etapLevel==1 ) { etapLevel=2;  _NbEtapAfterCosBY+=1; }

		double pLep = f*boostedLep.p();
		double pXu = f*boostedHad.p();
		double sum = pLep + _pStar2Dslope_etap.value()*pXu;

		if( sum >= _pStar2Dsum_etap.value() || pXu>= _pXuUpsMin_etap.value() || pLep>= _pLepUpsMin_etap.value() )		
		  {
		    if( etapLevel==2 ) { etapLevel=3;  _NbEtapAfterpStar2D+=1; }
		    
		    BtaOpMakeTree comb;
		    BtaCandidate* Y = comb.create(*hadron,*lepton);
		    if(lepton->charge()==1) Y->setType(PdtBplus);
		    else if(lepton->charge()==-1) Y->setType(PdtBminus);
		    else cout<<"Houston, we have a problem with the etap mode!"<<endl;
		    
		    double cTT = cosTT(Y);
		    if( cTT< _cosTTMax.value() ) 
		      {
			if( etapLevel==3 ) { etapLevel=4;  _NbEtapAfterCosTT+=1;  _NbEtapAfterCoupleCuts+=1; }
			
			outYList->append( new BtaCandidate(*Y) );
			_NbCoupleEtapAfterAllCuts+=1;
			
		      }//cosTT
		    
		    delete Y;
		    
		  }//pStar2D
	      }//cosBY
	  }//if HadNotMixedToLepton	
      }//while ( 0 != ( hadron = iter_etap() ) )
    }//if(_analyzeEtap)


    ///////////////////////////////////////////////////////////////////////////////
    if( _analyzeChargedRho.value()==true ) {
      iter_rhoC.rewind();

      while ( 0 != ( hadron = iter_rhoC() ) ) {	
	
	//We don't want Rho's daughters to be the lepton!
	//if( lepton->charge()!=hadron->charge() && !(hadron->overlaps(*lepton)) )
	if( !(hadron->overlaps(*lepton)) )
	  {
	    if( rhoCLevel==0 && lepton->charge()!=hadron->charge() )
	      { rhoCLevel=1;  _NbRhoCAfterLeptonSelCuts+=1; }
	    else if( rhoCLevelWrongCharge==0 && lepton->charge()==hadron->charge() ) 
	      { rhoCLevelWrongCharge=1;  _NbRhoCAfterLeptonSelCuts+=1; }
	    BtaCandidate boostedLep = _CMS->boostTo( *lepton );
	    BtaCandidate boostedHad = _CMS->boostTo( *hadron );

	    HepLorentzVector boostedP4Y = boostedLep.p4() + boostedHad.p4();
	    HepLorentzVector P4Y = lepton->p4() + hadron->p4();

	    double cBY = cosBY(eBeamCM*f,_mB0,P4Y.mag()*f,boostedP4Y.rho()*f);

	    if( cBY>=_cosBYmin.value() &&  cBY<=_cosBYmax.value() ) 
	      {
		if( rhoCLevel==1 && lepton->charge()!=hadron->charge() ) 
		  { rhoCLevel=2;  _NbRhoCAfterCosBY+=1; }
		else if( rhoCLevelWrongCharge==1 && lepton->charge()==hadron->charge() ) 
		  { rhoCLevelWrongCharge=2;  _NbRhoCAfterCosBY+=1; }

		double pLep = f*boostedLep.p();
		double pXu = f*boostedHad.p();
		double sum = pLep + _pStar2Dslope_rhoC.value()*pXu;

		if( sum >= _pStar2Dsum_rhoC.value() || pXu>= _pXuUpsMin_rhoC.value() || pLep>= _pLepUpsMin_rhoC.value() )		
		  {
		    if( rhoCLevel==2 && lepton->charge()!=hadron->charge() ) 
		      { rhoCLevel=3;  _NbRhoCAfterpStar2D+=1; }
		    else if( rhoCLevelWrongCharge==2 && lepton->charge()==hadron->charge() )
		      { rhoCLevelWrongCharge=3;  _NbRhoCAfterpStar2D+=1; }
		    
		    BtaOpMakeTree comb;
		    BtaCandidate* Y = comb.create(*hadron,*lepton);
		    if(lepton->charge()==1) Y->setType(PdtB0);
		    else if(lepton->charge()==-1) Y->setType(PdtAntiB0);
		    else cout<<"Houston, we have a problem with the rhoC mode!"<<endl;

		    double cTT = cosTT(Y);
		    if( cTT< _cosTTMax.value() ) 
		      {
			if( rhoCLevel==3 && lepton->charge()!=hadron->charge() )
			  { rhoCLevel=4;  _NbRhoCAfterCosTT+=1;  _NbRhoCAfterCoupleCuts+=1; }
			else if( rhoCLevelWrongCharge==3 && lepton->charge()==hadron->charge() )
			  { rhoCLevelWrongCharge=4;  _NbRhoCAfterCosTT+=1;  _NbRhoCAfterCoupleCuts+=1; }
			
			outYList->append( new BtaCandidate(*Y) );
			_NbCoupleRhoCAfterAllCuts+=1;

		      }//cosTT

		    delete Y;

		  }//pStar2D
	      }//cosBY
	  }//if HadNotMixedToLepton
       }//while ( 0 != ( hadron = iter_rhoC() ) )
    }//if(_analyzeChargedRho)

    ///////////////////////////////////////////////////////////////////////////////
    if( _analyzeNeutralRho.value()==true ) {
      iter_rho0.rewind();

      while ( 0 != ( hadron = iter_rho0() ) ) {	
	
	//We don't want Rho's daughters to be the lepton!
	if( !(hadron->overlaps(*lepton)) )
	  {
	    if( rho0Level==0 ) { rho0Level=1;  _NbRho0AfterLeptonSelCuts+=1; }

	    BtaCandidate boostedLep = _CMS->boostTo( *lepton );
	    BtaCandidate boostedHad = _CMS->boostTo( *hadron );

	    HepLorentzVector boostedP4Y = boostedLep.p4() + boostedHad.p4();
	    HepLorentzVector P4Y = lepton->p4() + hadron->p4();
	    
	    double cBY = cosBY(eBeamCM*f,_mBp,P4Y.mag()*f,boostedP4Y.rho()*f);

	    if( cBY>=_cosBYmin.value() &&  cBY<=_cosBYmax.value() ) 
	      {
		if( rho0Level==1 ) { rho0Level=2;  _NbRho0AfterCosBY+=1; }

		double pLep = f*boostedLep.p();
		double pXu = f*boostedHad.p();
		double sum = pLep + _pStar2Dslope_rho0.value()*pXu;

		if( sum >= _pStar2Dsum_rho0.value() || pXu>= _pXuUpsMin_rho0.value() || pLep>= _pLepUpsMin_rho0.value() )		
		  {
		    if( rho0Level==2 ) { rho0Level=3;  _NbRho0AfterpStar2D+=1; }
		    
		    BtaOpMakeTree comb;
		    BtaCandidate* Y = comb.create(*hadron,*lepton);
		    if(lepton->charge()==1) Y->setType(PdtBplus);
		    else if(lepton->charge()==-1) Y->setType(PdtBminus);
		    else cout<<"Houston, we have a problem with the rho0 mode!"<<endl;

		    double cTT = cosTT(Y);
		    if( cTT< _cosTTMax.value() ) 
		      {
			if( rho0Level==3 ) { rho0Level=4;  _NbRho0AfterCosTT+=1;  _NbRho0AfterCoupleCuts+=1; }
			
			outYList->append( new BtaCandidate(*Y) );
			_NbCoupleRho0AfterAllCuts+=1;
			
		      }//cosTT
		    
		    delete Y;

		  }//pStar2D
	      }//cosBY
	  }//if HadNotMixedToLepton
      }//while ( 0 != ( hadron = iter_rho0() ) )
    }//if(_analyzeNeutralRho)

    ///////////////////////////////////////////////////////////////////////////////
    if( _analyzeOmega.value()==true ) {
      iter_ome.rewind();

      while ( 0 != ( hadron = iter_ome() ) ) {	
	
	//We don't want Omega's daughters to be the lepton!
	if( !(hadron->overlaps(*lepton)) )
	  {
	    if( omeLevel==0 ) { omeLevel=1;  _NbOmeAfterLeptonSelCuts+=1; }

	    BtaCandidate boostedLep = _CMS->boostTo( *lepton );
	    BtaCandidate boostedHad = _CMS->boostTo( *hadron );

	    HepLorentzVector boostedP4Y = boostedLep.p4() + boostedHad.p4();
	    HepLorentzVector P4Y = lepton->p4() + hadron->p4();
	    
	    double cBY = cosBY(eBeamCM*f,_mBp,P4Y.mag()*f,boostedP4Y.rho()*f);

	    if( cBY>=_cosBYmin.value() &&  cBY<=_cosBYmax.value() ) 
	      {
		if( omeLevel==1 ) { omeLevel=2;  _NbOmeAfterCosBY+=1; }

		double pLep = f*boostedLep.p();
		double pXu = f*boostedHad.p();
		double sum = pLep + _pStar2Dslope_omega.value()*pXu;

		if( sum >= _pStar2Dsum_omega.value() || pXu>= _pXuUpsMin_omega.value() || pLep>= _pLepUpsMin_omega.value() )		
		  {
		    if( omeLevel==2 ) { omeLevel=3;  _NbOmeAfterpStar2D+=1; }
		    
		    BtaOpMakeTree comb;
		    BtaCandidate* Y = comb.create(*hadron,*lepton);
		    if(lepton->charge()==1) Y->setType(PdtBplus);
		    else if(lepton->charge()==-1) Y->setType(PdtBminus);
		    else cout<<"Houston, we have a problem with the omega l nu mode!"<<endl;

		    double cTT = cosTT(Y);
		    if( cTT< _cosTTMax.value() ) 
		      {
			if( omeLevel==3 ) { omeLevel=4;  _NbOmeAfterCosTT+=1;  _NbOmeAfterCoupleCuts+=1;}
						    
			outYList->append( new BtaCandidate(*Y) );
			_NbCoupleOmegaAfterAllCuts+=1;
			
		      }//cosTT
		    
		    delete Y;
		    
		  }//pStar2D
	      }//cosBY
	  }//if HadNotMixedToLepton
      }//while ( 0 != ( hadron = iter_ome() ) )
    }//if(_analyzeOmega)

    ///////////////////////////////////////////////////////////////////////////////
    if( _analyzeGamma.value()==true ) {
      iter_gam.rewind();
      
      while ( 0 != ( hadron = iter_gam() ) ) {	
	// Check for overlap due to brem recovery
	if( !(hadron->overlaps(*lepton))) 
	  {
	    if( gamLevel==0 ) { gamLevel=1;  _NbGammaAfterLeptonSelCuts+=1; }
	    
	    BtaCandidate boostedLep = _CMS->boostTo( *lepton );
	    BtaCandidate boostedHad = _CMS->boostTo( *hadron );
	    
	    HepLorentzVector boostedP4Y = boostedLep.p4() + boostedHad.p4();
	    HepLorentzVector P4Y = lepton->p4() + hadron->p4();
	    
	    double cBY = cosBY(eBeamCM*f,_mBp,P4Y.mag()*f,boostedP4Y.rho()*f);
	    
	    if( cBY>=_cosBYLNGmin.value() &&  cBY<=_cosBYLNGmax.value() ) 
	      {
		if( gamLevel==1 ) { gamLevel=2;  _NbGammaAfterCosBY+=1; }
		
		BtaOpMakeTree comb;
		BtaCandidate* Y = comb.create(*hadron,*lepton);
		if(lepton->charge()==1) Y->setType(PdtBplus);
		else if(lepton->charge()==-1) Y->setType(PdtBminus);
		else cout<<"Houston, we have a problem with the gamma l nu mode!"<<endl;
		
		double cTT = cosTT(Y);
		if( cTT< _cosTTMax.value() ) 
		  {
		    if( gamLevel==2 ) { gamLevel=3;  _NbGammaAfterCosTT+=1;  _NbGammaAfterCoupleCuts+=1; }
		    
		    outYList->append( new BtaCandidate(*Y) );
		    _NbCoupleGammaAfterAllCuts+=1;			
		  }//cosTT
		
		delete Y;
		
	      }//cosBY
	  }
      }//while ( 0 != ( hadron = iter_gam() ) )
    }//if(_analyzeGamma)
    
    
  }//while ( 0 != ( lepton = iter_lep() ) )


  ///////////////////
  //  Final result
  ///////////////////
  
  if( outYList->length()!=0 ) { 
    //put the outYList in the event (so it can be persisted)
    IfdHepAListProxy<BtaCandidate>* proxy = new IfdHepAListProxy<BtaCandidate>( outYList );
    if( proxy==0 ){ ErrMsg(fatal) << "proxy==0 in XSLBtoXulnuFilter !!" << endmsg; }
    bool ok = Ifd< HepAList<BtaCandidate> >::put( anEvent, proxy, _OutputYList.value() );
    if( !ok ){ ErrMsg(fatal) << "XSLBtoXulnuFilter: outYList already in the event !!" << endmsg; }

    //tell the skim to accept this event
    setPassed(true);

    _NbAllModesAfterCoupleCuts+=1; 
    _NbCoupleAllModesAfterAllCuts+=outYList->length(); 
  }
  else{
    setPassed(false);
    delete outYList;
  }


  //////////////////////////////////////
  //  user data part (using bit mask):
  //////////////////////////////////////

  bool isPi=false,isPi0=false,isEta=false,isEtap=false,isRhoC=false,isRho0=false,isOmega=false,isGam=false;
  bool isPiWrongCharge=false,isRhoCWrongCharge=false;
  if(piLevel==5) isPi=true;
  if(piLevelWrongCharge==5) isPiWrongCharge=true;
  if(pi0Level==5) isPi0=true;
  if(etaLevel==4) isEta=true;
  if(etapLevel==4) isEtap=true;
  if(rhoCLevel==4) isRhoC=true;
  if(rhoCLevelWrongCharge==4) isRhoCWrongCharge=true;
  if(rho0Level==4) isRho0=true;
  if(omeLevel==4) isOmega=true;
  if(gamLevel==3) isGam=true;

  int i=0;
  if ( isPi )    i = i | 1; 
  if ( isPi0 )   i = i | 2; 
  if ( isEta )   i = i | 4; 
  if ( isEtap )  i = i | 8; 
  if ( isRhoC )  i = i | 16; 
  if ( isRho0 )  i = i | 32; 
  if ( isOmega ) i = i | 64; 
  if ( isGam )   i = i | 128;
  if ( isPiWrongCharge)   i = i | 256;
  if ( isRhoCWrongCharge) i = i | 512;
  _decayMode=i;


  UsrIfd<UsrEventBlock>::put( anEvent, &_eventBlock, _eventData.value() );
  bool ok = true;
  ok &= _eventBlock.put( _decayMode );
  assert( ok );

  return QuitEvent(); 

}


bool
XSLBtoXulnuFilter::SurvivedTrigger(AbsEvent* anEvent)
{
  bool evtOK=false;
  float trigOK=-1;

  AbsEventTag* tag = Ifd<AbsEventTag>::get( anEvent );
  if(tag==0) 
    { 
      cout<<"NOTHING IN AbsEvent!"<<endl;
    }
  else
    {
      bool L3EMC,L3DCH;
      if(tag->getBool(L3EMC,"L3OutEmc")
	 &&tag->getBool(L3DCH,"L3OutDch"))	
	{
	  if(L3EMC||L3DCH) trigOK=1;
	  else trigOK=0;
	}
      else ErrMsg(fatal) << "Could not find L3 Trigger bits!"<<endmsg;
    }
  
  if( trigOK>= _trigMIN.value() ) evtOK=true;
  
  return evtOK;
}

bool
XSLBtoXulnuFilter::SurvivedISH(AbsEvent* anEvent)
{
  bool evtOK=false;
  float ish=-1;
  float isnh=-1;

  AbsEventTag* tag = Ifd<AbsEventTag>::get( anEvent );
  if(tag==0) 
    { 
      cout<<"NOTHING IN AbsEvent!"<<endl;
    }
  else
    {
      bool ismultihad;
      if(tag->getBool(ismultihad,"BGFMultiHadron"))	{
	if(ismultihad) ish=1;
	else ish=0;
      }
      else ErrMsg(fatal) << "Could not find BGFMultiHadron"<<endmsg;

      bool isneutralhad;
      if(tag->getBool(isneutralhad,"BGFNeutralHadron"))	{
	if(isneutralhad) isnh=1;
	else isnh=0;
      }
      else ErrMsg(fatal) << "Could not find BGFNeutralHadron"<<endmsg;
    }
  
  if( ish>= _ishMIN.value() ){
    evtOK=true;
  }
  else if( isnh>= _isnhMIN.value() ){
    evtOK=true;
  }
  return evtOK;
}

bool
XSLBtoXulnuFilter::SurvivedR2All(AbsEvent* anEvent)
{
  bool evtOK=false;
  float r2all=2;

  AbsEventTag* tag = Ifd<AbsEventTag>::get( anEvent );
  if(tag==0) 
    { 
      cout<<"NOTHING IN AbsEvent!"<<endl;
    }
  else
    {
      if(tag->getFloat(r2all,"R2All"));
      else ErrMsg(fatal) << "Could not find R2All bit!"<<endmsg;
    }  
  
  if( r2all<=_r2allMAX.value() ) evtOK=true;

  return evtOK;
}


void
XSLBtoXulnuFilter::MakeHadronsAndLeptonsList(AbsEvent* anEvent)
{
  //We apply here additional kinematic and quality cuts to the various lists
  //Note that the base list are already somewhat customized in various CompositionSequences modules
  BtaCandidate* candid;  

  //tmp...
  if( _doPionPidYourself.value() ){ 
    static const IfdStrKey piLHKey("piLHLoose");
    _piLooseLH = Ifd<HepAList<BtaCandidate> >::get(anEvent,piLHKey);
    if (_piLooseLH == 0)  ErrMsg(fatal) << "XSLBtoXulnuFilter could not locate list piLHLoose" << endmsg;
  }


  ///////////////
  //  LEPTONS  //
  ///////////////

  HepAList< BtaCandidate >* InputLeptonList = Ifd<HepAList<BtaCandidate> >::get(anEvent,_InputLeptonList.value());
  if (InputLeptonList == 0)   ErrMsg(fatal)<<"No leptonList!!"<<endmsg;
  HepAListIterator<BtaCandidate> iter_InputLepton(*InputLeptonList);  

  while ( 0 != ( candid = iter_InputLepton()) ) 
    {
      PdtLund::LundType lepLund = candid->pdtEntry()->lundId();
      bool ePID=false,muPID=false; 
      if(lepLund==PdtLund::e_minus || lepLund==PdtLund::e_plus){ ePID=true; }
      else if(lepLund==PdtLund::mu_minus || lepLund==PdtLund::mu_plus){ muPID=true; }
      else{ ErrMsg(fatal)<<"XSLBtoXulnuFilter: InputLeptonList not containing cand with Lund of a lepton!!!"<<endmsg; }

      bool eCut=false;
      bool muCut=false;
      if(candid->p() >= _pLABmin_electron.value() && ePID ) eCut=true;
      if(candid->p() >= _pLABmin_muon.value() && muPID ) muCut=true;
      
      if( (eCut || muCut)	
	  && candid->p()<10  
	  && candid->p3().theta() >= _thetaLABmin_electron.value()
	  && candid->p3().theta() <= _thetaLABmax_electron.value() )  
	{
	  NewLepList.append( candid );
	}
    }
  

  ///////////////
  //  HADRONS  //
  ///////////////

  //Put Hadron Selection cuts here! 

  if(_analyzeChargedPi.value()==true)
    {
      HepAList< BtaCandidate >* InputPiList = Ifd<HepAList<BtaCandidate> >::get(anEvent,_InputPiList.value());
      if (InputPiList == 0)   ErrMsg(fatal)<<"No piList!!"<<endmsg;
      HepAListIterator<BtaCandidate> iter_InputPi(*InputPiList);  
      while ( 0 != ( candid = iter_InputPi()) ) 
	{
	  bool daughtersOK=false;
	  if( _doPionPidYourself.value()==false) daughtersOK=true;
	  else daughtersOK = pionPID(candid);

	  if( daughtersOK && candid->nDaughters()==0 ) NewPiList.append( candid ); 
	}
    }

  if(_analyzeNeutralPi.value()==true)
    {
      HepAList< BtaCandidate >* InputPi0List = Ifd<HepAList<BtaCandidate> >::get(anEvent,_InputPi0List.value());
      if (InputPi0List == 0)   ErrMsg(fatal)<<"No pi0List!!"<<endmsg;
      HepAListIterator<BtaCandidate> iter_InputPi0(*InputPi0List);  
      while ( 0 != ( candid = iter_InputPi0()) ) 
	{
	  if( candid->p() <= _pLABmax_pi0.value() ) NewPi0List.append( candid );
	}
    }

  if(_analyzeEta.value()==true)
    {
      HepAList< BtaCandidate >* InputEtaList = Ifd<HepAList<BtaCandidate> >::get(anEvent,_InputEtaList.value());
      if (InputEtaList == 0)   ErrMsg(fatal)<<"No etaList!!"<<endmsg;
      HepAListIterator<BtaCandidate> iter_InputEta(*InputEtaList);  
      while ( 0 != ( candid = iter_InputEta()) ) 
	{
	  bool daughtersOK=false;
	  if(candid->nDaughters()==2 || _doPionPidYourself.value()==false ) daughtersOK=true; //eta-> gam gam mode
	  else if( candid->nDaughters()==3 && _doPionPidYourself.value()==true ) //eta->pi+ pi- pi0 mode
	    {
	      BtaCandidate* cand;
	      int g=0;
	      HepAListIterator<BtaCandidate>iter_fille = candid->daughterIterator();
	      //We ask the two charged pions daughters to pass the piLH selector
	      while ( 0 != (cand =iter_fille()) && g>=0 ) 
		{ if(cand->charge()!=0 && !pionPID(cand) ) g=-1; }
	      if(g==0) daughtersOK=true;
	    }

	  if( daughtersOK && candid->p()<=_pLABmax_eta.value() ) NewEtaList.append( candid );
	}
    }

  if(_analyzeEtap.value()==true)
    {
      HepAList< BtaCandidate >* InputEtapList = Ifd<HepAList<BtaCandidate> >::get(anEvent,_InputEtapList.value());
      if (InputEtapList == 0)   ErrMsg(fatal)<<"No etapList!!"<<endmsg;
      HepAListIterator<BtaCandidate> iter_InputEtap(*InputEtapList);  
      while ( 0 != ( candid = iter_InputEtap()) ) 
	{
	  if( candid->p()<=_pLABmax_etap.value() ) NewEtapList.append( candid );
	}
    }

  if(_analyzeNeutralRho.value()==true)
    {
      HepAList< BtaCandidate >* InputRho0List = Ifd<HepAList<BtaCandidate> >::get(anEvent,_InputRho0List.value());
      if (InputRho0List == 0)   ErrMsg(fatal)<<"No rho0List!!"<<endmsg;
      HepAListIterator<BtaCandidate> iter_InputRho0(*InputRho0List);  
      while ( 0 != ( candid = iter_InputRho0()) ) 
	{
	  bool daughtersOK=false;

	  if(_doPionPidYourself.value()==false) daughtersOK=true;
	  else 
	    {
	      BtaCandidate* cand;
	      int g=0;
	      HepAListIterator<BtaCandidate>iter_fille = candid->daughterIterator();
	      //We ask the two charged pions daughters to pass the piLH selector
	      while ( 0 != (cand =iter_fille()) && g>=0 ) 
		{ if(cand->charge()!=0 && !pionPID(cand) ) g=-1; }
	      if(g==0) daughtersOK=true;
	    }

	  if( daughtersOK && candid->p()<=_pLABmax_rho0.value() && candid->mass()>=_massmin_rho0.value() && candid->mass()<=_massmax_rho0.value()) NewRho0List.append( candid );
	}
    }

  if(_analyzeChargedRho.value()==true)
    {
      HepAList< BtaCandidate >* InputRhoCList = Ifd<HepAList<BtaCandidate> >::get(anEvent,_InputRhoCList.value());
      if (InputRhoCList == 0)   ErrMsg(fatal)<<"No rhoCList!!"<<endmsg;
      HepAListIterator<BtaCandidate> iter_InputRhoC(*InputRhoCList);  
      while ( 0 != ( candid = iter_InputRhoC()) ) 
	{
	  bool daughtersOK=false;

	  if(_doPionPidYourself.value()==false) daughtersOK=true;
	  else 
	    {
	      BtaCandidate* cand;
	      int g=0;
	      HepAListIterator<BtaCandidate>iter_fille = candid->daughterIterator();
	      //We ask the two charged pions daughters to pass the piLH selector
	      while ( 0 != (cand =iter_fille()) && g>=0 ) 
		{ if(cand->charge()!=0 && !pionPID(cand) ) g=-1; }
	      if(g==0) daughtersOK=true;
	    }
	  
	  if( daughtersOK && candid->p()<=_pLABmax_rhoC.value() && candid->mass()>=_massmin_rhoC.value() && candid->mass()<=_massmax_rhoC.value()) NewRhoCList.append( candid );
	}
    }
  
  if(_analyzeOmega.value()==true)
    {
      HepAList< BtaCandidate >* InputOmegaList = Ifd<HepAList<BtaCandidate> >::get(anEvent,_InputOmegaList.value());
      if (InputOmegaList == 0)   ErrMsg(fatal)<<"No omegaList!!"<<endmsg;
      HepAListIterator<BtaCandidate> iter_InputOmega(*InputOmegaList);  
      while ( 0 != ( candid = iter_InputOmega()) ) 
	{
	  bool daughtersOK=false;

	  if(_doPionPidYourself.value()==false) daughtersOK=true;
	  else 
	    {
	      BtaCandidate* cand;
	      int g=0;
	      HepAListIterator<BtaCandidate>iter_fille = candid->daughterIterator();
	      //We ask the two charged pions daughters to pass the piLH selector
	      while ( 0 != (cand =iter_fille()) && g>=0 ) 
		{ if(cand->charge()!=0 && !pionPID(cand) ) g=-1; }
	      if(g==0) daughtersOK=true;
	    }

	  if( daughtersOK && candid->p()<=_pLABmax_omega.value() ) NewOmegaList.append( candid );
	}
    }
  
  if(_analyzeGamma.value()==true)
    {
      HepAList< BtaCandidate >* InputGammaList = Ifd<HepAList<BtaCandidate> >::get(anEvent,_InputGammaList.value());
      if (InputGammaList == 0)   ErrMsg(fatal)<<"No gammaList!!"<<endmsg;
      HepAListIterator<BtaCandidate> iter_InputGamma(*InputGammaList);  
      while ( 0 != ( candid = iter_InputGamma()) ) 
	{
	  if( candid->p() <= _pLABmax_gamma.value() ) NewGammaList.append( candid );
	}
    } // if _analyzeGamma
}



AppResult
XSLBtoXulnuFilter::endJob( AbsEvent* anEvent )
{

  if(_MyVerbose.value())
    {	
      //Setting the precision of the numbers to be printed to a higher value...
      ostream& os = ErrMsg(routine);
      int p=os.precision(); 
      os<<setprecision(8);

      cout<<"Results from B->Xu l nu XSL skim"<<endl;
      cout<<endl;
      cout<<"SUMMARY:"<<endl;
      cout<<"Total number of events before any cut:              "<<_TotalNbEvents<<endl;
      cout<<"Events passing Event cuts:                          "<<_NbAfterEventCuts<<" --> "<<_NbAfterEventCuts/_TotalNbEvents*100<<"%"<<endl;
      cout<<"Events passing Couple cuts (All Modes):             "<<_NbAllModesAfterCoupleCuts<<" --> "<<_NbAllModesAfterCoupleCuts/_TotalNbEvents*100<<"%"<<endl;
      cout<<"Events passing Couple cuts (Pi Mode):               "<<_NbPiAfterCoupleCuts<<" --> "<<_NbPiAfterCoupleCuts/_TotalNbEvents*100<<"%"<<endl;
      cout<<"Events passing Couple cuts (Pi0 Mode):              "<<_NbPi0AfterCoupleCuts<<" --> "<<_NbPi0AfterCoupleCuts/_TotalNbEvents*100<<"%"<<endl;
      cout<<"Events passing Couple cuts (Eta Mode):              "<<_NbEtaAfterCoupleCuts<<" --> "<<_NbEtaAfterCoupleCuts/_TotalNbEvents*100<<"%"<<endl;
      cout<<"Events passing Couple cuts (Eta' Mode):             "<<_NbEtapAfterCoupleCuts<<" --> "<<_NbEtapAfterCoupleCuts/_TotalNbEvents*100<<"%"<<endl;
      cout<<"Events passing Couple cuts (RhoC Mode):             "<<_NbRhoCAfterCoupleCuts<<" --> "<<_NbRhoCAfterCoupleCuts/_TotalNbEvents*100<<"%"<<endl;
      cout<<"Events passing Couple cuts (Rho0 Mode):             "<<_NbRho0AfterCoupleCuts<<" --> "<<_NbRho0AfterCoupleCuts/_TotalNbEvents*100<<"%"<<endl;
      cout<<"Events passing Couple cuts (Omega Mode):            "<<_NbOmeAfterCoupleCuts<<" --> "<<_NbOmeAfterCoupleCuts/_TotalNbEvents*100<<"%"<<endl;
      cout<<"Events passing Couple cuts (Gamma Mode):            "<<_NbGammaAfterCoupleCuts<<" --> "<<_NbGammaAfterCoupleCuts/_TotalNbEvents*100<<"%"<<endl;
      cout<<endl;
      cout<<"BtaCandidates INFO:"<<endl;
      cout<<"Nb of BtaCandidates/accepted event after all cuts (All Modes):  "<<_NbCoupleAllModesAfterAllCuts<<" --> "<<_NbCoupleAllModesAfterAllCuts/_NbAllModesAfterCoupleCuts<<endl;
      cout<<"Nb of BtaCandidates/accepted event after all cuts (pi Mode):    "<<_NbCouplePiAfterAllCuts<<" --> "<<_NbCouplePiAfterAllCuts/_NbPiAfterCoupleCuts<<endl;
      cout<<"Nb of BtaCandidates/accepted event after all cuts (pi0 Mode):   "<<_NbCouplePi0AfterAllCuts<<" --> "<<_NbCouplePi0AfterAllCuts/_NbPi0AfterCoupleCuts<<endl;
      cout<<"Nb of BtaCandidates/accepted event after all cuts (eta Mode):   "<<_NbCoupleEtaAfterAllCuts<<" --> "<<_NbCoupleEtaAfterAllCuts/_NbEtaAfterCoupleCuts<<endl;
      cout<<"Nb of BtaCandidates/accepted event after all cuts (eta' Mode):   "<<_NbCoupleEtapAfterAllCuts<<" --> "<<_NbCoupleEtapAfterAllCuts/_NbEtapAfterCoupleCuts<<endl;
      cout<<"Nb of BtaCandidates/accepted event after all cuts (rhoC Mode):  "<<_NbCoupleRhoCAfterAllCuts<<" --> "<<_NbCoupleRhoCAfterAllCuts/_NbRhoCAfterCoupleCuts<<endl;
      cout<<"Nb of BtaCandidates/accepted event after all cuts (rho0 Mode):  "<<_NbCoupleRho0AfterAllCuts<<" --> "<<_NbCoupleRho0AfterAllCuts/_NbRho0AfterCoupleCuts<<endl;
      cout<<"Nb of BtaCandidates/accepted event after all cuts (omega Mode): "<<_NbCoupleOmegaAfterAllCuts<<" --> "<<_NbCoupleOmegaAfterAllCuts/_NbOmeAfterCoupleCuts<<endl;
      cout<<"Nb of BtaCandidates/accepted event after all cuts (gamma Mode): "<<_NbCoupleGammaAfterAllCuts<<" --> "<<_NbCoupleGammaAfterAllCuts/_NbGammaAfterCoupleCuts<<endl;
      cout<<endl;
      cout<<"CUT BY CUT RESULTS:"<<endl;
      cout<<"Events passing Trigger:                             "<<_NbAfterTrigger<<" --> "<<_NbAfterTrigger/_TotalNbEvents*100<<"%"<<endl;
      cout<<"Events passing ISH:                                 "<<_NbAfterISHCuts<<" --> "<<_NbAfterISHCuts/_TotalNbEvents*100<<"%"<<endl;
      cout<<"Events passing R2All:                               "<<_NbAfterR2AllCuts<<" --> "<<_NbAfterR2AllCuts/_TotalNbEvents*100<<"%"<<endl;
      cout<<"Events passing >=1 lepton:                          "<<_NbAfterOneLeptonTight<<" --> "<<_NbAfterOneLeptonTight/_TotalNbEvents*100<<"%"<<endl;
      cout<<endl;
      cout<<"Events passing Couple lepton Sel cuts (Pi Mode):    "<<_NbPiAfterLeptonSelCuts<<" --> "<<_NbPiAfterLeptonSelCuts/_TotalNbEvents*100<<"%"<<endl;
      cout<<"Events passing Couple TrkNb cut (Pi Mode):          "<<_NbPiAfterTrkNb<<" --> "<<_NbPiAfterTrkNb/_TotalNbEvents*100<<"%"<<endl;
      cout<<"Events passing Couple cosBY cut (Pi Mode):          "<<_NbPiAfterCosBY<<" --> "<<_NbPiAfterCosBY/_TotalNbEvents*100<<"%"<<endl;
      cout<<"Events passing Couple pStar2D cut (Pi Mode):        "<<_NbPiAfterpStar2D<<" --> "<<_NbPiAfterpStar2D/_TotalNbEvents*100<<"%"<<endl;
      cout<<"Events passing Couple cosTT cut (Pi Mode):          "<<_NbPiAfterCosTT<<" --> "<<_NbPiAfterCosTT/_TotalNbEvents*100<<"%"<<endl;
      cout<<endl;
      cout<<"Events passing Couple lepton Sel cuts (Pi0 Mode):   "<<_NbPi0AfterLeptonSelCuts<<" --> "<<_NbPi0AfterLeptonSelCuts/_TotalNbEvents*100<<"%"<<endl;
      cout<<"Events passing Couple TrkNb cut (Pi0 Mode):         "<<_NbPi0AfterTrkNb<<" --> "<<_NbPi0AfterTrkNb/_TotalNbEvents*100<<"%"<<endl;
      cout<<"Events passing Couple cosBY cut (Pi0 Mode):         "<<_NbPi0AfterCosBY<<" --> "<<_NbPi0AfterCosBY/_TotalNbEvents*100<<"%"<<endl;
      cout<<"Events passing Couple pStar2D cut (Pi0 Mode):       "<<_NbPi0AfterpStar2D<<" --> "<<_NbPi0AfterpStar2D/_TotalNbEvents*100<<"%"<<endl;
      cout<<"Events passing Couple cosTT cut (Pi0 Mode):         "<<_NbPi0AfterCosTT<<" --> "<<_NbPi0AfterCosTT/_TotalNbEvents*100<<"%"<<endl;
      cout<<endl;
      cout<<"Events passing Couple lepton Sel cuts (Eta Mode):   "<<_NbEtaAfterLeptonSelCuts<<" --> "<<_NbEtaAfterLeptonSelCuts/_TotalNbEvents*100<<"%"<<endl;
      cout<<"Events passing Couple cosBY cut (Eta Mode):         "<<_NbEtaAfterCosBY<<" --> "<<_NbEtaAfterCosBY/_TotalNbEvents*100<<"%"<<endl;
      cout<<"Events passing Couple pStar2D cut (Eta Mode):       "<<_NbEtaAfterpStar2D<<" --> "<<_NbEtaAfterpStar2D/_TotalNbEvents*100<<"%"<<endl;
      cout<<"Events passing Couple cosTT cut (Eta Mode):         "<<_NbEtaAfterCosTT<<" --> "<<_NbEtaAfterCosTT/_TotalNbEvents*100<<"%"<<endl;
      cout<<endl;
      cout<<"Events passing Couple lepton Sel cuts (Eta' Mode):  "<<_NbEtapAfterLeptonSelCuts<<" --> "<<_NbEtapAfterLeptonSelCuts/_TotalNbEvents*100<<"%"<<endl;
      cout<<"Events passing Couple cosBY cut (Eta' Mode):        "<<_NbEtapAfterCosBY<<" --> "<<_NbEtapAfterCosBY/_TotalNbEvents*100<<"%"<<endl;
      cout<<"Events passing Couple pStar2D cut (Eta' Mode):      "<<_NbEtapAfterpStar2D<<" --> "<<_NbEtapAfterpStar2D/_TotalNbEvents*100<<"%"<<endl;
      cout<<"Events passing Couple cosTT cut (Eta' Mode):        "<<_NbEtapAfterCosTT<<" --> "<<_NbEtapAfterCosTT/_TotalNbEvents*100<<"%"<<endl;
      cout<<endl;
      cout<<"Events passing Couple lepton Sel cuts (RhoC Mode):  "<<_NbRhoCAfterLeptonSelCuts<<" --> "<<_NbRhoCAfterLeptonSelCuts/_TotalNbEvents*100<<"%"<<endl;
      cout<<"Events passing Couple cosBY cut (RhoC Mode):        "<<_NbRhoCAfterCosBY<<" --> "<<_NbRhoCAfterCosBY/_TotalNbEvents*100<<"%"<<endl;
      cout<<"Events passing Couple pStar2D cut (RhoC Mode):      "<<_NbRhoCAfterpStar2D<<" --> "<<_NbRhoCAfterpStar2D/_TotalNbEvents*100<<"%"<<endl;
      cout<<"Events passing Couple cosTT cut (RhoC Mode):        "<<_NbRhoCAfterCosTT<<" --> "<<_NbRhoCAfterCosTT/_TotalNbEvents*100<<"%"<<endl;
      cout<<endl;
      cout<<"Events passing Couple lepton Sel cuts (Rho0 Mode):  "<<_NbRho0AfterLeptonSelCuts<<" --> "<<_NbRho0AfterLeptonSelCuts/_TotalNbEvents*100<<"%"<<endl;
      cout<<"Events passing Couple cosBY cut (Rho0 Mode):        "<<_NbRho0AfterCosBY<<" --> "<<_NbRho0AfterCosBY/_TotalNbEvents*100<<"%"<<endl;
      cout<<"Events passing Couple pStar2D cut (Rho0 Mode):      "<<_NbRho0AfterpStar2D<<" --> "<<_NbRho0AfterpStar2D/_TotalNbEvents*100<<"%"<<endl;
      cout<<"Events passing Couple cosTT cut (Rho0 Mode):        "<<_NbRho0AfterCosTT<<" --> "<<_NbRho0AfterCosTT/_TotalNbEvents*100<<"%"<<endl;
      cout<<endl;
      cout<<"Events passing Couple lepton Sel cuts (Omega Mode): "<<_NbOmeAfterLeptonSelCuts<<" --> "<<_NbOmeAfterLeptonSelCuts/_TotalNbEvents*100<<"%"<<endl;
      cout<<"Events passing Couple cosBY cut (Omega Mode):       "<<_NbOmeAfterCosBY<<" --> "<<_NbOmeAfterCosBY/_TotalNbEvents*100<<"%"<<endl;
      cout<<"Events passing Couple pStar2D cut (Omega Mode):     "<<_NbOmeAfterpStar2D<<" --> "<<_NbOmeAfterpStar2D/_TotalNbEvents*100<<"%"<<endl;
      cout<<"Events passing Couple cosTT cut (Omega Mode):       "<<_NbOmeAfterCosTT<<" --> "<<_NbOmeAfterCosTT/_TotalNbEvents*100<<"%"<<endl;
      cout<<endl;
      cout<<"Events passing Couple lepton Sel cuts (Gamma Mode): "<<_NbGammaAfterLeptonSelCuts<<" --> "<<_NbGammaAfterLeptonSelCuts/_TotalNbEvents*100<<"%"<<endl;
      cout<<"Events passing Couple cosBY cut (Gamma Mode):       "<<_NbGammaAfterCosBY<<" --> "<<_NbGammaAfterCosBY/_TotalNbEvents*100<<"%"<<endl;
      cout<<"No pStar2D cut for the Gamma Mode"<<endl;
      cout<<"Events passing Couple cosTT cut (Gamma Mode):       "<<_NbGammaAfterCosTT<<" --> "<<_NbGammaAfterCosTT/_TotalNbEvents*100<<"%"<<endl;
      cout<<endl;
      cout<<"C't'a peu pres ca pour a soir... Byebye... Byebye..."<<endl;

      //...setting it back to its original precision.
      os<<setprecision(p)<<endmsg;
    }
  
  return AppResult::OK;
  
}

//////////////

bool XSLBtoXulnuFilter::pionPID( BtaCandidate* candid )
{
  BtaCandidate* PIDcand;
  
  HepAListIterator<BtaCandidate> piL(*_piLooseLH);
  while( (PIDcand = piL()) ){
    if( PIDcand->overlaps(*candid) )
      { 
	return true;
      }
  }
  
  return false;  
}


double 
XSLBtoXulnuFilter::cosTT( BtaCandidate* Y )
{
  HepAList<BtaCandidate> YList; //The YList is filled with trks and photons only
  BtaCandidate* filleL1,*filleL2,*filleL3,*filleL4; //L1 is Xu, L2 is eta'->eta, L3 is eta->pi0, L4 is pi0->gamma      
  HepAListIterator<BtaCandidate> YL1 = Y->daughterIterator();
  while ( 0 != (filleL1 = YL1()) )   
    {
      if(filleL1->nDaughters()==0) YList.append(filleL1);
      else {
	HepAListIterator<BtaCandidate> YL2 = filleL1->daughterIterator();
	while ( 0 != (filleL2 = YL2()) )
	  {   
	    if(filleL2->nDaughters()==0) YList.append(filleL2);
	    else {
	      HepAListIterator<BtaCandidate> YL3 = filleL2->daughterIterator();
	      while ( 0 != (filleL3 = YL3()) )
		{   
		  if(filleL3->nDaughters()==0) YList.append(filleL3);
		  else {
		    HepAListIterator<BtaCandidate> YL4 = filleL3->daughterIterator();
		    while ( 0 != (filleL4 = YL4()) )
		      {   
			if(filleL4->nDaughters()==0) YList.append(filleL4);
			else ErrMsg(fatal) << "Problem in XSLReader::FillNtupleWithCoupleVariables !!"<<endmsg;
		      }//while level4
		  }//else level4
		}//while level 3
	    }//else Level3
	  }//while Level2
      }//else Level2
    }//Level1
  
  
  HepAList<BtaCandidate> ListAll; 
  ListAll.append( *_BasicTrkList );
  ListAll.append( *_BasicBumpList );
  HepAListIterator<BtaCandidate> iterAll( ListAll );
  
  HepAList<BtaCandidate> ROEList;  //Rest of event 
  BtaCandidate *CandAllL1,*CandAllL2, *CandY;
  while ( 0 != ( CandAllL1 = iterAll()) ) 
    {
      if(CandAllL1->nDaughters()==0)
	{
	  bool StillNotFound=true;
	  HepAListIterator<BtaCandidate> iterY( YList );
	  while ( 0 != ( CandY = iterY()) && StillNotFound ) 
	    {
	      if(CandY->uid()==CandAllL1->uid()) StillNotFound=false;
	    }//
	  if(StillNotFound) ROEList.append(CandAllL1);
	}// if(CandAllL1->nDaughters()==0)
      else
	{
	  HepAListIterator<BtaCandidate> AllL2 = CandAllL1->daughterIterator();
	  while ( 0 != (CandAllL2 = AllL2()) )
	    {   
	      bool StillNotFound=true;
	      HepAListIterator<BtaCandidate> iterY( YList );
	      while ( 0 != ( CandY = iterY()) && StillNotFound ) 
		{
		  if(CandY->uid()==CandAllL2->uid()) StillNotFound=false;
		}//
	      if(StillNotFound) ROEList.append(CandAllL2);
	    }	      
	}//else Level2 (this happens with Brem reco'ed electrons)
    }// while ( 0 != ( CandAllL1 = iterAll()) ) 
  
  
  
  ////////////////////////////////////////
  //  Thrust (input list should NOT be in the CMS frame)
  BtaThrust YTH(YList, *_eventInfo);		    
  Hep3Vector Yaxis = YTH.thrust_axis();
  
  BtaThrust RoeThrust(ROEList, *_eventInfo);		    
  Hep3Vector RoeAxis = RoeThrust.thrust_axis();
  
  double cosTT = fabs( Yaxis.dot(RoeAxis) );
  return cosTT;
}



// clone
//
AppModule* 
XSLBtoXulnuFilter::clone(const char* cloneName)
{
  return new XSLBtoXulnuFilter(cloneName, "clone of XSLBtoXulnuFilter");
}


AppResult
XSLBtoXulnuFilter::QuitEvent()
{
  //CLEANING the list before quiting the event...
  //Note that this does not create a memory leak because the BtaCandidates in the lists are not created with new, 
  //but just pointers to other BtaCandidates owned outside or XSLBtoXulnuFilter 
  NewLepList.removeAll();
  NewPiList.removeAll();
  NewPi0List.removeAll();
  NewEtaList.removeAll();
  NewEtapList.removeAll();
  NewOmegaList.removeAll();
  NewRho0List.removeAll();
  NewRhoCList.removeAll();
  NewGammaList.removeAll();
  
  HepAListDeleteAll( NewLepList );
  HepAListDeleteAll( NewPiList );
  HepAListDeleteAll( NewPi0List );
  HepAListDeleteAll( NewEtaList );
  HepAListDeleteAll( NewEtapList );
  HepAListDeleteAll( NewOmegaList );
  HepAListDeleteAll( NewRho0List );
  HepAListDeleteAll( NewRhoCList );
  HepAListDeleteAll( NewGammaList );
  
  delete _CMS;

  return AppResult::OK;

}



double 
XSLBtoXulnuFilter::cosBY(double Ebeam, double mB, double mY, double pY){
  
  double EY=sqrt(mY*mY + pY*pY);
  
  double num = 2*Ebeam*EY-(mB*mB)-(mY*mY);
  double den = 2*sqrt(Ebeam*Ebeam-mB*mB)*pY;
  double cBY = num/den;

  return cBY;
}
