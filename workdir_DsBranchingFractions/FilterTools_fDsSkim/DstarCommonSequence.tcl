#
#  $Id: DstarCommonSequence.tcl,v 1.13 2004/10/14 22:10:06 mwilson Exp $
#
# common configuration for D* streams
#

global DSTARCOMMONSEQUENCE
if [ expr ! [ info exists DSTARCOMMONSEQUENCE ] ] {

    set DSTARCOMMONSEQUENCE 1
    
    sourceFoundFile ErrLogger/ErrLog.tcl
    
    proc setDstarDecayModes {module args} {

	talkto $module {
	    
	    tagPrefix set Dstar
	    
	    momentumThresholds clear
	    momentumThresholds set -0.1
	    momentumThresholds set 0.5
	    momentumThresholds set 1.0
	    momentumThresholds set 1.3
	    momentumThresholds set 1.5
	    momentumThresholds set 1.8
	    momentumThresholds set 2.2
	    
	    decayLists clear
	    decayLists set D0ToKPiLoose
	    decayLists set D0MainLoose
	    decayLists set D0CabibboLoose
	    decayLists set D0Loose
	    
	    decayLists set D0ToKPiLoosePID
	    decayLists set D0MainLoosePID
	    decayLists set D0CabibboLoosePID
	    decayLists set D0LoosePID
	    
	    decayLists set D0ToKPiLoose
	    decayLists set D0ChrgKLoose
#	    decayLists set D0ToKPiLooseRsWs
#	    decayLists set D0ChrgKLooseRsWs
	    
	    decayLists set D0ToKPiLoosePID
	    decayLists set D0ChrgKLoosePID
#	    decayLists set D0ToKPiLooseRsWsPID
#	    decayLists set D0ChrgKLooseRsWsPID
	    
	    decayLists set Dstar0(KPi)Loose
	    
	    decayLists set Dstar0(KPi)LoosePID
	    
	    decayLists set DcToKPiPiLoosePID
	    decayLists set DcLoosePID

	    if { [ llength $args ] > 0 } {
		
		motherLists clear
		motherListsModes clear

		motherLists set DstarLoose
		motherListsModes set 4
		
		motherLists set DstarLoosePID
		motherListsModes set 4
		
		motherLists set DstarChrgKLoose
		motherListsModes set 2
		
		motherLists set DstarChrgKLoosePID
		motherListsModes set 2
		
		motherLists set Dstar0(KPi)Loose
		motherListsModes set 1
		
		motherLists set Dstar0(KPi)LoosePID
		motherListsModes set 1
		
		motherLists set DstarToDcPi0Loose
		motherListsModes set 2
	    }
	    
	}
	ErrMsg trace "$module> Defined Dstar decay modes"
    }

    sequence create DstarCommonSequence
    
}
