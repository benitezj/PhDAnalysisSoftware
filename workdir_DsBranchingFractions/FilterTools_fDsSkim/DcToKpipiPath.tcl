#
# $Id: DcToKpipiPath.tcl,v 1.8 2005/11/16 23:01:57 fwinkl Exp $
#
# defines DcToKpipiPath for tag filter based on "aDsToKPiPi" tag bit
#

sourceFoundFile EventTagTools/TagProcs.tcl

path create DcToKpipiPath

# only run on MultiHadrons
path append DcToKpipiPath FilterBGFMultiHadron

# create tag bit
# inclusive D+ -> K- pi+ pi+ with P*>1.3, mass window +-40 MeV
sequence create TagDcToKpipiSequence
makeTagFromList DcToKPiPiHardDefault aDcToKPiPi TagDcToKpipiSequence
path append DcToKpipiPath TagDcToKpipiSequence

# add Dc filter. 
module clone TagFilterByName DcToKPiPiTagFilter
path append DcToKpipiPath DcToKPiPiTagFilter
mod talk DcToKPiPiTagFilter
   andList set aDcToKPiPi
exit


