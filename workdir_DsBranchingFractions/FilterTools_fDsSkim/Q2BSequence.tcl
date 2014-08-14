##################################################
#
# $Id: Q2BSequence.tcl,v 1.1 2005/06/13 14:37:38 fnc Exp $
#
#   A. Bevan Jan '05 Original author
#
# G. Finocchiaro Jun '05 reworked in FilterTools
####################################################

# Create main sequence
global Q2BSEQUENCE
if [expr ![info exists Q2BSEQUENCE]] {
  set Q2BSEQUENCE 1
  sequence create Q2BSequence

  sourceFoundFile SimpleComposition/SmpProcs.tcl

  createsmpmerger Q2BSequence Q2BTheEventList {
    inputListNames set "GoodTracksVeryLoose"
    inputListNames set "CalorNeutral"
  }

  createsmpsublister Q2BSequence Q2BPi0 {
    unrefinedListName  set "pi0LooseMass"
    maxNumberOfCandidates set 30
  }

  createsmpsublister Q2BSequence Q2BRho {
    unrefinedListName set "rhoCBRecoLoose"
    selectors set "Mass 0.4:1.1"
  }

  createsmpsublister Q2BSequence Q2BRhoLH {
    decayMode          set "rho+ -> pi+ pi0"
    unrefinedListName set "Q2BRho"
    daughterListNames set "piLHLoose"
    daughterListNames set "Q2BPi0"
  }

  createsmpsublister Q2BSequence Q2BRho0LH {
    decayMode         set "rho0 -> pi+ pi-"
    unrefinedListName set "rho0Loose"
    daughterListNames set "piLHLoose"
    daughterListNames set "piLHLoose"
  }

  createsmpsublister Q2BSequence Q2BTightRho {
    #  decayMode         set "rho0 -> pi+ pi0"
    unrefinedListName set "Q2BRho"
    selectors         set "Mass 0.45:1.05"
    selectors         set "Helicity -0.95:1.00"
    #  maxNumberOfCandidates set 50
  }

  createsmpsublister Q2BSequence Q2BTightRho0 {
    #  decayMode         set "rho0 -> pi+ pi-"
    unrefinedListName set "rho0Loose"
    selectors         set "Mass 0.45:1.05"
    selectors         set "Helicity -0.95:1.00"
    #  maxNumberOfCandidates set 50
  }
  
  createsmpsublister Q2BSequence Q2B_A1_PiCPiCPiC {
    unrefinedListName set "a1CLoose"
    selectors set "Mass 0.6:1.8"
  }

  createsmpsublister Q2BSequence Q2B_A1_PiCPiCPi0 {
    unrefinedListName  set "a10Loose"
    selectors    set "Mass 0.6:1.8"
  }

  createsmpsublister Q2BSequence Q2BKst_Kpi0 {
    decayMode         set "K*+ -> K+ pi0"
    unrefinedListName set "KstarKPi0VeryVeryLoose"
    daughterListNames set "GoodTracksVeryLoose"
    daughterListNames set "pi0LooseMass"
    selectors         set "Mass 0.738:1.044"
  }

  createsmpsublister Q2BSequence Q2BKst_K0pi {
    decayMode         set "K*+ -> K_S0 pi+"
    unrefinedListName set "KstarKsPiVeryLoose"
    daughterListNames set "KsDefaultMass"
    daughterListNames set "piLHLoose"
    selectors         set "Mass 0.738:1.044"
  }

  createsmpsublister Q2BSequence Q2BKst0_Kpi {
    unrefinedListName set "KstarKPiVeryVeryLoose"
    selectors         set "Mass 0.743:1.049"
  }

  createsmpsublister Q2BSequence Q2BKst0_K0pi0 {
    decayMode         set "K*0 -> K_S0 pi0"
    unrefinedListName set "KstarKsPi0VeryLoose"
    daughterListNames set "KsLoose"
    daughterListNames set "pi0LooseMass"
    selectors   set "Mass 0.743:1.049"
  }

  createsmpmerger Q2BSequence Q2BKst {
    inputListNames set Q2BKst_Kpi0
    inputListNames set Q2BKst_K0pi
  }

  createsmpmerger Q2BSequence Q2BKst0 {
    inputListNames set Q2BKst0_Kpi
    inputListNames set Q2BKst0_K0pi0
  }

}
