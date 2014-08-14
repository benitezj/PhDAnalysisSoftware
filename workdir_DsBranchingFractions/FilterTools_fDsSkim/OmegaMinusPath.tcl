#
# $Id: OmegaMinusPath.tcl,v 1.4 2005/11/16 23:01:58 fwinkl Exp $
#
#
# Define tag filters for Omega- decay modes.
# For now, just skim on the simplest: Omega- -> Lambda K-

global BtaCandLists

path create OmegaMinusPath

# only run on MultiHadrons
path append OmegaMinusPath FilterBGFMultiHadron

# make tag from list
sourceFoundFile EventTagTools/TagProcs.tcl
makeTagFromList OmegaToLambdaK_Loose OmegaToLambdaK_Loose OmegaMinusPath

# make a clone of the filter module
mod clone TagFilterByName OmegaMinusFilterForOmegaMinusSkim
path append OmegaMinusPath OmegaMinusFilterForOmegaMinusSkim

# Configure tag filter
talkto OmegaMinusFilterForOmegaMinusSkim {
  orList set OmegaToLambdaK_Loose
}
