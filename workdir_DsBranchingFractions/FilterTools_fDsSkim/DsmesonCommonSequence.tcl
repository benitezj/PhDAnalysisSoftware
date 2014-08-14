#
# $Id: DsmesonCommonSequence.tcl,v 1.3 2005/03/14 17:28:07 fnc Exp $
#
#
# common configuration for Ds streams
#

global DSMESONCOMMONSEQUENCE
if [ expr ! [ info exists DSMESONCOMMONSEQUENCE ] ] {

    set DSMESONCOMMONSEQUENCE 1

    proc setDsmesonDecayModes module {

	sourceFoundFile ErrLogger/ErrLog.tcl
    
	talkto $module {
	    
	    tagPrefix set Dsmeson
	    momentumThresholds clear
	    momentumThresholds set -0.1
	    momentumThresholds set 0.5
	    momentumThresholds set 0.85
	    momentumThresholds set 1.3
	    momentumThresholds set 1.5
	    momentumThresholds set 1.8
	    momentumThresholds set 2.2
	    decayLists clear
	    decayLists set DsToPhiPiLoosePID
	    decayLists set Ds3prongLoosePID
	    decayLists set DsToPhiPiPiPiLoosePID
	    decayLists set DsToPhiRhoCLoosePID
	    decayLists set DsToKsKPiPiLoosePID 
	    decayLists set DsToEtaPiLoosePID
	    decayLists set DsToEtaRhoCLoosePID
	    decayLists set DsToEtaPPiLoosePID
	    decayLists set DsToEtaPRhoCLoosePID
	    decayLists set LambdaCToPKPiLoosePID 
	    verbose set f
	    
	}
	ErrMsg trace "$module> Defined Ds decay modes"
    }

    sequence create DsmesonCommonSequence
}
