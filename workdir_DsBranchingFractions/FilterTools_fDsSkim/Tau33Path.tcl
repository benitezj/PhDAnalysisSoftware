#
# $Id: Tau33Path.tcl,v 1.2 2003/09/27 02:14:23 swaban Exp $
#
# defines Tau33 tag+micro filter path
# 2003/09/14, Swagato Banerjee (University of Victoria)
# 
#
path create Tau33Path

# create L3Trigger filter
module clone TagFilterByName Tau33L3Filter
#
# accept tags that we are interested in
#
talkto Tau33L3Filter {
    orList set L3OutDch
    orList set L3OutEmc
}

path append Tau33Path Tau33L3Filter

# create tagbit filter
module clone TagFilterByName Tau33TagFilter

#
# accept tags that we are interested in
#
talkto Tau33TagFilter {
    orList set BGFMultiHadron
    orList set BGFNeutralHadron
    orList set BGFTau
    orList set BGFMuMu
    orList set BGFTwoProng
}

path append Tau33Path Tau33TagFilter

#
# now my filter
#

module clone TagFilterByName Tau33MicroFilter

talkto Tau33MicroFilter {
    # 2-2, 2-3 and 3-3 Tau events
    orList set  Tau33
}

path append Tau33Path Tau33MicroFilter
