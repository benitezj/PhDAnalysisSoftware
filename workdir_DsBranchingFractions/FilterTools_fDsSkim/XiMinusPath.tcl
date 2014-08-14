#
# $Id: XiMinusPath.tcl,v 1.3 2005/11/16 23:01:59 fwinkl Exp $
#
#
# Define tag filters for Xi- -> Lambda pi-

global BtaCandLists

path create XiMinusPath

# only run on MultiHadrons
path append XiMinusPath FilterBGFMultiHadron

# make tag from list
sourceFoundFile EventTagTools/TagProcs.tcl
makeTagFromList XiMinusToLambdaPi_Loose XiMinusToLambdaPi_Loose XiMinusPath

# make a clone of the filter module
mod clone TagFilterByName XiMinusFilterForXiMinusSkim
path append XiMinusPath XiMinusFilterForXiMinusSkim

# Configure tag filter
talkto XiMinusFilterForXiMinusSkim {
  orList set XiMinusToLambdaPi_Loose
}
