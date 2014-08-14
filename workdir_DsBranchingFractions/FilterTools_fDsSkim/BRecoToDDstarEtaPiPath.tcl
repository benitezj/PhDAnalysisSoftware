#
# $Id: BRecoToDDstarEtaPiPath.tcl,v 1.3 2005/11/16 23:01:56 fwinkl Exp $
#
#
# defines BRecoToDDstarEtaPiPath tag filter path
#

global BtaCandLists
global writeUsrDataForSkim
global UsrBCandLists

path create BRecoToDDstarEtaPiPath

# only run on MultiHadrons
path append BRecoToDDstarEtaPiPath FilterBGFMultiHadron

# create tag bits
sourceFoundFile EventTagTools/TagProcs.tcl
makeTagFromList B0ToDstarEtaPiDefault     B0ToDstarEtaPi    BRecoToDDstarEtaPiPath  ;# B0 -> D*- eta pi+
makeTagFromList B0ToDcEtaPiDefault        B0ToDcEtaPi       BRecoToDDstarEtaPiPath  ;# B0 -> D- eta pi+
makeTagFromList BchToDstar0EtaPiDefault   BchToDstar0EtaPi  BRecoToDDstarEtaPiPath  ;# B- -> D*0 eta pi-
makeTagFromList BchToD0EtaPiDefault       BchToD0EtaPi      BRecoToDDstarEtaPiPath  ;# B- -> D0 eta pi-

# clone and customize filter module
mod clone TagFilterByName BRecoToDDstarEtaPiFilter
path append BRecoToDDstarEtaPiPath BRecoToDDstarEtaPiFilter
talkto BRecoToDDstarEtaPiFilter {
# new tagbits
  orList set B0ToDstarEtaPi
  orList set B0ToDcEtaPi
  orList set BchToDstar0EtaPi
  orList set BchToD0EtaPi
}

# Add lists of composites to write
lappend BtaCandLists  B0ToDstarEtaPiDefault B0ToDcEtaPiDefault BchToDstar0EtaPiDefault BchToD0EtaPiDefault 

# Add lists of composites with UsrData
set UsrBCandLists $BtaCandLists 
# Write UsrData
set writeUsrDataForSkim 1
