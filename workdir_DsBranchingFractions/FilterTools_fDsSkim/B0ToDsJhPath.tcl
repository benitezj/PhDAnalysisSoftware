#
# $Id: B0ToDsJhPath.tcl,v 1.3 2005/08/03 16:04:57 fnc Exp $
#
# =======================================================================
# Defines B0 -> DsJ h path
# =======================================================================
# Decay modes considered for skimming..

#1. B0 -> Dsj(2460)-   pi+  -->  [Ds+ gamma] pi+ || "B0 -> D_s1- pi+
#2. B0 -> Dsj(2460)-   pi+  -->  [Ds*+ pi0]  pi+ || "B0 -> D_s1- pi+" 
#3. B0 -> Dsj*(2317)-  pi+  -->  [Ds+ pi0]   pi+ || "B0 -> D_s0*- pi+"

#4. B0 -> Dsj(2460)+   K-  -->   [Ds+ gamma] K+  || "B0 -> D_s1- K+
#5. B0 -> Dsj(2460)+   K-  -->   [Ds*+ pi0]  K+  || "B0 -> D_s1- K+"   
#6. B0 -> Dsj*(2317)+  K-  -->   [Ds+ pi0]   K+  || "B0 -> D_s0*- K+" 
#
# Author : Asish Satpathy ; Univ. of Texas at Austin
#========================================================================
global BtaCandLists
global writeUsrDataForSkim

set writeUsrDataForSkim 1

path create B0ToDsJhPath

sequence create DsJhSkim

# Create decaylist using SimpleComposition Package

mod clone SmpMakerDefiner D_sstar
talkto D_sstar {
    decayMode         set "D_s*- -> D_s- gamma"
    checkType         set false
    daughterListNames set "DsMainDefault"
    daughterListNames set "GoodPhotonLoose"
    disableCloneCheck set true

    createUsrData     set true
    fittingAlgorithm  set "Add4"  
    preFitSelectors   set "Mass 2.05:2.17"
    postFitSelectors  set "Mass 2.05:2.17"
}
sequence append DsJhSkim D_sstar


mod clone SmpMakerDefiner B0ToDs1Pi
talkto B0ToDs1Pi {
    decayMode         set "B0 -> D_s- gamma pi+"
    checkType         set false
    daughterListNames set "DsMainDefault"
    daughterListNames set "GoodPhotonLoose"
    daughterListNames set "piLHVeryLoose"
    disableCloneCheck set true
    createUsrData     set true
    fittingAlgorithm  set "Add4"  
    preFitSelectors   set "DeltaE -0.32:0.32"
    preFitSelectors   set "Mes 5.18:5.32"
    postFitSelectors  set "DeltaE -0.32:0.32"
    postFitSelectors  set "Mes 5.18:5.32"
}
sequence append DsJhSkim B0ToDs1Pi

mod clone SmpMakerDefiner B0ToDs2Pi
talkto B0ToDs2Pi {
    decayMode         set "B0 -> D_s*- pi0 pi+"
    checkType         set false
    daughterListNames set "D_sstar"
    daughterListNames set "pi0VeryLoose"
    daughterListNames set "piLHVeryLoose"
    disableCloneCheck set true
    createUsrData     set true
    fittingAlgorithm  set "Add4"  
    preFitSelectors   set "DeltaE -0.32:0.32"
    preFitSelectors   set "Mes 5.18:5.32"
    postFitSelectors  set "DeltaE -0.32:0.32"
    postFitSelectors  set "Mes 5.18:5.32"
}
sequence append DsJhSkim B0ToDs2Pi

mod clone SmpMakerDefiner B0ToDs3Pi
talkto B0ToDs3Pi {
    decayMode         set "B0 -> D_s- pi0 pi+"
    checkType         set false
    daughterListNames set "DsMainDefault"
    daughterListNames set "pi0VeryLoose"
    daughterListNames set "piLHVeryLoose"
    disableCloneCheck set true
    createUsrData     set true
    fittingAlgorithm  set "Add4"  
    preFitSelectors   set "DeltaE -0.32:0.32"
    preFitSelectors   set "Mes 5.18:5.32"
    postFitSelectors  set "DeltaE -0.32:0.32"
    postFitSelectors  set "Mes 5.18:5.32"
}
sequence append DsJhSkim B0ToDs3Pi


mod clone SmpMakerDefiner B0ToDs1K
talkto B0ToDs1K {
    decayMode         set "B0 -> D_s- gamma K+"
    checkType         set false
    daughterListNames set "DsMainDefault"
    daughterListNames set "GoodPhotonLoose"
    daughterListNames set "KLHNotPion"
    disableCloneCheck set true
    createUsrData     set true
    fittingAlgorithm  set "Add4"  
    preFitSelectors   set "DeltaE -0.32:0.32"
    preFitSelectors   set "Mes 5.18:5.32"
    postFitSelectors  set "DeltaE -0.32:0.32"
    postFitSelectors  set "Mes 5.18:5.32"
}
sequence append DsJhSkim B0ToDs1K

mod clone SmpMakerDefiner B0ToDs2K
talkto B0ToDs2K {
    decayMode         set "B0 -> D_s*- pi0 K+"
    checkType         set false
    daughterListNames set "D_sstar"
    daughterListNames set "pi0VeryLoose"
    daughterListNames set "KLHNotPion"
    disableCloneCheck set true
    createUsrData     set true
    fittingAlgorithm  set "Add4"  
    preFitSelectors   set "DeltaE -0.32:0.32"
    preFitSelectors   set "Mes 5.18:5.32"
    postFitSelectors  set "DeltaE -0.32:0.32"
    postFitSelectors  set "Mes 5.18:5.32"
}
sequence append DsJhSkim B0ToDs2K

mod clone SmpMakerDefiner B0ToDs3K
talkto B0ToDs3K {
    decayMode         set "B0 -> D_s- pi0 K+"
    checkType         set false
    daughterListNames set "DsMainDefault"
    daughterListNames set "pi0VeryLoose"
    daughterListNames set "KLHNotPion"
    disableCloneCheck set true
    createUsrData     set true
    fittingAlgorithm  set "Add4"  
    preFitSelectors   set "DeltaE -0.32:0.32"
    preFitSelectors   set "Mes 5.18:5.32"
    postFitSelectors  set "DeltaE -0.32:0.32"
    postFitSelectors  set "Mes 5.18:5.32"
}
sequence append DsJhSkim B0ToDs3K


# only run on Multihadrons
path append B0ToDsJhPath FilterBGFMultiHadron

module clone TagFromList     TagBToDsJ
module clone TagFilterByName BToDsJFilter

seq append DsJhSkim TagBToDsJ
seq append DsJhSkim BToDsJFilter

mod talk TagBToDsJ
  listsToTag set B0ToDs1Pi
  listsToTag set B0ToDs2Pi
  listsToTag set B0ToDs3Pi
  listsToTag set B0ToDs1K
  listsToTag set B0ToDs2K
  listsToTag set B0ToDs3K 
  tagName    set tagBToDsJ
  verbose    set false
  production set true
exit

mod talk BToDsJFilter
  orList set tagBToDsJ
  verbose set false
  production set true
exit

path append B0ToDsJhPath DsJhSkim

lappend BtaCandLists B0ToDs1K B0ToDs2K B0ToDs3K B0ToDs1Pi B0ToDs2Pi B0ToDs3Pi
lappend BtaCandLists D_sstar
