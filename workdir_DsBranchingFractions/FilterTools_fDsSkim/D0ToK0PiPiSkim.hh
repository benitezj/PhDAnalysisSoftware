//
// CVS $Id: D0ToK0PiPiSkim.hh,v 1.2 2007/10/09 08:43:31 marchior Exp $
//

#ifndef D0TOK0PIPISKIM_HH
#define D0TOK0PIPISKIM_HH 

//----------------------
// Base Class Headers --
//----------------------
#include "CLHEP/Alist/AList.h"
#include "CLHEP/Matrix/Vector.h"
#include "CLHEP/Vector/LorentzVector.h"

#include "Beta/BtaCandidate.hh"
#include "Framework/AppFilterModule.hh"
#include "Framework/AbsParmBool.hh"
#include "Framework/AbsParmDouble.hh"
#include "Framework/AbsParmGeneral.hh"
#include "Framework/AbsParmString.hh"
#include "AbsParm/AbsParmIfdStrKey.hh"
#include "Framework/AbsParmBool.hh"
#include "AbsParm/AbsParmVector.hh"
                                                                                                                                                          
#include "VtxBase/VtxParmAlgId.hh"
#include "VtxBase/VtxParmAlgMode.hh"
                                                                                                                                                          

#include "TagModules/TagFilterModule.hh"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

class AbsEvent;
class EventInfo;
class BtaBooster;

//		---------------------
// 		-- Class Interface --
//		---------------------
 

class D0ToK0PiPiSkim : public TagFilterModule {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  D0ToK0PiPiSkim ( const char* const theName, const char* const theDescription );

  // Destructor
  virtual ~D0ToK0PiPiSkim( );
  
  // Operations
  virtual AppResult           beginJob( AbsEvent* anEvent );
  virtual AppResult           beginRun( AbsEvent* anEvent );
  virtual AppResult           event   ( AbsEvent* anEvent );
  virtual AppResult           endRun  ( AbsEvent* anEvent );
  virtual AppResult           endJob  ( AbsEvent* anEvent );

protected:

  HepLorentzVector klfi_h(const HepLorentzVector twopi, const HepPoint orig, const HepPoint klCenter);
  HepLorentzVector klfi_l(const HepLorentzVector twopi, const HepPoint orig, const HepPoint klCenter);

  AbsParmIfdStrKey _DstrToKLList;
  AbsParmIfdStrKey _DstrToKSList;  

 AbsParmIfdStrKey  eventIdKey;
  AbsParmIfdStrKey _eventInfoList;
  AbsParmIfdStrKey _btaNeutralList;
  AbsParmIfdStrKey _btaNeutHadList;
  AbsParmIfdStrKey _K0SList;
  AbsParmIfdStrKey _pionList;
  
  VtxParmAlgId     vtxFitAlgo;
  VtxParmAlgMode   vtxFitMode;
  
  AbsParmGeneral<double> slowPiMomLo;        // Lower cut slow pion lab pt
  AbsParmGeneral<double> slowPiMomHi;        // Upper cut slow pion lab pt
  AbsParmGeneral<double> slowPiRdocaMax;     // Max R doca of slow pion
  AbsParmGeneral<double> slowPiZdocaMax;     // Max Z doca of slow pion
  AbsParmGeneral<int> slowPiSvtHit; // Min number SVT hits for slow pion
  AbsParmGeneral<int> slowPiDchHit; // Min number DCH hits for slow pion
  AbsParmGeneral<double> deltaMassCutHi;     // delta mass cut on improved delta mass
  AbsParmGeneral<double> deltaMassCutLo;     // delta mass cut on improved delta mass
  AbsParmGeneral<double> pStarCut;           // D* cm momentum cut
  AbsParmGeneral<double> dxymax;         //these are all Ks events cuts...
  AbsParmGeneral<double> dzmax;
  AbsParmGeneral<double> pkmin;
  AbsParmGeneral<double> pkmax;
  AbsParmGeneral<double> pksmin;
  AbsParmGeneral<double> pksmax;
  AbsParmGeneral<double> mkstarmin;
  AbsParmGeneral<double> mkstarmax;
  AbsParmGeneral<double> ecalmin;
  AbsParmGeneral<double> eksmin;
  AbsParmGeneral<double> helmin;


private:
  // Some counters for the skim
  int _nReadEvents;
  int _nEventsPassed;
};

#endif
