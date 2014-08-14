###############################################################################
# $Id: B0ToDDstar0h0Path.tcl,v 1.4 2005/11/17 02:58:04 fwinkl Exp $
#
# define B0ToDDstar0h0 tag filter path
#
# For  B0 -> D(*)0h0;  D0 -> NonCP, CPEven, CPOdd and KsPiPi
# Lists are defined in SimpleComposition/SmpB0ToDDstar0h0ProdSequence.tcl
#
#  Chih-hsiang Cheng 2005-10-07
###############################################################################

## for writing composite candidates
global BtaCandLists
global writeUsrDataForSkim
global UsrBCandLists
##

path create B0ToDDstar0h0Path

## Event Tag filters
path append B0ToDDstar0h0Path FilterBGFMultiHadron
mod clone TagFilterByValue B0ToDDstar0h0_R2AllFilter
path append B0ToDDstar0h0Path B0ToDDstar0h0_R2AllFilter
talkto B0ToDDstar0h0_R2AllFilter {
    TagValueName set R2All
    TagValueType set Floating
    TagValueMaximum set 0.5
    assertIfMissing set true
}

## Create tagbits
set h0Names { Pi0  Eta  Etap  Omega }
set ddNames { D0  Dstar0 }
set d0ModeCats { NonCP CPEven CPOdd KsPiPi }
# Lists are defined in SimpleComposition/SmpB0ToDDstar0h0ProdSequence.tcl
#  B list names are like  B0ToD0Pi0_D0ToNonCP
foreach aH0Name $h0Names {
    foreach aDDName $ddNames {
	foreach aD0ModeCat $d0ModeCats {
	    set B0ToDDstar0h0List B0To${aDDName}${aH0Name}_D0To${aD0ModeCat}
	    makeTagFromList $B0ToDDstar0h0List $B0ToDDstar0h0List B0ToDDstar0h0Path
	}
    }
}


## B candidate list filters
mod clone TagFilterByName B0ToDDstar0h0_TagFilter
path append B0ToDDstar0h0Path B0ToDDstar0h0_TagFilter

set B0ToDDstar0h0Lsits [ list ]
foreach aH0Name $h0Names {
    foreach aDDName $ddNames {
	foreach aD0ModeCat $d0ModeCats {
	    lappend B0ToDDstar0h0Lsits B0To${aDDName}${aH0Name}_D0To${aD0ModeCat}
	}
    }
}

talkto B0ToDDstar0h0_TagFilter {
    foreach b0list $B0ToDDstar0h0Lsits {
	orList set $b0list
    }
#    show
}

# Add lists of composites to write
foreach b0list $B0ToDDstar0h0Lsits {
    lappend BtaCandLists $b0list
}

# Add lists of composites with UsrData
set UsrBCandLists $BtaCandLists
# Write UsrData
set writeUsrDataForSkim 1
