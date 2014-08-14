#
# $Id: Tau1NPath.tcl,v 1.1 2003/09/15 15:11:02 swaban Exp $
#
# defines Tau1N tag+micro filter path
# 2003/09/14, Swagato Banerjee (University of Victoria)
# 
#
path create Tau1NPath

# create L3Trigger filter
module clone TagFilterByName Tau1NL3Filter
#
# accept tags that we are interested in
#
talkto Tau1NL3Filter {
    orList set L3OutDch
    orList set L3OutEmc
}

path append Tau1NPath Tau1NL3Filter

# create tagbit filter
module clone TagFilterByName Tau1NTagFilter

#
# accept tags that we are interested in
#
talkto Tau1NTagFilter {
    orList set BGFMultiHadron
    orList set BGFNeutralHadron
    orList set BGFTau
    orList set BGFMuMu
    orList set BGFTwoProng
}

path append Tau1NPath Tau1NTagFilter

#
# now my filter
#

module clone TagFilterByName Tau1NMicroFilter

talkto Tau1NMicroFilter {
    # 1-N Tau events (where N>=3)
    orList set  Tau1N
}

path append Tau1NPath Tau1NMicroFilter
