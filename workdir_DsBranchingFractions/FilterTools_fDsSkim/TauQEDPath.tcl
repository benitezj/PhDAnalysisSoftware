#
# $Id: TauQEDPath.tcl,v 1.9 2001/11/29 16:06:24 paoloni Exp $
#
# defines TauQEDPath tag+micro filter path
# 03/09/00, Jinlong Zhang
# 
# redefine 1-on-1 tau events, remove prescale events 
# 08/24/00, Jinlong Zhang
#
path create TauQEDPath

# create tagbit filter
module clone TagFilterByName TauQEDTagFilter

#
# accept tags that we are interested in
#
talkto TauQEDTagFilter {
    orList set BGFMultiHadron
    orList set BGFTau
}

path append TauQEDPath TauQEDTagFilter

#
# now my filter
#

module clone TagFilterByName TauQEDMicroFilter
talkto TauQEDMicroFilter {
    # 2 prong Tau events
    orList set  Tau1on1

    # 1-3  Tau events
    orList set  Tau31

    # 3-3 Tau events
    orList set  Tau3on3

    # 1-5 Tau events
    orList set  Tau1on5

    # 4 prong 2-photon events
    orList set  is2Gam4Prongs

    # 1-3 Tau events, selected with only 2 tracks on 3 side required
    orList set Tau2on1Tracking
}

path append TauQEDPath TauQEDMicroFilter



