#
# $Id: BtoXGammaPath.tcl,v 1.8 2007/10/22 11:21:17 marchior Exp $
#
# defines DstarKpiPath tag filter path
#
path create BtoXGammaPath

# create tagbit filter
# mod clone TagFilterByName BtoXGammaBGF

# accept tags that we are interested in
# mod talk BtoXGammaBGF
#     orList set BGFMultiHadron
# ###    orList set BGFNeutralHadron
# exit

# path append BtoXGammaPath BtoXGammaBGF

path append BtoXGammaPath BtoXGammaFilter

# load the sequence that creates the BtoXGamma lists
sourceFoundFile FilterTools/BtoXGammaSequence.tcl
path append BtoXGammaPath BtoXGammaSequence

# add here all lists for which we would like a tag in the skim
# this takes some extra CPU, so maybe it needs more thinking.
set liststotag { 
    BtoXGamma_GammaHard 
    B0toPhiGamma
    B0toOmegaGamma
    B0toRho0Gamma
    BtoRhoGamma
    B0toGammaGamma
    B0toKsPi0Gamma
    B0toKPiGamma
    BtoKPi0Gamma
    BtoKsPiGamma
    B0toKsEtaGamma
    BtoKEtaGamma
    B0toKsEtaPGamma
    BtoKEtaPGamma
    B0toKsPhiGamma
    BtoKPhiGamma
    B0toPiPiGamma
    BtoPiPi0Gamma
    B0toKst0GammaKst0toKPi
    B0toKst0GammaKst0toKsPi0
    BtoKstCGammaKstCtoKPi0
    BtoKstCGammaKstCtoKsPi }
#    B0toKPiPi0Gamma
#    BtoKPiPiGamma
#    B0toKsPiPiGamma
#    BtoKsPiPi0Gamma

foreach list $liststotag {
    mod clone TagFromList ${list}TagFromListXgamma
    path append BtoXGammaPath ${list}TagFromListXgamma
    set tagname BtoXGammaTag_${list}
    talkto ${list}TagFromListXgamma {
	tagName    set $tagname
	listsToTag set $list
        production set true
        verbose set false
    }
#    moved to SkimMini/listOfTagBits.tcl
#    catch { talkto TagInspector { boolTagList set $tagname } }
}
