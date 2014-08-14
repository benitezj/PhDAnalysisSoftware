#
# $Id: BFlav_FinalPath.tcl,v 1.22 2007/10/24 17:39:28 chcheng Exp $
#
# defines BFlav_FinalPath tag filter path
#
#  O.Long, UCSB
#

#need to enable some other "skim" paths - lange
#sourceFoundFile FilterTools/BFlav_LoosePath.tcl
#path enable BFlav_LoosePath
# needs to happen FIRST -before the path create

lappend BFlavPaths BFlav_RhoC_Path
lappend BFlavPaths BFlav_A1C_Path
lappend BFlavPaths BFlav_D0_KPi_Path
lappend BFlavPaths BFlav_D0_KPiPi0_Path
lappend BFlavPaths BFlav_D0_K3Pi_Path
lappend BFlavPaths BFlav_D0_KsPiPi_Path
lappend BFlavPaths BFlav_Dc_KPiPi_Path
lappend BFlavPaths BFlav_Dc_KsPi_Path
lappend BFlavPaths BFlav_B0ToDstarPi_Path
lappend BFlavPaths BFlav_B0ToDstarRhoC_Path
lappend BFlavPaths BFlav_B0ToDstarA1C_Path
lappend BFlavPaths BFlav_B0ToDPi_Path
lappend BFlavPaths BFlav_B0ToDRhoC_Path
lappend BFlavPaths BFlav_B0ToDA1C_Path

foreach path $BFlavPaths {
  sourceFoundFile FilterTools/${path}.tcl

  path insert $path -before FilterBGFMultiHadron RecoBGSkimFilter
  path enable $path
}


path create BFlav_FinalPath
path append BFlav_FinalPath FilterBGFMultiHadron

sourceFoundFile CompositionSequences/CompBFlavB0Sequences.tcl
path append BFlav_FinalPath CompBFlavB0Sequence

sourceFoundFile CompositionSequences/CompBFlavBchSequences.tcl
path append BFlav_FinalPath CompBFlavBchSequence


global tagSkim
if [ expr ! [ info exists tagSkim ] ] {

  # Append and enable the tag module
  sourceFoundFile FilterTools/SkimTagBFlavSequence.tcl
  useAllBFlavTagModules BFlav_FinalPath
  
  catch {
    mod clone TagFromTag BFlav_Final_Tag
    path append BFlav_FinalPath BFlav_Final_Tag
    talkto BFlav_Final_Tag {
      tagName set BFlav_Final
      orList set B0ToDstarPi_FinalBFlav
      orList set B0ToDstarRhoC_FinalBFlav
      orList set B0ToDstarA1C_FinalBFlav
      orList set B0ToDPi_FinalBFlav
      orList set B0ToDRhoC_FinalBFlav
      orList set B0ToDA1C_FinalBFlav
    }
        
    mod clone TagFromTag BFlav_FinalCTT_Tag
    path append BFlav_FinalPath BFlav_FinalCTT_Tag
    talkto BFlav_FinalCTT_Tag {
      tagName set BFlav_Final_CTT
      orList set B0ToDstarPi_FinalBFlav_CTT
      orList set B0ToDstarRhoC_FinalBFlav_CTT
      orList set B0ToDstarA1C_FinalBFlav_CTT
      orList set B0ToDPi_FinalBFlav_CTT
      orList set B0ToDRhoC_FinalBFlav_CTT
      orList set B0ToDA1C_FinalBFlav_CTT
    }
  }

}

mod clone TagFilterByName BFlav_FinalFilter
path append BFlav_FinalPath BFlav_FinalFilter
talkto BFlav_FinalFilter {
  orList set BFlav_Final
}

sourceFoundFile CompositionSequences/CompBFlavTrkPIDSequence.tcl
path append BFlav_FinalPath BFlavTrkPIDSequence
path append BFlav_FinalPath BrecoQa

# Add lists of composites to write
lappend BtaCandLists B0ToDstarPiBFlav B0ToDstarRhoCBFlav B0ToDstarA1CBFlav B0ToDPiBFlav B0ToDRhoCBFlav B0ToDA1CBFlav
