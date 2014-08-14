path create BToXsll_peakingPath
seq create Sll_peakingSeq

mod clone KllTagFilter Sll_peaking_KllTagFilter
mod talk Sll_peaking_KllTagFilter
  R2AllCut set 0.7
  NLepCut set 1
  muonPCut set 0.5
exit

path append BToXsll_peakingPath Sll_peaking_KllTagFilter
path append BToXsll_peakingPath Sll_peakingSeq

global writeUsrDataForSkim

set writeUsrDataForSkim 1

global XsllBtaCandLists_peaking
global XsllBtaCandListsKs_peaking

set XsllBtaCandLists_peaking ""
set XsllBtaCandListsKs_peaking ""

sourceFoundFile FilterTools/BToXsllProcs_peaking.tcl
sourceFoundFile FilterTools/BToXsllBLists_peaking.tcl

global BtaCandLists
set BtaCandLists ""

foreach list $XsllBtaCandLists_peaking { lappend BtaCandLists $list }
foreach list $XsllBtaCandListsKs_peaking { lappend BtaCandLists $list }

##############FOR TAGGING#############
foreach list $BtaCandLists {
    clone TagFromList     Xsll${list}Tag
    talkto Xsll${list}Tag {
	listsToTag set $list
	tagName    set ${list}Tag
    }
    seq append Sll_peakingSeq Xsll${list}Tag
}

clone TagFilterByName Xsll_peakingFilter
talkto Xsll_peakingFilter {
    foreach list $BtaCandLists { orList set ${list}Tag }
    verbose set false
}

seq append Sll_peakingSeq Xsll_peakingFilter

##############USR DATA##################
# Store candidates by particle types
global CandBranches ""
global CandBlocks ""
set CandBranches ""
set CandBlocks ""

lappend CandBranches "Pion     pi+   pi-      p4"
lappend CandBranches "Kaon     K+    K-       p4"
lappend CandBranches "Pi0      pi0            p4 vtx"
lappend CandBranches "Kshort   K_S0           p4 vtx"
lappend CandBranches "Kstar    K*0   anti-K*0 p4 vtx"
lappend CandBranches "B0       B0    anti-B0  p4 vtx"
lappend CandBranches "Bch      B+    B-       p4 vtx"
lappend CandBranches "Gam      gamma          p4"
lappend CandBranches "Elec     e+    e-       p4"
lappend CandBranches "Muon     mu+   mu-      p4"
