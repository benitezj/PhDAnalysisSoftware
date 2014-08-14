#
# $Id: B0ToDstarhKlnuPath.tcl,v 1.2 2005/11/05 01:19:43 rolfa Exp $
#
############################################################################
##
## Defines the B0ToDstarhKlnuPath skim filter path
##
## This filter searches for the decay:
##
##   B0 -> D*- K+
##          |
##           -> D0 pi-
##               |
##                -> K- l+ nu
##
############################################################################

path create B0ToDstarhKlnuPath

global BtaCandLists
lappend BtaCandLists B0ToDstarD0PiSemiLept

#################################
## global control variables
#################################

  set D0Debug false
  set D0Verbose false

#################################
## SimpleComposition Cuts
#################################

# FastVtx probability
  set ProbChiSqFastVtx  "ProbChiSq 0.005:"

# D0 -> K l nu cuts
  set DeltaMSemiLep     "DeltaM 0.:0.7"
  set MassSemiLepe      "Mass 0.:1.82"
  set MassSemiLepmu     "Mass 0.:1.95"

######################################################################
######################################################################

#################################
## Hard Tracks
#################################

mod clone SmpSubListerDefiner RbHardTracks
path append B0ToDstarhKlnuPath RbHardTracks
talkto RbHardTracks {
  unrefinedListName set GoodTracksHardVeryLoose
  selectors         set "CmsP 1.8:2.7"
}

# D0 -> K l nu

mod clone SmpMakerDefiner D0ToKElecNu
path append B0ToDstarhKlnuPath D0ToKElecNu
catch { setProduction D0ToKElecNu }
talkto D0ToKElecNu {
  debug             set $D0Debug
  verbose           set $D0Verbose
  disableCloneCheck set true
  decayMode         set "D0 -> K- e+"
  daughterListNames set "KLHNotPion"
  daughterListNames set "PidLHElectrons"
  fittingAlgorithm  set "FastVtx"
  preFitSelectors   set $MassSemiLepe
  postFitSelectors  set $MassSemiLepe
  postFitSelectors  set $ProbChiSqFastVtx
}

mod clone SmpMakerDefiner D0ToKElecNuSS
path append B0ToDstarhKlnuPath D0ToKElecNuSS
catch { setProduction D0ToKElecNuSS }
talkto D0ToKElecNuSS {
  debug             set $D0Debug
  verbose           set $D0Verbose
  disableCloneCheck set true
  decayMode         set "D0 -> K- e-"
  daughterListNames set "KLHNotPion"
  daughterListNames set "PidLHElectrons"
  checkType         set false
  fittingAlgorithm  set "FastVtx"
  preFitSelectors   set $MassSemiLepe
  postFitSelectors  set $MassSemiLepe
  postFitSelectors  set $ProbChiSqFastVtx
}

mod clone SmpMakerDefiner D0ToKMuonNu
path append B0ToDstarhKlnuPath D0ToKMuonNu
catch { setProduction D0ToKMuonNu }
talkto D0ToKMuonNu {
  debug             set $D0Debug
  verbose           set $D0Verbose
  disableCloneCheck set true
  decayMode         set "D0 -> K- mu+"
  daughterListNames set "KLHNotPion"
  daughterListNames set "muNNLoose"
  fittingAlgorithm  set "FastVtx"
  preFitSelectors   set $MassSemiLepmu
  postFitSelectors  set $MassSemiLepmu
  postFitSelectors  set $ProbChiSqFastVtx
}

mod clone SmpMergerDefiner D0ToKlnu
path append B0ToDstarhKlnuPath D0ToKlnu
catch { setProduction D0ToKlnu }
talkto D0ToKlnu {
  inputListNames set D0ToKElecNuSS
  inputListNames set D0ToKElecNu
  inputListNames set D0ToKMuonNu
}


# D*+ -> D0 Pi+ (RS)

mod clone SmpMakerDefiner DstarToD0PiSemiLeptRS
path append B0ToDstarhKlnuPath DstarToD0PiSemiLeptRS
catch { setProduction DstarToD0PiSemiLeptRS }
talkto DstarToD0PiSemiLeptRS {
  debug             set $D0Debug
  verbose           set $D0Verbose
  disableCloneCheck set true
  decayMode         set "D*+ -> D0 pi+"
  daughterListNames set "D0ToKlnu"
  daughterListNames set "GoodTracksVeryLoose"
  fittingAlgorithm  set "Add4"
  postFitSelectors  set $DeltaMSemiLep
}

# D*- -> D0 Pi- (WS)

mod clone SmpMakerDefiner DstarToD0PiSemiLeptWS
path append B0ToDstarhKlnuPath DstarToD0PiSemiLeptWS
catch { setProduction DstarToD0PiSemiLeptWS }
talkto DstarToD0PiSemiLeptWS {
  debug             set $D0Debug
  verbose           set $D0Verbose
  disableCloneCheck set true
  decayMode         set "D*- -> D0 pi-"
  daughterListNames set "D0ToKlnu"  
  daughterListNames set "GoodTracksVeryLoose"
  fittingAlgorithm  set "Add4"
  postFitSelectors  set $DeltaMSemiLep
}

mod clone SmpMergerDefiner DstarToD0PiSemiLept
path append B0ToDstarhKlnuPath DstarToD0PiSemiLept
catch { setProduction DstarToD0PiSemiLept }
talkto DstarToD0PiSemiLept {
  inputListNames set DstarToD0PiSemiLeptRS
  inputListNames set DstarToD0PiSemiLeptWS
}

# B0 -> D*- (-> D0 Pi-) K/pi+


mod clone SmpMakerDefiner B0ToDstarD0PiSemiLept
path append B0ToDstarhKlnuPath B0ToDstarD0PiSemiLept
catch { setProduction B0ToDstarD0PiSemiLept}
talkto B0ToDstarD0PiSemiLept {
    disableCloneCheck set true
    decayMode          set "B0 -> D*- K+"
    daughterListNames  set "DstarToD0PiSemiLept"
    daughterListNames  set "RbHardTracks"
    checkType          set false
    fittingAlgorithm   set "Add4"
#    postFitSelectors   set "Mass 3.5:6."
}


path append B0ToDstarhKlnuPath B0ToDstarhKlnuSkim
talkto B0ToDstarhKlnuSkim {
  inputList set B0ToDstarD0PiSemiLept
}
