
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "DRecoCheckCharge.hh"

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
#include "CLHEP/Alist/AList.h"
#include "CLHEP/Alist/AIterator.h"
#include "ErrLogger/ErrLog.hh"
#include "PDT/Pdt.hh"

using std::cout;
using std::endl;
using std::ostream;

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//----------------
// Constructors --
//----------------
DRecoCheckCharge::DRecoCheckCharge( const char* const theName,const char* const theDescription )
  : TagFilterModule( theName, theDescription )
  , SelectWrongSign("SelectWrongSign",this,0)
  , _scalers("DRecoCheckCharge")
{
  commands()->append( &SelectWrongSign);
}

//--------------
// Destructor --
//--------------
DRecoCheckCharge::~DRecoCheckCharge()
{}

//--------------
// Operations --
//--------------
AppResult DRecoCheckCharge::beginJob( AbsEvent* anEvent )
{ 
  cout<<"DRecoCheckCharge:: SelectWrongSign="<<SelectWrongSign.value()<<endl;

 return AppResult::OK;
}

AppResult DRecoCheckCharge::event( AbsEvent* anEvent )
{
  _scalers.sum("#  events ");

  //retreive the event candidates
  HepAList< HepAList<BtaCandidate> >* DRecoEvtCandidateList = Ifd<HepAList< HepAList<BtaCandidate> > >::get(anEvent,"DRecoEvtCandidateList");
  if(DRecoEvtCandidateList == 0)  ErrMsg(fatal) <<"DRecoEvtCandidateList  not found" << endmsg;   
  if(DRecoEvtCandidateList->length()==0) return AppResult::OK;//nothing to do

  //  
  HepAList< HepAList<BtaCandidate> >  TempEvtCandList ;
  HepAListIterator< HepAList<BtaCandidate> > ListIter(*DRecoEvtCandidateList);
  while (HepAList<BtaCandidate> * candlist = ListIter()){
    TempEvtCandList += new HepAList<BtaCandidate>(*candlist);
  }
  HepAListDeleteAll(*DRecoEvtCandidateList);  
  

  HepAListIterator< HepAList<BtaCandidate> > EvtListIter(TempEvtCandList); 
  while (HepAList<BtaCandidate> * candlist = EvtListIter()){    
    _scalers.sum("# initial lists"); 
    
    float totalDKXcharge=0.;
    int TagCQuarks=0;
    int TagSQuarks=0;    
    int TagantiCQuarks=0;
    int TagantiSQuarks=0;    
    int SigCQuarks=0;
    int SigSQuarks=0;
    int SigantiCQuarks=0;
    int SigantiSQuarks=0;


    HepAListIterator<BtaCandidate> CandListIter(*candlist);  
    BtaCandidate* TagD=CandListIter();//should be first one
    if(!TagD)ErrMsg(fatal) <<" DRecoCheckCharge:: Tag not found" << endmsg;  
    totalDKXcharge+=TagD->charge();

    //--------------------------------------------------------
    //count the c quarks
    //--------------------------------------------------------
    if(TagD->pdtEntry()->lundId()==Pdt::lookup(PdtPdg::D_plus)->lundId()
       ||TagD->pdtEntry()->lundId()==Pdt::lookup(PdtPdg::D_star_plus)->lundId())
      TagCQuarks++;
    else if(TagD->pdtEntry()->lundId()==Pdt::lookup(PdtPdg::D_minus)->lundId()
	    ||TagD->pdtEntry()->lundId()==Pdt::lookup(PdtPdg::D_star_minus)->lundId())
      TagantiCQuarks++;      	
    else if(abs(TagD->pdtEntry()->lundId())==abs(Pdt::lookup(PdtPdg::D0)->lundId())
	    ||abs(TagD->pdtEntry()->lundId())==abs(Pdt::lookup(PdtPdg::D_star0)->lundId())){
      
      //get the D0
      HepAListIterator<BtaCandidate> D0DauIter=TagD->daughterIterator();
      if(abs(TagD->pdtEntry()->lundId())==abs(Pdt::lookup(PdtPdg::D_star0)->lundId())){
	//replace iterator with the D0 iter
	const BtaCandidate* D0dau=TagD->getDaughter(Pdt::lookup(PdtPdg::D0)->lundId());
	if(!D0dau) D0dau=TagD->getDaughter(Pdt::lookup(PdtPdg::anti_D0)->lundId());
	if(!D0dau) ErrMsg(fatal) <<" DRecoCheckCharge:: D0 daughter of D*0 not found" << endmsg;   
	D0DauIter=D0dau->daughterIterator(); 
      }

      //look for its charged Kaon to tell the flavor	
      const BtaCandidate* D0Kdau=NULL;
      while(BtaCandidate*dau=D0DauIter())//find the D0 K+ daughter
	if(abs(dau->pdtEntry()->lundId())==abs(Pdt::lookup(PdtPdg::K_plus)->lundId()))
	  D0Kdau=dau;

      if(D0Kdau){
	if(D0Kdau->charge()==-1.)TagCQuarks++;//Kaon has a s quark --> came from c quark
	if(D0Kdau->charge()==1.)TagantiCQuarks++;//Kaon has a sbar quark --> came from cbar quark
      }else {//its a mode with a Kshort --> can't tell
	TagCQuarks++;
	TagantiCQuarks++;
      }
	
    }else if(TagD->pdtEntry()->lundId()==Pdt::lookup( PdtPdg::D_s_plus)->lundId()){
      TagCQuarks++;
      TagantiSQuarks++;
    }else if(TagD->pdtEntry()->lundId()==Pdt::lookup( PdtPdg::D_s_minus)->lundId()){
      TagantiCQuarks++;
      TagSQuarks++;
    }else if(TagD->pdtEntry()->lundId()==Pdt::lookup( PdtPdg::Lambda_c_plus)->lundId()){
      TagCQuarks++;
    }else if(TagD->pdtEntry()->lundId()==Pdt::lookup( PdtPdg::anti_Lambda_c_minus)->lundId()){
      TagantiCQuarks++;
    }else ErrMsg(fatal) <<" DRecoCheckCharge:: TagD not recognized" << endmsg;   


    //--------------------------------------------------------
    //count the s quarks
    //-------------------------------------------------------
    BtaCandidate* TagK=CandListIter();//should be second one------------>Relies on number of members in list
    if(!TagK)ErrMsg(fatal) <<" DRecoCheckCharge:: K not found" << endmsg;  
    totalDKXcharge+=TagK->charge();//not sure about this charge assignment in Lamda_c modes	

    if(TagK->pdtEntry()->lundId()==Pdt::lookup(PdtPdg::K_plus)->lundId())
      TagantiSQuarks++;
    else if(TagK->pdtEntry()->lundId()==Pdt::lookup(PdtPdg::K_minus)->lundId())
      TagSQuarks++;
    else if(TagK->pdtEntry()->lundId()==Pdt::lookup(PdtPdg::K_S0)->lundId()){//can't tell content
      TagantiSQuarks++;
      TagSQuarks++;
    }else if(TagK->pdtEntry()->lundId()==Pdt::lookup( PdtPdg::gamma)->lundId()){//dummy gamma for Ds modes
      //leave s quarks at 0
    }else if(abs(TagK->pdtEntry()->lundId())==abs(Pdt::lookup( PdtPdg::Xi_b_minus)->lundId())){//LambdaC tag: must have a K0
      TagSQuarks++;
      TagantiSQuarks++;
    }else if(TagK->pdtEntry()->lundId()==Pdt::lookup( PdtPdg::Xi_b0)->lundId()){//LambdaC tag: p+ K-
      TagSQuarks++;
    }else if(TagK->pdtEntry()->lundId()==Pdt::lookup( PdtPdg::anti_Xi_b0)->lundId()){//LambdaC tag: p- K+
      TagantiSQuarks++;
    }else{ ErrMsg(fatal) <<" DRecoCheckCharge:: TagK not recognized  Klund="<<TagK->pdtEntry()->lundId() << endmsg; }

    //--------------------------------------------------------
    //complete the charge
    //--------------------------------------------------------
    BtaCandidate* X=CandListIter();//should be 3rd one------------>Relies on number of members in list
    if(!X)ErrMsg(fatal) <<" DRecoCheckCharge:: X not found" << endmsg;  
    if(X->nDaughters()>0){
      HepAListIterator<BtaCandidate> XIter=X->daughterIterator();//otherwise will return pdt charge
      while(BtaCandidate*xdau=XIter())
	totalDKXcharge+=xdau->charge();	
    }else totalDKXcharge+=X->charge();	
    
    if(fabs(totalDKXcharge)==0.)
      _scalers.sum("totalDKXcharge=0");
    if(fabs(totalDKXcharge)==2.)
      _scalers.sum("totalDKXcharge=2");
    if(fabs(totalDKXcharge)==3.)
      _scalers.sum("totalDKXcharge=3");
    
    bool pass=0;
    if(fabs(totalDKXcharge)==1.){//charge of DKX determines flavor of Ds
      _scalers.sum("totalDKXcharge=1"); 
      
      //determine the signal side quark content based on the DKX charge
      if(totalDKXcharge==-1.){//--> other side has Ds+ = (c-sbar)
	SigCQuarks=1;SigSQuarks=0;
        SigantiCQuarks=0;SigantiSQuarks=1;
      }
      if(totalDKXcharge==1.){//--> other side has Ds- = (cbar-s)
	SigCQuarks=0;SigSQuarks=1;
	SigantiCQuarks=1;SigantiSQuarks=0;
      }
      
      
      if(!SelectWrongSign.value()){//Right-Sign --> Tag has oposite quark content to Ds
	if(TagCQuarks==SigantiCQuarks&&TagantiSQuarks==SigSQuarks){
	  _scalers.sum("quark check"); 
	  pass=1;
	}
	if(TagantiCQuarks==SigCQuarks&&TagSQuarks==SigantiSQuarks){
	  _scalers.sum("anti-quark check"); 
	  pass=1;
	}

      }
      if(SelectWrongSign.value()){//Wrong-Sign --> Tag has same quark content to Ds
	if(TagCQuarks==SigCQuarks&&TagSQuarks==SigSQuarks){
	  _scalers.sum("quark check"); 
	  pass=1;
	}
	if(TagantiCQuarks==SigantiCQuarks&&TagantiSQuarks==SigantiSQuarks){
	  _scalers.sum("anti-quark check"); 
	  pass=1;
	}
      }

    }
   

    if(pass){
      *DRecoEvtCandidateList += new HepAList<BtaCandidate>(*candlist);
      _scalers.sum("final lists");	       
    }else
      HepAListDeleteAll(*candlist);//delete the BtaCandidates in this list because the list will be deleted below
    
    
  }


  HepAListDeleteAll(TempEvtCandList);
  return AppResult::OK;
}


AppResult DRecoCheckCharge::endJob( AbsEvent* anEvent )
{
  ostream& o=cout;
  _scalers.print(o);
  o << endl;

  return AppResult::OK;
}

