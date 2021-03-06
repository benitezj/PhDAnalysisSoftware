#Reconstruction of:X->D* pi e
#           D*->D0 Pi
#                      D0->K 3pi,K pi,
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
FwkCfgVar histFileName "DstarCharged/DstarPi/DATARun1On/Ntuple1.root" 

#--------------------------------------------------
#Set Number of events to be analyzed
#--------------------------------------------------
FwkCfgVar NEvent 10000000


#--------------------------------------------------
#Set the first event to be analyzed
#--------------------------------------------------
talkto KanEventInput {
first set 1
}

#--------------------------------------------------
#should I use refit?
#--------------------------------------------------
FwkCfgVar levelOfDetail "cache"  

#--------------------------------------------------
#Specify the input data
#--------------------------------------------------
source /u/eb/benitezj/ana42/workdir/DstarCharged/DstarPi/DATARun1On/DataSet-1.tcl 

#--------------------------------------------------
#DATA or MC
#--------------------------------------------------
FwkCfgVar ConfigPatch "Run2"
FwkCfgVar MCTruth false
    
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
   D0MomentumCutLo set 0
exit

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
   D0MomentumCutLo set 0
exit

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

#--------------------------------------------------
#configure Dstar decay selection
#--------------------------------------------------
module talk DstToD0Pi 
   slowPionList set GoodTracksVeryLoose
   D0CandList set MyMergedD0List
   #D0CandList set MyD0FromKPiList   
   DstarCandList set MyDstarCandList
   massConstrain set false
   beamConstrain set false
   FitVtx set false
   VtxProbCut set 0	
   deltaMassCutLo set .13
   deltaMassCutHi set .155
   DstarMomentumCutLo set 1.5
exit
#--------------------------------------------------
#configure X decay selection
#--------------------------------------------------
module talk XToDstPiTrk
    DstCandList set MyDstarCandList
    Pion1List set piLHVeryTight

    #TrkPDTType set electron
    #TrkList set PidLHElectrons 
    #TrkPDTType set muon
    #TrkList set muLikeTight
    #TrkPDTType set pion
    #TrkList set piLHVeryTight
    TrkPDTType set kaon
    TrkList set KLHVeryTight
    #TrkPDTType set proton
    #TrkList set pLHVeryTight
 
    XList set MyXCandList   
    KeepWrongSign set true
    beamConstrain set true
    FitVtx set true
    VtxProbCut set .001
    deltaMassCutLo set 0.0
    deltaMassCutHi set 6.0
    XMomentumCutLo set 3.0
exit


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


#--------------------------------------------------
#append analysis modules
#--------------------------------------------------
path append Everything D0ToKPi
path append Everything D0ToK3Pi
path append Everything MyMergedD0List
path append Everything DstToD0Pi
path append Everything XToDstPiTrk
path append Everything BtuTupleMaker


#--------------------------------------------------
#disable the other analysis modules
#--------------------------------------------------
module disable QaMiniBtaCandidates
module disable XToDstPiPi
module disable XToDstPi
module disable Ds1pToDstKs 
module disable XToDstTrk


#--------------------------------------------------
#Configure NtupleMaker
#--------------------------------------------------
talkto BtuTupleMaker {
    listToDump set MyXCandList
    ntupleName set "ntp1"

writeEveryEvent set false
fillMC set false

    checkClones set false 

    eventBlockContents set "EventID CMp4 BeamSpot PrimVtx "  
 
    ntpBlockConfigs set "B*+    X      3 100"
    ntpBlockConfigs set "D*+    Dstar  2 100" 
    ntpBlockConfigs set "D0     D0     4 100" 
    ntpBlockConfigs set "pi+    Pi     0 100" 
    ntpBlockConfigs set "K+     K      0 100" 

    #ntpBlockConfigs set "e+     Trk    0 100" 
    #ntpBlockConfigs set "mu+    Trk    0 100" 
    #ntpBlockConfigs set "p+     Trk    0 100" 

    ntpBlockContents set "X : Mass Vertex VtxChi2 Momentum CMMomentum " 
    ntpBlockContents set "Dstar : Mass Vertex VtxChi2 Momentum CMMomentum"
    ntpBlockContents set "D0 :  Mass Vertex VtxChi2 Momentum CMMomentum"
    ntpBlockContents set "Pi:  Momentum Doca Poca CMMomentum "
    ntpBlockContents set "K:  Momentum Doca Poca CMMomentum "
    #ntpBlockContents set "Trk:  Momentum Doca Poca CMMomentum "
    
    #checkCloneBlocks set "K Pi Trk"
    checkCloneBlocks set "K Pi"
  
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
