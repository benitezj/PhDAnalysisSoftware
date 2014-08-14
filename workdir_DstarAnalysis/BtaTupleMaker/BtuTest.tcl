#------------------------------------------------------------------------------
# $Id: BtuTest.tcl,v 1.1 2007/02/07 07:17:09 chcheng Exp $
# Sample MyMiniAnalysis.tcl file
#------------------------------------------------------------------------------
# always source the error logger early in your main tcl script
sourceFoundFile ErrLogger/ErrLog.tcl
sourceFoundFile FrameScripts/FwkCfgVar.tcl
sourceFoundFile FrameScripts/talkto.tcl
# Disable the use of envvars
set ProdTclOnly true

# set the error logging level to 'warning'.  If you encounter a configuration 
# error you can get more information using 'trace'
ErrLoggingLevel warning

## allowed values of BetaMiniReadPersistence are (currently) "Kan", "Bdb"
##
FwkCfgVar BetaMiniReadPersistence Kan

## allowed (non-expert) values of levelOfDetail are "micro", "cache", "extend" 
## or "refit"
##
FwkCfgVar levelOfDetail "cache"

## allowed values of ConfigPatch are "Run1", "Run2" or "MC".  This MUST be set 
## consistent ## with your input data type or you will get INCONSISTENT OR 
## INCORRECT RESULTS
##
FwkCfgVar ConfigPatch "MC"

##
## Set the number of events to run. If this isn't set, all events in the
## input collections will be processed.
##
FwkCfgVar NEvent

## choose the flavor of ntuple to write (hbook or root) and the file name
##
FwkCfgVar BetaMiniTuple "root"
FwkCfgVar histFileName "BtuTest.root"

sourceFoundFile BetaMiniUser/btaMiniPhysics.tcl

mod clone SmpMergerDefiner mergeTracksNeutrals
talkto mergeTracksNeutrals {
    inputListNames set ChargedTracks
    inputListNames set CalorNeutral
}


path append Everything mergeTracksNeutrals
path append Everything BtuTupleMaker

talkto BtuTupleMaker {
    listToDump   set mergeTracksNeutrals

    if { $ConfigPatch == "MC" } {
	fillMC set true
	mcBlockContents    set "Mass CMMomentum Momentum Vertex"
    } else {
	fillMC set false
    }
    eventBlockContents set "EventID CMp4 BeamSpot"
    ntpBlockConfigs    set "pi+  pi  0  50"
    ntpBlockConfigs    set "gamma  gamma  0  100"
    ntpBlockToTrk      set "pi"
    ntpBlockContents   set "pi   : Momentum"
    ntpBlockContents   set "gamma : Momentum"
    if { $ConfigPatch == "MC" } {
	ntpBlockContents  set "TRK  : MCIdx"
    }

    gamExtraContents set EMC

    baseTrackList   set ChargedTracks

    show
}

# path list
if [info exists NEvent] {
   ev begin -nev $NEvent
} else {
  ev begin 
}

ErrMsg trace "completed OK"
exit
