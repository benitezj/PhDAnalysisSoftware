#
# $Id: Psi2SExclPath.tcl,v 1.5 2005/11/16 23:01:58 fwinkl Exp $
#
# defines JpsitoeePath tag+micro filter path
#

path create Psi2SExclPath

# only run on MultiHadrons
path append  Psi2SExclPath FilterBGFMultiHadron
    
# now read micro and filter
path append Psi2SExclPath  BCountMHFilter

# make sure tag bits get created
sourceFoundFile EventTagTools/TagProcs.tcl
useTagFromList Psi2SKs      Psi2SExclPath
useTagFromList Psi2SKs2Pi0  Psi2SExclPath
useTagFromList Psi2SKp      Psi2SExclPath
useTagFromList Psi2SKstar   Psi2SExclPath


#  tag filter
mod clone TagFilterByName Psi2SExclFilter
path append Psi2SExclPath Psi2SExclFilter
talkto Psi2SExclFilter { orList set Psi2SKs}
talkto Psi2SExclFilter { orList set Psi2SKs2Pi0}
talkto Psi2SExclFilter { orList set Psi2SKp}
talkto Psi2SExclFilter { orList set Psi2SKstar}
