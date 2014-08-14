#
# $Id: IsrNoGammaPath.tcl,v 1.4 2005/03/14 17:28:09 fnc Exp $
#
##
# defines IsrNoGammaPath tag filter path
#
# Author:  Joe Izen    UTDallas
#

path create IsrNoGammaPath

# this is needed to create eventInfo object
#path append IsrNoGammaPath BtaLoadBeamSpot

# append the Isr Filter and tell it do find NoGamma candidates
mod clone IsrTagbitFilter IsrTagbitNoGammaFilter
path append IsrNoGammaPath IsrTagbitNoGammaFilter

path enable IsrNoGammaPath

mod enable IsrTagbitNoGammaFilter
mod talk IsrTagbitNoGammaFilter
  verbose             set f
  enableGammaISR      set f
  enableNoGammaISR    set t
  enableLooseGammaISR set f
  enableRadBhabha     set f
#  echo IsrNoGammaPath has the following settings for IsrNoGammaFilter
#  show
exit 

