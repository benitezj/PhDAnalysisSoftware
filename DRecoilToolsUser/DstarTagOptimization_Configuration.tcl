#-------------------------------------------------- 
#Reconstruction of:
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
    printFreq set 100
}

#--------------------------------------------------
#Choose Root as output
#--------------------------------------------------
FwkCfgVar BetaMiniTuple "root"
FwkCfgVar histFileName "DsTolnu/SP1005Skim/TagOpt/Ntuples/Run3/Ntuple1.root" 

#--------------------------------------------------
#Set Number of events to be analyzed
#--------------------------------------------------
FwkCfgVar NEvent 100

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
source DsTolnu/SP1005Skim/TagOpt/NtuplesData/Run3/DataSet-1.tcl


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
#actions 
#--------------------------------------------------
sourceFoundFile BetaMiniSequences/BetaMiniActions.tcl
action disable HistoDirAction

#--------------------------------------------------
#Enable User defined Data
#--------------------------------------------------
sourceFoundFile UsrTools/UsrDataProcs.tcl
enableReadUsrData


##----------------
##create tag modes 
##-------------------------
sourceFoundFile DRecoilToolsUser/DReco_RefineChmLists.tcl


#--------------------------------------------------
#configure the module for mass window, vtx prob. and p* cuts.
#--------------------------------------------------
module talk DRecoTagInterface

      TagList1 set DRecoChmD0ToKPi
      TagListFit1 set true
      TagListMass1 set 1.86361  
      TagListMassCut1 set .1
      TagListPstarCut1 set 2.4

      TagList2 set DRecoChmD0ToKPiPi0
      TagListFit2 set true
      TagListMass2 set 1.8619  
      TagListMassCut2 set .1 
      TagListPstarCut2 set 2.4

      TagList3 set DRecoChmD0ToK3Pi
      TagListFit3 set true
      TagListMass3 set 1.8636  
      TagListMassCut3 set .1 
      TagListPstarCut3 set 2.4

      TagList4 set DRecoChmD0ToK3PiPi0Tight
      TagListFit4 set true
      TagListMass4 set 1.8627  
      TagListMassCut4 set .1
      TagListPstarCut4 set 3.0

      TagList5 set DRecoChmD0ToKShortPiPi
      TagListFit5 set true
      TagListMass5 set 1.8638  
      TagListMassCut5 set .1
      TagListPstarCut5 set 2.4

      TagList6 set DRecoChmD0ToKShortPiPiPi0
      TagListFit6 set true
      TagListMass6 set 1.8627  
      TagListMassCut6 set  .1
      TagListPstarCut6 set 2.8

      TagList7 set DRecoChmDPlusToKPiPi
      TagListFit7 set true
      TagListMass7 set 1.86850 
      TagListMassCut7 set .1
      TagListPstarCut7 set 2.4

      TagList8 set DRecoChmDPlusToKPiPiPi0
      TagListFit8 set true
      TagListMass8 set 1.86695  
      TagListMassCut8 set .1
      TagListPstarCut8 set 3.0

      TagList9 set DRecoChmDPlusToKShortPi
      TagListFit9 set true
      TagListMass9 set 1.86900 
      TagListMassCut9 set .1
      TagListPstarCut9 set 2.4

      TagList10 set DRecoChmDPlusToKShortPiPi0
      TagListFit10 set true
      TagListMass10 set 1.8675  
      TagListMassCut10 set .1
      TagListPstarCut10 set 2.8

      TagList11 set DRecoChmDPlusToKShort3Pi
      TagListFit11 set true
      TagListMass11 set 1.86864  
      TagListMassCut11 set .1
      TagListPstarCut11 set 2.7

exit
seq append ChmDrecoSequence DRecoTagInterface



##--------------------------------
##Cofigure Dstar tagging
##--------------------------------
module talk DRecoDstarTag
  PiList set GoodTracksVeryLoose

  Pi0List set pi0DefaultMass
  Pi0GamEnergyCut set .030

  GammaList set GoodPhotonLoose
  #disable the D*0 -> D0 gamma because it has too much background
  GamEnergyCut set .050

  D0PiDeltaMCut set .015
  D0Pi0DeltaMCut set .020
  D0GamDeltaMCut set .050
  DPlusPi0DeltaMCut set .020

exit
seq append ChmDrecoSequence DRecoDstarTag


sequence create DRecoSequence
sequence append DRecoSequence ChmDrecoSequence
sequence append DRecoSequence DRecoTagDump
sequence append DRecoSequence DRecoCleanUp

#--------------------------------------------------
#Configure NtupleMaker
#--------------------------------------------------
talkto BtuTupleMaker {
    listToDump set DRecoTagList
    ntupleName set "ntp1"

    writeEveryEvent set false
    fillMC set false
    checkClones set false 
    eventBlockContents set "EventID CMp4 BeamSpot PrimVtx"
    mcBlockContents  set "Mass CMMomentum Momentum Vertex"
    
    ntpBlockConfigs set "D*+    Tag   5 30" 
    ntpBlockConfigs set "D*0    Tag   5 30" 
    ntpBlockConfigs set "D+     Tag   5 30" 
    ntpBlockConfigs set "D0     Tag   5 30" 
    ntpBlockConfigs set "K+     K     0 100"  
    ntpBlockConfigs set "K_S0   Ks    2 100" 
    ntpBlockConfigs set "pi+    Pi    0 100" 
    ntpBlockConfigs set "pi0    Pi0   2 200" 
    ntpBlockConfigs set "gamma  Gam   0 100" 
 
    ntpBlockContents set "Tag:  Mass Vertex VtxChi2 FlightBS CMMomentum nDaughters"
    ntpBlockContents set "Ks:   Mass Vertex FlightBS CMMomentum"
    ntpBlockContents set "Pi0:  Mass Vertex CMMomentum"
    ntpBlockContents set "Pi:   CMMomentum"
    ntpBlockContents set "K:    CMMomentum"
    ntpBlockContents set "Gam:  CMMomentum Momentum"
    
}
sequence append DRecoSequence BtuTupleMaker

#--------------------------------------------------
#append analysis modules
#--------------------------------------------------

path append Everything DRecoSequence 



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






