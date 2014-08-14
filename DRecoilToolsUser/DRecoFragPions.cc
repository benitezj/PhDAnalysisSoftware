
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "DRecoFragPions.hh"

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
#include "Beta/BtaCandidate.hh"
#include "Beta/EventInfo.hh"
#include "BetaCoreTools/BtaOpMakeTree.hh"
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
#include "Beta/BtaConstraint.hh"
#include "BetaCoreTools/BtaBooster.hh"

using std::cout;
using std::endl;
using std::ostream;

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//----------------
// Constructors --
//----------------
DRecoFragPions::DRecoFragPions( const char* const theName,const char* const theDescription )
  : TagFilterModule( theName, theDescription )
  , PiList( "PiList", this, "GoodTracksVeryLoose" )
  , PiPCutLo( "PiPCutLo", this, .050)
  , PiPCutHi( "PiPCutHi", this, 2.)
  , PiKsVetoList("PiKsVetoList",this,"0")
  , PiKVetoList("PiKVetoList",this,"0")
  , PiMaxNumber("PiMaxNumber",this,5)
  , Pi0List( "Pi0List", this, "pi0TightMassNoMomentumCut" )
  , Pi0GamEnergyCutLo( "Pi0GamEnergyCutLo", this, .05)
  , Pi0GamEnergyCutHi( "Pi0GamEnergyCutHi", this, 2.)
  , Pi0MassCutLo("Pi0MassCutLo",this,.115)
  , Pi0MassCutHi("Pi0MassCutHi",this,.150)
  , Pi0MassConstrain("Pi0MassConstrain",this,1)
  , Pi0MaxNumber("Pi0MaxNumber",this,3)
  , GammaList( "GammaList", this, "0" )
  , GammaEnergyCutLo( "GammaEnergyCutLo", this,.1)
  , GammaEnergyCutHi( "GammaEnergyCutHi", this,100.)
  , GammaPi0VetoList("GammaPi0VetoList",this,"pi0DefaultMass")
  , GammaEtaVetoList("GammaEtaVetoList",this,"etaggDefaultNoMass")
  , GammaMaxNumber("GammaMaxNumber",this,1)
  , XChargeCut("XChargeCut",this,1)
  , _scalers("DRecoFragPions") 
{
  commands()->append( &PiList );
  commands()->append( &PiPCutLo );
  commands()->append( &PiPCutHi );
  commands()->append( &PiKsVetoList );
  commands()->append( &PiKVetoList );
  commands()->append( &PiMaxNumber);

  commands()->append( &Pi0List );
  commands()->append( &Pi0GamEnergyCutLo );
  commands()->append( &Pi0GamEnergyCutHi );
  commands()->append( &Pi0MassCutLo);
  commands()->append( &Pi0MassCutHi);
  commands()->append( &Pi0MassConstrain);
  commands()->append( &Pi0MaxNumber);

  commands()->append( &GammaList);
  commands()->append( &GammaEnergyCutLo);
  commands()->append( &GammaEnergyCutHi);
  commands()->append( &GammaPi0VetoList);
  commands()->append( &GammaEtaVetoList);
  commands()->append( &GammaMaxNumber);

  commands()->append( &XChargeCut);

}

//--------------
// Destructor --
//--------------
DRecoFragPions::~DRecoFragPions( )
{}

//--------------
// Operations --
//--------------
AppResult DRecoFragPions::beginJob( AbsEvent* anEvent )
{ 
  cout<<"DRecoFragPions::PiList "<<PiList.value()<<endl;
  cout<<"DRecoFragPions::PiPCutLo "<<PiPCutLo.value()<<endl;
  cout<<"DRecoFragPions::PiPCutHi "<<PiPCutHi.value()<<endl;
  cout<<"DRecoFragPions::Pi0List "<<Pi0List.value()<<endl;
  cout<<"DRecoFragPions::Pi0GamEnergyCutLo "<<Pi0GamEnergyCutLo.value()<<endl;
  cout<<"DRecoFragPions::Pi0GamEnergyCutHi "<<Pi0GamEnergyCutHi.value()<<endl;
  cout<<"DRecoFragPions::Pi0MassCutLo "<<Pi0MassCutLo.value()<<endl;
  cout<<"DRecoFragPions::Pi0MassCutHi "<<Pi0MassCutHi.value()<<endl;

  return AppResult::OK;
}

AppResult DRecoFragPions::event( AbsEvent* anEvent )
{
  _scalers.sum("# events ");
 
  HepAList< HepAList<BtaCandidate> >* DRecoEvtCandidateList = Ifd<HepAList< HepAList<BtaCandidate> > >::get(anEvent,"DRecoEvtCandidateList");  
  if(DRecoEvtCandidateList == 0)return AppResult::OK;
  if(DRecoEvtCandidateList->length()==0) return AppResult::OK;//nothing to do

  // Create temp. new list
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
  if (eventInfo == 0){
    ErrMsg(fatal) << "Nothing in eventInfoList...aborting" << endmsg;
  }

  //the beam spot below has been smeared in MC and fixed error=10mu data/MC
  const BbrPointErr beamSpot = eventInfoListPtr->first()->beamSpot();
  //booster
  BtaBooster cmbooster(eventInfo->cmFrame());  


  ///
  HepAList<BtaCandidate>  TempPiList ;
  //get the input pion list and add
  HepAList<BtaCandidate>* EventPiList  = Ifd<HepAList< BtaCandidate > >::get(anEvent, PiList.value());
  if (EventPiList == 0)  ErrMsg(fatal) << PiList.value() << " list not found" << endmsg;    
  HepAList<BtaCandidate>* EventPiKsVetoList  = Ifd<HepAList< BtaCandidate > >::get(anEvent, PiKsVetoList.value());
  HepAList<BtaCandidate>* EventPiKVetoList  = Ifd<HepAList< BtaCandidate > >::get(anEvent, PiKVetoList.value());
  HepAListIterator<BtaCandidate> PiIter(*EventPiList);
  while (BtaCandidate* fragcand = PiIter()){
    _scalers.sum("total pi's");

    if(EventPiKsVetoList)
      if(fragcand->overlaps(*EventPiKsVetoList))continue;
    if(EventPiKVetoList)
      if(fragcand->overlaps(*EventPiKVetoList))continue;
    _scalers.sum("pi's after veto cut");  


    if(fragcand->p()<PiPCutLo.value()) continue;    
    if(fragcand->p()>PiPCutHi.value()) continue;    
    _scalers.sum("pi's after p_Lab");  
      

    BtaCandidate* newcand = new BtaCandidate(*fragcand);
    newcand->setType((newcand->charge()>0)? Pdt::lookup( PdtPdg::pi_plus) : Pdt::lookup( PdtPdg::pi_minus));
    TempPiList +=  newcand;         
  }

  //get the input pi0 list and add 
  HepAList<BtaCandidate>  TempPi0List ;
  HepAList<BtaCandidate>* EventPi0List  = Ifd<HepAList< BtaCandidate > >::get(anEvent, Pi0List.value());
  if (EventPi0List == 0)  ErrMsg(fatal) << Pi0List.value() << " list not found" << endmsg;  
  HepAListIterator<BtaCandidate> Pi0Iter(*EventPi0List);
  while (BtaCandidate* fragcand = Pi0Iter()){
    _scalers.sum("initial pi0's");     

    //cut on the mass
    if(fragcand->mass()<Pi0MassCutLo.value()||Pi0MassCutHi.value()<fragcand->mass())continue;
    _scalers.sum("pi0's after mass cut");

    //get the gammas
    HepAListIterator<BtaCandidate> Pi0GamIter=fragcand->daughterIterator();
    BtaCandidate* gamma1=Pi0GamIter();        
    BtaCandidate* gamma2=Pi0GamIter(); 
    //max energy cut
    if(gamma1->energy()>Pi0GamEnergyCutHi.value()||gamma2->energy()>Pi0GamEnergyCutHi.value()) continue;          
    //angle dependent min cut
    float angledependentcut=Pi0GamEnergyCutLo.value();
    //cut gamma 1
    float cosine=gamma1->p3().z()/gamma1->p();
    if(cosine>.5)angledependentcut=Pi0GamEnergyCutLo.value()+.1*(cosine-.5)/.5;
    if(gamma1->energy()<angledependentcut) continue; 
    //cut gamma 2
    angledependentcut=Pi0GamEnergyCutLo.value();
    cosine=gamma2->p3().z()/gamma2->p();
    if(cosine>.5)angledependentcut=Pi0GamEnergyCutLo.value()+.1*(cosine-.5)/.5;
    if(gamma2->energy()<angledependentcut) continue;         
    _scalers.sum("pi0's after gamma energy cut");

           
    _scalers.sum("final pi0's"); 
    BtaCandidate* newcand = new BtaCandidate(*fragcand);
    newcand->removeConstraint(BtaConstraint::Beam);//remove constriant to primVtx
    //setBeamConstraint(*newcand,eventInfo) ;//should not be necesary because it will have mother beamspot constrained
    newcand->setType(Pdt::lookup( PdtPdg::pi0) );
    TempPi0List +=  newcand;           
  }
  

  //get the input gamma list 
  HepAList<BtaCandidate>  TempGammaList ;
  HepAList<BtaCandidate>* EventGammaList = Ifd<HepAList< BtaCandidate > >::get(anEvent, GammaList.value());
  if (EventGammaList != 0){//  ErrMsg(fatal) << GammaList.value() << " list not found" << endmsg;   

    HepAList<BtaCandidate>* EventGammaPi0VetoList  = Ifd<HepAList< BtaCandidate > >::get(anEvent, GammaPi0VetoList.value());
    HepAList<BtaCandidate>* EventGammaEtaVetoList  = Ifd<HepAList< BtaCandidate > >::get(anEvent, GammaEtaVetoList.value());

    HepAListIterator<BtaCandidate> GammaIter(*EventGammaList);
    while (BtaCandidate* fragcand = GammaIter()){
      _scalers.sum("initial gammas's");     

      if(EventGammaPi0VetoList)
	if(fragcand->overlaps(*EventGammaPi0VetoList))continue;
      if(EventGammaEtaVetoList)
	if(fragcand->overlaps(*EventGammaEtaVetoList))continue;
      _scalers.sum("gamma's after pi0 and eta veto cut");   
    

      //max energy cut
      if(fragcand->energy()>GammaEnergyCutHi.value()) continue;          
      //angle dependent min cut
      float angledependentcut=GammaEnergyCutLo.value();
      float cosine=fragcand->p3().z()/fragcand->p();
      if(cosine>.5)angledependentcut=GammaEnergyCutLo.value()+.1*(cosine-.5)/.5;
      if(fragcand->energy()<angledependentcut) continue; 
      _scalers.sum("gamma's after gamma energy cut");   
           
      _scalers.sum("final gamma's"); 
      BtaCandidate* newcand = new BtaCandidate(*fragcand);
      newcand->removeConstraint(BtaConstraint::Beam);//remove constriant to primVtx
      //setBeamConstraint(*newcand,eventInfo) ;//should not be necesary because it will have mother beamspot constrained
      newcand->setType(Pdt::lookup( PdtPdg::gamma) );
      TempGammaList +=  newcand;           
    }
  }

  //for each evt candidate select the fragmentation
  HepAListIterator< HepAList<BtaCandidate> > TempEvtListIter(TempEvtCandList);
  while (HepAList<BtaCandidate> * tmpcandlist = TempEvtListIter()){
    _scalers.sum("initial total");

    //------------------------------ 
    HepAListIterator<BtaCandidate> PiCandIter(TempPiList);
    HepAList<BtaCandidate> NonOverlapPiList;
    while (BtaCandidate* fragcand = PiCandIter()){           
      if(fragcand->overlaps(*tmpcandlist))continue;       
      _scalers.sum("non-overlap pi's"); 
      NonOverlapPiList+=fragcand;
    }
    HepAListIterator<BtaCandidate> Pi0CandIter(TempPi0List);
    HepAList<BtaCandidate> NonOverlapPi0List;
    while (BtaCandidate* fragcand = Pi0CandIter()){           
      if(fragcand->overlaps(*tmpcandlist))continue;       
      _scalers.sum("non-overlap pi0's"); 
      NonOverlapPi0List+=fragcand;
    }
    HepAListIterator<BtaCandidate> GammaCandIter(TempGammaList);
    HepAList<BtaCandidate> NonOverlapGammaList;
    while (BtaCandidate* fragcand = GammaCandIter()){           
      if(fragcand->overlaps(*tmpcandlist))continue;       
      _scalers.sum("non-overlap gamma's"); 
      NonOverlapGammaList+=fragcand;
    }
    

    //-------------------------------
    //Create the Fragmentation lists
    //-------------------------------
    HepAList< HepAList<BtaCandidate> > XCandList;
    CreateXCandidates(&XCandList,&NonOverlapPiList,&NonOverlapPi0List,&NonOverlapGammaList);
            

    HepAListIterator< HepAList<BtaCandidate> > XCandListIter(XCandList);
    while(HepAList<BtaCandidate>* Xcand=XCandListIter()){
      _scalers.sum("total X candidates");

      //determine charge and decay mode
      float charge=0.; 
      int npi=0;int npi0=0;int ngam=0;
      HepAListIterator<BtaCandidate> XCandIter(*Xcand);
      while(BtaCandidate* fragcand = XCandIter()){
	charge+=fragcand->charge();
	if(abs(fragcand->pdtEntry()->lundId())==abs(Pdt::lookup( PdtPdg::gamma )->lundId())) ngam++;
	if(abs(fragcand->pdtEntry()->lundId())==abs(Pdt::lookup( PdtPdg::pi0)->lundId())) npi0++;
	if(abs(fragcand->pdtEntry()->lundId())==abs(Pdt::lookup( PdtPdg::pi_plus)->lundId())) npi++;
      }
      char chargetext[40];
      sprintf(chargetext,"X charge= %.0f",fabs(charge));
      _scalers.sum(chargetext);
      char modetext[40];
      sprintf(modetext,"X mode= %d",8*npi+2*npi0+ngam);
      _scalers.sum(modetext);

 
      if(fabs(charge)>XChargeCut.value())continue;
      _scalers.sum("X candidates after charge ");
	
      XCandIter.rewind();
      BtaCandidate* FragCand=NULL;
      if(Xcand->length()==0){///Note this a special case (No real X=pi0 with p=0.) to be treated carefully in later modules
	FragCand=new BtaCandidate(HepLorentzVector(0.,0.,0.,0.));
	FragCand->setType(Pdt::lookup( PdtPdg::gamma ) ); 
      }else if(Xcand->length()==1){
	FragCand=new BtaCandidate(*(XCandIter()));//no need to set type
      }else {
	BtaOpMakeTree combFragPions;
	XCandIter.rewind();//just in case
	FragCand=combFragPions.createFromList(XCandIter);//returns ownership
	if(charge==0.)
	  FragCand->setType(Pdt::lookup( PdtPdg::D_10 )); 
	else //note that all positive charges (+1,+2,+3, ...) get the same type.
	  FragCand->setType(charge>0. ? Pdt::lookup( PdtPdg::D_1_plus ) : Pdt::lookup( PdtPdg::D_1_minus));

      }
      //in summary note that X can have type gamma,pi0,pi+,D_10 or D_1+

      HepAList<BtaCandidate>* newcandlist= new HepAList<BtaCandidate>;    
      HepAListIterator< BtaCandidate > candIter(*tmpcandlist);
      while (BtaCandidate * cand = candIter())
	*newcandlist+=new BtaCandidate(*cand);	
      *newcandlist += FragCand;
      *DRecoEvtCandidateList += newcandlist ;
      _scalers.sum("final lists");         
      
    }
       
    
    HepAListDeleteAll(*tmpcandlist);//clean up old candidates      
    HepAListDeleteAll(XCandList);
  }
  

  HepAListDeleteAll(TempPiList);
  HepAListDeleteAll(TempPi0List);
  HepAListDeleteAll(TempGammaList);
  HepAListDeleteAll(TempEvtCandList);
 

  return AppResult::OK;
}

void DRecoFragPions::CreateXCandidates(HepAList< HepAList< BtaCandidate > >* XCandidates, HepAList< BtaCandidate >* PiList, HepAList< BtaCandidate >* Pi0List, HepAList< BtaCandidate >* GammaList){
  
  //create X cands with 0 pi+
  HepAList<BtaCandidate> pilist;
  CreateXPi0Lists(XCandidates,&pilist,Pi0List,GammaList);

  //-------------------------------------------------------------------------------------
  //create X cands with 1 pi+
  if(PiList->length()>=1&&PiMaxNumber.value()>=1){
    HepAListIterator<BtaCandidate> PiIter1(*PiList);
    while (BtaCandidate* picand1= PiIter1()){      
      HepAList<BtaCandidate> pilist;
      pilist+=picand1;
      CreateXPi0Lists(XCandidates,&pilist,Pi0List,GammaList);
    }
  }
 
  //-------------------------------------------------------------------------------------
  //Create X Cands with 2 pi+
  if(PiList->length()>=2&&PiMaxNumber.value()>=2){
    int picounter1=0;	      
    HepAListIterator<BtaCandidate> PiIter1(*PiList);
    while (BtaCandidate* picand1= PiIter1()){   
      picounter1++;
      int picounter2=0;
      HepAListIterator<BtaCandidate> PiIter2(*PiList);
      while (BtaCandidate* picand2= PiIter2()){
	picounter2++;
	if(picounter2<=picounter1)continue;
	
	HepAList<BtaCandidate> pilist;
	pilist+=picand1;
	pilist+=picand2;	
	CreateXPi0Lists(XCandidates,&pilist,Pi0List,GammaList);

      }
    }
  }

  //-------------------------------------------------------------------------------------
  //Create X Cands with 3 pi+
  if(PiList->length()>=3&&PiMaxNumber.value()>=3){
    int picounter1=0;	      
    HepAListIterator<BtaCandidate> PiIter1(*PiList);
    while (BtaCandidate* picand1= PiIter1()){   
      picounter1++;
      int picounter2=0;
      HepAListIterator<BtaCandidate> PiIter2(*PiList);
      while (BtaCandidate* picand2= PiIter2()){
	picounter2++;
	if(picounter2<=picounter1)continue;
	int picounter3=0;
	HepAListIterator<BtaCandidate> PiIter3(*PiList);
	while (BtaCandidate* picand3= PiIter3()){
	  picounter3++;
	  if(picounter3<=picounter2)continue;
	
	  HepAList<BtaCandidate> pilist;
	  pilist+=picand1;
	  pilist+=picand2;
	  pilist+=picand3;	
	  CreateXPi0Lists(XCandidates,&pilist,Pi0List,GammaList);

	}
      }
    }
  }

  //-------------------------------------------------------------------------------------
  //Create X Cands with 4 pi+
  if(PiList->length()>=4&&PiMaxNumber.value()>=4){
    int picounter1=0;	      
    HepAListIterator<BtaCandidate> PiIter1(*PiList);
    while (BtaCandidate* picand1= PiIter1()){   
      picounter1++;
      int picounter2=0;
      HepAListIterator<BtaCandidate> PiIter2(*PiList);
      while (BtaCandidate* picand2= PiIter2()){
	picounter2++;
	if(picounter2<=picounter1)continue;
	int picounter3=0;
	HepAListIterator<BtaCandidate> PiIter3(*PiList);
	while (BtaCandidate* picand3= PiIter3()){
	  picounter3++;
	  if(picounter3<=picounter2)continue;
	  int picounter4=0;
	  HepAListIterator<BtaCandidate> PiIter4(*PiList);
	  while (BtaCandidate* picand4= PiIter4()){
	    picounter4++;
	    if(picounter4<=picounter3)continue;
	
	    HepAList<BtaCandidate> pilist;
	    pilist+=picand1;
	    pilist+=picand2;
	    pilist+=picand3;	
	    pilist+=picand4;	
	    CreateXPi0Lists(XCandidates,&pilist,Pi0List,GammaList);
	  }
	}
      }
    }
  }


  //-------------------------------------------------------------------------------------
  //Create X Cands with 5 pi+
  if(PiList->length()>=5&&PiMaxNumber.value()>=5){
    int picounter1=0;	      
    HepAListIterator<BtaCandidate> PiIter1(*PiList);
    while (BtaCandidate* picand1= PiIter1()){   
      picounter1++;
      int picounter2=0;
      HepAListIterator<BtaCandidate> PiIter2(*PiList);
      while (BtaCandidate* picand2= PiIter2()){
	picounter2++;
	if(picounter2<=picounter1)continue;
	int picounter3=0;
	HepAListIterator<BtaCandidate> PiIter3(*PiList);
	while (BtaCandidate* picand3= PiIter3()){
	  picounter3++;
	  if(picounter3<=picounter2)continue;
	  int picounter4=0;
	  HepAListIterator<BtaCandidate> PiIter4(*PiList);
	  while (BtaCandidate* picand4= PiIter4()){
	    picounter4++;
	    if(picounter4<=picounter3)continue;
	    int picounter5=0;
	    HepAListIterator<BtaCandidate> PiIter5(*PiList);
	    while (BtaCandidate* picand5= PiIter5()){
	      picounter5++;
	      if(picounter5<=picounter4)continue;
	
	      HepAList<BtaCandidate> pilist;
	      pilist+=picand1;
	      pilist+=picand2;
	      pilist+=picand3;	
	      pilist+=picand4;	
	      pilist+=picand5;	
	      CreateXPi0Lists(XCandidates,&pilist,Pi0List,GammaList);
	    }
	  }
	}
      }
    }
  }


}

void DRecoFragPions::CreateXPi0Lists(HepAList< HepAList< BtaCandidate > >* XCandidates, HepAList< BtaCandidate >* PiList, HepAList< BtaCandidate >* Pi0List, HepAList< BtaCandidate >* GammaList){

  //-----------------------------------------------------
  //create X cands with 0 pi0
  HepAList<BtaCandidate> pipi0list(*PiList);
  CreateXGammaLists(XCandidates,&pipi0list,GammaList);
  

  //-----------------------------------------------------
  //create X cands with 1 pi0
  if(Pi0List->length()>=1&&Pi0MaxNumber.value()>=1){
    HepAListIterator<BtaCandidate> Pi0Iter1(*Pi0List);
    while (BtaCandidate* pi0cand1= Pi0Iter1()){
      HepAList<BtaCandidate> pipi0list(*PiList);
      pipi0list+=pi0cand1;
      CreateXGammaLists(XCandidates,&pipi0list,GammaList);       
    }
  }
      
  //-----------------------------------------------------
  //create X cands with 2 pi0   
  if(Pi0List->length()>=2&&Pi0MaxNumber.value()>=2){    
    int pi0counter1=0;
    HepAListIterator<BtaCandidate> Pi0Iter(*Pi0List);
    while (BtaCandidate* pi0cand1= Pi0Iter()){
      pi0counter1++;
      int pi0counter2=0;
      HepAListIterator<BtaCandidate> Pi0Iter2(*Pi0List);
      while (BtaCandidate* pi0cand2= Pi0Iter2()){
	pi0counter2++;
	if(pi0counter2<=pi0counter1)continue;
	//do not allow for overlap
	if(pi0cand2->overlaps(*pi0cand1))continue;
	    
	HepAList<BtaCandidate> pipi0list(*PiList);
	pipi0list+=pi0cand1;
	pipi0list+=pi0cand2;
	CreateXGammaLists(XCandidates,&pipi0list,GammaList); 
      }	  
    }
  }

  //-----------------------------------------------------
  //create X cands with 3 pi0     
  if(Pi0List->length()>=3&&Pi0MaxNumber.value()>=3){
    int pi0counter1=0;
    HepAListIterator<BtaCandidate> Pi0Iter(*Pi0List);
    while(BtaCandidate* pi0cand1= Pi0Iter()){
      pi0counter1++;
      int pi0counter2=0;
      HepAListIterator<BtaCandidate> Pi0Iter2(*Pi0List);
      while(BtaCandidate* pi0cand2= Pi0Iter2()){
	pi0counter2++;
	if(pi0counter2<=pi0counter1)continue;
	if(pi0cand2->overlaps(*pi0cand1))continue;
	int pi0counter3=0;
	HepAListIterator<BtaCandidate> Pi0Iter3(*Pi0List);
	while(BtaCandidate* pi0cand3= Pi0Iter3()){	  
	  pi0counter3++;
	  if(pi0counter3<=pi0counter2)continue;
	  if(pi0cand3->overlaps(*pi0cand1)||pi0cand3->overlaps(*pi0cand2))continue;
		  
	  HepAList<BtaCandidate> pipi0list(*PiList);
	  pipi0list+=pi0cand1;
	  pipi0list+=pi0cand2;
	  pipi0list+=pi0cand3;
	  CreateXGammaLists(XCandidates,&pipi0list,GammaList); 
  	    
	}
      }
    }      
  }
    
}

void DRecoFragPions::CreateXGammaLists(HepAList< HepAList< BtaCandidate > >* XCandidates, HepAList< BtaCandidate >* PiPi0List, HepAList< BtaCandidate >* GammaList){

  //create X cands with 0 gamma
  HepAList<BtaCandidate>* candX=new HepAList<BtaCandidate>(*PiPi0List);
  *XCandidates+=candX;	
  
  //create X cands with 1 gamma
  if(GammaList->length()>=1&&GammaMaxNumber.value()>=1){
    HepAListIterator<BtaCandidate> GammaIter(*GammaList);
    while (BtaCandidate* gammacand= GammaIter()){
      HepAList<BtaCandidate>* candX=new HepAList<BtaCandidate>(*PiPi0List);
      *candX+=gammacand;
      *XCandidates+=candX;
    }
  }
    
}




AppResult DRecoFragPions::endJob( AbsEvent* anEvent )
{
  ostream& o=cout;
  _scalers.print(o);
  o << endl;

  return AppResult::OK;
}










