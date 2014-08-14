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
source DsTolnu/SP1005/DsReco/Ntuples/Run3/DataSet-14.tcl 

#--------------------------------------------------
#DATA or MC
#--------------------------------------------------
#FwkCfgVar ConfigPatch "Run2"
#FwkCfgVar MCTruth false
FwkCfgVar ConfigPatch "MC"
FwkCfgVar MCTruth true

#--------------------------------------------------
#Choose Root as output
#--------------------------------------------------
FwkCfgVar BetaMiniTuple "root"
FwkCfgVar histFileName "DsTolnu/SP1005/DsReco/Ntuples/Run3/Ntuple14.root" 
    
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
##Create and append DsInclusive modules
##-------------------------------
sourceFoundFile DRecoilToolsUser/DsInclusiveReco_Configuration.tcl
path append Everything DRecoDsSequence 

##wrong-sign selection
module talk DRecoCheckCharge
  SelectWrongSign set 0
exit

# module talk DRecoPrintTree
#   PrintMCTruth set 0
#   DefaultRecoPrint set 1
# exit

#--------------------------------------------------
#Configure NtupleMaker: NtupleCode depends on these block names
#--------------------------------------------------
talkto BtuTupleMaker {
    listToDump set DRecoCandidateList 
    ntupleName set "ntp1"

    writeEveryEvent set false
    fillMC set $MCTruth
    checkClones set false 
    #PrimVtx not recognized in current release 24.3.6a
    eventBlockContents set "EventID CMp4 BeamSpot"
    mcBlockContents set "Mass CMMomentum Momentum Vertex"    

    #note that the maximum array sizes below have been counted properly according to the maximum Evt and decay tree content.
    ntpBlockConfigs set "Upsilon(4S)   Evt  4 20" 
    ntpBlockConfigs set "D_s*+  DsSt  2  20" 
    ntpBlockConfigs set "D_s+   Ds    4  40" 
    ntpBlockConfigs set "D_1+   Frag  10 20" 
    ntpBlockConfigs set "D_10   Frag  10 20"
    ntpBlockConfigs set "D*+    Tag   5 40" 
    ntpBlockConfigs set "D*0    Tag   5 40" 
    ntpBlockConfigs set "D+     Tag   5 40" 
    ntpBlockConfigs set "D0     Tag   5 40" 
    ntpBlockConfigs set "Lambda_c+    Tag   5 40"
    ntpBlockConfigs set "Lambda0      Lam0  2 20"
    ntpBlockConfigs set "Sigma0       Sig0  2 20"
    ntpBlockConfigs set "eta'   EtaP  3 20"
    ntpBlockConfigs set "rho+   RhoC  2 20"
    ntpBlockConfigs set "rho0   Rho0  2 20"
    ntpBlockConfigs set "phi    Phi   2 20"
    #only eta->gg should be used but there is a bug and eta->3pi was included
    ntpBlockConfigs set "eta    Eta   3 20"
    #for Lambda_c modes the Proton an Kaon are combined into one dummy PK candidate
    ntpBlockConfigs set "Xi_b0  PK    2 20"
    ntpBlockConfigs set "Xi_b-  PK    2 20"
    ntpBlockConfigs set "K_S0   Ks    2 40" 
    ntpBlockConfigs set "K*0    KSt   2 20" 
    ntpBlockConfigs set "pi0    Pi0   2 100" 
    ntpBlockConfigs set "p+     P     0 20"
    ntpBlockConfigs set "K+     K     0 40"  
    ntpBlockConfigs set "pi+    Pi    0 220" 
    ntpBlockConfigs set "gamma  Gam   0 240" 
                                                                                                                                       
    ntpBlockContents set "Evt:  Mass CMMomentum Vertex VtxChi2 nDaughters UsrData(DRecoUsrBlock)"
    ntpBlockContents set "DsSt: Mass Vertex CMMomentum"
    ntpBlockContents set "Ds:   MCIdx Mass CMMomentum nDaughters"
    ntpBlockContents set "Frag: MCIdx Mass CMMomentum Vertex VtxChi2 nDaughters" 
    ntpBlockContents set "Tag:  MCIdx Mass Vertex VtxChi2 FlightBS CMMomentum nDaughters"
    ntpBlockContents set "Ks:   MCIdx Mass Vertex FlightBS CMMomentum"
    ntpBlockContents set "KSt:  MCIdx Mass Vertex CMMomentum"
    ntpBlockContents set "K:    MCIdx CMMomentum"
    ntpBlockContents set "Pi:   MCIdx CMMomentum"
    ntpBlockContents set "Pi0:  MCIdx Mass Vertex CMMomentum"
    ntpBlockContents set "Gam:  MCIdx CMMomentum"
    ntpBlockContents set "Lam0: MCIdx Mass Vertex CMMomentum"
    ntpBlockContents set "Sig0: MCIdx Mass Vertex CMMomentum"
    ntpBlockContents set "P:    MCIdx Mass CMMomentum"
    ntpBlockContents set "EtaP: MCIdx Mass Vertex CMMomentum nDaughters"
    ntpBlockContents set "RhoC: MCIdx Mass Vertex CMMomentum nDaughters"
    ntpBlockContents set "Rho0: MCIdx Mass Vertex CMMomentum nDaughters"
    ntpBlockContents set "Phi:  MCIdx Mass Vertex CMMomentum"
    ntpBlockContents set "Eta:  MCIdx Mass Vertex CMMomentum"
    ntpBlockContents set "PK:   Mass CMMomentum"   
    
    checkCloneBlocks set "K Pi P Gam"

    ntpBlockToTrk set "K Pi P"
    trkExtraContents set BitMap:piSelectorsMap,KSelectorsMap,pSelectorsMap

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
FwkCfgVar NEvent 5000

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







