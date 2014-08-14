#
# $Id: DmesonCommonSequence.tcl,v 1.4 2008/12/01 22:20:07 gjackson Exp $
#
#
# common configuration for D streams
#

global DMESONCOMMONSEQUENCE
if [ expr ! [ info exists DMESONCOMMONSEQUENCE ] ] {

    set DMESONCOMMONSEQUENCE 1
    
    sourceFoundFile ErrLogger/ErrLog.tcl
    
    proc setDmesonDecayModes module {
#	catch {
	    talkto $module {
		tagPrefix set Dmeson
		momentumThresholds clear
		momentumThresholds set -0.1
		momentumThresholds set 0.5 
		momentumThresholds set 1.0 
		momentumThresholds set 1.3    
		momentumThresholds set 1.5    
		momentumThresholds set 1.8    
		momentumThresholds set 2.2    
		momentumThresholds set 2.4    
		decayLists clear
		decayLists set D0ToKPiLoosePID
		decayLists set D0MainLoosePID
		decayLists set D0CabibboLoosePID
		decayLists set D0LoosePID
		decayLists set Dc3prongLoosePID
		decayLists set DcToKsPiPi0LoosePID
		decayLists set DcToKsPiPiPiLoosePID
		decayLists set DcToKPiPiPi0LoosePID
		decayLists set DcToKsKPiPiLoosePID
		decayLists set DcToKPiPiLoosePID
	    }
	    ErrMsg trace "$module> Defined D decay modes"
#	}
    }

    sequence create DmesonCommonSequence

    
}
