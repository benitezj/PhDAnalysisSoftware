#
# $Id: RandomBackgroundPath.tcl,v 1.2 2002/08/26 21:33:11 bartoldu Exp $
# 
# defines RandomBackground filter path
# 010410, Anders Ryd
# Select background triggers for simulation production.
#
path create RandomBackgroundPath

# create tagbit filter
module clone TagFilterByName RandomBackgroundTagFilter
path append RandomBackgroundPath RandomBackgroundTagFilter
#
# accept tags that we are interested in
#
talkto RandomBackgroundTagFilter {
    orList set DigiFBackground
}
