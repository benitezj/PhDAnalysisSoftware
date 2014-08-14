# $Id: SkimTagBrecoSequence.tcl,v 1.2 2006/01/25 21:11:26 fwinkl Exp $

global SkimTagBrecoSequenceDefinition
if [expr ![info exists SkimTagBrecoSequenceDefinition]] {
  set SkimTagBrecoSequenceDefinition 1

  echo "Executing SkimTagBrecoSequence.tcl ..."

  sequence create SkimTagBrecoSequence

  # define useful procs
  sourceFoundFile EventTagTools/TagProcs.tcl

  # disable some modules which are no longer used
  mod disable TagMakeBRecoToDDstar
  mod disable TagMakeA1Pi
  
  loadTagModule TagMakeBRecoToDD SkimTagBrecoSequence {
    Blist set BToDDLoose
  }
  
  loadTagModule TagMakeDDK SkimTagBrecoSequence {
    ChargedBlist set BchToDDKDefault
    NeutralBlist set B0ToDDKDefault
    doHistos     set false
  }

  loadTagModule TagMakeDDK_DeltaC2 SkimTagBrecoSequence {
    ChargedBlist set BchToDDK_DeltaC2_Default
    NeutralBlist set B0ToDDK_DeltaC2_Default
    doHistos     set false
  }
  
  loadTagModule TagMakeBRecoToD2sD SkimTagBrecoSequence {
    ChargedBlist set BchToD2sDDefault
    NeutralBlist set B0ToD2sDDefault
    doHistos     set false
  }

#  sequence append TagProdCreateSequence TagMakeBRecoToDDstar
#  lappend modulesOfTagProdCreateSequence TagMakeBRecoToDDstar
#  catch { setProduction TagMakeBRecoToDDstar }
#  talkto TagMakeBRecoToDDstar {
#    ChargedBlist set BchToDLightDefault
#    NeutralBlist set B0ToDLightDefault
#    #	doHistos     set false
#    doHistos     set true
#  }

  ## Tag-setting for BReco-modes (previously done TagMakeBRecoToDDstar) using the TagFromList clone. ##
  module disable TagMakeBRecoToDDstar
  
  defineTagFromList BchToD0KDefault          BchToD0K          SkimTagBrecoSequence  ;# B->D0K
  defineTagFromList BchToD0PiDefault         BchToD0Pi         SkimTagBrecoSequence  ;# B->D0Pi
  defineTagFromList BchToDstar0PiDefault     BchToDstar0Pi     SkimTagBrecoSequence  ;# B->D*0pi
  defineTagFromList BchToD0KstarDefault      BchToD0Kstar      SkimTagBrecoSequence  ;# B->D0K*
  defineTagFromList BchToDstar0KstarDefault  BchToDstar0Kstar  SkimTagBrecoSequence  ;# B->D*0K*
  defineTagFromList B0ToDstarPiDefault       B0ToDstarPi       SkimTagBrecoSequence  ;# B0->D*0pi
  defineTagFromList B0ToDstarRhoCDefault     B0ToDstarRhoC     SkimTagBrecoSequence  ;# B0->D*0rho
  defineTagFromList B0ToDstarA1CDefault      B0ToDstarA1C      SkimTagBrecoSequence  ;# B0->D*0a1
  defineTagFromList B0ToDPiDefault           B0ToDchPi         SkimTagBrecoSequence  ;# B0->D0pi
  defineTagFromList B0ToDRhoCDefault         B0ToDchRhoC       SkimTagBrecoSequence  ;# B0->D0rho
  defineTagFromList B0ToDA1CDefault          B0ToDchA1C        SkimTagBrecoSequence  ;# B0->D0a1
  defineTagFromList BchToDsPi0Default        BchToDsPi0        SkimTagBrecoSequence  ;# B->Dspi0
  
  
  # the following tag bits seem not to be used any longer
  
  #	makeTagFromList B0ToDsA1CTight           B0ToDsA1CTight      ;# B0->Dsa1  (tight a1)
  #  makeTagFromList B0ToDsstarA1CTight       B0ToDSstarA1CTight  ;# B0->Ds*a1 (tight a1)
  #  makeTagFromList BchToDsA10Tight          BchToDsA10Tight     ;# B->Dsa10  (tight a1)
  #  makeTagFromList BchToDsstarA10Tight      BchToDSstarA10Tight ;# B->Ds*a10 (tight a1)
  #  makeTagFromList B0ToD0KsPi0Default       B0ToD0KsPi0         ;# B0->D0Kspi0
  #  makeTagFromList B0ToD0KsRho0Default      B0ToD0KsRho0        ;# B0->D0Ksrho0
  #  makeTagFromList B0ToDstar0KsPi0Default   B0ToDstar0KsPi0     ;# B0->D*0Kspi0
  #  makeTagFromList B0ToDstar0KsRho0Default  B0ToDstar0KsRho0    ;# B0->D*0Ksrho0
  #  makeTagFromList BchToD0CPKstarDefault      BchToD0CPKstar       ;# B->D0K*, D0->CP
  #  makeTagFromList BchToDstar0PiD0CPOddDefault  BchToDstar0PiD0CPOdd  ;# B->D*0pi/K decays with D0 -> CP-Odd states and NoPID
  #  makeTagFromList BchToDstar0PiD0CPEvenDefault BchToDstar0PiD0CPEven ;# B->D*0pi/K decays with D0 -> CP-Even states and NoPID
  #  makeTagFromList BchToD0KstarD0CPOddDefault   BchToD0KstarD0CPOdd   ;# B->D0K* decays with D0 -> CP-Odd states
  #  makeTagFromList BchToD0KstarD0CPEvenDefault  BchToD0KstarD0CPEven  ;# B->D0K* decays with D0 -> CP-Even states
  #  makeTagFromList BchToDLightDefaultWS         BchToDLightWrongSign  ;# B->D0Pi With the Wrong Sign (Color/Cabibbo-Suppressed flavour assigned to the D0)

  echo "SkimTagBrecoSequence.tcl done."
}
