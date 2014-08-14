#
# $Id: IsrRadBhabhaPath.tcl,v 1.3 2005/03/14 17:28:09 fnc Exp $
#
##
# defines IsrGammaPath tag filter path
#
# Author:  Joe Izen    UTDallas
#

path create IsrRadBhabhaPath

# append the Isr Filter and tell it do find Gamma candidates
mod clone IsrTagbitFilter IsrTagbitRadBhabhaFilter
path append IsrRadBhabhaPath IsrTagbitRadBhabhaFilter

path enable IsrRadBhabhaPath

mod talk IsrTagbitRadBhabhaFilter
  verbose             set f
  enableGammaISR      set f
  enableNoGammaISR    set f
  enableLooseGammaISR set f
  enableRadBhabha     set t
#  echo IsrRadBhabhaPath has the following settings for IsrRadBhabhaFilter
#  show
exit

