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
source DsTolnu/SP1005Sig/DsReco/NtuplesSmall/Run3On/DataSet-1.tcl

#--------------------------------------------------
#DATA or MC
#--------------------------------------------------
FwkCfgVar ConfigPatch "MC"
FwkCfgVar MCTruth true

#--------------------------------------------------
#Choose Root as output
#--------------------------------------------------
FwkCfgVar BetaMiniTuple "root"
FwkCfgVar histFileName "DsTolnu/SP1005Sig/DsReco/NtuplesSmall/Run3On/Ntuple1.root" 
    
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


##-------------------------------
##Configure and append DsInclusive modules
##-------------------------------
sourceFoundFile DRecoilToolsUser/DsInclusiveReco_Configuration.tcl

##wrong-sign selection
module talk DRecoCheckCharge
 SelectWrongSign set 0
exit


##-------------------------------
##Configure and append Ds to Hadronic modules
##-------------------------------
#create the individual lists, merge them and then feed into DsToHadronic
sourceFoundFile CompositionSequences/CompProdCreateSequence.tcl
sequence append BetaMiniPhysicsSequence CompProdCreateSequence
#sequence append BetaMiniPhysicsSequence CompDsProdSequence

mod clone SmpMakerDefiner DsToKKPiPrompt
seq append CompDsProdSequence DsToKKPiPrompt
catch { setProduction DsToKKPiPrompt }
talkto DsToKKPiPrompt {
  decayMode         set "D_s+ -> K+ K- pi+"
  daughterListNames set "KLHTight"
  daughterListNames set "KLHTight"
  daughterListNames set "GoodTracksVeryLoose"
  fittingAlgorithm  set "Add4"
  postFitSelectors  set "Mass pdg-.2:pdg+.2"
}
 
mod clone SmpMakerDefiner DsTo3PiPrompt
seq append  CompDsProdSequence DsTo3PiPrompt
catch { setProduction DsTo3PiPrompt }
talkto DsTo3PiPrompt {
  decayMode         set "D_s+ -> pi- pi+ pi+"
  daughterListNames set "piLHTight"
  daughterListNames set "piLHTight"
  daughterListNames set "piLHTight"
  fittingAlgorithm  set "Add4"
  postFitSelectors  set "Mass pdg-.2:pdg+.2"
}

mod clone SmpMergerDefiner DRecoDsMainPID
catch { setProduction DRecoDsMainPID }
talkto DRecoDsMainPID {
  inputListNames set DsToKsKLoosePID
  inputListNames set DsToKKPiPrompt
  inputListNames set DsToPhiRhoCLoosePID
  inputListNames set DsToKsKPiPiLoosePID
  inputListNames set DsTo3PiPrompt
  inputListNames set DsToPhiPiPiPiLoosePID
  inputListNames set DsToEtaRhoCLoosePID
  inputListNames set DsToEtaPRhoCLoosePID
  inputListNames set DsToEtaPiLoosePID
  inputListNames set DsToEtaPPiLoosePID
}
sequence append DRecoSequence DRecoDsMainPID


talkto DRecoDsToHadronic {
  DsHadronicList set DRecoDsMainPID
  FitProbCut set .001
  DsHadronicMassCutLo set 1.77
  DsHadronicMassCutHi set 2.17
  DsHadronicPstarCut set 2.
  #DeltaMCut set .05
  RequireNoOverlap set 0
}
sequence append DRecoSequence DRecoDsToHadronic


#--------------------------------------------------
#Configure NtupleMaker: NtupleCode depends on these block names
#--------------------------------------------------
talkto BtuTupleMaker {
    listToDump set DsToHadronicCandidateList
    ntupleName set "ntp1"

    writeEveryEvent set false
    fillMC set $MCTruth
    checkClones set false 
    eventBlockContents set "EventID CMp4 BeamSpot PrimVtx"
    mcBlockContents  set "Mass CMMomentum Momentum Vertex"    
 
    ##ntpBlockConfigs set "D_s*+        DsSt  2  30" 
    ntpBlockConfigs set "D_s+         Ds    5  30" 
    ntpBlockConfigs set "f_0          f_0   3 30"
    ntpBlockConfigs set "eta'         EtaP  3 30"
    ntpBlockConfigs set "rho+         Rho   3 30"
    ntpBlockConfigs set "rho0         Rho0  3 30"
    ntpBlockConfigs set "phi          Phi   3 30"
    ntpBlockConfigs set "eta          Eta   3 30"
    ntpBlockConfigs set "K_S0         Ks    2 30"
    ntpBlockConfigs set "pi0          Pi0   2 100"
    ntpBlockConfigs set "K+           K     0 60"
    ntpBlockConfigs set "pi+          Pi    0 200"
    ntpBlockConfigs set "gamma        Gam   0 200"


    #ntpBlockContents set "DsSt: MCIdx Mass Vertex CMMomentum "
    ntpBlockContents set "Ds:   MCIdx Mass Vertex CMMomentum nDaughters UsrData(DsToHadUsrBlock)"
    ntpBlockContents set "f_0:  MCIdx Mass Vertex CMMomentum nDaughters"
    ntpBlockContents set "EtaP: MCIdx Mass Vertex CMMomentum nDaughters"
    ntpBlockContents set "Rho:  MCIdx Mass Vertex CMMomentum nDaughters"
    ntpBlockContents set "Rho0: MCIdx Mass Vertex CMMomentum nDaughters"
    ntpBlockContents set "Phi:  MCIdx Mass Vertex CMMomentum nDaughters"
    ntpBlockContents set "Eta:  MCIdx Mass Vertex CMMomentum nDaughters"
    ntpBlockContents set "Ks:   MCIdx Mass Vertex FlightBS CMMomentum"
    ntpBlockContents set "Pi0:  MCIdx Mass Vertex CMMomentum"
    ntpBlockContents set "Pi:   MCIdx CMMomentum"
    ntpBlockContents set "K:    MCIdx CMMomentum"
    ntpBlockContents set "Gam:  MCIdx CMMomentum Momentum"
    
}
sequence append DRecoSequence BtuTupleMaker

##
path append Everything DRecoSequence 


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






