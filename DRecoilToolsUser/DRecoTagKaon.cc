
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "DRecoTagKaon.hh"

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

using std::cout;
using std::endl;
using std::ostream;

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//----------------
// Constructors --
//----------------
DRecoTagKaon::DRecoTagKaon( const char* const theName,const char* const theDescription )
  : TagFilterModule( theName, theDescription )
  , KList( "KList", this, "KLHLoose" )
  , K0List( "K0List", this, "0" )
  , KaonPstarCutLo( "KaonPstarCutLo", this, .5)
  , KaonPstarCutHi( "KaonPstarCutHi", this, 2.)
  , ProtonList("ProtonList",this,"pKMSuperLoose")
  , _scalers("DRecoTagKaon")
{
  commands()->append( &KList);
  commands()->append( &K0List);
  commands()->append( &KaonPstarCutLo);
  commands()->append( &KaonPstarCutHi);
  commands()->append( &ProtonList);
}

//--------------
// Destructor --
//--------------
DRecoTagKaon::~DRecoTagKaon( )
{}

//--------------
// Operations --
//--------------
AppResult DRecoTagKaon::beginJob( AbsEvent* anEvent )
{ 
  return AppResult::OK;
}

AppResult DRecoTagKaon::event( AbsEvent* anEvent )
{
  _scalers.sum("# events ");
 

  HepAList< HepAList<BtaCandidate> >* DRecoEvtCandidateList = Ifd<HepAList< HepAList<BtaCandidate> > >::get(anEvent,"DRecoEvtCandidateList");  
  if(DRecoEvtCandidateList == 0)return AppResult::OK;
  if(DRecoEvtCandidateList->length()==0) return AppResult::OK;//nothing to do

  // Create 
  HepAList< HepAList<BtaCandidate> >  TempEvtCandList ;
  HepAListIterator< HepAList<BtaCandidate> > ListIter(*DRecoEvtCandidateList);
  while (HepAList<BtaCandidate> * candlist = ListIter()){
    TempEvtCandList += new HepAList<BtaCandidate>(*candlist);
  }
  HepAListDeleteAll(*DRecoEvtCandidateList);    

  // static pdtTypes
  const static PdtEntry* KaonMinusType = Pdt::lookup( PdtPdg::K_minus);
  const static PdtEntry* KaonPlusType  = Pdt::lookup( PdtPdg::K_plus);
  const static PdtEntry* K0Type    = Pdt::lookup( PdtPdg::K_S0);
 
  // pointer to the event summary info object
  HepAList< EventInfo >* eventInfoListPtr =  Ifd<HepAList< EventInfo > >::get(anEvent,"Default");
  if (eventInfoListPtr == 0){
    ErrMsg(fatal) << "Could not locate event info list in the event.This is likely to be a configuration error." << endmsg;
  } 
  EventInfo* eventInfo = eventInfoListPtr->first();
  if (eventInfo == 0) {
    ErrMsg(fatal) << "Nothing in eventInfoList...aborting" << endmsg;
  }
  BtaBooster cmbooster(eventInfo->cmFrame());


  //get the input kaon list
  HepAList<BtaCandidate>* EventKList = Ifd<HepAList< BtaCandidate > >::get(anEvent, KList.value());
  if (EventKList == 0) ErrMsg(fatal) << KList.value() << " list not found" << endmsg;    
  HepAListIterator<BtaCandidate> KaonIter(*EventKList);
  HepAList<BtaCandidate> TempKaonList;
  while (BtaCandidate* kaoncand = KaonIter()){
    BtaCandidate* newcand = new BtaCandidate(*kaoncand);
    newcand->setType((newcand->charge()>0) ? KaonPlusType:KaonMinusType);  
    TempKaonList+=newcand;
    _scalers.sum("#K");
  }
  //get the input K0
  if(K0List.valueString()!="0"){
    HepAList<BtaCandidate>* EventK0List = Ifd<HepAList< BtaCandidate > >::get(anEvent, K0List.value());
    if (EventK0List == 0)  ErrMsg(fatal) << K0List.value() << " list not found" << endmsg;    
    
    HepAListIterator<BtaCandidate> K0Iter(*EventK0List);
    while (BtaCandidate* kaoncand = K0Iter()){  
      BtaCandidate* newcand = new BtaCandidate(*kaoncand);
      newcand->setType(K0Type);
      TempKaonList+=newcand;
      _scalers.sum("#K0");	
    }
  }

  //get the input Protons and create a PK cand list
  HepAList<BtaCandidate>* EventProtonList = Ifd<HepAList< BtaCandidate > >::get(anEvent, ProtonList.value());
  if (EventProtonList == 0) ErrMsg(fatal) << ProtonList.value() << " list not found" << endmsg;    
  HepAListIterator<BtaCandidate> ProtonIter(*EventProtonList);
  HepAList<BtaCandidate> TempPKList;//combine p K candidates into one because DRecoCheckCharge relies on have 4 list members. 
  while (BtaCandidate* pcand = ProtonIter()){
    HepAListIterator<BtaCandidate> TempKIter(TempKaonList);
    while (BtaCandidate* kcand = TempKIter()){
      BtaOpMakeTree combPKCand;
      BtaCandidate* newPKcand = combPKCand.create(*pcand,*kcand);//this order is required by subsequent code 
      if(pcand->charge()==+1.&&kcand->charge()==-1.)	
	newPKcand->setType(Pdt::lookup( PdtPdg::Xi_b0));  
      else if(pcand->charge()==-1.&&kcand->charge()==+1.)	
	newPKcand->setType(Pdt::lookup( PdtPdg::anti_Xi_b0));  
      else newPKcand->setType((newPKcand->charge()>0) ? Pdt::lookup( PdtPdg::anti_Xi_b_plus) : Pdt::lookup( PdtPdg::Xi_b_minus));
      //would like to use Xi_c+ but there is a bug it is switched with Xi'_c+

      TempPKList+=newPKcand;
    }
    _scalers.sum("#P");
  }
  

  //add flavor balancing particle
  HepAListIterator< HepAList<BtaCandidate> > TempListIter(TempEvtCandList);
  while (HepAList<BtaCandidate> * candlist = TempListIter()){    
    _scalers.sum("initial lists");

    HepAListIterator<BtaCandidate> TagDIter(*candlist);
    BtaCandidate* tagd=TagDIter();//should be the first one
    
    if(abs(tagd->pdtEntry()->lundId())==abs(Pdt::lookup( PdtPdg::D0)->lundId())
       ||abs(tagd->pdtEntry()->lundId())==abs(Pdt::lookup( PdtPdg::D_plus)->lundId())
       ||abs(tagd->pdtEntry()->lundId())==abs(Pdt::lookup( PdtPdg::D_star_minus)->lundId())
       ||abs(tagd->pdtEntry()->lundId())==abs(Pdt::lookup( PdtPdg::D_star0)->lundId())
       ){
      HepAListIterator<BtaCandidate> KaonIter(TempKaonList);
      while (BtaCandidate* kaoncand = KaonIter()){     
	if(kaoncand->overlaps(*candlist))continue;        

	HepAList<BtaCandidate>* newcandlist= new HepAList<BtaCandidate>;    
	HepAListIterator< BtaCandidate > candIter(*candlist);
	while (BtaCandidate * cand = candIter())
	  *newcandlist+=new BtaCandidate(*cand);
	*newcandlist  += new BtaCandidate(*kaoncand);
	*DRecoEvtCandidateList += newcandlist;
	_scalers.sum("final lists");
      
      }
    }else if(abs(tagd->pdtEntry()->lundId())==abs(Pdt::lookup( PdtPdg::D_s_plus)->lundId())){
      //no K required -->add a dummy candidate to keep code similar 
      HepAList<BtaCandidate>* newcandlist= new HepAList<BtaCandidate>;    
      HepAListIterator< BtaCandidate > candIter(*candlist);
      while (BtaCandidate * cand = candIter())
	*newcandlist+=new BtaCandidate(*cand);
      BtaCandidate* dummycand=new BtaCandidate(HepLorentzVector(0.,0.,0.,0.));
      dummycand->setType(Pdt::lookup( PdtPdg::gamma ) ); 
      *newcandlist+=dummycand;
      *DRecoEvtCandidateList += newcandlist;
      _scalers.sum("final lists"); 

    }else if(abs(tagd->pdtEntry()->lundId())==abs(Pdt::lookup( PdtPdg::Lambda_c_plus)->lundId())){
      //Proton required !!!!! still need to add Kaon
      HepAListIterator<BtaCandidate> PKIter(TempPKList);
      while (BtaCandidate* pkcand = PKIter()){
	if(pkcand->overlaps(*candlist))continue;        

	HepAList<BtaCandidate>* newcandlist= new HepAList<BtaCandidate>;    
	HepAListIterator< BtaCandidate > candIter(*candlist);
	while (BtaCandidate * cand = candIter())
	  *newcandlist+=new BtaCandidate(*cand);
	*newcandlist  += new BtaCandidate(*pkcand);
	*DRecoEvtCandidateList += newcandlist;
	_scalers.sum("final lists");
      
      }

    }else ErrMsg(fatal) <<"DRecoDstarTag:  Tag-D type not recognized." << endmsg; 
    
    HepAListDeleteAll(*candlist);
  }   
 

  HepAListDeleteAll(TempKaonList);
  HepAListDeleteAll(TempEvtCandList);
  HepAListDeleteAll(TempPKList);
  return AppResult::OK;
}


AppResult DRecoTagKaon::endJob( AbsEvent* anEvent )
{
  ostream& o=cout;
  _scalers.print(o);
  o << endl;

  return AppResult::OK;
}




