#
# $Id: Tau11Path.tcl,v 1.2 2003/09/27 02:14:23 swaban Exp $
#
# defines Tau11 tag+micro filter path
# 2003/09/14, Swagato Banerjee (University of Victoria)
# 
#
path create Tau11Path

# create L3Trigger filter
module clone TagFilterByName Tau11L3Filter
#
# accept tags that we are interested in
#
talkto Tau11L3Filter {
    orList set L3OutDch
    orList set L3OutEmc
}

path append Tau11Path Tau11L3Filter

# create tagbit filter
module clone TagFilterByName Tau11TagFilter

#
# accept tags that we are interested in
#
talkto Tau11TagFilter {
    orList set BGFMultiHadron
    orList set BGFNeutralHadron
    orList set BGFTau
    orList set BGFMuMu
    orList set BGFTwoProng
}

path append Tau11Path Tau11TagFilter

#
# now my filter
#

module clone TagFilterByName Tau11MicroFilter

talkto Tau11MicroFilter {
    # 1-1, 2-1 and 2-0 Tau events
    orList set  Tau11
}

path append Tau11Path Tau11MicroFilter
