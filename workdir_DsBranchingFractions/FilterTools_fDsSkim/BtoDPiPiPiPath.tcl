
################################################ 
#
#   Sequence for skim of B-->D(*)pipipi 
#
#   Haiwen Zhao 03/2007 
#
################################################

path create BtoDPiPiPiPath
path append BtoDPiPiPiPath FilterBGFMultiHadron

sequence create BtoDPiPiPiCompSequence
sourceFoundFile SimpleComposition/SmpCompositionSequence.tcl
sequence append BtoDPiPiPiCompSequence pi0VeryLoose
sequence append BtoDPiPiPiCompSequence pi0Loose
sourceFoundFile CompositionSequences/defineCompAlias.tcl

# talk to the tag module
 talkto BtoDPiPiPiTagFilter {
     B0D3PiList   set B0ToDchPiPiPiDefault
     B0Ds3PiList  set B0ToDstarPiPiPiDefault
     BchD3PiList   set BchToD0PiPiPiDefault
     BchDs3PiList  set BchToDstar0PiPiPiDefault
 }

#### D0 ..........
mod clone SmpMakerDefiner MyD0ToKPiList
seq append BtoDPiPiPiCompSequence MyD0ToKPiList
talkto MyD0ToKPiList {
    decayMode         set "D0 -> K- pi+ "
    daughterListNames set KLHNotPion
    daughterListNames set "GoodTracksVeryLoose"
    preFitSelectors   set "Mass 1.825:1.9" 
}


#### D+ .............
mod clone SmpMakerDefiner MyDchToKPiPiList
seq append BtoDPiPiPiCompSequence MyDchToKPiPiList
talkto MyDchToKPiPiList {
    decayMode         set "D+ -> K- pi+ pi+"
    daughterListNames set KLHNotPion
    daughterListNames set "GoodTracksVeryLoose"
    daughterListNames set "GoodTracksVeryLoose"
    preFitSelectors   set "Mass 1.835:1.9"
}


#### D*+ ................
mod clone SmpMakerDefiner MyDstarToD0PiList
seq append BtoDPiPiPiCompSequence MyDstarToD0PiList
talkto MyDstarToD0PiList {
    decayMode         set "D*+ -> D0 pi+"    
    checkType         set false
    daughterListNames set MyD0ToKPiList
    daughterListNames set "GoodTracksVeryLoose"
    disableCloneCheck set true
    fittingAlgorithm  set "Add4"
    postFitSelectors  set "DeltaM 0.13:0.165"
}


#### D*0 -> D0 pi0 ...............
mod clone SmpMakerDefiner MyDstar0ToD0Pi0List
seq append BtoDPiPiPiCompSequence MyDstar0ToD0Pi0List
talkto MyDstar0ToD0Pi0List {
    decayMode         set "D*0 -> D0 pi0"
    checkType         set false
    daughterListNames set MyD0ToKPiList
    daughterListNames set pi0VeryLoose
    disableCloneCheck set true
    fittingAlgorithm  set "Add4"
    postFitSelectors  set "DeltaM 0.13:0.165"
}
 

#### B0 -> D- pi pi pi ............
mod clone SmpMakerDefiner B0ToDchPiPiPiDefault
seq append BtoDPiPiPiCompSequence B0ToDchPiPiPiDefault
talkto B0ToDchPiPiPiDefault {
    decayMode         set "B0 -> D- pi+ pi- pi+"    
    checkType         set false
    daughterListNames set MyDchToKPiPiList 
    daughterListNames set "GoodTracksVeryLoose"
    daughterListNames set "GoodTracksVeryLoose"
    daughterListNames set "GoodTracksVeryLoose"
    disableCloneCheck set true
    fittingAlgorithm  set "Add4"
    preFitSelectors   set "Mass 0:10"
    postFitSelectors  set "Mass 0:10"
    postFitSelectors  set "DeltaE -0.3:0.3"
    postFitSelectors  set "Mes 5.18:5.35"
	
}

#### B0 -> D*- pi pi pi ...........
mod clone SmpMakerDefiner B0ToDstarPiPiPiDefault
seq append BtoDPiPiPiCompSequence B0ToDstarPiPiPiDefault
talkto B0ToDstarPiPiPiDefault {
    decayMode         set "B0 -> D*- pi+ pi- pi+"    
    checkType         set false
    daughterListNames set MyDstarToD0PiList
    daughterListNames set "GoodTracksVeryLoose"
    daughterListNames set "GoodTracksVeryLoose"
    daughterListNames set "GoodTracksVeryLoose"
    disableCloneCheck set true
    fittingAlgorithm  set "Add4"
    preFitSelectors   set "Mass 0:10"    
    postFitSelectors  set "Mass 0:10"
    postFitSelectors  set "DeltaE -0.3:0.3"
    postFitSelectors  set "Mes 5.18:5.35"
}


#### B- -> D0 pi pi pi ...............
mod clone SmpMakerDefiner BchToD0PiPiPiDefault
seq append BtoDPiPiPiCompSequence BchToD0PiPiPiDefault
talkto BchToD0PiPiPiDefault {
    decayMode         set "B+ -> anti-D0 pi+ pi- pi+"
    checkType         set false
    daughterListNames set MyD0ToKPiList
    daughterListNames set "GoodTracksVeryLoose"
    daughterListNames set "GoodTracksVeryLoose"
    daughterListNames set "GoodTracksVeryLoose"        
    disableCloneCheck set true 
    fittingAlgorithm  set "Add4"
    preFitSelectors   set "Mass 0:10"    
    postFitSelectors  set "Mass 0:10"
    postFitSelectors  set "DeltaE -0.3:0.3"
    postFitSelectors  set "Mes 5.18:5.35"
}

#### B- -> D*0 pi pi pi .............
mod clone SmpMakerDefiner BchToDstar0PiPiPiDefault
seq append BtoDPiPiPiCompSequence BchToDstar0PiPiPiDefault
talkto BchToDstar0PiPiPiDefault {
    decayMode         set "B+ -> anti-D*0 pi+ pi- pi+"
    checkType         set false
    daughterListNames set MyDstar0ToD0Pi0List  
    daughterListNames set "GoodTracksVeryLoose"
    daughterListNames set "GoodTracksVeryLoose"
    daughterListNames set "GoodTracksVeryLoose"
    disableCloneCheck set true
    fittingAlgorithm  set "Add4"
    preFitSelectors   set "Mass 0:10"
    postFitSelectors  set "Mass 0:10"
    postFitSelectors  set "DeltaE -0.3:0.3"
    postFitSelectors  set "Mes 5.18:5.35"
}


##########################################
# mod clone SmpMergerDefiner MyBToDPiPiPiLoose
# seq append BtoDPiPiPiCompSequence MyBToDPiPiPiLoose
# talkto MyBToDPiPiPiLoose {
#     inputListNames set B0ToDstarPiPiPiDefault
#     inputListNames set B0ToDchPiPiPiDefault
#     inputListNames set BchToD0PiPiPiDefault
#     inputListNames set BchToDstar0PiPiPiDefault
# }

################################################

path append BtoDPiPiPiPath BtoDPiPiPiCompSequence
path append BtoDPiPiPiPath BtoDPiPiPiTagFilter


