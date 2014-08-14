# $Id: SkimTagBFlavSequence.tcl,v 1.1 2005/11/16 23:51:46 fwinkl Exp $
# This sequence is sourced in each of the BFlav_*Path.tcl files.
# All modules are disabled here and get enabled in the individual skim paths.
# This file is NOT sourced in TagProdCreateSequence!

global SkimTagBFlavSequenceDefinition
if [expr ![info exists SkimTagBFlavSequenceDefinition]] {
  set SkimTagBFlavSequenceDefinition 1

  sequence create SkimTagBFlavSequence

  # define useful procs
  sourceFoundFile EventTagTools/TagProcs.tcl

#---ORL: Add Bflav tag bits that specify the D0 or D+/- mode.
  loadTagModule TagBFlavDstarPi SkimTagBFlavSequence {
    inputList   set B0ToDstarPiDefault
    bitNameBase set B0ToDstarPi_
    splitByD0   set true
  }
  loadTagModule TagBFlavDstarRhoC SkimTagBFlavSequence {
    inputList   set B0ToDstarRhoCDefault
    bitNameBase set B0ToDstarRhoC_
    splitByD0   set true
  }
  loadTagModule TagBFlavDstarA1C SkimTagBFlavSequence {
    inputList   set B0ToDstarA1CDefault
    bitNameBase set B0ToDstarA1C_
    splitByD0   set true
  }
  loadTagModule TagBFlavDPi SkimTagBFlavSequence {
    inputList   set B0ToDPiDefault
    bitNameBase set B0ToDchPi_
    splitByD0   set false
  }
  loadTagModule TagBFlavDRhoC SkimTagBFlavSequence {
    inputList   set B0ToDRhoCDefault
    bitNameBase set B0ToDchRhoC_
    splitByD0   set false
  }
  loadTagModule TagBFlavDA1C SkimTagBFlavSequence {
    inputList   set B0ToDA1CDefault
    bitNameBase set B0ToDchA1C_
    splitByD0   set false
  }

  # Proc to append and enable all of the above modules to a sequence
  proc useAllBFlavTagModules {sequence} {
    addTagModule TagBFlavDstarPi   $sequence
    addTagModule TagBFlavDstarRhoC $sequence
    addTagModule TagBFlavDstarA1C  $sequence
    addTagModule TagBFlavDPi       $sequence
    addTagModule TagBFlavDRhoC     $sequence
    addTagModule TagBFlavDA1C      $sequence
  }
}
