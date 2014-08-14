#
# $Id: BToJpsiHHPath.tcl,v 1.7 2005/11/16 23:01:56 fwinkl Exp $
#
#
# defines BToJpsiHHPath tag filter path
#
#

global BtaCandLists
global writeUsrDataForSkim
global CandBranches

path create BToJpsiHHPath

path append BToJpsiHHPath FilterBGFMultiHadron
path append BToJpsiHHPath BCountMHFilter

# make sure tag bits get produced
sourceFoundFile EventTagTools/TagProcs.tcl
useTagFromList JpsiELoose  BToJpsiHHPath
useTagFromList JpsiMuLoose BToJpsiHHPath

# filter on tag bits
mod clone TagFilterByName BToJpsiHHPreFilter
path append BToJpsiHHPath BToJpsiHHPreFilter
mod talk BToJpsiHHPreFilter
  orList set JpsiELoose
  orList set JpsiMuLoose
  production set true
  verbose set false
exit

mod clone SmpMergerDefiner JpsiFromBToJpsiHH
path append BToJpsiHHPath JpsiFromBToJpsiHH
mod talk JpsiFromBToJpsiHH
  inputListNames set JPsiTightEE
  inputListNames set JPsiNNVeryLooseMuMu
  disableCloneCheck  set t
exit

mod clone SmpRefitterDefiner JpsiFromBToJpsiHHMass
path append BToJpsiHHPath JpsiFromBToJpsiHHMass
mod talk JpsiFromBToJpsiHHMass
 unrefinedListName set "JpsiFromBToJpsiHH"
 fittingAlgorithm   set "Cascade"
 fitConstraints     set "Geo"
 fitConstraints     set "Mass"
 preFitSelectors set "Mass 2.85:3.35"
exit
 
sequence create B0ToJpsiHHSequence
path append BToJpsiHHPath B0ToJpsiHHSequence

mod clone SmpMakerDefiner myPiPiList
sequence append B0ToJpsiHHSequence myPiPiList
mod talk myPiPiList
decayMode set "rho0 -> pi+ pi-"
daughterListNames set "GoodTracksVeryLoose"
daughterListNames set "GoodTracksVeryLoose"
fittingAlgorithm set "Cascade"
fitConstraints set "Geo"
fitConstraints set "Momentum"
postFitSelectors set "Mass 0.2:3.0"
exit

mod clone SmpMakerDefiner B0ToJpsiPiPi
sequence append B0ToJpsiHHSequence B0ToJpsiPiPi
mod talk B0ToJpsiPiPi
decayMode set "B0 -> J/psi rho0"
daughterListNames set "JpsiFromBToJpsiHHMass"
daughterListNames set "myPiPiList"
preFitSelectors set "Mass 3.0:7.0"
fittingAlgorithm set "Cascade"
fitConstraints set "Geo"
fitConstraints set "Momentum"
postFitSelectors set "Mes 5.1:5.3" 
postFitSelectors set "DeltaE -0.3:0.3"
exit

mod clone SmpMakerDefiner myKPiList
sequence append B0ToJpsiHHSequence myKPiList
mod talk myKPiList
decayMode set "K*0 -> K+ pi-"
daughterListNames set "GoodTracksVeryLoose"
daughterListNames set "GoodTracksVeryLoose"
fittingAlgorithm set "Cascade"
fitConstraints set "Geo"
fitConstraints set "Momentum"
postFitSelectors set "Mass 0.2:3.0"
exit

mod clone SmpMakerDefiner B0ToJpsiKPi
sequence append B0ToJpsiHHSequence B0ToJpsiKPi
mod talk B0ToJpsiKPi
decayMode set "B0 -> J/psi K*0"
daughterListNames set "JpsiFromBToJpsiHHMass"
daughterListNames set "myKPiList"
preFitSelectors set "Mass 3.0:7.0"
fittingAlgorithm set "Cascade"
fitConstraints set "Geo"
fitConstraints set "Momentum"
postFitSelectors set "Mes 5.1:5.3" 
postFitSelectors set "DeltaE -0.3:0.3"
exit

mod clone SmpMakerDefiner myKKList
sequence append B0ToJpsiHHSequence myKKList
mod talk myKKList
decayMode set "phi -> K+ K-"
daughterListNames set "GoodTracksVeryLoose"
daughterListNames set "GoodTracksVeryLoose"
fittingAlgorithm set "Cascade"
fitConstraints set "Geo"
fitConstraints set "Momentum"
postFitSelectors set "Mass 0.2:3.0"
exit


mod clone SmpMakerDefiner B0ToJpsiKK
sequence append B0ToJpsiHHSequence B0ToJpsiKK
mod talk B0ToJpsiKK
decayMode set "B0 -> J/psi phi"
daughterListNames set "JpsiFromBToJpsiHHMass"
daughterListNames set "myKKList"
preFitSelectors set "Mass 3.0:7.0"
fittingAlgorithm set "Cascade"
fitConstraints set "Geo"
fitConstraints set "Momentum"
postFitSelectors set "Mes 5.1:5.3" 
postFitSelectors set "DeltaE -0.3:0.3"
exit

mod clone SmpMergerDefiner B0ToJpsiHH
sequence append B0ToJpsiHHSequence B0ToJpsiHH
mod talk B0ToJpsiHH
inputListNames set B0ToJpsiPiPi
inputListNames set B0ToJpsiKPi
inputListNames set B0ToJpsiKK
disableCloneCheck set t
exit

sequence create BchToJpsiHPi0Sequence
path append BToJpsiHHPath BchToJpsiHPi0Sequence

mod clone SmpRefitterDefiner Pi0FromBToJpsiHPi0
sequence append BchToJpsiHPi0Sequence Pi0FromBToJpsiHPi0
mod talk Pi0FromBToJpsiHPi0
 unrefinedListName set "pi0Loose"
 fittingAlgorithm   set "Add4"
 fitConstraints     set "Momentum"
 fitConstraints     set "Mass"
 fitConstraints     set "PrimaryVertex"
 preFitSelectors set "Mass 0.11:0.15"
 preFitSelectors set "LabE 0.3:"
exit

mod clone SmpMakerDefiner BchToJpsiPiPi0
sequence append BchToJpsiHPi0Sequence BchToJpsiPiPi0
mod talk BchToJpsiPiPi0
decayMode set "B+ -> J/psi pi+ pi0"
daughterListNames set "JpsiFromBToJpsiHHMass"
daughterListNames set "GoodTracksVeryLoose"
daughterListNames set "Pi0FromBToJpsiHPi0"
#daughterListNames set "pi0LooseMass"
preFitSelectors set "Mass 3.0:7.0"
fittingAlgorithm set "Cascade"
fitConstraints set "Geo"
fitConstraints set "Momentum"
postFitSelectors set "Mes 5.1:5.3" 
postFitSelectors set "DeltaE -0.3:0.3"
exit

mod clone SmpMakerDefiner BchToJpsiKPi0
sequence append BchToJpsiHPi0Sequence BchToJpsiKPi0
mod talk BchToJpsiKPi0
decayMode set "B+ -> J/psi K+ pi0"
daughterListNames set "JpsiFromBToJpsiHHMass"
daughterListNames set "GoodTracksVeryLoose"
daughterListNames set "Pi0FromBToJpsiHPi0"
#daughterListNames set "pi0LooseMass"
preFitSelectors set "Mass 3.0:7.0"
fittingAlgorithm set "Cascade"
fitConstraints set "Geo"
fitConstraints set "Momentum"
postFitSelectors set "Mes 5.15:5.3" 
postFitSelectors set "DeltaE -0.3:0.3"
exit

mod clone SmpMergerDefiner BchToJpsiHPi0
sequence append BchToJpsiHPi0Sequence BchToJpsiHPi0
mod talk BchToJpsiHPi0
inputListNames set BchToJpsiPiPi0
inputListNames set BchToJpsiKPi0
disableCloneCheck set t
exit

mod clone SmpMergerDefiner BToJpsiHH
path append BToJpsiHHPath BToJpsiHH
mod talk BToJpsiHH
inputListNames set B0ToJpsiHH
inputListNames set BchToJpsiHPi0
disableCloneCheck set t
exit

mod clone TagFromList TagBToJpsiHH
mod talk TagBToJpsiHH
  listsToTag set BToJpsiHH 
  tagName set BToJpsiHH
  production set true
  verbose set false
exit

 path append BToJpsiHHPath TagBToJpsiHH

   mod clone TagFilterByName BToJpsiHHFilter
   path append BToJpsiHHPath BToJpsiHHFilter
   talkto BToJpsiHHFilter {
        orList set BToJpsiHH
        production set true
        verbose set false
   }

# Add lists of composites to write
lappend BtaCandLists BToJpsiHH JpsiFromBToJpsiHH

lappend CandBranches "B0 B0 anti-B0 vtx p4"
lappend CandBranches "Bch B+ B- vtx p4"
lappend CandBranches "Jpsi J/psi vtx p4"
lappend CandBranches "PiPiComb rho0 vtx p4"
lappend CandBranches "KPiComb K*0 anti-K*0 vtx p4"
lappend CandBranches "KKComb phi vtx p4"
lappend CandBranches "Electron e+ e- p4"
lappend CandBranches "Muon mu+ mu- p4"
lappend CandBranches "Pion pi+ pi- p4"
lappend CandBranches "Kaon K+ K- p4"
lappend CandBranches "Pi0 pi0 p4"
lappend CandBranches "Gamma gamma p4"

# Write UsrData
set writeUsrDataForSkim 1
