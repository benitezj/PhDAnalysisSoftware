#
# $Id: InclDstarDsPath.tcl,v 1.4 2000/08/30 09:20:23 yury Exp $
#
# defines InclDstarDs tag+micro filter path
#

path create InclDstarDsPath

# only run on MultiHadrons
path append InclDstarDsPath FilterBGFMultiHadron

# add D* filter. 
module clone TagFilterByName InclDstarDsTagFilter
path append InclDstarDsPath InclDstarDsTagFilter
mod talk InclDstarDsTagFilter
   andList set InclDstarDsLoose
exit

