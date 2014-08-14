#
# $Id: BtoRhoGammaPath.tcl,v 1.8 2007/01/03 19:50:47 krosebe Exp $
#
# defines BtoRhoGammaPath skim path
#
  sourceFoundFile ErrLogger/ErrLog.tcl

  global writeComposites
  global BtaCandLists
  global writeUsrData
  global writeUsrDataForSkim

  path create BtoRhoGammaPath

  # need to run on MultiHadrons and NeutralHadron
  mod clone TagFilterByName BtoRhoGammaBGF
  path append BtoRhoGammaPath BtoRhoGammaBGF
  talkto BtoRhoGammaBGF {
    orList set BGFMultiHadron
    orList set BGFNeutralHadron
  }

  path append BtoRhoGammaPath BtoRhoGammaBtoXGF
  talkto BtoRhoGammaBtoXGF {
    activeR2AllCut set true
    activeE1Cut set true
    activeNTrkCut set true
    maxR2All set 0.9
    minNTRK set 2
    minE1 set 1.45
    maxE1 set 3.5
  }


  # Run the Penguin composition tools
  sourceFoundFile CompositionSequences/CompHllSequence.tcl
  path append BtoRhoGammaPath CompHllSequence

  sourceFoundFile CompositionSequences/CompPenguinSequence.tcl
  path append BtoRhoGammaPath CompPenguinSequence


  # Add the analysis skim
  path append BtoRhoGammaPath BtoRhoGammaFilter
  talkto BtoRhoGammaFilter {
    BLists set B0ToRho0GammaLoose 
    BLists set BToRhoCGammaLoose
    BLists set B0ToOmegaGammaLoose
    BLists set B0ToKst0GammaKst0ToKPiLoose
    BLists set B0ToKst0GammaKst0ToKsPi0Loose
    BLists set BToKstCGammaKstCToKPi0Loose
    BLists set BToKstCGammaKstCToKsPiLoose
    BLists set B0ToPhiGammaLoose
    verbose set false
    production set true
  }

  # Persist the required composites from CompPenguinSequence
  set writeComposites 1
  if [info exists writeComposites ] {
    ErrMsg trace "Writing composites for BtoRhoGamma"
    lappend BtaCandLists B0ToRho0GammaLoose BToRhoCGammaLoose B0ToOmegaGammaLoose B0ToKst0GammaKst0ToKPiLoose B0ToKst0GammaKst0ToKsPi0Loose BToKstCGammaKstCToKPi0Loose BToKstCGammaKstCToKsPiLoose B0ToPhiGammaLoose
  } else {
    ErrMsg trace "Not writing composites for BtoRhoGamma"
  }

  set writeUsrDataForSkim 1
