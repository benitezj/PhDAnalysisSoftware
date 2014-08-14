//--------------------------------------------------------------------------
// 
// $Id: BtoDPiPiPiTagFilter.cc,v 1.2 2007/12/19 18:27:11 avtelnov Exp $
//
//------------------------------------------------------------------------
//-----------------------
// This Class's Header --
//-----------------------
#include "FilterTools/BtoDPiPiPiTagFilter.hh" 
//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//----------------
// Constructors --
//----------------

BtoDPiPiPiTagFilter::BtoDPiPiPiTagFilter(const char* const theName, 
					 const char* const theDescription )
  : TagFilterModule( theName, theDescription )
  , _BchD3PiList  ("BchD3PiList",  this, "BchToD0PiPiPiDefault")
  , _BchDs3PiList ("BchDs3PiList", this, "BchToDstar0PiPiPiDefault")
  , _B0D3PiList   ("B0D3PiList",   this, "B0ToDchPiPiPiDefault")
  , _B0Ds3PiList  ("B0Ds3PiList",  this, "B0ToDstarPiPiPiDefault")
  , _DPiPiPiMode("DPiPiPiMode")
  , _eventLog(0)
  , _nReadEvents(0) 
  , _nBchD3PiPassed(0) 
  , _nBchDs3PiPassed(0) 
  , _nB0D3PiPassed(0) 
  , _nB0Ds3PiPassed(0) 
  , _nPassed(0) 
{
  commands()->append(& _BchD3PiList);
  commands()->append(& _BchDs3PiList);
  commands()->append(& _B0D3PiList);
  commands()->append(& _B0Ds3PiList);
  _DPiPiPiEventBlock.addVariable(_DPiPiPiMode);
}

//--------------
// Destructor --
//--------------

BtoDPiPiPiTagFilter::~BtoDPiPiPiTagFilter( ) 
{}

//--------------
// Operations --
//--------------
AppResult
BtoDPiPiPiTagFilter::beginJob( AbsEvent* anEvent ) 
{
  ErrMsg(trace) << name() << ": begin job" << endmsg;
  return AppResult::OK;
}

AppResult
BtoDPiPiPiTagFilter::event( AbsEvent* anEvent ) 
{
  _eventLog++;
  _nReadEvents++;

  static const IfdStrKey myEventDataKey("myEvent");
if (! UsrIfd<UsrEventBlock>::put(anEvent,&_DPiPiPiEventBlock, myEventDataKey))
    ErrMsg(trace) << "Can't put myEvent " << endmsg;


  bool passed = false;

  // get the lists
  HepAList<BtaCandidate>* theBchD3PiList; 
  getTmpAList(anEvent,  theBchD3PiList,  _BchD3PiList.value());
  assert(theBchD3PiList != 0);

  HepAList<BtaCandidate>* theB0D3PiList; 
  getTmpAList(anEvent,  theB0D3PiList,  _B0D3PiList.value());
  assert(theB0D3PiList != 0);

  HepAList<BtaCandidate>* theB0Ds3PiList; 
  getTmpAList(anEvent,  theB0Ds3PiList,  _B0Ds3PiList.value());
  assert(theB0Ds3PiList != 0);

  HepAList<BtaCandidate>* theBchDs3PiList; 
  getTmpAList(anEvent,  theBchDs3PiList,  _BchDs3PiList.value());
  assert(theBchDs3PiList != 0);

  _DPiPiPiMode = 0;


  if ( theBchDs3PiList->length() ) {
    _nBchDs3PiPassed++;
    _DPiPiPiMode += 1;
    passed = true;
  }

  if ( theBchD3PiList->length() ) {
    _nBchD3PiPassed++;
    _DPiPiPiMode += 10;
    passed = true;
  }
  if ( theB0D3PiList->length() ) {
    _nB0D3PiPassed++;
    _DPiPiPiMode += 100;
    passed = true;
  }

  if ( theB0Ds3PiList->length() ) {
    _nB0Ds3PiPassed++;
    _DPiPiPiMode += 1000;
    passed = true;
  }

  setPassed( passed  );

  if (passed) {
         if ( ! _DPiPiPiEventBlock.put(_DPiPiPiMode) )
                  ErrMsg(error) << "Can't put usr variable " 
                  << _DPiPiPiMode.name() 
                  << " into UsrEventBlock DPiPiPiEventData" << endmsg;
    _nPassed++; 
  }
  return  AppResult::OK;
}



AppResult
BtoDPiPiPiTagFilter::endJob( AbsEvent* anEvent ) 
{

  cout << " -----------------------------------------------------"<<endl;
  cout << name()  << " end job: " << _nPassed << " out of " 
       << _nReadEvents << " passed" << endl;
  cout << " -----------------------------------------------------"<<endl;
  cout << name()  <<" end job: "<< _nBchD3PiPassed <<" B+ (D 3pi) out of " 
       << _nReadEvents << " passed" << endl;
  cout << " -----------------------------------------------------"<<endl;
  cout << name()  <<" end job: "<< _nBchDs3PiPassed <<" B+ (D* 3pi) out of " 
       << _nReadEvents << " passed" << endl;
  cout << " -----------------------------------------------------"<<endl;
  cout << name()  <<" end job: "<< _nB0D3PiPassed << " B0 (D 3pi) out of " 
       << _nReadEvents << " passed" << endl;
  cout << " -----------------------------------------------------"<<endl;
  cout << name()  <<" end job: "<< _nB0Ds3PiPassed << " B0 (D* 3pi) out of " 
       << _nReadEvents << " passed" << endl;

  cout<<" *******************************************************"<<endl;
  cout<<" ELAPSED TIME: ";
  //theWatch->Print();
  cout<<" *******************************************************"<<endl;
  //delete theWatch;
  return AppResult::OK;
}

void 
BtoDPiPiPiTagFilter::help(int argc, char** argv){
  cout << "Tagging module for finding events for B- -> D**0 pi-: " << endl;
  AppModule::help(argc,argv);
}


ostream& 
BtoDPiPiPiTagFilter::print(ostream& os) const 
{
  os << "Summary:\n";
  if ( _eventLog > 0 ) {
    os << " ok \n";
  } else {
    os << "\t No events processed\n";
  }
  return os;
}
