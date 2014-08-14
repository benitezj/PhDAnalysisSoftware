#
# $Id: Jpsi_Psi2sToKsKsGammaPath.tcl,v 1.2 2007/07/18 01:16:07 echenard Exp $
#
# defines Jpsi_Psi2sToKsKsGammaPath tag filter path
#
# Description:
# ------------
# Select radiative J/psi and Psi2s decays into two Ks, J/psi->Ks Ks gamma 
# and Psi(2s)-> Ks Ks gamma, to study the Ks Ks intermediate state. The 
# J/psi and Psi(2s) are mainly produced in ISR events, but no attemp is made to 
# identify the ISR photon (inclusive ISR selection). 
# 
# Details can be found in doc/Jpsi_Psi2sToKsKsGamma.readme
#
#
#====================== GENERAL ===============================

global BtaCandLists

sourceFoundFile EventTagTools/TagProcs.tcl
path create Jpsi_Psi2sToKsKsGammaPath



#------------------------------------------------------------------

### TAG FILTER ON NUMBER OF TRACKS

 module clone TagFilterByValue         TagJpsi_PsiToKsKsnTracks
 path append Jpsi_Psi2sToKsKsGammaPath TagJpsi_PsiToKsKsnTracks
 mod talk TagJpsi_PsiToKsKsnTracks
   TagValueName set nTracks
   TagValueType set Integer
   TagValueMinimum set 4
   TagValueMaximum set 5
   assertIfMissing set true
 exit

 module clone TagFilterByValue         TagJpsi_PsiToKsKsnKsLoose
 path append Jpsi_Psi2sToKsKsGammaPath TagJpsi_PsiToKsKsnKsLoose
 mod talk TagJpsi_PsiToKsKsnKsLoose
   TagValueName set nKsLoose
   TagValueType set Integer
   TagValueMinimum set 2
   TagValueMaximum set 50
   assertIfMissing set true
 exit

#------------------------------------------------------------------

### DEFINE LISTS HERE

 mod clone  SmpMakerDefiner            JpsiToKsKsGamma
 path append Jpsi_Psi2sToKsKsGammaPath JpsiToKsKsGamma
 mod talk JpsiToKsKsGamma     
     decayMode         set "J/psi -> K_S0 K_S0 gamma"
     daughterListNames set "KsLoose"
     daughterListNames set "KsLoose"
     daughterListNames set "GoodPhotonLoose"

     preFitSelectors   set "Mass pdg-0.5:pdg+0.3"
     fittingAlgorithm  set "TreeFitter"
 exit


 mod clone  SmpMakerDefiner            Psi2sToKsKsGamma
 path append Jpsi_Psi2sToKsKsGammaPath Psi2sToKsKsGamma
 mod talk Psi2sToKsKsGamma     
     decayMode         set "psi(2S) -> K_S0 K_S0 gamma"
     daughterListNames set "KsLoose"
     daughterListNames set "KsLoose"
     daughterListNames set "GoodPhotonLoose"

     preFitSelectors   set "Mass pdg-0.3:pdg+0.3"
     fittingAlgorithm  set "TreeFitter"
 exit



#------------------------------------------------------------------

### TAG BITS

 mod clone TagFromList                 TagJpsiToKsKsGamma
 path append Jpsi_Psi2sToKsKsGammaPath TagJpsiToKsKsGamma
 mod talk TagJpsiToKsKsGamma
   listsToTag set JpsiToKsKsGamma
   tagName set JpsiToKsKsGamma
 exit

 mod clone TagFromList                 TagPsi2sToKsKsGamma
 path append Jpsi_Psi2sToKsKsGammaPath TagPsi2sToKsKsGamma
 mod talk TagPsi2sToKsKsGamma
   listsToTag set Psi2sToKsKsGamma
   tagName set Psi2sToKsKsGamma
 exit


### FILTER ON THESE BITS

 mod clone TagFilterByName             JpsiPsi2sToKsKsGammaFilter
 path append Jpsi_Psi2sToKsKsGammaPath JpsiPsi2sToKsKsGammaFilter
 mod talk JpsiPsi2sToKsKsGammaFilter
   orList set JpsiToKsKsGamma
   orList set Psi2sToKsKsGamma
 exit




#------------------------------------------------------------------

## ADD JPSIKSKSGAMMA AND PSI2SKSKSGAMMA LISTS 

 lappend BtaCandLists Jpsi2sToKsKsGamma Psi2sToKsKsGamma













