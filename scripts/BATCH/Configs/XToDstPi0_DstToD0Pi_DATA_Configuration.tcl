#Reconstruction of:X->D*+ pi0
#           D*+->D0 Pi+
#                D0->K pi,K 3pi,Kpipi0,Kspipi
#Author: Jose Benitez
#--------------------------------------------------

#--------------------------------------------------
#some default stuff
#--------------------------------------------------
sourceFoundFile ErrLogger/ErrLog.tcl
sourceFoundFile FrameScripts/FwkCfgVar.tcl
sourceFoundFile FrameScripts/talkto.tcl
sourceFoundFile FrameScripts/setProduction.tcl

#--------------------------------------------------
#Disable the use of envvars
#--------------------------------------------------
set ProdTclOnly true                                      

#--------------------------------------------------
#set the error logging level to 'warning'
#--------------------------------------------------
ErrLoggingLevel warning

#--------------------------------------------------
#
#--------------------------------------------------
FwkCfgVar BetaMiniReadPersistence "Kan"    

#--------------------------------------------------
#set the print frequency of event counter
#--------------------------------------------------
talkto EvtCounter {
    printFreq set 1000
}

#--------------------------------------------------
#Choose Root as output
#--------------------------------------------------
FwkCfgVar BetaMiniTuple "root"                                    
FwkCfgVar histFileName "DstarCharged/DstarPi0/DATARun1Off/Ntuple1.root" 

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
#
#--------------------------------------------------
FwkCfgVar levelOfDetail "cache"  

#--------------------------------------------------
#Specify the input data
#--------------------------------------------------
source /u/eb/benitezj/ana42/workdir/DstarCharged/DstarPi0/DATARun1Off/DataSet-1.tcl 

#--------------------------------------------------
#DATA or MC
#--------------------------------------------------
FwkCfgVar ConfigPatch "Run2"
FwkCfgVar MCTruth false
    


#--------------------------------------------------                            
#create Everything path and add core sequences to it
#--------------------------------------------------                        
sourceFoundFile BetaMiniUser/btaMiniPhysics.tcl

#--------------------------------------------------
# 
#--------------------------------------------------
sourceFoundFile BetaMiniSequences/BetaMiniActions.tcl
action disable HistoDirAction

#--------------------------------------------------
#Enable User defined Data
#--------------------------------------------------
sourceFoundFile UsrTools/UsrDataProcs.tcl
enableReadUsrData




#--------------------------------------------------
#disable the other analysis modules
#--------------------------------------------------
module disable QaMiniBtaCandidates


#--------------------------------------------------
#configure D0 decay selection
#--------------------------------------------------
module talk D0ToKPi
   kaonList set KLHTight
   pionList set piLHTight
   D0List set MyD0FromKPiList
   massConstrain set false
   beamConstrain set false
   FitVtx set false
   VtxProbCut set 0
   D0MassCutLo set 1.76
   D0MassCutHi set 1.96
   D0MomentumCutLo set 1.5
exit
path append Everything D0ToKPi                                                                                                                                       
module talk D0ToK3Pi
   kaonList set KLHTight
   pion1List set piLHTight
   pion2List set GoodTracksLoose
   pion3List set GoodTracksLoose
   D0List set MyD0FromK3PiList
   massConstrain set false
   beamConstrain set false
   FitVtx set false
   VtxProbCut set 0
   D0MassCutLo set 1.76
   D0MassCutHi set 1.96
   D0MomentumCutLo set 1.5
exit
path append Everything D0ToK3Pi

##################
## Merge the 2 modes
##################
mod clone SmpMergerDefiner MyMergedD0List
catch { setProduction MyMergedD0List }
talkto MyMergedD0List {
  debug             set f
  verbose           set f 
  disableCloneCheck set true 

  inputListNames    set "MyD0FromKPiList"
  inputListNames    set "MyD0FromK3PiList"
}
path append Everything MyMergedD0List


#--------------------------------------------------
#configure Dstar decay selection
#--------------------------------------------------
module talk DstToD0Pi 
   slowPionList set GoodTracksVeryLoose
   D0CandList set MyMergedD0List
   DstarCandList set MyDstarCandList
   massConstrain set false
   beamConstrain set false
   FitVtx set false
   VtxProbCut set 0	
   deltaMassCutLo set .13
   deltaMassCutHi set .155
   DstarMomentumCutLo set 1.5
exit
path append Everything DstToD0Pi

#--------------------------------------------------
#configure X decay selection
#--------------------------------------------------
module talk XToDstPi0
    DstCandList set MyDstarCandList
    PionList set pi0VeryLoose
    XList set MyXCandList
    XPDTType set 0
    beamConstrain set true
    FitVtx set true
    VtxProbCut set .001
    deltaMassCutLo set 0.1
    deltaMassCutHi set 2.0
    XMomentumCutLo set 3.
    CosDstCut set .8
exit
path append Everything XToDstPi0


#--------------------------------------------------
#Configure NtupleMaker
#--------------------------------------------------
talkto BtuTupleMaker {
    listToDump set MyXCandList
    ntupleName set "ntp1"

    writeEveryEvent set false
    fillMC set $MCTruth 
    checkClones set false 

    eventBlockContents set "EventID CMp4 BeamSpot PrimVtx "
    mcBlockContents  set "Mass CMMomentum Momentum Vertex"
 
    ntpBlockConfigs set "B*+    X      2 30"
    ntpBlockConfigs set "D*+    Dst    2 30" 
    ntpBlockConfigs set "D0     D      4 30" 
    ntpBlockConfigs set "K_S0   Ks     2 30" 
    ntpBlockConfigs set "K+     K      0 30" 
    ntpBlockConfigs set "pi+    Pi     0 100" 
    ntpBlockConfigs set "pi0    Pi0    2 100"  
    ntpBlockConfigs set "gamma  Gam    0 100" 

    ntpBlockContents set "X:     MCIdx  Mass Vertex VtxChi2 Momentum CMMomentum" 
    ntpBlockContents set "Dst:   MCIdx  Mass Vertex Momentum CMMomentum"
    ntpBlockContents set "D:     MCIdx  Mass Vertex Momentum CMMomentum nDaughters"
    ntpBlockContents set "Ks:    MCIdx  Mass Vertex Momentum CMMomentum Flight FlightBS"
    ntpBlockContents set "Pi:    MCIdx  Momentum CMMomentum"
    ntpBlockContents set "K:     MCIdx  Momentum CMMomentum"
    ntpBlockContents set "Pi0:   MCIdx  Mass Vertex Momentum CMMomentum"
    ntpBlockContents set "Gam:   MCIdx  Momentum CMMomentum" 
    

}
path append Everything BtuTupleMaker

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
