#
# $Id: FilterToolsProcs.tcl,v 1.4 2007/12/18 13:49:30 marchior Exp $
#
global FILTERTOOLSPROCS
if [expr ! [ info exists FILTERTOOLSPROCS] ] {
  set FILTERTOOLSPROCS 1

  proc addSkimSequence {sequence tclFile {mainSequence SkimCompProdSequence}} {
    sourceFoundFile $tclFile
    sequence append $mainSequence $sequence
  }

  proc setup3BodySkimPath {tagBit {definition ""}} {
    set path ${tagBit}Path
    path create $path
    # only run on MultiHadrons
    path append $path FilterBGFMultiHadron

    # add 2 body skim tag sequence, enable all modules (needed for 3Body)
    sourceFoundFile FilterTools/SkimTag2BodySequence.tcl
    path append $path SkimTag2BodySequence
    enableAll2BodyModules
 
    # add 3 body skim tag sequence, enable tag module
    sourceFoundFile FilterTools/SkimTag3BodySequence.tcl
    path append $path SkimTag3BodySequence
    module enable Tag${tagBit}
    # special case for this tag bit (needs the merger module)
    if {$tagBit == "BKsKsKs3body" || $tagBit == "BKsKsKs03a3body"} {mod enable ThreeBody3KsMergepi0pi}
    talkto Tag${tagBit} {
      production set true
      verbose set false
    }

    # add filter module
    path append $path ${tagBit}TagFilter
    if {$definition == ""} {set definition "3bodytagbit set $tagBit"}
    talkto ${tagBit}TagFilter {uplevel $definition}
  }

  proc setupMulti3BodySkimPath {module path} {
    # only run on MultiHadrons
    path append $path FilterBGFMultiHadron

    # add 2 body skim tag sequence, enable all modules (needed for 3Body)
    sourceFoundFile FilterTools/SkimTag2BodySequence.tcl
    path append $path SkimTag2BodySequence
    enableAll2BodyModules

    # add 3 body skim tag sequence, enable tag module
    sourceFoundFile FilterTools/SkimTag3BodySequence.tcl
    path append $path SkimTag3BodySequence
    module enable $module
  }
}
