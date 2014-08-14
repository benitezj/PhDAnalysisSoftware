#
# $Id: A0Path.tcl,v 1.2 2005/11/16 23:01:54 fwinkl Exp $
#
# defines ExclEtaPPath tag filter path
#

sourceFoundFile EventTagTools/TagProcs.tcl
path create A0Path

# only run on MultiHadrons
path append A0Path FilterBGFMultiHadron

sequence create TagA0Sequence
addTagModule TagA0 TagA0Sequence {
	trackCandidates set "GoodTracksLoose"
	rho0Candidates  set "rho0Default"
	rhocCandidates  set "rhoCDefault"
	k0sCandidates   set "KsDefault"
	a0Candidates    set "a0Default"
	kstar0Candidates set "KstarKPiLoose"
	kstarkspiCandidates set "KstarKsPiLoose"
	kstarkpi0Candidates set "KstarKPi0Loose"
	pi0Candidates set "pi0AllLoose"
	minCmsPExcl set 1.9
	maxCmsPExcl set 3.1
	minCmsPIncl set 1.7
	maxCmsPIncl set 3.1
	MBcut set 5.15
	DEcut set 0.35
	verbose set false
}

path append A0Path TagA0Sequence
path append A0Path A0Filter
