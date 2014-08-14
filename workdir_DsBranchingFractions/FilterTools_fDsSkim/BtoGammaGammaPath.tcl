#
# $Id: BtoGammaGammaPath.tcl,v 1.5 2005/06/01 16:44:11 mommsen Exp $
#
sourceFoundFile ErrLogger/ErrLog.tcl

global BtaCandLists
global CandBranches
global writeUsrDataForSkim

path create BtoGammaGammaPath
seq create BtoGammaGammaTag
path append BtoGammaGammaPath BtoGammaGammaTag

# Build tag sequence
mod clone TagFilterByName BtoGammaGammaTagList
seq append BtoGammaGammaTag BtoGammaGammaTagList
talkto BtoGammaGammaTagList {
    orList set BGFMultiHadron
    orList set BGFNeutralHadron
    assertIfMissing set true
}

mod clone TagFilterByValue BtoGammaGammaR2All
seq append BtoGammaGammaTag BtoGammaGammaR2All
talkto BtoGammaGammaR2All {
    TagValueName  set R2All
    TagValueType  set Floating
    TagValueMinimum set 0.0
    TagValueMaximum set 0.9
}

# Use simple composition to build candidates
sequence create BggSkim
path append BtoGammaGammaPath BggSkim

mod clone SmpMergerDefiner MyGammaList
seq append BggSkim MyGammaList
talkto MyGammaList {
    inputListNames set GoodPhotonLoose
    inputListNames set "gammaConversionDefault"
} 

mod clone SmpSubListerDefiner MyGamma
seq append BggSkim MyGamma
talkto MyGamma {
    unrefinedListName  set "MyGammaList"
    selectors          set "CmsE 1.15:3.5"
}

mod clone SmpMakerDefiner BtoGammaGamma
seq append BggSkim BtoGammaGamma
talkto BtoGammaGamma {
    decayMode          set "B0 -> gamma gamma"
    daughterListNames  set "MyGamma"
    daughterListNames  set "MyGamma"
    fittingAlgorithm   set "Add4"
    fitConstraints     set "PrimaryVertex"
    postFitSelectors   set "Mes 5.1:5.3"
    postFitSelectors   set "DeltaE -0.5:0.5"
}

path append BtoGammaGammaPath BtoGammaGammaFilter

# Write found candidates and usr data
lappend BtaCandLists BtoGammaGamma

lappend CandBranches "Photon   gamma         p4"
lappend CandBranches "B0       B0    anti-B0 p4"

set writeUsrDataForSkim 1
