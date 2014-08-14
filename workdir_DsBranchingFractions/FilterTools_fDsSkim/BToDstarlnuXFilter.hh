//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: BToDstarlnuXFilter.hh,v 1.4 2006/02/02 18:31:30 kelsey Exp $
//
// Description:
//
// Framework module to select D* lnu events for the BToDlnu skim.
//
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Eugenio Paoloni & Gabriele Simi
//
// History (add to end):
//
// Copyleft Information:
//      Copyleft (C) 2004                      INFN Pisa and SLAC
//
//------------------------------------------------------------------------

#ifndef BTODSTARLNUFILTER_HH
#define BTODSTARLNUFILTER_HH



#include "Framework/AppModule.hh"
#include "AbsParm/AbsParmIfdStrKey.hh"
#include "Framework/AbsParmBool.hh"
#include "UsrData/UsrData.hh"
#include "CLHEP/Vector/LorentzVector.h"
#include "CLHEP/Alist/AList.h"
#include <map>


using std::map;

class AbsEvent;
class BtaCandidate;

class BToDstarlnuXFilter : public AppModule 
{
public:
	BToDstarlnuXFilter( const char* const, const char* const );
	virtual ~BToDstarlnuXFilter( );
	virtual AppResult beginJob( AbsEvent* );
	virtual AppResult event( AbsEvent* );
	virtual AppResult endJob  ( AbsEvent* );    
private:

	void initialize(void);


	AbsParmIfdStrKey _BList;
	AbsParmIfdStrKey _refittedBList;
	AbsParmIfdStrKey _eventData;
	AbsParmIfdStrKey _candData;

	AbsParmIfdStrKey _eSelectorKey;
	AbsParmIfdStrKey _muSelectorKey;
	AbsParmIfdStrKey _KSelectorKey;

	AbsEvent     * _event;
	UsrEventBlock  _eventBlock;
	UsrCandBlock   _candBlock;
	EventInfo    * _eventInfo;
        HepLorentzVector    _p4CM;
	Hep3Vector          _CM_frame;

	std::map<BtaCandidate*, int>* _eSelectorsMap;
	std::map<BtaCandidate*, int>* _muSelectorsMap;
	std::map<BtaCandidate*, int>* _KSelectorsMap;

	bool  set_B_decay_mode( void );
	bool  fill_PID        ( void );
	bool  refit_B         ( void );
	bool  cos_theta_BY    ( void );

	BtaCandidate *  make_D_tree     ( void );

  	const BtaCandidate * _theB;
	const BtaCandidate * _lepton;
	const BtaCandidate * _Dstar;

	const BtaCandidate * _pis;
	const BtaCandidate * _theD;

	const BtaCandidate * _theK;

	BtaCandidate * _B_refitted;
	BtaCandidate * _Dstar_refitted;

	
	UsrVariable<char>  _B_decay_mode;
	UsrVariable<float> _B_chi2_prob;
	UsrVariable<float> _B_vtx_status;
       	UsrVariable<HepLorentzVector> _B_p4;

	UsrVariable<HepLorentzVector> _l_p4;
  
	UsrVariable<float> _Dstar_mass_raw;
	UsrVariable<float> _Dstar_mass;
	UsrVariable<HepLorentzVector> _Dstar_p4;

	UsrVariable<float> _D_mass_raw;
	UsrVariable<float> _D_mass;
	UsrVariable<float> _D_chi2_prob;
	UsrVariable<float> _D_vtx_status;
	UsrVariable<HepLorentzVector> _D_p4;

	UsrVariable<HepLorentzVector> _pis_p4;

	UsrVariable<float> _cos_theta_BY;
	
	UsrVariable<char>   _e_bitmask;
	UsrVariable<short> _mu_bitmask;
	UsrVariable<short>  _K_bitmask;

	HepAList<BtaCandidate> _garbage;
	bool _put_cand_ok;

  
	enum BitMask{ Charged_B = 0x01,
		      quark_b   = 0x02,
		      lepton    = 0x04,
		      neutral_D = 0x08,
		      D_Decay   = 0x70,
		      // Neutral D decay modes
		      K_pi      = 0x10,
		      K_3pi     = 0x20,
		      K_pi_pi0  = 0x30,
		      KS_2pi    = 0x40,
		      // Charged D decay modes
		      K_2pi     = 0x50,
		      KS_pi     = 0x60,
		      
		      electron  = 0x80 };
		     
};

#endif






