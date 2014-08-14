#
# $Id: BchToDDKPath.tcl,v 1.2 2005/11/16 23:01:57 fwinkl Exp $
# 
# defines B0 and B+ to DDK filter path
#
#

path create BchToDDKPath

path append BchToDDKPath FilterBGFMultiHadron

path append BchToDDKPath TagMakeDDK

mod clone TagFilterByName BchToDDKFilter
path append BchToDDKPath BchToDDKFilter
talkto BchToDDKFilter {
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
}
