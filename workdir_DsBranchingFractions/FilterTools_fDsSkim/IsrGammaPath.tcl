#
# $Id: IsrGammaPath.tcl,v 1.4 2005/03/14 17:28:09 fnc Exp $
#
##
# defines IsrGammaPath tag filter path
#
# Author:  Joe Izen    UTDallas
#

path create IsrGammaPath

# this is needed to create eventInfo object
#path append IsrGammaPath BtaLoadBeamSpot

# append the Isr Filter and tell it do find Gamma candidates
mod clone IsrTagbitFilter IsrTagbitGammaFilter
path append IsrGammaPath IsrTagbitGammaFilter

path enable IsrGammaPath

mod talk IsrTagbitGammaFilter
  verbose             set f
  enableGammaISR      set t
  enableNoGammaISR    set f
  enableLooseGammaISR set f
  enableRadBhabha     set f
#  echo IsrGammaPath has the following settings for IsrGammaFilter
#  show
exit

