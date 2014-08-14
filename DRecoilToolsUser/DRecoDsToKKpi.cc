
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "DRecoDsToKKpi.hh"

//-------------
// C Headers --
//-------------
#include <assert.h>

//---------------
// C++ Headers --
//---------------
#include <iostream>
#include <math.h>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "AbsEvent/AbsEvent.hh"
#include "Beta/EventInfo.hh"
#include "Beta/BtaCandidate.hh"
#include "BetaCoreTools/BtaOpMakeTree.hh"
#include "CLHEP/Alist/AList.h"
#include "CLHEP/Alist/AIterator.h"
#include "ErrLogger/ErrLog.hh"
#include "PDT/Pdt.hh"
#include "ProxyDict/IfdStrKey.hh"
#include "ProxyDict/IfdKey.hh"
#include "ProxyDict/Ifd.hh"
#include "ProxyDict/IfdHepAListProxy.hh"

#include "TrkBase/TrkAbsFit.hh"
#include "VtxBase/BtaAbsVertexer.hh"
#include "VtxFitter/VtxFitterOper.hh"
#include "VtxBase/VtxAbsAlgorithm.hh"
#include "Beta/BtaAbsVertex.hh"
#include "ProbTools/probab.hh"

#include "TrkBase/TrkRecoTrk.hh"

#include "BetaCoreTools/BtaMcAssoc.hh"
#include "BtaTupleMaker/BtuUtil.hh"

#include "BetaCoreTools/BtaPrintTree.hh"
#include "BetaCoreTools/BtaBooster.hh"

#include "BbrGeom/BbrVectorErr.hh"

using std::cout;
using std::endl;
using std::ostream;

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//----------------
// Constructors --
//----------------
DRecoDsToKKpi::DRecoDsToKKpi( const char* const theName,const char* const theDescription )
  : TagFilterModule( theName, theDescription )
  , DsMassCutLo("DsMassCutLo",this,1.87)
  , DsMassCutHi("DsMassCutHi",this,2.07)
  , DeltaMCutLo("DeltaMCutLo",this,.100)
  , DeltaMCutHi("DeltaMCutHi",this,.170)
  , FitProbCut("FitProbCut",this,.0)
  , DsMomentumCut("DsMomentumCut",this,2.5)
  , DsStMassCutLo("DsStMassCutLo",this,2.012)
  , DsStMassCutHi("DsStMassCutHi",this,2.212)
  , NMaxExtraTrk("NMaxExtraTrk",this,0)
  , MaxExtraEnergy("MaxExtraEnergy",this,2.)
  , KList("KList",this,"GoodTracksVeryLoose")
  , PiList("PiList",this,"GoodTracksVeryLoose")
  , ExtraTrkList("ExtraTrkList",this,"GoodTracksVeryLoose")
  , ExtraNeutralList("ExtraNeutralList",this,"GoodPhotonLoose")
  , CutSelectedCands("CutSelectedCands",this,0)
  , _scalers("DRecoDsToKKpi")
  , DRecoDsToKKpiUsrBlockName("DRecoDsToKKpiUsrBlock",this,"DRecoDsToKKpiUsrBlock")
  , KKpiMass("KKpiMass",0.)
  , KKpiDeltaM("KKpiDeltaM",0.)
  , KKpiDsStMass("KKpiDsStMass",0.)
  , KKpiPstar("KKpiPstar",0.)
  , KKpiAngle("KKpiAngle",0.)
  , KKMass("KKMass",0.)
  , KpiMass("KpiMass",0.)
  , KKpiNExtraTrk("KKpiNExtraTrk",0)
  , KKpiNExtraNeutral("KKpiNExtraNeutral",0)
  , KKpiExtraNeutralE("KKpiExtraNeutralE",0.)
  , KKpiDsMCIdx("KKpiDsMCIdx",0)
  , KKpiDsStMCIdx("KKpiDsStMCIdx",0)
{

  commands()->append( &FitProbCut);
  commands()->append( &DsMassCutLo);
  commands()->append( &DsMassCutHi);
  commands()->append( &DeltaMCutLo);
  commands()->append( &DeltaMCutHi);
  commands()->append( &DsMomentumCut);
  commands()->append( &DsStMassCutLo);
  commands()->append( &DsStMassCutHi);
  commands()->append( &NMaxExtraTrk);
  commands()->append( &MaxExtraEnergy);
  commands()->append( &KList);
  commands()->append( &PiList);
  commands()->append( &ExtraTrkList);
  commands()->append( &ExtraNeutralList);
  commands()->append( &CutSelectedCands);

  DRecoDsToKKpiUsrBlock.addVariable(KKpiMass);
  DRecoDsToKKpiUsrBlock.addVariable(KKpiDeltaM);
  DRecoDsToKKpiUsrBlock.addVariable(KKpiDsStMass);
  DRecoDsToKKpiUsrBlock.addVariable(KKpiPstar);
  DRecoDsToKKpiUsrBlock.addVariable(KKpiAngle);
  DRecoDsToKKpiUsrBlock.addVariable(KKMass);
  DRecoDsToKKpiUsrBlock.addVariable(KpiMass);
  DRecoDsToKKpiUsrBlock.addVariable(KKpiNExtraTrk);
  DRecoDsToKKpiUsrBlock.addVariable(KKpiNExtraNeutral);
  DRecoDsToKKpiUsrBlock.addVariable(KKpiExtraNeutralE);
  DRecoDsToKKpiUsrBlock.addVariable(KKpiDsMCIdx);
  DRecoDsToKKpiUsrBlock.addVariable(KKpiDsStMCIdx);
}

//--------------
// Destructor --
//--------------
DRecoDsToKKpi::~DRecoDsToKKpi()
{}

//--------------
// Operations --
//--------------
AppResult DRecoDsToKKpi::beginJob( AbsEvent* anEvent )
{ 
  return AppResult::OK;
}

AppResult DRecoDsToKKpi::event( AbsEvent* anEvent )
{
  _scalers.sum("#  events ");
  
  UsrIfd<UsrCandBlock>::put(anEvent,&DRecoDsToKKpiUsrBlock,DRecoDsToKKpiUsrBlockName.value());

  //create evt list and add to event
  HepAList<BtaCandidate>* DRecoDsToKKpiList = new HepAList<BtaCandidate>;
  if (DRecoDsToKKpiList == 0) ErrMsg(fatal) << "Failed to create a DRecoDsToKKpiList list"	<< endmsg;
  IfdHepAListProxy<BtaCandidate>* proxy = new IfdHepAListProxy<BtaCandidate>(DRecoDsToKKpiList);
  if(proxy == 0) ErrMsg(fatal) << "Failed to create DRecoDsToKKpiList list proxy" << endmsg;
  if( !Ifd<HepAList<BtaCandidate> >::put(anEvent, proxy,"DRecoDsToKKpiList"))
    ErrMsg(fatal) << "failed to add DRecoDsToKKpiList list to event using proxy" << endmsg; 


  //create extra track and photon lists to dump later
  HepAList<BtaCandidate>* DRecoDsToKKpiExtraTrkList = new HepAList<BtaCandidate>;
  if (DRecoDsToKKpiExtraTrkList == 0) ErrMsg(fatal) << "Failed to create a DRecoDsToKKpiExtraTrkList list"	<< endmsg;
  IfdHepAListProxy<BtaCandidate>* proxytrack = new IfdHepAListProxy<BtaCandidate>(DRecoDsToKKpiExtraTrkList);
  if(proxy == 0) ErrMsg(fatal) << "Failed to create DRecoDsToKKpiExtraTrkList list proxy" << endmsg;
  if( !Ifd<HepAList<BtaCandidate> >::put(anEvent, proxytrack,"DRecoDsToKKpiExtraTrkList"))
    ErrMsg(fatal) << "failed to add DRecoDsToKKpiExtraTrkList list to event using proxy" << endmsg; 

  HepAList<BtaCandidate>* DRecoDsToKKpiExtraGamList = new HepAList<BtaCandidate>;
  if (DRecoDsToKKpiExtraGamList == 0) ErrMsg(fatal) << "Failed to create a DRecoDsToKKpiExtraGamList list"	<< endmsg;
  IfdHepAListProxy<BtaCandidate>* proxygam = new IfdHepAListProxy<BtaCandidate>(DRecoDsToKKpiExtraGamList);
  if(proxy == 0) ErrMsg(fatal) << "Failed to create DRecoDsToKKpiExtraGamList list proxy" << endmsg;
  if( !Ifd<HepAList<BtaCandidate> >::put(anEvent, proxygam,"DRecoDsToKKpiExtraGamList"))
    ErrMsg(fatal) << "failed to add DRecoDsToKKpiExtraGamList list to event using proxy" << endmsg; 


  //retreive the event candidates
  HepAList< BtaCandidate >* DRecoEvtCandidateList = Ifd<HepAList< BtaCandidate>  >::get(anEvent,"DRecoCandidateList");
  if (DRecoEvtCandidateList == 0)  ErrMsg(fatal) <<" DRecoDsToKKpi: DRecoCandidateList not found" << endmsg;   

  // Create 
  HepAList< BtaCandidate >  TempEvtCandList ;
  HepAListIterator< BtaCandidate > EvtListIter(*DRecoEvtCandidateList);
  while (BtaCandidate * candlist = EvtListIter()){
    TempEvtCandList += new BtaCandidate(*candlist);
  }
  HepAListDeleteAll(*DRecoEvtCandidateList);    


  // pointer to the event summary info object
  HepAList< EventInfo >* eventInfoListPtr =  Ifd<HepAList< EventInfo > >::get(anEvent,"Default");
  if (eventInfoListPtr == 0){
    ErrMsg(fatal) << "Could not locate event info list named  in the event."
                  << " This is likely to be a configuration error." << endmsg;
  } 
  EventInfo* eventInfo = eventInfoListPtr->first();
  if (eventInfo == 0) {
    ErrMsg(fatal) << "Nothing in eventInfoList...aborting" << endmsg;
  }
  //the beam spot below has been smeared in MC and fixed error=10mu data/MC
  const BbrPointErr beamSpot = eventInfoListPtr->first()->beamSpot();

  //cm booster
  BtaBooster cmbooster(eventInfo->cmFrame()); 
   
  //get the K list
  HepAList< BtaCandidate >* KEvtList = Ifd<HepAList< BtaCandidate>  >::get(anEvent,KList.value());
  if (KEvtList == 0)  ErrMsg(fatal) <<" DRecoDsToKKpi: KEvtList not found" << endmsg;  
  HepAList< BtaCandidate >  TempKList ;
  HepAListIterator< BtaCandidate > KIter(*KEvtList); 
  while (BtaCandidate * candK = KIter()){//set the type
    BtaCandidate * newcandK= new BtaCandidate(*candK);
    newcandK->setType((candK->charge()>0) ? Pdt::lookup( PdtPdg::K_plus) : Pdt::lookup( PdtPdg::K_minus));
    TempKList+=newcandK;
  }

  //get the Pi list
  HepAList< BtaCandidate >* PiEvtList = Ifd<HepAList< BtaCandidate>  >::get(anEvent,PiList.value());
  if (PiEvtList == 0)  ErrMsg(fatal) <<" DRecoDsToKKpi: PiEvtList not found" << endmsg;  
  HepAList< BtaCandidate >  TempPiList ;
  HepAListIterator< BtaCandidate > PiIter(*PiEvtList); 
  while (BtaCandidate * candPi = PiIter()){//set the type
    BtaCandidate * newcandPi= new BtaCandidate(*candPi);
    newcandPi->setType((candPi->charge()>0) ? Pdt::lookup( PdtPdg::pi_plus) : Pdt::lookup( PdtPdg::pi_minus));
    TempPiList+=newcandPi;
  }

  //create a list of Ds->KKpi
  HepAList< BtaCandidate >  TempDsCandList;
  HepAListIterator< BtaCandidate > KIter1(TempKList); 
  while (BtaCandidate * candK1 = KIter1()){     
    if(candK1->charge()==-1.)continue;//choose K+K-pi+- otherwise duplication occurs
    HepAListIterator< BtaCandidate > KIter2(TempKList); 
    while (BtaCandidate * candK2 = KIter2()){     
      if(candK1->uid()==candK2->uid()||candK2->charge()==+1.)continue;
      HepAListIterator< BtaCandidate > PiIter(TempPiList); 
      while (BtaCandidate * candPi= PiIter()){ 
	if(candK1->uid()==candPi->uid()||candK2->uid()==candPi->uid())continue;	
	_scalers.sum("Ds after uid/charge cut");
	
	BtaOpMakeTree combDsCand;
	BtaCandidate recoDs= combDsCand.combine(*candK1,*candK2,*candPi);
	recoDs.setType((candPi->charge()>0) ? Pdt::lookup( PdtPdg::D_s_plus) : Pdt::lookup( PdtPdg::D_s_minus));
	//pre mass cut
	if(recoDs.mass()<DsMassCutLo.value()-.1||DsMassCutHi.value()+.1<recoDs.mass())continue;
	_scalers.sum("Ds pre mass cut");
	

	//pre p* cut
	BtaCandidate preboostfitDs=cmbooster.boostTo(recoDs);
	if(preboostfitDs.p()<DsMomentumCut.value()-.2)continue;
	_scalers.sum("Ds pre p* cut");

	//fit the Ds 
	setBeamConstraint(recoDs,eventInfo) ; 
	VtxFitterOper fitter(recoDs,VtxAbsAlgorithm::TreeFitter);
	fitter.invalidateFit();
	fitter.fitAll();
	recoDs=fitter.getFittedTree();
	if((recoDs.decayVtx())->status()!=BtaAbsVertex::Success)continue;
	_scalers.sum("Ds fit-succes");	
	if(probab((int)recoDs.decayVtx()->nDof(),recoDs.decayVtx()->chiSquared()) < FitProbCut.value()) continue;
	_scalers.sum("Ds fit-prob");
	
	//final cut on mass
	if(recoDs.mass()<DsMassCutLo.value()||DsMassCutHi.value()<recoDs.mass())continue;
	_scalers.sum("Ds  mass cut");

	//p* cut
	BtaCandidate boostfitDs=cmbooster.boostTo(recoDs);
	if(boostfitDs.p()<DsMomentumCut.value())continue;
	_scalers.sum("Ds p* cut");
    
	
	TempDsCandList+=new BtaCandidate(recoDs);
      } 
    }
  }

  //get extra lists 
  HepAList< BtaCandidate >* EvtExtraTrkList = Ifd<HepAList< BtaCandidate>  >::get(anEvent,ExtraTrkList.value());
  if (EvtExtraTrkList == 0)  ErrMsg(fatal) <<" DRecoDsToKKpi: ExtraTrkList not found" << endmsg;  
  HepAList< BtaCandidate >* EvtExtraNeutralList = Ifd<HepAList< BtaCandidate>  >::get(anEvent,ExtraNeutralList.value());
  if (EvtExtraNeutralList == 0)  ErrMsg(fatal) <<" DRecoDsToKKpi: ExtraNeutralList not found" << endmsg;  

  //truth match
  BtaMcAssoc* _truthMap=Ifd< BtaMcAssoc >::get( anEvent,"GHit");
  HepAList<BtaCandidate>* _mcList=Ifd< HepAList<BtaCandidate> >::get(anEvent,"MCTruth" );   
  
  //create a list of Ds->ToKKpi which have been selected and make sure below that the same cand does not get added twice
  //this should remove multiple reconstructions that are truth-matched to true
  HepAList< BtaCandidate >  SelectedDsCandList;  

  //loop over the upsilons
  HepAListIterator< BtaCandidate > ListIter(TempEvtCandList); 
  while (BtaCandidate * candEvt = ListIter()){     
    _scalers.sum("initial Ups");
    
    HepAListIterator< BtaCandidate >  UpsIter=candEvt->daughterIterator();
    const BtaCandidate * candTag=UpsIter(); 
    const BtaCandidate * candDsSt=UpsIter();  
    HepAListIterator< BtaCandidate >  DsStIter=candDsSt->daughterIterator();
    const BtaCandidate * candDs=DsStIter();  
    const BtaCandidate * candGam=DsStIter(); 
    
    HepAListIterator< BtaCandidate > DsIter(TempDsCandList); 
    while (BtaCandidate * recoDs = DsIter()){     
      if(recoDs->overlaps(*candEvt))continue;
      _scalers.sum("Ds overlap cut");

      if(recoDs->charge()!=candDs->charge())continue;
      _scalers.sum("Ds charge cut");
      
      BtaOpMakeTree combDsStCand;
      BtaCandidate recoDsSt= combDsStCand.combine(*recoDs,*candGam);

      if(recoDsSt.mass()<DsStMassCutLo.value()||DsStMassCutHi.value()<recoDsSt.mass())continue;
      _scalers.sum("Ds* mass cut");

      if((recoDsSt.mass()-recoDs->mass())<DeltaMCutLo.value()||DeltaMCutHi.value()<(recoDsSt.mass()-recoDs->mass()))continue;
      _scalers.sum("Ds deltaM cut");
      
      //extra tracks
      int NExtraTrk=0;
      HepAListIterator< BtaCandidate > ExtraTrkIter(*EvtExtraTrkList);
      while (BtaCandidate * candtrk= ExtraTrkIter()){    
	if(candtrk->overlaps(*candEvt)||candtrk->overlaps(*recoDs))continue;
	if(!DRecoDsToKKpiExtraTrkList->hasMember(candtrk))
	  *DRecoDsToKKpiExtraTrkList+=new BtaCandidate(*candtrk);
	NExtraTrk++;
      }
      if(NExtraTrk>NMaxExtraTrk.value())continue;
      _scalers.sum("extra trk cut");

      //extra neutrals
      int NExtraNeutral=0;
      float ExtraNeutralE=0.;
      HepAListIterator< BtaCandidate > ExtraNeutralIter(*EvtExtraNeutralList);    
      while (BtaCandidate * candphoton = ExtraNeutralIter()){    
	if(candphoton->overlaps(*candEvt))continue;
	if(!DRecoDsToKKpiExtraGamList->hasMember(candphoton))
	  *DRecoDsToKKpiExtraGamList+=new BtaCandidate(*candphoton);
	NExtraNeutral++;
	ExtraNeutralE+=candphoton->energy();
      }  
      if(ExtraNeutralE>MaxExtraEnergy.value())continue;
      _scalers.sum("extra energy cut");
      
      if(CutSelectedCands.value())
	if(SelectedDsCandList.hasMember(recoDs))continue;
      _scalers.sum("already selected Ds cut");

      KKpiDsStMass=recoDsSt.mass();
      KKpiDeltaM=recoDsSt.mass()-recoDs->mass();
      KKpiNExtraTrk=NExtraTrk;
      KKpiNExtraNeutral=NExtraNeutral;
      KKpiExtraNeutralE=ExtraNeutralE;
	      
      //add to selected list
      SelectedDsCandList+=recoDs;
      
      ///fill mass
      KKpiMass=recoDs->mass(); 
      
      //fill  p*
      BtaCandidate boostDs=cmbooster.boostTo(*recoDs);
      KKpiPstar= boostDs.p();

      //fill angle to Tag
      BtaCandidate boostTag=cmbooster.boostTo(*candTag);            
      KKpiAngle=(boostDs.p3().unit())*(boostTag.p3().unit());

      //Get the dalitz variables
      //first have to refit the Ds with mass constrain
      BtaCandidate fitrecoDs=*recoDs;
      setMassConstraint(fitrecoDs);
      setBeamConstraint(fitrecoDs,eventInfo) ; 
      VtxFitterOper fitter(fitrecoDs,VtxAbsAlgorithm::TreeFitter) ;
      fitter.invalidateFit();
      fitter.fitAll();
      fitrecoDs=fitter.getFittedTree();
      //if((fitrecoDs.decayVtx())->status()!=BtaAbsVertex::Success)continue;
      _scalers.sum("Ds dalitz fit-succes");	      
      const BtaCandidate*candKplus=fitrecoDs.getDaughter(Pdt::lookup( PdtPdg::K_plus)->lundId());
      const BtaCandidate*candKminus=fitrecoDs.getDaughter(Pdt::lookup( PdtPdg::K_minus)->lundId());
      const BtaCandidate*candPi=fitrecoDs.getDaughter((recoDs->charge()>0) 
						      ? Pdt::lookup( PdtPdg::pi_plus)->lundId()
						      : Pdt::lookup( PdtPdg::pi_minus)->lundId());      
      KKMass=(candKplus->p4()+candKminus->p4())*(candKplus->p4()+candKminus->p4());
      KpiMass=(recoDs->charge()>0) ? (candKminus->p4()+candPi->p4())*(candKminus->p4()+candPi->p4())
	: (candKplus->p4()+candPi->p4())*(candKplus->p4()+candPi->p4());
             

      //truth match
      KKpiDsMCIdx=-1;
      KKpiDsStMCIdx=-1;   
      if(_truthMap&&_mcList){     
	//check that the tracks are true
	BtaCandidate* candKplustrue =_truthMap->mcFromReco(candKplus);
	if(candKplustrue)
	  if(candKplustrue->pdtEntry()->lundId()!=candKplus->pdtEntry()->lundId()) candKplustrue=NULL;
	BtaCandidate* candKminustrue =_truthMap->mcFromReco(candKminus);
	if(candKminustrue)
	  if(candKminustrue->pdtEntry()->lundId()!=candKminus->pdtEntry()->lundId()) candKminustrue=NULL;
	BtaCandidate* candPitrue =_truthMap->mcFromReco(candPi);
	if(candPitrue)
	  if(candPitrue->pdtEntry()->lundId()!=candPi->pdtEntry()->lundId()) candPitrue=NULL;

	//check that the tracks have same Ds mother
	if(candKplustrue&&candKminustrue&&candPitrue){
	  const BtaCandidate * KKpiMother=candKplustrue->theMother();
	  if(abs(KKpiMother->pdtEntry()->lundId())!=abs(Pdt::lookup(PdtPdg::D_s_plus)->lundId()))
	    KKpiMother=KKpiMother->theMother();//try again due to resonances
	  if(abs(KKpiMother->pdtEntry()->lundId())!=abs(Pdt::lookup(PdtPdg::D_s_plus)->lundId()))
	    KKpiMother=NULL;//bad mother
	    
	  if(KKpiMother!=candKminustrue->theMother() && KKpiMother!=candKminustrue->theMother()->theMother()) KKpiMother=NULL;
	  if(KKpiMother!=candPitrue->theMother() && KKpiMother!=candPitrue->theMother()->theMother()) KKpiMother=NULL;     
 
	  if(KKpiMother){
	    KKpiDsMCIdx=_mcList->index(BtuUtil::matchUid(_mcList,KKpiMother));

	    BtaCandidate* candGamtrue =_truthMap->mcFromReco(candGam);
	    if(candGamtrue)
	      if(candGamtrue->pdtEntry()->lundId()!=candGam->pdtEntry()->lundId())candGamtrue=NULL;
	  
	    if(candGamtrue){
	      const BtaCandidate * KKpiGamMother=KKpiMother->theMother();
	      if(abs(KKpiGamMother->pdtEntry()->lundId())!=abs(Pdt::lookup(PdtPdg::D_s_star_plus)->lundId()))
		KKpiGamMother=NULL;//bad mother
	      if(KKpiGamMother!=candGamtrue->theMother()) KKpiGamMother=NULL;
	      if(KKpiGamMother)
		KKpiDsStMCIdx=_mcList->index(BtuUtil::matchUid(_mcList,KKpiGamMother));
	    }	    	    	  
	  }
	}
	
      }


   
      ///
      DRecoDsToKKpiUsrBlock.put(*candEvt,KKpiMass); 
      DRecoDsToKKpiUsrBlock.put(*candEvt,KKpiDeltaM);
      DRecoDsToKKpiUsrBlock.put(*candEvt,KKpiDsStMass);
      DRecoDsToKKpiUsrBlock.put(*candEvt,KKpiPstar);
      DRecoDsToKKpiUsrBlock.put(*candEvt,KKpiAngle);         
      DRecoDsToKKpiUsrBlock.put(*candEvt,KKMass);  
      DRecoDsToKKpiUsrBlock.put(*candEvt,KpiMass);  
      DRecoDsToKKpiUsrBlock.put(*candEvt,KKpiNExtraTrk);
      DRecoDsToKKpiUsrBlock.put(*candEvt,KKpiNExtraNeutral);
      DRecoDsToKKpiUsrBlock.put(*candEvt,KKpiExtraNeutralE);
      DRecoDsToKKpiUsrBlock.put(*candEvt,KKpiDsMCIdx);
      DRecoDsToKKpiUsrBlock.put(*candEvt,KKpiDsStMCIdx);

      *DRecoDsToKKpiList += new BtaCandidate(*candEvt);
      _scalers.sum("final Ups");
      
    }


  }
  
  //clean up old lists
  HepAListDeleteAll(TempKList);
  HepAListDeleteAll(TempPiList);
  HepAListDeleteAll(TempEvtCandList);
  HepAListDeleteAll(TempDsCandList);

  return AppResult::OK;
}



AppResult DRecoDsToKKpi::endJob( AbsEvent* anEvent )
{
  ostream& o=cout;
  _scalers.print(o);
  o << endl;

  return AppResult::OK;
}




