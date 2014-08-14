#--------------------------------------------------
#some default stuff
#--------------------------------------------------
sourceFoundFile ErrLogger/ErrLog.tcl
sourceFoundFile FrameScripts/FwkCfgVar.tcl
sourceFoundFile FrameScripts/talkto.tcl
sourceFoundFile FrameScripts/setProduction.tcl
set ProdTclOnly true                                      
ErrLoggingLevel warning
FwkCfgVar BetaMiniReadPersistence "Kan"    
FwkCfgVar levelOfDetail "cache" 

#--------------------------------------------------
#Specify the input data
#--------------------------------------------------
source DsTolnu/KKpi/NoDenom/SP1005/DataSet-1.tcl


#--------------------------------------------------
#DATA or MC
#--------------------------------------------------
FwkCfgVar ConfigPatch "Run2"
FwkCfgVar MCTruth false
#FwkCfgVar ConfigPatch "MC"
#FwkCfgVar MCTruth true

#--------------------------------------------------
#Choose Root as output
#--------------------------------------------------
FwkCfgVar BetaMiniTuple "root"
FwkCfgVar histFileName "DsTolnu/KKpi/NoDenom/SP1005/Ntuple1.root"
    
#--------------------------------------------------                            
#create Everything path and add core sequences to it
#--------------------------------------------------                        
sourceFoundFile BetaMiniUser/btaMiniPhysics.tcl

#--------------------------------------------------
#actions 
#--------------------------------------------------
sourceFoundFile BetaMiniSequences/BetaMiniActions.tcl
action disable HistoDirAction

#--------------------------------------------------
#Enable User defined Data
#--------------------------------------------------
sourceFoundFile UsrTools/UsrDataProcs.tcl
enableReadUsrData

##-------------------------------------
##Configure KKpi modules
###--------------------------------
#sequence insert BetaMiniPhysicsSequence -b BtaPidCalibOprSequence FilterBGFMultiHadron
#sequence append BetaMiniInitSequence FilterBGFMultiHadron
#path append Everything FilterBGFMultiHadron

mod clone SmpMakerDefiner DRecoDsToKKPi 
catch { setProduction DRecoDsToKKPi }
talkto DRecoDsToKKPi {
    decayMode         set "D_s+ -> K- K+ pi+"
    #daughterListNames set "KBDTKaonVeryLoose"
    #daughterListNames set "KBDTKaonVeryLoose"
    daughterListNames set "GoodTracksVeryLoose"
    daughterListNames set "GoodTracksVeryLoose"
    daughterListNames set "GoodTracksVeryLoose"
    fittingAlgorithm  set "TreeFitter"
    fitConstraints    set "Beam"
    preFitSelectors   set "Mass pdg-.2:pdg+.2"
    preFitSelectors   set "CmsP 2.5:"
    postFitSelectors  set "Mass pdg-.15:pdg+.15"
    postFitSelectors  set "CmsP 3.:"
    postFitSelectors  set "ProbChiSq 0.00001:"
}
path append Everything DRecoDsToKKPi

mod clone SmpSubListerDefiner DRecoKKpiPhotonList
catch { setProduction DRecoKKpiPhotonList }
talkto DRecoKKpiPhotonList {
  unrefinedListName set "GoodPhotonLoose"
  selectors         set "LabE 0.100:10.0"
  selectors         set "Lat 0.01:0.6"
}
path append Everything DRecoKKpiPhotonList

# Ds* -> Ds gamma
mod clone SmpMakerDefiner DRecoKKpiDsStToDsGam 
catch { setProduction DRecoKKpiDsStToDsGam }
talkto DRecoKKpiDsStToDsGam {
    decayMode         set "D_s*+ -> D_s+ gamma"
    daughterListNames set "DRecoDsToKKPi"
    daughterListNames set "DRecoKKpiPhotonList"
    fittingAlgorithm  set "Add4"
    postFitSelectors  set "Mass pdg-.15:pdg+.15"

}
path append Everything DRecoKKpiDsStToDsGam



#--------------------------------------------------
#Configure NtupleMaker: NtupleCode depends on these block names
#--------------------------------------------------
talkto BtuTupleMaker {
    listToDump set DRecoKKpiDsStToDsGam
    ntupleName set "ntp1"

    writeEveryEvent set false
    fillMC set $MCTruth
    checkClones set false 
    eventBlockContents set "EventID CMp4 BeamSpot PrimVtx"
    mcBlockContents set "Mass CMMomentum Momentum Vertex"    

    #note that the maximum array sizes below have been counted properly according to the maximum Evt and decay tree content.
    ntpBlockConfigs set "D_s*+  DsSt  2  200" 
    ntpBlockConfigs set "D_s+   Ds    3  200" 
    ntpBlockConfigs set "K+     K     0 400"  
    ntpBlockConfigs set "pi+    Pi    0 200" 
    ntpBlockConfigs set "gamma  Gam   0 200" 
                                                                                       
    ntpBlockContents set "DsSt: MCIdx Mass Momentum CMMomentum"
    ntpBlockContents set "Ds:   MCIdx Vertex VtxChi2 Mass Momentum CMMomentum"
    ntpBlockContents set "K:    MCIdx Momentum CMMomentum"
    ntpBlockContents set "Pi:   MCIdx Momentum CMMomentum"
    ntpBlockContents set "Gam:  MCIdx Momentum CMMomentum"
 
    ntpBlockToTrk set "K Pi"
    trkExtraContents set BitMap:piSelectorsMap,KSelectorsMap
    
    gamExtraContents set EMC:closestBump,closestTrk


}
path append Everything BtuTupleMaker



#--------------------------------------------------
#set the print frequency of event counter
#--------------------------------------------------
talkto EvtCounter {
    printFreq set 100
}

#--------------------------------------------------
#Set Number of events to be analyzed
#--------------------------------------------------
FwkCfgVar NEvent 1000

#--------------------------------------------------
#Set the first event to be analyzed
#--------------------------------------------------
talkto KanEventInput {
first set 1
}

#--------------------------------------------------
#start the job
#--------------------------------------------------
path list

if [ info exists NEvent ] {
    ErrMsg trace " Processing $NEvent events"
    ev begin -nev $NEvent
} else {
    ErrMsg trace " Processing all events"
    ev begin
}

ErrMsg trace "completed OK"
exit

