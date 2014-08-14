
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "DRecoPrintTree.hh"

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
#include "CLHEP/Alist/AList.h"
#include "CLHEP/Alist/AIterator.h"
#include "ErrLogger/ErrLog.hh"
#include "ProxyDict/Ifd.hh"
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
DRecoPrintTree::DRecoPrintTree( const char* const theName,const char* const theDescription )
  : TagFilterModule( theName, theDescription )
  , PrintMCTruth("PrintMCTruth",this,0)
  , DefaultRecoPrint("DefaultRecoPrint",this,0)
  , _scalers("DRecoPrintTree")
{

  commands()->append( &PrintMCTruth);
  commands()->append( &DefaultRecoPrint);
}

//--------------
// Destructor --
//--------------
DRecoPrintTree::~DRecoPrintTree()
{}

//--------------
// Operations --
//--------------
AppResult DRecoPrintTree::beginJob( AbsEvent* anEvent )
{ 
  return AppResult::OK;
}

AppResult DRecoPrintTree::event( AbsEvent* anEvent )
{
  _scalers.sum("#  events");


  //retreive the event candidates
  HepAList< HepAList<BtaCandidate> >* DRecoEvtCandidateList = Ifd<HepAList< HepAList<BtaCandidate> > >::get(anEvent,"DRecoEvtCandidateList");
  if (DRecoEvtCandidateList == 0)  ErrMsg(fatal) <<"DRecoEvtCandidateList  not found" << endmsg;   
  if (DRecoEvtCandidateList->length()==0)return AppResult::OK;

  
  if(PrintMCTruth.value()){
    cout<<"----------Begin print MCTruth for this event-------------"<<endl;
    HepAList< BtaCandidate >* truthlist = Ifd<HepAList< BtaCandidate > >::get(anEvent,"MCTruth");
    if (truthlist == 0)  ErrMsg(fatal) <<"truthlist  not found" << endmsg; 
    HepAListIterator< BtaCandidate > truthListIter(*truthlist); 
    while (BtaCandidate*vphoton=truthListIter()){
      if(vphoton->pdtEntry()->lundId()==10022){		  
	if(DefaultRecoPrint.value())
	  cout<<CandPrinter.print(*vphoton);
	else
	  printCand(vphoton); 
	
	break;
      }
    }
    
  }

  cout<<"----------Begin print Reco for this event-------------"<<endl;


  int listnumber=0;
  HepAListIterator< HepAList<BtaCandidate> > ListIter(*DRecoEvtCandidateList); 
  while (HepAList<BtaCandidate> * candlist = ListIter()){     
    _scalers.sum("# initial lists");
    listnumber++;	 
    
    //
    cout<<"---------begin print of candlist # "<<listnumber<<"  address="<<candlist<<" ----------------"<<endl;
    
    HepAListIterator<BtaCandidate> CandListIter(*candlist); 
    while(BtaCandidate* cand = CandListIter()){   
      if(DefaultRecoPrint.value()){//use BtaPrintTree format	
	cout<<CandPrinter.print( *cand);
	
      }else {//use my own format
	
	printCand(cand);
 
	HepAListIterator<BtaCandidate> dauIter1=cand->daughterIterator();
	while(BtaCandidate* dau1 = dauIter1()){
	  cout<<"   ";
	  printCand(dau1);

	  HepAListIterator<BtaCandidate> dauIter2=dau1->daughterIterator();
	  while(BtaCandidate* dau2 = dauIter2()){
	    cout<<"      ";
	    printCand(dau2);

	    HepAListIterator<BtaCandidate> dauIter3=dau2->daughterIterator();
	    while(BtaCandidate* dau3 = dauIter3()){
	      cout<<"         ";
	      printCand(dau3);

	      HepAListIterator<BtaCandidate> dauIter4=dau3->daughterIterator();
	      while(BtaCandidate* dau4= dauIter4()){
		cout<<"            ";
		printCand(dau4);
	      }
	    }
	  }
	}

      }
    }
     
  }

  return AppResult::OK;
}


void DRecoPrintTree::printCand(BtaCandidate* c){
  cout<<" name="<<c->pdtEntry()->name()
      <<" uid="<<c->uid()
      <<" address="<<c
      <<" ndaus="<<c->nDaughters()<<endl;  
}

AppResult DRecoPrintTree::endJob( AbsEvent* anEvent )
{
  ostream& o=cout;
  _scalers.print(o);
  o << endl;

  return AppResult::OK;
}




