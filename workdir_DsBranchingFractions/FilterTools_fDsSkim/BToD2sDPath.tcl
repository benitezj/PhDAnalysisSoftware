#
# $Id: BToD2sDPath.tcl,v 1.12 2005/11/16 23:01:56 fwinkl Exp $
#
# 
# defines B0 and B+ to D2sD filter path
#
#

global BtaCandLists
global writeUsrDataForSkim
global UsrBCandLists

path create BToD2sDPath

path append BToD2sDPath FilterBGFMultiHadron

addTagModule TagMakeBRecoToD2sD BToD2sDPath

mod clone TagFilterByName BToD2sDFilter
path append BToD2sDPath BToD2sDFilter
talkto BToD2sDFilter {
  orList set BchToD0Dspi0
  orList set BchToDstar0Dspi0
  orList set BchToD0Dsstarpi0
  orList set BchToDstar0Dsstarpi0
  orList set BchToD0Dsgamma
  orList set BchToDstar0Dsgamma
  orList set BchToD0Dsstargamma
  orList set BchToDstar0Dsstargamma
  orList set B0ToDchDspi0
  orList set B0ToDstarDspi0
  orList set B0ToDchDsstarpi0
  orList set B0ToDstarDsstarpi0
  orList set B0ToDchDsgamma
  orList set B0ToDstarDsgamma
  orList set B0ToDchDsstargamma
  orList set B0ToDstarDsstargamma
	assertIfMissing set true
}

# Add lists of composites to write
lappend BtaCandLists B0ToD2sDDefault BchToD2sDDefault

# Add lists of composites with UsrData
set UsrBCandLists $BtaCandLists
# Write UsrData
set writeUsrDataForSkim 1
