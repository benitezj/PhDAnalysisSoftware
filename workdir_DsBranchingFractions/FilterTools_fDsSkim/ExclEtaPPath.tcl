#
# $Id: ExclEtaPPath.tcl,v 1.2 2005/11/16 23:01:57 fwinkl Exp $
#
# defines ExclEtaPPath tag filter path
#

sourceFoundFile EventTagTools/TagProcs.tcl

path create ExclEtaPPath

# only run on MultiHadrons
path append ExclEtaPPath FilterBGFMultiHadron

# create tag bits
sequence create TagExclEtaPSequence
addTagModule TagEtapOtherMeson TagExclEtaPSequence {
	trackCandidates set "GoodTracksLoose"
	rho0Candidates  set "rho0Default"
	rhocCandidates  set "rhoCDefault"
	k0sCandidates   set "KsDefault"
	etaggCandidates set "etaggDefault"
	eta3piCandidates set "eta3piDefault"
	omegaCandidates  set "omegaDefault"
	kstar0Candidates set "KstarKPiLoose"
	kstarkspiCandidates set "KstarKsPiLoose"
	kstarkpi0Candidates set "KstarKPi0Loose"
	kstarkspi0Candidates set "KstarKsPi0Loose"
	etaptorho0gamCandidates set "etaPrgDefault"
	etaptoetapipiCandidates set "etaPeppDefault"
	pi0Candidates set "pi0AllLoose"
	phiCandidates set "phiDefault"
	minCmsP set 1.9
	maxCmsP set 3.1
	MBcut set 5.15
	pgcut set 0.05
	verbose set false
}
path append ExclEtaPPath TagExclEtaPSequence

path append ExclEtaPPath ExclEtaPFilter
