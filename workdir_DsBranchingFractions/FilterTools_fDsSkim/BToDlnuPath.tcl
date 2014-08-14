#
# $Id: BToDlnuPath.tcl,v 1.11 2005/11/17 07:04:35 fwinkl Exp $
#
# defines the BToDlnu tag filter path
#

global BtaCandLists
global writeUsrDataForSkim

path create BToDlnuPath

# append and enable the tag modules
sourceFoundFile EventTagTools/TagProcs.tcl
addTagModule TagBToDstarlnu BToDlnuPath {
    dslLooseList0            set B0ToDstarlnuLoose
    dslTightList0            set B0ToDstarlnuTight
    dslVTightElecList0       set B0ToDstarlnuVTightElec
    dslVTightMuonList0       set B0ToDstarlnuVTightMuon
    dslVTightFakeList0       set B0ToDstarlnuVTightFake
    dslLooseListCh           set BchToDstarlnuLoose
    dslTightListCh           set BchToDstarlnuTight
    dslVTightElecListCh      set BchToDstarlnuVTightElec
    dslVTightMuonListCh      set BchToDstarlnuVTightMuon
    dslVTightFakeListCh      set BchToDstarlnuVTightFake
    cutR2                    set 2.0
    requireBGFMultiHadron    set false
    cutCosThDsl              set 0.0
    doHist                   set false
}

# Include the BToDlnu tagbit (jack)
addTagModule TagBToDlnu BToDlnuPath {
    dlnuLooseListCh           set BchToDlnuLoose
    dlnuTightListCh           set BchToDlnuTight
    dlnuVTightElecListCh      set BchToDlnuVTightElec
    dlnuVTightMuonListCh      set BchToDlnuVTightMuon
    dlnuVTightFakeListCh      set BchToDlnuVTightFake
    cutR2                    set 2.0
    requireBGFMultiHadron    set false
    cutCosThDsl              set 0.0
    doHist                   set true
}
mod disable TagBToDlnu

# filter on tags
path append BToDlnuPath BToDlnuXFilter
path append BToDlnuPath BToDstarlnuXFilter

talkto BToDlnuXFilter {
    verbose set false
    inputBtaCandidateList set BToDlnuAll
    minCandidates set 1
    maxCandidates set 100000
}


talkto BToDstarlnuXFilter {
    eventBlock set BToDstarlnuEventData
    candBlock  set BToDstarlnuCandData
    BinputList set BToDstarlnuAll 
    BoutputList set BToDstarlnuSkimmedList
}

# Add lists of composites to write

lappend BtaCandLists BToDlnuSkimmedList
lappend BtaCandLists BToDstarlnuSkimmedList 

# Write UsrData
set writeUsrDataForSkim 1
