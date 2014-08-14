#
# $Id: BToDDPath.tcl,v 1.12 2007/10/22 11:19:04 marchior Exp $
# 
# Defines B(0) to D(s)(*)(0) D(*)(0) path
#
# For more info, refer to
# http://www.slac.stanford.edu/BFROOT/www/Physics/Analysis/AWG/EHBDOC/skims/BToDD.htm

global BtaCandLists
global CandBranches
global writeUsrDataForSkim

set writeUsrDataForSkim 1

sourceFoundFile CompositionSequences/CompBDDTrackSelection.tcl
sourceFoundFile CompositionSequences/CompBDDD0Selection.tcl
sourceFoundFile CompositionSequences/CompBDDDcSelection.tcl
sourceFoundFile CompositionSequences/CompBDDDsSelection.tcl
sourceFoundFile CompositionSequences/CompBDDDstarSelection.tcl
sourceFoundFile CompositionSequences/CompBDDDstar0Selection.tcl
sourceFoundFile CompositionSequences/CompBDDDsstarSelection.tcl
## This is sourced in CompProdCreateSequence.tcl
# sourceFoundFile CompositionSequences/CompBDDSelection.tcl


proc BDDFilter { {suffix ""} } {

  mod clone TagFromList TagBToDD${suffix}

  mod talk TagBToDD${suffix}
    listsToTag set BToDD${suffix}
    tagName    set tagBToDD${suffix}
    production set true
    verbose set false
  exit

  mod clone TagFilterByName BToDD${suffix}_Filter

  mod talk BToDD${suffix}_Filter
    orList set tagBToDD${suffix}
    production set true
    verbose set false
  exit
}

BDDFilter Loose ;# Filter on loose selection in Prod Seq
BDDFilter       ;# Filter on fast  selection of B to DD
BDDFilter Final ;# Filter on final selection of B to DD

## Create path and add fast selections to it

path create BToDDPath
path append BToDDPath FilterBGFMultiHadron
path append BToDDPath TagBToDDLoose
path append BToDDPath BToDDLoose_Filter
path append BToDDPath PionSelection
path append BToDDPath KaonSelection
path append BToDDPath KsSelection
path append BToDDPath PhiSelection
path append BToDDPath Kstar0Selection
path append BToDDPath D0Selection
path append BToDDPath DcSelection
path append BToDDPath DsSelection
path append BToDDPath DstarSelection
path append BToDDPath Dstar0Selection
path append BToDDPath DsstarSelection
path append BToDDPath BToDDSelection
path append BToDDPath TagBToDD
path append BToDDPath BToDD_Filter

## Add final selections to path

path append BToDDPath D0FinalSelection
path append BToDDPath DcFinalSelection
path append BToDDPath DsFinalSelection
path append BToDDPath DstarFinalSelection
path append BToDDPath Dstar0FinalSelection
path append BToDDPath DsstarFinalSelection
path append BToDDPath BToDDFinalSelection
path append BToDDPath TagBToDDFinal
path append BToDDPath BToDDFinal_Filter
addTagModule TagMakeBRecoToDD BToDDPath


## Write out list of B candidates and store them by particle types

lappend BtaCandLists BToDDFinal

lappend CandBranches "Photon gamma          p4"
lappend CandBranches "Pion   pi+   pi-      p4"
lappend CandBranches "Kaon   K+    K-       p4"
lappend CandBranches "Pi0    pi0            p4 vtx"
lappend CandBranches "Kshort K_S0           p4 vtx"
lappend CandBranches "Kstar0 K*0   anti-K*0 p4 vtx"
lappend CandBranches "Phi    phi            p4 vtx"
lappend CandBranches "Dc     D+    D-       p4 vtx"
lappend CandBranches "D0     D0    anti-D0  p4 vtx"
lappend CandBranches "Ds     D_s+  D_s-     p4 vtx"
lappend CandBranches "Dstarc D*+   D*-      p4 vtx"
lappend CandBranches "Dstar0 D*0   anti-D*0 p4 vtx"
lappend CandBranches "Dsstar D_s*+ D_s*-    p4 vtx"
lappend CandBranches "B0     B0    anti-B0  p4 vtx"
lappend CandBranches "Bch    B+    B-       p4 vtx"

## Write user data

if { $writeUsrDataForSkim == 1 } {

## B candidates stored have mass-constrained D daughters
## Merge vertexed D0, D+, Ds to get the unconstrained mass from this list

  mod clone CompMergeLists D_Vtx_BDD

  mod talk D_Vtx_BDD
    outputList    set DVtxBDD
    nOfInputLists set 3
    inputList0    set D0BDD
    inputList1    set DcBDD
    inputList2    set DsBDD
  exit

  path append BToDDPath D_Vtx_BDD
}
