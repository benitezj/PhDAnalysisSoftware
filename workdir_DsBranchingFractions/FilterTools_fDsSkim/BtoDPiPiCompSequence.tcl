#
# $Id: BtoDPiPiCompSequence.tcl,v 1.7 2005/03/22 17:27:27 hulsberg Exp $
#
sequence create BtoDPiPiCompSequence

sourceFoundFile SimpleComposition/SmpCompositionSequence.tcl
sequence append BtoDPiPiCompSequence KsLoose
#sequence append BtoDPiPiCompSequence KsDefault
sequence append BtoDPiPiCompSequence D0ToKPiLoose
sequence append BtoDPiPiCompSequence GammaForPi0
sequence append BtoDPiPiCompSequence pi0VeryLoose
sequence append BtoDPiPiCompSequence pi0Loose

# sourceFoundFile PidTools/PidSequence.tcl
# sequence append BtoDPiPiCompSequence PidSequence

sourceFoundFile CompositionSequences/defineCompAlias.tcl


# talk to the tag module
talkto BtoDPiPiTagFilter {
    BchDsPPList set BchToDstarPiPiDefault
    BchDPPList  set BchToDchPiPiDefault
    B0DPPList   set B0ToD0PiPiDefault
    B0DsPPList  set B0ToDstar0PiPiDefault
}


# good enough for pi0 mode
set looseD0Mass     "Mass 1.8245:1.9045"
set looseD0MassPi0  "Mass 1.7945:1.9345"

set looseDchMass     "Mass 1.75:1.96"

########################## D0 ###############################
# This is necessary since the default D0ToKsPiPiLoose from
# CompTools only makes D0 and not anti-D0 !!!!!!!!
#
# This is no longer necessary, because candidates are
# flagged as CP ambiguous (WH).

mod clone SmpMakerDefiner MyD0ToKsPiPiLoose
seq append BtoDPiPiCompSequence MyD0ToKsPiPiLoose
talkto MyD0ToKsPiPiLoose {
    decayMode         set "D0 -> K_S0 pi+ pi-"
 
    daughterListNames set KsLoose   
#    daughterListNames set KsDefault
    daughterListNames set "GoodTracksVeryLoose"
    daughterListNames set "GoodTracksVeryLoose"

    disableCloneCheck set false
    preFitSelectors   set $looseD0Mass
}

mod clone SmpMakerDefiner MyD0ToKPiPi0Loose
seq append BtoDPiPiCompSequence MyD0ToKPiPi0Loose
talkto MyD0ToKPiPi0Loose {
    decayMode         set "D0 -> K- pi+ pi0"
    
    daughterListNames set KLHNotPion
    daughterListNames set "GoodTracksVeryLoose"
    daughterListNames set pi0Loose

    preFitSelectors set $looseD0MassPi0
}

mod clone SmpMakerDefiner MyD0ToK3PiLoose
seq append BtoDPiPiCompSequence MyD0ToK3PiLoose
talkto MyD0ToK3PiLoose {
    decayMode         set "D0 -> K- pi+ pi+ pi-"
    
    daughterListNames set KLHNotPion
    daughterListNames set "GoodTracksVeryLoose"
    daughterListNames set "GoodTracksVeryLoose"
    daughterListNames set "GoodTracksVeryLoose"

    preFitSelectors set $looseD0Mass
}

mod clone SmpMergerDefiner MyD0Loose
seq append BtoDPiPiCompSequence MyD0Loose
talkto MyD0Loose {
    inputListNames set "D0ToKPiLoose"
    inputListNames set "MyD0ToKPiPi0Loose"
    inputListNames set "MyD0ToK3PiLoose"
    inputListNames set "MyD0ToKsPiPiLoose"
} 


####################### D+ #############################
mod clone SmpMakerDefiner MyDcToKPiPiLoose
seq append BtoDPiPiCompSequence MyDcToKPiPiLoose
talkto MyDcToKPiPiLoose {
    decayMode         set "D+ -> K- pi+ pi+"
    
    daughterListNames set KLHNotPion
    daughterListNames set "GoodTracksVeryLoose"
    daughterListNames set "GoodTracksVeryLoose"

    preFitSelectors set $looseDchMass
}

mod clone SmpMakerDefiner MyDcToKsPiLoose
seq append BtoDPiPiCompSequence MyDcToKsPiLoose
talkto MyDcToKsPiLoose {
    decayMode         set "D+ -> K_S0 pi+"
    
#    daughterListNames set KsDefault
    daughterListNames set KsLoose
    daughterListNames set "GoodTracksVeryLoose"

    disableCloneCheck set false
    preFitSelectors set $looseDchMass
}

mod clone SmpMergerDefiner MyDchLoose
seq append BtoDPiPiCompSequence MyDchLoose
talkto MyDchLoose {
    inputListNames set "MyDcToKPiPiLoose"
    inputListNames set "MyDcToKsPiLoose"
} 



######################## D* #########################
mod clone SmpMakerDefiner MyDstarToD0PiLoose
seq append BtoDPiPiCompSequence MyDstarToD0PiLoose
talkto MyDstarToD0PiLoose {
    decayMode         set "D*+ -> D0 pi+"
    
    checkType         set false
    daughterListNames set MyD0Loose
    daughterListNames set "GoodTracksVeryLoose"

    disableCloneCheck set true
    fittingAlgorithm  set "Add4"
    postFitSelectors  set "DeltaM 0.13:0.17"
}

######################## D*0 -> D0 pi0 #####################
mod clone SmpMakerDefiner newDstar0
seq append BtoDPiPiCompSequence newDstar0
talkto newDstar0 {
    decayMode         set "D*0 -> D0 pi0"
    checkType         set false
    daughterListNames set MyD0Loose
    daughterListNames set pi0Loose

    disableCloneCheck set true
    fittingAlgorithm  set "Add4"
    postFitSelectors  set "DeltaM 0.13:0.17"
}
 

######################## B -> D* pi pi #####################
mod clone SmpMakerDefiner BchToDstarPiPiDefault
seq append BtoDPiPiCompSequence BchToDstarPiPiDefault
talkto BchToDstarPiPiDefault {
    decayMode         set "B- -> D*+ pi- pi-"
    
    checkType         set false
    daughterListNames set "MyDstarToD0PiLoose"
    daughterListNames set "GoodTracksVeryLoose"
    daughterListNames set "GoodTracksVeryLoose"

    disableCloneCheck set true
    fittingAlgorithm  set "Add4"

    preFitSelectors  set "Mass 0:10"
    
    postFitSelectors  set "Mass 0:10"
    postFitSelectors  set "DeltaE -0.3:0.3"
    postFitSelectors  set "Mes 5.15:5.35"
}
######################## B -> D*0 pi pi #####################
mod clone SmpMakerDefiner B0ToDstar0PiPiDefault
seq append BtoDPiPiCompSequence B0ToDstar0PiPiDefault
talkto B0ToDstar0PiPiDefault {
    decayMode         set "B0 -> anti-D*0 pi+ pi-"
    checkType         set false
    daughterListNames set newDstar0  
    daughterListNames set "GoodTracksVeryLoose"
    daughterListNames set "GoodTracksVeryLoose"

    disableCloneCheck set true
    fittingAlgorithm  set "Add4"

    preFitSelectors  set "Mass 0:10"

    postFitSelectors  set "Mass 0:10"
    postFitSelectors  set "DeltaE -0.25:0.25"
    postFitSelectors  set "Mes 5.17:5.35"
}

######################## B -> D+ pi pi #####################
mod clone SmpMakerDefiner BchToDchPiPiDefault
seq append BtoDPiPiCompSequence BchToDchPiPiDefault
talkto BchToDchPiPiDefault {
    decayMode         set "B+ -> D- pi+ pi+"
    
    checkType         set false
    daughterListNames set MyDchLoose
    daughterListNames set "GoodTracksVeryLoose"
    daughterListNames set "GoodTracksVeryLoose"

    disableCloneCheck set true
    fittingAlgorithm  set "Add4"

    preFitSelectors  set "Mass 0:10"
    
    postFitSelectors  set "Mass 0:10"
    postFitSelectors  set "DeltaE -0.3:0.3"
    postFitSelectors  set "Mes 5.15:5.35"
	
}
######################## B0 -> D0 pi pi #####################
mod clone SmpMakerDefiner B0ToD0PiPiDefault
seq append BtoDPiPiCompSequence B0ToD0PiPiDefault
talkto B0ToD0PiPiDefault {
    decayMode         set "B0 -> anti-D0 pi- pi+"
    checkType         set false
    daughterListNames set MyD0Loose
    daughterListNames set "GoodTracksVeryLoose"
    daughterListNames set "GoodTracksVeryLoose"
        
    disableCloneCheck set true 
    fittingAlgorithm  set "Add4"

    preFitSelectors  set "Mass 0:10"
    
    postFitSelectors  set "Mass 0:10"
    postFitSelectors  set "DeltaE -0.25:0.25"
    postFitSelectors  set "Mes 5.17:5.35"
}

