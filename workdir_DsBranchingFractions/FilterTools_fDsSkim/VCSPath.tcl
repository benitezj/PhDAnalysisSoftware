#
# $Id: VCSPath.tcl,v 1.4 2005/11/15 19:33:18 fwinkl Exp $
#
# defines Virtual Compton Scattering tag filter path
#

path create VCSPath

mod clone TagFilterByName VCSTagFilter
path append VCSPath  VCSTagFilter 
talkto VCSTagFilter { 
# replaced a long time ago by the next tag bit
#    orList set BGFVirtualComptonScattering 
    orList set DigiFVirtualCompton 
}
