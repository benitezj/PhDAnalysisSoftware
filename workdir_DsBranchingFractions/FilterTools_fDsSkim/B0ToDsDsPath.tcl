#
# $Id: B0ToDsDsPath.tcl,v 1.3 2005/11/16 23:01:54 fwinkl Exp $
# 
# Defines B0 -> Ds(*)Ds(*) path
#
# For more info, refer to
# http://www.slac.stanford.edu/BFROOT/www/Physics/Analysis/AWG/EHBDOC/DstarDstar/B0ToDsDs.htm

sourceFoundFile EventTagTools/TagProcs.tcl

mod clone TagFilterByName B0ToDsDs_Filter

mod talk B0ToDsDs_Filter
  orList set B0ToDsDsLoose
  orList set B0ToDsstarDsLoose
  orList set B0ToDsstarDsstarLoose
  assertIfMissing set true
exit

path create B0ToDsDsPath
path append B0ToDsDsPath FilterBGFMultiHadron
addTagModule TagMakeBRecoToDD B0ToDsDsPath
path append B0ToDsDsPath B0ToDsDs_Filter
