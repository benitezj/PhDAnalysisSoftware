#
#  Define the fDs path
#

#The fDs is a merge of the lists from 7 other skims, which are;
#
#1 D0To2ProngDcj
#2 D0ToKnPiPi0
#3 DRecoDmesonTag
#4 DRecoDsmesonTag
#5 DcTo3PiPrompt
#6 DcToKpipiPrompt
#7 LambdaC
#

set do_D0To2ProngDcj 0
set do_D0ToKnPiPi0 0
set do_DRecoDmesonTag 0
set do_DRecoDsmesonTag 1
set do_DcTo3PiPrompt 0
set do_DcToKpipiPrompt 0
set do_LambdaC 0

path create fDsPath

########################################################################
#1 D0To2ProngDcj
if { $do_D0To2ProngDcj } {
   ##################
   ## D0 -> K pi
   ##################

    
   mod clone SmpRefinerDefiner D0ToKPiTightPID_DCJ
   path append fDsPath D0ToKPiTightPID_DCJ
   catch { setProduction D0ToKPiTightPID_DCJ }
   talkto D0ToKPiTightPID_DCJ {
       debug             set f
       verbose           set f
 
       decayMode         set "D0 -> K- pi+"

    unrefinedListName  set "D0ToKPiLoose"

    daughterListNames set "KCombinedSuperLoose"
    daughterListNames set "piCombinedSuperLoose"

##     choose fitting constraint and method

    fittingAlgorithm  set "Cascade"

    fitConstraints    set "Geo"
    fitConstraints    set "Momentum"

##     postFitSelectors chisquare, CMS p*
    postFitSelectors  set "ProbChiSq 0.005:1.0"
    postFitSelectors  set "CmsP 2.4:10.0"


   }
    
    
   ##################
   ## D0 -> K K
   ##################
    
   mod clone SmpRefinerDefiner D0ToKKTightPID_DCJ
   path append fDsPath D0ToKKTightPID_DCJ
   catch { setProduction D0ToKKTightPID_DCJ }
   talkto D0ToKKTightPID_DCJ {
       debug             set f
       verbose           set f
 
       decayMode         set "D0 -> K+ K-"
       unrefinedListName  set "D0ToKKLoose"
       daughterListNames set "KCombinedSuperLoose"
       daughterListNames set "KCombinedSuperLoose"

##     choose fitting constraint and method

    fittingAlgorithm  set "Cascade"

    fitConstraints    set "Geo"
    fitConstraints    set "Momentum"

##     postFitSelectors chisquare, CMS p*
    postFitSelectors  set "ProbChiSq 0.005:1.0"
    postFitSelectors  set "CmsP 2.4:10.0"


   }


   ##########################################
   ## D0 -> pi K : Doubly Cabibbo Suppressed
   ##########################################

    
   mod clone SmpRefinerDefiner D0ToPiKTightPID_DCJ
   path append fDsPath D0ToPiKTightPID_DCJ
   catch { setProduction D0ToPiKTightPID_DCJ }
   talkto D0ToPiKTightPID_DCJ {
       debug             set f
       verbose           set f
 
       decayMode         set "D0 -> K+ pi-"
       checkType         set false

       unrefinedListName  set "D0ToKPiTightPID_DCJ"

       fittingAlgorithm  set "Cascade"

       fitConstraints    set "Geo"
       fitConstraints    set "Momentum"
   }


   ##################
   ## D0 -> pi pi
   ##################
    
   mod clone SmpRefinerDefiner D0ToPiPiTight_DCJ
   path append fDsPath D0ToPiPiTight_DCJ
   catch { setProduction D0ToPiPiTight_DCJ }
   talkto D0ToPiPiTight_DCJ {
       debug             set f
       verbose           set f
 
       decayMode         set "D0 -> pi+ pi-"
       unrefinedListName set "D0ToPiPiLoose"
       daughterListNames set "piCombinedSuperLoose"
       daughterListNames set "piCombinedSuperLoose"

##     choose fitting constraint and method

    fittingAlgorithm  set "Cascade"

    fitConstraints    set "Geo"
    fitConstraints    set "Momentum"

##     postFitSelectors chisquare, CMS p*
    postFitSelectors  set "ProbChiSq 0.005:1.0"
    postFitSelectors  set "CmsP 2.4:10.0"


   }
}
########################################################################
########################################################################
#2 D0ToKnPiPi0
if { $do_D0ToKnPiPi0 } {
set mySkimSequence D0ToKnPiPi0Sequence

sequence create $mySkimSequence
path append fDsPath $mySkimSequence
 
#  Create the D Lists...2 modes in total:
#  1. D0 -> Ks pi+ pi- pi0
#  2. D0 -> K- pi+ pi+ pi-  pi0


#################################
## SimpleComposition Cuts
#################################

## D0 cuts
##
#set pi0ListD           "pi0AllVeryLoose"
set pi0ListD           "pi0VeryLoose"
set ksListD            "KsLoose"
set kListD             "KCombinedVeryLoose"
#set pionListD          "GoodTracksVeryLoose"
set pionListD          "piCombinedVeryLoose"

##
##

# FastVtx probability
  set ProbChiSqFastVtxTight  "ProbChiSq 0.0005:"

# pre- and post-fit mass, p* cuts
  set verylooseMass          "Mass 1.5:2.3"
  set looseMass              "Mass 1.75:1.96"
  set preCmsP                "CmsP 2.0:"
  set postCmsP               "CmsP 2.4:"

mod clone SmpMakerDefiner D0ToKsPimPipPi0
sequence append $mySkimSequence D0ToKsPimPipPi0
talkto D0ToKsPimPipPi0 {
    decayMode          set "D0 -> K_S0 pi- pi+ pi0"

    daughterListNames  set $ksListD
    daughterListNames  set $pionListD
    daughterListNames  set $pionListD
    daughterListNames  set $pi0ListD


  fittingAlgorithm  set "Cascade"
  fitConstraints    set "Geo"

  preFitSelectors   set $preCmsP
  preFitSelectors   set $verylooseMass

  postFitSelectors  set $ProbChiSqFastVtxTight
  postFitSelectors  set $postCmsP
  postFitSelectors  set $looseMass
}


mod clone SmpMakerDefiner D0ToKmPipPipPimPi0
sequence append $mySkimSequence D0ToKmPipPipPimPi0
talkto D0ToKmPipPipPimPi0 {
    decayMode          set "D0 -> K- pi+ pi+ pi- pi0"

    daughterListNames  set $kListD
    daughterListNames  set $pionListD
    daughterListNames  set $pionListD
    daughterListNames  set $pionListD
    daughterListNames  set $pi0ListD

  fittingAlgorithm  set "Cascade"
  fitConstraints    set "Geo"

  preFitSelectors   set $preCmsP
  preFitSelectors   set $verylooseMass

  postFitSelectors  set $ProbChiSqFastVtxTight
  postFitSelectors  set $postCmsP
  postFitSelectors  set $looseMass

}
}
########################################################################
########################################################################
#3 DRecoDmesonTag
if { $do_DRecoDmesonTag } {
mod clone SmpRefinerDefiner RefinedDRecoDmesonTag_D0ToKPiLoosePID
path append fDsPath RefinedDRecoDmesonTag_D0ToKPiLoosePID
talkto RefinedDRecoDmesonTag_D0ToKPiLoosePID {
    unrefinedListName set "D0ToKPiLoosePID"
    postFitSelectors   set "CmsP 2.4:"
}
mod clone SmpRefinerDefiner RefinedDRecoDmesonTag_D0ToKPiPi0LoosePID
path append fDsPath RefinedDRecoDmesonTag_D0ToKPiPi0LoosePID
talkto RefinedDRecoDmesonTag_D0ToKPiPi0LoosePID {
    unrefinedListName set "D0ToKPiPi0LoosePID"
    postFitSelectors   set "CmsP 2.4:"
}
mod clone SmpRefinerDefiner RefinedDRecoDmesonTag_D0ToK3PiLoosePID
path append fDsPath RefinedDRecoDmesonTag_D0ToK3PiLoosePID
talkto RefinedDRecoDmesonTag_D0ToK3PiLoosePID {
    unrefinedListName set "D0ToK3PiLoosePID"
    postFitSelectors   set "CmsP 2.4:"
}
mod clone SmpRefinerDefiner RefinedDRecoDmesonTag_D0ToKsPiPiLoosePID
path append fDsPath RefinedDRecoDmesonTag_D0ToKsPiPiLoosePID
talkto RefinedDRecoDmesonTag_D0ToKsPiPiLoosePID {
    unrefinedListName set "D0ToKsPiPiLoosePID"
    postFitSelectors   set "CmsP 2.4:"
}
mod clone SmpRefinerDefiner RefinedDRecoDmesonTag_DcToKsPiLoosePID
path append fDsPath RefinedDRecoDmesonTag_DcToKsPiLoosePID
talkto RefinedDRecoDmesonTag_DcToKsPiLoosePID {
    unrefinedListName set "DcToKsPiLoosePID"
    postFitSelectors   set "CmsP 2.4:"
}
mod clone SmpRefinerDefiner RefinedDRecoDmesonTag_DcToKsKLoosePID
path append fDsPath RefinedDRecoDmesonTag_DcToKsKLoosePID
talkto RefinedDRecoDmesonTag_DcToKsKLoosePID {
    unrefinedListName set "DcToKsKLoosePID"
    postFitSelectors   set "CmsP 2.4:"
}
mod clone SmpRefinerDefiner RefinedDRecoDmesonTag_DcToKKPiLoosePID
path append fDsPath RefinedDRecoDmesonTag_DcToKKPiLoosePID
talkto RefinedDRecoDmesonTag_DcToKKPiLoosePID {
    unrefinedListName set "DcToKKPiLoosePID"
    postFitSelectors   set "CmsP 2.4:"
}
mod clone SmpRefinerDefiner RefinedDRecoDmesonTag_DcToPiPiPiLoosePID
path append fDsPath RefinedDRecoDmesonTag_DcToPiPiPiLoosePID
talkto RefinedDRecoDmesonTag_DcToPiPiPiLoosePID {
    unrefinedListName set "DcToPiPiPiLoosePID"
    postFitSelectors   set "CmsP 2.4:"
}
mod clone SmpRefinerDefiner RefinedDRecoDmesonTag_DcToKsPiPi0LoosePID
path append fDsPath RefinedDRecoDmesonTag_DcToKsPiPi0LoosePID
talkto RefinedDRecoDmesonTag_DcToKsPiPi0LoosePID {
    unrefinedListName set "DcToKsPiPi0LoosePID"
    postFitSelectors   set "CmsP 2.4:"
}
mod clone SmpRefinerDefiner RefinedDRecoDmesonTag_DcToKsPiPiPiLoosePID
path append fDsPath RefinedDRecoDmesonTag_DcToKsPiPiPiLoosePID
talkto RefinedDRecoDmesonTag_DcToKsPiPiPiLoosePID {
    unrefinedListName set "DcToKsPiPiPiLoosePID"
    postFitSelectors   set "CmsP 2.4:"
}
mod clone SmpRefinerDefiner RefinedDRecoDmesonTag_DcToKPiPiPi0LoosePID
path append fDsPath RefinedDRecoDmesonTag_DcToKPiPiPi0LoosePID
talkto RefinedDRecoDmesonTag_DcToKPiPiPi0LoosePID {
    unrefinedListName set "DcToKPiPiPi0LoosePID"
    postFitSelectors   set "CmsP 2.4:"
}


}
########################################################################
########################################################################
#4 DRecoDsmesonTag
if { $do_DRecoDsmesonTag } {
mod clone SmpMergerDefiner DRecoDsMainPID
path append fDsPath DRecoDsMainPID
catch { setProduction DRecoDsMainPID }
talkto DRecoDsMainPID {
  inputListNames set Ds3prongLoosePID
  inputListNames set DsToPhiPiPiPiLoosePID
  inputListNames set DsToPhiRhoCLoosePID
  inputListNames set DsToKsKPiPiLoosePID
  inputListNames set DsToEtaPiLoosePID
  inputListNames set DsToEtaRhoCLoosePID
  inputListNames set DsToEtaPPiLoosePID
  inputListNames set DsToEtaPRhoCLoosePID
}

mod clone SmpRefinerDefiner Refined_DRecoDsMainPID
path append fDsPath Refined_DRecoDsMainPID
talkto Refined_DRecoDsMainPID {
    unrefinedListName set "DRecoDsMainPID"
    postFitSelectors   set "CmsP 2.2:"
}

# mod clone SmpRefinerDefiner RefinedDRecoDsmesonTag_Ds3prongLoosePID
# path append fDsPath RefinedDRecoDsmesonTag_Ds3prongLoosePID
# talkto RefinedDRecoDsmesonTag_Ds3prongLoosePID {
#     unrefinedListName set "Ds3prongLoosePID"
#     postFitSelectors   set "CmsP 2.2:"
# }
# mod clone SmpRefinerDefiner RefinedDRecoDsmesonTag_DsToPhiPiPiPiLoosePID
# path append fDsPath RefinedDRecoDsmesonTag_DsToPhiPiPiPiLoosePID
# talkto RefinedDRecoDsmesonTag_DsToPhiPiPiPiLoosePID {
#     unrefinedListName set "DsToPhiPiPiPiLoosePID"
#     postFitSelectors   set "CmsP 2.2:"
# }
# mod clone SmpRefinerDefiner RefinedDRecoDsmesonTag_DsToPhiRhoCLoosePID
# path append fDsPath RefinedDRecoDsmesonTag_DsToPhiRhoCLoosePID
# talkto RefinedDRecoDsmesonTag_DsToPhiRhoCLoosePID {
#     unrefinedListName set "DsToPhiRhoCLoosePID"
#     postFitSelectors   set "CmsP 2.2:"
# }
# mod clone SmpRefinerDefiner RefinedDRecoDsmesonTag_DsToKsKPiPiLoosePID
# path append fDsPath RefinedDRecoDsmesonTag_DsToKsKPiPiLoosePID
# talkto RefinedDRecoDsmesonTag_DsToKsKPiPiLoosePID {
#     unrefinedListName set "DsToKsKPiPiLoosePID"
#     postFitSelectors   set "CmsP 2.2:"
# }
# mod clone SmpRefinerDefiner RefinedDRecoDsmesonTag_DsToEtaPiLoosePID
# path append fDsPath RefinedDRecoDsmesonTag_DsToEtaPiLoosePID
# talkto RefinedDRecoDsmesonTag_DsToEtaPiLoosePID {
#     unrefinedListName set "DsToEtaPiLoosePID"
#     postFitSelectors   set "CmsP 2.2:"
# }
# mod clone SmpRefinerDefiner RefinedDRecoDsmesonTag_DsToEtaRhoCLoosePID
# path append fDsPath RefinedDRecoDsmesonTag_DsToEtaRhoCLoosePID
# talkto RefinedDRecoDsmesonTag_DsToEtaRhoCLoosePID {
#     unrefinedListName set "DsToEtaRhoCLoosePID"
#     postFitSelectors   set "CmsP 2.2:"
# }
# mod clone SmpRefinerDefiner RefinedDRecoDsmesonTag_DsToEtaPPiLoosePID
# path append fDsPath RefinedDRecoDsmesonTag_DsToEtaPPiLoosePID
# talkto RefinedDRecoDsmesonTag_DsToEtaPPiLoosePID {
#     unrefinedListName set "DsToEtaPPiLoosePID"
#     postFitSelectors   set "CmsP 2.2:"
# }
# mod clone SmpRefinerDefiner RefinedDRecoDsmesonTag_DsToEtaPRhoCLoosePID
# path append fDsPath RefinedDRecoDsmesonTag_DsToEtaPRhoCLoosePID
# talkto RefinedDRecoDsmesonTag_DsToEtaPRhoCLoosePID {
#     unrefinedListName set "DsToEtaPRhoCLoosePID"
#     postFitSelectors   set "CmsP 2.2:"
# }
}
########################################################################
########################################################################
#5 DcTo3PiPrompt
if { $do_DcTo3PiPrompt } {
#################################
## global control variables
#################################

  set D0Debug                false
  set D0Verbose              false

#########################
## SimpleComposition cuts
#########################

# FastVtx probability
  set ProbChiSqFastVtxTight  "ProbChiSq 0.005:"

# pre- and post-fit mass, p* cuts
  set verylooseMass          "Mass 1.5:2.3"
  set looseMass              "Mass 1.7:2.1"
  set preCmsP                "CmsP 2.0:"
  set postCmsP               "CmsP 2.4:"

# D+ -> pi- pi+ pi+

mod clone SmpMakerDefiner DcTo3PiPrompt
path append fDsPath DcTo3PiPrompt
catch { setProduction DcTo3PiPrompt }
talkto DcTo3PiPrompt {
  debug             set $D0Debug
  verbose           set $D0Verbose
  disableCloneCheck set true

  decayMode         set "D+ -> pi- pi+ pi+"

  daughterListNames set "piCombinedSuperLoose"
  daughterListNames set "piCombinedSuperLoose"
  daughterListNames set "piCombinedSuperLoose"

  fittingAlgorithm  set "Cascade"
  fitConstraints    set "Geo"

  preFitSelectors   set $preCmsP
  preFitSelectors   set $verylooseMass

  postFitSelectors  set $ProbChiSqFastVtxTight
  postFitSelectors  set $postCmsP
  postFitSelectors  set $looseMass
}
}
########################################################################
########################################################################
#6 DcToKpipiPrompt
if { $do_DcToKpipiPrompt } {
#################################
## global control variables
#################################

  set D0Debug                false
  set D0Verbose              false

#########################
## SimpleComposition cuts
#########################

# FastVtx probability
  set ProbChiSqFastVtxTight  "ProbChiSq 0.005:"

# pre- and post-fit mass, p* cuts
  set verylooseMass          "Mass 1.5:2.3"
  set looseMass              "Mass 1.7:2.1"
  set preCmsP                "CmsP 2.0:"
  set postCmsP               "CmsP 2.4:"

# D+ -> K- pi+ pi+

mod clone SmpMakerDefiner DcToKpipiPrompt
path append fDsPath DcToKpipiPrompt
catch { setProduction DcToKpipiPrompt }
talkto DcToKpipiPrompt {
  debug             set $D0Debug
  verbose           set $D0Verbose
  disableCloneCheck set true

  decayMode         set "D+ -> K- pi+ pi+"

  daughterListNames set "KCombinedSuperLoose"
  daughterListNames set "piCombinedSuperLoose"
  daughterListNames set "piCombinedSuperLoose"

  fittingAlgorithm  set "Cascade"
  fitConstraints     set "Geo"

  preFitSelectors   set $preCmsP
  preFitSelectors   set $verylooseMass

  postFitSelectors  set $ProbChiSqFastVtxTight
  postFitSelectors  set $postCmsP
  postFitSelectors  set $looseMass
}
}
########################################################################
########################################################################
#7 LambdaC 
if { $do_LambdaC } {
mod clone SmpRefinerDefiner RefinedLambdaC_LambdaCTopKpi
path append fDsPath RefinedLambdaC_LambdaCTopKpi
talkto RefinedLambdaC_LambdaCTopKpi {
    unrefinedListName set "LambdaCTopKpi"
    postFitSelectors   set "CmsP 0.0:"
}
mod clone SmpRefinerDefiner RefinedLambdaC_LambdaCTopKs
path append fDsPath RefinedLambdaC_LambdaCTopKs
talkto RefinedLambdaC_LambdaCTopKs {
    unrefinedListName set "LambdaCTopKs"
    postFitSelectors   set "CmsP 0.0:"
}
mod clone SmpRefinerDefiner RefinedLambdaC_LambdaCTopKspipi
path append fDsPath RefinedLambdaC_LambdaCTopKspipi
talkto RefinedLambdaC_LambdaCTopKspipi {
    unrefinedListName set "LambdaCTopKspipi"
    postFitSelectors   set "CmsP 0.0:"
}
mod clone SmpRefinerDefiner RefinedLambdaC_LambdaCToLzpi
path append fDsPath RefinedLambdaC_LambdaCToLzpi
talkto RefinedLambdaC_LambdaCToLzpi {
    unrefinedListName set "LambdaCToLzpi"
    postFitSelectors   set "CmsP 0.0:"
}
mod clone SmpRefinerDefiner RefinedLambdaC_LambdaCToLzpipi
path append fDsPath RefinedLambdaC_LambdaCToLzpipi
talkto RefinedLambdaC_LambdaCToLzpipi {
    unrefinedListName set "LambdaCToLzpipi"
    postFitSelectors   set "CmsP 0.0:"
}
mod clone SmpRefinerDefiner RefinedLambdaC_LambdaCToLzpipipi
path append fDsPath RefinedLambdaC_LambdaCToLzpipipi
talkto RefinedLambdaC_LambdaCToLzpipipi {
    unrefinedListName set "LambdaCToLzpipipi"
    postFitSelectors   set "CmsP 0.0:"
}
mod clone SmpRefinerDefiner RefinedLambdaC_LambdaCToSzpi
path append fDsPath RefinedLambdaC_LambdaCToSzpi
talkto RefinedLambdaC_LambdaCToSzpi {
    unrefinedListName set "LambdaCToSzpi"
    postFitSelectors   set "CmsP 0.0:"
}
mod clone SmpRefinerDefiner RefinedLambdaC_LambdaCToSzpipipi
path append fDsPath RefinedLambdaC_LambdaCToSzpipipi
talkto RefinedLambdaC_LambdaCToSzpipipi {
    unrefinedListName set "LambdaCToSzpipipi"
    postFitSelectors   set "CmsP 0.0:"
}
} 
########################################################################

mod clone FilterByList fDsFilter
path append fDsPath fDsFilter
talkto fDsFilter {
    #1 D0To2ProngDcj
    if { $do_D0To2ProngDcj  } {
	orList set "D0ToKPiTightPID_DCJ"
	orList set "D0ToKKTightPID_DCJ"
	orList set "D0ToPiKTightPID_DCJ"
	orList set "D0ToPiPiTight_DCJ"
    }
    #2 D0ToKnPiPi0
    if { $do_D0ToKnPiPi0 } {
	orList set "D0ToKsPimPipPi0"
	orList set "D0ToKmPipPipPimPi0"
    }
    #3 DRecoDmesonTag
    if { $do_DRecoDmesonTag } {   
	orList set "RefinedDRecoDmesonTag_D0ToKPiLoosePID"
	orList set "RefinedDRecoDmesonTag_D0ToKPiPi0LoosePID"
	orList set "RefinedDRecoDmesonTag_D0ToK3PiLoosePID"
	orList set "RefinedDRecoDmesonTag_D0ToKsPiPiLoosePID"
	
	orList set "RefinedDRecoDmesonTag_DcToKsPiLoosePID"
	orList set "RefinedDRecoDmesonTag_DcToKsKLoosePID"
	orList set "RefinedDRecoDmesonTag_DcToKKPiLoosePID"
	orList set "RefinedDRecoDmesonTag_DcToPiPiPiLoosePID"
	orList set "RefinedDRecoDmesonTag_DcToKsPiPi0LoosePID"
	orList set "RefinedDRecoDmesonTag_DcToKsPiPiPiLoosePID"
	orList set "RefinedDRecoDmesonTag_DcToKPiPiPi0LoosePID"
    }
    #4 DRecoDmesonTag
    if { $do_DRecoDsmesonTag } { 
	orList set "Refined_DRecoDsMainPID"
	#orList set "RefinedDRecoDsmesonTag_Ds3prongLoosePID"
	#orList set "RefinedDRecoDsmesonTag_DsToPhiPiPiPiLoosePID"
	#orList set "RefinedDRecoDsmesonTag_DsToPhiRhoCLoosePID"
	#orList set "RefinedDRecoDsmesonTag_DsToKsKPiPiLoosePID"
	#orList set "RefinedDRecoDsmesonTag_DsToEtaPiLoosePID"
	#orList set "RefinedDRecoDsmesonTag_DsToEtaRhoCLoosePID"
	#orList set "RefinedDRecoDsmesonTag_DsToEtaPPiLoosePID"
	#orList set "RefinedDRecoDsmesonTag_DsToEtaPRhoCLoosePID"
    }
    #5 DcTo3PiPrompt
    if { $do_DcTo3PiPrompt } {
	orList set "DcTo3PiPrompt"
    }
    #6 DcToKpipiPrompt
    if { $do_DcToKpipiPrompt } {
	orList set "DcToKpipiPrompt"
    }
    #7 LambdaC
    if { $do_LambdaC } {
	orList set "RefinedLambdaC_LambdaCTopKpi"
	orList set "RefinedLambdaC_LambdaCTopKs"
	orList set "RefinedLambdaC_LambdaCTopKspipi"
	orList set "RefinedLambdaC_LambdaCToLzpi"
	orList set "RefinedLambdaC_LambdaCToLzpipi"
	orList set "RefinedLambdaC_LambdaCToLzpipipi"
    }
}

