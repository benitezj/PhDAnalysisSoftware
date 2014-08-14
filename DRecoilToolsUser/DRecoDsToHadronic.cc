#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "DRecoDsToHadronic.hh"

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
#include "BetaCoreTools/BtaBooster.hh"

#include "TrkBase/TrkRecoTrk.hh"

#include "BetaCoreTools/BtaMcAssoc.hh"
#include "BtaTupleMaker/BtuUtil.hh"

#include "BetaCoreTools/BtaPrintTree.hh"

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
DRecoDsToHadronic::DRecoDsToHadronic( const char* const theName,const char* const theDescription )
  : TagFilterModule( theName, theDescription )
  , FitProbCut("FitProbCut",this,.001)
  , DsHadronicList("DsHadronicList",this,"0")
  , DsHadronicMassCutLo("DsHadronicMassCutLo",this,1.797)
  , DsHadronicMassCutHi("DsHadronicMassCutHi",this,2.197)
  , DsHadronicPstarCut("DsHadronicPstarCut",this,2.5)
  , DeltaMCut("DeltaMCut",this,.04)
  , DeltaMMean("DeltaMMean",this,.1439)
  , RequireNoOverlap("RequireNoOverlap",this,0)
  , _scalers("DRecoDsToHadronic")
  , UsrBlockName("UsrBlockName",this,"DsToHadUsrBlock")
  , DsPx("DsPx",0.)
  , DsPy("DsPy",0.)
  , DsPz("DsPz",0.)
  , DsE("DsE",0.)
  , DsCharge("DsCharge",0.)
  , GamPx("GamPx",0.)
  , GamPy("GamPy",0.)
  , GamPz("GamPz",0.)
{

  commands()->append(&FitProbCut);
  commands()->append(&DsHadronicList);
  commands()->append(&DsHadronicMassCutLo);
  commands()->append(&DsHadronicMassCutHi);
  commands()->append(&DsHadronicPstarCut);
  commands()->append(&DeltaMCut);
  commands()->append(&DeltaMMean);
  commands()->append(&RequireNoOverlap);

  UsrBlock.addVariable(DsPx); 
  UsrBlock.addVariable(DsPy);
  UsrBlock.addVariable(DsPz); 
  UsrBlock.addVariable(DsE);  
  UsrBlock.addVariable(DsCharge);
  UsrBlock.addVariable(GamPx); 
  UsrBlock.addVariable(GamPy);
  UsrBlock.addVariable(GamPz);  
  
}

//--------------
// Destructor --
//--------------
DRecoDsToHadronic::~DRecoDsToHadronic()
{}

//--------------
// Operations --
//--------------
AppResult DRecoDsToHadronic::beginJob( AbsEvent* anEvent )
{ 
  return AppResult::OK;
}

AppResult DRecoDsToHadronic::event( AbsEvent* anEvent )
{
  _scalers.sum("#  events ") ;
  
  // Create evt list and add to event
  HepAList<BtaCandidate>* DsHadronicOutList = new HepAList<BtaCandidate> ;
  if(DsHadronicOutList==0) ErrMsg(fatal) << "Failed to create an DsHadronicOutList list" << endmsg ;

  // Create proxy to dump Ups_out list
  IfdHepAListProxy<BtaCandidate>* proxy = new IfdHepAListProxy<BtaCandidate>(DsHadronicOutList) ;
  if(proxy == 0) ErrMsg(fatal) << "Failed to create DsHadronicOutList list proxy" << endmsg ;
  if(!Ifd<HepAList<BtaCandidate> >::put(anEvent, proxy,"DsToHadronicCandidateList")) 
    ErrMsg(fatal) << "failed to add DsToHadronicCandidateList  list to event using proxy" << endmsg ; 

  //add Usr block to event
  UsrIfd<UsrCandBlock>::put(anEvent,&UsrBlock,UsrBlockName.value());
  

  // Retreive the event candidates
  HepAList< BtaCandidate >* DRecoEvtCandidateList = Ifd<HepAList<BtaCandidate > >::get(anEvent,"DRecoCandidateList") ;
  if(DRecoEvtCandidateList==0)  ErrMsg(fatal) <<"DRecoCandidateList not found" << endmsg ;
  if(DRecoEvtCandidateList->length()==0)return AppResult::OK ;
  
  // Pointer to the event summary info object
  HepAList< EventInfo >* eventInfoListPtr =  Ifd<HepAList< EventInfo > >::get(anEvent,"Default") ;
  if(eventInfoListPtr==0) ErrMsg(fatal) << "Could not locate event info list in the event." << endmsg ;
  EventInfo* eventInfo = eventInfoListPtr->first() ;
  if(eventInfo==0) ErrMsg(fatal) << "Nothing in eventInfoList...aborting" << endmsg ;
  const BbrPointErr beamSpot = eventInfoListPtr->first()->beamSpot() ;
  //cm booster
  BtaBooster cmbooster(eventInfo->cmFrame());

  //  
  HepAList< BtaCandidate > TmpEvtDsHadronicList;
  HepAList< BtaCandidate >* EvtDsHadronicList = Ifd<HepAList<BtaCandidate > >::get(anEvent,DsHadronicList.value()) ;
  if(EvtDsHadronicList==0)  ErrMsg(fatal) <<DsHadronicList.value()<<" not found" << endmsg ;
  HepAListIterator<BtaCandidate> DsIter(*EvtDsHadronicList) ; 
  while(BtaCandidate* DsCand = DsIter()){
    _scalers.sum("total Ds") ;	   
    if(DsCand->mass()<DsHadronicMassCutLo.value()-.05||DsHadronicMassCutHi.value()+.05<DsCand->mass())continue;
    _scalers.sum("pre mass cut") ;	   
    BtaCandidate DsCandBoost=cmbooster.boostTo(*DsCand);
    if(DsCandBoost.p()<DsHadronicPstarCut.value()-.1)continue;
    _scalers.sum("pre p* cut") ;	   
    TmpEvtDsHadronicList+=new BtaCandidate(*DsCand);
  }
  

  //
  HepAListIterator<BtaCandidate> EvtIter(*DRecoEvtCandidateList) ; 
  while(BtaCandidate* EvtCand= EvtIter()){
    _scalers.sum("initial lists") ;	     
    
    HepAListIterator<BtaCandidate> DsIter(TmpEvtDsHadronicList) ; 
    while(BtaCandidate* DsCand = DsIter()){
      _scalers.sum("Total") ;
      if(RequireNoOverlap.value())
	if(DsCand->overlaps(*EvtCand))continue;
      _scalers.sum(" no overlap") ;

  
      const BtaCandidate* DsSt=EvtCand->getDaughter(Pdt::lookup(PdtPdg::D_s_star_plus)->lundId());
      if(!DsSt)DsSt=EvtCand->getDaughter(Pdt::lookup(PdtPdg::D_s_star_minus)->lundId());
      if(!DsSt)ErrMsg(fatal) << "Ds* not found...aborting" << endmsg ;
      
      const BtaCandidate* Gamma=DsSt->getDaughter(Pdt::lookup(PdtPdg::gamma)->lundId());
      if(!Gamma)ErrMsg(fatal) << "Ds* Gamma not found...aborting" << endmsg ;
      
      const BtaCandidate* DsRecoil=DsSt->getDaughter(DsSt->charge()>0. ? Pdt::lookup(PdtPdg::D_s_plus)->lundId() :  Pdt::lookup(PdtPdg::D_s_minus)->lundId());
      if(!DsRecoil)ErrMsg(fatal) << "Ds* Ds not found...aborting" << endmsg ;


//       BtaOpMakeTree combDsStCand;
//       BtaCandidate candDsSt=combDsStCand.combine(*DsCand,*Gamma);
//       candDsSt.setType(DsCand->charge() > 0. ?Pdt::lookup(PdtLund::D_s_star_plus) : Pdt::lookup(PdtLund::D_s_star_minus) ) ;
//      setBeamConstraint(candDsSt,eventInfo) ;
      setBeamConstraint(*DsCand,eventInfo) ;
      VtxFitterOper fitter(*DsCand,VtxAbsAlgorithm::TreeFitter) ;
      fitter.invalidateFit() ;
      fitter.fitAll() ;
	
      BtaCandidate fitCand=fitter.getFittedTree();
      // Extract 	
      if((fitCand.decayVtx())->status()!=BtaAbsVertex::Success)
	continue ;
      _scalers.sum("fit-success") ;
	
      if(probab((int)fitCand.decayVtx()->nDof(),fitCand.decayVtx()->chiSquared()) < FitProbCut.value())
	continue;
      _scalers.sum("fit-prob") ;

//       const BtaCandidate* fitDsCand=fitCand.getDaughter(fitCand.charge()>0. ? Pdt::lookup(PdtPdg::D_s_plus)->lundId() :  Pdt::lookup(PdtPdg::D_s_minus)->lundId());
      
      if(fitCand.mass()<DsHadronicMassCutLo.value()||DsHadronicMassCutHi.value()<fitCand.mass())continue;
      _scalers.sum("mass cut") ;

      //if(fabs(fitCand.mass()-fitDsCand->mass()-DeltaMMean.value())>DeltaMCut.value())continue;
      //_scalers.sum("deltaM cut") ;

      BtaCandidate DsCandBoost=cmbooster.boostTo(fitCand);
      if(DsCandBoost.p()<DsHadronicPstarCut.value())continue;
      _scalers.sum("p* cut") ;	   
      
      
      DsPx=DsRecoil->p4().x();
      DsPy=DsRecoil->p4().y();
      DsPz=DsRecoil->p4().z();
      DsE=DsRecoil->p4().t();
      DsCharge=DsRecoil->charge();
      GamPx=Gamma->p4().x();
      GamPy=Gamma->p4().y();
      GamPz=Gamma->p4().z();
      UsrBlock.put(fitCand,DsPx);
      UsrBlock.put(fitCand,DsPy);
      UsrBlock.put(fitCand,DsPz);
      UsrBlock.put(fitCand,DsE);
      UsrBlock.put(fitCand,DsCharge);
      UsrBlock.put(fitCand,GamPx);
      UsrBlock.put(fitCand,GamPy);
      UsrBlock.put(fitCand,GamPz);
 
      *DsHadronicOutList+=new BtaCandidate(fitCand);
      _scalers.sum("final") ;	   
      
    }
  }

  HepAListDeleteAll(TmpEvtDsHadronicList);

  
  return AppResult::OK ;
}

AppResult DRecoDsToHadronic::endJob( AbsEvent* anEvent )
{
  ostream& o=cout ;
  _scalers.print(o) ;
  o << endl ;

  return AppResult::OK ;
}

