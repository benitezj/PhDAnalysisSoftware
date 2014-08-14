#
# $Id: JpsiISRPath.tcl,v 1.4 2005/11/16 23:01:58 fwinkl Exp $
#

path create JpsiISRPath

# NOT on MultiHadrons!

# ISR Jpsi tag bit
addTagModule TagISRJpsi JpsiISRPath

mod clone TagFilterByName JpsiISRFilter
path append JpsiISRPath JpsiISRFilter
talkto JpsiISRFilter { andList set ISRJpsi}


