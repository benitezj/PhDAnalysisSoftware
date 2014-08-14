#
# $Id: BTollPath.tcl,v 1.2 2005/11/16 23:01:57 fwinkl Exp $
#
# defines BTollPath
#

sourceFoundFile EventTagTools/TagProcs.tcl

path create BTollPath

sequence create TagBTollSequence
makeTagFromList B0ToPiPiDefault   BToll   TagBTollSequence        ;# B0->ll decays
path append  BTollPath TagBTollSequence

mod clone TagFilterByName TagBTollFilter
talkto TagBTollFilter {
    andList set BToll
}

path append  BTollPath TagBTollFilter

