# $Id: BtoXGammaSequence.tcl,v 1.3 2005/11/02 00:38:51 hulsberg Exp $

#########################################################################
# list definitions for exclusive B->X gamma decays
#########################################################################

global BtoXGammaSequenceDefinition

if [ expr ! [info exists BtoXGammaSequenceDefinition ] ] {
    set BtoXGammaSequenceDefinition 1
    
    sequence create BtoXGammaSequence
    
#set pi0list      "pi0Loose"
#set kslist       "KsDefault"
#set kaonlist     "KLHNotPion"
#set pionlist     "ChargedTracks"
#set etalist      "etaDefault"
#set etaprimelist "etaPDefault"


# proc to create one B->Xgamma list
proc createbtoxgammalist { sequence name mode daulists } {
    mod clone SmpMakerDefiner $name
    sequence append $sequence $name
    foreach dau $daulists {
	set thedaughter {uplevel ${dau}}
	talkto $name {
	    daughterListNames set $dau
	}
    }
    talkto $name {
	decayMode         set $mode
	fittingAlgorithm  set "Add4"
	# leave sufficent room for vertexing these candidates after the skimming
	preFitSelectors   set "Mass pdg-0.5:pdg+0.5"
	postFitSelectors  set "Mass pdg-0.4:pdg+0.4"
	postFitSelectors  set "Mmiss 5.0:5.4"
	createUsrData     set true
    }
}

#########################################################################
#
# primary photons
#
#########################################################################

createsmpsublister BtoXGammaSequence BtoXGamma_GammaHard {
    unrefinedListName set "GoodPhotonLoose"
    selectors         set "CmsE 1.5:3.5"
}

createsmpsublister BtoXGammaSequence BtoBaryonAntiBaryonGamma_GammaHard {
    unrefinedListName set "GoodPhotonLoose"
    selectors         set "CmsE 0.3:3.5"
}

##########################################################################
#
# B0 -> phi gamma
#
##########################################################################

createbtoxgammalist BtoXGammaSequence B0toPhiGamma "B0 -> gamma phi" {
    "BtoXGamma_GammaHard" "phiDefault" }

##########################################################################
#
# B0 -> omega gamma
#
##########################################################################

createbtoxgammalist BtoXGammaSequence B0toOmegaGamma "B0 -> gamma omega" {
    "BtoXGamma_GammaHard" "omegaDefault" }

##########################################################################
#
# B0 -> rho0 gamma
#
##########################################################################

createbtoxgammalist BtoXGammaSequence B0toRho0Gamma "B0 -> gamma rho0" {
    "BtoXGamma_GammaHard" "rho0Default" }

##########################################################################
#
# B+ -> rho+ gamma
#
##########################################################################

createbtoxgammalist BtoXGammaSequence BtoRhoGamma "B+ -> gamma rho+" {
    "BtoXGamma_GammaHard" "rhoCDefault" }

##########################################################################
#
# B0 -> gamma gamma
#
##########################################################################

createbtoxgammalist BtoXGammaSequence B0toGammaGamma "B0 -> gamma gamma" {
    "BtoXGamma_GammaHard" "GoodPhotonLoose" }

##########################################################################
#
# B0 ->Ks pi0 gamma
#
##########################################################################

createbtoxgammalist BtoXGammaSequence B0toKsPi0Gamma "B0 -> gamma K_S0 pi0" { 
    "BtoXGamma_GammaHard" "KsDefault" "pi0Loose" }

##########################################################################
#
# B0 ->K+ pi- gamma
#
##########################################################################

createbtoxgammalist BtoXGammaSequence B0toKPiGamma "B0 -> gamma K+ pi-" { 
    "BtoXGamma_GammaHard" "KLHNotPion" "ChargedTracks" }

##########################################################################
#
# B+ ->K+ pi0 gamma
#
##########################################################################

createbtoxgammalist BtoXGammaSequence BtoKPi0Gamma "B+ -> gamma K+ pi0" { 
    "BtoXGamma_GammaHard" "KLHNotPion" "pi0Loose" }


##########################################################################
#
# B+ ->KS pi+ gamma
#
##########################################################################

createbtoxgammalist BtoXGammaSequence BtoKsPiGamma "B+ -> gamma K_S0 pi+" { 
    "BtoXGamma_GammaHard" "KsDefault" "ChargedTracks"}


##########################################################################
#
# B0 ->Ks eta gamma
#
##########################################################################

createbtoxgammalist BtoXGammaSequence B0toKsEtaGamma "B0 -> gamma K_S0 eta" { 
    "BtoXGamma_GammaHard" "KsDefault" "etaDefault" }

##########################################################################
#
# B+ ->K+ eta gamma
#
##########################################################################

createbtoxgammalist BtoXGammaSequence BtoKEtaGamma "B+ -> gamma K+ eta" { 
    "BtoXGamma_GammaHard" "KLHNotPion" "etaDefault" }

##########################################################################
#
# B0 ->Ks eta' gamma
#
##########################################################################

createbtoxgammalist BtoXGammaSequence B0toKsEtaPGamma "B0 -> gamma K_S0 eta'" { 
    "BtoXGamma_GammaHard" "KsDefault" "etaPDefault" }

##########################################################################
#
# B+ ->K+ eta' gamma
#
##########################################################################

createbtoxgammalist BtoXGammaSequence BtoKEtaPGamma "B+ -> gamma K+ eta'" { 
    "BtoXGamma_GammaHard" "KLHNotPion" "etaPDefault" }


##########################################################################
#
# B0 ->Ks phi gamma
#
##########################################################################

createbtoxgammalist BtoXGammaSequence B0toKsPhiGamma "B0 -> gamma K_S0 phi" { 
    "BtoXGamma_GammaHard" "KsDefault" "phiDefault" }

##########################################################################
#
# B+ ->K+ phi gamma
#
##########################################################################

createbtoxgammalist BtoXGammaSequence BtoKPhiGamma "B+ -> gamma K+ phi" { 
    "BtoXGamma_GammaHard" "KLHNotPion" "phiDefault" }

##########################################################################
#
# B0 -> pi+ pi- gamma
#
##########################################################################

createbtoxgammalist BtoXGammaSequence B0toPiPiGamma "B0 -> gamma pi+ pi-" { 
    "BtoXGamma_GammaHard" "ChargedTracks" "ChargedTracks" }

##########################################################################
#
# B+ -> pi+ pi0 gamma
#
##########################################################################

createbtoxgammalist BtoXGammaSequence BtoPiPi0Gamma "B+ -> gamma pi+ pi0" { 
    "BtoXGamma_GammaHard" "ChargedTracks" "pi0Loose" }

##########################################################################
#
# B0 -> K+ pi- pi0 gamma
#
##########################################################################

createbtoxgammalist BtoXGammaSequence B0toKPiPi0Gamma "B0 -> gamma K+ pi- pi0" { 
    "BtoXGamma_GammaHard" "KLHNotPion" "ChargedTracks" "pi0Loose" }

##########################################################################
#
# B+ -> K+ pi- pi+ gamma
#
##########################################################################

createbtoxgammalist BtoXGammaSequence BtoKPiPiGamma "B+ -> gamma K+ pi- pi+" { 
    "BtoXGamma_GammaHard" "KLHNotPion" "ChargedTracks" "ChargedTracks" }

##########################################################################
#
# B0 -> K_S0 pi- pi+ gamma
#
##########################################################################

createbtoxgammalist BtoXGammaSequence B0toKsPiPiGamma "B0 -> gamma K_S0 pi- pi+" { 
    "BtoXGamma_GammaHard" "KsDefault" "ChargedTracks" "ChargedTracks" }

##########################################################################
#
# B+ -> K_S0 pi+ pi0 gamma
#
##########################################################################

createbtoxgammalist BtoXGammaSequence BtoKsPiPi0Gamma "B+ -> gamma K_S0 pi+ pi0" { 
    "BtoXGamma_GammaHard" "KsDefault" "ChargedTracks" "pi0Loose" }


##########################################################################
#
# B- -> lambda anti-proton gamma 
#
##########################################################################

createbtoxgammalist BtoXGammaSequence BtoLambdaPbarGamma "B- -> gamma Lambda0 anti-p-" { 
    "BtoBaryonAntiBaryonGamma_GammaHard" "LambdaVeryLoosePID" "ChargedTracks" }

##########################################################################
#
# B- -> sigma0 anti-proton gamma 
#
##########################################################################

createbtoxgammalist BtoXGammaSequence BtoSigma0PbarGamma "B- -> gamma Sigma0 anti-p-" { 
    "BtoBaryonAntiBaryonGamma_GammaHard" "Sigma0ToLambdaPhotonVeryLoosePID" "ChargedTracks" }

#############################################################################
# Below follow lists migrated from the CompositionTools/CompPenguinSequence
#############################################################################

createsmpsublister BtoXGammaSequence KstarKPiPengDefault {
    unrefinedListName set KstarKPiVeryLoose
    selectors         set "Mass 0.5:1.3" 
}

createsmpsublister BtoXGammaSequence KstarKsPi0PengDefault {
    unrefinedListName set KstarKPiVeryLoose
    selectors         set "Mass 0.5:1.3" 
}

createsmpsublister BtoXGammaSequence KstarKsPiPengDefault {
    unrefinedListName set KstarKsPiVeryLoose
    selectors         set "Mass 0.5:1.3" 
}

createsmpsublister BtoXGammaSequence KstarKPi0PengDefault {
    unrefinedListName set KstarKPi0VeryLoose
    selectors         set "Mass 0.5:1.3" 
}

createbtoxgammalist BtoXGammaSequence B0toKst0GammaKst0toKPi "B0 -> gamma K*0" {
    "BtoXGamma_GammaHard" "KstarKPiPengDefault" }

createbtoxgammalist BtoXGammaSequence B0toKst0GammaKst0toKsPi0 "B0 -> gamma K*0" {
    "BtoXGamma_GammaHard" "KstarKsPi0PengDefault" }


createbtoxgammalist BtoXGammaSequence BtoKstCGammaKstCtoKPi0 "B+ -> gamma K*+" {
    "BtoXGamma_GammaHard" "KstarKPi0PengDefault" }


createbtoxgammalist BtoXGammaSequence BtoKstCGammaKstCtoKsPi "B+ -> gamma K*+" {
    "BtoXGamma_GammaHard" "KstarKsPiPengDefault" }

}