
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "DRecoDstarTag.hh"

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
#include "CLHEP/Alist/AList.h"
#include "CLHEP/Alist/AIterator.h"
#include "ErrLogger/ErrLog.hh"
#include "PDT/Pdt.hh"
#include "ProxyDict/IfdStrKey.hh"
#include "ProxyDict/IfdKey.hh"
#include "ProxyDict/Ifd.hh"
#include "ProxyDict/IfdHepAListProxy.hh"
#include "BetaCoreTools/BtaBooster.hh"
#include "BetaCoreTools/BtaOpMakeTree.hh"
#include "VtxBase/BtaAbsVertexer.hh"
#include "VtxFitter/VtxFitterOper.hh"
#include "VtxBase/VtxAbsAlgorithm.hh"
#include "Beta/BtaAbsVertex.hh"
#include "ProbTools/probab.hh"


using std::cout;
using std::endl;
using std::ostream;

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//----------------
// Constructors --
//----------------
DRecoDstarTag::DRecoDstarTag( const char* const theName,const char* const theDescription )
  : TagFilterModule( theName, theDescription )
  , PiList( "PiList", this, "GoodTracksVeryLoose" )
  , Pi0List( "Pi0List", this, "pi0DefaultMass" )
  , GammaList( "GammaList", this, "GoodPhotonLoose" )
  , D0PiDeltaMMean("D0PiDeltaMMean",this,.14542)
  , D0Pi0DeltaMMean("D0Pi0DeltaMMean",this,.14212)
  , D0GamDeltaMMean("D0GamDeltaMMean",this,.141)
  , DPlusPi0DeltaMMean("DPlusPi0DeltaMMean",this,.14064)
  , D0PiDeltaMCut("D0PiDeltaMCut",this,.0015)
  , D0Pi0DeltaMCut("D0Pi0DeltaMCut",this,.003)
  , D0GamDeltaMCut("D0GamDeltaMCut",this,.01)
  , DPlusPi0DeltaMCut("DPlusPi0DeltaMCut",this,.003)
  , Pi0GamEnergyCut("Pi0GamEnergyCut",this,.03)
  , GamEnergyCut("GamEnergyCut",this,.2)//note that D* optimization study showed a 250MeV flat cut is needed to give equal purity as the D but here I apply an angle dependent cut so this cut has been lowered.
  , GammaPi0VetoList("GammaPi0VetoList",this,"pi0DefaultMass")
  , GammaEtaVetoList("GammaEtaVetoList",this,"etaggDefaultNoMass")
  , VtxProbCut("VtxProbCut",this,.00001)
  , _scalers("DRecoDstarTag")
{
  commands()->append( &PiList);
  commands()->append( &Pi0List);
  commands()->append( &GammaList);
  commands()->append( &D0PiDeltaMMean);
  commands()->append( &D0Pi0DeltaMMean);
  commands()->append( &D0GamDeltaMMean);
  commands()->append( &DPlusPi0DeltaMMean);
  commands()->append( &D0PiDeltaMCut);
  commands()->append( &D0Pi0DeltaMCut);
  commands()->append( &D0GamDeltaMCut);
  commands()->append( &DPlusPi0DeltaMCut);
  commands()->append( &Pi0GamEnergyCut);
  commands()->append( &GamEnergyCut);
  commands()->append( &GammaPi0VetoList);
  commands()->append( &GammaEtaVetoList);
  commands()->append( &VtxProbCut);
}

//--------------
// Destructor --
//--------------
DRecoDstarTag::~DRecoDstarTag( )
{}

//--------------
// Operations --
//--------------
AppResult DRecoDstarTag::beginJob( AbsEvent* anEvent )
{ 
  return AppResult::OK;
}

AppResult DRecoDstarTag::event( AbsEvent* anEvent )
{
  _scalers.sum("# events ");
 

  HepAList< HepAList<BtaCandidate> >* DRecoEvtCandidateList = Ifd<HepAList< HepAList<BtaCandidate> > >::get(anEvent,"DRecoEvtCandidateList");  
  if(DRecoEvtCandidateList == 0)return AppResult::OK;
  if(DRecoEvtCandidateList->length()==0) return AppResult::OK;//nothing to do

  
  // pointer to the event summary info object
  HepAList< EventInfo >* eventInfoListPtr =  Ifd<HepAList< EventInfo > >::get(anEvent,"Default");
  if (eventInfoListPtr == 0){
    ErrMsg(fatal) << "Could not locate event info list in the event.This is likely to be a configuration error." << endmsg;
  } 
  EventInfo* eventInfo = eventInfoListPtr->first();
  if (eventInfo == 0) {
    ErrMsg(fatal) << "Nothing in eventInfoList...aborting" << endmsg;
  }


  //get the input pi list
  HepAList<BtaCandidate>  TempPiList ;
  HepAList<BtaCandidate>* EventPiList  = Ifd<HepAList< BtaCandidate > >::get(anEvent, PiList.value());
  if (EventPiList == 0)  ErrMsg(fatal) << PiList.value() << " list not found" << endmsg;    
  HepAListIterator<BtaCandidate> PiIter(*EventPiList);
  while (BtaCandidate* cand = PiIter()){
    _scalers.sum("total pi's");

    BtaCandidate* newcand = new BtaCandidate(*cand);
    newcand->setType((newcand->charge()>0)? Pdt::lookup( PdtPdg::pi_plus) : Pdt::lookup( PdtPdg::pi_minus));
    TempPiList +=  newcand;         
  }

  //get the input pi0 list
  HepAList<BtaCandidate>  TempPi0List ;
  HepAList<BtaCandidate>* EventPi0List  = Ifd<HepAList< BtaCandidate > >::get(anEvent, Pi0List.value());
  if (EventPi0List == 0)  ErrMsg(fatal) << Pi0List.value() << " list not found" << endmsg;  
  HepAListIterator<BtaCandidate> Pi0Iter(*EventPi0List);
  while (BtaCandidate* cand = Pi0Iter()){
    _scalers.sum("initial pi0's");      

    //get the gammas
    HepAListIterator<BtaCandidate> Pi0GamIter=cand->daughterIterator();
    BtaCandidate* gamma1=Pi0GamIter();        
    BtaCandidate* gamma2=Pi0GamIter();    
    //angle dependent min cut
    //Note that here I deviate from the angle dependent cut used everywhere else.
    //a looser cut is applied here because these D* modes are clean
    float angledependentcut=Pi0GamEnergyCut.value();
    //cut gamma 1
    float cosine=gamma1->p3().z()/gamma1->p();
    if(cosine>.5)angledependentcut=Pi0GamEnergyCut.value()+.05*(cosine-.5)/.5;
    if(gamma1->energy()<angledependentcut) continue; 
    //cut gamma 2
    angledependentcut=Pi0GamEnergyCut.value();
    cosine=gamma2->p3().z()/gamma2->p();
    if(cosine>.5)angledependentcut=Pi0GamEnergyCut.value()+.05*(cosine-.5)/.5;
    if(gamma2->energy()<angledependentcut) continue;         
    _scalers.sum("pi0's after gamma energy cut");

           
    _scalers.sum("final pi0's"); 
    BtaCandidate* newcand = new BtaCandidate(*cand);
    newcand->removeConstraint(BtaConstraint::Beam);//remove constriant to primVtx
    newcand->setType(Pdt::lookup( PdtPdg::pi0) );
    TempPi0List +=  newcand;           
  }
  

  //get the input gamma list
  HepAList<BtaCandidate>  TempGammaList ;
  HepAList<BtaCandidate>* EventGammaList  = Ifd<HepAList< BtaCandidate > >::get(anEvent, GammaList.value());
  if (EventGammaList == 0)  ErrMsg(fatal) << GammaList.value() << " list not found" << endmsg;  
  
  HepAList<BtaCandidate>* EventGammaPi0VetoList  = Ifd<HepAList< BtaCandidate > >::get(anEvent, GammaPi0VetoList.value());
  HepAList<BtaCandidate>* EventGammaEtaVetoList  = Ifd<HepAList< BtaCandidate > >::get(anEvent, GammaEtaVetoList.value());

  HepAListIterator<BtaCandidate> GammaIter(*EventGammaList);
  while (BtaCandidate* cand = GammaIter()){
    _scalers.sum("initial gamma's");      
  
    if(EventGammaPi0VetoList)
      if(cand->overlaps(*EventGammaPi0VetoList))continue;
    if(EventGammaEtaVetoList)
      if(cand->overlaps(*EventGammaEtaVetoList))continue;
    _scalers.sum("gamma's after pi0 and eta veto cut");   

    //angle dependent min cut
    float angledependentcut=GamEnergyCut.value();
    float cosine=cand->p3().z()/cand->p();
    if(cosine>.5)angledependentcut=GamEnergyCut.value()+.1*(cosine-.5)/.5;
    if(cand->energy()<angledependentcut) continue; 
    _scalers.sum("gammas after gamma energy cut");

    _scalers.sum("final gamma's"); 
    BtaCandidate* newcand = new BtaCandidate(*cand);
    newcand->removeConstraint(BtaConstraint::Beam);//remove constriant to primVtx
    newcand->setType(Pdt::lookup( PdtPdg::gamma) );
    TempGammaList +=  newcand;           
  }
  
  
  HepAListIterator< HepAList<BtaCandidate> > ListIter(*DRecoEvtCandidateList);
  while (HepAList<BtaCandidate> * candlist = ListIter()){    
    _scalers.sum("initial lists");

    HepAListIterator<BtaCandidate> TagDIter(*candlist);
    BtaCandidate* tagd=TagDIter();//should be the first one    
    BtaCandidate* DstarTag=NULL;

    if(abs(tagd->pdtEntry()->lundId())==abs(Pdt::lookup( PdtPdg::D0)->lundId())){
      //if tag is D0 it may come from D*+-->D0pi+, D*0-->D0pi0 or D*0-->D0 gamma
      _scalers.sum("D0's");

      //look for its charged Kaon to tell the flavor
      float charge=0.;
      HepAListIterator<BtaCandidate> D0DauIter=tagd->daughterIterator(); 
      while(BtaCandidate*dau=D0DauIter())
	if(abs(dau->pdtEntry()->lundId())==abs(Pdt::lookup(PdtPdg::K_plus)->lundId()))
	  charge=dau->charge();

      
      HepAListIterator<BtaCandidate> PiIter(TempPiList);
      while (BtaCandidate* cand = PiIter()){
	if(DstarTag)continue;
	if(charge*cand->charge()!=-1.&&charge!=0.)continue;
	BtaOpMakeTree combCand;
	BtaCandidate candDst= combCand.combine(*tagd,*cand);
	if(fabs(candDst.mass()-tagd->mass()-D0PiDeltaMMean.value())>5.*D0PiDeltaMCut.value())continue;
	candDst.setType(cand->charge()>0 ? Pdt::lookup( PdtPdg::D_star_plus): Pdt::lookup( PdtPdg::D_star_minus));
	setBeamConstraint(candDst,eventInfo) ;
	VtxFitterOper fitter(candDst,VtxAbsAlgorithm::TreeFitter) ;
	fitter.invalidateFit();
	fitter.fitAll();
	candDst=fitter.getFittedTree();      
	if((candDst.decayVtx())->status()!=BtaAbsVertex::Success)continue;
	if(probab((int)candDst.decayVtx()->nDof(),candDst.decayVtx()->chiSquared()) < VtxProbCut.value()) continue;
	const BtaCandidate* Ddau=candDst.getDaughter(tagd->pdtEntry()->lundId());
	if(fabs(candDst.mass()-Ddau->mass()-D0PiDeltaMMean.value())>D0PiDeltaMCut.value())continue;
      	DstarTag=new BtaCandidate(candDst);
	_scalers.sum("D*'s D0pi+");
      }
      
      HepAListIterator<BtaCandidate> Pi0Iter(TempPi0List);
      while (BtaCandidate* cand = Pi0Iter()){
	if(DstarTag)continue;
	BtaOpMakeTree combCand;
	BtaCandidate candDst= combCand.combine(*tagd,*cand);
	if(fabs(candDst.mass()-tagd->mass()-D0Pi0DeltaMMean.value())>5.*D0Pi0DeltaMCut.value())continue;
	candDst.setType(charge==1. ? Pdt::lookup( PdtPdg::anti_D_star0) : Pdt::lookup( PdtPdg::D_star0));
	setBeamConstraint(candDst,eventInfo) ;
	VtxFitterOper fitter(candDst,VtxAbsAlgorithm::TreeFitter) ;
	fitter.invalidateFit();
	fitter.fitAll();
	candDst=fitter.getFittedTree();      
	if((candDst.decayVtx())->status()!=BtaAbsVertex::Success)continue;
	if(probab((int)candDst.decayVtx()->nDof(),candDst.decayVtx()->chiSquared()) < VtxProbCut.value()) continue;
	const BtaCandidate* Ddau=candDst.getDaughter(tagd->pdtEntry()->lundId());
	if(fabs(candDst.mass()-Ddau->mass()-D0Pi0DeltaMMean.value())>D0Pi0DeltaMCut.value())continue;
      	DstarTag=new BtaCandidate(candDst);
	_scalers.sum("D*'s D0pi0");
      }
      
      
      HepAListIterator<BtaCandidate> GammaIter(TempGammaList);
      while (BtaCandidate* cand = GammaIter()){
	if(DstarTag)continue;
	BtaOpMakeTree combCand;
	BtaCandidate candDst= combCand.combine(*tagd,*cand);
	if(fabs(candDst.mass()-tagd->mass()-D0GamDeltaMMean.value())>5.*D0GamDeltaMCut.value())continue;
	candDst.setType(charge==1. ? Pdt::lookup( PdtPdg::anti_D_star0) : Pdt::lookup( PdtPdg::D_star0));
	setBeamConstraint(candDst,eventInfo) ;
	VtxFitterOper fitter(candDst,VtxAbsAlgorithm::TreeFitter) ;
	fitter.invalidateFit();
	fitter.fitAll();
	candDst=fitter.getFittedTree();      
	if((candDst.decayVtx())->status()!=BtaAbsVertex::Success)continue;
	if(probab((int)candDst.decayVtx()->nDof(),candDst.decayVtx()->chiSquared()) < VtxProbCut.value()) continue;
	const BtaCandidate* Ddau=candDst.getDaughter(tagd->pdtEntry()->lundId());
	if(fabs(candDst.mass()-Ddau->mass()-D0GamDeltaMMean.value())>D0GamDeltaMCut.value())continue;
      	DstarTag=new BtaCandidate(candDst);
	_scalers.sum("D*'s D0gamma");
      }
      

    }else if(abs(tagd->pdtEntry()->lundId())==abs(Pdt::lookup( PdtPdg::D_plus)->lundId())){
      //if tag is D+ it may come from D*+ --> D+pi0,      
      _scalers.sum("D+'s");
      HepAListIterator<BtaCandidate> Pi0Iter(TempPi0List);
      while (BtaCandidate* cand = Pi0Iter()){
	if(DstarTag)continue;
	BtaOpMakeTree combCand;
	BtaCandidate candDst= combCand.combine(*tagd,*cand);
	if(fabs(candDst.mass()-tagd->mass()-DPlusPi0DeltaMMean.value())>5.*DPlusPi0DeltaMCut.value())continue;
	candDst.setType(tagd->charge()>0 ? Pdt::lookup( PdtPdg::D_star_plus): Pdt::lookup( PdtPdg::D_star_minus));
	setBeamConstraint(candDst,eventInfo) ;
	VtxFitterOper fitter(candDst,VtxAbsAlgorithm::TreeFitter) ;
	fitter.invalidateFit();
	fitter.fitAll();
	candDst=fitter.getFittedTree();      
	if((candDst.decayVtx())->status()!=BtaAbsVertex::Success)continue;
	if(probab((int)candDst.decayVtx()->nDof(),candDst.decayVtx()->chiSquared()) < VtxProbCut.value()) continue;
	const BtaCandidate* Ddau=candDst.getDaughter(tagd->pdtEntry()->lundId());
	if(fabs(candDst.mass()-Ddau->mass()-DPlusPi0DeltaMMean.value())>DPlusPi0DeltaMCut.value())continue;
      	DstarTag=new BtaCandidate(candDst);
	_scalers.sum("D*'s D+pi0");
      }
    }else if(abs(tagd->pdtEntry()->lundId())==abs(Pdt::lookup( PdtPdg::D_s_plus)->lundId())){
      //no D* tagging 
    }else if(abs(tagd->pdtEntry()->lundId())==abs(Pdt::lookup( PdtPdg::Lambda_c_plus)->lundId())){
      //no D* tagging 
    }else ErrMsg(fatal) <<"DRecoDstarTag:  Tag-D type not recognized." << endmsg;   

    if(DstarTag){//substitute tag with new one
      //candlist->remove(tagd);
      HepAListDeleteAll(*candlist);
      *candlist+=DstarTag;
      _scalers.sum("total D*'s");
    }
        
  }   
 

  HepAListDeleteAll(TempPiList);
  HepAListDeleteAll(TempPi0List);
  HepAListDeleteAll(TempGammaList);

  return AppResult::OK;
}


AppResult DRecoDstarTag::endJob( AbsEvent* anEvent )
{
  ostream& o=cout;
  _scalers.print(o);
  o << endl;

  return AppResult::OK;
}




