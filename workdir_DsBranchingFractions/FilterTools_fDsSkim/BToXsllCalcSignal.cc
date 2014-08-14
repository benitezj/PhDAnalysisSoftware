#include "BaBar/BaBar.hh"
//#include <iostream>
#include "Beta/BtaCandidate.hh"
#include "CLHEP/Alist/AIterator.h"
#include "ErrLogger/ErrLog.hh"
#include "PDT/PdtLund.hh"
#include "PDT/PdtEntry.hh"
#include "PDT/Pdt.hh"
#include "ProxyDict/Ifd.hh"

#include "FilterTools/BToXsllCalcSignal.hh"

bool BToXsllCalcSignal::calc( IfdProxyDict* event )
{  
  HepAList< BtaCandidate >*  truthCandsList =
    Ifd< HepAList<BtaCandidate> >::get( event, "MCTruth" );

  //shouldnt these be static?

  static const PdtEntry* pdt_B0 = Pdt::lookup(PdtLund::B0);
  static const PdtEntry* pdt_anti_B0 = Pdt::lookup(PdtLund::anti_B0);
  static const PdtEntry* pdt_B_plus = Pdt::lookup(PdtLund::B_plus);
  static const PdtEntry* pdt_B_minus = Pdt::lookup(PdtLund::B_minus);
  
  static const PdtEntry* pdt_eplus = Pdt::lookup(PdtLund::e_plus);
  static const PdtEntry* pdt_eminus = Pdt::lookup(PdtLund::e_minus);
  static const PdtEntry* pdt_muplus = Pdt::lookup(PdtLund::mu_plus);
  static const PdtEntry* pdt_muminus = Pdt::lookup(PdtLund::mu_minus);
  
  static const PdtEntry* pdt_Jpsi = Pdt::lookup(PdtLund::J_psi);
  static const PdtEntry* pdt_psi2S = Pdt::lookup(PdtLund::psi_2S);


  if(truthCandsList)
    {
      HepAListIterator<BtaCandidate> truthCandsIter(*truthCandsList);
      BtaCandidate* truthCand(0);
      
      // determine if this event contains
      // any signal or signal-like decays
      while ( truthCand = truthCandsIter() ) {
	
	BtaCandidate* genLepPlus(0);
	BtaCandidate* genLepMinus(0);
	
	if ( (truthCand->pdtEntry() == pdt_B0 ||
	      truthCand->pdtEntry() == pdt_anti_B0 ||
	      truthCand->pdtEntry() == pdt_B_plus ||
	      truthCand->pdtEntry() == pdt_B_minus) ) {
	  
	  HepAListIterator<BtaCandidate> dgtrIter(truthCand->daughterIterator());
	  BtaCandidate* bdgtr(0);
	  
	  while ( bdgtr = dgtrIter() ) {
	    if (bdgtr->pdtEntry() == pdt_eplus ||
		bdgtr->pdtEntry() == pdt_muplus) {
	      genLepPlus = bdgtr;
	      
	    } else if (bdgtr->pdtEntry() == pdt_eminus ||
		       bdgtr->pdtEntry() == pdt_muminus) {
	      genLepMinus = bdgtr;
	    } else if (bdgtr->pdtEntry() == pdt_Jpsi ||
		       bdgtr->pdtEntry() == pdt_psi2S ) {
	      return true;
	    }
	  }
	  
	  if ( genLepPlus && genLepMinus ) return true;
	  
	}
	
      }
      
      return false;
    }

  return false;
}
