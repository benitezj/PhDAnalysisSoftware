path create BToXsllPath
sequence create SllSeq

mod clone KllTagFilter Sll_KllTagFilter
mod talk Sll_KllTagFilter
  R2AllCut set 0.7
  NLepCut set 2
  muonPCut set 0.5
exit

path append BToXsllPath Sll_KllTagFilter
path append BToXsllPath SllSeq

global writeUsrDataForSkim 

set writeUsrDataForSkim 1

global XsllBtaCandLists
global XsllBtaCandListsKs

set XsllBtaCandLists ""
set XsllBtaCandListsKs ""

sourceFoundFile FilterTools/BToXsllProcs.tcl
sourceFoundFile FilterTools/BToXsllBLists.tcl

global BtaCandLists
set BtaCandLists ""

foreach list $XsllBtaCandLists { lappend BtaCandLists $list }
foreach list $XsllBtaCandListsKs { lappend BtaCandLists $list }

##############FOR TAGGING#############
foreach list $BtaCandLists { 
    clone TagFromList     Xsll${list}Tag
    talkto Xsll${list}Tag {
	listsToTag set $list 
	tagName    set ${list}Tag
    }
    seq append SllSeq Xsll${list}Tag
}

clone TagFilterByName XsllFilter
talkto XsllFilter {
    foreach list $BtaCandLists { orList set ${list}Tag }
    verbose set false
}

seq append SllSeq XsllFilter

##############USR DATA##################
# Store candidates by particle types
global CandBranches
global CandBlocks
set CandBranches ""
set CandBlocks ""

lappend CandBranches "Pion     pi+   pi-      p4"
lappend CandBranches "Kaon     K+    K-       p4"
lappend CandBranches "Pi0      pi0            p4 vtx"
lappend CandBranches "Kshort   K_S0           p4 vtx"
lappend CandBranches "B0       B0    anti-B0  p4 vtx"
lappend CandBranches "Bch      B+    B-       p4 vtx"
lappend CandBranches "Gam      gamma          p4"
lappend CandBranches "Elec     e+    e-       p4"
lappend CandBranches "Muon     mu+   mu-      p4"
