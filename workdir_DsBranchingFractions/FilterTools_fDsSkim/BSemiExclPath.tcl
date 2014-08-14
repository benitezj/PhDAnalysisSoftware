#
# $Id: BSemiExclPath.tcl,v 1.22 2007/05/03 16:46:11 snelson Exp $
#
# defines B->D*X - semi excl filter path
#
global BtaCandLists
global writeUsrDataForSkim
path create BSemiExclPath


#
# SemiExcl
#
proc makeSemiExclTag {dmode level} {
  module clone TagFromList TagSemiExcl${dmode}${level}
  addTagModule TagSemiExcl${dmode}${level} TagBSemiExclSequence
  talkto TagSemiExcl${dmode}${level} {
    listsToTag set ${level}BTo${dmode}
    tagName    set BTo${dmode}SemiExcl${level}
  }
}

sequence create TagBSemiExclSequence
path append BSemiExclPath TagBSemiExclSequence

# ugly: need to trigger the creation of these lists because 
# the 'level' lists are created as a byproduct. (WDH)
createsmplisttrigger TagBSemiExclSequence BToDstar
createsmplisttrigger TagBSemiExclSequence BToDc
createsmplisttrigger TagBSemiExclSequence BToDstar0
createsmplisttrigger TagBSemiExclSequence BToD0

foreach level {Neat Clean Dirty Other} {
  makeSemiExclTag Dstar  $level
  makeSemiExclTag Dc     $level
  makeSemiExclTag Dstar0 $level
  makeSemiExclTag D0     $level
}


mod clone TagFilterByName BSemiExclFilter
sequence append TagBSemiExclSequence BSemiExclFilter

talkto BSemiExclFilter {
#common tag
    andList set BGFMultiHadron


# D0
    orList set BToD0SemiExclNeat
    orList set BToD0SemiExclClean
    orList set BToD0SemiExclDirty
# do not save the dirtiest modes
#    orList set BToD0SemiExclOther

# Dc
    orList set BToDcSemiExclNeat
    orList set BToDcSemiExclClean
    orList set BToDcSemiExclDirty
# do not save the dirtiest modes
#    orList set BToDcSemiExclOther

# Dstar
    orList set BToDstarSemiExclNeat
    orList set BToDstarSemiExclClean
    orList set BToDstarSemiExclDirty
# do not save the dirtiest modes
#    orList set BToDstarSemiExclOther

# Dstar0
    orList set BToDstar0SemiExclNeat
    orList set BToDstar0SemiExclClean
    orList set BToDstar0SemiExclDirty
# do not save the dirtiest modes
#    orList set BToDstar0SemiExclOther
}

# Add lists of composites to write
lappend BtaCandLists NeatBToD0  CleanBToD0  DirtyBToD0
lappend BtaCandLists NeatBToDc CleanBToDc DirtyBToDc 
lappend BtaCandLists NeatBToDstar0 CleanBToDstar0 DirtyBToDstar0
lappend BtaCandLists NeatBToDstar CleanBToDstar DirtyBToDstar
 
set writeUsrDataForSkim 1
