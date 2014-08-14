#
# $Id: LambdaVeryVeryLoosePath.tcl,v 1.3 2005/11/16 23:01:58 fwinkl Exp $
#
#
# Define tag filters for Lambda -> p+ pi-

global BtaCandLists

path create LambdaVeryVeryLoosePath

# only run on MultiHadrons
path append LambdaVeryVeryLoosePath FilterBGFMultiHadron

# make tag from list
sourceFoundFile EventTagTools/TagProcs.tcl
makeTagFromList LambdaToPPiVeryVeryLoosePID LambdaToPPiVeryVeryLoosePID LambdaVeryVeryLoosePath

# make a clone of the filter module
mod clone TagFilterByName LambdaFilterForVeryVeryLooseLambdaSkim
path append LambdaVeryVeryLoosePath LambdaFilterForVeryVeryLooseLambdaSkim

# Configure tag filter
talkto LambdaFilterForVeryVeryLooseLambdaSkim {
  orList set LambdaToPPiVeryVeryLoosePID
}
