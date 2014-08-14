#
# $Id: BCountMHFilter08Path.tcl,v 1.1 2008/05/02 21:23:34 gmcgreg Exp $
#
#
#
# defines tag and micro BCountMHFilter08Path
# 2008 revised BCounting skim
# Grant McGregor, 2 May 2008


#mod talk BCountMHFilter08
#nTrkCut set 5
#doNtuple set t
#doHisto set t
#verbose set t
#exit

path create BCountMHFilter08Path

# only run on MultiHadrons
path append BCountMHFilter08Path FilterBGFMultiHadron

# now read micro and filter
path append BCountMHFilter08Path BCountMHFilter08
