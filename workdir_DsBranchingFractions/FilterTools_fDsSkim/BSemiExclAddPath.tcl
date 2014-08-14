#
# $Id: BSemiExclAddPath.tcl,v 1.1 2008/01/07 23:03:40 manuelf Exp $
#
# defines B->D*X - semi excl filter path
#
global BtaCandLists
global writeUsrDataForSkim
path create BSemiExclAddPath

# Filter on MC decay modes
#sourceFoundFile SkimMini/SelectMCDecayModes.tcl


#
# SemiExcl
#
proc makeSemiExclAddTag {dmode level} {
  module clone TagFromList TagSemiExcl${dmode}${level}
  addTagModule TagSemiExcl${dmode}${level} TagBSemiExclAddSequence
  talkto TagSemiExcl${dmode}${level} {
    listsToTag set ${level}BTo${dmode}
    tagName    set BTo${dmode}SemiExcl${level}
  }
}

sequence create TagBSemiExclAddSequence
path append BSemiExclAddPath TagBSemiExclAddSequence

# ugly: need to trigger the creation of these lists because 
# the 'level' lists are created as a byproduct. (WDH)
createsmplisttrigger TagBSemiExclAddSequence BToDstarAdd
createsmplisttrigger TagBSemiExclAddSequence BToDcAdd
createsmplisttrigger TagBSemiExclAddSequence BToDstar0Add
createsmplisttrigger TagBSemiExclAddSequence BToD0Add
createsmplisttrigger TagBSemiExclAddSequence BToJPsi
createsmplisttrigger TagBSemiExclAddSequence BToDs
createsmplisttrigger TagBSemiExclAddSequence BToDsStar

foreach level {Neat Clean Dirty Other} {
  makeSemiExclAddTag DstarAdd  $level
  makeSemiExclAddTag DcAdd     $level
  makeSemiExclAddTag Dstar0Add $level
  makeSemiExclAddTag D0Add     $level
  makeSemiExclAddTag JPsi      $level
  makeSemiExclAddTag Ds        $level
  makeSemiExclAddTag DsStar    $level
}


mod clone TagFilterByName BSemiExclAddFilter
sequence append TagBSemiExclAddSequence BSemiExclAddFilter

talkto BSemiExclAddFilter {
#common tag
    andList set BGFMultiHadron 
    # Originally there was isBCMultiHadron

# JPsi
    orList set BToJPsiSemiExclNeat
    orList set BToJPsiSemiExclClean
    orList set BToJPsiSemiExclDirty
# do not save the dirtiest modes
    orList set BToJPsiSemiExclOther
# Ds
    orList set BToDsSemiExclNeat
    orList set BToDsSemiExclClean
    orList set BToDsSemiExclDirty
# do not save the dirtiest modes
    orList set BToDsSemiExclOther

# DsStar
    orList set BToDsStarSemiExclNeat
    orList set BToDsStarSemiExclClean
    orList set BToDsStarSemiExclDirty
# do not save the dirtiest modes
    orList set BToDsStarSemiExclOther
# D0
    orList set BToD0AddSemiExclNeat
    orList set BToD0AddSemiExclClean
    orList set BToD0AddSemiExclDirty
# do not save the dirtiest modes
    orList set BToD0AddSemiExclOther

# Dc
    orList set BToDcAddSemiExclNeat
    orList set BToDcAddSemiExclClean
    orList set BToDcAddSemiExclDirty
# do not save the dirtiest modes
    orList set BToDcAddSemiExclOther

# Dstar
    orList set BToDstarAddSemiExclNeat
    orList set BToDstarAddSemiExclClean
    orList set BToDstarAddSemiExclDirty
# do not save the dirtiest modes
    orList set BToDstarAddSemiExclOther

# Dstar0
    orList set BToDstar0AddSemiExclNeat
    orList set BToDstar0AddSemiExclClean
    orList set BToDstar0AddSemiExclDirty
# do not save the dirtiest modes
    orList set BToDstar0AddSemiExclOther
}

# Add lists of composites to write
lappend BtaCandLists NeatBToJPsi  CleanBToJPsi  DirtyBToJPsi OtherBToJPsi
lappend BtaCandLists NeatBToDs  CleanBToDs  DirtyBToDs OtherBToDs
lappend BtaCandLists NeatBToDsStar  CleanBToDsStar  DirtyBToDsStar OtherBToDsStar
lappend BtaCandLists NeatBToD0Add  CleanBToD0Add  DirtyBToD0Add OtherBToD0Add
lappend BtaCandLists NeatBToDcAdd CleanBToDcAdd DirtyBToDcAdd  OtherBToDcAdd
lappend BtaCandLists NeatBToDstar0Add CleanBToDstar0Add DirtyBToDstar0Add OtherBToDstar0Add
lappend BtaCandLists NeatBToDstarAdd CleanBToDstarAdd DirtyBToDstarAdd OtherBToDstarAdd
 
set writeUsrDataForSkim 1
