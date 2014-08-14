//------------------------------------------------------------------------
// BrecoQa
//
// Description:
//   Stores Breco QA histograms
//
// Author List:
//      Nick Danielson
//
//------------------------------------------------------------------------
#ifndef BRECOQA_HH
#define BRECOQA_HH

//----------------------
// Base Class Headers --
//----------------------
#include "Framework/AppModule.hh"
#include "Framework/AbsParmDouble.hh"
#include "HepTuple/HTValOrderedVector.h"
#include "PDT/PdtLund.hh"


//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
class BtaAttributes;
class BtaCandidate;
class BtaExclusiveDecayList;
class HepHistogram;
class HepTuple;

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class BrecoQa : public AppModule {

//--------------------
// Instance Members --
//--------------------

public:

    // Constructors
  BrecoQa( const char* const theName, const char* const theDescription );

    // Destructor
  virtual ~BrecoQa( );

    // Operations

  virtual AppResult           beginJob( AbsEvent* anEvent );
  virtual AppResult event( AbsEvent* anEvent );
  virtual AppResult           other( AbsEvent* anOther );
  virtual AppResult           endJob  ( AbsEvent* anEvent );
  virtual AppResult           abortJob( AbsEvent* anEvent );
  
protected:

  AbsParmDouble       _deCut;

private:
  void processCandList(HepAList<BtaCandidate>* list, BtaCandidate *&brecCand, int listID);
  void fillDMassHisto(PdtLund::LundType particle, BtaCandidate brecCand, HepHistogram *histo, AbsEvent* anEvent);
  void fillDstarHisto(PdtLund::LundType particle, BtaCandidate brecCand, HepHistogram *histo, AbsEvent* anEvent);
  void fillHistograms(BtaCandidate *brecCand, AbsEvent *anEvent);
  BtaAttributes *_attributes;
  BtaExclusiveDecayList *_decayList;

  enum {B0TODSTARPI, B0TODSTARRHOC, B0TODSTARA1C, B0TODPI, B0TODRHOC, B0TODA1C};
  int _bestCandDecay;

  // Histograms...
  HepHistogram *_h_mesB0;
  HepHistogram *_h_mesDstar;
  HepHistogram *_h_mesDstarPi;
  HepHistogram *_h_mesNonDstar;
  HepHistogram *_h_mesPi0;
  HepHistogram *_h_mesKs;

  HepHistogram *_h_deB0;
  HepHistogram *_h_deDstar;
  HepHistogram *_h_deDstarPi;
  HepHistogram *_h_deNonDstar;
  HepHistogram *_h_dePi0;
  HepHistogram *_h_deKs;

  HepHistogram *_h_dmass_KsPi;
  HepHistogram *_h_dmass_KPiPi;
  HepHistogram *_h_dmass_KPi;
  HepHistogram *_h_dmass_KPiPi0;
  HepHistogram *_h_dmass_K3Pi;
  HepHistogram *_h_dmass_KsPiPi;
  HepHistogram *_h_dstarmass_KPi;
  HepHistogram *_h_dstarmass_K3Pi;
  HepHistogram *_h_dstarmass_KPiPi0;
  HepHistogram *_h_dstarmass_KsPiPi;

  HepHistogram *_h_recoFlavB0;
  HepHistogram *_h_modeB0;

  HepHistogram *_h_bestCandDecay;

};

#endif



