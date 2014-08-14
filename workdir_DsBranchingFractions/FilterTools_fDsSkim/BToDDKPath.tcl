#
# $Id: BToDDKPath.tcl,v 1.13 2005/11/16 23:01:56 fwinkl Exp $
#
#
# defines B to DDK filter path
#

global BtaCandLists
global writeUsrDataForSkim
global UsrBCandLists

path create BToDDKPath

path append BToDDKPath FilterBGFMultiHadron

addTagModule TagMakeDDK BToDDKPath
addTagModule TagMakeDDK_DeltaC2 BToDDKPath

mod clone TagFilterByName BToDDKFilter
path append BToDDKPath BToDDKFilter
talkto BToDDKFilter {
  orList set BchToD0D0Kch
  orList set BchToD0DchK0
  orList set BchToDchDchKch
  orList set BchToDstar0D0Kch
  orList set BchToDstar0DchK0
  orList set BchToDstarDchKch
  orList set BchToD0Dstar0Kch
  orList set BchToD0DstarK0
  orList set BchToDchDstarKch
  orList set BchToDstar0Dstar0Kch
  orList set BchToDstar0DstarK0
  orList set BchToDstarDstarKch
  orList set B0ToDchD0Kch
  orList set B0ToD0D0K0
  orList set B0ToDchDchK0
  orList set B0ToDchDstar0Kch
  orList set B0ToDstarD0Kch
  orList set B0ToDstar0D0K0
  orList set B0ToDstarDchK0
  orList set B0ToDstarDstar0Kch
  orList set B0ToDstar0Dstar0K0
  orList set B0ToDstarDstarK0
  orList set B0ToD0D0K0_DeltaC2
  orList set B0ToDchD0Kch_DeltaC2
  orList set B0ToDstar0D0K0_DeltaC2
  orList set B0ToDstarD0Kch_DeltaC2
  orList set B0ToDchDstar0Kch_DeltaC2
  orList set B0ToDstar0Dstar0K0_DeltaC2
  orList set B0ToDstarDstar0Kch_DeltaC2
  orList set BchToD0D0Kch_DeltaC2
  orList set BchToD0barD0barKch_DeltaC2
  orList set BchToD0DchK0_DeltaC2
  orList set BchToDchDchKch_DeltaC2
  orList set BchToDstar0D0Kch_DeltaC2
  orList set BchToDstar0barD0barKch_DeltaC2
  orList set BchToDstar0D0Kch_DeltaC2
  orList set BchToDstar0DchK0_DeltaC2
  orList set BchToDstarDchKch_DeltaC2
  orList set BchToD0DstarK0_DeltaC2
  orList set BchToDstar0Dstar0Kch_DeltaC2
  orList set BchToDstar0barDstar0barKch_DeltaC2
  orList set BchToDstar0DstarK0_DeltaC2
  orList set BchToDstarDstarKch_DeltaC2
	assertIfMissing set true
}

# Add lists of composites to write
lappend BtaCandLists BchToDDKDefault B0ToDDKDefault BchToDDK_DeltaC2_Default B0ToDDK_DeltaC2_Default

# Add lists of composites with UsrData
set UsrBCandLists $BtaCandLists
# Switch on UsrData
set writeUsrDataForSkim 1
