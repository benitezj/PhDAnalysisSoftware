#
# $Id: BFlav_LoosePath.tcl,v 1.8 2005/11/16 23:01:55 fwinkl Exp $
#
# defines BFlav_LoosePath tag filter path
#
#  O.Long, UCSB
#

path create BFlav_LoosePath

path append BFlav_LoosePath FilterBGFMultiHadron

global tagSkim
if [ expr ! [ info exists tagSkim ] ] {

  # Append and enable the tag module
  sourceFoundFile FilterTools/SkimTagBFlavSequence.tcl
  useAllBFlavTagModules BFlav_LoosePath

  catch {
    mod clone TagFromTag BFlav_Loose_Tag
    path append BFlav_LoosePath BFlav_Loose_Tag
    talkto BFlav_Loose_Tag {
      tagName set BFlav_Loose
      orList set B0ToDstarPi_KPi
      orList set B0ToDstarPi_KPiPi0
      orList set B0ToDstarPi_K3Pi
      orList set B0ToDstarPi_KsPiPi
      orList set B0ToDstarRhoC_KPi
      orList set B0ToDstarRhoC_KPiPi0
      orList set B0ToDstarRhoC_K3Pi
      orList set B0ToDstarRhoC_KsPiPi
      orList set B0ToDstarA1C_KPi
      orList set B0ToDstarA1C_KPiPi0
      orList set B0ToDstarA1C_K3Pi
      orList set B0ToDstarA1C_KsPiPi
      orList set B0ToDchPi_KPiPi
      orList set B0ToDchPi_KsPi
      orList set B0ToDchRhoC_KPiPi
      orList set B0ToDchRhoC_KsPi
      orList set B0ToDchA1C_KPiPi
      orList set B0ToDchA1C_KsPi
    }
  }

}

mod clone TagFilterByName BFlav_LooseFilter
path append BFlav_LoosePath BFlav_LooseFilter
talkto BFlav_LooseFilter {
  orList set BFlav_Loose
}
