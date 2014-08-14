//--------------------------------------------------------------------------
// 
// $Id: BtoDPiPiTagFilter.cc,v 1.9 2007/12/19 18:27:11 avtelnov Exp $
//
//------------------------------------------------------------------------
//-----------------------
// This Class's Header --
//-----------------------
#include "FilterTools/BtoDPiPiTagFilter.hh" 
//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//----------------
// Constructors --
//----------------

BtoDPiPiTagFilter::BtoDPiPiTagFilter(const char* const theName, 
					 const char* const theDescription )
  : TagFilterModule( theName, theDescription )
  , _BchDPPList  ("BchDPPList",  this, "BchToDchMainPiPi")
  , _BchDsPPList ("BchDsPPList", this, "BchToDstarMainPiPi")
  , _B0DPPList   ("B0DPPList",   this, "B0ToD0MainPiPi")
  , _B0DsPPList  ("B0DsPPList",  this, "B0ToDstar0MainPiPi")
  , _DPiPiMode("DPiPiMode")
  , _eventLog(0)
  , _nBchDPPPassed(0) 
  , _nBchDsPPPassed(0)
  , _nB0DPPPassed(0)
  , _nB0DsPPPassed(0)
  , _nPassed(0)
{
  commands()->append(& _BchDPPList);
  commands()->append(& _BchDsPPList);
  commands()->append(& _B0DPPList);
  commands()->append(& _B0DsPPList);
  _DPiPiEventBlock.addVariable(_DPiPiMode);
}

//--------------
// Destructor --
//--------------

BtoDPiPiTagFilter::~BtoDPiPiTagFilter( ) 
{}

//--------------
// Operations --
//--------------
AppResult
BtoDPiPiTagFilter::beginJob( AbsEvent* anEvent ) 
{
  ErrMsg(trace) << name() << ": begin job" << endmsg;
  return AppResult::OK;
}


AppResult
BtoDPiPiTagFilter::event( AbsEvent* anEvent ) 
{
  _eventLog++;
  _nReadEvents++;

  static const IfdStrKey myEventDataKey("myEventData");
if (! UsrIfd<UsrEventBlock>::put(anEvent,&_DPiPiEventBlock, myEventDataKey))
    ErrMsg(trace) << "Can't put myEventData " << endmsg;


  // if (_nReadEvents == 1) 
  //  theWatch = new TStopwatch();

  bool passed = false;

  // get the lists
  HepAList<BtaCandidate>* theBchDPPList; 
  getTmpAList(anEvent,  theBchDPPList,  _BchDPPList.value());
  assert(theBchDPPList != 0);

  HepAList<BtaCandidate>* theB0DPPList; 
  getTmpAList(anEvent,  theB0DPPList,  _B0DPPList.value());
  assert(theB0DPPList != 0);

  HepAList<BtaCandidate>* theB0DsPPList; 
  getTmpAList(anEvent,  theB0DsPPList,  _B0DsPPList.value());
  assert(theB0DsPPList != 0);

  HepAList<BtaCandidate>* theBchDsPPList; 
  getTmpAList(anEvent,  theBchDsPPList,  _BchDsPPList.value());
  assert(theBchDsPPList != 0);

  _DPiPiMode = 0;


  if ( theBchDsPPList->length() ) {
    _nBchDsPPPassed++;
    _DPiPiMode += 1;
    passed = true;
  }

  if ( theBchDPPList->length() ) {
    _nBchDPPPassed++;
    _DPiPiMode += 10;
    passed = true;
  }
  if ( theB0DPPList->length() ) {
//    BtaCandidate* candid;
//    HepAListIterator<BtaCandidate> iter_B0DPP(*theB0DPPList);
//    while ( 0 != ( candid = iter_B0DPP()) )
//      {
//      PdtLund::LundType lund = candid->pdtEntry()->lundId();
//      if( lund == 511){_D0PiPiB0++;}
//      if( lund == -511){_D0PiPiAB0++;}
//      }    
    _nB0DPPPassed++;
    _DPiPiMode += 100;
    passed = true;
  }

  if ( theB0DsPPList->length() ) {
//    BtaCandidate* candid;
//    HepAListIterator<BtaCandidate> iter_B0DsPP(*theB0DsPPList);
//    while ( 0 != ( candid = iter_B0DsPP()) )
//      {
//      PdtLund::LundType lund = candid->pdtEntry()->lundId();
//      if( lund == 511){_DS0PiPiB0++;}
//      if( lund == -511){_DS0PiPiAB0++;}
//      }

    _nB0DsPPPassed++;
    _DPiPiMode += 1000;
    passed = true;
  }

  setPassed( passed  );

  if (passed) {
         if ( ! _DPiPiEventBlock.put(_DPiPiMode) )
                  ErrMsg(error) << "Can't put usr variable " << _DPiPiMode.name() 
                  << " into UsrEventBlock DPiPiEventData" << endmsg;
    _nPassed++; 
  }
  return  AppResult::OK;
}



AppResult
BtoDPiPiTagFilter::endJob( AbsEvent* anEvent ) 
{
//  cout << "D0PiPiB0:" << _D0PiPiB0 <<" D0PiPiAB0:" << _D0PiPiAB0 <<endl;
//  cout << "DS0PiPiB0:"<< _DS0PiPiB0 <<" DS0PiPiAB0:" << _DS0PiPiAB0<<endl;



  cout << " -----------------------------------------------------"<<endl;
  cout << name()  << " end job: " << _nPassed << " out of " 
       << _nReadEvents << " passed" << endl;
  cout << " -----------------------------------------------------"<<endl;
  cout << name()  <<" end job: "<< _nBchDPPPassed <<" B+ (D pi pi) out of " 
       << _nReadEvents << " passed" << endl;
  cout << " -----------------------------------------------------"<<endl;
  cout << name()  <<" end job: "<< _nBchDsPPPassed <<" B+ (D* pi pi) out of " 
       << _nReadEvents << " passed" << endl;
  cout << " -----------------------------------------------------"<<endl;
  cout << name()  <<" end job: "<< _nB0DPPPassed << " B0 (D0 pi pi) out of " 
       << _nReadEvents << " passed" << endl;
  cout << " -----------------------------------------------------"<<endl;
  cout << name()  <<" end job: "<< _nB0DsPPPassed << " B0 (D*0 pi pi) out of " 
       << _nReadEvents << " passed" << endl;

  cout<<" *******************************************************"<<endl;
  cout<<" ELAPSED TIME: ";
  //theWatch->Print();
  cout<<" *******************************************************"<<endl;
  //delete theWatch;
  return AppResult::OK;
}

void 
BtoDPiPiTagFilter::help(int argc, char** argv){
  cout << "Tagging module for finding events for B- -> D**0 pi-: " << endl;
  AppModule::help(argc,argv);
}


ostream& 
BtoDPiPiTagFilter::print(ostream& os) const 
{
  os << "Summary:\n";
  if ( _eventLog > 0 ) {
    os << " ok \n";
  } else {
    os << "\t No events processed\n";
  }
  return os;
}
