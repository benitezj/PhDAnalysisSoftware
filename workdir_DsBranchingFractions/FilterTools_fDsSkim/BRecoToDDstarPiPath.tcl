#
# $Id: BRecoToDDstarPiPath.tcl,v 1.12 2006/01/13 19:23:18 fwinkl Exp $
#
# defines BRecoToDDstarPiPath tag filter path
#

global BtaCandLists
global writeUsrDataForSkim
global UsrBCandLists

path create BRecoToDDstarPiPath

# only run on MultiHadrons
path append BRecoToDDstarPiPath FilterBGFMultiHadron

# include those tag bits in this path
useTagFromList BchToD0Pi      BRecoToDDstarPiPath
useTagFromList BchToDstar0Pi  BRecoToDDstarPiPath
useTagFromList B0ToDstarPi    BRecoToDDstarPiPath
useTagFromList B0ToDchPi      BRecoToDDstarPiPath

# clone and customize filter module
mod clone TagFilterByName BRecoToDDstarPiFilter
path append BRecoToDDstarPiPath BRecoToDDstarPiFilter
talkto BRecoToDDstarPiFilter {
# new tagbits
  orList set BchToD0Pi
  orList set BchToDstar0Pi
  orList set B0ToDstarPi
  orList set B0ToDchPi

  assertIfMissing set true
}

# Add lists of composites to write
lappend BtaCandLists  BchToD0PiDefault BchToDstar0PiDefault B0ToDstarPiDefault B0ToDPiDefault

# Add lists of composites with UsrData
set UsrBCandLists $BtaCandLists 
# Write UsrData
set writeUsrDataForSkim 1
