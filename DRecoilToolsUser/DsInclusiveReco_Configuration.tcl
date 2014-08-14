
#-------------------------------------
#create and append tag modules
#-------------------------------------
# sourceFoundFile DRecoilToolsUser/DReco_GenericTag_SkimConfig.tcl
# path append Everything DRecoTagRecoSequence 

##----------------
##create this analysis sequence
##----------------
sequence create DRecoDsSequence


####################################
##Veto lists
####################################
mod clone SmpSubListerDefiner SignalGamma
seq append DRecoDsSequence SignalGamma
catch { setProduction SignalGamma }
talkto SignalGamma {
    unrefinedListName  set "GoodPhotonLoose"
    #120 MeV is placed in CleanTag Skim
    selectors   set "LabE 0.12:10.0"
    selectors   set "Lat 0.01:0.6"
}

# pi0 veto list
mod clone SmpMakerDefiner DRecoPi0VetoList
seq append DRecoDsSequence DRecoPi0VetoList
catch { setProduction DRecoPi0VetoList }
talkto DRecoPi0VetoList {
  decayMode          set "pi0 -> gamma gamma"
  daughterListNames  set "SignalGamma"
  daughterListNames  set "SignalGamma"
  fittingAlgorithm   set "TreeFitter"
  fitConstraints     set "Mass"
  fitConstraints     set "PrimaryVertex"
  fitConstraints     set "Beam"
  preFitSelectors    set "Mass 0.100:0.165"
  postFitSelectors   set "ProbChiSq 0.01:"
 
}                                                                                                                                      
# eta veto list
mod clone SmpMakerDefiner DRecoEtaVetoList
seq append DRecoDsSequence DRecoEtaVetoList
catch { setProduction DRecoEtaVetoList }
talkto DRecoEtaVetoList {
  decayMode          set "eta -> gamma gamma"
  daughterListNames  set "SignalGamma"
  daughterListNames  set "SignalGamma"
  fittingAlgorithm   set "TreeFitter"
  fitConstraints     set "Mass"
  fitConstraints     set "PrimaryVertex"
  fitConstraints     set "Beam"
  preFitSelectors    set "Mass .480:.610"
  postFitSelectors   set "ProbChiSq 0.01:"
 
}                                  


########################
##Grab the persisted list of tags from the skim 
########################
module talk DRecoTagInterface
  TagList1 set DRecoTagList
  TagListFit1 set false
  TagListMass1 set 1.864
  TagListMassCut1 set 10.
  TagListPstarCut1 set 0.
  TagListProbminCut1 set 0.
exit
seq append DRecoDsSequence DRecoTagInterface

mod clone DRecoCounter CounterTagInterface
module talk CounterTagInterface
  counterNumber set 10
exit
seq append DRecoDsSequence CounterTagInterface


##--------------------------------
##Cofigure Dstar tagging
##--------------------------------
module talk DRecoDstarTag
  PiList set GoodTracksVeryLoose
  Pi0List set pi0DefaultMass
  GammaList set GoodPhotonLoose
  GammaPi0VetoList set DRecoPi0VetoList
  GammaEtaVetoList set DRecoEtaVetoList
  VtxProbCut set .00001
exit
seq append DRecoDsSequence DRecoDstarTag

mod clone DRecoCounter CounterDstarTag
module talk CounterDstarTag
  counterNumber set 20
exit
seq append DRecoDsSequence CounterDstarTag

##--------------------------------
##Cofigure Tag Kaon
##--------------------------------
mod clone SmpMakerDefiner DRecoFlavorKShortList
seq append DRecoDsSequence DRecoFlavorKShortList
catch { setProduction DRecoFlavorKShortList }
talkto DRecoFlavorKShortList {
  decayMode         set "K_S0 -> pi+ pi-"
  daughterListNames set "ChargedTracks"
  daughterListNames set "ChargedTracks"
  preFitSelectors   set "Mass 0.3:0.7"
  fittingAlgorithm  set "TreeFitter"
  fitConstraints    set "Mass"
  postFitSelectors  set "ProbChiSq 0.001:"
  postFitSelectors  set "FlightSignificance 5.:"
  postFitSelectors  set "Flight 0.:"
}
module talk DRecoTagKaon
  KList set KBDTKaonVeryLoose
  K0List set DRecoFlavorKShortList
  ProtonList set pKMVeryLoose
exit
seq append DRecoDsSequence DRecoTagKaon

mod clone DRecoCounter CounterTagKaon
module talk CounterTagKaon
  counterNumber set 30
exit
seq append DRecoDsSequence CounterTagKaon


##--------------------------------
##Cofigure X
##--------------------------------
module talk DRecoFragPions
    PiList set GoodTracksVeryLoose
    PiPCutLo set .1
    PiPCutHi set 2.
    PiMaxNumber set 3

    #pi0 list must not be mass constrained so one can cut on the mass
    Pi0List set pi0TightMassNoMomentumCut
    Pi0MassCutLo set .1213
    Pi0MassCutHi set .1453
    Pi0MassConstrain set 1
    Pi0GamEnergyCutLo set .1
    Pi0GamEnergyCutHi set 3.
    Pi0MaxNumber set 1

    #########################
    ### no gammas allowed in X
    #########################
#    GammaList set GoodPhotonLoose
#     GammaEnergyCutLo set .15
#     GammaEnergyCutHi set 5.
#     GammaPi0VetoList set 
#     GammaEtaVetoList set 
    GammaMaxNumber set 0


    #wrong-sign needs up to |charge|=3
    XChargeCut set 3.
exit
seq append DRecoDsSequence DRecoFragPions

mod clone DRecoCounter CounterFragPions
module talk CounterFragPions
    counterNumber set 40
exit
seq append DRecoDsSequence CounterFragPions

##--------------------------------
## Configure Signal gamma
##--------------------------------                                                                                                    
module talk DRecoDsStGam
  GamList set SignalGamma
  GamEnergyCutLo set .12
  GamEnergyCutHi set 2.
  GamAngleCut set .0
  Pi0VetoList set DRecoPi0VetoList
  EtaVetoList set DRecoEtaVetoList

  GamLabAngleCut set .94
  GamLatCutLo set .01
  GamLatCutHi set .6
  GamBumpDistCut set 15
  GamTrkDistCut set 15
exit
seq append DRecoDsSequence DRecoDsStGam

mod clone DRecoCounter CounterDsStGam
module talk CounterDsStGam
    counterNumber set 50
exit
seq append DRecoDsSequence CounterDsStGam

##--------------------------------
## Configure RS - WS selection
##--------------------------------
module talk DRecoCheckCharge
  SelectWrongSign set 0
exit
seq append DRecoDsSequence DRecoCheckCharge

mod clone DRecoCounter CounterCheckCharge
module talk CounterCheckCharge
  counterNumber set 60
exit
seq append DRecoDsSequence CounterCheckCharge

##--------------------------------
## Configure Ds event fit
##--------------------------------
module talk DRecoDsInclusiveFit
 DsStMassCutLo set 1.912
 DsStMassCutHi set 2.312
 FitProbCut set .00001
 DsMomentumCut set 3.
 MassConstrainDsSt set 1
exit 
seq append DRecoDsSequence DRecoDsInclusiveFit

mod clone DRecoCounter CounterDsInclusiveFit
module talk CounterDsInclusiveFit
    counterNumber set 70
exit
seq append DRecoDsSequence CounterDsInclusiveFit

#--------------------------------------------------
#append clean up module otherwise memory leaks
#--------------------------------------------------
#seq append DRecoDsSequence DRecoPrintTree
seq append DRecoDsSequence DRecoCleanUp

