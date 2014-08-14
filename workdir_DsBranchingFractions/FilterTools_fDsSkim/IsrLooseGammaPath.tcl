#
# $Id: IsrLooseGammaPath.tcl,v 1.3 2005/03/14 17:28:09 fnc Exp $
#
##
# defines IsrGammaPath tag filter path
#
# Author:  Joe Izen    UTDallas
#

path create IsrLooseGammaPath

# append the Isr Filter and tell it do find Gamma candidates
mod clone IsrTagbitFilter IsrTagbitLooseGammaFilter
path append IsrLooseGammaPath IsrTagbitLooseGammaFilter

path enable IsrLooseGammaPath

mod talk IsrTagbitLooseGammaFilter
  verbose             set f
  enableGammaISR      set f
  enableNoGammaISR    set f
  enableLooseGammaISR set t
  enableRadBhabha     set f
#  echo IsrLooseGammaPath has the following settings for IsrLooseGammaFilter
#  show
exit

