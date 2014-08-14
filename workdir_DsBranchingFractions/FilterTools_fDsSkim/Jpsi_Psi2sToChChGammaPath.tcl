#
# $Id: Jpsi_Psi2sToChChGammaPath.tcl,v 1.2 2007/07/18 01:17:31 echenard Exp $
#
# Defines Jpsi_Psi2sToChChGammaPath tag filter path
#
# Description:
# ------------
# Select radiative J/psi and Psi2s decays into two charged hadrons, J/psi->H+ H- gamma 
# and Psi(2s)->H+ H- gamma, where H=pi,K,p, to study the H+ H- intermediate state. The 
# J/psi and Psi(2s) are mainly produced in ISR events, but no attemp is made to 
# identify the ISR photon (inclusive ISR selection). Particle identification is used to
# select the pions, kaons and protons.
#
# Details can be found in doc/Jpsi_Psi2sToChChGamma.readme
#
#
#
#====================== GENERAL ===============================

global BtaCandLists

sourceFoundFile EventTagTools/TagProcs.tcl
path create Jpsi_Psi2sToChChGammaPath




#=================== FILTER ON NUMBER OF GTL ==================

 module clone TagFilterByValue         TagJpsi_PsiToChChnTracks
 path append Jpsi_Psi2sToChChGammaPath TagJpsi_PsiToChChnTracks
 mod talk                              TagJpsi_PsiToChChnTracks
   TagValueName set nGoodTrkLoose 
   TagValueType set Integer
   TagValueMinimum set 1.5
   TagValueMaximum set 2.5
   assertIfMissing set true
 exit





#====================== COMPOSITION ===============================

#-------------DEFINE REFINED PID LISTS HERE------------------------

 mod clone   SmpSubListerDefiner       piLH_GTL
 path append Jpsi_Psi2sToChChGammaPath piLH_GTL
 mod talk                              piLH_GTL     
    unrefinedListName  set piLHLoose
    isCloneOfListName  set GoodTracksLoose
 exit

 mod clone  SmpSubListerDefiner        KLH_GTL
 path append Jpsi_Psi2sToChChGammaPath KLH_GTL
 mod talk                              KLH_GTL     
    unrefinedListName  set KLHNotPion
    isCloneOfListName  set GoodTracksLoose
 exit

 mod clone  SmpSubListerDefiner        pLH_GTL
 path append Jpsi_Psi2sToChChGammaPath pLH_GTL
 mod talk                              pLH_GTL     
    unrefinedListName  set pLHVeryLoose
    isCloneOfListName  set GoodTracksLoose
 exit


#-------------DEFINE J/PSI DECAY LISTS HERE------------------------

 mod clone  SmpMakerDefiner            JpsiToPiPiGamma
 path append Jpsi_Psi2sToChChGammaPath JpsiToPiPiGamma
 mod talk                              JpsiToPiPiGamma
     decayMode         set "J/psi -> pi+ pi- gamma"
     daughterListNames set "piLH_GTL"
     daughterListNames set "piLH_GTL"
     daughterListNames set "GoodPhotonLoose"
                                                                                
     preFitSelectors   set "Mass pdg-0.5:pdg+0.3"
     fittingAlgorithm  set "TreeFitter"
 exit


 mod clone  SmpMakerDefiner            JpsiToKKGamma
 path append Jpsi_Psi2sToChChGammaPath JpsiToKKGamma
 mod talk                              JpsiToKKGamma
     decayMode         set "J/psi -> K+ K- gamma"
     daughterListNames set "KLH_GTL"
     daughterListNames set "KLH_GTL"
     daughterListNames set "GoodPhotonLoose"
                                                                                
     preFitSelectors   set "Mass pdg-0.5:pdg+0.3"
     fittingAlgorithm  set "TreeFitter"
 exit

 mod clone  SmpMakerDefiner            JpsiToPPGamma
 path append Jpsi_Psi2sToChChGammaPath JpsiToPPGamma
 mod talk                              JpsiToPPGamma
     decayMode         set "J/psi -> p+ anti-p- gamma"
     daughterListNames set "pLH_GTL"
     daughterListNames set "pLH_GTL"
     daughterListNames set "GoodPhotonLoose"

     preFitSelectors   set "Mass pdg-0.5:pdg+0.3"
     fittingAlgorithm  set "TreeFitter"
 exit
                                                                                

#-------------DEFINE JPSI(2S) DECAY LISTS HERE------------------------


 mod clone  SmpMakerDefiner            Psi2sToPiPiGamma
 path append Jpsi_Psi2sToChChGammaPath Psi2sToPiPiGamma
 mod talk                              Psi2sToPiPiGamma
     decayMode         set "psi(2S) -> pi+ pi- gamma"
     daughterListNames set "piLH_GTL"
     daughterListNames set "piLH_GTL"
     daughterListNames set "GoodPhotonLoose"

     preFitSelectors   set "Mass pdg-0.3:pdg+0.3"
     fittingAlgorithm  set "TreeFitter"
 exit

 mod clone  SmpMakerDefiner            Psi2sToKKGamma
 path append Jpsi_Psi2sToChChGammaPath Psi2sToKKGamma
 mod talk                              Psi2sToKKGamma
     decayMode         set "psi(2S) -> K+ K- gamma"
     daughterListNames set "KLH_GTL"
     daughterListNames set "KLH_GTL"
     daughterListNames set "GoodPhotonLoose"

     preFitSelectors   set "Mass pdg-0.3:pdg+0.3"
     fittingAlgorithm  set "TreeFitter"
 exit

 mod clone  SmpMakerDefiner            Psi2sToPPGamma
 path append Jpsi_Psi2sToChChGammaPath Psi2sToPPGamma
 mod talk                              Psi2sToPPGamma
     decayMode         set "psi(2S) -> p+ anti-p- gamma"
     daughterListNames set "pLH_GTL"
     daughterListNames set "pLH_GTL"
     daughterListNames set "GoodPhotonLoose"

     preFitSelectors   set "Mass pdg-0.3:pdg+0.3"
     fittingAlgorithm  set "TreeFitter"
 exit




#====================== TAG BITS ===============================

#-------------------TAG BITS FOR J/PSI--------------------------

 mod clone TagFromList                 TagJpsiToPiPiGamma
 path append Jpsi_Psi2sToChChGammaPath TagJpsiToPiPiGamma
 mod talk                              TagJpsiToPiPiGamma
   listsToTag set JpsiToPiPiGamma
   tagName    set JpsiToPiPiGamma
 exit

 mod clone TagFromList                 TagJpsiToKKGamma
 path append Jpsi_Psi2sToChChGammaPath TagJpsiToKKGamma
 mod talk                              TagJpsiToKKGamma
   listsToTag set JpsiToKKGamma
   tagName    set JpsiToKKGamma
 exit

 mod clone TagFromList                 TagJpsiToPPGamma
 path append Jpsi_Psi2sToChChGammaPath TagJpsiToPPGamma
 mod talk                              TagJpsiToPPGamma
   listsToTag set JpsiToPPGamma
   tagName    set JpsiToPPGamma
 exit


#-------------------TAG BITS FOR PSI(2S)-------------------------

 mod clone TagFromList                 TagPsi2sToPiPiGamma
 path append Jpsi_Psi2sToChChGammaPath TagPsi2sToPiPiGamma
 mod talk                              TagPsi2sToPiPiGamma
   listsToTag set Psi2sToPiPiGamma
   tagName    set Psi2sToPiPiGamma
 exit

 mod clone TagFromList                 TagPsi2sToKKGamma
 path append Jpsi_Psi2sToChChGammaPath TagPsi2sToKKGamma
 mod talk                              TagPsi2sToKKGamma
   listsToTag set Psi2sToKKGamma
   tagName    set Psi2sToKKGamma
 exit

 mod clone TagFromList                 TagPsi2sToPPGamma
 path append Jpsi_Psi2sToChChGammaPath TagPsi2sToPPGamma
 mod talk                              TagPsi2sToPPGamma
   listsToTag set Psi2sToPPGamma
   tagName    set Psi2sToPPGamma
 exit



#====================== FINAL FILTER ==============================


 path append Jpsi_Psi2sToChChGammaPath Jpsi_Psi2sToChChGammaFilter


## OLD   FILTER ON THE TAGBITS ONLY WITHOUT FILTER
# mod clone TagFilterByName             JpsiPsi2sToChChGammaTagFilt
# path append Jpsi_Psi2sToChChGammaPath JpsiPsi2sToChChGammaTagFilt
# mod talk                              JpsiPsi2sToChChGammaTagFilt
#   orList set JpsiToPiPiGamma
#   orList set JpsiToKKGamma
#   orList set JpsiToPPGamma
#   orList set Psi2sToPiPiGamma
#   orList set Psi2sToKKGamma
#   orList set Psi2sToPPGamma
# exit


#================== PERSIST BTACANDIDATES LISTS ===================

 lappend BtaCandLists JpsiToPiPiGamma  JpsiToKKGamma  JpsiToPPGamma 
 lappend BtaCandLists Psi2sToPiPiGamma Psi2sToKKGamma Psi2sToPPGamma















