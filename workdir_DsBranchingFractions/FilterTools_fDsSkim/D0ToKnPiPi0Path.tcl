#
#  $Id: D0ToKnPiPi0Path.tcl,v 1.1 2008/12/01 22:20:06 gjackson Exp $
#  Define the D0ToKnPiPi0 sequence
#  Jonathon Coleman : coleman@slac.stanford.edu
#  Jose Benitez :    benitezj@slac.stanford.edu


path create D0ToKnPiPi0Path
set mySkimSequence D0ToKnPiPi0Sequence

sequence create $mySkimSequence
path append D0ToKnPiPi0Path $mySkimSequence
 
#  Create the D Lists...2 modes in total:
#  1. D0 -> Ks pi+ pi- pi0
#  2. D0 -> K- pi+ pi+ pi-  pi0


#################################
## SimpleComposition Cuts
#################################

## D0 cuts
##
#set pi0ListD           "pi0AllVeryLoose"
set pi0ListD           "pi0VeryLoose"
set ksListD            "KsLoose"
set kListD             "KCombinedVeryLoose"
#set pionListD          "GoodTracksVeryLoose"
set pionListD          "piCombinedVeryLoose"

##
##

# FastVtx probability
  set ProbChiSqFastVtxTight  "ProbChiSq 0.0005:"

# pre- and post-fit mass, p* cuts
  set verylooseMass          "Mass 1.5:2.3"
  set looseMass              "Mass 1.75:1.96"
  set preCmsP                "CmsP 2.0:"
  set postCmsP               "CmsP 2.4:"

mod clone SmpMakerDefiner D0ToKsPimPipPi0
sequence append $mySkimSequence D0ToKsPimPipPi0
talkto D0ToKsPimPipPi0 {
    decayMode          set "D0 -> K_S0 pi- pi+ pi0"

    daughterListNames  set $ksListD
    daughterListNames  set $pionListD
    daughterListNames  set $pionListD
    daughterListNames  set $pi0ListD


  fittingAlgorithm  set "Cascade"
  fitConstraints    set "Geo"

  preFitSelectors   set $preCmsP
  preFitSelectors   set $verylooseMass

  postFitSelectors  set $ProbChiSqFastVtxTight
  postFitSelectors  set $postCmsP
  postFitSelectors  set $looseMass
}


mod clone SmpMakerDefiner D0ToKmPipPipPimPi0
sequence append $mySkimSequence D0ToKmPipPipPimPi0
talkto D0ToKmPipPipPimPi0 {
    decayMode          set "D0 -> K- pi+ pi+ pi- pi0"

    daughterListNames  set $kListD
    daughterListNames  set $pionListD
    daughterListNames  set $pionListD
    daughterListNames  set $pionListD
    daughterListNames  set $pi0ListD

  fittingAlgorithm  set "Cascade"
  fitConstraints    set "Geo"

  preFitSelectors   set $preCmsP
  preFitSelectors   set $verylooseMass

  postFitSelectors  set $ProbChiSqFastVtxTight
  postFitSelectors  set $postCmsP
  postFitSelectors  set $looseMass

}




mod clone FilterByList D0ToKnPiPi0Filter
path append D0ToKnPiPi0Path D0ToKnPiPi0Filter
talkto D0ToKnPiPi0Filter {
    orList set D0ToKmPipPipPimPi0
    orList set D0ToKsPimPipPi0
}