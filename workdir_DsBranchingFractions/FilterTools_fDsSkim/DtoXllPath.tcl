#
# $Id: DtoXllPath.tcl,v 1.3 2005/11/16 23:01:57 fwinkl Exp $
#
#
# defines the path for DtoXll 
# 
path create DtoXllPath
path append DtoXllPath FilterBGFMultiHadron

# add tag module
sourceFoundFile EventTagTools/TagProcs.tcl
addTagModule TagDtoXll DtoXllPath

# add tag filter
path append DtoXllPath DtoXllTagFilter       


