#
# $Id: BtoDPiPiPath.tcl,v 1.2 2005/03/14 17:28:06 fnc Exp $
#

#global BtaCandLists
#global writeUsrDataForSkim
#global UsrBCandLists

path create BtoDPiPiPath

path append BtoDPiPiPath FilterBGFMultiHadron

sourceFoundFile FilterTools/BtoDPiPiCompSequence.tcl
path append BtoDPiPiPath BtoDPiPiCompSequence

path append BtoDPiPiPath BtoDPiPiTagFilter

#mod clone TagFilterByName BtoDPiPiFilter
#path append BtoDPiPiPath BtoDPiPiFilter

#talkto BtoDPiPiFilter {
#    orList set BchToDstarPiPiDefault
#    orList set BchToDchPiPiDefault
#}

#lappend BtaCandLists BchToDstarPiPiDefault BchToDchPiPiDefault

#set UsrBCandLists $BtaCandLists

#set writeUsrDataForSkim 1
