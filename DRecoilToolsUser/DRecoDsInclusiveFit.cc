
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "DRecoDsInclusiveFit.hh"

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
DRecoDsInclusiveFit::DRecoDsInclusiveFit( const char* const theName,const char* const theDescription )
  : TagFilterModule( theName, theDescription )
  , DsStMassCutLo("DsStMassCutLo",this,1.812)
  , DsStMassCutHi("DsStMassCutHi",this,2.412)
  , FitEvt("FitEvt",this,1)
  , FitProbCut("FitProbCut",this,.001)
  , DsMomentumCut("DsMomentumCut",this,3.)
  , CorrDsSt("CorrDsSt",this,0)
  , MassConstrainTag("MassConstrainTag",this,1)
  , MassConstrainDsSt("MassConstrainDsSt",this,1)
  , _scalers("DRecoDsInclusiveFit")
  , DRecoUsrBlockName("DRecoUsrBlock",this,"DRecoUsrBlock")
  , TagUncVtxProb("TagUncVtxProb",1.)
  , TagUncMass("TagUncMass",0.)
  , DsStUncMass("DsStUncMass",0.)
  , DsUncMass("DsUncMass",0.)
  , MyGamMCIdx("MyGamMCIdx",-1)
  , MyTagMCIdx("MyTagMCIdx",-1)
  , MyKMCIdx("MyKMCIdx",-1)
  , MyXd1MCIdx("MyXd1MCIdx",-1)
  , MyXd2MCIdx("MyXd2MCIdx",-1)
  , MyXd3MCIdx("MyXd3MCIdx",-1)
  , MyXd4MCIdx("MyXd4MCIdx",-1)
  , MyXd5MCIdx("MyXd5MCIdx",-1)
  , MyXd6MCIdx("MyXd6MCIdx",-1)
  , MyXd7MCIdx("MyXd7MCIdx",-1)
  , MyXd8MCIdx("MyXd8MCIdx",-1)
  , MyXd9MCIdx("MyXd9MCIdx",-1)
  , MyXd10MCIdx("MyXd10MCIdx",-1)
  , MyXPi0Mass("MyXPi0Mass",0.)
  , DsStPxRaw("DsStPxRaw",0.)
  , DsStPyRaw("DsStPyRaw",0.)
  , DsStPzRaw("DsStPzRaw",0.)
  , DsStERaw("DsStERaw",0.)
  , DsStPxCorr("DsStPxCorr",0.)
  , DsStPyCorr("DsStPyCorr",0.)
  , DsStPzCorr("DsStPzCorr",0.)
  , DsStECorr("DsStECorr",0.)
  , DsMassUnFit("DsMassUnFit",0.)
{
  commands()->append( &DsStMassCutLo);
  commands()->append( &DsStMassCutHi);
  commands()->append( &FitEvt);
  commands()->append( &FitProbCut);
  commands()->append( &DsMomentumCut);
  commands()->append( &CorrDsSt);
  commands()->append( &MassConstrainTag);
  commands()->append( &MassConstrainDsSt);
  
  DRecoUsrBlock.addVariable(TagUncVtxProb);
  DRecoUsrBlock.addVariable(TagUncMass);
  DRecoUsrBlock.addVariable(DsStUncMass);
  DRecoUsrBlock.addVariable(DsUncMass);
  DRecoUsrBlock.addVariable(MyGamMCIdx);
  DRecoUsrBlock.addVariable(MyTagMCIdx);
  DRecoUsrBlock.addVariable(MyKMCIdx);
  DRecoUsrBlock.addVariable(MyXd1MCIdx);
  DRecoUsrBlock.addVariable(MyXd2MCIdx);
  DRecoUsrBlock.addVariable(MyXd3MCIdx);
  DRecoUsrBlock.addVariable(MyXd4MCIdx);
  DRecoUsrBlock.addVariable(MyXd5MCIdx);
  DRecoUsrBlock.addVariable(MyXd6MCIdx);
  DRecoUsrBlock.addVariable(MyXd7MCIdx);
  DRecoUsrBlock.addVariable(MyXd8MCIdx);
  DRecoUsrBlock.addVariable(MyXd9MCIdx);
  DRecoUsrBlock.addVariable(MyXd10MCIdx);
  DRecoUsrBlock.addVariable(MyXPi0Mass);
  DRecoUsrBlock.addVariable(DsStPxRaw);
  DRecoUsrBlock.addVariable(DsStPyRaw);
  DRecoUsrBlock.addVariable(DsStPzRaw);
  DRecoUsrBlock.addVariable(DsStERaw);
  DRecoUsrBlock.addVariable(DsStPxCorr);
  DRecoUsrBlock.addVariable(DsStPyCorr);
  DRecoUsrBlock.addVariable(DsStPzCorr);
  DRecoUsrBlock.addVariable(DsStECorr);
  DRecoUsrBlock.addVariable(DsMassUnFit);
}

//--------------
// Destructor --
//--------------
DRecoDsInclusiveFit::~DRecoDsInclusiveFit()
{}

//--------------
// Operations --
//--------------
AppResult DRecoDsInclusiveFit::beginJob( AbsEvent* anEvent )
{ 
  return AppResult::OK;
}

AppResult DRecoDsInclusiveFit::event( AbsEvent* anEvent )
{
  _scalers.sum("#  events ");
  
  UsrIfd<UsrCandBlock>::put(anEvent,&DRecoUsrBlock,DRecoUsrBlockName.value());

  //create evt list and add to event
  HepAList<BtaCandidate>* DRecoCandidateList = new HepAList<BtaCandidate>;
  if (DRecoCandidateList == 0) ErrMsg(fatal) << "Failed to create a DRecoCandidateList list"	<< endmsg;
  IfdHepAListProxy<BtaCandidate>* proxy = new IfdHepAListProxy<BtaCandidate>(DRecoCandidateList);
  if(proxy == 0) ErrMsg(fatal) << "Failed to create DRecoCandidateList list proxy" << endmsg;
  if( !Ifd<HepAList<BtaCandidate> >::put(anEvent, proxy,"DRecoCandidateList"))
    ErrMsg(fatal) << "failed to add DRecoCandidateList list to event using proxy" << endmsg; 

  //retreive the event candidates
  HepAList< HepAList<BtaCandidate> >* DRecoEvtCandidateList = Ifd<HepAList< HepAList<BtaCandidate> > >::get(anEvent,"DRecoEvtCandidateList");
  if (DRecoEvtCandidateList == 0)  ErrMsg(fatal) <<"DRecoEvtCandidateList  not found" << endmsg;   

  // Create 
  HepAList< HepAList<BtaCandidate> >  TempEvtCandList ;
  HepAListIterator< HepAList<BtaCandidate> > EvtListIter(*DRecoEvtCandidateList);
  while (HepAList<BtaCandidate> * candlist = EvtListIter()){
    TempEvtCandList += new HepAList<BtaCandidate>(*candlist);
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
  
  ///get the MC Truth info
  BtaMcAssoc* _truthMap=Ifd< BtaMcAssoc >::get( anEvent,"GHit");
  HepAList<BtaCandidate>* _mcList=Ifd< HepAList<BtaCandidate> >::get(anEvent,"MCTruth" );  

  BtaCandidate*GenGam=NULL;
  if(_truthMap&&_mcList){//truth-match if this is MC
    _mcList=BtuUtil::rearrangeMcList( Ifd< HepAList<BtaCandidate> >::get(anEvent,"MCTruth" ));    
    HepAListIterator<BtaCandidate> mcIter(*_mcList);
    while( BtaCandidate* c = mcIter()){
      //Find the Ds* 
      if(abs(c->pdtEntry()->lundId())==abs(Pdt::lookup( PdtPdg::D_s_star_plus)->lundId())){
	//get its gamma for later matching 
	HepAListIterator<BtaCandidate> DsStdauIter=c->daughterIterator();  
	while(BtaCandidate* DsStdau = DsStdauIter()){ 
	  if(abs(DsStdau->pdtEntry()->lundId())==abs(Pdt::lookup( PdtPdg::gamma)->lundId())){
	    GenGam=DsStdau;		
	  }
	}
      }
    }  
  }
  

  HepAListIterator< HepAList<BtaCandidate> > ListIter(TempEvtCandList); 
  while (HepAList<BtaCandidate> * candlist = ListIter()){     
    _scalers.sum("initial lists");     


    //cout<<"marker 1"<<endl;
    //Get the D-K-X-gamma : Note candlist MUST be consist of 4 members 
    HepAListIterator<BtaCandidate> ListIter(*candlist);  
    BtaCandidate* rectagD=ListIter();//must be first one
    const BtaCandidate* recKaon=ListIter();//must be second one
    BtaCandidate* recfrag=ListIter();//must be third one
    BtaCandidate* recgam=ListIter();//must be fourth one
    if(!rectagD||!recKaon||!recfrag||!recgam)
      ErrMsg(fatal) << "DRecoDsInclusiveFit:: Evt dau not found aborting" << endmsg;  

//     //do not allow Ds tags which could be on the signal side //only allow one 
//     if(abs(rectagD->pdtEntry()->lundId())==abs(Pdt::lookup(PdtLund::D_s_plus)->lundId())
//        &&DRecoCandidateList->length()>0) continue;
//     _scalers.sum(" Tag Ds cut");	    

    //cout<<"marker 2"<<endl;
    TagUncVtxProb=probab((int)rectagD->decayVtx()->nDof(),rectagD->decayVtx()->chiSquared());
    TagUncMass=rectagD->mass();
    if(MassConstrainTag.value()) setMassConstraint(*rectagD);
    

    //Switching the type doesnt work-->Fit prob cut removes all candidates
//     //switch type to put into Tag block; has to be allowed to fly in vertex fit    
//     if(abs(rectagD->pdtEntry()->lundId())==abs(Pdt::lookup(PdtPdg::D_s_plus)->lundId()))
//       rectagD->setType((rectagD->charge()>0) ? Pdt::lookup(PdtPdg::B_plus) : Pdt::lookup(PdtPdg::B_minus));
//     //switch type to put into Tag block; has to be allowed to fly in vertex fit    
//     if(abs(rectagD->pdtEntry()->lundId())==abs(Pdt::lookup(PdtPdg::D_s_plus)->lundId()))
//       rectagD->setType((rectagD->charge()>0) ? Pdt::lookup(PdtPdg::B_c_plus) : Pdt::lookup(PdtPdg::B_c_minus));
    
    //compute missing 4-vector
    BbrLorentzVectorErr p4ups = eventInfo->electronBeam().fitParams().p4WErr() + eventInfo->positronBeam().fitParams().p4WErr() ;  
    HepLorentzVector DsStp4miss= p4ups - rectagD->p4();
    if(recKaon->p()>0.)DsStp4miss -= recKaon->p4();
    if(recfrag->p()>0.)DsStp4miss -= recfrag->p4();
    if(DsStp4miss.mag()<DsStMassCutLo.value()-.5||DsStMassCutHi.value()+.5<DsStp4miss.mag()) continue;
    _scalers.sum(" pre Ds* mass cut");	      
    DsStPxRaw=DsStp4miss.x();
    DsStPyRaw=DsStp4miss.y();
    DsStPzRaw=DsStp4miss.z();
    DsStERaw=DsStp4miss.t(); 

    //Charge of DKX determines type of Ds
    float DKXcharge=rectagD->charge()+recKaon->charge();
    //need to iterate over the X daus because ->charge() will return pdt value but X can have up to charge 3.
    if(recfrag->nDaughters()>=2){
      HepAListIterator<BtaCandidate> recoXdauIter=recfrag->daughterIterator();  
      while (BtaCandidate* recoXdau = recoXdauIter())
	DKXcharge+=recoXdau->charge();
    }else DKXcharge+=recfrag->charge();
    //note case where X=0 or K=0 particles are OK here (pdt=gamma) 


    //cout<<"marker 3"<<endl;
    //----------------------------------------
    //now create the Ds+
    //----------------------------------------
//     BtaCandidate candDs(DsStp4miss - recgam->p4()); ///compute Ds init 4-Vector        
//     if(DKXcharge==-1.)candDs.setType( Pdt::lookup(PdtLund::D_s_plus)) ;  
//     else if(DKXcharge==1.)candDs.setType( Pdt::lookup(PdtLund::D_s_minus)) ;   
//     else ErrMsg(fatal) << "DRecoDsInclusiveFit:: abs(DKX charge) != 1 aborting" << endmsg;  

    //-----------------------------------------
    //now create the Ds*+
    //-----------------------------------------
    BtaCandidate candDsSt(DsStp4miss);     
    //BtaOpMakeTree combDsStCand;
    //BtaCandidate candDsSt=combDsStCand.combine(candDs,*recgam);
    if(DKXcharge==-1.) candDsSt.setType(Pdt::lookup(PdtLund::D_s_star_plus)) ; 
    else if(DKXcharge==1.) candDsSt.setType(Pdt::lookup(PdtLund::D_s_star_minus)) ; 
    else ErrMsg(fatal) << "DRecoDsInclusiveFit:: abs(DKX charge) != 1 aborting" << endmsg;  
    BtaCandidate boostcandDsSt=cmbooster.boostTo(candDsSt);  
    if(boostcandDsSt.p()<DsMomentumCut.value()-.5)continue;
    _scalers.sum("pre Ds* p* cut");  

    //cout<<"marker 4"<<endl;
    //-----------------------------------------
    //create the first "Upsilon"
    //-----------------------------------------
    BtaOpMakeTree combEvtCand;
    BtaCandidate candEvt;
    if(recKaon->p()>0. && recfrag->p()>0.) candEvt= combEvtCand.combine(*rectagD,candDsSt,*recKaon,*recfrag);//D + LambdaC tags
    else if(recKaon->p()>0. && recfrag->p()==0.) candEvt= combEvtCand.combine(*rectagD,candDsSt,*recKaon);//X=0 //D + LambdaC tags
    else if(recKaon->p()==0. && recfrag->p()>0.) candEvt= combEvtCand.combine(*rectagD,candDsSt,*recfrag);//K=0 //Ds tags
    else candEvt= combEvtCand.combine(*rectagD,candDsSt);//Ds tags
    candEvt.setType(Pdt::lookup(PdtLund::Upsilon_4S));
  
    //cout<<"marker 5"<<endl;
    //----------------------------------
    //Fit the event where the Ds* is not mass constrained
    //----------------------------------
    if(FitEvt.value()){
      setBeamConstraint(candEvt,eventInfo) ;
      BbrVectorErr eMinusMom(eventInfo->electronBeam().p3WCov());
      BbrVectorErr ePlusMom(eventInfo->positronBeam().p3WCov());
      eMinusMom.setCovMatrix(eMinusMom.covMatrix()*2.5);//factor of 2.5 is to get beam-energy error right
      ePlusMom.setCovMatrix(ePlusMom.covMatrix()*2.5);	
      setEnergyConstraint(candEvt,eMinusMom,ePlusMom,.5);	
      //setEnergyConstraint(candEvt,eventInfo) ;
	 
      // fit the upsilon
      VtxFitterOper fitter(candEvt,VtxAbsAlgorithm::TreeFitter) ;
      fitter.invalidateFit();
      fitter.fitAll();
      
      // extract 
      candEvt=fitter.getFittedTree();
      
      if((candEvt.decayVtx())->status()!=BtaAbsVertex::Success)continue;
      _scalers.sum("first fit-succes");		 

      if(probab((int)candEvt.decayVtx()->nDof(),candEvt.decayVtx()->chiSquared()) < FitProbCut.value()) continue;
      _scalers.sum("first fit-prob");  	      	 
      
    }


    //-----------------------------------------------------------------------------------------
    ///Extract daughters from first fit
    //---------------------------------------------------------------------------------
    HepAListIterator<BtaCandidate> candEvtIter=candEvt.daughterIterator();  
    BtaCandidate* firstFittagD=candEvtIter();//must be first one
    BtaCandidate* firstFitDsSt=candEvtIter();//must be second one
    BtaCandidate* firstFitd3=candEvtIter();//Kaon or X, but can be empty
    BtaCandidate* firstFitd4=candEvtIter();//X but can be empty    
    if(!firstFittagD||!firstFitDsSt)
      ErrMsg(fatal) << "DRecoDsInclusiveFit:: Evt dau after first fit not found aborting" << endmsg;  
    //const BtaCandidate* firstFitDs=firstFitDsSt->getDaughter(candDs.pdtEntry()->lundId());
    
    if(firstFitDsSt->p4().mag()<DsStMassCutLo.value()||DsStMassCutHi.value()<firstFitDsSt->p4().mag()) continue;
    _scalers.sum(" Ds* mass cut");	    
    DsStUncMass=firstFitDsSt->p4().mag();       
    DsStPxCorr=firstFitDsSt->p4().x();
    DsStPyCorr=firstFitDsSt->p4().y();
    DsStPzCorr=firstFitDsSt->p4().z();
    DsStECorr=firstFitDsSt->p4().t();
    DsUncMass=(firstFitDsSt->p4() - recgam->p4()).mag();      
    //cout<<"marker 6"<<endl;


    //----------------------------------------
    //now create the Ds+
    //----------------------------------------
    BtaCandidate candDs(firstFitDsSt->p4() - recgam->p4()); ///compute Ds init 4-Vector        
    if(DKXcharge==-1.) candDs.setType( Pdt::lookup(PdtLund::D_s_plus)) ;  
    else if(DKXcharge==1.) candDs.setType( Pdt::lookup(PdtLund::D_s_minus)) ;   
    
    //-----------------------------------------
    //now create the Ds*+ which will be mass constrained
    //-----------------------------------------
    BtaOpMakeTree combsecondDsStCand;
    BtaCandidate secondcandDsSt=combsecondDsStCand.combine(candDs,*recgam);
    if(DKXcharge==-1.) secondcandDsSt.setType(Pdt::lookup(PdtLund::D_s_star_plus)) ; 
    else if(DKXcharge==1.) secondcandDsSt.setType(Pdt::lookup(PdtLund::D_s_star_minus)) ;
    if(MassConstrainDsSt.value())setMassConstraint(secondcandDsSt);

    //-----------------------------------------
    //create final "Upsilon"
    //-----------------------------------------
    BtaCandidate secondCandEvt;
    if(firstFitd3 && firstFitd4) secondCandEvt= combEvtCand.combine(*firstFittagD,secondcandDsSt,*firstFitd3,*firstFitd4);
    else if(firstFitd3&&!firstFitd4) secondCandEvt= combEvtCand.combine(*firstFittagD,secondcandDsSt,*firstFitd3);      
    else if(!firstFitd3&&!firstFitd4)secondCandEvt= combEvtCand.combine(*firstFittagD,secondcandDsSt);
    else ErrMsg(fatal) << "DRecoDsInclusiveFit:: number of daus for second fit not right aborting" << endmsg;  
    secondCandEvt.setType(Pdt::lookup(PdtLund::Upsilon_4S));
  
    //cout<<"marker 7"<<endl;
    //----------------------------------
    //Fit The event where Ds* is mass constrained
    //----------------------------------
    if(FitEvt.value()){
      setBeamConstraint(secondCandEvt,eventInfo) ;
      BbrVectorErr eMinusMom(eventInfo->electronBeam().p3WCov());
      BbrVectorErr ePlusMom(eventInfo->positronBeam().p3WCov());
      eMinusMom.setCovMatrix(eMinusMom.covMatrix()*2.5);//factor of 2.5 is to get beam-energy error right
      ePlusMom.setCovMatrix(ePlusMom.covMatrix()*2.5);	
      setEnergyConstraint(secondCandEvt,eMinusMom,ePlusMom,.5);	
      //setEnergyConstraint(secondCandEvt,eventInfo) ;
	 
      // fit the upsilon
      VtxFitterOper fitter(secondCandEvt,VtxAbsAlgorithm::TreeFitter) ;
      fitter.invalidateFit();
      fitter.fitAll();
      
      // extract 
      secondCandEvt=fitter.getFittedTree();
      
      if((secondCandEvt.decayVtx())->status()!=BtaAbsVertex::Success)continue;
      _scalers.sum("fit-succes");		 

      if(probab((int)secondCandEvt.decayVtx()->nDof(),secondCandEvt.decayVtx()->chiSquared()) < FitProbCut.value()) continue;
      _scalers.sum("fit-prob");  	      	 
      
    }


    ///cut on Ds p*
    const BtaCandidate* fitDsSt=secondCandEvt.getDaughter(secondcandDsSt.pdtEntry()->lundId());
    if(!fitDsSt)
      ErrMsg(fatal) << "DRecoDsInclusiveFit:: Ds* dau after second fit not found aborting" << endmsg;  
    const BtaCandidate* fitDs=fitDsSt->getDaughter(candDs.pdtEntry()->lundId());
    if(!fitDs)
      ErrMsg(fatal) << "DRecoDsInclusiveFit:: Ds dau after second fit not found aborting" << endmsg;  
    BtaCandidate boostfitDs=cmbooster.boostTo(*fitDs);
    if(boostfitDs.p()<DsMomentumCut.value())continue;
    _scalers.sum("Ds p* cut");  	      	 
      
    //cout<<"marker 8"<<endl;

    if(DRecoCandidateList->length()>20) continue;
    _scalers.sum("array length cut");  
       
    //fill unc masses
    if(!DRecoUsrBlock.put(secondCandEvt,TagUncVtxProb)
       ||!DRecoUsrBlock.put(secondCandEvt,TagUncMass)
       ||!DRecoUsrBlock.put(secondCandEvt,DsStUncMass)
       ||!DRecoUsrBlock.put(secondCandEvt,DsUncMass)
       ){
      cout<<" unable to save Unconstrained Mass UsrData for secondCandEvt uid="<<secondCandEvt.uid()
	  <<" original fragcand uid="<<recfrag->uid()<<" address="<<recfrag<<endl;
      continue;
    }
    

    //fill intermediate vectors
    if(!DRecoUsrBlock.put(secondCandEvt,DsStPxRaw)
       ||!DRecoUsrBlock.put(secondCandEvt,DsStPyRaw)
       ||!DRecoUsrBlock.put(secondCandEvt,DsStPzRaw)
       ||!DRecoUsrBlock.put(secondCandEvt,DsStERaw)
       ||!DRecoUsrBlock.put(secondCandEvt,DsStPxCorr)
       ||!DRecoUsrBlock.put(secondCandEvt,DsStPyCorr)
       ||!DRecoUsrBlock.put(secondCandEvt,DsStPzCorr)
       ||!DRecoUsrBlock.put(secondCandEvt,DsStECorr)
       ||!DRecoUsrBlock.put(secondCandEvt,DsMassUnFit)
       ){
      cout<<" unable to save intermediate vectors UsrData for secondCandEvt uid="<<secondCandEvt.uid()
	  <<" original fragcand uid="<<recfrag->uid()<<" address="<<recfrag<<endl;
      continue;
    }


     //cout<<"marker 9"<<endl;

    //------------------------------------------------
    //----------Fill Truth Info
    //-------------------------------------------------
    MyGamMCIdx=-1;
    MyKMCIdx=-1;
    MyTagMCIdx=-1;
    int myxdIdx[10];
    myxdIdx[0]=-1;  myxdIdx[1]=-1;  myxdIdx[2]=-1;  myxdIdx[3]=-1;  myxdIdx[4]=-1; 
    myxdIdx[5]=-1;  myxdIdx[6]=-1;  myxdIdx[7]=-1;  myxdIdx[8]=-1;  myxdIdx[9]=-1;
    float firstpi0mass=0.;   
    if(_truthMap&&_mcList){
      //-----------------------------------
      //truth-match the gamma   
      //----------------------------------- 
      if(GenGam){
	BtaCandidate* recgamtrue = _truthMap->mcFromReco(recgam); 
	if(recgamtrue )
	  if(recgamtrue->pdtEntry()->lundId()!=Pdt::lookup( PdtPdg::gamma)->lundId())recgamtrue=NULL;
	if(recgamtrue ){  
	  if(recgamtrue->uid()==GenGam->uid()){
	    MyGamMCIdx=_mcList->index(BtuUtil::matchUid(_mcList,recgamtrue));
	    _scalers.sum("# truth-matched gammas");
	  }
	} 
      }
   

      //-----------------------------------
      //check the K   
      //-----------------------------------
      //for a Lambda_c mode separate the kaon from the proton;
      const BtaCandidate* recProton =NULL;
      if(abs(recKaon->pdtEntry()->lundId())==abs(Pdt::lookup( PdtPdg::Xi_b0)->lundId())
	 ||abs(recKaon->pdtEntry()->lundId())==abs(Pdt::lookup( PdtPdg::Xi_b_minus)->lundId())){     
	//get the proton to truth match later
	recProton=recKaon->getDaughter(Pdt::lookup( PdtPdg::p_plus)->lundId()); 
	if(!recProton)recProton=recKaon->getDaughter(Pdt::lookup( PdtPdg::anti_p_minus)->lundId()); 
	//substitue the Kaon to reuse original code below!!!!!!!!!!
	const BtaCandidate* recKaonDau =NULL;
	recKaonDau=recKaon->getDaughter(Pdt::lookup( PdtPdg::K_plus)->lundId());    
	if(!recKaonDau)recKaonDau=recKaon->getDaughter(Pdt::lookup( PdtPdg::K_minus)->lundId());
	if(!recKaonDau)recKaonDau=recKaon->getDaughter(Pdt::lookup( PdtPdg::K_S0)->lundId());    

	if(!recKaonDau||!recProton)ErrMsg(fatal) << "DRecoDsInclusiveFit:: Proton or K not found in Lambda_c mode!!!" << endmsg;  
	recKaon=recKaonDau;
      }
      //Note that for a Ds tag recKaon is a gamma and the following code will be OK.						       
      //the followin code was originally written for a D tag
      BtaCandidate* recKaontrue =NULL;
      if(abs(recKaon->pdtEntry()->lundId())==abs(Pdt::lookup( PdtPdg::K_plus)->lundId()))
	recKaontrue=_truthMap->mcFromReco(recKaon);
      if(recKaontrue)
	if(recKaontrue->pdtEntry()->lundId()!=recKaon->pdtEntry()->lundId())recKaontrue=NULL;   
      if(abs(recKaon->pdtEntry()->lundId())==abs(Pdt::lookup( PdtPdg::K_S0)->lundId())){//truth-match pions ignore gammas
	HepAListIterator<BtaCandidate> recKdauIter=recKaon->daughterIterator();
	BtaCandidate* recpi1=recKdauIter();
	BtaCandidate* recpi2=recKdauIter();
	BtaCandidate* recpi1true=_truthMap->mcFromReco(recpi1);//check both pions are true
	if(recpi1true)
	  if(recpi1true->pdtEntry()->lundId()!=recpi1->pdtEntry()->lundId())recpi1true=NULL;  
	BtaCandidate* recpi2true=_truthMap->mcFromReco(recpi2);
	if(recpi2true)
	  if(recpi2true->pdtEntry()->lundId()!=recpi2->pdtEntry()->lundId())recpi2true=NULL;  
      
	if(recpi1true&&recpi2true)//check both true pions have same Ks mother
	  if(recpi1true->theMother()==recpi2true->theMother())
	    if(abs(recpi1true->theMother()->pdtEntry()->lundId())==abs(Pdt::lookup( PdtPdg::K_S0)->lundId()))
	      recKaontrue=recpi1true->theMother();
      } 


      if(recProton){    //for Lambda_c require both Proton and Kaon to be truth-matched
	BtaCandidate* recProtontrue=_truthMap->mcFromReco(recProton);
	if(recProtontrue)
	  if(recProtontrue->pdtEntry()->lundId()!=recProton->pdtEntry()->lundId())recProtontrue=NULL;       
	if(!recProtontrue)recKaontrue=NULL;
      }

      if(recKaontrue){
	MyKMCIdx=_mcList->index(BtuUtil::matchUid(_mcList,recKaontrue));
	_scalers.sum("# truth-matched kaons"); 
      }  

 
      //cout<<"marker 10"<<endl;
         

      //-----------------------------------
      //check the tagD 
      //-----------------------------------
      bool isTruthMatched=0;
      BtaCandidate*GenTagD=NULL;
      HepAListIterator<BtaCandidate> mcIter(*_mcList);
      while(  GenTagD = mcIter()){//check if there such a tag in the event
	if(abs(GenTagD->pdtEntry()->lundId())==abs(rectagD->pdtEntry()->lundId())){//do not check sign because of D0 modes with Ks
	
	  isTruthMatched=1;     
	  HepAListIterator<BtaCandidate> recoTagDdauIter=rectagD->daughterIterator();  

	  //for D* tags get the D
	  if(abs(rectagD->pdtEntry()->lundId())==abs(Pdt::lookup( PdtPdg::D_star_plus)->lundId())
	     ||abs(rectagD->pdtEntry()->lundId())==abs(Pdt::lookup( PdtPdg::D_star0)->lundId())){	   
	    recoTagDdauIter=recoTagDdauIter()->daughterIterator();//Note that D* must have D as first daughter.
	  }
      
	  //the follwing code was originally written for D tags build from K+,Ks,pi+ or pi0 
	  while (BtaCandidate* recoTagDdau = recoTagDdauIter()){
	    if(recoTagDdau->pdtEntry()->lundId()==Pdt::lookup( PdtPdg::pi0)->lundId()//Pi0 case is simple
	       ||abs(recoTagDdau->pdtEntry()->lundId())==abs(Pdt::lookup( PdtPdg::eta)->lundId())
	       ||abs(recoTagDdau->pdtEntry()->lundId())==abs(Pdt::lookup( PdtPdg::eta_prime)->lundId())){
	      //eta' case does not belong here--> final state radiation will not be ignored
	    
	      BtaCandidate* recoTagDdautrue = _truthMap->mcFromReco(recoTagDdau);
	      if(recoTagDdautrue)
		if(recoTagDdautrue->pdtEntry()->lundId()!=recoTagDdau->pdtEntry()->lundId())recoTagDdautrue=NULL;  
	      if(!recoTagDdautrue){isTruthMatched=0; break;}
	    
	      isTruthMatched=0;  //There must a pi0 in GenTagD with same uid
	      if(recoTagDdautrue->overlaps(*GenTagD))isTruthMatched=1; 
	  
	    }else if(abs(recoTagDdau->pdtEntry()->lundId())==abs(Pdt::lookup( PdtPdg::K_plus)->lundId())
		     ||abs(recoTagDdau->pdtEntry()->lundId())==abs(Pdt::lookup( PdtPdg::pi_plus)->lundId())
		     ||abs(recoTagDdau->pdtEntry()->lundId())==abs(Pdt::lookup( PdtPdg::p_plus)->lundId())){//K,pi,p cases
	    
	      BtaCandidate* recoTagDdautrue = _truthMap->mcFromReco(recoTagDdau);
	      if(recoTagDdautrue)
		if(recoTagDdautrue->pdtEntry()->lundId()!=recoTagDdau->pdtEntry()->lundId())recoTagDdautrue=NULL;  
	      if(!recoTagDdautrue){isTruthMatched=0; break;}

	      isTruthMatched=0;//There must a K/pi in GenTagD with same type and uid
	      if(recoTagDdautrue->overlaps(*GenTagD))isTruthMatched=1; 

	   
	    }else if(abs(recoTagDdau->pdtEntry()->lundId())==abs(Pdt::lookup( PdtPdg::K_S0)->lundId())
		     ||abs(recoTagDdau->pdtEntry()->lundId())==abs(Pdt::lookup( PdtPdg::K_star0)->lundId())
		     ||abs(recoTagDdau->pdtEntry()->lundId())==abs(Pdt::lookup( PdtPdg::phi)->lundId())
		     ||abs(recoTagDdau->pdtEntry()->lundId())==abs(Pdt::lookup( PdtPdg::Lambda0)->lundId())
		     ||abs(recoTagDdau->pdtEntry()->lundId())==abs(Pdt::lookup( PdtPdg::rho_plus)->lundId())
		     ||abs(recoTagDdau->pdtEntry()->lundId())==abs(Pdt::lookup( PdtPdg::rho0)->lundId())
		     ||abs(recoTagDdau->pdtEntry()->lundId())==abs(Pdt::lookup( PdtPdg::Sigma0)->lundId())){
	      //These are all composites from 2 particles
	      //Ks,K* phi,rhoC, Lambda0 and Sigma case
	      //Note that Sigma does not ignore final state radiation because decays to Lambda0
	    
	      HepAListIterator<BtaCandidate> recTagDKsDauIter=recoTagDdau->daughterIterator();
	      BtaCandidate* recoKsPi1=recTagDKsDauIter();
	      BtaCandidate* recoKsPi2=recTagDKsDauIter();
	      BtaCandidate* recoKsPi1true = _truthMap->mcFromReco(recoKsPi1);
	      if(recoKsPi1true)
		if(recoKsPi1true->pdtEntry()->lundId()!=recoKsPi1->pdtEntry()->lundId())recoKsPi1true=NULL;  
	      BtaCandidate* recoKsPi2true = _truthMap->mcFromReco(recoKsPi2);
	      if(recoKsPi2true)
		if(recoKsPi2true->pdtEntry()->lundId()!=recoKsPi2->pdtEntry()->lundId())recoKsPi2true=NULL;  
	      if(!recoKsPi1true||!recoKsPi2true){isTruthMatched=0; break;}
	  
	      BtaCandidate* recoKstrue=NULL;
	      if(recoKsPi1true->theMother()==recoKsPi2true->theMother())
		if(abs(recoKsPi1true->theMother()->pdtEntry()->lundId())==abs(recoTagDdau->pdtEntry()->lundId()))
		  recoKstrue=recoKsPi1true->theMother();
	      if(!recoKstrue){isTruthMatched=0; break;}

	      isTruthMatched=0;//There must be a Ks in GenTagD  whose daus are the same pions above
	      if(recoKstrue->overlaps(*GenTagD))isTruthMatched=1; 
	    
	    }else ErrMsg(fatal) << "Tag-D has unknown daughter for truth-matching" << endmsg;  	  
	  
	  }           
	
	  if(isTruthMatched==1) break; 
	}          
      }


      if(isTruthMatched){
	MyTagMCIdx=_mcList->index(BtuUtil::matchUid(_mcList,GenTagD));
	_scalers.sum("# truth-matched tagD's"); 
      }      
 

      //cout<<"marker 11"<<endl;    

      //truth match fragmentation
      int xdaucount=0;
      if(recfrag->nDaughters()>=2&&recfrag->pdtEntry()->lundId()!=Pdt::lookup( PdtPdg::pi0)->lundId()){//X= n*pi + m*pi0 
	HepAListIterator<BtaCandidate> recoFragdauIter=recfrag->daughterIterator();  
	while (BtaCandidate* recoFragdau = recoFragdauIter()){
	  BtaCandidate* recoFragdautrue = _truthMap->mcFromReco(recoFragdau);
	  if(recoFragdautrue)
	    if(recoFragdautrue->pdtEntry()->lundId()!=recoFragdau->pdtEntry()->lundId()) recoFragdautrue=NULL;
	  if(recoFragdautrue)
	    myxdIdx[xdaucount]=_mcList->index(BtuUtil::matchUid(_mcList,recoFragdautrue));	
	  xdaucount++;

	  //find the first pi0 and compute its mass
	  if(recoFragdau->pdtEntry()->lundId()==Pdt::lookup( PdtPdg::pi0)->lundId()&&firstpi0mass==0.){
	    HepAListIterator<BtaCandidate> FragPi0dauIter=recoFragdau->daughterIterator();  
	    BtaCandidate* pi0dau1=FragPi0dauIter();
	    BtaCandidate* pi0dau2=FragPi0dauIter();
	    firstpi0mass=(pi0dau1->p4()+pi0dau2->p4()).mag();
	  }

	}
      }else if(recfrag->p()>0.){//X= 1 pi+, 1 pi0 or 1 gam
	BtaCandidate* recoFragdautrue = _truthMap->mcFromReco(recfrag);
	if(recoFragdautrue)
	  if(recoFragdautrue->pdtEntry()->lundId()!=recfrag->pdtEntry()->lundId()) recoFragdautrue=NULL;
	if(recoFragdautrue)
	  myxdIdx[0]=_mcList->index(BtuUtil::matchUid(_mcList,recoFragdautrue));

	if(recfrag->pdtEntry()->lundId()==Pdt::lookup(PdtPdg::pi0)->lundId()){
	  HepAListIterator<BtaCandidate> FragPi0dauIter=recfrag->daughterIterator();  
	  BtaCandidate* pi0dau1=FragPi0dauIter();
	  BtaCandidate* pi0dau2=FragPi0dauIter();
	  firstpi0mass=(pi0dau1->p4()+pi0dau2->p4()).mag();
	}

      }
    
    }

    if(!DRecoUsrBlock.put(secondCandEvt,MyGamMCIdx))
      cout<<"Error saving MyGamMCIdx Usr DATA"<<endl;  
    if(!DRecoUsrBlock.put(secondCandEvt,MyKMCIdx))
      cout<<"Error saving MyKMCIdx Usr DATA"<<endl; 
    if(!DRecoUsrBlock.put(secondCandEvt,MyTagMCIdx))
      cout<<"Error saving MyTagMCIdx UsrDATA"<<endl;
    MyXd1MCIdx=myxdIdx[0];    MyXd2MCIdx=myxdIdx[1];    MyXd3MCIdx=myxdIdx[2];    MyXd4MCIdx=myxdIdx[3];    MyXd5MCIdx=myxdIdx[4];    
    MyXd6MCIdx=myxdIdx[5];    MyXd7MCIdx=myxdIdx[6];    MyXd8MCIdx=myxdIdx[7];    MyXd9MCIdx=myxdIdx[8];    MyXd10MCIdx=myxdIdx[9]; 
    MyXPi0Mass=firstpi0mass;    
    if(!DRecoUsrBlock.put(secondCandEvt,MyXd1MCIdx)
       ||!DRecoUsrBlock.put(secondCandEvt,MyXd2MCIdx)
       ||!DRecoUsrBlock.put(secondCandEvt,MyXd3MCIdx)
       ||!DRecoUsrBlock.put(secondCandEvt,MyXd4MCIdx)
       ||!DRecoUsrBlock.put(secondCandEvt,MyXd5MCIdx)
       ||!DRecoUsrBlock.put(secondCandEvt,MyXd6MCIdx)
       ||!DRecoUsrBlock.put(secondCandEvt,MyXd7MCIdx)
       ||!DRecoUsrBlock.put(secondCandEvt,MyXd8MCIdx)
       ||!DRecoUsrBlock.put(secondCandEvt,MyXd9MCIdx)
       ||!DRecoUsrBlock.put(secondCandEvt,MyXd10MCIdx)
       ||!DRecoUsrBlock.put(secondCandEvt,MyXPi0Mass)
       )
      cout<<"Error saving MyXMCIdx Usr DATA"<<endl;
    
    
    //add Upsilon to list
    *DRecoCandidateList += new BtaCandidate(secondCandEvt);
    _scalers.sum("final candidates");
    

    //create a list to match the fitted Upsilons for debugging
    HepAList<BtaCandidate>* newcandlist= new HepAList<BtaCandidate>;    
    HepAListIterator< BtaCandidate > candIter(*candlist);
    while (BtaCandidate * cand = candIter())
      *newcandlist+=new BtaCandidate(*cand);
    *DRecoEvtCandidateList += newcandlist;

    
  }

  //clean up old lists
  HepAListIterator< HepAList<BtaCandidate> > CleanListIter(TempEvtCandList); 
  while (HepAList<BtaCandidate> * candlist = CleanListIter())
    HepAListDeleteAll(*candlist);
  HepAListDeleteAll(TempEvtCandList);

  return AppResult::OK;
}


HepLorentzVector DRecoDsInclusiveFit::DsStp4Corrected(HepLorentzVector DsStp4)
{
  //this calculation calculates a correction 4-Vector which makes the DsStp4 mass to be equal to Ds* PDGMass
  //The calculation minimizes the function S=deltaP*deltaP where deltaP is the correction 4-Vector.

  float pDst=DsStp4.rho();
  float eDst=DsStp4.e();
  float MDst=Pdt::lookup(PdtLund::D_s_star_plus)->mass();
  //The calculation shows that the 3-momentum vector of deltaP has the same direction as DsStp4.p3();
  //But the magnitude has to be calculated from a Quartic equation:
  //This code finds the smallest solution to the following equation:
  ///A*x^4+B*x^3+C*x^2+D*x+E=0 
  float A=9.;
  float B=30.*pDst;
  float C=9.*MDst*MDst + 3.*pDst*pDst - eDst*eDst;
  float D=12.*MDst*MDst*pDst + 20.*pDst*pDst*pDst - 4.*eDst*eDst*pDst;
  float E=4.*MDst*MDst*pDst*pDst + 4.*pDst*pDst*pDst*pDst - 4.*eDst*eDst*pDst*pDst;
  
  //Now define range for w : (-T,T)
  int Nsteps=200;
  float step=.001;//set max correction to +-200MeV
  float w0=0.;
  if(MDst*MDst+pDst*pDst==eDst*eDst)
    w0=0.;
  else {
    float fprev=0.;
    float w=0.;
    float f=0.;
    for(int i=1;i<=Nsteps;i++){
      if((MDst*MDst+pDst*pDst)>eDst*eDst)w=.001-i*step;//raw p3 is too large need negative correction
      if((MDst*MDst+pDst*pDst)<eDst*eDst)w=-.001+i*step;//raw p3 is too small need positive correction
      f=A*w*w*w*w+B*w*w*w+C*w*w+D*w+E;
      //cout<<i<<" "<<w<<" "<<f<<endl;
      if(f*fprev<0.){w0=w-step/2.;break;}//found solution if crossed 0 line
      fprev=f;
    }
  }

  Hep3Vector deltap3(w0*DsStp4.v().unit());

  //The calculation shows that Energy component of deltaP can be calculated as 
  HepLorentzVector deltaP(deltap3,-eDst+sqrt(MDst*MDst+(pDst+w0)*(pDst+w0)));//there is ambiguity in sign of sqrt()=> choos pos. E. so that for 0 p3 correction you get 0 p4 corretion (i.e. second argument cancels)
  return DsStp4+deltaP; 
}


AppResult DRecoDsInclusiveFit::endJob( AbsEvent* anEvent )
{
  ostream& o=cout;
  _scalers.print(o);
  o << endl;

  return AppResult::OK;
}




