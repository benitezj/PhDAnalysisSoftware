# $Id: BToXsllSequence.tcl,v 1.1 2007/07/27 23:11:28 cschill Exp $

#########################################################################
# list definitions for exclusive B->X gamma decays
#########################################################################

global BToXsllSequenceDefinition

if [ expr ! [info exists BToXsllSequenceDefinition ] ] {
    set BToXsllSequenceDefinition 1
    
    sequence create BToXsllSequence
    
    #Refine PidLHElectrons before Brem-Reco
    mod clone SmpSubListerDefiner XsllPidLHEList
    seq append BToXsllSequence XsllPidLHEList
    catch { setProduction XsllPidLHEList }
    talkto XsllPidLHEList {
	unrefinedListName set "PidLHElectrons"
	selectors         set "LabP 0.3:"
    }
    
    #Brem Reco
    mod clone eBremRecoPidHll XslleBremRecoPidHll
    seq append BToXsllSequence XslleBremRecoPidHll
    talkto XslleBremRecoPidHll {
	# Run the bremsstrahlung recovery module to combine gammas with 
	# electrons. Place combined electrons in the "eBremRecoPidHll" list.
	inputList0           set XsllPidLHEList
	outputList           set XslleBremRecoPidHll
	type0                set 0
	inputNeutralList     set GoodPhotonLoose
	typeNeutral          set 5
	# Combine at most one photon with each electron candidate.
	nPhotMax             set 1
	EnergyGammaCutLow    set 0.015
	EnergyGammaCutHigh   set 10.58
	LatGammaCutLow       set 0.0001
	LatGammaCutHigh      set 0.8
	Zerni42GammaCut      set 0.25
	diffThetaGammaCut    set 0.035
	diffPhiGammaCutBisM  set 0.0
	diffPhiGammaCutHorM  set -0.050
	diffPhiGammaCutBisP  set 0.0
	diffPhiGammaCutHorP  set 0.050
	doKin                set false
	doMass               set false
	doVertex             set false
	pChi2Cut             set -0.001
	verbose              set false
	histoFlag            set false
	debugHistograms      set false
	QAHistograms         set false
	histoID              set 35
	#show
    }
    
    #Merged Brem Reco
    mod clone eBremRecoPidHllMerge XslleBremRecoPidHllMerge
    seq append BToXsllSequence XslleBremRecoPidHllMerge
    talkto XslleBremRecoPidHllMerge {
	# Merge the bremsstrahlung-recovered electrons with the 
	# normal electrons into a merged electron list, which we 
	# will use as the input to the main analysis module.
	outputList           set XslleBremRecoPidHllMerge
	nOfInputLists        set 2
	inputList0           set XslleBremRecoPidHll
	addOverlaps0         set true
	addClones0           set false
	inputList1           set XsllPidLHEList
	addOverlaps1         set false
	addClones1           set false
	verbose              set false
	#show
    }
    
    #Refined Muon list
    mod clone SmpSubListerDefiner XsllMuList
    seq append BToXsllSequence XsllMuList
    catch { setProduction XsllMuList }
    talkto XsllMuList {
	unrefinedListName set "muNNVeryLoose"
	selectors         set "LabP 0.5:"
    }
    
    mod clone SmpMakerDefiner XsllGammaConv
    seq append BToXsllSequence XsllGammaConv
    catch { setProduction XsllGammaConv }
    talkto XsllGammaConv {
	decayMode         set "gamma -> e+ e-"
	daughterListNames set "ChargedTracks"
	daughterListNames set "ChargedTracks"
	preFitSelectors   set "Mass :0.15"
	#  preFitSelectors   set "PocaMass :0.1"
	fittingAlgorithm  set "GammaConversion"
	postFitSelectors  set "Mass :0.03"
	postFitSelectors  set "ProbChiSq 0.001:"
    }
    
}