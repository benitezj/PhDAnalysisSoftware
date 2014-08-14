#
# $Id: L3CosmicPath.tcl,v 1.3 2002/08/29 00:12:28 bartoldu Exp $
#
# defines L3CosmicPath tag filter path
#

path create L3CosmicPath

# clone and customize filter module
mod clone TagFilterByName CosmicFilter
path append L3CosmicPath CosmicFilter
talkto CosmicFilter {
    andList set DigiFCosmic
}
