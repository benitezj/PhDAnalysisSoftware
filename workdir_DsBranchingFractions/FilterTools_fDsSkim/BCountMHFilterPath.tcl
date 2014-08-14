#
# $Id: BCountMHFilterPath.tcl,v 1.4 2005/03/14 17:28:02 fnc Exp $
#
#
#
# defines tag and micro BCountMHFilterPath
#

#mod talk BCountMHFilter
#nTrkCut set 5
#doNtuple set t
#doHisto set t
#verbose set t
#exit

path create BCountMHFilterPath

# only run on MultiHadrons
path append BCountMHFilterPath FilterBGFMultiHadron

# now read micro and filter
path append BCountMHFilterPath BCountMHFilter
