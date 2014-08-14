path create BToXsllBGPath
sequence create SllBGSeq

mod clone KllTagFilter Sll_BGTagFilter
mod talk Sll_BGTagFilter
  R2AllCut set 0.7
  NLepCut set 2
  muonPCut set 0.5
exit

path append BToXsllBGPath Sll_BGTagFilter
path append BToXsllBGPath SllBGSeq

global writeUsrDataForSkim 

set writeUsrDataForSkim 0

global XsllBtaBGLists

set XsllBtaBGLists ""

sourceFoundFile FilterTools/BToXsllProcs.tcl
sourceFoundFile FilterTools/BToXsllBGLists.tcl

global BtaCandLists
set BtaCandLists ""

foreach list $XsllBtaBGLists { lappend BtaCandLists $list }

##############FOR TAGGING#############
foreach list $XsllBtaBGLists { 
    clone TagFromList     Xsll${list}Tag
    talkto Xsll${list}Tag {
	listsToTag set $list 
	tagName    set ${list}Tag
    }
    seq append SllBGSeq Xsll${list}Tag
}


clone TagFilterByName XsllBGFilter
talkto XsllBGFilter {
    foreach list $BtaCandLists { orList set ${list}Tag }
    verbose set false
}

seq append SllBGSeq XsllBGFilter

