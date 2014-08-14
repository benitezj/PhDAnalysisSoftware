//--------------------------------------------------------------------------

#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "Framework/AppUserBuild.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
// core sequences
#include "BetaMiniEnvSequences/BetaMiniEnvSequence.hh"
#include "BetaMiniSequences/BetaMiniSequence.hh"

// actions
#include "BetaMiniSequences/BetaMiniActions.hh"

// Minimilist set of physics sequences
#include "BetaMiniSequences/BetaMiniPhysicsSequence.hh"
// Tagging
#include "BTaggingSequences/BtsTaggingSequence.cc"

//#include "BtaTupleMaker/BtuMyAnalysis.hh"
#include "DRecoilToolsUser.hh"
#include "BtaTupleMaker/BtuTupleMaker.hh"

#include "BetaPid/PidKaonMicroSequence.hh"
#include "BetaPid/PidPionMicroSequence.hh"
#include "BetaPid/PidProtonMicroSequence.hh"


#include "UsrTools/UsrMakerTools.hh"

//
#include "DRecoTagInterface.hh"
#include "DRecoDstarTag.hh"
#include "DRecoTagKaon.hh"
#include "DRecoFragPions.hh"
#include "DRecoDsStGam.hh"
#include "DRecoDsToEllNuFit.hh"
#include "DRecoCheckCharge.hh"
#include "DRecoDsInclusiveFit.hh"
#include "DRecoDsToHadronic.hh"
#include "DRecoCleanUp.hh"
#include "DRecoPrintTree.hh"
#include "DRecoTagDump.hh"
#include "DRecoCounter.hh"

#include "DRecoDsToKKpi.hh"


//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------
class AppFramework;

//----------------
// Constructors --
//----------------

AppUserBuild::AppUserBuild( AppFramework* theFramework )
    : AppBuild( theFramework )
{
  // core sequence
  BetaMiniEnvSequence(this);
  BetaMiniSequence(this);

  // actions
  BetaMiniActions(theFramework);

  // minimilist physics sequence
  BetaMiniPhysicsSequence(this);

  // Tagging
  BtsTaggingSequence(this);

  PidKaonMicroSequence(this);
  PidPionMicroSequence(this);
  PidProtonMicroSequence(this);


  // add modules for writing user data
  UsrMakerTools(this);

  //  add(new           BtuMyAnalysis("BtuMyAnalysis",       "BtuMyAnalysis"));
  add(new           BtuTupleMaker("BtuTupleMaker",       "Tuple maker"));
  add(new       DRecoTagInterface("DRecoTagInterface",   "Interface to Tag modes"));
  add(new           DRecoDstarTag("DRecoDstarTag",       "D* tagging"));
  add(new            DRecoTagKaon("DRecoTagKaon",        "Kaon addition"));
  add(new          DRecoFragPions("DRecoFragPions",      "Fragmentation addition"));
  add(new            DRecoDsStGam("DRecoDsStGam",        "Ds* gamma addition"));
  add(new       DRecoDsToEllNuFit("DRecoDsToEllNuFit",   "ell nu reconstruction" ));
  add(new        DRecoCheckCharge("DRecoCheckCharge",    "Right and Wrong sign selection"));
  add(new     DRecoDsInclusiveFit("DRecoDsInclusiveFit", "fit for inclusive Ds"));
  add(new       DRecoDsToHadronic("DRecoDsToHadronic",   "Fit for Ds to Hadronic modes"));
  add(new            DRecoCleanUp("DRecoCleanUp",        "clean up event candidate lists"));
  add(new          DRecoPrintTree("DRecoPrintTree",      "Prints Candidate trees in event candidate list"));
  add(new            DRecoTagDump("DRecoTagDump",        "Creates simple list of Tags"));
  add(new            DRecoCounter("DRecoCounter",        "Counts events that reach the module with at least one candidate")) ;
  add(new            DRecoDsToKKpi("DRecoDsToKKpi",      "Ds-->KKpi reconstruction."));          
}

//--------------
// Destructor --
//--------------

AppUserBuild::~AppUserBuild( )
{
}
