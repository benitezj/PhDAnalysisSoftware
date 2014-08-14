
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "DRecoTagInterface.hh"

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
#include "CLHEP/Alist/AIterator.h"
#include "ErrLogger/ErrLog.hh"
#include "ProxyDict/IfdStrKey.hh"
#include "ProxyDict/IfdKey.hh"
#include "ProxyDict/Ifd.hh"
#include "ProxyDict/IfdHepAListProxy.hh"
#include "BetaCoreTools/BtaBooster.hh"

#include "TrkBase/TrkAbsFit.hh"
#include "VtxBase/BtaAbsVertexer.hh"
#include "VtxFitter/VtxFitterOper.hh"
#include "VtxBase/VtxAbsAlgorithm.hh"
#include "Beta/BtaAbsVertex.hh"
#include "ProbTools/probab.hh"

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
DRecoTagInterface::DRecoTagInterface( const char* const theName,const char* const theDescription )
  : TagFilterModule( theName, theDescription )
  , eventInfoList( "eventInfoList", this, "Default" )
  , TagList1( "TagList1", this, "0" )
  , TagList2( "TagList2", this, "0" )
  , TagList3( "TagList3", this, "0" )
  , TagList4( "TagList4", this, "0" )
  , TagList5( "TagList5", this, "0" )
  , TagList6( "TagList6", this, "0" )
  , TagList7( "TagList7", this, "0" )
  , TagList8( "TagList8", this, "0" )
  , TagList9( "TagList9", this, "0" )
  , TagList10( "TagList10", this, "0" )
  , TagList11( "TagList11", this, "0" )
  , TagList12( "TagList12", this, "0" )
  , TagList13( "TagList13", this, "0" )
  , TagList14( "TagList14", this, "0" )
  , TagList15( "TagList15", this, "0" )
  , TagList16( "TagList16", this, "0" )
  , TagList17( "TagList17", this, "0" )
  , TagList18( "TagList18", this, "0" )
  , TagList19( "TagList19", this, "0" )
  , TagList20( "TagList20", this, "0" )
  , TagList21( "TagList21", this, "0" )
  , TagList22( "TagList22", this, "0" )
  , TagList23( "TagList23", this, "0" )
  , TagList24( "TagList24", this, "0" )
  , TagList25( "TagList25", this, "0" )
  , TagList26( "TagList26", this, "0" )
  , TagList27( "TagList27", this, "0" )
  , TagList28( "TagList28", this, "0" )
  , TagList29( "TagList29", this, "0" )
  , TagList30( "TagList30", this, "0" )

  , TagListFit1( "TagListFit1", this, 0)
  , TagListFit2( "TagListFit2", this, 0)
  , TagListFit3( "TagListFit3", this, 0)
  , TagListFit4( "TagListFit4", this, 0)
  , TagListFit5( "TagListFit5", this, 0)
  , TagListFit6( "TagListFit6", this, 0)
  , TagListFit7( "TagListFit7", this, 0)
  , TagListFit8( "TagListFit8", this, 0)
  , TagListFit9( "TagListFit9", this, 0)
  , TagListFit10( "TagListFit10", this, 0)
  , TagListFit11( "TagListFit11", this, 0)
  , TagListFit12( "TagListFit12", this, 0)
  , TagListFit13( "TagListFit13", this, 0)
  , TagListFit14( "TagListFit14", this, 0)
  , TagListFit15( "TagListFit15", this, 0)
  , TagListFit16( "TagListFit16", this, 0)
  , TagListFit17( "TagListFit17", this, 0)
  , TagListFit18( "TagListFit18", this, 0)
  , TagListFit19( "TagListFit19", this, 0)
  , TagListFit20( "TagListFit20", this, 0)
  , TagListFit21( "TagListFit21", this, 0)
  , TagListFit22( "TagListFit22", this, 0)
  , TagListFit23( "TagListFit23", this, 0)
  , TagListFit24( "TagListFit24", this, 0)
  , TagListFit25( "TagListFit25", this, 0)
  , TagListFit26( "TagListFit26", this, 0)
  , TagListFit27( "TagListFit27", this, 0)
  , TagListFit28( "TagListFit28", this, 0)
  , TagListFit29( "TagListFit29", this, 0)
  , TagListFit30( "TagListFit30", this, 0)

  , TagListMass1( "TagListMass1", this, 0.)
  , TagListMass2( "TagListMass2", this, 0.)
  , TagListMass3( "TagListMass3", this, 0.)
  , TagListMass4( "TagListMass4", this, 0.)
  , TagListMass5( "TagListMass5", this, 0.)
  , TagListMass6( "TagListMass6", this, 0.)
  , TagListMass7( "TagListMass7", this, 0.)
  , TagListMass8( "TagListMass8", this, 0.)
  , TagListMass9( "TagListMass9", this, 0.)
  , TagListMass10( "TagListMass10", this, 0.)
  , TagListMass11( "TagListMass11", this, 0.)
  , TagListMass12( "TagListMass12", this, 0.)
  , TagListMass13( "TagListMass13", this, 0.)
  , TagListMass14( "TagListMass14", this, 0.)
  , TagListMass15( "TagListMass15", this, 0.)
  , TagListMass16( "TagListMass16", this, 0.)
  , TagListMass17( "TagListMass17", this, 0.)
  , TagListMass18( "TagListMass18", this, 0.)
  , TagListMass19( "TagListMass19", this, 0.)
  , TagListMass20( "TagListMass20", this, 0.)
  , TagListMass21( "TagListMass21", this, 0.)
  , TagListMass22( "TagListMass22", this, 0.)
  , TagListMass23( "TagListMass23", this, 0.)
  , TagListMass24( "TagListMass24", this, 0.)
  , TagListMass25( "TagListMass25", this, 0.)
  , TagListMass26( "TagListMass26", this, 0.)
  , TagListMass27( "TagListMass27", this, 0.)
  , TagListMass28( "TagListMass28", this, 0.)
  , TagListMass29( "TagListMass29", this, 0.)
  , TagListMass30( "TagListMass30", this, 0.)

  , TagListMassCut1( "TagListMassCut1", this, 0.)
  , TagListMassCut2( "TagListMassCut2", this, 0.)
  , TagListMassCut3( "TagListMassCut3", this, 0.)
  , TagListMassCut4( "TagListMassCut4", this, 0.)
  , TagListMassCut5( "TagListMassCut5", this, 0.)
  , TagListMassCut6( "TagListMassCut6", this, 0.)
  , TagListMassCut7( "TagListMassCut7", this, 0.)
  , TagListMassCut8( "TagListMassCut8", this, 0.)
  , TagListMassCut9( "TagListMassCut9", this, 0.)
  , TagListMassCut10( "TagListMassCut10", this, 0.)
  , TagListMassCut11( "TagListMassCut11", this, 0.)
  , TagListMassCut12( "TagListMassCut12", this, 0.)
  , TagListMassCut13( "TagListMassCut13", this, 0.)
  , TagListMassCut14( "TagListMassCut14", this, 0.)
  , TagListMassCut15( "TagListMassCut15", this, 0.)
  , TagListMassCut16( "TagListMassCut16", this, 0.)
  , TagListMassCut17( "TagListMassCut17", this, 0.)
  , TagListMassCut18( "TagListMassCut18", this, 0.)
  , TagListMassCut19( "TagListMassCut19", this, 0.)
  , TagListMassCut20( "TagListMassCut20", this, 0.)
  , TagListMassCut21( "TagListMassCut21", this, 0.)
  , TagListMassCut22( "TagListMassCut22", this, 0.)
  , TagListMassCut23( "TagListMassCut23", this, 0.)
  , TagListMassCut24( "TagListMassCut24", this, 0.)
  , TagListMassCut25( "TagListMassCut25", this, 0.)
  , TagListMassCut26( "TagListMassCut26", this, 0.)
  , TagListMassCut27( "TagListMassCut27", this, 0.)
  , TagListMassCut28( "TagListMassCut28", this, 0.)
  , TagListMassCut29( "TagListMassCut29", this, 0.)
  , TagListMassCut30( "TagListMassCut30", this, 0.)

  , TagListPstarCut1( "TagListPstarCut1", this, 0.)
  , TagListPstarCut2( "TagListPstarCut2", this, 0.)
  , TagListPstarCut3( "TagListPstarCut3", this, 0.)
  , TagListPstarCut4( "TagListPstarCut4", this, 0.)
  , TagListPstarCut5( "TagListPstarCut5", this, 0.)
  , TagListPstarCut6( "TagListPstarCut6", this, 0.)
  , TagListPstarCut7( "TagListPstarCut7", this, 0.)
  , TagListPstarCut8( "TagListPstarCut8", this, 0.)
  , TagListPstarCut9( "TagListPstarCut9", this, 0.)
  , TagListPstarCut10( "TagListPstarCut10", this, 0.)
  , TagListPstarCut11( "TagListPstarCut11", this, 0.)
  , TagListPstarCut12( "TagListPstarCut12", this, 0.)
  , TagListPstarCut13( "TagListPstarCut13", this, 0.)
  , TagListPstarCut14( "TagListPstarCut14", this, 0.)
  , TagListPstarCut15( "TagListPstarCut15", this, 0.)
  , TagListPstarCut16( "TagListPstarCut16", this, 0.)
  , TagListPstarCut17( "TagListPstarCut17", this, 0.)
  , TagListPstarCut18( "TagListPstarCut18", this, 0.)
  , TagListPstarCut19( "TagListPstarCut19", this, 0.)
  , TagListPstarCut20( "TagListPstarCut20", this, 0.)
  , TagListPstarCut21( "TagListPstarCut21", this, 0.)
  , TagListPstarCut22( "TagListPstarCut22", this, 0.)
  , TagListPstarCut23( "TagListPstarCut23", this, 0.)
  , TagListPstarCut24( "TagListPstarCut24", this, 0.)
  , TagListPstarCut25( "TagListPstarCut25", this, 0.)
  , TagListPstarCut26( "TagListPstarCut26", this, 0.)
  , TagListPstarCut27( "TagListPstarCut27", this, 0.)
  , TagListPstarCut28( "TagListPstarCut28", this, 0.)
  , TagListPstarCut29( "TagListPstarCut29", this, 0.)
  , TagListPstarCut30( "TagListPstarCut30", this, 0.)

  , TagListProbminCut1( "TagListProbminCut1", this, 0.00001)
  , TagListProbminCut2( "TagListProbminCut2", this, 0.00001)
  , TagListProbminCut3( "TagListProbminCut3", this, 0.00001)
  , TagListProbminCut4( "TagListProbminCut4", this, 0.00001)
  , TagListProbminCut5( "TagListProbminCut5", this, 0.00001)
  , TagListProbminCut6( "TagListProbminCut6", this, 0.00001)
  , TagListProbminCut7( "TagListProbminCut7", this, 0.00001)
  , TagListProbminCut8( "TagListProbminCut8", this, 0.00001)
  , TagListProbminCut9( "TagListProbminCut9", this, 0.00001)
  , TagListProbminCut10( "TagListProbminCut10", this, 0.00001)
  , TagListProbminCut11( "TagListProbminCut11", this, 0.00001)
  , TagListProbminCut12( "TagListProbminCut12", this, 0.00001)
  , TagListProbminCut13( "TagListProbminCut13", this, 0.00001)
  , TagListProbminCut14( "TagListProbminCut14", this, 0.00001)
  , TagListProbminCut15( "TagListProbminCut15", this, 0.00001)
  , TagListProbminCut16( "TagListProbminCut16", this, 0.00001)
  , TagListProbminCut17( "TagListProbminCut17", this, 0.00001)
  , TagListProbminCut18( "TagListProbminCut18", this, 0.00001)
  , TagListProbminCut19( "TagListProbminCut19", this, 0.00001)
  , TagListProbminCut20( "TagListProbminCut20", this, 0.00001)
  , TagListProbminCut21( "TagListProbminCut21", this, 0.00001)
  , TagListProbminCut22( "TagListProbminCut22", this, 0.00001)
  , TagListProbminCut23( "TagListProbminCut23", this, 0.00001)
  , TagListProbminCut24( "TagListProbminCut24", this, 0.00001)
  , TagListProbminCut25( "TagListProbminCut25", this, 0.00001)
  , TagListProbminCut26( "TagListProbminCut26", this, 0.00001)
  , TagListProbminCut27( "TagListProbminCut27", this, 0.00001)
  , TagListProbminCut28( "TagListProbminCut28", this, 0.00001)
  , TagListProbminCut29( "TagListProbminCut29", this, 0.00001)
  , TagListProbminCut30( "TagListProbminCut30", this, 0.00001)

  ,          muonList( "muonList",          this, "muNNTight")
  ,      electronList( "electronList",      this, "PidLHElectrons")
  ,     requireLepton( "requireLepton",     this, 0.)

  , _scalers("DRecoTagInterface")
{

  commands()->append( &TagList1   );
  commands()->append( &TagList2   );
  commands()->append( &TagList3   );
  commands()->append( &TagList4   );
  commands()->append( &TagList5   );
  commands()->append( &TagList6   );
  commands()->append( &TagList7   );
  commands()->append( &TagList8   );
  commands()->append( &TagList9   );
  commands()->append( &TagList10  );
  commands()->append( &TagList11  );
  commands()->append( &TagList12  );
  commands()->append( &TagList13  );
  commands()->append( &TagList14  );
  commands()->append( &TagList15  );
  commands()->append( &TagList16  );
  commands()->append( &TagList17  );
  commands()->append( &TagList18  );
  commands()->append( &TagList19  );
  commands()->append( &TagList20  );
  commands()->append( &TagList21  );
  commands()->append( &TagList22  );
  commands()->append( &TagList23  );
  commands()->append( &TagList24  );
  commands()->append( &TagList25  );
  commands()->append( &TagList26  );
  commands()->append( &TagList27  );
  commands()->append( &TagList28  );
  commands()->append( &TagList29  );
  commands()->append( &TagList30  );

  commands()->append( &TagListFit1   );
  commands()->append( &TagListFit2   );
  commands()->append( &TagListFit3   );
  commands()->append( &TagListFit4   );
  commands()->append( &TagListFit5   );
  commands()->append( &TagListFit6   );
  commands()->append( &TagListFit7   );
  commands()->append( &TagListFit8   );
  commands()->append( &TagListFit9   );
  commands()->append( &TagListFit10  );
  commands()->append( &TagListFit11  );
  commands()->append( &TagListFit12  );
  commands()->append( &TagListFit13  );
  commands()->append( &TagListFit14  );
  commands()->append( &TagListFit15  );
  commands()->append( &TagListFit16  );
  commands()->append( &TagListFit17  );
  commands()->append( &TagListFit18  );
  commands()->append( &TagListFit19  );
  commands()->append( &TagListFit20  );
  commands()->append( &TagListFit21  );
  commands()->append( &TagListFit22  );
  commands()->append( &TagListFit23  );
  commands()->append( &TagListFit24  );
  commands()->append( &TagListFit25  );
  commands()->append( &TagListFit26  );
  commands()->append( &TagListFit27  );
  commands()->append( &TagListFit28  );
  commands()->append( &TagListFit29  );
  commands()->append( &TagListFit30  );

  commands()->append( &TagListMass1   );
  commands()->append( &TagListMass2   );
  commands()->append( &TagListMass3   );
  commands()->append( &TagListMass4   );
  commands()->append( &TagListMass5   );
  commands()->append( &TagListMass6   );
  commands()->append( &TagListMass7   );
  commands()->append( &TagListMass8   );
  commands()->append( &TagListMass9   );
  commands()->append( &TagListMass10  );
  commands()->append( &TagListMass11  );
  commands()->append( &TagListMass12  );
  commands()->append( &TagListMass13  );
  commands()->append( &TagListMass14  );
  commands()->append( &TagListMass15  );
  commands()->append( &TagListMass16  );
  commands()->append( &TagListMass17  );
  commands()->append( &TagListMass18  );
  commands()->append( &TagListMass19  );
  commands()->append( &TagListMass20  );
  commands()->append( &TagListMass21  );
  commands()->append( &TagListMass22  );
  commands()->append( &TagListMass23  );
  commands()->append( &TagListMass24  );
  commands()->append( &TagListMass25  );
  commands()->append( &TagListMass26  );
  commands()->append( &TagListMass27  );
  commands()->append( &TagListMass28  );
  commands()->append( &TagListMass29  );
  commands()->append( &TagListMass30  );


  commands()->append( &TagListMassCut1    );
  commands()->append( &TagListMassCut2    );
  commands()->append( &TagListMassCut3    );
  commands()->append( &TagListMassCut4    );
  commands()->append( &TagListMassCut5    );
  commands()->append( &TagListMassCut6    );
  commands()->append( &TagListMassCut7    );
  commands()->append( &TagListMassCut8    );
  commands()->append( &TagListMassCut9    );
  commands()->append( &TagListMassCut10   );
  commands()->append( &TagListMassCut11   );
  commands()->append( &TagListMassCut12   );
  commands()->append( &TagListMassCut13   );
  commands()->append( &TagListMassCut14   );
  commands()->append( &TagListMassCut15   );
  commands()->append( &TagListMassCut16   );
  commands()->append( &TagListMassCut17   );
  commands()->append( &TagListMassCut18   );
  commands()->append( &TagListMassCut19   );
  commands()->append( &TagListMassCut20   );
  commands()->append( &TagListMassCut21   );
  commands()->append( &TagListMassCut22   );
  commands()->append( &TagListMassCut23   );
  commands()->append( &TagListMassCut24   );
  commands()->append( &TagListMassCut25   );
  commands()->append( &TagListMassCut26   );
  commands()->append( &TagListMassCut27   );
  commands()->append( &TagListMassCut28   );
  commands()->append( &TagListMassCut29   );
  commands()->append( &TagListMassCut30   );

  commands()->append( &TagListPstarCut1   );
  commands()->append( &TagListPstarCut2   );
  commands()->append( &TagListPstarCut3   );
  commands()->append( &TagListPstarCut4   );
  commands()->append( &TagListPstarCut5   );
  commands()->append( &TagListPstarCut6   );
  commands()->append( &TagListPstarCut7   );
  commands()->append( &TagListPstarCut8   );
  commands()->append( &TagListPstarCut9   );
  commands()->append( &TagListPstarCut10  );
  commands()->append( &TagListPstarCut11  );
  commands()->append( &TagListPstarCut12  );
  commands()->append( &TagListPstarCut13  );
  commands()->append( &TagListPstarCut14  );
  commands()->append( &TagListPstarCut15  );
  commands()->append( &TagListPstarCut16  );
  commands()->append( &TagListPstarCut17  );
  commands()->append( &TagListPstarCut18  );
  commands()->append( &TagListPstarCut19  );
  commands()->append( &TagListPstarCut20  );
  commands()->append( &TagListPstarCut21  );
  commands()->append( &TagListPstarCut22  );
  commands()->append( &TagListPstarCut23  );
  commands()->append( &TagListPstarCut24  );
  commands()->append( &TagListPstarCut25  );
  commands()->append( &TagListPstarCut26  );
  commands()->append( &TagListPstarCut27  );
  commands()->append( &TagListPstarCut28  );
  commands()->append( &TagListPstarCut29  );
  commands()->append( &TagListPstarCut30  );

  commands()->append( &TagListProbminCut1   );
  commands()->append( &TagListProbminCut2   );
  commands()->append( &TagListProbminCut3   );
  commands()->append( &TagListProbminCut4   );
  commands()->append( &TagListProbminCut5   );
  commands()->append( &TagListProbminCut6   );
  commands()->append( &TagListProbminCut7   );
  commands()->append( &TagListProbminCut8   );
  commands()->append( &TagListProbminCut9   );
  commands()->append( &TagListProbminCut10  );
  commands()->append( &TagListProbminCut11  );
  commands()->append( &TagListProbminCut12  );
  commands()->append( &TagListProbminCut13  );
  commands()->append( &TagListProbminCut14  );
  commands()->append( &TagListProbminCut15  );
  commands()->append( &TagListProbminCut16  );
  commands()->append( &TagListProbminCut17  );
  commands()->append( &TagListProbminCut18  );
  commands()->append( &TagListProbminCut19  );
  commands()->append( &TagListProbminCut20  );
  commands()->append( &TagListProbminCut21  );
  commands()->append( &TagListProbminCut22  );
  commands()->append( &TagListProbminCut23  );
  commands()->append( &TagListProbminCut24  );
  commands()->append( &TagListProbminCut25  );
  commands()->append( &TagListProbminCut26  );
  commands()->append( &TagListProbminCut27  );
  commands()->append( &TagListProbminCut28  );
  commands()->append( &TagListProbminCut29  );
  commands()->append( &TagListProbminCut30  );

  commands()->append( &muonList );
  commands()->append( &electronList );
  commands()->append( &requireLepton );
  
}

//--------------
// Destructor --
//--------------
DRecoTagInterface::~DRecoTagInterface( )
{}

//--------------
// Operations --
//--------------
AppResult DRecoTagInterface::event( AbsEvent* anEvent )
{
  _scalers.sum("# events ");

  // pointer to the event summary info object
  HepAList< EventInfo >* eventInfoListPtr =  Ifd<HepAList< EventInfo > >::get(anEvent, eventInfoList.value());
  if (eventInfoListPtr == 0){
    ErrMsg(fatal) << "Could not locate event info list named "
                  << eventInfoList.value() << " in the event."
                  << " This is likely to be a configuration error." << endmsg;
  } 
  EventInfo* eventInfo = eventInfoListPtr->first();
  if (eventInfo == 0) {
    ErrMsg(fatal) << "Nothing in eventInfoList...aborting" << endmsg;
  }
  BtaBooster cmbooster(eventInfo->cmFrame()); 
  //the beam spot below has been smeared in MC and fixed error=10mu data/MC
  const BbrPointErr beamSpot = eventInfoListPtr->first()->beamSpot();

  // Create 
  HepAList< HepAList<BtaCandidate> >* EvtCandList = new HepAList< HepAList<BtaCandidate> >;
  if (EvtCandList == 0) ErrMsg(fatal) << "Failed to create a Evt list"	<< endmsg;

  // Aidan - moved proxy upstream to prevent subsequent modules aborting in the absence of EvtCandList
  IfdHepAListProxy< HepAList<BtaCandidate> >* proxy = new IfdHepAListProxy< HepAList<BtaCandidate> >(EvtCandList);
  if(proxy == 0) ErrMsg(fatal) << "Failed to create DRecoEvtCandidate list proxy" << endmsg;
  if( !Ifd<HepAList< HepAList<BtaCandidate> > >::put(anEvent, proxy, "DRecoEvtCandidateList"))
    ErrMsg(fatal) << "failed to add DRecoEvtCandidate list to event using proxy" << endmsg; 

  // Check to make sure there is at least one lepton in the event.
  // Only skip the event if requireLepton is set to 1 (default is 0)
  HepAList<BtaCandidate>* muonCandList = Ifd<HepAList< BtaCandidate > >::get(anEvent, muonList.value()) ;
  if(muonCandList==0) ErrMsg(fatal) << muonList.value() << " list not found" << endmsg ;
  HepAList<BtaCandidate>* electronCandList = Ifd<HepAList< BtaCandidate > >::get(anEvent, electronList.value()) ;
  if(electronCandList==0) ErrMsg(fatal) << electronList.value() << " list not found" << endmsg ;
  int nLeptons = muonCandList->length() + electronCandList->length() ;
  if(requireLepton.value()>0.5 && nLeptons==0) return AppResult::OK ; // Don't bother to reconstruct the rest of the event

  //iterate over the  requested tag lists
  int listcounter=0;
  HepAListIterator< AbsParmIfdStrKey > ListOfTagListIter(ListOfTagList);
  while( AbsParmIfdStrKey* lname = ListOfTagListIter()){
    listcounter++;//has to be here otherwise  skipping lists makes out of synch
    if(lname->valueString()=="0")continue;
    HepAList<BtaCandidate>* tagDlist  = Ifd<HepAList< BtaCandidate > >::get(anEvent, lname->value());
    if (tagDlist== 0) ErrMsg(fatal) << lname->value()<< " list not found" << endmsg;    
    _scalers.sum("#  lists");

    HepAListIterator<BtaCandidate> tagDlistIter(*tagDlist);
    while( BtaCandidate* tagd = tagDlistIter() ) {
          
      _scalers.sum("#  total tags");
      bool tagdisnew=0;
      
      //Fit vertex
      AbsParmBool* lfit=NULL;
      HepAListIterator<AbsParmBool> lfitIter(ListOfTagListFit);
      int thislistidfit=0;
      while(thislistidfit<listcounter){
	lfit=lfitIter();
	thislistidfit++;
      }
      if(thislistidfit!=listcounter) ErrMsg(fatal) << lname->value()<< " does not match with list of Fit values" << endmsg; 

      if(lfit->value()){//fit vertex
	tagd = new BtaCandidate(*tagd);//tagd is locked in list
	setBeamConstraint(*tagd, beamSpot);
	VtxFitterOper TagFitter(*tagd,VtxAbsAlgorithm::TreeFitter,VtxAbsAlgorithm::Standard);
	TagFitter.invalidateFit();
	TagFitter.fitAll();	
	delete tagd;
	tagd = new BtaCandidate(TagFitter.getFittedTree());
	if((tagd->decayVtx())->status()!=BtaAbsVertex::Success){delete tagd; continue;}
	//get probmin
	AbsParmDouble* lprobmincut=NULL;            
	HepAListIterator<AbsParmDouble> lprobmincutIter(ListOfTagListProbminCut);
	int thislistidprobmincut=0;
	while(thislistidprobmincut<listcounter){
	  lprobmincut=lprobmincutIter();
	  thislistidprobmincut++; 
	}
	if(thislistidprobmincut!=listcounter) ErrMsg(fatal) << lname->value()<< " does not match with list of Probmin values" << endmsg; 
	if(probab((int)tagd->decayVtx()->nDof(),tagd->decayVtx()->chiSquared()) < lprobmincut->value()){delete tagd; continue;}
	_scalers.sum("# vertex fit");
	tagdisnew=1;
      }

      //mass and p* for cutting on
      float tagmass=tagd->mass();
      //cut on the mass
      AbsParmDouble* lmass=NULL;
      HepAListIterator<AbsParmDouble> lmassIter(ListOfTagListMass);
      int thislistidm=0;
      while(thislistidm<listcounter){
	lmass=lmassIter();
	thislistidm++;
      }
      if(thislistidm!=listcounter) ErrMsg(fatal) << lname->value()<< " does not match with list of Mass values" << endmsg; 

      AbsParmDouble* lmasscut=NULL;       
      HepAListIterator<AbsParmDouble> lmasscutIter(ListOfTagListMassCut);
      int thislistidmcut=0;
      while(thislistidmcut<listcounter){
	lmasscut=lmasscutIter();
	thislistidmcut++;
      }
      if(thislistidmcut!=listcounter) ErrMsg(fatal) << lname->value()<< " does not match with list of Mass Cut values" << endmsg; 
         
      if(fabs(tagmass-lmass->value())>lmasscut->value()){ if(tagdisnew)delete tagd; continue;}
      _scalers.sum("#  masscut");

      
      //cut on the p*
      AbsParmDouble* lpcut=NULL;            
      HepAListIterator<AbsParmDouble> lpstarcutIter(ListOfTagListPstarCut);
      int thislistidpcut=0;
      while(thislistidpcut<listcounter){
	lpcut=lpstarcutIter();
	thislistidpcut++; 
      }
      if(thislistidpcut!=listcounter) ErrMsg(fatal) << lname->value()<< " does not match with list of Pstar cut values" << endmsg; 
      BtaCandidate boosttagd=cmbooster.boostTo(*tagd);
      if(boosttagd.p()<lpcut->value()){ if(tagdisnew)delete tagd; continue;}
      _scalers.sum("#  p* cut");

      //check that tagd is not a clone of another one already in list
      bool isclone=0;
      HepAListIterator< HepAList<BtaCandidate> > CloneEvtIter(*EvtCandList);
      while( HepAList<BtaCandidate>* cloneEvt=CloneEvtIter()){
	if(tagd->isCloneOf(*cloneEvt,1)) isclone=1;
      }      
      if(isclone){ if(tagdisnew)delete tagd; continue;}

      HepAList<BtaCandidate>* EvtCand = new HepAList<BtaCandidate>;
      if (EvtCand == 0) ErrMsg(fatal) << "Failed to create a EvtCand "	<< endmsg;
      
      *EvtCand += (tagdisnew ?  tagd : new BtaCandidate(*tagd) );
      *EvtCandList += EvtCand;
      _scalers.sum(lname->valueString());
      _scalers.sum("#  recoilCands");

    }    
  }

    


  return AppResult::OK;
}



AppResult DRecoTagInterface::beginJob( AbsEvent* anEvent )
{ 

  ListOfTagList+=TagList1; 
  ListOfTagList+=TagList2; 
  ListOfTagList+=TagList3; 
  ListOfTagList+=TagList4; 
  ListOfTagList+=TagList5; 
  ListOfTagList+=TagList6; 
  ListOfTagList+=TagList7; 
  ListOfTagList+=TagList8; 
  ListOfTagList+=TagList9; 
  ListOfTagList+=TagList10; 
  ListOfTagList+=TagList11; 
  ListOfTagList+=TagList12; 
  ListOfTagList+=TagList13; 
  ListOfTagList+=TagList14; 
  ListOfTagList+=TagList15; 
  ListOfTagList+=TagList16; 
  ListOfTagList+=TagList17; 
  ListOfTagList+=TagList18; 
  ListOfTagList+=TagList19; 
  ListOfTagList+=TagList20; 
  ListOfTagList+=TagList21; 
  ListOfTagList+=TagList22; 
  ListOfTagList+=TagList23; 
  ListOfTagList+=TagList24; 
  ListOfTagList+=TagList25; 
  ListOfTagList+=TagList26; 
  ListOfTagList+=TagList27; 
  ListOfTagList+=TagList28; 
  ListOfTagList+=TagList29; 
  ListOfTagList+=TagList30; 
  HepAListIterator< AbsParmIfdStrKey > ListOfTagListIter(ListOfTagList);
  while( AbsParmIfdStrKey* lname = ListOfTagListIter())
    if(lname->valueString()!="0")cout<<" Tag-D list: "<<lname->command()<<"   Name= "<<lname->valueString()<<endl;



  ListOfTagListFit+=TagListFit1; 
  ListOfTagListFit+=TagListFit2; 
  ListOfTagListFit+=TagListFit3; 
  ListOfTagListFit+=TagListFit4; 
  ListOfTagListFit+=TagListFit5; 
  ListOfTagListFit+=TagListFit6; 
  ListOfTagListFit+=TagListFit7; 
  ListOfTagListFit+=TagListFit8; 
  ListOfTagListFit+=TagListFit9; 
  ListOfTagListFit+=TagListFit10; 
  ListOfTagListFit+=TagListFit11; 
  ListOfTagListFit+=TagListFit12; 
  ListOfTagListFit+=TagListFit13; 
  ListOfTagListFit+=TagListFit14; 
  ListOfTagListFit+=TagListFit15; 
  ListOfTagListFit+=TagListFit16; 
  ListOfTagListFit+=TagListFit17; 
  ListOfTagListFit+=TagListFit18; 
  ListOfTagListFit+=TagListFit19; 
  ListOfTagListFit+=TagListFit20; 
  ListOfTagListFit+=TagListFit21; 
  ListOfTagListFit+=TagListFit22; 
  ListOfTagListFit+=TagListFit23; 
  ListOfTagListFit+=TagListFit24; 
  ListOfTagListFit+=TagListFit25; 
  ListOfTagListFit+=TagListFit26; 
  ListOfTagListFit+=TagListFit27; 
  ListOfTagListFit+=TagListFit28; 
  ListOfTagListFit+=TagListFit29; 
  ListOfTagListFit+=TagListFit30; 

  ListOfTagListMass+=TagListMass1; 
  ListOfTagListMass+=TagListMass2; 
  ListOfTagListMass+=TagListMass3; 
  ListOfTagListMass+=TagListMass4; 
  ListOfTagListMass+=TagListMass5; 
  ListOfTagListMass+=TagListMass6; 
  ListOfTagListMass+=TagListMass7; 
  ListOfTagListMass+=TagListMass8; 
  ListOfTagListMass+=TagListMass9; 
  ListOfTagListMass+=TagListMass10; 
  ListOfTagListMass+=TagListMass11; 
  ListOfTagListMass+=TagListMass12; 
  ListOfTagListMass+=TagListMass13; 
  ListOfTagListMass+=TagListMass14; 
  ListOfTagListMass+=TagListMass15; 
  ListOfTagListMass+=TagListMass16; 
  ListOfTagListMass+=TagListMass17; 
  ListOfTagListMass+=TagListMass18; 
  ListOfTagListMass+=TagListMass19; 
  ListOfTagListMass+=TagListMass20; 
  ListOfTagListMass+=TagListMass21; 
  ListOfTagListMass+=TagListMass22; 
  ListOfTagListMass+=TagListMass23; 
  ListOfTagListMass+=TagListMass24; 
  ListOfTagListMass+=TagListMass25; 
  ListOfTagListMass+=TagListMass26; 
  ListOfTagListMass+=TagListMass27; 
  ListOfTagListMass+=TagListMass28; 
  ListOfTagListMass+=TagListMass29; 
  ListOfTagListMass+=TagListMass30; 

  ListOfTagListMassCut+=TagListMassCut1; 
  ListOfTagListMassCut+=TagListMassCut2; 
  ListOfTagListMassCut+=TagListMassCut3; 
  ListOfTagListMassCut+=TagListMassCut4; 
  ListOfTagListMassCut+=TagListMassCut5; 
  ListOfTagListMassCut+=TagListMassCut6; 
  ListOfTagListMassCut+=TagListMassCut7; 
  ListOfTagListMassCut+=TagListMassCut8; 
  ListOfTagListMassCut+=TagListMassCut9; 
  ListOfTagListMassCut+=TagListMassCut10; 
  ListOfTagListMassCut+=TagListMassCut11; 
  ListOfTagListMassCut+=TagListMassCut12; 
  ListOfTagListMassCut+=TagListMassCut13; 
  ListOfTagListMassCut+=TagListMassCut14; 
  ListOfTagListMassCut+=TagListMassCut15; 
  ListOfTagListMassCut+=TagListMassCut16; 
  ListOfTagListMassCut+=TagListMassCut17; 
  ListOfTagListMassCut+=TagListMassCut18; 
  ListOfTagListMassCut+=TagListMassCut19; 
  ListOfTagListMassCut+=TagListMassCut20; 
  ListOfTagListMassCut+=TagListMassCut21; 
  ListOfTagListMassCut+=TagListMassCut22; 
  ListOfTagListMassCut+=TagListMassCut23; 
  ListOfTagListMassCut+=TagListMassCut24; 
  ListOfTagListMassCut+=TagListMassCut25; 
  ListOfTagListMassCut+=TagListMassCut26; 
  ListOfTagListMassCut+=TagListMassCut27; 
  ListOfTagListMassCut+=TagListMassCut28; 
  ListOfTagListMassCut+=TagListMassCut29; 
  ListOfTagListMassCut+=TagListMassCut30; 

  ListOfTagListPstarCut+=TagListPstarCut1; 
  ListOfTagListPstarCut+=TagListPstarCut2; 
  ListOfTagListPstarCut+=TagListPstarCut3; 
  ListOfTagListPstarCut+=TagListPstarCut4; 
  ListOfTagListPstarCut+=TagListPstarCut5; 
  ListOfTagListPstarCut+=TagListPstarCut6; 
  ListOfTagListPstarCut+=TagListPstarCut7; 
  ListOfTagListPstarCut+=TagListPstarCut8; 
  ListOfTagListPstarCut+=TagListPstarCut9; 
  ListOfTagListPstarCut+=TagListPstarCut10; 
  ListOfTagListPstarCut+=TagListPstarCut11; 
  ListOfTagListPstarCut+=TagListPstarCut12; 
  ListOfTagListPstarCut+=TagListPstarCut13; 
  ListOfTagListPstarCut+=TagListPstarCut14; 
  ListOfTagListPstarCut+=TagListPstarCut15; 
  ListOfTagListPstarCut+=TagListPstarCut16; 
  ListOfTagListPstarCut+=TagListPstarCut17; 
  ListOfTagListPstarCut+=TagListPstarCut18; 
  ListOfTagListPstarCut+=TagListPstarCut19; 
  ListOfTagListPstarCut+=TagListPstarCut20; 
  ListOfTagListPstarCut+=TagListPstarCut21; 
  ListOfTagListPstarCut+=TagListPstarCut22; 
  ListOfTagListPstarCut+=TagListPstarCut23; 
  ListOfTagListPstarCut+=TagListPstarCut24; 
  ListOfTagListPstarCut+=TagListPstarCut25; 
  ListOfTagListPstarCut+=TagListPstarCut26; 
  ListOfTagListPstarCut+=TagListPstarCut27; 
  ListOfTagListPstarCut+=TagListPstarCut28; 
  ListOfTagListPstarCut+=TagListPstarCut29; 
  ListOfTagListPstarCut+=TagListPstarCut30; 

  ListOfTagListProbminCut+=TagListProbminCut1; 
  ListOfTagListProbminCut+=TagListProbminCut2; 
  ListOfTagListProbminCut+=TagListProbminCut3; 
  ListOfTagListProbminCut+=TagListProbminCut4; 
  ListOfTagListProbminCut+=TagListProbminCut5; 
  ListOfTagListProbminCut+=TagListProbminCut6; 
  ListOfTagListProbminCut+=TagListProbminCut7; 
  ListOfTagListProbminCut+=TagListProbminCut8; 
  ListOfTagListProbminCut+=TagListProbminCut9; 
  ListOfTagListProbminCut+=TagListProbminCut10; 
  ListOfTagListProbminCut+=TagListProbminCut11; 
  ListOfTagListProbminCut+=TagListProbminCut12; 
  ListOfTagListProbminCut+=TagListProbminCut13; 
  ListOfTagListProbminCut+=TagListProbminCut14; 
  ListOfTagListProbminCut+=TagListProbminCut15; 
  ListOfTagListProbminCut+=TagListProbminCut16; 
  ListOfTagListProbminCut+=TagListProbminCut17; 
  ListOfTagListProbminCut+=TagListProbminCut18; 
  ListOfTagListProbminCut+=TagListProbminCut19; 
  ListOfTagListProbminCut+=TagListProbminCut20; 
  ListOfTagListProbminCut+=TagListProbminCut21; 
  ListOfTagListProbminCut+=TagListProbminCut22; 
  ListOfTagListProbminCut+=TagListProbminCut23; 
  ListOfTagListProbminCut+=TagListProbminCut24; 
  ListOfTagListProbminCut+=TagListProbminCut25; 
  ListOfTagListProbminCut+=TagListProbminCut26; 
  ListOfTagListProbminCut+=TagListProbminCut27; 
  ListOfTagListProbminCut+=TagListProbminCut28; 
  ListOfTagListProbminCut+=TagListProbminCut29; 
  ListOfTagListProbminCut+=TagListProbminCut30; 

  return AppResult::OK;
}


AppResult DRecoTagInterface::endJob( AbsEvent* anEvent )
{
  ostream& o=cout;
  _scalers.print(o);
  o << endl;

  return AppResult::OK;
}




