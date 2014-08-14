

##----------------------
#create raw tag modes
#-----------------------
sourceFoundFile DRecoilToolsUser/DReco_DaughterLists.tcl
sourceFoundFile DRecoilToolsUser/DRecoD0Tags.tcl
sourceFoundFile DRecoilToolsUser/DRecoDPlusTags.tcl
sourceFoundFile DRecoilToolsUser/DRecoDsTags.tcl
sourceFoundFile DRecoilToolsUser/DRecoLambdaCTags.tcl

path append Everything DRecoTagSequence


##-----------------------------------
#Refine daughter lists and tag modes
##-----------------------------------
#Note: The following variables were used in the tag optimization:
#mass, p*, vtx. prob., pid, pi0gammaenergy.
#-vtx prob did not help much it is kept at .1% here
#-pi0 gamma energy was not used in tightening this skim
#For Kshort the flight significance has been tightened to 10, and two other variables have been used 
#  -angle between Ks p3 and flight direction from mother vtx. (cos>0)
#  -angle between Ks p3 and fligth direction from BeamSpot (cos>0)
##------------------------------------

seq create DRecoRefinedSequence

###############
#Refine Ks list to cut on flight cos-BeamSpot and flight significance
###############
mod clone SmpSubListerDefiner DRecoRefinedKShortMassList
seq append DRecoRefinedSequence DRecoRefinedKShortMassList
catch { setProduction DRecoRefinedKShortMassList }
talkto DRecoRefinedKShortMassList {
  unrefinedListName set "DRecoKShortMassList"
  selectors         set "FlightSignificance 10:"
  selectors         set "Flight 0.:"
}

###############
#Refine Lambda0 list
###############
mod clone SmpSubListerDefiner DRecoRefinedLambda0SelectList
seq append DRecoRefinedSequence DRecoRefinedLambda0SelectList
catch { setProduction DRecoRefinedLambda0SelectList }
talkto DRecoRefinedLambda0SelectList {
  decayMode          set "Lambda0 -> p+ pi-"
  unrefinedListName  set "DRecoLambda0SelectList"  
  daughterListNames set "pLHLoose"
  daughterListNames set "DRecoPions"
}

######################################################################

###########################
###Teadeous pi0/eta  lists
###########################

#First, which modes are optimized using pi0/eta gamma energy
#2 D0ToKPiPi0
#6 D0ToKShortPiPiPi0
#12 DPlusToKPiPiPi0
#14 DPlusToKShortPiPi0
#23 DsToPhiRhoC
#25 DsToEtaPPi
#32 LambdaCTopKpipi0
#36 LambdaCToLzpipi0

#Second, define photon lists based on minimum energy cut for each mode
#D0ToKPiPi0 
mod clone SmpSubListerDefiner DRecoRefinedPhotonList_D0ToKPiPi0
seq append DRecoRefinedSequence DRecoRefinedPhotonList_D0ToKPiPi0
catch { setProduction DRecoRefinedPhotonList_D0ToKPiPi0 }
talkto DRecoRefinedPhotonList_D0ToKPiPi0 {
  unrefinedListName set "DRecoPhotonList"
  selectors         set "LabE 0.110:10.0"
}
#D0ToKsPiPiPi0
mod clone SmpSubListerDefiner DRecoRefinedPhotonList_D0ToKsPiPiPi0
seq append DRecoRefinedSequence DRecoRefinedPhotonList_D0ToKsPiPiPi0
catch { setProduction DRecoRefinedPhotonList_D0ToKsPiPiPi0 }
talkto DRecoRefinedPhotonList_D0ToKsPiPiPi0 {
  unrefinedListName set "DRecoPhotonList"
  selectors         set "LabE 0.110:10.0"
}
#DPlusToKPiPiPi0
mod clone SmpSubListerDefiner DRecoRefinedPhotonList_DPlusToKPiPiPi0
seq append DRecoRefinedSequence DRecoRefinedPhotonList_DPlusToKPiPiPi0
catch { setProduction DRecoRefinedPhotonList_DPlusToKPiPiPi0 }
talkto DRecoRefinedPhotonList_DPlusToKPiPiPi0 {
  unrefinedListName set "DRecoPhotonList"
  selectors         set "LabE 0.120:10.0"
}
#DPlusToKsPiPi0
mod clone SmpSubListerDefiner DRecoRefinedPhotonList_DPlusToKsPiPi0
seq append DRecoRefinedSequence DRecoRefinedPhotonList_DPlusToKsPiPi0
catch { setProduction DRecoRefinedPhotonList_DPlusToKsPiPi0 }
talkto DRecoRefinedPhotonList_DPlusToKsPiPi0 {
  unrefinedListName set "DRecoPhotonList"
  selectors         set "LabE 0.120:10.0"
}
#DsToPhiRhoC
mod clone SmpSubListerDefiner DRecoRefinedPhotonList_DsToPhiRhoC
seq append DRecoRefinedSequence DRecoRefinedPhotonList_DsToPhiRhoC
catch { setProduction DRecoRefinedPhotonList_DsToPhiRhoC }
talkto DRecoRefinedPhotonList_DsToPhiRhoC {
  unrefinedListName set "DRecoPhotonList"
  selectors         set "LabE 0.100:10.0"
}
#DsToEtaPPi
mod clone SmpSubListerDefiner DRecoRefinedPhotonList_DsToEtaPPi
seq append DRecoRefinedSequence DRecoRefinedPhotonList_DsToEtaPPi
catch { setProduction DRecoRefinedPhotonList_DsToEtaPPi }
talkto DRecoRefinedPhotonList_DsToEtaPPi {
  unrefinedListName set "DRecoPhotonList"
  selectors         set "LabE 0.100:10.0"
}
#LambdaCTopKpipi0
mod clone SmpSubListerDefiner DRecoRefinedPhotonList_LambdaCTopKpipi0
seq append DRecoRefinedSequence DRecoRefinedPhotonList_LambdaCTopKpipi0
catch { setProduction DRecoRefinedPhotonList_LambdaCTopKpipi0 }
talkto DRecoRefinedPhotonList_LambdaCTopKpipi0 {
  unrefinedListName set "DRecoPhotonList"
  selectors         set "LabE 0.100:10.0"
}
#LambdaCToLzpipi0
mod clone SmpSubListerDefiner DRecoRefinedPhotonList_LambdaCToLzpipi0
seq append DRecoRefinedSequence DRecoRefinedPhotonList_LambdaCToLzpipi0
catch { setProduction DRecoRefinedPhotonList_LambdaCToLzpipi0 }
talkto DRecoRefinedPhotonList_LambdaCToLzpipi0 {
  unrefinedListName set "DRecoPhotonList"
  selectors         set "LabE 0.130:10.0"
}

#Third make refined pi0 lists for each mode
#D0ToKPiPi0
mod clone SmpSubListerDefiner DRecoRefinedPi0List_D0ToKPiPi0
seq append DRecoRefinedSequence DRecoRefinedPi0List_D0ToKPiPi0
catch { setProduction DRecoRefinedPi0List_D0ToKPiPi0 }
talkto DRecoRefinedPi0List_D0ToKPiPi0 {
  decayMode          set "pi0 -> gamma gamma"
  unrefinedListName  set "DRecoPi0SelectList"
  daughterListNames  set "DRecoRefinedPhotonList_D0ToKPiPi0"
  daughterListNames  set "DRecoRefinedPhotonList_D0ToKPiPi0"
}
#D0ToKsPiPiPi0
mod clone SmpSubListerDefiner DRecoRefinedPi0List_D0ToKsPiPiPi0
seq append DRecoRefinedSequence DRecoRefinedPi0List_D0ToKsPiPiPi0
catch { setProduction DRecoRefinedPi0List_D0ToKsPiPiPi0 }
talkto DRecoRefinedPi0List_D0ToKsPiPiPi0 {
  decayMode          set "pi0 -> gamma gamma"
  unrefinedListName  set "DRecoPi0SelectList"
  daughterListNames  set "DRecoRefinedPhotonList_D0ToKsPiPiPi0"
  daughterListNames  set "DRecoRefinedPhotonList_D0ToKsPiPiPi0"
}
#DPlusToKPiPiPi0
mod clone SmpSubListerDefiner DRecoRefinedPi0List_DPlusToKPiPiPi0
seq append DRecoRefinedSequence DRecoRefinedPi0List_DPlusToKPiPiPi0
catch { setProduction DRecoRefinedPi0List_DPlusToKPiPiPi0 }
talkto DRecoRefinedPi0List_DPlusToKPiPiPi0 {
  decayMode          set "pi0 -> gamma gamma"
  unrefinedListName  set "DRecoPi0SelectList"
  daughterListNames  set "DRecoRefinedPhotonList_DPlusToKPiPiPi0"
  daughterListNames  set "DRecoRefinedPhotonList_DPlusToKPiPiPi0"
}
#DPlusToKsPiPi0
mod clone SmpSubListerDefiner DRecoRefinedPi0List_DPlusToKsPiPi0
seq append DRecoRefinedSequence DRecoRefinedPi0List_DPlusToKsPiPi0
catch { setProduction DRecoRefinedPi0List_DPlusToKsPiPi0 }
talkto DRecoRefinedPi0List_DPlusToKsPiPi0 {
  decayMode          set "pi0 -> gamma gamma"
  unrefinedListName  set "DRecoPi0SelectList"
  daughterListNames  set "DRecoRefinedPhotonList_DPlusToKsPiPi0"
  daughterListNames  set "DRecoRefinedPhotonList_DPlusToKsPiPi0"
}
#DsToPhiRhoC
mod clone SmpSubListerDefiner DRecoRefinedPi0List_DsToPhiRhoC
seq append DRecoRefinedSequence DRecoRefinedPi0List_DsToPhiRhoC
catch { setProduction DRecoRefinedPi0List_DsToPhiRhoC }
talkto DRecoRefinedPi0List_DsToPhiRhoC {
  decayMode          set "pi0 -> gamma gamma"
  unrefinedListName  set "DRecoPi0SelectList"
  daughterListNames  set "DRecoRefinedPhotonList_DsToPhiRhoC"
  daughterListNames  set "DRecoRefinedPhotonList_DsToPhiRhoC"
}
#DsToEtaPPi
mod clone SmpSubListerDefiner DRecoRefinedEtaList_DsToEtaPPi
seq append DRecoRefinedSequence DRecoRefinedEtaList_DsToEtaPPi
catch { setProduction DRecoRefinedEtaList_DsToEtaPPi }
talkto DRecoRefinedEtaList_DsToEtaPPi {
  decayMode          set "eta -> gamma gamma"
  unrefinedListName  set "DRecoEtaSelectList"
  daughterListNames  set "DRecoRefinedPhotonList_DsToEtaPPi"
  daughterListNames  set "DRecoRefinedPhotonList_DsToEtaPPi"
}
#LambdaCTopKpipi0
mod clone SmpSubListerDefiner DRecoRefinedPi0List_LambdaCTopKpipi0
seq append DRecoRefinedSequence DRecoRefinedPi0List_LambdaCTopKpipi0
catch { setProduction DRecoRefinedPi0List_LambdaCTopKpipi0 }
talkto DRecoRefinedPi0List_LambdaCTopKpipi0 {
  decayMode          set "pi0 -> gamma gamma"
  unrefinedListName  set "DRecoPi0SelectList"
  daughterListNames  set "DRecoRefinedPhotonList_LambdaCTopKpipi0"
  daughterListNames  set "DRecoRefinedPhotonList_LambdaCTopKpipi0"
}
#
mod clone SmpSubListerDefiner DRecoRefinedPi0List_LambdaCToLzpipi0
seq append DRecoRefinedSequence DRecoRefinedPi0List_LambdaCToLzpipi0
catch { setProduction DRecoRefinedPi0List_LambdaCToLzpipi0 }
talkto DRecoRefinedPi0List_LambdaCToLzpipi0 {
  decayMode          set "pi0 -> gamma gamma"
  unrefinedListName  set "DRecoPi0SelectList"
  daughterListNames  set "DRecoRefinedPhotonList_LambdaCToLzpipi0"
  daughterListNames  set "DRecoRefinedPhotonList_LambdaCToLzpipi0"
}
######################################################################

##################Refine the D0#################################
mod clone SmpSubListerDefiner DRecoRefinedD0ToKPi
seq append DRecoRefinedSequence DRecoRefinedD0ToKPi
catch { setProduction DRecoRefinedD0ToKPi }
talkto DRecoRefinedD0ToKPi {
  decayMode          set "D0 -> K- pi+"
  unrefinedListName  set "DRecoD0ToKPi"
  daughterListNames  set "KBDTKaonVeryLoose"
  daughterListNames  set "piKMSuperLoose"
}

mod clone SmpSubListerDefiner DRecoRefinedD0ToKPiPi0
seq append DRecoRefinedSequence DRecoRefinedD0ToKPiPi0
catch { setProduction DRecoRefinedD0ToKPiPi0 }
talkto DRecoRefinedD0ToKPiPi0 {
  decayMode          set "D0 -> K- pi+ pi0"
  unrefinedListName  set "DRecoD0ToKPiPi0"
  daughterListNames  set "KBDTKaonLoose"
  daughterListNames  set "piKMLoose"
  daughterListNames  set "DRecoRefinedPi0List_D0ToKPiPi0"
}


mod clone SmpSubListerDefiner DRecoRefinedD0ToK3Pi
seq append DRecoRefinedSequence DRecoRefinedD0ToK3Pi
catch { setProduction DRecoRefinedD0ToK3Pi }
talkto DRecoRefinedD0ToK3Pi {
  decayMode          set "D0 -> K- pi+ pi- pi+"
  unrefinedListName  set "DRecoD0ToK3Pi"
  daughterListNames  set "KBDTKaonLoose"
  daughterListNames  set "piKMSuperLoose"
  daughterListNames  set "piKMSuperLoose"
  daughterListNames  set "piKMSuperLoose"
}

mod clone SmpSubListerDefiner DRecoRefinedD0ToKsPiPi
seq append DRecoRefinedSequence DRecoRefinedD0ToKsPiPi
catch { setProduction DRecoRefinedD0ToKsPiPi }
talkto DRecoRefinedD0ToKsPiPi {
  decayMode          set "D0 -> K_S0 pi+ pi-"
  unrefinedListName  set "DRecoD0ToKsPiPi"
  daughterListNames  set "DRecoRefinedKShortMassList"
  daughterListNames  set "piKMSuperLoose"
  daughterListNames  set "piKMSuperLoose"
}

mod clone SmpSubListerDefiner DRecoRefinedD0ToK3PiPi0
seq append DRecoRefinedSequence DRecoRefinedD0ToK3PiPi0
catch { setProduction DRecoRefinedD0ToK3PiPi0 }
talkto DRecoRefinedD0ToK3PiPi0 {
  decayMode          set "D0 -> K- pi+ pi- pi+ pi0"
  unrefinedListName  set "DRecoD0ToK3PiPi0"
  daughterListNames  set "KBDTKaonVeryTight"
  daughterListNames  set "DRecoPions"
  daughterListNames  set "DRecoPions"
  daughterListNames  set "DRecoPions"
  daughterListNames  set "DRecoPi0SelectList"
}

mod clone SmpSubListerDefiner DRecoRefinedD0ToKsPiPiPi0
seq append DRecoRefinedSequence DRecoRefinedD0ToKsPiPiPi0
catch { setProduction DRecoRefinedD0ToKsPiPiPi0 }
talkto DRecoRefinedD0ToKsPiPiPi0 {
  decayMode          set "D0 -> K_S0 pi+ pi- pi0"
  unrefinedListName  set "DRecoD0ToKsPiPiPi0"
  daughterListNames  set "DRecoRefinedKShortMassList"
  daughterListNames  set "piKMVeryLoose"
  daughterListNames  set "piKMVeryLoose"
  daughterListNames  set "DRecoRefinedPi0List_D0ToKsPiPiPi0"
}


####################Refine the D+###########################
mod clone SmpSubListerDefiner DRecoRefinedDPlusToKPiPi
seq append DRecoRefinedSequence DRecoRefinedDPlusToKPiPi
catch { setProduction DRecoRefinedDPlusToKPiPi }
talkto DRecoRefinedDPlusToKPiPi {
  decayMode          set "D+ -> K- pi+ pi+"
  unrefinedListName  set "DRecoDPlusToKPiPi"
  daughterListNames  set "KBDTKaonLoose"
  daughterListNames  set "piKMVeryLoose"
  daughterListNames  set "piKMVeryLoose"
}

mod clone SmpSubListerDefiner DRecoRefinedDPlusToKPiPiPi0
seq append DRecoRefinedSequence DRecoRefinedDPlusToKPiPiPi0
catch { setProduction DRecoRefinedDPlusToKPiPiPi0 }
talkto DRecoRefinedDPlusToKPiPiPi0 {
  decayMode          set "D+ -> K- pi+ pi+ pi0"
  unrefinedListName  set "DRecoDPlusToKPiPiPi0"
  daughterListNames  set "KBDTKaonTight"
  daughterListNames  set "piKMLoose"
  daughterListNames  set "piKMLoose"
  daughterListNames  set "DRecoRefinedPi0List_DPlusToKPiPiPi0"
}

mod clone SmpSubListerDefiner DRecoRefinedDPlusToKsPi
seq append DRecoRefinedSequence DRecoRefinedDPlusToKsPi
catch { setProduction DRecoRefinedDPlusToKsPi }
talkto DRecoRefinedDPlusToKsPi {
  decayMode          set "D+ -> K_S0 pi+"
  unrefinedListName  set "DRecoDPlusToKsPi"
  daughterListNames  set "DRecoRefinedKShortMassList"
  daughterListNames  set "piKMSuperLoose"
}

mod clone SmpSubListerDefiner DRecoRefinedDPlusToKsPiPi0
seq append DRecoRefinedSequence DRecoRefinedDPlusToKsPiPi0
catch { setProduction DRecoRefinedDPlusToKsPiPi0 }
talkto DRecoRefinedDPlusToKsPiPi0 {
  decayMode          set "D+ -> K_S0 pi+ pi0"
  unrefinedListName  set "DRecoDPlusToKsPiPi0"
  daughterListNames  set "DRecoRefinedKShortMassList"
  daughterListNames  set "piKMVeryLoose"
  daughterListNames  set "DRecoRefinedPi0List_DPlusToKsPiPi0"
}

mod clone SmpSubListerDefiner DRecoRefinedDPlusToKs3Pi
seq append DRecoRefinedSequence DRecoRefinedDPlusToKs3Pi
catch { setProduction DRecoRefinedDPlusToKs3Pi }
talkto DRecoRefinedDPlusToKs3Pi {
  decayMode          set "D+ -> K_S0 pi+ pi+ pi-"
  unrefinedListName  set "DRecoDPlusToKs3Pi"
  daughterListNames  set "DRecoRefinedKShortMassList"
  daughterListNames  set "piKMLoose"
  daughterListNames  set "piKMLoose"
  daughterListNames  set "piKMLoose"
}



###############################
###Ds decay modes
#############################

mod clone SmpSubListerDefiner DRecoRefinedDsToKsK
seq append DRecoRefinedSequence DRecoRefinedDsToKsK
catch { setProduction DRecoRefinedDsToKsK }
talkto DRecoRefinedDsToKsK {
  decayMode         set "D_s+ -> K_S0 K+"
  unrefinedListName  set "DRecoDsToKsK"
  daughterListNames  set "DRecoRefinedKShortMassList"
  daughterListNames  set "KBDTKaonVeryLoose"
}

mod clone SmpSubListerDefiner DRecoRefinedDsToPhiPi
seq append DRecoRefinedSequence DRecoRefinedDsToPhiPi
catch { setProduction DRecoRefinedDsToPhiPi }
talkto DRecoRefinedDsToPhiPi {
  decayMode         set "D_s+ -> phi pi+"
  unrefinedListName  set "DRecoDsToPhiPi"
  daughterListNames set "DRecoPhiMassList" 
  daughterListNames set "piKMSuperLoose"
}

mod clone SmpSubListerDefiner DRecoRefinedDsToPhiRhoC
seq append DRecoRefinedSequence DRecoRefinedDsToPhiRhoC
catch { setProduction DRecoRefinedDsToPhiRhoC }
talkto DRecoRefinedDsToPhiRhoC {
  decayMode         set "D_s+ -> phi rho+"
  unrefinedListName  set "DRecoDsToPhiRhoC"
  #These daughter lists are the same as in the unrefined case.
  daughterListNames  set "DRecoPhiListOldPID"
  daughterListNames set "DRecoRhoCList"
#  daughterListNames  set "<new rhoC list here BUT pi0 optimization returned 0.1>"
#  ...and so no need to do anything since 0.1 was the minimum cut anyway
}

mod clone SmpSubListerDefiner DRecoRefinedDsToKsKPiPifav
seq append DRecoRefinedSequence DRecoRefinedDsToKsKPiPifav
catch { setProduction DRecoRefinedDsToKsKPiPifav }
talkto DRecoRefinedDsToKsKPiPifav {
  decayMode         set "D_s+ -> K_S0 K- pi+ pi+"
  unrefinedListName  set "DRecoDsToKsKPiPifav"
  daughterListNames  set "DRecoRefinedKShortMassList"
  daughterListNames  set "KBDTKaonTight"
  daughterListNames  set "piKMSuperLoose"
  daughterListNames  set "piKMSuperLoose"
}
mod clone SmpSubListerDefiner DRecoRefinedDsToKsKPiPisup
seq append DRecoRefinedSequence DRecoRefinedDsToKsKPiPisup
catch { setProduction DRecoRefinedDsToKsKPiPisup }
talkto DRecoRefinedDsToKsKPiPisup {
  decayMode         set "D_s+ -> K_S0 K+ pi+ pi-"
  unrefinedListName  set "DRecoDsToKsKPiPisup"
  daughterListNames  set "DRecoRefinedKShortMassList"
  daughterListNames  set "KBDTKaonTight"
  daughterListNames  set "piKMSuperLoose"
  daughterListNames  set "piKMSuperLoose"
}

mod clone SmpMergerDefiner DRecoRefinedDsToKsKPiPi
seq append DRecoRefinedSequence DRecoRefinedDsToKsKPiPi
catch { setProduction DRecoRefinedDsToKsKPiPi }
talkto DRecoRefinedDsToKsKPiPi {
    ## disableCloneCheck=true is necessary to keep both the
    ## right-sign and wrong-sign combinations
    disableCloneCheck set true
    inputListNames    set "DRecoRefinedDsToKsKPiPifav"
    inputListNames    set "DRecoRefinedDsToKsKPiPisup"
}

mod clone SmpSubListerDefiner DRecoRefinedDsToEtaPPi
seq append DRecoRefinedSequence DRecoRefinedDsToEtaPPi
catch { setProduction DRecoRefinedDsToEtaPPi }
talkto DRecoRefinedDsToEtaPPi {
  decayMode         set "D_s+ -> eta' pi+"
  unrefinedListName  set "DRecoDsToEtaPPi"
  daughterListNames set "DRecoEtaPSelectList"
  daughterListNames set "piKMVeryLoose"
}

mod clone SmpSubListerDefiner DRecoRefinedDsToPhiRho0pi
seq append DRecoRefinedSequence DRecoRefinedDsToPhiRho0pi
catch { setProduction DRecoRefinedDsToPhiRho0pi }
talkto DRecoRefinedDsToPhiRho0pi {
    decayMode         set "D_s+ -> phi rho0 pi+"
    unrefinedListName  set "DRecoDsToPhiRho0Pi"
    daughterListNames set "DRecoPhiListOldPID"
    daughterListNames set "DRecoRho0SelectList"
    daughterListNames set "piKMVeryTight"
}

mod clone SmpSubListerDefiner DRecoRefinedDsToKKstar0
seq append DRecoRefinedSequence DRecoRefinedDsToKKstar0
catch { setProduction DRecoRefinedDsToKKstar0 }
talkto DRecoRefinedDsToKKstar0 {
    decayMode         set "D_s+ -> K*0 K+"
    unrefinedListName  set "DRecoDsToKstar0K"
    daughterListNames set "DRecoKstar0List" 
    daughterListNames set "KBDTKaonVeryLoose"
}


#############################
###Lambda_c decay modes
#############################

mod clone SmpSubListerDefiner DRecoRefinedLambdaCTopKpi
seq append DRecoRefinedSequence DRecoRefinedLambdaCTopKpi
catch { setProduction DRecoRefinedLambdaCTopKpi }
talkto DRecoRefinedLambdaCTopKpi {
  decayMode         set "Lambda_c+ -> p+ K- pi+"
  unrefinedListName  set "DRecoLambdaCTopKpi"
  daughterListNames  set "pKMTight"
  daughterListNames  set "KBDTKaonLoose"
  daughterListNames  set "piKMSuperLoose"
}


mod clone SmpSubListerDefiner DRecoRefinedLambdaCTopKpipi0
seq append DRecoRefinedSequence DRecoRefinedLambdaCTopKpipi0
catch { setProduction DRecoRefinedLambdaCTopKpipi0 }
talkto DRecoRefinedLambdaCTopKpipi0 {
  decayMode         set "Lambda_c+ -> p+ K- pi+ pi0"
  unrefinedListName  set "DRecoLambdaCTopKpipi0"
  daughterListNames  set "pKMVeryTight"
  daughterListNames  set "KBDTKaonTight"
  daughterListNames  set "piKMSuperLoose"
  daughterListNames  set "DRecoRefinedPi0List_LambdaCTopKpipi0"
}


mod clone SmpSubListerDefiner DRecoRefinedLambdaCTopKs
seq append DRecoRefinedSequence DRecoRefinedLambdaCTopKs
catch { setProduction DRecoRefinedLambdaCTopKs }
talkto DRecoRefinedLambdaCTopKs {
  decayMode         set "Lambda_c+ -> p+ K_S0"
  unrefinedListName  set "DRecoLambdaCTopKs"
  daughterListNames  set "pKMLoose"
  daughterListNames  set "DRecoRefinedKShortMassList"
}


mod clone SmpSubListerDefiner DRecoRefinedLambdaCTopKspipi
seq append DRecoRefinedSequence DRecoRefinedLambdaCTopKspipi
catch { setProduction DRecoRefinedLambdaCTopKspipi }
talkto DRecoRefinedLambdaCTopKspipi {
  decayMode         set "Lambda_c+ -> p+ K_S0 pi+ pi-"
  unrefinedListName  set "DRecoLambdaCTopKspipi"
  daughterListNames  set "pKMTight"
  daughterListNames  set "DRecoRefinedKShortMassList"
  daughterListNames  set "piKMSuperLoose"
  daughterListNames  set "piKMSuperLoose"
}

mod clone SmpSubListerDefiner DRecoRefinedLambdaCToLzpi
seq append DRecoRefinedSequence DRecoRefinedLambdaCToLzpi
catch { setProduction DRecoRefinedLambdaCToLzpi }
talkto DRecoRefinedLambdaCToLzpi {
  decayMode         set "Lambda_c+ -> Lambda0 pi+"
  unrefinedListName  set "DRecoLambdaCToLzpi"
  daughterListNames  set "DRecoRefinedLambda0SelectList"
  daughterListNames  set "piKMSuperLoose"
}

mod clone SmpSubListerDefiner DRecoRefinedLambdaCToLzpipi0
seq append DRecoRefinedSequence DRecoRefinedLambdaCToLzpipi0
catch { setProduction DRecoRefinedLambdaCToLzpipi0 }
talkto DRecoRefinedLambdaCToLzpipi0 {
  decayMode         set "Lambda_c+ -> Lambda0 pi+ pi0"
  unrefinedListName  set "DRecoLambdaCToLzpipi0"
  daughterListNames  set "DRecoLambda0SelectList"
  daughterListNames  set "piKMLoose"
  daughterListNames  set "DRecoRefinedPi0List_LambdaCToLzpipi0"
}


mod clone SmpSubListerDefiner DRecoRefinedLambdaCToLzpipipi
seq append DRecoRefinedSequence DRecoRefinedLambdaCToLzpipipi
catch { setProduction DRecoRefinedLambdaCToLzpipipi }
talkto DRecoRefinedLambdaCToLzpipipi {
  decayMode         set "Lambda_c+ -> Lambda0 pi+ pi+ pi-"
  unrefinedListName  set "DRecoLambdaCToLzpipipi"
  daughterListNames set "DRecoLambda0SelectList"
  daughterListNames set "piKMVeryLoose"
  daughterListNames set "piKMVeryLoose"
  daughterListNames set "piKMVeryLoose"
}

mod clone SmpSubListerDefiner DRecoRefinedLambdaCToSzpi
seq append DRecoRefinedSequence DRecoRefinedLambdaCToSzpi
catch { setProduction DRecoRefinedLambdaCToSzpi}
talkto DRecoRefinedLambdaCToSzpi {
  decayMode         set "Lambda_c+ -> Sigma0 pi+"
  unrefinedListName  set "DRecoLambdaCToSzpi"
  daughterListNames set "DRecoSigma0List"
  daughterListNames set "piKMTight"
}







