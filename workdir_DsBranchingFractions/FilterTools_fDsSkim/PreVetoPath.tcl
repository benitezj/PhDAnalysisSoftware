#
# $Id: PreVetoPath.tcl,v 1.3 2002/08/26 21:33:11 bartoldu Exp $
#
# defines PreVetoPath tag filter path
#

path create PreVetoPath

# clone and customize filter module
mod clone TagFilterByName PreVetoFilter
path append PreVetoPath  PreVetoFilter
talkto  PreVetoFilter {
    andList set DigiFDchEmcPreVeto
}
