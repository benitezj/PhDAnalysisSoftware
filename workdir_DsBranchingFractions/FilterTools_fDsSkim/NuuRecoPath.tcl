#
# $Id: NuuRecoPath
#
#
# defines includes Neutrino reconstruction path.  Includes TrkFixup (run in parallel with
# normal track selection)
#

# By making these local instead of global, I decouple this skim from the standard preset defaults
#
#global BtaCandLists
#global writeUsrDataForSkim
#global UsrBCandLists


# the following sets-up TrkFixupSequence to run in parallel (normal tracks
# are not affected), and configures loading BtaCandidates from these tracks
#
#  This is done globally for 20-series skims
#
#sourceFoundFile TrkFixup/TrkFixupParallel.tcl

# prescale on bgfilter mutlihad
module clone TagFilterByPrescale TagFilterByPrescaleNuuReco
talkto TagFilterByPrescaleNuuReco {
    andList set "BGFMultiHadron 0.1"
#    show
}

# create and configure module to assign PID 
module clone TaggingDispatch TrkPidNuuReco
talkto TrkPidNuuReco {
    verbose set false
    production set true
#    inputBtaCandidateList  set ChargedTracksFixup
    outputBtaCandidateList set NuuPidList
    outputMap              set NuuPidMap
#
# electron 
    electronSelector       set PidLHElectronSelector
    electronCriteria       set tight
# muon
    muonSelector           set PidMuonMicroSelector
    muonCriteria           set tight
# pion
    pionSelector           set none
# kaon
    kaonSelector           set PidKaonLHSelector
    kaonCriteria           set tight
# proton
    protonSelector         set PidProtonLHSelector
    protonCriteria         set tight
#
# activate histogramming for this module
    histoFlag              set f
# specify the number of entries in the ntuple (-1 means no limit)
    maxEventTuple          set 0
#activate debugging histograms
    debugHistograms        set f
}

# configure test to use correct lists
talkto NuuReco {
# use multiple-associated neutral list
    NeutralList set "CalorNeutral"
    TrackList set "NuuPidList"
# no V0s (yet!)
    V0List set "None"
    production set true
    verbose set false
}

path create NuuRecoPath
path append NuuRecoPath BetaMiniSequence
path append NuuRecoPath TagFilterByPrescaleNuuReco
#path append NuuRecoPath TrkFixupParallel
path append NuuRecoPath TrkPidNuuReco
path append NuuRecoPath NuuReco
path enable NuuRecoPath 

#
#  Configure output manually, as we must overwrite so many default settings.  This is basically
#  deepCopyMini with specialized lists
#

# first, derrive output collection
global SkimsToWrite
global SkimsNotToWrite
set theStream NuuReco
if {$SkimsToWrite == "all"} {
    set outputColl [getCollNameForStream $theStream]
} elseif {[lsearch -exact $SkimsToWrite $theStream] > -1} {
    set outputColl [getCollNameForStream $theStream]
} else {
    set outputColl "noOutput"
}
if {[lsearch -exact $SkimsNotToWrite $theStream] > -1} {
    set outputColl "noOutput"
}

ErrMsg trace "Setting output collection to $outputColl"

# if we're writing this skim, setup the output
if {$outputColl != "noOutput"} {
# Lists of (fixed) BtaCandidates
    lappend BtaCandLists  CalorNeutral NuuPidList NeutralHadrons PrimaryVertex
# no usr data for this skim (yet)
    set UsrBCAndLists ""
    set writeUsrDataForSkim 0
    
    global SkimMC
    lappend components Tag Aod Cnd Esd
    if [info exists SkimMC] {
	lappend components Tru
    }

    ErrMsg trace "Writing components $components of stream $theStream to $outputColl"
    writeMini NuuRecoPath $outputColl $components Kan $BtaCandLists trkFitType=All

# Override some defaults in the reco compression
    set output [miniStreamFromCollName $outputColl]

#    talkto BtaMiniReduceReco${output} {
#	TrkRecoTrkList set Complete
#	if { [ ErrLogging trace ] } {
#	    show
#	}	
#    }

#    talkto BtaMiniListMerge${output} {
#	BtaCandIdMapKey set Fixup
#	StoreCompositeP4 set true
#	production set true
#	verbose set false
#	if { [ ErrLogging trace ] } {
#	    show
#	}
#    }

# configure pid branches (for interactive access)
    talkto BtaMiniKanLoad${output}Cnd {
	sourceFoundFile SkimMini/SkimMiniDefaultPidBranches.tcl
    }

}
