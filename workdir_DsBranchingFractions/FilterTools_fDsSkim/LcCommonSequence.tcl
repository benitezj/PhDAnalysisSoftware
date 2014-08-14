#
# $Id: LcCommonSequence.tcl,v 1.6 2005/03/14 17:28:10 fnc Exp $
#
#
# common configuration for Lc streams
#

global LCCOMMONSEQUENCE
if [ expr ! [ info exists LCCOMMONSEQUENCE ] ] {

    set LCCOMMONSEQUENCE 1

    sourceFoundFile ErrLogger/ErrLog.tcl
    
    proc setLcDecayModes module {
	talkto $module {
	    tagPrefix set Lcbaryon
	    momentumThresholds clear
	    momentumThresholds set -0.1
	    momentumThresholds set 0.4
	    momentumThresholds set 2.2
	    decayLists clear

	    decayLists set LambdaCTopKpi
	    decayLists set LambdaCTopKpipi
	    decayLists set LambdaCTopKs
	    decayLists set LambdaCTopKspi
	    decayLists set LambdaCTopKspipi
	    decayLists set LambdaCToLzpi
	    decayLists set LambdaCToLzpipi
	    decayLists set LambdaCToLzpipipi
	    decayLists set LambdaCToSzpi
	    decayLists set LambdaCToSzpipipi
	    decayLists set LambdaCToLzK
	    decayLists set LambdaCToLzKpi
	    decayLists set LambdaCToLzKpipi
	    decayLists set LambdaCToSzK
	    decayLists set LambdaCToSzKpi
	    decayLists set LambdaCToSzKpipi
	    
	    
	}
	ErrMsg trace "$module> Defined Lc decay modes"
    }

    sequence create LcCommonSequence
}
