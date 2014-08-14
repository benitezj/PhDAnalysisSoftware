#--------------------------------------------------
# Some default stuff
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
# Specify the input data
#--------------------------------------------------
sourceFoundFile $collection_tcl

#--------------------------------------------------
# DATA or MC
#--------------------------------------------------
#FwkCfgVar ConfigPatch "Run2"
FwkCfgVar ConfigPatch "MC"
FwkCfgVar MCTruth true

#--------------------------------------------------
# Choose Root as output
#--------------------------------------------------
FwkCfgVar BetaMiniTuple "root"
FwkCfgVar histFileName $ntuple_file_name

#--------------------------------------------------                            
# Create Everything path and add core sequences to it
#--------------------------------------------------                        
sourceFoundFile BetaMiniUser/btaMiniPhysics.tcl

#--------------------------------------------------
# Actions 
#--------------------------------------------------
sourceFoundFile BetaMiniSequences/BetaMiniActions.tcl
action disable HistoDirAction

#--------------------------------------------------
# Enable User defined Data
#--------------------------------------------------
sourceFoundFile UsrTools/UsrDataProcs.tcl
enableReadUsrData

# #--------------------------------------------------
# # Create and append inclusive DsInclusive modules
# #--------------------------------------------------
sourceFoundFile DRecoilToolsUser/DsToEllNu_Configuration.tcl

# Wrong-sign selection
module talk DRecoCheckCharge
 SelectWrongSign set 0
exit

#--------------------------------------------------
# Configure NtupleMaker: NtupleCode depends on these block names
#--------------------------------------------------
talkto BtuTupleMaker {
    #listToDump set DsToEllNuCandidateList
    listToDump set DsToKKsCandidateList
    #listToDump set DRecoCandidateList
    ntupleName set "ntp1"

    writeEveryEvent set false
    fillMC set $MCTruth
    checkClones set true
    eventBlockContents set "EventID CMp4 BeamSpot PrimVtx"
    mcBlockContents  set "Mass CMMomentum Momentum Vertex"

    ntpBlockConfigs set "Upsilon(4S)   Evt   4  300" 
    ntpBlockConfigs set "D_s*+         DsSt  2  300"
    ntpBlockConfigs set "D_s+          Ds    2  300"
    ntpBlockConfigs set "D_1+          Frag  20 300"
    ntpBlockConfigs set "D_10          Frag  20 300"
    ntpBlockConfigs set "D*+           Tag   5  300" 
    ntpBlockConfigs set "D*0           Tag   5  300" 
    ntpBlockConfigs set "D+            Tag   5  300"
    ntpBlockConfigs set "D0            Tag   5  300"
    ntpBlockConfigs set "K+            K     0  900"
    ntpBlockConfigs set "K_S0          Ks    2  900"
    ntpBlockConfigs set "pi+           Pi    0  900"
    ntpBlockConfigs set "pi0           Pi0   2  900"
    ntpBlockConfigs set "gamma         Gam   0  900"
    ntpBlockConfigs set "mu+           Mu    0  900"
    ntpBlockConfigs set "e+            E     0  900"
    ntpBlockConfigs set "p+            P     0  900"
    ntpBlockConfigs set "nu_mu         Nu    0  900"
    ntpBlockConfigs set "nu_e          Nu    0  900"

    #ntpBlockContents set "Evt:        Mass CMMomentum Vertex VtxChi2 nDaughters UsrData(DRecoUsrBlockEllNu)"
    ntpBlockContents set "Evt:        Mass CMMomentum Vertex VtxChi2 nDaughters UsrData(DRecoUsrBlockKKs)"
    #ntpBlockContents set "Evt:        Mass CMMomentum Vertex VtxChi2 nDaughters UsrData(DRecoUsrBlock)"
    ntpBlockContents set "DsSt:       Mass CMMomentum Vertex"
    ntpBlockContents set "Ds:         Mass CMMomentum"
    ntpBlockContents set "Frag:       Mass CMMomentum Vertex VtxChi2          nDaughters" 
    ntpBlockContents set "Tag:  MCIdx Mass CMMomentum Vertex VtxChi2 FlightBS nDaughters"
    ntpBlockContents set "Ks:   MCIdx Mass CMMomentum Vertex         FlightBS"
    ntpBlockContents set "Pi:   MCIdx      CMMomentum"
    ntpBlockContents set "Pi0:  MCIdx Mass CMMomentum"
    ntpBlockContents set "K:    MCIdx      CMMomentum"
    ntpBlockContents set "Mu:   MCIdx      CMMomentum Doca DocaXY Poca PocaXY PocaErr PocaCov"
    ntpBlockContents set "E:    MCIdx      CMMomentum Doca DocaXY Poca PocaXY PocaErr PocaCov"
    ntpBlockContents set "P:    MCIdx      CMMomentum"
    ntpBlockContents set "Nu:         Mass CMMomentum Doca DocaXY Poca PocaXY PocaErr PocaCov"
    ntpBlockContents set "Gam:  MCIdx      CMMomentum"

    ntpBlockToTrk set "K Pi Mu E P"

    trkExtraContents set HOTS:detailSVT,detailDCH
    trkExtraContents set Eff:ave,charge
    trkExtraContents set BitMap:KSelectorsMap,eSelectorsMap,muSelectorsMap,piSelectorsMap,pSelectorsMap
    trkExtraContents set Dirc:pi,K,p,mu,e
    gamExtraContents set EMC:allQuals,closestBump,closestTrk
}
sequence append DRecoSequence BtuTupleMaker

path append Everything DRecoSequence 


#--------------------------------------------------
# Set the print frequency of event counter
#--------------------------------------------------
talkto EvtCounter {
    printFreq set 1000
}

#--------------------------------------------------
# Set Number of events to be analyzed
#--------------------------------------------------
FwkCfgVar NEvent 10000000

#--------------------------------------------------
# Set the first event to be analyzed
#--------------------------------------------------
talkto KanEventInput {
first set 1
}

#--------------------------------------------------
# Start the job
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






