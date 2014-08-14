#
# $Id: BRecoToDDstarPath.tcl,v 1.15 2005/11/16 23:01:56 fwinkl Exp $
#
# defines BRecoToDDstarPath tag filter path
#

global BtaCandLists
global writeUsrDataForSkim
global UsrBCandLists

path create BRecoToDDstarPath

# only run on MultiHadrons
path append BRecoToDDstarPath FilterBGFMultiHadron

# Create tag bits
sequence create TagBRecoToDDstarSequence

useTagFromList BchToD0Pi        TagBRecoToDDstarSequence
useTagFromList BchToDstar0Pi    TagBRecoToDDstarSequence
useTagFromList BchToD0K         TagBRecoToDDstarSequence
useTagFromList BchToD0Kstar     TagBRecoToDDstarSequence
useTagFromList BchToDstar0Kstar TagBRecoToDDstarSequence 
useTagFromList B0ToDstarPi      TagBRecoToDDstarSequence 
useTagFromList B0ToDstarRhoC    TagBRecoToDDstarSequence
useTagFromList B0ToDstarA1C     TagBRecoToDDstarSequence
useTagFromList B0ToDchPi        TagBRecoToDDstarSequence
useTagFromList B0ToDchRhoC      TagBRecoToDDstarSequence
useTagFromList B0ToDchA1C       TagBRecoToDDstarSequence

makeTagFromList BchToDstar0RhoCDefault   BchToDstar0RhoC TagBRecoToDDstarSequence ;# B->D*0rho
makeTagFromList BchToD0RhoCDefault       BchToD0RhoC     TagBRecoToDDstarSequence  ;# B->D0rho
makeTagFromList BchToD0A1CDefault        BchToD0A1C      TagBRecoToDDstarSequence  ;# B->D0a1
makeTagFromList BchToDstar0A1CDefault    BchToDstar0A1C  TagBRecoToDDstarSequence  ;# B->D*0a1
makeTagFromList B0ToDstarPiPi0Default    B0ToDstarPiPi0  TagBRecoToDDstarSequence  ;# B0->D*pipi0 (non-resonant)
makeTagFromList B0ToDcKstarDefault       B0ToDchKstar    TagBRecoToDDstarSequence  ;# B0->DK*
makeTagFromList B0ToDstarKstarDefault    B0ToDstarKstar  TagBRecoToDDstarSequence  ;# B0->D*K*

path append BRecoToDDstarPath TagBRecoToDDstarSequence

# clone and customize filter module
mod clone TagFilterByName BRecoToDDstarFilter
path append BRecoToDDstarPath BRecoToDDstarFilter
talkto BRecoToDDstarFilter {
# new tagbits
  orList set BchToD0Pi
  orList set BchToD0RhoC
  orList set BchToD0A1C
  orList set BchToDstar0Pi
  orList set BchToDstar0RhoC
  orList set BchToDstar0A1C
  orList set BchToD0K
  orList set BchToD0Kstar
  orList set BchToDstar0Kstar
  orList set B0ToDstarPi
  orList set B0ToDstarRhoC
  orList set B0ToDstarA1C
  orList set B0ToDchPi
  orList set B0ToDchRhoC
  orList set B0ToDchA1C
  orList set B0ToDstarPiPi0
  orList set B0ToDchKstar
  orList set B0ToDstarKstar
}

# Add lists of composites to write
lappend BtaCandLists BchToD0PiDefault BchToD0RhoCDefault BchToD0A1CDefault BchToDstar0PiDefault BchToDstar0RhoCDefault BchToDstar0A1CDefault BchToD0KDefault BchToD0KstarDefault BchToDstar0KstarDefault
lappend BtaCandLists B0ToDstarPiDefault B0ToDstarRhoCDefault B0ToDstarA1CDefault B0ToDPiDefault B0ToDRhoCDefault B0ToDA1CDefault B0ToDstarPiPi0Default B0ToDcKstarDefault B0ToDstarKstarDefault

# Add lists of composites with UsrData
set UsrBCandLists $BtaCandLists 
# Write UsrData
set writeUsrDataForSkim 1
