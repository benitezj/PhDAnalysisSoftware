#
# $Id: InclDstarDstarPath.tcl,v 1.4 2000/08/30 09:20:23 yury Exp $
#
# defines InclDstarDstar tag+micro filter path
#

path create InclDstarDstarPath

# only run on MultiHadrons
path append InclDstarDstarPath FilterBGFMultiHadron

# add D* filter. 
module clone TagFilterByName InclDstarDstarTagFilter
path append InclDstarDstarPath InclDstarDstarTagFilter
mod talk InclDstarDstarTagFilter
   andList set InclDstarDstarLoose
exit
